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

#define UWB_CAN_UCI_CMD_RECV_CAN_ID	(0x781)
#if BASE_ID == 0
#define UWB_CAN_UCI_RSP_SEND_CAN_ID	(0x782)
#else
#define UWB_CAN_UCI_RSP_SEND_CAN_ID	(0x783)
#endif

#define UWB_CAN_UCI_CMD_DATA_DLC	(64)
#define UWB_CAN_UCI_RSP_DATA_DLC	(64)

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

void uwbCan_UciRspDataPreSendCallback(uint8_t index);

void uwbCan_UciCmdDataRecvedCallback(uint8_t index);

void uwbCan_UciCmdDataRecvedTimeutCallback(uint8_t index);

#endif /* CAN_COMM_UWB_CAN_H_ */
