//------------------------------------------------------------------------------
// USB_Desc.h
//------------------------------------------------------------------------------
// Copyright 2013, crazy_code Electronics Co., Ltd.
// crazy_code
// 2013-07-31
//
// Program Description:
//
//
//

#ifndef __USB_DESC_H__
#define __USB_DESC_H__

#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>

extern uint16_t g_USB_DataSize;
extern uint8_t *g_USB_DataPtr;
extern uint16_t g_USB_DataSent;

union word_t
{
  uint16_t i; 
  uint8_t c[2];
};
typedef union word_t WORD;

#define MSB   0x00 
#define LSB   0x01

//有关setup 的定义
struct usb_setup_t 
{
  uint8_t bmRequestType;
  uint8_t bRequest;
  WORD wValue;
  WORD wIndex;
  WORD wLength;    
};

extern struct usb_setup_t g_USB_Setup;

//setup.bmRequestType
#define STP_DIR       BIT7            /*Data transfer direction*/                           
#define STD_DIR_IN    BIT7            /*device to host*/
#define STD_DIR_OUT   0x00            /*host to device*/

#define STP_TYPE            (BIT5+BIT6)     /*Type*/  
#define STP_TYPE_STD        0x00            /*standard*/ 
#define STP_TYPE_CLASS      BIT5            /*class*/ 
#define STP_TYPE_VENDOR     BIT6            /*vendor*/
#define STP_TYPE_RSVD       (BIT5+BIT6)     /*reserved*/   

#define STP_REPT            0x1F            /*Recipient*/
#define STP_REPT_DEV        0x00            /*Device*/
#define STP_REPT_INTFACE    BIT1            /*interface*/ 
#define STP_REPT_EDPT       BIT2            /*endpoint*/
#define STP_REPT_OTHER      (BIT1+BIT2)     /*other*/

//class request
/*the uart properities include uart baud, stop bits, parity, data bits*/
#define CDC_GET_LINE_CODING           0x21   /*host gets the uart properities*/
#define CDC_SET_CONTROL_LINE_STATE    0x22
#define CDC_SET_LINE_CODING						0x20   /*host set the uart properities*/

//standard request
#define STD_RQST_GET_STATUS            0x00
#define STD_RQST_CLEAR_FEATURE         0x01
#define STD_RQST_RSVD0                 0x02
#define STD_RQST_SET_FEATURE           0x03
#define STD_RQST_RSVD1                 0x04
#define STD_RQST_SET_ADDRESS           0x05
#define STD_RQST_GET_DESCRIPTOR        0x06
#define STD_RQST_SET_DESCRIPTOR        0x07
#define STD_RQST_GET_CONFIGURATION     0x08
#define STD_RQST_SET_CONFIGURATION     0x09
#define STD_RQST_GET_INTERFACE         0x0A
#define STD_RQST_SET_INTERFACE         0x0B
#define STD_RQST_SYNCH_FRAME           0x0C

// HID Request Codes
#define HID_GET_REPORT                 0x01   // Code for Get Report
#define HID_GET_IDLE                   0x02   // Code for Get Idle
#define HID_GET_PROTOCOL               0x03   // Code for Get Protocol
#define HID_SET_REPORT                 0x09   // Code for Set Report
#define HID_SET_IDLE                   0x0A   // Code for Set Idle
#define HID_SET_PROTOCOL               0x0B   // Code for Set Protocol

//Descriptor types
#define DSCR_TYPE_DEV                  0x01
#define DSCR_TYPE_CONFIGURATION        0x02
#define DSCR_TYPE_STRING               0x03
#define DSCR_TYPE_INTERFACE            0x04
#define DSCR_TYPE_ENDPOINT             0x05
#define DSCR_TYPE_DEV_QUALIFIER        0x06
#define DSCR_TYPE_OTHER_SPEED_CONFIG   0x07
#define DSCR_TYPE_INTERFACE_POWER      0x08 

// HID Descriptor Types
#define DSCR_TYPE_HID                  0x21   // HID Class Descriptor
#define DSCR_TYPE_HID_REPORT           0x22   // HID Report Descriptor

//------------------------------------------
// Standard Device Descriptor Type Defintion
//------------------------------------------
struct device_descriptor_t
{
	uint8_t bLength;                       // Size of this Descriptor in uint8_ts
	uint8_t bDescriptorType;               // Descriptor Type (=1)
	WORD bcdUSB;                        // USB Spec Release Number in BCD
	uint8_t bDeviceClass;                  // Device Class Code
	uint8_t bDeviceSubClass;               // Device Subclass Code
	uint8_t bDeviceProtocol;               // Device Protocol Code
	uint8_t bMaxPacketSize0;               // Maximum Packet Size for EP0
	WORD idVendor;                      // Vendor ID
	WORD idProduct;                     // Product ID
	WORD bcdDevice;                     // Device Release Number in BCD
	uint8_t iManufacturer;                 // Index of String Desc for Manufacturer
	uint8_t iProduct;                      // Index of String Desc for Product
	uint8_t iSerialNumber;                 // Index of String Desc for SerNo
	uint8_t bNumConfigurations;            // Number of possible Configurations
};
// End of Device Descriptor Type

//--------------------------------------------------
// Standard Configuration Descriptor Type Definition
//--------------------------------------------------
struct configuration_descriptor_t
{
	uint8_t bLength;                       // Size of this Descriptor in Bytes
	uint8_t bDescriptorType;               // Descriptor Type (=2)
	WORD wTotalLength;                  // Total Length of Data for this Conf
	uint8_t bNumInterfaces;                // # of Interfaces supported by Conf
	uint8_t bConfigurationValue;           // Designator Value for *this* Conf
	uint8_t iConfiguration;                // Index of String Desc for this Conf
	uint8_t bmAttributes;                  // Configuration Characteristics
	uint8_t bMaxPower;                     // Max. Power Consumption in Conf (*2mA)
};
// End of Configuration Descriptor Type

//----------------------------------------------
// Standard Interface Descriptor Type Definition
//---------------------------------------------
struct interface_descriptor_t
{
	uint8_t bLength;                       // Size of this Descriptor in Bytes
	uint8_t bDescriptorType;               // Descriptor Type (=4)
	uint8_t bInterfaceNumber;              // Number of *this* Interface (0..)
	uint8_t bAlternateSetting;             // Alternative for this Interface
	uint8_t bNumEndpoints;                 // No of EPs used by this IF (excl. EP0)
	uint8_t bInterfaceClass;               // Interface Class Code
	uint8_t bInterfaceSubClass;            // Interface Subclass Code
	uint8_t bInterfaceProtocol;            // Interface Protocol Code
	uint8_t iInterface;                    // Index of String Desc for Interface
};
// End of Interface Descriptor Type

//---------------------------------------------
// Standard Endpoint Descriptor Type Definition
//---------------------------------------------
struct endpoint_descriptor_t
{
	uint8_t bLength;                       // Size of this Descriptor in Bytes
	uint8_t bDescriptorType;               // Descriptor Type (=5)
	uint8_t bEndpointAddress;              // Endpoint Address (Number + Direction)
	uint8_t bmAttributes;                  // Endpoint Attributes (Transfer Type)
	WORD wMaxPacketSize;                // Max. Endpoint Packet Size
	uint8_t bInterval;                     // Polling Interval (Interrupt) ms
};
// End of Endpoint Descriptor Type

//-----------------------------------
// HID Descriptor
//-----------------------------------
struct HID_descriptor_t 
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	WORD bcdHID;
	uint8_t bCountyCode;
	uint8_t bNumDescriptors;
	WORD bDescriptorLength;
	uint8_t bDescriptorType1;
	WORD wDescriptorLength;
};

/* standard */
extern void Set_Address(void);
extern void Set_Configuration(void); 
extern void Clear_Feature(void);
extern void Get_Configuration (void);
extern void Get_Interface(void);
extern void Get_Status (void);
extern void Set_Feature (void);
extern void Set_Interface (void); 
extern void Set_Idle (void);
extern void Set_Protocol (void);
extern void Set_Report (void);
extern void Get_Report (void);
extern void Get_Idle(void);
extern void Get_Protocol(void);
extern void Get_Line_Code(void);
extern void Set_Control_Line_State(void);
extern void Set_Line_State(void);

/* custom design */
extern void USB_Get_Descriptor(void); 

#endif

/*----------------------------------End Of File-------------------------------*/
