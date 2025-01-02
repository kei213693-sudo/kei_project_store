
#include "spi_flash.h"
#include <stdio.h>

void sflash_init(HS_SPI_Type *spi, spi_dma_t *dma)
{
    if (spi == NULL)
        spi = HS_SPI0;
   
    spi_open(spi, SPI_MODE_MASTER, SPI_TRANS_MODE_0, 1*1000);
    if (dma) {
        spi_dma_config(spi, dma, NULL);
    }
}

uint32_t sflash_read_jedec_id(HS_SPI_Type *spi, spi_dma_t *dma)
{
    uint8_t txbuf[4] = {0x9F, 0xFF, 0xFF, 0xFF};
    uint8_t rxbuf[4] = {0, 0, 0, 0};

    spi_master_cs_low(spi);
    if (dma)
        spi_master_exchange_dma(spi, dma, txbuf, rxbuf, 4);
    else
        spi_master_exchange(spi, txbuf, rxbuf, 4);
    spi_master_cs_high(spi);

    printf("%s", dma ? "dma: " : "man: ");
    printf("MID = 0x%02X, JEDEC_ID = 0x%02X%02X\r\n", rxbuf[1], rxbuf[2], rxbuf[3]);

    return *((uint32_t*)rxbuf);
}

void sflash_deinit(HS_SPI_Type *spi, spi_dma_t *dma)
{
    spi_close(spi);
    if (dma) {
        dma_release(dma->rx_dma);
        dma_release(dma->tx_dma);
    }
}
