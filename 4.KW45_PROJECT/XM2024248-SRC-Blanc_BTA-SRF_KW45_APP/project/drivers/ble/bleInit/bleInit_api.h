/*
 * bleInit_api.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLEINIT_API_H_
#define DRV_BLE_BLEINIT_API_H_

#include "signals_api.h"

void APP_InitServices(void);
int APP_InitBle(void);
void BluetoothLEHost_AppInit(void);
void BluetoothLEHost_Init(appBluetoothLEInitCompleteCallback_t pCallback);
bleResult_t Ble_DeInitialize(void);

bleResult_t Ble_Initialize(gapGenericCallback_t gapGenericCallback);

#endif /* DRV_BLE_BLEINIT_API_H_ */
