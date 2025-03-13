/*
 * bleGatt.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */
#include "bleGatt_cfg.h"

/*! *********************************************************************************
*\private
*\fn           void App_GattServerCallback(deviceId_t          peerDeviceId,
                                           gattServerEvent_t*  pServerEvent)
*\brief        Callback used by the Host Stack to propagate Gatt Server events
*              to the application.
*
*\param  [in]  peerDeviceId    Peer device id.
*\param  [in]  pServerEvent    Pointer to the Gatt Server event.
*
*\retval       void.
********************************************************************************** */
void App_GattServerCallback
(
    deviceId_t          peerDeviceId,
    gattServerEvent_t*  pServerEvent
)
{
    appMsgFromHost_t *pMsgIn = NULL;
    uint32_t msgLen = (uint32_t)&(pMsgIn->msgData) + sizeof(gattServerMsg_t);

    if (pServerEvent->eventType == gEvtAttributeWritten_c ||
        pServerEvent->eventType == gEvtAttributeWrittenWithoutResponse_c)
    {
        msgLen += pServerEvent->eventData.attributeWrittenEvent.cValueLength;
    }

    pMsgIn = MSG_Alloc(msgLen);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGattServerMsg_c;
    pMsgIn->msgData.gattServerMsg.deviceId = peerDeviceId;
    FLib_MemCpy(&pMsgIn->msgData.gattServerMsg.serverEvent,
                pServerEvent,
                sizeof(gattServerEvent_t));

    if ((pMsgIn->msgData.gattServerMsg.serverEvent.eventType ==
            gEvtAttributeWritten_c) ||
        (pMsgIn->msgData.gattServerMsg.serverEvent.eventType ==
            gEvtAttributeWrittenWithoutResponse_c))
    {
        /* Copy value after the gattServerEvent_t structure and update the aValue pointer*/
        pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue =
          (uint8_t *)&pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue +
          sizeof(uint8_t*) + sizeof(bearerId_t);
        pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.bearerId =
            pServerEvent->eventData.attributeWrittenEvent.bearerId;
        FLib_MemCpy(pMsgIn->msgData.gattServerMsg.serverEvent.eventData.attributeWrittenEvent.aValue,
                    pServerEvent->eventData.attributeWrittenEvent.aValue,
                    pServerEvent->eventData.attributeWrittenEvent.cValueLength);

    }

    /* Put message in the Host Stack to App queue */
    (void)MSG_QueueAddTail(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}

/*! *********************************************************************************
*\private
*\fn           void App_GattClientProcedureCallback(
*                  deviceId_t            deviceId,
*                  gattProcedureType_t   procedureType,
*                  gattProcedureResult_t procedureResult,
*                  bleResult_t           error
*              )
*\brief        Callback used by the Host Stack to propagate Gatt Client procedure
*              events to the application.
*
*\param  [in]  deviceId           Peer device id.
*\param  [in]  procedureType      Procedure type.
*\param  [in]  procedureResult    Procedure result.
*\param  [in]  error              Error result.
*
*\retval       void.
********************************************************************************** */
void App_GattClientProcedureCallback
(
    deviceId_t              deviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error)
{
    appMsgFromHost_t *pMsgIn = NULL;

    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(gattClientProcMsg_t));

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGattClientProcedureMsg_c;
    pMsgIn->msgData.gattClientProcMsg.deviceId = deviceId;
    pMsgIn->msgData.gattClientProcMsg.procedureType = procedureType;
    pMsgIn->msgData.gattClientProcMsg.error = error;
    pMsgIn->msgData.gattClientProcMsg.procedureResult = procedureResult;

    /* Put message in the Host Stack to App queue */
    (void)MSG_QueueAddTail(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}

/*! *********************************************************************************
*\private
*\fn           void App_GattClientNotificationCallback(
*                  deviceId_t deviceId,
*                  uint16_t   characteristicValueHandle,
*                  uint8_t*   aValue,
*                  uint16_t   valueLength
*              )
*\brief        Callback used by the Host Stack to propagate Gatt Client notifications
*              to the application.
*
*\param  [in]  deviceId                     Peer device id.
*\param  [in]  characteristicValueHandle    Handle of the notified Gatt characteristic
*                                           value.
*\param  [in]  aValue                       Pointer to the characteristic value.
*\param  [in]  valueLength                  Value length.
*
*\retval       void.
********************************************************************************** */
void App_GattClientNotificationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
)
{
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store also the notified value*/
    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) +
                       sizeof(gattClientNotifIndMsg_t) +
                       (uint32_t)valueLength);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGattClientNotificationMsg_c;
    pMsgIn->msgData.gattClientNotifIndMsg.deviceId = deviceId;
    pMsgIn->msgData.gattClientNotifIndMsg.characteristicValueHandle =
                                characteristicValueHandle;
    pMsgIn->msgData.gattClientNotifIndMsg.valueLength = valueLength;

    /*
     * Copy value after the gattClientNotifIndMsg_t structure and update
     * the aValue pointer
     */
    pMsgIn->msgData.gattClientNotifIndMsg.aValue =
                                (uint8_t*)&pMsgIn->msgData +
                                sizeof(gattClientNotifIndMsg_t);
    FLib_MemCpy(pMsgIn->msgData.gattClientNotifIndMsg.aValue,
                aValue,
                valueLength);

    /* Put message in the Host Stack to App queue */
    (void)MSG_QueueAddTail(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}

/*! *********************************************************************************
*\private
*\fn           void App_GattClientIndicationCallback(
*                  deviceId_t deviceId,
*                  uint16_t   characteristicValueHandle,
*                  uint8_t*   aValue,
*                  uint16_t   valueLength
*              )
*\brief        Callback used by the Host Stack to propagate Gatt Client indications
*              to the application.
*
*\param  [in]  deviceId                    Peer device id.
*\param  [in]  characteristicValueHandle   Handle of the indicated Gatt characteristic
*                                          value.
*\param  [in]  aValue                      Pointer to the characteristic value.
*\param  [in]  valueLength                 Value length.
*
*\retval       void.
********************************************************************************** */
void App_GattClientIndicationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
)
{
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store also the notified value*/
    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) +
                       sizeof(gattClientNotifIndMsg_t) +
                       (uint32_t)valueLength);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppGattClientIndicationMsg_c;
    pMsgIn->msgData.gattClientNotifIndMsg.deviceId = deviceId;
    pMsgIn->msgData.gattClientNotifIndMsg.characteristicValueHandle =
                characteristicValueHandle;
    pMsgIn->msgData.gattClientNotifIndMsg.valueLength = valueLength;

    /*
     * Copy value after the gattClientIndIndMsg_t structure and update
     * the aValue pointer
     */
    pMsgIn->msgData.gattClientNotifIndMsg.aValue =
                (uint8_t*)&pMsgIn->msgData +
                sizeof(gattClientNotifIndMsg_t);
    FLib_MemCpy(pMsgIn->msgData.gattClientNotifIndMsg.aValue,
                aValue,
                valueLength);

    /* Put message in the Host Stack to App queue */
    (void)MSG_QueueAddTail(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}

/*! *********************************************************************************
\fn            bleResult_t App_RegisterGattServerCallback(
*                  gattServerCallback_t serverCallback)
*\brief        Set application gatt server callback and install static callback for
*              the GATT Server module.
*
*\param  [in]  serverCallback    Callback used by the application to receive gatt
*                                server events.
*
*\return       bleResult_t        Result of the operation.
********************************************************************************** */
bleResult_t App_RegisterGattServerCallback
(
    gattServerCallback_t serverCallback
)
{
    pfGattServerCallback = serverCallback;

    return GattServer_RegisterCallback(App_GattServerCallback);
}

/*! *********************************************************************************
\fn            bleResult_t App_RegisterGattClientProcedureCallback(
*                  gattClientProcedureCallback_t  callback)
*\brief        Set application gatt client callback and install static callback for
*              the GATT Client module procedures.
*
*\param  [in]  callback       Callback used by the application to receive gatt
*                             client events.
*
*\return       bleResult_t    Result of the operation.
********************************************************************************** */
bleResult_t App_RegisterGattClientProcedureCallback
(
    gattClientProcedureCallback_t  callback
)
{
    pfGattClientProcCallback = callback;

    return GattClient_RegisterProcedureCallback(App_GattClientProcedureCallback);
}

/*! *********************************************************************************
\fn            bleResult_t App_RegisterGattClientNotificationCallback(
*                  gattClientNotificationCallback_t  callback)
*\brief        Set application gatt client notification callback and install static
*              callback for Gatt server notifications.
*
*\param  [in]  callback       Callback used by the application to receive gatt
*                             server notifications.
*
*\return       bleResult_t    Result of the operation.
********************************************************************************** */
bleResult_t App_RegisterGattClientNotificationCallback
(
    gattClientNotificationCallback_t  callback
)
{
    pfGattClientNotifCallback = callback;

    return GattClient_RegisterNotificationCallback(App_GattClientNotificationCallback);
}

/*! *********************************************************************************
\fn            bleResult_t App_RegisterGattClientIndicationCallback(
*                  gattClientIndicationCallback_t  callback)
*\brief        Set application gatt client indication callback and install static
*              callback for Gatt server indications.
*
*\param  [in]  callback       Callback used by the application to receive gatt
*                             server indications.
*
*\return       bleResult_t    Result of the operation.
********************************************************************************** */
bleResult_t App_RegisterGattClientIndicationCallback
(
    gattClientIndicationCallback_t  callback
)
{
    pfGattClientIndCallback = callback;

    return GattClient_RegisterIndicationCallback(App_GattClientIndicationCallback);
}
