/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_callbacks.h
 * 摘要: Declaration of callbacks implemented by the Data Link Layer Interface.
 * This file is covered by the licence and disclaimer document which is attached in
* the UDS software package.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDSCAN_CALLBACKS_H
#define _UDSCAN_CALLBACKS_H

#include "uds_target.h"

/*
 * 函数介绍:
 * This function is called by the CAN driver whenever a CAN message is successfully transmitted on a message object.
 *
 * Note that this function is called from within the CAN driver's TX confirmation interrupt handler and therefore
 * it cannot be re-entered or interrupted by transmission from the UDS software package.
 * It *can* be interrupted by UdsCan_LData_RxIndication().
 *
 * This function changes the buffer state which latestly sent message from UDSCAN_TXINPROGRESS to UDSCAN_TXRXFREE and
 * thereby freeing the buffer if the "transferStatus" equals to ZERO which means last transmit successfully finished.
 *
 * 参数:
 * *
 * \param [in] msgId     		  The CAN message ID which has successfully transmitted a message.
 * \param [in] transferStatus	The result of transfer request
 * 返回值: {*}
 * 备注:
 * *  - This function will discard the CAN message buffer which sent last message and reset buffer state only if
 *    transferStatus equals to ZERO which means transmit successfully finished.
 */
void UdsCan_LData_TxConfirmation( uint32_t msgId, uint8_t transferStatus );

/*
 * 函数介绍:
 * This function is called by the CAN driver whenever a CAN message is received which is associated
 * with the UDS software package.
 *
 * The function tries to assign the received message to the buffers maintained by the Uds CAN Data Link Layer Interface.
 * If this is not possible, the function silently discards the message.
 *
 * This function is called from the CAN driver's RX interrupt, so we do not need to worry about re-entrancy.
 * 参数:
 * * \param [in] msgId      The CAN message ID of the received CAN message.
 * \param [in] msgLen       The length of the received CAN message.
 * \param [in] pMsgData     The received CAN message. The caller can discard this after the function returns.
 * 返回值: {*}
 * 备注:
 */
void UdsCan_LData_RxIndication	( uint32_t msgId, uint8_t msgLen, uint8_t* pMsgData );

#endif /* _UDSCAN_CALLBACKS_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
