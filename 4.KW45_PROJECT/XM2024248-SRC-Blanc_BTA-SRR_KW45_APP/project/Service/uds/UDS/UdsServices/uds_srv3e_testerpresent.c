/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv3e_testerpresent.c
 * 摘要: UDS Application Layer service 0x3E TesterPresent.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"


#ifdef UDS_ENABLE_TESTERPRESENT

/*
 * 函数介绍: Sun-Function parameter for TesterPresent(ISO15765-3 limited)
 * 	zeroSubFunction
	0x00	no sub-function value beside the suppressPosRspMsgIndicationBit is supported.
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 * NRC for TesterPresent
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_TesterPresent(Uds_APduData_t* pAPduInd,
                                                Uds_APduData_t* pAPduResp )
{
  uint8_t  lub_error;
  uint8_t  lub_tmp_sub_func_3e;

  /* init the return code to positive first  */
  lub_error = UDS_POSITIVE_RESPONSE;

    /*Check whether the pointer is empty*/
   if ((NULL ==pAPduInd) || (NULL == pAPduResp))
  {
    return UDS_NRC_GENERALREJECT;
  }
  else
  {
    /* code */
  }
  /*  to check the length validation */
  if (pAPduInd->msgLength != (uint8_t)0x02u)
  {
      /* if the length of incoming diag request is not valid
         return NRC. 0x13 IncorrectMsgLenOrInvalidFormat;
       */
      lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }
  else
  {
      /* read into the sub function for 0x3E service  */
      lub_tmp_sub_func_3e = pAPduInd->pduData[0x01u];

      /* to get the actual sub function value except
         the bit 7 SuppressPostiveResponseMsgIndBit
       */
      if ((lub_tmp_sub_func_3e & UDS_C_SUB_FUNC_MASK_NO_SPRB7) != TESTERPRESENT_ZERO_SUBFUNC )
      {
          /* if the incoming subfunction value is not supported ,
             return NRC 0x12 SubFuncNotSupported.
           */
          lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
      }
      else
      {
      }
  }  /* end of else of if (luw_len != (uint8_t)0x02u) */

  if (lub_error == UDS_POSITIVE_RESPONSE)
  {
      /* if the incoming diag request is valid , then process it :
         step_1st: to reload S3_Server timer after receive the valid 3E request.
                   if current no service is running.
       */
      UDS_APP_ITF_IMP_SET_RELOAD_S3SVR_TMR();
      pAPduResp->msgLength = 0x02u;
  }
  else /* if (lub_error != DIR_AFFIRMATIVE) */
  {

  } /* end of else of if (lub_error == UDS_POSITIVE_RESPONSE) */

  return  lub_error;
}

#endif /* UDS_ENABLE_TESTERPRESENT */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
