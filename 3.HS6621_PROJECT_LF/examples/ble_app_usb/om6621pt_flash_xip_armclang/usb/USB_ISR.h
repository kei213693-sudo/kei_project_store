//------------------------------------------------------------------------------
// USB_ISR.h
//------------------------------------------------------------------------------
// Copyright 2013, crazy_code Electronics Co., Ltd.
// crazy_code
// 2013-07-30
//
// Program Description:
//
//
//

#ifndef __USB_ISR_H__
#define __USB_ISR_H__

#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>

extern uint8_t g_USB_State;
extern  uint8_t g_USB_Ep_Status[6];


// Define device states
#define  DEV_ATTACHED        0x00      // Device is in Attached State
#define  DEV_POWERED         0x01      // Device is in Powered State
#define  DEV_DEFAULT         0x02      // Device is in Default State
#define  DEV_ADDRESS         0x03      // Device is in Addressed State
#define  DEV_CONFIGURED      0x04      // Device is in Configured State
#define  DEV_SUSPENDED       0x05      // Device is in Suspended State

// Define Endpoint States
#define  EP_IDLE             0x00      // This signifies Endpoint Idle State
#define  EP_TX               0x01      // Endpoint Transmit State
#define  EP_RX               0x02      // Endpoint Receive State
#define  EP_HALT             0x03      // Endpoint Halt State (return stalls)
// Endpoint Stall (send procedural stall next status phase)
#define  EP_STALL            0x04
// Endpoint Address (change FADDR during next status phase)
#define  EP_ADDRESS          0x05

/*----------------------------------------------------------------------*/
//extern uint8_t USB_Read_Reg(uint8_t Addr);
//extern void USB_Write_Reg(uint8_t Addr, uint8_t AddrData);

/*----------------------------------------------------------------------*/
//extern void USB_Set_FAddr(uint8_t FAddr);
//extern void USB_Select_Ep(uint8_t EndpointIdx);
extern void USB_Ep0_Send_Stall(void);
//extern void USB_Epx_Send_Stall(void);
extern uint8_t USB_Read_Ep0_Rx_Cnt(void);
extern uint16_t USB_Read_Epx_Rx_Cnt(void);
extern void USB_Generate_Resume(void);
extern void USB_Suspend_Mode_Enable(void);
extern void USB_Suspend_Mode_Disable(void);
extern void USB_Resume_Mode_Disable(void);
extern uint8_t USB_Fifo_Is_Empty(void);
extern 	void Send_USB_Packet(uint8_t EPindex, uint8_t *Packet, uint8_t len);
extern uint8_t Get_USB_Packet(uint8_t EPindex, uint8_t *Packet);

/*-----------------------------------------------------------------------*/
extern void USB_Init(void);

extern void USB_Ep0_ISR(void);


#endif   /*__USB_ISR_H_*/

/*-------------------------------------------End Of File----------------------*/

