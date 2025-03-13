/*
 * bleL2ca.c
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#include "bleL2ca_cfg.h"

STATIC void App_L2caLeDataCallback
(
    deviceId_t deviceId,
    uint16_t channelId,
    uint8_t* pPacket,
    uint16_t packetLength
);

STATIC void App_L2caLeControlCallback
(
    l2capControlMessage_t* pMessage
);

/*! *********************************************************************************
\fn            bleResult_t App_RegisterLeCbCallbacks(
*                  l2caLeCbDataCallback_t      pCallback,
*                  l2caLeCbControlCallback_t   pCtrlCallback
*              )
*\brief        Set application L2CAP callbacks for credit based data and control
*              events and register static callbacks on L2CAP.
*
*\param  [in]  pCallback      Callback function for data plane messages.
*\param  [in]  pCtrlCallback  Callback function for control plane messages.
*
*\return       bleResult_t    Result of the operation.
********************************************************************************** */
bleResult_t App_RegisterLeCbCallbacks
(
    l2caLeCbDataCallback_t      pCallback,
    l2caLeCbControlCallback_t   pCtrlCallback
)
{
    pfL2caLeCbDataCallback = pCallback;
    pfL2caLeCbControlCallback = pCtrlCallback;

    return L2ca_RegisterLeCbCallbacks(App_L2caLeDataCallback, App_L2caLeControlCallback);
}

/*! *********************************************************************************
*\private
*\fn           void App_L2caLeDataCallback(deviceId_t deviceId,
                                           uint16_t   channelId,
                                           uint8_t*   pPacket,
                                           uint16_t   packetLength)
*\brief        Callback used by the Host Stack to send L2CAP credit based data to
*              the application.
*
*\param  [in]  deviceId        Peer device id.
*\param  [in]  channelId       Identifier of the L2CAP channel used for credit based
*                              data transmission.
*\param  [in]  pPacket         Pointer to the L2CAP packet.
*\param  [in]  packetLength    Packet length.
*
*\retval       void.
********************************************************************************** */
STATIC void App_L2caLeDataCallback
(
    deviceId_t deviceId,
    uint16_t channelId,
    uint8_t* pPacket,
    uint16_t packetLength
)
{
    appMsgFromHost_t *pMsgIn = NULL;

    /* Allocate a buffer with enough space to store the packet */
    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) +
                       (sizeof(l2caLeCbDataMsg_t) - 1U) +
                       (uint32_t)packetLength);

    if (pMsgIn == NULL)
    {
        return;
    }

    pMsgIn->msgType = (uint32_t)gAppL2caLeDataMsg_c;
    pMsgIn->msgData.l2caLeCbDataMsg.deviceId = deviceId;
    pMsgIn->msgData.l2caLeCbDataMsg.channelId = channelId;
    pMsgIn->msgData.l2caLeCbDataMsg.packetLength = packetLength;

    FLib_MemCpy(pMsgIn->msgData.l2caLeCbDataMsg.aPacket,
                pPacket,
                packetLength);

    /* Put message in the Host Stack to App queue */
    (void)MSG_QueueAddTail(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}

/*! *********************************************************************************
*\private
*\fn           void App_L2caLeControlCallback(l2capControlMessage_t* pMessage)
*\brief        Callback used by the Host Stack to propagate L2CAP control events to
*              the application.
*
*\param  [in]  pMessage    Pointer to the L2CAP control message.
*
*\retval       void.
********************************************************************************** */
STATIC void App_L2caLeControlCallback
(
    l2capControlMessage_t* pMessage
)
{
    appMsgFromHost_t *pMsgIn = NULL;
    uint8_t messageLength = 0U;

    switch (pMessage->messageType) {
        case gL2ca_LePsmConnectRequest_c:
        {
            messageLength = (uint8_t)sizeof(l2caLeCbConnectionRequest_t);
            break;
        }
        case gL2ca_LePsmConnectionComplete_c:
        {
            messageLength = (uint8_t)sizeof(l2caLeCbConnectionComplete_t);
            break;
        }
        case gL2ca_LePsmDisconnectNotification_c:
        {
            messageLength = (uint8_t)sizeof(l2caLeCbDisconnection_t);
            break;
        }
        case gL2ca_NoPeerCredits_c:
        {
            messageLength = (uint8_t)sizeof(l2caLeCbNoPeerCredits_t);
            break;
        }
        case gL2ca_LocalCreditsNotification_c:
        {
            messageLength = (uint8_t)sizeof(l2caLeCbLocalCreditsNotification_t);
            break;
        }
        case gL2ca_LowPeerCredits_c:
        {
            messageLength = (uint8_t)sizeof(l2caLeCbLowPeerCredits_t);
            break;
        }
        case gL2ca_Error_c:
        {
            messageLength = (uint8_t)sizeof(l2caLeCbError_t);
            break;
        }

        case gL2ca_HandoverConnectionComplete_c:
        {
            messageLength = (uint8_t)sizeof(l2caHandoverConnectionComplete_t);
        }
        break;

        default:
        {
            messageLength = 0U;
            break;
        }
    }

    if (messageLength == 0U)
    {
        return;
    }

    /* Allocate a buffer with enough space to store the biggest packet */
    pMsgIn = MSG_Alloc((uint32_t)&(pMsgIn->msgData) + sizeof(l2capControlMessage_t));

    if (pMsgIn == NULL)
    {
          return;
    }

    pMsgIn->msgType = (uint32_t)gAppL2caLeControlMsg_c;
    pMsgIn->msgData.l2caLeCbControlMsg.messageType = pMessage->messageType;

    FLib_MemCpy(&pMsgIn->msgData.l2caLeCbControlMsg.messageData,
                &pMessage->messageData,
                messageLength);

    /* Put message in the Host Stack to App queue */
    (void)MSG_QueueAddTail(&mHostAppInputQueue, pMsgIn);

    /* Signal application */
    (void)OSA_EventSet(mAppEvent, gAppEvtMsgFromHostStack_c);
}
