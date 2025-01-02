//------------------------------------------------------------------------------------
// USB_Regs.h
//------------------------------------------------------------------------------------
// Copyright 2013, crazy_code Electronics Co., Ltd.
// crazy_code
// 2013-07-29
//
// Program Description:
//
//
//

#ifndef __USB_REGS_H__
#define __USB_REGS_H__



extern volatile unsigned char xdata M_REG_BASE_ADDR;

//每个Endpoint的FIFO的size
#define IP_FIFO_SIZE             16
#define USB_EP0_FIFO_SIZE        IP_FIFO_SIZE
#define USB_EP1_FIFO_SIZE        IP_FIFO_SIZE
#define USB_EP2_FIFO_SIZE        IP_FIFO_SIZE
#define USB_EP3_FIFO_SIZE        IP_FIFO_SIZE
#define USB_EP4_FIFO_SIZE        IP_FIFO_SIZE
#define USB_EP5_FIFO_SIZE        IP_FIFO_SIZE

//common USB registers (address 0x00 to 0x0F)

#define M_REG_BASE_ADDR0      0x00
#define M_REG_FADDR           M_REG_BASE_ADDR0
#define M_REG_POWER          (M_REG_BASE_ADDR0+0x01)
#define M_REG_INTRIN1        (M_REG_BASE_ADDR0+0x02)
#define M_REG_INTRIN2        (M_REG_BASE_ADDR0+0x03)
#define M_REG_INTROUT1       (M_REG_BASE_ADDR0+0x04)
#define M_REG_INTROUT2       (M_REG_BASE_ADDR0+0x05)
#define M_REG_INTRUSB        (M_REG_BASE_ADDR0+0x06)
#define M_REG_INTRIN1E       (M_REG_BASE_ADDR0+0x07)
#define M_REG_INTRIN2E       (M_REG_BASE_ADDR0+0x08)
#define M_REG_INTROUT1E      (M_REG_BASE_ADDR0+0x09)
#define M_REG_INTROUT2E      (M_REG_BASE_ADDR0+0x0A)
#define M_REG_INTRUSBE       (M_REG_BASE_ADDR0+0x0B)
#define M_REG_FRAME1         (M_REG_BASE_ADDR0+0x0C)
#define M_REG_FRAME2         (M_REG_BASE_ADDR0+0x0D)
#define M_REG_INDEX          (M_REG_BASE_ADDR0+0x0E)

//indexed register(addresss 0x10 to 0x1F)
#define M_REG_INMAXP         (M_REG_BASE_ADDR0+0x10)
#define M_REG_CSR0           (M_REG_BASE_ADDR0+0x11)
#define M_REG_INCSR1         (M_REG_BASE_ADDR0+0x11)
#define M_REG_INCSR2         (M_REG_BASE_ADDR0+0x12)
#define M_REG_OUTMAXP        (M_REG_BASE_ADDR0+0x13)
#define M_REG_OUTCSR1        (M_REG_BASE_ADDR0+0x14)
#define M_REG_OUTCSR2        (M_REG_BASE_ADDR0+0x15) 
#define M_REG_COUNT0         (M_REG_BASE_ADDR0+0x16)
#define M_REG_OUTCOUNT1      (M_REG_BASE_ADDR0+0x16)
#define M_REG_OUTCOUNT2      (M_REG_BASE_ADDR0+0x17) 

//FIFOs(addresss 0x20 to 0x2F) 
#define M_REG_FIFOx(x)       (M_REG_BASE_ADDR0+0x20+(x)) 

#define USB_EP0               BIT0
#define USB_EP1               BIT1
#define USB_EP2               BIT2
#define USB_EP3               BIT3
#define USB_EP4               BIT4
#define USB_EP5               BIT5
#define USB_EP6               BIT6
#define USB_EP7               BIT7
#define USB_EP8               BIT8
#define USB_EP9               BIT9
#define USB_EP10              BIT10
#define USB_EP11              BIT11
#define USB_EP12              BIT12
#define USB_EP13              BIT13
#define USB_EP14              BIT14
#define USB_EP15              BIT15


/*-------------------------------- Register Bit define-------------------------------*/
#define USB_RSTMD        BIT3         /*reset*/    
#define USB_RESUME       BIT2         /*generate resume (remote wakeup)*/
#define USB_SUSMD        BIT1         /*suspend mode*/
#define USB_SUSEN        BIT0         /*suspend enable*/


/*---------------------------current status register--------------------------*/
#define CSR0_SSUEND                BIT7       /*serviced setup End*/
#define CSR0_SOPRDY                BIT6       /*serviced out packet ready*/
#define CSR0_SDSTL                 BIT5       /*send stall*/
#define CSR0_SUEND                 BIT4       /*setup end*/
#define CSR0_DATAEND               BIT3       /*data end bit*/
#define CSR0_STSTL                 BIT2       /*sent stall bit*/ 
#define CSR0_INPRDY                BIT1       /*In packet ready bit*/
#define CSR0_OPRDY                 BIT0       /*out packet ready bit*/


//interrupt flag
#define INT_SUS               BIT0     /*suspend interrupt*/ 
#define INT_RSU               BIT1     /*resume interrupt*/
#define INT_RST               BIT2     /*reset interrupt*/
#define INT_SOF               BIT3     /*set when SOF tikon*/  

#define INT_EP0              BIT0
#define INT_EP1              BIT1
#define INT_EP2              BIT2
#define INT_EP3              BIT3
#define INT_EP4              BIT4
#define INT_EP5              BIT5
#define INT_EP6              BIT6
#define INT_EP7              BIT7
#define INT_EP8              BIT8
#define INT_EP9              BIT9
#define INT_EP10             BIT10
#define INT_EP11             BIT11
#define INT_EP12             BIT12
#define INT_EP13             BIT13
#define INT_EP14             BIT14
#define INT_EP15             BIT15 

//OUTCSR include OUTCSR1 and OUTCSR2 register.
#define OUTCSR_OPRDY         BIT0     /*OUTCSR1*/
#define OUTCSR_FIFO_FULL     BIT1
#define OUTCSR_OVER_RUN      BIT2
#define OUTCSR_DATA_ERR      BIT3
#define OUTCSR_FLUSH_FIFO    BIT4
#define OUTCSR_SEND_STALL    BIT5
#define OUTCSR_SENT_STALL    BIT6
#define OUTCSR_CLR_DATA_TOG  BIT7

#define OUTCSR_AUTO_CLR      BIT7     /*OUTCSR2*/
#define OUTCSR_ISO           BIT6
#define OUTCSR_DMA_EN        BIT5
#define OUTCSR_DMA_MODE      BIT4 
 
//INCSR include INCSR1 and INCSR2 register
#define INCSR_IPRDY            BIT0
#define INCSR_FIFO_NOT_EMPTY   BIT1
#define INCSR_UNDRUN           BIT2
#define INCSR_FLUSH_FIFO       BIT3
#define INCSR_SDSTL            BIT4
#define INCSR_STSTL            BIT5
#define INCSR_CLRDT            BIT6

#define INCSR_AUTO_SET         BIT7
#define INCSR_TRX_MODE         BIT6
#define INCSR_TRX_MODE_ISC     BIT6
#define INCSR_TRX_MODE_INT     0x00
#define INCSR_TRX_MODE_BULK    0x00

#define INCSR_DIR              BIT5
#define INCSR_DIR_IN           BIT5
#define INCSR_DIR_OUT          0x00

#define INCSR_DMA_EN           BIT4
#define INCSR_FRC_DATA_TOG     BIT3


#endif  /*__USB_REGS_H__*/


/*-------------------------------End Of File----------------------------------*/

