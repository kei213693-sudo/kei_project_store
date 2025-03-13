/*
 * bleManagerL2CA_api.h
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#ifndef SRV_BLEMANAGER_BLEMANAGERL2CA_BLEMANAGERL2CA_API_H_
#define SRV_BLEMANAGER_BLEMANAGERL2CA_BLEMANAGERL2CA_API_H_

#include "bleManagerL2CA_cfg.h"

void BleApp_L2capPsmDataCallback (deviceId_t deviceId, uint16_t lePsm, uint8_t* pPacket, uint16_t packetLength);
void BleApp_L2capPsmControlCallback (l2capControlMessage_t* pMessage);

#endif /* SRV_BLEMANAGER_BLEMANAGERL2CA_BLEMANAGERL2CA_API_H_ */
