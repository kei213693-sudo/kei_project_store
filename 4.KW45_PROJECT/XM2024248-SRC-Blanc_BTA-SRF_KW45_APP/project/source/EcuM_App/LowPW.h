/*
 * LowPW.h
 *
 *  Created on: 2024年7月27日
 *      Author: F704011769_
 */

#ifndef ECUM_APP_LOWPW_H_
#define ECUM_APP_LOWPW_H_
#include "stdint.h"

void MCU_lowPW(void);
uint8_t PreSleep_Judge();
void Sleep_IOC(void);
void Wakeup_Init(void);
void Clear_Isr(void);
void Enable_Isr(void);
uint8_t UWBSleep_timeout_Judge();
void NM_SleepPre_Judge(void);
#endif /* ECUM_APP_LOWPW_H_ */
