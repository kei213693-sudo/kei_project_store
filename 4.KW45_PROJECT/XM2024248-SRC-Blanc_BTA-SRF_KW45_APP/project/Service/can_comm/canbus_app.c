/*
***************************************************************************************
* FileName:  canbus_app.c
* Description:
* Date:
***************************************************************************************
*/
#include <string.h>
#include "fsl_os_abstraction.h"
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"
#include "canbus_trans.h"
#include "canbus_app.h"
#include "autosar_nm.h"
#include "uds_api.h"
#include "DFP.h"
#include "Dtc_Manage.h"
#include "app_dtc.h"
#include "UWB_CAN.h"
#include "SysTimer.h"

#define canbusTimerCreate(t)	(SysTimerCreate(t))
#define canbusTimerIsExpired(t)	(SysTimerIsExpired(t))

#define NR_TX_MSGS (sizeof(Tx_AppMsgs)/sizeof(Can_Tx_Msg))

//tx
static BtaOutputRfData_un			BtaOutputRfData_bf;
static BtaOutputRfData1_un 			BtaOutputRfData1_bf;
static BLE_Common_Sts_un 			BLE_Common_Sts_bf;
static BLE_SnifferParam_Dev1_un 	BLE_SnifferParam_Dev1_bf;
static BLE_SnifferParam_Dev2_un 	BLE_SnifferParam_Dev2_bf;
static BLE_SnifferParam_Dev3_un 	BLE_SnifferParam_Dev3_bf;
static BLE_SnifferParam_Dev4_un 	BLE_SnifferParam_Dev4_bf;
static BLE_SnifferParam_Dev5_un 	BLE_SnifferParam_Dev5_bf;
static BTA1_Dev1_Sts_un 			BTA1_Dev1_Sts_bf;
static BTA1_Dev2_Sts_un 			BTA1_Dev2_Sts_bf;
static BTA1_Dev3_Sts_un 			BTA1_Dev3_Sts_bf;
static BTA1_Dev4_Sts_un 			BTA1_Dev4_Sts_bf;
static BTA1_Dev5_Sts_un 			BTA1_Dev5_Sts_bf;
static BTA1_RSSI_UWB_un 			BTA1_RSSI_UWB_bf;
static BTA_SRF_295_un 				BTA_SRF_295_bf;
static CccBleDkSts_un 				CccBleDkSts_bf;
static Device_IRK_Group_1_un 		Device_IRK_Group_1_bf;
static Device_IRK_Group_2_un 		Device_IRK_Group_2_bf;
static BTA1_01_un 					BTA1_01_bf;
static BTA_02_un 					BTA_02_bf;



static BLE_DK_RSP_un 				BLE_DK_RSP_bf;
static BLE_Downlink_FlowCtrl_un 	BLE_Downlink_FlowCtrl_bf;
static BLE_Uplink_Data_un 			BLE_Uplink_Data_bf;
static BTA1_Log_Data_un 			BTA1_Log_Data_bf;
static BTA_SRF_FactoryRsp_un 		BTA_SRF_FactoryRsp_bf;
static CccDkBleCmdFlowCtrl_un 		CccDkBleCmdFlowCtrl_bf;
static CccDkBleRsp_un 				CccDkBleRsp_bf;
//static DIAG_RESP_BTA1_un 			DIAG_RESP_BTA1_bf;
static UWB_Info_Ntf_un 				UWB_Info_Ntf_bf;
static UWB_Info_Ntf_Sup_un 			UWB_Info_Ntf_Sup_bf;


static void BtaOutputRfData_PreSendCallback(uint8_t index);
static void BtaOutputRfData1_PreSendCallback(uint8_t index);
static void BLE_Common_Sts_PreSendCallback(uint8_t index);
static void BLE_SnifferParam_Dev1_PreSendCallback(uint8_t index);
static void BLE_SnifferParam_Dev2_PreSendCallback(uint8_t index);
static void BLE_SnifferParam_Dev3_PreSendCallback(uint8_t index);
static void BLE_SnifferParam_Dev4_PreSendCallback(uint8_t index);
static void BLE_SnifferParam_Dev5_PreSendCallback(uint8_t index);
static void BTA1_Dev1_Sts_PreSendCallback(uint8_t index);
static void BTA1_Dev2_Sts_PreSendCallback(uint8_t index);
static void BTA1_Dev3_Sts_PreSendCallback(uint8_t index);
static void BTA1_Dev4_Sts_PreSendCallback(uint8_t index);
static void BTA1_Dev5_Sts_PreSendCallback(uint8_t index);
static void BTA1_RSSI_UWB_PreSendCallback(uint8_t index);
static void BTA_SRF_295_PreSendCallback(uint8_t index);
static void CccBleDkSts_PreSendCallback(uint8_t index);
static void Device_IRK_Group_1_PreSendCallback(uint8_t index);
static void Device_IRK_Group_2_PreSendCallback(uint8_t index);
static void BTA1_01_PreSendCallback(uint8_t index);
static void BTA_02_PreSendCallback(uint8_t index);
static void BLE_DK_RSP_PreSendCallback(uint8_t index);
static void BLE_Downlink_FlowCtrl_PreSendCallback(uint8_t index);
static void BLE_Uplink_Data_PreSendCallback(uint8_t index);
static void BTA1_Log_Data_PreSendCallback(uint8_t index);
static void BTA_SRF_FactoryRsp_PreSendCallback(uint8_t index);
static void CccDkBleCmdFlowCtrl_PreSendCallback(uint8_t index);
static void CccDkBleRsp_PreSendCallback(uint8_t index);
static void DIAG_RESP_BTA1_PreSendCallback(uint8_t index);
static void UWB_Info_Ntf_PreSendCallback(uint8_t index);
static void UWB_Info_Ntf_Sup_PreSendCallback(uint8_t index);

//tx
static Can_Tx_Msg Tx_AppMsgs[] =
{
	{BLE_Common_Sts_CANID,  	    BLE_Common_Sts_DLC,	         0, 0, 1, 0, 0, 0,   1, 50,      0, 0,   BLE_Common_Sts_PreSendCallback,	    	0, 	BLE_Common_Sts_bf.data},
	{BLE_SnifferParam_Dev1_CANID,  	BLE_SnifferParam_Dev1_DLC,	 0, 0, 1, 0, 0, 0,   2, 100,     0, 0,   BLE_SnifferParam_Dev1_PreSendCallback,	    0, 	BLE_SnifferParam_Dev1_bf.data},
	{BLE_SnifferParam_Dev2_CANID,  	BLE_SnifferParam_Dev2_DLC,	 0, 0, 1, 0, 0, 0,   3, 100,     0, 0,   BLE_SnifferParam_Dev2_PreSendCallback,	    0, 	BLE_SnifferParam_Dev2_bf.data},
	{BLE_SnifferParam_Dev3_CANID,  	BLE_SnifferParam_Dev3_DLC,	 0, 0, 1, 0, 0, 0,   4, 100,     0, 0,   BLE_SnifferParam_Dev3_PreSendCallback,	    0, 	BLE_SnifferParam_Dev3_bf.data},
	{BLE_SnifferParam_Dev4_CANID,  	BLE_SnifferParam_Dev4_DLC,	 0, 0, 1, 0, 0, 0,   5, 100,     0, 0,   BLE_SnifferParam_Dev4_PreSendCallback,	    0, 	BLE_SnifferParam_Dev4_bf.data},
	{BLE_SnifferParam_Dev5_CANID,  	BLE_SnifferParam_Dev5_DLC,	 0, 0, 1, 0, 0, 0,   6, 100,     0, 0,   BLE_SnifferParam_Dev5_PreSendCallback,	    0, 	BLE_SnifferParam_Dev5_bf.data},
	{BTA1_Dev1_Sts_CANID,  			BTA1_Dev1_Sts_DLC,	 		 0, 0, 1, 0, 0, 0,   7, 50,      0, 0,   BTA1_Dev1_Sts_PreSendCallback,	    		0, 	BTA1_Dev1_Sts_bf.data},
	{BTA1_Dev2_Sts_CANID,  			BTA1_Dev2_Sts_DLC,	 		 0, 0, 1, 0, 0, 0,   8, 50,      0, 0,   BTA1_Dev2_Sts_PreSendCallback,	   		    0, 	BTA1_Dev2_Sts_bf.data},
	{BTA1_Dev3_Sts_CANID,  			BTA1_Dev3_Sts_DLC,	 		 0, 0, 1, 0, 0, 0,   9, 50,      0, 0,   BTA1_Dev3_Sts_PreSendCallback,	   		    0, 	BTA1_Dev3_Sts_bf.data},
	{BTA1_Dev4_Sts_CANID,  			BTA1_Dev4_Sts_DLC,	 		 0, 0, 1, 0, 0, 0,  10, 50,      0, 0,   BTA1_Dev4_Sts_PreSendCallback,	    	    0, 	BTA1_Dev4_Sts_bf.data},
	{BTA1_Dev5_Sts_CANID,  			BTA1_Dev5_Sts_DLC,	 		 0, 0, 1, 0, 0, 0,  11, 50,      0, 0,   BTA1_Dev5_Sts_PreSendCallback,	    	    0, 	BTA1_Dev5_Sts_bf.data},
	{BTA1_RSSI_UWB_CANID,  			BTA1_RSSI_UWB_DLC,	 		 0, 0, 1, 0, 0, 0,  12, 50,      0, 0,   BTA1_RSSI_UWB_PreSendCallback,	    	    0, 	BTA1_RSSI_UWB_bf.data},
	{BTA_SRF_295_CANID,  			BTA_SRF_295_DLC,	 		 0, 0, 1, 0, 0, 0,  13, 100,     0, 0,   BTA_SRF_295_PreSendCallback,	    	    0, 	BTA_SRF_295_bf.data},
	{CccBleDkSts_CANID,  			CccBleDkSts_DLC,	 		 0, 0, 1, 0, 0, 0,  14, 50,      0, 0,   CccBleDkSts_PreSendCallback,	    	    0, 	CccBleDkSts_bf.data},
	{Device_IRK_Group_1_CANID,  	Device_IRK_Group_1_DLC,	 	 0, 0, 1, 0, 0, 0,  15, 100,     0, 0,   Device_IRK_Group_1_PreSendCallback,	    0, 	Device_IRK_Group_1_bf.data},
	{Device_IRK_Group_2_CANID,  	Device_IRK_Group_2_DLC,	 	 0, 0, 1, 0, 0, 0,  16, 100,     0, 0,   Device_IRK_Group_2_PreSendCallback,	    0, 	Device_IRK_Group_2_bf.data},
	{BTA1_01_CANID,  				BTA1_01_DLC,	 			 0, 0, 1, 0, 0, 0,  17, 200,     0, 0,   BTA1_01_PreSendCallback,	    			0, 	BTA1_01_bf.data},
	{BTA_02_CANID,  				BTA_02_DLC,	 				 0, 0, 1, 0, 0, 0,  18, 500,     0, 0,   BTA_02_PreSendCallback,	    			0, 	BTA_02_bf.data},
	{BLE_DK_RSP_CANID,  			BLE_DK_RSP_DLC,	 	 		 0, 0, 1, 0, 17, 0,  0, 0,   	 0, 0,   BLE_DK_RSP_PreSendCallback,	    		0, 	BLE_DK_RSP_bf.data},
	{BLE_Downlink_FlowCtrl_CANID,  	BLE_Downlink_FlowCtrl_DLC,	 0, 0, 1, 0, 18, 0,  0, 0,   	 0, 0,   BLE_Downlink_FlowCtrl_PreSendCallback,	    0, 	BLE_Downlink_FlowCtrl_bf.data},
	{BLE_Uplink_Data_CANID,  		BLE_Uplink_Data_DLC,		 0, 0, 1, 0, 19, 0,  0, 0,   	 0, 0,   BLE_Uplink_Data_PreSendCallback,	 	    0, 	BLE_Uplink_Data_bf.data},
	{BTA1_Log_Data_CANID,  			BTA1_Log_Data_DLC,			 0, 0, 1, 0, 20, 0,  0, 0,   	 0, 0,   BTA1_Log_Data_PreSendCallback,	 	    	0, 	BTA1_Log_Data_bf.data},
	{BTA_SRF_FactoryRsp_CANID,  	BTA_SRF_FactoryRsp_DLC,		 0, 0, 1, 0, 21, 0,  0, 0,   	 0, 0,   BTA_SRF_FactoryRsp_PreSendCallback,	 	0, 	BTA_SRF_FactoryRsp_bf.data},
	{BtaOutputRfData_CANID,  	    BtaOutputRfData_DLC,	     0, 0, 1, 0, 22, 0,  0, 0,       0, 0,   BtaOutputRfData_PreSendCallback,			0, 	BtaOutputRfData_bf.data},
	{BtaOutputRfData1_CANID,	    BtaOutputRfData1_DLC,	     0, 0, 1, 0, 23, 0,  0, 0,  	 0, 0,   BtaOutputRfData1_PreSendCallback, 			0, 	BtaOutputRfData1_bf.data},
	{CccDkBleCmdFlowCtrl_CANID,	    CccDkBleCmdFlowCtrl_DLC,	 0, 0, 1, 0, 24, 0,  0, 0,  	 0, 0,   CccDkBleCmdFlowCtrl_PreSendCallback, 		0, 	CccDkBleCmdFlowCtrl_bf.data},
	{CccDkBleRsp_CANID,	    		CccDkBleRsp_DLC,			 0, 0, 1, 0, 25, 0,  0, 0,  	 0, 0,   CccDkBleRsp_PreSendCallback, 				0, 	CccDkBleRsp_bf.data},
//	{DIAG_RESP_BTA1_CANID,	    	DIAG_RESP_BTA1_DLC,			 0, 0, 1, 0, 26, 0,  0, 0,  	 1, 0,   DIAG_RESP_BTA1_PreSendCallback, 			0, 	DIAG_RESP_BTA1_bf.data},
	{UWB_Info_Ntf_CANID,	    	UWB_Info_Ntf_DLC,			 0, 0, 1, 0, 27, 0,  0, 0,  	 0, 0,   UWB_Info_Ntf_PreSendCallback, 				0, 	UWB_Info_Ntf_bf.data},
	{UWB_Info_Ntf_Sup_CANID,	    UWB_Info_Ntf_Sup_DLC,		 0, 0, 1, 0, 28, 0,  0, 0,  	 0, 0,   UWB_Info_Ntf_Sup_PreSendCallback, 			0, 	UWB_Info_Ntf_Sup_bf.data},
	{UWB_CAN_UCI_RSP_SEND_CAN_ID,	UWB_CAN_UCI_RSP_DATA_DLC,	 0, 0, 1, 0, 0, 0,   0, 0,  	 0, 0,   uwbCan_UciRspDataPreSendCallback, 	NULL, 	uwbCan_UciMsgTxBuff}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define NR_RX_MSGS (sizeof(Rx_AppMsgs)/sizeof(Can_Rx_Msg))

Can_Rx_MsgBuff	ZONE_L_TPMS_01_481_bf;
Can_Rx_MsgBuff	BCU_11E_11E_bf;
Can_Rx_MsgBuff	BLE_DK_REQ_bf;
Can_Rx_MsgBuff	BLE_Downlink_Data_bf;
Can_Rx_MsgBuff	BLE_Uplink_FlowCtrl_bf;
Can_Rx_MsgBuff	BTA2_RSSI_UWB_bf;
Can_Rx_MsgBuff	BTA3_RSSI_UWB_bf;
Can_Rx_MsgBuff	BTA4_FusnData_bf;
Can_Rx_MsgBuff	BTA4_RSSI_UWB_bf;
Can_Rx_MsgBuff	BTA5_RSSI_UWB_bf;
Can_Rx_MsgBuff	BTA_SRF_FactoryReq_bf;
Can_Rx_MsgBuff	CccDkBleCmd_bf;
Can_Rx_MsgBuff	CccDkBleRspFlowCtrl_bf;
Can_Rx_MsgBuff	CccDkReq_bf;
Can_Rx_MsgBuff	DIAG_REQ_AllECU_bf;
Can_Rx_MsgBuff	DIAG_REQ_BTA1_bf;
Can_Rx_MsgBuff	Posn_Para_Set_bf;
Can_Rx_MsgBuff	VC_Data_5E0_bf;
Can_Rx_MsgBuff	ZONE_13C_bf;
Can_Rx_MsgBuff	ZONE_296_bf;
Can_Rx_MsgBuff	ZONE_3AA_bf;
Can_Rx_MsgBuff	ZONE_3B2_bf;
Can_Rx_MsgBuff	BTA6_RSSI_UWB_bf;
Can_Rx_MsgBuff	BTA7_RSSI_UWB_bf;



DFPM_ExtDataInfo dfpm_exData;

static void ZONE_L_TPMS_01_481_RecvCallback(uint8_t index);
static void ZONE_L_TPMS_01_481_TimeoutCallback(uint8_t index);
static void BCU_11E_11E_RecvCallback(uint8_t index);
static void BCU_11E_11E_TimeoutCallback(uint8_t index);
static void ZONE_3AA_RecvCallback(uint8_t index);
static void ZONE_13C_RecvCallback(uint8_t index);
static void ZONE_3B2RecvCallback(uint8_t index);
static void BLE_Downlink_Data_RecvCallback(uint8_t index);
static void BLE_Uplink_FlowCtrl_RecvCallback(uint8_t index);
static void VC_Data_5E0_RecvCallback(uint8_t index);
static void VC_Data_5E0_TimeoutCallback(uint8_t index);
static void ZONE_296_RecvCallback(uint8_t index);
static void ZONE_296_TimeoutCallback(uint8_t index);

static Can_Rx_Msg Rx_AppMsgs[] =
{
	{BCU_11E_CANID, BCU_11E_DLC,							20,   1,0,	  150, 	 0, 	BCU_11E_11E_RecvCallback, BCU_11E_11E_TimeoutCallback,	&BCU_11E_11E_bf},
	{BLE_DK_REQ_CANID, BLE_DK_REQ_DLC,		 				 0,   1,0,		0, 	 0, 	0,  						0,							&BLE_DK_REQ_bf},
	{BLE_Downlink_Data_CANID, BLE_Downlink_Data_DLC,		 0,   1,0,		0, 	 0, 	BLE_Downlink_Data_RecvCallback,	0,						&BLE_Downlink_Data_bf},
	{BLE_Uplink_FlowCtrl_CANID, BLE_Uplink_FlowCtrl_DLC,	 0,   1,0,		0, 	 0, 	BLE_Uplink_FlowCtrl_RecvCallback,0,						&BLE_Uplink_FlowCtrl_bf},
	{BTA2_RSSI_UWB_CANID, BTA2_RSSI_UWB_DLC,		 		50,   1,0,	50*10, 	 0, 	0,  						0,							&BTA2_RSSI_UWB_bf},
	{BTA3_RSSI_UWB_CANID, BTA3_RSSI_UWB_DLC,		 		50,   1,0,	50*10, 	 0, 	0,  						0,							&BTA3_RSSI_UWB_bf},
	{BTA4_FusnData_CANID, BTA4_FusnData_DLC,		 	   100,   1,0, 100*10,   0, 	0,  						0,							&BTA4_FusnData_bf},
	{BTA4_RSSI_UWB_CANID, BTA4_RSSI_UWB_DLC,		 		50,   1,0,	50*10,   0, 	0,  						0,							&BTA4_RSSI_UWB_bf},
	{BTA5_RSSI_UWB_CANID, BTA5_RSSI_UWB_DLC,		 		50,   1,0,	50*10,   0, 	0,  						0,							&BTA5_RSSI_UWB_bf},
	{BTA_SRF_FactoryReq_CANID, BTA_SRF_FactoryReq_DLC,		 0,   1,0,	    0,   0, 	0,  						0,							&BTA_SRF_FactoryReq_bf},
	{CccDkBleCmd_CANID, CccDkBleCmd_DLC,		 			 0,   1,0,	    0,   0, 	0,  						0,							&CccDkBleCmd_bf},
	{CccDkBleRspFlowCtrl_CANID, CccDkBleRspFlowCtrl_DLC,	 0,   1,0,	    0,   0, 	0,  						0,							&CccDkBleRspFlowCtrl_bf},
	{CccDkReq_CANID, CccDkReq_DLC,	 						50,   1,0,	50*10,   0, 	0,  						0,							&CccDkReq_bf},
	{DIAG_REQ_AllECU_CANID, DIAG_REQ_AllECU_DLC,	 		 0,   1,0,	    0,   0, 	0,  						0,							&DIAG_REQ_AllECU_bf},
	{DIAG_REQ_BTA1_CANID, DIAG_REQ_BTA1_DLC,	 		     0,   1,0,	    0,   0, 	0,  						0,							&DIAG_REQ_BTA1_bf},
	{Posn_Para_Set_CANID, Posn_Para_Set_DLC,	 		    20,   1,0,	20*10,   0, 	0,  						0,							&Posn_Para_Set_bf},
	{VC_Data_5E0_CANID, VC_Data_5E0_DLC,	 		    	50,   1,0,	50*10,   0, 	VC_Data_5E0_RecvCallback,  	VC_Data_5E0_TimeoutCallback,&VC_Data_5E0_bf},
	{ZONE_13C_CANID, ZONE_13C_DLC,	 		    			20,   1,0,	20*10,   0, 	ZONE_13C_RecvCallback,  	0,							&ZONE_13C_bf},
	{ZONE_296_CANID, ZONE_296_DLC,	 		    		   100,   1,0, 100*10,   0, 	ZONE_296_RecvCallback,  	ZONE_296_TimeoutCallback,&ZONE_296_bf},
	{ZONE_3AA_CANID, ZONE_3AA_DLC,	 		    		  1000,   1,0,1000*10,   0, 	ZONE_3AA_RecvCallback,  	0,							&ZONE_3AA_bf},
	{ZONE_3B2_CANID, ZONE_3B2_DLC,	 		    		  1000,   1,0,1000*10,   0, 	ZONE_3B2RecvCallback,  		0,							&ZONE_3B2_bf},
	{ZONE_L_TPMS_01_CANID, ZONE_L_TPMS_01_DLC,			   500,   1,0, 500*10, 	 0, 	ZONE_L_TPMS_01_481_RecvCallback, ZONE_L_TPMS_01_481_TimeoutCallback,	&ZONE_L_TPMS_01_481_bf},
	{BTA6_RSSI_UWB_CANID, BTA6_RSSI_UWB_DLC,			   50,    1,0, 50*10, 	 0, 	0, 							0,						&BTA6_RSSI_UWB_bf},
	{BTA7_RSSI_UWB_CANID, BTA7_RSSI_UWB_DLC,			    0,    1,0,     0, 	 0, 	0, 							0,						&BTA7_RSSI_UWB_bf},
	{UWB_CAN_UCI_CMD_RECV_CAN_ID, UWB_CAN_UCI_CMD_DATA_DLC,	500,   1,0,	500*10, 	 0, 	uwbCan_UciCmdDataRecvedCallback, uwbCan_UciCmdDataRecvedTimeutCallback,	&uwbCan_UciMsgRxBuff}
};

static void BLE_Common_Sts_PreSendCallback(uint8_t index)
{
	memcpy(BLE_Common_Sts_bf.data, BLE_Common_Sts_t.msg.data, BLE_Common_Sts_DLC);
}

static void BtaOutputRfData_PreSendCallback(uint8_t index)
{
	memcpy(BtaOutputRfData_bf.data, BtaOutputRfData_t.msg.data, BtaOutputRfData_DLC);
}

static void BtaOutputRfData1_PreSendCallback(uint8_t index)
{
	memcpy(BtaOutputRfData1_bf.data, BtaOutputRfData1_t.msg.data, BtaOutputRfData1_DLC);
}

static void BLE_SnifferParam_Dev1_PreSendCallback(uint8_t index)
{
	memcpy(BLE_SnifferParam_Dev1_bf.data, BLE_SnifferParam_Dev1_t.msg.data, BLE_SnifferParam_Dev1_DLC);
}

static void BLE_SnifferParam_Dev2_PreSendCallback(uint8_t index)
{
	memcpy(BLE_SnifferParam_Dev2_bf.data, BLE_SnifferParam_Dev2_t.msg.data, BLE_SnifferParam_Dev2_DLC);
}

static void BLE_SnifferParam_Dev3_PreSendCallback(uint8_t index)
{
	memcpy(BLE_SnifferParam_Dev3_bf.data, BLE_SnifferParam_Dev3_t.msg.data, BLE_SnifferParam_Dev3_DLC);
}

static void BLE_SnifferParam_Dev4_PreSendCallback(uint8_t index)
{
	memcpy(BLE_SnifferParam_Dev4_bf.data, BLE_SnifferParam_Dev4_t.msg.data, BLE_SnifferParam_Dev4_DLC);
}

static void BLE_SnifferParam_Dev5_PreSendCallback(uint8_t index)
{
	memcpy(BLE_SnifferParam_Dev5_bf.data, BLE_SnifferParam_Dev5_t.msg.data, BLE_SnifferParam_Dev5_DLC);
}

static void BTA1_Dev1_Sts_PreSendCallback(uint8_t index)
{
	memcpy(BTA1_Dev1_Sts_bf.data, BTA1_Dev1_Sts_t.msg.data, BTA1_Dev1_Sts_DLC);
}

static void BTA1_Dev2_Sts_PreSendCallback(uint8_t index)
{
	memcpy(BTA1_Dev2_Sts_bf.data, BTA1_Dev2_Sts_t.msg.data, BTA1_Dev2_Sts_DLC);
}

static void BTA1_Dev3_Sts_PreSendCallback(uint8_t index)
{
	memcpy(BTA1_Dev3_Sts_bf.data, BTA1_Dev3_Sts_t.msg.data, BTA1_Dev3_Sts_DLC);
}

static void BTA1_Dev4_Sts_PreSendCallback(uint8_t index)
{
	memcpy(BTA1_Dev4_Sts_bf.data, BTA1_Dev4_Sts_t.msg.data, BTA1_Dev4_Sts_DLC);
}

static void BTA1_Dev5_Sts_PreSendCallback(uint8_t index)
{
	memcpy(BTA1_Dev5_Sts_bf.data, BTA1_Dev5_Sts_t.msg.data, BTA1_Dev5_Sts_DLC);
}

static void BTA1_RSSI_UWB_PreSendCallback(uint8_t index)
{
	memcpy(BTA1_RSSI_UWB_bf.data, BTA1_RSSI_UWB_t.msg.data, BTA1_RSSI_UWB_DLC);
}

static void BTA_SRF_295_PreSendCallback(uint8_t index)
{
	memcpy(BTA_SRF_295_bf.data, BTA_SRF_295_t.msg.data, BTA_SRF_295_DLC);
}

static void CccBleDkSts_PreSendCallback(uint8_t index)
{
	memcpy(CccBleDkSts_bf.data, CccBleDkSts_t.msg.data, CccBleDkSts_DLC);
}

static void Device_IRK_Group_1_PreSendCallback(uint8_t index)
{
	memcpy(Device_IRK_Group_1_bf.data, Device_IRK_Group_1_t.msg.data, Device_IRK_Group_1_DLC);
}

static void Device_IRK_Group_2_PreSendCallback(uint8_t index)
{
	memcpy(Device_IRK_Group_2_bf.data, Device_IRK_Group_2_t.msg.data, Device_IRK_Group_2_DLC);
}

static void BTA1_01_PreSendCallback(uint8_t index)
{
	memcpy(BTA1_01_bf.data, BTA1_01_t.msg.data, BTA1_01_DLC);
}

static void BTA_02_PreSendCallback(uint8_t index)
{
	memcpy(BTA_02_bf.data, BTA_02_t.msg.data, BTA_02_DLC);
}

static void BLE_DK_RSP_PreSendCallback(uint8_t index)
{
	memcpy(BLE_DK_RSP_bf.data, BLE_DK_RSP_t.msg.data, BLE_DK_RSP_DLC);
}

static void BLE_Downlink_FlowCtrl_PreSendCallback(uint8_t index)
{
	memcpy(BLE_Downlink_FlowCtrl_bf.data, BLE_Downlink_FlowCtrl_t.msg.data, BLE_Downlink_FlowCtrl_DLC);
}

static void BLE_Uplink_Data_PreSendCallback(uint8_t index)
{
	memcpy(BLE_Uplink_Data_bf.data, BLE_Uplink_Data_t.msg.data, BLE_Uplink_Data_DLC);
}

static void BTA1_Log_Data_PreSendCallback(uint8_t index)
{
	memcpy(BTA1_Log_Data_bf.data, BTA1_Log_Data_t.msg.data, BTA1_Log_Data_DLC);
}

static void BTA_SRF_FactoryRsp_PreSendCallback(uint8_t index)
{
	memcpy(BTA_SRF_FactoryRsp_bf.data, BTA_SRF_FactoryRsp_t.msg.data, BTA_SRF_FactoryRsp_DLC);
}
static void CccDkBleCmdFlowCtrl_PreSendCallback(uint8_t index)
{
	memcpy(CccDkBleCmdFlowCtrl_bf.data, CccDkBleCmdFlowCtrl_t.msg.data, CccDkBleCmdFlowCtrl_DLC);
}

static void CccDkBleRsp_PreSendCallback(uint8_t index)
{
	memcpy(CccDkBleRsp_bf.data, CccDkBleRsp_t.msg.data, BTA_SRF_FactoryRsp_DLC);
}

//static void DIAG_RESP_BTA1_PreSendCallback(uint8_t index)
//{
//	memcpy(DIAG_RESP_BTA1_bf.data, DIAG_RESP_BTA1_t.msg.data, DIAG_RESP_BTA1_DLC);
//}

static void UWB_Info_Ntf_PreSendCallback(uint8_t index)
{
	memcpy(UWB_Info_Ntf_bf.data, UWB_Info_Ntf_t.msg.data, UWB_Info_Ntf_DLC);
}

static void UWB_Info_Ntf_Sup_PreSendCallback(uint8_t index)
{
	memcpy(UWB_Info_Ntf_Sup_bf.data, UWB_Info_Ntf_Sup_t.msg.data, UWB_Info_Ntf_Sup_DLC);
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

static void ZONE_L_TPMS_01_481_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(ZONE_L_TPMS_01_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		ZONE_L_TPMS_01_t.On_line_flag = 1;
		ZONE_L_TPMS_01_t.RecvMsgCnt ++;

		extern uint8_t RF_RecvTpmsIdFlag;
		RF_RecvTpmsIdFlag = 1;
	}
}

static void BCU_11E_11E_RecvCallback(uint8_t index)
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

static void ZONE_296_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(ZONE_296_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		ZONE_296_t.On_line_flag = 1;
		ZONE_296_t.RecvMsgCnt ++;
	}
}

static void VC_Data_5E0_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(VC_Data_5E0_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		OSA_InterruptEnable();

		VC_Data_5E0_t.On_line_flag = 1;
		VC_Data_5E0_t.RecvMsgCnt ++;
	}
}



static void ZONE_L_TPMS_01_481_TimeoutCallback(uint8_t index)
{
	ZONE_L_TPMS_01_t.On_line_flag = 0;
}

static void VC_Data_5E0_TimeoutCallback(uint8_t index)
{
	VC_Data_5E0_t.On_line_flag = 0;
}

static void BCU_11E_11E_TimeoutCallback(uint8_t index)
{
	//Dtc_Active(&DFC_11E_TimOut);
	BCU_11E_t.On_line_flag = 0;
}

static void ZONE_296_TimeoutCallback(uint8_t index)
{
	ZONE_296_t.On_line_flag = 0;
}

static void BLE_Downlink_Data_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		//OSA_InterruptDisable();
//		canTp_RxFromInterrupt(Rx_AppMsgs[index].len, Rx_AppMsgs[index].rx_buffer->rx_buff);
/*		memcpy(BLE_Downlink_Data_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);*/
		//OSA_InterruptEnable();

		BLE_Downlink_Data_t.On_line_flag = 1;
		BLE_Downlink_Data_t.RecvMsgCnt ++;
	}
}

static void BLE_Uplink_FlowCtrl_RecvCallback(uint8_t index)
{
	if(Rx_AppMsgs[index].rx_buffer->rx_len == Rx_AppMsgs[index].len)
	{
		OSA_InterruptDisable();
		memcpy(BLE_Uplink_FlowCtrl_t.msg.data, Rx_AppMsgs[index].rx_buffer->rx_buff, Rx_AppMsgs[index].len);
		/*TODO ?????????*/
		OSA_InterruptEnable();

		BLE_Uplink_FlowCtrl_t.On_line_flag = 1;
		BLE_Uplink_FlowCtrl_t.RecvMsgCnt ++;
	}
}


void BUSOFF_MsgSend(void)
{
	uint8_t nmData[8] = {0x70,0x40,0x04,0x00,0x00,0x00,0x00,0x00};
	canbusTransSendBusoffMsgFrame(NM_BTA1_CANID, NM_BTA1_DLC, nmData);
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
