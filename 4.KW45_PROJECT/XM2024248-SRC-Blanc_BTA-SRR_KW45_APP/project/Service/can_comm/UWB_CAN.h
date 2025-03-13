/*
 * UWB_CAN.h
 *
 *  Created on: 2024年7月23日
 *      Author: F704010638_
 */

#ifndef CAN_COMM_UWB_CAN_H_
#define CAN_COMM_UWB_CAN_H_
#include <stdint.h>
#include "canbus_app.h"
#define BASE_ID 0
//radio Test
#define UWB_CAN_UCI_CMD_RECV_CAN_ID	(0x781)
#if BASE_ID == 0
#define UWB_CAN_UCI_RSP_SEND_CAN_ID	(0x782)
#else
#define UWB_CAN_UCI_RSP_SEND_CAN_ID	(0x783)
#endif

#define UWB_CAN_UCI_CMD_DATA_DLC	(64u)
#define UWB_CAN_UCI_RSP_DATA_DLC	(64u)

//DV Test Receive
#define DV_TEST_UPPER_296_CAN_ID	(0x296)
#define DV_TEST_SRF_1CC_CAN_ID		(0x1cc)
#define DV_TEST_SRF_1CA_CAN_ID		(0x1ca)

#define DV_TEST_UPPER_296_DATA_DLC	(8u)
#define DV_TEST_SRF_1CC_DATA_DLC	(24u)
#define DV_TEST_SRF_1CA_DATA_DLC	(8u)

//DV Test Send
#define DV_TEST_SRF_215_CAN_ID		(0x215)
#define DV_TEST_SRF_216_CAN_ID		(0x216)
#define DV_TEST_SRF_20D_CAN_ID		(0x20D)

#define DV_TEST_SRF_215_DATA_DLC	(8u)
#define DV_TEST_SRF_216_DATA_DLC	(16u)
#define DV_TEST_SRF_20D_DATA_DLC	(8u)

typedef union
{
	uint8_t Byte[4];
	struct
	{
		uint8_t	gid	:4;
		uint8_t pbf	:1;
		uint8_t	msgType :3;
		uint8_t oid	:6;
		uint8_t rfu1 :2;
		uint8_t payloadLength_H;
		uint8_t payloadLength;
	}Bits;
}uciMsg_PackHead_t;

extern uint8_t uwbCan_UciMsgTxBuff[];
extern Can_Rx_MsgBuff uwbCan_UciMsgRxBuff;

extern uint8_t uwbCan_canMsg215TxBuff[];
extern uint8_t uwbCan_canMsg216TxBuff[];
extern uint8_t uwbCan_canMsg20dTxBuff[];
extern Can_Rx_MsgBuff	canMsg296RxBuff;
extern Can_Rx_MsgBuff	canMsg1ccRxBuff;
extern Can_Rx_MsgBuff	canMsg1caRxBuff;

typedef struct
{
	uint8_t newMsgRecv_flg;
	union
	{
		uint8_t Byte[8];
		struct
		{
			uint8_t lowPowerEnable;
			uint8_t cpdEnable;
		}sig;
	}msg;
}canMsg296_t;
extern canMsg296_t	canMsg296Rx;

typedef struct
{
	uint8_t newMsgRecv_flg;
	union
	{
		uint8_t Byte[24];
		struct
		{
			uint8_t scanCMD;
			uint8_t scanPara[6];
		}sig;
	}msg;
}canMsg1cc_t;
extern canMsg1cc_t	canMsg1ccRx;

typedef struct
{
	uint8_t newMsgRecv_flg;
	union
	{
		uint8_t Byte[8];
		struct
		{
			uint8_t rollingCode[4];
			uint8_t checkSum;
		}sig;
	}msg;
}canMsg1ca_t;
extern canMsg1ca_t	canMsg1caRx;

typedef struct
{
	uint8_t msgUpdata_flg;
	union
	{
		uint8_t Byte[8];
		struct
		{
			uint8_t rssiAndDisValid;
			uint8_t rssiValue;
			uint16_t disValue;
		}sig;
	}msg;
}canMsg215_t;
extern canMsg215_t	canMsg215Tx;

typedef struct
{
	uint8_t msgUpdata_flg;
	union
	{
		uint8_t Byte[16];
		struct
		{
			uint16_t cpdMotion;
			uint16_t cpdFreq;
			uint16_t amp_bfeature;
			uint16_t phs_bfeature;
			uint8_t cpdResult;
		}sig;
	}msg;
}canMsg216_t;
extern canMsg216_t	canMsg216Tx;

typedef struct
{
	uint8_t msgUpdata_flg;
	union
	{
		uint8_t Byte[8];
		struct
		{
			uint8_t srr_rx_checksum_error;
			uint8_t srr_rx_fail;
		}sig;
	}msg;
}canMsg20d_t;
extern canMsg20d_t	canMsg20dTx;

void uwbCan_UciRspDataPreSendCallback(uint8_t index);

void uwbCan_UciCmdDataRecvedCallback(uint8_t index);

void uwbCan_UciCmdDataRecvedTimeutCallback(uint8_t index);


void uwbCan_CanMsg215PreSendCallback(uint8_t index);

void uwbCan_CanMsg216PreSendCallback(uint8_t index);

void uwbCan_CanMsg20dPreSendCallback(uint8_t index);

void uwbCan_CanMsg296RecvedCallback(uint8_t index);

void uwbCan_CanMsg1ccRecvedCallback(uint8_t index);

void uwbCan_CanMsg1caRecvedCallback(uint8_t index);

void uwbCan_CanMsg1caRecvedTimeutCallback(uint8_t index);

#endif /* CAN_COMM_UWB_CAN_H_ */
