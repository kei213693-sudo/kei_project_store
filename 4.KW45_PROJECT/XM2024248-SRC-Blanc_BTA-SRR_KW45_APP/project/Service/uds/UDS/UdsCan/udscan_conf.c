/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_conf.c
 * 摘要: Definition of data structures containing the configuration of the CAN Data Link Layer interface.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef UDSCAN_CONF_C
#define UDSCAN_CONF_C

#include "udscan_priv.h"
#include "udscan_confdefs.h"
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
#include "BtaPos.h"
#endif
/*lint -e750*//*lint -e835*/

/* [MISRA 2004 Advisory Rule 12.1] dependence placed on C's operator precedence; operators: '+' and '-' */
/*lint -esym(961,12.1) */

/******************************************************************************
*
* CAN channel structures
*
******************************************************************************/
#if 1
UdsCan_ChannelCfg_t UdsCan0_ChannelCfgs[] =
{
	{/* Rx */
		UDS0_CAN_RX_START,			/* idxStart */
		UDS0_CAN_RX_END,			/* idxEnd */
		UDS0_CAN_RX_MSGID_PHY,		/* msgIdPhy */
		UDS0_CAN_RX_MSGID_FNC,		/* msgIdFnc */
	},
	{/* Tx */
		UDS0_CAN_TX_START,			/* idxStart */
		UDS0_CAN_TX_END,			/* idxEnd */
		UDS0_CAN_TX_MSGID_PHY,		/* msgIdPhy */
		UDS0_CAN_TX_MSGID_FNC,		/* msgIdFnc */
	},
};
#else
UdsCan_ChannelCfg_t UdsCan0_ChannelCfgs[2];
void udscan_conf_deini(void)
{
	UdsCan0_ChannelCfgs[0].idxStart = UDS0_CAN_RX_START;
	UdsCan0_ChannelCfgs[0].idxEnd = UDS0_CAN_RX_END;

	UdsCan0_ChannelCfgs[1].idxStart = UDS0_CAN_RX_START;
	UdsCan0_ChannelCfgs[1].idxEnd = UDS0_CAN_RX_START;
	if(Get_BTAPos() == External_Rear)
	{
		UdsCan0_ChannelCfgs[0].msgIdPhy = 0x675;
		UdsCan0_ChannelCfgs[0].msgIdFnc = 0x601;
		UdsCan0_ChannelCfgs[1].msgIdPhy = 0x6F5;
		UdsCan0_ChannelCfgs[1].msgIdFnc = UDS0_CAN_TX_MSGID_FNC;
	}
	else if(Get_BTAPos() == External_Front)
	{
		UdsCan0_ChannelCfgs[0].msgIdPhy = 0x67C;
		UdsCan0_ChannelCfgs[0].msgIdFnc = 0x601;
		UdsCan0_ChannelCfgs[1].msgIdPhy = 0x6FC;
		UdsCan0_ChannelCfgs[1].msgIdFnc = UDS0_CAN_TX_MSGID_FNC;
	}
	else
	{}
}
#endif
/******************************************************************************
*
* CAN queue structures
*
******************************************************************************/

UdsCan_QueuePos_t		UdsCan0_QueuePositions       [ UDS0_CAN_NUM_CHANNELS ];
UdsCan_QueueBuf_t		UdsCan0_QueueBuffers         [ UDS0_CAN_QUEUEALL_NUMBUF ];
/* Each element has values from UdsCan_QueueBufStates_t. */
UdsCan_QueueBufProps_t	UdsCan0_QueueBufferStates    [ UDS0_CAN_QUEUEALL_NUMBUF ];


/******************************************************************************
*
* CAN session structures
*
******************************************************************************/

UdsCan_SessionCfg_t UdsCan0_SessionCfg =
{
	&UdsCan0_ChannelCfgs[0],		/* pChannelCfgs */
    &UdsCan0_QueuePositions[0],		/* pQueuePositions */
    &UdsCan0_QueueBuffers[0],		/* pQueueBuffers */
    &UdsCan0_QueueBufferStates[0],	/* pQueueBufferStates */
	UDS0_CAN_NUM_CHANNELS,			/* numChannels */
};


#endif /* _UDSCAN_CONF_C */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
