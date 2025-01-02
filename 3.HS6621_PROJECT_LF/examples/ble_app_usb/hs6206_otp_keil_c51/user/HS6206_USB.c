//------------------------------------------------------------------------------------
// rw_sfr.c
//------------------------------------------------------------------------------------
// Copyright 2022, OnMicro Electronics Co., Ltd.
// shangzhou.hu
// 2022-04-05
//
// Program Description:
//
//
//


#include <stdio.h>
#include <string.h>
#include "HS6206_Regs.h"
#include "HS6206_Types.h"
#include "HS6206_Delay.h"
#include "HS6206_USB.h"
#include "HS6206_RF.h"

U8 IntIn;
U8 IntOut;
U8 IntUSB;
U8 usb_interrupt_flag = 0;

volatile unsigned char xdata M_REG_BASE_ADDR  _at_ 0xC000;           /*USB base address*/

U8 USB_Read_Reg(U8 Addr) reentrant	                  
{
    return (*((Addr)+&M_REG_BASE_ADDR));
}

void USB_Write_Reg(U8 Addr, U8 AddrData)  reentrant		 
{
    *(Addr+&M_REG_BASE_ADDR) = AddrData;
}

void USB_Inactive_Clock()
{
    USBCON &= ~BIT6;
}
void USB_Active_Clock()
{
    USBCON |= BIT6;
}


void USB_Ep0_Send_Stall(void)
{
  USB_Write_Reg(M_REG_INDEX, 0);
  USB_Write_Reg(M_REG_CSR0, CSR0_SDSTL);
}

void USB_Resume_Mode_Enable(void)
{
	U8 Power_Reg;
	/* FIXME: resume USB must be cleared before enable suspend mode */
	USB_Inactive_Clock();

	Power_Reg=USB_Read_Reg(M_REG_POWER);
	Power_Reg |= USB_RESUME;  //enable resume mode
	USB_Write_Reg(M_REG_POWER,Power_Reg);
}

void USB_Resume_Mode_Disable(void)
{
	U8 Power_Reg;
	/* FIXME: resume USB must be cleared before enable suspend mode */
	USB_Inactive_Clock();

	Power_Reg=USB_Read_Reg(M_REG_POWER);
	Power_Reg &= ~USB_RESUME;  //disable resume mode
	USB_Write_Reg(M_REG_POWER,Power_Reg);
}

void USB_Suspend_Mode_Enable(void)
{
	U8 Power_Reg;
	/* FIXME: wakeup USB must be cleared before enable suspend mode */
	USB_Inactive_Clock();

	Power_Reg=USB_Read_Reg(M_REG_POWER);
	Power_Reg |= USB_SUSEN;  //enable suspend mode
	USB_Write_Reg(M_REG_POWER,Power_Reg);
}

/* If the  USB is in Suspend mode and the software wants to initiate a remote wakeup, it should write to the  sw_wu to 
enable the 48M PLL. It will take 100us to get the PLL stable then software need to  write to the Power register   set 
the Resume bit (D2) to 1 .  The software should leave this bit set for approximately 10 ms (minimum of 2ms, a
maximum of 15ms) then reset it to 0. */
void USB_Generate_Resume(void)
{
	U8 Power_Reg;

	USB_Active_Clock();
	delay_us(150);          //wait PLL become stable

	Power_Reg=USB_Read_Reg(M_REG_POWER);
	Power_Reg |= USB_RESUME | USB_SUSEN;
	USB_Write_Reg(M_REG_POWER,Power_Reg);
	delay_ms(10);           //2~15ms
	Power_Reg &= ~USB_RESUME;
	USB_Write_Reg(M_REG_POWER,Power_Reg);
}


//read Fifo
void USB_Read_Fifo(U8 EpIdx,U8 *FifoBuf,U8 FifoBufSize) reentrant
{
  U8 i;
  for(i=0x00;i<FifoBufSize;i++) FifoBuf[i]=USB_Read_Reg( M_REG_FIFOx(EpIdx) );
}

void USB_Write_Fifo(U8 EpIdx,U8 *FifoBuf,U8 FifoBufSize) reentrant
{
  U8 i;
  for(i=0x00;i<FifoBufSize;i++) USB_Write_Reg( M_REG_FIFOx(EpIdx ), FifoBuf[i] );
}

U8 USB_Fifo_Is_Empty(void)
{
  return ( !(USB_Read_Reg(M_REG_INCSR1) & INCSR_FIFO_NOT_EMPTY) );
}

static void ctrllr_init(void)
{
    USB_Suspend_Mode_Enable();

    /*interrupt enable*/
    //USB_Write_Reg( M_REG_INTRUSBE,0x0F );  //enable suspend, SOF, reset and resume interrupt.
    //USB_Write_Reg( M_REG_INTRIN1E,(BIT0+BIT1+BIT2+BIT3) );  //Enable EP0 EP1 BIT2 BIT3
    // @{add by myself
    USB_Write_Reg( M_REG_INTRUSBE,0x04 ); //only enable reset interrupt
    //USB_Write_Reg( M_REG_INTRIN1E,(BIT0+BIT1+BIT2));
    //USB_Write_Reg( M_REG_INTROUT1E,(BIT3));
    //}@
    USB_Write_Reg(M_REG_INDEX,0x01);
    USB_Write_Reg(M_REG_INMAXP,USB_EP1_FIFO_SIZE);//USB_Set_InMaxP_Size(USB_EP1_FIFO_SIZE); 
    USB_Write_Reg(M_REG_INDEX,0x00);
    USB_Write_Reg(M_REG_INMAXP,USB_EP0_FIFO_SIZE);
    // @{add by myself
    //USB_Write_Reg(M_REG_INDEX,0x02);//端点2为输入
    //USB_Set_InMaxP_Size(USB_EP2_FIFO_SIZE);
    //USB_Write_Reg(M_REG_INDEX,0x03);//端点3为输出
    //USB_Set_OutMaxP_Size(USB_EP3_FIFO_SIZE); 
    //USB_Write_Reg(M_REG_INDEX,0x00);
    //}@
}

void USB_Init(void)
{
	U8 temp;

	PWRDWN |= BIT3;   //USB 提供CLK, enable usb
	USBCON &= ~BIT4;   //DP,DM uses normal pins

	USBPCON &= ~BIT0; //enable USB single-end data input (usb_ndie)
	USBPCON1 &= ~(BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0); //disable all pull up/down

	/* software reset USB controller & PHY */
	USBCON |= BIT7;//usb software reset, assert reset
	USBCON &= ~BIT5; //48MHz clock, reset
	delay_ms(15);
	USBCON &= ~BIT7;//usb software reset, de-assert reset
	USBCON |= BIT5; //

	USBCON |= BIT3;	 //diagal enable the 48m clock  

	ctrllr_init();

	IEN1 |= BIT4;   /*USB interrupt enable*/
	//IEN1 |= BIT3;   /*USB wakeup interrupt enable*/
	EA=1;

	/* pull up D+, then host can detect me and start to enumerate */
	USBPCON1 |= BIT4;

	RF_Bank_Switch(RF_Bank0);
	temp = RF_read_byte(RF_BANK0_FEATURE); 
	temp |= 0x10;
	RF_write_byte(RF_BANK0_FEATURE, temp);  // analog  enable the 48m clock  
}


/**
 * send data to host
 *
 * This function can be called by other routines to force an IN packet
 * transmit.  It takes as an input the Report ID of the packet to be
 * transmitted.
 */
void Send_USB_Packet(u8 EPindex, u8 *Packet, u8 len)
{
    unsigned char ControlReg;

    EA = 0;
    
    USB_Write_Reg(M_REG_INDEX,EPindex); 			 // Set index to endpoint 1 registers

    // Read contol register for EP 1
    ControlReg = USB_Read_Reg(M_REG_INCSR1);

    if (ControlReg & INCSR_STSTL) 		 // Clear sent stall if last
    // packet returned a stall
    {
        USB_Write_Reg (M_REG_INCSR1, INCSR_CLRDT);
    }

    if (ControlReg & INCSR_UNDRUN)		 // Clear underrun bit if it was set
    {
        USB_Write_Reg (M_REG_INCSR1, 0x00);
    }

    // Put new data on Fifo
    USB_Write_Fifo(EPindex, Packet, len);
    USB_Write_Reg (M_REG_INCSR1, INCSR_IPRDY);
    // Set In Packet ready bit,

    EA = 1;
    
}


void USB_ISR(void) interrupt 0x0C
{
    P1_0 = 1;
    IntUSB = USB_Read_Reg(M_REG_INTRUSB); 
    IntIn  = USB_Read_Reg(M_REG_INTRIN1);
    IntOut = USB_Read_Reg(M_REG_INTROUT1);
    Close_USB_IRQ();
    P1_0 = 0; 
}

/*-------------------------------------------End Of File---------------------------------------------*/


