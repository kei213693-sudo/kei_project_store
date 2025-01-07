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
#include "sysdump.h"

#include "hal_uart.h"
#include "hal_flash.h"
#include "bsp_protocol.h"
#include "app_task.h"
#include "app.h"
#include "sf.h"
#include "app_sec.h"


/*******************************内部函数的声明*******************************/
static void app_ble_sys_info_reset(void);

static int app_task_update_param(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int app_ble_mcu_rx_handle(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int app_ble_debug_log_handle(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);

/*******************************全局变量定义*******************************/
app_ble_sys_info_t g_stAppBleSysInfo = {0};

/*******************************内部变量定义*******************************/
KE_MSG_HANDLER_TAB(app_ble)
{
    {APP_BLE_OPT_GAP_RSSI,      (ke_msg_func_t)NULL},//获取rssi数值，然后传递给mcu
    {APP_BLE_OPT_UPDATA_PARAM,  (ke_msg_func_t)app_task_update_param},//slave主动更新连接参数
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

	//获取系统信息
	hal_flash_read(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleSysInfo,sizeof(app_ble_sys_info_t));
	if((0x0000 == g_stAppBleSysInfo.AdvInterval) || (0xffff == g_stAppBleSysInfo.AdvInterval))
	{//初始化状态重置系统参数
		app_ble_sys_info_reset();
	}

	//配置rf power，最大7dB
	rf_tx_power_set(false,RF_TX_POWER_MAX);

    // Create APP task
    u8Ret = ke_task_create(APP_BLE_TASK_ID, &g_stTaskDescAppBle);

	if(KE_TASK_OK != u8Ret)
	{
		i32Ret = pdFAIL;
	}
    //Initialize Task state
    ke_state_set(APP_BLE_TASK_ID, 1);
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
*函数功能描述 ：重置系统信息
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_sys_info_reset(void)
{
	uint8_t sn_temp[APP_BLE_SN_LEN + 1] = "000000001";
	memset(&g_stAppBleSysInfo,0x00,sizeof(app_ble_sys_info_t));

	memcpy(g_stAppBleSysInfo.sn,sn_temp,APP_BLE_SN_LEN);
	g_stAppBleSysInfo.pairNum = 0;
	g_stAppBleSysInfo.AdvInterval = APP_ADV_FAST_INTERVAL;//默认500ms
	hal_flash_erase(HAL_FLASH_USER_DATA_START,SF_SECTOR_SIZE);
	hal_flash_write(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleSysInfo,sizeof(app_ble_sys_info_t));
    log_debug("app_ble_sys_info_reset\n");
}

/**************************************************************
*函数功能描述 ：slave发起连接参数更新，在建立连接后调用
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static int app_task_update_param(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    log_debug("send con param updata req\n");
	struct gapc_conn_param st_conn_param = {80,80,0,300};//100ms,100ms,0潜伏时间，3s,写死

	appm_update_param(&st_conn_param);
		
    return (KE_MSG_CONSUMED);
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
		log_debug_array_ex("uart rx hex", pData, u16Len);
		
		if(0 == strncmp("reboot",(const char*)pData,6))
		{//强制重启
			pmu_force_reboot();
		}
		// else if(0 == strncmp("task",(const char*)pData,4))
		// {//task state测试，暂时看不出什么成效
		// 	extern ke_state_t appm_state[APP_IDX_MAX];//状态列表

		// 	uint8_t u8Opt = pData[4] - 0x30;
		// 	ke_state_set(APP_BLE_TASK_ID, u8Opt);
		// 	ke_state_set(TASK_APP, (u8Opt + 1));
		// 	log_debug("state:%d %d\n",ke_state_get(APP_BLE_TASK_ID),ke_state_get(TASK_APP));
		// 	log_debug_array_ex("appm_state hex", appm_state, APP_IDX_MAX);
		// 	log_debug_array_ex("app_ble_task_state hex", app_ble_task_state, APP_BLE_IDX_MAX);
		// }
		else if(0 == strncmp("sysdump",(const char*)pData,7))
		{//打印系统信息
			log_debug("Version data:%s\n",APP_VERSION_DATE);
			log_debug("Version time:%s\n",APP_VERSION_TIME);
			//0掉电，1深度睡眠唤醒，2ota升级，3主动重启，4看门狗重启，5低电压重启
			log_debug("reboot_reason:%d\n", pmu_reboot_reason());
			sysdump();
		}
		else if(0 == strncmp("coreuidget",(const char*)pData,10))
		{//获取芯片uid
			uint8_t uid[16];
			sfs_read_uid_ex(uid,sizeof(uid));
			log_debug_array_ex("core uid", uid,sizeof(uid));
		}
		else if(0 == strncmp("sysreset",(const char*)pData,8))
		{
			app_ble_sys_info_reset();
		}
		else if(0 == strncmp("sysget",(const char*)pData,6))
		{
			log_debug_array_ex("SN", g_stAppBleSysInfo.sn, APP_BLE_SN_LEN);
			log_debug("Pair num:%d\n",g_stAppBleSysInfo.pairNum);
			log_debug_array_ex("Admin Mac Addr", g_stAppBleSysInfo.administratorMAC, 6);
			log_debug("AdvInterval:%d\n",g_stAppBleSysInfo.AdvInterval);
		}
		else if(0 == strncmp("blesetmac",(const char*)pData,9))
		{//配置mac地址
			uint8_t ret = 0;
			bd_addr_t addr;
			memcpy((uint8_t *)&addr,&pData[9],sizeof(addr));
			log_debug_array_ex("set mac", addr.addr, sizeof(addr.addr));
			#if (NVDS_SUPPORT)
			ret = nvds_put(NVDS_TAG_BD_ADDRESS, sizeof(addr.addr), addr.addr);//对应通过nvds_get获取
			log_debug("nvds_put ret:%d",ret);
			#endif 
		}
		else if(0 == strncmp("bleinfoget",(const char*)pData,10))
		{//获取ble控制信息
			log_debug_array_ex("app_env", &app_env, sizeof(app_env));
			log_debug_array_ex("app_sec_env", &app_sec_env, sizeof(app_sec_env));
		}
		else if(0 == strncmp("bledis",(const char*)pData,6))
		{//断开连接
			appm_disconnect(0);
		}
		else if(0 == strncmp("advOpt",(const char*)pData,6))
		{//停止广播
			uint8_t u8Opt = pData[6] - 0x30;
			if(0 == u8Opt)
			{//0代表暂停此次广播
				appm_adv_stop(0);
			}
			else if(1 == u8Opt)
			{//1代表删除此次广播
				appm_adv_stop(1);
			}
			else if(2 == u8Opt)
			{//启动广播
				appm_adv_start();
			}
			else if(3 == u8Opt)
			{//广播状态获取，参考枚举app_adv_state
				log_debug("adv state:%d\n",appm_adv_get_state());
			}
		}
		else if(0 == strncmp("rssi",(const char*)pData,4))
		{//获取最新的rssi值
			appm_get_rssi();
		}
		else if(0 == strncmp("DelBond",(const char*)pData,7))
		{//删除绑定信息
			uint8_t u8Opt = pData[7] - 0x30;
			if(0 == u8Opt)
			{//删除所有
				app_sec_remove_bond();
			}
			else
			{//删掉除管理员之外的全部
				
			}
		}
		else if(0 == strncmp("SendBond",(const char*)pData,8))
		{
			uint8_t u8Opt = pData[8] - 0x30;
			if(0 == u8Opt)
			{
				app_sec_send_security_req(0);//slave主动发送安全请求
				log_debug("send security req\n");
			}
			else if(1 == u8Opt)
			{
				app_sec_bond_req(0);//slave主动发送配对绑定请求
				log_debug("send bond req\n");
			}
		}
		else
		{
		}
		
		//清空接收缓存
		hal_uart_log_rx_buf_reset();
	}

    return KE_MSG_CONSUMED;
}

