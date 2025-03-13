/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv2c_dynamicdefineid.c
 * 摘要: UDS Application Layer service 0x2C DynamicallyDefineDataIdentifier.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_DYNAMICALLYDEFINEDATAIDENTIFIER

uint8_t  rub_DynaRecUsedNum    = 0x00u;
uint8_t  rub_DynaRecFreeIdx    = 0x00u;
uint16_t ruw_ReqDynaDefDid     = 0x0000u;
uint32_t rul_ReqDynaDefMemAdd  = 0x00000000u;
uint32_t rul_ReqDynaDefMemSize = 0x00000000u;

static Std_UdsReturnType UDS_FN_TYPE UdsApp_2CDynDefDid_DefByDid_ReqProc(Uds_APduData_t* pAPduInd,
                                                                         Uds_APduData_t* pAPduResp );

static Std_UdsReturnType UDS_FN_TYPE UdsApp_2CDynDefDid_DefByMemAdd_ReqProc(Uds_APduData_t* pAPduInd,
                                                                            Uds_APduData_t* pAPduResp );

static Std_UdsReturnType UDS_FN_TYPE UdsApp_2CDynDefDid_ClrDynDid_ReqProc(Uds_APduData_t* pAPduInd,
                                                                          Uds_APduData_t* pAPduResp );

/* no Sun-Function parameter for this service */

/* NRC for ReadMemoryByAddress
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */

Std_UdsReturnType UDS_FN_TYPE UdsApp_DynamicDefineDataIdentifier(Uds_APduData_t* pAPduInd,
                                                              Uds_APduData_t* pAPduResp )
{
   uint8_t lub_error = UDS_POSITIVE_RESPONSE;
   uint8_t lub_SubFunctionByte = ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7);

   if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
   {
       /* if this request is received from the functional addressing channel
          reject it.
        */
       lub_error = UDS_NRC_SERVICENOTSUPPORTED;
   }
   else if (pAPduInd->msgLength < 0x04u)
   {
       /* the 0x2C DynamicallyDefinedDataIdentifier valid request length is not less than 4  bytes
        */
       lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else if (pAPduInd->pduData[0x02u] != 0xF2u &&   /*The MSB of DynDid must be periodicDataIdentifier or */
            pAPduInd->pduData[0x02u] != 0xF3u)     /*The MSB of DynDid must be Dynamically defined DataIdentifier */
   {
 	   lub_error = UDS_NRC_REQUESTOUTOFRANGE;
   }
   else if (lub_SubFunctionByte  != UDS_C_DYNA_DEF_DID_SF_DEF_BY_DID     &&
            lub_SubFunctionByte  != UDS_C_DYNA_DEF_DID_SF_DEF_BY_MEM_ADD &&
            lub_SubFunctionByte  != UDS_C_DYNA_DEF_DID_SF_CLR_DYNA_DID )
   {
	   lub_error = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
   }
   else if (lub_SubFunctionByte == UDS_C_DYNA_DEF_DID_SF_DEF_BY_DID     &&
            ( pAPduInd->msgLength < 0x08u             ||
              (pAPduInd->msgLength % 0x04u) != 0x00u
            )
           )
   {
	   lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else if (lub_SubFunctionByte == UDS_C_DYNA_DEF_DID_SF_DEF_BY_MEM_ADD  &&
            ( pAPduInd->msgLength < 13u                                  ||
              ((pAPduInd->msgLength  - 0x05u) % 0x08u) != 0x00u           ||
              pAPduInd->pduData[0x04u] != UDS_C_DYNA_DEF_DID_ADD_LEN_FMT_ID
            )
           )
   {
	   lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else if (lub_SubFunctionByte == UDS_C_DYNA_DEF_DID_SF_CLR_DYNA_DID  &&
            pAPduInd->msgLength != 0x04u
           )
   {
	   lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else if( rub_DynaRecUsedNum == UDS_C_DYNA_DEF_DID_MAX_NUM)
   {
  	   lub_error = UDS_NRC_CONDITIONSNOTCORRECT;
   }
   else
   {
       /* the general conditions are passed, to call the 2C service processing interface
          to do more handling
        */
       ruw_ReqDynaDefDid  = (uint16_t)((pAPduInd->pduData[0x02u]) << 0x08u);
       ruw_ReqDynaDefDid |= (uint16_t)((pAPduInd->pduData[0x03u])         );

       switch (lub_SubFunctionByte)
       {
       	 default:
       	      break;
       	 case UDS_C_DYNA_DEF_DID_SF_DEF_BY_DID:
              lub_error = UdsApp_2CDynDefDid_DefByDid_ReqProc(pAPduInd,
                                                              pAPduResp );
       	      break;
         case UDS_C_DYNA_DEF_DID_SF_DEF_BY_MEM_ADD:
              lub_error = UdsApp_2CDynDefDid_DefByMemAdd_ReqProc(pAPduInd,
                                                                 pAPduResp );
       	      break;
          case UDS_C_DYNA_DEF_DID_SF_CLR_DYNA_DID:
              lub_error = UdsApp_2CDynDefDid_ClrDynDid_ReqProc(pAPduInd,
                                                               pAPduResp );
       	      break;
       }; /* end of switch (lub_SubFunctionByte) */
   }

   return lub_error;
} /* end of UdsApp_DynamicDefineDataIdentifier() */

static Std_UdsReturnType UDS_FN_TYPE UdsApp_2CDynDefDid_DefByDid_ReqProc(Uds_APduData_t* pAPduInd,
                                                                         Uds_APduData_t* pAPduResp )
{
   uint8_t  lub_response = UDS_POSITIVE_RESPONSE; /* init to positive response */
   uint16_t luw_ReqIdx   = 0x0000u;
   uint16_t luw_RefDid   = 0x0000u;
   uint8_t  lub_RefDidPosSrcDat = 0x00u;
   uint8_t  lub_RefDidMemSize   = 0x00u;
   uint8_t  i;
   uint8_t  lub_DynDidHasExisted = 0xFFu;
   uint8_t  lub_RefDidHasExisted = FALSE;

   for (luw_ReqIdx = 0x04u; luw_ReqIdx < (pAPduInd->msgLength); luw_ReqIdx += 0x04u)
   {
       luw_RefDid  = (uint16_t)((pAPduInd->pduData[luw_ReqIdx]) << 0x08u);
       luw_RefDid |= (uint16_t)((pAPduInd->pduData[luw_ReqIdx + 0x01u]) );

       lub_RefDidPosSrcDat = pAPduInd->pduData[luw_ReqIdx + 0x02u];
       lub_RefDidMemSize   = pAPduInd->pduData[luw_ReqIdx + 0x03u];

  	   lub_RefDidHasExisted = FALSE;

   	   for (i = 0x00u; i < UDS_C_DYNA_DEF_DID_MAX_NUM; i++)
   	   {
   	       /* if the input dynamic defined DID has already been registered in table
   	        */
   	   	   if (Uds_g_DynaDefDidMngTbl[i].DynaDefDid == ruw_ReqDynaDefDid)
   	   	   {
   	   	       lub_DynDidHasExisted = i;
   	   	       /* if the referenced DID has already been defined in the dynamic did table
   	   	          then this means the repeated referanced DID used, and
   	   	          return the NRC directly.
   	   	        */
   	   	   	   if (Uds_g_DynaDefDidMngTbl[i].ReferedDid == luw_RefDid)
   	   	   	   {
   	   	   	       lub_RefDidHasExisted = TRUE;
   	   	   	   	   lub_response = UDS_NRC_CONDITIONSNOTCORRECT;
   	   	   	   }
   	   	   	   else
   	   	   	   {
   	   	   	   }
   	   	   }
   	   	   else
   	   	   {
   	   	   }
   	   } /* end of for (i = 0x00u; i < UDS_C_DYNA_DEF_DID_MAX_NUM; i++) */

   	   if (lub_RefDidHasExisted == TRUE)
   	   {
   	       /* a repeated reference DID has been requested , this is a
   	          mistake , just reject the whole of request
   	        */
   	   	   break;
   	   }
   	   else
   	   {

   	   }

       lub_response = UdsApp_22RdDatByDid_ChkDidValidity(luw_RefDid);
   	   if (lub_response != UDS_POSITIVE_RESPONSE)
   	   {
   	   	   break;
   	   }
   	   else
   	   {
   	   }
   	   /*NOTE : we still not have check the validition for the referenced DID ,
   	            currently, we assume all of them are valid input value range
   	    */
       Uds_g_DynaDefDidMngTbl[rub_DynaRecFreeIdx].DynaDefDid            = ruw_ReqDynaDefDid;
       Uds_g_DynaDefDidMngTbl[rub_DynaRecFreeIdx].ReferedDid            = luw_RefDid;
       Uds_g_DynaDefDidMngTbl[rub_DynaRecFreeIdx].ReferedDidDatPosition = lub_RefDidPosSrcDat;
       Uds_g_DynaDefDidMngTbl[rub_DynaRecFreeIdx].ReferedDidDatSize     = lub_RefDidMemSize;

       Uds_g_DynaDefDidMngTbl[rub_DynaRecFreeIdx].NextDynaTblIdx        = 0xFFu;
       if (lub_DynDidHasExisted != 0xFFu)
       {
       	   Uds_g_DynaDefDidMngTbl[lub_DynDidHasExisted].NextDynaTblIdx  = rub_DynaRecFreeIdx;
       	   Uds_g_DynaDefDidMngTbl[rub_DynaRecFreeIdx].PriorDynaTblIdx   = lub_DynDidHasExisted;
       }
       else
       {
       }
       rub_DynaRecFreeIdx++;
       /* reset the index for the next reference DID
        */
       lub_DynDidHasExisted = 0xFFu;
   } /* end of for (luw_ReqIdx = 0x04u; luw_ReqIdx < (pAPduInd->msgLength); luw_ReqIdx++) */

   pAPduResp->msgLength = 0x04u;

   return lub_response;
} /* end of UdsApp_2CDynDefDid_DefByDid_ReqProc() */

static Std_UdsReturnType UDS_FN_TYPE UdsApp_2CDynDefDid_DefByMemAdd_ReqProc(Uds_APduData_t* pAPduInd,
                                                                            Uds_APduData_t* pAPduResp )
{
   uint8_t  lub_response          = UDS_POSITIVE_RESPONSE; /* init to positive response */

   rul_ReqDynaDefMemAdd  = (uint32_t)((pAPduInd->pduData[0x05u]) << 24u);
   rul_ReqDynaDefMemAdd |= (uint32_t)((pAPduInd->pduData[0x06u]) << 16u);
   rul_ReqDynaDefMemAdd |= (uint32_t)((pAPduInd->pduData[0x07u]) <<  8u);
   rul_ReqDynaDefMemAdd |= (uint32_t)((pAPduInd->pduData[0x08u])       );

   rul_ReqDynaDefMemAdd  = (uint32_t)((pAPduInd->pduData[0x09u]) << 24u);
   rul_ReqDynaDefMemAdd |= (uint32_t)((pAPduInd->pduData[0x0Au]) << 16u);
   rul_ReqDynaDefMemAdd |= (uint32_t)((pAPduInd->pduData[0x0Bu]) <<  8u);
   rul_ReqDynaDefMemAdd |= (uint32_t)((pAPduInd->pduData[0x0Cu])       );

   return lub_response;
} /* end of UdsApp_2CDynDefDid_DefByMemAdd_ReqProc() */

static Std_UdsReturnType UDS_FN_TYPE UdsApp_2CDynDefDid_ClrDynDid_ReqProc(Uds_APduData_t* pAPduInd,
                                                                          Uds_APduData_t* pAPduResp )
{
   uint8_t  lub_response          = UDS_POSITIVE_RESPONSE; /* init to positive response */

   return lub_response;
} /* end of UdsApp_2CDynDefDid_ClrDynDid_ReqProc() */

#endif /* UDS_ENABLE_DYNAMICALLYDEFINEDATAIDENTIFIER */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
