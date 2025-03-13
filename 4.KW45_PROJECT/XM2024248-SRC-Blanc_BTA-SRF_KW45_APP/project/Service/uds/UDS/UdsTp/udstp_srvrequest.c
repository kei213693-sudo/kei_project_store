/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_srvrequest.c
 * 摘要: UDS network transport layer service request.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confdefs.h"
#include "udstp_confpriv.h"
#include "udstp_pub.h"

/*lint -esym(818,pMsgData)*/


/*
 * 函数介绍: This function dispatch a transmit request from upper layer.
 * 参数: \param [in] mType        The type of address information who is generating the request.
 * \param [in] nSa        	The address of the server.
 * \param [in] nTa        	The address of the client.
 * \param [in] nTaType      The communication mode which shall be used by the communication peer.
 * \param [in] nAe        	The remote address of the client.
 * \param [in] msgLen       The number of bytes in the request data.
 * \param [in] pMsgData     The container for the request data.
 * 返回值: This frame was processed successfully.
 * 备注:
 */
Std_UdsTpReturnType UDS_FN_TYPE UdsTp_NUSData_Request(
			uint8_t mType, uint8_t nSa, uint8_t nTa,
			uint8_t nTaType, uint8_t nAe,
			uint16_t msgLen, uint8_t* pMsgData
)
{
    UdsTp_SessionCfg_t*			pTpSessionCfg	 = UdsTp_SessionCfgs;
	UdsTp_Session_t*			pTpSession		 = UdsTp_Sessions;
	UdsTp_NSduCfg_t*			pNSduCfg		 = pTpSessionCfg->pNSduCfg;
	UdsTp_NPduMsgData_t*		pPduMsgData		 = (++pNSduCfg)->pNPdu_MsgData;
	uint8_t						sessionId;
	uint8_t						retVal = E_NOT_OK;
	if(NULL == pMsgData)
	{
		return E_NOT_OK;
	}
	else
	{
		/* code */
	}

	for( sessionId = 0x00u; sessionId < UDSTP_NUM_SESSIONS; ++sessionId )
	{
		if( mType == (uint8_t)(pTpSessionCfg->nPdu_Mtype) )	/** UdsTp_req 12 */ /** UdsTp_req 13 */
		{
			if( nSa == pTpSessionCfg->pNPdu_AI->addrServer )
			{
				if( nTa == pTpSessionCfg->pNPdu_AI->addrClient )
				{
					if( nTaType == (uint8_t)(CANTP_PHYSICAL) )
					{
						if( (mType == (uint8_t)(REMOTEDIAGNOSTIC)) && (nAe != pTpSessionCfg->pNPdu_AI->addrExtenClient) ) /** UdsTp_req 13 *//** UdsTp_req 15 */
						{
							return retVal;										/* MISRA 2004 Rule 14.7 *//*lint !e904*/
						}
						else
						{

						}
						break;
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
		}
		else
		{

		}

		++pTpSessionCfg;
		++pTpSession;
	}

    if( UDSTP_NUM_SESSIONS == sessionId)
    {
        /* No matching UdsTp configuration found for the request. */
        return retVal;															/* MISRA 2004 Rule 14.7 *//*lint !e904*/
    }
    else
    {

    }
	if( TRUE == pTpSession->nUSDataTransReq)
	{
		/* Last request not yet processed by UdsTp module. */
		retVal = E_PENDING;
        return retVal;															/* MISRA 2004 Rule 14.7 *//*lint !e904*/
	}
    else
    {

    }
    pTpSession->nTaType = (UdsTp_NTaType_t)(nTaType);
	pPduMsgData->msgLen = msgLen;
	Uds_MemCopy( &pPduMsgData->msgData[0x00u], pMsgData, (uint32_t)(msgLen) );

	/* Set Flag of new N_USData transmit request from upper layer. */
	pTpSession->nUSDataTransReq = TRUE;
	retVal = E_OK;

	return retVal;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
