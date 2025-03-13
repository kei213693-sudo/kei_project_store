/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_rxnext.c
 * 摘要: Implements UdsCan_RxNext().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udscan_pub.h"
#include "udstp_conf.h"

/*
 * 函数介绍: This function pops (and discards) the RX buffer at the head of the specified queue of buffers.
 * 参数:
 * \param [in] sessionId    The ID of the session whose RX buffer is to be poped and discarded.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsCan_RxNext( uint8_t sessionId )
{
    UdsCan_SessionCfg_t* const	pSessionCfg     = UDSCAN_SESSIONCFG( sessionId );
    Uds_StatePtr8				pUdsCanQueuePos = &( pSessionCfg->pQueuePositions[ 0x00u ].ctUdsPos );
    Uds_StatePtr8				pQueueBufState  = pSessionCfg->pQueueBufferStates + *pUdsCanQueuePos;

    /* Does the current Uds CAN buffer of the specified channel contain a received message? */
    if( *pQueueBufState == (uint8_t)(UDSCAN_RXDATA) )
    {
        *pQueueBufState = (uint8_t)(UDSCAN_TXRXFREE);

        /* Advance the current Uds CAN Rx queue position to the next entry in the queue,
         * wrapping if necessary. */
        if( *pUdsCanQueuePos != pSessionCfg->pChannelCfgs[ 0x00u ].idxEnd )
        {
            ++( *pUdsCanQueuePos );
        }
        else
        {
            *pUdsCanQueuePos = pSessionCfg->pChannelCfgs[ 0x00u ].idxStart;
        }
    }
    else
    {

    }

    return;
} /* end of UdsCan_RxNext() */

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
