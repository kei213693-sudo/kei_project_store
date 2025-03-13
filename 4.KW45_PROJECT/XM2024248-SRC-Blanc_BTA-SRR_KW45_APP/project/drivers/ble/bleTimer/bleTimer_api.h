/*
 * bleTimer.h
 *
 *  Created on: 2024年7月12日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLETIMER_BLETIMER_API_H_
#define DRV_BLE_BLETIMER_BLETIMER_API_H_

#include "bleTimer_cfg.h"

void BleTimer_Init(void);
bleAppTimer_t* Get_BleTimerInfo(void);
void BleAppTimerCallback(void);
void Start_BleTimer(uint8_t device_id, uint8_t timerId);
void Clear_BleTimer(uint8_t device_id, uint8_t timerId);
void Reset_BleTimer(uint8_t device_id);
uint8_t Get_TpTimer_TimeoutFlag(uint8_t device_id, uint8_t timerId);
void Set_BleTimerTimeoutValue(uint8_t device_id, uint8_t timerId, uint32_t timeoutVal);

#endif /* DRV_BLE_BLETIMER_BLETIMER_API_H_ */
