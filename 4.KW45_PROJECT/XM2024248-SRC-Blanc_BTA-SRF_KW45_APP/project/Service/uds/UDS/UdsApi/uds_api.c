
/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_api.c
 * 摘要: UDS接口文件
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2022-09-08
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "uds_api.h"
#include "DFP.h"
#include "hal_flash_if.h"
#include "string.h"
#include "uds_conf.h"
#include "udstp_conf.h"
#include "udscan_callbacks.h"
#include "app.h"
#include "hal_sys.h"
#include "UdsItf_Srv.h"
#include "hal_uart_if.h"
#include "E2Para.h"
#include "canbus_trans.h"
#if (1 == UdsApi_BTA_SRR_FUN_EN || 1 == UdsApi_BTA_SRF_FUN_EN)
#include "D6Update.h"
#endif
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
#include "BtaPos.h"
#include "E2Para.h"
#endif
static uint8_t is_diag_frames_u8; /* uds diag req flag */
static uint8_t rub_0x10Program_SubFunc;

uint8_t udsapi_Supplier_unlock_flag = FALSE;
uint32_t counter_num = 0;
uint32_t Uds_AddrMode = 0;

static uint8_t UdsApi_D6_Upgrade_Start = 0;
static uint8_t delay_10ms;

static uint8_t udsapi_power_state = UDSAPI_IGN_OFF;

/*车速信息*/
static uint16_t UdsApi_VehSpd;
/*电压信息*/
static uint8_t  UdsApi_BattVol;
static uint8_t UdsApi_VehSpd_st;
static uint8_t UdsApi_can_sig_11E_timeout_flg = FALSE;

static uint8_t UdsApi_ble_broadcasting_state = 0x0;

#ifdef UdsApi_BTA_R_FUN_EN
__attribute__((section("PN")))  uint8_t pn[8] = {'P', '0', '3', '8', '0','5','0','3'};
#endif

#ifdef UdsApi_BTA_L_FUN_EN
__attribute__((section("PN")))  uint8_t pn[8] = {'P', '0', '3', '8', '0','5','0','4'};
#endif
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
__attribute__((section("PN")))  uint8_t pn[8] = {'P', '0', '3', '8', '2','8','6','8'};
#endif
#ifdef UdsApi_BTA_SRR_FUN_EN
__attribute__((section("PN")))  uint8_t pn[8] = {'P', '0', '3', '8', '1','2','0','3'};
#endif
#ifdef UdsApi_BTA_SRF_FUN_EN
__attribute__((section("PN")))  uint8_t pn[8] = {'P', '0', '3', '8', '1','2','0','4'};
#endif

#ifdef UdsApi_BTA_RB2_FUN_EN
__attribute__((section("PN")))  uint8_t pn[8] = {'P', '0', '3', '8', '0','5','0','6'};
#endif
#if 0
void uds_d6_test(void)
{
	D6SWUPStart((uint8_t *)0xa6000,1);
	UdsApi_D6_Upgrade_Start = 1;

}
#endif
/*
 * 函数介绍: UDS初始化函数接口
 * 参数:
 * 返回值: {*}
 * 备注: 系统初始化时调用
 */
static void UdsApi_Set_PN_NUM(void)
{
#ifdef UdsApi_BTA_R_FUN_EN
    pn[7] = '3';
#endif
#ifdef UdsApi_BTA_L_FUN_EN
    pn[7] = '4';
#endif
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
    pn[7] = '8';
#endif
#ifdef UdsApi_BTA_SRR_FUN_EN
    pn[7] = '3';
#endif
#ifdef UdsApi_BTA_SRF_FUN_EN
    pn[7] = '4';
#endif
#ifdef UdsApi_BTA_RB2_FUN_EN
    pn[7] = '6';
#endif


}
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
/*
 * 函数介绍: 前后保零件初始化通过PIN脚电压来判断是前保还是后保
 * 参数:
 * 返回值: {*}
 * 备注: 系统初始化时调用
 */
extern uint8_t DID_F118[DID_F118_LENGTH];

static void UdsApi_External_BTA_ini(void)
{
	uint8_t FB_DID_F118[DID_F118_LENGTH] = {'P','0','3','9','0','4','1','5',' ','A','A'};
	uint8_t RB_DID_F118[DID_F118_LENGTH] = {'P','0','3','8','9','6','6','5',' ','A','A'};
    /* 检查是前保还是后保，前保是原来的BTA_FB，后保是BTA_RB2 */
    if (External_Front == Get_BTAPos())
    {
            Set_Bta_External_Front_CAN_Param();
		memcpy(DID_F118,FB_DID_F118,DID_F118_LENGTH);
    }
    if (External_Rear == Get_BTAPos())
    {
            Set_Bta_External_Rear_CAN_Param();
		memcpy(DID_F118,RB_DID_F118,DID_F118_LENGTH);
    }
}
#endif
/*
 * 函数介绍: UDS初始化函数接口
 * 参数:
 * 返回值: {*}
 * 备注: 系统初始化时调用
 */
void UdsApi_Uds_Initialize(void)
{

    is_diag_frames_u8 = 0;
    /*flash initialize*/
    hal_cfls_if_init();
    /*DTC initialize*/
    DTC_Init();
    /*UDS Initialize*/
    Uds_Initialize();
    E2Para_deinit();
#if (1 == UdsApi_BTA_SRR_FUN_EN || 1 == UdsApi_BTA_SRF_FUN_EN)
    /*D6刷写初始化*/
    UdsApi_D6_upgrage_Deinit();
#endif
    UdsApi_Set_PN_NUM();
    /**/
    udsapi_power_state = UDSAPI_IGN_ON;
    UdsApi_ble_broadcasting_state = 0;
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
    UdsApi_External_BTA_ini();

#endif
}

/*
 * 函数介绍: dtc 测试函数
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void uds_dtc_test(void)
{
    Dtc_Active(&DFC_BattMin);

    Dtc_Recove(&DFC_BattMin);
}

/*
 * 函数介绍: UDS任务函数
 * 参数:
 * 返回值: {*}
 * 备注: 需要循环调用
 */
static uint16_t tim_10ms = 0;
void UdsApi_Uds_Process(void)
{
    /*DTC update process*/
    Dtc_update();
    /*UDS Perform process*/
    Uds_Processor();
   // uds_dtc_test();
#if (1 == UdsApi_BTA_SRR_FUN_EN || 1 == UdsApi_BTA_SRF_FUN_EN)
    D6SWUPEntry();

    if(tim_10ms==0)
    {
    	UdsApi_D6_upgrage_Check();
    	tim_10ms = 1;
    }
#endif


}
/*
 * 函数介绍: UDS 10ms任务函数
 * 参数:
 * 返回值: {*}
 * 备注: 需要循环调用
 */
void UdsApi_1ms_Proc(void)
{
	UdsApi_TimeoutTimeer();
}
/*
 * 函数介绍: UDS 10ms任务函数
 * 参数:
 * 返回值: {*}
 * 备注: 需要循环调用
 */

void UdsApi_10ms_Proc(void)
{
	Uds_MainTaskEntryPoint_CyclicTask();
	if(tim_10ms != 0)
	{
		tim_10ms--;
	}
}

/*
 * 函数介绍:flash擦除写入封装函数
 * 参数: index：    需要操作的flash地址序列号
 *       databuf：  写入的数据
 *       len    ：  写入的长度
 * 返回值: R_FDL_OK：操作成功
 * 备注:
 */
r_fdl_status_t Uds_Api_fdl_Ereas_Write(uint8_t index, const uint8_t *databuf, uint32_t len)
{

    r_fdl_status_t result = R_FDL_OK;

    if (databuf != NULL)
    {
        result = hal_cfls_if_erase_write(index, databuf, len, len);
    }
    else
    {
        result = R_FDL_ERR_PARAMETER;
    }

    return result;
}
/*
 * 函数介绍: flash读操作函数接口
 * 参数:  index：    需要操作的flash地址序列号
 *       databuf：  写入的数据
 *       len    ：  写入的长度
 * 返回值: R_FDL_OK：操作成功
 * 备注:
 */
r_fdl_status_t Uds_Api_fdl_FlashRead(uint8_t index, uint8_t *databuf, uint32_t len)
{

    r_fdl_status_t result = R_FDL_OK;

    if (databuf != NULL)
    {
        result = hal_cfls_if_read(index, databuf, len);
    }
    else
    {
        result = R_FDL_ERR_PARAMETER;
    }

    return result;
}

/*
 * 函数介绍: UDS数据发送接口
 * 参数: msgId：CAN/CANFD ID
 *      numBytes:数据长度
 *      pBytes：发送数据
 * 返回值: {*}
 * 备注:
 */
void UdsApi_TXSendData(uint32_t msgId, uint8_t numBytes, uint8_t *pBytes)
{

    hal_can_pdu_st can_trans_pdu;
    if (pBytes != NULL)
    {
    	if(UdsApi_GetD6UpradeRefreshFlg() != 0)
    	{
//    		 /*CANfd发送结构体数据填充*/
//			can_trans_pdu.m_id_type_e = HAL_CAN_ID_TYPE_StAND;
//			can_trans_pdu.m_u32_canid = msgId;
//			can_trans_pdu.m_u8_dlc = numBytes;
//			can_trans_pdu.m_u8_pdata = pBytes;
//			/*调用底层发送函数接口*/
//			hal_can_if_write(0, &can_trans_pdu);
    		canbusTransSendBusoffMsgFrame(msgId,numBytes,pBytes);
    	}
        else
        {
        	canbusTransSendMsgFrame(msgId,numBytes,pBytes);
        }

    }
    else
    {
        /* code */
    }

}
/*
 * 函数介绍: uds数据发送完成回调函数
 * 参数: id：发送完成的CAN id
 * 返回值: {*}
 * 备注:
 */
void UdsApi_Can_TxConfirm_cbk(uint32_t id)
{
    UdsCan_LData_TxConfirmation((uint32_t)id, (uint8_t)0); // send  successfully
};
/*
 * 函数介绍: 获取uds地址
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint32_t UdsApi_GetAddrMode(void)
{
    return Uds_AddrMode;
}
/*
 * 函数介绍: UdsApi_ReciveData函数用于处理接收到的UDS（统一诊断服务）消息
 * 参数:
 *@param msgId 接收到的消息的ID，用于识别消息的类型。
 * @param msgLen 接收到的消息的数据长度。
 * @param pMsgData 指向接收到的消息数据的指针。
 * 返回值: 返回一个表示处理结果的代码，UDSAPI_RLT_OK表示成功，UDSAPI_RLT_ERR表示错误。
 * 备注:
 */

uint8_t UdsApi_ReciveData(uint32_t msgId, uint8_t msgLen, uint8_t *pMsgData)
{
    uint8_t relt; // 用于存储处理结果的变量

    // 检查消息数据指针是否为空
    if (pMsgData != NULL)
    {
        // 检查消息ID是否为物理层或功能层消息ID
        if ((msgId == UdsCan0_ChannelCfgs[0].msgIdPhy) || (msgId == UDS0_CAN_RX_MSGID_FNC))
        {
#if 1
        	if(UDS_PROGRAMMING_SESSION == rub_0x10Program_SubFunc)
		  {
			  /*编程会话模式下收到3E80丢弃*/
			  if((0x02 == pMsgData[0]) && ( 0x3E == pMsgData[1]) && ( 0x80 == pMsgData[2]))
			  {
				  return UDSAPI_RLT_OK;
			  }
			  else
			  {}
		  }
		  else
		  {}
#endif
            // 设置当前地址模式，并调用消息处理函数
            Uds_AddrMode = msgId;
            UdsCan_LData_RxIndication(msgId, msgLen, pMsgData);
            relt = UDSAPI_RLT_OK; // 消息处理成功，返回OK代码
        }
        else
        {
            relt = UDSAPI_RLT_ERR; // 消息ID不匹配，返回错误代码
        }
    }
    else
    {
        relt = UDSAPI_RLT_ERR; // 消息数据指针为空，返回错误代码
    }

    return relt; // 返回处理结果代码
}
/*
 * 函数介绍: 接收中断回调函数
 * 参数: can_recv_st：can接收结构体
 * 返回值: {*}
 * 备注:
 */

void UdsApi_ReceiveCallBack(hal_can_pdu_st * can_recv_st)
{
    if (NULL != can_recv_st )
    {
      UdsApi_ReciveData((uint32_t)(can_recv_st->m_u32_canid), (uint8_t)(can_recv_st->m_u8_dlc), (uint8_t *)(can_recv_st->m_u8_pdata));
    }
    else
    {
        /* code */
    }
}

/*
 * 函数介绍: UDS timer
 * 参数:
 * 返回值: {*}
 * 备注: 1ms任务中调用
 */
void UdsApi_TimeoutTimeer(void)
{

    TICK_UDSTP_SESSION1_TIMER();
    TICK_UDS_SESSION_TIMER();
    TICK_UDS_APP_TIMER();
#if (1 == UdsApi_BTA_SRR_FUN_EN || 1 == UdsApi_BTA_SRF_FUN_EN)
    D6SWUPTICK();
#endif

    DTC_1ms_Proc();

}

/*
 * 函数介绍: 设置诊断请求标志
 * 参数: reqFlag：诊断请求标志
 * 返回值: {*}
 * 备注: 用于网络管理睡眠唤醒
 */
void UdsApi_setDiagFrameReqFlag(uint8_t reqFlag)
{
    is_diag_frames_u8 = reqFlag;
}
/*
 * 函数介绍: 获取诊断请求标志
 * 参数:
 * 返回值: 诊断请求状态
 * 备注:
 */
uint8_t UdsApi_getDiagFrameReqFlag(void)
{
    return is_diag_frames_u8;
}

/*
 * 函数介绍: 清楚boot跳转标志
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UdsApi_clearBootFlag(uint8_t sub)
{
    uint8_t data[5] = {0};
    uint8_t rec_buf[3];
    uint8_t data1[8] = {0};

    rec_buf[0] = 0x02; // msg length
    rec_buf[1] = 0x10; // server
    rec_buf[2] = sub;  // sub
    memcpy(data1, data, 5);
    memcpy(&data1[5], rec_buf, 3);
    //Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT2, data1, 8);
}

/**
 * 函 数 介 绍 ：由编程会话请求，写入标志后系统复位
 * 参       数 ：无
 * 返   回  值 ：无
 * 备       注 ：无
 */
void UdsApi_APP_JumpToBoot(void)
{
    r_fdl_status_t result = R_FDL_OK;
    JUMP_REFRESH_INFOR g_jumpRefreshInf;

    uint32_t primaskValue;
    /* 读取跳转标志信息 */
    result = Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_BOOT0, g_jumpRefreshInf.buff, sizeof(g_jumpRefreshInf.buff));

	 /* 将跳转标志修改为禁止跳转 */
	g_jumpRefreshInf.infor.resp_flag = 0x1000;
	g_jumpRefreshInf.infor.resp_flag |= rub_0x10Program_SubFunc;
	g_jumpRefreshInf.infor.jump_flag = JUMP_FALSE;



    result = Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT0, g_jumpRefreshInf.buff, sizeof(g_jumpRefreshInf.buff));
    hal_sys_enabled_irq(HAL_ENABLED_DISABLE);
    /*复位*/
    UdsApi_Hardware_Reset();
}
#if (1 == UdsApi_BTA_SRR_FUN_EN || 1 == UdsApi_BTA_SRF_FUN_EN)

/**
 * 函 数 介 绍 ：D6升级初始化操作
 * 参       数 ：无
 * 返   回  值 ：无
 * 备       注 ：无
 */
void UdsApi_D6_upgrage_Deinit(void)
{
	JUMP_REFRESH_INFOR g_jumpRefreshInf;
	uint8_t i;
	uint8_t pRspdata2[8] = {0x03,0x7f,0x31,0x78};
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_BOOT0, g_jumpRefreshInf.buff, sizeof(g_jumpRefreshInf.buff));

	if(0xD6D6 == g_jumpRefreshInf.infor.d6_flag)
	{
//		for(i= 4; i< 8;i++)
//		{
//			pRspdata2[i] = 0xAA;
//		}
//		UdsApi_TXSendData(UDS0_CAN_TX_MSGID_PHY,8,pRspdata2);
		/*开启D6升级流程*/
		if(1== D6SWUPStart((uint8_t *)0xA0000,1))
		{
			/*D6刷写标志开启*/
			UdsApi_D6_Upgrade_Start = 1;
			logPrintf("D6 UPDATE FALG 1 !\r\n");
		}
		else{
			g_jumpRefreshInf.infor.d6_flag = D6_UPGRADE_FAIL;
			g_jumpRefreshInf.infor.jump_flag = JUMP_FALSE;
			Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT0, g_jumpRefreshInf.buff, sizeof(g_jumpRefreshInf.buff));
			UdsApi_Hardware_Reset();
		}
	}
	else
	{}
}
/**
 * 函 数 介 绍 ：设置扩展数据
 * 参       数 ：dtc_extdata_info详情参考DFPM_ExtDataInfo结构体
 * 返   回  值 ：无
 * 备       注 ：无
 */
void UdsApi_DFPM_setDtcExtData(DFPM_ExtDataInfo * dtc_extdata_info)
{
	if(dtc_extdata_info != NULL)
	{
		DFPM_setDtcExtData(dtc_extdata_info);
	}
	else
	{
		/* code */
	}

}

/**
 * 函 数 介 绍 ：D6升级返回结果检查
 * 参       数 ：无
 * 返   回  值 ：
 * 备       注 ：10ms任务周期调度
 */
#define PENDING_RSP_78_TIME	(100U)
void UdsApi_D6_upgrage_Check(void)
{

	JUMP_REFRESH_INFOR g_jumpRefreshInf;
	uint8_t i;
	D6PriviteInfo *pD6_info = NULL;
	uds_write_flash_data_un  Diddata;
	static uint16_t pending_10ms_cnt = 0;
	static uint8_t frist_sned_pending_flag = 0;
	uint8_t pRspdata2[8] = {0x03,0x7f,0x31,0x78};
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_BOOT0, g_jumpRefreshInf.buff, sizeof(g_jumpRefreshInf.buff));

	if(1 == UdsApi_D6_Upgrade_Start)
	{
		/*失败*/
		if(0 == D6SWUPGetSta())
		{
			UdsApi_D6_Upgrade_Start = 0;
			g_jumpRefreshInf.infor.d6_flag = D6_UPGRADE_FAIL;
			g_jumpRefreshInf.infor.jump_flag = JUMP_FALSE;
			Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT0, g_jumpRefreshInf.buff, sizeof(g_jumpRefreshInf.buff));
			logPrintf("0x78 Running0 !\r\n");
			UdsApi_Hardware_Reset();
		}/*升级中*/
		else if(1 == D6SWUPGetSta())
		{
			if(0 == frist_sned_pending_flag)
			{	/*刷写中发送pengding报文*/
				if(pending_10ms_cnt >= PENDING_RSP_78_TIME)
				{
					for(i= 0; i< 4;i++)
					{
						pRspdata2[4+i] = 0xAA;
					}
					UdsApi_TXSendData(UDS0_CAN_TX_MSGID_PHY,8,pRspdata2);
					frist_sned_pending_flag = 1;
					pending_10ms_cnt = 0;
					logPrintf("0x78 Running !\r\n");
				}
				else
				{
					pending_10ms_cnt++;
				}

			}
			else
			{
				if(pending_10ms_cnt>=PENDING_RSP_78_TIME)
				{
					for(i= 0; i< 4;i++)
					{
						pRspdata2[4+i] = 0xAA;
					}
					UdsApi_TXSendData(UDS0_CAN_TX_MSGID_PHY,8,pRspdata2);
					frist_sned_pending_flag = 1;
					pending_10ms_cnt =0;
					logPrintf("0x78 Running 2 !\r\n");
				}
				else
				{
					pending_10ms_cnt++;
					//dbg_Printf("%05d !\r\n",pending_10ms_cnt);
				}
			}
		}/*刷写成功*/
		else if(2 == D6SWUPGetSta())
		{
			pD6_info = D6SWUPGetInfo();
			Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,Diddata.write_buffer,DID_WRITE_DATA_LEN);
			memcpy(Diddata.sig.F130_data,pD6_info->sig0,11);
			memcpy(Diddata.sig.F131_data,pD6_info->sig1,2);
			memcpy(Diddata.sig.F151_data,pD6_info->sig2,6);
			Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID,Diddata.write_buffer,DID_WRITE_DATA_LEN);
			UdsApi_D6_Upgrade_Start = 0;
			g_jumpRefreshInf.infor.d6_flag = D6_UPGRADE_SUCC;
			g_jumpRefreshInf.infor.jump_flag = JUMP_FALSE;
			Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT0, g_jumpRefreshInf.buff, sizeof(g_jumpRefreshInf.buff));
			logPrintf("D6 SWUUPDATE is ok !\r\n");
			UdsApi_Hardware_Reset();

		}
		else
		{
			UdsApi_D6_Upgrade_Start = 0;
			g_jumpRefreshInf.infor.d6_flag = D6_UPGRADE_FAIL;
			g_jumpRefreshInf.infor.jump_flag = JUMP_FALSE;
			Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT0, g_jumpRefreshInf.buff, sizeof(g_jumpRefreshInf.buff));
			logPrintf("0x78 error !\r\n");
			UdsApi_Hardware_Reset();
		}
	}
	else
	{}
}
#endif
/*
 * 函数介绍: 获取D6刷写标志
 * 参数:
 * 返回值:  1：开始刷写D6底层发送接口允许发送数据
 * 		  0：D6刷写完成或失败，底层发送接口需要检测网络管理状态来决定是否能发送报文
 *
 *
 * 备注:
 */
uint8_t UdsApi_GetD6UpradeRefreshFlg(void)
{
	return UdsApi_D6_Upgrade_Start;
}
/*
 * 函数介绍: 复位接口
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UdsApi_Hardware_Reset(void)
{
    hal_sys_if_reset();
}
/*
 * 函数介绍:设置10会话模式
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UdsApi_SetRub_0x10Program_SubFunc(uint8_t subFunc)
{
    rub_0x10Program_SubFunc = subFunc;
}
/*
 * 函数介绍: 编程会话处理函数
 * 参数:
 * 返回值: {*}
 * 备注: 10ms周期调用
 */
void UDS_FN_TYPE UdsApp_10Dsc_Programing_Session_Handler(void)
{
	static uint8_t jump_cnt = 0;
    if (UDS_PROGRAMMING_SESSION == (rub_0x10Program_SubFunc & 0x7F))
    {
    	jump_cnt++;
    	/*延时一段时间再跳转*/
    	if(10 == jump_cnt)
    	{
    		jump_cnt = 0;
    		/*跳转到boot*/
    		UdsApi_APP_JumpToBoot();
    	}
    	else
    	{}
    }
    else
    {}
}
/*
 * 函数介绍: 获取点火状态
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UdsApi_getIgnstate(void)
{
    return udsapi_power_state;
}

/*
 * 函数介绍: 获取点火状态有效标志位
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UdsApi_getIgnstateValid(void)
{
    return TRUE;
}
/*
 * 函数介绍: 获取APP报文接收状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_get_normal_rx_sta(void)
{
	return (uint8_t)Uds_itf_normal_rx_state();
}
/*
 * 函数介绍: 获取APP报文发送状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_get_normal_tx_sta(void)
{
	return (uint8_t)Uds_itf_normal_tx_state();
}
/*
 * 函数介绍: 获取NM报文接收状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_get_nm_rx_sta(void)
{
	return (uint8_t)Uds_itf_nm_rx_state();
}
/*
 * 函数介绍: 获取NM报文发送状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_get_nm_tx_sta(void)
{
	return (uint8_t)Uds_itf_nm_tx_state();
}
/*
 * 函数介绍: 检查boot跳转条件是否满足
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_Check_Bootjump_Condition(void)
{
	uint16 veh_temp;
	 uint8_t batt_temp;
	 uint8_t vehspd_st;
	 uint8_t timeout_flg;
	 uint8_t ign_temp;
	 uint8_t RspStatus = TRUE;
	 veh_temp = UdsApi_getVehspdInfo();
	 batt_temp = UdsApi_getBattInfo();
	 vehspd_st = UdsApi_getVehSpdst();
	 timeout_flg = UdsApi_get11EtimeoutFlg();
	 //vehspd< 3km/h,电压在9V-16V
	 if(TRUE == timeout_flg)
	 {
		 if((batt_temp >=90) && (batt_temp <= 160))
		 {
			 RspStatus = TRUE;
		 }
		 else
		 {

			 RspStatus = FALSE;
		 }
	 }
	 else
	 {
		 if ((veh_temp <= 53)&&(0x1 == vehspd_st)&&((batt_temp >=90) && (batt_temp <= 160)))
		 {
			 RspStatus = TRUE;
		 }
		 else
		 {
			 RspStatus = FALSE;
		 }
	 }
	 return RspStatus;
}
/*
 * 函数介绍: 设置车速信息
 * 参数:
 * 返回值:
 * 备注:km/h
 */
void UdsApi_setVehspdInfo(uint16_t vehspd)
{
	/*精度0.05625*/
	UdsApi_VehSpd = vehspd;
}
/*
 * 函数介绍: 设置电压信息
 * 参数:
 * 返回值:
 * 备注:
 */
void UdsApi_setBattInfo(uint16_t BattVol)
{
	/*精度0.1*/
	UdsApi_BattVol = BattVol;
}
/*
 * 函数介绍: 获取车速信息
 * 参数:
 * 返回值: 车速信息
 * 备注:单位km/h
 */
uint16_t UdsApi_getVehspdInfo(void)
{
	return UdsApi_VehSpd;
}
/*
 * 函数介绍: 获取电压信息
 * 参数:
 * 返回值: 电压信息
 * 备注:单位v
 */
uint8_t UdsApi_getBattInfo(void)
{
	return (UdsApi_BattVol);
}
/*
 * 函数介绍: 设置车速有效信号
 * 参数:
 * 返回值:
 * 备注:
 */
void UdsApi_setVehSpdst(uint8_t VehSpdst)
{
	UdsApi_VehSpd_st = VehSpdst;
}
/*
 * 函数介绍: 获取车速有效信号
 * 参数:
 * 返回值: 车速信息
 * 备注:单位km/h
 */
uint8_t UdsApi_getVehSpdst(void)
{
	return UdsApi_VehSpd_st;
}
/*
 * 函数介绍: 设置0x11E报文超时标志
 * 参数:
 * 返回值:
 * 备注:
 */
void UdsApi_set11EtimeoutFlg(uint8_t timeoutflg)
{

	UdsApi_can_sig_11E_timeout_flg = timeoutflg;
}
/*
 * 函数介绍: 获取0x11E报文超时标志
 * 参数:
 * 返回值: 车速信息
 * 备注:单位km/h
 */
uint8_t UdsApi_get11EtimeoutFlg(void)
{
	return UdsApi_can_sig_11E_timeout_flg;
}
/*
 * 函数介绍: 设置kw45复位源
 * 参数:resetData：复位源数据
 * 返回值:
 * 备注:
 * 	0x01=WAKEUP Wake-up Reset/唤醒复位
	0x02=POR Power-on Reset/Power-on复位
	0x03=LVD Low Voltage Detect Reset/低电压检测复位
	0x04=HVD High Voltage Detect Reset/高电压检测复位
	0x05=WARM Warm Reset/热复位
	0x06=FATAL Fatal Reset/Fatal复位
	0x08=Reset Pin Reset/复位管脚复位
	0x09=DAP Debug Access Port Reset/Debug管脚复位
	0x0A=RSTACK Reset Timeout/系统复位生成逻辑中出现超时或其他错误条件导致复位
	0x0B=LPACK Low Power Acknowledge Timeout Reset/低功耗模式进入逻辑中的超时导致的复位
	0x0C=SCG System Clock Generation Reset/SCG 中的时钟丢失或锁定丢失事件导致复位
	0x0D=WDOG0 Watchdog 0 Reset/看门狗0超时复位
	0x0E=SW Software Reset/ ARM内核SYSRESETREQ软件复位
	0x0F=Lockup Reset/ARM内核指示LOCKUP事件导致复位
	0x10=WDOG1 Watchdog 1 Reset/看门狗1超时复位
	0x11=SECVIO Security Violation Reset/安全违规逻辑导致复位
 */
void UdsApi_set_KW45resetSource(uint8_t* resetData)
{
	uds_write_flash_data_un did_flash_data_un;
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,did_flash_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(did_flash_data_un.sig._4a0a_data,resetData,DID_4A0A_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID,did_flash_data_un.write_buffer,DID_WRITE_DATA_LEN);
}
/*
 * 函数介绍: 设置D6复位源
 * 参数:resetData：复位源数据
 * 返回值:
 * 备注:
 * 	0x01=brownout on the CPU retention power domain/CPU 保留电源域上的欠压故障
0x02=toggling the reset pin during deep power down/在深度掉电期间触发复位引脚
0x03=last reset was due to EDC/上一次重置由EDC引起
0x04=last reset was due to failure detected by SRAM ECC/上次重置是由于 SRAM ECC 检测到故障
0x05=last reset was due to failure detected by Frequency monitoring/上次重置是由于频率监控检测到故障
0x06=Reset caused by power failure of IO power domain/IO 电源域断电导致复位
0x07=Reset caused by power failure of IO power domain related to NV operation/与 NV 操作有关的 IO 电源域电源故障导致复位
0x08=Reset caused by power failure on the bias bandgap/由于偏置带隙上的电源故障导致的复位
0x09=Reset caused by power failure on the CPU retention power domain/CPU 保留电源域电源故障导致复位
0x0A=Reset caused by power failure on the non volatile (page flash) power domain(NV_POK)/非易失性电源域断电导致的复位
0x0B=last reset was due to glitch detector/上次重置是由于探测器故障
0x0C=last reset was due to Analog glitch sensor/上次重置是由于模拟瞬态检测器触发
0x0D=Reset caused by toggling the reset pin during hard power down/在硬掉电期间触发复位引脚导致复位
0x0E=Reset caused by toggling the wake up pin during hard power down/在硬掉电期间触发唤醒引脚导致复位
0x0F=last reset was due to noise generation and fault injection detection/上次重置是由于噪声产生和故障注入检测
0x10=last reset was due to INGFID/上一次重置是由于INGFID
0x11=last reset was due to failure detected by local supply drop detector/上次重置是由于本地供电中断检测器检测到故障
0x12=Indicates a Power on reset (POR)/开机复位 (POR)
0x13=Reset caused by toggling the reset pin/通过触发复位引脚引起复位
0x14=last reset was due to failure detected by secure fetch/上次重置是由于安全获取检测到故障
0x15=Reset caused by software master RESET/由软件主控 RESET 引起的复位
0x16=Indicate the last reset was due to System reset request/表示上次重置是由于系统重置请求
0x17=last reset was due to failure detected by violation detector/上次重置是由于违规检测器检测到故障
0x18=Reset caused by the watchdog timer/看门狗定时器引起的复位
 */
void UdsApi_set_D6resetSource(uint8_t* resetData)
{
	uds_write_flash_data_un did_flash_data_un;
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,did_flash_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(did_flash_data_un.sig._4a57_data,resetData,DID_4A57_LENGTH);
	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID,did_flash_data_un.write_buffer,DID_WRITE_DATA_LEN);
}
/*
 * 函数介绍: 设置蓝牙广播状态
 * 参数:state = 0x01:打开，=0x00:关闭
 * 返回值:
 * 备注:
 */
void UdsApi_set_BleTurnOnState(uint8_t state)
{
	UdsApi_ble_broadcasting_state = state;
}
/*
 * 函数介绍: 设置蓝牙广播状态
 * 参数:
 * 返回值: 状态信息，state = 0x01:打开，=0x00:关闭
 * 备注:
 */
uint8_t UdsApi_get_BleTurnOnState(void)
{
	return UdsApi_ble_broadcasting_state;
}
