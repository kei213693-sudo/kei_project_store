/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_segrxprocessor.c
 * 摘要: UDS network transport layer segmented data reception processor.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udstp_inf.h"
#include "udscan_pub.h"

/*
 * 函数介绍: This function handles segmented data reception.
 * 参数: \param [in] pTpSessionCfg    The transport layer session configure data for current sessionId.
 * \param [in] pTpSession       The transport layer session runtime data for current sessionId.
 * \param [in] sessionId        The index of the session which has pending transmit request.
 * \param [in] frameType        The type of frame that received.
 * \param [in] pRxPacket        The received packet.
 * \param [out] pTxPacket       The response packet.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_SegRxProc(
	UdsTp_SessionCfg_t* 	 pTpSessionCfg,
	UdsTp_Session_t*		 pTpSession,
	const uint8_t 			 sessionId,						/*lint !e963*/
	const UdsTp_FrameType_t	 frameType,						/*lint !e963*/
	const UdsCan_QueueBufPtr pRxPacket,						/*lint !e963*/
	UdsCan_QueueBufPtr		 pTxPacket
)
{
	if (NULL == pTpSession || NULL == pTpSessionCfg)
	{
		//return ;
	}
	else
	{
	}

	UdsTp_NPduMsgData_t*	pPduMsgData	 = pTpSessionCfg->pNSduCfg->pNPdu_MsgData;
#if (UDSTP_TIMING_ENABLE == TRUE)
	UdsTp_RxNTiming_t*		pRxNTiming	 = (UdsTp_RxNTiming_t*)(&pTpSessionCfg->pNSduCfg->timingData);
#endif /* UDSTP_TIMING_ENABLE == TRUE */
	uint8_t                   handleResult = 0x00u;

	switch( frameType )
	{
		case SINGLE_FRAME:
			/* terminate current segment reception, report N_USData.indication(N_Result= N_UNEXP_PDU)
			 * to the upper layer, and process the SF N_PDU as the start of a new reception.*/
			/* set tpState to RX_TX_IDLE also discard PduMsgData and reset CF related variables */
			UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrClient,
					pTpSessionCfg->pNPdu_AI->addrServer, (uint8_t)(pTpSession->nTaType),
					pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_UNEXP_PDU), 0u, 0 ); /** UdsTp_req 10 *//** UdsTp_req 20 */
#if 0
			if( !pTxPacket )
			{
				( pTpSessionCfg->pTxAbortRequest )( sessionId );
			}
			else
			{
			}
#endif
			handleResult = UdsTp_GetSingleFrame( sessionId, pRxPacket );
			if( (handleResult == E_OK) || (handleResult == E_PROTOCOL_NOT_ALLOWED) )
			{/* Single frame already forwarded to upper layer or protocol error detected in the frame. */
				pTpSession->tpState = RX_TX_IDLE;
				/* Discard received frame from buffer since a successfully forward to upper layer
				 * for single frame, or due to a detected protocol format error in the frame. */
				( pTpSessionCfg->pRxNext )( sessionId );				/** UdsTp_req 88 */
			}else
			{/* handleResult = E_PENDING */
				/* Single frame is OK and kept in local buffer of transport layer, but
				 * can't forward to upper layer now. */
				pTpSession->pendingFrameType = SINGLE_FRAME;
				pTpSession->tpState = RX_IN_PROGRESS;
			}
			return;														/* MISRA 2004 Rule 14.7 *//*lint !e904*/
			break;														/*lint !e527*/
		case FIRST_FRAME:
			/* terminate current segment reception, report N_USData.indication(N_Result= N_UNEXP_PDU)
			 * to the upper layer, and process the FF N_PDU as the start of a new reception.*/
			UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrClient,
					pTpSessionCfg->pNPdu_AI->addrServer, (uint8_t)(pTpSession->nTaType),
					pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_UNEXP_PDU), 0x00u, 0x00u ); /** UdsTp_req 10 *//** UdsTp_req 20 */
#if 0
			if( !pTxPacket )
			{
				( pTpSessionCfg->pTxAbortRequest )( sessionId );
			}
			else
			{
			}
#endif
			pTpSession->tpState = SEG_RX_IN_PROGRESS;
			handleResult = UdsTp_GetFirstFrame( sessionId, pRxPacket, pTxPacket );
			switch( handleResult )
			{
				case E_OK:
					/* Segmented data is receivable, Flow control CTS sent. */
					pTpSession->segRxCtrl = SEG_RX_WAIT_SENT_FC_CTS_CON;
					/* Discard received frame from buffer due to a successful processing. */
					( pTpSessionCfg->pRxNext )( sessionId );	/** UdsTp_req 88 */
					break;
				case E_NOT_OK:
					/* Required segmented data size will exceed the available receive buffer. */
					pTpSession->segRxCtrl = SEG_RX_WAIT_SENT_FC_OVFLW_CON;
					/* Discard received frame from buffer. */
					( pTpSessionCfg->pRxNext )( sessionId );	/** UdsTp_req 88 */
					break;
				case E_PROTOCOL_NOT_ALLOWED:
					/* Protocol error detected in received first frame. */
					pTpSession->tpState = RX_TX_IDLE;
					/* Discard received frame from buffer due to a detected protocol format error. */
					( pTpSessionCfg->pRxNext )( sessionId );	/** UdsTp_req 88 */
					break;
				case E_PENDING:
					/* handleResult = E_PENDING */
					/* First frame data is not receivable for now */
					pTpSession->pendingFrameType = FIRST_FRAME;
					pTpSession->tpState = RX_IN_PROGRESS;
					break;
				default:
					break;
			} /* end of switch( handleResult ) */
			return;												/* MISRA 2004 Rule 14.7 *//*lint !e904*/
			break;												/*lint !e527*/
		case CONSECUTIVE_FRAME:
		case FLOW_CONTROL_FRAME:
		case INVALID_FRAME:
		case NO_FRAME:
		default:/* we will process other type of frames depending on current segmented Rx state. */
			break;
	} /* end of switch( frameType ) */

	switch( pTpSession->segRxCtrl )	/* (4) */
	{
		case SEG_RX_WAIT_SENT_FC_CTS_CON:	/* (4-1) */
			switch( frameType )	/* (3-3) */
			{/* for single frame and first frame, we already processed above. */
				case CONSECUTIVE_FRAME:
					if( pTpSession->sentConfirmed != TRUE )
					{/* CF frame received while Flow Control CTS not sent out yet after processing the FF. */
						/* Discard received frame due to the unexpected reception of order error. */
						( pTpSessionCfg->pRxNext )( sessionId );				/** UdsTp_req 88 */
					}else
					{/* Flow Control frame already sent, so the CF frame received is valid. */
#if (UDSTP_TIMING_ENABLE == TRUE)
						UDSTP_TIMER_STOP( sessionId ); 	/* Stop timer for N_Ar. */ 	/** UdsTp_req 77 */
						UDSTP_TIMER_START(sessionId, pRxNTiming->nCr);				/** UdsTp_req 81 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
						pTpSession->sentConfirmed = FALSE;
						handleResult = UdsTp_GetConsecutiveFrame( sessionId, pRxPacket, pTxPacket );
						switch( handleResult )
						{
							case E_PENDING:
								/* wait for more CF frames for current block. */
								/* tpState remains SEG_RX_IN_PROGRESS */
								pTpSession->segRxCtrl = SEG_RX_WAIT_CF;
								break;
							case E_FLOWCONTROL_CONTINUE:
								/* Finished current block, continue for next block. */
#if (UDSTP_TIMING_ENABLE == TRUE)
								UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Br. */	/** UdsTp_req 79 */
								UDSTP_TIMER_START(sessionId, pRxNTiming->nAr);					/** UdsTp_req 77 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
								/* tpState remains SEG_RX_IN_PROGRESS */
								pTpSession->segRxCtrl = SEG_RX_WAIT_SENT_FC_CTS_CON;
								break;
							case E_FLOWCONTROL_WAIT:
								/* Wait to be able to process subsequent blocks.*/
#if (UDSTP_TIMING_ENABLE == TRUE)
							UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Br. */	/** UdsTp_req 79 */
							UDSTP_TIMER_START(sessionId, pRxNTiming->nAr);					/** UdsTp_req 77 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
								/* tpState remains SEG_RX_IN_PROGRESS */
								pTpSession->segRxCtrl = SEG_RX_WAIT_SENT_FC_WAIT_CON;
								break;
							case E_OK:
								/* Finished seg data reception. */
								/* Discard received frame from buffer due to a successful processing. */
#if (UDSTP_TIMING_ENABLE == TRUE)
								UDSTP_TIMER_STOP( sessionId ); 	/* Stop timer for N_Cr. */	/** UdsTp_req 81 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
								/* Forward segmented data to upper layer in case this is the last CF frame in last block. */
								UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrClient,
										pTpSessionCfg->pNPdu_AI->addrServer, (uint8_t)(pTpSession->nTaType), pTpSessionCfg->pNPdu_AI->addrExtenClient,
										(uint8_t)(N_OK), pTpSession->nPdu_Length, pPduMsgData->msgData ); /** UdsTp_req 2 *//** UdsTp_req 10 *//** UdsTp_req 26 */
								pTpSession->tpState = RX_TX_IDLE;
								break;
							case E_PROTOCOL_NOT_ALLOWED:
								/* Unexpected CF frame received. */
								/* Discard received frame due to a protocol format error. */ /** UdsTp_req 88 */
								pTpSession->tpState = RX_TX_IDLE;
								break;
							case E_FLOWCONTROL_SN_ERR:
								/* E_FLOWCONTROL_SN_ERR */									/** UdsTp_req 50 */
								/* Discard PduMsgData and reset CF related variables by set tpState to RX_TX_IDLE */
								pTpSession->tpState = RX_TX_IDLE;
								break;
							default:
								/* We will never drop here in normal execution. */
								break;
						} /* end of switch( handleResult ) */
						/* Discard received frame from buffer. */
						( pTpSessionCfg->pRxNext )( sessionId );
					}
					break;
				case FLOW_CONTROL_FRAME:
				case INVALID_FRAME:
					/* unexpected frame(FC, unknown) received(include order error), just ignore the reception. */
					/* Ignore the received frame and discard it from receive buffer. */
					( pTpSessionCfg->pRxNext )( sessionId );							/** UdsTp_req 86 *//** UdsTp_req 88 */
				case NO_FRAME:
                    /* No Frame received yet, so we continue to process confirmation of transmitted FC frame. */
                    //lint -fallthrough
				default:
					if( pTpSession->sentConfirmed != TRUE )
					{/* Flow Control frame still not set yet. */
#if (UDSTP_TIMING_ENABLE == TRUE)
						if( (UDSTP_TIMER_ISPENDING(sessionId)) && (UDSTP_TIMER_ISTIMEOUT(sessionId)) ) /** UdsTp_req 77 */
						{/* N_Ar timing pending */
								/* Abort message reception and issue N_USData.indication(<N_Result> = N_TIMEOUT_A).*/
							UDSTP_TIMER_STOP(sessionId);
								( pTpSessionCfg->pTxAbortRequest )( sessionId );
								UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrClient,
										pTpSessionCfg->pNPdu_AI->addrServer, (uint8_t)(pTpSession->nTaType),
										pTpSessionCfg->pNPdu_AI->addrExtenClient,
										(uint8_t)(N_TIMEOUT_A), 0x00u, 0x00u ); /** UdsTp_req 75 *//** UdsTp_req 83 */
								pTpSession->tpState = RX_TX_IDLE;
						}
#endif /* UDSTP_TIMING_ENABLE == TRUE */
					}else
					{/* Flow Control frame already sent. */
#if (UDSTP_TIMING_ENABLE == TRUE)
						UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Ar. */	/** UdsTp_req 77 */
						UDSTP_TIMER_START(sessionId, pRxNTiming->nCr);					/** UdsTp_req 81 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
						pTpSession->sentConfirmed = FALSE;
						/* tpState remains SEG_RX_IN_PROGRESS */
						pTpSession->segRxCtrl = SEG_RX_WAIT_CF;
					}
					break;
				case SINGLE_FRAME:
				case FIRST_FRAME:
					break;
			}
			break;
		case SEG_RX_WAIT_SENT_FC_WAIT_CON:	/* (4-2) */
			switch( frameType )	/* (3-4) */
			{/* for single frame and first frame, we already processed above. */
				case NO_FRAME:/* no frame received. */
				case CONSECUTIVE_FRAME:
				case FLOW_CONTROL_FRAME:
				case INVALID_FRAME:
				default:/* unexpected frame(CF, FC, unknown) received, just ignore the reception. */
					if( pTpSession->sentConfirmed != TRUE )
					{/* Flow Control wait frame still not set yet. */
						/* tpState remains SEG_RX_IN_PROGRESS */
						/* segRxCtrl remains SEG_RX_WAIT_SENT_FC_WAIT_CON */
#if (UDSTP_TIMING_ENABLE == TRUE)
						if( (UDSTP_TIMER_ISPENDING(sessionId)) && (UDSTP_TIMER_ISTIMEOUT(sessionId)) ) /** UdsTp_req 77 */
						{/* N_Ar timing pending and N_Ar timeout occurs. */
								/* Abort message reception and issue N_USData.indication(<N_Result> = N_TIMEOUT_A).*/
							UDSTP_TIMER_STOP(sessionId);
								( pTpSessionCfg->pTxAbortRequest )( sessionId );
								UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrClient,
										pTpSessionCfg->pNPdu_AI->addrServer, (uint8_t)(pTpSession->nTaType),
										pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_TIMEOUT_A), 0x00u, 0x00u); /** UdsTp_req 75 *//** UdsTp_req 83 */
								pTpSession->tpState = RX_TX_IDLE;
						}
#endif /* UDSTP_TIMING_ENABLE == TRUE */
					}else
					{/* Flow Control wait frame already sent. */
#if (UDSTP_TIMING_ENABLE == TRUE)
						UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Ar. */	/** UdsTp_req 77 */
						UDSTP_TIMER_START(sessionId, pRxNTiming->nBr); 					/** UdsTp_req 79 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
						/* If Ready to receive more data then transmit FC N_PDU(FS = CTS) with
						 * BS and STmin, otherwise transmit FC N_PDU(FS = WAIT) with BS and STmin . */
						if( UdsTpApp_IsDataReceivable( sessionId, pTpSession->nPdu_Length ) )
						{
#if (UDSTP_TIMING_ENABLE == TRUE)
							UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Br. */	/** UdsTp_req 79 */
							UDSTP_TIMER_START(sessionId, pRxNTiming->nAr); 					/** UdsTp_req 77 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
							pTpSession->sentConfirmed = FALSE;
							UdsTp_SendFlowCtrlFrame(sessionId, pTxPacket, ISO15765_FLOW_CTRL_STATUS_CTS,
									pTpSession->nPdu_Bs, pTpSession->nPdu_STmin);		/** UdsTp_req 28 */
							/* tpState remains SEG_RX_IN_PROGRESS */
							pTpSession->segRxCtrl = SEG_RX_WAIT_SENT_FC_CTS_CON;
						}else
						{
#ifdef UDSTP0_NPDU_WFTMAX
  #if ( UDSTP0_NPDU_WFTMAX != 0u )
							if( pTpSession->counterWFT == 0x00u )
							{/* Max number of WAIT excedded, issue an N_USData.indication(<N_Result>=N_WFT_OVRN).*/
								UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrClient,
										pTpSessionCfg->pNPdu_AI->addrServer, (uint8_t)(pTpSession->nTaType), pTpSessionCfg->pNPdu_AI->addrExtenClient,
										(uint8_t)(N_WFT_OVRN), 0x00u, 0x00u ); /** UdsTp_req 10 *//** UdsTp_req 20 *//** UdsTp_req 112 */
								/* Discard PduMsgData and reset CF related variables by set tpState to RX_TX_IDLE */
								pTpSession->tpState = RX_TX_IDLE;
							}else
							{/* Still need to send FC frame (WAIT). */	/** UdsTp_req 70 *//** UdsTp_req 73 */
#if (UDSTP_TIMING_ENABLE == TRUE)
								UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Br. */	/** UdsTp_req 79 */
								UDSTP_TIMER_START(sessionId, pRxNTiming->nAr); 					/** UdsTp_req 77 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
								pTpSession->counterWFT--;
								pTpSession->sentConfirmed = FALSE;
								UdsTp_SendFlowCtrlFrame(sessionId, pTxPacket, ISO15765_FLOW_CTRL_STATUS_WAIT,
										pTpSession->nPdu_Bs, pTpSession->nPdu_STmin);
								/* tpState remains SEG_RX_IN_PROGRESS */
								/* segRxCtrl remains SEG_RX_WAIT_SENT_FC_WAIT_CON */
							}
  #endif /* UDSTP0_NPDU_WFTMAX != 0 */
#endif /* UDSTP0_NPDU_WFTMAX */
						}
					}
					/* Ignore the received frame and discard it from receive buffer. */
					( pTpSessionCfg->pRxNext )( sessionId );/** UdsTp_req 86 *//** UdsTp_req 88 */
					break;
				case SINGLE_FRAME:
				case FIRST_FRAME:
					break;
			}
			break;
		case SEG_RX_WAIT_SENT_FC_OVFLW_CON:	/* (4-3) */
			if(pTpSession->sentConfirmed != TRUE)
			{/* If we have received frames, then these are unexpected reception order
			  * due to Flow Control Over_Flow not confirmed to have been sent yet while new frames received. */
#if (UDSTP_TIMING_ENABLE == TRUE)
				if( (UDSTP_TIMER_ISPENDING(sessionId)) && (UDSTP_TIMER_ISTIMEOUT(sessionId)) ) /** UdsTp_req 79 */
				{/* N_Br timing pending and N_Br timeout occurs. */
					/* Abort message transmission and rest tpState to idle.*/
					UDSTP_TIMER_STOP(sessionId);
					( pTpSessionCfg->pTxAbortRequest )( sessionId );
					pTpSession->tpState = RX_TX_IDLE;
				}
#endif /* UDSTP_TIMING_ENABLE == TRUE */
				/* discard received frame from receive buffer. */
				( pTpSessionCfg->pRxNext )( sessionId );
			}else
			{/* Flow Control Over_Flow frame already sent due to valid tx confirmation. */
#if (UDSTP_TIMING_ENABLE == TRUE)
				UDSTP_TIMER_STOP(sessionId);	/* Stop the timer for N_Br. */	/** UdsTp_req 79 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
				pTpSession->tpState = RX_TX_IDLE;
			}
			break;
		case SEG_RX_WAIT_CF:				/* (4-4) */
			switch( frameType )	/* (3-6) */
			{/* for single frame and first frame, we already processed above. */
				case CONSECUTIVE_FRAME:
#if (UDSTP_TIMING_ENABLE == TRUE)
					UDSTP_TIMER_START(sessionId, pRxNTiming->nCr);				/** UdsTp_req 81 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
					pTpSession->sentConfirmed = FALSE;
					handleResult = UdsTp_GetConsecutiveFrame( sessionId, pRxPacket, pTxPacket );
					switch( handleResult )
					{
						case E_PENDING:
							/* wait for more CF frames for current block. */
							/* tpState remains SEG_RX_IN_PROGRESS */
							/* segRxCtrl remains SEG_RX_WAIT_CF */
							break;
						case E_FLOWCONTROL_CONTINUE:
							/* Finished current block, continue for next block. */
#if (UDSTP_TIMING_ENABLE == TRUE)
							UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Br. */	/** UdsTp_req 79 */
							UDSTP_TIMER_START(sessionId, pRxNTiming->nAr);					/** UdsTp_req 77 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
							/* tpState remains SEG_RX_IN_PROGRESS */
							pTpSession->segRxCtrl = SEG_RX_WAIT_SENT_FC_CTS_CON;
							break;
						case E_FLOWCONTROL_WAIT:
							/* Wait to be able to process subsequent blocks.*/
#if (UDSTP_TIMING_ENABLE == TRUE)
							UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Br. */	/** UdsTp_req 79 */
							UDSTP_TIMER_START(sessionId, pRxNTiming->nAr);					/** UdsTp_req 77 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
							/* tpState remains SEG_RX_IN_PROGRESS */
							pTpSession->segRxCtrl = SEG_RX_WAIT_SENT_FC_WAIT_CON;
							break;
						case E_OK:
							/* Finished seg data reception. */
							/* Discard received frame from buffer due to a successful processing. */
#if (UDSTP_TIMING_ENABLE == TRUE)
							UDSTP_TIMER_STOP( sessionId ); 	/* Stop timer for N_Cr. */	/** UdsTp_req 81 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
							/* Forward segmented data to upper layer in case this is the last CF frame in last block. */
							UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrClient,
									pTpSessionCfg->pNPdu_AI->addrServer, (uint8_t)(pTpSession->nTaType), pTpSessionCfg->pNPdu_AI->addrExtenClient,
									(uint8_t)(N_OK), pTpSession->nPdu_Length, pPduMsgData->msgData ); /** UdsTp_req 2 *//** UdsTp_req 10 *//** UdsTp_req 26 */
							pTpSession->tpState = RX_TX_IDLE;
							break;
						case E_PROTOCOL_NOT_ALLOWED:
							/* Unexpected CF frame received. */
							/* Discard received frame due to a protocol format error. */ /** UdsTp_req 88 */
							pTpSession->tpState = RX_TX_IDLE;
							break;
						case E_FLOWCONTROL_SN_ERR:
							/* E_FLOWCONTROL_SN_ERR */									/** UdsTp_req 50 */
							/* Discard PduMsgData and reset CF related variables by set tpState to RX_TX_IDLE */
							pTpSession->tpState = RX_TX_IDLE;
							break;
						default:
							/* We will never drop here in normal execution. */
							break;
					} /* end of switch( handleResult ) */
					/* Discard received frame from buffer. */
					( pTpSessionCfg->pRxNext )( sessionId );
					break;
				case FLOW_CONTROL_FRAME:
				case INVALID_FRAME:
					/* unexpected frame(FC, unknown) received, just ignore. */
					/* Discard received frame from buffer. */
					( pTpSessionCfg->pRxNext )( sessionId );
				case NO_FRAME:
                    /* No Frame received yet, so we fallthrough to process Ncr timeout checking. */
                    //lint -fallthrough
				default:
#if (UDSTP_TIMING_ENABLE == TRUE)
					if( (UDSTP_TIMER_ISPENDING(sessionId)) && (UDSTP_TIMER_ISTIMEOUT(sessionId)) ) /** UdsTp_req 81 */
					{/* N_Cr timing pending and N_Cr timeout occurs. */
						/* Abort message reception and issue N_USData.indication(<N_Result> = N_TIMEOUT_Cr).*/
						UDSTP_TIMER_STOP(sessionId);
							UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrClient,
									pTpSessionCfg->pNPdu_AI->addrServer, (uint8_t)(pTpSession->nTaType),
									pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_TIMEOUT_CR), 0x00u, 0x00u); /** UdsTp_req 75 *//** UdsTp_req 85 */
							pTpSession->tpState = RX_TX_IDLE;
					}
#endif /* UDSTP_TIMING_ENABLE == TRUE */
					break;
				case SINGLE_FRAME:
				case FIRST_FRAME:
					break;
			}
			break;
		default:
			break;
	} /* end of switch(pTpSession->segRxCtrl) */

	return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
