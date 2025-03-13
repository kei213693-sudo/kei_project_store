/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv23_readmemorybyaddress.c
 * 摘要: UDS Application Layer service 0x23 ReadMemoryByAddress.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_READMEMORYBYADDRESS


static Std_UdsReturnType UDS_FN_TYPE UdsApp_23RdMemByAdd_ReadData(Uds_APduData_t* pAPduInd,
                                                                  Uds_APduData_t* pAPduResp );


/* no Sun-Function parameter for this service */

/* NRC for ReadMemoryByAddress
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */

Std_UdsReturnType UDS_FN_TYPE UdsApp_ReadMemoryByAddress(Uds_APduData_t* pAPduInd,
                                                      Uds_APduData_t* pAPduResp )
{
   uint8_t lub_error = UDS_POSITIVE_RESPONSE;
   /* the 0x23 ReadMemoryByAddress can only be allowed running in physical addressing channel
    */
   if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
   {
       lub_error = UDS_NRC_SERVICENOTSUPPORTED;
   }
   /* the 0x23 ReadMemoryByAddress can only be allowed running in non-default session
    */
   else if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_DEFAULT_SESSION)
   {
       lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
   }
   /* the 0x23 ReadMemoryByAddress valid request length is 10 bytes
    */
   else if (UDS_C_23RDMEMBYADD_VALID_REQ_LEN  != pAPduInd->msgLength)
   {
	     lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT ;
   }
   else
   {
       lub_error = UdsApp_23RdMemByAdd_ReadData(pAPduInd,
                                                pAPduResp );
   }

   return lub_error;
}


static Std_UdsReturnType UDS_FN_TYPE UdsApp_23RdMemByAdd_ReadData(Uds_APduData_t* pAPduInd,
                                                                  Uds_APduData_t* pAPduResp )

{
   uint8_t  lub_response     = UDS_POSITIVE_RESPONSE;
   uint32_t lul_InputAddress = 0x00000000u;
   uint32_t lul_InputMemSize = 0x0000u;
   uint8_t  lub_TblIndex     = 0x00u;
   uint8_t  IsFound          = (uint8_t)FALSE;
   uint8_t  lub_PreRdChkResult = (uint8_t)FALSE;
   uint8_t  lub_RdActResult    = (uint8_t)FALSE;
   uint8_t  lub_CurrActiveSession  = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();

   /* check the addressAndLengthFormatIdentifier must have the below fixed format :
      bit0~3 : length of memory address must be 4 bytes.
      bit4~7 : length of memory size must be 4 bytes.
    */
   if (pAPduInd->pduData[0x01u] != UDS_C_23RDMEMBYADD_FIX_ADD_LEN_FORMAT)
   {
	     lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT ;
	     return lub_response;

   }
   else
   {
   }
   lul_InputAddress  =  (uint32_t)((pAPduInd->pduData[0x02u]) << 24u);
   lul_InputAddress |= (uint32_t)((pAPduInd->pduData[0x03u]) << 16u);
   lul_InputAddress |= (uint32_t)((pAPduInd->pduData[0x04u]) <<  8u);
   lul_InputAddress |= (uint32_t)((pAPduInd->pduData[0x05u])       );

   lul_InputMemSize  = (uint16_t)((pAPduInd->pduData[0x06u])   << 24u);
   lul_InputMemSize |= (uint16_t)((pAPduInd->pduData[0x07u])  << 16u);
   lul_InputMemSize |= (uint16_t)((pAPduInd->pduData[0x08u])  <<   8u);
   lul_InputMemSize |= (uint16_t)((pAPduInd->pduData[0x09u])             );

   while (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].RdWtByAddStartAdd != 0xFFFFFFFFu )
   {
      /* check the starting address must be not less than the configured address in table
         and the end address must be also in the valid address range.
       */
          if ((lul_InputAddress >= Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].RdWtByAddStartAdd) &&
              (lul_InputAddress + lul_InputMemSize) <= (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].RdWtByAddStartAdd +
                                                                              Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].DataSize))
          {
              IsFound = (uint8_t)TRUE;
              break;
          }
          else
          {
              lub_TblIndex++;
          }
   }

   if (IsFound == (uint8_t)FALSE)
   {
       lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
       return lub_response;
   }
   else
   {
   }

   /* to check the configured access attribute is allowing to read
   */
   if ( ( (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) &
          UDS_C_DID_ATR_ENA_RD
        ) != UDS_C_DID_ATR_ENA_RD
      )
   {
       lub_response = UDS_NRC_REQUESTOUTOFRANGE;
       return lub_response;
   }
   else
   {
   }

   /* if the configured session is extended session , then to check if the current active session is correct or not
    */
   if ( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_EXTD) ==
          UDS_C_DID_ATR_RD_SESN_EXTD ) &&
        (lub_CurrActiveSession !=  UDS_EXTENDED_DIAGNOSTIC_SESSION ) &&
        (lub_CurrActiveSession !=  UDS_ECU_SUPPLIER_SPEC_SESSION )  &&
        (lub_CurrActiveSession !=  UDS_PROGRAMMING_SESSION )
      )
   {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
   }
   else
   {
   }

   /* if currently configured session is supplier specific session , then this service can only be allowed to access in supplier
       specific session (SSS) and programming session
    */
   if ( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_SSS) ==
          UDS_C_DID_ATR_RD_SESN_SSS ) &&
        (lub_CurrActiveSession !=  UDS_ECU_SUPPLIER_SPEC_SESSION )  &&
        (lub_CurrActiveSession !=  UDS_PROGRAMMING_SESSION )
      )
   {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
   }
   else
   {
   }

   /* if the configured session is programming session , then the current active session must be also programming session too
    */
   if ( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_PRGM) ==
          UDS_C_DID_ATR_RD_SESN_PRGM ) &&
        (lub_CurrActiveSession !=  UDS_PROGRAMMING_SESSION )
      )
   {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
   }
   else
   {
   }

   /* if the security access level (SAL) is configured , then to check current the responding SAL is unlocked or not
    */
   if (( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SAL1) ==
           UDS_C_DID_ATR_RD_SAL1
         ) &&
         (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL1))
       ) ||
       ( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SAL2) ==
           UDS_C_DID_ATR_RD_SAL2) &&
         (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL2))
       ) ||
       ( ( ((Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SAL3) ==
           UDS_C_DID_ATR_RD_SAL3) &&
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

   /* if the customer provided pre-reading check function pointer is not null , this means
       the customer specific checking interface must be run , before to allow to read the memory
     */
   if (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pFuncPtrPreRdChk != NULL)
   {
       lub_PreRdChkResult = (*(Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pFuncPtrPreRdChk))(lul_InputAddress,
                                                                                             lul_InputMemSize);
       if (lub_PreRdChkResult == (uint8_t)FALSE)
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

   /* if customer provide a reading function interface to read data , then to call it to read data
    */
   if (Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pFuncPtrToReadData != NULL)
   {
       lub_RdActResult = (*(Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pFuncPtrToReadData))(lul_InputAddress,
                                                                                            lul_InputMemSize,
                                                                                            Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pDataBuf);
       if (lub_RdActResult == (uint8_t)FALSE)
       {
           lub_response = UDS_NRC_REQUESTOUTOFRANGE;
           return lub_response;
       }
       else
       {
           /* read action is successfully finished, can prepare the response message buffer
            */
				   /* copy 1 or more bytes */
           Uds_MemCopy(&pAPduResp->pduData[0x01u],
				               Uds_g_RdWtMemoryByAddrMngTbl[lub_TblIndex].pDataBuf,
				               lul_InputMemSize);

           pAPduResp->msgLength = lul_InputMemSize + 0x01u;
       }


   }
   else
   {
        /* copy 1 or more bytes */
        /* if customer does not provide a reading function interface to read data , then to read data from the
            specified address space directly
          */

        Uds_MemCopy(&pAPduResp->pduData[0x01u],
                                 (uint8_t*) lul_InputAddress,
		                   lul_InputMemSize);

         pAPduResp->msgLength = lul_InputMemSize + 0x01u;
   }


   return lub_response;

} /* end of UdsApp_23RdMemByAdd_ReadData()  */


#endif /* UDS_ENABLE_READMEMORYBYADDRESS */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
