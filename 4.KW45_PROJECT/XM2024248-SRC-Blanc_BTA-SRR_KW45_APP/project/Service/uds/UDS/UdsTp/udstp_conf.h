/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_conf.h
 * 摘要: Publically available declarations derived from UDS network transport layer configuration.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDSTP_CONF_H
#define _UDSTP_CONF_H

#include "udstp_confpriv.h"

/*lint -e750*//*lint -e766*/

/******************************************************************************
*
* Macro definitions
*
******************************************************************************/

/* This is helper macros to access the transport-layer-specific state and configuration of a session.
 * The transport-layer-specific state and configuration is stored in this manner to allow uniform access,
 * irrespective of the definition of the structures which contain the state and configuation. */
#define UDSCAN_SESSIONCFG( sessionId )  ( (UdsCan_SessionCfg_t*)(UdsTp_SessionCfgs[ (sessionId) ].pTransportCfg ) )

#define UDSCAN_SET_TRANSMITCONF( sessionId )	( UdsTp_Sessions[ (sessionId) ].sentConfirmed = TRUE )

/* Macros to drive Network Transport layer NTiming timer.
 *
 * [Note]:
 * The period of ECU resource, e.g. a periodic timer, which is used to tick the network transport layer
 * NTiming timer shall be defined in microsecond using "UDSTP_TIMING_TICK_PERIOD" which is configured
 * in "udstp_confdefs.h".
 */
#define TICK_UDSTP_SESSION1_TIMER()        UDSTP_TIMER_DECREMENT(UdsTp_Sessions[0x00u].timeoutCounter)

#endif /* _UDSTP_CONF_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
