/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_aborttxrequest.c
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "udscan_pub.h"
#include "udstp_conf.h"
#include "udscan_inf.h"

/*
 * 函数介绍:
  * This function abort the ongoing transmission of a CAN message which is pending at the CAN
 * driver and not transmitted yet.
 *
 * Note that:
 *  - This function will first clear the pending buffer and mark it as UDSCAN_TXRXFREE.
 *  - This function will cann UdsCanApp_AbortTransmit() which is suppose to never fail on cancling of the pending
 *	  transmit request.
 * 参数:
 * \param [in] sessionId    The number of session which need to abort current message transmission.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsCan_AbortTxRequest( uint8_t sessionId )
{
    UdsCan_SessionCfg_t*  	pCanSessionCfg;
    Uds_StatePtr8         	pCtCanPos;
    Uds_StatePtr8         	pQueueBufState;
#if (UDS0_CAN_HW_TX_ABORT == 0x01u)
    UdsCan_QueueBuf_t*		pQueueBuffer;
	uint32_t				msgId;
#endif /* (UDS0_CAN_HW_TX_ABORT == 0x01u) */

	pCanSessionCfg  = UDSCAN_SESSIONCFG( sessionId );
	pCtCanPos       = &( pCanSessionCfg->pQueuePositions[ 0x01u ].ctCanPos );
    pQueueBufState  = pCanSessionCfg->pQueueBufferStates + *pCtCanPos;
#if (UDS0_CAN_HW_TX_ABORT == 0x01u)
	pQueueBuffer    = pCanSessionCfg->pQueueBuffers + *pCtCanPos;
	msgId			= pQueueBuffer->canID;
#endif /* (UDS0_CAN_HW_TX_ABORT == 0x01u) */

	/* Set buffer to free to discard the buffer. */
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

#if (UDS0_CAN_HW_TX_ABORT == 0x01u)
	/* Call the CAN driver to abort the pending transmission request. */
    UdsCanApp_AbortTransmit( msgId );
#endif /* (UDS0_CAN_HW_TX_ABORT == 0x01u) */

    return;
} /* end of UdsCan_AbortTxRequest() */

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
