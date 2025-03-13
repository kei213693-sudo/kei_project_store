/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp.c
 * 摘要: UDS network transport layer initialisation and processor.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udstp_inf.h"
#include "udscan_pub.h"
#include "udscan_confdefs.h"


/*
 * 函数介绍: This function initialises the CAN transport Layer. It must be called exactly once.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_Initialize( void )
{
    uint8_t				sessionId;
	UdsTp_Session_t*	pTpSession		= UdsTp_Sessions;
	UdsTp_SessionCfg_t*	pTpSessionCfg	= UdsTp_SessionCfgs;

	/* Initialise the data link layer interface. */
	UdsCan_Initialize();

	/* Initialize the state of all Tp sessions. */
    for( sessionId = 0x00u; sessionId < UDSTP_NUM_SESSIONS; ++sessionId )
	{
		pTpSession->tpState             = RX_TX_IDLE;
		pTpSession->nPdu_STmin          = pTpSessionCfg->nPdu_STmin;
		pTpSession->nPdu_Bs             = pTpSessionCfg->nPdu_Bs;
		pTpSession->pendingFrameType    = NO_FRAME;
		pTpSession->nUSDataTransReq     = FALSE;
		pTpSession->flagZeroFlowCtrlReq = FALSE;
		pTpSession->sentConfirmed       = FALSE;
		pTpSession->flagTimeoutCounter  = FALSE;
		/* If more initialization need, add here. */
        ++pTpSession;
		++pTpSessionCfg;
	}

	return;
}

/**
 * This function identifies the type of framed received by UdsTp layer.
 *
 * Error inspections on N_PCItype are implemented according to the type of frame.
 */
UdsTp_FrameType_t UDS_FN_TYPE UdsTp_GetFrameType(
	const UdsTp_AddressingFormat_t addrFormat,			/*lint !e963*/
	const UdsCan_QueueBuf_t* rxPduPtr
)
{
	uint8_t 			pci;
	UdsTp_FrameType_t 	retVal = INVALID_FRAME;

	if (NULL == rxPduPtr)
	{
		return INVALID_FRAME;
	}
	else
	{
		/* code */
	}

	switch((uint8_t)(addrFormat))
	{
		case ADDRESSING_NORMAL:
			 pci = rxPduPtr->msgBuffer[0x00u];
			 break;
		case ADDRESSING_NORMAL_FIXED:
			 pci = rxPduPtr->msgBuffer[0x00u];
			 break;
		case ADDRESSING_EXTENDED:
			 pci = rxPduPtr->msgBuffer[0x01u];
			 break;
		case ADDRESSING_MIXED11BIT:
			 pci = rxPduPtr->msgBuffer[0x01u];
			 break;
		case ADDRESSING_MIXED29BIT:
			 pci = rxPduPtr->msgBuffer[0x01u];
			 break;
		default: /* We will never arrive here */
			 return retVal;	/* MISRA 2004 Rule 14.7 - Return statement before end of function *//*lint !e904*/
			 break;									/*lint !e527*/
	} /* end of switch(addrFormat) */

	switch(pci & ISO15765_TPCI_MASK)
	{
		case ISO15765_TPCI_SF:		/* Single Frame */
			 retVal = SINGLE_FRAME;
			 break;
		case ISO15765_TPCI_FF:		/* First Frame */
			 retVal = FIRST_FRAME;
			 break;
		case ISO15765_TPCI_CF:		/* Consecutive Frame */
			 retVal = CONSECUTIVE_FRAME;
			 break;
		case ISO15765_TPCI_FC:		/* Flow Control Frame */
			 retVal = FLOW_CONTROL_FRAME;
			 break;
		default:
			 retVal = INVALID_FRAME;					/** UdsTp_req 86 */
			 break;
	} /* end of switch(pci & ISO15765_TPCI_MASK) */

	return retVal;
}

/**
 * This function check the addressing type of received frames.
 *
 * \param [in] sessionId        The index of the session which received this frame.
 * \param [in] pRxPacket        The received packet.
 *
 * \return
 *  - This frame was processed successfully.
 */
UdsTp_NTaType_t UDS_FN_TYPE UdsTp_GetTaType(
	const uint8_t            sessionId,							/*lint !e963*/
	const UdsCan_QueueBuf_t* pRxPacket
)
{
	UdsTp_Session_t*	pTpSession	= UdsTp_Sessions + sessionId;
	UdsTp_NTaType_t		retVal;

	if (NULL == pRxPacket)
	{
		return 0;
	}
	else
	{
		/* code */
	}


#if (UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_NORMAL)
	if( UdsCan0_ChannelCfgs[0].msgIdPhy == pRxPacket->canID )
	{
		pTpSession->nTaType = CANTP_PHYSICAL;
		retVal = CANTP_PHYSICAL;
	}else
	{/* pRxPacket->canID = UDS0_CAN_RX_MSGID_FNC */
		pTpSession->nTaType = CANTP_FUNCTIONAL;
		retVal = CANTP_FUNCTIONAL;
	}
#elif (UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_NORMAL_FIXED)
	if((pRxPacket->canID & 0x1FFF0000u) == 0x18DA0000u)
	{
		pTpSession->nTaType = CANTP_PHYSICAL;
		retVal = CANTP_PHYSICAL;
	}else
	{/* (pRxPacket->canID & 0x1FFF0000u) = 0x18DB0000u */
		pTpSession->nTaType = CANTP_FUNCTIONAL;
		retVal = CANTP_FUNCTIONAL;
	}
#elif (UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_EXTENDED)
	if( pRxPacket->canID == UDS0_CAN_RX_MSGID_PHY )
	{
		pTpSession->nTaType = CANTP_PHYSICAL;
		retVal = CANTP_PHYSICAL;
	}else
	{/* pRxPacket->canID = UDS0_CAN_RX_MSGID_FNC */
		pTpSession->nTaType = CANTP_FUNCTIONAL;
		retVal = CANTP_FUNCTIONAL;
	}
#elif (UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_MIXED11BIT)
	if( pRxPacket->canID == UDS0_CAN_RX_MSGID_PHY )
	{
		pTpSession->nTaType = CANTP_PHYSICAL;
		retVal = CANTP_PHYSICAL;
	}else
	{/* pRxPacket->canID = UDS0_CAN_RX_MSGID_FNC */
		pTpSession->nTaType = CANTP_FUNCTIONAL;
		retVal = CANTP_FUNCTIONAL;
	}
#elif (UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_MIXED29BIT)
	if((pRxPacket->canID & 0x1FFF0000u) == 0x18CE0000u)
	{
		pTpSession->nTaType = CANTP_PHYSICAL;
		retVal = CANTP_PHYSICAL;
	}else
	{/* (pRxPacket->canID & 0x1FFF0000u) == 0x18CD0000u */
		pTpSession->nTaType = CANTP_FUNCTIONAL;
		retVal = CANTP_FUNCTIONAL;
	}
#endif

	return retVal;
}

/*
 * 函数介绍: This function Pack the CAN ID in transmit buffer for all types of transmit frames.
 * 参数: \param [in] pTxPacket        The transmit packet.
 * 返回值: {*}
 * 备注:
 */
#include "udscan_conf.h"
void UDS_FN_TYPE UdsTp_PackCanId( UdsCan_QueueBufPtr pTxPacket )
{
	/* Load CAN ID */														/** UdsTp_req 91 */
#if (UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_NORMAL)  /** UdsTp_req 93 *//** UdsTp_req 94 */
	/* normal addressing - 11bit ID. */
	pTxPacket->canID = (0x000007FFu & UdsCan0_ChannelCfgs[1].msgIdPhy);
#elif(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_NORMAL_FIXED)	/** UdsTp_req 95 *//** UdsTp_req 96 */
	/* normal fixed addressing - 29bit ID. */
	pTxPacket->canID = (0x1FFFFFFFu & UDS0_CAN_TX_MSGID_PHY);
#elif(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_EXTENDED)   	/** UdsTp_req 98 */
	/* extended addressing - 29bit ID. */
	pTxPacket->canID = (0x1FFFFFFFu & UDS0_CAN_TX_MSGID_PHY);
#elif(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_MIXED11BIT)		/** UdsTp_req 103 *//** UdsTp_req 104 */
	/* mixed addressing - 11bit ID. */
	pTxPacket->canID = (0x000007FFu & UDS0_CAN_TX_MSGID_PHY);
#elif(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_MIXED29BIT)		/** UdsTp_req 100 *//** UdsTp_req 101 */
	/* mixed addressing - 29bit ID. */
	pTxPacket->canID = (0x1FFFFFFFu & UDS0_CAN_TX_MSGID_PHY);
#endif

	return;
}

/*
 * 函数介绍: This function identifies the type of framed to be transmited by UdsTp layer.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
UdsTp_FrameType_t UDS_FN_TYPE UdsTp_SetFrameType( uint16_t msgLen )
{
	UdsTp_FrameType_t 	retVal = INVALID_FRAME;

	if( msgLen == 0x00u )											/** UdsTp_req 90 */
	{
		return retVal;	/* MISRA 2004 Rule 14.7 - Return statement before end of function *//*lint !e904*/
	}
    else
    {

    }
#ifdef CANFD_TP_FLAG
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	if( msgLen <= 62u)											/** UdsTp_req 34 */
	{
		retVal = SINGLE_FRAME;
	}else
	{
		retVal = FIRST_FRAME;
	}
#else
	if( msgLen <= 61u)											/** UdsTp_req 34 */
	{
		retVal = SINGLE_FRAME;
	}else
	{
		retVal = FIRST_FRAME;
	}
#endif
	return retVal;
#endif

#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	if( msgLen <= 0x07u)											/** UdsTp_req 34 */
	{
		retVal = SINGLE_FRAME;
	}else
	{
		retVal = FIRST_FRAME;
	}
#else
	/* extended or mixed addressing. */
	if( msgLen <= 0x06u)											/** UdsTp_req 34 */
	{
		retVal = SINGLE_FRAME;
	}else
	{
		retVal = FIRST_FRAME;
	}
#endif

	return retVal;
}

/*
 * 函数介绍: This function reads a received message from the CAN data link layer interface channel
 * buffer, processes it and, if possible, sends a response on the response channel.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_ServiceProc( void )
{
	UdsTp_SessionCfg_t* 	pTpSessionCfg	= UdsTp_SessionCfgs;
	UdsTp_Session_t*	  	pTpSession		= UdsTp_Sessions;
	UdsTp_NPduMsgData_t*	pPduMsgData;
#if (UDSTP_TIMING_ENABLE == TRUE)
	UdsTp_RxNTiming_t*		pRxNTiming		= (UdsTp_RxNTiming_t*)(&pTpSessionCfg->pNSduCfg->timingData);  /* MISRA 2004 Rule 11.4 *//*lint !e929*/
#endif /* UDSTP_TIMING_ENABLE == TRUE */
	uint8_t				  	sessionId;
	UdsTp_FrameType_t		frameType;
	ISO15765_TpState_t		tpState			= pTpSession->tpState;
	uint8_t 				handleResult	= 0x00u;

	for( sessionId = 0x00u; sessionId < UDSTP_NUM_SESSIONS; ++sessionId )
	{
		UdsCan_QueueBufPtr	pRxPacket;
		UdsCan_QueueBufPtr	pTxPacket;

		/* pointing to corresponding buffer for this session who has available received message. */
		pRxPacket = ( ( pTpSessionCfg->pGetRxBuf )( sessionId ) );

		if( !pRxPacket )
		{/* No messages received. */
			frameType = NO_FRAME;
		}else
		{/* New messages received. */
			frameType = UdsTp_GetFrameType( pTpSessionCfg->addressingFormat, pRxPacket );
		}
		switch( tpState )			/* (2) */	/** UdsTp_req 90 */
		{
			case RX_TX_IDLE:		/* (2-1) */
				/* Check&Process new N_USData request. */
				if( pTpSession->nUSDataTransReq )
				{
					/* pointing to corresponding buffer for this session who has free buffer to
					 * allocate message to transmit. */
					pTxPacket = ( ( pTpSessionCfg->pGetTxBuf )( sessionId ) );
					if( pTxPacket )
					{
						/* single frame transmit will process and finished here. */
						/* Multi frame transmit will start from here(segTxCtrl). */
						UdsTp_TransmitProc( sessionId, pTxPacket );
						pTpSession->nUSDataTransReq = FALSE;
					}
					else
					{

					}
				}else
				{
					switch( frameType ) /* (3-1) */
					{
						case NO_FRAME:
							break;
						case SINGLE_FRAME:	/* Single Frame */
							handleResult = UdsTp_GetSingleFrame( sessionId, pRxPacket );
							if( (handleResult == E_OK) || (handleResult == E_PROTOCOL_NOT_ALLOWED) )
							{/* Single frame already forwarded to upper layer or protocol error detected in the frame. */
								/* tpState remains RX_TX_IDLE */
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
							break;
						case FIRST_FRAME:	/* First Frame */
							pTxPacket = ( ( pTpSessionCfg->pGetTxBuf )( sessionId ) );
							if( pTxPacket )
							{
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
							} /* end of if( pTxPacket ) */
							break;
						case CONSECUTIVE_FRAME:
						case FLOW_CONTROL_FRAME:
						case INVALID_FRAME:
						default:/* invalid frame or unexpected frame(CF, FC, unknown) received. */
							/* Ignore the received frame and discard it from receive buffer. */
							( pTpSessionCfg->pRxNext )( sessionId );/** UdsTp_req 86 *//** UdsTp_req 88 */
							break;
					} /* end of switch( frameType ) */
				}
				break;
			case RX_IN_PROGRESS:	/* (2-2) */
				/* This can be the case that a previously dispatched <service>.ind has not been
				   start to process by upper layer since a periodic schedule mechanism(so the session
				   receive buffer can still be occupied), while a extremely short interval of a new frame
				   (no matter a single frame or else) has been received between the schedule period. */
				/* So here first send the response frame, if required, then start processing of possible
				 * subsequently received frames. */
				/* Check&Process new N_USData request. */
				if( pTpSession->nUSDataTransReq )
				{
					/* pointing to corresponding buffer for this session who has free buffer to
					 * allocate message to transmit. */
					pTxPacket = ( ( pTpSessionCfg->pGetTxBuf )( sessionId ) );
					if( pTxPacket )
					{
						/* single frame transmit will process and finished here. */
						/* Multi frame transmit will start from here(segTxCtrl). */
						UdsTp_TransmitProc( sessionId, pTxPacket );
						pTpSession->nUSDataTransReq = FALSE;
					}
					else
					{

					}
				}else
				{/* Check whether upper layer is ready to receive the single frame data kept in local buffer. */
					if( UdsTpApp_IsDispatchAllowed() )
					{/* Pending servie data is receivable by upper layer. */
						switch( pTpSession->pendingFrameType )
						{
							case SINGLE_FRAME:
								/* Forward SF N_PDU to upper layer. */
								pPduMsgData = pTpSessionCfg->pNSduCfg->pNPdu_MsgData;
								UdsTp_NUSData_indication((uint8_t)(pTpSessionCfg->nPdu_Mtype),
								                         pTpSessionCfg->pNPdu_AI->addrClient,
										                 pTpSessionCfg->pNPdu_AI->addrServer,
										                 (uint8_t)(pTpSession->nTaType),
										                 pTpSessionCfg->pNPdu_AI->addrExtenClient,
										                 (uint8_t)(N_OK),
										                 pPduMsgData->msgLen,
										                 pPduMsgData->msgData);			/** UdsTp_req 10 */
								/* Successfully forward SF N_PDU to upper layer. */
								pTpSession->pendingFrameType = NO_FRAME;
								pTpSession->tpState = RX_TX_IDLE;
								/* Discard received frame from UdsCan buffer since forwarded to upper layer. */
								( pTpSessionCfg->pRxNext )( sessionId );
								break;
							case FIRST_FRAME:
								pTxPacket = ( ( pTpSessionCfg->pGetTxBuf )( sessionId ) );
								if( pTxPacket )
								{
									/* Forward FF N_PDU to upper layer. */
									UdsTp_NUSData_FFindication( (uint8_t)(pTpSessionCfg->nPdu_Mtype),
									                            pTpSessionCfg->pNPdu_AI->addrClient,
											                    pTpSessionCfg->pNPdu_AI->addrServer,
											                    (uint8_t)(pTpSession->nTaType),
											                    pTpSessionCfg->pNPdu_AI->addrExtenClient,
											                    pTpSession->nPdu_Length );
									/* Since ready to receive more data then transmit FC N_PDU(FS = CTS) with
									 * BS(value will be ZERO in case only one block expected) and STmin. */
#if (UDSTP_TIMING_ENABLE == TRUE)
									UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Br. */		/** UdsTp_req 79 */
									UDSTP_TIMER_START(sessionId, pRxNTiming->nAr);						/** UdsTp_req 77 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
									pTpSession->sentConfirmed = FALSE;
									/* "remainBlocks" is guaranteed ether equal to or greater than one when checking here. */
									if( pTpSession->remainBlocks == 0x01u )
									{/* segmented data can be received within one block. */
										UdsTp_SendFlowCtrlFrame(sessionId,
										                        pTxPacket,
										                        ISO15765_FLOW_CTRL_STATUS_CTS,
												                0x00u,
												                pTpSession->nPdu_STmin);				/** UdsTp_req 27 *//** UdsTp_req 61 */
									}else
									{/* segmented data can not be received within one block. */
										UdsTp_SendFlowCtrlFrame(sessionId,
										                        pTxPacket,
										                        ISO15765_FLOW_CTRL_STATUS_CTS,
												                (uint8_t)(pTpSession->remainConseFrame),
												                pTpSession->nPdu_STmin);	/** UdsTp_req 27 */
									}
									pTpSession->pendingFrameType = NO_FRAME;
									pTpSession->tpState = SEG_RX_IN_PROGRESS;
									pTpSession->segRxCtrl = SEG_RX_WAIT_SENT_FC_CTS_CON;	/* Flow control CTS sent. */
									/* Discard received frame from buffer due to a successful processing. */
									( pTpSessionCfg->pRxNext )( sessionId );				/** UdsTp_req 88 */
								} /* end of if( pTxPacket ) */
								else
								{

								}
								break;
							case CONSECUTIVE_FRAME:
							case FLOW_CONTROL_FRAME:
							case INVALID_FRAME:
							case NO_FRAME:
							default:
								break;
						} /* end of switch( pTpSession->pendingFrameType ) */
					}
					/* If still not receivable by upper, the tpState remains "RX_IN_PROGRESS". */
				}
				break;
			case SEG_RX_IN_PROGRESS:/* (2-3) */
				pTxPacket = ( ( pTpSessionCfg->pGetTxBuf )( sessionId ) );
				if( pTxPacket )
				{
					UdsTp_SegRxProc( pTpSessionCfg, pTpSession, sessionId, frameType, pRxPacket, pTxPacket );
				}
				else
				{

				}
				break;
			case TX_IN_PROGRESS:	/* (2-4) */
				if(pTpSession->sentConfirmed != TRUE)
				{
#if (UDSTP_TIMING_ENABLE == TRUE)
					if( (UDSTP_TIMER_ISPENDING(sessionId)) && (UDSTP_TIMER_ISTIMEOUT(sessionId)) )	/** UdsTp_req 76 */
					{/* N_As timing pending and N_As timeout occurs. */
						/* Abort message transmission and issue N_USData.confirm(<N_Result> = N_TIMEOUT_A).*/
						UDSTP_TIMER_STOP(sessionId);
						( pTpSessionCfg->pTxAbortRequest )( sessionId );		/** UdsTp_req 82 */
							UdsTp_NUSData_Confirmation((uint8_t)(pTpSessionCfg->nPdu_Mtype),
							                           pTpSessionCfg->pNPdu_AI->addrServer,
								                       pTpSessionCfg->pNPdu_AI->addrClient,
								                       (uint8_t)(pTpSession->nTaType),
									                   pTpSessionCfg->pNPdu_AI->addrExtenClient,
									                   (uint8_t)(N_TIMEOUT_A)); /** UdsTp_req 75 *//** UdsTp_req 82 */
							pTpSession->tpState = RX_TX_IDLE;
					}
					else
					{

					}
#endif /* UDSTP_TIMING_ENABLE == TRUE */
				}else
				{/* Single Frame already sent due to valid tx confirmation. */
#if (UDSTP_TIMING_ENABLE == TRUE)
					UDSTP_TIMER_STOP(sessionId);	/* Stop the timer for N_As. */	/** UdsTp_req 76 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
					UdsTp_NUSData_Confirmation((uint8_t)(pTpSessionCfg->nPdu_Mtype),
					                           pTpSessionCfg->pNPdu_AI->addrServer,
									           pTpSessionCfg->pNPdu_AI->addrClient,
										       (uint8_t)(pTpSession->nTaType),
										       pTpSessionCfg->pNPdu_AI->addrExtenClient,
										       (uint8_t)(N_OK));
					pTpSession->tpState = RX_TX_IDLE;
				}
				break;
			case SEG_TX_IN_PROGRESS:/* (2-5) */
				pTxPacket = ( ( pTpSessionCfg->pGetTxBuf )( sessionId ) );
				if( pTxPacket )
				{
					UdsTp_SegTxProc( pTpSessionCfg, pTpSession, sessionId, frameType, pRxPacket, pTxPacket );
				}
				else
				{

				}
				break;
			default:
				break;
		} /* end of switch(tpState)*/

		++pTpSessionCfg;
		++pTpSession;
	}  /* end of for( sessionId = 0x00u; sessionId < UDSTP_NUM_SESSIONS; ++sessionId ) */

	return;
} /* end of UdsTp_ServiceProc() */

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
