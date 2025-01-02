//------------------------------------------------------------------------------------
// hs6206_#define_reg.h
//------------------------------------------------------------------------------------
// Copyright (c) 2022, OnMicro Electronics Co., Ltd.
// crazy_code
//
// Program Description: Header file for HS6206 which based on generic 8051 core.
//
//
//

#ifndef __HS6206_SFR_REGS_H__
#define __HS6206_SFR_REGS_H__

/*-------------------------Special Function Register------------------------------*/
// 0x80~0x87
#define P0      0x80
#define SP      0x81
#define DPL     0x82
#define DPH     0x83
#define DPL1    0x84
#define DPH1    0x85

#define PCON    0x87

// 0x88~0x8F
#define TCON    0x88
#define TMOD    0x89
#define TL0     0x8A
#define TL1     0x8B
#define TH0     0x8C
#define TH1     0x8D
#define CKCON   0x8E


// 0x90~0x97
#define P1       0x90

#define DPS      0x92
#define P0DIR    0x93  
#define P1DIR    0x94  
#define P2DIR    0x95

#define P2CON    0x97

// 0x98~0x9F
#define S0CON    0x98
#define S0BUF    0x99
#define IEN2     0x9A



#define P0CON    0x9E
#define P1CON    0x9F

// 0xA0~0xA7
#define P2         0xA0
#define PWMDC0     0xA1
#define PWMDC1     0xA2
#define CLKCTRL    0xA3
#define PWRDWN     0xA4
#define WUCON      0xA5
#define INTEXP     0xA6
#define MEMCON     0xA7

// 0xA8~0xAF
#define IEN0       0xA8
#define IP0        0xA9
#define S0RELL     0xAA

#define CLKGATE    0xAC
#define CLKLFCTRL  0xAD
#define OPMCON     0xAE
#define OSC_CTL    0xAF

// 0xB0~0xB7
#define P3         0xB0
#define RSTREAS    0xB1
#define PWMCON     0xB2
#define REGXL      0xB3
#define REGXH      0xB4
#define REGXC      0xB5
#define TICKDV     0xB6
#define PWMDC2     0xB7

// 0xB8~0xBF
#define IEN1       0xB8
#define IP1        0xB9
#define S0RELH     0xBA

#define SPISCON    0xBC

#define SPISSTAT    0xBE
#define SPISDAT     0xBF

// 0xC0~0xC7
#define IRCON     0xC0
#define CCEN      0xC1
#define CCL1      0xC2
#define CCH1      0xC3
#define CCL2      0xC4
#define CCH2      0xC5
#define CCL3      0xC6
#define CCH3      0xC7

// 0xC8~0xCF
#define T2CON     0xC8
#define IEN3      0xC9
#define CRCL      0xCA
#define CRCH      0xCB
#define TL2       0xCC
#define TH2       0xCD
#define USBCON    0xCE
#define USBPCON    0xCF

// 0xD0~0xD7
#define PSW       0xD0
#define IEN4      0xD1
#define T2IER     0xD2
#define T2SR      0xD3
#define PWMCR1    0xD4
#define PWMDC3    0xD5
#define PWMDC4    0xD6
#define PWMDC5    0xD7

// 0xD8~0xDF
#define ADCON     0xD8        /*Serial Port 0 Baud Rate Select register - ADCON,reset 0x00.*/
#define PWMCR0    0xD9
#define PWMCON1     0xDA
#define USBPCON1     0xDB
#define POFCON    0xDC
#define CCEN1     0xDD
#define DBGSEL0    0xDE
#define DBGSEL1    0xDF

// 0xE0~0xE7
#define ACC       0xE0
#define CFG_DFEN_P0    0xE1
#define CFG_DFEN_P1    0xE2
#define CFG_DFEN_P2    0xE3

#define RFSPIRCON    0xE5
#define RFSPIRSTAT    0xE6
#define RFSPIRDAT     0xE7

// 0xE8~0xEF
#define RF_CON    0xE8
#define MD0      0xE9
#define MD1      0xEA
#define MD2      0xEB
#define MD3      0xEC
#define MD4      0xED
#define MD5      0xEE
#define ARCON    0xEF

// 0xF0~0xF7
#define B          0xF0

#define SPITEST     0xF2

#define CPLDOCON     0xF4
#define CIDH     0xF5
#define CIDL     0xF6
#define SRST     0xF7

// 0xF8~0xFF
#define FSR     0xF8


#define SPIDLY    0xFB
#define SPIMCON0    0xFC
#define SPIMCON1    0xFD
#define SPIMSTAT    0xFE
#define SPIMDAT    0xFF

/*----------------------BIT Register----------------------*/
/*  PSW   */
#define CY      0xD7
#define AC      0xD6
#define F0      0xD5
#define RS1     0xD4
#define RS0     0xD3
#define OV      0xD2
#define P       0xD0

/*  TCON  */
#define TF1     0x8F
#define TR1     0x8E
#define TF0     0x8D
#define TR0     0x8C
#define IE1     0x8B
#define IT1     0x8A
#define IE0     0x89
#define IT0     0x88

/*  IEN0   */
#define EA      0xAF      //enable all.
#define ET2     0xAD      // enable Timer2 interrupt.
#define ES      0xAC      // enable serial port interrupt. 
#define ET1     0xAB      // enable timer1 overflow interrupt.
#define EPF     0xAA      // enable power failure interrrupt.
#define ET0     0xA9      // enable timer0 overflow interrupt.
#define EX      0xA8      // enable interrupt from pin.  

#define ERTC      0xBD      // enable interrupt from rtc.  

/* P2 */
#define P2_7    0xA7
#define P2_6    0xA6
#define P2_5    0xA5
#define P2_4    0xA4
#define P2_3    0xA3
#define P2_2    0xA2
#define P2_1    0xA1
#define P2_0    0xA0

/*  S0CON  */
#define SM00     0x9F
#define SM10     0x9E
#define SM2      0x9D
#define REN0     0x9C
#define TB80     0x9B
#define RB80     0x9A
#define TI0      0x99
#define RI0      0x98

/* P1 */
#define P1_7    0x97
#define P1_6    0x96
#define P1_5    0x95
#define P1_4    0x94
#define P1_3    0x93
#define P1_2    0x92
#define P1_1    0x91
#define P1_0    0x90

/* P0 */
#define P0_7    0x87
#define P0_6    0x86
#define P0_5    0x85
#define P0_4    0x84
#define P0_3    0x83
#define P0_2    0x82
#define P0_1    0x81
#define P0_0    0x80


#endif  /*__HS6206_SFR_REGS_H__*/


/*-------------------------------------------End Of File---------------------------------------------*/

