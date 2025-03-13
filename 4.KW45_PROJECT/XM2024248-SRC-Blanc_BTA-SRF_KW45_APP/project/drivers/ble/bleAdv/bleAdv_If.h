/*
 * bleAdv_If.h
 *
 *  Created on: 2024年7月4日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLEADV_BLEADV_IF_H_
#define DRV_BLE_BLEADV_BLEADV_IF_H_

void BleAppIf_Start(void);
void BleAppIf_StartScan(void);
void BleAppIf_StartAdv(void);
void BleAppIf_NioAdvertise(void);
void BleAppIf_BeaconAdvertise(void);

#endif /* DRV_BLE_BLEADV_BLEADV_IF_H_ */
