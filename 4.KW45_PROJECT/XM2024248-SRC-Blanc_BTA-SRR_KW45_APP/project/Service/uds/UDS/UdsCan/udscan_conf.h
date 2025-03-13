/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_conf.h
 * 摘要: Publically available declarations derived from the CAN Data Link Layer
* interface's configuration.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#ifndef _UDSCAN_CONF_H
#define _UDSCAN_CONF_H

#include "udscan_priv.h"

#define   CAN_CANFD_bits  8u
/*lint -e750*//*lint -e751*/

/******************************************************************************
*
* Type declarations
*
******************************************************************************/

typedef UdsCan_QueueBuf_t* UdsCan_QueueBufPtr;
extern UdsCan_ChannelCfg_t UdsCan0_ChannelCfgs[];

void udscan_conf_deini(void);
#endif /* _UDSCAN_CONF_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
