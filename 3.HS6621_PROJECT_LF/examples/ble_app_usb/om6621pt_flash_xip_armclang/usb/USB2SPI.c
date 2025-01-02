// File Name:  USB_STDREQ.c    
// Author:     Yao@HunterSun Electronics Co.,Ltd. 
// Date:       2013/07/31


#include <stdio.h>
#include <string.h>
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include "hs6206_sfr_reg.h"
#include "peripheral.h"
#include "USB_Regs.h"
#include "om6621pt_usb_interface.h"
#include "co_delay.h"

extern uint8_t usb_spi_rw(uint8_t value);
extern void usb_irq_init(void);
extern void usb_spi_init(void);
extern void usb_spi_csn_low(void);
extern void usb_spi_csn_high(void);
static uint8_t txbuf[18], rxbuf[18];

#define delay_ms co_delay_ms
#define delay_us co_delay_us

uint8_t interrupt_in;
uint8_t interrupt_out;
uint8_t interrupt_usb;

void _delay_cycle(uint32_t cycle)
{
    for(int i=0;i<cycle;i++) __NOP(); 
}

uint8_t usb_read_reg(uint8_t addr)	    
{
    uint8_t cmd = ((CMD_USB_REG_READ<<CMD_SHIFT)&0xe0) | 1;
    
    usb_spi_csn_low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]); delay_us(50);//12us 
    txbuf[0] = addr;
    usb_spi_rw(txbuf[0]);delay_us(50); //20us 
	  
    txbuf[0] = 0xaa;
    rxbuf[0] = usb_spi_rw(txbuf[0]);delay_us(50); //12us    
    usb_spi_csn_high(); delay_us(50);//9us  
    
    return rxbuf[0];
}
void usb_write_reg(uint8_t addr, uint8_t val)
{
    uint8_t cmd = ((CMD_USB_REG_WRITE<<CMD_SHIFT)&0xe0) | 1;

    usb_spi_csn_low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]); delay_us(50);//12us 
    txbuf[0] = addr;
    usb_spi_rw(txbuf[0]); delay_us(50);//14us
    txbuf[0] = val;
    usb_spi_rw(val);  delay_us(50);//12us   
    usb_spi_csn_high(); delay_us(50);//12us 
}
uint8_t USB_Is_Suspend_Mode() 
{  
    return (usb_read_reg(M_REG_POWER) | USB_RSTMD);
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
	usb_write_reg(M_REG_INMAXP,Size);
}
void USB_Set_OutMaxP_Size(uint8_t Size)		
{
	usb_write_reg(M_REG_OUTMAXP,Size);
}

void USB_Set_FAddr(uint8_t FAddr)				
{
    usb_write_reg(M_REG_FADDR,FAddr);
}
void USB_Select_Ep(uint8_t EndpointIdx)		
{
	usb_write_reg(M_REG_INDEX,EndpointIdx);
}

void write_sfr_reg(uint8_t addr, uint8_t val)
{
    uint8_t cmd = ((CMD_SFR_WRITE<<CMD_SHIFT)&0xe0) | 1;

    usb_spi_csn_low();  //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]);delay_us(50); //12us
    txbuf[0] = addr;
    usb_spi_rw(txbuf[0]); delay_us(50); //15us
    txbuf[0] = val;
    usb_spi_rw(val);  //12us  
    usb_spi_csn_high(); delay_us(150);//126us
}

void usb_get_irq(void)
{
    uint8_t cmd = ((CMD_FUNC_CALL<<CMD_SHIFT)&0xe0) | 3;
    
    usb_spi_csn_low();  //9us
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]);  delay_us(5);//12us
    txbuf[0] = FUNC_USB_GET_IRQ;
    usb_spi_rw(txbuf[0]); delay_us(5);//18us
    txbuf[0] = 0xaa;
    interrupt_usb = usb_spi_rw(txbuf[0]); //13us
    delay_us(5);	
    interrupt_in = usb_spi_rw(txbuf[0]); //13us
	delay_us(5);
    interrupt_out = usb_spi_rw(txbuf[0]); //13us
    usb_spi_csn_high();           //10us
   
}

uint8_t read_sfr_reg(uint8_t addr)
{
    uint8_t cmd = ((CMD_SFR_READ<<CMD_SHIFT)&0xe0) | 1;
    
    usb_spi_csn_low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]); delay_us(50); //12us
    txbuf[0] = addr;
    usb_spi_rw(txbuf[0]); delay_us(162);//175us
    txbuf[0] = 0xaa;
    rxbuf[0] = usb_spi_rw(txbuf[0]);  //15us     
    usb_spi_csn_high();  //10us
    
    return rxbuf[0];
}

void usb_open_interrupt(void)
{
    //IEN1 |= BIT4;
    write_sfr_reg(IEN1, 0x10);

}

void usb_close_interrupt(void)
{
    //IEN1 &= (~BIT4);
    write_sfr_reg(IEN1, 0x00);
}

void usb_dp_pullup(void)
{
    /* pull up D+, then host can detect me and start to enumerate */
    //USBPCON1 |= BIT4;  
    write_sfr_reg(USBPCON1, 0x90);    
}

void usb_dp_pulldown(void)
{
    /* pull down D+, then usb device disconnect to host */
    //USBPCON1 &= ~BIT4;  
    write_sfr_reg(USBPCON1, 0x80);
}

void usb_start(void)
{
    uint8_t i;
    uint8_t cmd = ((CMD_FUNC_CALL<<CMD_SHIFT)&0xe0) | 1;

    usb_spi_csn_low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(cmd); delay_us(1);  //12us 
    txbuf[0] = FUNC_USB_INIT;
    usb_spi_rw(txbuf[0]);delay_us(2);  //16us 
    usb_spi_csn_high();  
    delay_ms(1);
}

void usb_send_packet(uint8_t ep_idx,uint8_t *pkt,uint8_t size)
{
    uint8_t i;
    uint8_t cmd = ((CMD_FUNC_CALL<<CMD_SHIFT)&0xe0) | size;

    usb_spi_csn_low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(cmd); delay_us(1);  //12us 
    txbuf[0] = (((ep_idx&0x03)<<6)&0xc0) | FUNC_USB_SEND_PKT;
    usb_spi_rw(txbuf[0]);delay_us(2);  //16us 
    for(i=0;i<size;i++)
       { usb_spi_rw(pkt[i]); delay_us(1); }//12us 
    usb_spi_csn_high();  //304us 
	delay_us(304);
}

void usb_write_fifo(uint8_t ep_idx,uint8_t *fifo_buf,uint8_t fifo_bufSize)
{
    uint8_t i;
    uint8_t cmd = ((CMD_FUNC_CALL<<CMD_SHIFT)&0xe0) | fifo_bufSize;

    usb_spi_csn_low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(cmd); delay_us(1);  //12us 
    txbuf[0] = (((ep_idx&0x03)<<6)&0xc0) | FUNC_USB_WRITE_FIFO;
    usb_spi_rw(txbuf[0]);delay_us(2);  //16us 
    for(i=0;i<fifo_bufSize;i++)
       { usb_spi_rw(fifo_buf[i]); delay_us(1); }//12us 
    usb_spi_csn_high();  //304us 
	delay_us(304);
}

void usb_read_fifo(uint8_t ep_idx,uint8_t *fifo_buf,uint8_t fifo_bufSize)
{
    uint8_t i;
    uint8_t cmd = ((CMD_FUNC_CALL<<CMD_SHIFT)&0xe0) | fifo_bufSize;
    
    usb_spi_csn_low(); //9us 
    txbuf[0] = cmd;
    usb_spi_rw(txbuf[0]);  delay_us(1); //12us 
    txbuf[0] = (((ep_idx&0x03)<<6)&0xc0) | FUNC_USB_READ_FIFO;
    usb_spi_rw(txbuf[0]); delay_us(160); //160us 
    delay_us(160);
    txbuf[0] = 0xaa;
    for(i=0;i<fifo_bufSize;i++) {
        fifo_buf[i] = usb_spi_rw(txbuf[0]); delay_us(1); } //11us 
    usb_spi_csn_high();  //10us 
}

void usb_init(void)
{
    uint8_t reg = 0;
    usb_irq_init();    
    usb_spi_init();
#if 1
    write_sfr_reg(0xC8, 0xaa);
	delay_ms(10);
    reg = read_sfr_reg(0xC8);
    printf("T2CON = %02x\r\n", (int)reg);
    write_sfr_reg(0xC8, 0x55);
	delay_ms(10);
    reg = read_sfr_reg(0xC8);
    printf("T2CON = %02x\r\n", (int)reg);
#endif
    usb_start();
}

/**
 * Relative mouse demo's data structure.
 * This HID report should be changed in custom HID design.
 */


void usb_interrupt_service(uint32_t pin_mask)
{
    usb_get_irq();
    
    if(interrupt_usb & INT_RSU) USB_Resume_ISR();

    if(interrupt_usb & INT_RST) USB_Rst_ISR();

    if(interrupt_in & INT_EP0)
    {
        //prints("USB_Ep0_ISR\r\n");
        USB_Ep0_ISR();			/*endpoint 0 interrupt service routine*/
    }

    if(interrupt_in & INT_EP1) USB_Ep1_In_ISR();
    if(interrupt_in & INT_EP2) USB_Ep2_In_ISR();
    //if(interrupt_in & INT_EP3) USB_Ep3_In_ISR();
    if(interrupt_out & INT_EP3) USB_Ep3_Out_ISR();

    if(interrupt_usb & INT_SUS) USB_Suspend_ISR();
    if(interrupt_usb & INT_SOF) USB_Sof_ISR();
    interrupt_usb = interrupt_in = interrupt_out = 0;
    usb_open_interrupt();
}


