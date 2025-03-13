/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv37_reqtransferexit.c
 * 摘要: UDS Application Layer service 0x37 RequestTransferExit.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"
#include "uds_pub.h"

#ifdef UDS_ENABLE_REQUESTTRANSFEREXIT

/* no Sun-Function parameter for this service */

/*
 * 函数介绍: 退出数据传输
 * 参数:
 * 返回值: {*}
 * 备注: NRC for ReadDataByIdentifier
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_RequestTransferExit(Uds_APduData_t* pAPduInd,
                                                      Uds_APduData_t* pAPduResp )
{
   uint8_t lub_error = UDS_POSITIVE_RESPONSE;
   UDS_TransferDatState_t lub_ReqDnUpLdState = UdsApp_36TransferDat_GetCurrTsfrDatSt();

   if ((NULL == pAPduInd) || (NULL == pAPduResp))
   {
       return UDS_NRC_GENERALREJECT;
   }
   else
   {
       /* code */
   }

   pAPduResp = pAPduResp; /* just to remove the compilation warning */
   if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
   {
       lub_error = UDS_NRC_SERVICENOTSUPPORTED;
   }
   else if (pAPduInd->msgLength != 0x01u)
   {
       lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else if (UDS_DEFAULT_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION()||
   UDS_EXTENDED_DIAGNOSTIC_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION())

   {
       /* if the current active session is still default session ,
            reject it.
          */
       lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
   }
   else if (lub_ReqDnUpLdState != UDS_C_TANSFER_DAT_ST_DNLD &&
            lub_ReqDnUpLdState != UDS_C_TANSFER_DAT_ST_UPLD)
   {
       lub_error = UDS_NRC_REQUESTSEQUENCEERROR;
   }
   else
   {
       /* in fact , we should allow the tester to exit the tranfer data anytime
          after sequence is correct, instead of must wait the end of transfer data
        */
       UdsApp_34ReqDnld_InitCurrReqDnldData();
#ifdef UDS_ENABLE_REQUESTUPLOAD
       UdsApp_35ReqUpld_InitCurrReqUpldData();
#endif
       UdsApp_36TransferDat_InitBlkSeqCntrBefrTsfrDat();
   }
   pAPduResp->msgLength = 0x01u;
   return lub_error;
}

#endif /* UDS_ENABLE_REQUESTTRANSFEREXIT */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
