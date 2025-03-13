/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv14_cleardiaginformation.c
 * 摘要: UDS Application Layer service 0x14 ClearDiagnosticInformation.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "uds_confdefs.h"
#include "uds_confpriv.h"
#include "uds_inf.h"
#include "uds_priv.h"
#include "uds_target.h"
#include "UdsItf_Srv.h"

#ifdef UDS_ENABLE_CLEARDIAGNOSTICINFORMATION
static uint8_t rub_didtc_pending;
static uint8_t rub_WhichTypeDtc2Clear;
static uint8_t rub_DtcHighByte2Clear;
static uint8_t rub_DtcMiddleByte2Clear;
static uint8_t rub_DtcLowByte2Clear;
static uint32_t rul_DtcCode2Clear;

static Uds_APduData_t *pSavedAPduResp;

/* private functions declerations */
static uint8_t UDS_FN_TYPE UdsApp_14ClrDiagInfor_ClearDtcAction(uint32_t i_lul_DtcCode2Clear);

/*
 * 函数介绍: Sun-Function parameter for ClearDiagnosticInformation(ISO15765-3 limited)
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 *  NRC for EcuReset
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 *	   0x33	UDS_NRC_SECUTITYACCESSDENIED
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_ClearDiagnosticInformation(Uds_APduData_t *pAPduInd,
                                                                Uds_APduData_t *pAPduResp)
{
   uint8_t lub_response = UDS_POSITIVE_RESPONSE;
   /*Check whether the pointer is empty*/
   if ((NULL ==pAPduInd) || (NULL == pAPduResp))
   {
      return UDS_NRC_GENERALREJECT;
   }
   else
   {
      /* code */
   }
   if (0x04u != (pAPduInd->msgLength))
   {
      /* 0x14 ClearDiagnosticInformation have the fixed length: 4 bytes
       */
      lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else /* if (0x04u == (pAPduInd->msgLength)) */
   {

      /* to read in 3 bytes DTC identifiers , which will be cleared.
       */
      rub_DtcHighByte2Clear = (pAPduInd->pduData[0x01u]);
      rub_DtcMiddleByte2Clear = (pAPduInd->pduData[0x02u]);
      rub_DtcLowByte2Clear = (pAPduInd->pduData[0x03u]);

      rul_DtcCode2Clear = 0x00000000u;
      rul_DtcCode2Clear |= (uint32_t)((uint32_t)rub_DtcHighByte2Clear << 16u);
      rul_DtcCode2Clear |= (uint32_t)((uint32_t)rub_DtcMiddleByte2Clear << 8u);
      rul_DtcCode2Clear |= (uint32_t)((uint32_t)rub_DtcLowByte2Clear);
      if ((rul_DtcCode2Clear != 0xFFFFFF) && (rul_DtcCode2Clear != 0xFFFF) && (rul_DtcCode2Clear != 0xFF))
      {
         lub_response = UDS_NRC_REQUESTOUTOFRANGE;
         return lub_response;
      }

      /* to decide which kind of dtc type will be cleared.
         according to value range :
         1. DTCHighByte   == 0xFFu &&
            DTCMiddleByte == 0xFFu &&
            DTCLowByte    == 0xFFu
            i.e. 3 bytes 0xFFu, means to clear all items in all of groups of DTCs
         2. 0x00u < DTCHighByte   < 0xFFu &&
            DTCMiddleByte == 0xFFu &&
            DTCLowByte    == 0xFFu
            i.e. 1st byte is not 0xFFu , rest 2 bytes 0xFFu,
               means to clear all items in the specified group of DTCs,
               which is indicated by 1st byte value
         3. 0x00u < DTCHighByte   < 0xFFu &&
            0x00u < DTCMiddleByte < 0xFFu &&
            0x00u < DTCLowByte    < 0xFFu
            i.e. 3 bytes are not 0x00u nore 0xFFu,
               means to clear a single specified DTC,
               which is indicated by these 3 bytes value,
               given it is supported by ECU currently
       */
      if (rub_DtcHighByte2Clear == 0xFFu &&
          rub_DtcMiddleByte2Clear == 0xFFu &&
          rub_DtcLowByte2Clear == 0xFFu)
      {
         rub_WhichTypeDtc2Clear = UDS_C_CLRDTC_CLRTYP_ALL_DTCS;
      }
      else if ((rub_DtcHighByte2Clear == 0xFFu) &&
               (rub_DtcMiddleByte2Clear == 0xFFu))
      {
         rub_WhichTypeDtc2Clear = UDS_C_CLRDTC_CLRTYP_SPEC_GRP_DTCS;
      }
      else if (rul_DtcCode2Clear > 0XFFu)
      {
         rub_WhichTypeDtc2Clear = UDS_C_CLRDTC_CLRTYP_SPEC_ONE_DTC;
      }
      else
      {
         rub_WhichTypeDtc2Clear = 0x00u;
         lub_response = UDS_NRC_REQUESTOUTOFRANGE;
      }

      if (lub_response == UDS_POSITIVE_RESPONSE)
      {
         rub_didtc_pending = (uint8_t)FALSE;

         lub_response = UdsApp_14ClrDiagInfor_ClearDtcAction(rul_DtcCode2Clear);

         if (lub_response == UDS_POSITIVE_RESPONSE)
         {
#if 0
	             pSavedAPduResp = pAPduResp;
	             lub_response = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING ;
	             rub_didtc_pending = (uint8_t)TRUE ;
               /* positive rsp will only have one byte : 0x54 = 0x14 + 0x40
                */
               (pAPduResp->msgLength) = 0x01u;
#else
            lub_response = UDS_POSITIVE_RESPONSE;
            (pAPduResp->msgLength) = 0x01u;
            pAPduResp->pduData[0x00u] = 0x14u; /*SID should be response now */
#endif
         }
         else
         {
         }
      }
      else
      {
      } /* end of else if (lub_response != UDS_POSITIVE_RESPONSE) */
   } /* end of else // if (0x04u == (pAPduInd->msgLength)) */

   return lub_response;

} /* end of UdsApp_14ClrDiagInfor_ReqSpecialProcess() */

/*
 * 函数介绍: 根据掩码清除DTC
 * 参数:
 * i_lul_DtcCode2Clear: 掩码
 * 返回值: {*}
 * 备注:
 */
static uint8_t UDS_FN_TYPE UdsApp_14ClrDiagInfor_ClearDtcAction(uint32_t i_lul_DtcCode2Clear)
{
   uint8_t lub_response = UDS_POSITIVE_RESPONSE;

   switch (rub_WhichTypeDtc2Clear)
   {
   default:
      /* some mistake happened in this case */
      lub_response = UDS_NRC_REQUESTOUTOFRANGE;
      break;
   /* after dtc manager is added , the below case
      can be finished.
    */
   case UDS_C_CLRDTC_CLRTYP_ALL_DTCS:
      UDS_ITF_IMP_CLR_ALL_DTCS(i_lul_DtcCode2Clear);
      break;

   case UDS_C_CLRDTC_CLRTYP_SPEC_GRP_DTCS:
      UDS_ITF_IMP_CLR_SPEC_GRP_DTCS(i_lul_DtcCode2Clear);
      break;

   case UDS_C_CLRDTC_CLRTYP_SPEC_ONE_DTC:
      UDS_ITF_IMP_CLR_SPEC_ONE_DTC(i_lul_DtcCode2Clear);
      break;

   } /* end of  switch (rub_WhichTypeDtc2Clear) */

   return lub_response;
} /* end of UdsApp_14ClrDiagInfor_ClearDtcAction() */

/*
 * 函数介绍: 挂起操作函数
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_FN_TYPE UdsApp_14ClrDiagInfor_Pending(void)
{
   uint8_t lub_response = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING; // UDS_POSITIVE_RESPONSE;

   /* the pending flag must be check first, so to make sure the pSavedAPduResp will not be null pointer
    */

   if (rub_didtc_pending == (uint8_t)TRUE)
   {
      // if (!UDS_DTCMGR_CHK_E2_UPDT_IS_BUSY)
      if (1)
      {
         rub_didtc_pending = FALSE;
         /* positive rsp will only have one byte : 0x54 = 0x14 + 0x40
          */
         lub_response = UDS_POSITIVE_RESPONSE;
         (pSavedAPduResp->msgLength) = 0x01u;
         pSavedAPduResp->pduData[0x00u] = 0x14u; /*SID should be response now */
         Uds_generic_SendResp(pSavedAPduResp, lub_response);
      }
      else
      {
         /* rub_didtc_pending = TRUE; */
      }
   }
   else
   {
   }

   return lub_response;
}
/*
 * 函数介绍: 挂起超时
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_14ClrDiagInfor_Pend_Overtime(void)
{
   // uint8_t lub_response = UDS_NRC_NORESPONSEFROMSUBNETCOMPONENT;

   if (rub_didtc_pending == (uint8_t)TRUE)
   {
      rub_didtc_pending = FALSE;
      /* (pSavedAPduResp->msgLength) = 0x01u; */
      pSavedAPduResp->pduData[0x00u] = 0x14u; /*SID should be response now */
                                              // Uds_generic_SendResp(pSavedAPduResp, lub_response);
   }
   else
   {
   }
}
/*
 * 函数介绍: 14服务初始化
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_14ClrDiagInfor_Init(void)
{
   rub_didtc_pending = (uint8_t)FALSE;
}

#endif /* UDS_ENABLE_CLEARDIAGNOSTICINFORMATION */
/******************************************************************************
 *                               end of file                                   *
 ******************************************************************************/
