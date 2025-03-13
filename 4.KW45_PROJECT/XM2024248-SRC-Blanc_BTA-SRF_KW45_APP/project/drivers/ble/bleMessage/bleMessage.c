/*
 * bleMessage.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "signals_api.h"
#include "fsl_component_messaging.h"


/*! *********************************************************************************
\fn            bleResult_t App_PostCallbackMessage(
*                  appCallbackHandler_t   handler,
*                  appCallbackParam_t     param
               )
*\brief        Store a callback message in the Cb App queue and signal application.
*
*\param  [in]  handler              Callback handler.
*\param  [in]  param                Callback parameter.
*
*\retval       gBleOutOfMemory_c    Message allocation fail.
*\retval       gBleSuccess_c        Successful addition to the Cb App queue.
********************************************************************************** */
bleResult_t App_PostCallbackMessage
(
    appCallbackHandler_t   handler,
    appCallbackParam_t     param
)
{
    appMsgCallback_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store the packet */
    pMsgIn = MSG_Alloc(sizeof (appMsgCallback_t));

    if (pMsgIn == NULL)
    {
        return gBleOutOfMemory_c;
    }

    pMsgIn->handler = handler;
    pMsgIn->param = param;

    /* Put message in the Cb App queue */
    (void)MSG_QueueAddTail(&mAppCbInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtAppCallback_c);

    return gBleSuccess_c;
}

/*! *********************************************************************************
\fn            bleResult_t App_PostCallbackMessage(
*                  appCallbackHandler_t   handler,
*                  appCallbackParam_t     param
               )
*\brief        Store a callback message in the Cb App queue and signal application.
*
*\param  [in]  handler              Callback handler.
*\param  [in]  param                Callback parameter.
*
*\retval       gBleOutOfMemory_c    Message allocation fail.
*\retval       gBleSuccess_c        Successful addition to the Cb App queue.
********************************************************************************** */
bleResult_t BleApp_EventCallback
(
    appCallbackHandler_t   handler,
    appCallbackParam_t     param
)
{
    appMsgCallback_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store the packet */
    pMsgIn = MSG_Alloc(sizeof (appMsgCallback_t));

    if (pMsgIn == NULL)
    {
        return gBleOutOfMemory_c;
    }

    pMsgIn->handler = handler;
    pMsgIn->param = param;

    /* Put message in the Cb App queue */
    (void)MSG_QueueAddTail(&mAppEventQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mBleAppEvent, gAppEvtService_c);

    return gBleSuccess_c;
}

/*! *********************************************************************************
\fn            bleResult_t App_PostCallbackMessage(
*                  appCallbackHandler_t   handler,
*                  appCallbackParam_t     param
               )
*\brief        Store a callback message in the Cb App queue and signal application.
*
*\param  [in]  handler              Callback handler.
*\param  [in]  param                Callback parameter.
*
*\retval       gBleOutOfMemory_c    Message allocation fail.
*\retval       gBleSuccess_c        Successful addition to the Cb App queue.
********************************************************************************** */
bleResult_t BleApp_MessageCallback
(
    appCallbackHandler_t   handler,
    appCallbackParam_t     param
)
{
    appMsgCallback_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store the packet */
    pMsgIn = MSG_Alloc(sizeof (appMsgCallback_t));

    if (pMsgIn == NULL)
    {
        return gBleOutOfMemory_c;
    }

    pMsgIn->handler = handler;
    pMsgIn->param = param;

    /* Put message in the Cb App queue */
    (void)MSG_QueueAddTail(&mAppMessageQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mBleAppEvent, gAppEvtService_c);

    return gBleSuccess_c;
}
