/*
 * bleMsg_par.h
 *
 *  Created on: 2024年6月16日
 *      Author: admin
 */

#ifndef DRV_BLE_BLEMESSAGE_BLEMSG_PAR_H_
#define DRV_BLE_BLEMESSAGE_BLEMSG_PAR_H_

#include "bleMessage_cfg.h"

//extern appDKMessage_t appDKMsg;
/*extern void * bleMsgTxPtr[enum_bleMsgTx_total_num];*/
extern uint16_t bleMsgTxLen[enum_bleMsgTx_total_num];
extern const uint8_t bleMsgTxHandle[enum_bleMsgTx_total_num][3];
extern const ApplIndicationFunc bleTx_PreHandleFuncPtr[enum_bleMsgTx_total_num];
extern const ApplIndicationFunc bleTx_ConfirmFuncPtr[enum_bleMsgTx_total_num];


/*extern void * bleMsgRxPtr[enum_bleMsgRx_total_num];*/
extern uint16_t bleMsgRxLen[enum_bleMsg_total_num];
extern const uint8_t bleMsgRxHandle[enum_bleMsg_total_num][3];
extern const ApplIndicationFunc bleRx_PreHandleFuncPtr[enum_bleMsg_total_num];
extern const ApplIndicationFunc bleRx_IndAppFuncPtr[enum_bleMsg_total_num];

#endif /* DRV_BLE_BLEMESSAGE_BLEMSG_PAR_H_ */
