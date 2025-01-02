/**
 *  Spi-flash hal layer for SPI testing.
 *  By hgz.
 *  2019.11.28.
 */

#include "spi.h"


void sflash_init(HS_SPI_Type *spi, spi_dma_t *dma);
uint32_t sflash_read_jedec_id(HS_SPI_Type *spi, spi_dma_t *dma);
void sflash_deinit(HS_SPI_Type *spi, spi_dma_t *dma);



