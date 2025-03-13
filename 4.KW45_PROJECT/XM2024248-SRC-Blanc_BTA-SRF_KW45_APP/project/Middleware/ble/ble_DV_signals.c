/*
 * ble_DV_signals.c
 *
 *  Created on: 2024年6月25日
 *      Author: F704012276_
 */

#include "stdint.h"
#include "string.h"

uint8_t emcBleSlaveRecvFlag 				= 0;
uint8_t BLE_ErrInfo 					= 0;
uint8_t BLE_State 						= 0;
uint8_t BLE_DataErrCnt 					= 0;
uint8_t BLE_RecvErrCnt 					= 0;
uint8_t BLE_SleepFlag					= 0;

uint8_t BLE_DV_Master_State				= 0;

uint8_t Did4A56_Value					= 0xff;
uint8_t ble_rssi_value					= 0;

uint8_t BLE_ADV_Cmd						= 0;
uint8_t bleInitFailureFlag				= 0;
uint8_t NBUInitFailureFlag				= 0;

static uint8_t blePublicAddress[6] = {0};

void Set_BlePublicAddress(uint8_t* address)
{
	memcpy(blePublicAddress, address, 6);
}
void Get_BlePublicAddress(uint8_t* address)
{
	memcpy(address, blePublicAddress, 6);
//	address = blePublicAddress;
}
