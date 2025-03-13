/*
 * bleTP_cfg.h
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#ifndef SRV_BLETP_BLETP_CFG_H_
#define SRV_BLETP_BLETP_CFG_H_

#include "signals_api.h"
#include "drv_api.h"
#include "srv_api.h"

#define BLE_MESSAGE_MALLOC_BUFF		200U

#define	BLE_CANTP_TX_FC_TIMEOUT 		50u		/* 发送完首帧/BS，到接收到流控帧的超时时间*/
#define	BLE_CANTP_RX_CF_TIMEOUT 		50u		/* 接收连续帧时超时 */
#define BLE_CANTP_RX_SEND_FC_DELAY		30u		/* 接收到首帧到发送流控帧之间的时间间隔 */
#define BLE_CANTP_RX_SEND_FC_DELAY1		30		/* 接收完BS到发送流控帧之间的时间间隔 */
#define BLE_CANTP_FAST_TIMEOUT			1u
#define BLE_CANTP_TX_SEND_CF_DELAY		10u		/* 接收到流控帧到开始发送连续帧的时间间隔 */

#define BLE_AIRTP_RX_TIMEOUT			100u

typedef enum
{
	CAN_PASSTHROUGH_BUF_IDLE = 0,
	CAN_PASSTHROUGH_BUF_BUSY,
}canTpBufSts_t;

typedef struct{
	uint8_t MAC_IRK[6];
	uint8_t MType;
	uint8_t MID;
	uint16_t len;
	uint8_t payloads[500];
}canPayloads_t;

typedef struct{
	union{
		uint8_t data[1504];
		struct{
			uint8_t MType;
			uint8_t MID;
			uint16_t len;
			uint8_t data[1500];
		}msg;
	}bytes;
}blePayloads_t;

typedef struct{
	uint8_t* DataBufferPtr;
	uint16_t DataIndex;
	uint16_t DataLength;
	uint8_t BlockSize;
	uint8_t STMin;
	uint8_t FFDataBuffer[6];
	uint8_t sequencenumber:4;
	uint8_t stuffbits1:4;
	uint8_t ApplGetBufferStatus:2;
	uint8_t withoutFC:1;
	uint8_t SNwait:1;
	uint8_t ta_type:1;
	uint8_t bufSts:1;
} canRxMsgInfo_t;

typedef struct{
	uint8_t bufSts;				/* transmit buffer status */
	uint16_t bufLen;			/* transmit length */
	uint8_t deviceId;
	uint16_t bufIndex;			/* transmit data index */
	uint8_t BSCounter;			/* the number frame of CF*/
	uint8_t STMin;
	uint8_t *buffPtr;
	uint16_t sequencenumber:4;	/* the index of BS counter */
	uint16_t CurrentTpCanDLC:12;	/* current tp can dlc*/
} bleTxMsgInfo_t;

typedef struct{
	uint8_t* DataBufferPtr;
	uint16_t DataIndex;
	uint16_t DataLength;
	uint8_t BlockSize;
	uint8_t STMin;
	uint8_t FFDataBuffer[6];
	uint8_t sequencenumber:4;
	uint8_t stuffbits1:4;
	uint8_t ApplGetBufferStatus:2;
	uint8_t withoutFC:1;
	uint8_t SNwait:1;
	uint8_t ta_type:1;
	uint8_t bufSts:1;
} bleRxMsgInfo_t;

/*typedef struct{
	uint8_t device_id;
} bleAppTp_t;*/

#endif /* SRV_BLETP_BLETP_CFG_H_ */
