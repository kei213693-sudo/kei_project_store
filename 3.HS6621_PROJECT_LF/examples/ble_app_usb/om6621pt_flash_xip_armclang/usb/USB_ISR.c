//------------------------------------------------------------------------------------
// USB_ISR.c
//------------------------------------------------------------------------------------
// Copyright 2013, Electronics Co., Ltd.
// crazy_code
// 2013-07-30
//
// Program Description:
//
//
//

#include <stdio.h>
#include "hs6206_sfr_reg.h"
#include "USB_Regs.h"
#include "USB_ISR.h"
#include "USB_Desc.h"
#include "USB2SPI.h"
//#include "RF.h"

#define U_DEBUG

extern uint8_t Set_report_flag;

//volatile unsigned char  M_REG_BASE_ADDR  _at_ 0xC000;           /*USB base address*/ 

uint8_t g_USB_State;     // Holds the current USB State
uint8_t g_USB_Ep_Status[6]={EP_IDLE,EP_IDLE,EP_IDLE,EP_IDLE,EP_IDLE,EP_IDLE};
extern  uint8_t Set_report_flag;
extern  uint8_t set_line_code_flag;
extern uint8_t  Line_Coding[7];
 uint8_t suspend_flag=0;
extern uint8_t uart_set_done;

void USB_Ep0_Send_Stall(void)
{
  USB_Write_Reg(M_REG_INDEX, 0);
  USB_Write_Reg(M_REG_CSR0, CSR0_SDSTL);
  g_USB_Ep_Status[0] = EP_STALL;
}

void USB_Resume_Mode_Enable(void)
{
	uint8_t Power_Reg;
	/* FIXME: resume USB must be cleared before enable suspend mode */
	USB_Inactive_Clock();

	Power_Reg=USB_Read_Reg(M_REG_POWER);
	Power_Reg |= USB_RESUME;  //enable resume mode
	USB_Write_Reg(M_REG_POWER,Power_Reg);
}

void USB_Resume_Mode_Disable(void)
{
	uint8_t Power_Reg;
	/* FIXME: resume USB must be cleared before enable suspend mode */
	USB_Inactive_Clock();

	Power_Reg=USB_Read_Reg(M_REG_POWER);
	Power_Reg &= ~USB_RESUME;  //disable resume mode
	USB_Write_Reg(M_REG_POWER,Power_Reg);
}

void USB_Suspend_Mode_Enable(void)
{
	uint8_t Power_Reg;
	/* FIXME: wakeup USB must be cleared before enable suspend mode */
	USB_Inactive_Clock();

	Power_Reg=USB_Read_Reg(M_REG_POWER);
	Power_Reg |= USB_SUSEN;  //enable suspend mode
	USB_Write_Reg(M_REG_POWER,Power_Reg);
}

uint8_t USB_Fifo_Is_Empty(void)
{
  return ( !(USB_Read_Reg(M_REG_INCSR1) & INCSR_FIFO_NOT_EMPTY) );
}



#ifdef U_DEBUG
const unsigned char HexTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                 'A', 'B', 'C', 'D', 'E', 'F'};

extern int ser_putchar (int c);

void prints(unsigned char *pd)
{
	while('\0' != *pd)
	{
		ser_putchar(*pd++);
	}
}

void print_hex(unsigned char u8data)
{
//	ser_putchar('0');
//	ser_putchar('x');
	ser_putchar(HexTable[u8data >> 4]);
	ser_putchar(HexTable[u8data & 0x0F]);
	ser_putchar(' ');
	
}

void print_short_int(unsigned short u16data)
{
	signed char i;
	unsigned char buf[7];

	buf[0] = '0';
	buf[1] = 'x';
	buf[6] = '\0';

	for(i = 5; i >= 2; i--)
	{
		buf[i] = HexTable[u16data & 0x0F];
		u16data >>= 4;
	}

	prints(buf);
	prints((uint8_t*)" ");
}
void print_long_int(unsigned long u32data)
{
	signed char i;
	unsigned buf[10];

	for(i = 9; i >= 0; i--)
	{
		buf[i] = '0'+u32data%10;
		u32data /= 10;
	}

	for(i = 0; i < 9; i++)
	{
		if('0' != buf[i])
			break;
	}

	for(; i < 10; i++)
	{
		ser_putchar(buf[i]);
	}
	
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////
/*-----------------------------USB Interrupt Service Routine---------------------------------*/
///////////////////////////////////////////////////////////////////////////////////////////////


void USB_Resume_ISR(void)
{
//Open_USB_IRQ();
}

void USB_Rst_ISR(void)
{
	#ifdef U_DEBUG
	prints((uint8_t*)"USB复位\r\n");
	#endif
  g_USB_State=DEV_DEFAULT;
  g_USB_Ep_Status[0]=EP_IDLE;
  g_USB_Ep_Status[1]=EP_HALT;
  g_USB_Ep_Status[2]=EP_HALT;
  g_USB_Ep_Status[3]=EP_HALT;
  g_USB_Ep_Status[4]=EP_HALT;
  g_USB_Ep_Status[5]=EP_HALT;
}

void USB_Suspend_ISR(void)
{
	suspend_flag = 1;
//    Open_USB_IRQ();
}

//endpoint0 interrupt service routine 
void USB_Ep0_ISR(void)
{
  uint8_t Ep0CSR;
  uint8_t TempReg;
  uint8_t len;
  uint32_t BitRate;
  #ifdef U_DEBUG
  uint8_t i;
  #endif
	
  USB_Write_Reg(M_REG_INDEX,0x00);
  Ep0CSR=USB_Read_Reg(M_REG_CSR0);

  /* Handle Status Phase of Set Address command */
  if (g_USB_Ep_Status[0] == EP_ADDRESS)
	{
    USB_Write_Reg(M_REG_FADDR, g_USB_Setup.wValue.c[MSB]);
    g_USB_Ep_Status[0] = EP_IDLE;
	}

  /* If last packet was a sent stall,reset STSTL bit and return EP0 to idle state */
  if(Ep0CSR & CSR0_STSTL)
  {
		USB_Write_Reg(M_REG_CSR0, 0x00);
		g_USB_Ep_Status[0] = EP_IDLE;
		return;
  }

  /* if last SETUP transaction was ended prematurely,set Serviced SETUP End bit and return EP0 to idle state */
  if(Ep0CSR & CSR0_SUEND)
  {
	  USB_Write_Reg(M_REG_CSR0, CSR0_DATAEND);
	  USB_Write_Reg(M_REG_CSR0, CSR0_SSUEND);
	  g_USB_Ep_Status[0] = EP_IDLE;
  }

  if(g_USB_Ep_Status[0]==EP_RX)    //Rx mode
	{	
		if(Set_report_flag)
		{
			//uint8_t len;
			USB_Read_Reg(M_REG_COUNT0);
			USB_Read_Fifo(0x00,(uint8_t *)(&g_USB_Setup), len);
			USB_Write_Reg(M_REG_CSR0, (CSR0_SOPRDY | CSR0_DATAEND));
			Set_report_flag = 0;
			g_USB_Ep_Status[0] = EP_IDLE;
			return ;
		}
#ifdef USB_APP_CDC		
		if(set_line_code_flag)
		{
			len = USB_Read_Reg(M_REG_COUNT0);
			USB_Read_Fifo(0x00, Line_Coding, len);

			if(7 == len)
			{
				BitRate = Line_Coding[3];
				BitRate = (BitRate << 8) + Line_Coding[2];
				BitRate = (BitRate << 8) + Line_Coding[1];
				BitRate = (BitRate << 8) + Line_Coding[0];

//				serial_init(BitRate);

				Line_Coding[0] = BitRate&0xFF;
				Line_Coding[1] = (BitRate>>8)&0xFF;
				Line_Coding[2] = (BitRate>>16)&0xFF;
				Line_Coding[3] = (BitRate>>24)&0xFF;
				Line_Coding[4] = 0x00;
				Line_Coding[5] = 0x00;
				Line_Coding[6] = 0x08;
			}

			#ifdef U_DEBUG
				prints((uint8_t*)"波特率设置为: ");
				print_long_int(BitRate);
				prints((uint8_t*)"bps\r\n");
			#endif
			
			USB_Write_Reg(M_REG_CSR0, (CSR0_SOPRDY | CSR0_DATAEND));
			set_line_code_flag = 0;
			uart_set_done = 1;
			g_USB_Ep_Status[0] = EP_IDLE;
			return;
		}
#endif
	}
	
  if(g_USB_Ep_Status[0]==EP_IDLE)    //Idle mode
  {
    if(Ep0CSR & CSR0_OPRDY)   //USB device (HS6206)receive data, 得到描述符请求
  	{
      USB_Read_Fifo(0x00,(uint8_t *)(&g_USB_Setup),sizeof(g_USB_Setup));   //read usb host sent cmd
			//little-endian to big-endian
      g_USB_Setup.wValue.i = g_USB_Setup.wValue.c[MSB] + (g_USB_Setup.wValue.c[LSB]<<8);
      g_USB_Setup.wIndex.i = g_USB_Setup.wIndex.c[MSB] + (g_USB_Setup.wIndex.c[LSB]<<8);
      g_USB_Setup.wLength.i = g_USB_Setup.wLength.c[MSB] + (g_USB_Setup.wLength.c[LSB]<<8);
	    //printf("setup pkt: %02x %02x %04x %04x %04x\n", (int)(g_USB_Setup.bmRequestType), (int)(g_USB_Setup.bRequest), (int)(g_USB_Setup.wValue.i), (int)(g_USB_Setup.wIndex.i), (int)(g_USB_Setup.wLength.i));

//standard device quest data structure
//bmRequestType  1Byte
//bRequest            1Byte
//wValue                2Byte
//wIndex                2Byte
//wLength              2Byte

			if( (g_USB_Setup.bmRequestType & STP_TYPE) == STP_TYPE_CLASS)	 //Class request cmd, 类请求
			{
				if(0x80 == (g_USB_Setup.bmRequestType & 0x80))//input request
				{
					#ifdef U_DEBUG
					prints((uint8_t*)"类输入请求: ");
					print_hex(g_USB_Setup.bmRequestType);
					print_hex(g_USB_Setup.bRequest);
					print_short_int(g_USB_Setup.wValue.i);
					print_short_int(g_USB_Setup.wIndex.i);
					print_short_int(g_USB_Setup.wLength.i);
					prints("\r\n");
					#endif
					switch(g_USB_Setup.bRequest)	
					{
						case HID_GET_REPORT:
							Get_Report();
							break;
	
						case HID_GET_IDLE:
							Get_Idle();
							break;
	
						case HID_GET_PROTOCOL:
							Get_Protocol();
							break;
#ifdef USB_APP_CDC	
						case CDC_GET_LINE_CODING:
							#ifdef U_DEBUG
							prints((uint8_t*)"GET_LINE_CODING\r\n");
							#endif
							Get_Line_Code();
							break;
#endif	
						default:
							USB_Ep0_Send_Stall();
							break;													
					}

				}
				else//output request
				{
					#ifdef U_DEBUG
					prints((uint8_t*)"类输出请求: ");
					print_hex(g_USB_Setup.bmRequestType);
					print_hex(g_USB_Setup.bRequest);
					print_short_int(g_USB_Setup.wValue.i);
					print_short_int(g_USB_Setup.wIndex.i);
					print_short_int(g_USB_Setup.wLength.i);
					prints((uint8_t*)"\r\n");
					#endif
					switch(g_USB_Setup.bRequest)
					{
#ifdef USB_APP_CDC
						case CDC_SET_CONTROL_LINE_STATE:
							#ifdef U_DEBUG
							prints((uint8_t*)"CDC_SET_CONTROL_LINE_STATE\r\n");
							#endif
							Set_Control_Line_State();
							break;

						case CDC_SET_LINE_CODING:
							#ifdef U_DEBUG
							prints((uint8_t*)"CDC_SET_LINE_CODING\r\n");
							#endif
							Set_Line_State();
							break;
#endif                        
						case HID_SET_REPORT:
							Set_Report();
							break;
	
						case HID_SET_IDLE:
							Set_Idle();
							break;
	
						case HID_SET_PROTOCOL:
							Set_Protocol();
							break;	
						default:
							USB_Ep0_Send_Stall();
							break;

					}
				}
				
			}
      
      if( (g_USB_Setup.bmRequestType & STP_TYPE) == STP_TYPE_STD )    //Standard request cmd, 标准请求                      
      {
      	if(0x80 == (g_USB_Setup.bmRequestType & 0x80))//input request
      	{
      		#ifdef U_DEBUG
					prints((uint8_t*)"标准输入请求: ");
					print_hex(g_USB_Setup.bmRequestType);
					print_hex(g_USB_Setup.bRequest);
					print_short_int(g_USB_Setup.wValue.i);
					print_short_int(g_USB_Setup.wIndex.i);
					print_short_int(g_USB_Setup.wLength.i);
					prints((uint8_t*)"\r\n");
					#endif
      		switch(g_USB_Setup.bRequest)
					{
						case STD_RQST_GET_STATUS:
							Get_Status();
							break;
							
						case STD_RQST_RSVD0:
							break;
	
						case STD_RQST_RSVD1:
							break;
	
						case STD_RQST_GET_DESCRIPTOR://请求输入，即设备到主机,所以bmRequestType[7] = 1
							USB_Get_Descriptor();
							break;
	
						case STD_RQST_GET_CONFIGURATION:
							Get_Configuration();								 
							break;
	
						case STD_RQST_GET_INTERFACE:
							Get_Interface();
							break;
	
						case STD_RQST_SYNCH_FRAME:
							break;
	
						default:
							USB_Ep0_Send_Stall();
							break;
					}

				}
        else//output request
        {
        	#ifdef U_DEBUG
        	prints((uint8_t*)"标准输出请求: ");
					print_hex(g_USB_Setup.bmRequestType);
					print_hex(g_USB_Setup.bRequest);
					print_short_int(g_USB_Setup.wValue.i);
					print_short_int(g_USB_Setup.wIndex.i);
					print_short_int(g_USB_Setup.wLength.i);
					prints((uint8_t*)"\r\n");
					#endif
        	switch(g_USB_Setup.bRequest)
        	{
        		//case STD_RQST_RSVD0:
						//	break;
						//case STD_RQST_RSVD1:
						//	break;
		      	case STD_RQST_CLEAR_FEATURE:
							Clear_Feature();
							break;
						case STD_RQST_SET_FEATURE:
							Set_Feature();
							break;
						case STD_RQST_SET_ADDRESS:
							#ifdef U_DEBUG
							prints((uint8_t*)"设置设备地址阶段\r\n");
							#endif
							Set_Address();
							break;
						case STD_RQST_SET_DESCRIPTOR:
							break;
						case STD_RQST_SET_CONFIGURATION:
							#ifdef U_DEBUG
							prints((uint8_t*)"设置配置阶段:");
							print_short_int(g_USB_Setup.wValue.i);
							prints((uint8_t*)"\r\n");
							#endif
							Set_Configuration();
							break;
						case STD_RQST_SET_INTERFACE:
							Set_Interface();
							break;
						
						default:
							USB_Ep0_Send_Stall();
							break;
        	}
        }
				
     	}
  	}       
  }
	
  if(g_USB_Ep_Status[0]==EP_TX)     //Tx mode
	{
		if(!(Ep0CSR & CSR0_INPRDY))  //sent data,说明,FIFO,中没有数据
		{
			#ifdef U_DEBUG
			prints((uint8_t*)"设备返回: ");
			#endif
			
			//#ifdef UART_DEBUG
			// printf("\nUSB Endpoint0 send data interrupt!");
			//#endif
			Ep0CSR=USB_Read_Reg(M_REG_CSR0);
			if( (!(Ep0CSR & CSR0_SUEND)) || (!(Ep0CSR & CSR0_OPRDY)) )//CSR0_SUEND的置位说明控制传输的结束
			{
				TempReg=CSR0_INPRDY;   
				if(g_USB_DataSize>=USB_EP0_FIFO_SIZE)
				{
					#ifdef U_DEBUG
					for(i = 0; i < USB_EP0_FIFO_SIZE; i++)////////
					{
						print_hex(g_USB_DataPtr[i]);
					}
					prints((uint8_t*)"\r\n");
					#endif
					
					USB_Write_Fifo(0x00,(uint8_t *)g_USB_DataPtr,USB_EP0_FIFO_SIZE);
					g_USB_DataSize -= USB_EP0_FIFO_SIZE;  
					g_USB_DataPtr  += USB_EP0_FIFO_SIZE;
					g_USB_DataSent += USB_EP0_FIFO_SIZE; 
				}
				else
				{
					#ifdef U_DEBUG
					for(i = 0; i < g_USB_DataSize; i++)////////
					{
						print_hex(g_USB_DataPtr[i]);
					}
					prints((uint8_t*)"\r\n");
					#endif
					USB_Write_Fifo(0x00,(uint8_t *)g_USB_DataPtr,g_USB_DataSize);
					TempReg |= CSR0_DATAEND;//当所有的数据都发送完成,CPU,才置位,DATAEND,   
					g_USB_Ep_Status[0] = EP_IDLE;    
				}
				
				if(g_USB_DataSent==g_USB_Setup.wLength.i)
				{
					TempReg |= CSR0_DATAEND;
					g_USB_Ep_Status[0] = EP_IDLE; // and return Endpoint 0 to an idle state
				}
				
				USB_Write_Reg(M_REG_CSR0, TempReg);                          // Write mask to E0CSR
			}               

		}
	}

}

//-----------------------------------------------------------------------------
//
// Handler will be entered after the endpoint's buffer has been
// transmitted to the host.  In1_StateMachine is set to Idle, which
// signals the foreground routine SendPacket that the Endpoint
// is ready to transmit another packet.
//-----------------------------------------------------------------------------
void USB_Ep1_In_ISR(void)
{
  g_USB_Ep_Status[1] = EP_IDLE;
}
void USB_Ep2_In_ISR(void)
{
  g_USB_Ep_Status[2] = EP_IDLE;
}
void USB_Ep3_Out_ISR(void)
{
#ifdef USB_APP_CDC
  uint8_t rx_buffer[16];
  uint8_t len;
  g_USB_Ep_Status[3] = EP_RX;
  len = Get_USB_Packet(3, rx_buffer);
  usb_send_packet(2, rx_buffer, len);
#endif
}

uint8_t Get_USB_Packet(uint8_t EPindex, uint8_t *Packet)
{
	unsigned char ControlReg;
	unsigned char len = 0;

	g_USB_Ep_Status[EPindex] = EP_IDLE;

	USB_Write_Reg(M_REG_INDEX, EPindex);
	ControlReg = USB_Read_Reg(M_REG_OUTCSR1);

	if(ControlReg & OUTCSR_OPRDY)
	{
		len = USB_Read_Reg(M_REG_OUTCOUNT1);
		len = (USB_Read_Reg(M_REG_OUTCOUNT2) << 8) + len;
	
		USB_Read_Fifo(EPindex, Packet, len);
		USB_Write_Reg(M_REG_OUTCSR1, ControlReg&(~OUTCSR_OPRDY));
		return len;
	}
	else if(ControlReg & OUTCSR_SENT_STALL)
	{
		USB_Write_Reg (M_REG_OUTCSR1, (ControlReg & (~(OUTCSR_SENT_STALL|OUTCSR_SEND_STALL))));//clear 
		return 0;
	}
	else
	{
		return 0;
	}
}

void USB_Sof_ISR(void)
{
  USB_Write_Reg( M_REG_INTRUSBE,0x07 );  //disable SOF, enable suspend, reset and resume interrupt.
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
void Send_USB_Packet(uint8_t EPindex, uint8_t *Packet, uint8_t len)
{
    //bit EAState;
    unsigned char ControlReg;

    //EAState = EA;
    //EA = 0;
    //Close_USB_IRQ();

    USB_Write_Reg(M_REG_INDEX,EPindex); 			 // Set index to endpoint 1 registers

    // Read contol register for EP 1
    ControlReg = USB_Read_Reg(M_REG_INCSR1);

    if (g_USB_Ep_Status[EPindex] == EP_HALT)				// If endpoint is currently halted, 																	
    {
        USB_Write_Reg (M_REG_INCSR1, INCSR_SDSTL);	// send a stall
    }

    else if(g_USB_Ep_Status[EPindex] == EP_IDLE)
    {
        // the state will be updated inside the ISR handler
        g_USB_Ep_Status[EPindex] = EP_TX;

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
    } 																	// indicating fresh data on FIFO 1

    //EA = EAState;
    //Open_USB_IRQ();
}


				   
// USB interrupt service routine entry
/*---------------------------USB Interrupt Service Routine-------------------------------*/

void USB_WAKEUP_ISR(void) //interrupt HS6206_INT_USBWU
{
	/* USB host wakeup me or SW set sw_wu to wakeup myself */
	//	debug("USB wakeup Intterrupt!\n");
}

/*---------------------------------------End Of File----------------------------------------*/

