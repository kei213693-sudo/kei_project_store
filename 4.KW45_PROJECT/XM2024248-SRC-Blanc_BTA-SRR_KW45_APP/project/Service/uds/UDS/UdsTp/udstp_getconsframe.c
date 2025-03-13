/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_getconsframe.c
 * 摘要: UDS network transport layer get consecutive frame.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udstp_inf.h"
#include "udscan_pub.h"

/*lint -esym(818, pRxPacket)*/

/*
 * 函数介绍: This function get consecutive frame the UdsTp module received.
 * 参数: \param [in] sessionId        The index of the session which received this frame.
 * \param [in] pRxPacket        The received packet.
 * \param [out] pTxPacket       The response packet.
 * 返回值: This frame was processed successfully.
 * 备注:
 */
Std_UdsTpReturnType UDS_FN_TYPE UdsTp_GetConsecutiveFrame(
	const uint8_t sessionId,										/*lint !e963*/
	const UdsCan_QueueBufPtr 	pRxPacket,
	UdsCan_QueueBufPtr			pTxPacket
)
{
    UdsTp_SessionCfg_t*			pTpSessionCfg	 = UdsTp_SessionCfgs + sessionId;
	UdsTp_Session_t*			pTpSession		 = UdsTp_Sessions + sessionId;
	UdsTp_NPduMsgData_t*		pPduMsgData		 = pTpSessionCfg->pNSduCfg->pNPdu_MsgData;
    #if (UDSTP_TIMING_ENABLE == TRUE)
	    UdsTp_RxNTiming_t*			pRxNTiming		 = (UdsTp_RxNTiming_t*)(&pTpSessionCfg->pNSduCfg->timingData); /* MISRA 2004 Rule 11.4 *//*lint !e929*/
    #endif /* UDSTP_TIMING_ENABLE == TRUE */
	uint16_t					remainConseFrame = pTpSession->remainConseFrame;
	uint16_t					currentBufPos	 = pTpSession->currentBufPos;
	uint8_t						numBytesLstFrame = pTpSession->numBytesLstFrame;
	uint8_t						tmpSeqNum;
	uint16_t					tmp;
	uint16_t					tmp2;
	Std_UdsTpReturnType			retVal 			= E_PROTOCOL_NOT_ALLOWED;
	uint8_t						i;
	uint8_t 					CAN_FD_DLC_64[8u] = {8u,12u,16u,20u,24u,32u,48u,64u};
	uint8_t						getCF_Dlc = 0;
    uint8_t                     numBytesLstFrame01 = numBytesLstFrame+1;
	for ( i = 0; i <= 8; i++)
	{
		if (pRxPacket->dlc <= CAN_FD_DLC_64[i])
		{
			getCF_Dlc = CAN_FD_DLC_64[i];
			break;
		}
	}
    	for ( i = 0; i <= 8; i++)
	{
		if (numBytesLstFrame01 <= CAN_FD_DLC_64[i])
		{
			numBytesLstFrame01 = CAN_FD_DLC_64[i];
			break;
		}
	}
	/* Get type of Functional or physical addressing. */
	tmp = (uint16_t)( UdsTp_GetTaType( sessionId, pRxPacket ) );		/** UdsTp_req 16 */
	/* Functionally addressed consecutive frames shall be ignored and
	 * no notification to the upper layers of its arrival shall be made. */
	if( (uint16_t)(CANTP_FUNCTIONAL) == tmp )							/** UdsTp_req 17 *//** UdsTp_req 24 */
	{
		return retVal;												/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }

	if( 0x00u == pTpSession->remainBlocks)
	{
		/* Unexpected order of reception, just ignore. */
		return retVal;												/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }

	/* DLC error handling. */
    #if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
	    if( (pRxPacket->dlc < 0x08u) || (pRxPacket->dlc > 64u) )								/** UdsTp_req 110 */
	    {
    #elif (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_OFF)
	    if( remainConseFrame > 0x01u )
	    {/* Not last CF Frame */
		    tmp = getCF_Dlc;					/* The expected DLC value in the received frame. *//** UdsTp_req 109 */
	    }else
	    {/* This is last CF Frame */
            #if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
		        /* normal(fixed) addressing. */
		        tmp = numBytesLstFrame + 0x01u; 		/** UdsTp_req 106 *//** UdsTp_req 108 *//** UdsTp_req 109 */
            #else
		        /* extended or mixed addressing. */
		        tmp = numBytesLstFrame + 0x02u; 		/** UdsTp_req 106 *//** UdsTp_req 108 *//** UdsTp_req 109 */
            #endif
	    }
	    if( (pRxPacket->dlc < 0x08u) && (pRxPacket->dlc > 15u))								/** UdsTp_req 111 */
	    {
    #endif
		   /* The reception of a CAN frame with a DLC value smaller than expected
		    * shall be ignored by the network layer without any further action. */
		   return retVal;										/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	    }
        else
        {

        }

	/* Get CF_SN */
    #if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	    /* normal(fixed) addressing. */
	    tmpSeqNum  = (pRxPacket->msgBuffer[0x00u] & ISO15765_TPCI_CF_SN_MASK);
    #else
	    /* extended or mixed addressing. */
	    tmpSeqNum  = (pRxPacket->msgBuffer[0x01u] & ISO15765_TPCI_CF_SN_MASK);
    #endif

	/* CF_SN error handling. */
	/* In case an error order detected with received CF_SN, the received message will be aborted
	 * and an N_USData.indication with N_Result=N_WRONG_SN shall send to upper layer. */
	if( pTpSession->nPdu_SeqNum != 15u )
	{
		if( tmpSeqNum != (pTpSession->nPdu_SeqNum + 0x01u) )			/** UdsTp_req 2 *//** UdsTp_req 50 */
		{/* sequence error */
			UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype),
			                          pTpSessionCfg->pNPdu_AI->addrClient,
					                  pTpSessionCfg->pNPdu_AI->addrServer,
					                  (uint8_t)(pTpSession->nTaType),
					                  pTpSessionCfg->pNPdu_AI->addrExtenClient,
					                  (uint8_t)(N_WRONG_SN),
					                  0x00u,
					                  0x00u );					/** UdsTp_req 10 *//** UdsTp_req 20 */
			/* Discard PduMsgData and reset CF related variables by set tpState to RX_TX_IDLE */
			retVal = E_FLOWCONTROL_SN_ERR;
			return retVal;											/* MISRA 2004 Rule 14.7 *//*lint !e904*/
		}
		else
		{

		}
	}else
	{
		if( 0x00u != tmpSeqNum )										/** UdsTp_req 2 *//** UdsTp_req 50 */
		{/* sequence error */
			UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype),
			                          pTpSessionCfg->pNPdu_AI->addrClient,
					                  pTpSessionCfg->pNPdu_AI->addrServer,
					                  (uint8_t)(pTpSession->nTaType),
					                  pTpSessionCfg->pNPdu_AI->addrExtenClient,
					                  (uint8_t)(N_WRONG_SN),
					                  0x00u,
					                  0x00u );					/** UdsTp_req 10 *//** UdsTp_req 20 */
			/* Discard PduMsgData and reset CF related variables by set tpState to RX_TX_IDLE */
			retVal = E_FLOWCONTROL_SN_ERR;
			return retVal;											/* MISRA 2004 Rule 14.7 *//*lint !e904*/
		}
		else
		{

		}
	}

	/* A correct CF frame message has been received.*/
#if (UDSTP_TIMING_ENABLE == TRUE)
	if( remainConseFrame > 0x01u )
	{/* not a last consecutive frame */
		UDSTP_TIMER_START(sessionId, pRxNTiming->nCr); 		/** UdsTp_req 81 */
	}else
	{/* the last consecutive frame */
		UDSTP_TIMER_START(sessionId, pRxNTiming->nBr); 		/** UdsTp_req 79 */
	}
#endif /* UDSTP_TIMING_ENABLE == TRUE */

	/* Updata Sequence number in session runtime data. */
	if( pTpSession->nPdu_SeqNum != 15u )	/** UdsTp_req 46 *//** UdsTp_req 47 *//** UdsTp_req 48 *//** UdsTp_req 49 */
	{
		pTpSession->nPdu_SeqNum++;
	}else
	{
		pTpSession->nPdu_SeqNum = 0x00u;
	}
	pTpSession->remainConseFrame--;

#ifdef UDSTP0_NPDU_WFTMAX
  #if ( UDSTP0_NPDU_WFTMAX != 0x00u )
		pTpSession->counterWFT = UDSTP0_NPDU_WFTMAX;
  #endif /* UDSTP0_NPDU_WFTMAX != 0 */
#endif /* UDSTP0_NPDU_WFTMAX */

	/* Copy CF frame data to N_PDU_Data buffer. */
	if( pTpSession->remainConseFrame != 0x00u )
	{/* not a last consecutive frame */
	#ifdef CANFD_TP_FLAG
		if (getCF_Dlc != 64u)
		{
			retVal = E_FLOWCONTROL_SN_ERR;
			return retVal;
		}
	#else
		if (getCF_Dlc != 0x08u)
		{
			retVal = E_FLOWCONTROL_SN_ERR;
			return retVal;
		}
	#endif

		tmp = getCF_Dlc;
	}else
	{/* The last consecutive frame */
		if( pTpSession->remainBlocks > 0x01u )
		{/* This is not the last CF frame of the last block */
			tmp = getCF_Dlc;
		}else
		{/* This is the last CF frame of the last block */
          if ((getCF_Dlc) != numBytesLstFrame01)
		{
			retVal = E_FLOWCONTROL_SN_ERR;
			return retVal;
		}
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
			/* normal(fixed) addressing. */
			tmp = (uint16_t)(numBytesLstFrame + 0x01u);
#else
			/* extended or mixed addressing. */
			tmp = (uint16_t)(numBytesLstFrame + 0x02u);
#endif
		}
	}
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	for(tmp2 = 0x01u; tmp2 < tmp; ++tmp2, ++currentBufPos)		/* MISRA 2004 Required Rule 12.10 *//*lint !e960*/
#else
	/* extended or mixed addressing. */
	for(tmp2 = 0x02u; tmp2 < tmp; ++tmp2, ++currentBufPos)		/* MISRA 2004 Required Rule 12.10 *//*lint !e960*/
#endif
	{
		pPduMsgData->msgData[currentBufPos] = pRxPacket->msgBuffer[tmp2];
	}
	pTpSession->currentBufPos = currentBufPos;

	if( pTpSession->remainConseFrame == 0x00u )
	{/* Finished current block reception. */
		pTpSession->remainBlocks--;						/* Updata block number. */
		if(pTpSession->remainBlocks != 0x00u)
		{/* Still have more blocks expected to receive. */
			/* Updata expected number of CF frames for next block. */
			if(pTpSession->remainBlocks != 0x01u)
			{/* More than one block are expected to be received. */
				pTpSession->remainConseFrame = pTpSession->nPdu_Bs;
			}else
			{/* The last one block is expected to be received. */
				tmp = pTpSession->nPdu_Length - pTpSession->currentBufPos;
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
				/* normal(fixed) addressing. */
				tmp2 = tmp % (getCF_Dlc-1U);
				tmp = tmp / (getCF_Dlc-1U);
#else
				/* extended or mixed addressing. */
				tmp2 = tmp % (getCF_Dlc-2U);
				tmp = tmp / (getCF_Dlc-2U);
#endif
				if( tmp2 )
				{
					pTpSession->remainConseFrame = tmp + 0x01u;
				}else
				{
					pTpSession->remainConseFrame = tmp;
				}
			}
			/* If Ready to receive more data then transmit FC N_PDU(FS = CTS) with
			 * BS and STmin, otherwise transmit FC N_PDU(FS = WAIT) with BS and STmin . */
			if( UdsTpApp_IsDataReceivable( sessionId, pTpSession->nPdu_Length ) )
			{
				pTpSession->sentConfirmed = FALSE;
				UdsTp_SendFlowCtrlFrame(sessionId, pTxPacket, ISO15765_FLOW_CTRL_STATUS_CTS,
										pTpSession->nPdu_Bs, pTpSession->nPdu_STmin);		/** UdsTp_req 28 */
				retVal = E_FLOWCONTROL_CONTINUE;	/* Waiting for more blocks of CF frames. */
			}else
			{
#ifdef UDSTP0_NPDU_WFTMAX
  #if ( UDSTP0_NPDU_WFTMAX != 0x00u )
				pTpSession->sentConfirmed = FALSE;
				pTpSession->counterWFT--;
				UdsTp_SendFlowCtrlFrame(sessionId,
				                        pTxPacket,
				                        ISO15765_FLOW_CTRL_STATUS_WAIT,
										pTpSession->nPdu_Bs,
										pTpSession->nPdu_STmin);
  #endif /* UDSTP0_NPDU_WFTMAX != 0u */
#endif
				/* Not able to receive subsequent segmented data for now, will wait and continue to check. */
				retVal = E_FLOWCONTROL_WAIT;
			}
		}else
		{/* Finished reception of the segmented data. */
			retVal = E_OK;		/* Finished seg data receive. (tpState shall return to RX_TX_IDLE)*/
		}
	}else
	{/* More CF frames expected for current block. */
		retVal = E_PENDING;		/* Waiting for more CF frames. */
	}

	return retVal;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
