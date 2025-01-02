//------------------------------------------------------------------------------
// HS6206_UART.c
//------------------------------------------------------------------------------
// Copyright 2013, HunterSun Electronics Co., Ltd.
// shangzhou.hu@huntersun.com.cn
// 2013-12-23
//
// Program Description:
//
//
//

#include  <intrins.h> 
#include "HS6206_Regs.h"
#include "HS6206_UART.h"
#include "HS6206_Types.h"
//#include "app_ram.h"

#define F_CPU  8000000UL
//#define USE_UART_ISR

code unsigned char HexTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                 'A', 'B', 'C', 'D', 'E', 'F'};

extern u8 uart_send_busy;
//extern xdata u8 Uart_Buf[UART_TO_USB_RAM_LEN];



char putchar(char c)
{
  if (c == '\n')
		serial_putchar('\r');
  serial_putchar(c);
  return c;
}

void prints(unsigned char *pd)
{
	while('\0' != *pd)
	{
		serial_putchar(*pd++);
	}
}

void print_hex(unsigned char u8data)
{
//	serial_putchar('0');
//	serial_putchar('x');
	serial_putchar(HexTable[u8data >> 4]);
	serial_putchar(HexTable[u8data & 0x0F]);
	serial_putchar(' ');
	
}

void print_short_int(unsigned short u16data)
{
	signed char i;
	unsigned char buf[7];

	buf[0] = '0';
	buf[1] = 'x';
	buf[6] = '\0';

	for(i = 5; i >= 2; i--)
	{
		buf[i] = HexTable[u16data & 0x0F];
		u16data >>= 4;
	}

	prints(buf);
	prints(" ");
}
void print_long_int(unsigned long u32data)
{
	signed char i;
	unsigned buf[10];

	for(i = 9; i >= 0; i--)
	{
		buf[i] = '0'+u32data%10;
		u32data /= 10;
	}

	for(i = 0; i < 9; i++)
	{
		if('0' != buf[i])
			break;
	}

	for(; i < 10; i++)
	{
		serial_putchar(buf[i]);
	}
	
}

void serial_putchar(char c)
{
	TI0 = 0;
	S0BUF = c;
	while (!TI0);
	_nop_();
	TI0 = 0;
}



char serial_getchar()
{
	char c;
	while (!RI0);
	c = S0BUF;
	RI0 = 0;
	return c;
}

char serial_avail() 
{
	return RI0;
}

void serial_init()
{
	unsigned short brt;
	int baudrate = 19200;
	
	P0DIR &= ~0x08;   //output TXD
	P0DIR |= 0x10;    //input RXD  

	/* 8N1 */
	//	SM00 = 0;
	//	SM10 = 1;
	//	REN0 = 1; //enable UART receive.
	S0CON = 0x50;

	PCON |= SMOD;
	ADCON |= BD_SEL;   //select baud rate clock.
	brt = (2<<10) - F_CPU/32/baudrate;  //2^10 - (2^SMOD)*F_CPU/64/BaudRate
	S0RELL = brt & 0xff;
	S0RELH = brt >> 8;

}



/*-------------------------------------------End Of File----------------------*/
