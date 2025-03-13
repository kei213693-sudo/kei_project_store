/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_callbacks.h
 * 摘要: This file defines a body for each of the callback functions which the UDS Application Layer
* expects the user to implement.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDS_CALLBACKS_H
#define _UDS_CALLBACKS_H

#include "uds_target.h"

/*lint -e750*//*lint -e752*/

/******************************************************************************
 *
 * General functions and types
 *
 *****************************************************************************/

extern uint8_t UDS_FN_TYPE UdsApp_87LnkCtrl_ChkFixedBaudrateIsAllowed(uint8_t i_FixedBaudRateId2Chk);
extern uint8_t UDS_FN_TYPE UdsApp_87LnkCtrl_Transit2FixedBaudrate(uint8_t i_FixedBaudRateId2Transit);
extern uint8_t UDS_FN_TYPE UdsApp_87LnkCtrl_ChkSpecifiedBaudrateIsAllowed(uint8_t i_SpecBaudRateHByte,
																		uint8_t i_SpecBaudRateMByte,
																		uint8_t i_SpecBaudRateLByte);
extern uint8_t UDS_FN_TYPE UdsApp_87LnkCtrl_TransitSpecifiedBaudrate( uint8_t i_SpecBaudRateHByte,
																	uint8_t i_SpecBaudRateMByte,
																	uint8_t i_SpecBaudRateLByte);
extern uint8_t UDS_FN_TYPE UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(uint8_t i_lub_Sal2Checked);

#endif /* _UDS_CALLBACKS_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
