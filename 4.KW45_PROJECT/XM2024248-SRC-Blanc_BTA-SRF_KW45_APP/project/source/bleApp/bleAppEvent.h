/*
 * bleAppEvent.h
 *
 *  Created on: 2024年7月11日
 *      Author: F704012276_
 */

#ifndef APP_BLEAPP1_BLEAPPEVENT_H_
#define APP_BLEAPP1_BLEAPPEVENT_H_

#include "signals_api.h"

void App_HandleLeScLocalOobDataCallback(appEventData_t *pEventData);
void App_HandleL2capPsmDataCallback(appEventData_t *pEventData);

#if (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U))
void A2A_HandleGenericCallbackBondCreatedEvent(appEventData_t *pEventData);
#else
void App_HandleGenericCallbackBondCreatedEvent(appEventData_t *pEventData);
#endif

#endif /* APP_BLEAPP1_BLEAPPEVENT_H_ */
