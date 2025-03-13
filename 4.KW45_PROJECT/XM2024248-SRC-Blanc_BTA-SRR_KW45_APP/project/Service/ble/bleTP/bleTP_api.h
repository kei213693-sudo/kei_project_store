/*
 * bleTP_api.h
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#ifndef SRV_BLETP_BLETP_API_H_
#define SRV_BLETP_BLETP_API_H_

typedef enum {
	TpSuccess = 0,
	TpFailed,
	TpBusy,
	TpTxBufferUnderrun,
	TpCanTxFailed,
	TpTimeoutFC,
	TpErrTxFCNotExpected,
	TpErrTxWaitFrLimitExeeded,
	TpErrTxOverrun,
	TpErrTxFSInvalid
} TpReturnType;

void Tp_Init(void);
void CanTp_Init(void);
void BleTp_Init(void);
void Tp_Transmit(uint32_t id, bleMsgInfo_t *bleMsgInfo);
void Tp_Receive(bleMsgInfo_t *bleMsgInfo);
void CanTp_TxMainFunction(void);
void CanTp_RxMainFunction(void);
void BleTp_TxMainFunction(uint8_t deviceId);
void BleTp_RxMainFunction(uint8_t deviceId);

uint8_t BleTp_Transmit(bleMsgInfo_t *bleMsgInfo);
uint8_t CanTp_Transmit(uint32_t id, bleMsgInfo_t *bleMsgInfo);
void CanTpDrvConfirmation(uint32_t id);
void BleTpDrvConfirmation(uint8_t deviceId);

void CanTp_RxFromInterrupt(uint8_t dlc, const uint8_t *data);
void BleTp_RxFromInterrupt(uint16_t dlc, uint8_t deviceId, const uint8_t *data);
void CanTpRxIndication(void);
void BleTpRxIndication(void);

#endif /* SRV_BLETP_BLETP_API_H_ */
