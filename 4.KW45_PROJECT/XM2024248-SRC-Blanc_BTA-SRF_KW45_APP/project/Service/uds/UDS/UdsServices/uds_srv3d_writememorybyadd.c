/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv3d_writememorybyadd.c
 * 摘要: UDS Application Layer service 0x3D WriteMemoryByAddress.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_WRITEMEMORYBYADDRESS
static Std_UdsReturnType UDS_FN_TYPE UdsApp_3dWtMemByAdd_WriteData(Uds_APduData_t* pAPduInd,
                                                                   Uds_APduData_t* pAPduResp );

/* no Sun-Function parameter for this service */

/* NRC for ReadDataByIdentifier
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */

Std_UdsReturnType UDS_FN_TYPE UdsApp_WriteMemoryByAddress(Uds_APduData_t* pAPduInd,
                                                          Uds_APduData_t* pAPduResp )
{
   uint8_t lub_error = UDS_POSITIVE_RESPONSE;

   if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
   {
       lub_error = UDS_NRC_SERVICENOTSUPPORTED;
   }
   else if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_DEFAULT_SESSION)
   {
       lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
   }
   else if (UDS_C_3DWTMEMBYADD_MIN_REQ_LEN > pAPduInd->msgLength)
  {
	     lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT ;
   }
   else
   {
       lub_error = UdsApp_3dWtMemByAdd_WriteData(pAPduInd,
                                                 pAPduResp );
   }

   return lub_error;
}



static Std_UdsReturnType UDS_FN_TYPE UdsApp_3dWtMemByAdd_WriteData(Uds_APduData_t* pAPduInd,
                                                                   Uds_APduData_t* pAPduResp )

{

   uint8_t  lub_response     = UDS_POSITIVE_RESPONSE;
   uint32_t lul_InputAddress = 0x00000000u;
   uint32_t lul_InputMemSize = 0x00000000u;
   uint8_t  lub_TblIndex     = 0x00u;
   uint8_t  IsFound          = (uint8_t)FALSE;
   uint8_t  lub_PreWtChkResult = (uint8_t)FALSE;
   uint8_t  lub_WtActResult    = (uint8_t)FALSE;
   uint8_t  lub_CurrActiveSession  = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
   uint32_t *lul_pInputAddress = NULL;

   /* check the addressAndLengthFormatIdentifier must have the below fixed format :
      bit0~3 : length of memory address must be 4 bytes.
      bit4~7 : length of memory size must be 2 bytes.
    */
   if (pAPduInd->pduData[0x01u] != UDS_C_23RDMEMBYADD_FIX_ADD_LEN_FORMAT)
   {
	     lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT ;
	     return lub_response;

   }
   else
   {
   }
   lul_InputAddress  = (uint32_t)((pAPduInd->pduData[0x02u]) << 24u);
   lul_InputAddress |= (uint32_t)((pAPduInd->pduData[0x03u]) << 16u);
   lul_InputAddress |= (uint32_t)((pAPduInd->pduData[0x04u]) <<  8u);
   lul_InputAddress |= (uint32_t)((pAPduInd->pduData[0x05u])       );

   lul_InputMemSize  = (uint32_t)((pAPduInd->pduData[0x06u]) << 24u);
   lul_InputMemSize |= (uint32_t)((pAPduInd->pduData[0x07u]) << 16u);
   lul_InputMemSize |= (uint32_t)((pAPduInd->pduData[0x08u]) <<  8u);
   lul_InputMemSize |= (uint32_t)((pAPduInd->pduData[0x09u])       );

   while (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].RdWtByAddStartAdd != 0xFFFFFFFFu )
   {
          if ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].RdWtByAddStartAdd <= lul_InputAddress) &&
              ( (lul_InputAddress + lul_InputMemSize) <=
                 (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].RdWtByAddStartAdd +
                  Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].DataSize
                 )
              )
             )
          {

              IsFound = (uint8_t)TRUE;
              break;


          }
          else
          {
              lub_TblIndex++;

          }


   }  /* end of while (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].RdWtByAddStartAdd != 0xFFFFFFFFu )  */

   if (IsFound == (uint8_t)FALSE)
   {
	   lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
	   return lub_response;

   }
   else
   {

   }


   if (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].DataSize < lul_InputMemSize)
   {
	   lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
	   return lub_response;
   }
   else
   {
   }

   if ( ( (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) &
          UDS_C_DID_ATR_ENA_WT
        ) != UDS_C_DID_ATR_ENA_WT
      )
   {
       lub_response = UDS_NRC_REQUESTOUTOFRANGE;
       return lub_response;
   }
   else
   {
   }

   if ( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_WT_SESN_EXTD) ==
          UDS_C_DID_ATR_WT_SESN_EXTD ) &&
        (lub_CurrActiveSession !=  UDS_EXTENDED_DIAGNOSTIC_SESSION ) &&
        (lub_CurrActiveSession !=  UDS_ECU_SUPPLIER_SPEC_SESSION )
      )
   {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
   }
   else
   {
   }

   if ( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_WT_SESN_SSS) ==
          UDS_C_DID_ATR_WT_SESN_SSS ) &&
        (lub_CurrActiveSession !=  UDS_ECU_SUPPLIER_SPEC_SESSION )
      )
   {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
   }
   else
   {
   }

   if (( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_WT_SAL1) ==
           UDS_C_DID_ATR_WT_SAL1
         ) &&
         (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL1))
       ) ||
       ( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_WT_SAL2) ==
           UDS_C_DID_ATR_WT_SAL2) &&
         (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL2))
       ) ||
       ( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_WT_SAL3) ==
           UDS_C_DID_ATR_WT_SAL3) &&
         (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL3))
       )
      )
   {
       lub_response = UDS_NRC_SECUTITYACCESSDENIED;
       return lub_response;
   }
   else
   {
   }

   if (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pFuncPtrPreWtChk != NULL)
   {
       lub_PreWtChkResult = (*(Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pFuncPtrPreWtChk))(lul_InputAddress,
                                                                                             lul_InputMemSize,
                                                                                             &(pAPduInd->pduData[0x0Au]));

       if (lub_PreWtChkResult == (uint8_t)FALSE)
       {
           lub_response = UDS_NRC_REQUESTOUTOFRANGE;
           return lub_response;
       }
       else
       {

       }


   }
   else
   {

   }

   /* write action is successfully finished, can prepare the response message buffer
    */
   /* the request buffer content has already been copied into response buffer copy 1 or more bytes
    */
   pAPduResp->msgLength = 0x0Au;

   if (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pFuncPtrToWriteData != NULL)
   {
       lul_pInputAddress = (uint32_t *)lul_InputAddress;

       lub_WtActResult = (*(Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pFuncPtrToWriteData))((uint32_t *)lul_pInputAddress,
                                                                                             lul_InputMemSize,
                                                                                             (uint32_t *)&(pAPduInd->pduData[0x0Au]));
       if (lub_WtActResult == (uint8_t)FALSE)
       {
           lub_response = UDS_NRC_REQUESTOUTOFRANGE;
           return lub_response;
       }
       else
       {
       }


   }
   else
   {

   }



   return lub_response;


}  /* end of UdsApp_3dWtMemByAdd_WriteData()  */





#endif /* UDS_ENABLE_WRITEMEMORYBYADDRESS */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
