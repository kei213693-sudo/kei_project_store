/*
***************************************************************************************
* FileName:  canbus_app.c
* Description:
* Date:
***************************************************************************************
*/
#include <string.h>
#include "fsl_os_abstraction.h"
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"
#include "canbus_trans.h"
#include "canbus_app.h"
#include "autosar_nm.h"
#include "uds_api.h"
#include "DFP.h"
#include "Dtc_Manage.h"
#include "app_dtc.h"
#include "UWB_CAN.h"
//#include "hci_interface.h"

#define canbusTimerCreate(t)	(OSA_TimeGetMsec() + (t))
#define canbusTimerIsExpired(t)	(OSA_TimeGetMsec() >= (t))

#define NR_TX_MSGS (sizeof(Tx_AppMsgs)/sizeof(Can_Tx_Msg))

//tx
static BTA4_FusnData_un			BTA4_FusnData_bf;
static BTA4_RSSI_UWB_un 		BTA4_RSSI_UWB_bf;
static BTA_SRR_297_un			BTA_SRR_297_bf;
static IotHubSts_un 			IotHubSts_bf;
static BTA4_Log_Data_un			BTA4_Log_Data_bf;
static BTA_SRR_FactoryRsp_un 	BTA_SRR_FactoryRsp_bf;
static DIAG_RESP_BTA4_un		DIAG_RESP_BTA4_bf;
static IotConnectSts_un 		IotConnectSts_bf;
static IotDownLinkFlowCtrl_un	IotDownLinkFlowCtrl_bf;
static IotUplinkData_un 	 	IotUplinkData_bf;

static void BTA4_FusnData_PreSendCallback(uint8_t index);
static void BTA4_RSSI_UWB_PreSendCallback(uint8_t index);
static void BTA_SRR_297_PreSendCallback(uint8_t index);
static void IotHubSts_PreSendCallback(uint8_t index);
static void BTA4_Log_Data_PreSendCallback(uint8_t index);
static void BTA_SRR_FactoryRsp_PreSendCallback(uint8_t index);
//static void DIAG_RESP_BTA4_PreSendCallback(uint8_t index);
static void IotConnectSts_PreSendCallback(uint8_t index);
static void IotDownLinkFlowCtrl_PreSendCallback(uint8_t index);
static void IotUplinkData_PreSendCallback(uint8_t index);



static Can_Tx_Msg Tx_AppMsgs[] =
{
//	{BTA4_FusnData_CANID,  	BTA4_FusnData_DLC,				0, 0, 1, 0, 0, 0,   1, 100,      	0, 0,   BTA4_FusnData_PreSendCallback,		 0, 	BTA4_FusnData_bf.data},
//	{BTA4_RSSI_UWB_CANID,	BTA4_RSSI_UWB_DLC,				0, 0, 1, 0, 0, 0,   2, 50,  		0, 0,   BTA4_RSSI_UWB_PreSendCallback, 		 0, 	BTA4_RSSI_UWB_bf.data},
//	{BTA_SRR_297_CANID,		BTA_SRR_297_DLC,				0, 0, 1, 0, 0, 0,   3, 100,  		0, 0,   BTA_SRR_297_PreSendCallback, 		 0, 	BTA_SRR_297_bf.data},
//	{IotHubSts_CANID,		IotHubSts_DLC,					0, 0, 1, 0, 0, 0,   4, 100,  		0, 0,   IotHubSts_PreSendCallback, 			 0, 	IotHubSts_bf.data},
//	{BTA4_Log_Data_CANID,	BTA4_Log_Data_DLC,				0, 0, 1, 0, 0, 0,   5,  0,  		0, 0,   BTA4_Log_Data_PreSendCallback, 		 0, 	BTA4_Log_Data_bf.data},
//	{BTA_SRR_FactoryRsp_CANID,	BTA_SRR_FactoryRsp_DLC,		0, 0, 1, 0, 0, 0,   6,  0,  		0, 0,   BTA_SRR_FactoryRsp_PreSendCallback,  0, 	BTA_SRR_FactoryRsp_bf.data},
//	//{DIAG_RESP_BTA4_CANID,	DIAG_RESP_BTA4_DLC,				0, 0, 0, 0, 0, 0,   0,  0,  		0, 0,   DIAG_RESP_BTA4_PreSendCallback, 	 0, 	DIAG_RESP_BTA4_bf.data},
//	{IotConnectSts_CANID,	IotConnectSts_DLC,				0, 0, 1, 0, 0, 0,   7,  0,  		0, 0,   IotConnectSts_PreSendCallback, 		 0, 	IotConnectSts_bf.data},
//	{IotDownLinkFlowCtrl_CANID,	IotDownLinkFlowCtrl_DLC,	0, 0, 1, 0, 0, 0,   8,  0,  		0, 0,   IotDownLinkFlowCtrl_PreSendCallback, 0, 	IotDownLinkFlowCtrl_bf.data},
//	{IotUplinkData_CANID,	IotUplinkData_DLC,				0, 0, 1, 0, 0, 0,   9,  0,  		0, 0,   IotUplinkData_PreSendCallback, 		 0, 	IotUplinkData_bf.data},
	{UWB_CAN_UCI_RSP_SEND_CAN_ID,	UWB_CAN_UCI_RSP_DATA_DLC,	 0, 0, 0, 0, 0, 0, 1, 0,  	 0, 0,   uwbCan_UciRspDataPreSendCallback, 	NULL, 	uwbCan_UciMsgTxBuff},
//	{DV_TEST_SRF_215_CAN_ID,	DV_TEST_SRF_215_DATA_DLC,	 0, 0, 0, 0, 0, 0,  2, 1000,  	 0, 0,   uwbCan_CanMsg215PreSendCallback, 	NULL, 	uwbCan_canMsg215TxBuff},
//	{DV_TEST_SRF_216_CAN_ID,	DV_TEST_SRF_216_DATA_DLC,	 0, 0, 0, 0, 0, 0,  3, 0,  	 0, 0,   uwbCan_CanMsg216PreSendCallback, 	NULL, 	uwbCan_canMsg216TxBuff},
//	{DV_TEST_SRF_20D_CAN_ID,	DV_TEST_SRF_20D_DATA_DLC,	 0, 0, 0, 0, 0, 0,  4, 100,  	 0, 0,   uwbCan_CanMsg20dPreSendCallback, 	NULL, 	uwbCan_canMsg20dTxBuff}

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define NR_RX_MSGS (sizeof(Rx_AppMsgs)/sizeof(Can_Rx_Msg))

Can_Rx_MsgBuff	BCU_11E_bf;
Can_Rx_MsgBuff	BLE_SnifferParam_Dev1_bf;
Can_Rx_MsgBuff	BLE_SnifferParam_Dev2_bf;
Can_Rx_MsgBuff	BLE_SnifferParam_Dev3_bf;
Can_Rx_MsgBuff	BLE_SnifferParam_Dev4_bf;
Can_Rx_MsgBuff	BLE_SnifferParam_Dev5_bf;
Can_Rx_MsgBuff	Device_IRK_Group_1_bf;
Can_Rx_MsgBuff	Device_IRK_Group_2_bf;
Can_Rx_MsgBuff	VC_Data_5E0_bf;
Can_Rx_MsgBuff	ZONE_13C_bf;
Can_Rx_MsgBuff	ZONE_296_bf;
Can_Rx_MsgBuff	ZONE_3AA_bf;
Can_Rx_MsgBuff	ZONE_3B2_bf;
Can_Rx_MsgBuff	BTA_SRR_FactoryReq_bf;
Can_Rx_MsgBuff	DIAG_REQ_AllECU_bf;
Can_Rx_MsgBuff	DIAG_REQ_BTA4_bf;
Can_Rx_MsgBuff	IotDownLinkData_bf;
Can_Rx_MsgBuff	IotScanConnectReq_bf;
Can_Rx_MsgBuff	IotUpLinkFlowCtrl_bf;
Can_Rx_MsgBuff	UWB_Info_Ntf_bf;
Can_Rx_MsgBuff	UWB_Info_Ntf_Sup_bf;

DFPM_ExtDataInfo dfpm_exData;


static void BCU_11E_RecvCallback(uint8_t index);
static void ZONE_3AA_RecvCallback(uint8_t index);
static void ZONE_3B2RecvCallback(uint8_t index);
static void BCU_11E_TimeoutCallback(uint8_t index);
static void ZONE_13C_RecvCallback(uint8_t index);
static void IRKgroup1_30B_RecvCallback(uint8_t index);
static void IRKgroup1_30C_RecvCallback(uint8_t index);
static void UWB_Info_Ntf_RecvCallback(uint8_t index);
static void UWB_Info_Ntf_Sup_RecvCallback(uint8_t index);

static Can_Rx_Msg Rx_AppMsgs[] =
{
	{BCU_11E_CANID, 				BCU_11E_DLC,				 20,   1,0,	   150, 	 0, 	BCU_11E_RecvCallback, BCU_11E_TimeoutCallback,&BCU_11E_bf},
	//{BLE_SnifferParam_Dev1_CANID,   BLE_SnifferParam_Dev1_DLC,	100,   1,0,	100*10, 	 0, 	0, 0,	&BLE_SnifferParam_Dev1_bf},
	//{BLE_SnifferParam_Dev2_CANID,   BLE_SnifferParam_Dev2_DLC,	100,   1,0,	100*10, 	 0, 	0, 0,	&BLE_SnifferParam_Dev2_bf},
	//{BLE_SnifferParam_Dev3_CANID,   BLE_SnifferParam_Dev3_DLC,	100,   1,0,	100*10, 	 0, 	0, 0,	&BLE_SnifferParam_Dev3_bf},
	//{BLE_SnifferParam_Dev4_CANID,   BLE_SnifferParam_Dev4_DLC,	100,   1,0,	100*10, 	 0, 	0, 0,	&BLE_SnifferParam_Dev4_bf},
	//{BLE_SnifferParam_Dev5_CANID,   BLE_SnifferParam_Dev5_DLC,	100,   1,0,	100*10, 	 0, 	0, 0,	&BLE_SnifferParam_Dev5_bf},
	{Device_IRK_Group_1_CANID,   	Device_IRK_Group_1_DLC,		100,   1,0,	100*10, 	 0, 	IRKgroup1_30B_RecvCallback, 	0,		&Device_IRK_Group_1_bf},
	{Device_IRK_Group_2_CANID,   	Device_IRK_Group_2_DLC,		100,   1,0,	100*10, 	 0, 	IRKgroup1_30C_RecvCallback, 	0,		&Device_IRK_Group_2_bf},
	{VC_Data_5E0_CANID,   			VC_Data_5E0_DLC,			 50,   1,0,	 50*10, 	 0, 	0, 	0,		&VC_Data_5E0_bf},
	{ZONE_13C_CANID,   				ZONE_13C_DLC,				 20,   1,0,	 20*10, 	 0, 	ZONE_13C_RecvCallback, 	0,		&ZONE_13C_bf},
	{ZONE_296_CANID,   				ZONE_296_DLC,				100,   1,0,	100*10, 	 0, 	0, 	0,		&ZONE_296_bf},
	{ZONE_3AA_CANID,   				ZONE_3AA_DLC,				1000,  1,0,1000*10, 	 0, 	ZONE_3AA_RecvCallback, 	0,		&ZONE_3AA_bf},
	{ZONE_3B2_CANID,   				ZONE_3B2_DLC,				1000,  1,0,1000*10, 	 0, 	ZONE_3B2RecvCallback, 	0,		&ZONE_3B2_bf},
//	{BTA_SRR_FactoryReq_CANID,   	BTA_SRR_FactoryReq_DLC,		   0,  1,0,      0, 	 0, 	0, 	0,		&BTA_SRR_FactoryReq_bf},
//	{DIAG_REQ_AllECU_CANID,   		DIAG_REQ_AllECU_DLC,		   0,  1,0,      0, 	 0, 	0, 	0,		&DIAG_REQ_AllECU_bf},
//	{DIAG_REQ_BTA4_CANID,   		DIAG_REQ_BTA4_DLC,		  	   0,  1,0,      0, 	 0, 	0, 	0,		&DIAG_REQ_BTA4_bf},
//	{IotDownLinkData_CANID,   		IotDownLinkData_DLC,		   0,  1,0,      0, 	 0, 	0, 	0,		&IotDownLinkData_bf},
//	{IotScanConnectReq_CANID,   	IotScanConnectReq_DLC,		   0,  1,0,      0, 	 0, 	0, 	0,		&IotScanConnectReq_bf},
//	{IotUpLinkFlowCtrl_CANID,   	IotUpLinkFlowCtrl_DLC,		   0,  1,0,      0, 	 0, 	0, 	0,		&IotUpLinkFlowCtrl_bf},
	//{UWB_Info_Ntf_CANID,   			UWB_Info_Ntf_DLC,		   	   0,  1,0,      0, 	 0, 	UWB_Info_Ntf_RecvCallback, 	0,		&UWB_Info_Ntf_bf},
	//{UWB_Info_Ntf_Sup_CANID,   		UWB_Info_Ntf_Sup_DLC,		   0,  1,0,      0, 	 0, 	UWB_Info_Ntf_Sup_RecvCallback, 	0,		&UWB_Info_Ntf_Sup_bf},
	//{UWB_CAN_UCI_CMD_RECV_CAN_ID, UWB_CAN_UCI_CMD_DATA_DLC,	0,   1,0,	0, 	 0, 	uwbCan_UciCmdDataRecvedCallback, uwbCan_UciCmdDataRecvedTimeutCallback,	&uwbCan_UciMsgRxBuff},
	//{DV_TEST_UPPER_296_CAN_ID, DV_TEST_UPPER_296_DATA_DLC,	0,   1,0,	0, 	 0, 	uwbCan_CanMsg296RecvedCallback, NULL,	&canMsg296RxBuff},
	//{DV_TEST_SRF_1CC_CAN_ID, DV_TEST_SRF_1CC_DATA_DLC,	0,   1,0,	0, 	 0, 	uwbCan_CanMsg1ccRecvedCallback, NULL,	&canMsg1ccRxBuff},
	//{DV_TEST_SRF_1CA_CAN_ID, DV_TEST_SRF_1CA_DATA_DLC,	100,   1,0,	100*10, 	 0, 	uwbCan_CanMsg1caRecvedCallback, NULL,	&canMsg1caRxBuff}
};


static void BTA4_FusnData_PreSendCallback(uint8_t index)
{
	memcpy(BTA4_FusnData_bf.data, BTA4_FusnData_t.msg.data, BTA4_FusnData_DLC);
}
static void BTA4_RSSI_UWB_PreSendCallback(uint8_t index)
{
	memcpy(BTA4_RSSI_UWB_bf.data, BTA4_RSSI_UWB_t.msg.data, BTA4_RSSI_UWB_DLC);
}

static void BTA_SRR_297_PreSendCallback(uint8_t index)
{
	memcpy(BTA_SRR_297_bf.data, BTA_SRR_297_t.msg.data, BTA_SRR_297_DLC);
}
static void IotHubSts_PreSendCallback(uint8_t index)
{
	memcpy(IotHubSts_bf.data, IotHubSts_t.msg.data, IotHubSts_DLC);
}

static void BTA4_Log_Data_PreSendCallback(uint8_t index)
{
	memcpy(BTA4_Log_Data_bf.data, BTA4_Log_Data_t.msg.data, BTA4_Log_Data_DLC);
}
static void BTA_SRR_FactoryRsp_PreSendCallback(uint8_t index)
{
	memcpy(BTA_SRR_FactoryRsp_bf.data, BTA_SRR_FactoryRsp_t.msg.data, BTA_SRR_FactoryRsp_DLC);
}

//static void DIAG_RESP_BTA4_PreSendCallback(uint8_t index)
//{
//	memcpy(DIAG_RESP_BTA4_bf.data, DIAG_RESP_BTA4_t.msg.data, DIAG_RESP_BTA4_DLC);
//}
static void IotConnectSts_PreSendCallback(uint8_t index)
{
	memcpy(IotConnectSts_bf.data, IotConnectSts_t.msg.data, IotConnectSts_DLC);
}

static void IotDownLinkFlowCtrl_PreSendCallback(uint8_t index)
{
	memcpy(IotDownLinkFlowCtrl_bf.data, IotDownLinkFlowCtrl_t.msg.data, IotDownLinkFlowCtrl_DLC);
}
static void IotUplinkData_PreSendCallback(uint8_t index)
{
	memcpy(IotUplinkData_bf.data, IotUplinkData_t.msg.data, IotUplinkData_DLC);
}
static void sendMsgEngine(void)
{
	uint8_t i;
	for(i = 0; i < NR_TX_MSGS; i ++)
	{
		if(1U == Tx_AppMsgs[i].tx_enable)
		{
			if(Tx_AppMsgs[i].tx_FastFlag)
			{
				if(Tx_AppMsgs[i].tx_FastCount > 0)
				{
					Tx_AppMsgs[i].tx_count = Tx_AppMsgs[i].tx_FastCount;
					if(Tx_AppMsgs[i].tx_FastPeroid > 0)
					{
						Tx_AppMsgs[i].tx_CycleT = Tx_AppMsgs[i].tx_FastPeroid;
					}
					Tx_AppMsgs[i].tx_req  = 1;
					Tx_AppMsgs[i].tx_TimeoutT = canbusTimerCreate(Tx_AppMsgs[i].tx_CycleT);
				}
				Tx_AppMsgs[i].tx_FastFlag = 0;
			}

			if(Tx_AppMsgs[i].tx_CycleT > 0)
			{
				if(canbusTimerIsExpired(Tx_AppMsgs[i].tx_TimeoutT))
				{
					Tx_AppMsgs[i].tx_req  = 1;
					Tx_AppMsgs[i].tx_TimeoutT = canbusTimerCreate(Tx_AppMsgs[i].tx_CycleT);
				}
			}

			if(Tx_AppMsgs[i].tx_req)
			{
				if(0 != Tx_AppMsgs[i].PreCallBack)
				{
					OSA_DisableScheduler();
					(*Tx_AppMsgs[i].PreCallBack)(i);
					OSA_EnableScheduler();
				}

				if(CANBUS_SUCCESS == canbusTransSendMsgFrame((uint32_t)Tx_AppMsgs[i].id, (uint8_t)Tx_AppMsgs[i].len, Tx_AppMsgs[i].tx_buff))
	            {
					if(Tx_AppMsgs[i].tx_count > 0) Tx_AppMsgs[i].tx_count --;
					if(0 == Tx_AppMsgs[i].tx_count)
					{
						Tx_AppMsgs[i].tx_CycleT = Tx_AppMsgs[i].tx_Peroid;
					}

					if(0 != Tx_AppMsgs[i].LastCallBack)
					{
						(*Tx_AppMsgs[i].LastCallBack)(i);
					}
					Tx_AppMsgs[i].tx_req  = 0;
					Tx_AppMsgs[i].tx_TimeoutT = canbusTimerCreate(Tx_AppMsgs[i].tx_CycleT);
	            }
			}
		}
		else
		{
			Tx_AppMsgs[i].tx_FastFlag   = 0;
			Tx_AppMsgs[i].tx_FastCount  = 0;
			Tx_AppMsgs[i].tx_FastPeroid = 0;
		}
	}
}

static void IRKgroup1_30B_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(Device_IRK_Group_1_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		Device_IRK_Group_1_t.On_line_flag = 1;
		Device_IRK_Group_1_t.RecvMsgCnt ++;
	}
}
static void IRKgroup1_30C_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(Device_IRK_Group_2_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		Device_IRK_Group_2_t.On_line_flag = 1;
		Device_IRK_Group_2_t.RecvMsgCnt ++;
	}
}


static void BCU_11E_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(BCU_11E_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		BCU_11E_t.On_line_flag = 1;
		BCU_11E_t.RecvMsgCnt ++;
		Dtc_Recove(&DFC_11E_TimOut);
		/*TODO wait Tao's Interface*/
		if(BCU_11E_t.msg.sig.VehSpdSts_Rdnt1)
		{
			dfpm_exData.vehspd_H = BCU_11E_t.msg.sig.VehSpd_Rdnt1_phy_0;
			dfpm_exData.vehspd_L = BCU_11E_t.msg.sig.VehSpd_Rdnt1_phy_1;
		}else
		{
			dfpm_exData.vehspd_H = 0;
			dfpm_exData.vehspd_L = 0;
		}
	}
}

static void ZONE_3B2RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(ZONE_3B2_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		ZONE_3B2_t.On_line_flag = 1;
		ZONE_3B2_t.RecvMsgCnt ++;
		dfpm_exData.Timestamp_year = ZONE_3B2_t.msg.sig.Yr_phy;
		dfpm_exData.Timestamp_Month = ZONE_3B2_t.msg.sig.Mth;
		dfpm_exData.Timestamp_Day = ZONE_3B2_t.msg.sig.Day;
		dfpm_exData.Timestamp_Hour = ZONE_3B2_t.msg.sig.Hr;
		dfpm_exData.Timestamp_Minute = ZONE_3B2_t.msg.sig.Min;
		dfpm_exData.Timestamp_Second = ZONE_3B2_t.msg.sig.Sec;
	}
}

static void ZONE_3AA_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(ZONE_3AA_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		ZONE_3AA_t.On_line_flag = 1;
		ZONE_3AA_t.RecvMsgCnt ++;
		dfpm_exData.Odometer_value_h = ZONE_3AA_t.msg.sig.VehOdometer_0;
		dfpm_exData.Odometer_value_m = ZONE_3AA_t.msg.sig.VehOdometer_1;
		dfpm_exData.Odometer_value_l = ZONE_3AA_t.msg.sig.VehOdometer_2;
	}
}
static void ZONE_13C_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(ZONE_13C_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		ZONE_13C_t.On_line_flag = 1;
		ZONE_13C_t.RecvMsgCnt ++;

	}
}

static void UWB_Info_Ntf_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(UWB_Info_Ntf_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();
		UWB_Info_Ntf_t.DLC = Rx_AppMsgs[index].len;
		UWB_Info_Ntf_t.On_line_flag = 1;
		UWB_Info_Ntf_t.RecvMsgCnt ++;

	}
}

static void UWB_Info_Ntf_Sup_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(UWB_Info_Ntf_Sup_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		UWB_Info_Ntf_Sup_t.On_line_flag = 1;
		UWB_Info_Ntf_Sup_t.RecvMsgCnt ++;

	}
}


static void BCU_11E_TimeoutCallback(uint8_t index)
{
	//Dtc_Active(&DFC_11E_TimOut);
	BCU_11E_t.On_line_flag = 0;
}

void BUSOFF_MsgSend(void)
{
	BTA4_RSSI_UWB_PreSendCallback(0);
	canbusTransSendBusoffMsgFrame(BTA4_RSSI_UWB_CANID, BTA4_RSSI_UWB_DLC, BTA4_RSSI_UWB_bf.data);
}

static void recvMsgEngine(void)
{
	uint8_t i;
	for(i = 0; i < NR_RX_MSGS; i ++)
	{
		if(1U == Rx_AppMsgs[i].rx_enable)
		{
			if(Rx_AppMsgs[i].rx_buffer->rx_flag)
			{
				if(0 != Rx_AppMsgs[i].RecvCallBack)
				{
					NVIC_DisableIRQ(CAN0_IRQn);
					(*Rx_AppMsgs[i].RecvCallBack)(i);
					NVIC_EnableIRQ(CAN0_IRQn);
				}

				if(0 != Rx_AppMsgs[i].cycle)
				{
					Rx_AppMsgs[i].rx_CycleT = canbusTimerCreate(Rx_AppMsgs[i].rx_TimeoutT);
					Rx_AppMsgs[i].rx_online = 1;
				}
				Rx_AppMsgs[i].rx_buffer->rx_flag = 0;
			}
			else
			{
				if(Rx_AppMsgs[i].rx_online)
				{
					if(0 != Rx_AppMsgs[i].cycle)
					{
						if(canbusTimerIsExpired(Rx_AppMsgs[i].rx_CycleT))
						{
							if(0 != Rx_AppMsgs[i].TimeoutCallBack)
							{
                                                            if(!Get_canBusoffFlag())
                                                            {
								(*Rx_AppMsgs[i].TimeoutCallBack)(i);
                                                            }
							}
							Rx_AppMsgs[i].rx_online = 0;
						}
					}
				}
			}
		}
		else
		{
			Rx_AppMsgs[i].rx_buffer->rx_flag = 0;
		}
	}
}

uint8_t canbusTxMsgFast(uint32_t dwMsgID, uint8_t count, uint32_t peroid, uint8_t count_reset)
{
	uint8_t i, ret = CANBUS_FAIL;
	for(i = 0; i < NR_TX_MSGS; i ++)
	{
		if((uint32_t)Tx_AppMsgs[i].id == dwMsgID) break;
	}
	if(i < NR_TX_MSGS)
	{
		Tx_AppMsgs[i].tx_FastCount = count;
		Tx_AppMsgs[i].tx_FastPeroid = peroid;
		if(0 == Tx_AppMsgs[i].tx_count)
		{
			Tx_AppMsgs[i].tx_FastFlag = 1;
		}
		else
		{
			if(count_reset)
			{
				Tx_AppMsgs[i].tx_count = Tx_AppMsgs[i].tx_FastCount;
			}
		}

		ret = CANBUS_SUCCESS;
	}
	return ret;
}

uint8_t canbusTxMsgAble(uint32_t dwMsgID, uint8_t able)
{
	uint8_t i, ret = CANBUS_FAIL;
	for(i = 0; i < NR_TX_MSGS; i ++)
	{
		if((uint32_t)Tx_AppMsgs[i].id == dwMsgID) break;
	}
	if(i < NR_TX_MSGS)
	{
		Tx_AppMsgs[i].tx_enable = able;
		ret = CANBUS_SUCCESS;
	}
	return ret;
}

void canbusAppTxInit(void)
{

	for(uint8_t i = 0; i < NR_TX_MSGS; i ++)
	{
			Tx_AppMsgs[i].tx_req = 0;
			Tx_AppMsgs[i].tx_count = 0;
			Tx_AppMsgs[i].tx_CycleT = Tx_AppMsgs[i].tx_Peroid;
			Tx_AppMsgs[i].tx_TimeoutT = canbusTimerCreate(Tx_AppMsgs[i].tx_Offset);
			Tx_AppMsgs[i].tx_FastCount = 0;
			Tx_AppMsgs[i].tx_FastPeroid = 0;
	}
}

void canbusAppRxInit(void)
{
	for(uint8_t i = 0; i < NR_RX_MSGS; i ++)
	{
		Rx_AppMsgs[i].rx_buffer->rx_flag = 0;
		Rx_AppMsgs[i].rx_online = 0;
		if(0 != Rx_AppMsgs[i].TimeoutCallBack)
		{
			(*Rx_AppMsgs[i].TimeoutCallBack)(i);
		}
	}
}
void canbusServiceInit(void)
{
	canbusAppTxInit();
	canbusAppRxInit();
	canbusTransInit();
}

void canbusTxService(void)
{
	Nm_StateType nmStatePtr;
	Nm_ModeType nmModePtr;

	CanNm_GetState(0, &nmStatePtr, &nmModePtr);
	static uint8_t txflag = 0;
	if((nmModePtr == NM_NETWORK) && (Get_canBusoffFlag() == 0) && ( UdsApi_get_normal_tx_sta()) && (Get_PMState() == PMS_NORMAL) && (NM_FF_YES == CanNm_Get_first_frame(0)))
	{
		if(!txflag)
		{
			canbusAppTxInit();
			txflag = 1;
		}
		sendMsgEngine();
	}
	else
	{
		if(txflag)
		{

			txflag = 0;
		}
	}
}

void canbusRxService(void)
{
	Nm_StateType nmStatePtr;
	Nm_ModeType nmModePtr;
	static uint8_t rxflag = 0;
	CanNm_GetState(0, &nmStatePtr, &nmModePtr);
	if((nmModePtr != NM_BUS_SLEEP) && (UdsApi_get_normal_rx_sta()) && (Get_PMState() == PMS_NORMAL))
	{
		if(!rxflag)
		{
			canbusAppRxInit();
			rxflag = 1;
		}
		recvMsgEngine();
	}
	else
	{
		if(rxflag)
		{

			rxflag = 0;
		}
	}
}
