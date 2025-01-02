//------------------------------------------------------------------------------------
// spi.h
//------------------------------------------------------------------------------------
// Copyright (c) 2013, HunterSun Electronics Co., Ltd.
// Huiqing, Zheng
// 2013-12-12
//
// Program Description: Header file for SPI.
//
//
//

#ifndef __SPI_H__
#define __SPI_H__

/*--------------------SPI MASTER-----------------*/

//--------------SPIMCON0----------------
#define MSPI_3_WIRE         0x80         /* SPI 3-wire mode */       

#define MSPI_CLK_SFT        4            /* MSCK clock divided from ckMCU */
#define MSPI_CLK            (0x7 << MSPI_CLK_SFT)        //(BIT6+BIT5+BIT4)
#define MSPI_CLK_2          (0x0 << MSPI_CLK_SFT)
#define MSPI_CLK_4          (0x1 << MSPI_CLK_SFT)        //BIT4
#define MSPI_CLK_8          (0x2 << MSPI_CLK_SFT)        //BIT5
#define MSPI_CLK_16         (0x3 << MSPI_CLK_SFT)        //(BIT4+BIT5)
#define MSPI_CLK_32         (0x4 << MSPI_CLK_SFT)        //BIT6
#define MSPI_CLK_64         (0x5 << MSPI_CLK_SFT)        //(BIT6+BIT4)
 
#define MSPI_DATA_LSB       0x08         /* SPI data order,LSBit first,MSBit last */
#define MSPI_DATA_MSB       0x00         /* SPI data order, MSBit first,LSBit last */ 

#define MSPI_CPOL_LOW       0x04         /* MCLK active low */
#define MSPI_CPOL_HIGH      0x00         /* MCLK active high */ 

#define MSPI_CPHA_EVEN      0x02         /* Sampling of data occurs at even edges (2,4,6,...,16) of the SCK clock */
#define MSPI_CPHA_ODD       0x00         /* Sampling of data occurs at odd edges (1,3,5,...,15) of the SCK clock */ 

#define MSPI_EN             0x01         /* SPI Master is enabled. The clock to the SPI */


//--------------SPIMCON1----------------
#define MSPI_O_SEL          0x80      /* 0: api_master output to GPIO p1.4-p1.6
                                         1: spi master output to GPIO p2.0-p2.2 */
#define MSPI_2_WIRE         0x40      /* SPI 2 wire mode */
#define MSPI_CSN_EN         0x20      /* generate CSN */
#define MSPI_BURST 	    0x10      /* burst enable, used in 4-wire mode */
#define MSPI_MOSI_FLAG      0x08      /* 0: mosi output; 1: mosi input */
#define MSPI_RE_SYNC        0x04      /* 0: no sync; 1: sync */
#define MSPI_RETRANS_EN     0x02      /* retransfer again after wait 1.7ms re-sync mode, used in 2-wire mode */
#define MSPI_MOSI_OUT_EN    0x01      /* 0: mosi is output; 1: mosi maybe input
                                         used in 2-wire or 3-wire mode */


//--------------SPIMSTAT----------------
#define MSPI_RX_FIFO_FULL            0x80  //BIT3      /*RX FIFO full.*/ 
#define MSPI_RX_DATA_RDY             0x40  //BIT2      /*Data available in RX FIFO*/
#define MSPI_TX_FIFO_EMPTY           0x20  //BIT1      /*TX FIFO empty.*/
#define MSPI_TX_FIFO_RDY             0x10  //BIT0      /*Location available in TX FIFO*/
#define MSPI_MASK_RX_FIFO_FULL_INT   0x08  //BIT3          /*Disable interrupt when RX FIFO is full.*/    
#define MSPI_MASK_RX_DATA_RDY_INT    0x04  //BIT2          /*Disable interrupt when data is available in RX FIFO*/
#define MSPI_MASK_TX_FIFO_EMPTY_INT  0x02  //BIT1          /*Disable interrupt when TX FIFO is empty.*/
#define MSPI_MASK_TX_FIFO_RDY_INT    0x01  //BIT0          /*Disable interrupt when a location is available in TX FIFO*/

//--------------SPIDLY----------------
#define MSPI_CSN_DLY                 0xF0  /* CSN delay counter, used in 4-wire mode */
#define MSPI_CSN_DLY_SFT             4
#define MSPI_GAP_DLY                 0x0F  /* GAP delay counter */
#define MSPI_GAP_DLY_SFT             0

//--------------SPITEST----------------
#define MSPI_MASK_TX_UNDERFLOW       (1 << 7)
#define MSPI_MASK_TX_OVERFLOW        (1 << 6)
#define MSPI_MASK_RX_UNDERFLOW       (1 << 5)
#define MSPI_MASK_RX_OVERFLOW        (1 << 4)
#define MSPI_TX_UNDERFLOW_FLAG       (1 << 3)
#define MSPI_TX_OVERFLOW_FLAG        (1 << 2)
#define MSPI_RX_UNDERFLOW_FLAG       (1 << 1)
#define MSPI_RX_OVERFLOW_FLAG        (1 << 0)

/*---------------------------SPI SLAVE--------------------------*/
//--------------SPISCON0----------------
#define SSPI_MASK_CSN_HIGH_INT       0x40  /* 0: enable interrupt when SCSN goes high
					      1: disable interrupt when SCSN goes high */
#define SSPI_MASK_CSN_LOW_INT        0x20  /* 0: enable interrupt when SCSN goes low
					      1: disable interrupt when SCSN goes low */
#define SSPI_MASK_TRANS_DONE         0x10  /* 0: enable interrupt when the current transaction is done
					      1: disable interrupt when the current transaction is done */
#define SSPI_DATA_LSB                0x08  /* SPI data order,LSBit first,MSBit last */
#define SSPI_DATA_MSB                0x00  /* SPI data order, MSBit first,LSBit last */ 

#define SSPI_CPOL_LOW                0x04  /* MCLK active low */
#define SSPI_CPOL_HIGH               0x00  /* MCLK active high */ 

#define SSPI_CPHA_EVEN               0x02  /* Sampling of data occurs at even edges (2,4,6,...,16) of the SCK clock */
#define SSPI_CPHA_ODD                0x00  /* Sampling of data occurs at odd edges (1,3,5,...,15) of the SCK clock */ 
#define SSPI_EN                      0x01  /* SPI Saster is enabled. The clock is output to the SPI slave */

//--------------SPISSTAT----------------
#define SSPI_TX_FIFO_FULL            (1 << 7)
#define SSPI_CS_DEASSERT             (1 << 5)
#define SSPI_CS_ASSERT               (1 << 4)
#define SSPI_XFER_DONE               (1 << 0)
 
#endif  /* __SPI_H__ */

/*-------------------------------------------End Of File---------------------------------------------*/
