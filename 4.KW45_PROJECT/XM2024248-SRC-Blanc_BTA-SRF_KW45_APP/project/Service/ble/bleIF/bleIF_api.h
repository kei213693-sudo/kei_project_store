/*
 * bleIF_api.h
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#ifndef SRV_BLEIF_BLEIF_API_H_
#define SRV_BLEIF_BLEIF_API_H_

#include "signals_api.h"

void BleIf_Receive(bleMsgRevInfo_t *bleMsgInfo);

void BleIf_Transmit
(
	uint8_t deviceId,
	uint8_t len,
	uint16_t serviceHandle,
	const uint8_t* data
);

#endif /* SRV_BLEIF_BLEIF_API_H_ */
