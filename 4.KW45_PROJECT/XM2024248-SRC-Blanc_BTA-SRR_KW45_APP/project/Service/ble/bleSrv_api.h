/*
 * ble_api.h
 *
 *  Created on: 2024年7月16日
 *      Author: F704012276_
 */

#ifndef BLE_BLESRV_API_H_
#define BLE_BLESRV_API_H_

#include "bleIF_api.h"
#include "bleCom_api.h"
#include "bleCCC_api.h"
#include "bleSM_api.h"
#include "bleApp1_api.h"
#include "blePduR_api.h"
#include "bleManager_api.h"
#include "ble_conn_manager.h"
#include "ble_low_power_api.h"
#include "ble_service_discovery.h"

void Ble_Init(void);

#endif /* BLE_BLESRV_API_H_ */
