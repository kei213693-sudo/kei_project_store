/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_txnext.c
 * 摘要: Implements Implements Uds_TxNext().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"

/*
 * 函数介绍: This function performs two operations:
 *  1) It places a new, empty buffer onto the tail of the specified queue of TX buffers.
 *  2) It examines the buffer at the head of the specified queue of TX buffers. If this buffer is awaiting
 *     transmission, the function transmits the buffer.
 * 参数: The type of transmission(SERVICE_RESPONSE or SERVICE_CHANGEPARAM_REQ).
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_TxNext( const Uds_ServiceType_t txType )
{
#if(UDS_SESSIONBUF_TX_NUM > 0x01u)
    uint8_t                      		doTransmit      = 0x00u;
	Uds_SessionCfg_t*				pUdsSessionCfg	= &Uds_SessionCfg;
    Uds_SessionQueueBufPos_t* const pUdsQueuePos	= pUdsSessionCfg->pSessionTxQueuePos;
    Uds_StatePtr8					pQueueBufState  = (uint8_t*)(pUdsSessionCfg->pSessionTxQueueBufState + pUdsQueuePos->ctUdsPos);/*MISRA 2004 Rule 11.4*//*lint !e928*/
	Uds_SessionTxBuf_t*				pSessionTxBuf;
#else
	Uds_SessionCfg_t*				pUdsSessionCfg	= &Uds_SessionCfg;
    Uds_StatePtr8					pQueueBufState  = (uint8_t*)(pUdsSessionCfg->pSessionTxQueueBufState);/*MISRA 2004 Rule 11.4*//*lint !e928*/
	Uds_SessionTxBuf_t*				pSessionTxBuf;
#endif
	Std_UdsReturnType               serviceRespResult;

#if(UDS_SESSIONBUF_TX_NUM > 0x01u)
    /* While a buffer is waiting for transmission, we set its state to UDS_TX_RESPONSE_NOTSENT. */
    *pQueueBufState = (uint8_t)(UDS_TX_RESPONSE_NOTSENT);

    /* Advance the current Uds session Tx queue position to the next entry in the queue, wrapping if necessary. */
    if( pUdsQueuePos->ctUdsPos != (uint8_t)(UDS_SESSIONBUF_TX_NUM - 0x01u) )
    {
        ++( pUdsQueuePos->ctUdsPos );
    }else
    {
        pUdsQueuePos->ctUdsPos = 0x00u;
    }
    /* Is:
     *  - the entry at the current head position(ctUdsTpPos) still waiting to be sent to the transport layer,
     * If so, transmit the entry at the current head position(ctUdsTpPos) in the queue.
     *
     * We disable interrupts to make this function not re-entrant. */
    UDS_DISABLE_ALL_INTERRUPTS();

    pQueueBufState = (Uds_StatePtr8)(pUdsSessionCfg->pSessionTxQueueBufState + pUdsQueuePos->ctUdsTpPos);

	if( (uint8_t)(UDS_TX_RESPONSE_NOTSENT) == (*pQueueBufState) )
	{
#endif
        /* Change the buffer's state to UDS_TX_RESPONSE_INPROGRESS. */
        *pQueueBufState = (uint8_t)(UDS_TX_RESPONSE_INPROGRESS);

#if(UDS_SESSIONBUF_TX_NUM > 0x01u)
		doTransmit = 0x01u;
	}
    else
    {

    }

    UDS_ENABLE_ALL_INTERRUPTS();

	if( doTransmit )
    {
		pSessionTxBuf  = pUdsSessionCfg->pSessionTxBuffer + pUdsQueuePos->ctUdsTpPos;
#else
		pSessionTxBuf  = pUdsSessionCfg->pSessionTxBuffer;
#endif
		pSessionTxBuf->serverType = pUdsSessionCfg->pASdu_AI->serverType;
		pSessionTxBuf->tAType = TARGETADDR_PHYSICAL;
		pSessionTxBuf->sA = pUdsSessionCfg->pASdu_AI->sA;
		pSessionTxBuf->tA = pUdsSessionCfg->pASdu_AI->tA;
		pSessionTxBuf->rA = pUdsSessionCfg->pASdu_AI->rA;

		if( txType == SERVICE_RESPONSE )
		{/* SERVICE_RESPONSE, this is a UDS service response data to send. */
			serviceRespResult = Uds_ServiceResponse( pSessionTxBuf );
			if( UDS_E_OK == serviceRespResult )
			{/* We already dispatched the service response data to transport layer. */
			}else if( UDS_E_NOT_OK == serviceRespResult )
			{
				/* The AI configured in transport layer not matchs AI in the packet we are attenping to transmit.
				 * - Then we indicate to session layer to back to idle state.
				 * - Of course the packet is ignored both by transport layer and session layer buffer.
				 */
				Uds_SesnMgr_ResetSessionState();
			}else
			{/* serviceRespResult = UDS_E_PENDING */
				/* If the A_PDU response can't send this time due to transpport layer still busy sending last response
				 * we will not discard the response transmission and transmit it next time.
				 */
				 return;
			}
		}else if( txType == SERVICE_CHANGEPARAM_REQ )
		{/* SERVICE_CHANGEPARAM_REQ, this is a change parameter(STmin or BlockSize) to send. */
#ifdef UDS_ENABLE_CHANGEPARAMETER
			Uds_ChangeParamRequest( pSessionTxBuf );
#endif /* UDS_ENABLE_CHANGEPARAMETER */
		}else
		{/* Request type is not a valid transmit content, we will discard the transmit buffer directly. */
		}

		/* Set buffer to free which means We already dispatched the service response data or change parameter request data,
         * or request type is not a valid transmit content and we discard the transmit buffer directly.
		 */
			*pQueueBufState = (uint8_t)(UDS_TXRXFREE);

#if(UDS_SESSIONBUF_TX_NUM > 0x01u)
			/* Advance the current session Tx buffer in the queue for the channel, wrapping if necessary. */
			if( pUdsQueuePos->ctUdsTpPos != (uint8_t)(UDS_SESSIONBUF_TX_NUM - 0x01u) )
			{
				++( pUdsQueuePos->ctUdsTpPos );
			}else
			{
				/* Back to queue start */
				pUdsQueuePos->ctUdsTpPos = 0x00u;
		    }
    } /* end of if( doTransmit ) */
    else
    {

    }
#endif

    return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
