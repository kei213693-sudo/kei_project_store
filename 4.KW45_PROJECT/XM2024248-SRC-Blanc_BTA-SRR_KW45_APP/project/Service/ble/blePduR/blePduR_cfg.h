/*
 * blePduR_cfg.h
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#ifndef SRV_BLEPDUR_BLEPDUR_CFG_H_
#define SRV_BLEPDUR_BLEPDUR_CFG_H_

#include "signals_api.h"
#include "drv_api.h"
#include "srv_api.h"

typedef struct airMsg_tag {
    dkMessageType_t   messageHeader;
    rangingMsgId_t    payloadHeader;
    uint16_t          payloadLength;
    uint8_t*          payloadData;
} airMsg_t;

#endif /* SRV_BLEPDUR_BLEPDUR_CFG_H_ */
