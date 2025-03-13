/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_confpriv.h
 * 摘要: Private declarations derived from the UDS network transport layer configuration.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDSTP_CONFPRIV_H
#define _UDSTP_CONFPRIV_H

#include "uds_target.h"
#include "udstp_priv.h"

/*lint -esym(752, UdsTp_SessionCfgs) */
/*lint -esym(752, UdsTp_Sessions) */
/*lint -esym(750, _UDSTP_CONFPRIV_H) */
/*lint -esym(750, UDSTP_TIMER_START) */
/*lint -esym(750, UDSTP_TIMER_STOP) */
/*lint -esym(750, UDSTP_TIMER_ISPENDING) */
/*lint -esym(750, UDSTP_TIMER_ISTIMEOUT) */
/*lint -esym(750, UDSTP_TIMER_DECREMENT) */

/* [MISRA 2004 Advisory Rule 19.7] Function-like macro defined */
/*lint -esym(961,19.7) */

/* [MISRA 2004 Required Rule 14.3] null statement not in line by itself */
/*lint -esym(960,14.3) */

/******************************************************************************
*
* Preprocessor declarations
*
******************************************************************************/

/******************************************************************************
*
* Variables declarations
*
******************************************************************************/

extern UdsTp_SessionCfg_t	UdsTp_SessionCfgs[];
extern UdsTp_Session_t		UdsTp_Sessions[];

/******************************************************************************
*
* Macro definitions
*
******************************************************************************/

/* Define Network Transport layer NTiming timer
 *
 */
#define UDSTP_TIMER_START(sessionId, timer)							\
		UdsTp_Sessions[ (sessionId) ].timeoutCounter = (timer);		\
		UdsTp_Sessions[ (sessionId) ].flagTimeoutCounter = TRUE;

#define UDSTP_TIMER_STOP(sessionId)									\
		UdsTp_Sessions[ (sessionId) ].flagTimeoutCounter = FALSE;

#define UDSTP_TIMER_ISPENDING(sessionId)							\
		UdsTp_Sessions[ (sessionId) ].flagTimeoutCounter

#define UDSTP_TIMER_ISTIMEOUT(sessionId)							\
		(UdsTp_Sessions[ (sessionId) ].timeoutCounter == 0x00u)

#define UDSTP_TIMER_DECREMENT(timer) 				\
		if ((timer) >= UDSTP_TIMING_TICK_PERIOD) 	\
		{ 											\
			(timer) -= UDSTP_TIMING_TICK_PERIOD;	\
		}else										\
		{											\
			(timer) = 0x00u;						\
		}

#endif /* _UDSTP_CONFPRIV_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
