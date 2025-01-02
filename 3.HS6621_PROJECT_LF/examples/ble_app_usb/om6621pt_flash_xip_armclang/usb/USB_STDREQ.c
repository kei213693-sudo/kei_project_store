// File Name:  USB_STDREQ.c    
// Author:     crazy_code
// Date:       2013/07/31


#include <stdio.h>
#include <string.h>
#include "hs6206_sfr_reg.h"
#include "USB_Desc.h"
#include "USB_ISR.h"
#include "USB_Regs.h"
#include "USB2SPI.h"

struct usb_setup_t g_USB_Setup;
uint16_t g_USB_DataSize;
uint8_t g_sys_alternate=0;
uint8_t usb_device_status=0;
uint8_t bwakeup_usb_enable=0;
uint8_t Mouse_Current_Protocol=1;    //default report procol
uint8_t Keyboard_Current_Protocol=1; //default report procol
uint8_t Control_Idle=0;
uint8_t Keyboard_Idle=0;
uint8_t Mouse_Idle=0;
uint8_t Define_Idle=0;
uint8_t Set_report_flag=0;
uint8_t g_USB_DataBuf[IP_FIFO_SIZE];
uint8_t *g_USB_DataPtr = g_USB_DataBuf;
uint16_t g_USB_DataSent;

uint8_t ZERO_PACKET[2] = {0x00, 0x00};
uint8_t ONES_PACKET[2] = {0x01, 0x00};

/**
 * Set new function address.
 * Standard request that should not change in custom HID designs.
 */
void Set_Address(void)                          
{
	if ((g_USB_Setup.bmRequestType != STD_DIR_OUT)     ||// Request must be directed to device
	g_USB_Setup.wIndex.c[MSB]  || g_USB_Setup.wIndex.c[LSB]||// with index and length set to zero.
	g_USB_Setup.wLength.c[MSB] || g_USB_Setup.wLength.c[LSB]||
	g_USB_Setup.wValue.c[LSB]  || (g_USB_Setup.wValue.c[MSB] & 0x80))
	{
		USB_Ep0_Send_Stall();
	}

	g_USB_Ep_Status[0] = EP_ADDRESS;                   // Set endpoint zero to update address next status phase
	if (g_USB_Setup.wValue.c[MSB] != 0)
	{      
		g_USB_State = DEV_ADDRESS;                  // Indicate that device state is now address
	}
	else
	{
		g_USB_State = DEV_DEFAULT;                  // If new address was 0x00, return device to default
	}                                            // state
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Indicate setup packet has been serviced
		USB_Write_Reg(M_REG_INDEX, 0);
		//printf("M_REG_CSR0 = %bx\r\n", USB_Read_Reg(M_REG_CSR0));
		//Caution: Read M_REG_CSR0, and the value is 0x01//
		//USB_Write_Reg(M_REG_CSR0,(CSR0_DATAEND | CSR0_SOPRDY) );
		USB_Write_Reg(M_REG_CSR0,(CSR0_DATAEND | CSR0_INPRDY | CSR0_SOPRDY));//I think that we should set the InPktRdy bit
	}
}

/**
 * This routine alows host to change current device configuration value.
 * Standard request that should not change in custom HID designs.
 */
void Set_Configuration(void)                   
{    
	// Device must be addressed before configured     
	// and request recipient must be the device 
	// the index and length words must be zero 
	// This software only supports config = 0,1
	if ((g_USB_State == DEV_DEFAULT) || (g_USB_Setup.bmRequestType != STD_DIR_OUT) || (g_USB_Setup.wIndex.c[MSB]) || (g_USB_Setup.wIndex.c[LSB]) || (g_USB_Setup.wLength.c[MSB]) || (g_USB_Setup.wLength.c[LSB]) || (g_USB_Setup.wValue.c[LSB]) || (g_USB_Setup.wValue.c[MSB] > 1))        
	{
		USB_Ep0_Send_Stall();
	}
	else
	{
		if (g_USB_Setup.wValue.c[MSB] > 0)              // Any positive configuration request
		{                                         // results in configuration being set to 1
			g_USB_State = DEV_CONFIGURED;
			g_USB_Ep_Status[1] = EP_IDLE;                // Set endpoint status to idle (enabled)
			g_USB_Ep_Status[2] = EP_IDLE;
			g_USB_Ep_Status[3] = EP_IDLE;
			g_USB_Ep_Status[4] = EP_IDLE;
			g_USB_Ep_Status[5] = EP_IDLE;
			USB_Write_Reg( M_REG_INTRIN1E,(BIT0+BIT1+BIT2));
			USB_Write_Reg( M_REG_INTROUT1E,(BIT3));
			USB_Write_Reg(M_REG_INDEX, 1);             // Change index to endpoint 1
			USB_Write_Reg(M_REG_INCSR2,INCSR_DIR_IN);  // Set DIRSEL to indicate endpoint 1 is IN
			USB_Write_Reg(M_REG_INDEX, 2);             // Change index to endpoint 1
			USB_Set_InMaxP_Size(USB_EP2_FIFO_SIZE);
			USB_Write_Reg(M_REG_INCSR2,INCSR_DIR_IN);  // Set DIRSEL to indicate endpoint 1 is IN
			USB_Write_Reg(M_REG_INCSR1,(BIT3+BIT6));//Flush the FIFO
			USB_Write_Reg(M_REG_INDEX, 3);             // Change index to endpoint 1
			USB_Set_OutMaxP_Size(USB_EP3_FIFO_SIZE); 
			//USB_Write_Reg(M_REG_OUTCSR2,0x00);  // Set DIRSEL to indicate endpoint 1 is IN
			USB_Write_Reg(M_REG_INCSR2,0x00); 
			USB_Write_Reg(M_REG_OUTCSR1,(BIT4+BIT7));//Flush the FIFO
			USB_Write_Reg(M_REG_INDEX, 0);            // Set index back to endpoint 0
		}
		else
		{
			g_USB_State = DEV_ADDRESS;               // Unconfigures device by setting state to
			g_USB_Ep_Status[1] = EP_HALT;                // address, and changing endpoint 1 and 2
			g_USB_Ep_Status[2] = EP_HALT;                // status to halt
			g_USB_Ep_Status[3] = EP_HALT;
			g_USB_Ep_Status[4] = EP_HALT;
			g_USB_Ep_Status[5] = EP_HALT;	
		}
	}
	
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Indicate setup packet has been serviced
		USB_Write_Reg(M_REG_CSR0, (CSR0_SOPRDY | CSR0_DATAEND));
	}
}

//-----------------------------------------------------------------------------
// Get_Interface
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - Non
//
// Standard request that should not change in custom HID designs.
//
//-----------------------------------------------------------------------------
void Get_Interface (void)              // This routine returns 0x00, since
{                                      // only one interface is supported by
	// this firmware

	if (g_USB_Setup.bmRequestType == 0x81) 
	{
		// Otherwise, return interface setting value to host
		if(g_USB_Setup.wIndex.c[MSB] == 0x00)           //interface 0
		{
			g_USB_DataPtr[0] = g_sys_alternate;
		}

		if(g_USB_Setup.wIndex.c[MSB] == 0x01)           //interface 1
		{
			g_USB_DataPtr[0]  = g_sys_alternate;
		}

		if(g_USB_Setup.wIndex.c[MSB] == 0x02)           //interface 2
		{
			g_USB_DataPtr[0] = g_sys_alternate;
		}

		g_USB_DataSize = 1;
	}
	else
	{
		USB_Ep0_Send_Stall();
	}

	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Set Serviced SETUP packet, put endpoint in transmit mode and reset
		// Data sent counter
		USB_Write_Reg(M_REG_CSR0, CSR0_SOPRDY);          // Service Setup Packet
		g_USB_Ep_Status[0] = EP_TX;                      // Put endpoint in transmit mode
		g_USB_DataSent = 0;                              // Reset Data Sent counter
	}
}

//-----------------------------------------------------------------------------
// Set_Interface
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
//-----------------------------------------------------------------------------
void Set_Interface (void)
{
	if (g_USB_Setup.bmRequestType == 0x01) 	  
	{
		if(g_USB_Setup.wIndex.c[MSB] == 0x00)           //interface 0
		{
			g_sys_alternate = g_USB_Setup.wValue.c[LSB];
			g_USB_Ep_Status[1] = EP_IDLE;
		}

		if(g_USB_Setup.wIndex.c[MSB] == 0x01)           //interface 1
		{
			g_sys_alternate = g_USB_Setup.wValue.c[LSB];
			g_USB_Ep_Status[2] = EP_IDLE;
		}

		if(g_USB_Setup.wIndex.c[MSB] == 0x02)           //interface 2
		{
			g_sys_alternate = g_USB_Setup.wValue.c[LSB];
			g_USB_Ep_Status[3] = EP_IDLE;
		}
		g_USB_DataPtr = (unsigned char*)&ZERO_PACKET;
		g_USB_DataSize = 2;
	}
	else
	{
		USB_Ep0_Send_Stall(); 
	}

	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Indicate setup packet has been serviced
		USB_Write_Reg(M_REG_CSR0,(CSR0_DATAEND | CSR0_SOPRDY) );
	}
}

//-----------------------------------------------------------------------------
// Get_Configuration
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
//-----------------------------------------------------------------------------
void Get_Configuration (void)          // This routine returns current
{                                      // configuration value
	// This request must be directed to the device
	if ( (g_USB_Setup.bmRequestType != 0x80)    ||
	// With value word set to zero
	g_USB_Setup.wValue.c[MSB]  || g_USB_Setup.wValue.c[LSB]||
	// And index set to zero
	g_USB_Setup.wIndex.c[MSB]  || g_USB_Setup.wIndex.c[LSB]||
	// And g_USB_Setup length set to one
	g_USB_Setup.wLength.c[LSB] || (g_USB_Setup.wLength.c[MSB] != 1) )
	{
		USB_Ep0_Send_Stall ();                  // Otherwise send a stall to host
	}
	else
	{
		if (g_USB_State == DEV_CONFIGURED)// If the device is configured, then
		{                                // return value 0x01 since this software
			// only supports one configuration
			g_USB_DataPtr = (unsigned char*)&ONES_PACKET;
			g_USB_DataSize = 1;
		}
		
		if (g_USB_State == DEV_ADDRESS)   // If the device is in address state, it
		{                                // is not configured, so return 0x00
			g_USB_DataPtr = (unsigned char*)&ZERO_PACKET;
			g_USB_DataSize = 1;
		}
	}
	
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Set Serviced Out Packet bit
		USB_Write_Reg(M_REG_CSR0, CSR0_SOPRDY);          // Service Setup Packet
		g_USB_Ep_Status[0] = EP_TX;                      // Put endpoint in transmit mode
		g_USB_DataSent = 0;                              // Reset Data Sent counter
	}
}


//-----------------------------------------------------------------------------
// Get_Status
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change for custom HID designs.
//
// ----------------------------------------------------------------------------
void Get_Status (void)                 // This routine returns a two byte
{                                      // status packet to the host

	static uint8_t status[2]={0x00,0x00};

	if (g_USB_Setup.wValue.c[MSB] || g_USB_Setup.wValue.c[LSB] ||
	// If non-zero return length or data
	// length not
	g_USB_Setup.wLength.c[LSB]    || (g_USB_Setup.wLength.c[MSB] != 2))
	// Equal to 2 then send a stall
	{                                   // indicating invalid request
		USB_Ep0_Send_Stall ();
	}

	switch(g_USB_Setup.bmRequestType)         // Determine if recipient was device,
	{								   // interface, or EP
		case 0x80:                 // If recipient was device
			if (g_USB_Setup.wIndex.c[MSB] || g_USB_Setup.wIndex.c[LSB])
			{
				USB_Ep0_Send_Stall ();            // Send stall if request is invalid
			}
			else
			{	 
				status[0] = usb_device_status;
				status[1] = 0x00;
				g_USB_DataPtr = status;
				g_USB_DataSize = 2;
			}
			break;

		case 0x81:              // See if recipient was interface
			if ((g_USB_State != DEV_CONFIGURED) ||
			g_USB_Setup.wIndex.c[MSB] || g_USB_Setup.wIndex.c[LSB])
			// Only valid if device is configured
			// and non-zero index
			{
				USB_Ep0_Send_Stall ();            // Otherwise send stall to host
			}
			else
			{
				// Status packet always returns 0x00
				g_USB_DataPtr = (unsigned char*)&ZERO_PACKET;
				g_USB_DataSize = 2;
			}
			break;

		case 0x82:               // See if recipient was an endpoint
			if ((g_USB_State != DEV_CONFIGURED) ||
			g_USB_Setup.wIndex.c[LSB])          // Make sure device is configured
			// and index msb = 0x00
			{                             // otherwise return stall to host
				USB_Ep0_Send_Stall();
			}
			else
			{
				// Handle case if request is directed to EP 1
				if (g_USB_Setup.wIndex.c[MSB] !=0 )
				{
					if (g_USB_Ep_Status[g_USB_Setup.wIndex.c[MSB]&0x0f] == EP_HALT)
					{                       // If endpoint is halted,
						// return 0x01,0x00
						g_USB_DataPtr = (unsigned char*)&ONES_PACKET;
						g_USB_DataSize = 2;
					}
					else
					{
						// Otherwise return 0x00,0x00 to indicate endpoint active
						g_USB_DataPtr = (unsigned char*)&ZERO_PACKET;
						g_USB_DataSize = 2;
					}
				}
				else
				{
					USB_Ep0_Send_Stall ();         // Send stall if unexpected data
					// encountered
				}
			}
			break;

		default:
			USB_Ep0_Send_Stall ();
			break;
	}
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Set serviced g_USB_Setup Packet, Endpoint 0 in transmit mode, and
		// reset DATASENT counter
		USB_Write_Reg(M_REG_CSR0, CSR0_SOPRDY);          // Service Setup Packet
		g_USB_Ep_Status[0] = EP_TX;                      // Put endpoint in transmit mode
		g_USB_DataSent = 0;                              // Reset Data Sent counter
	}
}

//-----------------------------------------------------------------------------
// Clear_Feature
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
//-----------------------------------------------------------------------------
void Clear_Feature ()                  // This routine can clear Halt Endpoint
{                                      // features on endpoint 1
	if(g_USB_Setup.bmRequestType == 0x00)	   //device remote wakeup
	{
		if(g_USB_Setup.wValue.c[MSB] == 0x01) 
		{
			usb_device_status &=(~0x02);
			bwakeup_usb_enable = 0;
			USB_Resume_Mode_Disable();
		}
		else
		{
			USB_Ep0_Send_Stall ();               // Send procedural stall
		}
	}
	
	else if(g_USB_Setup.bmRequestType == 0x02)		//not clear feature device
	{
		if(g_USB_Setup.wIndex.c[MSB] !=0) 
		{
			g_USB_Ep_Status[g_USB_Setup.wIndex.c[MSB]&0x0f] = EP_IDLE;
		}
		else
		{
			USB_Ep0_Send_Stall ();               // Send procedural stall
		}
	}
	else
	{
	 	USB_Ep0_Send_Stall ();               // Send procedural stall
	}

	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Indicate setup packet has been serviced
		USB_Write_Reg(M_REG_CSR0,(CSR0_DATAEND | CSR0_SOPRDY) );
	}

}


//-----------------------------------------------------------------------------
// Set_Feature
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
//-----------------------------------------------------------------------------
void Set_Feature (void)                // This routine will set the EP Halt
{                                      // feature for endpoint 1

	if(g_USB_Setup.bmRequestType == 0x00)	   //device remote wakeup
	{
		if(g_USB_Setup.wValue.c[MSB] == 0x01) 
		{
			usb_device_status = usb_device_status&0xfd;
			usb_device_status |=(1<<1);
			bwakeup_usb_enable = 1;	
			USB_Suspend_Mode_Enable();  
			g_USB_DataPtr = (unsigned char*)&ZERO_PACKET;
			g_USB_DataSize = 2;
		}
		else
		{
			 USB_Ep0_Send_Stall ();               // Send procedural stall
		}	
	}
	else if(g_USB_Setup.bmRequestType == 0x02)		//not clear feature device
	{	
		if (g_USB_Setup.wIndex.c[MSB] !=0 ) 
		{
			g_USB_Ep_Status[g_USB_Setup.wIndex.c[MSB]&0x0f] = EP_HALT;
		}
		else
		{
			USB_Ep0_Send_Stall ();               // Send procedural stall
		}
	}
	else
	{
		USB_Ep0_Send_Stall ();               // Send procedural stall
	}

	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Indicate setup packet has been serviced
		USB_Write_Reg(M_REG_CSR0,(CSR0_DATAEND | CSR0_SOPRDY) );
	}
  
}

//-----------------------------------------------------------------------------
// Get_Idle
//-----------------------------------------------------------------------------
// Not supported.
//
//-----------------------------------------------------------------------------
void Get_Idle(void) 
{
	if(g_USB_Setup.bmRequestType == 0xA2)
	{
		g_USB_Setup.bmRequestType--;
		g_USB_Setup.bRequest--;
	}

	if(g_USB_Setup.bmRequestType == 0xA1)
	{
		if(g_USB_Setup.wIndex.c[MSB] == 0xff)     //control interface
		{
			g_USB_DataPtr = &Control_Idle;
			g_USB_DataSize = 1;
		}
		else if(g_USB_Setup.wIndex.c[MSB] == 0x00)     //keyboard interface
		{	
			g_USB_DataPtr = &Keyboard_Idle;
			g_USB_DataSize = 1;
		}
		else if(g_USB_Setup.wIndex.c[MSB] == 0x01)     //mouse interface
		{
			g_USB_DataPtr = &Mouse_Idle;
			g_USB_DataSize = 1;		
		}
		else if(g_USB_Setup.wIndex.c[MSB] == 0x02)     //user define interface
		{
			g_USB_DataPtr = &Define_Idle;
			g_USB_DataSize = 1;		
		}

		else 
		{
			USB_Ep0_Send_Stall (); 
		}	
	}
	else 
	{
		USB_Ep0_Send_Stall (); 
	}

  if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Set serviced g_USB_Setup Packet, Endpoint 0 in transmit mode, and
		// reset DATASENT counter
		USB_Write_Reg(M_REG_CSR0, CSR0_SOPRDY);          // Service Setup Packet
		g_USB_Ep_Status[0] = EP_TX;                      // Put endpoint in transmit mode
		g_USB_DataSent = 0;                              // Reset Data Sent counter
	}
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Get_Protocol
//-----------------------------------------------------------------------------
// Not supported.
//
//-----------------------------------------------------------------------------
void Get_Protocol(void)
{
	if(g_USB_Setup.bmRequestType == 0xA2)
	{
		g_USB_Setup.bmRequestType--;
		g_USB_Setup.bRequest--;
	}

	if(g_USB_Setup.bmRequestType == 0xA1)
	{
		if(g_USB_Setup.wIndex.c[MSB] == 0x00)     //keyboard interface
		{
			g_USB_DataPtr = &Keyboard_Current_Protocol;
			g_USB_DataSize = 1;
		}
		else if(g_USB_Setup.wIndex.c[MSB] == 0x01)     //mouse interface
		{
			g_USB_DataPtr = &Mouse_Current_Protocol;
			g_USB_DataSize = 1;		
		}
		else 
		{
			USB_Ep0_Send_Stall (); 
		}
	}
	else 
	{
		USB_Ep0_Send_Stall (); 
	}
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Set serviced g_USB_Setup Packet, Endpoint 0 in transmit mode, and
		// reset DATASENT counter
		USB_Write_Reg(M_REG_CSR0, CSR0_SOPRDY);          // Service Setup Packet
		g_USB_Ep_Status[0] = EP_TX;                      // Put endpoint in transmit mode
		g_USB_DataSent = 0;                              // Reset Data Sent counter
	}
}

//-----------------------------------------------------------------------------
// Set_Protocol
//-----------------------------------------------------------------------------
// Not supported.
//
//-----------------------------------------------------------------------------
void Set_Protocol (void) 
{
	if(g_USB_Setup.bmRequestType == 0x22)
	{
		g_USB_Setup.bmRequestType--;
		g_USB_Setup.bRequest--;
	}

	if(g_USB_Setup.bmRequestType == 0x21)
	{
		if(g_USB_Setup.wIndex.c[MSB] == 0x00)     //keyboard interface
		{	
           
			Keyboard_Current_Protocol=g_USB_Setup.wValue.c[MSB];
		}
		else if(g_USB_Setup.wIndex.c[MSB] == 0x01)     //mouse interface
		{
			Mouse_Current_Protocol=	g_USB_Setup.wValue.c[MSB];
		}
		else 
		{
			USB_Ep0_Send_Stall (); 
		}
	}
	else 
	{
		USB_Ep0_Send_Stall (); 
	}
	
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
	// Indicate setup packet has been serviced
	USB_Write_Reg(M_REG_CSR0,(CSR0_DATAEND | CSR0_SOPRDY) );
	}
}



//-----------------------------------------------------------------------------
// Set_Idle()
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Description: Sets the idle feature on interrupt in endpoint.
//-----------------------------------------------------------------------------
void Set_Idle (void)
{
	if(g_USB_Setup.bmRequestType == 0x22)
	{
		g_USB_Setup.bmRequestType--;
		g_USB_Setup.bRequest--;
	}

	if(g_USB_Setup.bmRequestType == 0x21)
	{
		if(g_USB_Setup.wValue.i == 0x00)
		{  
			if (g_USB_Ep_Status[0] != EP_STALL)
			{
				USB_Write_Reg(M_REG_CSR0, (CSR0_SOPRDY | CSR0_DATAEND));
				return;
			}
		}
		else
		{
			if(g_USB_Setup.wIndex.c[MSB] == 0xff)     //control interface
			{	
				Control_Idle=g_USB_Setup.wValue.c[LSB];
			}
			else if(g_USB_Setup.wIndex.c[MSB] == 0x00)     //keyboard interface
			{	
				Keyboard_Idle=g_USB_Setup.wValue.c[LSB];
			}
			else if(g_USB_Setup.wIndex.c[MSB] == 0x01)     //mouse interface
			{
				Mouse_Idle=	g_USB_Setup.wValue.c[LSB];	
			}
			else if(g_USB_Setup.wIndex.c[MSB] == 0x02)     //user define  interface
			{
				Define_Idle= g_USB_Setup.wValue.c[LSB];
			}
			else 
			{
				USB_Ep0_Send_Stall (); 
			}
		}
	}
	else 
	{
		USB_Ep0_Send_Stall (); 
	}
	
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Indicate setup packet has been serviced
		USB_Write_Reg(M_REG_CSR0,(CSR0_DATAEND | CSR0_SOPRDY) );
	}
	
}



//-----------------------------------------------------------------------------
// Get_Report()
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Description: Sends a given report type to the host.
//
//-----------------------------------------------------------------------------
void Get_Report (void)
{
	USB_Ep0_Send_Stall (); 
}



//-----------------------------------------------------------------------------
// Set_Report()
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Description: Receives a report sent from the host.
//
//-----------------------------------------------------------------------------
void Set_Report (void)
{
	if (g_USB_Ep_Status[0] != EP_STALL)
	{
		// Indicate setup packet has been serviced
		USB_Write_Reg(M_REG_CSR0,(CSR0_SOPRDY) );
		Set_report_flag = 1;
		g_USB_Ep_Status[0] = EP_RX;
	}
}


/*----------------------------------End Of File-------------------------------*/
