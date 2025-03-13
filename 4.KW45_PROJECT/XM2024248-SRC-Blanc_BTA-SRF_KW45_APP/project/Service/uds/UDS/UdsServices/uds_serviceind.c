/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_serviceind.c
 * 摘要: Implements Uds_ServiceIndication().
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_pub.h"

/*lint -esym(818,pPduData)*/

/*
 * 函数介绍: This function push a received new service data onto session buffer.
 * 参数:
 * \param [in] serverType   The type of address information who is generating the request.
 * \param [in] sA        	The address of the server.
 * \param [in] tA        	The address of the client.
 * \param [in] tAType      	The communication mode which shall be used by the communication peer.
 * \param [in] rA        	The remote address of the client.
 * \param [in] result      	The execution result of the service response.
 * \param [in] msgLen       The number of bytes of received data.
 * \param [in] pPduData     The container for the received data.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_ServiceIndication(
		                               uint8_t  serverType,
		                               uint8_t  sA,
		                               uint8_t  tA,
		                               uint8_t  tAType,
		                               uint8_t  rA,
		                               uint8_t  result,
		                               uint16_t msgLen,
		                               uint8_t* pPduData)
{
	Uds_SessionCfg_t*	 pUdsSessionCfg	= &Uds_SessionCfg;
    Uds_StatePtr8        ctUdsTpPos		= &(pUdsSessionCfg->pSessionRxQueuePos->ctUdsTpPos);
    Uds_StatePtr8        pQueueBufState = (Uds_StatePtr8)(pUdsSessionCfg->pSessionRxQueueBufState + *ctUdsTpPos);/*MISRA 2004 Rule 11.4*//*lint !e928*/
	Uds_SessionRxBuf_t*  pSessionRxBuf	= &(pUdsSessionCfg->pSessionRxBuffer[ *ctUdsTpPos ]);
	Uds_APduData_t*		 pAPduData		= pSessionRxBuf->pAPduData;
	/*Check whether the pointer is empty*/
	if (NULL == pPduData)
	{
		return;
	}
	else
	{
		/* code */
	}

	/* Copy the received service data to the current session rx buffer, if the current buffer is free. */
    if( (uint8_t)(UDS_TXRXFREE) == *pQueueBufState )
    {
		pSessionRxBuf->serviceType = SERVICE_INDICATION;
		pSessionRxBuf->serverType  = (Uds_ServerType_t)(serverType);
		pSessionRxBuf->tAType = (Uds_TAType_t)(tAType);
		pSessionRxBuf->sA = sA;
		pSessionRxBuf->tA = tA;
		pSessionRxBuf->rA = rA;
		pSessionRxBuf->result = (UdsTp_NResult_t)(result);
		if( result == 0x00u )
		{/* result = N_OK */
			pAPduData->msgLength = msgLen;
			/* Copy msgLen bytes of data into the current session Rx buffer. */
			Uds_MemCopy( &(pAPduData->pduData[0]), pPduData, (uint32_t)(msgLen) );	/** UDSonCAN_req 80 */
		}
        else
        {

        }
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
		if( result != 0x00u )
		{/* result != N_OK */
			if( Uds_SesnMgr_IsFirstFrameReceived() )
			{/* If this <service>.indication is folowing last <service>.FF-ind, we call sessionManager immediately
			  * to terminate last segment reception(which is indicated by this <service>.indication) and release
			  * session rx buffer since this can be the case that a new single frame or first frame received during
			  * segment reception where it will be dispatched again in <service>.indication soon. */
				Uds_SesnMgr_SessionManager();
			}
			else
			{

			}
		}
		else
		{

		}
    }
    else
    {

    }
    return;
}

/********************************************************************************
*								end of file										*
********************************************************************************/
