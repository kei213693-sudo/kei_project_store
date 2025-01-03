/*
 * app_ble.c
 *
 *  Created on: 2023年6月19日
 *      Author: F704011834_
 *	  Abstract: ble模块，实现ble应用逻辑
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "app_ble.h"
#include "bsp_ble.h"

#include "rwip_config.h"
#include "co_utils.h"
#include "co_debug.h"
#include "ke_task.h"
#include "ke_msg.h"
#include "ke_timer.h"

#include "hal_uart.h"
#include "hal_flash.h"
#include "bsp_protocol.h"
#include "app_task.h"


/*******************************内部函数的声明*******************************/
static int app_ble_mcu_rx_handle(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int app_ble_debug_log_handle(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);

/*******************************全局变量定义*******************************/

/*******************************内部变量定义*******************************/
KE_MSG_HANDLER_TAB(app_ble)
{
    {APP_BLE_OPT_GAP_RSSI,      (ke_msg_func_t)NULL},//获取rssi数值，然后传递给mcu
    {APP_BLE_OPT_MCU_RX,        (ke_msg_func_t)app_ble_mcu_rx_handle},
    {APP_BLE_OPT_DEBUGLOG,      (ke_msg_func_t)app_ble_debug_log_handle}
};

/**< Defines the place holder for the states of all the task instances. */
ke_state_t app_ble_task_state[APP_BLE_IDX_MAX];//任务状态怎么用？

/// Application task descriptor
static const struct ke_task_desc g_stTaskDescAppBle = {app_ble_msg_handler_tab, app_ble_task_state, APP_BLE_IDX_MAX, ARRAY_LEN(app_ble_msg_handler_tab)};

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：创建ble任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_ble_init(void)
{
    int32_t i32Ret = pdPASS;
    uint8_t u8Ret = 0;
    // Create APP task
    u8Ret = ke_task_create(APP_BLE_TASK_ID, &g_stTaskDescAppBle);

	if(0 != u8Ret)
	{
		i32Ret = pdFAIL;
	}
    //Initialize Task state
    ke_state_set(APP_BLE_TASK_ID, 4);
    return i32Ret;
}

/**************************************************************
*函数功能描述 ：创建ble任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_ble_deinit(void)
{
    int32_t i32Ret = pdPASS;
    // Create APP task
    ke_task_delete(APP_BLE_TASK_ID);
    return i32Ret;
}

/**************************************************************
*函数功能描述 ：处理mcu通过uart1发送的数据
*函数参数 ：msgid，信息id，参考APP_BLE_OPT_E
            param，要传入的参数，可以自定义数据类型
            dest_id，操作目标源
            src_id，操作来源
*函数返回值 ：
*备注：不单独创建log task，就放在app ble task中处理
**************************************************************/
static int app_ble_mcu_rx_handle(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    log_debug("app_ble_mcu_rx_handle:%d %d %d\n",msgid,dest_id,src_id);
    return KE_MSG_CONSUMED;
}

/**************************************************************
*函数功能描述 ：处理上位机通过uart0发送的调试指令
*函数参数 ：msgid，信息id，参考APP_BLE_OPT_E
            param，要传入的参数，可以自定义数据类型
            dest_id，操作目标源
            src_id，操作来源
*函数返回值 ：
*备注：不单独创建log task，就放在app ble task中处理
**************************************************************/
static int app_ble_debug_log_handle(ke_msg_id_t const msgid,
                                        void const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    //log_debug("app_ble_debug_log_handle:%d %d %d\n",msgid,dest_id,src_id);

    uint8_t  *pData = hal_uart_log_rx_buf_addr_get();
	uint16_t u16Len = hal_uart_log_rx_buf_len_get();

	if((NULL != pData) && (0 != u16Len))
	{
		log_debug("uart rx string:%s\n",pData);
		log_debug("uart rx hex[%d]",u16Len);
		log_debug_array_ex("", pData, u16Len);
		
		if(0 == strncmp("LED",(const char*)pData,3))
		{//
			uint8_t u8Opt = pData[3] - 0x30;
			log_debug("LED opt:%d\n",u8Opt);
		}
		else if(0 == strncmp("flashtest",(const char*)pData,9))
		{//flash测试
			uint8_t u8Opt = pData[9] - 0x30;
			uint8_t data[128] = {0};
			uint8_t data2[128] = {0};
			uint8_t i = 0;
			for(i = u8Opt;i<(128 + u8Opt);i++)
			{
				data[i-u8Opt] = i;
			}
			
			hal_flash_erase(HAL_FLASH_USER_DATA_START,(HAL_FLASH_USER_DATA_END - HAL_FLASH_USER_DATA_START));//先擦除
			hal_flash_write(HAL_FLASH_USER_DATA_START,data,128);//先写入
			hal_flash_read(HAL_FLASH_USER_DATA_START,data2,128);//再读取
			log_debug_array_ex("flash data",data2,128);//打印待写入的数据
		}
		else if(0 == strncmp("mcurx",(const char*)pData,5))
		{
			ke_timer_set(APP_BLE_OPT_MCU_RX, APP_BLE_TASK_ID, 0);//让log task去处理uart数据
		}
		else if(0 == strncmp("state",(const char*)pData,5))
		{
			extern ke_state_t appm_state[APP_IDX_MAX];//状态列表

			uint8_t u8Opt = pData[5] - 0x30;
			ke_state_set(APP_BLE_TASK_ID, u8Opt);
			ke_state_set(TASK_APP, (u8Opt + 1));
			log_debug("state:%d %d\n",ke_state_get(APP_BLE_TASK_ID),ke_state_get(TASK_APP));
			log_debug_array_ex("appm_state hex", appm_state, APP_IDX_MAX);
			log_debug_array_ex("app_ble_task_state hex", app_ble_task_state, APP_BLE_IDX_MAX);

		}
		else
		{
		}
		
		//清空接收缓存
		hal_uart_log_rx_buf_reset();
	}

    return KE_MSG_CONSUMED;
}

