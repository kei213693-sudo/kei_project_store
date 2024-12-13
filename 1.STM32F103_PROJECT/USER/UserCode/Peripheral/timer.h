#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "FreeRTOS.h"

#define TIMR_TIM2_BASE_US              (50)//tim2定时中断周期，us
#define TIMR_TIM3_BASE_US              (1000)//tim3定时中断周期，us，1ms

extern TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 
void TIM3_Init(u16 arr,u16 psc);

extern TIM_HandleTypeDef TIM2_Handler;      //定时器句柄
extern void ConfigureTimeForRunTimeStats(void);
#if (1 == configGENERATE_RUN_TIME_STATS)
extern volatile unsigned long long FreeRTOSRunTimeTicks;//FreeRTOS 时间统计所用的节拍计数器
#endif

#endif
