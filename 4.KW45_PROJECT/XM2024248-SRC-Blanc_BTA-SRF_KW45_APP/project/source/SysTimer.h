/*
 * SysTimer.h
 *
 *  Created on: 2024年10月10日
 *      Author: ZXE
 */

#ifndef SYSTIMER_H_
#define SYSTIMER_H_

#define SYS_TIMER_COUNT_MAX_VAULE   0xFFFFFFFDU
#define SYS_TIMER_ALWAYS_TIMEOUT	0xFFFFFFFEU
#define SYS_TIMER_NEVER_TIMEOUT		0xFFFFFFFFU
#define SYS_TIMER_MAX_DELAY_MS 		259200000UL /* 72h */

void SysTimerIncrementTick(void);
uint32_t SysTimerGetCurrentTime(void);
uint32_t SysTimerDelayTime(uint32_t TimerPrevPoint);
uint32_t SysTimerCreate(uint32_t t);
bool SysTimerIsExpired(uint32_t TimerExpiredPoint);

#endif /* SYSTIMER_H_ */
