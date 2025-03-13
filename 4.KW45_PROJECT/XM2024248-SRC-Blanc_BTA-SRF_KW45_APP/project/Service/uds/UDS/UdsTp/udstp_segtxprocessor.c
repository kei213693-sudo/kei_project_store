/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_segtxprocessor.c
 * 摘要: UDS network transport layer segmented data transmission processor.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udscan_pub.h"

/*
 * 函数介绍: This function control segmented data transmission.
 * 参数: \param [in] pTpSessionCfg    The transport layer session configure data for current sessionId.
 * \param [in] pTpSession       The transport layer session runtime data for current sessionId.
 * \param [in] sessionId        The index of the session which has pending transmit request.
 * \param [in] frameType        The type of frame that received.
 * \param [in] pRxPacket        The received packet.
 * \param [out] pTxPacket       The response packet.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_SegTxProc(
	UdsTp_SessionCfg_t* 	 pTpSessionCfg,
	UdsTp_Session_t*		 pTpSession,
	const uint8_t 			 sessionId,						/*lint !e963*/
	const UdsTp_FrameType_t	 frameType,						/*lint !e963*/
	const UdsCan_QueueBufPtr pRxPacket,						/*lint !e963*/
	UdsCan_QueueBufPtr		 pTxPacket
)
{
	if(NULL == pTpSessionCfg || NULL == pTpSession || NULL == pRxPacket || NULL == pTxPacket)
	{
		//return;
	}
	else
	{
		/* code */
	}

	UdsTp_NSduCfg_t*		pNSduCfg		= pTpSessionCfg->pNSduCfg;
#if (UDSTP_TIMING_ENABLE == TRUE)
	UdsTp_TxNTiming_t*		pTxNTiming		= (UdsTp_TxNTiming_t*)(&((++pNSduCfg)->timingData)); /* MISRA 2004 Rule 11.4 *//*lint !e929*/
#endif /* UDSTP_TIMING_ENABLE == TRUE */
	uint8_t 					handleResult	= 0x00u;
	uint16_t					remainBytes;
	uint16_t					tmp;

	switch( frameType )	/* (3-7) */
	{
		case FLOW_CONTROL_FRAME:
			/* We check one state of below three will be accepted as a valid reception of FC frame
			 * in case the client is responsing in a extremely short time, e.g. short time that the
			 * UdsTp_ServiceProc have not been have time to check the transmit confirmation yet. */
			if( (pTpSession->segTxCtrl == SEG_TX_WAIT_FLOWCONTROL) ||
				(pTpSession->segTxCtrl == SEG_TX_WAIT_SENT_FF_CON) ||
                ((pTpSession->segTxCtrl == SEG_TX_WAIT_SENT_CF_CON) && (pTpSession->remainConseFrame == 0x00u)) )
			{/* If this is a expected FC frame in correct order. */
				pTpSession->segTxCtrl = SEG_TX_FLOWCONTROL_RECEIVED;
			}else
			{
				/* Ignore the received frame and discard it from receive buffer. */
				( pTpSessionCfg->pRxNext )( sessionId );/** UdsTp_req 86 *//** UdsTp_req 88 */
			}
			break;
		case NO_FRAME: /* no frame received. */
			break;
		case SINGLE_FRAME:
		case FIRST_FRAME:
		case CONSECUTIVE_FRAME:
		case INVALID_FRAME:
		default:
			/* unexpected frame(SF, FF, CF, unknown) received. */
			/* Ignore the received frame and discard it from receive buffer. */
			if((pTpSession->segTxCtrl == SEG_TX_WAIT_SENT_CF_CON) && ( pTpSession->sentConfirmed == TRUE ))
			{
				/* do nothing */
			}
			else
			{
				( pTpSessionCfg->pRxNext )( sessionId );/** UdsTp_req 86 *//** UdsTp_req 88 */
			}

			break;
	} /* end of switch( frameType ) (3-7) */

	switch( pTpSession->segTxCtrl )		/* (5) */
	{/* Process segmented data transmission. */
		case SEG_TX_WAIT_SENT_FF_CON:	/* (5-1) */
			if( pTpSession->sentConfirmed == TRUE )
			{/* Confirmation of complete transmission of FF frame received. */
#if (UDSTP_TIMING_ENABLE == TRUE)
				UDSTP_TIMER_STOP(sessionId);	/* Stop the timer for N_As. */	/** UdsTp_req 76 */
				UDSTP_TIMER_START(sessionId, pTxNTiming->nBs);					/** UdsTp_req 78 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
				/* pTpSession->sentConfirmed will be cleared when FlowControl frame received. */
				/* tpState remains SEG_TX_IN_PROGRESS */
				pTpSession->segTxCtrl = SEG_TX_WAIT_FLOWCONTROL;
			}else
			{/* FF frame not confirmed to sent by CAN driver. */
				/* We continue to wait in this state. */
#if (UDSTP_TIMING_ENABLE == TRUE)
				if( (UDSTP_TIMER_ISPENDING(sessionId)) && (UDSTP_TIMER_ISTIMEOUT(sessionId)) ) 	/** UdsTp_req 76 */
				{/* N_As timing pending and N_As timeout occurs.*/
						/* Abort message transmission and issue N_USData.confirm(<N_Result> = N_TIMEOUT_A).*/
					UDSTP_TIMER_STOP(sessionId);
						( pTpSessionCfg->pTxAbortRequest )( sessionId );		/** UdsTp_req 82 */
						UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
								pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
								pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_TIMEOUT_A)); /** UdsTp_req 75 *//** UdsTp_req 82 */
						pTpSession->tpState = RX_TX_IDLE;
				}
				else
				{

				}
#endif /* UDSTP_TIMING_ENABLE == TRUE */
			}
			break;
		case SEG_TX_FLOWCONTROL_RECEIVED:	/* (5-2) */
			if( pTpSession->sentConfirmed == TRUE )
			{/* Confirmation of completion of CAN transmission already received means this is a expected
			  * reception of FC frame. */
#if (UDSTP_TIMING_ENABLE == TRUE)
				UDSTP_TIMER_STOP(sessionId);		/* Stop the timer for N_Bs. */		/** UdsTp_req 78 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
				pTpSession->sentConfirmed = FALSE;
				handleResult = UdsTp_GetFlowCtrlFrame( sessionId, pRxPacket );
				switch( handleResult )
				{
					case E_OK:/* Ready to start N_Cs timer countering down from value of received STmin. */
                        /* N_Cs timing for STmin will not be controlled by UDSTP_TIMING_ENABLE. */
						UDSTP_TIMER_START(sessionId, pTpSession->nPdu_TxSTmin);			/** UdsTp_req 80 */
						/* tpState remains SEG_TX_IN_PROGRESS */
						pTpSession->segTxCtrl = SEG_TX_SEND_CF_WITH_STMIN;
						break;
					case E_PENDING:/* FC_WAIT frame received, continue to wait for a new FlowControl N_PDU. */
#if (UDSTP_TIMING_ENABLE == TRUE)
						UDSTP_TIMER_START(sessionId, pTxNTiming->nBs);	/** UdsTp_req 53 *//** UdsTp_req 78 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
						pTpSession->sentConfirmed = TRUE;		/* dummy "UDS_TRUE" to make subsequent check of flow control frame identical. */
						/* tpState remains SEG_TX_IN_PROGRESS */
						pTpSession->segTxCtrl = SEG_TX_WAIT_FLOWCONTROL;
						break;
					case E_PROTOCOL_NOT_ALLOWED:
						/* Ignore received frame, abort the segment transmission, and make an N_USData.confirm
						 * servicecall(<N_Result>=N_ERROR).*/
						UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
													pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
													pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_ERROR));
						pTpSession->tpState = RX_TX_IDLE;
						break;
					case E_FLOWCONTROL_ERR:
						/* Unexpected recept order(FC received while received BS is ZERO), ignore received frame,
						 * abort the transmission of a segmented message. */
					case E_FLOWCONTROL_FS_ERR:
						/* FC_FS is overFlow or reserved, ignore received frame, abort the segment transmission. */
						/* already made an N_USData.confirm service call(<N_Result>=N_BUFFER_OVFLW/N_INVALID_FS) */
					default:
						pTpSession->tpState = RX_TX_IDLE;
						break;
				} /* end of switch( handleResult ) */
			}else
			{/* Order error due to still waiting for transmit confirmation. */
				/* Ignored received frame, abort the segment transmission, and make an N_USData.confirm
				 * servicecall(<N_Result>=N_ERROR).*/
				UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
								pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
								pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_ERROR));
				pTpSession->tpState = RX_TX_IDLE;
			}
			/* discard received frame from receive buffer. */
			( pTpSessionCfg->pRxNext )( sessionId );
			break;
		case SEG_TX_SEND_CF_WITH_STMIN:	/* (5-3) */
            if( (UDSTP_TIMER_ISPENDING(sessionId)) && (UDSTP_TIMER_ISTIMEOUT(sessionId)) )    /** UdsTp_req 67 */ /** UdsTp_req 80 */
            {/* N_Cs timeout, which means STmin seperation time arrived. */
                UdsTp_SendConsecutiveFrame( sessionId, pTxPacket );                           /** UdsTp_req 52 */
				UDSTP_TIMER_STOP(sessionId);	/* Stop timing for N_Cs, not controlled by UDSTP_TIMING_ENABLE.*/ /** UdsTp_req 80 */
#if (UDSTP_TIMING_ENABLE == TRUE)
				UDSTP_TIMER_START(sessionId, pTxNTiming->nAs);	/* Start timing for N_As. */  /** UdsTp_req 76 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
				pTpSession->segTxCtrl = SEG_TX_WAIT_SENT_CF_CON;
            }
			break;
		case SEG_TX_WAIT_SENT_CF_CON:	/* (5-4) */
			if( pTpSession->sentConfirmed == TRUE )
			{/* Last CF frame already sent. */
#if (UDSTP_TIMING_ENABLE == TRUE)
				UDSTP_TIMER_STOP(sessionId);	/* Stop the timer for N_As. */	/** UdsTp_req 76 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
				/* pTpSession->sentConfirmed will be cleared when FlowControl frame received. */
				if( pTpSession->remainConseFrame != 0x00u )
				{/* For blocked segmented transmission: Current block not finished transmission. */
				 /* For non blocked segmented transmission: Current segmemt not finished transmission. */
					/* N_Cs timing for STmin will not be controlled by UDSTP_TIMING_ENABLE. */
					UDSTP_TIMER_START(sessionId, pTpSession->nPdu_TxSTmin);			/** UdsTp_req 67 */ /** UdsTp_req 80 */
					/* tpState remains SEG_TX_IN_PROGRESS */
					pTpSession->segTxCtrl = SEG_TX_SEND_CF_WITH_STMIN;
				}else
				{/* For blocked segmented transmission: Current block finished transmission. */
				 /* For non blocked segmented transmission: Segmented data transmission finished. */
					if( pTpSession->flagZeroFlowCtrlReq == FALSE )
					{/* Segmented data transmission is seperated into blocks. */
						if( pTpSession->remainBlocks > 1u )
						{/* Still have more than one blocks to be transmited. */
#if (UDSTP_TIMING_ENABLE == TRUE)
							UDSTP_TIMER_START(sessionId, pTxNTiming->nBs);			/** UdsTp_req 78 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
							pTpSession->remainConseFrame = pTpSession->nPdu_TxBs;	/** UdsTp_req 60 */
							/* tpState remains SEG_TX_IN_PROGRESS */
							pTpSession->segTxCtrl = SEG_TX_WAIT_FLOWCONTROL;
						}else if( pTpSession->remainBlocks == 0x01u )
						{/* The last block to transmit. */
#if (UDSTP_TIMING_ENABLE == TRUE)
							UDSTP_TIMER_START(sessionId, pTxNTiming->nBs);			/** UdsTp_req 78 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
							remainBytes = pTpSession->nPdu_Length - pTpSession->currentBufPos;
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
							/* normal(fixed) addressing. */
							tmp = remainBytes % 0x07u;
							remainBytes = remainBytes / 0x07u;
#else
							/* extended or mixed addressing. */
							tmp = remainBytes % 0x06u;
							remainBytes = remainBytes / 0x06u;
#endif /* UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE */
							if( tmp )
							{
								pTpSession->remainConseFrame = remainBytes + 0x01u;
							}else
							{
								pTpSession->remainConseFrame = remainBytes;
							}
							/* tpState remains SEG_TX_IN_PROGRESS */
							pTpSession->segTxCtrl = SEG_TX_WAIT_FLOWCONTROL;
						}else
						{/* Segmented data transmission finished. */
							/* Make a N_USData.confirmation call to tell upper layer we finished
							 * transmission of the segmented data. */
							UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
											pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
											pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_OK)); /** UdsTp_req 19 */
							pTpSession->tpState = RX_TX_IDLE;
						}
					}else
					{/* Segmented data transmission can be finisheded by one block. */
						/* Make a N_USData.confirmation call to tell upper layer we finished
						 * transmission of the segmented data. */
						UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
											pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
											pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_OK)); /** UdsTp_req 19 */
						pTpSession->tpState = RX_TX_IDLE;
					}
				}
			}else
			{/* Last CF frame not yet sent, we need wait and keep timeout monitoring. */
#if (UDSTP_TIMING_ENABLE == TRUE)
				if( (UDSTP_TIMER_ISPENDING(sessionId)) && (UDSTP_TIMER_ISTIMEOUT(sessionId)) ) 	/** UdsTp_req 76 */
				{/* N_As timing pending and N_As timeout occurs. */
						/* Abort message transmission and issue N_USData.confirm(<N_Result> = N_TIMEOUT_A).*/
					UDSTP_TIMER_STOP(sessionId);
						( pTpSessionCfg->pTxAbortRequest )( sessionId );		/** UdsTp_req 82 */
						UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
								pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
								pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_TIMEOUT_A)); /** UdsTp_req 75 *//** UdsTp_req 82 */
						pTpSession->tpState = RX_TX_IDLE;
				}
				else
				{

				}
#endif /* UDSTP_TIMING_ENABLE == TRUE */
			}
			break;
		case SEG_TX_WAIT_FLOWCONTROL:	/* (5-5) */
			/* When FlowControl frame received above, we will run to state SEG_TX_FLOWCONTROL_RECEIVED */
#if (UDSTP_TIMING_ENABLE == TRUE)
			if( (UDSTP_TIMER_ISPENDING(sessionId)) && (UDSTP_TIMER_ISTIMEOUT(sessionId)) ) 		/** UdsTp_req 78 */
			{/* N_Bs timing pending and N_Bs timeout occurs. */
					/* Abort segment transmission and issue N_USData.confirm(<N_Result> = N_TIMEOUT_Bs).*/
				UDSTP_TIMER_STOP(sessionId);
					UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
							pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
							pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_TIMEOUT_BS)); /** UdsTp_req 75 *//** UdsTp_req 84 */
					pTpSession->tpState = RX_TX_IDLE;
			}
			else
			{

			}
#endif /* UDSTP_TIMING_ENABLE == TRUE */
			break;
		default:/* Unused switch, normal program execution will not come here. */
			break;
	} /* end of switch( pTpSession->segTxCtrl) */

	return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
