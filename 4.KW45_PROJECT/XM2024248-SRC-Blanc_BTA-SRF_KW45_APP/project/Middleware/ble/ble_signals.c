/*
 * ble_signals.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "ble_signals_cfg.h"

/*Variable */
bleMsgInfo_t 						bleMsgCanBuff;
bleMsgInfo_t 						bleMsgAirBuff;
appPeerInfo_t 						maPeerInformation[gAppMaxConnections_c];
uint8_t 							gaAppOwnDiscAddress[gcBleDeviceAddressSize_c];
uint64_t 							mTsUwbDeviceTime;/* Time Sync UWB Device Time. Demo value. */
bool_t 								gPrivacyStateChangedByUser = FALSE;/* This global will be TRUE if the user adds or removes a bond */
bool_t 								mScanningOn = FALSE;
bool_t								mAdvingOn = FALSE;
bool_t 								mL2caTimerValid = FALSE;
uint8_t 							gCurrentAdvHandle = (uint8_t)gLegacyAdvSetHandle_c;
/*bool_t      						keyValidClientList[gAppMaxConnections_c] = { TRUE, FALSE, FALSE, FALSE, FALSE };
testConfig_t						testDataServiceConfig = {(uint16_t)SK_BLE_GATT,
															{4, "AAAA"},
															{0},
															keyValidClientList,
															gAppMaxConnections_c};*/
deviceId_t 							mCurrentPeerId = gInvalidDeviceId_c;
appAdvertisingParams_t 				*mpAdvParams = NULL;
appAdvertisingParams_t 				*iBeaconAdvParams = NULL;
appExtAdvertisingParams_t 			*mpExtAdvParams = NULL;
uint32_t 							globalTicks	= 0;
uint8_t 							bleStackConfigDoneFlag = 0;
uint8_t								validConnectedKeyNum = 0;
uint8_t								validConnectedKeyFobNum = 0;
bleStateStruct_t					bleStateStruct = {
	.validConnectedKeyFobNum = 0,
	.validConnectedKeyNum = 0,
	.bleAdvState = 0,
	.bleScanState = 0,
	.bleConfigDoneFlag = 0,
	.bleReserved = 0,
};


/* Timer */
TIMER_MANAGER_HANDLE_DEFINE(mL2caTimerId);

/* message */
messaging_t 						mAppCbInputQueue;
messaging_t 						mHostAppInputQueue;
messaging_t 						mAppEventQueue;
messaging_t 						mAppMessageQueue;

/* event */
OSA_EVENT_HANDLE_DEFINE(mAppEvent);
OSA_EVENT_HANDLE_DEFINE(mBleAppEvent);

/*Functions*/
pfBleCallback_t 					mpfBleEventHandler = NULL;
pfBleCallback_t 					mpfBleUserInterfaceEventHandler = NULL;
pfBleCallback_t 					mpfBleAppEventHandler = NULL;
pfBleCallback_t 					mpfBleAppMessageEventHandler = NULL;


appBluetoothLEInitCompleteCallback_t mpfInitDoneCallback       = NULL;
gapGenericCallback_t                 mpfAppGenericCallback     = NULL;
gapGenericCallback_t                 mpfGenericHandler         = NULL;
gattServerCallback_t                 pfGattServerCallback      = NULL;
gattClientProcedureCallback_t        pfGattClientProcCallback  = NULL;
gattClientNotificationCallback_t     pfGattClientNotifCallback = NULL;
gattClientNotificationCallback_t     pfGattClientIndCallback   = NULL;
l2caLeCbDataCallback_t               pfL2caLeCbDataCallback    = NULL;
l2caLeCbControlCallback_t            pfL2caLeCbControlCallback = NULL;

appAdvertiserHandler_t   			pfAdvertiserHandler = NULL;

gapAdvertisingCallback_t 			pfAdvCallback       = NULL;     /* Set only if advertising is used */
gapScanningCallback_t    			pfScanCallback      = NULL;     /* Set only if scanning is used */
gapConnectionCallback_t  			pfConnCallback      = NULL;     /* Set only if connection is used */
