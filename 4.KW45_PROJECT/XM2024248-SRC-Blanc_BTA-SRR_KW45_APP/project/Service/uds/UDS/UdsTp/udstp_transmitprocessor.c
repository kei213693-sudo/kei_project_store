/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_transmitprocessor.c
 * 摘要: UDS network transport layer transmit processor.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confpriv.h"
#include "udscan_pub.h"


/*
 * 函数介绍: This function handles transmit request dispatched from upper layer.
 * 参数: \param [in] sessionId        The index of the session which has pending transmit request.
 * \param [out] pTxPacket       The response packet.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_TransmitProc(
	const uint8_t sessionId,							/*lint !e963*/
	UdsCan_QueueBufPtr pTxPacket
)
{


    UdsTp_SessionCfg_t*			pTpSessionCfg	 = UdsTp_SessionCfgs + sessionId;
	UdsTp_Session_t*			pTpSession		 = UdsTp_Sessions + sessionId;
	UdsTp_NSduCfg_t*			pNSduCfg		 = pTpSessionCfg->pNSduCfg;
	UdsTp_NPduMsgData_t*		pPduMsgData		 = (++pNSduCfg)->pNPdu_MsgData;
#if (UDSTP_TIMING_ENABLE == TRUE)
	UdsTp_TxNTiming_t*			pTxNTiming		 = (UdsTp_TxNTiming_t*)(&((pNSduCfg)->timingData)); /* MISRA 2004 Rule 11.4 *//*lint !e929*/
#endif /* UDSTP_TIMING_ENABLE == TRUE */
	/* pointing to UdsTp<sessionId>_NPdu_MsgData[0x01u]. */
	UdsTp_FrameType_t			frameType;
	uint8_t						handleResult;

	if (NULL == pTxPacket)
	{
		return;
	}
	else
	{
	}
	frameType = UdsTp_SetFrameType( pPduMsgData->msgLen );

	switch( frameType )
	{
		case SINGLE_FRAME:	/* Transmit request can be put into one single frame. */
			pTpSession->sentConfirmed = FALSE;		/* Completion of CAN transmission is pending, confirmation not received. */
			pTpSession->tpState = TX_IN_PROGRESS;
			UdsTp_SendSingleFrame( sessionId, pTxPacket );
#if (UDSTP_TIMING_ENABLE == TRUE)
			UDSTP_TIMER_START(sessionId, pTxNTiming->nAs);			/** UdsTp_req 76 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
			break;
		case FIRST_FRAME:	/* Transmit request have to be seperated into multi-frame fransmission. */
			pTpSession->sentConfirmed = FALSE;	    /* Completion of CAN transmission is pending, confirmation not received. */
			handleResult = UdsTp_SendFirstFrame( sessionId, pTxPacket );
			if( handleResult == E_OK )
			{
#if (UDSTP_TIMING_ENABLE == TRUE)
				UDSTP_TIMER_START(sessionId, pTxNTiming->nAs);		/** UdsTp_req 76 */
#endif /* UDSTP_TIMING_ENABLE == TRUE */
				/* Initialize block reception control variables.*/
				pTpSession->nPdu_Length = pPduMsgData->msgLen;
				pTpSession->nPdu_SeqNum = 0u;
				pTpSession->flagZeroFlowCtrlReq = FALSE;
				pTpSession->tpState = SEG_TX_IN_PROGRESS;
				pTpSession->segTxCtrl = SEG_TX_WAIT_SENT_FF_CON;
			}else
			{/* E_NOT_OK, required length excedded total buffer size. */
				/* Abort the segment transmission, and make an N_USData.confirm servicecall(<N_Result>=N_ERROR).*/
				UdsTp_NUSData_Confirmation( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrServer,
											pTpSessionCfg->pNPdu_AI->addrClient, (uint8_t)(pTpSession->nTaType),
											pTpSessionCfg->pNPdu_AI->addrExtenClient, (uint8_t)(N_ERROR));
				pTpSession->tpState = RX_TX_IDLE;
			}
			break;
		case CONSECUTIVE_FRAME:
		case FLOW_CONTROL_FRAME:
		case NO_FRAME:
		case INVALID_FRAME:	/* In any case the required transmit length is ZERO, we will drop here.  */
		default:
			pTpSession->tpState = RX_TX_IDLE;
			break;
	}

	return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
