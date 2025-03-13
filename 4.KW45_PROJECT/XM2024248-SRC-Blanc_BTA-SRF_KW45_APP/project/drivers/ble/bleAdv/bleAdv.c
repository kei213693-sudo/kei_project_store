/*
 * bleAdv.c
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */


/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "bleAdv_cfg.h"

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
void App_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
*\fn          bleResult_t BluetoothLEHost_StartAdvertising(
*                 appAdvertisingParams_t   *pAdvParams,
                  gapAdvertisingCallback_t pfAdvertisingCallback,
                  gapConnectionCallback_t  pfConnectionCallback
              )
*\brief       Set advertising data, set advertising parameters and start advertising.
*
*\param  [in] pAdvParams             Pointer to the structure containing the
*                                    advertising parameters.
*\param  [in] pfAdvertisingCallback  Callback used by the application to receive
*                                    advertising events.
*                                    Can be NULL.
*\param  [in] pfConnectionCallback   Callback used by the application to receive
*                                    connection events.
*                                    Can be NULL.
*
*\return      bleResult_t            Result of the operation.
********************************************************************************** */
bleResult_t BluetoothLEHost_StartAdvertising(
    appAdvertisingParams_t *pAdvParams,
    gapAdvertisingCallback_t pfAdvertisingCallback,
    gapConnectionCallback_t pfConnectionCallback
)
{
    pfAdvCallback = pfAdvertisingCallback;
    pfConnCallback = pfConnectionCallback;

    pfAdvertiserHandler = App_AdvertiserHandler;
    mpAdvParams = pAdvParams;
    return Gap_SetAdvertisingParameters(pAdvParams->pGapAdvParams);
}

/*! *********************************************************************************
*\fn          bleResult_t BluetoothLEHost_StartExtAdvertising(
*                 appExtAdvertisingParams_t *pExtAdvParams,
                  gapAdvertisingCallback_t  pfAdvertisingCallback,
                  gapConnectionCallback_t   pfConnectionCallback
              )
*\brief       Set advertising data, set advertising parameters and start extended
*             advertising.
*
*\param  [in] pAdvParams             Pointer to the structure containing the
*                                    advertising parameters.
*\param  [in] pfAdvertisingCallback  Callback used by the application to receive
*                                    advertising events.
*                                    Can be NULL.
*\param  [in] pfConnectionCallback   Callback used by the application to receive
*                                    connection events.
*                                    Can be NULL.
*
*\return      bleResult_t            Result of the operation.
********************************************************************************** */
bleResult_t BluetoothLEHost_StartExtAdvertising(
    appExtAdvertisingParams_t *pExtAdvParams,
    gapAdvertisingCallback_t pfAdvertisingCallback,
    gapConnectionCallback_t pfConnectionCallback
)
{
    pfAdvCallback = pfAdvertisingCallback;
    pfConnCallback = pfConnectionCallback;

    pfAdvertiserHandler = App_AdvertiserHandler;
    mpExtAdvParams = pExtAdvParams;
    return Gap_SetExtAdvertisingParameters(pExtAdvParams->pGapExtAdvParams);
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
*\private
*\fn          void App_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
*\brief       Sends the GAP Advertising Event triggered by the Host Stack to the
*             application.
*
*\param  [in] pAdvertisingEvent      Pointer to the advertising event.
*
*\retval      void.
********************************************************************************** */
void App_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
    appMsgFromHost_t *pMsgIn = NULL;

    pMsgIn = MSG_Alloc(sizeof(uint32_t) + sizeof(gapAdvertisingEvent_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGapAdvertisementMsg_c;
    pMsgIn->msgData.advMsg.eventType = pAdvertisingEvent->eventType;
    pMsgIn->msgData.advMsg.eventData = pAdvertisingEvent->eventData;

    /* Put message in the Host Stack to App queue */
    (void)MSG_Queue(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}
