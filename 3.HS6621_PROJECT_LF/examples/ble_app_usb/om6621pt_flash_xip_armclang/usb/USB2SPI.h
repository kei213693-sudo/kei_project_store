//------------------------------------------------------------------------------
// USB_ISR.h
//------------------------------------------------------------------------------
// Copyright 2013, HunterSun Electronics Co., Ltd.
// Yao
// 2013-07-30
//
// Program Description:
//
//
//

#ifndef ___USB_INTERFACE_H__
#define ___USB_INTERFACE_H__

#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>

uint8_t usb_read_reg(uint8_t Addr);	    

void usb_write_reg(uint8_t Addr, uint8_t AddrData);

uint8_t USB_Is_Suspend_Mode(void); 

void USB_Inactive_Clock(void);   

void USB_Active_Clock(void);

void USB_Set_InMaxP_Size(uint8_t Size); 		

void USB_Set_OutMaxP_Size(uint8_t Size);		

void USB_Ep0_Send_Stall(void);           

void USB_Set_FAddr(uint8_t FAddr);				

void USB_Select_Ep(uint8_t EndpointIdx);		

uint8_t read_sfr_reg(uint8_t addr);

void write_sfr_reg(uint8_t addr, uint8_t val);

void usb_open_interrupt(void);

void usb_close_interrupt(void);

void usb_get_irq(void);

void usb_init(void);

void usb_start(void);

void usb_dp_pullup(void);

void usb_dp_pulldown(void);

void usb_read_fifo(uint8_t EpIdx,uint8_t *FifoBuf,uint8_t FifoBufSize);

void usb_write_fifo(uint8_t EpIdx,uint8_t *FifoBuf,uint8_t FifoBufSize);

void usb_send_packet(uint8_t ep_idx,uint8_t *pkt,uint8_t size);

#define USB_Write_Reg  usb_write_reg
#define USB_Read_Reg   usb_read_reg
#define USB_Write_Fifo usb_write_fifo
#define USB_Read_Fifo  usb_read_fifo

#define PIN_USB_INTERRUPT  17
#define PIN_USB_SPI_MISO   14
#define PIN_USB_SPI_MOSI   16
#define PIN_USB_SPI_SCK    15
#define PIN_USB_SPI_CSN    13

//#define USB_APP_CDC        1

#endif   /*___USB_INTERFACE_H__*/

/*-------------------------------------------End Of File----------------------*/

