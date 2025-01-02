// File Name:  USB_STDREQ.c    
// Author:     Yao@HunterSun Electronics Co.,Ltd. 
// Date:       2013/07/31


#include <stdio.h>
#include <string.h>
//#include "hs6206_sfr_reg.h"
//#include "USB_Desc.h"
//#include "USB_ISR.h"
#include "USB_Regs.h"
#include "om6621pt_usb_interface.h"
#include "HS6206_Uti.h"
#include "RF.h"
#include "HS6206_Regs.h"
//#include "HS6206_Types.h"
#include "HS6206_Delay.h"
#define uint8_t  unsigned char
    
extern uint8_t usb_spi_rw(uint8_t value);
extern void USB_IRQ_Init(void);
extern void USB_SPI_Init(void);
extern void USB_SPI_CSN_Low(void);
extern void USB_SPI_CSN_High(void);
static  uint8_t txbuf[18], rxbuf[18];

#define CSN_LOW_DELAY  10

uint8_t IntIn;
uint8_t IntOut;
uint8_t IntUSB;

uint8_t USB_Read_Reg(uint8_t addr) reentrant	    
{
    uint8_t cmd = ((CMD_USB_REG_READ<<CMD_SHIFT)&0xe0) | 1;
    
    USB_SPI_CSN_Low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]); //12us 
    txbuf[0] = addr;
    usb_spi_rw(txbuf[0]); //20us 
	  
    txbuf[0] = 0xaa;
    rxbuf[0] = usb_spi_rw(txbuf[0]); //12us    
    USB_SPI_CSN_High(); //9us  
    
    return rxbuf[0];
}
void USB_Write_Reg(uint8_t addr, uint8_t val)reentrant
{
    uint8_t cmd = ((CMD_USB_REG_WRITE<<CMD_SHIFT)&0xe0) | 1;

    USB_SPI_CSN_Low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]); //12us 
    txbuf[0] = addr;
    usb_spi_rw(txbuf[0]); //14us
    txbuf[0] = val;
    usb_spi_rw(val);  //12us   
    USB_SPI_CSN_High(); //12us 
}
uint8_t USB_Is_Suspend_Mode() 
{  
//    return (USB_Read_Reg(M_REG_POWER) | USB_RSTMD);
}
void USB_Inactive_Clock()   
{
    // (USBCON &= ~BIT6)
}
void USB_Active_Clock() 
{
    // (USBCON |= BIT6)
}
void USB_Set_InMaxP_Size(uint8_t Size) 		
{
	USB_Write_Reg(M_REG_INMAXP,Size);
}
void USB_Set_OutMaxP_Size(uint8_t Size)		
{
	// (USB_Write_Reg(M_REG_OUTMAXP,Size))
}

void USB_Set_FAddr(uint8_t FAddr)				
{
    // (USB_Write_Reg(M_REG_FADDR,FAddr))
}
void USB_Select_Ep(uint8_t EndpointIdx)		
{
	// (USB_Write_Reg(M_REG_INDEX,EndpointIdx))
}

void Write_Sfr_Reg(uint8_t addr, uint8_t val)
{
    uint8_t cmd = ((CMD_SFR_WRITE<<CMD_SHIFT)&0xe0) | 1;

    USB_SPI_CSN_Low();  //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]); //12us
    txbuf[0] = addr;
    usb_spi_rw(txbuf[0]); //15us
    txbuf[0] = val;
    usb_spi_rw(val);   //12us  
    USB_SPI_CSN_High(); //126us
}

void USB_Get_Irq(void)
{
    uint8_t cmd = ((CMD_FUNC_CALL<<CMD_SHIFT)&0xe0) | 3;
    
    USB_SPI_CSN_Low();  //9us
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]);  //12us
    txbuf[0] = FUNC_USB_GET_IRQ;
    usb_spi_rw(txbuf[0]); //18us
	  Delay10us(2);
    txbuf[0] = 0xaa;
    IntUSB = usb_spi_rw(txbuf[0]); //13us
    Delay10us(2);	
    IntIn = usb_spi_rw(txbuf[0]); //13us
	  Delay10us(2);
    IntOut = usb_spi_rw(txbuf[0]); //13us
    USB_SPI_CSN_High();           //10us
   
}

uint8_t Read_Sfr_Reg(uint8_t addr)
{
    uint8_t cmd = ((CMD_SFR_READ<<CMD_SHIFT)&0xe0) | 1;
    
    USB_SPI_CSN_Low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]); //12us
    txbuf[0] = addr;
    usb_spi_rw(txbuf[0]); //175us
	  Delay10us(40);
    txbuf[0] = 0xaa;
    rxbuf[0] = usb_spi_rw(txbuf[0]); //15us     
    USB_SPI_CSN_High();  //10us
    
    return rxbuf[0];
}

void USB_Open_Irq(void)
{
	//IEN1 |= BIT4;
	Write_Sfr_Reg(0xB8, 0x14);
P1_0 = ~P1_0;
}

void USB_Close_Irq(void)
{
	//IEN1 &= (~BIT4);
	Write_Sfr_Reg(0xB8, 0x04);
}

void USB_Write_Fifo(uint8_t EpIdx,uint8_t *FifoBuf,uint8_t FifoBufSize) reentrant
{
    uint8_t i;
    uint8_t cmd = ((CMD_FUNC_CALL<<CMD_SHIFT)&0xe0) | FifoBufSize;
    RF_Operation(FifoBufSize);
    USB_SPI_CSN_Low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(cmd);  //12us 
    txbuf[0] = (((EpIdx&0x03)<<6)&0xc0) | FUNC_USB_WRITE_FIFO;
    usb_spi_rw(txbuf[0]); //16us 
    for(i=0;i<FifoBufSize;i++)
        usb_spi_rw(FifoBuf[i]); //12us 
    USB_SPI_CSN_High();  //304us 
	  Delay10us(25);
}

void USB_Read_Fifo(uint8_t EpIdx,uint8_t *FifoBuf,uint8_t FifoBufSize) reentrant
{
    uint8_t i;
    uint8_t cmd = ((CMD_FUNC_CALL<<CMD_SHIFT)&0xe0) | FifoBufSize;
    
    USB_SPI_CSN_Low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]);  //12us 
    txbuf[0] = (((EpIdx&0x03)<<6)&0xc0) | FUNC_USB_READ_FIFO;
    usb_spi_rw(txbuf[0]); //160us 
    Delay10us(25);
    txbuf[0] = 0xaa;
    for(i=0;i<FifoBufSize;i++)
        FifoBuf[i] = usb_spi_rw(txbuf[0]); //11us 
    USB_SPI_CSN_High();  //10us 
}

void USB_Init(void)
{
    uint8_t reg = 0;
    USB_IRQ_Init();    
    USB_SPI_Init();
#if 1
    Write_Sfr_Reg(0xC8, 0xaa);
	  Delay10ms(1);
    reg = Read_Sfr_Reg(0xC8);
    printf("T2CON = %02x\r\n", (int)reg);
    Write_Sfr_Reg(0xC8, 0x55);
	  Delay10ms(1);
    reg = Read_Sfr_Reg(0xC8);
    printf("T2CON = %02x\r\n", (int)reg);
#endif
}

extern uint8_t usb_interrupt_flag; 
uint8_t gpio_interrupt_flag = 0; 
void HS6206_GPINT_Isr(void) interrupt HS6206_INT_IFP
{
//	uint8_t i;
//	P1_0 = ~P1_0;
//	for(i=2;i<18;i++)
//		txbuf[i] = 0x33+i;
//  //  
//	USB_Read_Fifo(0, rxbuf+2, 16);
//	USB_Write_Fifo(0, txbuf+2, 16);
	gpio_interrupt_flag = 1;
}


