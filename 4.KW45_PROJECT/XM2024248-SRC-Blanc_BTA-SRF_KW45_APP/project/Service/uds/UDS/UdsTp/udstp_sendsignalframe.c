/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_sendsignalframe.c
 * 摘要: UDS network transport layer send single frame.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udscan_pub.h"

/*
 * 函数介绍: This function send single frame message which the UdsTp module requires to send
 * 参数: \param [in] sessionId   The index of the session which sent this frame.
 * \param [out] pTxPacket       The response packet.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_SendSingleFrame(
	const uint8_t sessionId,										/*lint !e963*/
	UdsCan_QueueBufPtr pTxPacket
)
{

	if (NULL == pTxPacket)
	{
		return;
	}
	else
	{
		/* code */
	}

    UdsTp_SessionCfg_t*			pTpSessionCfg	 = UdsTp_SessionCfgs + sessionId;
	UdsTp_NSduCfg_t*			pNSduCfg		 = pTpSessionCfg->pNSduCfg;
	UdsTp_NPduMsgData_t*		pPduMsgData		 = (++pNSduCfg)->pNPdu_MsgData;
	/* pointing to UdsTp<sessionId>_NPdu_MsgData[1]. */
	uint8_t						numBytes;
	uint8_t						tmp;
    uint8_t						SendSF_Dlc;
	uint8_t 					CAN_FD_DLC_8[8u] = {8u,12u,16u,20u,24u,32u,48u,64u};
    uint8_t                     i;
	numBytes = (uint8_t)pPduMsgData->msgLen;

	UdsTp_PackCanId( pTxPacket );
	if(numBytes<=7)
	{
	  SendSF_Dlc = (numBytes+1);
	}else{
	  SendSF_Dlc = (numBytes+2);
	}
	for ( i = 0; i <= 8; i++)
	{
		if (SendSF_Dlc <= CAN_FD_DLC_8[i])
		{
			SendSF_Dlc = CAN_FD_DLC_8[i];
			break;
		}
	}
	/* Load DLC. */
#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
	pTxPacket->dlc = SendSF_Dlc;								/** UdsTp_req 105 */
#else
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
		/* normal(fixed) addressing. */
		pTxPacket->dlc = numBytes + 0x01u;					/** UdsTp_req 107 */
	#else
		/* extended or mixed addressing. */
		pTxPacket->dlc = numBytes + 0x02u;					/** UdsTp_req 107 */
	#endif
#endif

	/* Load N_PCI. */
if (numBytes <= 7u)
{
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	pTxPacket->msgBuffer[0x00u] = ISO15765_TPCI_SF_DL_MASK & numBytes;	/** UdsTp_req 106 */
#else
	/* extended or mixed addressing. */
	pTxPacket->msgBuffer[0x01u] = ISO15765_TPCI_SF_DL_MASK & numBytes;	/** UdsTp_req 106 */
#endif
}else
{
#ifdef CANFD_TP_FLAG
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	pTxPacket->msgBuffer[0x00u] = 0x00;	/** UdsTp_req 106 */
	pTxPacket->msgBuffer[0x01u] = ISO15765_TPCI_SF_DL_MASK_FD & numBytes;	/** UdsTp_req 106 */
	#else
	pTxPacket->msgBuffer[0x01u] = 0x00;	/** UdsTp_req 106 */
	pTxPacket->msgBuffer[0x02u] = ISO15765_TPCI_SF_DL_MASK_FD & numBytes;	/** UdsTp_req 106 */
	#endif
#endif
}

	/* Load message data */											/** UdsTp_req 92 */
if (numBytes <= 7u)
{
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	numBytes += 0x01u;
	for(tmp = 0x01u; tmp < numBytes; ++tmp)
	{
		pTxPacket->msgBuffer[tmp] = pPduMsgData->msgData[(tmp - 0x01u)];
	}
#else
	/* extended or mixed addressing. */
	#if(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_EXTENDED)
		/* extended addressing. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrClient;		/** UdsTp_req 99 */
	#else
		/* mixed addressing. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrExtenClient;
	#endif
	numBytes += 0x02u;
	for(tmp = 0x02u; tmp < numBytes; ++tmp)
	{
		pTxPacket->msgBuffer[tmp] = pPduMsgData->msgData[(tmp - 0x02u)];
	}
#endif
}else
{
#ifdef CANFD_TP_FLAG
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	numBytes += 0x02u;
	for(tmp = 0x02u; tmp < numBytes; ++tmp)
	{
		pTxPacket->msgBuffer[tmp] = pPduMsgData->msgData[(tmp - 0x02u)];
	}
	#else
	#if(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_EXTENDED)
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrClient;		/** UdsTp_req 99 */
	#else
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrExtenClient;
	#endif
	numBytes += 0x03u;
	for(tmp = 0x03u; tmp < numBytes; ++tmp)
	{
		pTxPacket->msgBuffer[tmp] = pPduMsgData->msgData[(tmp - 0x03u)];
	}
	#endif
#endif
}


#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
	for(; tmp < SendSF_Dlc; ++tmp)
	{
		pTxPacket->msgBuffer[tmp] = UDSTP0_CAN_PADDINGVALUE; //0u;
	}
#endif
	/* Send the frame. */
	( pTpSessionCfg->pTxNext )( sessionId );

	return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
