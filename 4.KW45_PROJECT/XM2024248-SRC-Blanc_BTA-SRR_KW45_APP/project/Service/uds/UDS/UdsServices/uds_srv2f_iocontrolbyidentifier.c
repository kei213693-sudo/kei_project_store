/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv2f_iocontrolbyidentifier.c
 * 摘要: UDS Application Layer service 0x2F InputOutputControlByIdentifier.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#include "Uds_IODIDCtl.h"

#ifndef ENMaskRecord
#define ENMaskRecord 0
#endif

#ifdef UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER

static Std_UdsReturnType UDS_FN_TYPE UdsApp_2fIoCtrlByDid_IoCtrlProc(Uds_APduData_t* pAPduInd,
                                                                  Uds_APduData_t* pAPduResp );

/* no Sun-Function parameter for this service */

/* NRC for ReadDataByIdentifier
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */

Std_UdsReturnType UDS_FN_TYPE UdsApp_IOControlByIdentifier(Uds_APduData_t* pAPduInd,
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
   else if ( 0x04u > pAPduInd->msgLength )
	 {
		   lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT ;
	 }
   else
   {
       lub_error = UdsApp_2fIoCtrlByDid_IoCtrlProc(pAPduInd,
                                                pAPduResp );
   }

   return lub_error;

}  /* end of Uds_IOControlByIdentifier()  */




static Std_UdsReturnType UDS_FN_TYPE UdsApp_2fIoCtrlByDid_IoCtrlProc(Uds_APduData_t* pAPduInd,
                                                                  Uds_APduData_t* pAPduResp )

{

   uint8_t  lub_response    = UDS_POSITIVE_RESPONSE;
   uint8_t  lub_indexID     = 0x00u;
   uint8_t  lub_dataIndex   = 0x00u;
   uint8_t  lub_outIndex    = 0x00u;
   uint8_t  lub_entryIndex  = 0x00u;
   uint8_t  lub_count       = 0x00u;
   uint8_t  lub_DummyIoCtrlOptByte = 0x00u;
   uint8_t* plub_IoCtrlOptBytesPtr = &lub_DummyIoCtrlOptByte;
   uint8_t  lub_CurrActiveSession = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
   uint16_t luw_tmpInDid2Rd = 0x0000u;
   uint16_t luw_IncomCtrlMskBitmap = 0x0000u;
   UdsApp_IODidCtrlCommand_t lub_IoCtrlByDidCmd = UDS_C_2FIOCTRL_CMD_NO;

   luw_tmpInDid2Rd  = (uint16_t)((pAPduInd->pduData[0x01u]) << 0x08u);
   luw_tmpInDid2Rd |= (uint16_t)( pAPduInd->pduData[0x02u]);
   lub_indexID = Uds_Generic_DatMgr_SearchTblByKey(luw_tmpInDid2Rd,
	                                               KeyTypeIsUINT16,
	                                               (uint8_t *)&(Uds_g_IoCtrlByDidIdxTable[0x00u].dataIdentifier),
									               sizeof(Uds_g_IoCtrlByDidIdxTable[0x00u]));



   if (lub_indexID  == UDS_C_IDX_NOT_IN_TBL )
   {
	     /* the id is not found in table! */
	     lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
       return lub_response ;
   }
   else
   {
   }


   if ((uint16_t)((Uds_g_IoCtrlByDidIdxTable[lub_indexID].AccessAttribute)  & (UDS_C_DID_ATR_ENA_IOCTL)) != UDS_C_DID_ATR_ENA_IOCTL)
   {
       lub_response = UDS_NRC_REQUESTOUTOFRANGE;
       return lub_response ;
   }
   else
   {
   }
   if ( ( (uint16_t)( (Uds_g_IoCtrlByDidIdxTable[lub_indexID].AccessAttribute) &
             UDS_C_DID_ATR_WT_SESN_EXTD
          ) == UDS_C_DID_ATR_WT_SESN_EXTD
        ) &&
        (lub_CurrActiveSession !=  UDS_EXTENDED_DIAGNOSTIC_SESSION) &&
        (lub_CurrActiveSession !=  UDS_SAFTEY_SYSTEM_DIAGNOSTIC_SESSION)
      )
   {
       lub_response = UDS_NRC_REQUESTOUTOFRANGE;
       return lub_response;
   }
   else
   {
   }

   if ( ( (uint16_t)( (Uds_g_IoCtrlByDidIdxTable[lub_indexID].AccessAttribute) &
             UDS_C_DID_ATR_WT_SESN_SSS
          ) == UDS_C_DID_ATR_WT_SESN_SSS
        ) &&
        (lub_CurrActiveSession !=  UDS_SAFTEY_SYSTEM_DIAGNOSTIC_SESSION)
      )
   {
       lub_response = UDS_NRC_REQUESTOUTOFRANGE;
       return lub_response;
   }
   else
   {

   }

   if ( ( ( (uint16_t)((Uds_g_IoCtrlByDidIdxTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_WT_SAL1) ==
            UDS_C_DID_ATR_WT_SAL1
          ) &&
          (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL1))
        ) ||
        ( ( (uint16_t)((Uds_g_IoCtrlByDidIdxTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_WT_SAL2) ==
            UDS_C_DID_ATR_WT_SAL2
          ) &&
          (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL2))
        ) ||
        ( ( (uint16_t)((Uds_g_IoCtrlByDidIdxTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_WT_SAL3) ==
            UDS_C_DID_ATR_WT_SAL3
          ) &&
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

    if ( pAPduInd->pduData[0x03u] != UDS_C_2FIOCTRL_PARAM_RETN_CTRL_2ECU &&
         pAPduInd->pduData[0x03u] != UDS_C_2FIOCTRL_PARAM_RESET_TO_DFLT  &&
         pAPduInd->pduData[0x03u] != UDS_C_2FIOCTRL_PARAM_FREEZE_CURR_ST &&
         pAPduInd->pduData[0x03u] != UDS_C_2FIOCTRL_PARAM_SHORT_TERM_ADJ
       )
    {
       lub_response = UDS_NRC_REQUESTOUTOFRANGE;
       return lub_response;
    }
    else
    {

    }

    if ( ( ( pAPduInd->pduData[0x03u] == UDS_C_2FIOCTRL_PARAM_RETN_CTRL_2ECU ||
             pAPduInd->pduData[0x03u] == UDS_C_2FIOCTRL_PARAM_RESET_TO_DFLT  ||
             pAPduInd->pduData[0x03u] == UDS_C_2FIOCTRL_PARAM_FREEZE_CURR_ST
           ) &&
    	     ( pAPduInd->msgLength != 0x04u)
    	   ) ||
         (  pAPduInd->pduData[0x03u] == UDS_C_2FIOCTRL_PARAM_SHORT_TERM_ADJ   &&
    	      pAPduInd->msgLength < 0x05u
    	   )
       )
    {
       lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
       return lub_response;
    }
    else
    {

    }

    /* to get the searching range of IoControl */
		lub_dataIndex = Uds_g_IoCtrlByDidIdxTable[lub_indexID].StartIndex ;
		lub_outIndex  = Uds_g_IoCtrlByDidIdxTable[lub_indexID].StartIndex +
								    Uds_g_IoCtrlByDidIdxTable[lub_indexID].NumOfEntry ;

		lub_count = 0x00u ;	/* to count if there is at least a function to call */
    luw_IncomCtrlMskBitmap  = (uint16_t)((pAPduInd->pduData[0x04u]) << 0x08u);
    luw_IncomCtrlMskBitmap |= (uint16_t)((pAPduInd->pduData[0x05u])         );


		for (lub_entryIndex = lub_dataIndex; lub_entryIndex < lub_outIndex; lub_entryIndex++)
		{
			 if (Uds_g_IoCtrlByDidMngtTable[lub_entryIndex].pFuncPtrToIoCtrl != NULL)
			 {
				   lub_count++;
#if ENMaskRecord
			 	   /* InputOutputControlParameter value dispatching */
			 	   switch(pAPduInd->pduData[0x03u])
			 	   {
			 	       default:
			 	            /* impossile to drop into this case */
			 	            break;
			 	       case UDS_C_2FIOCTRL_PARAM_RETN_CTRL_2ECU:
			 	            /* this case is just for IoCtrlParameter ReturnControlToEcu */
			 	            lub_IoCtrlByDidCmd = UDS_C_2FIOCTRL_CMD_REL_2_ECU;
			 	            break;

			 	       case UDS_C_2FIOCTRL_PARAM_RESET_TO_DFLT:
			 	            /* this case is just for IoCtrlParameter ResetToDefault */
			 	            lub_IoCtrlByDidCmd = UDS_C_2FIOCTRL_CMD_RST_2_DFLT;
			 	            break;

			 	       case UDS_C_2FIOCTRL_PARAM_FREEZE_CURR_ST:
			 	            /* this case is just for IoCtrlParameter FreezeCurrentState */
			 	            lub_IoCtrlByDidCmd = UDS_C_2FIOCTRL_CMD_FREZ_ST;
			 	            break;

			 	       case UDS_C_2FIOCTRL_PARAM_SHORT_TERM_ADJ:
			 	            /* this case is just for IoCtrlParameter ShortTermAdjustment */
			 	            plub_IoCtrlOptBytesPtr = &(pAPduInd->pduData[0x06u]);
			 	            if (luw_IncomCtrlMskBitmap & Uds_g_IoCtrlByDidMngtTable[lub_entryIndex].IoCtrlBitmapMask)
			 	            {
			 	                /* if the specified bit in the bitmap mask is 1 then to let Io on
			 	                 */
			 	                lub_IoCtrlByDidCmd = UDS_C_2FIOCTRL_CMD_ON;
			 	            }
			 	            else
			 	            {
			 	                /* if the specified bit in the bitmap mask is 0 then to let Io off
			 	                 */
			 	                lub_IoCtrlByDidCmd = UDS_C_2FIOCTRL_CMD_OFF;

			 	            }

			 	            break;

			 	   } /* end of switch(pAPduInd->pduData[0x03u])  */
#else
					lub_IoCtrlByDidCmd = pAPduInd->pduData[0x03u];
				   plub_IoCtrlOptBytesPtr = &(pAPduInd->pduData[0x04u]);
#endif

				   if ((Uds_g_IoCtrlByDidMngtTable[lub_entryIndex].pFuncPtrToIoCtrl) != NULL)
				   {

			 	       (*(Uds_g_IoCtrlByDidMngtTable[lub_entryIndex].pFuncPtrToIoCtrl))(lub_IoCtrlByDidCmd,
			 	                                                                        plub_IoCtrlOptBytesPtr);
				   }
				   else
				   {

				   }

			 }
             else
             {
                 /* this means the IoCtrl Operation for this single input or output is not supported
                  */
             }
		} /* end of 	for (lub_entryIndex = lub_dataIndex; lub_entryIndex < lub_outIndex; lub_entryIndex++) */

		if (lub_count == 0x00u)		/* IO control not supported for this DID */
		{
			lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
            return lub_response ;
		}
        else
        {
            pAPduResp->msgLength = 0x04u+Uds_g_IoCtrlByDidIdxTable[lub_indexID].DataSize;
        }


		return lub_response;

}  /* end of Uds_2fIoCtrlByDid_IoCtrlProc()  */



void UDS_FN_TYPE UdsApp_2fIoCtrlByDid_ReturnAllCtrl2ECU (void)
{
  uint8_t lub_dataIndex = 0x00u;
  uint8_t lub_DummyIoCtrlOptByte4Rel = 0x00u;

  for(lub_dataIndex = 0x00u; lub_dataIndex < UDS_2FIOCTRL_MNGT_RCD_MAX_NUM; lub_dataIndex ++)
  {
    if (Uds_g_IoCtrlByDidMngtTable[lub_dataIndex].pFuncPtrToIoCtrl != NULL)

    {
       (*(Uds_g_IoCtrlByDidMngtTable[lub_dataIndex].pFuncPtrToIoCtrl))(UDS_C_2FIOCTRL_CMD_REL_2_ECU,
                                                                       (&lub_DummyIoCtrlOptByte4Rel));
    }
    else
    {

    }
  }

}  /* end of Uds_2fIoCtrlByDid_ReturnAllCtrl2ECU() */

extern uint8_t rub_IODidDataBuf[8];
extern uint8_t rub_IODidDataOperBuf[UDS_2FIOCTRL_MNGT_RCD_MAX_NUM];
uint8_t g_OperData=0;
uint8_t UdsGet_IODidCtlP(uint16_t IoDid)
{
	uint8_t index;
	uint8_t OperData=0xff;

	for(index=0;index<UDS_2FIOCTRL_MNGT_RCD_MAX_NUM;index++)
	{
		if(IoDid == Uds_g_IoCtrlByDidIdxTable[index].dataIdentifier)
			{

				OperData= rub_IODidDataOperBuf[index];
				return OperData;
			}
	}
}

void UdsGet_IODidCtlStatus(uint16_t IoDid,uint8_t *IODidCtlStatusBuff)
{
	uint8_t index;
	uint8_t CtlIndex;
	uint8_t IODidDataBufNum=0;

	for(index=0;index<UDS_2FIOCTRL_MNGT_RCD_MAX_NUM;index++)
	{

		if(IoDid == Uds_g_IoCtrlByDidIdxTable[index].dataIdentifier)
			{
				for(CtlIndex=0;CtlIndex<=Uds_g_IoCtrlByDidIdxTable[index].DataSize;CtlIndex++)
					{
						IODidCtlStatusBuff[CtlIndex] = rub_IODidDataBuf[IODidDataBufNum+CtlIndex];

					}
				return ;
			}
		IODidDataBufNum=IODidDataBufNum+Uds_g_IoCtrlByDidIdxTable[index].DataSize;
	}

}




#endif /* UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
