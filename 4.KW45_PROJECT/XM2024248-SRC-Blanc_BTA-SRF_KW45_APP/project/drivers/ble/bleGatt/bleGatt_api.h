/*
 * bleGatt_api.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef BLEDRV_BLEGATT_BLEGATT_API_H_
#define BLEDRV_BLEGATT_BLEGATT_API_H_

#include "bleGatt_cfg.h"

void App_GattServerCallback
(
    deviceId_t         peerDeviceId,
    gattServerEvent_t* pServerEvent
);
void App_GattClientProcedureCallback
(
    deviceId_t              deviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
);
void App_GattClientNotificationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
);
void App_GattClientIndicationCallback
(
    deviceId_t      deviceId,
    uint16_t        characteristicValueHandle,
    uint8_t*        aValue,
    uint16_t        valueLength
);

bleResult_t App_RegisterGattServerCallback
(
    gattServerCallback_t serverCallback
);

bleResult_t App_RegisterGattClientProcedureCallback
(
    gattClientProcedureCallback_t  callback
);

bleResult_t App_RegisterGattClientNotificationCallback
(
    gattClientNotificationCallback_t  callback
);

bleResult_t App_RegisterGattClientIndicationCallback
(
    gattClientIndicationCallback_t  callback
);

#endif /* BLEDRV_BLEGATT_BLEGATT_API_H_ */
