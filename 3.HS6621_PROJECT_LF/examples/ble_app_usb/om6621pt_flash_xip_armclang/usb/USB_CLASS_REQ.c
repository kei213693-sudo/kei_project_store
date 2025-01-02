// File Name:  USB_STDREQ.c    
// Author:     shangzhou.hu
// Date:       2013/07/31


#include <stdio.h>
#include <string.h>
#include "USB_Desc.h"
#include "USB_ISR.h"
#include "USB_Regs.h"
#include "USB2SPI.h"

extern  uint8_t g_USB_Ep_Status[6];
extern uint16_t g_USB_DataSize;
extern uint8_t *g_USB_DataPtr;
extern uint8_t ZERO_PACKET[2];
uint8_t uart_set_done = 0;

 uint8_t set_line_code_flag = 0;

//Baud rate: 19200, Stop bits: 1 bit, Parity: None, Data bits: 8bits
uint8_t  Line_Coding[7] = {0x00, 0x4B, 0x00, 0x00, 0x00, 0x00, 0x08};

void Get_Line_Code(void)
{
	 g_USB_DataPtr = (uint8_t*)Line_Coding;
   g_USB_DataSize = 0x07;
	 if (g_USB_Ep_Status[0] != EP_STALL)                // Make sure endpoint not in stall mode
	 {
	   USB_Write_Reg(M_REG_CSR0, CSR0_SOPRDY);          // Service Setup Packet, 清除设备收到数据标志
	   g_USB_Ep_Status[0] = EP_TX;                      // Put endpoint in transmit mode
	   g_USB_DataSent = 0;                              // Reset Data Sent counter
	 }
}

void Set_Control_Line_State(void)
{
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// 返回0长度的状态过程数据包
		USB_Write_Reg(M_REG_CSR0,(CSR0_DATAEND | CSR0_SOPRDY) );
	}

}

void Set_Line_State(void)
{
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Indicate setup packet has been serviced
		USB_Write_Reg(M_REG_CSR0,(CSR0_SOPRDY));
		set_line_code_flag = 1;
		g_USB_Ep_Status[0] = EP_RX;
	}

}
/*----------------------------------End Of File-------------------------------*/
