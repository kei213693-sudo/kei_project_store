// Header:     usb descriptor file 
// File Name:  USB_Desc.c    
// Author:     crazy_code 
// Date:       2013/07/31

#include <stdio.h>
#include "hs6206_sfr_reg.h"
#include "USB_Desc.h"
#include "USB_ISR.h"
#include "USB_Regs.h"
#include "usb_descriptor.h"
#include "USB2SPI.h"
#include "USB_Interface.h"

/*------------------ 语言ID-------------------------*/
const uint8_t String0Desc[4] =
{
   0x04, 0x03, 0x09, 0x04
}; //end of LanguageID

const uint8_t Serial_Num_Descriptor[] =
{
   26, 0x03,
   '2', 0,
   '0', 0,
   '2', 0,
   '1', 0,
   '0', 0,
   '5', 0,
   '0', 0,
   '3', 0,
   '1', 0,
   '0', 0,
   '0', 0,
   '1', 0,
}; //end of Serial_Num_Descriptor

const uint8_t ManufactureStringDescriptor[] = {
	16, 0x03,
	'o', 0,
	'n', 0,
	'm', 0,
	'i', 0,
	'c', 0,
	'r', 0,
	'o', 0,
};

const uint8_t ProductStringDescriptor[] = {
	40, 0x03,
	'o', 0,
	'm', 0,
	'6', 0,
	'6', 0,
	'2', 0,
	'1', 0,
	'p', 0,
	't', 0,
	' ', 0,    
	'm', 0,
	'o', 0,
	'u', 0,
	's', 0,
	'e', 0,    
	' ', 0,
	'd', 0,
	'e', 0,
	'm', 0,
	'o', 0,

};

//Device Descriptor
//
unsigned char const DEVICE_Descriptor[] = {
    USB_DEVICE_DESC_SIZE,                       //长度大小,0x12
    USB_DEVICE_DESCRIPTOR_TYPE,                 //描述符类型,0x01(设备描述符)
    WORD(BCD_USB_VERSION),                      //符合的USB版本规范,0x0110(USB1.1)
#ifdef USB_APP_CDC
    0x02,                                       //Class code
#else
    0x00,                                       //Class code
#endif
    0x00,                                       //Subclass code
    0x00,                                       //Protocol code
    0x10,                                       //最大封包大小,64
    WORD(iUSB_DEFAULT_VID),                      //Vendor ID;
    WORD(iUSB_DEFAULT_PID),                      //Product ID;
    WORD(BCD_DEVICE_NUMBER),                    //以BCD表示设备发行的序列号,0x0200(00.03)
    0x01,                                       //制造商的字符串描述索引
#ifdef USB_APP_CDC
    0x00,                                       //产品的字符串描述索引
#else
    0x02,                                       //产品的字符串描述索引
#endif                                      
    0x03,                                       //设备序号的字符串描述符索引
    0x01                                        //配置数目
};


#ifdef USB_APP_CDC
//Configuration Descriptor
unsigned char const CONFIGURATION_Descriptor[] = {
	USB_CONFIGUARTION_DESC_SIZE,       // 0 bLength 1 09h  
	USB_CONFIGURATION_DESCRIPTOR_TYPE, // 1 bDescriptorType 1 02h Configuration 
	WORD(67),                          // 2 wTotalLength 2 0027h	
	//sizeof(CONFIGURATION_Descriptor)&0xFF,
 //(sizeof(CONFIGURATION_Descriptor) >> 8)&0xFF,
	0x02,                              // 4 bNumInterfaces 1 02h	
	0x01,                              // 5 bConfigurationValue 1 01h  
	0x00,                              // iConfiguration 1 00h	
	0x80,                              // bmAttributes 1 80h Self Powered 
	0x32,                              // bMaxPower 1 32h 100 mA 
	
	//CDC class Interface Descriptor 0/0 , communication Interface Class , 1 Endpoints
	//Offset Field Size Value Description 
	USB_INTERFACE_DESC_SIZE,           // 0 bLength 1 09h 
	USB_INTERFACE_DESCRIPTOR_TYPE,     // 1 bDescriptorType 1 04h Interface 
	0x00,                              // 2 bInterfaceNumber 1 00h	
	0x00,                              // 3 bAlternateSetting 1 00h  
	0x01,                              // 4 bNumEndpoints 1 01h  
	0x02,                              // 5 bInterfaceClass 1 02h CDC class interface
	0x02,                              // 6 bInterfaceSubClass 1 02h	 Abstract Control Model
	0x01,                              // 7 bInterfaceProtocol 1 01h Common AT commands	
	0x00,                              // 8 iInterface 1 00h	

  /***Header Functional Descriptor***/
  0x05,   /* bLength: Endpoint Descriptor size */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x00,   /* bDescriptorSubtype: Header Functional Descriptor */
  0x10,   /* bcdCDC: spec release number */
  0x01,

  /***Call Management Functional Descriptor***/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x01,   /* bDescriptorSubtype: Call Management Func Desc */
  0x00,   /* bmCapabilities: D0+D1 */
  0x00,   /* bDataInterface: 1 *///01h
  /*ACM Functional Descriptor*/
  0x04,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
  0x02,   /* bmCapabilities */

	/*Union Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x06,   /* bDescriptorSubtype: Union func desc */
  0x00,   /* bMasterInterface: Communication class interface */
  0x01,   /* bSlaveInterface0: Data Class Interface */
  
  //Endpoint Descriptor 81 1 In, Interrupt, 255 ms
  //Offset Field Size Value Description 
  USB_ENDPOINT_DESC_SIZE,            // 0 bLength 1 07h  
  USB_ENDPOINT_DESCRIPTOR_TYPE,      // 1 bDescriptorType 1 05h Endpoint 
  USB_ENDPOINT_IN(1),                // 2 bEndpointAddress 1 81h 1 In 
  USB_ENDPOINT_TYPE_INTERRUPT,       // 3 bmAttributes 1 03h Interrupt 
                                     //1..0: Transfer Type	......11	Interrupt 
                                     //7..2: Reserved  000000.. 	
  WORD(16),
  0x0A,                              // 6 bInterval 1 0Ah 10 ms 
  
  //Interface Descriptor 1/0 , communication Interface Class , 1 Endpoints
  USB_INTERFACE_DESC_SIZE,   /* bLength: Endpoint Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
  0x01,   /* bInterfaceNumber: Number of Interface */
  0x00,   // 3 bAlternateSetting 1 00h  
  0x02,   /* bNumEndpoints: Two endpoints used */
  0x0A,   /* bInterfaceClass: CDC */
  0x00,   /* bInterfaceSubClass: */
  0x00,   /* bInterfaceProtocol: */
  0x00,   /* iInterface: */
  
  //Endpoint Descriptor 02 2 IN, Bulk, 16 bytes
  //Offset Field Size Value Description 
  USB_ENDPOINT_DESC_SIZE,            // 0 bLength 1 07h  
  USB_ENDPOINT_DESCRIPTOR_TYPE,      // 1 bDescriptorType 1 05h Endpoint 
  USB_ENDPOINT_IN(2),               // 2 bEndpointAddress 1 02h 2 In 
  USB_ENDPOINT_TYPE_BULK,            // 3 bmAttributes 1 02h Bulk 
                                     //1..0: Transfer Type	......10	Bulk 
                                     //7..2: Reserved  000000.. 	
  WORD(16),
  0x00,                              // 6 bInterval 1 00h  
  
  //Endpoint Descriptor 03 3 OUT, Bulk, 16 bytes
  //Offset Field Size Value Description 
  USB_ENDPOINT_DESC_SIZE,            // 0 bLength 1 07h  
  USB_ENDPOINT_DESCRIPTOR_TYPE,      // 1 bDescriptorType 1 05h Endpoint 
  USB_ENDPOINT_OUT(3),                // 2 bEndpointAddress 1 83h 3 out 
  USB_ENDPOINT_TYPE_BULK,            // 3 bmAttributes 1 02h Bulk 
                                     //1..0: Transfer Type	......10	Bulk 
                                     //7..2: Reserved  000000.. 
  WORD(16),
  0x00,                              //6 bInterval 1 00h  

};
#else
//Configuration Descriptor
unsigned char const CONFIGURATION_Descriptor[] = {

//CONFIGURATION(9 bytes)
    USB_CONFIGUARTION_DESC_SIZE,                //bLength,9bytes.
    USB_CONFIGURATION_DESCRIPTOR_TYPE,          //Descriptor type,0x02(配置描述符)
    WORD(9+9+9+7),                                   //描述符总长度,59						//20130826
    0x01,                                       //bNumInterface,0x02(接口数目)				//20130826
    0x01,                                       //bConfigurationValue
    0x00,                                       //iConfiguration
    0xA0,                                       //bmAttributes,(USB_CONFIG_BUS_POWERED|USB_CONFIG_REMOTE_WAKEUP)
    USB_CONFIG_POWER_MA(100),                   //bMaxPower,100mA

//Mouse CONFIGURATION
//INTERFACE(9 bytes)
    USB_INTERFACE_DESC_SIZE,                    //bLength,9bytes
    USB_INTERFACE_DESCRIPTOR_TYPE,              //Descriptor type,0x04(接口描述符)
    0x00,                                       //bInterfaceNumber
    0x00,                                       //bAlternateSetting
    0x01,                                       //bNumEndpoints,0x01(端点数目)
    USB_DEVICE_CLASS_HUMAN_INTERFACE,           //bInterfaceClass,接口群组,USB规定HID码为0x03
    HID_SUBCLASS_BOOT,                          //bInterfaceSubClass,接口次群组,为0x01
    HID_PROTOCOL_MOUSE,                         //bInterfaceProtocol
    0x00,                                       //iInterface,接口字符串描述符的索引

//HID(9 bytes)
    HID_DESC_SIZE,                              //bLength,9bytes
    HID_HID_DESCRIPTOR_TYPE,                    //Descriptor type,0x21(HID描述符)
    WORD(HID_VERSION),                          //bcdHID
    0x00,                                       //bCountryCode(没有就设为0)
    0x01,                                       //bNumDescriptors,需遵循的HID群组报告的数目
    HID_REPORT_DESCRIPTOR_TYPE,                 //bDescriptorType,0x22(报告描述符).
    WORD(67),                                  //wDescriptorLength,报告描述符的长度					//20130826

//ENDPOINT(7 bytes)
    USB_ENDPOINT_DESC_SIZE,                     //bLength,7bytes
    USB_ENDPOINT_DESCRIPTOR_TYPE,               //Descriptor type,0x05(端点描述符)
    USB_ENDPOINT_IN(1),                         //bEndpointAddress //luwei
    USB_ENDPOINT_TYPE_INTERRUPT,                //bmAttributes,传输的特性(中断传输)
    WORD(8),                                    //wMaxPacketSize,0x08(最大封包)
    0x01,                                        //bInterval,轮询时间(1ms/unit)..

//Keyboard CONFIGURATION
//INTERFACE(9 bytes)
    USB_INTERFACE_DESC_SIZE,                    //bLength,9bytes
    USB_INTERFACE_DESCRIPTOR_TYPE,              //Descriptor type,0x04(接口描述符)
    0x01,                                       //bInterfaceNumber
    0x00,                                       //bAlternateSetting
    0x01,                                       //bNumEndpoints,0x01(端点数目)
    USB_DEVICE_CLASS_HUMAN_INTERFACE,           //bInterfaceClass,接口群组,USB规定HID码为0x03
    HID_SUBCLASS_BOOT,                          //bInterfaceSubClass,接口次群组,为0x01
    HID_PROTOCOL_KEYBOARD,                      //bInterfaceProtocol
    0x00,                                       //iInterface,接口字符串描述符的索引

//HID(9 bytes)
    HID_DESC_SIZE,                              //bLength,9bytes
    HID_HID_DESCRIPTOR_TYPE,                    //Descriptor type,0x21(HID描述符)
    WORD(HID_VERSION),                          //bcdHID
    0x00,                                       //bCountryCode(没有就设为0)
    0x01,                                       //bNumDescriptors,需遵循的HID群组报告的数目
    HID_REPORT_DESCRIPTOR_TYPE,                 //bDescriptorType,0x22(报告描述符).
    WORD(67),                                   //wDescriptorLength,报告描述符的长度

//ENDPOINT(7 bytes)
    USB_ENDPOINT_DESC_SIZE,                     //bLength,7bytes
    USB_ENDPOINT_DESCRIPTOR_TYPE,               //Descriptor type,0x05(端点描述符)
    USB_ENDPOINT_IN(2),                         //bEndpointAddress //luwei
    USB_ENDPOINT_TYPE_INTERRUPT,                //bmAttributes,传输的特性(中断传输)
    WORD(8),                                    //wMaxPacketSize,0x08(最大封包)
    0x01,                                       //bInterval,轮询时间(1ms/unit)..

//Define CONFIGURATION																			
//INTERFACE(9 bytes)																				//20130826
    USB_INTERFACE_DESC_SIZE,                    //bLength,9bytes
    USB_INTERFACE_DESCRIPTOR_TYPE,              //Descriptor type,0x04(接口描述符)
    0x02,                                       //bInterfaceNumber
    0x00,                                       //bAlternateSetting
    0x01,                                       //bNumEndpoints,0x01(端点数目)
    USB_DEVICE_CLASS_HUMAN_INTERFACE,           //bInterfaceClass,接口群组,USB规定HID码为0x03
    0,					                        //bInterfaceSubClass,接口次群组,为0x01				//20130826
    0,                         					//bInterfaceProtocol								//20130826
    0x00,                                       //iInterface,接口字符串描述符的索引

//HID(9 bytes)
    HID_DESC_SIZE,                              //bLength,9bytes
    HID_HID_DESCRIPTOR_TYPE,                    //Descriptor type,0x21(HID描述符)
    WORD(HID_VERSION),                          //bcdHID
    0x00,                                       //bCountryCode(没有就设为0)
    0x01,                                       //bNumDescriptors,需遵循的HID群组报告的数目
    HID_REPORT_DESCRIPTOR_TYPE,                 //bDescriptorType,0x22(报告描述符).
    WORD(116),                                  //wDescriptorLength,报告描述符的长度				//20130826

//ENDPOINT(7 bytes)
    USB_ENDPOINT_DESC_SIZE,                     //bLength,7bytes
    USB_ENDPOINT_DESCRIPTOR_TYPE,               //Descriptor type,0x05(端点描述符)
    USB_ENDPOINT_IN(3),                         //bEndpointAddress
    USB_ENDPOINT_TYPE_INTERRUPT,                //bmAttributes,传输的特性(中断传输)
    WORD(8),                                    //wMaxPacketSize,0x08(最大封包)
    0x01                                        //bInterval,轮询时间(1ms/unit)..
};

unsigned char const DeviceHidDescriptor[]={
    HID_DESC_SIZE,                              //bLength,9bytes
    HID_HID_DESCRIPTOR_TYPE,                    //Descriptor type,0x21(HID描述符)
    WORD(HID_VERSION),                          //bcdHID
    0x00,                                       //bCountryCode(没有就设为0)
    0x01,                                       //bNumDescriptors,需遵循的HID群组报告的数目
    HID_REPORT_DESCRIPTOR_TYPE,                 //bDescriptorType,0x22(报告描述符).
    WORD(67)                                    //wDescriptorLength,报告描述符的长度
};

unsigned char const HID_ReportDescriptor_Mouse[] = {
	//MOUSE REPORT
    HID_UsagePage(HID_USAGE_PAGE_GENERIC),
    HID_Usage(HID_USAGE_GENERIC_MOUSE),
    HID_Collection(HID_Application),
    //HID_ReportID(REPORT_ID_MOUSE),	          //20130826
      HID_Usage(HID_USAGE_GENERIC_POINTER),
      HID_Collection(HID_Physical),
        HID_UsagePage(HID_USAGE_PAGE_BUTTON),
        HID_UsageMin(1),
        HID_UsageMax(8),
        HID_LogicalMin(0),
        HID_LogicalMax(1),
        HID_ReportCount(8),
        HID_ReportSize(1),
        //HID_Input(HID_Data | HID_Variable | HID_Absolute),
        HID_Input(HID_Data | HID_Variable | HID_Relative), //luwei
        HID_UsagePage(HID_USAGE_PAGE_GENERIC),
        HID_Usage(HID_USAGE_GENERIC_X),
        HID_Usage(HID_USAGE_GENERIC_Y),
        HID_LogicalMinS(0xF801),                // HID_LogicalMinS(-2047)
        HID_LogicalMaxS(0x07FF),                // HID_LogicalMaxS(2047)
        //HID_ReportSize(12),
        HID_ReportSize(8), //luwei
        HID_ReportCount(2),
        HID_Input(HID_Data | HID_Variable | HID_Relative),
        HID_Usage(HID_USAGE_GENERIC_WHEEL),
        HID_LogicalMin(0x81),                   // HID_LogicalMin(-127)
        HID_LogicalMax(0x7F),                   // HID_LogicalMax(127)
        HID_ReportSize(8),
        HID_ReportCount(1),
        HID_Input(HID_Data | HID_Variable | HID_Relative),
        HID_UsagePage(HID_USAGE_PAGE_CONSUMER),
        HID_UsageS(0x0238),
        HID_ReportCount(1),
        HID_Input(HID_Data | HID_Variable | HID_Relative),
      HID_EndCollection,
    HID_EndCollection,
};

unsigned char const HID_ReportDescriptor_Define[] = {			//20130826
//CUSTOMER_DATA
    HID_UsagePageVendor(0xB3),
    HID_Usage(0x01),
    HID_Collection(HID_Application),
      HID_ReportID(REPORT_ID_CUSTOMER_DATA),
      HID_Usage(0x02),
      HID_LogicalMin(0),
      HID_LogicalMaxS(0xFF),
      HID_ReportSize(8),
      HID_ReportCount(7),
      HID_Input(HID_Data | HID_Variable | HID_Absolute),
      HID_Usage(0x02),
      HID_LogicalMin(0),
      HID_LogicalMaxS(0xFF),
      HID_ReportSize(8),
      HID_ReportCount(7),
      HID_Output(HID_Data | HID_Variable | HID_Absolute),
    HID_EndCollection,

//MULTIM KEY REPORT
    HID_UsagePage(HID_USAGE_PAGE_CONSUMER),
    HID_Usage(HID_USAGE_CONSUMER_CONTROL),
    HID_Collection(HID_Application),
      HID_ReportID(REPORT_ID_MULTIM_KEY),
      HID_UsageMin(1),
      HID_UsageMaxS(0x03FF),
      HID_LogicalMin(1),
      HID_LogicalMaxS(0x03FF),
      HID_ReportCount(1),
      HID_ReportSize(16),
      HID_Input(HID_Data | HID_Array),
    HID_EndCollection,

//SYSTEM KEY REPORT
    HID_UsagePage(HID_USAGE_PAGE_GENERIC),
    HID_Usage(HID_USAGE_GENERIC_SYSTEM_CTL),
    HID_Collection(HID_Application),
      HID_ReportID(REPORT_ID_SYSTEM_KEY),
      HID_UsageMinS(0x81),
      HID_UsageMaxS(0x83),
      HID_LogicalMin(0),
      HID_LogicalMax(1),
      HID_ReportCount(3),
      HID_ReportSize(1),
      HID_Input(HID_Data | HID_Variable | HID_Absolute),
      HID_ReportCount(5),
      HID_Input(HID_Constant),
    HID_EndCollection,

//MCE KEY REPORT
    HID_UsagePageVendor(0xBC),
    HID_Usage(0x88),
    HID_Collection(HID_Application),
      HID_ReportID(REPORT_ID_MCE_KEY),
      HID_UsageMin(0x00),
      HID_UsageMaxS(0xFF),
      HID_LogicalMin(0x00),
      HID_LogicalMaxS(0xFF),
      HID_ReportCount(1),
      HID_ReportSize(8),
      HID_Input(HID_Data | HID_Array),
    HID_EndCollection
};

//HID Report Descriptor
unsigned char const HID_ReportDescriptor_Keyboard[] = {
    HID_UsagePage(HID_USAGE_PAGE_GENERIC),      //2byte
    HID_Usage(HID_USAGE_GENERIC_KEYBOARD),      //2byte
    HID_Collection(HID_Application),            //2byte
      HID_UsagePage(HID_USAGE_PAGE_LED),        //2byte
      HID_UsageMin(0x01),                       //2byte
      HID_UsageMax(0x05),                       //2byte
      HID_LogicalMin(0),                        //2byte
      HID_LogicalMax(1),                        //2byte
      HID_ReportCount(5),                       //2byte
      HID_ReportSize(1),                        //2byte
      HID_Output(HID_Data | HID_Variable | HID_Absolute),//2byte
      HID_ReportCount(1),                       //2byte
      HID_ReportSize(3),                        //2byte
      HID_Output(HID_Constant),                 //2byte
      HID_UsagePage(HID_USAGE_PAGE_KEYBOARD),   //2byte
      HID_UsageMinS(0xE0),                      //3byte
      HID_UsageMaxS(0xE7),                      //3byte
      HID_LogicalMin(0),                        //2byte
      HID_LogicalMax(1),                        //2byte
      HID_ReportCount(8),                       //2byte
      HID_ReportSize(1),                        //2byte
      HID_Input(HID_Data | HID_Variable | HID_Absolute),//2byte
      HID_ReportCount(1),                       //2byte
      HID_ReportSize(8),                        //2byte
      HID_Input(HID_Constant),                  //2byte
      HID_UsageMin(0),                          //2byte
      HID_UsageMaxS(0xFF),                      //3byte
      HID_LogicalMin(0),                        //2byte
      HID_LogicalMaxS(0xFF),                    //3byte
      HID_ReportCount(6),                       //2byte
      HID_Input(HID_Data | HID_Array),          //2byte
    HID_EndCollection                           //1byte
};
#endif 

//LANGIDs Descriptor
unsigned char const String0Descriptor[] = {
    USB_STRING_DESC_SIZE,                       //bLength
    USB_STRING_DESCRIPTOR_TYPE,                 //bDescriptorType
    WORD(0x0409)                                //wLANGID,US English
};


/**
 * This routine sets the data pointer and size to correct descriptor and
 * sets the endpoint status to transmit.
 */
void USB_Get_Descriptor(void)                       
{     
	uint8_t offset;   
   switch(g_USB_Setup.wValue.c[LSB])                  // Determine which type of descriptor was requested, and set data ptr and size accordingly
   {                                             
      case DSCR_TYPE_DEV://获取设备描述符
			g_USB_DataPtr = (uint8_t*) DEVICE_Descriptor;
          g_USB_DataSize = DEVICE_Descriptor[0];
          break;
      case DSCR_TYPE_CONFIGURATION:           //配置描述符
			g_USB_DataPtr=(uint8_t *) CONFIGURATION_Descriptor;
          g_USB_DataSize= CONFIGURATION_Descriptor[2]+(CONFIGURATION_Descriptor[3]<<8);
          break;
      case DSCR_TYPE_STRING:
          switch(g_USB_Setup.wValue.c[MSB])//wValue的低字节用于选择同一描述符中具体某个描述符
          {
              case 0:     //get language ID
				g_USB_DataSize=String0Desc[0];
				g_USB_DataPtr=(uint8_t *)String0Desc;
                  break;
              case 1:   //manufacture
                  g_USB_DataSize=ManufactureStringDescriptor[0];
                  g_USB_DataPtr=(uint8_t *)ManufactureStringDescriptor;
									break;

              case 2:    //product
                  g_USB_DataSize=ProductStringDescriptor[0];
                  g_USB_DataPtr=(uint8_t *)ProductStringDescriptor;
									break;

              case 3:   //serial num
                  g_USB_DataSize=Serial_Num_Descriptor[0];
                  g_USB_DataPtr=(uint8_t *)Serial_Num_Descriptor; 
                  break;
              default:
                  g_USB_DataSize=0x00;
                  g_USB_DataPtr=NULL;
				break;
			}
			break;
#ifndef USB_APP_CDC
		case DSCR_TYPE_HID: 
			switch(g_USB_Setup.wIndex.c[MSB])
			{
				case 0:
					offset = USB_CONFIGUARTION_DESC_SIZE+USB_INTERFACE_DESC_SIZE;
					g_USB_DataPtr = (uint8_t*)(CONFIGURATION_Descriptor+offset);
					g_USB_DataSize = DeviceHidDescriptor[0];
					break;
              case 1:
			      offset = USB_CONFIGUARTION_DESC_SIZE+USB_INTERFACE_DESC_SIZE+HID_DESC_SIZE+USB_ENDPOINT_DESC_SIZE+USB_INTERFACE_DESC_SIZE;
				  g_USB_DataPtr = (uint8_t*)(CONFIGURATION_Descriptor+offset);
				  g_USB_DataSize = DeviceHidDescriptor[0];
                  break;
              case 2:
			      offset = USB_CONFIGUARTION_DESC_SIZE+USB_INTERFACE_DESC_SIZE+HID_DESC_SIZE+USB_ENDPOINT_DESC_SIZE+USB_INTERFACE_DESC_SIZE+HID_DESC_SIZE+USB_ENDPOINT_DESC_SIZE+USB_INTERFACE_DESC_SIZE;
				  g_USB_DataPtr = (uint8_t*)(CONFIGURATION_Descriptor+offset);
				  g_USB_DataSize = DeviceHidDescriptor[0];
                  break;
              default:
					USB_Ep0_Send_Stall();
					break;
			}
			break;

		case DSCR_TYPE_HID_REPORT: 
			//************////添加报告描述符
			switch(g_USB_Setup.wIndex.c[MSB])
			{
				case 0:
                  g_USB_DataPtr=(uint8_t*)HID_ReportDescriptor_Mouse; 
                  g_USB_DataSize=sizeof(HID_ReportDescriptor_Mouse);
                  break;
              case 1:
                  g_USB_DataPtr=(uint8_t*)HID_ReportDescriptor_Keyboard; 
                  g_USB_DataSize=sizeof(HID_ReportDescriptor_Keyboard);
                  break;
              case 2:
                  g_USB_DataPtr=(uint8_t*)HID_ReportDescriptor_Define; 
                  g_USB_DataSize=sizeof(HID_ReportDescriptor_Define);
                  break;
				default:
					USB_Ep0_Send_Stall();
					break;
          }
          break;
#endif
      default:
          USB_Ep0_Send_Stall();
          break;
   }

   if ( (g_USB_Setup.wValue.c[LSB] == DSCR_TYPE_DEV) ||  (g_USB_Setup.wValue.c[LSB] == DSCR_TYPE_CONFIGURATION) || (g_USB_Setup.wValue.c[LSB] == DSCR_TYPE_STRING) || (g_USB_Setup.wValue.c[LSB] == DSCR_TYPE_INTERFACE) || (g_USB_Setup.wValue.c[LSB] == DSCR_TYPE_ENDPOINT) ) 
   {
      if ((g_USB_Setup.wLength.c[MSB] < g_USB_DataSize) &&  (g_USB_Setup.wLength.c[LSB] == 0))    // Send only requested amount of data
      {
         g_USB_DataSize = g_USB_Setup.wLength.i;      
      }
   }
   if (g_USB_Ep_Status[0] != EP_STALL)                // Make sure endpoint not in stall mode
   {
     USB_Write_Reg(M_REG_CSR0, CSR0_SOPRDY);          // Service Setup Packet, 清除设备收到数据标志
     g_USB_Ep_Status[0] = EP_TX;                      // Put endpoint in transmit mode
     g_USB_DataSent = 0;                              // Reset Data Sent counter
   }
}

/*----------------------------------End Of File-------------------------------*/
