/**
*
* \file
*
* \brief UDS Application Layer service 0x86 ResponseOnEvent.
*
* Copyright ETAS GmbH, Stuttgart.
*
* This file is covered by the licence and disclaimer document which is attached in
* the UDS software package.
*
* \version $Id: Version 1.0 Revision 0 2012-07-05 Deng Yongqiang $
*
*============================================================================*
*                               REVERSION HISTORY                            *
*============================================================================*
*  REVISION     |   DATE        |                         |      AUTHOR      *
*----------------------------------------------------------------------------*
*  UDS_1.0      | June/27/2012  |                         | Yongqiang Deng   *
* Creation of this file                                                      *
*----------------------------------------------------------------------------*
*  UDS_1.1      | Mar/03/2013   |                         | Yongqiang Deng   *
* remove the compiling warning                                               *
*----------------------------------------------------------------------------*
******************************************************************************/

#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"

#ifdef UDS_ENABLE_RESPONSEONEVENT

/* Sun-Function parameter for ResponseOnEvent(ISO15765-3 limited)
 *
 * eventStorageState
   [Note]:	Only bit0 represent bit6 of eventType in received service.
	0x00	UDS_EVENTSTORESTATE_NOTSTORE
	0x01	UDS_EVENTSTORESTATE_STORE
 */

/* Sun-Function parameter for ResponseOnEvent(ISO15765-3 limited)
 *
 * eventType
	0x00	UDS_EVENTTYPE_STOP_RESPONEVENT
	0x01	UDS_EVENTTYPE_ONDTCSTATUSCHANGE
	0x02	UDS_EVENTTYPE_ONTIMERINTERRUPT
	0x03	UDS_EVENTTYPE_ONDIDCHANGE
	0x04	UDS_EVENTTYPE_REPORT_ACTIVEEVENT
	0x05	UDS_EVENTTYPE_START_RESPONEVENT
	0x06	UDS_EVENTTYPE_CLEAR_RESPONEVENT
	0x07	UDS_EVENTTYPE_ONCMPOFVALUES
 */

/* NRC for ResponseOnEvent
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 */

Std_UdsReturnType UDS_FN_TYPE UdsApp_ResponseOnEvent(Uds_APduData_t* pAPduInd,
                                                  Uds_APduData_t* pAPduResp )
{
   pAPduInd = pAPduInd; /* just to remove compilation warning */

   Uds_generic_SendResp(pAPduResp, UDS_NRC_SERVICENOTSUPPORTED);

   return  UDS_NRC_SERVICENOTSUPPORTED;
}

#endif /* UDS_ENABLE_RESPONSEONEVENT */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
