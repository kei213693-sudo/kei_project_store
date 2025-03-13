/**
*
* \file
*
* \brief UDS network transport layer send consecutive frame.
*
* Copyright ETAS GmbH, Stuttgart.
*
* This file is covered by the licence and disclaimer document which is attached in
* the UDS software package.
*
* \version $Id: Version 1.1 Revision 1 2014-01-14 Tan Yang $
*
*============================================================================*
*                               REVERSION HISTORY                            *
*============================================================================*
*  REVISION     |   DATE        |                         |      AUTHOR      *
*----------------------------------------------------------------------------*
* UDS_1.0 Rev4  | Dec/12/2012   |                         | Yang TAN         *
* Delivery_001                                                               *
*----------------------------------------------------------------------------*
* UDS_1.0 Rev5  | Jan/18/2013   |                         | Yongqiang Deng   *
* Delivery_002                                                               *
*         update the code after 1st round integration with Customer running  *
*         sw project, change detail please refer to                          *
*         UDSonCAN Protocol Stack Release Note_v1_0r5.doc                    *
*                                                                            *
*----------------------------------------------------------------------------*
* UDS_1.2 Rev1 | Nov/07/2013    |                         | Yongqiang Deng   *
* Delivery_005                                                               *
*    to use the unified basic type : uint8_t , uint16_t, uint32_t, uint64_t  *
*    to remove the similar coexisitn basic type for UINT8  uint8 and so on   *
*    and remove some MISRA warning                                           *
*----------------------------------------------------------------------------*
* UDS_1.1 Rev1 | Jan/14/2014    |                         | Yang TAN         *                                                               *
*         Correct one mistake written preprocessing operation as enumerate   *
*         values at line146.                                                 *
*         Correct some mistakes written structure elements as defined values *
*         at line116, line119, line148 and line151.                          *
*                                                                            *
*----------------------------------------------------------------------------*
******************************************************************************/

#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udscan_pub.h"

/**
 * This function send a consecutive frame the UdsTp module required to transmit.
 *
 * \param [in] sessionId        The index of the session which received this frame.
 * \param [out] pTxPacket       The response packet.
 *
 */
void UDS_FN_TYPE UdsTp_SendConsecutiveFrame(
	const uint8_t sessionId,								/*lint !e963*/
	UdsCan_QueueBufPtr pTxPacket
)
{
    UdsTp_SessionCfg_t*			pTpSessionCfg	 = UdsTp_SessionCfgs + sessionId;
	UdsTp_Session_t*			pTpSession		 = UdsTp_Sessions + sessionId;
	UdsTp_NSduCfg_t*			pNSduCfg		 = pTpSessionCfg->pNSduCfg;
	UdsTp_NPduMsgData_t*		pPduMsgData		 = (++pNSduCfg)->pNPdu_MsgData;
	/* pointing to UdsTp<sessionId>_NPdu_MsgData[1]. */
	uint16_t					totalBytes		 = pTpSession->nPdu_Length;
	uint16_t					currentBufPos	 = pTpSession->currentBufPos;
	uint8_t						tmp;
	uint8_t						tmp2;
	uint8_t						SendCF_Len;
	uint8_t						SendCF_dlc;

	if (NULL == pTxPacket)
	{
		return;
	}
	else
	{
		/* code */
	}


	/* Packing of CAN ID */
	UdsTp_PackCanId( pTxPacket );							/** UdsTp_req 31 */
	SendCF_dlc = 8u;
	SendCF_Len = 8u;
#ifdef	CANFD_TP_FLAG
	SendCF_Len = 64u;
	SendCF_dlc = totalBytes-currentBufPos;
        if(SendCF_dlc>63u)
        {
                SendCF_dlc = 64u;
        }
#endif
	/* Load DLC. */
	if( pTpSession->remainConseFrame > 0x01u )
	{/* not a last consecutive frame */
		pTxPacket->dlc = SendCF_dlc;
	}else
	{/* The last consecutive frame */
		if( pTpSession->remainBlocks > 0x01u )
		{/* This is not the last block */
			pTxPacket->dlc = SendCF_dlc;
		}else
		{/* This is the last block */
		 /* remaining bytes of datas may less than compacity of one CF frame. */
#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
			pTxPacket->dlc = SendCF_dlc;									/** UdsTp_req 105 */
#else
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
			/* normal(fixed) addressing. */
			pTxPacket->dlc = totalBytes - currentBufPos + 0x01u;		/** UdsTp_req 107 */
	#else
			/* extended or mixed addressing. */
			pTxPacket->dlc = totalBytes - currentBufPos + 0x02u;		/** UdsTp_req 107 */
	#endif
#endif
		}
	}

	/* Updata Sequence number in session runtime data. */
	if( pTpSession->nPdu_SeqNum != 15u )	/** UdsTp_req 46 *//** UdsTp_req 47 *//** UdsTp_req 48 *//** UdsTp_req 49 */
	{
		pTpSession->nPdu_SeqNum++;
	}else
	{
		pTpSession->nPdu_SeqNum = 0x00u;
	}

	/* Load N_PCI. */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	pTxPacket->msgBuffer[0x00u] = ISO15765_TPCI_CF + pTpSession->nPdu_SeqNum;
#else
	/* extended or mixed addressing. */
	pTxPacket->msgBuffer[0x01u] = ISO15765_TPCI_CF + pTpSession->nPdu_SeqNum;
#endif

	/* Load message data */									/** UdsTp_req 92 */
	if( ( pTpSession->remainConseFrame > 0x01u ) || ( pTpSession->remainBlocks > 0x01u ) )
	{/* not a last consecutive frame or this is the last frame of not the last block */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
		/* normal(fixed) addressing. */
		for(tmp = 0x01u; tmp < SendCF_Len; ++tmp, ++(pTpSession->currentBufPos)) /* [MISRA 2004 Required Rule 12.10-comma operator used] *//*lint !e960*/
		{
			pTxPacket->msgBuffer[tmp] = pPduMsgData->msgData[pTpSession->currentBufPos];
		}


#else
		/* extended or mixed addressing. */
#if(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_EXTENDED)
		/* extended addressing. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrClient;    /** UdsTp_req 14 */ /** UdsTp_req 99 */
#else
		/* mixed addressing. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrExtenClient; /** UdsTp_req 15 */
#endif
		for(tmp = 0x02u; tmp < SendCF_Len; ++tmp, ++(pTpSession->currentBufPos))
		{
			pTxPacket->msgBuffer[tmp] = pPduMsgData->msgData[pTpSession->currentBufPos];
		}


#endif
	}else
	{/* The last consecutive frame */
		/* We guaranteed that the remaining number of bytes can not exceed ability of one CF frame
		 * no matter of the addressing type. */
		tmp2 = (uint8_t)(totalBytes - currentBufPos);		/* remaining bytes to be transmited. */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
		/* normal(fixed) addressing. */
		tmp2 += 0x01u;
		for(tmp = 0x01u; tmp < tmp2; ++tmp, ++(pTpSession->currentBufPos)) /* [MISRA 2004 Required Rule 12.10-comma operator used] *//*lint !e960*/
		{
			pTxPacket->msgBuffer[tmp] = pPduMsgData->msgData[pTpSession->currentBufPos];
		}

		#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
		for(tmp = tmp2; tmp < SendCF_Len; ++tmp) /* [MISRA 2004 Required Rule 12.10-comma operator used] *//*lint !e960*/
		{
			pTxPacket->msgBuffer[tmp] = UDSTP0_CAN_PADDINGVALUE; //0u;
		}
		#endif // end of 		#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)

#else
		/* extended or mixed addressing. */
	#if(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_EXTENDED)
		/* extended addressing. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrClient;    /** UdsTp_req 14 */ /** UdsTp_req 99 */
	#else
		/* mixed addressing. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrExtenClient; /** UdsTp_req 15 */
	#endif
		tmp2 += 0x02u;
		for(tmp = 0x02u; tmp < tmp2; ++tmp, ++(pTpSession->currentBufPos))
		{
			pTxPacket->msgBuffer[tmp] = pPduMsgData->msgData[pTpSession->currentBufPos];
		}

		#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)

		for(tmp = tmp2; tmp < SendCF_Len; ++tmp) /* [MISRA 2004 Required Rule 12.10-comma operator used] *//*lint !e960*/
		{
			pTxPacket->msgBuffer[tmp] = UDSTP0_CAN_PADDINGVALUE; //0u;
		}
		#endif // end of 		#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)


#endif
	}

	pTpSession->remainConseFrame--;

	if( pTpSession->flagZeroFlowCtrlReq == FALSE )
	{/* Segmented data transmission is seperated into blocks. */
		/* Updata remaining blocks of session runtime. */
		if( pTpSession->remainConseFrame == 0x00u )
		{/* The last CF frame of current block is reached. */
			pTpSession->remainBlocks--;
		}
		else
		{

		}
	}
    else
    {

    }
	/* Send the frame. */
	( pTpSessionCfg->pTxNext )( sessionId );

	return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
