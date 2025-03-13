/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_getrxbuf.c
 * 摘要: mplements UdsCan_GetRxBuf().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "udscan_pub.h"
#include "udstp_conf.h"

/*
 * 函数介绍:
 * This function returns a pointer to the RX buffer at the head of the specified queue of buffers.
 * Buffers are pushed onto the tail of the queue when the CAN driver calls UdsCan_LData_RxIndication().
 *
 * Note that this function does not pop the buffer from the queue and that therefore a second call to
 * this function (with the same arguments) will return the same buffer.
 * 参数:
 *  * \param [in] sessionId    The ID of the session requesting an RX buffer.
 * 返回值: A pointer to the next available RX buffer, or 0 if no such buffer is available.
 * 备注:
 */
UdsCan_QueueBufPtr UDS_FN_TYPE UdsCan_GetRxBuf( uint8_t sessionId )
{
    UdsCan_QueueBufPtr pUdsCanBuf = NULL;

    UdsCan_SessionCfg_t* const	pCanSessionCfg	= UDSCAN_SESSIONCFG( sessionId );
    const uint8_t				ctUdsQueuePos	= pCanSessionCfg->pQueuePositions[ 0x00u ].ctUdsPos;	/*lint !e963*/

    /* Does the current Uds CAN buffer in the specified channel queue contain data to be received? */
    if( pCanSessionCfg->pQueueBufferStates[ ctUdsQueuePos ] == (uint8_t)(UDSCAN_RXDATA) )
    {
		pUdsCanBuf = (UdsCan_QueueBufPtr)( &(pCanSessionCfg->pQueueBuffers[ ctUdsQueuePos ]) );
    }
    else
    {

    }

    return pUdsCanBuf;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
