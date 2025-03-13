/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_serviceFFind.c
 * 摘要: Implements Uds_ServiceFFIndication().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_pub.h"

/*
 * 函数介绍: This function push a received new first frame data onto session buffer.
 * 参数:
 * \param [in] serverType   The type of address information who is generating the request.
 * \param [in] sA        	The address of the server.
 * \param [in] tA        	The address of the client.
 * \param [in] tAType      	The communication mode which shall be used by the communication peer.
 * \param [in] rA        	The remote address of the client.
 * \param [in] msgLen       The number of bytes of received data.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_ServiceFFIndication(
		                                 uint8_t  serverType,
		                                 uint8_t  sA,
		                                 uint8_t  tA,
		                                 uint8_t  tAType,
		                                 uint8_t  rA,
		                                 uint16_t msgLen
)
{
	Uds_SessionCfg_t*	 pUdsSessionCfg	= &Uds_SessionCfg;
    Uds_StatePtr8        ctUdsTpPos		= &(pUdsSessionCfg->pSessionRxQueuePos->ctUdsTpPos);
    Uds_StatePtr8        pQueueBufState = (Uds_StatePtr8)(pUdsSessionCfg->pSessionRxQueueBufState + *ctUdsTpPos);/*MISRA 2004 Rule 11.4*//*lint !e928*/
	Uds_SessionRxBuf_t*  pSessionRxBuf	= &(pUdsSessionCfg->pSessionRxBuffer[ *ctUdsTpPos ]);
	Uds_APduData_t*		 pAPduData		= pSessionRxBuf->pAPduData;

	/* Copy the received service data to the current session rx buffer, if the current buffer is free. */
    if( (uint8_t)(UDS_TXRXFREE) == *pQueueBufState )
    {
		pSessionRxBuf->serviceType = SERVICE_FF_INDICATION;
		pSessionRxBuf->serverType  = (Uds_ServerType_t)(serverType);
		pSessionRxBuf->tAType = (Uds_TAType_t)(tAType);
		pSessionRxBuf->sA = sA;
		pSessionRxBuf->tA = tA;
		pSessionRxBuf->rA = rA;
		pAPduData->msgLength = msgLen;												/** UDSonCAN_req 79 */

        /* Set current queue buffer to UDS_RX_DATA */
        *pQueueBufState = (uint8_t)(UDS_RX_DATA);

#if(UDS_SESSIONBUF_RX_NUM > 0x01u)
        /* Last buffer in queue? */
        if( *ctUdsTpPos != (uint8_t)(UDS_SESSIONBUF_RX_NUM - 0x01u) )
        {
            /* next queue buffer */
            ++( *ctUdsTpPos );
        }else
        {
            /* Back to queue start */
            *ctUdsTpPos = 0x00u;
        }
#endif
    }
    else
    {

    }
    return;
}

/********************************************************************************
*								end of file										*
********************************************************************************/
