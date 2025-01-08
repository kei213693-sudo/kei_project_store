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
#include "md5.h"
#include "nvds.h"

#include "hal_uart.h"
#include "hal_flash.h"
#include "app_task.h"
#include "app.h"
#include "sf.h"
#include "app_sec.h"
#include "app_adv.h"


/*******************************内部函数的声明*******************************/
static void app_ble_sys_info_reset(void);
static void app_ble_pair_check_timer_handler(co_timer_t *timer, void *param);
static void app_ble_rssi_get_timer_handler(co_timer_t *timer, void *param);

static int app_ble_rssi(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int app_ble_update_param(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int app_ble_processRxRec(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int app_ble_processTxRec(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);
static int app_ble_debug_log_handle(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id);

static void app_ble_cmdBleSnSetRsp(void *pData);
static void app_ble_cmdBleSnSetReq(void *pData);
static void app_ble_cmdBleBondAddRsp(void *pData);
static void app_ble_cmdBleBondAddReq(void *pData);
static void app_ble_cmdBleBondDelRsp(void *pData);
static void app_ble_cmdBleBondDelReq(void *pData);
static void app_ble_cmdBleParamSetRsp(void *pData);
static void app_ble_cmdBleParamSetReq(void *pData);
static void app_ble_cmdBleRssiSendReq(void *pData);

/*******************************全局变量定义*******************************/
app_ble_ctl_t g_stAppBleCtl = {0};
co_timer_t g_pair_check_timer;
co_timer_t g_rssi_get_timer;
co_timer_t g_adv_upgrade_timer;

/*******************************内部变量定义*******************************/
//任务参数相关
KE_MSG_HANDLER_TAB(app_ble)
{
    {APP_BLE_OPT_GAP_RSSI,      (ke_msg_func_t)app_ble_rssi},//rssi操作
    {APP_BLE_OPT_UPDATA_PARAM,  (ke_msg_func_t)app_ble_update_param},//slave主动更新连接参数
    {APP_BLE_OPT_DEBUGLOG,      (ke_msg_func_t)app_ble_debug_log_handle},
	{APP_BLE_OPT_MCU_RX,        (ke_msg_func_t)app_ble_processRxRec},

	//以下事件都是APP_BLE_OPT_MCU_TX的细分，统一通过app_ble_processTxRec处理
	{APP_BLE_OPT_SN_SET,        (ke_msg_func_t)app_ble_processTxRec},
	{APP_BLE_OPT_BOND_ADD,      (ke_msg_func_t)app_ble_processTxRec},
	{APP_BLE_OPT_BOND_DEL,      (ke_msg_func_t)app_ble_processTxRec},
	{APP_BLE_OPT_PARAM_SET,      (ke_msg_func_t)app_ble_processTxRec},
	{APP_BLE_OPT_RSSI_SEND,      (ke_msg_func_t)app_ble_processTxRec}
};
/**< Defines the place holder for the states of all the task instances. */
static ke_state_t app_ble_task_state[APP_BLE_IDX_MAX];//任务状态怎么用？
/// Application task descriptor
static const struct ke_task_desc g_stTaskDescAppBle = {app_ble_msg_handler_tab, app_ble_task_state, APP_BLE_IDX_MAX, ARRAY_LEN(app_ble_msg_handler_tab)};

//自定义通信协议相关
static const BSP_PROTOCOL_API_T* g_pBspProtocolApi = &g_BspProtocolApi;
static protocol_ctl_t g_stBleProCtl;

static const APP_BLE_CMD_FUC_TAB_T g_stBleCmdFucTab[] =
{//自定义指令处理函数列表
    {APP_BLE_OPT_SN_SET
	,cmdBleSnSetRsp
    ,app_ble_cmdBleSnSetRsp
    ,cmdBleSnSetReq
    ,app_ble_cmdBleSnSetReq},

    {APP_BLE_OPT_BOND_ADD
	,cmdBleBondAddRsp
    ,app_ble_cmdBleBondAddRsp
    ,cmdBleBondAddReq
    ,app_ble_cmdBleBondAddReq},

    {APP_BLE_OPT_BOND_DEL
	,cmdBleBondDelRsp
    ,app_ble_cmdBleBondDelRsp
    ,cmdBleBondDelReq
    ,app_ble_cmdBleBondDelReq},

    {APP_BLE_OPT_PARAM_SET
	,cmdBleParamSetRsp
    ,app_ble_cmdBleParamSetRsp
    ,cmdBleParamSetReq
    ,app_ble_cmdBleParamSetReq},

	{APP_BLE_OPT_RSSI_SEND
	,cmdBleRssiSendReq
    ,app_ble_cmdBleRssiSendReq
    ,cmdBleRssiSendRsp
    ,NULL}
};

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
	hal_flash_read(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));
	if((0x0000 == g_stAppBleCtl.stAppBleSysInfo.AdvInterval) || (0xffff == g_stAppBleCtl.stAppBleSysInfo.AdvInterval))
	{//初始化状态重置系统参数
		app_ble_sys_info_reset();
	}

	//初始化自定义协议控制结构体
	g_pBspProtocolApi->protocol_init(&g_stBleProCtl);

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
*函数功能描述 ：存储管理员mac addr
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void app_ble_pair_succ_opt(uint8_t *pMacAddr)
{
	if(NULL != pMacAddr)
	{
		hal_flash_read(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));

		if(0 == g_stAppBleCtl.stAppBleSysInfo.pairNum)
		{
			memcpy(g_stAppBleCtl.stAppBleSysInfo.administratorMAC,pMacAddr,GAP_BD_ADDR_LEN);
			log_debug_array_ex("admin addr store",g_stAppBleCtl.stAppBleSysInfo.administratorMAC,GAP_BD_ADDR_LEN);
		}

		g_stAppBleCtl.stAppBleSysInfo.pairNum++;

		hal_flash_erase(HAL_FLASH_USER_DATA_START,SF_SECTOR_SIZE);
		hal_flash_write(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));
		log_debug("now pairNum:%d\n",g_stAppBleCtl.stAppBleSysInfo.pairNum);
	}
}

/**************************************************************
*函数功能描述 ：判定当前建立连接的手机是否是管理员用户
*函数参数 ：
*函数返回值 ：
*备注：1是管理员用户，2是普通员用户
**************************************************************/
uint8_t app_ble_admin_check(uint8_t *pMacAddr)
{
	g_stAppBleCtl.u8AdminFlag = 0;

	if(NULL != pMacAddr)
	{
		if(0 == strncmp((const char*)pMacAddr,(const char*)g_stAppBleCtl.stAppBleSysInfo.administratorMAC,GAP_BD_ADDR_LEN))
		{
			g_stAppBleCtl.u8AdminFlag = 1;//管理员用户
			log_debug("Administrator user!\n");
		}
		else
		{
			g_stAppBleCtl.u8AdminFlag = 2;//普通用户
			log_debug("Normal user.\n");
		}
	}
	return g_stAppBleCtl.u8AdminFlag;
}

/**************************************************************
*函数功能描述 ：判定当前建立连接的手机是否是管理员用户
*函数参数 ：opt，0停止定时获取（断开连接时调用
				1启动定时获取（快速回连时调用
				2接收最新数值并存储起来，过滤完成后传递给mcu（获取成功时调用
*函数返回值 ：
*备注：
**************************************************************/
void app_ble_rssi_opt(uint8_t opt,int8_t i8Rssi)
{
	if(0 == opt)
	{
		co_timer_del(&g_rssi_get_timer);
	}
	else if(1 == opt)
	{
		log_debug("The startup period(%d ms) obtains the rssi\n",g_stAppBleCtl.u32RssiGetTime);
		g_stAppBleCtl.u8RssiLen = 0;
		memset(g_stAppBleCtl.g_i8RssiBuf,0x00,APP_BLE_RSSI_NUM);

		co_timer_set(&g_rssi_get_timer, g_stAppBleCtl.u32RssiGetTime, TIMER_REPEAT
			, app_ble_rssi_get_timer_handler, NULL);
	}
	else if(2 == opt)
	{
		//log_debug("get rssi:%d dB\n",i8Rssi);//获取RSSI值
		
		g_stAppBleCtl.g_i8RssiBuf[g_stAppBleCtl.u8RssiLen] = i8Rssi;
		g_stAppBleCtl.u8RssiLen++;

		if(g_stAppBleCtl.u8RssiLen >= APP_BLE_RSSI_NUM)
		{
			int8_t i8RssiAvg = 0xff;
			//TODO 待添加rssi过滤api
			// log_debug("rssi data[%d]:",APP_BLE_RSSI_NUM);
			// for(uint8_t i = 0;i < APP_BLE_RSSI_NUM;i++)
			// {
			// 	log_debug(" %d",g_stAppBleCtl.g_i8RssiBuf[i]);
			// }
			// log_debug("\r\n");

			//计算完成后清空buf
			g_stAppBleCtl.u8RssiLen = 0;
			memset(g_stAppBleCtl.g_i8RssiBuf,0x00,APP_BLE_RSSI_NUM);

			typedef struct Param_t
			{
				int8_t i8Rssi;
				uint8_t res[3];
			}Param_t;

			#if (APP_BLE_MSG_SWITCH)
			//直接调用返回响应的api
			Param_t stData;
			stData.i8Rssi = i8RssiAvg;
			app_ble_cmdBleRssiSendReq((void *)&stData);
			#else
			//转一道，统一交给app_ble_processTxRec（）去调用返回响应的api
			Param_t *pStData = KE_MSG_ALLOC(APP_BLE_OPT_RSSI_SEND,
											APP_BLE_TASK_ID, TASK_APP,
											Param_t);
			
			pStData->i8Rssi = i8RssiAvg;
			ke_msg_send(pStData);
			#endif
		}
	}
	else
	{
		appm_get_rssi();
	}
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
	memset(&g_stAppBleCtl.stAppBleSysInfo,0x00,sizeof(app_ble_sys_info_t));

	memcpy(g_stAppBleCtl.stAppBleSysInfo.sn,sn_temp,APP_BLE_SN_LEN);
	g_stAppBleCtl.stAppBleSysInfo.pairNum = 0;
	g_stAppBleCtl.stAppBleSysInfo.AdvInterval = APP_ADV_FAST_INTERVAL;//默认500ms
	hal_flash_erase(HAL_FLASH_USER_DATA_START,SF_SECTOR_SIZE);
	hal_flash_write(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));
    log_debug("app_ble_sys_info_reset\n");
}

/**************************************************************
*函数功能描述 ：超时未完成配对，退出配对模式
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_pair_check_timer_handler(co_timer_t *timer, void *param)
{
    g_stAppBleCtl.u8PairFlag = 0;//禁止配对
	appm_disconnect(0);//并断开连接
	log_debug("pair bond check timerout,cancel pair\n");
}

/**************************************************************
*函数功能描述 ：定时获取rssi数值，然后传递给mcu
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_rssi_get_timer_handler(co_timer_t *timer, void *param)
{
	appm_get_rssi();
}

/**************************************************************
*函数功能描述 ：定时获取rssi数值，然后传递给mcu
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_adv_upgarde_timer_handler(co_timer_t *timer, void *param)
{
	appm_advertising_init();
	appm_adv_start();//启动广播
}

/**************************************************************
*函数功能描述 ：slave发起连接参数更新，在建立连接后调用
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static int app_ble_rssi(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
	typedef struct
	{
		uint8_t u8Opt;//1启动周期获取rssi定时任务
					//2删除周期任务
	}Ret_t;
	Ret_t *pStRet = NULL;
	
	if(NULL != param)
	{
		pStRet = (Ret_t *)param;
		if(1 == pStRet->u8Opt)
		{
			co_timer_set(&g_rssi_get_timer, g_stAppBleCtl.u32RssiGetTime, TIMER_REPEAT
				, app_ble_rssi_get_timer_handler, NULL);
		}
		else
		{
			co_timer_del(&g_rssi_get_timer);
		}
	}
	
    return (KE_MSG_CONSUMED);
}

/**************************************************************
*函数功能描述 ：slave发起连接参数更新，在建立连接后调用
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static int app_ble_update_param(ke_msg_id_t const msgid,void const *param,ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    log_debug("send con param updata req\n");
	struct gapc_conn_param st_conn_param = {(100)*5/6,(100)*5/6,0,(3000)/10};//100ms,100ms,0潜伏时间，3s,写死

	appm_update_param(&st_conn_param);
	
    return (KE_MSG_CONSUMED);
}

/**************************************************************
*函数功能描述 ：ble rx自定义事件处理线程
*函数参数 ：msgid，信息id，参考APP_BLE_OPT_E
            param，要传入的参数，可以自定义数据类型
            dest_id，操作目标源
            src_id，操作来源
*函数返回值 ：
*备注：处理mcu通过uart1发送的数据
**************************************************************/
static int app_ble_processRxRec(ke_msg_id_t const msgid,
								void const *param,
								ke_task_id_t const dest_id,
								ke_task_id_t const src_id)
{
	uint16_t u16Cmd = 0;
	uint8_t  *pData = hal_uart_fm33lg_rx_buf_addr_get();
	uint16_t u16Len = hal_uart_fm33lg_rx_buf_len_get();

	if((NULL != pData) && (0 != u16Len))
	{
		memset(g_stBleProCtl.rxBuf,0x00,PROTOCOL_BUFF_SIZE);//先清空rxBuffer
		memcpy(g_stBleProCtl.rxBuf,pData,u16Len);
		g_stBleProCtl.rxLen = u16Len;
		log_debug_array_ex("mcu rx hex",g_stBleProCtl.rxBuf,g_stBleProCtl.rxLen);

		u16Cmd = protocol_parse(&g_stBleProCtl,g_stBleProCtl.rxBuf,g_stBleProCtl.rxLen);
		log_debug("mcu rx cmd[%02x:%02x]\n",(uint8_t)u16Cmd,(uint8_t)(u16Cmd>>8));

		for(uint8_t i = 0;i < (sizeof(g_stBleCmdFucTab)/sizeof(g_stBleCmdFucTab[0]));i++)
		{
			if(u16Cmd == g_stBleCmdFucTab[i].enCmdRx)
			{//判定cmd指令码
				log_debug("App ble rx opt:%d\n",g_stBleCmdFucTab[i].enOpt);
				
				if(NULL != g_stBleCmdFucTab[i].appBleFucRx)
				{
					(*g_stBleCmdFucTab[i].appBleFucRx)(g_stBleProCtl.rx.buf);
				}
			}
		}
	}
	return (KE_MSG_CONSUMED);
}

/**************************************************************
*函数功能描述 ：ble 自定义协议tx时间
*函数参数 ：msgid，信息id，参考APP_BLE_OPT_E
            param，要传入的参数，可以自定义数据类型
            dest_id，操作目标源
            src_id，操作来源
*函数返回值 ：
*备注：通过uart1发送数据给mcu
**************************************************************/
static int app_ble_processTxRec(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	log_debug("App ble tx opt:%d\n",msgid);

	for(uint8_t i = 0;i < (sizeof(g_stBleCmdFucTab)/sizeof(g_stBleCmdFucTab[0]));i++)
    {
        if(msgid == g_stBleCmdFucTab[i].enOpt)
        {//判断opt事件类型
            if(NULL != g_stBleCmdFucTab[i].appBleFucTx)
            {
                (*g_stBleCmdFucTab[i].appBleFucTx)((void *)param);
            }
            break;
        }
    }
	return (KE_MSG_CONSUMED);
}

/**************************************************************
*函数功能描述 ：返回sn码设置结果
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleSnSetRsp(void *pData)
{
	typedef struct
	{
		uint8_t err;
	}Ret_t;
	Ret_t *pStRet = (Ret_t *)pData;

	g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
		,cmdBleSnSetRsp,0,NULL);
	if(NULL != pData)
	{
		g_stBleProCtl.tx.head->err = pStRet->err;
		//g_stBleProCtl.tx.head->err = (uint8_t)(*((uint8_t*)pData));
	}
	g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);
	
	hal_uart_fm33lg_tx_buf_set(g_stBleProCtl.txBuf,g_stBleProCtl.txLen);
}

/**************************************************************
*函数功能描述 ：处理sn码设置指令
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleSnSetReq(void *pData)
{
	typedef struct
	{
		uint8_t err;
	}Ret_t;
	Ret_t stRet;
	stRet.err = 0;

	typedef struct
	{
		uint8_t u8SN[APP_BLE_SN_LEN];
		uint8_t res[16 - APP_BLE_SN_LEN];
	}Param_t;
	Param_t *pStData = (Param_t *)pData;

	//重新读一次flash
	hal_flash_read(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));

	memcpy(g_stAppBleCtl.stAppBleSysInfo.sn,pStData->u8SN,APP_BLE_SN_LEN);//修改sn
	g_stAppBleCtl.stAppBleSysInfo.AdvInterval = APP_ADV_FAST_INTERVAL;//广播间隔恢复默认数值500ms

	hal_flash_erase(HAL_FLASH_USER_DATA_START,SF_SECTOR_SIZE);
	hal_flash_write(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));
	
	log_debug("sn set:%s\n",pStData->u8SN);

	#if (APP_BLE_MSG_SWITCH)
	//直接调用返回响应的api
	app_ble_cmdBleSnSetRsp((void *)&stRet);
	#else
	//转一道，统一交给app_ble_processTxRec（）去调用返回响应的api
	Ret_t *pStRet = KE_MSG_ALLOC(APP_BLE_OPT_SN_SET,APP_BLE_TASK_ID, APP_BLE_TASK_ID,Ret_t);
    pStRet->err = stRet.err;
    ke_msg_send(pStRet);
	#endif
}

/**************************************************************
*函数功能描述 ：返回启动配对指令响应
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleBondAddRsp(void *pData)
{
	typedef struct
	{
    	uint32_t u32Pincode;//配对码
	}Param_t;
	Param_t *pStData;
	
	g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
		,cmdBleBondAddRsp,sizeof(Param_t),(void*)&pStData);
	if((pStData != NULL) && (NULL != pData))
	{
		memcpy(pStData,(const void*)pData,sizeof(Param_t));
		// pStData->u32Pincode = *((uint32_t*)pData);
	}
	g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);
	
	hal_uart_fm33lg_tx_buf_set(g_stBleProCtl.txBuf,g_stBleProCtl.txLen);
}

/**************************************************************
*函数功能描述 ：处理启动配对指令
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleBondAddReq(void *pData)
{
	uint8_t md5_out[16];

	typedef struct
	{
    	uint32_t u32Pincode;//配对码
	}Ret_t;
	Ret_t stRet;
	stRet.u32Pincode = 0;

	if(g_stAppBleCtl.stAppBleSysInfo.pairNum < APP_BLE_PAIR_NUM_MAX)
	{
		g_stAppBleCtl.u8PairFlag = 1;//允许配对
		if(app_sec_get_bond_status() == 0)
		{//一个手机都没绑定，根据sn码计算得到配对码
			//通过md5算法加密sn码得到16bytes的输出结果
			md5((char *)g_stAppBleCtl.stAppBleSysInfo.sn, md5_out);
			log_debug_array_ex("md5_out", md5_out, 16);

			//一次将md5_out的bytes0~5取余数转换为0~9的数值，然后赋给pincode的1~6的位置，最终生成一个6位长度的数字
			g_stAppBleCtl.u32Pincode = ((uint32_t)(md5_out[0] % 10) * 100000) + ((uint32_t)(md5_out[1] % 10) * 10000)
						+ ((uint32_t)(md5_out[2] % 10) * 1000) + ((uint32_t)(md5_out[3] % 10) * 100)
						+ ((uint32_t)(md5_out[4] % 10) * 10) + ((uint32_t)(md5_out[5] % 10) * 1);
		}
		else
		{//随机生成
			g_stAppBleCtl.u32Pincode = (100000 + (co_rand_word()%900000));
		}
		
		//定时60s检查配对是否成功
		co_timer_set(&g_pair_check_timer, APP_BLE_PAIR_TIMEROUT, TIMER_ONE_SHOT
					, app_ble_pair_check_timer_handler, NULL);
		log_debug("Generate pincode:%d\n",g_stAppBleCtl.u32Pincode);
	}
	else
	{
		g_stAppBleCtl.u32Pincode = 0xffffffff;
		log_debug("pair num is full:%d\n",g_stAppBleCtl.stAppBleSysInfo.pairNum);
	}


	#if (APP_BLE_MSG_SWITCH)
	//直接调用返回响应的api
	stRet.u32Pincode = g_stAppBleCtl.u32Pincode;
	app_ble_cmdBleBondAddRsp((void *)&stRet);
	#else
	//转一道，统一交给app_ble_processTxRec（）去调用返回响应的api
	Ret_t *pStRet = KE_MSG_ALLOC(APP_BLE_OPT_BOND_ADD,APP_BLE_TASK_ID, APP_BLE_TASK_ID,Ret_t);
    pStRet->u32Pincode = g_stAppBleCtl.u32Pincode;
    ke_msg_send(pStRet);
	#endif
}

/**************************************************************
*函数功能描述 ：返回删除配对指令响应
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleBondDelRsp(void *pData)
{
	typedef struct
	{
		uint8_t err;
	}Ret_t;
	Ret_t *pStRet = (Ret_t *)pData;

	g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
		,cmdBleBondDelRsp,0,NULL);
	if(NULL != pData)
	{
		g_stBleProCtl.tx.head->err = pStRet->err;
		// g_stBleProCtl.tx.head->err = (uint8_t)(*((uint8_t*)pData));
	}
	g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);
	
	hal_uart_fm33lg_tx_buf_set(g_stBleProCtl.txBuf,g_stBleProCtl.txLen);
}

/**************************************************************
*函数功能描述 ：处理删除配对指令
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleBondDelReq(void *pData)
{
	typedef struct
	{
		uint8_t err;
	}Ret_t;
	Ret_t stRet;
	stRet.err = 0;

	typedef struct
	{
		uint8_t u8Opt;//0x00：删除除了管理员之外所有设备；
						//0x01：删除所有设备；
		uint8_t res[3];
	}Param_t;
	Param_t *pStData = (Param_t *)pData;

	//重新读一次flash
	hal_flash_read(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));

	if(0 == pStData->u8Opt)
	{
		app_sec_remove_bond_not_admin();
		g_stAppBleCtl.stAppBleSysInfo.pairNum = 1;
		log_debug("Delete users other than the administrator!\n");
	}
	else
	{
		app_sec_remove_bond();
		g_stAppBleCtl.stAppBleSysInfo.pairNum = 0;
		memset(g_stAppBleCtl.stAppBleSysInfo.administratorMAC,0x00,GAP_BD_ADDR_LEN);
		log_debug("Delete all users!\n");
	}

	//再修改参数，擦除、写入
	hal_flash_erase(HAL_FLASH_USER_DATA_START,SF_SECTOR_SIZE);
	hal_flash_write(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));

	#if (APP_BLE_MSG_SWITCH)
	//直接调用返回响应的api
	app_ble_cmdBleBondDelRsp((void *)&stRet);
	#else
	//转一道，统一交给app_ble_processTxRec（）去调用返回响应的api
	Ret_t *pStRet = KE_MSG_ALLOC(APP_BLE_OPT_BOND_DEL,APP_BLE_TASK_ID, APP_BLE_TASK_ID,Ret_t);
    pStRet->err = stRet.err;
    ke_msg_send(pStRet);
	#endif
}

/**************************************************************
*函数功能描述 ：返回参数配置指令响应
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleParamSetRsp(void *pData)
{
	typedef struct
	{
		uint8_t err;
	}Ret_t;
	Ret_t *pStRet = (Ret_t *)pData;

	g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
		,cmdBleParamSetRsp,0,NULL);
	if(NULL != pData)
	{
		g_stBleProCtl.tx.head->err = pStRet->err;
		// g_stBleProCtl.tx.head->err = (uint8_t)(*((uint8_t*)pData));
	}
	g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);
	
	hal_uart_fm33lg_tx_buf_set(g_stBleProCtl.txBuf,g_stBleProCtl.txLen);
}

/**************************************************************
*函数功能描述 ：处理参数配置指令
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleParamSetReq(void *pData)
{
	typedef struct
	{
		uint8_t err;
	}Ret_t;
	Ret_t stRet;
	stRet.err = 0;

	typedef struct
	{
		uint16_t u16AdvInterval;//广播间隔，ms
		uint8_t res[14];
	}Param_t;
	Param_t *pStData = (Param_t *)pData;

	//重新读一次flash
	hal_flash_read(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));
	//再修改参数，擦除、写入
	g_stAppBleCtl.stAppBleSysInfo.AdvInterval = pStData->u16AdvInterval*8/5;//0.625转换
	hal_flash_erase(HAL_FLASH_USER_DATA_START,SF_SECTOR_SIZE);
	hal_flash_write(HAL_FLASH_USER_DATA_START,(uint8_t *)&g_stAppBleCtl.stAppBleSysInfo,sizeof(app_ble_sys_info_t));
	
	log_debug("Adv inter set:%d ms\n",g_stAppBleCtl.stAppBleSysInfo.AdvInterval*5/8);//0.625转换

	//修改广播参数
	appm_adv_stop(1);//必须先当前广播包删除，才能修改广播包数据，0代表暂停此次广播，1代表删除此次广播
	co_timer_set(&g_adv_upgrade_timer, 1000, TIMER_ONE_SHOT//一定要延时足够的时间，等待当前广播包删除完毕，不然新的广播无法正常启动
		, app_ble_adv_upgarde_timer_handler, NULL);

	#if (APP_BLE_MSG_SWITCH)
	//直接调用返回响应的api
	app_ble_cmdBleBondDelRsp((void *)&stRet);
	#else
	//转一道，统一交给app_ble_processTxRec（）去调用返回响应的api
	Ret_t *pStRet = KE_MSG_ALLOC(APP_BLE_OPT_BOND_DEL,APP_BLE_TASK_ID, APP_BLE_TASK_ID,Ret_t);
    pStRet->err = stRet.err;
    ke_msg_send(pStRet);
	#endif
}

/**************************************************************
*函数功能描述 ：传递rssi值给MCU主控
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleRssiSendReq(void *pData)
{
	typedef struct
	{
		int8_t i8Rssi;
		uint8_t res[3];
	}Param_t;
	Param_t *pStData;
	
	g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
		,cmdBleRssiSendReq,sizeof(Param_t),(void*)&pStData);
	if((pStData != NULL) && (NULL != pData))
	{
		memcpy(pStData,(const void*)pData,sizeof(Param_t));
		// pStData->i8Rssi = *((int8_t*)pData);
	}
	g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);
	
	hal_uart_fm33lg_tx_buf_set(g_stBleProCtl.txBuf,g_stBleProCtl.txLen);
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
		else if(0 == strncmp("coreuidget",(const char*)pData,10))
		{//获取芯片uid
			uint8_t uid[16];
			sfs_read_uid_ex(uid,sizeof(uid));
			log_debug_array_ex("core uid", uid,sizeof(uid));
		}
		else if(0 == strncmp("sysget",(const char*)pData,6))
		{//打印所有信息
			log_debug("***************************************************\n");
			log_debug("Version data:%s\n",APP_VERSION_DATE);
			log_debug("Version time:%s\n",APP_VERSION_TIME);
			log_debug("reboot_reason:%d\n", pmu_reboot_reason());//0掉电，1深度睡眠唤醒，2ota升级，3主动重启，4看门狗重启，5低电压重启
			sysdump();

			log_debug_array_ex("SN", g_stAppBleCtl.stAppBleSysInfo.sn, APP_BLE_SN_LEN);
			log_debug("Pair num:%d\n",g_stAppBleCtl.stAppBleSysInfo.pairNum);
			log_debug_array_ex("Admin Mac Addr", g_stAppBleCtl.stAppBleSysInfo.administratorMAC, 6);
			log_debug("AdvInterval:%d ms\n",g_stAppBleCtl.stAppBleSysInfo.AdvInterval*5/8);//0.625
			log_debug("u32Pincode:%d\nu8PairFlag:%d\nu8AdminFlag:%d\nu32RssiGetTime:%d ms\n"
					,g_stAppBleCtl.u32Pincode,g_stAppBleCtl.u8PairFlag,g_stAppBleCtl.u8AdminFlag,g_stAppBleCtl.u32RssiGetTime);
			
			log_debug_array_ex("app_env", &app_env, sizeof(app_env));
			log_debug_array_ex("app_sec_env", &app_sec_env, sizeof(app_sec_env));
			log_debug("***************************************************\n");
		}
		else if(0 == strncmp("sysreset",(const char*)pData,8))
		{
			//重置存储信息
			app_ble_sys_info_reset();
			//清空控制信息
			memset((uint8_t *)&g_stAppBleCtl.u32Pincode,0x00,sizeof(app_ble_ctl_t) - sizeof(app_ble_sys_info_t));
			//清空ble绑定信息
			app_sec_remove_bond();
		}
		else if(0 == strncmp("bledis",(const char*)pData,6))
		{//断开连接
			appm_disconnect(0);
		}
		else if(0 == strncmp("conUpParam",(const char*)pData,10))
		{//连接参数更新
    		ke_timer_set(APP_BLE_OPT_UPDATA_PARAM, APP_BLE_TASK_ID, 10);//100ms后主动发送连接参数更新请求
		}
		else if(0 == strncmp("bondPrint",(const char*)pData,9))
		{//打印所有有效配对信息
			struct app_sec_conn_info_tag info;
			nvds_tag_len_t length = sizeof(struct app_sec_conn_info_tag);
			uint8_t nvds_len = NVDS_TAG_BLE_LINK_KEY_LAST-NVDS_TAG_BLE_LINK_KEY_FIRST;

			if (app_sec_env.bonded == true)
			{
				for(uint8_t index =0; index <= nvds_len; index++)
				{
					memset(&info, 0, length);
					if (nvds_get(NVDS_TAG_BLE_LINK_KEY_FIRST+index, &length, (uint8_t *)&info) == NVDS_OK)
					{
						log_debug("------Bond device info[%d],type:%d,mac addr",index,info.id_addr.addr_type);
						log_debug_array_ex("", info.id_addr.addr.addr, GAP_BD_ADDR_LEN);
					}
				}
			}
		}
		
		//*********************************************以下是用log cmd测试mcu cmd的指令码
		else if(0 == strncmp("snset",(const char*)pData,5))
		{//设置sn码，后面加9位字符
			typedef struct
			{
				uint8_t u8SN[APP_BLE_SN_LEN];
				uint8_t res[16 - APP_BLE_SN_LEN];
			}Param_t;
			Param_t stParam;
			memcpy(stParam.u8SN,&pData[5],APP_BLE_SN_LEN);
			app_ble_cmdBleSnSetReq((void *)&stParam);
		}
		else if(0 == strncmp("bondadd",(const char*)pData,7))
		{//启动配对指令
			app_ble_cmdBleBondAddReq(NULL);
		}
		else if(0 == strncmp("bonddel",(const char*)pData,7))
		{//删除配对指令
			uint8_t u8Opt = pData[7] - 0x30;//0不删管理员,1删除所有
			typedef struct
			{
				uint8_t u8Opt;//0x00：删除除了管理员之外所有设备；
								//0x01：删除所有设备；
				uint8_t res[3];
			}Param_t;
			Param_t stParam;
			stParam.u8Opt = u8Opt;
			app_ble_cmdBleBondDelReq((void *)&stParam);
		}
		else if(0 == strncmp("paramset",(const char*)pData,8))
		{//参数配置指令
			uint16_t u16Value = ((uint16_t)(pData[8] - 0x30) * 10) + (uint16_t)(pData[9] - 0x30);
			u16Value *= 100;//单位，100ms，例如传入paramset10就是广播间隔为1000ms = 1s

			typedef struct
			{
				uint16_t u16AdvInterval;//广播间隔
				uint8_t res[14];
			}Param_t;
			Param_t stParam;
			stParam.u16AdvInterval = u16Value;
			app_ble_cmdBleParamSetReq((void *)&stParam);
		}
		else if(0 == strncmp("rssiopt",(const char*)pData,7))
		{//rssi传递操作
			uint8_t u8Opt = pData[7] - 0x30;
			app_ble_rssi_opt(u8Opt,0);
			//也可以调用APP_BLE_OPT_GAP_RSSI
		}
		else
		{}
		
		//清空接收缓存
		hal_uart_log_rx_buf_reset();
	}

    return KE_MSG_CONSUMED;
}

