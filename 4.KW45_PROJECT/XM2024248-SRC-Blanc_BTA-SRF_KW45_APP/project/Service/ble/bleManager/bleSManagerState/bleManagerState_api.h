/*
 * bleManagerState_api.h
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#ifndef SRV_BLEMANAGER_BLESMANAGERSTATE_BLEMANAGERSTATE1_API_H_
#define SRV_BLEMANAGER_BLESMANAGERSTATE_BLEMANAGERSTATE_API_H_

#include "bleManagerState_cfg.h"

void BleApp_StateMachineHandler(deviceId_t peerDeviceId, appEvent_t event);

#endif /* SRV_BLEMANAGER_BLESMANAGERSTATE_BLEMANAGERSTATE1_API_H_ */
