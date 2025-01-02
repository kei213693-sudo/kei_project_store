//------------------------------------------------------------------------------------
// HS6206_Regs.h
//------------------------------------------------------------------------------------
// Copyright (c) 2013, HunterSun Electronics Co., Ltd.
// Yao
// 2013-08-06
//
// Program Description: Header file for HS6206 which based on generic 8051 core.
//
//
//

#ifndef __HS6206_REGS_H__
#define __HS6206_REGS_H__

/*-------------------------Special Function Register------------------------------*/
// 0x80~0x87
sfr P0   = 0x80;
sfr SP   = 0x81;
sfr DPL  = 0x82;
sfr DPH  = 0x83;
sfr DPL1 = 0x84;
sfr DPH1 = 0x85;

sfr PCON = 0x87;

// 0x88~0x8F
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0  = 0x8A;
sfr TL1  = 0x8B;
sfr TH0  = 0x8C;
sfr TH1  = 0x8D;
sfr CKCON = 0x8E;


// 0x90~0x97
sfr P1    = 0x90;

sfr DPS   = 0x92;
sfr P0DIR = 0x93;  
sfr P1DIR = 0x94;  
sfr P2DIR = 0x95;

sfr P2CON = 0x97;

// 0x98~0x9F
sfr S0CON = 0x98;
sfr S0BUF = 0x99;
sfr IEN2  = 0x9A;



sfr P0CON = 0x9E;
sfr P1CON = 0x9F;

// 0xA0~0xA7
sfr P2      = 0xA0;
sfr PWMDC0  = 0xA1;
sfr PWMDC1  = 0xA2;
sfr CLKCTRL = 0xA3;
sfr PWRDWN  = 0xA4;
sfr WUCON   = 0xA5;
sfr INTEXP  = 0xA6;
sfr MEMCON  = 0xA7;

// 0xA8~0xAF
sfr IEN0    = 0xA8;
sfr IP0     = 0xA9;
sfr S0RELL  = 0xAA;

sfr CLKGATE  = 0xAC;
sfr CLKLFCTRL = 0xAD;
sfr OPMCON  = 0xAE;
sfr OSC_CTL = 0xAF;

// 0xB0~0xB7
sfr P3      = 0xB0;
sfr RSTREAS = 0xB1;
sfr PWMCON  = 0xB2;
sfr REGXL   = 0xB3;
sfr REGXH   = 0xB4;
sfr REGXC   = 0xB5;
sfr TICKDV  = 0xB6;
sfr PWMDC2  = 0xB7;

// 0xB8~0xBF
sfr IEN1    = 0xB8;
sfr IP1     = 0xB9;
sfr S0RELH  = 0xBA;

sfr SPISCON = 0xBC;

sfr SPISSTAT = 0xBE;
sfr SPISDAT  = 0xBF;

// 0xC0~0xC7
sfr IRCON  = 0xC0;
sfr CCEN   = 0xC1;
sfr CCL1   = 0xC2;
sfr CCH1   = 0xC3;
sfr CCL2   = 0xC4;
sfr CCH2   = 0xC5;
sfr CCL3   = 0xC6;
sfr CCH3   = 0xC7;

// 0xC8~0xCF
sfr T2CON  = 0xC8;
sfr IEN3   = 0xC9;
sfr CRCL   = 0xCA;
sfr CRCH   = 0xCB;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;
sfr USBCON = 0xCE;
sfr USBPCON = 0xCF;

// 0xD0~0xD7
sfr PSW    = 0xD0;
sfr IEN4   = 0xD1;
sfr T2IER  = 0xD2;
sfr T2SR   = 0xD3;
sfr PWMCR1 = 0xD4;
sfr PWMDC3 = 0xD5;
sfr PWMDC4 = 0xD6;
sfr PWMDC5 = 0xD7;

// 0xD8~0xDF
sfr ADCON  = 0xD8;        /*Serial Port 0 Baud Rate Select register - ADCON,reset 0x00.*/
sfr PWMCR0 = 0xD9;
sfr PWMCON1  = 0xDA;
sfr USBPCON1  = 0xDB;
sfr POFCON = 0xDC;
sfr CCEN1  = 0xDD;
sfr DBGSEL0 = 0xDE;
sfr DBGSEL1 = 0xDF;

// 0xE0~0xE7
sfr ACC    = 0xE0;
sfr CFG_DFEN_P0 = 0xE1;
sfr CFG_DFEN_P1 = 0xE2;
sfr CFG_DFEN_P2 = 0xE3;

sfr SPIRCON = 0xE5;
sfr SPIRSTAT = 0xE6;
sfr SPIRDAT  = 0xE7;

// 0xE8~0xEF
sfr RFCON = 0xE8;
sfr MD0   = 0xE9;
sfr MD1   = 0xEA;
sfr MD2   = 0xEB;
sfr MD3   = 0xEC;
sfr MD4   = 0xED;
sfr MD5   = 0xEE;
sfr ARCON = 0xEF;

// 0xF0~0xF7
sfr B       = 0xF0;

sfr SPITEST  = 0xF2;

sfr CPLDOCON  = 0xF4;
sfr CIDH  = 0xF5;
sfr CIDL  = 0xF6;
sfr SRST  = 0xF7;

// 0xF8~0xFF
sfr FSR  = 0xF8;


sfr SPIDLY = 0xFB;
sfr SPIMCON0 = 0xFC;
sfr SPIMCON1 = 0xFD;
sfr SPIMSTAT = 0xFE;
sfr SPIMDAT = 0xFF;

/*----------------------BIT Register----------------------*/
/*  PSW   */
sbit CY   = 0xD7;
sbit AC   = 0xD6;
sbit F0   = 0xD5;
sbit RS1  = 0xD4;
sbit RS0  = 0xD3;
sbit OV   = 0xD2;
sbit P    = 0xD0;

/*  TCON  */
sbit TF1  = 0x8F;
sbit TR1  = 0x8E;
sbit TF0  = 0x8D;
sbit TR0  = 0x8C;
sbit IE1  = 0x8B;
sbit IT1  = 0x8A;
sbit IE0  = 0x89;
sbit IT0  = 0x88;

/*  IEN0   */
sbit EA   = 0xAF;      //enable all.
sbit ET2  = 0xAD;      // enable Timer2 interrupt.
sbit ES   = 0xAC;      // enable serial port interrupt. 
sbit ET1  = 0xAB;      // enable timer1 overflow interrupt.
sbit EPF  = 0xAA;      // enable power failure interrrupt.
sbit ET0  = 0xA9;      // enable timer0 overflow interrupt.
sbit EX   = 0xA8;      // enable interrupt from pin.  

sbit ERTC   = 0xBD;      // enable interrupt from rtc.  

/* P2 */
sbit P2_7  =0xA7;
sbit P2_6  =0xA6;
sbit P2_5  =0xA5;
sbit P2_4  =0xA4;
sbit P2_3  =0xA3;
sbit P2_2  =0xA2;
sbit P2_1  =0xA1;
sbit P2_0  =0xA0;

/*  S0CON  */
sbit SM00  = 0x9F;
sbit SM10  = 0x9E;
sbit SM2   = 0x9D;
sbit REN0  = 0x9C;
sbit TB80  = 0x9B;
sbit RB80  = 0x9A;
sbit TI0   = 0x99;
sbit RI0   = 0x98;

/* P1 */
sbit P1_7  =0x97;
sbit P1_6  =0x96;
sbit P1_5  =0x95;
sbit P1_4  =0x94;
sbit P1_3  =0x93;
sbit P1_2  =0x92;
sbit P1_1  =0x91;
sbit P1_0  =0x90;

/* P0 */
sbit P0_7  =0x87;
sbit P0_6  =0x86;
sbit P0_5  =0x85;
sbit P0_4  =0x84;
sbit P0_3  =0x83;
sbit P0_2  =0x82;
sbit P0_1  =0x81;
sbit P0_0  =0x80;




/*--------------------------external defines---------------------------------*/
/*PCON*/
#define SMOD 0x80      /*baud rate *2 */
#define PMW  0x10      /* program memory write */

/*ADCON*/
#define BD_SEL 0x80            /*Serial Port 0 baud rate select (in modes 1 and 3). When 1, additional internal baud rate generator is used, otherwise Timer 1 overflow is used*/  

/* CKCON */
#define CKCON_PW         0x70     /* Program memory wait state control */
#define CKCON_PW_SFT     4

/*--------------------interrupt vector table-----------------------*/                
#define HS6206_INT_IFP               0x00       /*interrupt from GPINT0, GPINT1 and GPINT2*/
#define HS6206_INT_TIMER0            0x01       /*Timer0 overflow interrupt*/
#define HS6206_INT_POF               0x02       /*power failure interrupt*/ 
#define HS6206_INT_TIMER1            0x03       /*Timer1 overflow interrupt*/ 
#define HS6206_INT_UART              0x04       /*UART interrupt*/
#define HS6206_INT_TIMER2            0x05       /*Timer2 overflow interrupt and external reload*/  

#define HS6206_INT_RFRDY             0x08       /*RF SPI ready*/
#define HS6206_INT_RFIRQ             0x09       /*RF interupt*/
#define HS6206_INT_MISC              0x0A       /*Master and Slave SPI transaction completed,2-Wire transaction completed and XOSC16M started (X16IRQ)*/
#define HS6206_INT_USBWU             0x0B       /*USB wakeup Interrupt*/
#define HS6206_INT_USBIRQ            0x0C       /*USB interrupt*/
#define HS6206_INT_WU                0x0D       /*Internal Wakeup interrupt*/
#define HS6206_INT_RTC               0x0D       /*RTC interrupt*/



#endif  /*__HS6206_REGS_H__*/


/*-------------------------------------------End Of File---------------------------------------------*/

