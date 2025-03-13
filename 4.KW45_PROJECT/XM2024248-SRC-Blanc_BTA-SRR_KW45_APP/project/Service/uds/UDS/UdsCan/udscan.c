/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan.c
 * 摘要: CAN Data Link Layer Interface initialisation.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "udstp_confdefs.h"
#include "udstp_confpriv.h"		/*lint !e537*/
#include "udscan_pub.h"

/*
 * 函数介绍: This function initialises the CAN Data Link Layer Interface. It must be called exactly once.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsCan_Initialize( void )
{
    uint8_t					i;
	uint8_t					j;
    uint8_t					sessionId;
    UdsTp_SessionCfg_t*   	pTpSessionCfg     = UdsTp_SessionCfgs;
    UdsCan_SessionCfg_t*  	pCanSessionCfg;

	for( sessionId = 0x00u; sessionId < UDSTP_NUM_SESSIONS; ++sessionId )
	{
		pCanSessionCfg = (UdsCan_SessionCfg_t*)( pTpSessionCfg->pTransportCfg );

		for( i = 0x00u; i < pCanSessionCfg->numChannels; ++i )
		{
			/* Reset both CAN and UdsTp queue positions to the start of the queue. */
			pCanSessionCfg->pQueuePositions[ i ].ctCanPos = pCanSessionCfg->pChannelCfgs[ i ].idxStart;
			pCanSessionCfg->pQueuePositions[ i ].ctUdsPos = pCanSessionCfg->pChannelCfgs[ i ].idxStart;

			/* Reset the queue buffers state corresponding to buffers allocated for this channel. */
			for( j = pCanSessionCfg->pChannelCfgs[ i ].idxStart;
			     j < (pCanSessionCfg->pChannelCfgs[ i ].idxEnd + 0x01u);
			     ++j )
			{
				pCanSessionCfg->pQueueBufferStates[j] = 0x00u;
			}
		}

		++pTpSessionCfg;
	}

	return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
