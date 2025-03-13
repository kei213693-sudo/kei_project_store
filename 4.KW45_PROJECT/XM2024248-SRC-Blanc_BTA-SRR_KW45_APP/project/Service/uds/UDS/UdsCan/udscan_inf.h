/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udscan_inf.h
 * 摘要: This file declares all the APIs (i.e. interfaces) which the Data Link Layer Interface
* requires from other components of the system.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDSCAN_INF_H
#define _UDSCAN_INF_H

#include "uds_target.h"
#include "udscan_confdefs.h"

/*lint -e750*//*lint -e752*/


/******************************************************************************
*
* APIs required from the CAN driver.
*
******************************************************************************/


/*
 * 函数介绍:
 * This function should transmit (or queue for transmission) the specified CAN message.
 * The CAN driver can decide by itself which CAN message object(e.g. Hardware message objects) to be used.
 *
 * The UdsCan module assumes that:
 *  - The UdsCan module can discard the contents of the buffer pBytes after this API call has returned.
 * 参数:
 * \param [in] msgId        The CAN message ID to be used for the message.
 * \param [in] numBytes		The DLC of the CAN message.
 * \param [in] pBytes       The payload of the CAN message.
 * 返回值: {*}
 * 备注:
 */
void UdsCan_LData_TxRequest( uint32_t msgId, uint8_t numBytes, uint8_t* pBytes );

#if (UDS0_CAN_HW_TX_ABORT == 0x01u)
/**
 * This function abort the ongoing transmission of the CAN message which is pending at the CAN
 * driver and not transmitted yet.
 *
 * Note that:
 *  - This function is suppose to never fail on cancling of the pending transmit request.
 *
 * \param [in] msgId        The CAN message ID which is used for the message.
 */
void UdsCanApp_AbortTransmit( uint32_t msgId );
#endif /* (UDS0_CAN_HW_TX_ABORT == 0x01u) */

#endif /* _UDSCAN_INF_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
