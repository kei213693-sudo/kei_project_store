/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv10_diagsessioncontrol.c
 * 摘要: UDS Application Layer service 0x10 DiagnosticSessionControl.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "uds_confdefs.h"
#include "uds_confpriv.h"
#include "uds_inf.h"
#include "uds_priv.h"
#include "uds_pub.h"
#include "uds_target.h"
#include "udscan_confdefs.h"
#include "uds_api.h"

#ifdef UDS_ENABLE_DIAGNOSTICSESSIONCONTROL

/*
 * 函数介绍:
 Sun-Function parameter for DiagnosticSessionControl(ISO15765-3 limited)
 * 	diagnosticSessionType
 * 	0x01	UDS_DEFAULT_SESSION
 * 	0x02	UDS_PROGRAMMING_SESSION
 * 	0x03	UDS_EXTENDED_DIAGNOSTIC_SESSION
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 * NRC for DiagnosticSessionControl
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 *
 * sessionParameterRecord for DiagnosticSessionControl(ISO15765-3 defined)
 *	4bytes
 *	parameter1 - P2CanServerMax high byte
 *	parameter2 - P2CanServerMax low byte
 *	parameter3 - P2StarCanServerMax high byte
 *	parameter4 - P2StarCanServerMax low byte
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_DiagSessionControl(Uds_APduData_t *pAPduInd,
                                                        Uds_APduData_t *pAPduResp)
{
  uint8_t lub_error;
  uint8_t lub_uds_returncode;
  uint8_t lub_CurrActiveSession;
  uint8_t lub_IsAllowDiagSessionToggle = (uint8_t)FALSE;

  lub_error = UDS_POSITIVE_RESPONSE;
  lub_uds_returncode = (uint8_t)FALSE;

  /*Check whether the pointer is empty*/
  if ((NULL ==pAPduInd) || (NULL == pAPduResp))
  {
    return UDS_NRC_GENERALREJECT;
  }
  else
  {
    /* code */
  }
  /* to check the current request length
   */
  if (pAPduInd->msgLength != 0x02u)
  {
    /*incorrect size of 0x10 request */
    lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }
  else
  {
    /* to read in the current active session
     */
    lub_CurrActiveSession = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
    /* to process the 0x10 service according to the subfunction , the 7th bit :
        suppress positive response indication bit has processed commonly , so mask out.
 */
    switch (((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7))
    {
    default:
    {
      /* for the un-supported sub funciton , to return to NRC: 0x12
       */
      lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
    }
    break;
    case UDS_INTERNAL_SESSION:
      /* if the current active session is default session , then the session switching to supplier specific  session
            will not need to run the new session actions.
          */

      if (UDS_DEFAULT_SESSION != lub_CurrActiveSession)
      {
        /* switch to Supplier Specific Session from any non-default session ,
           to execute all of the needed actions when
           entering a supplier specific session.
         */
        UdsApp_10Dsc_TriggerStartNewSessionActions();
      }
      else
      {
      }
      /* notify session manager to switch to supplier specific diag session
       */
      lub_uds_returncode = UDS_APP_ITF_IMP_SET_CHANGE_SESSION(UDS_INTERNAL_SESSION);
      if (lub_uds_returncode != TRUE)
      {
        /* if the session switching is not successfully performed , then to return NRC: 0x22
         */
        lub_error = UDS_NRC_CONDITIONSNOTCORRECT;
      }
      else
      {
      }
      /* to call the interface to prevent ECU get into sleep mode (low power mode )
                               and also prevent CAN network to sleep
       */
      UDS_APP_ITF_IMP_SET_PRVNT_ECU_2_SLEEP();
      UDS_APP_ITF_IMP_SET_PRVNT_CAN_2_SLEEP();

      break;
    case UDS_PROGRAMMING_SESSION:
    {
      if (lub_CurrActiveSession == UDS_DEFAULT_SESSION)
      {
        lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;
        return lub_error;
      }
      else
      {
    	  if(UdsApi_Check_Bootjump_Condition() != TRUE)
    	  {
    		  lub_error = UDS_NRC_CONDITIONSNOTCORRECT;
    		  return lub_error;
    	  }
    	  else
    	  {}
      }
#if 0
      if (UdsApi_GetAddrMode() == UDS0_CAN_RX_MSGID_PHY)
      {
#endif
        /* to check the session changing pre-condition , from current active session to the programming session
         */
        //  pAPduResp->pduData[0x00u] = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
        //  pAPduResp->pduData[0x01u] = 0x10;
        //  pAPduResp->pduData[0x02u] = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING;

        UdsApi_SetRub_0x10Program_SubFunc((pAPduInd->pduData[0x1u]));
        lub_IsAllowDiagSessionToggle = UDS_APP_ITF_IMP_CHK_SESN_CHNG_PRE_CND(lub_CurrActiveSession,
                                                                             UDS_PROGRAMMING_SESSION);
        // lub_error = UDS_POSITIVE_RESPONSE;
        if(0x80 != ((pAPduInd->pduData[0x1u]) & 0x80))
        {
        	 lub_error = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING;
        }
        else
        {
        }
        if (lub_IsAllowDiagSessionToggle == (uint8_t)TRUE)
        {
        }
        else
        {
          /* if the session switching to 0x02 is not allowed, then to return NRC: 0x22
           */
          lub_error = UDS_NRC_CONDITIONSNOTCORRECT;
        }
#if 0
      }
      else
      {
        lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
        return lub_error;
      }
#endif

    } /* end of case UDS_PROGRAMMING_SESSION: */
    break;
    case UDS_EXTENDED_DIAGNOSTIC_SESSION:
    {

      /* notify session manager to switch to extended diag session
       */
      lub_uds_returncode = UDS_APP_ITF_IMP_SET_CHANGE_SESSION(UDS_EXTENDED_DIAGNOSTIC_SESSION);
      if (lub_uds_returncode != TRUE)
      {
        /* if the session switching is not successfully performed , then to return NRC: 0x22
         */
        lub_error = UDS_NRC_CONDITIONSNOTCORRECT;
      }
      else
      {
      }
      /* if the current active session is default session , then the session switching to extended session
          will not need to run the new session actions.
        */
      if (UDS_DEFAULT_SESSION != lub_CurrActiveSession)
      {
        /* to execute all of the needed actions when
           from a non-default session , to entering a extended session.
         */
        UdsApp_10Dsc_TriggerStartNewSessionActions();
      }
      else
      {
      }
      /* to call the interface to prevent ECU get into sleep mode (low power mode )
          and also prevent CAN network to sleep
*/
      UDS_APP_ITF_IMP_SET_PRVNT_ECU_2_SLEEP();
      UDS_APP_ITF_IMP_SET_PRVNT_CAN_2_SLEEP();
    } /* end of case UDS_EXTENDED_DIAGNOSTIC_SESSION: */
    break;

    case UDS_DEFAULT_SESSION:
    {

      /* notify session manager to switch to default diag session
       */
      lub_uds_returncode = UDS_APP_ITF_IMP_SET_CHANGE_SESSION(UDS_DEFAULT_SESSION);

      /* switch back to default session can always be allowed.
        to execute all of the needed actions when
        entering a new session.
      */
      UdsApp_10Dsc_TriggerStartNewSessionActions();
      if (lub_uds_returncode != TRUE)
      {
          /* if the session switching is not successfully performed , then to return NRC: 0x22
            */
           lub_error = UDS_NRC_CONDITIONSNOTCORRECT;
         }
         else
         {
         }
       } /* end of  case DP_APPL_DEFAULT_SESSION_ID: */
       break;
#ifndef EP40_PRJ
       case UDS_ECU_SUPPLIER_SPEC_SESSION:
       {
         if (UDS_DEFAULT_SESSION != lub_CurrActiveSession)
         {
/* switch to Supplier Specific Session from any non-default session ,
              to execute all of the needed actions when
              entering a supplier specific session.
            */
           UdsApp_10Dsc_TriggerStartNewSessionActions();
         }
         else
         {
         }
 /* notify session manager to switch to supplier specific diag session
          */

         lub_uds_returncode = UDS_APP_ITF_IMP_SET_CHANGE_SESSION(UDS_ECU_SUPPLIER_SPEC_SESSION);
         if (lub_uds_returncode != TRUE)
         {
        lub_error = UDS_NRC_CONDITIONSNOTCORRECT;
      }
      else
      {
      }
         UDS_APP_ITF_IMP_SET_PRVNT_ECU_2_SLEEP();
         UDS_APP_ITF_IMP_SET_PRVNT_CAN_2_SLEEP();
    } /* end of  case DP_APPL_DEFAULT_SESSION_ID: */
    break;
#endif
    } /* end of switch (((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7)) */
  } /* end of else of if (pAPduInd->msgLength != 0x02u ) */

  if (lub_error == UDS_POSITIVE_RESPONSE)
  {
    /* the optional positive response content the P2_Server and P2*_Server timer value
        P2Can_server_max unit should be 1ms , P2StarCan_Server_max unite should be 10ms
        the returned value from interface are absolute value , unit are always 1ms , so  P2StarCan_Server_max
        will be divided by 10u
      */
    pAPduResp->pduData[0x02u] = ((0xFF00u) & (uint16_t)(UDS_APP_ITF_IMP_GET_TP_P2SVR_TMR_VALUE())) >> 0x08u;
    pAPduResp->pduData[0x03u] = (0x00FFu) & (uint16_t)(UDS_APP_ITF_IMP_GET_TP_P2SVR_TMR_VALUE());
    pAPduResp->pduData[0x04u] = ((0xFF00u) & (uint16_t)(UDS_APP_ITF_IMP_GET_TP_P2START_SVR_TMR_VALUE() / 10u)) >> 0x08u;
    pAPduResp->pduData[0x05u] = (0x00FFu) & (uint16_t)(UDS_APP_ITF_IMP_GET_TP_P2START_SVR_TMR_VALUE() / 10u);
    /*  the length of whole of positive response message
     */
    pAPduResp->msgLength = 0x06u;
  }
  else
  {
  }

  return lub_error;
}
/*
 * 函数介绍: n this function , to trigger all of necessary
    actions when starting a new session
    if the more actions are needed , can also
    put into this function to handle automatically
    when doing the session switching.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_10Dsc_TriggerStartNewSessionActions(void)
{
  /* in this function , to trigger all of necessary
    actions when starting a new session
    if the more actions are needed , can also
    put into this function to handle automatically
    when doing the session switching.
  */
  UDS_APP_ITF_INT_CMD_STRT_NEW_SESN_ACTION;
  UDS_APP_ITF_INT_DTC_STRT_NEW_SESN_ACTION;
  UDS_APP_ITF_INT_DID_STRT_NEW_SESN_ACTION;
  UDS_APP_ITF_INT_SECU_STRT_NEW_SESN_ACTION;
  UDS_APP_ITF_INT_COMCTRL_STRT_NEW_SESN_ACTION;
  UDS_APP_ITF_INT_E2BLK_STRT_NEW_SESN_ACTION;
  UDS_APP_ITF_INT_RTN_STRT_NEW_SESN_ACTION;
  UDS_APP_ITF_INT_LNKCTRL_STRT_NEW_SESN_ACTION;
}
/*
 * 函数介绍:  inform all of the other needed services that
    S3 Sever Timer Timout event ocurred,
    in fact the S3 server timer timeout will trigger the non-default session
    go back to default session , and do the similar actions
    for the above session switching.
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_10Dsc_generic_S3SeverTmrTimeoutEvtHandler(void)
{
  /* inform all of the other needed services that
    S3 Sever Timer Timout event ocurred,
    in fact the S3 server timer timeout will trigger the non-default session
    go back to default session , and do the similar actions
    for the above session switching.
  */
  UDS_APP_ITF_INT_CMD_HANDLE_S3_SVR_TMR_TO_EVT_ACTION;
  UDS_APP_ITF_INT_DTC_HANDLE_S3_SVR_TMR_TO_EVT_ACTION;
  UDS_APP_ITF_INT_DID_HANDLE_S3_SVR_TMR_TO_EVT_ACTION;
  UDS_APP_ITF_INT_SECU_HANDLE_S3_SVR_TMR_TO_EVT_ACTION;
  UDS_APP_ITF_INT_COMCTRL_HANDLE_S3_SVR_TMR_TO_EVT_ACTION;
  UDS_APP_ITF_INT_E2BLK_HANDLE_S3_SVR_TMR_TO_EVT_ACTION;
  UDS_APP_ITF_INT_RTN_HANDLE_S3_SVR_TMR_TO_EVT_ACTION;
  UDS_APP_ITF_INT_LNKCTRL_HANDLE_S3_SVR_TMR_TO_EVT_ACTION;
}

#endif /* UDS_ENABLE_DIAGNOSTICSESSIONCONTROL */
/******************************************************************************
 *                               end of file                                   *
 ******************************************************************************/
