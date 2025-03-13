/*
 * bleManagerGap.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "bleManagerGap_cfg.h"
#include "app_preinclude.h"

static void BleApp_GenericCallback_ControllerNotificationEvent(gapGenericEvent_t* pGenericEvent);
static uint64_t GetUwbClock(void);
static void BleApp_GenericCallback_HandlePrivacyEvents(gapGenericEvent_t* pGenericEvent);
static bool_t CheckScanEventLegacy(gapScannedDevice_t* pData);
static uint8_t BleApp_GetNoOfActiveConnections(void);

void App_GenericHandler(gapGenericEvent_t *pGenericEvent);
void App_AdvertiserHandler(gapGenericEventType_t  eventType);

static uint16_t cpReadHandles[] = { (uint16_t)value_SK_TX, 	(uint16_t)value_SK_RX, \
									(uint16_t)value_SKP_RX, 	(uint16_t)value_SKP_RX};
static uint16_t cpWriteHandles[] = { (uint16_t)value_SK_TX, 	(uint16_t)value_SK_RX, \
									 (uint16_t)value_SKP_RX, 	(uint16_t)value_SKP_RX};
static bool_t mOwnerPairingMode = TRUE;


#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
/* Scan state */
static bool_t mFoundPeripheralToConnect = FALSE;

#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
static bool_t mRestoringBondedLink = FALSE;
#endif /* gAppUseBonding_d */

#endif /* gAppScanNonCCC_d */

/*! *********************************************************************************
*\private
*\fn           void App_GenericHandler(gapGenericEvent_t *pGenericEvent)
*\brief        Handles BLE generic callback.
*
*\param  [in]  pGenericEvent    Pointer to gapGenericEvent_t.
*
*\retval       void.
********************************************************************************** */
void App_GenericHandler
(
    gapGenericEvent_t *pGenericEvent
)
{
    /* Call application handler for advanced use */
    if (mpfAppGenericCallback != NULL)
    {
        mpfAppGenericCallback(pGenericEvent);
    }

    switch (pGenericEvent->eventType)
    {
        case gInitializationComplete_c:
        {
            /* Call application handler */
            if (mpfInitDoneCallback != NULL)
            {
                mpfInitDoneCallback();
            }
        }
        break;
        case gDeInitializationComplete_c:
        {
            (void)Ble_DeInitialize();
        }
        break;
        /* Direct advertising events to the advertising handler */
        case gAdvertisingParametersSetupComplete_c:
                            /* Fall Through */
        case gAdvertisingDataSetupComplete_c:
                            /* Fall Through */
        case gAdvertisingSetupFailed_c:
                            /* Fall Through */
        case gExtAdvertisingParametersSetupComplete_c:
                            /* Fall Through */
        case gExtAdvertisingDataSetupComplete_c:
        {
            if (pfAdvertiserHandler != NULL)
            {
                pfAdvertiserHandler(pGenericEvent->eventType);
            }
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles BLE generic callback.
*
* \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
********************************************************************************** */
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    /* Call BLE Conn Manager */
    BleConnManager_GenericEvent(pGenericEvent);

    switch (pGenericEvent->eventType)
    {
        case gLePhyEvent_c:
        {
            if(pGenericEvent->eventData.phyEvent.phyEventType == gPhyUpdateComplete_c )
            {
                /* Inform application about Le Phy user interface event */
                if(mpfBleUserInterfaceEventHandler != NULL)
                {
                    appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(gapPhyEvent_t));
                    if(pEventData != NULL)
                    {
                        pEventData->appEvent = mAppEvt_LePhyEvent_c;
                        pEventData->eventData.pData = &pEventData[1];
                        FLib_MemCpy(pEventData->eventData.pData, &pGenericEvent->eventData.phyEvent, sizeof(gapPhyEvent_t));
                        if (gBleSuccess_c != App_PostCallbackMessage(mpfBleUserInterfaceEventHandler, pEventData))
                        {
                           (void)MEM_BufferFree(pEventData);
                        }
                    }
                }
            }
        }
        break;

        case gLeScLocalOobData_c:
        {
            /* Inform application about LE SC Local OOB Data event */
            if(mpfBleEventHandler != NULL)
            {
                appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(gapLeScOobData_t));
                if(pEventData != NULL)
                {
                    pEventData->appEvent = mAppEvt_GenericCallback_LeScLocalOobData_c;
                    pEventData->eventData.pData = &pEventData[1];
                    FLib_MemCpy(pEventData->eventData.pData, &pGenericEvent->eventData.localOobData, sizeof(gapLeScOobData_t));
                    if (gBleSuccess_c != App_PostCallbackMessage(mpfBleEventHandler, pEventData))
                    {
                       (void)MEM_BufferFree(pEventData);
                    }
                }
            }
        }
        break;

        case gBondCreatedEvent_c:
        {
            /* Inform application about bond created event */
            if(mpfBleEventHandler != NULL)
            {
                appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(bleBondCreatedEvent_t));
                if(pEventData != NULL)
                {
                    pEventData->appEvent = mAppEvt_GenericCallback_BondCreatedEvent_c;
                    pEventData->eventData.pData = &pEventData[1];
                    FLib_MemCpy(pEventData->eventData.pData, &pGenericEvent->eventData.bondCreatedEvent, sizeof(bleBondCreatedEvent_t));
                    if (gBleSuccess_c != App_PostCallbackMessage(mpfBleEventHandler, pEventData))
                    {
                       (void)MEM_BufferFree(pEventData);
                    }
                }
            }
        }
        break;

        case gRandomAddressReady_c:
        {
            if (pGenericEvent->eventData.addrReady.advHandle == gLegacyAdvSetHandle_c)
            {
                FLib_MemCpy(gaAppOwnDiscAddress, pGenericEvent->eventData.addrReady.aAddress, gcBleDeviceAddressSize_c);
            }
        }
        break;

        case gControllerNotificationEvent_c:
        {
            BleApp_GenericCallback_ControllerNotificationEvent(pGenericEvent);
        }
        break;

        case gHostPrivacyStateChanged_c:
        case gControllerPrivacyStateChanged_c:
        {
            BleApp_GenericCallback_HandlePrivacyEvents(pGenericEvent);
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
 * \brief        Configures BLE Stack after initialization
 *
 ********************************************************************************** */
void BluetoothLEHost_Initialized(void)
{
	/* Common GAP configuration */
	BleConnManager_GapCommonConfig();

    /* Register for callbacks*/
    (void)App_RegisterGattServerCallback(BleApp_GattServerCallback);
    (void)GattServer_RegisterHandlesForWriteNotifications(NumberOfElements(cpWriteHandles), cpWriteHandles);
    (void)GattServer_RegisterHandlesForReadNotifications(NumberOfElements(cpReadHandles), cpReadHandles);

#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
    (void)App_RegisterGattClientProcedureCallback(BleApp_GattClientCallback);
    (void)App_RegisterGattClientNotificationCallback(BleApp_GattNotificationCallback);
    BleServDisc_RegisterCallback(BleApp_ServiceDiscoveryCallback);

#endif /* (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U)) */

    /* Register DK L2CAP PSM */
    (void)L2ca_RegisterLePsm(gDK_DefaultVehiclePsm_c, gDKMessageMaxLength_c);

    /* Register stack callbacks */
    (void)App_RegisterLeCbCallbacks(BleApp_L2capPsmDataCallback, BleApp_L2capPsmControlCallback);

    /* Allocate application timers */
#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
    (void)TM_Open(mAppTimerId);
#endif
    if (TM_Open(mL2caTimerId) == kStatus_TimerSuccess)
    {
        mL2caTimerValid = TRUE;
    }

    /* Inform the user interface handler that Bluetooth application configuration done */
    if(mpfBleUserInterfaceEventHandler != NULL)
    {
         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t));
         if(pEventData != NULL)
         {
             pEventData->appEvent = mAppEvt_BleConfigDone_c;
             if (gBleSuccess_c != App_PostCallbackMessage(mpfBleUserInterfaceEventHandler, pEventData))
             {
                (void)MEM_BufferFree(pEventData);
             }
         }
    }

    (void)Gap_ControllerEnhancedNotification(((uint32_t)gNotifConnCreated_c | (uint32_t)gNotifPhyUpdateInd_c), 0U);
}

/*! *********************************************************************************
*\private
*\fn          void App_AdvertiserHandler(gapGenericEventType_t  eventType)
*\brief       This function handles the advertising events state machine.
*
*\param  [in] eventType              Received advertising event type.
*
*\retval      void.
********************************************************************************** */
void App_AdvertiserHandler(gapGenericEventType_t  eventType)
{
    switch (eventType)
    {
        case gAdvertisingParametersSetupComplete_c:
        {
            (void)Gap_SetAdvertisingData(mpAdvParams->pGapAdvData,
                                         mpAdvParams->pScanResponseData);
        }
        break;

        case gAdvertisingDataSetupComplete_c:
        {
            (void)Gap_StartAdvertising(App_AdvertisingCallback,
                                       App_ConnectionCallback);
        }
        break;

        case gAdvertisingSetupFailed_c:
        {
//        	(void)Gap_StopExtAdvertising(0xFF);
//        	BleApp_Start();
//        	TODO FFFF
//            panic(0,0,0,0);
        }
        break;

        case gExtAdvertisingParametersSetupComplete_c:
        {
            (void)Gap_SetExtAdvertisingData(mpExtAdvParams->handle,
                                      mpExtAdvParams->pGapAdvData,
                                      mpExtAdvParams->pScanResponseData);
        }
        break;

        case gExtAdvertisingDataSetupComplete_c:
        {
            (void)Gap_StartExtAdvertising(App_AdvertisingCallback,
                                    App_ConnectionCallback,
                                    mpExtAdvParams->handle,
                                    mpExtAdvParams->duration,
                                    mpExtAdvParams->maxExtAdvEvents);
        }
        break;

        case gExtAdvertisingSetRemoveComplete_c:
        {
            /* TBD */
        }
        break;

        default:
        {
            ; /* MISRA rule 16.4 */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */
void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent)
{
    switch (pAdvertisingEvent->eventType)
    {
        case gExtAdvertisingStateChanged_c:
        {
			appEvent_t   appEvent;
			Set_bit_bleAdvState(!Get_bit_bleAdvState());
            if (gCurrentAdvHandle == (uint8_t)gLegacyAdvSetHandle_c)
            {
                /* Inform the user interface handler that legacy advertising started */
                appEvent = mAppEvt_AdvertisingStartedLegacy_c;

                if (FALSE == mOwnerPairingMode)
                {
                    gCurrentAdvHandle = gExtendedAdvSetHandle_c;
                    BleAppIf_StartAdv();
                }

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
                /* Start advertising timer */
                (void)TM_InstallCallback((timer_handle_t)mAppTimerId, AdvertisingTimerCallback, NULL);
                (void)TM_Start((timer_handle_t)mAppTimerId, (uint8_t)kTimerModeSetSecondTimer | (uint8_t)kTimerModeLowPowerTimer, TmSecondsToMilliseconds(gAdvTime_c));
                Led1On();
#endif /* #if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) */
            }
            else if (gCurrentAdvHandle == (uint8_t)gExtendedAdvSetHandle_c)
            {
                /* Inform the user interface handler that extended advertising started */
                appEvent = mAppEvt_AdvertisingStartedExtendedLR_c;
            }
            else
            {
                /* Inform the user interface handler that advertising has stopped */
                appEvent = mAppEvt_AdvertisingStopped_c;

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
				timer_status_t status = TM_Stop((timer_handle_t)mAppTimerId);
                if (status != kStatus_TimerSuccess)
                {
                    panic(0, (uint32_t)BleApp_AdvertisingCallback, 0, 0);
                }
                Led1Off();
#else
                if (0U == BleApp_GetNoOfActiveConnections())
                {
                	/* Inform the user the number of active connections. */
                }

#endif /* #if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode) */
            }

            if(mpfBleUserInterfaceEventHandler != NULL)
            {
                 appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t));
                 if(pEventData != NULL)
                 {
                     pEventData->appEvent = appEvent;
                     if (gBleSuccess_c != App_PostCallbackMessage(mpfBleUserInterfaceEventHandler, pEventData))
                     {
                        (void)MEM_BufferFree(pEventData);
                     }
                 }
            }
        }
        break;

        case gAdvertisingCommandFailed_c:
        {
            /* Panic UI */
//        	TODO
//            panic(0,0,0,0);
        }
        break;

        case gAdvertisingSetTerminated_c:
        {

        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles BLE Connection callback from host stack.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    pConnectionEvent    Pointer to gapConnectionEvent_t.
********************************************************************************** */
void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    if (pConnectionEvent->eventType == gConnEvtConnected_c)
    {
        if (pConnectionEvent->eventData.connectedEvent.connectionRole == gBleLlConnectionCentral_c)
        {
            maPeerInformation[peerDeviceId].gapRole = gGapCentral_c;
            shell_write("\r\nmaPeerInformation: ---gGapCentral_c---.\r\n");
        }
        else
        {
            maPeerInformation[peerDeviceId].gapRole = gGapPeripheral_c;
            shell_write("\r\nmaPeerInformation: ---gGapPeripheral_c---.\r\n");
        }
    }

    /* Connection Manager to handle Host Stack interactions */
    if (maPeerInformation[peerDeviceId].gapRole == gGapCentral_c)
    {
    	/* Handle pairing process */
        BleConnManager_GapCentralEvent(peerDeviceId, pConnectionEvent);
    }
    else
    {
    	/* Handle pairing process */
        BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);
    }

    switch (pConnectionEvent->eventType)
    {
#if defined(gHandoverDemo_d) && (gHandoverDemo_d)
        case gConnEvtHandoverConnected_c:
        case gHandoverDisconnected_c:
        {
            AppHandover_ConnectionCallback(peerDeviceId, pConnectionEvent);
        }
        break;
#endif /* defined(gHandoverDemo_d) && (gHandoverDemo_d) */
        case gConnEvtConnected_c:
        {
            /* Save address used during discovery if controller privacy was used. */
            if (pConnectionEvent->eventData.connectedEvent.localRpaUsed)
            {
                FLib_MemCpy(gaAppOwnDiscAddress, pConnectionEvent->eventData.connectedEvent.localRpa, gcBleDeviceAddressSize_c);
            }

            /* Advertising stops when connected - on all PHYs */
            gCurrentAdvHandle = (uint8_t)gNoAdvSetHandle_c;
            (void)Gap_StopExtAdvertising(0xFF);

            maPeerInformation[peerDeviceId].deviceId = peerDeviceId;
            (void)Gap_CheckIfBonded(peerDeviceId, &maPeerInformation[peerDeviceId].isBonded, NULL);

#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
            (void)Gap_StopScanning();

            if (pConnectionEvent->eventData.connectedEvent.connectionRole == gBleLlConnectionCentral_c)
            {
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)

                if (maPeerInformation[peerDeviceId].isBonded == TRUE)
                {
                    mRestoringBondedLink = TRUE;
                    /* Restored custom connection information. Encrypt link */
                    (void)Gap_EncryptLink(peerDeviceId);
                }
#endif  /* defined(gAppUseBonding_d) && (gAppUseBonding_d) */
            }
#endif /* (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U)) */

            BleApp_StateMachineHandler(maPeerInformation[peerDeviceId].deviceId, mAppEvt_PeerConnected_c);

        }
        break;

        case gConnEvtDisconnected_c:
        {
            BleApp_StateMachineHandler(peerDeviceId, mAppEvt_PeerDisconnected_c);
        }
        break;

        case gConnEvtRssiRead_c:
        {

        }
        break;

#if gAppUsePairing_d
        case gConnEvtPairingRequest_c:
        {
            BleApp_StateMachineHandler(maPeerInformation[peerDeviceId].deviceId, mAppEvt_PairingReqRcv_c);
        }
        break;

        case gConnEvtLeScOobDataRequest_c:
        {
            BleApp_StateMachineHandler(peerDeviceId, mAppEvt_PairingPeerOobDataReq_c);
        }
        break;

        case gConnEvtPairingComplete_c:
        {
            /* Notify state machine handler on pairing complete */
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                BleApp_StateMachineHandler(maPeerInformation[peerDeviceId].deviceId, mAppEvt_PairingComplete_c);
            }
        }
        break;

#if defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U)
        case gConnEvtEncryptionChanged_c:
        {
#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
            if( mRestoringBondedLink )
            {
                mRestoringBondedLink = FALSE;
            }
#endif
            BleApp_StateMachineHandler(peerDeviceId, mAppEvt_EncryptionChanged_c);
        }
        break;

#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
        case gConnEvtAuthenticationRejected_c:
        {
            /* Start Pairing Procedure */
            (void)Gap_Pair(peerDeviceId, &gKeyFobPairingParameters);
        }
        break;
#endif /* (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U)) */
#endif /* gAppUseBonding_d */
#endif /* gAppUsePairing_d */

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
/*! *********************************************************************************
* \brief        Handles BLE Scanning callback from host stack.
*
* \param[in]    pScanningEvent    Pointer to gapScanningEvent_t.
********************************************************************************** */
void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
    switch (pScanningEvent->eventType)
    {
    	case gDeviceScanned_c:
    		/* If find the device, connect it. */
    		if(CheckScanEventLegacy(&(pScanningEvent->eventData.scannedDevice)))
    		{
    			if(pScanningEvent->eventData.scannedDevice.advEventType != gBleAdvRepAdvScanInd_c)
    			{
					dbg_Printf("-------------device found-----------");
					dbg_Printf("RSSI is: %d", abs(pScanningEvent->eventData.scannedDevice.rssi));
					mFoundPeripheralToConnect = TRUE;
					/* Set connection parameters and stop scanning. Connect on gScanStateChanged_c. */
					gConnReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;
					FLib_MemCpy(gConnReqParams.peerAddress,
								pScanningEvent->eventData.scannedDevice.aAddress,
								sizeof(bleDeviceAddress_t));

					(void)Gap_StopScanning();
	#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d)
					gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.scannedDevice.advertisingAddressResolved;
	#endif
    			}
    		}

    		/* fall through */
    		break;
        case gExtDeviceScanned_c:
        {
//            /* For now just connect to anyone. Non-CCC Key FOB data is not defined */
//            /* Set connection parameters and stop scanning. Connect on gScanStateChanged_c. */
//            gConnReqParams.peerAddressType = pScanningEvent->eventData.extScannedDevice.addressType;
//            FLib_MemCpy(gConnReqParams.peerAddress,
//                        pScanningEvent->eventData.extScannedDevice.aAddress,
//                        sizeof(bleDeviceAddress_t));
//
//            mFoundPeripheralToConnect = TRUE;
//            (void)Gap_StopScanning();
//#if defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d)
//            gConnReqParams.usePeerIdentityAddress = pScanningEvent->eventData.extScannedDevice.advertisingAddressResolved;
//#endif
        }
        break;

        case gScanStateChanged_c:
        {
//            mScanningOn = !mScanningOn;
        	Set_bit_bleScanState(!Get_bit_bleScanState());

            /* Node starts scanning */
            if (Get_bit_bleScanState())
            {
                /* Inform the user interface handler that the Bluetooth scan has started */
                if(mpfBleUserInterfaceEventHandler != NULL)
                {
                     appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t));
                     if(pEventData != NULL)
                     {
                         pEventData->appEvent = mAppEvt_BleScanning_c;
                         if (gBleSuccess_c != App_PostCallbackMessage(mpfBleUserInterfaceEventHandler, pEventData))
                         {
                            (void)MEM_BufferFree(pEventData);
                         }
                     }
                }

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode)
                Led1On();
#endif
            }
            /* Node is not scanning */
            else
            {
                /* Inform the user interface handler that the Bluetooth scan has stopped */
                if(mpfBleUserInterfaceEventHandler != NULL)
                {
                     appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t));
                     if(pEventData != NULL)
                     {
                         pEventData->appEvent = mAppEvt_BleScanStopped_c;
                         if (gBleSuccess_c != App_PostCallbackMessage(mpfBleUserInterfaceEventHandler, pEventData))
                         {
                            (void)MEM_BufferFree(pEventData);
                         }
                     }
                }

                if (TRUE == mFoundPeripheralToConnect)
                {
                    /* Inform the user interface handler that a device to connect to has been found */
                    if(mpfBleUserInterfaceEventHandler != NULL)
                    {
                         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t));
                         if(pEventData != NULL)
                         {
                             pEventData->appEvent = mAppEvt_BleConnectingToDevice_c;
                             if (gBleSuccess_c != App_PostCallbackMessage(mpfBleUserInterfaceEventHandler, pEventData))
                             {
                                (void)MEM_BufferFree(pEventData);
                             }
                         }
                    }
                    (void)BluetoothLEHost_Connect(&gConnReqParams, BleApp_ConnectionCallback);
                }
            }
        }
        break;

        case gScanCommandFailed_c:
        {
            ; /* No action required */
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}
#endif /* (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U)) */


/*! *********************************************************************************
 * \brief        Returns UWB clock.
 ********************************************************************************** */
static uint64_t GetUwbClock(void)
{
    uint32_t randomNo;
    RNG_GetRandomNo (&randomNo);
    /* Get a simulated UWB clock */
    return TM_GetTimestamp() + (uint8_t)randomNo;
}

/*! *********************************************************************************
* \brief        Process scanning events to search for the DK Ranging Service.
*               This function is called from the scanning callback.
*
* \param[in]    pData                   Pointer to gapScannedDevice_t.
*
* \return       TRUE if the scanned device implements the DK Ranging Service,
                FALSE otherwise
********************************************************************************** */
static uint8_t bleAddress[6] = {0x54, 0xD9, 0xBE, 0xAC, 0xA8, 0x57};
static bool_t CheckScanEventLegacy(gapScannedDevice_t* pData)
{
    uint32_t index = 0;
    bool_t foundMatch = TRUE;
//    static uint8_t uuid_dk_service[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
//    static uint8_t uuid_dk_service[8] = {'H', 'Z', '_', 'M', 'f', '9', '8', 'd'};

	while (index < pData->dataLength)
	{
		gapAdStructure_t adElement;

		adElement.length = pData->data[index];
		adElement.adType = (gapAdType_t)pData->data[index + 1U];
		adElement.aData = &pData->data[index + 2U];

		if((adElement.length == 0x11) && (adElement.adType == 0x07))
		{
			uint8_t tempData[20] = {0};
			uint8_t retData[32] = {0};

			/* 1.CONCATENATE vehicle id and the last random num */
			FLib_MemCpy(tempData, keyFobData.vehicle_id, 16);
			FLib_MemCpy(&tempData[16], &(adElement.aData[12]), 4);

			/* 2.Calculate the concatenate string, get the hash value */
			SHA256_Hash(tempData, 20, retData);

			/* 3.Compare the hash value*/
			foundMatch = BluetoothLEHost_MatchDataInAdvElementList(&adElement, &(retData[20]), 12);
			break;
		}
		else
		{
			foundMatch = FALSE;
		}


		/* Move on to the next AD element type */
		index += (uint32_t)adElement.length + sizeof(uint8_t);
	}

	if (foundMatch)
	{
		//TODO
//		uuid_dk_service[0] = 'B';
		shell_write("\r\nLegacy ADV: ");
	}
    return foundMatch;
}

/*! *********************************************************************************
* \brief        Handler of gControllerNotificationEvent_c from BleApp_GenericCallback.
*
* \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
********************************************************************************** */
static void BleApp_GenericCallback_ControllerNotificationEvent(gapGenericEvent_t* pGenericEvent)
{
    if ((pGenericEvent->eventData.notifEvent.eventType & ((uint16_t)gNotifConnCreated_c | (uint16_t)gNotifPhyUpdateInd_c)) > 0U )
    {
        uint64_t bleTime = TM_GetTimestamp();
        /* Get current timestamp */
        mTsUwbDeviceTime = GetUwbClock();
        /* Subtract event delay */
        if (bleTime >= pGenericEvent->eventData.notifEvent.timestamp)
        {
            mTsUwbDeviceTime = mTsUwbDeviceTime - ((uint64_t)bleTime - (uint64_t)pGenericEvent->eventData.notifEvent.timestamp);
        }
        else
        {
            mTsUwbDeviceTime = mTsUwbDeviceTime - ((0x00000000FFFFFFFFU - (uint64_t)pGenericEvent->eventData.notifEvent.timestamp) + bleTime);
        }
    }
}

/*! *********************************************************************************
* \brief        Handler of gHostPrivacyStateChanged_c and
*               gControllerPrivacyStateChanged_c from BleApp_GenericCallback.
*
* \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
********************************************************************************** */
static void BleApp_GenericCallback_HandlePrivacyEvents(gapGenericEvent_t* pGenericEvent)
{
    switch (pGenericEvent->eventType)
    {
        case gHostPrivacyStateChanged_c:
        {
            if (!pGenericEvent->eventData.newHostPrivacyState)
            {
                if(gPrivacyStateChangedByUser)
                {
                    gPrivacyStateChangedByUser = FALSE;
                    /* Host privacy disabled because a bond was removed
                       or added. Enable privacy. */
                    (void)BleConnManager_EnablePrivacy();
                }
            }
        }
        break;

        case gControllerPrivacyStateChanged_c:
        {
            if (!pGenericEvent->eventData.newControllerPrivacyState)
            {
                if(gPrivacyStateChangedByUser)
                {
                    gPrivacyStateChangedByUser = FALSE;
                    /* Controller privacy disabled because a bond was removed
                       or added. Enable privacy. */
                    (void)BleConnManager_EnablePrivacy();
                }
            }
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
 * \brief        Returns number of active connections.
 ********************************************************************************** */
static uint8_t BleApp_GetNoOfActiveConnections(void)
{
    uint8_t activeConnections = 0;
    uint8_t mPeerId = 0;

    for (mPeerId = 0; mPeerId < (uint8_t)gAppMaxConnections_c; mPeerId++)
    {
        if (maPeerInformation[mPeerId].deviceId != gInvalidDeviceId_c)
        {
            ++activeConnections;
        }
    }

    return activeConnections;
}
