#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "FreeRTOS.h"

#define TIMR_TIM2_BASE_US              (50)//tim2定时中断周期，us
#define TIMR_TIM3_BASE_US              (1000)//tim3定时中断周期，us，1ms

#define TIMR_TIM1_PWM_ARR_MAX          (899)//ARR数值，决定pwm波的频率，这里899就是80khz

extern TIM_HandleTypeDef TIM1_Handler;      //定时器句柄 
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_deInit(void);
void hw_tim_tim1PwmChannel1_compare_set(void *pData);
void hw_tim_tim1PwmChannel1_start(void);
void hw_tim_tim1PwmChannel1_stop(void);

extern TIM_HandleTypeDef TIM2_Handler;      //定时器句柄
extern void ConfigureTimeForRunTimeStats(void);
#if (1 == configGENERATE_RUN_TIME_STATS)
extern volatile unsigned long long FreeRTOSRunTimeTicks;//FreeRTOS 时间统计所用的节拍计数器
#endif

extern TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 
void TIM3_Init(u16 arr,u16 psc);

#endif
