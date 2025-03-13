/*
 * bleApp2_api.h
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#ifndef APP_BLECCC_BLECCC_API_H_
#define APP_BLECCC_BLECCC_API_H_

#include "bleCCC_cfg.h"

bleResult_t CCC_FirstApproachRsp(deviceId_t deviceId, uint8_t* pBdAddr, gapLeScOobData_t* pOobData);
bleResult_t CCCPhase2_SendSPAKERequest(deviceId_t deviceId, uint8_t *pData, uint16_t dataLen);
bleResult_t CCCPhase2_SendSPAKEVerify(deviceId_t deviceId, uint8_t *pData, uint16_t dataLen);
bleResult_t CCC_SendSubEvent(deviceId_t deviceId, dkSubEventCategory_t category, dkSubEventCommandCompleteType_t type);

#endif /* APP_BLECCC_BLECCC_API_H_ */
