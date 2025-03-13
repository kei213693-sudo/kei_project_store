/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_ldata_txcon.c
 * 摘要: Implements UdsCan_LData_TxConfirmation().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udscan_callbacks.h"
#include "udstp_conf.h"


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
 * 参数:
 * \param [in] msgId     		The CAN message ID which has successfully transmitted a message.
 * \param [in] transferStatus	The result of transfer request
 * 返回值: {*}
 * 备注:
 * - This function will discard the CAN message buffer which sent last message and reset buffer state only if
 *    transferStatus equals to ZERO which means transmit successfully finished.
 */
void UdsCan_LData_TxConfirmation( uint32_t msgId, uint8_t transferStatus )
{
    Uds_StatePtr8         	pCtCanPos;
    uint8_t                 sessionId;
    Uds_StatePtr8         	pQueueBufState;
    UdsCan_SessionCfg_t*  	pCanSessionCfg = NULL;


	/* we return without do anything in case the received confirmation with a failed status. */
	if( 0x00u != transferStatus )
	{
		return; /* MISRA 2004 Rule 14.7 - Return statement before end of function *//*lint !e904*/
	}
    else
    {

    }
    /* Search all CAN channels for all sessions to try to identify the CAN channel to which the RX message belongs. */

    for( sessionId = 0x00u; sessionId < UDSTP_NUM_SESSIONS; ++sessionId )
	{
		pCanSessionCfg = UDSCAN_SESSIONCFG( sessionId );

		/* Is the CAN message ID same as the physical addressing TA type(point-to-point) of TX direction for this session? */
		if( (msgId == pCanSessionCfg->pChannelCfgs[ 0x01u ].msgIdPhy) ||
		/* Is the CAN message ID same as the functional addressing TA type(broadcast) of TX direction for this session? */
			(msgId == pCanSessionCfg->pChannelCfgs[ 0x01u ].msgIdFnc))
		{
			break;
		}
		else
		{

		}
	}

    if( sessionId == UDSTP_NUM_SESSIONS )
    {
        /* The Tx callback from CAN drier given a message ID not associated with any of our CAN channels, ignored. */
        return; /* MISRA 2004 Rule 14.7 - Return statement before end of function *//*lint !e904*/
    }
	else
	{

	}
	/* If we reach this point, msgId identify the session for which the TX message sent. */
    pCtCanPos       = &( pCanSessionCfg->pQueuePositions[ 0x01u ].ctCanPos );
    pQueueBufState  = pCanSessionCfg->pQueueBufferStates + *pCtCanPos;

	/* Set buffer to free which means We got TX confirmation. */
    *pQueueBufState = (uint8_t)(UDSCAN_TXRXFREE);

	/* Advance the current CAN buffer in the queue for the channel, wrapping if necessary. */
    if( *pCtCanPos != pCanSessionCfg->pChannelCfgs[ 0x01u ].idxEnd )
    {
        ++( *pCtCanPos );
    }
    else
    {
		/* Back to queue start */
		*pCtCanPos = pCanSessionCfg->pChannelCfgs[ 0x01u ].idxStart;
    }

	/* Generate Tx confirmation to UdsTp module. */
    UDSCAN_SET_TRANSMITCONF( sessionId );
} /* end of UdsCan_LData_TxConfirmation */

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
