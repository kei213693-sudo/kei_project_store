/*
 * bleInstallCallback_api.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLEINSTALLCALLBACK_API_H_
#define DRV_BLE_BLEINSTALLCALLBACK_API_H_

#include "signals_api.h"
#include "drv_api.h"

void BleApp_RegisterAppMessageHandler(pfBleCallback_t pfAppInterfaceEventHandler);
void BleApp_RegisterAppEventHandler(pfBleCallback_t pfAppInterfaceEventHandler);
void BleApp_RegisterUserInterfaceEventHandler(pfBleCallback_t pfUserInterfaceEventHandler);
void BleApp_RegisterEventHandler(pfBleCallback_t pfBleEventHandler);
void BluetoothLEHost_SetGenericCallback(gapGenericCallback_t pfGenericCallback);


#endif /* DRV_BLE_BLEINSTALLCALLBACK_API_H_ */
