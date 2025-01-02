//------------------------------------------------------------------------------
// HS6206_UART.h
//------------------------------------------------------------------------------
// Copyright 2013, HunterSun Electronics Co., Ltd.
// luwei
// 2013-12-23
//
// Program Description:
//	 serial port APIs for debug in polling mode
//
//

#ifndef __HS6206_UART_H__
#define __HS6206_UART_H__
#include  <intrins.h> 
#include <stdio.h>


void serial_putchar(char c);
char serial_getchar();
char serial_avail();
void serial_init();
void prints(unsigned char *pd);
void print_hex(unsigned char u8data);
void print_short_int(unsigned short u16data);
void print_long_int(unsigned long u32data);

#define U_DEBUG

//device2pc buffer
//pc2device buffer

extern unsigned char UartBufInPoint;
extern unsigned char UartBufOutPoint;
extern unsigned char UartByteCount;

#endif  /*__HS6206_UART_H__*/


/*-------------------------------------------End Of File----------------------*/

