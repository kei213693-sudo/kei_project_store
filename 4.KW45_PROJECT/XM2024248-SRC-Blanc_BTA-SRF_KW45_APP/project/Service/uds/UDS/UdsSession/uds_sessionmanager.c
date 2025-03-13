
/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_sessionmanager.c
 * 摘要: UDS Session Layer manager.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_priv.h"
#include "uds_confpriv.h"

#include "uds_inf.h"

static uint8_t UDS_FN_TYPE Uds_SesnMgr_SetPendReProgFlag(void);

/*
 * 函数介绍: This function perform UDS session layer initialization.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_SesnMgr_SessionInitialize( void )
{
	Uds_SessionCfg_t*	  pUdsSessionCfg	 = &Uds_SessionCfg;
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;
	Uds_SessionRxBuf_t*	  pUdsSessionRxBuf	 = pUdsSessionCfg->pSessionRxBuffer;
	Uds_SessionTxBuf_t*	  pUdsSessionTxBuf	 = pUdsSessionCfg->pSessionTxBuffer;
	uint8_t				  i;

	/* Reset UDS session Rx queue positions to the start of the queue. */
	pUdsSessionCfg->pSessionRxQueuePos->ctUdsPos = 0x00u;
	pUdsSessionCfg->pSessionRxQueuePos->ctUdsTpPos = 0x00u;
	for( i = 0x00u; i < UDS_SESSIONBUF_RX_NUM; ++i, ++pUdsSessionRxBuf )	/*MISRA 2004 Required Rule 12.10*//*lint !e960*/
	{
		/* Reset UDS session Rx queue buffers state. */
		pUdsSessionCfg->pSessionRxQueueBufState[i] = (Uds_SessionBufState_t)(0x00u);
		pUdsSessionRxBuf->pAPduData = &(Uds_APduDataInd[i]);
	}

	/* Reset UDS session Tx queue positions to the start of the queue. */
	pUdsSessionCfg->pSessionTxQueuePos->ctUdsPos = 0x00u;
	pUdsSessionCfg->pSessionTxQueuePos->ctUdsTpPos = 0x00u;
	for( i = 0x00u; i < UDS_SESSIONBUF_TX_NUM; ++i, ++pUdsSessionTxBuf )	/*MISRA 2004 Required Rule 12.10*//*lint !e960*/
	{
		/* Reset UDS session Tx queue buffers state. */
		pUdsSessionCfg->pSessionTxQueueBufState[i] = (Uds_SessionBufState_t)(0x00u);
		pUdsSessionTxBuf->pAPduData = &(Uds_APduDataResp[i]);
	}

	pUdsSessionRuntime->sessionState = SESSION_IDLE;		/* Initialize session state to idle */
	pUdsSessionRuntime->serviceState = UDS_SERVICE_IDLE;	/* Initialize service state to idle */

	return;
}

/*
 * 函数介绍: This function return status whether UDS session layer has valid receive buffer to be used.
 * 参数:
 * 返回值:  - zero        No available receive buffer available for now.
 *  - non-zero    Session layer has available receive buffer.
 * 备注:
 */
uint8_t UDS_FN_TYPE Uds_SesnMgr_IsRxBufAvailable( void )
{
    uint8_t lub_returnCode;

	if(NULL == Uds_GetRxBuf())
	{
		/* MISRA 2004 Rule 14.7 - Return statement before end of function *//*lint !e904*/
	    lub_returnCode = 0x01u;

	}else
	{
		/* MISRA 2004 Rule 14.7 - Return statement before end of function *//*lint !e904*/
	    lub_returnCode = 0x00u;

	}
	return lub_returnCode;
}

/*
 * 函数介绍: This function reset UDS session state back to IDLE.
 *
 * - This function is called in case we are trying to transmit packet in Uds_SesnMgr_SessionManager()
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_SesnMgr_ResetSessionState( void )
{
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;

	pUdsSessionRuntime->sessionState = SESSION_IDLE;
}

/*
 * 函数介绍: This function return target address type in current received service data.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
Uds_TAType_t UDS_FN_TYPE Uds_SesnMgr_GetReceivedTaType( void )
{
	Uds_SessionCfg_t*	 pUdsSessionCfg	= &Uds_SessionCfg;
    const uint8_t			 ctUdsQueuePos	= pUdsSessionCfg->pSessionRxQueuePos->ctUdsPos;

	return( pUdsSessionCfg->pSessionRxBuffer[ ctUdsQueuePos ].tAType );
}

/*
 * 函数介绍: 	This function Update the service response requirement for session manager according to
 * the service processing.
 * 参数: \param [in] serviceState		The response action according to current service processing.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_SesnMgr_UpdateServiceRespState( Uds_ServiceState_t serviceState )
{
   	Uds_SessionRuntime_t*	pUdsSessionRuntime = &Uds_SessionRuntime;

	UDS_DISABLE_ALL_INTERRUPTS();
	    pUdsSessionRuntime->serviceState = serviceState;
	UDS_ENABLE_ALL_INTERRUPTS();
}
/*
 * 函数介绍: This function return the current active session level.
 * 参数:
 * 返回值: Current UDS diagnostic session level.
 * 备注:
 */
Uds_SessionLevel_t UDS_FN_TYPE Uds_SesnMgr_GetActiveSession( void )
{
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;
	Uds_SessionLevel_t	  sessionLevel;

	UDS_DISABLE_ALL_INTERRUPTS();
	    sessionLevel = pUdsSessionRuntime->sessionLevel;
	UDS_ENABLE_ALL_INTERRUPTS();

	return ( sessionLevel );
}

/*
 * 函数介绍: This function Set the current active session level.
 * 参数: \param [in] sessionLevel   The required session to be set.
 * 返回值:TRUE:	Set session OK.
 * - FALSE:	Set session failed.
 * 备注:
 */
uint8_t UDS_FN_TYPE Uds_SesnMgr_SetActiveSession( const Uds_SessionLevel_t sessionLevel )
{
	Uds_SessionCfg_t*	  	pUdsSessionCfg		= &Uds_SessionCfg;
	Uds_SessionRuntime_t* 	pUdsSessionRuntime	= &Uds_SessionRuntime;
	uint8_t				  	i;
	uint8_t					retVal = (uint8_t)(TRUE);

	UDS_DISABLE_ALL_INTERRUPTS();
	if( UDS_DEFAULT_SESSION == sessionLevel)
	{/* required to change to default session */
		pUdsSessionRuntime->sessionLevel = sessionLevel;
		UDS_S3TIMER_STOP();						/** UDSonCAN_req 20 */
		/* Copy timing configuration parameters from the session configuration to runtime data structure. */
		pUdsSessionRuntime->P2CanServerMax = pUdsSessionCfg->pUdsTimingCfg->P2CanServerMax;
		pUdsSessionRuntime->P2StarCanServerMax = pUdsSessionCfg->pUdsTimingCfg->P2StarCanServerMax;
		pUdsSessionRuntime->P2StarCanServerConsNegResp = pUdsSessionCfg->pUdsTimingCfg->P2StarCanServerConsNegResp;
	}else
	{/* non-default session. */
		for( i = 0x00u; i<UDS_NUM_NON_DEFAULT_SESSION; ++i )
		{/* Search timing parameters for current required session. */
			if( sessionLevel == pUdsSessionCfg->pSessionLevelCfg[i].sessionLevel )
			{/* found session configuration. */
				break;
			}
			else
			{
			}
		}
		/* Do session level switching according to the check result of known session levels. */
		if( i < UDS_NUM_NON_DEFAULT_SESSION )						/** UDSonCAN_req 11 */
		{/* The required sessionLevel is a known available level. */
			pUdsSessionRuntime->sessionLevel = sessionLevel;
			/* Copy timing configuration parameters from the session configuration to runtime data structure. */
			pUdsSessionRuntime->P2CanServerMax = pUdsSessionCfg->pSessionLevelCfg[i].sessionP2CanServerMax;
			pUdsSessionRuntime->P2StarCanServerMax = pUdsSessionCfg->pSessionLevelCfg[i].sessionP2StarCanServerMax;
			pUdsSessionRuntime->P2StarCanServerConsNegResp = pUdsSessionCfg->pSessionLevelCfg[i].sessionP2StarCanServerConsNegResp;
			UDS_S3TIMER_START( pUdsSessionRuntime->S3Server );		/** UDSonCAN_req 17 */ /* Start S3Server timeing.*/

			if (UDS_PROGRAMMING_SESSION == sessionLevel)
			{
			     retVal = Uds_SesnMgr_SetPendReProgFlag();
			     if (TRUE == retVal)
			     {
			         /* to trigger a warm reset ,after reset,
			             will switch from app sw to boot sw
			           */
#ifdef NIO_PRJ
                     UDS_APP_ITF_IMP_SET_ECURST_SOFT_RESET();
#endif
			     }
                 else
                 {
                 }
			}
			else
			{
			}
		}else
		{
			/*Intersession mode*/
			if (UDS_INTERNAL_SESSION == sessionLevel)
			{
				pUdsSessionRuntime->sessionLevel = sessionLevel;
				/* Copy timing configuration parameters from the session configuration to runtime data structure. */
				pUdsSessionRuntime->P2CanServerMax = pUdsSessionCfg->pUdsTimingCfg->P2CanServerMax;
				pUdsSessionRuntime->P2StarCanServerMax = pUdsSessionCfg->pUdsTimingCfg->P2StarCanServerMax;
				pUdsSessionRuntime->P2StarCanServerConsNegResp = pUdsSessionCfg->pUdsTimingCfg->P2StarCanServerConsNegResp;
				UDS_S3TIMER_START(pUdsSessionRuntime->S3Server); /** UDSonCAN_req 17 */ /* Start S3Server timeing.*/
			}
			else
			{
				/* The required sessionLevel is unknown, do not set the required level. */
				retVal = (uint8_t)(FALSE);
			}
		}
	}
	UDS_ENABLE_ALL_INTERRUPTS();

	return(retVal);
}

/*
 * 函数介绍: This function Get P2CAN_Server value for current activated session.
 * 参数:
 * 返回值: P2CAN_Server value.
 * 备注:
 */
uint32_t UDS_FN_TYPE Uds_SesnMgr_GetP2CanServerValue( void )
{
	Uds_SessionRuntime_t* 	pUdsSessionRuntime	= &Uds_SessionRuntime;

	return( (uint32_t)((pUdsSessionRuntime->P2CanServerMax) / 1000u) );
}


/*
 * 函数介绍: This function Get P2StarCAN_Server value for current activated session.
 * 参数:
 * 返回值: P2StarCAN_Server value.
 * 备注:
 */
uint32_t UDS_FN_TYPE Uds_SesnMgr_GetP2StarCanServerValue( void )
{
	Uds_SessionRuntime_t* 	pUdsSessionRuntime	= &Uds_SessionRuntime;

	return( (uint32_t)((pUdsSessionRuntime->P2StarCanServerMax) / 1000u) );
}

/*
 * 函数介绍: This function return whether received Uds <service>.FF-ind.
 *
 * It means a <service>.indication reflacting the result of segment reception
 * is expected to be received.
 * 参数:
 * 返回值: TRUE:	 A <service>.FF-ind already received.
 *	- FALSE: No <service>.FF-ind already received.
 * 备注:
 */
uint8_t UDS_FN_TYPE Uds_SesnMgr_IsFirstFrameReceived( void )
{
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;

	return ( pUdsSessionRuntime->flagFirstFrameReceived );
}

/*
 * 函数介绍: This function return whether received Uds <service>.response-confirmation for last response.
 * 参数:
 * 返回值: - TRUE:	 Response confirmation received.
 *	- FALSE: Response confirmation not received yet.
 * 备注:
 */
uint8_t UDS_FN_TYPE Uds_SesnMgr_IsRespConReceived( void )					/** UDSonCAN_req 77 */
{
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;

	return ( pUdsSessionRuntime->flagResponseConfirm );
}

/*
 * 函数介绍: This function return result of <service>.indication or <service>.resp-confirmation.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_FN_TYPE Uds_SesnMgr_GetRespConResult( void )
{
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;

	return ( (uint8_t)(pUdsSessionRuntime->indOrRespConResult) );
}

#ifdef UDS_ENABLE_CHANGEPARAMETER

/*
 * 函数介绍: This function return whether received <change parameter>.request-confirmation.
 * 参数:
 * 返回值: TRUE:	Change parameter request confirmation received.
 * - FALSE:	Change parameter request confirmation not received yet.
 * 备注:
 */
uint8_t UDS_FN_TYPE Uds_SesnMgr_IsChangeParamConReceived( void )
{
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;

	return ( pUdsSessionRuntime->flagChangeParamConfirm );
}

/*
 * 函数介绍: This function return result of <change parameter>.request-confirmation.
 * 参数:
 * 返回值: - 0:		N_ParamOK, service execution successfully completed.
 * - 1:		N_RX_ON, service didn't executed since a reception of the message identified by <AI> was taking place.
 * - 2:		N_WRONG_PARAMETER, service didn't executed due to an undefined <Parameter>.
 * - 3:		N_WRONG_VALUE, service didn't executed due to an out-of-range <Parameter_Value>.
 * 备注:
 */
uint8_t UDS_FN_TYPE Uds_SesnMgr_GetChangeParamResult( void )
{
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;

	return ( (uint8_t)(pUdsSessionRuntime->changeParameterResult) );
}
#endif /* UDS_ENABLE_CHANGEPARAMETER */

/*
 * 函数介绍: This function check the preconditions required to perform before apply new session.
 *   if customer define the OEM/Supplier specific session changing pre-condition
 *   this function must be updated and redesign.
 * 参数: \param [in] i_CurrSession        Current active session.
 * \param [in] i_DestSession        The required session to set.
 * 返回值: - TRUE:		This precondition for apply new session is fullfilled.
 *	- FALSE:	This precondition for apply new session is not fullfilled.
 * 备注:
 */
uint8_t UDS_FN_TYPE Uds_SesnMgr_ChkSesnChangePreCndIsAllow(
		uint8_t i_CurrSession,
		uint8_t i_DestSession
)
{
	uint8_t lub_returnCode;

	switch (i_DestSession)
	{
		default:
		case UDS_SAFTEY_SYSTEM_DIAGNOSTIC_SESSION:
		case UDS_OEM_VEHCLE_MANU_SPEC_SESSION:
		case UDS_ECU_SUPPLIER_SPEC_SESSION:
		     lub_returnCode = (uint8_t)FALSE;
		     break;
		case UDS_DEFAULT_SESSION:
		case UDS_PROGRAMMING_SESSION:
		case UDS_EXTENDED_DIAGNOSTIC_SESSION:
		     lub_returnCode = (uint8_t)TRUE;
		     break;
	}

	return lub_returnCode;
}
/*lint -restore*/
/*
 * 函数介绍: This function perform UDS session layer management.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_SesnMgr_SessionManager( void )
{
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;
	Uds_SessionLevel_t	  currentSession;
	Uds_ServiceState_t	  serviceState;
	Uds_SessionRxBufPtr	  pRxPacket;
	Uds_SessionTxBufPtr	  pTxPacket;
	Uds_ServiceType_t	  serviceRxType;

	UDS_DISABLE_ALL_INTERRUPTS();
	currentSession  = pUdsSessionRuntime->sessionLevel;
	serviceState	= pUdsSessionRuntime->serviceState;
	UDS_ENABLE_ALL_INTERRUPTS();

	/* pointing to corresponding buffer for this session who has available received message. */
	pRxPacket = Uds_GetRxBuf();

	if( pRxPacket )
	{/* New data received. */
		/* Get service type (indication, FF-indication, response-confirm, changeParameter-confirm). */
		serviceRxType = pRxPacket->serviceType;
	}else
	{

		// if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_DEFAULT_SESSION)
		// {
		// 	UdsApi_setDiagFrameReqFlag(0);
		// }
		// else
		// {
		// 	/* code */
		// }
		serviceRxType = NO_SERVICE;
	}
	/* No need to distinguish between physical communication and functional communication
	 * during default session. */
	/** UDSonCAN_req 1 *//** UDSonCAN_req 2 *//** UDSonCAN_req 3 *//** UDSonCAN_req 4 *//** UDSonCAN_req 5 */
	/** UDSonCAN_req 6 */
	switch( pUdsSessionRuntime->sessionState )
	{
		case SESSION_IDLE:
			/* When default session is idle, we will wait for <service>.indication. */
			if( SERVICE_INDICATION == serviceRxType)
			{/* <service>.ind received (ether single frame or multi-frame). */
				if( pUdsSessionRuntime->flagFirstFrameReceived == TRUE )
				{/* If the received <service>.indication is folowing last <service>.FF-ind, we clear the flag. */
					pUdsSessionRuntime->flagFirstFrameReceived = FALSE;
				}
				else
				{

				}
				/* Warning 613: pRxPacket will always asigned pointer when serviceRxType equals SERVICE_INDICATION. */
				pUdsSessionRuntime->indOrRespConResult = pRxPacket->result;				/*lint !e613*/
				if( pRxPacket->result == (UdsTp_NResult_t)(0) )							/*lint !e613*/
				{/* N_Result is N_OK */
					/** UDSonCAN_req 27-1 *//** UDSonCAN_req 36 *//** UDSonCAN_req 42 *//** UDSonCAN_req 46 */
					/** UDSonCAN_req 49-1 *//** UDSonCAN_req 50 *//** UDSonCAN_req 53 *//** UDSonCAN_req 62 */
					/** UDSonCAN_req 65-4 */
					UDS_P2TIMER_START( pUdsSessionRuntime->P2CanServerMax );		/* Start P2CAN_Server timing. */
					if( currentSession != UDS_DEFAULT_SESSION )
					{/* non-default session*/
						/** UDSonCAN_req 19 *//** UDSonCAN_req 39 *//** UDSonCAN_req 45 *//** UDSonCAN_req 65-1 */
						UDS_S3TIMER_STOP();	 				/* Stop S3Server timing */
					}
					else
					{

					}
					serviceState = UDS_SERVICE_ACCEPTED;	/* Set flag to new service indication available in session rx buffer. */
					/* pointing to corresponding buffer for this session who is free to allocate message to transmit. */
					pTxPacket = Uds_GetTxBuf();
					if( pTxPacket )
					{
						Uds_ServiceProc( pRxPacket, pTxPacket );
						/* Check service process results. */
						serviceState = pUdsSessionRuntime->serviceState;
						if( (UDS_SERVICE_RESP_AVAILABLE == serviceState)||
							(UDS_SERVICE_NO_RESPONSE == serviceState)	||
							(UDS_SERVICE_78HEX_NEG_RESP == serviceState) )
						{/* Service processing finished. */
							if( serviceState != UDS_SERVICE_NO_RESPONSE )
							{/* We need to transmit <service>.response which is available in A_PDU data container. */
								pUdsSessionRuntime->flagResponseConfirm = FALSE;
								/** UDSonCAN_req 27-2 *//** UDSonCAN_req 28 *//** UDSonCAN_req 29 *//** UDSonCAN_req 30 */
								/** UDSonCAN_req 37 *//** UDSonCAN_req 43 *//** UDSonCAN_req 51 *//** UDSonCAN_req 52 */
								/** UDSonCAN_req 54 *//** UDSonCAN_req 55 *//** UDSonCAN_req 63 *//** UDSonCAN_req 65-5 */
								if( UDS_SERVICE_RESP_AVAILABLE == serviceState)
								{/* We need to transmit <service>.response which is available in A_PDU data container. */
									pUdsSessionRuntime->sessionState = SESSION_WAIT_RESPCON;
								}else if( serviceState == UDS_SERVICE_78HEX_NEG_RESP ) /** UDSonCAN_req 31 *//** UDSonCAN_req 56 */
								{/* We need to transmit negative response(78hex) and start enhanced response timing window. */
									pUdsSessionRuntime->flagConsecutiveNegResp = FALSE;
									pUdsSessionRuntime->sessionState = SESSION_WAIT_78HEX_NEG_RESPCON;		/** UDSonCAN_req 7 */
								}else
								{ /* Empty, just to suppress MISRA warnning.*/
								}
								Uds_TxNext( SERVICE_RESPONSE );				/* Deliver service response to transport layer. */
								UDS_S3TIMER_START( pUdsSessionRuntime->S3Server );	/* Start S3Server timeing.*/
							}else
							{/* UDS_SERVICE_NO_RESPONSE: Finished service processing and no response required. */
								pUdsSessionRuntime->sessionState = SESSION_IDLE;
								if( currentSession != UDS_DEFAULT_SESSION )
								{/* non-default session*/
									/** UDSonCAN_req 23 *//** UDSonCAN_req 41 *//** UDSonCAN_req 47-2 *//** UDSonCAN_req 49-3 */
									/** UDSonCAN_req 64 *//** UDSonCAN_req 66-2 *//** UDSonCAN_req 66-3 */
									UDS_S3TIMER_START( pUdsSessionRuntime->S3Server );	/* Start S3Server timeing.*/
								}
								else
								{
								}
							}
							serviceState = UDS_SERVICE_IDLE;
							UDS_P2TIMER_STOP();	/** UDSonCAN_req 43 *//** UDSonCAN_req 51 *//** UDSonCAN_req 52 *//* Stop P2CAN_Server timer*/
							Uds_RxNext();							/* Discard session rx buffer data. */
						}else
						{/* Service processing not finished, serviceState remains UDS_SERVICE_ACCEPTED. */
							pUdsSessionRuntime->sessionState = SESSION_WAIT_RESPONSE;	/* Waiting for response from service processing. */
						}
					}else
					{/* No session transmit buffer available for now, we will process the received <service>.indication
					  * next time by again check the availablity of transmit buffer. */
						/* serviceState remains UDS_SERVICE_ACCEPTED. */
						pUdsSessionRuntime->sessionState = SESSION_WAIT_TXBUF;			/* Waiting for session transmit buffer. */
					}
				}else
				{/* N_Result is not N_OK */
					/* Ignore the request, discard session rx buffer data. */	/** UDSonCAN_req 0 *//** UDSonCAN_req 74 */
					if( currentSession != UDS_DEFAULT_SESSION )
					{/* non-default session*/
						/** UDSonCAN_req 24 *//** UDSonCAN_req 74 */
						UDS_S3TIMER_START( pUdsSessionRuntime->S3Server );		/* Start S3Server timing */
					}
					else
					{
					}
					serviceState = UDS_SERVICE_IDLE;
					pUdsSessionRuntime->sessionState = SESSION_IDLE;
					Uds_RxNext();								/* Discard session rx buffer data. */
				}
			}else if( SERVICE_FF_INDICATION == serviceRxType)
			{/* First frame indication received. */
				pUdsSessionRuntime->flagFirstFrameReceived = TRUE;
				if( currentSession != UDS_DEFAULT_SESSION )
				{/* non-default session*/
					/** UDSonCAN_req 18 *//** UDSonCAN_req 39 *//** UDSonCAN_req 65-1 *//** UDSonCAN_req 65-3 */
					UDS_S3TIMER_STOP();					 		/* Stop S3Server timing */
				}
				else
				{
				}
				Uds_RxNext();									/* Discard session rx buffer data. */
			}
#ifdef UDS_ENABLE_CHANGEPARAMETER
			else if( SERVICE_CHANGEPARAM_CONF == serviceRxType)
			{/* Change parameter confirmation received. */
				/* Warning 613: pRxPacket will always asigned pointer when serviceRxType equals SERVICE_CHANGEPARAM_CONF. */
				pUdsSessionRuntime->flagChangeParamConfirm = TRUE;
				pUdsSessionRuntime->changeParameterResult = pRxPacket->paramChangeResult;			/*lint !e613*/
				Uds_RxNext();									/* Discard session rx buffer data. */
			}
#endif /* UDS_ENABLE_CHANGEPARAMETER */
			else if( SERVICE_RESPONSE_CONF == serviceRxType)
			{/* Just discard the rx buffer because this is not a expected reception. */
				Uds_RxNext();									/* Discard session rx buffer data. */
			}else
			{/* NO_SERVICE received during SESSION_IDLE. */
				if( currentSession != UDS_DEFAULT_SESSION )
				{/* non-default session*/
					if( (UDS_S3TIMER_ISPENDING()) && (UDS_S3TIMER_ISTIMEOUT()) )
					{/* S3Server timing started and S3Server timer timeout occurs. */
							/* Back into default session, return value from Uds_SesnMgr_SetActiveSession() is ignored. */
							Uds_SesnMgr_SetActiveSession( (uint8_t)(UDS_DEFAULT_SESSION) );			/*lint !e534*/
							/* Notify uds application (services) layer that the S3_Server_Timer timeout occured. */
							UDS_SESN_MGR_ITF_IMP_CB_FUNC_S3_SVR_TMR_TO_NOTIFY();
					}
					else
					{

					}
				}
				else
				{

				}
			}
			break;
		case SESSION_WAIT_TXBUF:
			/* serviceState remains UDS_SERVICE_ACCEPTED. */
			/* Actually P2Can_Server timing already started. */
			if( (UDS_P2TIMER_ISPENDING()) && (UDS_P2TIMER_ISTIMEOUT()) )
			{/* P2Can_Server timeout occurs. */
				/* P2CAN_Server timeout in server handling: N/A. */	/** UDSonCAN_req 75 */
			}
			else
			{

			}
			/* pointing to corresponding buffer for this session who is free to allocate message to transmit. */
			pTxPacket = Uds_GetTxBuf();
			if( pTxPacket )
			{
				Uds_ServiceProc( pRxPacket, pTxPacket );
				/* Check service process results. */
				serviceState = pUdsSessionRuntime->serviceState;
				if( (UDS_SERVICE_RESP_AVAILABLE == serviceState)||
					(UDS_SERVICE_NO_RESPONSE == serviceState)	||
					(UDS_SERVICE_78HEX_NEG_RESP == serviceState) )
				{/* Service processing finished. */
					if( serviceState != UDS_SERVICE_NO_RESPONSE )
					{/* We need to transmit <service>.response which is available in A_PDU data container. */
						pUdsSessionRuntime->flagResponseConfirm = FALSE;
						/** UDSonCAN_req 27-2 *//** UDSonCAN_req 28 *//** UDSonCAN_req 29 *//** UDSonCAN_req 30 */
						/** UDSonCAN_req 37 *//** UDSonCAN_req 43 *//** UDSonCAN_req 51 *//** UDSonCAN_req 52 */
						/** UDSonCAN_req 54 *//** UDSonCAN_req 55 *//** UDSonCAN_req 63 *//** UDSonCAN_req 65-5 */
						if( UDS_SERVICE_RESP_AVAILABLE == serviceState)
						{/* We need to transmit <service>.response which is available in A_PDU data container. */
							pUdsSessionRuntime->sessionState = SESSION_WAIT_RESPCON;
						}else if( UDS_SERVICE_78HEX_NEG_RESP == serviceState) /** UDSonCAN_req 31 *//** UDSonCAN_req 56 */
						{/* We need to transmit negative response(78hex) and start enhanced response timing window. */
							pUdsSessionRuntime->flagConsecutiveNegResp = FALSE;
							pUdsSessionRuntime->sessionState = SESSION_WAIT_78HEX_NEG_RESPCON;		/** UDSonCAN_req 7 */
						}else
						{/* Empty, just to suppress MISRA warnning.*/
						}
						Uds_TxNext( SERVICE_RESPONSE );				/* Deliver service response to transport layer. */
					}else
					{/* UDS_SERVICE_NO_RESPONSE: Finished service processing and no response required. */
						pUdsSessionRuntime->sessionState = SESSION_IDLE;
						if( currentSession != UDS_DEFAULT_SESSION )
						{/* non-default session*/
							/** UDSonCAN_req 23 *//** UDSonCAN_req 41 *//** UDSonCAN_req 47-2 *//** UDSonCAN_req 49-3 */
							/** UDSonCAN_req 64 *//** UDSonCAN_req 66-2 *//** UDSonCAN_req 66-3 */
							UDS_S3TIMER_START( pUdsSessionRuntime->S3Server );	/* Start S3Server timeing.*/
						}
						else
						{

						}
					}
					serviceState = UDS_SERVICE_IDLE;
					UDS_P2TIMER_STOP();	/** UDSonCAN_req 43 *//** UDSonCAN_req 51 *//** UDSonCAN_req 52 *//* Stop P2CAN_Server timer*/
					Uds_RxNext();							/* Discard session rx buffer data. */
				}else
				{/* Service processing not finished, serviceState remains UDS_SERVICE_ACCEPTED. */
					pUdsSessionRuntime->sessionState = SESSION_WAIT_RESPONSE;	/* Waiting for response from service processing. */
				}
			}else
			{
				Uds_TxNext( SERVICE_RESPONSE );				/* Deliver the pending service response to transport layer. */
			}
			break;
		case SESSION_WAIT_RESPONSE:
			/* Waiting for response from service processing(both response required and not required). */
			if( (UDS_P2TIMER_ISPENDING()) && (UDS_P2TIMER_ISTIMEOUT()) )
			{/* P2Can_Server timing is started and P2Can_Server timeout occurs. */
				/* P2CAN_Server timeout in server handling: N/A. */					/** UDSonCAN_req 75 */
			}
			else
			{

			}
			/* Check service process results. */
			if( (UDS_SERVICE_RESP_AVAILABLE == serviceState)||
				(UDS_SERVICE_NO_RESPONSE == serviceState)	||
				(UDS_SERVICE_78HEX_NEG_RESP ==serviceState) )
			{/* Service processing finished. */
				if( serviceState != UDS_SERVICE_NO_RESPONSE )
				{/* We need to transmit <service>.response which is available in A_PDU data container. */
					pUdsSessionRuntime->flagResponseConfirm = FALSE;
					/** UDSonCAN_req 27-2 *//** UDSonCAN_req 28 *//** UDSonCAN_req 29 *//** UDSonCAN_req 30 *//** UDSonCAN_req 37 */
					/** UDSonCAN_req 43 *//** UDSonCAN_req 51 *//** UDSonCAN_req 52 *//** UDSonCAN_req 54 *//** UDSonCAN_req 55 */
					/** UDSonCAN_req 63 *//** UDSonCAN_req 65-5 */
					if( UDS_SERVICE_RESP_AVAILABLE == serviceState)
					{/* We need to transmit <service>.response which is available in A_PDU data container. */
						pUdsSessionRuntime->sessionState = SESSION_WAIT_RESPCON;
					}else if( serviceState == UDS_SERVICE_78HEX_NEG_RESP ) /** UDSonCAN_req 31 *//** UDSonCAN_req 56 */
					{/* We need to transmit negative response(78hex) and start enhanced response timing window. */
						pUdsSessionRuntime->flagConsecutiveNegResp = FALSE;
						pUdsSessionRuntime->sessionState = SESSION_WAIT_78HEX_NEG_RESPCON;		/** UDSonCAN_req 7 */
					}else
					{/* Empty, just to suppress MISRA warnning.*/
					}
					Uds_TxNext( SERVICE_RESPONSE );				/* Deliver service response to transport layer. */
				}else
				{/* UDS_SERVICE_NO_RESPONSE: Finished service processing and no response required. */
					pUdsSessionRuntime->sessionState = SESSION_IDLE;
					if( currentSession != UDS_DEFAULT_SESSION )
					{/* non-default session*/
						/** UDSonCAN_req 23 *//** UDSonCAN_req 41 *//** UDSonCAN_req 47-2 *//** UDSonCAN_req 49-3 */
						/** UDSonCAN_req 64 *//** UDSonCAN_req 66-2 *//** UDSonCAN_req 66-3 */
						UDS_S3TIMER_START( pUdsSessionRuntime->S3Server );	/* Start S3Server timeing.*/
					}
					else
					{

					}
				}
				serviceState = UDS_SERVICE_IDLE;
				UDS_P2TIMER_STOP();	/** UDSonCAN_req 43 *//** UDSonCAN_req 51 *//** UDSonCAN_req 52 *//* Stop P2CAN_Server timer*/
				Uds_RxNext();				/* Discard session rx buffer data. */
			}
			break;
		case SESSION_WAIT_RESPCON:
			/* When waiting for <service>.confirmation, we will check type of received information from reansport layer. */
			/* We are guaranteed by transport layer that no new <service>.indication will come before
			 * got the confirmation. */
			/* During SESSION_WAIT_RESPCON, serviceState is UDS_SERVICE_IDLE(setted in SESSION_WAIT_RESPONSE). */
			if( SERVICE_RESPONSE_CONF == serviceRxType)
			{/* <service>.resp-confirmaton received (ether single frame or multi-frame). */
				pUdsSessionRuntime->sessionState = SESSION_IDLE;
				if( currentSession == UDS_DEFAULT_SESSION )
				{/* default session*/
					/** UDSonCAN_req 35 *//** UDSonCAN_req 50-2 *//** UDSonCAN_req 54 *//** UDSonCAN_req 55 *//** UDSonCAN_req 61 */
					pUdsSessionRuntime->flagResponseConfirm = TRUE;
				}else
				{/* non-default session*/
					/* If N_USData.con from network layer with a negative result value(N_Result not N_OK), Server
					 * shall restart S3Server timer and must not retransmit the response message.*//** UDSonCAN_req 76 */
					pUdsSessionRuntime->flagResponseConfirm = TRUE;
					/** UDSonCAN_req 14 *//** UDSonCAN_req 16 *//** UDSonCAN_req 21 *//** UDSonCAN_req 38 */
					/** UDSonCAN_req 41 *//** UDSonCAN_req 44 *//** UDSonCAN_req 47-1 *//** UDSonCAN_req 49-2 */
					/** UDSonCAN_req 64 *//** UDSonCAN_req 66-1 *//** UDSonCAN_req 66-3 *//** UDSonCAN_req 72 */
					UDS_S3TIMER_START( pUdsSessionRuntime->S3Server );			/* Start S3Server timeing.*/
				}
				/* Warning 613: pRxPacket will always asigned pointer when serviceRxType equals SERVICE_RESPONSE_CONF. */
				pUdsSessionRuntime->indOrRespConResult = pRxPacket->result;				/*lint !e613*/
				Uds_RxNext();				/* Discard session rx buffer data. */
			}
			else
			{
				pUdsSessionRuntime->sessionState = SESSION_IDLE; //2023/3/14新增，解决78等待过程中收到报文，导致传输处理逻辑出错问题.
			}
			break;
		case SESSION_WAIT_78HEX_NEG_RESPCON:
			/* When waiting for <service>.confirmation, we will check type of received information from reansport layer. */
			/* We are guaranteed by transport layer that no new <service>.indication will come before
			 * got the confirmation. */
			/* During SESSION_WAIT_RESPCON, serviceState is UDS_SERVICE_IDLE(setted in SESSION_WAIT_RESPONSE). */
			if( SERVICE_RESPONSE_CONF == serviceRxType)					/** UDSonCAN_req 22 */
			{/* <service>.resp-confirmaton received. */
				/* Warning 613: pRxPacket will always asigned pointer when serviceRxType equals SERVICE_RESPONSE_CONF. */
				pUdsSessionRuntime->flagResponseConfirm = TRUE;
				pUdsSessionRuntime->indOrRespConResult = pRxPacket->result;	/*lint !e613*/
				pUdsSessionRuntime->sessionState = SESSION_WAIT_RESPONSE_AFTER78HEX;
				UDS_P2TIMER_START( pUdsSessionRuntime->P2StarCanServerMax ); /** UDSonCAN_req 32 *//** UDSonCAN_req 57 */
				Uds_RxNext();					/* Discard session rx buffer data. */
			}
			else
			{
				/*MISRA warning */
			}
			/* Check whether the service processing can provide response data. */
			if( UDS_SERVICE_RESP_AVAILABLE == serviceState)
			{/* final response ready in A_PDU data buffer. */
				pUdsSessionRuntime->flagResponseConfirm = FALSE;
				pUdsSessionRuntime->sessionState = SESSION_WAIT_RESPCON;
				serviceState = UDS_SERVICE_IDLE;
				pUdsSessionRuntime->P2CanSrvrTmrTONum = 0x00u;
				UDS_P2TIMER_STOP();				/** UDSonCAN_req 59 */ /* Stop P2StarCAN_Server timer*/
				Uds_TxNext( SERVICE_RESPONSE ); /** UDSonCAN_req 34 *//** UDSonCAN_req 58 */ /* Deliver service response to transport layer. */
			}
			else
			{
				/*MISRA warning */
			}
			break;

		case SESSION_WAIT_RESPONSE_AFTER78HEX:
			/* We are waiting for final <service>.response after previously transmitted negative
			 * response(78hex) indicated by received <service>.response-confirmation. */
			/* Check whether the service processing can provide response data. */
			if( UDS_SERVICE_RESP_AVAILABLE == serviceState)
			{/* final response ready in A_PDU data buffer. */
				/* Deliver service response to transport layer. */
				pUdsSessionRuntime->flagResponseConfirm = FALSE;
				pUdsSessionRuntime->sessionState = SESSION_WAIT_RESPCON;
				serviceState = UDS_SERVICE_IDLE;
				pUdsSessionRuntime->P2CanSrvrTmrTONum = 0x00u;
				UDS_P2TIMER_STOP();				/** UDSonCAN_req 59 *//* Stop P2StarCAN_Server timer*/
				Uds_TxNext( SERVICE_RESPONSE ); /** UDSonCAN_req 34 *//** UDSonCAN_req 58 */
			}else
			{/* still can not deliver final response( serviceState is ether "UDS_SERVICE_78HEX_NEG_RESP" or "UDS_SERVICE_IDLE"). */
				if(FALSE ==  pUdsSessionRuntime->flagConsecutiveNegResp)
				{/* This is the first negative response(78hex) timing window. */
					if( UDS_P2STAR_CONSENEG_TIMER_ISTIMEOUT() )
					{/* P2StarCan_Server timeout occurs. */
					     pUdsSessionRuntime->P2CanSrvrTmrTONum++;
						pUdsSessionRuntime->flagConsecutiveNegResp = TRUE;
						/* Deliver service response to transport layer. */
						pUdsSessionRuntime->flagResponseConfirm = FALSE;
						pUdsSessionRuntime->sessionState = SESSION_WAIT_78HEX_NEG_RESPCON;
						UDS_P2TIMER_STOP();				/* Stop P2StarCAN_Server timer*/
						Uds_TxNext( SERVICE_RESPONSE );	/** UDSonCAN_req 7 *//** UDSonCAN_req 33 *//** UDSonCAN_req 60 */
					}
					else
					{

					}
				}else
				{/* This is consecutive negative response(78hex) timing window. */
					if( UDS_P2STAR_CONSENEG_TIMER_ISTIMEOUT() )
					{/* P2StarCanServerConsNegResp timeout occurs. */

						if (pUdsSessionRuntime->P2CanSrvrTmrTONum < (P2CAN_SERVER_MAX_ALLOWED_TO_NUM - 0x01u))
						{
						    pUdsSessionRuntime->P2CanSrvrTmrTONum++;
						/* Deliver service response to transport layer. */
						pUdsSessionRuntime->flagResponseConfirm = FALSE;
							pUdsSessionRuntime->sessionState = SESSION_WAIT_78HEX_NEG_RESPCON;
						UDS_P2TIMER_STOP();				/* Stop P2StarCAN_Server timer*/
							Uds_TxNext( SERVICE_RESPONSE );	/** UDSonCAN_req 7 *//** UDSonCAN_req 33 *//** UDSonCAN_req 60 */
						}
						else  /* if (pUdsSessionRuntime->P2CanSrvrTmrTONum < (P2CAN_SRV_MAX_ALLOWED_TO_NUM - 0x01u)) */
						{
						    /* notify the maximum 0x78 pending timeout event to the upper uds service handling code
						      */
						    if (pUdsSessionRuntime->P2CanSrvrTmrTONum == (P2CAN_SERVER_MAX_ALLOWED_TO_NUM - 0x01u))
						    {
							      pUdsSessionRuntime->P2CanSrvrTmrTONum++;

						             UDS_SESN_MGR_ITF_IMP_CB_FUNC_P2S_MAX_TO_NOTIFY();
							       /* read out this serviceState into local variable immediately */
                                                        UDS_DISABLE_ALL_INTERRUPTS();
 	                                                        serviceState	= pUdsSessionRuntime->serviceState;
	                                                 UDS_ENABLE_ALL_INTERRUPTS();
						    }
						    else
						    {
						    }
						} /* end of else of if (pUdsSessionRuntime->P2CanSrvrTmrTONum < (P2CAN_SRV_MAX_ALLOWED_TO_NUM - 0x01u)) */

					}
					else
					{

					}
				}
			}
			break;
		default:/* We will never trop here in normal cases. */
			break;
	} /* end of switch( sessionState ) */

	UDS_DISABLE_ALL_INTERRUPTS();
	   pUdsSessionRuntime->serviceState = serviceState;
	UDS_ENABLE_ALL_INTERRUPTS();

	return;
}

/*
 * 函数介绍: this is a stub interface function for setting the reprogramming flag for the 0x02 programming session
 * 参数:
 * 返回值: {*}
 * 备注:
 */
static uint8_t UDS_FN_TYPE Uds_SesnMgr_SetPendReProgFlag(void)
{
   uint8_t lub_returnCode = FALSE;

   lub_returnCode = TRUE;

   return lub_returnCode;

}/* end of Uds_SesnMgr_SetPendReProgFlag */


/******************************************************************************
*                               end of file                                   *
******************************************************************************/
