/*
 * bleAppInterface.h
 *
 *  Created on: 2024年7月11日
 *      Author: F704012276_
 */

#ifndef APP_BLEAPP1_BLEAPPUSER_H_
#define APP_BLEAPP1_BLEAPPUSER_H_

#include "signals_api.h"

void AppEvt_BleConfigDoneHandle(void);
void App_PeerConnectedHandle(appEventData_t* event);
void App_PeerDisconnectedHandle(appEventData_t* event);
void App_PairingCompleteHandle(appEventData_t* event);
void App_PairingReqRcvHandle(appEventData_t* event);
void App_BleConnectingHandle(appEventData_t* event);
void App_BleAuthingHandle(appEventData_t* event);
void App_BleAuthPassHandle(appEventData_t* event);
void App_BleAuthPreFailureHandle(appEventData_t* event);
void App_BleAuthFailHandle(appEventData_t* event);
void App_TimeOutHandle(appEventData_t* pEventData);

#endif /* APP_BLEAPP1_BLEAPPUSER_H_ */
