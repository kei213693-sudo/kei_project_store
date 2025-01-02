/**
 * @file hw_transceiver.h
 * @brief
 * @date Thu 14 Jan 2016 10:11:13 AM CST
 * @author shangzhou.hu@huntersun.com
 *
 * @addtogroup
 * @ingroup
 * @details
 *
 * @{
 */

#ifndef __HW_TRANSCEIVER_H__
#define __HW_TRANSCEIVER_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "features.h"
#include "hs66xx.h"


/*********************************************************************
 * MACROS
 */

#define TR_STATUS_RX_DR             0x40
#define TR_STATUS_TX_DS             0x20
#define TR_STATUS_MAX_RT            0x10
#define TR_STATUS_TX_FULL           0x01

#define TR_FIFO_MAX_PACK_SIZE       0x20

/*register bit Mask define*/
/*R 0x17:*/
#define TR_FIFO_STA_TX_REUSE        0x40
#define TR_FIFO_STA_TX_FULL         0x20
#define TR_FIFO_STA_TX_EMPTY        0x10
#define TR_FIFO_STA_RX_FULL         0x02
#define TR_FIFO_STA_RX_EMPTY        0x01

//-------------RF command---------------------
#define TR_R_REGISTER               0x00 // Define read command to register
#define TR_W_REGISTER               0x20 // Define write command to register
#define TR_R_RX_PAYLOAD             0x61 // Define RX payload register address
#define TR_W_TX_PAYLOAD             0xA0 // Define TX payload register address
#define TR_FLUSH_TX                 0xE1 // Define flush TX register command
#define TR_FLUSH_RX                 0xE2 // Define flush RX register command
#define TR_REUSE_TX_PL              0xE3 // Define reuse TX payload register command
#define TR_ACTIVATE                 0x50 // Define ACTIVATE features register command
#define TR_R_RX_PL_WID              0x60 // Define read RX payload width register command
#define TR_W_ACK_PAYLOAD            0xA8 // Define write ACK payload register command
#define TR_W_TX_PAYLOAD_NOACK       0xB0 // Define disable TX ACK for one time register command
#define TR_NOP                      0xFF // Define No Operation, might be used to read status register

#define TR_ACTIVATE_DATA            0x53

// Bank0 Register Addr
#define TR_BANK0_CONFIG             0x00 // 'Config' register address
#define TR_BANK0_EN_AA              0x01 // 'Enable Auto Acknowledgment' register address
#define TR_BANK0_EN_RXADDR          0x02 // 'Enabled RX addresses' register address
#define TR_BANK0_SETUP_AW           0x03 // 'Setup address width' register address
#define TR_BANK0_SETUP_RETR         0x04 // 'Setup Auto. Retrans' register address
#define TR_BANK0_CH                 0x05 // 'RF channel' register address
#define TR_BANK0_RF_SETUP           0x06 // 'RF setup' register address
#define TR_BANK0_STATUS             0x07 // 'Status' register address
#define TR_BANK0_OBSERVE_TX         0x08 // 'Observe TX' register address
#define TR_BANK0_RPD                0x09 // 'Received Power Detector' register address
#define TR_BANK0_RX_ADDR_P0         0x0A // 'RX address pipe0' register address
#define TR_BANK0_RX_ADDR_P1         0x0B // 'RX address pipe1' register address
#define TR_BANK0_RX_ADDR_P2         0x0C // 'RX address pipe2' register address
#define TR_BANK0_RX_ADDR_P3         0x0D // 'RX address pipe3' register address
#define TR_BANK0_RX_ADDR_P4         0x0E // 'RX address pipe4' register address
#define TR_BANK0_RX_ADDR_P5         0x0F // 'RX address pipe5' register address
#define TR_BANK0_TX_ADDR            0x10 // 'TX address' register address
#define TR_BANK0_RX_PW_P0           0x11 // 'RX payload width, pipe0' register address
#define TR_BANK0_RX_PW_P1           0x12 // 'RX payload width, pipe1' register address
#define TR_BANK0_RX_PW_P2           0x13 // 'RX payload width, pipe2' register address
#define TR_BANK0_RX_PW_P3           0x14 // 'RX payload width, pipe3' register address
#define TR_BANK0_RX_PW_P4           0x15 // 'RX payload width, pipe4' register address
#define TR_BANK0_RX_PW_P5           0x16 // 'RX payload width, pipe5' register address
#define TR_BANK0_FIFO_STATUS        0x17 // 'FIFO Status Register' register address
#define TR_BANK0_DYNPD              0x1C // 'Enable dynamic payload length' register address
#define TR_BANK0_FEATURE            0x1D // 'Feature' register address
#define TR_BANK0_SETUP_VALUE        0x1E
#define TR_BANK0_PRE_GURD           0x1F
// Bank1 register
#define TR_BANK1_LINE               0x00
#define TR_BANK1_PLL_CTL0           0x01
#define TR_BANK1_PLL_CTL1           0x02
#define TR_BANK1_CAL_CTL            0x03
#define TR_BANK1_A_CNT_REG          0x04
#define TR_BANK1_B_CNT_REG          0x05
#define TR_BANK1_RESERVED1          0x06
#define TR_BANK1_STATUS             0x07
#define TR_BANK1_STATE              0x08
#define TR_BANK1_CHAN               0x09
#define TR_BANK1_IF_FREQ            0x0A
#define TR_BANK1_AFC_COR            0x0B
#define TR_BANK1_FDEV               0x0C
#define TR_BANK1_DAC_RANGE          0x0D
#define TR_BANK1_DAC_IN             0x0E
#define TR_BANK1_CTUNING            0x0F
#define TR_BANK1_FTUNING            0x10
#define TR_BANK1_RX_CTRL            0x11
#define TR_BANK1_FAGC_CTRL          0x12
#define TR_BANK1_FAGC_CTRL_1        0x13
#define TR_BANK1_DAC_CAL_LOW        0x17
#define TR_BANK1_DAC_CAL_HI         0x18
#define TR_BANK1_RESERVED3          0x19
#define TR_BANK1_DOC_DACI           0x1A
#define TR_BANK1_DOC_DACQ           0x1B
#define TR_BANK1_AGC_CTRL           0x1C
#define TR_BANK1_AGC_GAIN           0x1D
#define TR_BANK1_RF_IVGEN           0x1E
#define TR_BANK1_TEST_PKDET         0x1F

/* configure register defines */
#define TR_MAC_SELECT           (1UL << 7)
#define TR_IRQ_RXFIFO_FULL      (1UL << 3)
#define TR_IRQ_RXDATA_READY     (1UL << 2)
#define TR_IRQ_TxFIFO_EMPTY     (1UL << 1)
#define TR_IRQ_TXFIFO_READY     (1UL << 0)

/* status register defines */
#define TR_SPI_BUSY             (1UL << 4)
#define TR_RX_FULL              (1UL << 3)
#define TR_RXDATA_READY         (1UL << 2)
#define TR_TXFIFO_EMPTY         (1UL << 1)
#define TR_TXFIFO_READY         (1UL << 0)

/* RF Configure register defines */
#define TR_CSN                  (1UL << 1)
#define TR_CE                   (1UL << 0)

/* RF Clear Interrupt register defines */
#define TR_ICR                  (1UL << 0)

//-------------RF command---------------------
#define TR_R_REGISTER                 0x00 // Define read command to register
#define TR_W_REGISTER                 0x20 // Define write command to register
#define TR_R_RX_PAYLOAD               0x61 // Define RX payload register address
#define TR_W_TX_PAYLOAD               0xA0 // Define TX payload register address
#define TR_FLUSH_TX                   0xE1 // Define flush TX register command
#define TR_FLUSH_RX                   0xE2 // Define flush RX register command
#define TR_REUSE_TX_PL                0xE3 // Define reuse TX payload register command
#define TR_ACTIVATE                   0x50 // Define ACTIVATE features register command
#define TR_R_RX_PL_WID                0x60 // Define read RX payload width register command
#define TR_W_ACK_PAYLOAD              0xA8 // Define write ACK payload register command
#define TR_W_TX_PAYLOAD_NOACK         0xB0 // Define disable TX ACK for one time register command
#define TR_NOP                        0xFF // Define No Operation, might be used to read status register

#define TR_ACTIVATE_DATA              0x53

//----------------------defines--------------------------
/*
 * ¿?HS6600¿?¿?¿?¿?¿?¿?¿?¿?¿?BANK1¿?¿?¿?¿?¿?¿?¿?¿?1¿?¿?BANK0¿?¿?¿?¿?¿?¿?
 *
 */
#define TR_BANK        0x80      /*bank ¿?¿?¿?*/
#define TR_BANK1       0x80
#define TR_BANK0       0x00

//data rate
#define TR_BANK0_DR         (0x20+0x08)
#define TR_BANK0_DR_250K     0x20
#define TR_BANK0_DR_500K    (0x20+0x08)
#define TR_BANK0_DR_1M       0x00
#define TR_BANK0_DR_2M       0x08

//TR_BANK0_SETUP_AW
#define TR_AW_BITS     (0x01+0x02)
#define TR_AW_4_BYTES  0x02
#define TR_AW_5_BYTES  (0x01+0x02)

/*------------------------------5.PA power-----------------------------------*/
#define TR_BANK0_PA_POWER              (0x04+0x02+0x01)
#define TR_BANK0_PA_POWER_n18dBm        0x00
#define TR_BANK0_PA_POWER_n12dBm        0x02
#define TR_BANK0_PA_POWER_n6dBm         0x04
#define TR_BANK0_PA_POWER_0dBm          (0x04+0x02)
#define TR_BANK0_PA_POWER_5dBm          0x01

//Cont wave
#define TR_BANK0_CONT_WAVE                0x80
#define TR_BANK0_PRX                      0x01

//TR_BANK0_STATUS
#define TR_RX_DATA_READY    0x40
#define TR_TX_DATA_SENT     0x20
#define TR_TX_MAX_RT        0x10
#define TR_TX_FIFO_FULL     0x01
/*********************************************************************
 * TYPEDEFS
 */

typedef enum {
    TR_PRX_MODE,
    TR_PTX_MODE,
    TR_CARRIER_MODE
}
tr_mode_t;

typedef enum {
    TR_Bank0 = 0,
    TR_Bank1
} TR_Bank_TypeDef;

/*********************************************************************
 * EXTERN FUNCTIONS
 */
/**
 *******************************************************************************
 * @brief enable 2.4g rcc clk and phy clk.
 *
 * @return None
 *******************************************************************************
 */
void tr_phy_enable(void);

/**
 *******************************************************************************
 * @brief disable 2.4g rcc clk and phy clk.
 *
 * @return None
 *******************************************************************************
 */
void tr_phy_disable(void);

/**
 *******************************************************************************
 * @brief CE HIGH.
 *
 * @return None
 *******************************************************************************
 */
void tr_ce_high(void);

/**
 *******************************************************************************
 * @brief CE LOW.
 *
 * @return None
 *******************************************************************************
 */
void tr_ce_low(void);

/**
 *******************************************************************************
 * @brief CE pulse must be more then 16us.
 *
 * @return None
 *******************************************************************************
 */
void tr_ce_high_pulse(void);

/**
 *******************************************************************************
 * @brief Write One Register.
 *
 * @param[in] addr       2.4G register address
 * @param[in] data       Register value(R/W)
 *
 * @return None
 *******************************************************************************
 */
void tr_write_byte(unsigned char addr, unsigned char data);

/**
 *******************************************************************************
 * @brief read One Register.
 *
 * @param[in] addr       2.4G register address
 *
 * @return None
 *******************************************************************************
 */
unsigned char tr_read_byte(unsigned char addr);

/**
 *******************************************************************************
 * @brief Write value to 2.4G register through SPI.
 *
 * @param[in] reg         2.4G register address
 * @param[in] *reg_val    Register value(R/W)
 * @param[in] reg_width   Number of bytes of register value
 *
 * @return None
 *******************************************************************************
 */
void tr_spi_wr_reg(uint8_t reg, const uint8_t *reg_val, uint8_t reg_width);

/**
 *******************************************************************************
 * @brief Read value from 2.4G register through SPI.
 *
 * @param[in] reg         2.4G register address
 * @param[in] *reg_val    Register value(R/W)
 * @param[in] reg_width   Number of bytes of register value
 *
 * @return None
 *******************************************************************************
 */
void tr_spi_rd_reg(uint8_t reg, uint8_t *reg_val, uint8_t reg_width);

/**
 *******************************************************************************
 * @brief Read data from receive FIFO.
 *
 * @param[in] *pbuf       Stores the data read from the receive FIFO
 * @param[in]  bytes      Number of bytes to read
 *
 * @return None
 *******************************************************************************
 */
void tr_read_rx_payload(uint8_t *pbuf, uint8_t bytes);

/**
 *******************************************************************************
 * @brief Write data to transmit FIFO.
 *
 * @param[in] *pbuf       Data to be written to transmit FIFO
 * @param[in]  bytes      Number of bytes to write
 *
 * @return None
 *******************************************************************************
 */
void tr_write_tx_payload(uint8_t *pbuf, uint8_t bytes);

/**
 *******************************************************************************
 * @brief Clear transmit FIFO.
 *
 * @return None
 *******************************************************************************
 */
void tr_flush_tx_fifo(void);

/**
 *******************************************************************************
 * @brief Clear receive FIFO.
 *
 * @return None
 *******************************************************************************
 */
void tr_flush_rx_fifo(void);

/**
 *******************************************************************************
 * @brief Switch between bank0 and bank1.
 *
 * @param[in] bank     2.4G register bank address range
 *
 * @return None
 *******************************************************************************
 */
void tr_bank_switch(unsigned char bank);

/**
 *******************************************************************************
 * @brief Number of bytes received in receive FIFO.
 *
 * @return None
 *******************************************************************************
 */
uint8_t tr_read_rx_payload_width(void);

/**
 *******************************************************************************
 * @brief Write ack data to transmit FIFO.
 *
 * @param[in]  pipenum    number of pipe
 * @param[in] *pbuf       Data to be written to transmit FIFO
 * @param[in]  bytes      Number of bytes to write
 *
 * @return None
 *******************************************************************************
 */
void tr_write_ack_payload(uint8_t pipenum, uint8_t *pbuf, uint8_t bytes);

/**
 *******************************************************************************
 * @brief Write data to transmit FIFO in non reply mode.
 *
 * @param[in] *pbuf       Data to be written to transmit FIFO
 * @param[in]  bytes      Number of bytes to write
 *
 * @return None
 *******************************************************************************
 */
void tr_write_tx_payload_no_ack(uint8_t *pbuf, uint8_t bytes);

/**
 *******************************************************************************
 * @brief Setup address width.
 *
 * @param[in] addr_width    address width.
 *
 * @return None
 *******************************************************************************
 */
void tr_set_addr_width(uint8_t addr_width);

/**
 *******************************************************************************
 * @brief Write transmission channel address.
 *
 * @param[in]  px_addr_reg     pipe rigster
 * @param[in] *pipeaddr        pipe address
 * @param[in]  addr_width      address width
 *
 * @return None
 *******************************************************************************
 */
void tr_write_pipe_addr(uint8_t px_addr_reg, uint8_t *pipeaddr, uint8_t addr_width);

/**
 *******************************************************************************
 * @brief read transmission channel address.
 *
 * @param[in]  px_addr_reg     pipe rigster
 * @param[in] *pipeaddr        pipe address
 * @param[in]  addr_width      address width
 *
 * @return None
 *******************************************************************************
 */
void tr_read_pipe_addr(uint8_t px_addr_reg, uint8_t *pipeaddr, uint8_t addr_width);

/**
 *******************************************************************************
 * @brief set auto ack.
 *
 * @param[in] *pipe        pipe nummber
 *
 * @return None
 *******************************************************************************
 */
uint8_t tr_is_auto_ack_px(uint8_t pipe);

/**
 *******************************************************************************
 * @brief  Whether W_TX_PAYLOAD_NOACK cmd is enabled.
 *
 * @return None
 *******************************************************************************
 */
uint8_t tr_is_dyn_ack_feature(void);

/**
 *******************************************************************************
 * @brief  Determine whether the dynamic length.
 *
 *@return None
 *******************************************************************************
 */
uint8_t tr_is_dpl_feature(void);

/**
 *******************************************************************************
 * @brief Read the length of received static data.
 *
 * @param[in]  pipe nummber
 *
 * @return None
 *******************************************************************************
 */
uint8_t tr_read_rx_pipe_static_payload_width(uint8_t pipe_num);

/**
 *******************************************************************************
 * @brief Read status.
 *
 * @return None
 *******************************************************************************
 */
uint8_t tr_get_rf_status(void);

/**
 *******************************************************************************
 * @brief 2.4G init.
 *
 * @return None
 *******************************************************************************
 */
void tr_init(void);

/**
 *******************************************************************************
 * @brief printf 2.4G register.
 *
 * @return None
 *******************************************************************************
 */
void tr_dump_rf_register(void);

/**
 *******************************************************************************
 * @brief Read status register.
 *
 * @return None
 *******************************************************************************
 */
void tr_clear_all_irq(void);

/**
 *******************************************************************************
 * @brief Read status register.
 *
 * @return None
 *******************************************************************************
 */
unsigned char tr_read_status(void);

/**
 *******************************************************************************
 * @brief write status regsiter.
 *
 * @return None
 *******************************************************************************
 */
void tr_write_status(unsigned char status);

/**
 *******************************************************************************
 * @brief PTX/PRX switch.
 *
 * @param[in]  mod   2.4G transmit and receive mode
 *
 * @return None
 *******************************************************************************
 */
void tr_mode_switch(tr_mode_t mod);

/**
 *******************************************************************************
 * @brief Write data to transmit FIFO and pull CE pulse.
 *
 * @param[in] *pbuf       Data to be written to transmit FIFO
 * @param[in]  bytes      Number of bytes to write
 *
 * @return None
 *******************************************************************************
 */
void tr_write(unsigned char *buf, unsigned char len);

/**
 *******************************************************************************
 * @brief Write data to transmit FIFO and pull CE pulse in no_ack mode.
 *
 * @param[in] *pbuf       Data to be written to transmit FIFO
 * @param[in]  bytes      Number of bytes to write
 *
 * @return None
 *******************************************************************************
 */
void tr_write_no_ack(unsigned char *buf, unsigned char len);

/**
 *******************************************************************************
 * @brief Read data from receive FIFO.
 *
 * @param[in] *pbuf       Stores the data read from the receive FIFO
 *
 * @return None
 *******************************************************************************
 */
unsigned char tr_read(unsigned char *buf);

/**
 *******************************************************************************
 * @brief change pipe address.
 *
 * @param[in] *addrbuf       An array that stores new addresses
 * @param[in]  len           length of address
 *
 * @return None
 *******************************************************************************
 */
void tr_change_address(unsigned char *addrbuf, unsigned char len);

/**
 *******************************************************************************
 * @brief set 6621x frequency point and can not be used by 6620.
 *
 * @param[in]  freq       Frequency point size
 *
 * @return None
 *******************************************************************************
 */
void tr_set_freq(unsigned int freq);

/**
 *******************************************************************************
 * @brief change freq.
 *
 * @param[in]  ch_index           Frequency point size
 *
 * @return None
 *******************************************************************************
 */
void tr_change_ch(unsigned char ch_index);

/**
 *******************************************************************************
 * @brief configure bank0 register.
 *
 * @param[in]  *dbuf        Configuration table
 *
 * @return None
 *******************************************************************************
 */
void tr_configure_reg(const unsigned char *dbuf);

/**
 *******************************************************************************
 * @brief configure bank1 register.
 *
 * @param[in]  *dbuf        Configuration table
 *
 * @return None
 *******************************************************************************
 */
void tr_configure_reg_bank1(const unsigned char *dbuf);

/**
 *******************************************************************************
 * @brief read 2.4G rssi value.
 
 * @return rssi value
 *******************************************************************************
 */
int8_t tr_read_rssi(void);

/**
 *******************************************************************************
 * @brief add one clock.
 
 * @return NULL
 *******************************************************************************
 */
uint8_t tr_nop(void);

#ifdef __cplusplus
}
#endif

#endif

/** @} */

