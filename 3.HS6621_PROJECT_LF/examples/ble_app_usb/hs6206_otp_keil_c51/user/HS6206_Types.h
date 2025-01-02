//------------------------------------------------------------------------------------
// HS6206_Types.h
//------------------------------------------------------------------------------------
// Copyright 2013, HunterSun Electronics Co., Ltd.
// Yao
// 2013-08-06
//
// Program Description:
// HS6206 type define
//
//

#ifndef __HS6206_TYPES_H__
#define __HS6206_TYPES_H__

#define Sint8  signed   char
#define Uint8  unsigned char
#define Uint16 unsigned int
#define Uint32 unsigned long
    
#define uint8_t  unsigned char


typedef unsigned char U8; 
typedef unsigned char u8; 
typedef volatile unsigned char vu8; 
typedef signed char S8;
typedef signed char s8; 
typedef unsigned int U16;
typedef unsigned int u16;
typedef signed int S16;
typedef signed int s16;
typedef unsigned long U32;
typedef unsigned long u32;
typedef signed long S32;
typedef signed long s32;

#define BIT0     0x01
#define BIT1     0x02
#define BIT2     0x04
#define BIT3     0x08
#define BIT4     0x10
#define BIT5     0x20
#define BIT6     0x40
#define BIT7     0x80
#define BIT8     0x100
#define BIT9     0x200
#define BIT10    0x400
#define BIT11    0x800
#define BIT12    0x1000
#define BIT13    0x2000
#define BIT14    0x4000
#define BIT15    0x8000

#ifdef DEBUG
#define debug printf
#else
#define debug test_printf
#endif

#endif   /*__HS6206_TYPES_H__*/


/*-------------------------------------------End Of File---------------------------------------------*/ 
