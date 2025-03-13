/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_callbacks.c
 * 摘要: This file implements all the APIs (i.e. interfaces) which the UDS network transport layer
* requires from other components of the system.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_inf.h"
#include "uds_confpriv.h"

/*lint -e715*/


/*
 * 函数介绍: The UDS transport layer expects this function to indicate whether it is ready/allowed to receive
 * the specified length of segmented data indicated in received FirstFrame.
 * 参数: \param [in] sessionId    The id of the UdsTp session who issues this request.
 * \param [in] numBytes     The total length of the segmented data to be received.
 * 返回值: - zero        The specified length of the segmented data is not receivable for now.
 *  - non-zero  It's receivable.
 * 备注:
 */
uint8_t UdsTpApp_IsDataReceivable( uint8_t sessionId, uint16_t numBytes )
{
    /* TODO: review this implementation , just to remove the compiler warning
     */
    sessionId = sessionId;
    numBytes = numBytes;

    return 0x01u;
}

/*
 * 函数介绍: The UDS transport layer expects this function to indicate whether it is allowed to dispatch
 * the service data received by transport layer to upper layer.
 * 参数:
 * 返回值:  - zero        	The service data is not receivable for now.
 *  - non-zero		          It's receivable by upper layer.
 * 备注:
 */
uint8_t UdsTpApp_IsDispatchAllowed( void )
{
  return (TP_ITF_IMP_CB_FUNC_ISRXBUFAVAILABLE());
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
