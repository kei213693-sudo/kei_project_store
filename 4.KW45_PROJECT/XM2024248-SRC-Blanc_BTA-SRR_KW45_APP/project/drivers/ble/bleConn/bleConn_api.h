/*
 * bleConn_api.h
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLECONN_BLECONN_API_H_
#define DRV_BLE_BLECONN_BLECONN_API_H_

#include "bleConn_cfg.h"

bleResult_t BluetoothLEHost_Connect
(
    gapConnectionRequestParameters_t*   pParameters,
    gapConnectionCallback_t             connCallback
);

void App_ConnectionCallback
(
    deviceId_t            peerDeviceId,
    gapConnectionEvent_t* pConnectionEvent
);

#endif /* DRV_BLE_BLECONN_BLECONN_API_H_ */
