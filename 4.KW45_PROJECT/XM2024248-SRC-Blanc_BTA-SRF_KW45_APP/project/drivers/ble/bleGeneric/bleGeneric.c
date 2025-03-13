/*
 * bleGeneric.c
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#include "bleGeneric_cfg.h"

/*! *********************************************************************************
\fn            bleResult_t App_GenericCallback(gapGenericEvent_t* pGenericEvent)
*\brief        Callback used by the Host Stack to propagate GAP generic
*              events to the application.
*
*\param  [in]  pGenericEvent    Pointer to the GAP generic event.
*
*\retval       void.
********************************************************************************** */
void App_GenericCallback
(
    gapGenericEvent_t* pGenericEvent
)
{
    appMsgFromHost_t *pMsgIn = NULL;
    uint16_t size = 0;
    size = sizeof(gapGenericEvent_t);

    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(gapGenericEvent_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGapGenericMsg_c;
    FLib_MemCpy(&pMsgIn->msgData.genericMsg,
                pGenericEvent,
                sizeof(gapGenericEvent_t));

    /* Put message in the Host Stack to App queue */
    (void)MSG_QueueAddTail(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}
