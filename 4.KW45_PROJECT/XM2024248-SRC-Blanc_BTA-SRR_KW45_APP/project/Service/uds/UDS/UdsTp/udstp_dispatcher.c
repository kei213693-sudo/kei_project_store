/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: udstp_dispatcher.c
 * 摘要: UDS network transport layer dispatcher.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "udstp_inf.h"
#include "udstp_confpriv.h"

/*
 * 函数介绍: This function dispatch a N_USData request confirmation to upper layer.
 * 参数: \param [in] mType        The type of address information who is generating the request.
 * \param [in] nSa        	The address of the server.
 * \param [in] nTa        	The address of the client.
 * \param [in] nTaType      The communication mode which shall be used by the communication peer.
 * \param [in] nAe        	The remote address of the client.
 * \param [in] nResult      The execution result of the N_USData request.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_NUSData_Confirmation(
		uint8_t mType, uint8_t nSa, uint8_t nTa,
		uint8_t nTaType, uint8_t nAe, uint8_t nResult
)
{
	TP_ITF_IMP_CB_FUNC_NUSDAT_RESPCON( mType, nSa, nTa, nTaType, nAe, nResult );
}

/*
 * 函数介绍: This function dispatch a received new frame data to upper layer.
 * 参数:
 * \param [in] mType        The type of address information who is generating the request.
 * \param [in] nSa        	The address of the server.
 * \param [in] nTa        	The address of the client.
 * \param [in] nTaType      The communication mode which shall be used by the communication peer.
 * \param [in] nAe        	The remote address of the client.
 * \param [in] nResult      The execution result of the N_USData request.
 * \param [in] msgLen       The number of bytes of received data.
 * \param [in] pMsgData     The container for the received data.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_NUSData_indication(
		uint8_t mType, uint8_t nSa, uint8_t nTa,
		uint8_t nTaType, uint8_t nAe, uint8_t nResult,
		uint16_t msgLen, uint8_t* pMsgData
)
{
	TP_ITF_IMP_CB_FUNC_NUSDAT_IND( mType, nSa, nTa, nTaType, nAe, nResult, msgLen, pMsgData );
}


/*
 * 函数介绍: This function dispatch a received first frame to upper layer.
 * 参数: \param [in] mType        The type of address information who is generating the request.
 * \param [in] nSa        	The address of the server.
 * \param [in] nTa        	The address of the client.
 * \param [in] nTaType      The communication mode which shall be used by the communication peer.
 * \param [in] nAe        	The remote address of the client.
 * \param [in] msgLen       The total number of bytes to received together with subsequent consecutive frames.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_NUSData_FFindication(
		uint8_t mType, uint8_t nSa, uint8_t nTa,
		uint8_t nTaType, uint8_t nAe, uint16_t msgLen
)
{
	TP_ITF_IMP_CB_FUNC_NUSDAT_FFIND( mType, nSa, nTa, nTaType, nAe, msgLen );
}

#ifdef UDSTP_ENABLE_CHANGEPARAMETER

/*
 * 函数介绍: This function dispatch a N_ChangeParameter request confirmation to upper layer.
 * 参数: \param [in] mType        The type of address information who is generating the request.
 * \param [in] nSa        	The address of the server.
 * \param [in] nTa        	The address of the client.
 * \param [in] nTaType      The communication mode which shall be used by the communication peer.
 * \param [in] nAe        	The remote address of the client.
 * \param [in] param      	The type of parameter(STmin, BS) whose value is required to be changed.
 * \param [in] result		The result of change parameter request.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsTp_NChangeParam_Confirmation(
		uint8_t mType, uint8_t nSa,
		uint8_t nTa, uint8_t nTaType,
		uint8_t nAe, uint8_t param, uint8_t result
)
{
	TP_ITF_IMP_CB_FUNC_NUSDAT_CHANGEPARAMCON( mType, nSa, nTa, nTaType, nAe, param, result );
}

#endif /* UDSTP_ENABLE_CHANGEPARAMETER */

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
