//------------------------------------------------------------------------------------
// HS6206_Delay.c
//------------------------------------------------------------------------------------
// Copyright 2013, HunterSun Electronics Co., Ltd.
// Yao
// 2013-08-06
//
// Program Description:
//
//
//

#include "HS6206_Types.h"

/*  
 * This is a very lousy delay assuming a 1T core. 
 * Do not rely on it for ANY precision 
 */

/*
 * cycle count == 0 + 4*c. Max 1024;
 */
static void delay_cycles_small(unsigned char c)
{
  c=c;
#pragma asm
_L00001:
	NOP;
	NOP;
	DJNZ R7,_L00001
#pragma endasm
}

/*  
 * TODO: Rewrite this in asm, to add precision
 * The 32bit var handling introduces extra delay
 * that is REALLY visible
 * 
 */
void delay_4cycles(U32 c)
{
	if (c<0xff)
		delay_cycles_small((unsigned char) c);
	else {
		c = c >> 8;
		while(c--) 
			delay_cycles_small(0xff);
	}
}

/*-------------------------------------------End Of File---------------------------------------------*/
