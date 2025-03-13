/*
 * bleAppEvent.c
 *
 *  Created on: 2024年7月11日
 *      Author: F704012276_
 */

#include "signals_api.h"
#include "drv_api.h"
#include "srv_api.h"
#include "app_preinclude.h"
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"

#define mcEncryptionKeySize_c   16
/* LTK */
static uint8_t gaAppSmpLtk[gcSmpMaxLtkSize_c];
/* LTK size */
#define gAppLtkSize_c mcEncryptionKeySize_c
/* IRK */
static uint8_t gaAppSmpIrk[gcSmpIrkSize_c];
/* RAND*/
static uint8_t gaAppSmpRand[gcSmpMaxRandSize_c];
/* Address */
static uint8_t gaAppAddress[gcBleDeviceAddressSize_c];
static gapSmpKeys_t gAppOutKeys = {
    .cLtkSize = gAppLtkSize_c,
    .aLtk = (void *)gaAppSmpLtk,
    .aIrk = (void *)gaAppSmpIrk,
    .aCsrk = NULL,
    .aRand = (void *)gaAppSmpRand,
    .cRandSize = gcSmpMaxRandSize_c,
    .ediv = 0,
    .addressType = 0,
    .aAddress = gaAppAddress
};
static gapSmpKeyFlags_t gAppOutKeyFlags;
static bool_t gAppOutLeSc;
static bool_t gAppOutAuth;
#if defined(gAppUseShellInApplication_d) && (gAppUseShellInApplication_d == 1)
#if (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U))
/* LTK size */
#define gAppLtkSize_c gcSmpMaxBlobSize_c
/* LTK */
static uint8_t gaAppSmpLtk[gcSmpMaxBlobSize_c];
/* IRK */
static uint8_t gaAppSmpIrk[gcSmpMaxBlobSize_c];
#else /* (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U)) */
#endif /* (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U)) */
#if (defined (gAppSecureMode_d) && (gAppSecureMode_d == 0U) || (defined(gA2BEnabled_d) && (gA2BEnabled_d > 0U)))
/*
Global used to identify if a bond was added by
shell command or connection with the device
mBondAddedFromShell = FALSE bond created by connection
mBondAddedFromShell = TRUE  bond added by shell command
*/
static bool_t mBondAddedFromShell = FALSE;
#endif
#endif /* defined(gAppUseShellInApplication_d) && (gAppUseShellInApplication_d == 1) */

#if defined(gAppUseShellInApplication_d) && (gAppUseShellInApplication_d == 1)
void App_HandleLeScLocalOobDataCallback(appEventData_t *pEventData)
{
    if (mCurrentPeerId != gInvalidDeviceId_c)
    {
        FLib_MemCpy(&maPeerInformation[mCurrentPeerId].oobData, pEventData->eventData.pData, sizeof(gapLeScOobData_t));
        BleApp_StateMachineHandler(mCurrentPeerId, mAppEvt_PairingLocalOobData_c);
    }
}

#if (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U))
/*! *********************************************************************************
* \brief        Handles GenericCallback events.
*
* \param[in]    pEventData    pointer to appEventData_t.
********************************************************************************** */
void A2A_HandleGenericCallbackBondCreatedEvent(appEventData_t *pEventData)
{
    bleBondCreatedEvent_t *pBondEventData = (bleBondCreatedEvent_t *)pEventData->eventData.pData;
    bleResult_t status = gBleSuccess_c;

    status = Gap_LoadKeys(pBondEventData->nvmIndex,
                          &gAppOutKeys, &gAppOutKeyFlags, &gAppOutLeSc,
                          &gAppOutAuth);

    if ( status == gBleSuccess_c)
    {
        /* address type, address, ltk, irk */
#if 0
        shell_write("\r\nBondingData: ");
        shell_writeHex((uint8_t*)&gAppOutKeys.addressType, 1);
        shell_write(" ");
        shell_writeHex(gAppOutKeys.aAddress, gcBleDeviceAddressSize_c);
        shell_write(" ");
        shell_writeHex((uint8_t*)gAppOutKeys.aLtk, gcSmpMaxBlobSize_c);
        shell_write(" ");
        shell_writeHex((uint8_t*)gAppOutKeys.aIrk, gcSmpIrkSize_c);
        shell_write("\r\n");
#else
        dbg_Printf("\r\nBondingData: ");
        dbg_Printf("\r\nBondingData: %x", (uint8_t*)&gAppOutKeys.addressType);
        dbg_Printf("\r\nBondingData: %x", &(gAppOutKeys.aAddress));
        dbg_Printf("\r\nBondingData: %x", (uint8_t*)gAppOutKeys.aLtk);
        dbg_Printf("\r\nBondingData: %x", (uint8_t*)gAppOutKeys.aIrk);
#endif
    }
}
#else

#endif /* (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U)) */

/*! *********************************************************************************
* \brief        Handles L2capPsmDataCallback events.
*
* \param[in]    pEventData    pointer to appEventData_t.
********************************************************************************** */
void App_HandleL2capPsmDataCallback(appEventData_t *pEventData)
{

    appEventL2capPsmData_t *l2capDataEvent = (appEventL2capPsmData_t *)pEventData->eventData.pData;
    deviceId_t deviceId = l2capDataEvent->deviceId;
    uint16_t packetLength = l2capDataEvent->packetLength;
    uint8_t* pPacket = l2capDataEvent->pPacket;

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
                                shell_write("\r\nReceived Command Complete SubEvent: Request_owner_pairing.\r\n");
                                BleApp_StateMachineHandler(deviceId, mAppEvt_OwnerPairingRequestReceived_c);
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
                        uint8_t *pData = &pPacket[gMessageHeaderSize_c + gPayloadHeaderSize_c + gLengthFieldSize_c];

                        /* BD address not used. */
                        pData = &pData[gcBleDeviceAddressSize_c];
                        /* Confirm Value */
                        FLib_MemCpy(&maPeerInformation[deviceId].peerOobData.confirmValue, pData, gSmpLeScRandomConfirmValueSize_c);
                        pData = &pData[gSmpLeScRandomConfirmValueSize_c];
                        /* Random Value */
                        FLib_MemCpy(&maPeerInformation[deviceId].peerOobData.randomValue, pData, gSmpLeScRandomValueSize_c);

                        /* Send event to the application state machine. */
                        BleApp_StateMachineHandler(deviceId, mAppEvt_PairingPeerOobDataRcv_c);
                    }
                    else
                    {
                        shell_write("\r\nERROR: Invalid length for FirstApproachRQ.\r\n");
                    }
                }
                else if (msgId == gTimeSync_c)
                {
                    shell_write("\r\nTime Sync received.\r\n");
                    /* Handle Time Sync data */
                    (void)mTsUwbDeviceTime;
                }
                else
                {
                    /* For MISRA compliance */
                }
            }
            break;

            case gDKMessageTypeFrameworkMessage_c:
            {
                switch (maPeerInformation[deviceId].appState)
                {
                    case mAppCCCPhase2WaitingForResponse_c:
                    {
                         /* Process the data in pPacket here */
                         shell_write("\r\nSPAKE Response received.\r\n");
                         BleApp_StateMachineHandler(deviceId, mAppEvt_ReceivedSPAKEResponse_c);
                    }
                    break;

                    case mAppCCCPhase2WaitingForVerify_c:
                    {
                        /* Process the data in pPacket here */
                        shell_write("\r\nSPAKE Verify received.\r\n");
                        shell_write("\r\nSending Command Complete SubEvent: BLE_pairing_ready\r\n");
                        (void)CCC_SendSubEvent(deviceId, gCommandComplete_c, gBlePairingReady_c);
                        BleApp_StateMachineHandler(deviceId, mAppEvt_BlePairingReady_c);
                    }
                    break;

                    default:
                    {
                        ; /* For MISRA compliance */
                    }
                    break;
                }
            }
            break;

            case gDKMessageTypeVehicleOEMAppMessage_c:
            case gDKMessageType1A_c:
            case gDKMessageType20_c:
            case gDKMessageType40_c:
            	/*BleIf_Receive(deviceId, pPacket, packetLength);*/
            break;

            default:
                ; /* For MISRA compliance */
                break;
        }
    }
}
#endif

/*! *********************************************************************************
* \brief        Handles GenericCallback events.
*
* \param[in]    pEventData    pointer to appEventData_t.
********************************************************************************** */
extern Device_IRK_Group_1_msg Device_IRK_Group_1_t;
static btmWhitelist_t bondInfo;
void App_HandleGenericCallbackBondCreatedEvent(appEventData_t *pEventData)
{
    bleBondCreatedEvent_t *pBondEventData = (bleBondCreatedEvent_t *)pEventData->eventData.pData;
    bleResult_t status;

    status = Gap_LoadKeys(pBondEventData->nvmIndex,
                          &gAppOutKeys, &gAppOutKeyFlags, &gAppOutLeSc,
                          &gAppOutAuth);

    if ( status == gBleSuccess_c)
    {
        /* address type, address, ltk, irk */
        shell_write("\r\nBondingData: %d", gAppOutKeys.addressType);
        shell_write("\r\nPublic Address: ");
        for(uint8_t i = 0; i < 6; i++)
        {
        	shell_write(" %x", gAppOutKeys.aAddress[i]);
        }
        shell_write("\r\nLong-Time Key: %d", gAppOutKeys.aLtk[0]);
        shell_write("\r\nIdentity Resolving Key: ");
        for(uint8_t i = 0; i < 16; i++)
        {
        	shell_write(" %x", gAppOutKeys.aIrk[i]);
        }
        shell_write("\r\ngAppOutAuth: %d", gAppOutAuth);
        shell_write("\r\ngAppOutLeSc: %d", gAppOutLeSc);
        shell_write("\r\ngAppOutKeyFlags: %d", gAppOutKeyFlags);
        shell_write("\r\n");

        for(uint8_t i = 0; i < gAppMaxConnections_c; i++)
        {
        	if(maPeerInformation[i].appState == mAppWaitPairComplete_c)
        	{
                /*TODO 添加数据到白名单*/
                memcpy(bondInfo.BD_ADDR, gAppOutKeys.aAddress, gcBleDeviceAddressSize_c);
                memcpy(bondInfo.IRK, gAppOutKeys.aIrk, gcSmpIrkSize_c);
                memcpy(bondInfo.LTK, gAppOutKeys.aLtk, gcSmpMaxLtkSize_c);
                if(maPeerInformation[i].gapRole == gGapCentral_c)
                {
                	bondInfo.bleType = BTMWhitelistKeyfobType;
                	memcpy(maPeerInformation[i].maPeerDeviceAddress_IRK, \
                			gAppOutKeys.aAddress, gcBleDeviceAddressSize_c);
                }
                else if(maPeerInformation[i].gapRole == gGapPeripheral_c)
                {
                	bondInfo.bleType = BTMWhitelistPhoneType;
                	memcpy(maPeerInformation[i].maPeerDeviceAddress_IRK, \
                			gAppOutKeys.aIrk, gcSmpIrkSize_c);
                }

        		Bwl_AddWL(&bondInfo);
				break;
        	}
        }
    }
}
