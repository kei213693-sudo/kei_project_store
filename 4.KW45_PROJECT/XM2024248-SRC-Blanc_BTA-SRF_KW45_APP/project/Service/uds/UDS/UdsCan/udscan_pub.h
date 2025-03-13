/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_pub.h
 * 摘要:  Public definitions and declarations of functions intended to be called by
* the UDS network transport layer.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDSCAN_PUB_H
#define _UDSCAN_PUB_H

#include "udscan_conf.h"

/*lint -e750*//*lint -e752*/

void 		  		UDS_FN_TYPE UdsCan_Initialize		( void );
UdsCan_QueueBufPtr 	UDS_FN_TYPE UdsCan_GetTxBuf       	( uint8_t sessionId );
UdsCan_QueueBufPtr 	UDS_FN_TYPE UdsCan_GetRxBuf      	( uint8_t sessionId );
void          		UDS_FN_TYPE UdsCan_TxNext        	( uint8_t sessionId );
void          		UDS_FN_TYPE UdsCan_RxNext        	( uint8_t sessionId );
void          		UDS_FN_TYPE UdsCan_AbortTxRequest	( uint8_t sessionId );

#endif /* _UDSCAN_PUB_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
