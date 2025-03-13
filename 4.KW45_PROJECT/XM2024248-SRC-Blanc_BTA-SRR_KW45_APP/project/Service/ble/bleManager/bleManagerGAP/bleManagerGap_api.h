/*
 * bleManagerGap_api.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef SRV_BLEMANAGER_BLEMANAGERGAP_BLEMANAGERGAP_API_H_
#define SRV_BLEMANAGER_BLEMANAGERGAP_BLEMANAGERGAP_API_H_

#include "signals_api.h"

void App_GenericHandler(gapGenericEvent_t *pGenericEvent);
void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent);
void BleApp_ConnectionCallback (deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
void BleApp_GenericCallback(gapGenericEvent_t* pGenericEvent);
void BluetoothLEHost_Initialized(void);
void App_AdvertiserHandler(gapGenericEventType_t  eventType);

#endif /* SRV_BLEMANAGER_BLEMANAGERGAP_BLEMANAGERGAP_API_H_ */
