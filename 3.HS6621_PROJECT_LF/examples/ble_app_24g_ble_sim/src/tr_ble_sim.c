/**
 * @file tr_sim_ble.c
 * @brief 
 * @date Fri Dec 23 10:22:23 2022
 * @author Onmicro Software
 *
 * @addtogroup 
 * @ingroup 
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include <stdio.h>
#include <string.h>
#include "peripheral.h"
#include "hw_transceiver.h"
#include "tr_ble_sim.h"

/*********************************************************************
 * MACROS
 */
#define __RBIT8(n)  (__RBIT(n) >> 24)

#if 1 // debug
#undef log_debug
#undef log_debug_array
#undef log_debug_array_ex
#define log_debug(format, ...)
#define log_debug_array(array, len)
#define log_debug_array_ex(note, array, len)
#endif

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */
/*config infor*/
static const unsigned char tr_ble_init_data[]=
{
    /*Register Addr             DataLenth   Data*/

    TR_BANK0_CONFIG,        1,      0x02,
    TR_BANK0_RX_PW_P0,      1,      TR_BLE_RX_FIXED_LENGTH + 2 + 3 + 1, // RX length
    TR_BANK0_DYNPD,         1,      0x00,
    TR_BANK0_FEATURE,       1,      0x00,
    TR_BANK0_SETUP_VALUE,   5,      0x40,0x3c,0xff,0x06,0x44,
    TR_BANK0_EN_AA,         1,      0x00,
    TR_BANK0_EN_RXADDR,     1,      0x3F,
    TR_BANK0_SETUP_AW,      1,      0x03,
    TR_BANK0_SETUP_RETR,    1,      0x03,
    TR_BANK0_CH,            1,      0x26,
    TR_BANK0_RF_SETUP,      1,      0x47,
    0xFF    /*Addr=0xFF,complete flag*/
};

static const unsigned char tr_ble_analog_data[]=
{
    TR_BANK1_PLL_CTL0,      4,  0x40,0x01,0x30,0xE1,
    TR_BANK1_PLL_CTL1,      4,  0x00,70,0x10,0x07,  // pll wait: 70us
    TR_BANK1_CAL_CTL,       5,  0x29,0xA9,0x67,0x28,0x28,  // A9: bp_rx_addr
    TR_BANK1_RX_CTRL,       4,  0x56,0xC2,0x09,0xAC,  // 56: sync_mode=1
    TR_BANK1_FAGC_CTRL_1,   4,  0x80,0x14,0x08,0x29,
    TR_BANK1_RF_IVGEN,      4,  0x1F,0x64,0x00,0x11,

    0xFF    /*Addr=0xFF,complete flag*/
};

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */

/**
 * @brief  ll ble data whitening
 *
 * Data whitening is from payload to crc, and not include PREABLE and AA
 * For example: the in_data of scan_req_header is {0x03,0x0C}
 *
 * @ref core_v4.1_P2523
 *
 * @param[in] in_data  in data
 * @param[in] len  len
 * @param[in] channel_index  channel index
 * @param[in] out_data  out data
 **/
static void ll_ble_data_whitening(const uint8_t *in_data, unsigned int len, unsigned char channel_index, uint8_t *out_data)
{
    unsigned i, j;
    uint8_t seed;

    // Do linear feedback shift register (LFSR)

    seed = 0x01;                           // Position0 = 1;
    seed |= (channel_index & (1<<5)) >> 4; // Position1;
    seed |= (channel_index & (1<<4)) >> 2; // Position2;
    seed |= (channel_index & (1<<3)) << 0; // Position3;
    seed |= (channel_index & (1<<2)) << 2; // Position4;
    seed |= (channel_index & (1<<1)) << 4; // Position5;
    seed |= (channel_index & (1<<0)) << 6; // Position6;

    for(i=0; i<len; ++i)
    {
        uint8_t out = 0;

        for(j=0; j<8; ++j)
        {
            uint8_t factor = (seed & (1<<6)) >> 6;
            uint8_t shift_out;

            out |= (in_data[i] ^ (factor << j)) & (1<<j);

            seed <<= 1;
            shift_out = (seed >> 7) & 0x01;
            seed = (seed & ~(1<<0)) | shift_out;
            seed = (seed & ~(1<<4)) | ((seed ^ (shift_out << 4)) & (1<<4));
        }

        out_data[i] = out;
    }
}

/**
 * @brief  ll ble data crc24 generate
 *
 * ADV: {0x00,0x0d,0x08,0xac,0xd9,0xc9,0x84,0xf0,0x02,0x01,0x02,0x03,0x09,0x68,0x79}
 * seed: 0x555555
 * crc24: 0x53f51b
 *
 * @param[in] seed  seed
 * @param[in] pdata  pdata
 * @param[in] len  len
 *
 * @return crc24
 **/
static uint32_t ll_ble_data_crc24_generate(uint32_t seed, uint8_t *pdata, unsigned len)
{
    unsigned i, j;

    for(i=0; i<len; ++i)
    {
        for(j=0; j<8; ++j)
        {
            uint8_t factor = ((pdata[i] >> j) ^ (seed >> 23)) & 0x01;

            seed <<= 1;
            seed = (seed & ~(1<<10)) | ((seed ^ (factor<<10)) & (1<<10));
            seed = (seed & ~(1<<9 )) | ((seed ^ (factor<<9 )) & (1<<9 ));
            seed = (seed & ~(1<<6 )) | ((seed ^ (factor<<6 )) & (1<<6 ));
            seed = (seed & ~(1<<4 )) | ((seed ^ (factor<<4 )) & (1<<4 ));
            seed = (seed & ~(1<<3 )) | ((seed ^ (factor<<3 )) & (1<<3 ));
            seed = (seed & ~(1<<1 )) | ((seed ^ (factor<<1 )) & (1<<1 ));
            seed = (seed & ~(1<<0 )) | factor;
        }
    }

    return __RBIT(seed) >> 8;
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 * @brief  ll ble aa generate
 *
 * @param[in] aa  aa
 **/
void tr_ble_aa_set(uint32_t aa)
{
    uint8_t addr[6];
    uint8_t *p_aa = (uint8_t *)&aa;

    // ADV Access Address:  0x8E89BED6   
    addr[1] = __RBIT8(p_aa[3]);
    addr[2] = __RBIT8(p_aa[2]);
    addr[3] = __RBIT8(p_aa[1]);
    addr[4] = __RBIT8(p_aa[0]);
    addr[5] = 0x55;

    tr_spi_wr_reg(TR_BANK0_RX_ADDR_P0, addr, 5);
    tr_spi_wr_reg(TR_BANK0_TX_ADDR, &addr[1], 5);

    log_debug_array_ex("Bitordered AA Raw Data", addr, 4);
}

/**
 * @brief  ll ble packet generate
 *
 * Input:
 *   crc_seed = 0x555555
 *   channel_index = 37
 *   header0 = 0x40
 *   payload[] = {0x0D, 0xBF, 0x66, 0x21, 0xD0, 0xDD,   0x02, 0x01, 0x06, 0x03, 0x09, 0x58, 0x58};
 *   length = sizeof(payload)
 *
 * Output:
 *   Raw Data             40 0D 0D BF 66 21 D0 DD 02 01 06 03 09 58 58 53 A9 54
 *   Whitened Raw Data    CD DF 5A 1E 5B 86 B6 6D 77 30 17 4B 9F 2F A0 B0 EF BD
 *   Bitordered Raw Data  B3 FB 5A 78 DA 61 6D B6 EE 0C E8 D2 F9 F4 05 0D F7 BD
 *
 * @param[in] crc_seed  crc seed
 * @param[in] channel_index  channel index
 * @param[in] header0  header0
 * @param[in] payload  payload
 * @param[in] length  length
 * @param[in] packet  packet
 *
 * @return length
 **/
uint32_t tr_ble_pdu_generate(uint32_t crc_seed, uint32_t channel_index,
        uint8_t header0, const uint8_t *payload, uint8_t length, uint8_t packet[32])
{
    uint32_t crc24;
    uint32_t i = 0;

    packet[i++] = header0;
    packet[i++] = length;
    memcpy(&packet[i], payload, length); i += length;

    crc24 = ll_ble_data_crc24_generate(crc_seed, packet, i);
    packet[i++] = (crc24 >> 0)  & 0xFF;
    packet[i++] = (crc24 >> 8)  & 0xFF;
    packet[i++] = (crc24 >> 16) & 0xFF;

    log_debug_array_ex("Raw Data", packet, i);

    ll_ble_data_whitening(packet, i, channel_index, packet);

    log_debug_array_ex("Whitened Raw Data", packet, i);

    for (int j=0; j<i; ++j)
        packet[j] = __RBIT8(packet[j]);

    log_debug_array_ex("Bitordered Raw Data", packet, i);

    return i;
}

/**
 * @brief  ll ble packet parse
 *
 * @param[in] crc_seed  crc seed
 * @param[in] channel_index  channel index
 * @param[in] packet  packet
 * @param[in] length  length
 *
 * @return  length, -1: error
 **/
int tr_ble_packet_parse(uint32_t crc_seed, uint32_t channel_index, uint8_t packet[32], uint32_t length)
{
    uint32_t crc24, crc24_calc;

    //log_debug_array_ex("RX Bitordered Raw Data", packet, length);

    for (int i=0; i<length; ++i)
        packet[i] = __RBIT8(packet[i]);

    //log_debug_array_ex("RX Whitened Raw Data", packet, length);

    ll_ble_data_whitening(packet, length, channel_index, packet);

    length = packet[1];

    if (length + 5 > 32)
    {
        log_debug("RX Length: Error (%d)\n", length + 5);
        return -1;
    }

    log_debug_array_ex("RX Raw Data", packet, length + 5);

    crc24_calc = ll_ble_data_crc24_generate(crc_seed, packet, length+2);
    crc24  = packet[length+2+0] << 0;
    crc24 |= packet[length+2+1] << 8;
    crc24 |= packet[length+2+2] << 16;

    log_debug("RX CRC: %s (0x%06X)\n", crc24_calc==crc24 ? "OK" : "Error", crc24_calc);

    return crc24_calc == crc24 ? (int)(length+2) : -1;
}

/**
 * @brief  init the 2.4g transerive when the event begin
 **/
void tr_ble_init(void)
{
    tr_phy_enable();

    pmu_ana_enable(true, PMU_ANA_RF);

    tr_ce_low();
    tr_configure_reg_bank1(tr_ble_analog_data);
    tr_configure_reg(tr_ble_init_data);
    tr_ce_low();
    tr_flush_tx_fifo();
    tr_clear_all_irq();

    tr_phy_disable();
}

/**
 * @brief  tr ble enable
 *
 * @param[in] enable  enable
 **/
void tr_ble_enable(bool enable)
{
    if (enable)
    {
        tr_phy_enable();

        // For check sync detect
        cpm_ana_clock_enable();
        HS_DAIF->DBG_REG = 0x1B;
    }
    else
    {
        HS_DAIF->DBG_REG = 0x0;
        tr_phy_disable();

        // Force abort BLE
        *(__IO uint32_t *)0x413000D0 |= (1u<<15);
        *(__IO uint32_t *)0x41300000 |= (1u<<26);
        co_delay_us(1);
        *(__IO uint32_t *)0x413000D0 &= ~(1u<<15);
        *(__IO uint32_t *)0x41300000 &= ~(1u<<26);
    }
}

/** @} */

