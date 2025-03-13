/*
 * SysTimer.c
 *
 *  Created on: 2024年10月10日
 *      Author: ZXE
 */
#include "fsl_os_abstraction.h"
//#include "logger.h"
#include "SysTimer.h"

static uint32_t SysTimerTickCount = 0;
void SysTimerIncrementTick(void)
{
	SysTimerTickCount ++;
	if(SysTimerTickCount > SYS_TIMER_COUNT_MAX_VAULE)
	{
		SysTimerTickCount = 0;
	}
}

uint32_t SysTimerGetCurrentTime(void)
{
	return SysTimerTickCount;
}

uint32_t SysTimerDelayTime(uint32_t TimerPrevPoint)
{
	uint32_t time_u32;
	uint32_t TimerCurrentPoint = SysTimerTickCount;
	if(SYS_TIMER_ALWAYS_TIMEOUT == TimerPrevPoint) time_u32 = 0xFFFFFFFFU;
	else if(SYS_TIMER_NEVER_TIMEOUT == TimerPrevPoint) time_u32 = 0;
	else if(TimerCurrentPoint >= TimerPrevPoint)
	{
		time_u32 = TimerCurrentPoint - TimerPrevPoint;
	}
	else
	{
		time_u32 = SYS_TIMER_COUNT_MAX_VAULE - TimerPrevPoint + TimerCurrentPoint + 1;
	}
	return time_u32;
}

uint32_t SysTimerCreate(uint32_t t)
{
	uint32_t TimerCurrentPoint, TimerExpiredPoint;
	TimerCurrentPoint = SysTimerTickCount;
	TimerExpiredPoint = TimerCurrentPoint + t;

	/* 处理溢出问题 */
	if(TimerExpiredPoint > SYS_TIMER_COUNT_MAX_VAULE)
	{
		TimerExpiredPoint = TimerExpiredPoint - SYS_TIMER_COUNT_MAX_VAULE - 1;
//		logPrintf("System Timer is happen overflow! TimerCurrentPoint = %d, TimerExpiredPoint = %d\r\n", TimerCurrentPoint, TimerExpiredPoint);
	}
	else if(TimerExpiredPoint < TimerCurrentPoint)
	{
		TimerExpiredPoint = TimerExpiredPoint + (0xFFFFFFFFU - SYS_TIMER_COUNT_MAX_VAULE);
//		logPrintf("System Timer is happen overflow! TimerCurrentPoint = %d, TimerExpiredPoint = %d\r\n", TimerCurrentPoint, TimerExpiredPoint);
	}
	return TimerExpiredPoint;
}

bool SysTimerIsExpired(uint32_t TimerExpiredPoint)
{
	uint32_t TimerCurrentPoint = SysTimerTickCount;

	if(SYS_TIMER_ALWAYS_TIMEOUT == TimerExpiredPoint) return true;
	else if(SYS_TIMER_NEVER_TIMEOUT == TimerExpiredPoint) return false;
	else if(TimerCurrentPoint >= TimerExpiredPoint)
	{
		if((TimerCurrentPoint - TimerExpiredPoint) < SYS_TIMER_MAX_DELAY_MS)
		{
			return true;
		}
	}
	else if((TimerExpiredPoint - TimerCurrentPoint) > SYS_TIMER_MAX_DELAY_MS)
	{
		return true;
	}
	return false;
}
