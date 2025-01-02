//------------------------------------------------------------------------------------
// om6621pt_usb_interface.h
//------------------------------------------------------------------------------------
// Copyright (c) 2022, OnMicro Electronics Co., Ltd.
// shangzhou.hu@onmicro.com.cn
// 2022-04-08
//
// Program Description: Header file for USB interface.
//
//
//

#ifndef __OM6621PT_USB_INTERFACE_H__
#define __OM6621PT_USB_INTERFACE_H__

/*
  通信协议说明：
    1. SFR 读写：
       用于读写HS6206 MCU内部特殊功能寄存器(SFR, 定义在hs6206_reg.h)
       主要包含两个API：
       uint8_t read_sfr_reg(uint8_t addr);                -> spi数据  0x21[MOSI] addr[MOSI] reg_val[MISO]
       void write_sfr_reg(uint8_t addr, uint8_t reg_val); -> spi数据  0x01[MOSI] addr[MOSI] reg_val[MOSI]
       
    2. USB寄存器读写：
       用于读写HS6206 USB模块寄存器(定义在USB_Regs.h)
       主要包含两个API：
       uint8_t usb_read_reg(uint8_t addr);            -> spi数据  0x41[MOSI] addr[MOSI] reg_val[MISO]
       void usb_write_reg(uint8_t addr, uint8_t val); -> spi数据  0x61[MOSI] addr[MOSI] reg_val[MOSI]
       
    3. USB操作子程序：
       用于操作HS6206 USB模块，这部分是为了减小SPI传输，快速实现对USB的相关操作，不使用这些子函数仅通过1和2的寄存器操作也可以实现功能 
       主要包含如下功能API：

       void usb_open_interrupt(void); ->处理完USB中断程序或发送完数据包，需要调用这个函数打开USB中断[IEN1第4bit置1]。spi数据 0x01[MOSI] 0xB8[MOSI] 0x10[MOSI]

       void usb_close_interrupt(void); ->发送数据包写fifo或其他不希望被USB中断打断的情况，调用这个函数关闭usb中断[IEN1第4bit淸0]。spi数据 0x01[MOSI] 0xB8[MOSI] 0x00[MOSI]

       void usb_get_irq(void); -> 6206在产生USB中断后拉高IRQ引脚，再读取三个中断寄存器的值并关闭中断，然后通过IRQ引脚通知OM6621P，OM6621P
       通过调用这个函数获取三个中断寄存器的值，然后执行相应操作。 spi数据  0x83[MOSI] IntUSB[MISO] IntIn[MISO] IntOut[MISO]

       void usb_start(void); ->初始化USB模块，包括打开时钟，配置中断之类的，如用户有特殊配置，可通过读写SFR和USB寄存器覆盖相关配置。spi数据  0x81[MOSI] 0x00[MOSI]

       void usb_dp_pullup(void); ->调用本函数初始化usb模块后，调用本函数D+数据线上的1.5K上拉电阻开启，USB主机端检测到设备接入，开始枚举。 spi数据 0x01[MOSI] 0xDB[MOSI] 0x90[MOSI]

       void usb_dp_pulldown(void); ->调用本函数后，D+数据线上的1.5K上拉电阻关闭，USB主机端检测到设备断开。 spi数据 0x01[MOSI] 0xDB[MOSI] 0x80[MOSI]

       void usb_read_fifo(uint8_t ep,uint8_t *pdata,uint8_t size); ->读取USB fifo. spi数据 0x80|size[MOSI] (ep<<6)|2[MOSI] data...[MISO]

       void usb_write_fifo(uint8_t ep,uint8_t *pdata,uint8_t size); ->读取USB fifo. spi数据 0x80|size[MOSI] (ep<<6)|3[MOSI] data...[MOSI]

*/

/* 
    硬件连接关系,SPI使用模式2，HS6206 USB模块产生中断后IRQ拉高，读取中断标志后拉低,下降沿触发OM6621P中断
    OM6621P_SPIM  HS6206_SPIS   SPI
       GPIO<12>     P1.2	    CSN
       GPIO<13>     P2.1	    MISO
       GPIO<14>     P2.0	    SCK
       GPIO<15>     P1.3	    MOSI
       GPIO<16>     P1.0	    IRQ
    
*/

#define CMD_SFR_WRITE        0x00
#define CMD_SFR_READ         0x01
#define CMD_USB_REG_WRITE    0x02
#define CMD_USB_REG_READ     0x03
#define CMD_FUNC_CALL        0x04
#define CMD_SHIFT            5

#define USB_IRQ_PIN          P0_7

enum 
  {
    FUNC_USB_INIT=0,
    FUNC_USB_GET_IRQ,
    FUNC_USB_READ_FIFO,
    FUNC_USB_WRITE_FIFO,
    FUNC_USB_SEND_PKT,

  };

// End Enum's


#endif /* end of __OM6621PT_USB_INTERFACE_H__ */
/*-------------------------------------------End Of File---------------------------------------------*/
