/*
 * bleCCC.c
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */
#include "bleCCC_cfg.h"

/*! *********************************************************************************
 * \brief        First Approach messages enables BLE OOB Secure LE pairing for both owner and friend devices
 *
 ********************************************************************************** */
bleResult_t CCC_FirstApproachRsp(deviceId_t deviceId, uint8_t* pBdAddr, gapLeScOobData_t* pOobData)
{
    bleResult_t result = gBleSuccess_c;

    uint8_t aPayload[gFirstApproachReqRspPayloadLength] = {0};
    uint8_t *pData = aPayload;

    if (FLib_MemCmpToVal(pOobData, 0x00, sizeof(gapLeScOobData_t)))
    {
        result = gBleInvalidParameter_c;
    }
    else
    {
        FLib_MemCpy(pData, pBdAddr, gcBleDeviceAddressSize_c);
        pData = &pData[gcBleDeviceAddressSize_c];
        FLib_MemCpy(pData, pOobData->confirmValue, gSmpLeScRandomConfirmValueSize_c);
        pData = &pData[gSmpLeScRandomConfirmValueSize_c];
        FLib_MemCpy(pData, pOobData->randomValue, gSmpLeScRandomValueSize_c);

        result = DK_SendMessage(deviceId,
                                maPeerInformation[deviceId].customInfo.psmChannelId,
                                gDKMessageTypeSupplementaryServiceMessage_c,
                                gFirstApproachRS_c,
                                gFirstApproachReqRspPayloadLength,
                                aPayload);
    }

    return result;
}

/*! *********************************************************************************
 * \brief        Owner Pairing Certificate Exchange - step 1, SPAKE2+ Request Command
 *
 ********************************************************************************** */
bleResult_t CCCPhase2_SendSPAKERequest(deviceId_t deviceId, uint8_t *pData, uint16_t dataLen)
{
    bleResult_t result = gBleSuccess_c;

    result = DK_SendMessage(deviceId,
                            maPeerInformation[deviceId].customInfo.psmChannelId,
                            gDKMessageTypeFrameworkMessage_c,
                            gDkApduRQ_c,
                            dataLen,
                            pData);

    /* Inform the user interface handler that SPAKE Request has been sent */
    if(mpfBleUserInterfaceEventHandler != NULL)
    {
         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t));
         if(pEventData != NULL)
         {
             pEventData->appEvent = mAppEvt_SPAKERequestSent_c;
             pEventData->eventData.peerDeviceId = deviceId;
             if (gBleSuccess_c != App_PostCallbackMessage(mpfBleUserInterfaceEventHandler, pEventData))
             {
                (void)MEM_BufferFree(pEventData);
             }
         }
    }

    return result;
}

/*! *********************************************************************************
 * \brief        Owner Pairing Certificate Exchange - step 2, SPAKE2+ Verify Command
 *
 ********************************************************************************** */
bleResult_t CCCPhase2_SendSPAKEVerify(deviceId_t deviceId, uint8_t *pData, uint16_t dataLen)
{
    bleResult_t result = gBleSuccess_c;

    result = DK_SendMessage(deviceId,
                            maPeerInformation[deviceId].customInfo.psmChannelId,
                            gDKMessageTypeFrameworkMessage_c,
                            gDkApduRQ_c,
                            dataLen,
                            pData);

    /* Inform the user interface handler that SPAKE Verify has been sent */
    if(mpfBleUserInterfaceEventHandler != NULL)
    {
         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t));
         if(pEventData != NULL)
         {
             pEventData->appEvent = mAppEvt_SPAKEVerifySent_c;
             pEventData->eventData.peerDeviceId = deviceId;
             if (gBleSuccess_c != App_PostCallbackMessage(mpfBleUserInterfaceEventHandler, pEventData))
             {
                (void)MEM_BufferFree(pEventData);
             }
         }
    }
    return result;
}

/*! *********************************************************************************
 * \brief        Sends DK SubEvents to Device.
 *
 ********************************************************************************** */
bleResult_t CCC_SendSubEvent(deviceId_t deviceId,
                             dkSubEventCategory_t category,
                             dkSubEventCommandCompleteType_t type)
{
    bleResult_t result = gBleSuccess_c;

    uint8_t payload[gCommandCompleteSubEventPayloadLength_c] = {0}; /* SubEvent Category + SubEvent Type */
    payload[0] = (uint8_t)category;
    payload[1] = (uint8_t)type;

    result = DK_SendMessage(deviceId,
                            maPeerInformation[deviceId].customInfo.psmChannelId,
                            gDKMessageTypeDKEventNotification_c,
                            gDkEventNotification_c,
                            gCommandCompleteSubEventPayloadLength_c,
                            payload);

    return result;
}
