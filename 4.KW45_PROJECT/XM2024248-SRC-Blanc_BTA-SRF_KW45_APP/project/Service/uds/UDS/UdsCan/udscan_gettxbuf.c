/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_gettxbuf.c
 * 摘要: Implements UdsCan_GetTxBuf().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "udscan_pub.h"
#include "udstp_conf.h"

/*
 * 函数介绍:
 * This function returns a pointer to the TX buffer at the head of the specified queue of buffers.
 *
 * Note that this function does not modify the queue's state and that therefore a second call to
 * this function (with the same arguments) will return the same buffer.
 * 参数:
 * \param [in] sessionId    The ID of the session requesting an RX buffer.
 * 返回值: A pointer to the next available TX buffer, or 0 if no such buffer is available.
 * 备注:
 */
UdsCan_QueueBufPtr UDS_FN_TYPE UdsCan_GetTxBuf( uint8_t sessionId )
{
    UdsCan_QueueBufPtr pUdsCanBuf = NULL;

    UdsCan_SessionCfg_t* const 	pSessionCfg     = UDSCAN_SESSIONCFG( sessionId );
    const uint8_t				ctUdsQueuePos   = pSessionCfg->pQueuePositions[ 0x01u ].ctUdsPos;	/*lint !e963*/
    Uds_StatePtr8		 const	pQueueBufState  = pSessionCfg->pQueueBufferStates + ctUdsQueuePos;

    /* Is the current Uds CAN buffer in the specified channel queue free or already allocated?
     *
     * (Note that if this method is called twice for the same channel, with no intervening call to
     * UdsCan_TxNext(), we will allocate the same buffer twice.) */

    if( ( *pQueueBufState == (uint8_t)(UDSCAN_TXRXFREE) ) ||
        ( *pQueueBufState == (uint8_t)(UDSCAN_TXALLOC) ) )
    {
        *pQueueBufState = (uint8_t)(UDSCAN_TXALLOC);

        if ( 1U)
        {
        pUdsCanBuf = (UdsCan_QueueBufPtr)( &(pSessionCfg->pQueueBuffers[ ctUdsQueuePos ]) );
        }else{
            pUdsCanBuf = (UdsCan_QueueBufPtr)( &(pSessionCfg->pQueueBuffers[ 0 ]) );/*---------------- ?? ? ??  ---------------*/
        }
    }
    else
    {

    }

    return pUdsCanBuf;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
