/*
 * bleManagerL2CA.c
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#include "bleManagerL2CA_cfg.h"

static deviceId_t mDeviceIdToDisconnect;

static void L2caTimerCallback(void *param);

/*! *********************************************************************************
* \brief        Callback for incoming PSM data.
*
* \param[in]    deviceId        The device ID of the connected peer that sent the data
* \param[in]    lePsm           Channel ID
* \param[in]    pPacket         Pointer to incoming data
* \param[in]    packetLength    Length of incoming data
********************************************************************************** */
void BleApp_L2capPsmDataCallback (deviceId_t     deviceId,
                                         uint16_t       lePsm,
                                         uint8_t*       pPacket,
                                         uint16_t       packetLength)
{
    /* The vehicle shall trigger a disconnect 5 seconds after an unencrypted L2CAP connection establishment
     * if no First_Approach_RQ or Request_owner_pairing Command Complete SubEvent notification has been
     * received during the first approach and owner pairing, respectively.*/
    if(TM_IsTimerActive(mL2caTimerId) == 1U)
    {
        if (packetLength > (gMessageHeaderSize_c + gPayloadHeaderSize_c + gLengthFieldSize_c))
        {
            dkMessageType_t messageType = (dkMessageType_t)pPacket[0];
            rangingMsgId_t msgId = (rangingMsgId_t)pPacket[1];

            switch (messageType)
            {
                case gDKMessageTypeDKEventNotification_c:
                {
                    if ( msgId == gDkEventNotification_c )
                    {
                        if ( packetLength == (gMessageHeaderSize_c + gPayloadHeaderSize_c + gLengthFieldSize_c + gCommandCompleteSubEventPayloadLength_c) )
                        {
                            dkSubEventCategory_t category = (dkSubEventCategory_t)pPacket[gMessageHeaderSize_c + gPayloadHeaderSize_c + gLengthFieldSize_c];
                            if ( category == gCommandComplete_c)
                            {
                                dkSubEventCommandCompleteType_t type = (dkSubEventCommandCompleteType_t)pPacket[gMessageHeaderSize_c + gPayloadHeaderSize_c + gLengthFieldSize_c + sizeof(category)];
                                if ( type == gRequestOwnerPairing_c )
                                {
                                    (void)TM_Stop((timer_handle_t)mL2caTimerId);
                                }
                            }
                        }
                    }
                }
                break;

                case gDKMessageTypeSupplementaryServiceMessage_c:
                {
                    if ( msgId == gFirstApproachRQ_c )
                    {
                        if ( packetLength == (gMessageHeaderSize_c + gPayloadHeaderSize_c + gLengthFieldSize_c + gFirstApproachReqRspPayloadLength) )
                        {
                            (void)TM_Stop((timer_handle_t)mL2caTimerId);
                        }
                    }
                }
				break;

                default:
                    ; /* For MISRA compliance */
                    break;
            }
        }
    }


    /* Inform the application events handler that L2capPsmData package received */
    if(mpfBleEventHandler != NULL)
    {
        appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(appEventL2capPsmData_t) + (uint32_t)packetLength);
        if(pEventData != NULL)
        {
            pEventData->appEvent = mAppEvt_L2capPsmDataCallback_c;
            pEventData->eventData.pData = &pEventData[1];
            appEventL2capPsmData_t *pL2capPsmDataEvent = pEventData->eventData.pData;
            pL2capPsmDataEvent->deviceId = deviceId;
            pL2capPsmDataEvent->lePsm = lePsm;
            pL2capPsmDataEvent->packetLength = packetLength;
            pL2capPsmDataEvent->pPacket = (uint8_t*)(&pL2capPsmDataEvent[1]);
            FLib_MemCpy(pL2capPsmDataEvent->pPacket, pPacket, packetLength);
            pL2capPsmDataEvent = NULL;
            if (gBleSuccess_c != App_PostCallbackMessage(mpfBleEventHandler, pEventData))
            {
               (void)MEM_BufferFree(pEventData);
            }
        }
    }
}

/*! *********************************************************************************
* \brief        Callback for control messages.
*
* \param[in]    pMessage    Pointer to control message
********************************************************************************** */
void BleApp_L2capPsmControlCallback(l2capControlMessage_t* pMessage)
{
    switch (pMessage->messageType)
    {
#if defined(gHandoverDemo_d) && (gHandoverDemo_d == 1)
        case gL2ca_HandoverConnectionComplete_c:
        {
            /* Save L2CAP channel ID */
            maPeerInformation[pMessage->messageData.handoverConnectionComplete.deviceId].customInfo.psmChannelId =
                pMessage->messageData.handoverConnectionComplete.cId;
        }
        break;
#endif
        case gL2ca_LePsmConnectRequest_c:
        {
            deviceId_t deviceId = pMessage->messageData.connectionRequest.deviceId;

            /* For Passive Entry the link must be encrypted before opening the L2CAP CB channel */
            if ((maPeerInformation[deviceId].isBonded == TRUE) && (maPeerInformation[deviceId].isLinkEncrypted == FALSE))
            {
                (void)L2ca_CancelConnection(gDK_DefaultVehiclePsm_c, deviceId, gInsufficientEncryption_c);
            }
            else
            {
                (void)L2ca_ConnectLePsm(gDK_DefaultVehiclePsm_c, deviceId, mAppLeCbInitialCredits_c);
            }
        }
        break;

        case gL2ca_LePsmConnectionComplete_c:
        {
            if (pMessage->messageData.connectionComplete.result == gSuccessful_c)
            {
                /* Handle Conn Complete */
                maPeerInformation[pMessage->messageData.connectionComplete.deviceId].customInfo.psmChannelId = pMessage->messageData.connectionComplete.cId;
                if (maPeerInformation[pMessage->messageData.connectionComplete.deviceId].isLinkEncrypted == FALSE)
                {
                    if (mL2caTimerValid == TRUE)
                    {
                        mDeviceIdToDisconnect = pMessage->messageData.connectionComplete.deviceId;
                        (void)TM_InstallCallback((timer_handle_t)mL2caTimerId, L2caTimerCallback, &mDeviceIdToDisconnect);
                        (void)TM_Start((timer_handle_t)mL2caTimerId, (uint8_t)kTimerModeLowPowerTimer | (uint8_t)kTimerModeSetSecondTimer, gCCCL2caTimeout_c);
                    }
                }
                BleApp_StateMachineHandler(maPeerInformation[pMessage->messageData.connectionComplete.deviceId].deviceId, mAppEvt_PsmChannelCreated_c);
            }
        }
        break;

        case gL2ca_LePsmDisconnectNotification_c:
        {

        }
        break;

        case gL2ca_NoPeerCredits_c:
        {
            (void)L2ca_SendLeCredit (pMessage->messageData.noPeerCredits.deviceId,
                                    pMessage->messageData.noPeerCredits.cId,
                                    mAppLeCbInitialCredits_c);
        }
        break;

        case gL2ca_Error_c:
        {
            /* Handle error */
        }
        break;

        default:
        {
            ; /* For MISRA compliance */
            break;
        }
    }
}

/*! *********************************************************************************
 * \brief        Timer Callback
 ********************************************************************************** */
static void L2caTimerCallback(void *param)
{
    deviceId_t deviceId = *(deviceId_t*)param;

    if (maPeerInformation[deviceId].deviceId != gInvalidDeviceId_c)
    {
        (void)Gap_Disconnect(maPeerInformation[deviceId].deviceId);
    }
}
