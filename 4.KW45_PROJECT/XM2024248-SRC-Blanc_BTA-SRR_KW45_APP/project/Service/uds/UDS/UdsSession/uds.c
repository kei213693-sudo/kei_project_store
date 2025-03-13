/**
*
* \file
*
* \brief UDS Application Layer initialization and service processor.
*
* Copyright ETAS GmbH, Stuttgart.
*
* This file is covered by the licence and disclaimer document which is attached in
* the UDS software package.
*
* \version $Id: Version 1.0 Revision 5 2013-01-18 Yongqiang Deng $
*
*============================================================================*
*                               REVERSION HISTORY                            *
*============================================================================*
*  REVISION     |   DATE        |                         |      AUTHOR      *
*----------------------------------------------------------------------------*
* UDS_1.0 Rev4  | Dec/12/2012   |                         | Yang TAN         *
* Delivery_001                                                               *
*----------------------------------------------------------------------------*
* UDS_1.0 Rev5  | Jan/18/2013   |                         | Yongqiang Deng   *
* Delivery_002                                                               *
*         update the code after 1st round integration with customer running  *
*         sw project, change detail please refer to                          *
*         UDSonCAN Protocol Stack Release Note_v1_0r5.doc                    *
*                                                                            *
*----------------------------------------------------------------------------*
* UDS_1.2 Rev1  | Nov/07/2013   |                         | Yongqiang Deng   *
* Delivery_001                                                               *
*         update the code to process the pending and asynchronous request    *
*         and response                                                       *
*----------------------------------------------------------------------------*
* UDS_1.3 Rev1 | Apr/13/2013    |                         | Yongqiang Deng   *
* Delivery_006                                                               *
*  1) move the 0x14 pending interface to the unified pending function array  *
*----------------------------------------------------------------------------*
******************************************************************************/
#include "uds_target.h"
#include "uds_priv.h"
#include "uds_confpriv.h"
#include "uds_inf.h"
#include "uds_pub.h"
#include "uds_api.h"

static uint8_t  rub_Prev0x78PendingServiceId = 0x00u;

static uint8_t  rub_FblUds_AppFsmSt  = 0x00u;
static uint32_t rul_FblUds_AppFsmPendingTmr  = 0x00000000u;
static uint32_t rul_FblUds_AppFsmMaxPendingTmr = 0x00000000u;

static uint8_t  rub_SavedExitResponse = 0x00u;
static uint8_t  rub_ResultCanExitDpPend = FALSE;
static uint8_t  rub_RspSendCanExitDpPend = FALSE;


/* UDS A_PDU.Data unit for UDS <service>.indication to be exchanged with Uds services. */
extern Uds_APduData_t	Uds_APduDataInd[ UDS_SESSIONBUF_RX_NUM ];

/* UDS A_PDU.Data unit for UDS <service>.response to be exchanged with Uds services. */
extern Uds_APduData_t	Uds_APduDataResp[ UDS_SESSIONBUF_TX_NUM ];



Uds_APduData_t*  pul_FblUds_APduDataIndPtr  = &Uds_APduDataInd[0x00u];

Uds_APduData_t*  pul_FblUds_APduDataRespPtr = &Uds_APduDataResp[0x00u];



static const T_FBLUDS_FP_PERIODIC_PARAMVOID capf_FblUdsPeriodicItfTable[] =
{
   (T_FBLUDS_FP_PERIODIC_PARAMVOID)UDS_APP_ITF_INT_DTC_PEND_PERIODIC_CHK,
   (T_FBLUDS_FP_PERIODIC_PARAMVOID)UDS_APP_ITF_INT_RTN_PEND_PERIODIC_CHK,
   (T_FBLUDS_FP_PERIODIC_PARAMVOID)UDS_APP_ITF_INT_SECU_PEND_PERIODIC_CHK,
   (T_FBLUDS_FP_PERIODIC_PARAMVOID)UDS_APP_ITF_INT_WTDID_PEND_PERIODIC_CHK,
   (T_FBLUDS_FP_PERIODIC_PARAMVOID)UDS_APP_ITF_INT_RDDID_PEND_PERIODIC_CHK,
   (T_FBLUDS_FP_PERIODIC_PARAMVOID)UDS_APP_ITF_INT_IOCTL_PEND_PERIODIC_CHK,
   (T_FBLUDS_FP_PERIODIC_PARAMVOID)UDS_APP_ITF_INT_CMD_PEND_PERIODIC_CHK,
   (T_FBLUDS_FP_PERIODIC_PARAMVOID)UDS_APP_ITF_INT_TSFRDAT_PEND_PERIODIC_CHK
};



static void Uds_generic_if_Task (void);
static void UDS_FN_TYPE Uds_Service_Pending_StateManager(uint8_t lub_SevericNRC);
static void UDS_FN_TYPE UdsApp_PendingPeriod_OvertimeEvtHandler(void);


/**
 * This function initialises the UDS software package. It must be called exactly once.
 */
void UDS_FN_TYPE Uds_Initialize( void )
{

   rub_FblUds_AppFsmSt = UDS_APP_FSM_REGULAR_ST;/*睡眠唤醒后重置uds挂起状态*/
   UdsTp_Initialize();					/* Initialize UDS transport layer */
   Uds_SesnMgr_SessionInitialize();	/* Initialize UDS session layer */
   #ifdef UDS_ENABLE_CLEARDIAGNOSTICINFORMATION
       UdsApp_14ClrDiagInfor_Init();
   #endif /* end of #ifdef UDS_ENABLE_CLEARDIAGNOSTICINFORMATION */

   #ifdef UDS_ENABLE_SECURITYACCESS
       UdsApp_27SecuAcc_Init();
   #endif /* end of #ifdef UDS_ENABLE_SECURITYACCESS */

   #ifdef UDS_ENABLE_COMMUNICATIONCONTROL
       UdsApp_28ComCtrl_Init();
   #endif /* UDS_ENABLE_COMMUNICATIONCONTROL */

   #ifdef UDS_ENABLE_READDATABYPERIODICIDENTIFIER
       UdsApp_2aRdDatByPeriodDid_Init();
   #endif /* end of #ifdef UDS_ENABLE_READDATABYPERIODICIDENTIFIER */

   #ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER
       UdsApp_2eWtDatByDid_Init();
   #endif /* end of #ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER  */

   #ifdef UDS_ENABLE_CONTROLDTCSETTING
       UdsApp_85CtrlDtcSet_Init();
   #endif /* end of #ifdef UDS_ENABLE_CONTROLDTCSETTING */

   #ifdef UDS_ENABLE_LINKCONTROL
       UdsApp_87LnkCtrl_Init();
   #endif /* end of #ifdef UDS_ENABLE_LINKCONTROL */
   /* the initialization of the dtc manager module
    */
   //Uds_DtcMgr_Init();
   UDS_TIMER_CLEAR();


} /* end of Uds_Initialize() */


/* all of the UDS cyclic tasks which should be processed in 10ms period
   timebase shoud be put into this uds 40ms task main entrypoint.
*/
void Uds_MainTaskEntryPoint_CyclicTask(void)
{
   uint8_t lub_response = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING;
   uint8_t lub_FblUdsPendPeriodicTblNum = 0x00u;
   uint8_t lub_FblUdsPendPeriodicRecSize = 0x00u;

   #ifdef UDS_ENABLE_ECURESET
       UdsApp_11EcuRst_CyclicTskEntryPoint();
   #endif /* end of #ifdef UDS_ENABLE_ECURESET */

   //#ifdef UDS_ENABLE_CLEARDIAGNOSTICINFORMATION
       UdsApp_14ClrDiagInfor_Pending();
   //#endif /* end of #ifdef UDS_ENABLE_CLEARDIAGNOSTICINFORMATION */

   #ifdef UDS_ENABLE_SECURITYACCESS
       UdsApp_27SecuAcc_CyclicTskEntryPoint();
   #endif /* end of #ifdef UDS_ENABLE_SECURITYACCESS */

   #ifdef UDS_ENABLE_READDATABYPERIODICIDENTIFIER
       UdsApp_2aRdDatByPeriodDid_CyclicTskEntryPoint();
   #endif /* end of #ifdef UDS_ENABLE_READDATABYPERIODICIDENTIFIER */
		UdsApp_10Dsc_Programing_Session_Handler();
   /* to call the dtc asynchronous handling function in the cyclic task
    */
   //Uds_DtcMgr_HandleDtcAsyn();
   /* to call the dtc manager cyclic task main entrypoint */
#if 0
   Uds_DtcMgr_CyclicTaskMainEntryPoint();
#endif
   Uds_generic_if_Task();

   if (rub_FblUds_AppFsmSt == UDS_APP_FSM_PENDING_ST)
   {
   	   lub_FblUdsPendPeriodicRecSize = sizeof(capf_FblUdsPeriodicItfTable) /
		                                            sizeof (capf_FblUdsPeriodicItfTable[0x00u])	;
	/* search for all pending activities... */
       lub_FblUdsPendPeriodicTblNum = 0x00u;
	while ( lub_FblUdsPendPeriodicTblNum < lub_FblUdsPendPeriodicRecSize  )
	{
		if (   (capf_FblUdsPeriodicItfTable[lub_FblUdsPendPeriodicTblNum]) == NULL )
		{
		   /* nothing to do, function undefined */
		}
		else /* if (   (capf_FblUdsPeriodicItfTable[lub_FblUdsPendPeriodicTblNum]) != NULL ) */
		{
			lub_response = (capf_FblUdsPeriodicItfTable[lub_FblUdsPendPeriodicTblNum])() ;
			if (lub_response != UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING)
			{
                          /* the service routine has finished, this means , each time, only a single pending is allowed , single thread */
			     lub_FblUdsPendPeriodicTblNum = lub_FblUdsPendPeriodicRecSize ;

			}
			else
			{
			}
		} /* end of else if (   (capf_FblUdsPeriodicItfTable[lub_FblUdsPendPeriodicTblNum]) != NULL ) */
              lub_FblUdsPendPeriodicTblNum++;
	} /* end of while ( lub_FblUdsPendPeriodicTblNum < lub_FblUdsPendPeriodicRecSize  ) */
       Uds_Service_Pending_StateManager(lub_response);
   	}
   else
   {
   }

}

static void Uds_generic_if_Task (void)
{
  /*the pendig timers are used only in pnding sttate*/
   if(rub_FblUds_AppFsmSt == UDS_APP_FSM_PENDING_ST)
   {
      if(rul_FblUds_AppFsmPendingTmr < UDS_APP_FSM_TMR_PENDING_REL_TIME)
      {
         rul_FblUds_AppFsmPendingTmr ++;
      }
      if(rul_FblUds_AppFsmMaxPendingTmr < UDS_APP_FSM_TMR_MAX_PENDING_REL_TIME)
      {
         rul_FblUds_AppFsmMaxPendingTmr ++;
      }

   }
   else
   {
      /*reset pending timers (for robustness)*/
      rul_FblUds_AppFsmPendingTmr = 0x00000000u;
      rul_FblUds_AppFsmMaxPendingTmr = 0x00000000u;
   }


} /* end of Uds_generic_if_Task() */



/**
 * This function implement UDS services that do not implemented.
 */
/*lint -save  -e715 *//*lint -save  -e818 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_ServiceUnknown( Uds_APduData_t* pAPduInd, Uds_APduData_t* pAPduResp )
{
   pAPduInd = pAPduInd;  /* just to remove the compilation warning */
   Uds_generic_SendResp(pAPduResp, UDS_NRC_SERVICENOTSUPPORTED);

   return  UDS_NRC_SERVICENOTSUPPORTED;
}
/*lint -restore */

/**
 * This function will handling all of the service pending states.
 *
 * \param [in] lub_SevericNRC	Value of negative response code.
 *
 */
static void UDS_FN_TYPE Uds_Service_Pending_StateManager(uint8_t i_lubSeviceRspCode)
{
   uint8_t lub_SevericNRC = i_lubSeviceRspCode;

   switch (rub_FblUds_AppFsmSt)
   {
      default:
          lub_SevericNRC = UDS_NRC_GENERALREJECT;
  	      Uds_generic_SendResp( pul_FblUds_APduDataRespPtr, lub_SevericNRC);
          rub_FblUds_AppFsmSt = UDS_APP_FSM_REGULAR_ST;
          break;
      case UDS_APP_FSM_REGULAR_ST:

              /*check if a service needs to start the response pending mechanism*/
             if (lub_SevericNRC == UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING)
             {
                  rub_FblUds_AppFsmSt = UDS_APP_FSM_PENDING_ST;
                 rub_SavedExitResponse = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING;
                 rub_ResultCanExitDpPend = FALSE;
                 rub_RspSendCanExitDpPend = FALSE;
              }
	       else
	       {
	       }

              rul_FblUds_AppFsmPendingTmr = 0x00000000u;
              rul_FblUds_AppFsmMaxPendingTmr = 0x00000000u;
              break;
      case UDS_APP_FSM_PENDING_ST:

           if (rub_ResultCanExitDpPend == TRUE)
           {
               //if (rub_RspSendCanExitDpPend == TRUE )
               {
                   // if busy condition is false and
                   // rsp is sent successfully , to
                   /// return to DEFAUL state
                   rub_FblUds_AppFsmSt = UDS_APP_FSM_REGULAR_ST;
               }
               //else // if (rub_RspSendCanExitDpPend == FALSE )
               {
                   // to resend the response
                   // Uds_generic_SendResp( p_SavedInPduRspPtrForPending, rub_SavedExitResponse);
               } // end of else // if (rub_RspSendCanExitDpPend == TRUE )
           } // end of if (rub_ResultCanExitDpPend == TRUE)
           else // if (rub_ResultCanExitDpPend == FALSE)
           {
               if(lub_SevericNRC != UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING)
               {
                  /*the service has finished , so send response*/
		    /* dyq at 20130911 */
		     // pul_FblUds_APduDataRespPtr->	msgLength = 0x03u;

		          Uds_generic_SendResp( pul_FblUds_APduDataRespPtr, lub_SevericNRC);
                  rub_FblUds_AppFsmSt = UDS_APP_FSM_REGULAR_ST;
                  rub_ResultCanExitDpPend = TRUE;
                  rub_SavedExitResponse = lub_SevericNRC;
               }
               else //if(lub_SevericNRC == UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING)
               {
                  /*check if maxim response pendig time has ended*/
                  if(rul_FblUds_AppFsmMaxPendingTmr >= UDS_APP_FSM_TMR_MAX_PENDING_REL_TIME)
                  {
                     rub_FblUds_AppFsmSt = UDS_APP_FSM_REGULAR_ST;
                     rub_ResultCanExitDpPend = TRUE;
                     rub_SavedExitResponse = UDS_NRC_GENERALREJECT;
                     Uds_generic_SendResp( pul_FblUds_APduDataRespPtr, rub_SavedExitResponse);

                     /*notify the event of pending over timed to all of other services handler  */
                     UdsApp_PendingPeriod_OvertimeEvtHandler();
                  }
                  else //if(rul_FblUds_AppFsmMaxPendingTmr < UDS_APP_FSM_TMR_MAX_PENDING_REL_TIME)
                  {
                      /*check if is time for a new response pending message*/
                     if((rul_FblUds_AppFsmPendingTmr >= UDS_APP_FSM_TMR_PENDING_REL_TIME) ) //&&
                        // (rub_FblUds_AppFsmSt == UDS_APP_FSM_PENDING_ST))
                     {
                         rul_FblUds_AppFsmPendingTmr = 0x00000000u;
                         Uds_generic_SendResp( pul_FblUds_APduDataRespPtr, UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING);
                     }
                     else
                     {

                     }
                  } // end of else rul_FblUds_AppFsmMaxPendingTmr < UDS_APP_FSM_TMR_MAX_PENDING_REL_TIME
               } // end of elseif(lub_SevericNRC == UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING)
           } // end of else // if (rub_ResultCanExitDpPend == FALSE)
            // end of case DP_APPL_PENDING_STATE
           break;
   } // end of switch (rub_FblUds_AppFsmSt)
} /* end of Uds_Service_Pending_StateManager()  */

static void UDS_FN_TYPE UdsApp_PendingPeriod_OvertimeEvtHandler(void)
{
	/* inform all of the other needed services that
	    UDS Application Finite State Machine Pending Periodic timer is overtimed
	*/

   UDS_APP_ITF_INT_DTC_PEND_OVERTIME_HANDLER();
   UDS_APP_ITF_INT_RTN_PEND_OVERTIME_HANDLER();
   UDS_APP_ITF_INT_SECU_PEND_OVERTIME_HANDLER();
   UDS_APP_ITF_INT_WTDID_PEND_OVERTIME_HANDLER();
   UDS_APP_ITF_INT_RDDID_PEND_OVERTIME_HANDLER();
   UDS_APP_ITF_INT_IOCTL_PEND_OVERTIME_HANDLER();
   UDS_APP_ITF_INT_CMD_PEND_OVERTIME_HANDLER();

}

/**
 *
 *
 */
uint8_t UDS_FN_TYPE Uds_generic_Get_CurrSvcProcFSMState(void)
{
	/* TODO */
	return (uint8_t)UDS_SERVICE_RESP_AVAILABLE;
}


static UDS_FSM_PENDING_ST_t UDS_FN_TYPE Uds_generic_Get_CurrPendFSMState(void)
{
	/* TODO */
	return (UDS_FSM_PENDING_ST_t)rub_FblUds_AppFsmSt;
}

/**
 * This function check whether previous uds service still under processing.
 *
 * \return
 *  - TRUE:		This previous uds service pending.
 *	- FALSE:	This previous uds service finished processing.
 */
uint8_t UDS_FN_TYPE Uds_generic_IsAPrevReqAlreadyRunning(uint8_t i_lub_CurrIncomeReqSid)
{
   uint8_t                         lub_IsAPrevReqAlreadingRunning = FALSE;
   uint8_t				lub_IsRoutineRunning;
   UDS_FSM_PENDING_ST_t	lub_CurrSvcPendFsmState;

   lub_IsRoutineRunning = Uds_31RtnCtrl_Get_IsRoutineAlreadyRunning();
   lub_CurrSvcPendFsmState = Uds_generic_Get_CurrPendFSMState();
   if (i_lub_CurrIncomeReqSid != 0x31u)
   {
       if ( (lub_CurrSvcPendFsmState == UDS_APP_FSM_PENDING_ST) ||
             (lub_IsRoutineRunning == (uint8_t)TRUE) )
       {
             lub_IsAPrevReqAlreadingRunning =  (uint8_t)TRUE;
       }
       else
       {
             lub_IsAPrevReqAlreadingRunning = (uint8_t)FALSE;
       }
   }
   else // if (i_lub_CurrIncomeReqSid == 0x31u)
   {
        if ( (lub_CurrSvcPendFsmState == UDS_APP_FSM_PENDING_ST)  &&   /* if previous request has already enter pending state */
	      (pul_FblUds_APduDataIndPtr->CurrProcessingDiagReqSid != 0x31u))
        {
              lub_IsAPrevReqAlreadingRunning =  (uint8_t)TRUE;
        }
	 else
	 {
              lub_IsAPrevReqAlreadingRunning = (uint8_t)FALSE;

	 }

   } /* end of  else if (i_lub_CurrIncomeReqSid == 0x31u)  */
   return lub_IsAPrevReqAlreadingRunning;
}

/**
 * This function
 * - Copy received service data to response buffer.
 * - Update response code.
 * - Check availability of sub-functions and updata SupPosRspBit.
 *
 * \param [in] pAPduInd		The container of service indication data buffer.
 * \param [in] pAPduResp	The container of service response data buffer.
 *
 * \return
 *  - Service response code.
 */
Std_UdsReturnType UDS_FN_TYPE Uds_generic_RcvAndSaveDiagReqMsg(
		Uds_APduData_t* pAPduInd,
		Uds_APduData_t* pAPduResp
)
{
	Std_UdsReturnType lub_rc = UDS_POSITIVE_RESPONSE;

	/* Copy whole of the diag request data content into the response buffer. */
	Uds_MemCopy( &(pAPduResp->pduData[0x00u]), 		/* pDest */
				&(pAPduInd->pduData[0x00u]),  		/* pSrc */
				(uint32_t)(pAPduInd->msgLength) );    /* numBytes */

	/* Check whether have sub function or not according to the input service id. */
	switch ( pAPduInd->pduData[0x00u] )
	{
		default :
		   /* Drop in this case means service is not supported. */
		   /* In fact, before service is dispatched, non-supported services have already been found.*/
		   lub_rc = UDS_NRC_SERVICENOTSUPPORTED;
		   pAPduResp->CurrSvcHavSubFunc  = (uint8_t)FALSE;
		   break;

		/* totally, there are 12 services with sub functions */
		case 0x10u:  /* DiagnosticSessionControl        Service Id */
		case 0x11u:  /* EcuReset                        Service Id */
		case 0x19u:  /* ReadDtcInformation              Service Id */
		case 0x27u:  /* SecurityAccess                  Service Id */
		case 0x28u:  /* CommunicationControl            Service Id */
		case 0x2Cu:  /* DynamicallyDefineDataIdentifier Service Id */
		case 0x31u:  /* RoutineControl                  Service Id */
		case 0x3Eu:  /* TesterPresent                   Service Id */
		case 0x83u:  /* AccessTimingParameters          Service Id */
		case 0x85u:  /* ControlDtcSetting               Service Id */
		case 0x86u:  /* ResponseOnEvent                 Service Id */
		case 0x87u:  /* LinkControl                     Service Id */
		   pAPduResp->CurrSvcHavSubFunc  = (uint8_t)TRUE;
		   break;

		/* totally, there are 13 services without sub functions. */
		case 0x14u:  /* ClearDiagnosticInformation      Service Id */
		case 0x22u:  /* ReadDataByIdentifier            Service Id */
		case 0x23u:  /* ReadMemoryByAddress             Service Id */
		case 0x24u:  /* ReadScallingDataByIdentifier    Service Id */
		case 0x2Au:  /* ReadDataByPeriodicalIdenfifier  Service Id */
		case 0x2Eu:  /* WriteDataByIdentifier           Service Id */
		case 0x2Fu:  /* InputOutputControlByIdentifier  Service Id */
		case 0x34u:  /* RequestDownload                 Service Id */
		case 0x35u:  /* RequestUpload                   Service Id */
		case 0x36u:  /* TransferData                    Service Id */
		case 0x37u:  /* RequestTransferExit             Service Id */
		case 0x3Du:  /* WriteMemoryByAddress            Service Id */
		case 0x84u:  /* SecuredDataTransmission         Service Id */
		   pAPduResp->CurrSvcHavSubFunc  = (uint8_t)FALSE;
		   break;
	}

	if ( (pAPduResp->CurrSvcHavSubFunc == (uint8_t)TRUE) &&
		(pAPduInd->msgLength >= 0x02u) )
	{
		if ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_YES_SPRB7)
		{
			pAPduResp->CurrSvcIsSupPosRspBitSet = (uint8_t)TRUE;
		}
		else
		{
			pAPduResp->CurrSvcIsSupPosRspBitSet = (uint8_t)FALSE;
		}
	}
	else
	{
		pAPduResp->CurrSvcIsSupPosRspBitSet = (uint8_t)FALSE;
	}

	pAPduInd->CurrIncomDiagReqSid  = (pAPduInd->pduData[0x00u]);
	pAPduResp->CurrIncomDiagReqSid = (pAPduInd->pduData[0x00u]);

    pAPduInd->CurrProcessingDiagReqSid   = (pAPduInd->pduData[0x00u]);
	pAPduResp->CurrProcessingDiagReqSid = (pAPduInd->pduData[0x00u]);

	UDS_APP_ITF_IMP_SET_PRVNT_ECU_2_SLEEP();
	return lub_rc;
}

/**
 * This function check whether the current UDS service need asynchronous processing.
 *
 * \param [in] lub_SID		The service Id for current UDS service.
 *
 * \return
 *  - TRUE:		This current service need asynchronous process(result will be generate later).
 *	- FALSE:	This current service can be processed this time.
 */
static uint8_t UDS_FN_TYPE Uds_generic_chkPendingNeededForSvc( uint8_t lub_SID )
{
	uint8_t lub_tmpIsPendingNeeded = (uint8_t)FALSE;

	switch (lub_SID)
	{
	  default :
		break;
	  /* Below services need pending handling, more services may be added in the future  */
	  case 0x14u: /* ClearDiagnosticInformation */
	  case 0x22u: /* ReadDataByIdentifier (just remote asnychronized reading) */
	  case 0x2Eu: /* WriteDataByIdentifier  */
	  case 0x31u: /* RoutineControl */
		lub_tmpIsPendingNeeded = (uint8_t)TRUE;
		break;
	}

    return lub_tmpIsPendingNeeded;
}

/**
 * This function check service response type required to send and update serviceState.
 *
 * \param [in] pAPduResp	The container for the response data.
 * \param [in] lub_error	The response action according to current service processing.
 *
 */
void UDS_FN_TYPE Uds_generic_SendResp( Uds_APduData_t* pAPduResp, uint8_t lub_error )
{
	uint8_t	lub_IsAllowedToSendNegRsp = (uint8_t)FALSE;
	uint8_t	lub_IsAllowedToSendPosRsp = (uint8_t)FALSE;

	if ( Uds_generic_IsAPrevReqAlreadyRunning(pAPduResp->pduData[0x00u]) &&
	     pAPduResp->pduData[0x00u] == 0x3Eu)
	{
	     /* if there is already a service is running
	        then to ignore the 0x3E service , not to send back the response.
	      */
		 Uds_SesnMgr_UpdateServiceRespState(UDS_SERVICE_NO_RESPONSE);
	     return;					/* MISRA 2004 Rule 14.7 Return statement before end of the function *//*lint !e904*/
	}
	else
	{

	}

	if(lub_error != UDS_POSITIVE_RESPONSE)
	{
		/*make negative response*/
		if (pAPduResp->CurrSvcHavSubFunc == (uint8_t)TRUE)
		{
			/* current service have sub-function */
			if ( TARGETADDR_PHYSICAL == Uds_SesnMgr_GetReceivedTaType() )
			{/* Physical addressing */
				lub_IsAllowedToSendNegRsp = (uint8_t)TRUE;
			}
			else
			{/* Functional addressing */
				if ( (lub_error !=  UDS_NRC_SERVICENOTSUPPORTED)  	 					&&  /* NRC 0x11 SNS  */
					 (lub_error !=  UDS_NRC_SUBFUNCTIONNOTSUPPORTED) 					&&  /* NRC 0x12 SFNS */
					 (lub_error !=  UDS_NRC_REQUESTOUTOFRANGE)       					&&  /* NRC 0x31 ROOR */
					 (lub_error !=  UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION)       	&&	/* NRC 0x7f ROOR */
					 (lub_error !=  UDS_NRC_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION)     )	/* NRC 0x7E ROOR*/
				{
					lub_IsAllowedToSendNegRsp = (uint8_t)TRUE;
				}
			} /* end of if (pAPduResp->tpChannel == CANTP_PHYSICAL) */
		} /* end of if (pAPduResp->CurrSvcHavSubFunc == (uint8_t)TRUE) */
		else
		{/* pAPduResp->CurrSvcHavSubFunc == (uint8_t)FALSE */
			if ( TARGETADDR_PHYSICAL == Uds_SesnMgr_GetReceivedTaType() )
			{/* Physical addressing */
				lub_IsAllowedToSendNegRsp = (uint8_t)TRUE;
			}
			else
			{/* Functional addressing */
				if ( (lub_error !=  UDS_NRC_SERVICENOTSUPPORTED)  	 					&&  /* NRC 0x11 SNS  */
					 (lub_error !=  UDS_NRC_SUBFUNCTIONNOTSUPPORTED) 					&&  /* NRC 0x12 SFNS */
					 (lub_error !=  UDS_NRC_REQUESTOUTOFRANGE)       					&&  /* NRC 0x31 ROOR */
					 (lub_error !=  UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION)       	&&	/* NRC 0x7f ROOR */
					 (lub_error !=  UDS_NRC_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION)     )	/* NRC 0x7E ROOR*/
				{
					lub_IsAllowedToSendNegRsp = (uint8_t)TRUE;
				}
			} /* end of if(TARGETADDR_PHYSICAL == Uds_SesnMgr_GetReceivedTaType()) */
		} /* end of else of if(pAPduResp->CurrSvcHavSubFunc == (uint8_t)TRUE) */

		if (lub_IsAllowedToSendNegRsp == (uint8_t)TRUE)
		{

			pAPduResp->pduData[0x00u] = UDS_C_SVC_NEG_RSP_SID;
			if (lub_error == UDS_NRC_SERVICENOTSUPPORTED ||
			    lub_error == UDS_NRC_BUSYREPEATREQUEST     )
			{
			    pAPduResp->pduData[0x01u] = pAPduResp->CurrInTempDiagReqSid;
			}
			else
			{
			    pAPduResp->pduData[0x01u] = pAPduResp->CurrIncomDiagReqSid;

			}
			pAPduResp->pduData[0x02u] = lub_error;
			pAPduResp->msgLength      = 0x03u;

			/* notify the result to the session manager */
			if (lub_error == UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING)
			{
				/* if NRC is 0x78 then should tell session manager */
				Uds_SesnMgr_UpdateServiceRespState(UDS_SERVICE_78HEX_NEG_RESP);
				rub_Prev0x78PendingServiceId = pAPduResp->pduData[0x01u];
			}
			else
			{
				/* all other NRC is response available. */
				Uds_SesnMgr_UpdateServiceRespState(UDS_SERVICE_RESP_AVAILABLE);
			}
		}
		else
		{
			/* not allowed to send back negative response , just do nothing. */
			Uds_SesnMgr_UpdateServiceRespState(UDS_SERVICE_NO_RESPONSE);
		}
	} /* end of if(lub_error != UDS_POSITIVE_RESPONSE) */
	else
	{/* if(lub_error == UDS_POSITIVE_RESPONSE) */
		/*make positive response*/
		if (pAPduResp->CurrSvcHavSubFunc == (uint8_t)TRUE)
		{
			if (TARGETADDR_PHYSICAL == Uds_SesnMgr_GetReceivedTaType())
			{ /* Physical addressing. */
				if (pAPduResp->CurrSvcIsSupPosRspBitSet == (uint8_t)FALSE)
				{ /* SuppressPositiveResponseBit == 0 */
					if (pAPduResp->CurrSvcIsSupPosRspBitSet == TRUE)
					{
						 lub_IsAllowedToSendPosRsp = FALSE;
					}
					else
					{
						 lub_IsAllowedToSendPosRsp = (uint8_t)TRUE;
					}
				}
				else
				{ /* SuppressPositiveResponseBit == 1 */
					if (Uds_generic_chkPendingNeededForSvc(pAPduResp->CurrIncomDiagReqSid))
					{
						 lub_IsAllowedToSendPosRsp = (uint8_t)TRUE;
					}
					else
					{
						 /* Do not send back the positive response in this case */
					}
				}
			}
			else
			{/* Functional addressing */
				if (pAPduResp->CurrSvcIsSupPosRspBitSet == (uint8_t)FALSE)
				{/* SuppressPositiveResponseBit == 0 */
					lub_IsAllowedToSendPosRsp = (uint8_t)TRUE;
				}
				else /* if (pAPduResp->CurrSvcIsSupPosRspBitSet == (uint8_t)TRUE) */
				{/* SuppressPositiveResponseBit == 1 */
				   if (Uds_generic_chkPendingNeededForSvc(pAPduResp->CurrIncomDiagReqSid))
				   {
						lub_IsAllowedToSendPosRsp = (uint8_t)TRUE;
				   }
				   else
				   {
						/* Do not send back the positive response in this case */
				   }
				}
			} /* end of if(TARGETADDR_PHYSICAL == Uds_SesnMgr_GetReceivedTaType()) */
		}
		else
		{/* if (pAPduResp->CurrSvcHavSubFunc == (uint8_t)FALSE) */
			lub_IsAllowedToSendPosRsp = (uint8_t)TRUE;
		} /* end of if (pAPduResp->CurrSvcHavSubFunc == (uint8_t)TRUE) */

		if (lub_IsAllowedToSendPosRsp == (uint8_t)TRUE)
		{

 			pAPduResp->pduData[0x00u] = pAPduResp->CurrIncomDiagReqSid + UDS_C_SVC_POS_RSP_SID_OFFSET;
			Uds_SesnMgr_UpdateServiceRespState(UDS_SERVICE_RESP_AVAILABLE);
		}
		else
		{
			Uds_SesnMgr_UpdateServiceRespState(UDS_SERVICE_NO_RESPONSE);
		}
	} /* end of else if(lub_error == UDS_POSITIVE_RESPONSE) */

	return;
} /* end of Uds_generic_SendResp() */

void UDS_FN_TYPE Uds_generic_max78pendingTONumReachedEventProc(void)
{
       switch(rub_Prev0x78PendingServiceId)
       {
             default :
			      break;
	      case 0x14u:  /* 0x14 ClearDiagnosticInformation*/
		  	 UdsApp_14ClrDiagInfor_Pend_Overtime();
		  	 break;

             case 0x2eu:   /* 0x2e WriteDataByIdentifier */
			 UdsApp_2eWtDatByDid_Pend_Overtime();
			 break;
       }
	/* after this handling , the saved 0x78 pending service id can be cleared now
	  */
	rub_Prev0x78PendingServiceId = 0x00u;
}

/**
 * This function is the general entry point for a UDS service handler.
 * - It checks whether previous service finished.
 * - Perform service handling by call corresponding handler for each UDS service.
 * - Perform post-processing according to response code by calling Uds_generic_SendResp().
 *
 * \param [in] pAPduInd				The container of service indication data buffer.
 * \param [in] pAPduResp			The container of service response data buffer.
 * \param [in] pFnc_UdsSvcSpecProc	UDS service handler function pointer.
 *
 */
void UDS_FN_TYPE Uds_generic_ServiceCommonProcess(
		Uds_APduData_t* 			pAPduInd,
		Uds_APduData_t* 			pAPduResp,
		T_FUNC_PTR_UDS_SVC_HANDLER  pFnc_UdsSvcSpecProc
)
{
	Std_UdsReturnType lub_error = UDS_POSITIVE_RESPONSE;

	/* Call the reserved the indication interface to notify diag Req CAN msg is received. */
	UDS_APP_ITF_EXP_REQ_CAN_FRAME_IND;

	if ( Uds_generic_IsAPrevReqAlreadyRunning(pAPduInd->pduData[0x00u]) )
	{/* Previously received UDS service still under processing. */
		lub_error = UDS_NRC_BUSYREPEATREQUEST;
	}
	else
	{/* Previously received UDS service finished. */
		/* Transfer the request message from diagnostic protocol to applicaton buffer. */
		lub_error = Uds_generic_RcvAndSaveDiagReqMsg( pAPduInd, pAPduResp );
		if ( lub_error == UDS_POSITIVE_RESPONSE )
		{
			/* After the generic common checking is finished, call the each service special
			* process to handle the request. */
			lub_error = (*pFnc_UdsSvcSpecProc)(pAPduInd, pAPduResp);

			/* check if UDS service processing goes to pending state which means service processing
			* will be finished asynchronously, and final service response(if required)will be generated
			* when finished. */
			if ( lub_error == UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING )
			{
				Uds_Service_Pending_StateManager( UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING );
			}
			else
			{
			}
		}
		else
		{

		}
	} /* end of else if (!Uds_generic_IsAPrevReqAlreadyRunning(pAPduInd->pduData[0x00u])) */

	Uds_generic_SendResp(pAPduResp, lub_error);

	return;
} /* end of  Uds_generic_ServiceCommonProcess() */

/**
 * This function is the general entry point for UDS service processing.
 * - It looks up the service table according to received Sid and start service
 *   processing.
 *
 * \param [in] pRxPacket	The container for received service data.
 * \param [in] pTxPacket	The container for transmit data.
 *
 */
void UDS_FN_TYPE Uds_ServiceProc(
		const Uds_SessionRxBuf_t* pRxPacket,
		const Uds_SessionTxBuf_t* pTxPacket
)
{
	uint8_t 					i;
	uint8_t 					lub_RcvdReqSid;
	uint8_t 					lub_uds_service_found;

	/* Start processing of UDS service requests */
	lub_uds_service_found = 0xFFu;	/* Init to invalid index. */
	lub_RcvdReqSid = pRxPacket->pAPduData->pduData[0x00u];
    pRxPacket->pAPduData->CurrInTempDiagReqSid = lub_RcvdReqSid;
    pTxPacket->pAPduData->CurrInTempDiagReqSid = lub_RcvdReqSid;

	for ( i = 0x00u; i < UDS_NUM_OF_SERVICES; i++ )
	{
		if ( Uds_ServiceTable[i].serviceId == lub_RcvdReqSid )
		{
			lub_uds_service_found = i;
			break;
		}
	} /* end of for (i = 0x00u; i < UDS_NUM_OF_SERVICES; i++) */

	if ( lub_uds_service_found != 0xFFu )
	{
		/* Call the diag service handler to process service. */
		Uds_generic_ServiceCommonProcess( pRxPacket->pAPduData,
										  pTxPacket->pAPduData,
										  (Uds_ServiceTable[lub_uds_service_found].pFnc_UdsSvcHdlr) );
	}else
	{
		/* Service not found, should return the NRC: 0x11 ServiceNotSupported. */
		Uds_generic_SendResp(pTxPacket->pAPduData, UDS_NRC_SERVICENOTSUPPORTED);
	}

	return;
}

/**
 * This function shall be called periodicly with rate of UDS_DIAGDATA_INTERP_RATE.
 *
 * It schedules:
 * - UDS transport layer processor.
 * - UDS session layer manager which will call corresponding UDS service handler when
 *   available services indication received.
 */
extern uint8_t task_10ms_cnt;
void UDS_FN_TYPE Uds_Processor( void )
{
	/* Start transport layer processing */
	//UdsTp_ServiceProc();

	/* Start Session layer processing */
	Uds_SesnMgr_SessionManager();
	// the most of Uds Cyclic handling is put in 10ms task slot
	//Uds_MainTaskEntryPoint_CyclicTask();
//if(task_10ms_cnt >= 10)
//{
//	// the most of Uds Cyclic handling is put in 10ms task slot
//    Uds_MainTaskEntryPoint_CyclicTask();
//	task_10ms_cnt = 0;
//}


	/* Start transport layer processing */
	UdsTp_ServiceProc();
}  /* end of void UDS_FN_TYPE Uds_Processor( void ) */

void Uds_Get_2ndIndRespBufPtrSet(Uds_APduData_t* o_Ind2ndPtr,
                                                                           Uds_APduData_t* o_Resp2ndPtr)
{
   o_Ind2ndPtr  =  pul_FblUds_APduDataIndPtr;
   o_Resp2ndPtr =   pul_FblUds_APduDataRespPtr;
}





/******************************************************************************
*                               end of file                                   *
******************************************************************************/
