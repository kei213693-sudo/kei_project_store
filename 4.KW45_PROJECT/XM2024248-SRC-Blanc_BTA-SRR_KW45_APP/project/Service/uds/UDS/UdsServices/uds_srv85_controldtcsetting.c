/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv85_controldtcsetting.c
 * 摘要: UDS Application Layer service 0x85 ControlDTCSetting.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"
#include "udscan_confdefs.h"
#include "uds_api.h"

#ifdef UDS_ENABLE_CONTROLDTCSETTING
static   uint8_t rub_0x85CtrlDtcSetting_SubFunc;


/* Sun-Function parameter for ControlDTCSetting(ISO15765-3 limited)
 * 	DTCSettingType
	0x01	UDS_DTC_SETTING_START
	0x02	UDS_DTC_SETTING_STOP
 */


/*
 * 函数介绍: DTCSettingControlOptionRecord for ControlDTCSetting
 *	user Optional(ISO14229 9.9.2.3)
 * 参数:
 * 返回值: {*}
 * 备注:
 * NRC for ControlDTCSetting
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 *	0x31	UDS_NRC_REQUESTOUTOFRANGE
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_ControlDTCSetting(Uds_APduData_t* pAPduInd,
                                                    Uds_APduData_t* pAPduResp )
{

   uint8_t lub_error = UDS_POSITIVE_RESPONSE;
   uint32_t rub_0x85CtrlDtcGroupNummber = 0;
   if ((NULL == pAPduInd) || (NULL == pAPduResp))
   {
      return UDS_NRC_GENERALREJECT;
   }
   else
   {
      /* code */
   }
   UDS_DISABLE_ALL_INTERRUPTS();
   /* to handle the byte_1 sub function , except bit7
      SuppressPositiveResponseMessgeIndicationBit
    */
   rub_0x85CtrlDtcSetting_SubFunc = ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7);
   rub_0x85CtrlDtcGroupNummber = (pAPduInd->pduData[0x02u])+((pAPduInd->pduData[0x03u])<<8)+((pAPduInd->pduData[0x04u])<<16);
   UDS_ENABLE_ALL_INTERRUPTS();

   if (UDS_DEFAULT_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION())
   {
      /*Determine the addressing method*/
      if (UdsCan0_ChannelCfgs[0].msgIdPhy == UdsApi_GetAddrMode())
      {
         lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
      }
      else
      {
         /*Function addressing does not answer*/
         lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
      }
      return lub_error;
   }
   else if ((pAPduInd->msgLength) != 0x05u)
   {
      /* currently , optional DTCSettingControlOptionRecord
         is not supported temporarily
       */
      lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else if (UDS_C_CTRLDTCSET_DTC_STORE_ON != rub_0x85CtrlDtcSetting_SubFunc &&
            UDS_C_CTRLDTCSET_DTC_STORE_OFF != rub_0x85CtrlDtcSetting_SubFunc)
   {
      /* the DTC Setting parameter is not supported by the ECU */
      lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
   }
   else if(0xffffff != rub_0x85CtrlDtcGroupNummber)
   {
	   lub_error = UDS_NRC_REQUESTOUTOFRANGE;
   }
   else
   {

      if (UDS_C_CTRLDTCSET_DTC_STORE_ON == rub_0x85CtrlDtcSetting_SubFunc)
      {
         rub_0x85CtrlDtcSetting_SubFunc = UDS_C_CTRLDTCSET_DTC_STORE_ON;
      }
      else if(UDS_C_CTRLDTCSET_DTC_STORE_OFF == rub_0x85CtrlDtcSetting_SubFunc)
      {
         rub_0x85CtrlDtcSetting_SubFunc = UDS_C_CTRLDTCSET_DTC_STORE_OFF;
      }
      else
      {

      }
      /* ok, all of the checking is passed,
         prepare the positive response related data
         the length of reponse is 2 bytes , currently , still not include sid byte
       */
      (pAPduResp->msgLength) = 0x02u;
      /*ISO 14229 (2006)7.5.3.2For a function addressing request with no parameters,
      an action is performed but no response is given*/


   }

   return lub_error;

} /* end of Uds_ControlDTCSetting() */

/*
 * 函数介绍: init function for DCT setting
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UdsApp_85CtrlDtcSet_Init(void)
{
   uint8_t CURR_ACTIVE_SESSION = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
   if (UDS_EXTENDED_DIAGNOSTIC_SESSION == CURR_ACTIVE_SESSION)
   {
      return;
   }
   /* after start-up DTC Manager has to set DTCs
      so the Control Dtc Setting Flag should be set to initial value ON
    */
   UDS_DISABLE_ALL_INTERRUPTS();
   rub_0x85CtrlDtcSetting_SubFunc = UDS_C_CTRLDTCSET_DTC_STORE_ON;
   UDS_ENABLE_ALL_INTERRUPTS();
}

/*
 * 函数介绍: 获取当前的DTC设置状态
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UdsApp_85CtrlDtcSet_Get_CurrDtcSettingFlg(void)
{
	 return rub_0x85CtrlDtcSetting_SubFunc ;
}

#endif /* UDS_ENABLE_CONTROLDTCSETTING */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
