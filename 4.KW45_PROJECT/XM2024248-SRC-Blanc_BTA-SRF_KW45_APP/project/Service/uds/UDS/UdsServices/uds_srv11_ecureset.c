/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv11_ecureset.c
 * 摘要: UDS Application Layer service 0x11 EcuReset.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_ECURESET
/* static uint8_t rub_0x11EcuReset_Sid; */
static uint8_t rub_0x11EcuReset_SubFunc;
static uint32_t rul_0x11EcuReset_Dly_Tmr;

/*
 * 函数介绍:
 Sun-Function parameter for EcuReset(ISO15765-3 limited)
 *	resetType
 * 	0x01	HARD_RESET
 * 	0x02	KEYOFFON_RESET
 * 	0x03	SOFT_RESET
 *	0x04	ENABLE_RAPIDPOWERDOWN_RESET
 *	0x05	DISABLE_RAPIDPOWERDOWN_RESET
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 * NRC for EcuReset
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_EcuReset(Uds_APduData_t* pAPduInd,
                                           Uds_APduData_t* pAPduResp )
{
	 uint8_t lub_response ;
   uint8_t    lub_CurrActiveSession = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
    /*Check whether the pointer is empty*/
  if ((NULL ==pAPduInd) || (NULL == pAPduResp))
  {
    return UDS_NRC_GENERALREJECT;
  }
  else
  {
    /* code */
  }
	 /* first , to init the response value to positive response by first */
	 lub_response = UDS_POSITIVE_RESPONSE;

   if (lub_CurrActiveSession == UDS_DEFAULT_SESSION)
   {
     lub_response = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
     return lub_response;
   }

   /* check the request message length is valid or not */
   if (0x02u != (pAPduInd->msgLength))
   { /* if the length is not correct, to return NRC 0x13.
      */
     lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else /* if (0x02u == (pAPduInd->msgLength)) */
   {
        /* to save the input sub-function (reset type) into a static global variable , without the bit7 of sub-function
	   */
       UDS_DISABLE_ALL_INTERRUPTS();
         rub_0x11EcuReset_SubFunc = ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7);
       UDS_ENABLE_ALL_INTERRUPTS();

       /* here to check all of the supported sub functions.
	  */
       if ((rub_0x11EcuReset_SubFunc != UDS_C_ECURESET_SUBFUNC_HARDRESET)
#ifndef EP40_PRJ
           && (rub_0x11EcuReset_SubFunc != UDS_C_ECURESET_SUBFUNC_KEYONOFFRESET) &&
           (rub_0x11EcuReset_SubFunc != UDS_C_ECURESET_SUBFUNC_SOFTRESET) &&
           (rub_0x11EcuReset_SubFunc != UDS_C_ECURESET_SUBFUNC_ENA_RPD_PDN_RESET) &&
           (rub_0x11EcuReset_SubFunc != UDS_C_ECURESET_SUBFUNC_DIS_RPD_PDN_RESET)
#endif
       )
       {
             /* if the sub-function is not supported, return NRC: 0x12.
	       */
	   	     lub_response = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
       }
#ifdef NIO_PRJ
       else if(pm_batt_record_dtc() != TRUE)
      {/*Check whether the current voltage is within the reasonable range(9v-16v)*/
          lub_response = UDS_NRC_CONDITIONSNOTCORRECT;
      }
#endif
       else
       {
           /* to start a reset delay timer,  to return the response of this request, then to do the actual
               reset action.
        */
           UDS_DISABLE_ALL_INTERRUPTS();
           rul_0x11EcuReset_Dly_Tmr = UDS_C_ECURESET_POWER_DOWN_REL_TIME;
           UDS_ENABLE_ALL_INTERRUPTS();

           /* set the response message length to 0x02, i.e the positive response is 2 bytes message
            */
           (pAPduResp->msgLength) = 0x02u; /* whole of positive response length */
       }
   }  /* end of else if (0x02u == (pAPduInd->msgLength)) */
	 return lub_response;
}

/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_11EcuRst_CyclicTskEntryPoint( void )
{

  uint8_t lub_EcuResetType = 0x00u;

  /* to tick this software delay timer for 0x11EcuReset when bigger than 0x01
   */
  if (rul_0x11EcuReset_Dly_Tmr > 0x00000001u)
  {
      UDS_DISABLE_ALL_INTERRUPTS();
      rul_0x11EcuReset_Dly_Tmr--;
      UDS_ENABLE_ALL_INTERRUPTS();
  }
  else if (rul_0x11EcuReset_Dly_Tmr == 0x00000001u)
  {
      /* when this timer is expired ,
           save the previous saved sub function into local variable
           then to clear it, i.e. to make sure to consume it only once time
       */
      lub_EcuResetType = rub_0x11EcuReset_SubFunc;

      UDS_DISABLE_ALL_INTERRUPTS();
        rul_0x11EcuReset_Dly_Tmr--;
        rub_0x11EcuReset_SubFunc = 0x00u;
      UDS_ENABLE_ALL_INTERRUPTS();
      /* according to the reset type , to do the respective reset action
	 */
      switch (lub_EcuResetType)
      {
        default :
              break;
        case UDS_C_ECURESET_SUBFUNC_HARDRESET:
	          /* to call the hard reset interface to do the hard reset
		     */
        	  UDS_DISABLE_ALL_INTERRUPTS();
              UDS_APP_ITF_IMP_SET_ECURST_HARD_RESET();

              break;

        case UDS_C_ECURESET_SUBFUNC_KEYONOFFRESET:
              //UDS_APP_ITF_IMP_SET_ECURST_KEYONOFF_RESET();
              break;

        case UDS_C_ECURESET_SUBFUNC_SOFTRESET:
             //UDS_APP_ITF_IMP_SET_ECURST_SOFT_RESET();
             break;

        case UDS_C_ECURESET_SUBFUNC_ENA_RPD_PDN_RESET:
             //UDS_APP_ITF_IMP_SET_ENA_RPD_PWRDN_RESET();
             break;

        case UDS_C_ECURESET_SUBFUNC_DIS_RPD_PDN_RESET:
             //UDS_APP_ITF_IMP_SET_DIS_RPD_PWRDN_RESET();
             break;


      }  /* end of switch (lub_EcuResetType) */

  } /* end of else if (rul_0x11EcuReset_Dly_Tmr == 0x00000001u) */
  else
  {

  }

}
/*
 * 函数介绍: to close all of interrupts
       and go into a infinite cycle
       to trigger watchdog reset
       currently , this is not used, we can set a MPC5XXX register directly to trigger soft reset
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_11EcuRst_RunSoftReset(void)
{
    /* to close all of interrupts
       and go into a infinite cycle
       to trigger watchdog reset
       currently , this is not used, we can set a MPC5XXX register directly to trigger soft reset
     */
    /* UDS_DISABLE_ALL_INTERRUPTS(); */
    for (;;)
    {
    }
}

#endif /* UDS_ENABLE_ECURESET */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
