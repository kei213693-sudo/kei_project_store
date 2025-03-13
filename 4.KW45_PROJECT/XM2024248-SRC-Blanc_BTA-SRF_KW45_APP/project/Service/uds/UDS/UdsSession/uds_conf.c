/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_conf.c
 * 摘要: Definition of data structures containing the UDS Application Layer configuration.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef UDS_CONF_C
#define UDS_CONF_C

#include "uds_priv.h"
#include "uds_confdefs.h"

/******************************************************************************
*
* General structures
*
******************************************************************************/

Uds_Seed_t	Uds_SeedNKey[UDS_SECURITYACCESS_SEEDKEY_SIZE];

/******************************************************************************
*
* UDS Session MsgData Buffer structures
*
******************************************************************************/

/* UDS session Rx buffer for
 * <service>.indication
 * <service>.FF_indication
 * <service>.Resp-confirmation
 * <service>.ChangeParameter-confirmation
 */
Uds_SessionRxBuf_t Uds_SessionRxBuf[ UDS_SESSIONBUF_RX_NUM ];

/* UDS session Tx buffer for
 * <service>.response
 * <service>.changeParameter-request
 */
Uds_SessionTxBuf_t Uds_SessionTxBuf[ UDS_SESSIONBUF_TX_NUM ];

Uds_SessionQueueBufPos_t	Uds_SessionRxQueuePos;
Uds_SessionQueueBufPos_t	Uds_SessionTxQueuePos;
Uds_SessionBufState_t		Uds_SessionRxQueueBufState	[ UDS_SESSIONBUF_RX_NUM ];
Uds_SessionBufState_t		Uds_SessionTxQueueBufState	[ UDS_SESSIONBUF_TX_NUM ];

/******************************************************************************
*
* UDS A_PDU.Data Buffers
*
******************************************************************************/

/* UDS A_PDU.Data unit for UDS <service>.indication to be exchanged with Uds services. */
Uds_APduData_t	Uds_APduDataInd[ UDS_SESSIONBUF_RX_NUM ];

/* UDS A_PDU.Data unit for UDS <service>.response to be exchanged with Uds services. */
Uds_APduData_t	Uds_APduDataResp[ UDS_SESSIONBUF_TX_NUM ];

/******************************************************************************
*
* UDS Session Configuration structures
*
******************************************************************************/

Uds_ASduAICfg_t	Uds_ASduAICfg =
{
	UDS_SERVER_TYPE,					/* serverType */
	UDS_SERVER_ADDR,					/* SA */
	UDS_CLIENT_ADDR,					/* TA */
	UDS_REMOTE_ADDR						/* RA */
};

/*
 * UDS Application Layer and Session Layer Timing Parameters compliant to ISO15765-3.
 */
Uds_TimingCfg_t	Uds_TimingCfg =
{
	P2CAN_SERVER_MAX_DEFAULT,						/* P2CanServerMax */
	P2STAR_CAN_SERVER_MAX_DEFAULT,					/* P2StarCanServerMax */
	P2STAR_CAN_SERVER_CONSNEGRESP_DEFAULT,			/* P2StarCanServerConsNegResp */
	SESSION_TIMING_S3SERVER,				/* S3Server */
};

/*
 * Security Levels configuration array for each non-default session.
 *
 * UDS_NUM_NON_DEFAULT_SESSION - number of non-default sessions.
 */
Uds_SecurityAccessCfg_t	Uds_SecurityAccessCfg[UDS_NUM_NON_DEFAULT_SESSION] =
{
	{/* UDS_PROGRAMMING_SESSION */0u},
	{/* extended diagnostic session */
		UDS_SECURITYACCESS_DELAYTIMEONBOOT,		/* delayTimeOnBoot */
		UDS_SECURITYACCESS_DELAYTIME,			/* delayTime 	*/
		UDS_SECURITYACCESS_NUM_ATTDELAY,		/* numAttDelay 	*/
		UDS_SECURITYACCESS_NUM_ATTLOCK,			/* numAttLock 	*/
		UDS_SECURITYACCESS_SEEDKEY_SIZE,		/* seedSize 	*/
		UDS_SECURITYACCESS_SEEDKEY_SIZE,		/* keySize 		*/
#ifdef UDS_SECURITYACCESS_ENABLE_QUALIFYCHECK
		SECURITYACCESS_REQ_CLIENTADDR			/* clientAddr 	*/
#endif /* UDS_SECURITYACCESS_ENABLE_QUALIFYCHECK */
	},
	{/* UDS_SAFTEY_SYSTEM_DIAGNOSTIC_SESSION */0u}
};

/*
 * Session Levels configuration array for each non-default session.
 *
 * UDS_NUM_NON_DEFAULT_SESSION - number of non-default sessions.
 */
Uds_SessionLevelCfg_t	Uds_SessionLevelCfg[UDS_NUM_NON_DEFAULT_SESSION] =
{
	{/* Programming session */
		UDS_PROGRAMMING_SESSION,				/* sessionLevel */
#ifdef UDS_ENABLE_SWITCHSESSION_QUALIFYCHECK
		PROGRAMMING_SESSION_REQ_CLIENTADDR,		/* clientAddr */
#endif /* UDS_ENABLE_SWITCHSESSION_QUALIFYCHECK */
		P2CAN_SERVER_MAX_PGM,						/* sessionP2CanServerMax */
		P2STAR_CAN_SERVER_MAX_PGM,					/* sessionP2StarCanServerMax */
		P2STAR_CAN_SERVER_CONSNEGRESP_PGM,			/* sessionP2StarCanServerConsNegResp */
	},
	{/* extended diagnostic session */
		UDS_EXTENDED_DIAGNOSTIC_SESSION,		/* sessionLevel */
#ifdef UDS_ENABLE_SWITCHSESSION_QUALIFYCHECK
		EXTENDED_SESSION_REQ_CLIENTADDR,		/* clientAddr */
#endif /* UDS_ENABLE_SWITCHSESSION_QUALIFYCHECK */
		P2CAN_SERVER_MAX_EXTENDED,						/* sessionP2CanServerMax */
		P2STAR_CAN_SERVER_MAX_EXTENDED,					/* sessionP2StarCanServerMax */
		P2STAR_CAN_SERVER_CONSNEGRESP_EXTENDED,			/* sessionP2StarCanServerConsNegResp */
	},
	{/* ECU supplier session */
		UDS_ECU_SUPPLIER_SPEC_SESSION,			/* sessionLevel */
#ifdef UDS_ENABLE_SWITCHSESSION_QUALIFYCHECK
		ECU_SUPPLIER_SESSION_REQ_CLIENTADDR,	/* clientAddr */
#endif /* UDS_ENABLE_SWITCHSESSION_QUALIFYCHECK */
		P2CAN_SERVER_MAX_ECUSPL,					/* sessionP2CanServerMax */
		P2STAR_CAN_SERVER_MAX_ECUSPL,				/* sessionP2StarCanServerMax */
		P2STAR_CAN_SERVER_CONSNEGRESP_ECUSPL,		/* sessionP2StarCanServerConsNegResp */
	},
};

/*
 * UDS Session configuration compliant to ISO15765-3.
 */
Uds_SessionCfg_t	Uds_SessionCfg =
{
	&Uds_ASduAICfg,						/* pASdu_AI */
	&Uds_TimingCfg,						/* pUdsTimingCfg */
	&Uds_SecurityAccessCfg[0x00u],			/* pSecurityAccessCfg */
	&Uds_SessionLevelCfg[0x00u],			/* pSessionLevelCfg */
	&Uds_SessionRxQueuePos,				/* pSessionRxQueuePos */
	&Uds_SessionTxQueuePos,				/* pSessionTxQueuePos */
	&Uds_SessionRxQueueBufState[0x00u],		/* pSessionRxQueueBufState */
	&Uds_SessionTxQueueBufState[0x00u],		/* pSessionTxQueueBufState */
	&Uds_SessionRxBuf[0x00u],				/* pSessionRxBuffer */
	&Uds_SessionTxBuf[0x00u],				/* pSessionTxBuffer */
};

/******************************************************************************
*
* UDS Service Table Configuration structures
*
******************************************************************************/

/*
 * Configuration of uds services table.
 *
 * UDS_NUM_OF_SERVICES - Total number of services defined in uds_confdefs.h
 * Each service shall have one corresponding array element configured.
 */
Uds_ServiceTable_t	Uds_ServiceTable[] =
{
 /*      service  pFnc_UdsSvcHdlr                   supported  addrPhy    addrFun    securityLevel            */
 /*      Id                                         Session    Supported  Supported                           */
 /* 0u*/{0x10u,   UdsApp_DiagSessionControl,           0x0Eu,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_DiagSessionControl           */
 /* 1u*/{0x11u,   UdsApp_EcuReset,                     0x0Eu,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_EcuReset                     */
 /* 2u*/{0x14u,   UdsApp_ClearDiagnosticInformation,   0x0Au,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_ClearDiagnosticInformation   */
 /* 3u*/{0x19u,   UdsApp_ReadDTCInformation,           0x0Au,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_ReadDTCInformation           */
 /* 4u*/{0x22u,   UdsApp_ReadDataByIdentifier,         0x0Eu,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_ReadDataByIdentifier         */
 /* 5u*/{0x23u,   UdsApp_ReadMemoryByAddress,          0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_ReadMemoryByAddress          */
 /* 6u*/{0x24u,   UdsApp_ReadScalingDataByIdentifier,  0x0Au,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_ReadMemoryByAddress          */
 /* 7u*/{0x27u,   UdsApp_SecurityAccess,               0x0Eu,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_SecurityAccess               */
 /* 8u*/{0x28u,   UdsApp_ComControl,                   0x08u,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_CommunicationControl         */
 /* 9u*/{0x2Au,   UdsApp_ReadDataByPeriodicIdentifier, 0x08u,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_ReadDataByPeriodicIdentifier */
 /*10u*/{0x2Cu,   UdsApp_DynamicDefineDataIdentifier,  0x0Au,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_DynamicDefineDataIdentifier  */
 /*11u*/{0x2Eu,   UdsApp_WriteDataByIdentifier,        0x0Cu,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L1}, /* Uds_WriteDataByIdentifier        */
 /*12u*/{0x2Fu,   UdsApp_IOControlByIdentifier,        0x08u,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L1}, /* Uds_IOControlByIdentifier        */
 /*13u*/{0x31u,   UdsApp_RoutineControl,               0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L1}, /* Uds_RoutineControl               */
 /*14u*/{0x34u,   UdsApp_RequestDownload,              0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_RequestDownload              */
 /*15u*/{0x35u,   UdsApp_RequestUpload,                0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_RequestUpload                */
 /*16u*/{0x36u,   UdsApp_TransferData,                 0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_TransferData                 */
 /*17u*/{0x37u,   UdsApp_RequestTransferExit,          0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_RequestTransferExit          */
 /*18u*/{0x3Du,   UdsApp_WriteMemoryByAddress,         0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_WriteMemoryByAddress         */
 /*19u*/{0x3Eu,   UdsApp_TesterPresent,                0x0Eu,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_TesterPresent                */
 /*20u*/{0x83u,   UdsApp_AccessTimingParameter,        0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_AccessTimingParameter        */
 /*21u*/{0x84u,   UdsApp_SecuredDataTransmission,      0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_SecuredDataTransmission      */
 /*22u*/{0x85u,   UdsApp_ControlDTCSetting,            0x08u,     TRUE,      TRUE,      UDS_SECURITY_LEVEL_L0}, /* Uds_ControlDTCSetting            */
 /*23u*/{0x86u,   UdsApp_ResponseOnEvent,              0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_ResponseOnEvent              */
 /*24u*/{0x87u,   UdsApp_LinkControl,                  0x08u,     TRUE,      FALSE,     UDS_SECURITY_LEVEL_L0}, /* Uds_LinkControl                  */

}; /* end of table : Uds_ServiceTable_t	Uds_ServiceTable[]   */

/******************************************************************************
*
* UDS Session Runtime structure
*
******************************************************************************/

/* The Runtime State of uds session. */
Uds_SessionRuntime_t	Uds_SessionRuntime =
{
	SESSION_IDLE,						/* sessionState */
	UDS_DEFAULT_SESSION,				/* sessionLevel */
	UDS_SERVICE_IDLE,					/* serviceState */
	(UdsTp_NResult_t)(0),				/* indOrRespConResult */
#ifdef UDS_ENABLE_CHANGEPARAMETER
	N_ParamOK,							/* changeParameterResult */
	FALSE,							/* flagChangeParamConfirm */
#endif /* UDS_ENABLE_CHANGEPARAMETER */
	FALSE,							/* flagFirstFrameReceived */
	FALSE,							/* flagResponseConfirm */
	FALSE,							/* flagP2TimeoutCounter */
	FALSE,							/* flagS3TimeoutCounter */
	FALSE,							/* flagConsecutiveNegResp */
	0x00u,                              /* P2CanSrvrTmrTONum */
	0x00u,									/* timeoutCounter */
	0x00u,									/* timeoutCounterS3Server */
	P2CAN_SERVER_MAX_DEFAULT,					/* P2CanServerMax */
	P2STAR_CAN_SERVER_MAX_DEFAULT,				/* P2StarCanServerMax */
	P2STAR_CAN_SERVER_CONSNEGRESP_DEFAULT,		/* P2StarCanServerConsNegResp */
	SESSION_TIMING_S3SERVER,			/* S3Server */
#ifdef UDS_ENABLE_SECURITYACCESS
	UDS_SECURITY_LEVEL_L0,				/* securityLevel */
	UDS_SECURITY_LEVEL_UNLOCKED,		/* stateUnLock */
	0x00u,									/* delayTimerSecurity */
    0x00u,               					/* seedRemainBytes */
	&Uds_SeedNKey[0x00u]					/* seedOrKey */				/*lint !e708*/ /* MISRA 2004 Rule 9.2 omitted braces within an initializer*//*lint !e940*/
#endif /* UDS_ENABLE_SECURITYACCESS */
};

#endif /* _UDS_CONF_C */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
