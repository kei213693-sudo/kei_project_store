/*
 * bleApp1_api.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef APP_BLEAPP1_BLEAPP1_API_H_
#define APP_BLEAPP1_BLEAPP1_API_H_

#include "bleAppEvent.h"
#include "bleAppUser.h"

void APP_APPModeControl(void);
void APP_Ble10ms_Proc(void);
void APP_AppMessageHandler(void *pData);
void APP_AppEventHandler(void *pData);
void APP_UserInterfaceEventHandler(void *pData);
void APP_BleEventHandler(void *pData);
void APP_GapStopConnection(void);
void APP_CleanCycleCntFunc(void);

#endif /* APP_BLEAPP1_BLEAPP1_API_H_ */
