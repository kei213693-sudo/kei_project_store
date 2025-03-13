/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_getrxbuf.c
 * 摘要: Implements Uds_GetRxBuf().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "uds_target.h"
#include "uds_confpriv.h"

/**
 * This function returns a pointer to the Session RX buffer at the head of the specified queue of buffers.
 * Buffers are pushed onto the tail of the queue by the UdsTp module.
 *
 * Note that this function does not pop the buffer from the queue and that therefore a second call to
 * this function will return the same buffer.
 *
 * \return A pointer to the next available RX buffer, or 0 if no such buffer is available.
 */
Uds_SessionRxBufPtr UDS_FN_TYPE Uds_GetRxBuf( void )
{
	Uds_SessionCfg_t*	 pUdsSessionCfg	= &Uds_SessionCfg;
    const uint8_t			 ctUdsQueuePos	= pUdsSessionCfg->pSessionRxQueuePos->ctUdsPos;
	Uds_StatePtr8 const	 pQueueBufState = (Uds_StatePtr8)(pUdsSessionCfg->pSessionRxQueueBufState + ctUdsQueuePos);/*MISRA 2004 Rule 11.4*//*lint !e928*/

    /* Does the current Uds session buffer in the specified channel queue contain data to be received? */
    if( *pQueueBufState == (uint8_t)(UDS_RX_DATA) )
    {
        return (Uds_SessionRxBufPtr)( &(pUdsSessionCfg->pSessionRxBuffer[ ctUdsQueuePos ]) );/*MISRA 2004 Rule 14.7*//*lint !e904*//*MISRA 2004 Rule 11.4*//*lint !e929*/
    }

    return NULL;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
