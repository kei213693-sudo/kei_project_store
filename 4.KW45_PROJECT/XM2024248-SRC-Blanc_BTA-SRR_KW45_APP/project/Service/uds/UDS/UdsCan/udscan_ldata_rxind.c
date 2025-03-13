/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_ldata_rxind.c
 * 摘要:  Implements UdsCan_LData_RxIndication().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udscan_callbacks.h"
#include "udstp_conf.h"


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
 * \param [in] msgId        The CAN message ID of the received CAN message.
 * \param [in] msgLen       The length of the received CAN message.
 * \param [in] pMsgData     The received CAN message. The caller can discard this after the function returns.
 * 返回值: {*}
 * 备注:
 */
void UdsCan_LData_RxIndication(
	uint32_t msgId,
	uint8_t msgLen,
	uint8_t* pMsgData
)
{
    Uds_StatePtr8         	pCtCanPos;
    uint8_t                 	sessionId;
    Uds_StatePtr8         	pQueueBufState;
    UdsCan_SessionCfg_t*  	pCanSessionCfg;

    /*Check whether the pointer is empty*/
    if (NULL == pMsgData)
    {
        return;
    }
    else
    {
        /* code */
    }

    /* Search all CAN channels for all sessions to try to identify the CAN channel to which the RX message belongs. */

    for( sessionId = 0x00u; sessionId < UDSTP_NUM_SESSIONS; ++sessionId )
	{
		pCanSessionCfg = UDSCAN_SESSIONCFG( sessionId );

		/* Is the CAN message is a physical addressing TA type(point-to-point) for this session? */
		if( (msgId == pCanSessionCfg->pChannelCfgs[ 0x00u ].msgIdPhy) ||
		/* Is the CAN message is a functional addressing TA type(broadcast) for this session? */
			(msgId == pCanSessionCfg->pChannelCfgs[ 0x00u ].msgIdFnc))
		{
			break;
		}
		else
		{

		}
	}

    if( sessionId == UDSTP_NUM_SESSIONS )
    {
        /* The RX message is not associated with any of our CAN channels. */
        return; /* MISRA 2004 Rule 14.7 - Return statement before end of function *//*lint !e904*/
    }
    else
    {

    }
	/* If we reach this point, sessionId identify the session for which the RX message is intended. */

    pCtCanPos		= &( pCanSessionCfg->pQueuePositions[ 0x00u ].ctCanPos );
    pQueueBufState  = pCanSessionCfg->pQueueBufferStates + *pCtCanPos;

	/* Copy the received message to the current CAN buffer for the session Rx channel, if the current CAN buffer is free. */
    if( (uint8_t)(UDSCAN_TXRXFREE) == *pQueueBufState )
    {
        /* Copy msgLen bytes of data into the current CAN buffer for the Rx channel */
        Uds_MemCopy( (Uds_StatePtr8)pCanSessionCfg->pQueueBuffers[ *pCtCanPos ].msgBuffer, pMsgData, (uint32_t)(msgLen) ); /* MISRA 2004 Rule 11.4 *//*lint !e926*/ /*lint -e818*/

		pCanSessionCfg->pQueueBuffers[ *pCtCanPos ].canID	= msgId;
		pCanSessionCfg->pQueueBuffers[ *pCtCanPos ].dlc		= msgLen;

        /* Set current queue buffer to UDSCAN_RXDATA */
        *pQueueBufState = (uint8_t)(UDSCAN_RXDATA);

        /* Last buffer in queue? */
        if( *pCtCanPos != pCanSessionCfg->pChannelCfgs[ 0x00u ].idxEnd )
        {
            /* next queue buffer */
            ++( *pCtCanPos );
        }
        else
        {
            /* Back to queue start */
            *pCtCanPos = pCanSessionCfg->pChannelCfgs[ 0x00u ].idxStart;
        }
    }
    else
    {

    }

    return;
}

/********************************************************************************
*								end of file										*
********************************************************************************/
