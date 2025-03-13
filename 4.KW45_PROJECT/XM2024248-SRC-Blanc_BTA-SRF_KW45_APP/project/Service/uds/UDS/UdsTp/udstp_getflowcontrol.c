/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_getflowcontrol.c
 * 摘要: UDS network transport layer get flow control frame.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udscan_pub.h"

/*lint -esym(818, pRxPacket)*/


/*
 * 函数介绍: This function get Flow Control frame the UdsTp module received.
 * 参数: \param [in] sessionId        The index of the session which received this frame.
 * \param [in] pRxPacket        	  The received packet.
 * 返回值: This frame was processed successfully.
 * 备注:
 */
Std_UdsTpReturnType UDS_FN_TYPE UdsTp_GetFlowCtrlFrame(
	const uint8_t sessionId,								/*lint !e963*/
	const UdsCan_QueueBufPtr pRxPacket
)
{
    UdsTp_SessionCfg_t*			pTpSessionCfg	 = UdsTp_SessionCfgs + sessionId;
	UdsTp_Session_t*			pTpSession		 = UdsTp_Sessions + sessionId;
	uint16_t						tmpRemainBytes	 = pTpSession->nPdu_Length - pTpSession->currentBufPos;
	uint16_t						tmp2;
	uint16_t						tmp3;
	uint8_t						tmp;
	uint8_t						tmpFlowStatus;
	Std_UdsTpReturnType			retVal 			 = E_PROTOCOL_NOT_ALLOWED;

	/* Get FC_FS */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	tmpFlowStatus  = (pRxPacket->msgBuffer[0x00u] & ISO15765_TPCI_FC_FS_MASK);
	tmp = 0x03u;												/** UdsTp_req 109 */
#else
	/* extended or mixed addressing. */
	tmpFlowStatus  = (pRxPacket->msgBuffer[0x01u] & ISO15765_TPCI_FC_FS_MASK);
	tmp = 0x04u;												/** UdsTp_req 109 */
#endif

	/* DLC error handling. */
#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
	if( pRxPacket->dlc < 0x08u )								/** UdsTp_req 110 */
#elif (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_OFF)
	if( pRxPacket->dlc < tmp )								/** UdsTp_req 111 */
#endif
	{
		/* The reception of a CAN frame with a DLC value smaller than expected
		 * shall be ignored by the network layer without any further action. */
		return retVal;										/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }
	/* Get type of Functional or physical addressing. */
	tmp = (uint8_t)(UdsTp_GetTaType( sessionId, pRxPacket ));			/** UdsTp_req 16 */	/*lint !e838*/
	/* Functionally addressed flow control frames shall be ignored. */
	if( (uint8_t)(CANTP_FUNCTIONAL) == tmp )							/** UdsTp_req 17 */
	{
		return retVal;												/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }
	if( TRUE == pTpSession->flagZeroFlowCtrlReq)					/** UdsTp_req 61 */
	{/* BS that already received in FC after FF frame is 0: Receiver indicate to the sender
	  * that no more FC frames shall be sent during the transmission of the segmented message.*/
		/* So this is a error order case, abort the transmission of a segmented message,
		 * and make an N_USData.confirm service call(<N_Result>=N_ERROR).*/
		UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
						pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
						pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_ERROR)); /** UdsTp_req 19 */
		retVal = E_FLOWCONTROL_ERR;
		return retVal;												/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}

	/* Process according to FC_FS, also invalid FS processing. */
	switch( tmpFlowStatus )											/** UdsTp_req 51 */
	{
		case ISO15765_FLOW_CTRL_STATUS_CTS:
			/* Get FC_BS and STmin */								/** UdsTp_req 21 */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
			/* normal(fixed) addressing. */
			pTpSession->nPdu_TxBs  = pRxPacket->msgBuffer[0x01u];	/* FC_BS *//** UdsTp_req 60 *//** UdsTp_req 62 */
			tmp = pRxPacket->msgBuffer[0x02u];						/* FC_STmin */
#else
			/* extended or mixed addressing. */
			pTpSession->nPdu_TxBs  = pRxPacket->msgBuffer[0x02u];	/* FC_BS *//** UdsTp_req 60 *//** UdsTp_req 62 */
			tmp = pRxPacket->msgBuffer[0x03u];						/* FC_STmin */
#endif
			if( 0x00u == tmp)
			{/* Seperation time required by segment data receiver is 0ms. */
				//retVal = E_PROTOCOL_NOT_ALLOWED;
				//return retVal;
				tmp = 1;
			}
            else
            {

            }
			if( 0x00u == pTpSession->nPdu_TxBs)					/** UdsTp_req 61 */
			{/* BS=0: Receiver indicate to the sender that no more FC frames shall be sent during
			  * the transmission of the segmented message.*/
				pTpSession->flagZeroFlowCtrlReq = TRUE;
				/* In this case, during subsequent transmission of CF frames, we do not check "remainBlocks"
				 * but only counting down the "remainConseFrame". */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
				/* normal(fixed) addressing. */
				#ifdef CANFD_TP_FLAG
				tmp3 = tmpRemainBytes / 63u;
				tmp2 = tmpRemainBytes % 63u;
				#else
				tmp3 = tmpRemainBytes / 0x07u;
				tmp2 = tmpRemainBytes % 0x07u;
				#endif
#else
				/* extended or mixed addressing. */
				#ifdef CANFD_TP_FLAG
				tmp3 = tmpRemainBytes / 62u;
				tmp2 = tmpRemainBytes % 62u;
				#else
				tmp3 = tmpRemainBytes / 0x06u;
				tmp2 = tmpRemainBytes % 0x06u;
				#endif
#endif
				/* Get remaining consecutive frames to be transmited for the segmented data. */
				if( tmp2 )
				{
					pTpSession->remainConseFrame = tmp3 + 0x01u;
				}else
				{
					pTpSession->remainConseFrame = tmp3;
				}
			}else
			{/* Receiver indicate to the sender that FC frames will be sent between block transmission
			  * of the segmented data from the sender. */
				pTpSession->flagZeroFlowCtrlReq = FALSE;
				/* calculate block transmission related variables */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
				/* normal(fixed) addressing. */
				#ifdef CANFD_TP_FLAG
				tmp2 = pTpSession->nPdu_TxBs * 63u;			/** UdsTp_req 60 */ /* MISRA 2004 Required Rule 10.1 *//*lint !e960*/
				#else
				tmp2 = pTpSession->nPdu_TxBs * 0x07u;			/** UdsTp_req 60 */ /* MISRA 2004 Required Rule 10.1 *//*lint !e960*/
				#endif
#else
				/* extended or mixed addressing. */
				#ifdef CANFD_TP_FLAG
				tmp2 = pTpSession->nPdu_TxBs * 62u;			/** UdsTp_req 60 */ /* MISRA 2004 Required Rule 10.1 *//*lint !e960*/
				#else
				tmp2 = pTpSession->nPdu_TxBs * 0x06u;			/** UdsTp_req 60 */ /* MISRA 2004 Required Rule 10.1 *//*lint !e960*/
				#endif
#endif
				/* Get remaining blocks and remaining consecutive frames. */
				if( tmpRemainBytes >= tmp2 )
				{/* More than one blocks or just one whole block needed for sending the segmented data. */
					tmp3 = tmpRemainBytes / tmp2;
					tmp2 = tmpRemainBytes % tmp2;
					if( tmp2 )
					{/* The last block have less number of CF frames than value of BlockSize. */
						pTpSession->remainBlocks = tmp3 + 0x01u;
					}else
					{/* The last block have the same number of CF frames as value of BlockSize. */
						pTpSession->remainBlocks = tmp3;
					}
					/* Get remaining consecutive frames for the first block to be received. */
					pTpSession->remainConseFrame = pTpSession->nPdu_TxBs;		/** UdsTp_req 60 */
				}else
				{/* Only one block is enough to send the segmented data. */
					pTpSession->remainBlocks = 1u;
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
					/* normal(fixed) addressing. */
				#ifdef CANFD_TP_FLAG
				tmp3 = tmpRemainBytes / 63u;
				tmp2 = tmpRemainBytes % 63u;
				#else
					tmp3 = tmpRemainBytes / 0x07u;
					tmp2 = tmpRemainBytes % 0x07u;
				#endif
#else
					/* extended or mixed addressing. */
				#ifdef CANFD_TP_FLAG
				tmp3 = tmpRemainBytes / 62u;
				tmp2 = tmpRemainBytes % 62u;
				#else
					tmp3 = tmpRemainBytes / 0x06u;
					tmp2 = tmpRemainBytes % 0x06u;
				#endif
#endif
					/* Get remaining consecutive frames for the first block to be transmited. */
					if( tmp2 )
					{
						pTpSession->remainConseFrame = tmp3 + 0x01u;
					}else
					{
						pTpSession->remainConseFrame = tmp3;
					}
				}
			}

			/* FC_STmin handling. */
			if( tmp <= 0x7Fu )										            /** UdsTp_req 63 */
			{/* Seperation time range: 1ms - 127ms (STmin value of ZERO is returned as protocol error above). */
				pTpSession->nPdu_TxSTmin = (UdsTp_NPduTxSTmin_t)tmp * 1000u;
			}else if( (tmp >= 0xF1u) && (tmp <= 0xF9u))				            /** UdsTp_req 65 */
			{/* Seperation time range: 100us - 900us. */
				pTpSession->nPdu_TxSTmin = (UdsTp_NPduTxSTmin_t)(tmp & 0x0Fu) * 1000u;
			}else 													            /** UdsTp_req 64 *//** UdsTp_req 66 */
			{/* ((tmp >= 0x80u) && (tmp <= 0xF0u)) || (tmp >= 0xFAu) */
				/* Reserved range. */
				pTpSession->nPdu_TxSTmin = ISO15765_MAX_STMIN;	                /** UdsTp_req 68 */
			}
			retVal = E_OK;
			break;
		case ISO15765_FLOW_CTRL_STATUS_WAIT:
			/* Continue to wait for a new FlowControl N_PDU and the server shall restart its N_BS timer. */
			/* If FC frame(FS=WAIT) send by receiver exceeded WFTmax at receiver side, then it will cause here
			 * an N_USData.confirm(<N_Result>=N_TIMEOUT_Bs) because of the missing FlowControl N_PDU from the
 			 * receiver an N_Bs timeout occurs in the sender: The sender(us) will check the timeout in ServiceProc.*/
			retVal = E_PENDING;
			break;
		case ISO15765_FLOW_CTRL_STATUS_OVFLW:				        /** UdsTp_req 54 *//** UdsTp_req 55 *//** UdsTp_req 114 */
			/* Abort the transmission of a segmented message, make an N_USData.confirm service
			 * call(<N_Result>=N_BUFFER_OVFLW).*/
			UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
								pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
								pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_BUFFER_OVFLW));
			retVal = E_FLOWCONTROL_FS_ERR;
			break;
		default:/* FC_FS error handling. */	/** UdsTp_req 19 *//** UdsTp_req 57 *//** UdsTp_req 58 *//** UdsTp_req 59 */
			/* Abort message transmission, and make an N_USData.confirm service
			 * call(<N_Result> = N_INVALID_FS). */
			UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
								pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
								pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_INVALID_FS));
			retVal = E_FLOWCONTROL_FS_ERR;
			break;
	}

	return retVal;
} /* end of UdsTp_GetFlowCtrlFrame() */

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
