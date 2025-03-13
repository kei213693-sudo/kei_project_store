/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv83_accesstimingparameter.c
 * 摘要: UDS Application Layer service 0x83 AccessTimingParameter.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_ACCESSTIMINGPARAMETER
static uint8_t rub_0x83AccTimParam_SubFunc;
static uint8_t rub_0x83_PrevRdExtTimParamPassed;
static uint8_t rub_0x83_PrevRdExtTimeParamArray[UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN];


Std_UdsReturnType UDS_FN_TYPE UdsApp_AccessTimingParameter(Uds_APduData_t* pAPduInd,
                                                        Uds_APduData_t* pAPduResp )
{
   uint8_t lub_error = UDS_POSITIVE_RESPONSE;
   uint8_t *p_lubExtTimParamArray = ((void *)(0x00000000u));
   uint8_t lub_tmp_Req_IsFuncAddr;

   /* read the request is come from physical addressing TP channel
      or functional addressing TP channel
    */
   lub_tmp_Req_IsFuncAddr = UDS_APP_ITF_IMP_GET_TP_CHNL();


   if (lub_tmp_Req_IsFuncAddr == TARGETADDR_FUNCTIONAL) /* Functional addressing */
   {
       lub_error = UDS_NRC_SERVICENOTSUPPORTED;
   }
   else if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_DEFAULT_SESSION)
   {
       /* 0x83 AccessTimingParameter will only be allowed in the non-default session
          otherwise , to return NRC: 0x7F, SevericeNotSupportedInCurrActiveSession
        */
       lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
   }
   else if ((pAPduInd->msgLength) < 0x02u)
   {
       /* the valid request length for 0x83 svc is at least 0x02 bytes.
          otherwise , to return NRC: 0x13 : IncorrectMessageLengthOrInvalidFormat
        */
       lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else /* else if ((pAPduInd->msgLength) >= 0x02u) */
   {
       /* to handle the byte_1 control_type ,
          except bit7 SuppressPositiveResponseMessgeIndicationBit
        */
       rub_0x83AccTimParam_SubFunc = ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7);

       switch(rub_0x83AccTimParam_SubFunc)
       {
           default:
                lub_error= UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
                break;

           case UDS_C_ACCTIMPARAM_SF_RD_EXT_TIM_PARAM_SET:
                if ((pAPduInd->msgLength) != 0x02u)
                {
                    lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                }
                else /* if ((pAPduInd->msgLength) == 0x02u) */
                {
                    lub_error = UDS_APP_ITF_IMP_GET_EXT_ACC_TIM_PARAM(&rub_0x83_PrevRdExtTimeParamArray[0x00u]);

                    if (lub_error == UDS_POSITIVE_RESPONSE)
                    {
                        rub_0x83_PrevRdExtTimParamPassed = (uint8_t)TRUE;
                        /* prepare the positive response message content */
                        (pAPduResp->msgLength) = (UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN + 0x02u);
                        Uds_MemCopy((&(pAPduResp->pduData[0x02u])),              /* pDest */
                                    (&rub_0x83_PrevRdExtTimeParamArray[0x00u]),  /* pSrc */
                                    UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN); /* numBytes */

                    }
                    else
                    {
                        lub_error = UDS_NRC_REQUESTOUTOFRANGE;
                        rub_0x83_PrevRdExtTimParamPassed = (uint8_t)FALSE;
                        Uds_MemClear((&rub_0x83_PrevRdExtTimeParamArray[0x00u]),   /* pDest */
                                     UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN);  /* numBytes */

                    }

                }  /* end of  if ((pAPduInd->msgLength) == 0x02u) */
                break;

			     case UDS_C_ACCTIMPARAM_SF_SET_TIM_PARAM_2_DFLT :
                if ((pAPduInd->msgLength) != 0x02u)
                {
                    lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                }
                else /* if ((pAPduInd->msgLength) == 0x02u) */
                {
                    lub_error = UDS_APP_ITF_IMP_SET_TIM_PARAM_TO_DEFAULT();
                    /* the more handling can be put below , if needed in the future.
                     */

                }  /* end of  if ((pAPduInd->msgLength) == 0x02u) */

			          break;

			     case UDS_C_ACCTIMPARAM_SF_RD_CURR_ACT_TIM_PARAM  :
                if ((pAPduInd->msgLength) != 0x02u)
                {
                    lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                }
                else /* if ((pAPduInd->msgLength) == 0x02u) */
                {
                    lub_error = UDS_APP_ITF_IMP_GET_CURR_ACT_TIM_PARAM(&(pAPduResp->pduData[0x02u]));

                    if (lub_error == UDS_POSITIVE_RESPONSE)
                    {
                        (pAPduResp->msgLength) = (UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN + 0x02u);
                    }
                    else
                    {
                        lub_error = UDS_NRC_REQUESTOUTOFRANGE;
                    }
                }  /* end of else if ((pAPduInd->msgLength) == 0x02u) */

                break;

			     case UDS_C_ACCTIMPARAM_SF_SET_TIM_PARAM_2_GIVN  :
                if ((pAPduInd->msgLength) !=  0x02u &&
                    (pAPduInd->msgLength) != (UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN + 0x02u))
                {
                    lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                }
                else /* if ((pAPduInd->msgLength) != (UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN + 0x02u)) */
                {
                    if ((pAPduInd->msgLength) ==  0x02u)
                    {
                        if (rub_0x83_PrevRdExtTimParamPassed == (uint8_t)TRUE)
                        {

                            p_lubExtTimParamArray = (&rub_0x83_PrevRdExtTimeParamArray[0x00u]);
                        }
                        else
                        {
                            lub_error = UDS_NRC_REQUESTOUTOFRANGE;
                        }
                    }
                    else if ((pAPduInd->msgLength) == (UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN + 0x02u))
                    {
                        p_lubExtTimParamArray = &(pAPduResp->pduData[0x02u]);
                    }
                    else
                    {
                      /* impossible    */
                    }

                    if (p_lubExtTimParamArray != ((void *)(0x00000000u)))
                    {
                        lub_error = UDS_APP_ITF_IMP_SET_TIM_PARAM_TO_GIVEN(p_lubExtTimParamArray);

                        if (lub_error == UDS_POSITIVE_RESPONSE)
                        {
                            (pAPduResp->msgLength) = (0x02u);
                        }
                        else
                        {
                            lub_error = UDS_NRC_REQUESTOUTOFRANGE;
                        }

                    }
                    else
                    {
                        lub_error = UDS_NRC_REQUESTOUTOFRANGE;
                    }

                }  /* end of else // if ((pAPduInd->msgLength) != (UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN + 0x02u)) */

			          /* init the variables to initial value */
			          rub_0x83_PrevRdExtTimParamPassed = (uint8_t)FALSE;
                Uds_MemClear((&rub_0x83_PrevRdExtTimeParamArray[0x00u]),   /* pDest */
                             UDS_C_ACCTIMPARAM_SET_GIVEN_TIME_PARAM_LEN);  /* numBytes */

                break;
       }  /* end of switch(rub_0x83AccTimParam_SubFunc)  */
   } /* end of else // else if ((pAPduInd->msgLength) >= 0x02u) */



   return lub_error;

}  /* end of UdsApp_AccessTimingParameter() */





#endif /* UDS_ENABLE_ACCESSTIMINGPARAMETER */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
