/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_sendflowcontrol.c
 * 摘要: UDS network transport layer send flow control frame.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udscan_pub.h"

/*
 * 函数介绍: This function send Flow control frame.
 * 参数: \param [in] sessionId   The index of the session which received this frame.
 * \param [in] pTxPacket        The transmit packet.
 * \param [in] flowStatus       The FlowStatus to be transmited.
 * \param [in] blockSize        The BlockSize to be transmited.
 * \param [in] stMin        	The STmin to be transmited.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_SendFlowCtrlFrame(
	const uint8_t sessionId,											/*lint !e963*/
	UdsCan_QueueBufPtr pTxPacket,
	uint8_t flowStatus,
	uint8_t blockSize,
	uint8_t stMin
)
{
	if (NULL == pTxPacket || NULL == pTxPacket)
	{
		return;
	}
	else
	{
		/* code */
	}

    UdsTp_SessionCfg_t*	pTpSessionCfg = UdsTp_SessionCfgs + sessionId;

	/* A send Flow Control frame will alway use physical addressing. */
	UdsTp_PackCanId( pTxPacket );

	/* Load DLC */
#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
	pTxPacket->dlc = 0x08u;									/** UdsTp_req 105 */
#else
	#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
		/* normal(fixed) addressing. */
		pTxPacket->dlc = 0x03u;								/** UdsTp_req 107 */
	#else
		/* extended or mixed addressing. */
		pTxPacket->dlc = 0x04u;								/** UdsTp_req 107 */
	#endif
#endif

	/* Load message data */												/** UdsTp_req 92 */
#if (UDSTP0_CAN_ADDRESSINGFORMAT <= ADDRESSING_NORMAL_RANGE)
	/* normal(fixed) addressing. */
	pTxPacket->msgBuffer[0x00u] = ISO15765_TPCI_FC + (ISO15765_TPCI_FC_FS_MASK & flowStatus);
	pTxPacket->msgBuffer[0x01u] = blockSize;
	pTxPacket->msgBuffer[0x02u] = stMin;
	#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
		pTxPacket->msgBuffer[0x03u] = UDSTP0_CAN_PADDINGVALUE; //0u;
		pTxPacket->msgBuffer[0x04u] = UDSTP0_CAN_PADDINGVALUE; //0u;
		pTxPacket->msgBuffer[0x05u] = UDSTP0_CAN_PADDINGVALUE; //0u;
		pTxPacket->msgBuffer[0x06u] = UDSTP0_CAN_PADDINGVALUE; //0u;
		pTxPacket->msgBuffer[0x07u] = UDSTP0_CAN_PADDINGVALUE; //0u;
	#endif
#else
	/* extended or mixed addressing. */
	#if(UDSTP0_CAN_ADDRESSINGFORMAT == ADDRESSING_EXTENDED)
		/* extended addressing - 29bit ID. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrClient;      /** UdsTp_req 14 */ /** UdsTp_req 99 */
	#else
		/* mixed addressing - 11bit or 29bit ID. */
		pTxPacket->msgBuffer[0x00u] = pTpSessionCfg->pNPdu_AI->addrExtenClient; /** UdsTp_req 15 */
	#endif
	pTxPacket->msgBuffer[0x01u] = ISO15765_TPCI_FC + (ISO15765_TPCI_FC_FS_MASK & flowStatus);
	pTxPacket->msgBuffer[0x02u] = blockSize;
	pTxPacket->msgBuffer[0x03u] = stMin;
	#if (UDSTP0_CAN_PADDINGCFG == CAN_FRAME_DATA_PADDING_ON)
		pTxPacket->msgBuffer[0x04u] = UDSTP0_CAN_PADDINGVALUE; //0u;
		pTxPacket->msgBuffer[0x05u] = UDSTP0_CAN_PADDINGVALUE; //0u;
		pTxPacket->msgBuffer[0x06u] = UDSTP0_CAN_PADDINGVALUE; //0u;
		pTxPacket->msgBuffer[0x07u] = UDSTP0_CAN_PADDINGVALUE; //0u;
	#endif
#endif

	/* Send the Flow Control frame. */
	( pTpSessionCfg->pTxNext )( sessionId );

	return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
