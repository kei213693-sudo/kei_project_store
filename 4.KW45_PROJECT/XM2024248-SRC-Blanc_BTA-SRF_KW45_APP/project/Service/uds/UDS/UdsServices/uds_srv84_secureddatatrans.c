/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv84_secureddatatrans.c
 * 摘要: UDS Application Layer service 0x84 SecuredDataTransmission.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_SECUREDDATATRANSMISSION

Std_UdsReturnType UDS_FN_TYPE UdsApp_SecuredDataTransmission(Uds_APduData_t* pAPduInd,
                                                             Uds_APduData_t* pAPduResp )
{
   pAPduInd = pAPduInd; /* just to remove compilation warning */
   Uds_generic_SendResp(pAPduResp, UDS_NRC_SERVICENOTSUPPORTED);

   return  UDS_NRC_SERVICENOTSUPPORTED;
}

#endif /* UDS_ENABLE_SECUREDDATATRANSMISSION */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
