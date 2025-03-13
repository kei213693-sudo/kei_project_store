/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_conf.c
 * 摘要: file description.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef UDSTP_CONF_C
#define UDSTP_CONF_C

#include "udstp_priv.h"
#include "udstp_confdefs.h"
#include "udscan_pub.h"

/*lint -e750*/

/******************************************************************************
*
* Session general structures
*
******************************************************************************/

/******************************************************************************
*
* NPdu Message data structures
*
******************************************************************************/

/* Define NPdu message date buffers to be used for reception and transmission of datas
 *
 * length of array buffer is ISO15765_NSDU_MSGDATA_LENGTH
 * array[0]: for reception
 * array[1]: for transmission
 */
UdsTp_NPduMsgData_t UdsTp0_NPdu_MsgData[0x02u];


/******************************************************************************
*
* Session NSdu structures
*
******************************************************************************/

UdsTp_NPduAI_t UdsTp0_NPdu_AI =
{
	UDSTP0_NPDU_SERVER_ADDR,		/* addrServer, uint8_t type server address */
	UDSTP0_NPDU_CLIENT_ADDR,		/* addrClient, uint8_t type client address */
	UDSTP0_NPDU_SERVER_ADDREXTEN,	/* addrExtenServer, uint8_t type server address extention */
	UDSTP0_NPDU_CLIENT_ADDREXTEN	/* addrExtenClient, uint8_t type client address extention */
};

/******************************************************************************
*
* Session data structures
*
******************************************************************************/

extern UdsCan_SessionCfg_t UdsCan0_SessionCfg;

UdsTp_NSduCfg_t UdsTp0_NSduCfgs[] =
{
	{/* reception */
		&UdsTp0_NPdu_MsgData[0x00u],	/* pNPdu_MsgData */
		UDSTP0_NPDU_NAR,			/* nAr */		/*lint !e651*//*lint !e708*/ /* MISRA 2004 Rule 9.2 *//*lint !e940*/
		UDSTP0_NPDU_NBR,			/* nBr */
		UDSTP0_NPDU_NCR,			/* nCr */
	},
	{/* transmission */
		&UdsTp0_NPdu_MsgData[0x01u],	/* pNPdu_MsgData */
		UDSTP0_NPDU_NAS,			/* nAs */		/*lint !e708*/ /* MISRA 2004 Rule 9.2 *//*lint !e940*/
		UDSTP0_NPDU_NBS,			/* nBs */
		UDSTP0_NPDU_NCS,			/* nCs */
	}
};

/* number of element of UdsTp_SessionCfgs is UDSTP_NUM_SESSIONS */
UdsTp_SessionCfg_t UdsTp_SessionCfgs[] =
	{
		{
			&UdsCan0_SessionCfg,				  /* pTransportCfg */
			UdsCan_GetTxBuf,					  /* pGetTxBuf */
			UdsCan_GetRxBuf,					  /* pGetRxBuf */
			UdsCan_TxNext,						  /* pTxNext */
			UdsCan_RxNext,						  /* pRxNext */
			UdsCan_AbortTxRequest,				  /* pTxAbortRequest */
			(UdsTp_NPduMtype_t)UDSTP0_NPDU_MTYPE, /* nPdu_Mtype */
			&UdsTp0_NPdu_AI,					  /* pNPdu_AI */
			UDSTP0_NPDU_STMIN,					  /* nPdu_STmin */
			UDSTP0_NPDU_BLOCKSIZE,				  /* nPdu_Bs */
#ifdef UDSTP0_NPDU_WFTMAX
			UDSTP0_NPDU_WFTMAX,									   /* nPdu_WFTmax */
#endif															   /* UDSTP0_NPDU_WFTMAX */
			(UdsTp_AddressingFormat_t)UDSTP0_CAN_ADDRESSINGFORMAT, /* addressingFormat */
			(UdsTp_PaddingCfg_t)UDSTP0_CAN_PADDINGCFG,			   /* paddingCfg */
			&UdsTp0_NSduCfgs[0x00u],							   /* pNSduCfg */
		},
};

/* number of element of UdsTp_Sessions is UDSTP_NUM_SESSIONS */
UdsTp_Session_t UdsTp_Sessions[UDSTP_NUM_SESSIONS];

#endif /* _UDSTP_CONF_C */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
