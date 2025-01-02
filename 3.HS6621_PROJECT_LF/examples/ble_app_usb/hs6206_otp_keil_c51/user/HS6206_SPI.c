//------------------------------------------------------------------------------
// HS6206_SPI.c
//-----------------------------------------------------------------------------
// Copyright 2014, HunterSun Electronics Co., Ltd.
// wei.lu@huntersun.com.cn
// 2014-02-11
//
// Program Description:
//   SPI master & slave interfaces
//
//

#ifdef HS6206_SPI

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "HS6206_Types.h"
#include "HS6206_Regs.h"
#include "HS6206_Delay.h"
#include "HS6206_GPIO.h"
#include <HS6206_SPI.h>

/*---------------------------------------------SPI COMMON---------------------*/
#define RSTREAS_RST_SSPI         0x10       /* Writing 1 to reset SPI slave module */
#define RSTREAS_RST_MSPI         0x08       /* Writing 1 to reset SPI master module */
#define MSDONE_INT_EN              BIT1           /* Enable Master SPI completed (MSDONE)interrupt */
#define SSDONE_INT_EN              BIT0           /* Enable Slave SPI completed (SSDONE) interrupt */  
#define MISC_INT_EN            BIT2

static xdata char spi_ss_gpio;
static xdata char spi_ss_pol;
static xdata unsigned char *spi_din = NULL;
static xdata unsigned char *spi_dout = NULL;
static xdata int spi_len = 0;

/*
 * spi read & write byte
 */
 u8 spi_master_swap_byte(u8 byte)
{
	u8 value;

	while((SPIMSTAT & MSPI_TX_FIFO_EMPTY) != MSPI_TX_FIFO_EMPTY)
		;
	SPIMDAT = byte;
	while((SPIMSTAT & MSPI_RX_DATA_RDY) != MSPI_RX_DATA_RDY)
		;
	value = SPIMDAT;

	return value;
}

static void spi_master_swap_bytes(u8 *out, u8 *in)
{
	while((SPIMSTAT & MSPI_TX_FIFO_EMPTY) == 0)
		;
	SPIMDAT = in[0];
	SPIMDAT = in[1];
	while((SPIMSTAT & MSPI_RX_FIFO_FULL) == 0)
		;
	out[0] = SPIMDAT;
	out[1] = SPIMDAT;
}

void spi_cs_activate(void)
{
	if (spi_ss_gpio > 0) {
		gpio32_set_output(spi_ss_gpio, spi_ss_pol & 0x0f);
		delay_us(1);
	}
}

void spi_cs_deactivate(void)
{
	if (spi_ss_gpio > 0) {
		delay_us(1);
		gpio32_set_output(spi_ss_gpio, !(spi_ss_pol & 0x0f));
	}
}

int spi_master_setup(unsigned char cs, unsigned int max_hz, unsigned char mode)
{
	char clk_div;

	clk_div = (HS6206_SYSCLK + max_hz - 1) / max_hz;
	if (clk_div > 32)
		clk_div = MSPI_CLK_64;
	else if (clk_div > 16)
		clk_div = MSPI_CLK_32;
	else if (clk_div > 8)
		clk_div = MSPI_CLK_16;
	else if (clk_div > 4)
		clk_div = MSPI_CLK_8;
	else if (clk_div > 2)
		clk_div = MSPI_CLK_4;
	else
		clk_div = MSPI_CLK_2;

	/* FIXME: flush FIFOs */
	RSTREAS |= RSTREAS_RST_MSPI;
	delay_us(1);

	spi_ss_pol = (mode & SPI_CS_HIGH) ? 1 : 0;
	spi_ss_gpio = cs;
	if (cs == -2) {
//		debug("two-wire mode without CS\n");
		SPIMCON1 &= ~MSPI_CSN_EN;
		SPIMCON1 |= MSPI_2_WIRE;
	} else if (cs == -1) {
		/* hardware chip select */
		SPIMCON1 |= MSPI_CSN_EN;
		SPIDLY = 0xff; //maximum for gap delay & csn delay
		SPIMCON1 |= MSPI_BURST; //enable burst
	} else {
		/* software chip select in GPIO */
		SPIMCON1 &= ~MSPI_CSN_EN;
//		gpio32_set_output(spi_ss_gpio, !(spi_ss_pol));
//		gpio32_set_dir(spi_ss_gpio, GPIO_DIR_OUT);
		P1DIR &= ~0x10;
		P1_4 = !(spi_ss_pol);
	}

	/* MOSI & MISO is shared as SDIO */
	if ((mode & SPI_3WIRE) || (cs == -2))
		spi_ss_pol |= 0x10;

	SPIMCON0 = (mode & SPI_3WIRE ? MSPI_3_WIRE : 0)
		| clk_div
		| (mode & SPI_LSB_FIRST ? 0 : MSPI_DATA_MSB)
		| ((mode & 0x03) << 1) //SPI_MODE_0,1,2,3
		| MSPI_EN;

	if (-1 == cs) {
		SPIMSTAT &= ~(MSPI_MASK_RX_DATA_RDY_INT | MSPI_MASK_TX_FIFO_RDY_INT);
		SPITEST &= ~(MSPI_MASK_TX_UNDERFLOW | MSPI_MASK_TX_OVERFLOW | MSPI_MASK_RX_UNDERFLOW | MSPI_MASK_RX_OVERFLOW);
		/* enable Master SPI comppleted interrupt */
		INTEXP |= MSDONE_INT_EN;
	} else {
		/* mask all interrupts to poll if not HW CS */
		SPIMSTAT |= MSPI_MASK_RX_DATA_RDY_INT | MSPI_MASK_TX_FIFO_RDY_INT | MSPI_MASK_RX_FIFO_FULL_INT | MSPI_MASK_TX_FIFO_EMPTY_INT;
		SPITEST |= MSPI_MASK_TX_UNDERFLOW | MSPI_MASK_TX_OVERFLOW | MSPI_MASK_RX_UNDERFLOW | MSPI_MASK_RX_OVERFLOW;
//		IEN1 &= ~MISC_INT_EN;
	}
	return 0;
}

int spi_master_xfer(unsigned int len, unsigned char *dout, unsigned char *din, unsigned char flags)
{
	int i, ret;

	ret = 0;
	if (len == 0)
		/* Finish any previously submitted transfers */
		goto out;

	if (-1 == spi_ss_gpio) {
		unsigned char val = 0x00;
		if (din)
			val = *din++;
		/* send 1st char to trigger transfer_done interrupt */
		SPIMDAT = val;
		if (len >= 2) {
			if (din)
				val = *din++;
			SPIMDAT = val;
		}
		spi_din = din;
		spi_dout = dout;
		spi_len = len;
		/* enable MSDONE/SSDONE/X16IRQ interrupt */
		IEN1 |= MISC_INT_EN;
		EA = 1;
		/* wait transfer all bytes completed */
		while (IEN1 & MISC_INT_EN)
			;
		return 0;
	}

	if (flags & SPI_XFER_BEGIN) {
		spi_cs_activate();
	}

	/* MOSI & MISO is shared as SDIO */
	if (spi_ss_pol & 0xf0) {
		/* support 2 bytes only: address + data */
		if (*din & 0x80) { //write
			/* write address: set 0 as output */
			SPIMCON1 &= ~MSPI_MOSI_OUT_EN;
			spi_master_swap_byte(*din++);
			/* write data */
			spi_master_swap_byte(*din);
		} else { //read
			/* write address: set 0 as output */
			SPIMCON1 &= ~MSPI_MOSI_OUT_EN;
			spi_master_swap_byte(*din);
			/* read data: set 1 as input */
			delay_us(4); // ADNS-5050 tSRAD
			SPIMCON1 |= MSPI_MOSI_OUT_EN;
			*dout = spi_master_swap_byte(0xff);
		}
		goto out;
	}

	for (i = 0; i < len; i++) {
		unsigned char val = 0x00;
		if (din)
			val = *din++;
		val = spi_master_swap_byte(val);
		if (dout)
			*dout++ = val;
		/* error occurs during transfer */
		if (SPITEST & (MSPI_TX_UNDERFLOW_FLAG | MSPI_TX_OVERFLOW_FLAG | MSPI_TX_UNDERFLOW_FLAG | MSPI_TX_OVERFLOW_FLAG)) {
			ret = -1;
			goto out;
		}
	}

out:
	if (flags & SPI_XFER_END) {
		spi_cs_deactivate();
	}

	return ret;
}

#ifdef HS6206_SPI_SLAVE
int spi_slave_setup(unsigned char mode)
{
	/* FIXME: flush FIFOs */
	RSTREAS |= RSTREAS_RST_SSPI;
	delay_us(1);

//	SPISCON &= ~(SSPI_MASK_CSN_HIGH_INT | SSPI_MASK_CSN_LOW_INT | SSPI_MASK_TRANS_DONE); 
	SPISCON = (mode & SPI_LSB_FIRST ? 0 : SSPI_DATA_MSB)
		| ((mode & 0x03) << 1) //SPI_MODE_0,1,2,3
		| SSPI_EN;

	/* enable Slave SPI comppleted interrupt */
	INTEXP |= SSDONE_INT_EN;
	INTEXP &= ~MSDONE_INT_EN;
	IEN1 |= MISC_INT_EN;
	EA = 1;
	return 0;
}
#endif
u8 spi_slave_status = 0;
void HS6206_SPI_ISR(void) interrupt HS6206_INT_MISC using 3
{
P0_5 = ~P0_5;
#ifdef HS6206_SPI_SLAVE
	/* SPI Slave interrupt handler */
	spi_slave_status = SPISSTAT;
//	printf("SPISSTAT=%x\n", (int)status);
#endif

}

#endif

/*-------------------------------------------End Of File----------------------*/
