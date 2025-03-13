/*
 * ble_DV_signals.c
 *
 *  Created on: 2024年6月25日
 *      Author: F704012276_
 */

#include "stdint.h"

uint8_t emcBleSlaveRecvFlag 				= 0;
uint8_t BLE_ErrInfo 					= 0;
uint8_t BLE_State 						= 0;
uint8_t BLE_DataErrCnt 					= 0;
uint8_t BLE_RecvErrCnt 					= 0;

uint8_t BLE_DV_Master_State				= 0;
uint8_t BLE_DV_ConnectingSts			= 0;

uint8_t BLE_AddressIndex;
