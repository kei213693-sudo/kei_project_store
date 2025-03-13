/*
 * bleScan_api.h
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLESCAN_BLESCAN_API_H_
#define DRV_BLE_BLESCAN_BLESCAN_API_H_

#include "bleScan_cfg.h"

bleResult_t BluetoothLEHost_StartScanning
(
    appScanningParams_t   *pAppScanParams,
    gapScanningCallback_t pfCallback
);

bool_t BluetoothLEHost_MatchDataInAdvElementList
(
    gapAdStructure_t *pElement,
    void             *pData,
    uint8_t          iDataLen
);

#endif /* DRV_BLE_BLESCAN_BLESCAN_API_H_ */
