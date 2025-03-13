/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv35_requestupload.c
 * 摘要: UDS Application Layer service 0x35 RequestUpload.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"
#include "uds_pub.h"

#ifdef UDS_ENABLE_REQUESTUPLOAD

static UDS_ReqDnLdUpLdMemRange_t rub_CurrUpldMemRange = UDS_C_REQDNLD_UPLD_MEM_RANGE_INIT;
static uint32_t rul_InputMemAdd2ReqUpld       = 0x00000000u;
static uint32_t rul_InputMemSize2ReqUpld      = 0x00000000u;

/* no Sun-Function parameter for this service */

/* NRC for ReadDataByIdentifier
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */

static Std_UdsReturnType UDS_FN_TYPE UdsApp_35ReqUpld_ReqProc(Uds_APduData_t* pAPduInd,
                                                              Uds_APduData_t* pAPduResp );

Std_UdsReturnType UDS_FN_TYPE UdsApp_RequestUpload(Uds_APduData_t* pAPduInd,
                                                   Uds_APduData_t* pAPduResp )
{
   uint8_t lub_error = UDS_POSITIVE_RESPONSE;
   uint8_t  lub_CurrActiveSession = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();

   if (UDS_APP_ITF_IMP_GET_ECU_OVR_VOLT_FLG()) /* to get the overvoltage flag */
   {
       /* if the current battery supplier voltage is higher than the over threshold , not to
           allow to upload operation
	  */
       lub_error = UDS_NRC_VOLTAGETOOHIGH;

   }
   else if (UDS_APP_ITF_IMP_GET_ECU_UDR_VOLT_FLG()) /* to get the undervoltage flag */
   {
      /* if the current battery supplier voltage is lower than the under threshold , not to
           allow to upload operation
	  */
       lub_error = UDS_NRC_VOLTAGETOOLOW;
   }
   else if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
   {
       /* if this request is received from the functional addressing channel
          reject it.
        */
       lub_error = UDS_NRC_SERVICENOTSUPPORTED;
   }
   else if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_DEFAULT_SESSION)
   {
       /* if the current active session is still default session ,
          reject it.
        */
       lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
   }
   else  if ( lub_CurrActiveSession !=  UDS_PROGRAMMING_SESSION &&
   	          lub_CurrActiveSession !=  UDS_EXTENDED_DIAGNOSTIC_SESSION &&
   	          lub_CurrActiveSession !=  UDS_ECU_SUPPLIER_SPEC_SESSION)
   {
        /* the request download and request upload can only be allowed in the
           programming session (0x02) , extended session (0x03) , and the supplier specific session (0x60)
         */
        lub_error = UDS_NRC_REQUESTOUTOFRANGE;
   }
   else if (pAPduInd->msgLength != UDS_C_35REQUPLD_VALID_REQ_LEN)
   {
        /* the 0x35 request upload valid request length is : 11  bytes
         */
       lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else if (pAPduInd->pduData[0x01u] != UDS_C_35REQUPLD_VALID_DAT_FMT_ID)
   {
        /* the buffer[1] is data format identifier byte : must be 0x00, means:
          no encryptingMethod, nor compressMethod.
         */
	     lub_error = UDS_NRC_REQUESTOUTOFRANGE ;
   }
   else if (pAPduInd->pduData[0x02u] != UDS_C_35REQUPLD_VALID_ADD_LEN_FMT_ID)
   {
        /* the buffer[2] is address and length format identifier byte :
           must be 0x44, means:
           MemoryAddress have 4 bytes , 32 bits, and
           MemorySize have also 4 bytes, 32 bits.
         */
	     lub_error = UDS_NRC_REQUESTOUTOFRANGE ;
   }
   else if (UdsApp_36TransferDat_GetCurrTsfrDatSt() != UDS_C_TANSFER_DAT_ST_INIT)
   {
        /* currently the transfer data state should be init , i.e.
           no data is transferring
           for previous downloading or uploading
	     */
       lub_error = UDS_NRC_UPLOADDOWNLOADNOTACCEPTED;
   }
   else
   {
        /* the initial checking is valid , then to go to the service processing function
           to do the more action for this service
	    */
       lub_error = UdsApp_35ReqUpld_ReqProc(pAPduInd,
                                            pAPduResp );
   }

   return lub_error;
}/* end of UdsApp_RequestUpload() */

static Std_UdsReturnType UDS_FN_TYPE UdsApp_35ReqUpld_ReqProc(Uds_APduData_t* pAPduInd,
                                                              Uds_APduData_t* pAPduResp )
{
   uint8_t  lub_response          = UDS_POSITIVE_RESPONSE; /* init the response code to postive response first */
   uint8_t  lub_TblIndex          = 0x00u;
   uint8_t  IsFound               = (uint8_t)FALSE;
   uint8_t  lub_PreUpldChkResult  = (uint8_t)FALSE;
   uint8_t  lub_UpldRdDatResult   = (uint8_t)FALSE;
   uint8_t  lub_CurrActiveSession = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION(); /* read in the current active session */

   uint16_t luw_MaxNumOfBlkLen    = (uint16_t)0x0000u;

   /* read into the memory address and memory size for the 0x35 request upload service
    */
   rul_InputMemAdd2ReqUpld   = (uint32_t)((pAPduInd->pduData[0x03u]) << 24u);
   rul_InputMemAdd2ReqUpld  |= (uint32_t)((pAPduInd->pduData[0x04u]) << 16u);
   rul_InputMemAdd2ReqUpld  |= (uint32_t)((pAPduInd->pduData[0x05u]) <<  8u);
   rul_InputMemAdd2ReqUpld  |= (uint32_t)((pAPduInd->pduData[0x06u])       );

   rul_InputMemSize2ReqUpld  = (uint32_t)((pAPduInd->pduData[0x07u]) << 24u);
   rul_InputMemSize2ReqUpld |= (uint32_t)((pAPduInd->pduData[0x08u]) << 16u);
   rul_InputMemSize2ReqUpld |= (uint32_t)((pAPduInd->pduData[0x09u]) <<  8u);
   rul_InputMemSize2ReqUpld |= (uint32_t)((pAPduInd->pduData[0x0Au])       );

   /* to check the upload source memory address is located in the RAM range or flash range.
    */
   if (rul_InputMemAdd2ReqUpld  >= UDS_C_35REQUPLD_VALID_RAM_START_ADD &&
       rul_InputMemAdd2ReqUpld  <= UDS_C_35REQUPLD_VALID_RAM_END_ADD)
   {
	   /* this means the request upload targeting address is in the RAM range
	    */
       rub_CurrUpldMemRange = UDS_C_REQDNLD_UPLD_MEM_RANGE_RAM;

   }
   else if (rul_InputMemAdd2ReqUpld  >= UDS_C_35REQUPLD_VALID_FLASH_START_ADD &&
            rul_InputMemAdd2ReqUpld  <= UDS_C_35REQUPLD_VALID_FLASH_END_ADD)
   {
       /* this means the request upload targeting address is in the FLASH range
        */
       rub_CurrUpldMemRange = UDS_C_REQDNLD_UPLD_MEM_RANGE_FLASH;
   }
   else
   {
       /* the input memory address is neither in RAM range , nor in FLASH range
	    */
       lub_response = UDS_NRC_REQUESTOUTOFRANGE;
       return lub_response;
   }

   /* to search the service configuration database table to find the range
    */
   while (Uds_g_ReqUpldMngTbl[lub_TblIndex].ReqUpldStartMemAdd != 0xFFFFFFFFu)
   {
	  /* The requested address and size is located in the configured table item range
		 this record is found .
	   */
	  if ((rul_InputMemAdd2ReqUpld >= Uds_g_ReqUpldMngTbl[lub_TblIndex].ReqUpldStartMemAdd) &&
		  (rul_InputMemAdd2ReqUpld + rul_InputMemSize2ReqUpld) <=
		  (Uds_g_ReqUpldMngTbl[lub_TblIndex].ReqUpldStartMemAdd +
		   Uds_g_ReqUpldMngTbl[lub_TblIndex].MemorySize))
	  {
		  IsFound = (uint8_t)TRUE;
		  break;
	  }
	  else
	  {
		  lub_TblIndex++;
	  }
   }  /* end of while (Uds_g_ReqUpldMngTbl[lub_TblIndex].ReqUpldStartMemAdd != 0xFFFFFFFFu)
       */

   if (IsFound == (uint8_t)FALSE)
   {
       /* if not find the configured memory range , then reject request
        */
	   lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
	   return lub_response;
   }
   else
   {
   }

   /* to check the readable access attribute
    */
   if ( ( (Uds_g_ReqUpldMngTbl[lub_TblIndex].AccessAttribute) &
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

   /* to check the data buffer should be valid
    */
   if ( Uds_g_ReqUpldMngTbl[lub_TblIndex].pDataBuf == NULL)
   {
       lub_response = UDS_NRC_UPLOADDOWNLOADNOTACCEPTED;
       return lub_response;
   }
   else
   {
   }

   /* if the configured session is extended session , then to check if the current
      active session is correct or not
    */
   if ( ( ((Uds_g_ReqUpldMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_EXTD) ==
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

   /* if currently configured session is supplier specific session , then this
      service can only be allowed to access in supplier
       specific session (SSS) and programming session
    */
   if ( ( ((Uds_g_ReqUpldMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_SSS) ==
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

   /* if currently configured session is programming session , then
      this service can only be allowed to run in
       the programming session
    */
   if ( ( ((Uds_g_ReqUpldMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_PRGM) ==
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

   /* to check the configured security access level and the currently unlocked SAL
    */
   if (( ( ((Uds_g_ReqUpldMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SAL1) ==
           UDS_C_DID_ATR_RD_SAL1
         ) &&
         (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL1))
       ) ||
       ( ( ((Uds_g_ReqUpldMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SAL2) ==
           UDS_C_DID_ATR_RD_SAL2) &&
         (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL2))
       ) ||
       ( ( ((Uds_g_ReqUpldMngTbl[lub_TblIndex].AccessAttribute) & UDS_C_DID_ATR_RD_SAL3) ==
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

    /* if the customer provided pre-reading check function pointer is not null ,
       this means the customer specific checking interface must be run , before
       to allow to request to upload from this memory
     */
   if (Uds_g_ReqUpldMngTbl[lub_TblIndex].pFuncPtrPreUpldChk != NULL)
   {
       lub_PreUpldChkResult = (*(Uds_g_ReqUpldMngTbl[lub_TblIndex].pFuncPtrPreUpldChk))(rul_InputMemAdd2ReqUpld,
                                                                rul_InputMemSize2ReqUpld,
                                                                &luw_MaxNumOfBlkLen);
       if (lub_PreUpldChkResult == (uint8_t)FALSE)
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
        luw_MaxNumOfBlkLen = UDS_C_35REQUPLD_USING_NUM_BLK_LEN;
   }

   /* to check the max allowable number of block length
    */
   if (luw_MaxNumOfBlkLen == 0x0000u ||
	   luw_MaxNumOfBlkLen >  UDS_C_35REQUPLD_VALID_MAX_NUM_BLK_LEN)
   {
	   lub_response = UDS_NRC_UPLOADDOWNLOADNOTACCEPTED;
	   return lub_response;
   }
   else
   {
	   /* request uploading action is successfully finished,
		  can prepare the response message buffer
		  the request buffer content has already been copied into response buffer
		  1 or more bytes,
		  valid response size is 4 bytes.
		*/
	   /* rub_ReqDnldTblIdx = lub_TblIndex; */

	   UdsApp_36TransferDat_SetCurrTsfrDatSt(UDS_C_TANSFER_DAT_ST_UPLD);
	   UdsApp_36TransferDat_InitBlkSeqCntrBefrTsfrDat();

	   pAPduResp->msgLength = UDS_C_35REQUPLD_VALID_RSP_MSG_LEN;
	   /* the valid value for lengthFormatIdentifier must be 0x20, i.e.
		  the following field of maxNumberOfBlockLength is 2 bytes.
		*/
	   pAPduResp->pduData[0x01u] = UDS_C_35REQUPLD_VALID_RSP_LEN_FMT_ID;
	   pAPduResp->pduData[0x02u] = (uint8_t)((luw_MaxNumOfBlkLen & 0xFF00u) >> 0x08u);
	   pAPduResp->pduData[0x03u] = (uint8_t)((luw_MaxNumOfBlkLen & 0x00FFu)      );
   }

   return lub_response;
} /* end of UdsApp_35ReqUpld_ReqProc() */




void UDS_FN_TYPE UdsApp_35ReqUpld_InitCurrReqUpldData(void)
{
   UdsApp_36TransferDat_SetCurrTsfrDatSt(UDS_C_TANSFER_DAT_ST_INIT);
   rul_InputMemAdd2ReqUpld       = 0x00000000u;
   rul_InputMemSize2ReqUpld      = 0x00000000u;
}

uint32_t UDS_FN_TYPE UdsApp_35ReqUpld_GetCurrUpldStartAddr(void)
{
    return rul_InputMemAdd2ReqUpld;

}


uint32_t UDS_FN_TYPE UdsApp_35ReqUpld_GetCurrUpldMemSize(void)
{
    return rul_InputMemSize2ReqUpld;

}

/*
uint8_t UDS_FN_TYPE UdsApp_35ReqUpld_GetCurrUpldTblIdx(void)
{

  return 0x00u; //rub_ReqUpldTblIdx;
}
*/

uint8_t UdsApp_35ReqUpld_PreUpldCheckProc(uint32_t  i_InMemAdd2Rd,
                                          uint32_t  i_InMemSize2Upld,
                                          uint16_t *o_pMaxNumOfBlkLen)
{
   uint8_t lub_CheckResultOk = (uint8_t)FALSE;
   /* if possible , the checking for the uploading starting address and the size can be put here
    */
   i_InMemAdd2Rd = i_InMemAdd2Rd; /* just to remove compilation warning */
   i_InMemSize2Upld = i_InMemSize2Upld; /* just to remove compilation warning */

   /* step1 : to check the input memory address which will be used for uploading data
    */
   /* step2 : to check the input memory size    which will be used for uploading data
    */
   *o_pMaxNumOfBlkLen = UDS_C_35REQUPLD_USING_NUM_BLK_LEN;
   lub_CheckResultOk = (uint8_t)TRUE;

   return lub_CheckResultOk;
} /* end of UdsApp_35ReqUpld_PreUpldCheckProc() */

#endif /* UDS_ENABLE_REQUESTUPLOAD */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
