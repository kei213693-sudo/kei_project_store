//------------------------------------------------------------------------------------
// HS6206_Delay.h
//------------------------------------------------------------------------------------
// Copyright 2013, HunterSun Electronics Co., Ltd.
// wei.lu@huntersun.com.cn
// 2013-08-06
//
// Program Description:
//	 delay_xxx macros
//
//

#ifndef __HS6206_DELAY_H__
#define __HS6206_DELAY_H__

#include "HS6206_Types.h"

#define HS6206_SYSCLK 8000000UL
//#define HS6206_SYSCLK 16000000UL
#define F_CPU HS6206_SYSCLK

void delay_4cycles(U32 c);

#define delay_cycles(c) delay_4cycles(c/4);
#define delay_s(s)  delay_4cycles(s * (F_CPU / 4))
#define delay_ms(s) delay_4cycles(s * (F_CPU / 1000 / 4))
#define delay_us(s) delay_4cycles(s * (F_CPU / 1000 / 1000 / 4))

#endif  /*__HS6206_DELAY_H__*/


/*-------------------------------------------End Of File---------------------------------------------*/

