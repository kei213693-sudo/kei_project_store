/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_pub.h
 * 摘要: Public definitions and declarations
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#ifndef _UDSTP_PUB_H
#define _UDSTP_PUB_H

#include "uds_target.h"
#include "udstp_priv.h"
/*lint -e750*//*lint -e752*/

/******************************************************************************
 *
 * General functions and types
 *
 *****************************************************************************/

void				UDS_FN_TYPE	UdsTp_Initialize   				( void );
void				UDS_FN_TYPE	UdsTp_ServiceProc				( void );
uint8_t             UDS_FN_TYPE	UdsTp_NUSData_Request			( uint8_t mType, uint8_t nSa, uint8_t nTa, uint8_t nTaType, uint8_t nAe, uint16_t msgLen, uint8_t* pMsgData );
void				UDS_FN_TYPE	UdsTp_NUSData_Confirmation		( uint8_t mType, uint8_t nSa, uint8_t nTa, uint8_t nTaType, uint8_t nAe, uint8_t nResult );
void				UDS_FN_TYPE	UdsTp_NUSData_indication		( uint8_t mType, uint8_t nSa, uint8_t nTa, uint8_t nTaType, uint8_t nAe, uint8_t nResult, uint16_t msgLen, uint8_t* pMsgData );
void				UDS_FN_TYPE	UdsTp_NUSData_FFindication		( uint8_t mType, uint8_t nSa, uint8_t nTa, uint8_t nTaType, uint8_t nAe, uint16_t msgLen );
void				UDS_FN_TYPE	UdsTp_NChangeParam_Request		( uint8_t mType, uint8_t nSa, uint8_t nTa, uint8_t nTaType, uint8_t nAe, uint8_t param, uint8_t paramValue );
void				UDS_FN_TYPE	UdsTp_NChangeParam_Confirmation	( uint8_t mType, uint8_t nSa, uint8_t nTa, uint8_t nTaType, uint8_t nAe, uint8_t param, uint8_t result );

#endif /* _UDSTP_PUB_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
