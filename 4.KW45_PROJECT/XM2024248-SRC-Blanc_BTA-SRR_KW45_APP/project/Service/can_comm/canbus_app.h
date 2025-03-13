/*
***************************************************************************************
* FileName:  canbus_app.h
* Description:
* Date:
***************************************************************************************
*/
#ifndef CANBUS_APP_H__
#define CANBUS_APP_H__
#include "Dtc_Manage.h"

typedef void (*CanTxCallackPtr)(uint8_t);

typedef struct Can_Tx_MsgTag
{
	uint16_t id;		   		    /* 发送报文ID */
	uint8_t len;		   		    /* 发送报文长度 */

	uint8_t tx_req;
	uint8_t tx_count;
	uint8_t tx_enable;              /* 发送使能 */

	uint32_t tx_TimeoutT;
	uint32_t tx_CycleT;
	uint8_t	tx_FastFlag;

	uint32_t tx_Offset;				/* 发送周期偏移:ms */
	uint32_t tx_Peroid;       		/* 发送周期:ms */

	uint8_t tx_FastCount;          	/* 快发次数 */
	uint32_t tx_FastPeroid;   		/* 快发周期:ms */

	CanTxCallackPtr	PreCallBack;  	/* 发送报文前回调 */
	CanTxCallackPtr LastCallBack;	/* 成功发送报文后回调 */
	uint8_t *tx_buff;             	/* 报文发送对应buff */
}Can_Tx_Msg;

typedef void (*CanRxCallackPtr)(uint8_t);

typedef struct Can_Rx_MsgBuffTag
{
	uint8_t rx_flag;                /* 接收标记 */
	uint8_t rx_len;		   		    /* 接收报文长度 */
	uint8_t rx_buff[64];            /* 接收报文对应buff */
}Can_Rx_MsgBuff;

typedef struct Can_Rx_MsgTag
{
	uint16_t id;		   		    /* 报文ID */
	uint8_t len;		   		    /* 报文长度 */
	uint16_t cycle;

	uint8_t rx_enable;              /* 接收使能 */

	uint8_t rx_online;
	uint32_t rx_TimeoutT;
	uint32_t rx_CycleT;

	CanRxCallackPtr	RecvCallBack;  	/* 报文接收回调 */
	CanRxCallackPtr TimeoutCallBack;/* 报文接收超时后回调 */
	Can_Rx_MsgBuff *rx_buffer;		/* 接收buff */
}Can_Rx_Msg;

extern Can_Rx_MsgBuff	BCU_11E_bf;
extern Can_Rx_MsgBuff	BLE_SnifferParam_Dev1_bf;
extern Can_Rx_MsgBuff	BLE_SnifferParam_Dev2_bf;
extern Can_Rx_MsgBuff	BLE_SnifferParam_Dev3_bf;
extern Can_Rx_MsgBuff	BLE_SnifferParam_Dev4_bf;
extern Can_Rx_MsgBuff	BLE_SnifferParam_Dev5_bf;
extern Can_Rx_MsgBuff	Device_IRK_Group_1_bf;
extern Can_Rx_MsgBuff	Device_IRK_Group_2_bf;
extern Can_Rx_MsgBuff	VC_Data_5E0_bf;
extern Can_Rx_MsgBuff	ZONE_13C_bf;
extern Can_Rx_MsgBuff	ZONE_296_bf;
extern Can_Rx_MsgBuff	ZONE_3AA_bf;
extern Can_Rx_MsgBuff	ZONE_3B2_bf;
extern Can_Rx_MsgBuff	BTA_SRR_FactoryReq_bf;
extern Can_Rx_MsgBuff	DIAG_REQ_AllECU_bf;
extern Can_Rx_MsgBuff	DIAG_REQ_BTA4_bf;
extern Can_Rx_MsgBuff	IotDownLinkData_bf;
extern Can_Rx_MsgBuff	IotScanConnectReq_bf;
extern Can_Rx_MsgBuff	IotUpLinkFlowCtrl_bf;
extern Can_Rx_MsgBuff	UWB_Info_Ntf_bf;
extern Can_Rx_MsgBuff	UWB_Info_Ntf_Sup_bf;

extern DFPM_ExtDataInfo dfpm_exData;

void canbusServiceInit(void);

void canbusTxService(void);

void canbusRxService(void);

void BUSOFF_MsgSend(void);

uint8_t canbusTxMsgFast(uint32_t dwMsgID, uint8_t count, uint32_t peroid, uint8_t count_reset);
#endif







