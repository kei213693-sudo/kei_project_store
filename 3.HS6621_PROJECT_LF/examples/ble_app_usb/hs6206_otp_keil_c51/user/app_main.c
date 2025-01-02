/*********************************************************************                                                                   **
**  Device:   HS6206
**  File:		app_main.c
**  Target:
**  Tools:		KEIL
**  Created:	2016-06-04
**  author:  crazy_code@yeah.net
**  Description: 

/*********************************************************************/

#include "HS6206_Types.h"
#include "HS6206_Regs.h"
#include "HS6206_UART.h"
#include "HS6206_Uti.h"
#include "HS6206_USB.h"
#include "string.h"
#include "HS6206_RF.h"
#include <HS6206_SPI.h>
#include "om6621pt_usb_interface.h"

u8 idata spi_rx_buffer[18];
u8 idata spi_tx_buffer[18];
u8 idata cmd_type;
u8 idata cmd_len;
u8 idata spi_rx_len;
u8 *p_spi_rx;

#define sfr_addr  spi_rx_buffer[1]
#define sfr_val   spi_rx_buffer[2]
#define spi_tx_start 2

#if !defined(DEBUG)
int test_printf(const char *format , ...) //reentrant
{
	format = format;
	return 0;
}
#endif

extern u8 IntIn;
extern u8 IntOut;
extern u8 IntUSB;
extern u8 spi_slave_status;
extern void hs6206_write_register(unsigned char register_addr, unsigned char val);
extern unsigned char hs6206_read_register(unsigned char register_addr);      
extern  u8 spi_master_swap_byte(u8 byte);
extern void print_hex(unsigned char u8data);


void func_call_process(uint8_t func_idx)
{
   switch (func_idx) {
       case FUNC_USB_GET_IRQ:
           SPISDAT = IntUSB;
           spi_tx_buffer[spi_tx_start+1] = IntIn;       
           spi_tx_buffer[spi_tx_start+2] = IntOut;       
       break;
       case FUNC_USB_READ_FIFO:
            USB_Read_Fifo((spi_rx_buffer[1]&0xc0)>>6, spi_tx_buffer+spi_tx_start, cmd_len);
            SPISDAT = spi_tx_buffer[spi_tx_start];
       break;
   }       
}


/* callback of CS assert interrupt */
void spi_slave_start_callback(void)
{
    p_spi_rx = &spi_rx_buffer[0];
    spi_rx_len = 0;
}

/* callback of received done interrupt */
void spi_slave_rx_callback(unsigned char val)
{
	/* buffer the received Data */
    spi_rx_buffer[spi_rx_len] = val;
    spi_rx_len++;
    if(spi_rx_len == 1)
    {
       cmd_len = val&0x1f; 
       cmd_type = (val&0xe0)>>5;       
    }
    else if((spi_rx_len == 2))  {
        if(cmd_type == CMD_SFR_READ) 
            SPISDAT = hs6206_read_register(sfr_addr);
        else if(cmd_type == CMD_USB_REG_READ)  {
            spi_tx_buffer[2] = (*((sfr_addr)+&M_REG_BASE_ADDR)); //USB_Read_Reg(sfr_addr);
            SPISDAT = spi_tx_buffer[2];
        }
        else if(cmd_type == CMD_FUNC_CALL) {
            func_call_process(val&0x3f);
        }
	}
    else {
        SPISDAT = spi_tx_buffer[spi_rx_len];
    }
}

/* callback of CS deassert interrupt */
void spi_slave_end_callback(void)
{ 
    //printf("%02x %02x %02x\r\n", (int)spi_rx_buffer[0], (int)spi_rx_buffer[1], (int)spi_rx_buffer[2]);
    //print_hex(spi_rx_buffer[0]);print_hex(spi_rx_buffer[1]);print_hex(spi_rx_buffer[2]);
    switch (cmd_type) {
        case CMD_SFR_WRITE:
            hs6206_write_register(sfr_addr, sfr_val); 
        break;
        case CMD_USB_REG_WRITE:
            *(sfr_addr+&M_REG_BASE_ADDR) = sfr_val; //USB_Write_Reg(sfr_addr, sfr_val); 
        break;
        case CMD_FUNC_CALL:
           switch (spi_rx_buffer[1]&0x3f) {
               case FUNC_USB_WRITE_FIFO:
                 USB_Write_Fifo((spi_rx_buffer[1]&0xc0)>>6, spi_rx_buffer+2, cmd_len);
               break;
               case FUNC_USB_SEND_PKT:
                 Send_USB_Packet((spi_rx_buffer[1]&0xc0)>>6, spi_rx_buffer+2, cmd_len);
               break;
               case FUNC_USB_INIT:
                 USB_Init();
               break;
           } 
        break;
    }
}

/*********************************************************************
* main loop
*********************************************************************/
void main(void)
{
	u8 len=0;
    u8 val=0;
	
	/* program memory: wait value=1, read signal width is 2 cycles, OTP read cycle is 70us, but a cycle of 16MHz is 62.5us, therefore 2 cycles is the minimum */
	CKCON = (CKCON & ~0x70) | 0x10;
	/* MCU runs at 8MHz or 16MHz: divider = 1 */
	CLKCTRL &= ~0x07;

    serial_init();

    printf("hero usb demo is built at %s %s\n", __TIME__, __DATE__);
   
    P1DIR &= ~BIT0;
    P1_0 = 0;

//    spi_slave_setup(SPI_MODE_2);
    SPISCON  = 0x05; //spi mode2, msb, mask all Interrupt
    INTEXP = 0x00;  //close spi slave module Interrupt
    IEN1 = 0x00;

	EA = 1;

	while(1)
	{
        spi_slave_status = SPISSTAT;
        if(spi_slave_status) {
            if (spi_slave_status & SSPI_CS_ASSERT) {
                spi_slave_start_callback();
            }
            if (spi_slave_status & SSPI_XFER_DONE) {
                val = SPISDAT;
                //SPIRDAT = val;
                spi_slave_rx_callback(val);
            }
            if (spi_slave_status & SSPI_CS_DEASSERT) {
                spi_slave_end_callback();
            } 
            spi_slave_status = 0;
        }
    }
}



