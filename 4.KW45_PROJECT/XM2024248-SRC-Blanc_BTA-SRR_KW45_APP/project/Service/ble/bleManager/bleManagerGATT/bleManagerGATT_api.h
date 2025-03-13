/*
 * bleManagerGATT_api.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef SRV_BLEMANAGER_BLEMANAGERGATT_BLEMANAGERGATT_API_H_
#define SRV_BLEMANAGER_BLEMANAGERGATT_BLEMANAGERGATT_API_H_

#include "bleManagerGATT_cfg.h"
#include "ble_service_discovery.h"

void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent);
void BleApp_GattClientCallback(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
);
void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t* pEvent);
bleResult_t BleApp_ConfigureNotifications
(
    deviceId_t      peerDeviceId
);
void BleApp_GattNotificationCallback( deviceId_t serverDeviceId, uint16_t characteristicValueHandle, uint8_t* aValue, uint16_t valueLength );


#endif /* SRV_BLEMANAGER_BLEMANAGERGATT_BLEMANAGERGATT_API_H_ */
