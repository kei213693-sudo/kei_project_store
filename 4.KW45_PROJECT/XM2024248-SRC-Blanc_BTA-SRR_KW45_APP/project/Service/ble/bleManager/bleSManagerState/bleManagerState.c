/*
 * bleManagerState.c
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#include "bleManagerState_cfg.h"

static void BleApp_StateMachineHandler_AppPair(deviceId_t peerDeviceId, appEvent_t event);
static void BleApp_StateMachineHandler_AppIdle(deviceId_t peerDeviceId, appEvent_t event);
static void BleApp_StateMachineHandler_AppCCCWaitingForOwnerPairingRequest(deviceId_t peerDeviceId, appEvent_t event);
static void BleApp_StateMachineHandler_AppCCCPhase2WaitingForResponse(deviceId_t peerDeviceId, appEvent_t event);
#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
static void BleApp_StateMachineHandler_AppExchangeMtu(deviceId_t peerDeviceId, appEvent_t event);
#endif

/*! *********************************************************************************
* \brief        State machine handler of the Digital Key Car Anchor application.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    event               Event type.
********************************************************************************** */
void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event)
{
    switch (maPeerInformation[peerDeviceId].appState)
    {
        case mAppIdle_c:
        {
            BleApp_StateMachineHandler_AppIdle(peerDeviceId, event);
        }
        break;

 #if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
        case mAppExchangeMtu_c:
        {
            BleApp_StateMachineHandler_AppExchangeMtu(peerDeviceId, event);
        }
        break;
#endif /* (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U)) */

        case mAppCCCWaitingForOwnerPairingRequest_c:
        {
            BleApp_StateMachineHandler_AppCCCWaitingForOwnerPairingRequest(peerDeviceId, event);
        }
        break;

        case mAppCCCPhase2WaitingForResponse_c:
        {
            BleApp_StateMachineHandler_AppCCCPhase2WaitingForResponse(peerDeviceId, event);
        }
        break;

        case mAppCCCPhase2WaitingForVerify_c:
        {
            if (event == mAppEvt_BlePairingReady_c)
            {
                maPeerInformation[peerDeviceId].appState = mAppCCCReadyForPairing_c;
            }
        }
        break;

        case mAppRunning_c:
        {
            /* Enable Notify */
            if ( event == mAppEvt_PeerDisconnected_c )
            {
                maPeerInformation[peerDeviceId].deviceId = gInvalidDeviceId_c;
                maPeerInformation[peerDeviceId].appState = mAppIdle_c;
                maPeerInformation[peerDeviceId].isLinkEncrypted = FALSE;
            }
            else if ( event == mAppEvt_PsmChannelCreated_c)
            {
                /* No action required */
            }
            else
            {
                /* For MISRA compliance */
            }
        }
        break;

        case mAppCCCReadyForPairing_c:
        {
            if ( event == mAppEvt_PairingPeerOobDataRcv_c )
            {
                mCurrentPeerId = peerDeviceId;
                (void)Gap_LeScGetLocalOobData();
                maPeerInformation[peerDeviceId].appState = mAppPair_c;
            }
        }
        break;

        case mAppPair_c:
        {
            BleApp_StateMachineHandler_AppPair(peerDeviceId,event);
        }
        break;

        case mAppServiceDisc_c:
        {
        	shell_write("\r\n----mAppServiceDisc_c");
            if (event == mAppEvt_ServiceDiscoveryComplete_c)
            {
                /* Moving to Running State*/
                maPeerInformation[peerDeviceId].appState = mAppRunning_c;
                shell_write("\r\n----find all services.");
                BleApp_ConfigureNotifications(peerDeviceId);
#if gAppUseBonding_d
                /* Write data in NVM */
                (void)Gap_SaveCustomPeerInformation(maPeerInformation[peerDeviceId].deviceId,
                                                    (void *)&maPeerInformation[peerDeviceId].customInfo, 0,
                                                    (uint16_t)sizeof(appCustomInfo_t));
#endif
            }
            else if (event == mAppEvt_ServiceDiscoveryFailed_c)
            {
                (void)Gap_Disconnect(peerDeviceId);
                shell_write("\r\n service find failure---------");
            }
            else if (event == mAppEvt_PeerDisconnected_c)
            {
            	shell_write("\r\n ----- disconnecting while servicing ---------: ");
				maPeerInformation[peerDeviceId].deviceId = gInvalidDeviceId_c;
				maPeerInformation[peerDeviceId].appState = mAppIdle_c;
				maPeerInformation[peerDeviceId].isLinkEncrypted = FALSE;
            }
            else
            {
            	/* ignore other event types */
            }
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }

    /* Inform the user interface handler about events received in the BleApp_StateMachineHandler */
    if(mpfBleUserInterfaceEventHandler != NULL)
    {
         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t));
         if(pEventData != NULL)
         {
             pEventData->appEvent = event;
             pEventData->eventData.peerDeviceId = peerDeviceId;
             if (gBleSuccess_c != App_PostCallbackMessage(mpfBleUserInterfaceEventHandler, pEventData))
             {
                (void)MEM_BufferFree(pEventData);
             }
         }
    }
}

/*! *********************************************************************************
* \brief        Handler of the mAppIdle_c state for BleApp_StateMachineHandler.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    event               Event type.
************************************************************************************/
static void BleApp_StateMachineHandler_AppIdle(deviceId_t peerDeviceId, appEvent_t event)
{
    if (event == mAppEvt_PeerConnected_c)
    {
        if (maPeerInformation[peerDeviceId].gapRole == gGapPeripheral_c)
        {
            maPeerInformation[peerDeviceId].appState = mAppCCCWaitingForOwnerPairingRequest_c;
            maPeerInformation[peerDeviceId].appStage = pAppPairing_c;
        }
        else
        {
 #if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
 #if defined(gAppUseBonding_d) && (gAppUseBonding_d)
            if (maPeerInformation[peerDeviceId].isBonded == TRUE)
            {
                /* Moving to Running State and wait for Link encryption result */
                maPeerInformation[peerDeviceId].appState = mAppRunning_c;
            }
            else
#endif
            {
              /* Moving to Exchange MTU State */
              maPeerInformation[peerDeviceId].appState = mAppExchangeMtu_c;
              (void)GattClient_ExchangeMtu(peerDeviceId, gAttMaxMtu_c);
            }
#endif /* (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U)) */
        }
    }
}

/*! *********************************************************************************
* \brief        Handler of the mAppExchangeMtu_c state for BleApp_StateMachineHandler.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    event               Event type.
************************************************************************************/
#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
static void BleApp_StateMachineHandler_AppExchangeMtu(deviceId_t peerDeviceId, appEvent_t event)
{
	static uint8_t ret;
    if (event == mAppEvt_GattProcComplete_c)
    {
        /* Moving to Service Discovery State*/
        maPeerInformation[peerDeviceId].appState = mAppServiceDisc_c;

        /* Start Service Discovery*/
//        (void)BleServDisc_Start(peerDeviceId);
        ret = BleServDisc_Start(peerDeviceId);
        shell_write("\r\n ---------------service Disc state-----------------: ");
    }
    else
    {
        if (event == mAppEvt_GattProcError_c)
        {
            (void)Gap_Disconnect(peerDeviceId);
        }
    }
}
#endif /* (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U)) */

/*! *********************************************************************************
* \brief        Handler of the mAppIdle_c state for BleApp_StateMachineHandler.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    event               Event type.
************************************************************************************/
static void BleApp_StateMachineHandler_AppCCCWaitingForOwnerPairingRequest
(
    deviceId_t peerDeviceId,
    appEvent_t event
)
{
    if (event == mAppEvt_OwnerPairingRequestReceived_c)
    {
        bleResult_t status;

        /* Move to CCC Phase 2 */
        /* Dummy data - replace with calls to get actual data */
        uint16_t payloadLen = gDummyPayloadLength_c;
        uint8_t payload[gDummyPayloadLength_c] = gDummyPayload_c;
        status = CCCPhase2_SendSPAKERequest(peerDeviceId, payload, payloadLen);

        if (status == gBleSuccess_c)
        {
            maPeerInformation[peerDeviceId].appState = mAppCCCPhase2WaitingForResponse_c;
        }
    }
    else if (event == mAppEvt_EncryptionChanged_c)
    {
        maPeerInformation[peerDeviceId].isLinkEncrypted = TRUE;
        maPeerInformation[peerDeviceId].appState = mAppRunning_c;
    }
    else
    {
        /* For MISRA compliance */
    }
}

/*! *********************************************************************************
* \brief        Handler of the mAppCCCPhase2WaitingForResponse_c state for
*               BleApp_StateMachineHandler.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    event               Event type.
************************************************************************************/
static void BleApp_StateMachineHandler_AppCCCPhase2WaitingForResponse(deviceId_t peerDeviceId, appEvent_t event)
{
    if (event == mAppEvt_ReceivedSPAKEResponse_c)
    {
        /* SPAKE2+ Flow: Send Verify Command */
        /* Dummy data - replace with calls to get actual data */
        uint16_t payloadLen = gDummyPayloadLength_c;
        uint8_t payload[gDummyPayloadLength_c] = gDummyPayload_c;
        bleResult_t status = CCCPhase2_SendSPAKEVerify(peerDeviceId, payload, payloadLen);
        if (status == gBleSuccess_c)
        {
            maPeerInformation[peerDeviceId].appState = mAppCCCPhase2WaitingForVerify_c;
        }
    }
    else if (event == mAppEvt_EncryptionChanged_c)
    {
        maPeerInformation[peerDeviceId].isLinkEncrypted = TRUE;
        maPeerInformation[peerDeviceId].appState = mAppRunning_c;
    }
    else
    {
        /* For MISRA compliance */
    }
}

/*! *********************************************************************************
* \brief        Handler of the mAppCCCWaitingForOwnerPairingRequest_c state for
*               BleApp_StateMachineHandler.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    event               Event type.
************************************************************************************/
static void BleApp_StateMachineHandler_AppPair(deviceId_t peerDeviceId, appEvent_t event)
{
    if ( event == mAppEvt_PairingLocalOobData_c )
    {
        (void)CCC_FirstApproachRsp(peerDeviceId, gaAppOwnDiscAddress, &maPeerInformation[peerDeviceId].oobData);
    }
    else if ( event == mAppEvt_PairingPeerOobDataRcv_c )
    {
        mCurrentPeerId = peerDeviceId;
        (void)Gap_LeScGetLocalOobData();
    }
    else if (event == mAppEvt_PairingReqRcv_c)
    {
        /* No action required */
    	Gap_LeScGetLocalOobData();
    }
    else if ( event == mAppEvt_PairingPeerOobDataReq_c )
    {
        (void)Gap_LeScSetPeerOobData(peerDeviceId, &maPeerInformation[peerDeviceId].peerOobData);
    }
    else if ( event == mAppEvt_PairingComplete_c )
    {
        FLib_MemSet(&maPeerInformation[peerDeviceId].oobData, 0x00, sizeof(gapLeScOobData_t));
        FLib_MemSet(&maPeerInformation[peerDeviceId].peerOobData, 0x00, sizeof(gapLeScOobData_t));
        maPeerInformation[peerDeviceId].appState = mAppRunning_c;
    }
    else
    {
        /* For MISRA compliance */
    }
}
