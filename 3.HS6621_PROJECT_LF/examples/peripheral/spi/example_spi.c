/**
 * @file example_spi.c
 * @brief example for SPI
 * @date 2017/07/12 16:20:32
 * @author liqiang
 *
 * @defgroup EXAMPLE_SPI SPI Example
 * @ingroup EXAMPLE
 * @brief example for SPI
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

#define SPI_USE_DMA

/*********************************************************************
 * LOCAL VARIABLES
 */
static spi_dma_t spi_dma = {0};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_spi(void)
{
    __align(4) uint8_t txbuf[100] = {0}; // SPI DMA request 4bytes align
    __align(4) uint8_t rxbuf[100] = {0}; // SPI DMA request 4bytes align
    int i;

    for(i=0;i<100;i++)
        txbuf[i]=i;

    // Pinmux
    pinmux_config(27,  PINMUX_SPI0_MST_SDA_I_CFG); /*MISO*/
    pinmux_config(28,  PINMUX_SPI0_MST_SDA_O_CFG); /*MOSI*/
    pinmux_config(29,  PINMUX_SPI0_MST_SCK_CFG);   /*SCLK*/
    pinmux_config(30,  PINMUX_SPI0_MST_CSN_CFG);   /*CSN */

    // SPI Init
    spi_open(HS_SPI0, SPI_MODE_MASTER, SPI_TRANS_MODE_0, 200000);

#ifdef SPI_USE_DMA
    dma_init();
    spi_dma_config(HS_SPI0, &spi_dma, NULL);
    spi_master_cs_low(HS_SPI0);
    spi_master_exchange_dma(HS_SPI0, &spi_dma, txbuf, rxbuf, 100);
    spi_master_cs_high(HS_SPI0);
#else
    // Exchange data
    spi_master_cs_low(HS_SPI0);
    spi_master_exchange(HS_SPI0, txbuf, rxbuf, 100);
    spi_master_cs_high(HS_SPI0);
#endif

    while(1);
}

/** @} */

