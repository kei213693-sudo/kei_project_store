/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_gettxbuf.c
 * 摘要: Implements Uds_GetTxBuf().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"

/*
 * 函数介绍:  This function returns a pointer to the Session TX buffer at the tail of the specified queue of buffers.
 *
 * Note that this function does not modify the queue's state and that therefore a second call to
 * this function will return the same buffer.
 * 参数:
 * 返回值: A pointer to the next available TX buffer, or 0 if no such buffer is available.
 * 备注:
 */
Uds_SessionTxBufPtr UDS_FN_TYPE Uds_GetTxBuf( void )
{
	Uds_SessionCfg_t*	 pUdsSessionCfg	= &Uds_SessionCfg;
    const uint8_t			 ctUdsQueuePos	= pUdsSessionCfg->pSessionTxQueuePos->ctUdsPos;
    Uds_StatePtr8 const	 pQueueBufState = (Uds_StatePtr8)(pUdsSessionCfg->pSessionTxQueueBufState + ctUdsQueuePos);/*MISRA 2004 Rule 11.4*//*lint !e928*/

    /* Is the current Uds session buffer in the specified channel queue free or already allocated?
     *
     * (Note that if this method is called twice for the same channel, with no intervening call to
     * Uds_ServiceResponse() or Uds_ChangeParamRequest(), we will allocate the same buffer twice.) */

    if( ( *pQueueBufState == (uint8_t)(UDS_TXRXFREE) ) ||
        ( *pQueueBufState == (uint8_t)(UDS_TX_RESPONSE_ALLOC) ) )
    {
        *pQueueBufState = (uint8_t)(UDS_TX_RESPONSE_ALLOC);

        return (Uds_SessionTxBufPtr)( &(pUdsSessionCfg->pSessionTxBuffer[ ctUdsQueuePos ]) );/*MISRA 2004 Rule 14.7*//*lint !e904*//*MISRA 2004 Rule 11.4*//*lint !e929*/
    }

    return (NULL);
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
