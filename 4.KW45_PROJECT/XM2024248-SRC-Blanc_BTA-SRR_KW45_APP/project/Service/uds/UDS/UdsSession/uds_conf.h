/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_conf.h
 * 摘要: Publically available declarations derived from UDS Application Layer configuration.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDS_CONF_H
#define _UDS_CONF_H

#include "uds_confpriv.h"

/*lint -e750*//*lint -e752*/

/******************************************************************************
*
* Macro definitions
*
******************************************************************************/

/* Macros to drive UDS Session layer and Application layer timing.
 *
 * [Note]:
 * The period of ECU resource, e.g. a periodic timer, which is used to tick the session layer and
 * application layer timers shall be defined in microsecond using "UDS_SESSION_TIMING_TICK_PERIOD"
 * which is configured in "uds_confdefs.h".
 */
#define TICK_UDS_SESSION_TIMER()        UDS_TIMER_DECREMENT(Uds_SessionRuntime.timeoutCounterS3Server)
#define TICK_UDS_APP_TIMER()            UDS_TIMER_DECREMENT(Uds_SessionRuntime.timeoutCounter)

#endif /* _UDS_CONF_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
