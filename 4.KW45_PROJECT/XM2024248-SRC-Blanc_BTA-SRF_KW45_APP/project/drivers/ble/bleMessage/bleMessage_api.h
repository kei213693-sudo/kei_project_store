/*
 * bleMessage_api.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLEMESSAGE_API_H_
#define DRV_BLE_BLEMESSAGE_API_H_

#include "signals_api.h"

bleResult_t App_PostCallbackMessage(appCallbackHandler_t handler,appCallbackParam_t param);

bleResult_t BleApp_EventCallback(appCallbackHandler_t handler,appCallbackParam_t param);
bleResult_t BleApp_MessageCallback(appCallbackHandler_t handler,appCallbackParam_t param);

#endif /* DRV_BLE_BLEMESSAGE_API_H_ */
