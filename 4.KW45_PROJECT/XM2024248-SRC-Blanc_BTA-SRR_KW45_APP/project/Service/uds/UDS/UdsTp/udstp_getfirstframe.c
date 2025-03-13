/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_getfirstframe.c
 * 摘要: UDS network transport layer get first frame.
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
 * 函数介绍: This function get First frame the UdsTp module received.
 * 参数: \param [in] sessionId        The index of the session which received this frame.
 * \param [in] pRxPacket        The received packet.
 * \param [out] pTxPacket       The response packet.
 * 返回值: This frame was processed successfully.
 * 备注:
 */
Std_UdsTpReturnType UDS_FN_TYPE UdsTp_GetFirstFrame(
	const uint8_t sessionId,									/*lint !e963*/
	const UdsCan_QueueBufPtr pRxPacket,						/*lint !e963*/
	UdsCan_QueueBufPtr pTxPacket
)
{
    UdsTp_SessionCfg_t*			pTpSessionCfg	= UdsTp_SessionCfgs + sessionId;
	UdsTp_Session_t*			pTpSession		= UdsTp_Sessions + sessionId;
	UdsTp_NPduMsgData_t*		pPduMsgData		= pTpSessionCfg->pNSduCfg->pNPdu_MsgData;
#if (UDSTP_TIMING_ENABLE == TRUE)
	UdsTp_RxNTiming_t*			pRxNTiming		= (UdsTp_RxNTiming_t*)(&pTpSessionCfg->pNSduCfg->timingData); /* MISRA 2004 Rule 11.4 *//*lint !e929*/
#endif /* UDSTP_TIMING_ENABLE == TRUE */
	UdsTp_NPduLength_t			dataLength;
	uint16_t					currentBufPos;
	uint16_t						tmp;
	uint16_t						tmp2;
	UdsTp_NPduBs_t				tmpBlockSize;
	UdsTp_NTaType_t				svsResult;
	Std_UdsTpReturnType			retVal 			= E_PROTOCOL_NOT_ALLOWED;
	uint8_t						getFF_Dlc = 0;
	uint8_t 					CAN_FD_DLC_64[8u] = {8u,12u,16u,20u,24u,32u,48u,64u};
	uint8_t						i;
	uint8_t						CAN1_OR_CANFD0;
	/* Get type of Functional or physical addressing. */
	svsResult = UdsTp_GetTaType( sessionId, pRxPacket );					/** UdsTp_req 16 */
	/* Functionally addressed first frames shall be ignored and
	 * no notification to the upper layers of its arrival shall be made. */
	if( CANTP_FUNCTIONAL == svsResult )										/** UdsTp_req 17 *//** UdsTp_req 89 */
	{
		return retVal;														/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }
	for ( i = 0; i <= 8; i++)
	{
		if (pRxPacket->dlc <= CAN_FD_DLC_64[i])
		{
			getFF_Dlc = CAN_FD_DLC_64[i];
			break;
		}
	}
	/* Get FF_DL */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	tmp = (uint16_t)(pRxPacket->msgBuffer[0x00u]);
	tmp &= 0x0fu;
	tmp = (uint16_t)(tmp << 0x08u);
	dataLength = pRxPacket->msgBuffer[0x01u] + tmp;
	dataLength &= ISO15765_NPDU_LENGTH_MASK;					/** UdsTp_req 106 *//** UdsTp_req 108 */
	if (dataLength != 0)
	{
		CAN1_OR_CANFD0 = 1;
	}else{
		CAN1_OR_CANFD0 = 0;
	}
	tmp = getFF_Dlc;					/* The expected DLC value in the received frame. *//** UdsTp_req 109 */
#else
	/* extended or mixed addressing. */
	tmp = (uint16_t)(pRxPacket->msgBuffer[0x01u]);
	tmp &= 0x0fu;
	tmp = (uint16_t)(tmp << 0x08u);
	dataLength = pRxPacket->msgBuffer[0x02u] + tmp;
	dataLength &= ISO15765_NPDU_LENGTH_MASK;					/** UdsTp_req 106 *//** UdsTp_req 108 */
	if (dataLength == 0)
	{
		CAN1_OR_CANFD0 = 1;
	}else{
		CAN1_OR_CANFD0 = 0;
	}
	tmp = getFF_Dlc;					/* The expected DLC value in the received frame. *//** UdsTp_req 109 */
#endif

#ifdef CANFD_TP_FLAG
	if (CAN1_OR_CANFD0 == 0)
	{
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	dataLength == 0X00000000;
	dataLength |= (pRxPacket->msgBuffer[0x02u]) << 0x18;
	dataLength |= (pRxPacket->msgBuffer[0x03u]) << 0x10;
	dataLength |= (pRxPacket->msgBuffer[0x04u]) << 0x08;
	dataLength |= (pRxPacket->msgBuffer[0x05u]);
	dataLength &= ISO15765_NPDU_LENGTH_MASK_CANFD;					/** UdsTp_req 106 *//** UdsTp_req 108 */
	tmp = getFF_Dlc; /** UdsTp_req 109 */
	if (dataLength<=4095u)
	{
		return retVal;
	}
	#else
	dataLength |= (pRxPacket->msgBuffer[0x03u]) << 0x18;
	dataLength |= (pRxPacket->msgBuffer[0x04u]) << 0x10;
	dataLength |= (pRxPacket->msgBuffer[0x05u]) << 0x08;
	dataLength |= (pRxPacket->msgBuffer[0x06u]);
	dataLength &= ISO15765_NPDU_LENGTH_MASK_CANFD;					/** UdsTp_req 106 *//** UdsTp_req 108 */
	tmp = getFF_Dlc; /** UdsTp_req 109 */
	#endif
	}
	else{
   /*CAN1_OR_CANFD0 = 1;CAN或者CANFD普通首帧格式8-64字节*/
#ifdef CANFD_TP_FLAG
        if (62 >= dataLength)
        {
                return retVal;
        }
#endif
	}
#endif

	/* DLC error handling. */
#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
	if( (pRxPacket->dlc <= 0x00u) && (pRxPacket->dlc > 15u) )								/** UdsTp_req 110 */
#elif (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_OFF)
	if( (pRxPacket->dlc <= 0x00u) && (pRxPacket->dlc > 15u) )								/** UdsTp_req 111 */
#endif
	{
		/* The reception of a CAN frame with a DLC value smaller than expected
		 * shall be ignored by the network layer without any further action. */
		return retVal;										/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }
	/* FF_DL error handling. */
	/* In case an FF_DL error detected, the received message will be ignored and no FC frame will be send. */
	if( dataLength < 0x07u )/** UdsTp_req 4 *//** UdsTp_req 5 *//** UdsTp_req 6 *//** UdsTp_req 39 *//** UdsTp_req 43 *//** UdsTp_req 44 */
	{
		return retVal;										/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* ether normal or normal fixed addressing. */
	else if( 0x07u == dataLength )								/** UdsTp_req 40 */
	{
		return retVal;										/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
	else
	{

	}
#endif

	/* A correct first frame (FF) message segment has been received.*/
#if (UDSTP_TIMING_ENABLE == TRUE)
	UDSTP_TIMER_START(sessionId, pRxNTiming->nBr); 			/** UdsTp_req 79 */ /* MISRA 2004 Required Rule 14.10 */ /*lint !e960*/
#endif /* UDSTP_TIMING_ENABLE == TRUE */

	tmpBlockSize	= pTpSession->nPdu_Bs;
	/* Check available receive buffer size for segmentation against received FF_DL. */
	/* In case an buffer length error detected, the received message will be aborted and an FC frame(FS=OverFlow) will be send. */
	if( dataLength > ISO15765_NSDU_MSGDATA_LENGTH )			/** UdsTp_req 7 *//** UdsTp_req 8 *//** UdsTp_req 56 */
	{
		pTpSession->sentConfirmed = FALSE;
		UdsTp_SendFlowCtrlFrame(sessionId,
		                        pTxPacket,
		                        ISO15765_FLOW_CTRL_STATUS_OVFLW,
								tmpBlockSize,
								pTpSession->nPdu_STmin);		/** UdsTp_req 29 *//** UdsTp_req 42 */
		retVal = E_NOT_OK;													/** UdsTp_req 9 */
		return retVal;														/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }
	/* Record the total length of segmented data to be received and initialize related variables. */

	pTpSession->nPdu_SeqNum = 0x00u;											/** UdsTp_req 45 */
	pTpSession->nPdu_Length = dataLength;
if (dataLength<=4095U)
{
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	dataLength -= (getFF_Dlc-2U);								/* number of N_Data is 6 in FF and 7 in CF frame. */
	pTpSession->numBlockBytes = (uint16_t)(tmpBlockSize * (getFF_Dlc-1U));	/* calculate available number of bytes we can receive in one block. */
#else
	dataLength -= (getFF_Dlc-3U);								/* number of N_Data is 5 in FF and 6 in CF frame. */
	pTpSession->numBlockBytes = tmpBlockSize * (getFF_Dlc-2U);	/* calculate available number of bytes we can receive in one block. */ /* MISRA 2004 Required Rule 10.1 *//*lint !e960*/
#endif
}else{
#ifdef CANFD_TP_FLAG
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	dataLength -= (getFF_Dlc-6U);								/* number of N_Data is 6 in FF and 7 in CF frame. */
	pTpSession->numBlockBytes = (uint16_t)(tmpBlockSize * (getFF_Dlc-1U));	/* calculate available number of bytes we can receive in one block. */
#else
	/* extended or mixed addressing. */
	dataLength -= (getFF_Dlc-7U);								/* number of N_Data is 5 in FF and 6 in CF frame. */
	pTpSession->numBlockBytes = tmpBlockSize * (getFF_Dlc-2U);	/* calculate available number of bytes we can receive in one block. */ /* MISRA 2004 Required Rule 10.1 *//*lint !e960*/
#endif
#endif
}
	if( dataLength <= pTpSession->numBlockBytes )
	{/* segmented data can be received within one block. */
		pTpSession->remainBlocks = 0x01u;
		/* Calculate number of frames expected for this block. */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
		tmp2 = dataLength / (getFF_Dlc-1U);
		tmp  = dataLength % (getFF_Dlc-1U);
#else
	/* extended or mixed addressing. */
		tmp2 = dataLength / (getFF_Dlc-2U);
		tmp = dataLength % (getFF_Dlc-2U);
#endif
		if( tmp )
		{
			pTpSession->remainConseFrame = tmp2 + 0x01u;
			pTpSession->numBytesLstFrame = (uint8_t)(tmp);
		}else
		{
			pTpSession->remainConseFrame = tmp2;
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
			/* normal(fixed) addressing. */
			pTpSession->numBytesLstFrame = (getFF_Dlc-1U);
#else
			/* extended or mixed addressing. */
			pTpSession->numBytesLstFrame = (getFF_Dlc-2U);
#endif
		}
	}else
	{/* segmented data can not be received within one block. */
		/* Calculate number of blocks to be received. */
		tmp2 = dataLength / pTpSession->numBlockBytes;
		tmp  = dataLength % pTpSession->numBlockBytes;
		if( tmp )
		{
			pTpSession->remainBlocks = tmp2 + 0x01u;				/*lint !e438*/
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
			/* normal(fixed) addressing. */
			tmp  = tmp % (getFF_Dlc-1U);
#else
			/* extended or mixed addressing. */
			tmp  = tmp % (getFF_Dlc-2U);
#endif
			if( tmp )
			{
				pTpSession->numBytesLstFrame = (uint8_t)(tmp);
			}else
			{
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
				/* normal(fixed) addressing. */
				pTpSession->numBytesLstFrame = (getFF_Dlc-1U);
#else
				/* extended or mixed addressing. */
				pTpSession->numBytesLstFrame = (getFF_Dlc-2U);
#endif
			}
		}else
		{
			pTpSession->remainBlocks = tmp2;					/*lint !e438*/
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
			/* normal(fixed) addressing. */
			pTpSession->numBytesLstFrame = (getFF_Dlc-1U);
#else
			/* extended or mixed addressing. */
			pTpSession->numBytesLstFrame = (getFF_Dlc-2U);
#endif
		}
		/* Calculate number of frames expected for the first CF frame block. */
		pTpSession->remainConseFrame = tmpBlockSize;
	}

	/* Copy First frame data to N_PDU_Data buffer. */

if ( dataLength<=4095U)
{
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	for(tmp = 0x02u, currentBufPos = 0x00u; tmp < getFF_Dlc; ++tmp, ++currentBufPos)	/* MISRA 2004 Required Rule 10.1, 12.10 */ /*lint !e961*//*lint !e960*/
#else
	/* extended or mixed addressing. */
	for(tmp = 0x03u, currentBufPos = 0x00u; tmp < getFF_Dlc; ++tmp, ++currentBufPos)	/* MISRA 2004 Required Rule 10.1, 12.10 */ /*lint !e961*//*lint !e960*/
#endif
	{
		pPduMsgData->msgData[currentBufPos] = pRxPacket->msgBuffer[tmp];		/** UdsTp_req 25 */
	}
	pTpSession->currentBufPos = currentBufPos;
}
else{
#ifdef CANFD_TP_FLAG
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	for(tmp = 0x06u, currentBufPos = 0x00u; tmp < getFF_Dlc; ++tmp, ++currentBufPos)	/* MISRA 2004 Required Rule 10.1, 12.10 */ /*lint !e961*//*lint !e960*/
	#else
	for(tmp = 0x07u, currentBufPos = 0x00u; tmp < getFF_Dlc; ++tmp, ++currentBufPos)	/* MISRA 2004 Required Rule 10.1, 12.10 */ /*lint !e961*//*lint !e960*/
	#endif
	{
		pPduMsgData->msgData[currentBufPos] = pRxPacket->msgBuffer[tmp];		/** UdsTp_req 25 */
	}
	pTpSession->currentBufPos = currentBufPos;
#endif
}
	/* Initialize max number FC wait that can be transmitted consecutively. */
#ifdef UDSTP0_NPDU_WFTMAX
  #if ( UDSTP0_NPDU_WFTMAX != 0x00u )
		pTpSession->counterWFT = UDSTP0_NPDU_WFTMAX;
  #endif /* UDSTP0_NPDU_WFTMAX != 0 */
#endif /* UDSTP0_NPDU_WFTMAX */

	/* Check whether upper layer is ready to receive this first frame data. */
	if( UdsTpApp_IsDispatchAllowed() )
	{/* First frame data is receivable by upper layer. */
		/* Forward FF N_PDU to upper layer. */
		UdsTp_NUSData_FFindication( (uint8_t)(pTpSessionCfg->nPdu_Mtype),
		                            pTpSessionCfg->pNPdu_AI->addrClient,
				                    pTpSessionCfg->pNPdu_AI->addrServer,
				                    (uint8_t)(pTpSession->nTaType),
				                    pTpSessionCfg->pNPdu_AI->addrExtenClient,
				                    pTpSession->nPdu_Length ); /** UdsTp_req 3 *//** UdsTp_req 41 */

		/* Since ready to receive more data then transmit FC N_PDU(FS = CTS) with
		 * BS(value will be ZERO in case only one block expected) and STmin. */
#if (UDSTP_TIMING_ENABLE == TRUE)
		UDSTP_TIMER_STOP( sessionId ); 		/* Stop timer for N_Br. */		/** UdsTp_req 79 */
		UDSTP_TIMER_START(sessionId, pRxNTiming->nAr);						/** UdsTp_req 77 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
		pTpSession->sentConfirmed = FALSE;
		if( dataLength <= pTpSession->numBlockBytes )
		{/* segmented data can be received within one block. */
			UdsTp_SendFlowCtrlFrame(sessionId,
			                        pTxPacket,
			                        ISO15765_FLOW_CTRL_STATUS_CTS,
									0x00u,
									pTpSession->nPdu_STmin);			/** UdsTp_req 27 *//** UdsTp_req 61 */
		}else
		{/* segmented data can not be received within one block. */
			UdsTp_SendFlowCtrlFrame(sessionId,
			                        pTxPacket,
			                        ISO15765_FLOW_CTRL_STATUS_CTS,
									tmpBlockSize,
									pTpSession->nPdu_STmin);	/** UdsTp_req 27 */
		}
		retVal = E_OK;
	}else
	{/* First frame data is not receivable for now */
		retVal = E_PENDING;
	}

	return retVal;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
