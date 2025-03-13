/*
 * ble_signals_api.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef SIGNALS_BLE_BLE_SIGNALS_API_H_
#define SIGNALS_BLE_BLE_SIGNALS_API_H_

#include "ble_signals_cfg.h"

/* variables */
extern bleMsgInfo_t 						bleMsgCanBuff;
extern bleMsgInfo_t 						bleMsgAirBuff;
extern appPeerInfo_t 						maPeerInformation[gAppMaxConnections_c];
#define PeerDevcieChannelEnable(id)			(maPeerInformation[id].bleChanflags)
#define Set_BleDeviceChannel(id, x)			(maPeerInformation[id].bleChanflags = x)
#define BLE_DEVICE_CHANNEL_ENABLE			1
#define BLE_DEVICE_CHANNEL_DISABLE			0

extern uint8_t 								gaAppOwnDiscAddress[gcBleDeviceAddressSize_c];
/* Time Sync UWB Device Time. Demo value. */
extern uint64_t                         	mTsUwbDeviceTime;
/* This global will be TRUE if the user adds or removes a bond */
extern bool_t                           	gPrivacyStateChangedByUser;
extern gapPairingParameters_t 				gKeyFobPairingParameters;
extern gapPairingParameters_t 				gPhonePairingParameters;
extern bool_t 								mScanningOn;
extern bool_t								mAdvingOn;
extern bool_t 								mL2caTimerValid;
extern uint8_t 								gCurrentAdvHandle;
/*extern testConfig_t							testDataServiceConfig;
extern bool_t 								keyValidClientList[gAppMaxConnections_c];*/
extern deviceId_t 							mCurrentPeerId;
extern appAdvertisingParams_t 				*mpAdvParams;
extern appAdvertisingParams_t 				*iBeaconAdvParams;
extern appExtAdvertisingParams_t 			*mpExtAdvParams;
extern uint32_t 							globalTicks;
extern uint8_t 								bleStackConfigDoneFlag;
extern uint8_t								validConnectedKeyNum;
extern uint8_t								validConnectedKeyFobNum;

extern bleStateStruct_t						bleStateStruct;
#define Get_bit_bleAdvState()				(bleStateStruct.bleAdvState)
#define Set_bit_bleAdvState(x)				(bleStateStruct.bleAdvState = x)

#define Get_bit_bleScanState()				(bleStateStruct.bleScanState)
#define Set_bit_bleScanState(x)				(bleStateStruct.bleScanState = x)

#define Get_bit_bleConfigDoneFlag()			(bleStateStruct.bleConfigDoneFlag)
#define Set_bit_bleConfigDoneFlag(x)		(bleStateStruct.bleConfigDoneFlag = x)

#define Get_validConnectedKeyNum()			(bleStateStruct.validConnectedKeyNum)
#define Set_validConnectedKeyNum(x)			(bleStateStruct.validConnectedKeyNum = x)
#define Get_validConnectedKeyFobNum()		(bleStateStruct.validConnectedKeyFobNum)
#define Set_validConnectedKeyFobNum(x)		(bleStateStruct.validConnectedKeyFobNum = x)

/* Timer */
extern TIMER_MANAGER_HANDLE_DEFINE(mL2caTimerId);

/* message */
extern messaging_t mAppCbInputQueue;
extern messaging_t mHostAppInputQueue;
extern messaging_t mAppEventQueue;
extern messaging_t mAppMessageQueue;

/* event */
extern OSA_EVENT_HANDLE_DEFINE(mAppEvent);
extern OSA_EVENT_HANDLE_DEFINE(mBleAppEvent);

/* functions */
extern pfBleCallback_t mpfBleEventHandler;
extern pfBleCallback_t mpfBleUserInterfaceEventHandler;
extern pfBleCallback_t mpfBleAppEventHandler;
extern pfBleCallback_t mpfBleAppMessageEventHandler;

extern appBluetoothLEInitCompleteCallback_t mpfInitDoneCallback;
extern gapGenericCallback_t                 mpfAppGenericCallback;
extern gapGenericCallback_t                 mpfGenericHandler;
extern gattServerCallback_t                 pfGattServerCallback;
extern gattClientProcedureCallback_t        pfGattClientProcCallback;
extern gattClientNotificationCallback_t     pfGattClientNotifCallback;
extern gattClientNotificationCallback_t     pfGattClientIndCallback;
extern l2caLeCbDataCallback_t               pfL2caLeCbDataCallback;
extern l2caLeCbControlCallback_t            pfL2caLeCbControlCallback;

extern appAdvertiserHandler_t   pfAdvertiserHandler;

extern gapAdvertisingCallback_t pfAdvCallback;     /* Set only if advertising is used */
extern gapScanningCallback_t    pfScanCallback;     /* Set only if scanning is used */
extern gapConnectionCallback_t  pfConnCallback;     /* Set only if connection is used */

#endif /* SIGNALS_BLE_BLE_SIGNALS_API_H_ */
