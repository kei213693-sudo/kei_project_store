/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_confpriv.h
 * 摘要: Private declarations derived from the UDS Application Layer configuration.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDS_CONFPRIV_H
#define _UDS_CONFPRIV_H

#include "uds_priv.h"

/*MISRA 2004 Required Rule 14.3 - null statement not in line by itself */
/*lint -esym(960,14.3)*/

/******************************************************************************
*
* Preprocessor declarations
*
******************************************************************************/

/******************************************************************************
*
* Type declarations
*
******************************************************************************/

typedef Uds_SessionRxBuf_t* Uds_SessionRxBufPtr;
typedef Uds_SessionTxBuf_t* Uds_SessionTxBufPtr;

/******************************************************************************
*
* Variables declarations
*
******************************************************************************/

extern Uds_SessionCfg_t		Uds_SessionCfg;
extern Uds_SessionRuntime_t Uds_SessionRuntime;
extern Uds_ServiceTable_t	Uds_ServiceTable[];
extern Uds_APduData_t		Uds_APduDataInd[ UDS_SESSIONBUF_RX_NUM ];
extern Uds_APduData_t		Uds_APduDataResp[ UDS_SESSIONBUF_TX_NUM ];

extern UDS_CONFIG_TYPE UDS_DidDatMngRecd_t       Uds_g_DidDataTable[];
extern UDS_CONFIG_TYPE UDS_DidEepromDatMngRecd_t Uds_g_DidEepromDataTable[];
extern UDS_CONFIG_TYPE UDS_IoCtrlIdxTblRcd_t        Uds_g_IoCtrlByDidIdxTable[];
extern UDS_CONFIG_TYPE UDS_IoCtrlMngtDatTblRcd_t    Uds_g_IoCtrlByDidMngtTable[];
extern UDS_CONFIG_TYPE UDS_RdWtMemByAddDatMngRecd_t Uds_g_RdWtMemoryByAddrMngTbl[];
extern UDS_RdDatByPeriodDidMngRecd_t                Uds_g_RdDatByPeriodDidMngTbl[UDS_C_PERIODIC_DID_MAX_NUM];
extern UDS_DynaDefDidMngRecd_t                      Uds_g_DynaDefDidMngTbl[UDS_C_DYNA_DEF_DID_MAX_NUM];
extern UDS_CONFIG_TYPE UDS_ReqDnldMngRecd_t         Uds_g_ReqDnldMngTbl[];
extern UDS_CONFIG_TYPE UDS_RtnCtrlMngtDatTblRcd_t   Uds_g_RtnCtrlMngtDataTable[];
extern UDS_CONFIG_TYPE UDS_ReqUpldMngRecd_t         Uds_g_ReqUpldMngTbl[];

/******************************************************************************
*
* Macro definitions
*
******************************************************************************/

/* Define UDS Diagnostic Application ans Session Layer Timing timer
 *
 */
#define UDS_P2TIMER_START(timer)							\
		Uds_SessionRuntime.timeoutCounter = (timer);		\
		Uds_SessionRuntime.flagP2TimeoutCounter = TRUE;

#define UDS_P2TIMER_STOP()									\
		(Uds_SessionRuntime.flagP2TimeoutCounter = FALSE)

#define UDS_P2TIMER_ISPENDING()								\
		Uds_SessionRuntime.flagP2TimeoutCounter

#define UDS_P2TIMER_ISTIMEOUT()								\
		( Uds_SessionRuntime.timeoutCounter == 0x00u )

#define UDS_P2STAR_CONSENEG_TIMER_ISTIMEOUT()				\
		(( Uds_SessionRuntime.timeoutCounter < Uds_SessionRuntime.P2StarCanServerConsNegResp))

#define UDS_S3TIMER_START(timer)							\
		Uds_SessionRuntime.timeoutCounterS3Server = (timer);\
		Uds_SessionRuntime.flagS3TimeoutCounter = TRUE;

#define UDS_S3TIMER_STOP()									\
		(Uds_SessionRuntime.flagS3TimeoutCounter = FALSE)

#define UDS_S3TIMER_ISPENDING()								\
		Uds_SessionRuntime.flagS3TimeoutCounter

#define UDS_S3TIMER_ISTIMEOUT()								\
		( Uds_SessionRuntime.timeoutCounterS3Server == 0x00u )

#define UDS_TIMER_DECREMENT(timer) 							\
		if ((timer) >= UDS_SESSION_TIMING_TICK_PERIOD) 		\
		{ 													\
			(timer) -= UDS_SESSION_TIMING_TICK_PERIOD;		\
		}else												\
		{													\
			(timer) = 0x00u;								\
		}
#define UDS_TIMER_CLEAR()                           		\
			Uds_SessionRuntime.timeoutCounter = 0;          \
			Uds_SessionRuntime.flagP2TimeoutCounter = TRUE; \
			Uds_SessionRuntime.timeoutCounterS3Server = 0;  \
			Uds_SessionRuntime.flagS3TimeoutCounter = TRUE;
#endif /* _UDS_CONFPRIV_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
