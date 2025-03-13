/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv24_readscalingdatabyid.c
 * 摘要: UDS Application Layer service 0x24 ReadScallingDataByIdentifier.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_READSCALINGDATABYIDENTIFIER

/* no Sun-Function parameter for this service */

/* NRC for ReadMemoryByAddress
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */

Std_UdsReturnType UDS_FN_TYPE UdsApp_ReadScalingDataByIdentifier(Uds_APduData_t* pAPduInd,
                                                              Uds_APduData_t* pAPduResp )
{
   pAPduInd = pAPduInd; /* just to remove the compilation warning */
   Uds_generic_SendResp(pAPduResp, UDS_NRC_SERVICENOTSUPPORTED);

   return  UDS_NRC_SERVICENOTSUPPORTED;
}

#endif /* UDS_ENABLE_READSCALINGDATABYIDENTIFIER */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
