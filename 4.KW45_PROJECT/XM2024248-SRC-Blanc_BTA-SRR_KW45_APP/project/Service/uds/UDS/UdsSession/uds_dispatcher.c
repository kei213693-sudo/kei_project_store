/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_dispatcher.c
 * 摘要: UDS session layer dispatcher.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_inf.h"
#include "uds_pub.h"

/*lint -esym(818,pSessionTxBuf)*/

/**
 * This function dispatch an A_PDU response to N_PDU of network transport layer.
 *
 * \param [in] pSessionTxBuf	The container for the response data.
 *
 * \return
 *  -UDS_E_OK:         This packet was dispatched successfully.
 *  -UDS_E_NOT_OK:     This packet was not dispatched since AI not correct matching UdsTp layer.
 *  -UDS_E_PENDING:    The packet can't dispatch since UdsTp module is still busy sending last response.
 */
Std_UdsReturnType UDS_FN_TYPE Uds_ServiceResponse( Uds_SessionTxBufPtr pSessionTxBuf )
{
	Std_UdsReturnType	serviceResult;

	serviceResult = UDS_APP_ITF_IMP_SET_NUSDAT_REQ((uint8_t)(pSessionTxBuf->serverType),
	                                               pSessionTxBuf->sA,
	                                               pSessionTxBuf->tA,
									               (uint8_t)(pSessionTxBuf->tAType),
									               pSessionTxBuf->rA,
									               pSessionTxBuf->pAPduData->msgLength,
									               &(pSessionTxBuf->pAPduData->pduData[0]) ); /** UDSonCAN_req 78 */

	return serviceResult;
}

#ifdef UDS_ENABLE_CHANGEPARAMETER

/**
 * This function requires to change parameter of transport layer.
 *
 * \param [in] pSessionTxBuf	The container for the response data.
 *
 * \return
 */
/*
 * 函数介绍: This function requires to change parameter of transport layer.
 * 参数: param [in] pSessionTxBuf	The container for the response data.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_ChangeParamRequest( Uds_SessionTxBufPtr pSessionTxBuf )
{
	Uds_SessionRuntime_t* pUdsSessionRuntime = &Uds_SessionRuntime;

	pUdsSessionRuntime->flagChangeParamConfirm = FALSE;
	UDS_APP_ITF_IMP_CHANGE_PARAM_REQ((uint8_t)(pSessionTxBuf->serverType),
	                                 pSessionTxBuf->sA,
	                                 pSessionTxBuf->tA,
									 (uint8_t)(pSessionTxBuf->tAType),
									 pSessionTxBuf->rA,
									 pSessionTxBuf->parameter,
									 pSessionTxBuf->parameterValue );
}

#endif /* UDS_ENABLE_CHANGEPARAMETER */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
