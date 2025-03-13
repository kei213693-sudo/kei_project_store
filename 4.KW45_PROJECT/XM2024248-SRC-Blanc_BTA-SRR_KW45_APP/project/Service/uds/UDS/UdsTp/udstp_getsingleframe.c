/*
 * Copyright (c) 2021??????????????
 * All rights reserved.
 * ????: udstp_getsingleframe.c
 * ??: UDS network transport layer get single frame.
 * ????: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * ????: Vx.x.x????/?????????????????????????????
 */

#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udstp_inf.h"
#include "udscan_pub.h"

/* Get TA type of Functional or physical addressing is not necessary for single frame. But temporary we still keep them */
/*lint -esym(438,tmp)*/
/*lint -esym(550,tmp)*/
/*lint -esym(818, pRxPacket)*/

/*
 * ????: This function get single frame the UdsTp module received.
 * ??: \param [in] sessionId        The index of the session which received this frame.
 * \param [in] pRxPacket        The received packet.
 * ???: This frame was processed successfully.
 * ??:
 */
Std_UdsTpReturnType UDS_FN_TYPE UdsTp_GetSingleFrame(
	const uint8_t sessionId,								/*lint !e963*/
	const UdsCan_QueueBufPtr pRxPacket					/*lint !e963*/
)
{
    UdsTp_SessionCfg_t*			pTpSessionCfg	= UdsTp_SessionCfgs + sessionId;
	UdsTp_Session_t*			pTpSession		= UdsTp_Sessions + sessionId;
	UdsTp_NPduMsgData_t*		pPduMsgData		= pTpSessionCfg->pNSduCfg->pNPdu_MsgData;
	uint8_t						dataLength;
	uint8_t						cnt;
	uint8_t						cnt2;
	UdsTp_NTaType_t				tmp;
	Std_UdsTpReturnType			retVal 			= E_PROTOCOL_NOT_ALLOWED;
	uint8_t						getSF_Dlc = 0;
        uint8_t 					CAN_FD_DLC_64[8u] = {8u,12u,16u,20u,24u,32u,48u,64u};
	uint8_t						i;
	uint8_t						Frame_header_length;
	uint8_t						sf_data_dl = 0;
	for ( i = 0; i <= 8; i++)
	{
		if (pRxPacket->dlc <= CAN_FD_DLC_64[i])
		{
			getSF_Dlc = CAN_FD_DLC_64[i];
			break;
		}
	}

	/* Get SF_DL */
if(getSF_Dlc <= CAN_LEN_8U)
{
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	dataLength = ( pRxPacket->msgBuffer[0x00u] & ISO15765_TPCI_SF_DL_MASK );/** UdsTp_req 33 *//** UdsTp_req 106 *//** UdsTp_req 108 */
	cnt = dataLength + 0x01u;	/* The expected DLC value in the received frame. *//** UdsTp_req 109 */
	Frame_header_length = 0x01u;
#else
	/* extended or mixed addressing. */
	dataLength = ( pRxPacket->msgBuffer[0x01u] & ISO15765_TPCI_SF_DL_MASK );/** UdsTp_req 33 *//** UdsTp_req 106 *//** UdsTp_req 108 */
	cnt = dataLength + 0x02u;	/* The expected DLC value in the received frame. *//** UdsTp_req 109 */
	Frame_header_length = 0x02u;
#endif
}else
{
#ifdef CANFD_TP_FLAG
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	dataLength = ( pRxPacket->msgBuffer[0x01u] & ISO15765_TPCI_SF_DL_MASK_FD );/** UdsTp_req 33 *//** UdsTp_req 106 *//** UdsTp_req 108 */
	cnt = dataLength + 0x01u;	/* The expected DLC value in the received frame. ??????��????DLC?*//** UdsTp_req 109 */
	Frame_header_length = 0x02u;
	/*Check the actual length that should be sent*/
	if(0X00 != pRxPacket->msgBuffer[0x00u])
	{
		return retVal;
	}
	else
	{}
	#else
	dataLength = ( pRxPacket->msgBuffer[0x02u] & ISO15765_TPCI_SF_DL_MASK_FD );/** UdsTp_req 33 *//** UdsTp_req 106 *//** UdsTp_req 108 */
	cnt = dataLength + 0x02u;	/* The expected DLC value in the received frame. *//** UdsTp_req 109 */
	Frame_header_length 0x03u;
	#endif
#endif
}
	for ( i = 0; i <= 8; i++)
	{
		if ((dataLength+Frame_header_length) <= CAN_FD_DLC_64[i])
		{
			sf_data_dl = CAN_FD_DLC_64[i];
			break;
		}
		else
		{}
	}
	/*If the DLC sent is not equal to the length that should be sent,
	 * it is judged as an invalid length*/
	if(getSF_Dlc != sf_data_dl)
	{
		return retVal;
	}
	else
	{}

	/* SF_DL error handling. */
	if( (dataLength == 0x00u) || (dataLength > (getSF_Dlc-Frame_header_length)) )	/** UdsTp_req 32 *//** UdsTp_req 35 */
	{
		return retVal;								/** UdsTp_req 36 *//** UdsTp_req 37 */ /* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
	else
	{}
#if (UDSTP0_CAN_ADDRESSINGFORMAT > ADDRESSING_NORMAL_RANGE)
	/* ether normal or normal fixed addressing. */
	else if( dataLength == (getSF_Dlc-1u))								/** UdsTp_req 34 */
	{
		return retVal;										/** UdsTp_req 38 */
	}
	else
	{

	}
#endif

	/* DLC error handling. */
if(getSF_Dlc <= CAN_LEN_8U)
{
#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
	if( pRxPacket->dlc < 0x08u )								/** UdsTp_req 110 */
#elif (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_OFF)
	if( pRxPacket->dlc < cnt )								/** UdsTp_req 111 */
#endif
	{
		/* The reception of a CAN frame with a DLC value smaller than expected
		 * shall be ignored by the network layer without any further action. */
		return retVal;										/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }
}else
{
}
	/* Get type of Functional or physical addressing. */
	tmp = UdsTp_GetTaType( sessionId, pRxPacket );			/* return value is ignored. *//** UdsTp_req 16 *//** UdsTp_req 17 *//** UdsTp_req 23 */

	/* Copy single frame data to N_PDU_Data buffer. */
if(getSF_Dlc <= CAN_LEN_8U)
{
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	for(cnt = 0x01u, cnt2 = 0x00u; cnt < (dataLength + 0x01u); ++cnt, ++cnt2)	/*lint !e838*//* MISRA 2004 Required Rule 12.10,12.1 *//*lint !e960*//*lint !e961*/
#else
	/* extended or mixed addressing. */
	for(cnt = 0x02u, cnt2 = 0x00u; cnt < (dataLength + 0x02u); ++cnt, ++cnt2)	/** UdsTp_req 99 */	/*lint !e838*//* MISRA 2004 Required Rule 12.10,12.1 *//*lint !e960*//*lint !e961*/
#endif
	{
		pPduMsgData->msgData[cnt2] = pRxPacket->msgBuffer[cnt];
	}
}else
{
#ifdef	CANFD_TP_FLAG
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	for(cnt = 0x02u, cnt2 = 0x00u; cnt < (dataLength + 0x02u); ++cnt, ++cnt2)	/*lint !e838*//* MISRA 2004 Required Rule 12.10,12.1 *//*lint !e960*//*lint !e961*/
	#else
	for(cnt = 0x03u, cnt2 = 0x00u; cnt < (dataLength + 0x03u); ++cnt, ++cnt2)	/** UdsTp_req 99 */	/*lint !e838*//* MISRA 2004 Required Rule 12.10,12.1 *//*lint !e960*//*lint !e961*/
	#endif
	{
		pPduMsgData->msgData[cnt2] = pRxPacket->msgBuffer[cnt];
	}
#endif
}
	/* Set number of available bytes in the N_PDU message buffer. */
	pPduMsgData->msgLen = dataLength;

	/* Check whether upper layer is ready to receive this single frame data. */
	if( UdsTpApp_IsDispatchAllowed() )
	{/* Single frame data is receivable by upper layer. */
		/* Forward SF N_PDU to upper layer. */
		UdsTp_NUSData_indication( (uint8_t)(pTpSessionCfg->nPdu_Mtype), pTpSessionCfg->pNPdu_AI->addrClient,
				pTpSessionCfg->pNPdu_AI->addrServer, (uint8_t)(pTpSession->nTaType), pTpSessionCfg->pNPdu_AI->addrExtenClient,
				(uint8_t)(N_OK), (uint16_t)(dataLength), pPduMsgData->msgData ); /** UdsTp_req 10 */
		/* Successfully forward SF N_PDU to upper layer. */
		retVal = E_OK;
	}else
	{/* Single frame data is not receivable for now */
		/* Upper layer not able to receive the data for now. */
		retVal = E_PENDING;
	}

	return retVal;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
