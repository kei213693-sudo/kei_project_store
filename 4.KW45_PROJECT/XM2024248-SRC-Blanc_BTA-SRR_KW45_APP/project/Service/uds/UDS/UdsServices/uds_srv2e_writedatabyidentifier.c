/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv2e_writedatabyidentifier.c
 * 摘要: UDS Application Layer service 0x2E WriteDataByIdentifier.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "uds_confdefs.h"
#include "uds_confpriv.h"
#include "uds_inf.h"
#include "uds_priv.h"
#include "uds_target.h"
#include <stddef.h>
#include "uds_api.h"

#ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER

static uint8_t rub_2eWtDatByDid_PendingFlg = (uint8_t)FALSE;
static uint8_t rub_2eWtDatByDid_CurrUsingTblIndex = 0x00u;
static Uds_APduData_t *pSavedAPduResp;

static Std_UdsReturnType UDS_FN_TYPE UdsApp_2eWtDatByDid_WriteDataProc(Uds_APduData_t *pAPduInd,
                                                                       Uds_APduData_t *pAPduResp);

/*
 * 函数介绍: 写操作处理函数
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注: NRC for ReadDataByIdentifier
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_WriteDataByIdentifier(Uds_APduData_t *pAPduInd,
                                                           Uds_APduData_t *pAPduResp)
{
  uint8_t lub_error = UDS_POSITIVE_RESPONSE;

  if ((NULL == pAPduInd) || (NULL == pAPduResp))
  {
    return UDS_NRC_GENERALREJECT;
  }
  else
  {
    if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
    {
      lub_error = UDS_NRC_SERVICENOTSUPPORTED;
    }
    else if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_DEFAULT_SESSION)
    {
      lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
    }
    else if (pAPduInd->msgLength < 0x04u)
    {
      lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    else
    {
      lub_error = UdsApp_2eWtDatByDid_WriteDataProc(pAPduInd,
                                                    pAPduResp);
    }
  }

  return lub_error;

} /* end of Uds_WriteDataByIdentifier()  */
#ifdef NIO_PRJ_DID_CHECK
static Std_UdsReturnType UDS_FN_TYPE UdsApp_2eWtDatByDid_DidCheck(uint16_t WriteDid)
{
	 uint8_t lub_response = UDS_POSITIVE_RESPONSE;
	 uint8_t i;
	 uint16_t BTA_SRF_WriteDIDArr[5] = {0x4A00,0x4A01,0x4A04,0x4A04,0x4A06};
#ifndef UdsApi_BTA_L_FUN_EN
	  /*0x4A06只有BTA_L支持*/
	  if(0x4A1F == WriteDid)
	  {
		  lub_response = UDS_NRC_REQUESTOUTOFRANGE;

	  }
	  else
	  {}
#endif
#ifndef UdsApi_BTA_SRF_FUN_EN
	  for(i = 0; i<5;i++)
	  {
		  if(WriteDid == BTA_SRF_WriteDIDArr[i])
		  {
			  lub_response = UDS_NRC_REQUESTOUTOFRANGE;
		  }
		  else
		  {}
	  }
#endif
	  return lub_response;
}
#endif
static Std_UdsReturnType UDS_FN_TYPE UdsApp_2eWtDatByDid_WriteDataProc(Uds_APduData_t *pAPduInd,
                                                                       Uds_APduData_t *pAPduResp)
{
  uint8_t lub_index = 0x00u;
  // uint8_t  lub_E2DrvReturnCode = FALSE;
  uint8_t lub_response = UDS_POSITIVE_RESPONSE;
  uint16_t luw_InDid2WrtE2 = 0x0000u;
  uint8_t lub_CurrActiveSession = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
  /*Check whether the pointer is empty*/
  if (pAPduInd == NULL || pAPduResp == NULL)
  {
    return UDS_NO_RESPONSE;
  }
  else
  {
    /* code */
  }

  pAPduResp = pAPduResp; /* just to remove the compiliation warning */
  luw_InDid2WrtE2 = (uint16_t)((pAPduInd->pduData[0x01u]) << 0x08u);
  luw_InDid2WrtE2 |= (uint16_t)(pAPduInd->pduData[0x02u]);

  lub_index = Uds_Generic_DatMgr_SearchTblByKey(luw_InDid2WrtE2,
                                                KeyTypeIsUINT16,
                                                (uint8_t *)&(Uds_g_DidEepromDataTable[0x00u].dataIdentifier),
                                                sizeof(Uds_g_DidEepromDataTable[0x00u]));
#ifdef NIO_PRJ_DID_CHECK
  lub_response = UdsApp_2eWtDatByDid_DidCheck(luw_InDid2WrtE2);
  if(lub_response != UDS_POSITIVE_RESPONSE)
  {
	  return lub_response;
  }
  else
  {}
#endif
  if (lub_index == UDS_C_IDX_NOT_IN_TBL)
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else
  {
  }

  if (((Uds_g_DidEepromDataTable[lub_index].AccessAttribute) &
       (UDS_C_DID_ATR_ENA_WT)) != UDS_C_DID_ATR_ENA_WT)
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else
  {
  }

  if ((((Uds_g_DidEepromDataTable[lub_index].AccessAttribute) &
        UDS_C_DID_ATR_WT_SESN_EXTD) == UDS_C_DID_ATR_WT_SESN_EXTD) &&
      (lub_CurrActiveSession != UDS_EXTENDED_DIAGNOSTIC_SESSION)
      /*(lub_CurrActiveSession != UDS_SAFTEY_SYSTEM_DIAGNOSTIC_SESSION)*/)
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else if ((((Uds_g_DidEepromDataTable[lub_index].AccessAttribute) &
             UDS_C_DID_ATR_WT_SESN_PRGM) == UDS_C_DID_ATR_WT_SESN_PRGM) &&
           (lub_CurrActiveSession != UDS_PROGRAMMING_SESSION))
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else
  {
  }

  if ((((Uds_g_DidEepromDataTable[lub_index].AccessAttribute) &
        UDS_C_DID_ATR_WT_SESN_SSS) == UDS_C_DID_ATR_WT_SESN_SSS) &&
      /*
      (lub_CurrActiveSession !=  UDS_PROGRAMMING_SESSION) &&
      (lub_CurrActiveSession !=  UDS_SAFTEY_SYSTEM_DIAGNOSTIC_SESSION && */
      (lub_CurrActiveSession != UDS_INTERNAL_SESSION))
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else
  {
  }
  if (pAPduInd->msgLength != (0x03u + Uds_g_DidEepromDataTable[lub_index].DataSize))
  {
    lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    return lub_response;
  }
  else
  {
  }

   if (((((Uds_g_DidEepromDataTable[lub_index].AccessAttribute) & UDS_C_DID_ATR_WT_SAL1) ==
         UDS_C_DID_ATR_WT_SAL1) &&
        (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL1))) ||
       ((((Uds_g_DidEepromDataTable[lub_index].AccessAttribute) & UDS_C_DID_ATR_WT_SAL2) ==
         UDS_C_DID_ATR_WT_SAL2) &&
        (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL2))) ||
       ((((Uds_g_DidEepromDataTable[lub_index].AccessAttribute) & UDS_C_DID_ATR_WT_SAL3) ==
         UDS_C_DID_ATR_WT_SAL3) &&
        (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL3))) ||
       ((((Uds_g_DidEepromDataTable[lub_index].AccessAttribute) & UDS_C_DID_ATR_WT_SAL_S) ==
         UDS_C_DID_ATR_WT_SAL_S) &&
        (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL4))))
   {
       lub_response = UDS_NRC_SECUTITYACCESSDENIED;
       return lub_response;
   }
   else
   {
   }

  if (Uds_g_DidEepromDataTable[lub_index].pFuncPtrCheckData != NULL)
  {
    lub_response = (Uds_g_DidEepromDataTable[lub_index].pFuncPtrCheckData)(&(pAPduInd->pduData[0x03u]));
  }
  else
  {
  }

  if (UDS_APP_ITF_IMP_GET_E2_CURR_IS_BUSY(Uds_g_DidEepromDataTable[lub_index].EepromBlkIdx))
  {
    lub_response = UDS_NRC_BUSYREPEATREQUEST;
    return lub_response;
  }
  else
  {
  }

  if (UDS_POSITIVE_RESPONSE == lub_response)
  {
/* if the synchronous interface (function) provided to write data into EEPROM
   to provide the interface function , and map to the below macro ,
   and remove the #if 0  #endif to open the code
 */
#if 0
           lub_E2DrvReturnCode =  UDS_APP_ITF_IMP_SYNC_E2_WRITE_ITF(Uds_g_DidEepromDataTable[lub_index].EepromBlkIdx , // blockNum,
                                                                           &(pAPduInd->pduData[0x03u]), // pDatSrc,
                                                                            Uds_g_DidEepromDataTable[lub_index].DataSize );  // DataSize)

#endif

/* if the asynchronous interface provided to write data into EEPROM
   to provide the interface function , and map to the below macro ,
   and remove the #if 0  #endif to open the code
 */
#if 1
    /* Copy whole of the data content which will be written into
       eeprom into the configured buffer.
     */

    Uds_MemCopy(Uds_g_DidEepromDataTable[lub_index].pDataBuf,  /* pDest */
                &(pAPduInd->pduData[0x03u]),                   /* pSrc */
                Uds_g_DidEepromDataTable[lub_index].DataSize); /* numBytes */

    /* copy the data from the UDS data managing buffer
       into the Emulated Eeprom manager buffer
     */

    if (Uds_g_DidEepromDataTable[lub_index].pFuncPtrToWriteData != NULL)
    {
      (Uds_g_DidEepromDataTable[lub_index].pFuncPtrToWriteData)(Uds_g_DidEepromDataTable[lub_index].pDataBuf);
    }
    else
    {
      UDS_APP_ITF_IMP_SET_E2_WRITE_FLAG(Uds_g_DidEepromDataTable[lub_index].EepromBlkIdx);
    }
#endif
    rub_2eWtDatByDid_CurrUsingTblIndex = lub_index;
    pAPduResp->msgLength = 0x03u;

    /*first response is pending response*/
    lub_response = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING;
    pSavedAPduResp = pAPduResp;
    rub_2eWtDatByDid_PendingFlg = TRUE;
  } /* end of if (UDS_POSITIVE_RESPONSE == lub_response ) */
  else
  {
  }
  return lub_response;

} /* end of Uds_2eWtDatByDid_WriteDataProc () */

/******************************************************************************
 * Name: Uds_2eWtDatByDid_PendingStProc
 * Description: Checks the status of "Write Memory Block" request.
 *              Will send a success response when the write is finished.
 * Parameter: none
 * Return: the error code
 * Critical: Yes.
 * Explanation: Called periodically
 ******************************************************************************/
uint8_t UDS_FN_TYPE UdsApp_2eWtDatByDid_Pending(void)
{
  uint8_t lub_response = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING;

  if (rub_2eWtDatByDid_PendingFlg > 0x00u)
  {
    /* check if EEPROM driver has already finished updating the block
     */
    if (UDS_APP_ITF_IMP_GET_E2_CURR_IS_BUSY(
            Uds_g_DidEepromDataTable[rub_2eWtDatByDid_CurrUsingTblIndex].EepromBlkIdx) == (uint8_t)FALSE)
    {
      lub_response = UDS_POSITIVE_RESPONSE;
      rub_2eWtDatByDid_PendingFlg = (uint8_t)FALSE;
      (pSavedAPduResp->msgLength) = 0x03u;
      pSavedAPduResp->pduData[0x00u] = 0x2Eu; /*SID should be response now */
      pSavedAPduResp->pduData[0x01u] = (uint8_t)(((0xFF00u) & (Uds_g_DidEepromDataTable[rub_2eWtDatByDid_CurrUsingTblIndex].dataIdentifier)) >> 0x08u);
      pSavedAPduResp->pduData[0x02u] = (uint8_t)(((0x00FFu) & (Uds_g_DidEepromDataTable[rub_2eWtDatByDid_CurrUsingTblIndex].dataIdentifier)));

      Uds_generic_SendResp(pSavedAPduResp, lub_response);
    }
    else
    {
    }
  }
  else
  {
  }

  return lub_response;
} /* end of Uds_2eWtDatByDid_Pending() */

void UDS_FN_TYPE UdsApp_2eWtDatByDid_Pend_Overtime(void)
{
  uint8_t lub_response = UDS_NRC_NORESPONSEFROMSUBNETCOMPONENT;

  pSavedAPduResp->pduData[0x00u] = 0x2Eu; /*SID should be response now */

  rub_2eWtDatByDid_PendingFlg = (uint8_t)FALSE;
  rub_2eWtDatByDid_CurrUsingTblIndex = (uint8_t)0x00u;
  Uds_generic_SendResp(pSavedAPduResp, lub_response);
} /* end of Uds_2eWtDatByDid_Pend_Overtime() */

void UDS_FN_TYPE UdsApp_2eWtDatByDid_Init(void)
{
  rub_2eWtDatByDid_PendingFlg = (uint8_t)FALSE;
  rub_2eWtDatByDid_CurrUsingTblIndex = (uint8_t)0x00u;
}

#endif /* UDS_ENABLE_WRITEDATABYIDENTIFIER */
/******************************************************************************
 *                               end of file                                   *
 ******************************************************************************/
