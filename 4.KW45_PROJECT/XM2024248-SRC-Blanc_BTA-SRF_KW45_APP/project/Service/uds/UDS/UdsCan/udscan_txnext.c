/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_txnext.c
 * 摘要: Implements functions for transmitting CAN messages.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udscan_pub.h"
#include "udstp_conf.h"
#include "udscan_inf.h"
#include "typedefs.h"
#include "uds_api.h"

/*
 * 函数介绍:
 * This function transmits a CAN message containing the buffer which is at the head of a specified queue of buffers.
 *
 * The buffer is question is assumed to have the state UDSCAN_TXINPROGRESS.
 * 参数:
 * \param [in] sessionId    Transmit the buffer at the head of the Tx queue for this session.
 * 返回值: {*}
 * 备注:
 * *  - This function is not allowed to discard the CAN message buffer after UdsCan_LData_TxConfirmation() returns.
 *  - This function is not allowed to change the buffer state to UDSCAN_TXRXFREE which shall be changed in
 *    UdsCan_LData_TxConfirmation().
 */
static void UDS_FN_TYPE UdsCan_DoTransmit( uint8_t sessionId )
{
    UdsCan_SessionCfg_t* const pSessionCfg     = UDSCAN_SESSIONCFG( sessionId );
    Uds_StatePtr8  const       pCanPos         = &( pSessionCfg->pQueuePositions[ 0x01u ].ctCanPos );
    #ifndef CANFD_TP_FLAG
    UdsCan_QueueBuf_t* const   pQueueBuffer    = pSessionCfg->pQueueBuffers + 0;//*pCanPos
    #endif
    #ifdef CANFD_TP_FLAG
    UdsCan_QueueBuf_t* const   pQueueBuffer    = pSessionCfg->pQueueBuffers + *pCanPos;
    #endif
        UdsApi_TXSendData(UdsCan0_ChannelCfgs[1].msgIdPhy, pQueueBuffer->dlc, pQueueBuffer->msgBuffer);
    /* transmit the CAN message in required buffer */
    //UdsCan_LData_TxRequest( pQueueBuffer->canID, pQueueBuffer->dlc, (Uds_StatePtr8)pQueueBuffer->msgBuffer ); /* MISRA 2004 Rule 11.4 *//*lint !e926*/
   //_delay_ms(10);

}

/*
 * 函数介绍:
 * This function performs three operations:
 *  1) It records the length of the buffer which is currently at the tail of the specified queue of TX buffers.
 *  2) It places a new, empty buffer onto the tail of the specified queue of TX buffers.
 *  3) It examines the buffer at the head of the specified queue of TX buffers. If this buffer is awaiting
 *     transmission, the function transmits the buffer.
 * 参数:
 * \param [in] sessionId    Transmit the buffer at the head of the Tx queue for this session.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsCan_TxNext( uint8_t sessionId )
{
    uint8_t                    doTransmit      = 0x00u;
    UdsCan_SessionCfg_t* const pSessionCfg     = UDSCAN_SESSIONCFG( sessionId );
    UdsCan_QueuePos_t*	 const pQueuePositions = pSessionCfg->pQueuePositions + 0x01u;
    UdsCan_ChannelCfg_t* const pChannelCfg     = pSessionCfg->pChannelCfgs + 0x01u;
    Uds_StatePtr8              pQueueBufState  = pSessionCfg->pQueueBufferStates + pQueuePositions->ctUdsPos;

    /* Mark the state of buffer as waiting for transmission (which contains data to transmit). */
    *pQueueBufState = (uint8_t)(UDSCAN_TXNOTSENT);

    /* Advance the current Uds CAN queue position to the next entry in the queue, wrapping if necessary. */
    if( pQueuePositions->ctUdsPos != pChannelCfg->idxEnd )
    {
        ++( pQueuePositions->ctUdsPos );
    }
    else
    {
        pQueuePositions->ctUdsPos = pChannelCfg->idxStart;
    }
    /* Is:
     *  - the entry at the current CAN position still waiting to be sent to the CAN driver,
     *  - and the CAN message object for the specified channel available for TX? (We assume that the CAN driver has no
     *    ability to buffer a message which is to be sent on a message object which is already in use).
     * If so, transmit the entry at the current CAN position in the queue.
     *
     * We disable interrupts to make this function not re-entrant, and to guard against interruption from
     * TX confirmation. */
    UDS_DISABLE_ALL_INTERRUPTS();

    pQueueBufState = pSessionCfg->pQueueBufferStates + pQueuePositions->ctCanPos;

    if( (uint8_t)(UDSCAN_TXNOTSENT) == (*pQueueBufState) )
	{
		/*
		 * We need to change the buffer's state so that it is != UDSCAN_TXNOTSENT in case UdsCan_LData_TxConfirmation()
		 * is called as soon as we call UdsCan_LData_TxRequest().
		 */
		*pQueueBufState = (uint8_t)(UDSCAN_TXINPROGRESS);

		doTransmit = 0x01u;
	}
    else
    {

    }
    UDS_ENABLE_ALL_INTERRUPTS();

	if( doTransmit )
	{
		UdsCan_DoTransmit( sessionId );
	}
	else
	{

	}
    return;
} /* end of UdsCan_TxNext() */

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
