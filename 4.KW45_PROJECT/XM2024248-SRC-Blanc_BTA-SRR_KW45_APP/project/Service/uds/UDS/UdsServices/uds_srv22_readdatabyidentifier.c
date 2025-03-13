/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv22_readdatabyidentifier.c
 * 摘要: UDS Application Layer service 0x22 ReadDataByIdentifier.
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

#ifdef UDS_ENABLE_READDATABYIDENTIFIER

static Std_UdsReturnType UDS_FN_TYPE UdsApp_22RdDatByDid_ReadData(Uds_APduData_t *pAPduInd,
                                                                  Uds_APduData_t *pAPduResp);

/*
 * 函数介绍: 读DID数据
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 *  NRC for ReadDataByIdentifier
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */

Std_UdsReturnType UDS_FN_TYPE UdsApp_ReadDataByIdentifier(Uds_APduData_t *pAPduInd,
                                                          Uds_APduData_t *pAPduResp)
{
  uint8_t lub_error = UDS_POSITIVE_RESPONSE;
  /*Check whether the pointer is empty*/
  if (pAPduInd == NULL || pAPduResp == NULL)
  {
    return UDS_NO_RESPONSE;
  }
  else
  {
    /* code */
  }
#if 0
  if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
  {
    lub_error = UDS_NRC_SERVICENOTSUPPORTED;
  }
#endif
   if (0x02u > pAPduInd->msgLength ||
           UDS_C_RD_DAT_BY_DID_MAX_REQ_LEN < pAPduInd->msgLength ||
           0x00u != ((pAPduInd->msgLength - 0x01u) % 0x02u))
  {
    lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }
  else
  {
    lub_error = UdsApp_22RdDatByDid_ReadData(pAPduInd,
                                             pAPduResp);
  }

  return lub_error;
} /* end of Uds_ReadDataByIdentifier()  */
#ifdef NIO_PRJ_DID_CHECK
static Std_UdsReturnType UDS_FN_TYPE UdsApp_22RdDatByDid_Didcheck(uint16_t read_Didnum)
{
	uint8_t lub_response = UDS_POSITIVE_RESPONSE;
	uint8_t i;
	uint16_t BAT_SRF_Support_didArray[9] = {0x4A00,0x4A01,0x4A02,0x4A03,0x4A04,0x4A05,0x4A06,0x4A08,0x4A09};
#ifndef UdsApi_BTA_SRF_FUN_EN
	for(i = 0;i < 9;i++)
	{
		if(read_Didnum == BAT_SRF_Support_didArray[i])
		{
			lub_response = UDS_NRC_REQUESTOUTOFRANGE;
		}
		else
		{}
	}
#endif
#if ((UdsApi_EXTERNAL_BTA_FUN_EN != 1) && (UdsApi_BTA_RB_FUN_EN != 1))
	if(0x4A43 == read_Didnum)
	{
		lub_response = UDS_NRC_REQUESTOUTOFRANGE;
	}
	else
	{}
#endif
#ifndef UdsApi_BTA_SRR_FUN_EN
	if(0x4A33 == read_Didnum)
	{
		lub_response = UDS_NRC_REQUESTOUTOFRANGE;
	}
	else
	{}
#endif
#ifndef UdsApi_BTA_L_FUN_EN
	if(0x4A13 == read_Didnum)
	{
		lub_response = UDS_NRC_REQUESTOUTOFRANGE;
	}
	else
	{}
#endif
#ifndef UdsApi_BTA_R_FUN_EN
	if(0x4A23 == read_Didnum)
	{
		lub_response = UDS_NRC_REQUESTOUTOFRANGE;
	}
	else
	{}
#endif

    /*只有BTA_SRF和BTA_SRR支持*/
#if (1 == UdsApi_BTA_R_FUN_EN || 1 == UdsApi_BTA_L_FUN_EN || 1 == UdsApi_EXTERNAL_BTA_FUN_EN  || 1 == UdsApi_BTA_RB_FUN_EN)
    if((0x4A53 == read_Didnum)|| (0x4A54 == read_Didnum) || (0x4A55 == read_Didnum)||(0xF130 == read_Didnum) ||(0xF131 == read_Didnum) || (0xF151 == read_Didnum))
    {
    	 lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    	 return lub_response;
    }
    else
    {}
#endif

#ifndef UdsApi_BTA_L_FUN_EN
    /*只有BTA_L支持*/
	if(0x4A1F == read_Didnum)
	{
		lub_response = UDS_NRC_REQUESTOUTOFRANGE;
		return lub_response;

	}
	else
	{}
#endif
	if(0xF15A == read_Didnum)
	{
		lub_response = UDS_NRC_REQUESTOUTOFRANGE;
	}
	else
	{}
	return lub_response;
}
#endif
/*
 * 函数介绍: 读DID数据
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE UdsApp_22RdDatByDid_ReadData(Uds_APduData_t *pAPduInd,
                                                                  Uds_APduData_t *pAPduResp)
{
  uint8_t lub_response = UDS_POSITIVE_RESPONSE;
  uint8_t lub_indexID = UDS_C_IDX_NOT_IN_TBL;
  uint8_t lub_tmpInputPidNum = 0x00u;
  uint8_t lub_tmpInputPidIndex = 0x00u;
  uint16_t luw_tmpInDid2Rd = 0x0000u;
  uint8_t *p_lub_InPtrDid = NULL;
  uint8_t *p_lub_OutPtrDid = NULL;
  uint8_t lub_CurrActiveSession = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
  uint8_t luw_tmpOutAllPidDatLen = 0x0000u;

  lub_tmpInputPidNum = (uint8_t)((pAPduInd->msgLength - 0x01u) / 0x02u);
  p_lub_InPtrDid = (&(pAPduInd->pduData[0x01u]));
  p_lub_OutPtrDid = (&(pAPduResp->pduData[0x01u]));
  lub_tmpInputPidIndex = 0x00u;
  /*Check whether the pointer is empty*/
  if ((NULL ==pAPduInd) || (NULL == pAPduResp))
  {
    return UDS_NRC_GENERALREJECT;
  }
  else
  {
    /* code */
  }
  while (lub_tmpInputPidNum > 0x00u)
  {
    luw_tmpInDid2Rd = (uint16_t)((p_lub_InPtrDid[lub_tmpInputPidIndex]) << 0x08u);
    luw_tmpInDid2Rd |= (uint16_t)(p_lub_InPtrDid[lub_tmpInputPidIndex + 0x01u]);
    lub_indexID = Uds_Generic_DatMgr_SearchTblByKey(luw_tmpInDid2Rd,
                                                    KeyTypeIsUINT16,
                                                    (uint8_t *)&(Uds_g_DidDataTable[0x00u].dataIdentifier),
                                                    sizeof(Uds_g_DidDataTable[0x00u]));
#ifdef NIO_PRJ_DID_CHECK
    lub_response = UdsApp_22RdDatByDid_Didcheck(luw_tmpInDid2Rd);
    if(lub_response != UDS_POSITIVE_RESPONSE)
    {
    	return lub_response;
    }
    else
    {}
#endif
    if (lub_indexID == UDS_C_IDX_NOT_IN_TBL )
    {
      lub_response = UDS_NRC_REQUESTOUTOFRANGE;
      return lub_response;
    }
    else /* if (lub_indexID != UDS_C_IDX_NOT_IN_TBL) */
    {
      if (((Uds_g_DidDataTable[lub_indexID].AccessAttribute) &
           UDS_C_DID_ATR_ENA_RD) != UDS_C_DID_ATR_ENA_RD)
      {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
      }
      else
      {
      }

      if (((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_EXTD) ==
            UDS_C_DID_ATR_RD_SESN_EXTD) &&
           (((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_SSS) ==
            UDS_C_DID_ATR_RD_SESN_SSS)) &&
          (lub_CurrActiveSession != UDS_EXTENDED_DIAGNOSTIC_SESSION) &&
          (lub_CurrActiveSession != UDS_DEFAULT_SESSION) &&
          (lub_CurrActiveSession != UDS_INTERNAL_SESSION))
      {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
      }
      else if (((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_SSS) ==
                 UDS_C_DID_ATR_RD_SESN_SSS) &&
                (((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_EXTD) !=
                 UDS_C_DID_ATR_RD_SESN_EXTD)) &&
               lub_CurrActiveSession != UDS_INTERNAL_SESSION)
      {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
      }
      else
      {
        /* code */
      }

      if (((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SAL1) ==
            UDS_C_DID_ATR_RD_SAL1) &&
           (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL1))))
      {
        lub_response = UDS_NRC_SECUTITYACCESSDENIED;
        return lub_response;
      }
      else if ((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SAL2) ==
                UDS_C_DID_ATR_RD_SAL2) &&
               (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL4)))
      {
        lub_response = UDS_NRC_SECUTITYACCESSDENIED;
        return lub_response;
      }

      p_lub_OutPtrDid[luw_tmpOutAllPidDatLen] = p_lub_InPtrDid[lub_tmpInputPidIndex];
      luw_tmpOutAllPidDatLen++;
      p_lub_OutPtrDid[luw_tmpOutAllPidDatLen] = p_lub_InPtrDid[(lub_tmpInputPidIndex + 0x01u)];
      luw_tmpOutAllPidDatLen++;

      /* clear the diagnostic buffer */
      Uds_MemClear(&p_lub_OutPtrDid[luw_tmpOutAllPidDatLen],
                   Uds_g_DidDataTable[lub_indexID].DataSize);
      if ((Uds_g_DidDataTable[lub_indexID].pFuncPtrToReadData) != NULL)
      {

        (*(Uds_g_DidDataTable[lub_indexID].pFuncPtrToReadData))(Uds_g_DidDataTable[lub_indexID].pDataBuf);
      }
      else
      {
      }

      /* copy 1 or more bytes */
      Uds_MemCopy(&p_lub_OutPtrDid[luw_tmpOutAllPidDatLen],
                  Uds_g_DidDataTable[lub_indexID].pDataBuf,
                  Uds_g_DidDataTable[lub_indexID].DataSize);

      /* compute the length */
      luw_tmpOutAllPidDatLen = luw_tmpOutAllPidDatLen + Uds_g_DidDataTable[lub_indexID].DataSize;

    } /* end of else of if (lub_indexID != UDS_C_IDX_NOT_IN_TBL) */

    lub_tmpInputPidNum--;
    lub_tmpInputPidIndex = lub_tmpInputPidIndex + 0x02u;

  } /* end of while (lub_tmpInputPidNum > 0x00u) */

  pAPduResp->msgLength = (uint16_t)(luw_tmpOutAllPidDatLen + 0x01u);
#ifdef EP40_DEF
  if ((0xFEFC == luw_tmpInDid2Rd))
  {
    lub_response = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING;
  }
#endif
  return lub_response;
} /* end of Uds_22RdDatByDid_ReadData() */

uint8_t UDS_FN_TYPE UdsApp_22RdDatByDid_ChkDidValidity(uint16_t i_luw_RefDid2Chk)
{
  uint8_t lub_response = UDS_POSITIVE_RESPONSE;
  uint8_t lub_indexID = UDS_C_IDX_NOT_IN_TBL;
  uint8_t lub_CurrActiveSession = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();

  lub_indexID = Uds_Generic_DatMgr_SearchTblByKey(i_luw_RefDid2Chk,
                                                  KeyTypeIsUINT16,
                                                  (uint8_t *)&(Uds_g_DidDataTable[0x00u].dataIdentifier),
                                                  sizeof(Uds_g_DidDataTable[0x00u]));

  if (lub_indexID == UDS_C_IDX_NOT_IN_TBL)
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else /* if (lub_indexID != UDS_C_IDX_NOT_IN_TBL) */
  {
  }

  if (((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_ENA_RD) != UDS_C_DID_ATR_ENA_RD)
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else
  {
  }

  if ((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_EXTD) ==
       UDS_C_DID_ATR_RD_SESN_EXTD) &&
      (lub_CurrActiveSession != UDS_EXTENDED_DIAGNOSTIC_SESSION) &&
      (lub_CurrActiveSession != UDS_ECU_SUPPLIER_SPEC_SESSION) &&
      (lub_CurrActiveSession != UDS_PROGRAMMING_SESSION))
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else
  {
  }

  if ((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_SSS) ==
       UDS_C_DID_ATR_RD_SESN_SSS) &&
      (lub_CurrActiveSession != UDS_ECU_SUPPLIER_SPEC_SESSION) &&
      (lub_CurrActiveSession != UDS_PROGRAMMING_SESSION) &&
      (lub_CurrActiveSession == UDS_INTERNAL_SESSION))
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else
  {
  }

  if ((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_PRGM) ==
       UDS_C_DID_ATR_RD_SESN_PRGM) &&
      (lub_CurrActiveSession != UDS_PROGRAMMING_SESSION))
  {
    lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    return lub_response;
  }
  else
  {
  }

  if (((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SAL1) ==
        UDS_C_DID_ATR_RD_SAL1) &&
       (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL1))) ||
      ((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SAL2) ==
        UDS_C_DID_ATR_RD_SAL2) &&
       ((!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL2)) ||
        (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL4)))) ||
      ((((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SAL3) ==
        UDS_C_DID_ATR_RD_SAL3) &&
       (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL3))))
  {
    lub_response = UDS_NRC_SECUTITYACCESSDENIED;
    return lub_response;
  }
  else
  {
  }
  return lub_response;
} /* end of UdsApp_22RdDatByDid_ChkDidValidity() */

#endif /* UDS_ENABLE_READDATABYIDENTIFIER */
/******************************************************************************
 *                               end of file                                   *
 ******************************************************************************/
