/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_rxnext.c
 * 摘要: Implements Uds_RxNext().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"

/*
 * 函数介绍: This function pops (and discards) the UDS session RX buffer at the head of the specified queue of buffers.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_RxNext( void )
{
	Uds_SessionCfg_t*	pUdsSessionCfg	= &Uds_SessionCfg;
    Uds_StatePtr8		pUdsQueuePos	= &( pUdsSessionCfg->pSessionRxQueuePos->ctUdsPos );
    Uds_StatePtr8		pQueueBufState  = (Uds_StatePtr8)(pUdsSessionCfg->pSessionRxQueueBufState + *pUdsQueuePos);/*MISRA 2004 Rule 11.4*//*lint !e928*/

    /* Does the current Uds session buffer of the specified channel contain a received message? */
    if( *pQueueBufState == (uint8_t)(UDS_RX_DATA) )
    {
        *pQueueBufState = (uint8_t)(UDS_TXRXFREE);

#if(UDS_SESSIONBUF_RX_NUM > 1u)
        /* Advance the current Uds CAN Rx queue position to the next entry in the queue,
         * wrapping if necessary. */
        if( *pUdsQueuePos != (uint8_t)(UDS_SESSIONBUF_RX_NUM - 0x01u) )
        {
            ++( *pUdsQueuePos );
        }else
        {
            *pUdsQueuePos = 0x00u;
        }
#endif
    }

    return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
