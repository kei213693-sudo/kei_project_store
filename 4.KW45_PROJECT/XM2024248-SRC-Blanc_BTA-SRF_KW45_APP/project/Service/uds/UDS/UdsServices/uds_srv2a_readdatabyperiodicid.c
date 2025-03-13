/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv2a_readdatabyperiodicid.c
 * 摘要: UDS Application Layer service 0x2A ReadDataByPeriodicIdentifier.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */



#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_READDATABYPERIODICIDENTIFIER
static uint8_t Uds_g_CurrUsdPeriodDidNum   = 0x00u;
static Uds_APduData_t* p_2aRdDatByPeriodDidSvdAPduResp  = NULL;


static Std_UdsReturnType UDS_FN_TYPE UdsApp_2aRdDatByPeriodDidProc(Uds_APduData_t* pAPduInd,
                                                                   Uds_APduData_t* pAPduResp );


/* no Sun-Function parameter for this service */

/* NRC for ReadMemoryByAddress
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */

Std_UdsReturnType UDS_FN_TYPE UdsApp_ReadDataByPeriodicIdentifier(Uds_APduData_t* pAPduInd,
                                                               Uds_APduData_t* pAPduResp )
{
   uint8_t lub_error = UDS_POSITIVE_RESPONSE;
   /* 0x2A ReadDataByPeriodicalIdentifier is not allowed in
      functional adressing channel
    */
   if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
   {
       lub_error = UDS_NRC_SERVICENOTSUPPORTED;
   }
   else if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_DEFAULT_SESSION)
   {
       /* 0x2A ReadDataByPeriodicalIdentifier is not allowed running in
          default diagnostic session mode
        */

       lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
   }
   else if (pAPduInd->msgLength < 0x02u                         ||
            pAPduInd->msgLength > UDS_C_PERIODIC_DID_MAX_REQ_LEN )

   {   /* the minimum 0x2a request length is 2 bytes, max length
          is 8 bytes (maximum 6 Period DIDs) in a single reqest
        */
       lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
   }
   else
   {
       lub_error = UdsApp_2aRdDatByPeriodDidProc(pAPduInd,
                                                 pAPduResp );
   }



   return lub_error;
} /* end of UdsApp_ReadDataByPeriodicIdentifier() */


static Std_UdsReturnType UDS_FN_TYPE UdsApp_2aRdDatByPeriodDidProc(Uds_APduData_t* pAPduInd,
                                                                   Uds_APduData_t* pAPduResp )

{
	 uint8_t  lub_response        = UDS_POSITIVE_RESPONSE;
   uint8_t  lub_CurrActiveSession = UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION();
   uint8_t  lub_incomeReqTxMode   = 0x00u;
	 uint8_t  lub_indexID         = UDS_C_IDX_NOT_IN_TBL;
   uint8_t  lub_tmpInputDidNum    = 0x00u;
   uint8_t  lub_tmpInputDidIndex  = 0x00u;
   uint16_t luw_tmpInDid2Rd       = 0x0000u;
   uint8_t  lub_InputDidSupported = (uint8_t)FALSE;
   uint8_t  i                     = 0x00u;
   uint8_t  j                     = 0x00u;

   if (pAPduInd->pduData[0x01u] != UDS_C_PERIODIC_DID_TXMOD_SENDATSLOWRATE &&
       pAPduInd->pduData[0x01u] != UDS_C_PERIODIC_DID_TXMOD_SENDATMEDRATE &&
       pAPduInd->pduData[0x01u] != UDS_C_PERIODIC_DID_TXMOD_SENDATFSTRATE &&
       pAPduInd->pduData[0x01u] != UDS_C_PERIODIC_DID_TXMOD_STOPSENDING )
   {
	     lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
       return lub_response ;

   }
   else if ((pAPduInd->pduData[0x01u] == UDS_C_PERIODIC_DID_TXMOD_SENDATSLOWRATE ||
             pAPduInd->pduData[0x01u] == UDS_C_PERIODIC_DID_TXMOD_SENDATMEDRATE  ||
             pAPduInd->pduData[0x01u] == UDS_C_PERIODIC_DID_TXMOD_SENDATFSTRATE ) &&
            (pAPduInd->msgLength < 0x03u                                         ||
             pAPduInd->msgLength > UDS_C_PERIODIC_DID_MAX_REQ_LEN
            )
           )
   {
       lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
       return lub_response;
   }
   else if ((pAPduInd->pduData[0x01u] == UDS_C_PERIODIC_DID_TXMOD_SENDATSLOWRATE ||
             pAPduInd->pduData[0x01u] == UDS_C_PERIODIC_DID_TXMOD_SENDATMEDRATE  ||
             pAPduInd->pduData[0x01u] == UDS_C_PERIODIC_DID_TXMOD_SENDATFSTRATE ) &&
            ((pAPduInd->msgLength - 0x02u) >
               (UDS_C_PERIODIC_DID_MAX_NUM -  Uds_g_CurrUsdPeriodDidNum)
            )
           )
   {
       lub_response = UDS_NRC_REQUESTOUTOFRANGE;
       return lub_response;
   }

   /*
   else if (pAPduInd->pduData[0x01u] == UDS_C_PERIODIC_DID_TXMOD_STOPSENDING &&
            pAPduInd->msgLength != 0x02u
           )
   {
       lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
       return lub_response;
   }
   */
   else  /* begin of else if (pAPduInd->pduData[0x01u] == UDS_C_PERIODIC_DID_TXMOD_SENDATSLOWRATE .. */
   {
       lub_incomeReqTxMode = pAPduInd->pduData[0x01u];
       lub_tmpInputDidNum  = (uint8_t)(pAPduInd->msgLength - 0x02u);

       switch(lub_incomeReqTxMode)
       {
           default:
               break;

           case UDS_C_PERIODIC_DID_TXMOD_SENDATSLOWRATE:
           case UDS_C_PERIODIC_DID_TXMOD_SENDATMEDRATE:
           case UDS_C_PERIODIC_DID_TXMOD_SENDATFSTRATE:
                lub_tmpInputDidIndex = 0x00u;
                while (lub_tmpInputDidNum > 0x00u)
                {
                   luw_tmpInDid2Rd = (uint16_t)0xF200u | (uint16_t)(pAPduInd->pduData[lub_tmpInputDidIndex + 0x02u]);
	                 lub_indexID = Uds_Generic_DatMgr_SearchTblByKey(luw_tmpInDid2Rd ,
	                                                                 KeyTypeIsUINT16,
	                                                                 (uint8_t *)&(Uds_g_DidDataTable[0x00u].dataIdentifier),
             									                     sizeof(Uds_g_DidDataTable[0x00u]));

 	                 if (lub_indexID != UDS_C_IDX_NOT_IN_TBL)
                   {
                       if ( ( (Uds_g_DidDataTable[lub_indexID].AccessAttribute) &
                              UDS_C_DID_ATR_ENA_RD
                            ) != UDS_C_DID_ATR_ENA_RD
                          )
                       {
                           lub_response = UDS_NRC_REQUESTOUTOFRANGE;
                           /* return lub_response;  */
                       }
                       else
                       {
                       }

                       if ( ( ((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_EXTD) ==
                              UDS_C_DID_ATR_RD_SESN_EXTD
                            ) &&
                            (lub_CurrActiveSession !=  UDS_EXTENDED_DIAGNOSTIC_SESSION ) &&
                            (lub_CurrActiveSession !=  UDS_ECU_SUPPLIER_SPEC_SESSION )
                          )
                       {
                           lub_response = UDS_NRC_REQUESTOUTOFRANGE;
                           /* return lub_response;  */
                       }
                       else
                       {
                       }

                       if ( ( ((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SESN_SSS) ==
                              UDS_C_DID_ATR_RD_SESN_SSS
                            ) &&
                            (lub_CurrActiveSession !=  UDS_ECU_SUPPLIER_SPEC_SESSION )
                          )
                       {
                            lub_response = UDS_NRC_REQUESTOUTOFRANGE;
                            /* return lub_response; */
                       }
                       else
                       {
                       }


                       if (( ( ((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SAL1) ==
                               UDS_C_DID_ATR_RD_SAL1
                             ) &&
                             (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL1))
                           ) ||
                           ( ( ((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SAL2) ==
                               UDS_C_DID_ATR_RD_SAL2) &&
                             (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL2))
                           ) ||
                           ( ( ((Uds_g_DidDataTable[lub_indexID].AccessAttribute) & UDS_C_DID_ATR_RD_SAL3) ==
                               UDS_C_DID_ATR_RD_SAL3) &&
                             (!UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL3))
                           )
                          )
                       {
                           lub_response = UDS_NRC_SECUTITYACCESSDENIED;
                           /* return lub_response; */
                       }
                       else
                       {
                       }

                       if (lub_response == UDS_POSITIVE_RESPONSE)
                       {


                           for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)
                           {
                               if (Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex == 0xFFu)
                               {
                                   /* if an unsed periodDidMng record is found */
                                   lub_InputDidSupported = (uint8_t)TRUE;
                                   Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDid            = pAPduInd->pduData[lub_tmpInputDidIndex + 0x02u];
                                   Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex = lub_indexID;
                                   Uds_g_RdDatByPeriodDidMngTbl[i].IsDynaDef            = (uint8_t)FALSE;
                                   Uds_g_RdDatByPeriodDidMngTbl[i].PeriodRateType       = pAPduInd->pduData[0x01u];
                                   Uds_g_CurrUsdPeriodDidNum++;
                                   UDS_DISABLE_ALL_INTERRUPTS();
                                   Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer   = Uds_g_RdDatByPeriodDidMngTbl[i].StartingOffset;
                                   UDS_ENABLE_ALL_INTERRUPTS();
                               }
                               else
                               {
                                   /* current PeriodDidMng Record has already been used
                                      continue to search the next record.
                                    */
                                   continue;
                               }
                           }

                       } /*  end of if (lub_response == UDS_POSITIVE_RESPONSE)  */
                       else
                       {

                       } /* end of else of if (lub_response == UDS_POSITIVE_RESPONSE) */


                   } /* end of if (lub_indexID != UDS_C_IDX_NOT_IN_TBL) */
                   else /* if (lub_indexID == UDS_C_IDX_NOT_IN_TBL) */
                   {
                       /* still need to retrieve the dynamically defined
                          DID table
                        */
                       /*
                       lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
                       return lub_response;
                        */
                       luw_tmpInDid2Rd = (uint16_t)0xF300u | (uint16_t)(pAPduInd->pduData[lub_tmpInputDidIndex + 0x02u]);

                       for(j = 0x00; j < UDS_C_DYNA_DEF_DID_MAX_NUM;j++)
                       {
                           if (Uds_g_DynaDefDidMngTbl[j].DynaDefDid == luw_tmpInDid2Rd)
                           {
                               /* if the dynamically defined did is found in the dynamicaDefDidMngTable
                                */

                               for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)
                               {
                                   if (Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex == 0xFFu)
                                   {
                                       /* if an unsed periodDidMng record is found */
                                       lub_InputDidSupported = (uint8_t)TRUE;
                                       Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDid            = pAPduInd->pduData[lub_tmpInputDidIndex + 0x02u];
                                       Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex = j;
                                       Uds_g_RdDatByPeriodDidMngTbl[i].IsDynaDef            = (uint8_t)TRUE;
                                       Uds_g_RdDatByPeriodDidMngTbl[i].PeriodRateType       = pAPduInd->pduData[0x01u];
                                       Uds_g_CurrUsdPeriodDidNum++;
                                       UDS_DISABLE_ALL_INTERRUPTS();
                                       Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer   = Uds_g_RdDatByPeriodDidMngTbl[i].StartingOffset;
                                       UDS_ENABLE_ALL_INTERRUPTS();
                                   }
                                   else
                                   {
                                       /* current PeriodDidMng Record has already been used
                                          continue to search the next record.
                                        */
                                   }
                               }  /* end of for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)  */

                           } /* end of if (Uds_g_DynaDefDidMngTbl[j].DynaDefDid == luw_tmpInDid2Rd)  */
                           else
                           {

                           } /* end of else if (Uds_g_DynaDefDidMngTbl[j].DynaDefDid != luw_tmpInDid2Rd) */
                       } /* end of for(j = 0x00; j < UDS_C_DYNA_DEF_DID_MAX_NUM;j++) */

                   } /* end of else if (lub_indexID == UDS_C_IDX_NOT_IN_TBL) */
                   lub_tmpInputDidNum--;
                   lub_tmpInputDidIndex++;
                } /* end of while (lub_tmpInputDidNum > 0x00u)  */
                break;

           case UDS_C_PERIODIC_DID_TXMOD_STOPSENDING:
                if (pAPduInd->msgLength == 0x02u)
                {
                    /* this means to stop all of the periodical DID in the scheduler table
                     */
                    lub_InputDidSupported = (uint8_t)TRUE;

                    for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)
                    {
                         Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDid            = 0xFFu;
                         Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex = 0xFFu;
                         Uds_g_RdDatByPeriodDidMngTbl[i].IsDynaDef            = (uint8_t)FALSE;
                         Uds_g_RdDatByPeriodDidMngTbl[i].PeriodRateType       = UDS_C_PERIODIC_DID_TXMOD_STOPSENDING;
                         Uds_g_CurrUsdPeriodDidNum--;
                         UDS_DISABLE_ALL_INTERRUPTS();
                         Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer   = 0x00000000u;
                         UDS_ENABLE_ALL_INTERRUPTS();
                    } /* end of for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)  */

                }
                else  /* if (pAPduInd->msgLength > 0x02u) */
                {
                    lub_tmpInputDidIndex = 0x00u;
                    while (lub_tmpInputDidNum > 0x00u)
                    {
                           for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)
                           {
                                if (Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDid == pAPduInd->pduData[lub_tmpInputDidIndex + 0x02u])
                                {
                                    /* if an unsed periodDidMng record is found */
                                    lub_InputDidSupported = (uint8_t)TRUE;
                                    Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDid            = 0xFFu;
                                    Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex = 0xFFu;
                                    Uds_g_RdDatByPeriodDidMngTbl[i].IsDynaDef            = (uint8_t)FALSE;
                                    Uds_g_RdDatByPeriodDidMngTbl[i].PeriodRateType       = UDS_C_PERIODIC_DID_TXMOD_STOPSENDING;
                                    Uds_g_CurrUsdPeriodDidNum--;
                                    UDS_DISABLE_ALL_INTERRUPTS();
                                    Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer   = 0x00000000u;
                                    UDS_ENABLE_ALL_INTERRUPTS();
                                }
                                else
                                {
                                       /* current PeriodDidMng Record has already been used
                                          continue to search the next record.
                                        */
                                }
                           }  /* end of for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)  */
                           lub_tmpInputDidNum--;
                           lub_tmpInputDidIndex++;
                    } /* end of while (lub_tmpInputDidNum > 0x00u) */
                }  /* end of else if (pAPduInd->msgLength > 0x02u) */
                break;
       } /* end of switch(lub_incomeReqTxMode)  */
   } /* end of else if (pAPduInd->pduData[0x01u] == UDS_C_PERIODIC_DID_TXMOD_SENDATSLOWRATE .. */


   if(lub_InputDidSupported == (uint8_t)TRUE)
   {
      pAPduResp->msgLength = 0x01u;
      p_2aRdDatByPeriodDidSvdAPduResp = pAPduResp;

   }
   else
   {
      lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
   }

   return lub_response;
}   /* end of UdsApp_2aRdDatByPeriodDidProc() */


void UDS_FN_TYPE UdsApp_2aRdDatByPeriodDid_Init(void)
{
   uint8_t i = 0x00u;

   UDS_DISABLE_ALL_INTERRUPTS();
   Uds_g_CurrUsdPeriodDidNum = 0x00u;
   UDS_ENABLE_ALL_INTERRUPTS();

   for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)
   {
        Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDid            = 0xFFu;
        Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex = 0xFFu;
        Uds_g_RdDatByPeriodDidMngTbl[i].IsDynaDef            = (uint8_t)FALSE;
        Uds_g_RdDatByPeriodDidMngTbl[i].PeriodRateType       = UDS_C_PERIODIC_DID_TXMOD_STOPSENDING;
        UDS_DISABLE_ALL_INTERRUPTS();
        Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer   = 0x00000000u;
        UDS_ENABLE_ALL_INTERRUPTS();
   } /* end of for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)  */




} /* end of UdsApp_2aRdDatByPeriodDid_Init() */



void UDS_FN_TYPE UdsApp_2aRdDatByPeriodDid_CyclicTskEntryPoint(void)
{
   uint8_t  i = 0x00u;
   uint32_t lul_tmpTransModeRate  = 0x00000000;
   uint8_t  lub_DidTblIdx = 0x00;
	 uint8_t  lub_response          = UDS_POSITIVE_RESPONSE;

   for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)
   {

        if (Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex != 0xFFu &&
            Uds_g_RdDatByPeriodDidMngTbl[i].PeriodRateType       != UDS_C_PERIODIC_DID_TXMOD_STOPSENDING)
        {
            if (Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer > 0x00000000u)
            {
                UDS_DISABLE_ALL_INTERRUPTS();
                Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer--;
                UDS_ENABLE_ALL_INTERRUPTS();
            }
            else /* if (Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer == 0x00000000u) */
            {
                lub_DidTblIdx = Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex;
                if (Uds_g_RdDatByPeriodDidMngTbl[i].IsDynaDef == (uint8_t)FALSE)
                {
                    /* to read the statically configured DID table */
                    if (p_2aRdDatByPeriodDidSvdAPduResp != NULL)
                    {

                        p_2aRdDatByPeriodDidSvdAPduResp->msgLength = (uint16_t)(Uds_g_DidDataTable[lub_DidTblIdx].DataSize + 0x02u);

                        p_2aRdDatByPeriodDidSvdAPduResp->pduData[0x00u] = 0x2A; /* SID */
                        p_2aRdDatByPeriodDidSvdAPduResp->pduData[0x01u] = Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDid; /* Period DID */

                        /* clear the diagnostic buffer */
                        Uds_MemClear(&(p_2aRdDatByPeriodDidSvdAPduResp->pduData[0x02u]),
                                     Uds_g_DidDataTable[lub_DidTblIdx].DataSize);
                        if ((Uds_g_DidDataTable[lub_DidTblIdx].pFuncPtrToReadData) != NULL)
                        {

                            (*(Uds_g_DidDataTable[lub_DidTblIdx].pFuncPtrToReadData))(Uds_g_DidDataTable[lub_DidTblIdx].pDataBuf);
                        }
                        else
                        {

                        }

				                /* copy 1 or more bytes */
                        Uds_MemCopy(&(p_2aRdDatByPeriodDidSvdAPduResp->pduData[0x02u]),
				                            Uds_g_DidDataTable[lub_DidTblIdx].pDataBuf,
				                            Uds_g_DidDataTable[lub_DidTblIdx].DataSize);

				                Uds_generic_SendResp( p_2aRdDatByPeriodDidSvdAPduResp, lub_response);
                    }
                    else
                    {
                    }

                }
                else /* if (Uds_g_RdDatByPeriodDidMngTbl[i].IsDynaDef == (uint8_t)TRUE) */
                {
                    /* to read the dynamically configured DID table
                       TODO after 0x2C dynamically defined did service is ready
                     */






                } /* end of else if (Uds_g_RdDatByPeriodDidMngTbl[i].IsDynaDef == (uint8_t)TRUE) */


                /* get the transmissionModeRate */
                switch (Uds_g_RdDatByPeriodDidMngTbl[i].PeriodRateType)
                {
                   default :

                   case UDS_C_PERIODIC_DID_TXMOD_SENDATSLOWRATE:
                        lul_tmpTransModeRate = UDS_C_PERIODIC_DID_REL_SLOW_RATE;
                        break;
                   case UDS_C_PERIODIC_DID_TXMOD_SENDATMEDRATE:
                        lul_tmpTransModeRate = UDS_C_PERIODIC_DID_REL_MEDIUM_RATE;
                        break;
                   case UDS_C_PERIODIC_DID_TXMOD_SENDATFSTRATE:
                        lul_tmpTransModeRate = UDS_C_PERIODIC_DID_REL_FAST_RATE;
                        break;

                } /* end of switch (Uds_g_RdDatByPeriodDidMngTbl[i].PeriodRateType) */

                /* then restart this periodical timer
                 */
                UDS_DISABLE_ALL_INTERRUPTS();
                   Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer = lul_tmpTransModeRate;
                UDS_ENABLE_ALL_INTERRUPTS();

            } /* end of  else if (Uds_g_RdDatByPeriodDidMngTbl[i].RunningPeriodTimer == 0x0000u) */
        }
        else
        {
            /* this table record is not using currently , so do nothing
             */
        } /* end of else if (Uds_g_RdDatByPeriodDidMngTbl[i].PeriodDidTblRcdIndex == 0xFFu ... */
   } /* end of for (i = 0x00u; i< UDS_C_PERIODIC_DID_MAX_NUM; i++)  */




} /* end of UdsApp_2aRdDatByPeriodDid_CyclicTskEntryPoint() */






#endif /* UDS_ENABLE_READDATABYPERIODICIDENTIFIER */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
