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

uint8_t USB_Read_Reg(uint8_t Addr) ;	    

void USB_Write_Reg(uint8_t Addr, uint8_t AddrData) ;

uint8_t USB_Is_Suspend_Mode(void); 

void USB_Inactive_Clock(void);   

void USB_Active_Clock(void);

void USB_Set_InMaxP_Size(uint8_t Size); 		

void USB_Set_OutMaxP_Size(uint8_t Size);		

void USB_Ep0_Send_Stall(void);           

void USB_Set_FAddr(uint8_t FAddr);				

void USB_Select_Ep(uint8_t EndpointIdx);		

uint8_t Read_Sfr_Reg(uint8_t addr);

void Write_Sfr_Reg(uint8_t addr, uint8_t val);

void USB_Open_Irq(void);

void USB_Close_Irq(void);

#endif   /*___USB_INTERFACE_H__*/

/*-------------------------------------------End Of File----------------------*/

