/*
 * bleTP.h
 *
 *  Created on: 2024年7月31日
 *      Author: F704012276_
 */

#ifndef BLE_BLETP_BLETP_H_
#define BLE_BLETP_BLETP_H_

#include "signals_api.h"
#include "drv_api.h"
#include "srv_api.h"
#include "bleTP_cfg.h"

#define INVALID_CAN_ID		0xFFFFFFFF
typedef enum{
	kL4_SingleFrame = 0x00,
	kL4_FirstFrame = 0x10,
	kL4_ConsecutiveFrame = 0x20,
	kL4_FlowControl = 0x30,
	kL4_FlowStatus_Wait = 0x31,
	kL4_FlowStatus_Overrun = 0x32,
} canTpType_t;

typedef enum{
	ble_SingleFrame = 0x00,
	ble_FirstFrame,
	ble_ConsecutiveFrame,
	ble_FlowControl,
	ble_FlowStatus_Wait,
	ble_FlowStatus_Overrun,
} bleTpType_t;

typedef enum
{
	enum_bleTx_Idle = 0,
	enum_bleTx_FF,
	enum_bleTx_SF,
	enum_bleTx_CF,
	enum_bleTx_LastCF,
	enum_bleTx_FC,
}bleTpTxStateEngine_t;

typedef enum
{
	enum_canTx_Idle = 0,
	enum_canTx_FF,			/* Send First frame */
	enum_canTx_SF,			/* Send single frame */
	enum_canTxWait_FC,		/* Receive flow control frame */
	enum_canTx_CF,			/* Send consecutive frame */
	enum_canTx_LastCF,		/* Send last consecutive frame */
}canTpTxStateEngine_t;

typedef enum
{
	enum_bleRx_Idle = 0,
	enum_bleRx_FF,
	enum_bleRx_SF,
	enum_bleRx_CF,
	enum_bleRx_WaitFC,
}bleTpRxStateEngine_t;

typedef enum
{
	enum_canRx_Idle = 0,
	enum_canRx_FF,			/* Receive First frame */
	enum_canRx_SF,			/* Receive single frame */
	enum_canRxSend_FC,		/* Send flow control frame */
	enum_canRxSend_FCWait,
	kRxState_WaitForFCOverConfIsr,
	enum_canRx_CF,			/* Receive consecutive frame */
	kRxState_ApplInformed,
	kRxState_CanFrameReceived,
}canTpRxStateEngine_t;

typedef struct
{
	uint8_t Timer;
	uint8_t BSCounter;
	canTpTxStateEngine_t engine;
	uint8_t sendReq:1;
	uint8_t blocked:1;
	uint8_t retransmit:1;
	uint8_t firstFC:1;
} bleTpTxState_t;

typedef struct
{
	uint16_t sequenceNum;
	bleTpRxStateEngine_t engine;
} bleTpRxState_t;

typedef struct
{
	uint8_t Timer;
	uint8_t BSCounter;
	uint8_t WFTCounter;
	canTpRxStateEngine_t engine;
	uint8_t sendReq:1;
	uint8_t retransmit:1;
	uint8_t noCopyData:1;
} canTpTxState_t;

typedef struct
{
	uint8_t Timer;
	uint8_t BSCounter;
	uint8_t WFTCounter;
	canTpRxStateEngine_t engine;
	uint8_t sendReq:1;
	uint8_t retransmit:1;
	uint8_t noCopyData:1;
} canTpRxState_t;

#endif /* BLE_BLETP_BLETP_H_ */
