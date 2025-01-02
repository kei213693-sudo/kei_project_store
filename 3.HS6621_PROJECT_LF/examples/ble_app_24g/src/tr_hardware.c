//#include "llm_transceiver.h"
#include "hw_transceiver.h"
#include "tr_hardware.h"
#include "peripheral.h"

/*config infor*/
const unsigned char tr_init_data[]=
{
    /*Register Addr             DataLenth   Data*/

    TR_BANK0_CONFIG,        1,      0x8e,
    TR_BANK0_RX_PW_P0,      1,      0x20,
    TR_BANK0_DYNPD,         1,      0x3f,
    TR_BANK0_FEATURE,       1,      0x07,
    TR_BANK0_SETUP_VALUE,   5,      0x40,0x3c,0xff,0x06,0x44,
    TR_BANK0_EN_AA,         1,      0x3f,
    TR_BANK0_EN_RXADDR,     1,      0x3F,
    TR_BANK0_SETUP_AW,      1,      0x03,
    TR_BANK0_SETUP_RETR,    1,      0x03,
    TR_BANK0_CH,            1,      0x26,
    TR_BANK0_RF_SETUP,      1,      0x47,
    TR_BANK0_RX_ADDR_P0,    5,      0x02,0x56,0x76,0xD4,0xED,
    TR_BANK0_TX_ADDR,       5,      0x02,0x56,0x76,0xD4,0xED,
//    TR_BANK0_RX_ADDR_P0,    5,      0x46, 0x0b, 0xAF, 0x43, 0x98,
//    TR_BANK0_TX_ADDR,       5,      0x46, 0x0b, 0xAF, 0x43, 0x98,
//    TR_BANK0_RX_ADDR_P0,    5,      0x36, 0x2b, 0xAD, 0x58, 0x96,
//    TR_BANK0_TX_ADDR,       5,      0x36, 0x2b, 0xAD, 0x58, 0x96,
    0xFF    /*Addr=0xFF,complete flag*/
};


const unsigned char tr_analog_data[]=
{
    TR_BANK1_PLL_CTL0,      4,  0x40,0x01,0x30,0xE1,
//    TR_BANK1_PLL_CTL1,      4,  0x00,0x8a,0x10,0x07,
    TR_BANK1_PLL_CTL1,      4,  0x00,0x7a,0x10,0x07,
    TR_BANK1_CAL_CTL,       5,  0x29,0x89,0x77,0x28,0x28,
    TR_BANK1_RX_CTRL,       4,  0x52,0xC2,0x09,0xAC,
    TR_BANK1_FAGC_CTRL_1,   4,  0x80,0x14,0x08,0x29,
    TR_BANK1_RF_IVGEN,      4,  0x1F,0x64,0x00,0x11,

    0xFF    /*Addr=0xFF,complete flag*/
};

/**
 * @brief  init the 2.4g transerive when the event begin
 *
 * @param[in] null
 *
 * @return null
 **/
void tr_hardware_init(void)
{
    tr_phy_enable();
    tr_ce_low();
    tr_configure_reg_bank1(tr_analog_data);
    tr_configure_reg(tr_init_data);
    NVIC_DisableIRQ(MAC6200_RF_IRQn);
}

