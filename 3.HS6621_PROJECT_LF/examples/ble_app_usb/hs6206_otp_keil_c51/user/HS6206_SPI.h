//------------------------------------------------------------------------------------
// HS6206_SPI.h
//------------------------------------------------------------------------------------
// Copyright 2014, HunterSun Electronics Co., Ltd.
// wei.lu@huntersun.com.cn
// 2014-02-11
//
// Program Description:
//	 SPI master & slave APIs

#ifndef __HS6206_SPI_H__
#define __HS6206_SPI_H__

#include <hs6206_types.h>
#include <spi.h>

/* SPI mode flags */
#define	SPI_CPHA	0x01			/* clock phase */
#define	SPI_CPOL	0x02			/* clock polarity */
#define	SPI_MODE_0	(0|0)			/* (original MicroWire) */
#define	SPI_MODE_1	(0|SPI_CPHA)
#define	SPI_MODE_2	(SPI_CPOL|0)
#define	SPI_MODE_3	(SPI_CPOL|SPI_CPHA)
#define	SPI_CS_HIGH	0x04			/* CS active high */
#define	SPI_LSB_FIRST	0x08			/* per-word bits-on-wire */
#define	SPI_3WIRE	0x10			/* SI/SO signals shared */

/* SPI transfer flags */
#define SPI_XFER_BEGIN	0x01			/* Assert CS before transfer */
#define SPI_XFER_END	0x02			/* Deassert CS after transfer */

/*-----------------------------------------------------------------------
 * Set up SPI master communications parameters for a SPI slave.
 *
 *   cs:      Chip select's GPIO, such as 0x14 (P1.4, CS in software) or 
              -1 (CS in hardware & transfer in interrupt mode, using bank 3) or 
              -2 (no CS)
 *   max_hz:  Maximum SCK rate in Hz.
 *   mode:    Clock polarity, clock phase and other parameters.
 *
 * Returns: 0 if setup SPI bus sucessfully,
 * or a negative value if one or more of the parameters are not supported.
 */
int spi_master_setup(unsigned char cs, unsigned int max_hz, unsigned char mode);

/*-----------------------------------------------------------------------
 * SPI transfer
 *
 * This writes "bitlen" bits out the SPI MOSI port and simultaneously clocks
 * "bitlen" bits in the SPI MISO port.  That's just the way SPI works.
 *
 * The source of the outgoing bits is the "dout" parameter and the
 * destination of the input bits is the "din" parameter.  Note that "dout"
 * and "din" can point to the same memory location, in which case the
 * input data overwrites the output data (since both are buffered by
 * temporary variables, this is OK).
 *
 *   len:	How many bytes to write and read.
 *   dout:	Pointer to a string of bytes to send out.
 *   din:	Pointer to a string of bytes that will be filled in.
 *   flags:	A bitwise combination of SPI_XFER_* flags.
 *
 *   Returns: 0 on success, not 0 on failure
 */
int  spi_master_xfer(unsigned int len, const unsigned char *dout,
		unsigned char *din, unsigned char flags);

/*-----------------------------------------------------------------------
 * Activate a SPI chipselect.
 * This function is provided by the board code when using a driver
 * that can't control its chipselects automatically.
 */
void spi_cs_activate(void);

/*-----------------------------------------------------------------------
 * Deactivate a SPI chipselect.
 * This function is provided by the board code when using a driver
 * that can't control its chipselects automatically.
 */
void spi_cs_deactivate(void);

/*-----------------------------------------------------------------------
 * Set up SPI slave communications parameters.
 *
 *   mode:    Clock polarity, clock phase and other parameters.
 *
 * Returns: 0 if setup SPI bus sucessfully,
 * or a negative value if one or more of the parameters are not supported.
 */
int spi_slave_setup(unsigned char mode);

/*-----------------------------------------------------------------------
 * User implements these callback for CS assert, data transfer, CS deassert
 */
void spi_slave_start_callback(void);
void spi_slave_rx_callback(unsigned char val);
void spi_slave_end_callback(void);

#endif  /*__HS6206_SPI_H__*/

/*-------------------------------------------End Of File----------------------*/
