//------------------------------------------------------------------------------------
// rw_sfr.c
//------------------------------------------------------------------------------------
// Copyright 2022, OnMicro Electronics Co., Ltd.
// shangzhou.hu
// 2022-04-01
//
// Program Description:
//
//
//

#ifdef HS6206_USER_INTERFACE

#include <stdio.h>
#include <string.h>
#include "HS6206_Regs.h"
#include "HS6206_Types.h"
#include "HS6206_Delay.h"

//static char reg_name[16]={0};
//static char * p=reg_name;
unsigned char hs6206_read_register(unsigned char register_addr)
{
//	memset(reg_name, 0, 16);
    switch(register_addr)
      {
         case  0x80 : /* p="P0"; */ return  P0 ;
	 case  0x81 : return  SP ;
	 case  0x82 : return  DPL ;
	 case  0x83 : return  DPH ;
	 case  0x84 : return  DPL1 ;
	 case  0x85 : return  DPH1 ;
	 case  0x87 : /* p="PCON"; */   return  PCON ;

	  /* 0x88~0x8F */
	 case  0x88 :  /* p="TCON"; */    return  TCON ;
	 case  0x89 :  /* p="TMOD"; */    return  TMOD ;
	 case  0x8A :  /* p="TL0"; */    return  TL0 ;
	 case  0x8B :  /* p="TL1"; */    return  TL1 ;
	 case  0x8C :  /* p="TH0"; */    return  TH0 ;
	 case  0x8D :  /* p="TH1"; */    return  TH1 ;
	 case  0x8E :  /* p="CKCON"; */    return  CKCON ;


	 /* 0x90~0x97 */
	 case  0x90 :  /* p="P1"; */    return  P1 ;

//	 case  0x92 :  /* p=""; */    return  DPS ;
	 case  0x93 :  /* p="P0DIR"; */    return  P0DIR ;
	 case  0x94 :  /* p="P1DIR"; */    return  P1DIR ;
	 case  0x95 :  /* p="P2DIR"; */    return  P2DIR ;

	 case  0x97 :  /* p="P2CON"; */    return  P2CON ;

	 /* 0x98~0x9F */
	 case  0x98 :  /* p="S0CON"; */    return  S0CON ;
	 case  0x99 :  /* p="S0BUF"; */    return  S0BUF ;
	 case  0x9A :  /* p="IEN2"; */    return  IEN2 ;



	 case  0x9E :  /* p="P0CON"; */    return  P0CON ;
	 case  0x9F :  /* p="P1CON"; */    return  P1CON ;

	 /* 0xA0~0xA7 */
	 case  0xA0 :  /* p="P2"; */        return  P2 ;
	 case  0xA1 :  /* p="PWMDC0"; */    return  PWMDC0 ;
	 case  0xA2 :  /* p="PWMDC1"; */    return  PWMDC1 ;
	 case  0xA3 :  /* p="CLKCTRL"; */   return  CLKCTRL ;
	 case  0xA4 :  /* p="PWRDWN"; */    return  PWRDWN ;
	 case  0xA5 :  /* p="WUCON"; */     return  WUCON ;
	 case  0xA6 :  /* p="INTEXP"; */    return  INTEXP ;
	 case  0xA7 :  /* p="MEMCON"; */    return  MEMCON ;

	 /* 0xA8~0xAF */
	 case  0xA8 :  /* p="IEN0"; */    return  IEN0 ;
	 case  0xA9 :  /* p="IP0"; */     return  IP0 ;
	 case  0xAA :  /* p="S0RELL"; */  return  S0RELL ;

	 case  0xAC :  /* p="CLKGATE"; */    return  CLKGATE ;
	 case  0xAD :  /* p="CLKLFCTRL"; */  return  CLKLFCTRL ;
	 case  0xAE :  /* p="OPMCON"; */     return  OPMCON ;
	 case  0xAF :  /* p="OSC_CTL"; */    return  OSC_CTL ;

	 /* 0xB0~0xB7 */
	 case  0xB0 :  /* p="P3"; */        return  P3 ;
	 case  0xB1 :  /* p="RSTREAS"; */   return  RSTREAS ;
	 case  0xB2 :  /* p="PWMCON"; */    return  PWMCON ;
	 case  0xB3 :  /* p="REGXL"; */     return  REGXL ;
	 case  0xB4 :  /* p="REGXH"; */     return  REGXH ;
	 case  0xB5 :  /* p="REGXC"; */     return  REGXC ;
	 case  0xB6 :  /* p="TICKDV"; */    return  TICKDV ;
	 case  0xB7 :  /* p="PWMDC2"; */    return  PWMDC2 ;

	 /* 0xB8~0xBF */
	 case  0xB8 :  /* p="IEN1"; */    return  IEN1 ;
	 case  0xB9 :  /* p="IP1"; */     return  IP1 ;
	 case  0xBA :  /* p="S0RELH"; */  return  S0RELH ;

	 case  0xBC :  /* p="SPISCON"; */    return  SPISCON ;

	 case  0xBE :  /* p="SPISSTAT"; */   return  SPISSTAT ;
	 case  0xBF :  /* p="SPISDAT"; */    return  SPISDAT ;

	 /* 0xC0~0xC7 */
	 case  0xC0 :  /* p="IRCON"; */   return  IRCON ;
	 case  0xC1 :  /* p="CCEN"; */    return  CCEN ;
	 case  0xC2 :  /* p="CCL1"; */    return  CCL1 ;
	 case  0xC3 :  /* p="CCH1"; */    return  CCH1 ;
	 case  0xC4 :  /* p="CCL2"; */    return  CCL2 ;
	 case  0xC5 :  /* p="CCH2"; */    return  CCH2 ;
	 case  0xC6 :  /* p="CCL3"; */    return  CCL3 ;
	 case  0xC7 :  /* p="CCH3"; */    return  CCH3 ;

		 /* 0xC8~0xCF */
	 case  0xC8 :  /* p="T2CON"; */      return  T2CON ;
	 case  0xC9 :  /* p="IEN3"; */       return  IEN3 ;
	 case  0xCA :  /* p="CRCL"; */       return  CRCL ;
	 case  0xCB :  /* p="CRCH"; */       return  CRCH ;
	 case  0xCC :  /* p="TL2"; */        return  TL2 ;
	 case  0xCD :  /* p="TH2"; */        return  TH2 ;
	 case  0xCE :  /* p="USBCON"; */     return  USBCON ;
	 case  0xCF :  /* p="USBPCON"; */    return  USBPCON ;

	 /* 0xD0~0xD7 */
	 case  0xD0 :  /* p="PSW"; */       return  PSW ;
	 case  0xD1 :  /* p="IEN4"; */      return  IEN4 ;
	 case  0xD2 :  /* p="T2IER"; */     return  T2IER ;
	 case  0xD3 :  /* p="T2SR"; */      return  T2SR ;
	 case  0xD4 :  /* p="PWMCR1"; */    return  PWMCR1 ;
	 case  0xD5 :  /* p="PWMDC3"; */    return  PWMDC3 ;
	 case  0xD6 :  /* p="PWMDC4"; */    return  PWMDC4 ;
	 case  0xD7 :  /* p="PWMDC5"; */    return  PWMDC5 ;

	 /* 0xD8~0xDF */
	 case  0xD8 :  /* p="ADCON"; */      return  ADCON ;
	 case  0xD9 :  /* p="PWMCR0"; */     return  PWMCR0 ;
	 case  0xDA :  /* p="PWMCON1"; */    return  PWMCON1 ;
	 case  0xDB :  /* p="USBPCON1"; */   return  USBPCON1 ;
	 case  0xDC :  /* p="POFCON"; */     return  POFCON ;
	 case  0xDD :  /* p="CCEN1"; */      return  CCEN1 ;
	 case  0xDE :  /* p="DBGSEL0"; */    return  DBGSEL0 ;
	 case  0xDF :  /* p="DBGSEL1"; */    return  DBGSEL1 ;

	 /* 0xE0~0xE7 */
//	 case  0xE0 :  /* p=""; */    return  ACC ;
	 case  0xE1 :  /* p="CFG_DFEN_P0"; */    return  CFG_DFEN_P0 ;
	 case  0xE2 :  /* p="CFG_DFEN_P1"; */    return  CFG_DFEN_P1 ;
	 case  0xE3 :  /* p="CFG_DFEN_P2"; */    return  CFG_DFEN_P2 ;

	 case  0xE5 :  /* p="SPIRCON"; */    return  SPIRCON ;
	 case  0xE6 :  /* p="SPIRSTAT"; */   return  SPIRSTAT ;
	 case  0xE7 :  /* p="SPIRDAT"; */    return  SPIRDAT ;

	 /* 0xE8~0xEF */
	 case  0xE8 :  /* p="RFCON"; */    return  RFCON ;
	 case  0xE9 :  /* p="MD0"; */      return  MD0 ;
	 case  0xEA :  /* p="MD1"; */      return  MD1 ;
	 case  0xEB :  /* p="MD2"; */      return  MD2 ;
	 case  0xEC :  /* p="MD3"; */      return  MD3 ;
	 case  0xED :  /* p="MD4"; */      return  MD4 ;
	 case  0xEE :  /* p="MD5"; */      return  MD5 ;
	 case  0xEF :  /* p="ARCON"; */    return  ARCON ;

	 /* 0xF0~0xF7 */
//	 case  0xF0 :  /* p=""; */    return  B ;

	 case  0xF2 :  /* p="SPITEST"; */     return  SPITEST ;

	 case  0xF4 :  /* p="CPLDOCON"; */    return  CPLDOCON ;
	 case  0xF5 :  /* p="CIDH"; */        return  CIDH ;
	 case  0xF6 :  /* p="CIDL"; */        return  CIDL ;
	 case  0xF7 :  /* p="SRST"; */        return  SRST ;

	 /* 0xF8~0xFF */
	 case  0xF8 :  /* p="FSR"; */       return  FSR ;

	 case  0xFB :  /* p="SPIDLY"; */    return  SPIDLY ;
	 case  0xFC :  /* p="SPIMCON0"; */  return  SPIMCON0 ;
	 case  0xFD :  /* p="SPIMCON1"; */  return  SPIMCON1 ;
	 case  0xFE :  /* p="SPIMSTAT"; */  return  SPIMSTAT ;
	 case  0xFF :  /* p="SPIMDAT"; */   return  SPIMDAT ;

	}
    return 0;
}

void hs6206_write_register(unsigned char register_addr, unsigned char val)
{
    switch(register_addr)
      {
	 case  0x80 : P0 = val; break;
	 case  0x81 : SP = val; break;
/*	 case  0x82 : DPL = val; break;
	 case  0x83 : DPH = val; break;
	 case  0x84 : DPL1 = val; break;
	 case  0x85 : DPH1 = val; break;
*/
	 case  0x87 : PCON = val; break;

	 /* 0x88~0x8F */
	 case  0x88 : TCON = val; break;
	 case  0x89 : TMOD = val; break;
	 case  0x8A : TL0 = val; break;
	 case  0x8B : TL1 = val; break;
	 case  0x8C : TH0 = val; break;
	 case  0x8D : TH1 = val; break;
	 case  0x8E : CKCON = val; break;


	 /* 0x90~0x97 */
	 case  0x90 : P1 = val; break;

/*	 case  0x92 : DPS = val; break;
 */
	 case  0x93 : P0DIR = val; break;
	 case  0x94 : P1DIR = val; break;
	 case  0x95 : P2DIR = val; break;

	 case  0x97 : P2CON = val; break;

	 /* 0x98~0x9F */
	 case  0x98 : S0CON = val; break;
	 case  0x99 : S0BUF = val; break;
	 case  0x9A : IEN2 = val; break;



	 case  0x9E : P0CON = val; break;
	 case  0x9F : P1CON = val; break;

	 /* 0xA0~0xA7 */
	 case  0xA0 : P2 = val; break;
	 case  0xA1 : PWMDC0 = val; break;
	 case  0xA2 : PWMDC1 = val; break;
	 case  0xA3 : CLKCTRL = val; break;
	 case  0xA4 : PWRDWN = val; break;
	 case  0xA5 : WUCON = val; break;
	 case  0xA6 : INTEXP = val; break;
	 case  0xA7 : MEMCON = val; break;

	 /* 0xA8~0xAF */
	 case  0xA8 : IEN0 = val; break;
	 case  0xA9 : IP0 = val; break;
	 case  0xAA : S0RELL = val; break;

	 case  0xAC : CLKGATE = val; break;
	 case  0xAD : CLKLFCTRL = val; break;
	 case  0xAE : OPMCON = val; break;
	 case  0xAF : OSC_CTL = val; break;

	 /* 0xB0~0xB7 */
	 case  0xB0 : P3 = val; break;
	 case  0xB1 : RSTREAS = val; break;
	 case  0xB2 : PWMCON = val; break;
	 case  0xB3 : REGXL = val; break;
	 case  0xB4 : REGXH = val; break;
	 case  0xB5 : REGXC = val; break;
	 case  0xB6 : TICKDV = val; break;
	 case  0xB7 : PWMDC2 = val; break;

	 /* 0xB8~0xBF */
	 case  0xB8 : IEN1 = val; break;
	 case  0xB9 : IP1 = val; break;
	 case  0xBA : S0RELH = val; break;

	 case  0xBC : SPISCON = val; break;

	 case  0xBE : SPISSTAT = val; break;
	 case  0xBF : SPISDAT = val; break;

	 /* 0xC0~0xC7 */
	 case  0xC0 : IRCON = val; break;
	 case  0xC1 : CCEN = val; break;
	 case  0xC2 : CCL1 = val; break;
	 case  0xC3 : CCH1 = val; break;
	 case  0xC4 : CCL2 = val; break;
	 case  0xC5 : CCH2 = val; break;
	 case  0xC6 : CCL3 = val; break;
	 case  0xC7 : CCH3 = val; break;

	 /* 0xC8~0xCF */
	 case  0xC8 : T2CON = val; break;
	 case  0xC9 : IEN3 = val; break;
	 case  0xCA : CRCL = val; break;
	 case  0xCB : CRCH = val; break;
	 case  0xCC : TL2 = val; break;
	 case  0xCD : TH2 = val; break;
	 case  0xCE : USBCON = val; break;
	 case  0xCF : USBPCON = val; break;

	 /* 0xD0~0xD7 */
	 case  0xD0 : PSW = val; break;
	 case  0xD1 : IEN4 = val; break;
	 case  0xD2 : T2IER = val; break;
	 case  0xD3 : T2SR = val; break;
	 case  0xD4 : PWMCR1 = val; break;
	 case  0xD5 : PWMDC3 = val; break;
	 case  0xD6 : PWMDC4 = val; break;
	 case  0xD7 : PWMDC5 = val; break;

	 /* 0xD8~0xDF */
	 case  0xD8 : ADCON = val; break;
	 case  0xD9 : PWMCR0 = val; break;
	 case  0xDA : PWMCON1 = val; break;
	 case  0xDB : USBPCON1 = val; break;
	 case  0xDC : POFCON = val; break;
	 case  0xDD : CCEN1 = val; break;
	 case  0xDE : DBGSEL0 = val; break;
	 case  0xDF : DBGSEL1 = val; break;

	 /* 0xE0~0xE7 */
/*	 case  0xE0 : ACC = val; break;
 */
	 case  0xE1 : CFG_DFEN_P0 = val; break;
	 case  0xE2 : CFG_DFEN_P1 = val; break;
	 case  0xE3 : CFG_DFEN_P2 = val; break;

	 case  0xE5 : SPIRCON = val; break;
	 case  0xE6 : SPIRSTAT = val; break;
	 case  0xE7 : SPIRDAT = val; break;

	 /* 0xE8~0xEF */
	 case  0xE8 : RFCON = val; break;
	 case  0xE9 : MD0 = val; break;
	 case  0xEA : MD1 = val; break;
	 case  0xEB : MD2 = val; break;
	 case  0xEC : MD3 = val; break;
	 case  0xED : MD4 = val; break;
	 case  0xEE : MD5 = val; break;
	 case  0xEF : ARCON = val; break;

	 /* 0xF0~0xF7 */
/*	 case  0xF0 : B = val; break;
 */
	 case  0xF2 : SPITEST = val; break;

	 case  0xF4 : CPLDOCON = val; break;
	 case  0xF5 : CIDH = val; break;
	 case  0xF6 : CIDL = val; break;
	 case  0xF7 : SRST = val; break;

	 /* 0xF8~0xFF */
	 case  0xF8 : FSR = val; break;


	 case  0xFB : SPIDLY = val; break;
	 case  0xFC : SPIMCON0 = val; break;
	 case  0xFD : SPIMCON1 = val; break;
	 case  0xFE : SPIMSTAT = val; break;
	 case  0xFF : SPIMDAT = val; break;
      default:
         //printf("not support\n");
      break;
      }
}

#define RF_SPI_CSN_High  RFCON |= BIT1  
#define RF_SPI_CSN_Low   RFCON &=~BIT1  

/*RFSPI write/read 1 Byte to/from RF */
static U8 RFSPI_WR_Byte_Poll(U8 Byte)
{
    while(!(SPIRSTAT & BIT0));      //Tx Fifo not ready.
    SPIRDAT=Byte;
    while(!(SPIRSTAT & BIT2));      //Rx Fifo has data.
    return SPIRDAT;
}

void HS6206_SPI_Wr_Reg(U8 Reg,U8 *Reg_Val,U8 Reg_Width)
{
	U8 i;
	RF_SPI_CSN_Low;   //拉低
	RFSPI_WR_Byte_Poll(0x20+Reg);
	for(i=0x00;i<Reg_Width;i++)
	{
		RFSPI_WR_Byte_Poll(*Reg_Val++);
	}
	RF_SPI_CSN_High;  //拉高
}

void HS6206_SPI_Rd_Reg(U8 Reg,U8 *Reg_Val,U8 Reg_Width)
{
	U8 i;
	RF_SPI_CSN_Low;   //拉低
	RFSPI_WR_Byte_Poll(0x00+Reg);
	for(i=0x00;i<Reg_Width;i++)
	{
		Reg_Val[i]=RFSPI_WR_Byte_Poll(0x00);
	}
	RF_SPI_CSN_High ; //拉高	
}

/*
     P[0]      P[1]	  P[2]     P[3]...P[4]
	 TEST_PAT  DEV 	  length   cmd argemunt
 */

void HS6206_Bank1_Activate(void)	//切换到BANK1 状态下
{
    U8 status;
	HS6206_SPI_Rd_Reg(0x07,&status,0x01);
	if( (status & BIT7) == 0x00)	   //in Bank0 status
	{
		RF_SPI_CSN_Low;                     // CSN low, init SPI transaction
		RFSPI_WR_Byte_Poll(0x50);    // send command
		RFSPI_WR_Byte_Poll(0x53);                 // send command
		RF_SPI_CSN_High;                      // CSN high again
	}    
}	

void HS6206_Bank0_Activate(void)	 //切换到BANK0 状态下 
{
    U8 status;
	HS6206_SPI_Rd_Reg(0x07,&status,0x01);
    if( (status & BIT7) == BIT7)	   //in Bank1 status
    {
        RF_SPI_CSN_Low;                     // CSN low, init SPI transaction
        RFSPI_WR_Byte_Poll(0x50);    // send command
        RFSPI_WR_Byte_Poll(0x53);                 // send command
        RF_SPI_CSN_High;          // CSN high again
    }                                     
}

#endif

/*-------------------------------------------End Of File---------------------------------------------*/


