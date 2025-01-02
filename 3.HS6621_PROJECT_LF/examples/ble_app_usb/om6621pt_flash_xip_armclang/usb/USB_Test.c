#include <stdio.h>
#include "hs6206_sfr_reg.h"
#include "USB_Regs.h"
#include "USB_ISR.h"
#include "USB_Desc.h"
#include "USB2SPI.h"

#define IN_PACKET_SIZE 5
unsigned char IN_PACKET[IN_PACKET_SIZE];

void build_mouse_report(uint8_t button,uint8_t x, uint8_t y, uint8_t vwheel, uint8_t hwheel)
{
  IN_PACKET[0] = button;
  IN_PACKET[1] = x;
  IN_PACKET[2] = y;
  IN_PACKET[3] = vwheel;
  IN_PACKET[4] = hwheel;
}

/**
 * Relative mouse demo's report data is sent via endpoint 1.
 *
 * @param ReportID Report Data's ID, which is used for multi-report
 *
 * This function can be called by other routines to force an IN packet
 * transmit.  It takes as an input the Report ID of the packet to be
 * transmitted.
 */
void send_packet(unsigned char report_id)
{
//   bit EAState;
   unsigned char ControlReg;

   usb_close_interrupt();

   USB_Write_Reg(M_REG_INDEX,1);        // Set index to endpoint 1 registers

   // Read contol register for EP 1
   ControlReg = USB_Read_Reg(M_REG_INCSR1);

   if (g_USB_Ep_Status[1] == EP_HALT)        // If endpoint is currently halted,
                                       // send a stall
   {
     USB_Write_Reg (M_REG_INCSR1, INCSR_SDSTL);
   }

//   else if(g_USB_Ep_Status[1] == EP_IDLE)
   {
      // the state will be updated inside the ISR handler
      g_USB_Ep_Status[1] = EP_TX;

      if (ControlReg & INCSR_STSTL)      // Clear sent stall if last
                                       // packet returned a stall
      {
         USB_Write_Reg (M_REG_INCSR1, INCSR_CLRDT);
      }

      if (ControlReg & INCSR_UNDRUN)     // Clear underrun bit if it was set
      {
         USB_Write_Reg (M_REG_INCSR1, 0x00);
      }

      /* FIXME: add ReportID support here to
	     get different IN_PACKET and its size */
      report_id = report_id;

      // Put new data on Fifo
      USB_Write_Fifo(0x01, IN_PACKET, IN_PACKET_SIZE);
      USB_Write_Reg (M_REG_INCSR1, INCSR_IPRDY);
                                       // Set In Packet ready bit,
   }                                   // indicating fresh data on FIFO 1

   usb_open_interrupt();
}