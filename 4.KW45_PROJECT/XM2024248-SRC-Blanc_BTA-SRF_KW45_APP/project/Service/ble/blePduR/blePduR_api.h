/*
 * blePduR_api.h
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#ifndef SRV_BLEPDUR_BLEPDUR_API_H_
#define SRV_BLEPDUR_BLEPDUR_API_H_

#include "blePduR_cfg.h"

void BlePduR_Transmit(bleMsgInfo_t *bleMsgInfo);
void BlePduR_CanIfRxIndication(bleMsgInfo_t *bleMsgInfo);
void BlePduR_CanIfRxIndication(bleMsgInfo_t *bleMsgInfo);

#endif /* SRV_BLEPDUR_BLEPDUR_API_H_ */
