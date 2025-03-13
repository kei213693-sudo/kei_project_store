/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_inf.h
 * 摘要: This file declares all the APIs (i.e. interfaces) which the UDS network transport layer
* requires from other components of the system.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDSTP_INF_H
#define _UDSTP_INF_H

#include "uds_target.h"
#include "uds_pub.h"

/*lint -e750*//*lint -e752*/

/******************************************************************************
 *
 * 1st class interfaces: exported interfaces , which is provided by other modules.
 *
 *****************************************************************************/

/**
 * TP_ITF_EXP_CB_FUNC_NUSDAT_IND :
 * legend:
 * TP     : TransportProtocolLayer
 * ITF    : Interface
 * EXP    : Exported (interface type)
 * CB     : Callback
 * FUNC   : Function (interface type)
 * NUSDAT : Network Layer Unacknowledged Segemented Data
 * IND    : Indication Primitive
 */
#define TP_ITF_IMP_CB_FUNC_NUSDAT_IND( mType, nSa, nTa, nTaType, nAe, nResult, msgLen, pMsgData )	    \
        (UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_IND( mType, nSa, nTa, nTaType, nAe, nResult, msgLen, pMsgData ))

#define TP_ITF_IMP_CB_FUNC_NUSDAT_FFIND( mType, nSa, nTa, nTaType, nAe, msgLen )						\
		(UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_FFIND( mType, nSa, nTa, nTaType, nAe, msgLen ))

#define TP_ITF_IMP_CB_FUNC_NUSDAT_RESPCON( mType, nSa, nTa, nTaType, nAe, nResult )						\
		(UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_RESPCON( mType, nSa, nTa, nTaType, nAe, nResult ))

#define TP_ITF_IMP_CB_FUNC_NUSDAT_CHANGEPARAMCON( mType, nSa, nTa, nTaType, nAe, param, result )		\
		(UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_CHANGEPARAMCON( mType, nSa, nTa, nTaType, nAe, param, result ))

#define TP_ITF_IMP_CB_FUNC_ISRXBUFAVAILABLE()		(UDS_APP_ITF_EXP_CB_FUNC_ISRXBUFAVAILABLE())

/******************************************************************************
 *
 * General functions and types
 *
 *****************************************************************************/

/**
 * The UDS transport layer expects this function to indicate whether it is ready/allowed to receive
 * the specified length of segmented data indicated in received FirstFrame.
 *
 * \param [in] sessionId    The id of the UdsTp session who issues this request.
 * \param [in] numBytes     The total length of the segmented data to be received.
 *
 * \return
 *  - zero        The specified length of the segmented data is not receivable for now.
 *  - non-zero  It's receivable.
 */
uint8_t UdsTpApp_IsDataReceivable( uint8_t sessionId, uint16_t numBytes );

/**
 * The UDS transport layer expects this function to indicate whether it is allowed to dispatch
 * the service data received by transport layer to upper layer.
 *
 * \return
 *  - zero        	The service data is not receivable for now.
 *  - non-zero		It's receivable by upper layer.
 */
uint8_t UdsTpApp_IsDispatchAllowed( void );

#endif /* _UDSTP_INF_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
