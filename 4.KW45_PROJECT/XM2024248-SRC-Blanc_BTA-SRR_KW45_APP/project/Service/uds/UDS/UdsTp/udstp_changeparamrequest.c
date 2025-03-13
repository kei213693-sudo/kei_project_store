/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_changeparamrequest.c
 * 摘要: UDS network transport layer Change parameter request.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_confdefs.h"
#include "udstp_confpriv.h"

#ifdef UDSTP_ENABLE_CHANGEPARAMETER

/*
 * 函数介绍: This function requires to change parameter from upper layer.
 * 参数:
 * \param [in] mType        The type of address information who is generating the request.
 * \param [in] nSa        	The address of the server.
 * \param [in] nTa        	The address of the client.
 * \param [in] nTaType      The communication mode which shall be used by the communication peer.
 * \param [in] nAe        	The remote address of the client.
 * \param [in] param        The type of parameter(STmin, BS) whose value is required to be changed.
 * \param [in] paramValue   The parameter value.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_NChangeParam_Request(				/** UdsTp_req 11 */
		                                    uint8_t mType,
		                                    uint8_t nSa,
		                                    uint8_t nTa,
			                                uint8_t nTaType,
			                                uint8_t nAe,
			                                uint8_t param,
			                                uint8_t paramValue
)
{
    UdsTp_SessionCfg_t*			pTpSessionCfg	= UdsTp_SessionCfgs;
	UdsTp_Session_t*			pTpSession		= UdsTp_Sessions;
	uint8_t						sessionId;
	UdsTp_NResult_ChangeParam_t	reqState 		= N_ParamOK;

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
						if( (mType == (uint8_t)(REMOTEDIAGNOSTIC)) &&
						    (nAe != pTpSessionCfg->pNPdu_AI->addrExtenClient) ) /** UdsTp_req 13 *//** UdsTp_req 15 */
						{/* N_AI not matching available configured network address values, just ignore the request
						  * without send any ChangeParameter confirmation since this case is not define in spec. */
							return;	/* MISRA 2004 Rule 14.7 - Return statement before end of function *//*lint !e904*/
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
    {/* N_AI not matching available configured network address values, just ignore the request
	  * without send any ChangeParameter confirmation since this case is not define in spec. */
        return;	/* MISRA 2004 Rule 14.7 - Return statement before end of function *//*lint !e904*/
    }
    else
    {

    }

    if( SEG_RX_IN_PROGRESS == pTpSession->tpState  )
	{/* change parameter service can't executed since a reception of the message identified by <AI> was taking place. */
		reqState = N_RX_ON;							/** UdsTp_req 115 */
	}else
	{
		if( 0x00u == param)
		{/* Parameter is STmin. */
			/* Validate required parameter value. */
			if( ((paramValue >= 0x80u)&&(paramValue <= 0xF0u)) || (paramValue >= 0xFAu) )
			{/* change parameter service can't executed due to an out-of-range <Parameter_Value>. */
				reqState = N_WRONG_VALUE;
			}else
			{
				pTpSession->nPdu_STmin = paramValue;
			}
		}else if( 0x01u == param)
		{/* Parameter is BlockSize (BS). */
			pTpSession->nPdu_Bs = paramValue;
		}else
		{/* change parameter service can't executed since an undefined <Parameter>. */
			reqState = N_WRONG_PARAMETER;
		}
	}

	UdsTp_NChangeParam_Confirmation( mType,
	                                 nSa,
	                                 nTa,
	                                 nTaType,
	                                 nAe,
	                                 param,
	                                 (uint8_t)(reqState) );

	return;
}

#endif /* UDSTP_ENABLE_CHANGEPARAMETER */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
