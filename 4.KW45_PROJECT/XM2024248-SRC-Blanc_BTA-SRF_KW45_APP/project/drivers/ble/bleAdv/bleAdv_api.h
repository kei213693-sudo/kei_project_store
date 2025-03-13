/*
 * bleAdv_api.h
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLEADV_BLEADV_API_H_
#define DRV_BLE_BLEADV_BLEADV_API_H_

#include "bleAdv_cfg.h"
#include "bleAdv_If.h"

void App_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);

bleResult_t BluetoothLEHost_StartAdvertising(
    appAdvertisingParams_t *pAdvParams,
    gapAdvertisingCallback_t pfAdvertisingCallback,
    gapConnectionCallback_t pfConnectionCallback
);

bleResult_t BluetoothLEHost_StartExtAdvertising(
    appExtAdvertisingParams_t *pExtAdvParams,
    gapAdvertisingCallback_t pfAdvertisingCallback,
    gapConnectionCallback_t pfConnectionCallback
);

#endif /* DRV_BLE_BLEADV_BLEADV_API_H_ */
