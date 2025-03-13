/**
*
* \file
*
* \brief UDS network transport layer send first frame.
*
* Copyright ETAS GmbH, Stuttgart.
*
* This file is covered by the licence and disclaimer document which is attached in
* the UDS software package.
*
* \version $Id: Version 1.0 Revision 4 2012-12-12 Tan Yang $
*
*============================================================================*
*                               REVERSION HISTORY                            *
*============================================================================*
*  REVISION     |   DATE        |                         |      AUTHOR      *
*----------------------------------------------------------------------------*
* UDS_1.0 Rev4  | Dec/12/2012   |                         | Yang TAN         *
* Delivery_001                                                               *
*                                                                            *
*----------------------------------------------------------------------------*
* UDS_1.2 Rev1 | Nov/07/2013    |                         | Yongqiang Deng   *
* Delivery_005                                                               *
*    to use the unified basic type : uint8_t , uint16_t, uint32_t, uint64_t  *
*    to remove the similar coexisitn basic type for UINT8  uint8 and so on   *
*    and remove some MISRA warning                                           *
*----------------------------------------------------------------------------*
******************************************************************************/

#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udscan_pub.h"

/**
 * This function send First frame the UdsTp module required to transmit.
 *
 * \param [in] sessionId        The index of the session which received this frame.
 * \param [out] pTxPacket       The response packet.
 *
 * \return
 *  - This frame was processed successfully.
 */
Std_UdsTpReturnType UDS_FN_TYPE UdsTp_SendFirstFrame(
	const uint8_t sessionId,									/*lint !e963*/
	UdsCan_QueueBufPtr pTxPacket
)
{
    UdsTp_SessionCfg_t*			pTpSessionCfg	= UdsTp_SessionCfgs + sessionId;
	UdsTp_Session_t*			pTpSession		= UdsTp_Sessions + sessionId;
	UdsTp_NSduCfg_t*			pNSduCfg		 = pTpSessionCfg->pNSduCfg;
	UdsTp_NPduMsgData_t*		pPduMsgData		 = (++pNSduCfg)->pNPdu_MsgData;
	/* pointing to UdsTp<sessionId>_NPdu_MsgData[1]. */
	UdsTp_NPduLength_t			numBytes;
	Std_UdsTpReturnType			retVal 			= E_NOT_OK;
    uint8_t                     iPCI_bit;
	numBytes = pPduMsgData->msgLen;

	if (NULL == pTxPacket)
	{
		return E_NOT_OK ;
	}
	else
	{
		/* code */
	}

	if( numBytes > ISO15765_NSDU_MSGDATA_LENGTH )
	{/* number of bytes of required transmit data exceeded total N_SDUData buffer length. */
		return retVal;										/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }
	/* Packing of CAN ID */
	UdsTp_PackCanId( pTxPacket );							/** UdsTp_req 31 */

	/* Load DLC. */
	pTxPacket->dlc = 8u;
	iPCI_bit = 0x02u;
	/* Load N_PCI. */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	pTxPacket->msgBuffer[0x00u] = ISO15765_TPCI_FF + (uint8_t)(numBytes >> 0x08u);
	pTxPacket->msgBuffer[0x01u] = (uint8_t)(numBytes & 0x00FFu);
#else
	/* extended or mixed addressing. */
	pTxPacket->msgBuffer[0x01u] = ISO15765_TPCI_FF + (uint8_t)(numBytes >> 0x08u);
	pTxPacket->msgBuffer[0x02u] = (uint8_t)(numBytes & 0x00FFu);
#endif

#ifdef CANFD_TP_FLAG
	if (numBytes <= 4095u)
	{
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	pTxPacket->msgBuffer[0x00u] = ISO15765_TPCI_FF + (uint8_t)(numBytes >> 0x08u);
	pTxPacket->msgBuffer[0x01u] = (uint8_t)(numBytes & 0x00FFu);
	#else
	pTxPacket->msgBuffer[0x01u] = ISO15765_TPCI_FF + (uint8_t)(numBytes >> 0x08u);
	pTxPacket->msgBuffer[0x02u] = (uint8_t)(numBytes & 0x00FFu);
	#endif
	}
	else{
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	pTxPacket->msgBuffer[0x00u] = ISO15765_TPCI_FF;
	pTxPacket->msgBuffer[0x01u] = 0X00;
	pTxPacket->msgBuffer[0x02u] = (uint8_t)(numBytes & 0x000000FFu);
	pTxPacket->msgBuffer[0x03u] = (uint8_t)((numBytes & 0x0000FF00u) >> 0x08u);
	pTxPacket->msgBuffer[0x04u] = (uint8_t)((numBytes & 0x00FF0000u) >> 0x10u);
	pTxPacket->msgBuffer[0x05u] = (uint8_t)((numBytes & 0xFF000000u) >> 0x18u);
	#else
	pTxPacket->msgBuffer[0x01u] = ISO15765_TPCI_FF;
	pTxPacket->msgBuffer[0x02u] = 0X00;
	pTxPacket->msgBuffer[0x03u] = (uint8_t)(numBytes & 0x000000FFu);
	pTxPacket->msgBuffer[0x04u] = (uint8_t)((numBytes & 0x0000FF00u) >> 0x08u);
	pTxPacket->msgBuffer[0x05u] = (uint8_t)((numBytes & 0x00FF0000u) >> 0x10u);
	pTxPacket->msgBuffer[0x06u] = (uint8_t)((numBytes & 0xFF000000u) >> 0x18u);
	#endif
	iPCI_bit = 0x06u;
	}
	pTxPacket->dlc = 64u;
#endif
	/* Load message data */									/** UdsTp_req 92 */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	for(numBytes = iPCI_bit; numBytes < pTxPacket->dlc; ++numBytes)
	{
		pTxPacket->msgBuffer[numBytes] = pPduMsgData->msgData[(numBytes - iPCI_bit)];
	}
#else
	/* extended or mixed addressing. */
	#if(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_EXTENDED)
		/* extended addressing. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrClient;				/** UdsTp_req 99 */
	#else
		/* mixed addressing. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrExtenClient;
	#endif
	for(numBytes = (iPCI_bit+1); numBytes < pTxPacket->dlc; ++numBytes)
	{
		pTxPacket->msgBuffer[numBytes] = pPduMsgData->msgData[(numBytes - (iPCI_bit+1))];
	}
#endif

	/* Update position pPduMsgData.msgData[1] from where we will send remaining bytes
	 * by subsequent CF frames. */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	pTpSession->currentBufPos = numBytes - iPCI_bit;
#else
	/* extended or mixed addressing. */
	pTpSession->currentBufPos = numBytes - (iPCI_bit+1);
#endif

	/* Send the frame. */
	( pTpSessionCfg->pTxNext )( sessionId );

	retVal = E_OK;
	return retVal;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
