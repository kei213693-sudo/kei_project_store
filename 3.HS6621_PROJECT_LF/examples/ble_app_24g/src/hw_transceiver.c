/* ----------------------------------------------------------------------
 * Copyright (C) 2015 HunterSun Limited. All rights reserved.
 *
 * $Date:        28. July 2015
 * $Revision:    V1.0
 * $Author:      shangzhou.hu@huntersun.com
 * Project:      Radio Driver for HS6600
 * -------------------------------------------------------------------- */

#include "hw_transceiver.h"
#include "co_delay.h"
#include "cpm.h"
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition

#ifdef CONFIG_TRANSCEIVER

void tr_phy_enable(void)
{
    calib_rf_restore();
    // enable 2.4G clock 
    REGW0(&HS_PSO->HS6200_CFG, CPM_HS6200_GATE_EN_MASK);
    HS_PSO_XTAL32M_UPD_RDY();

    //HS_BTPHY->SPI_APB_SWITCH=0;
    HS_MAC6200->SPIRCON |= TR_MAC_SELECT;

    // enable phy clock
    REGW0(&HS_PSO->BTPHY_CFG, CPM_PHY_AHB_GATE_EN_MASK | CPM_PHY_CLK_GATE_EN_MASK);
    HS_PSO_XTAL32M_UPD_RDY();
  
    //Soft demodulation,need enable phy clock firstly.
    HS_PHY->EN_SDET = 0;

    // enable 16M clock 
    pmu_ana_enable(true, PMU_ANA_RF);
}

void tr_phy_disable(void)
{
    // HS_BTPHY->SPI_APB_SWITCH=1;
    HS_MAC6200->SPIRCON &= ~TR_MAC_SELECT;
  
    // disable 2.4G clock 
    REGW1(&HS_PSO->HS6200_CFG, CPM_HS6200_SOFT_RESET_MASK);
    REGW1(&HS_PSO->HS6200_CFG, CPM_HS6200_GATE_EN_MASK);
    HS_PSO->REG_UPD = 1;
  
    // disable phy clock  
    REGW1(&HS_PSO->BTPHY_CFG, CPM_PHY_AHB_GATE_EN_MASK | CPM_PHY_CLK_GATE_EN_MASK);
    HS_PSO_XTAL32M_UPD_RDY();
  
    // disable 16M clock
    pmu_ana_enable(false, PMU_ANA_RF);
}

void tr_csn_high(void)
{
    HS_MAC6200->RFCON |= TR_CSN;
}

void tr_csn_low(void)
{
    HS_MAC6200->RFCON &= ~TR_CSN;
}

void tr_ce_high(void)
{
    HS_MAC6200->RFCON |= TR_CE;
}

void tr_ce_low(void)
{
    HS_MAC6200->RFCON &= ~TR_CE;
}

void tr_ce_high_pulse(void)
{
    HS_MAC6200->RFCON |= TR_CE;
    // Must be more then 16us
    co_delay_10us(10);
    HS_MAC6200->RFCON &= ~TR_CE;
}

/*RFSPI write/read 1 Byte to/from RF */
uint8_t tr_spi_wr_byte_poll(uint8_t byte)
{
    while (!(HS_MAC6200->SPIRSTAT & TR_TXFIFO_READY));     //Tx Fifo not ready.
    HS_MAC6200->SPIRDAT = byte;
    while (!(HS_MAC6200->SPIRSTAT & TR_RXDATA_READY));     //Rx Fifo has data.
    return HS_MAC6200->SPIRDAT;
}

void tr_spi_wr_reg(uint8_t reg, const uint8_t *reg_val, uint8_t reg_width)
{
    uint8_t i;
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_W_REGISTER + reg);
    for (i = 0x00; i < reg_width; i++) {
        tr_spi_wr_byte_poll(*reg_val++);
    }
    tr_csn_high();  //拉高
}

void tr_spi_rd_reg(uint8_t reg, uint8_t *reg_val, uint8_t reg_width)
{
    uint8_t i;
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_R_REGISTER + reg);
    for (i = 0x00; i < reg_width; i++) {
        reg_val[i] = tr_spi_wr_byte_poll(0x00);
    }
    tr_csn_high() ; //拉高
}

/*Write One Register*/
void tr_write_byte(unsigned char addr, unsigned char data)
{
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_W_REGISTER | addr);
    tr_spi_wr_byte_poll(data);
    tr_csn_high() ; //拉高
}

/*Read One Register */
unsigned char tr_read_byte(unsigned char addr)
{
    unsigned char rxdata;

    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_R_REGISTER | addr);
    rxdata = tr_spi_wr_byte_poll(0);
    tr_csn_high() ; //拉高

    return (rxdata);


}
/*write Commad  function, cmd = code; D = data*/
void tr_wr_cmd(unsigned char cmd, unsigned char data)
{
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(cmd);
    tr_spi_wr_byte_poll(data);
    tr_csn_high() ; //拉高
}

void tr_bank_switch(unsigned char bank)
{
    volatile  unsigned char sta;

    sta = tr_read_byte(TR_BANK0_STATUS);
    if (bank != TR_Bank0) {
        if ((sta & TR_BANK) != TR_BANK1) {
            tr_wr_cmd(TR_ACTIVATE, TR_ACTIVATE_DATA);
        }
    } else {
        if ((sta & TR_BANK) != TR_BANK0) {
            tr_wr_cmd(TR_ACTIVATE, TR_ACTIVATE_DATA);
        }
    }
}

void tr_read_rx_payload(uint8_t *pbuf, uint8_t bytes)
{
    uint8_t i;
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_R_RX_PAYLOAD);
    for (i = 0x00; i < bytes; i++) {
        pbuf[i] = tr_spi_wr_byte_poll(0x00);
    }
    tr_csn_high();
}

uint8_t tr_nop(void)
{
    uint8_t status;
    tr_csn_low();
    status = tr_spi_wr_byte_poll(TR_NOP);
    tr_csn_high();
    return (status);
}

void tr_write_tx_payload(uint8_t *pbuf, uint8_t bytes)
{
    uint8_t i;
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_W_TX_PAYLOAD);
    for (i = 0x00; i < bytes; i++) {
        tr_spi_wr_byte_poll(pbuf[i]);
    }
    tr_csn_high();
    tr_nop();
}

void tr_flush_tx_fifo(void)
{
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_FLUSH_TX);    // send command
    tr_csn_high();                   // CSN high again
}

void tr_flush_rx_fifo(void)
{
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_FLUSH_RX);    // send command
    tr_csn_high();                   // CSN high again
}

uint8_t tr_read_rx_payload_width(void)
{
    uint8_t Width;
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_R_RX_PL_WID);
    Width = tr_spi_wr_byte_poll(0x00);
    tr_csn_high();
    return Width;
}

void tr_write_ack_payload(uint8_t pipenum, uint8_t *pbuf, uint8_t bytes)
{
    uint8_t byte_ctr;
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_W_ACK_PAYLOAD | pipenum);
    for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++) { // then write all byte in buffer(*pBuf)
        tr_spi_wr_byte_poll(*pbuf++);
    }
    tr_csn_high();
    tr_nop();
}

void tr_write_tx_payload_no_ack(uint8_t *pbuf, uint8_t bytes)
{
    uint8_t byte_ctr;
    tr_csn_low();   //拉低
    tr_spi_wr_byte_poll(TR_W_TX_PAYLOAD_NOACK);
    for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++) { // then write all byte in buffer(*pBuf)
        tr_spi_wr_byte_poll(*pbuf++);
    }
    tr_csn_high();
    tr_nop();
}

void tr_set_addr_width(uint8_t addr_width)
{
    uint8_t temp;
    switch (addr_width) {
        case 0x04:
            temp = TR_AW_4_BYTES;
            break;

        case 0x05:
            temp = TR_AW_5_BYTES;
            break;

        default:
            temp = 0x00;
            break;
    }
    tr_spi_wr_reg(TR_BANK0_SETUP_AW, &temp, 1);
}

void tr_write_pipe_addr(uint8_t px_addr_reg, uint8_t *pipeaddr, uint8_t addr_width)
{
    uint8_t byte_ctr;

    tr_csn_low();                                     // CSN low, init SPI transaction
    tr_spi_wr_byte_poll(TR_W_REGISTER + px_addr_reg);    // Select register to write to and read status byte
    for (byte_ctr = 0; byte_ctr < addr_width; byte_ctr++) { // then write all byte in buffer(*pBuf)
        tr_spi_wr_byte_poll(*pipeaddr++);
    }
    tr_csn_high();                                     // CSN high again                                                            // return HS6600 status byte
}

void tr_read_pipe_addr(uint8_t px_addr_reg, uint8_t *pipeaddr, uint8_t addr_width)
{
    uint8_t byte_ctr;

    tr_csn_low();                                     // CSN low, init SPI transaction
    tr_spi_wr_byte_poll(TR_R_REGISTER + px_addr_reg);    // Select register to write to and read status byte
    for (byte_ctr = 0; byte_ctr < addr_width; byte_ctr++) {                   // then write all byte in buffer(*pBuf)
        *pipeaddr++ = tr_spi_wr_byte_poll(0x00);
    }
    tr_csn_high();                                     // CSN high again                                                            // return HS6600 status byte
}

uint8_t tr_is_auto_ack_px(uint8_t pipe)
{
    uint8_t temp;
    temp = 0x00;
    tr_spi_rd_reg(TR_BANK0_EN_AA, &temp, 1);
    temp = temp & (1 << pipe);
    return temp;
}

uint8_t tr_is_dyn_ack_feature(void)
{
    uint8_t temp;
    tr_spi_rd_reg(TR_BANK0_FEATURE, &temp, 1);
    return  (temp & 0x01);
}

uint8_t tr_is_dpl_feature(void)   //判断是否是动长
{
    uint8_t temp;
    tr_spi_rd_reg(TR_BANK0_FEATURE, &temp, 1);
    return  (temp & 0x04);
}

uint8_t tr_read_rx_pipe_static_payload_width(uint8_t pipe_num)
{
    uint8_t temp;
    tr_spi_rd_reg(TR_BANK0_RX_PW_P0 + pipe_num, &temp, 1);
    return temp;
}

void tr_switch_role(unsigned char role)
{
    tr_ce_low();
    tr_mode_switch((tr_mode_t)role);
    tr_flush_tx_fifo();
    tr_flush_rx_fifo();
    tr_ce_high();
}


uint8_t tr_get_rf_status(void)
{
    uint8_t status;
    tr_spi_rd_reg(TR_BANK0_STATUS, &status, 1);
    return status;
}

void tr_calibration(void)
{
    uint8_t temp[5];
    tr_bank_switch(TR_Bank0);
    temp[0] = 0x03;
    tr_spi_wr_reg(TR_BANK0_CONFIG, temp, 1);

    temp[0] = 0x32;
    tr_spi_wr_reg(TR_BANK0_CH, temp, 1);

    temp[0] = 0x40;     //1MHz速率,发射功率-18dBm.
    tr_spi_wr_reg(TR_BANK0_RF_SETUP, temp, 1);

    tr_bank_switch(TR_Bank1);
    temp[0] = 0x40;
    temp[1] = 0x00;
    temp[2] = 0x10;
    temp[3] = 0xE4;
    tr_spi_wr_reg(TR_BANK1_PLL_CTL0, temp, 4);

    temp[0] = 0x00;
    temp[1] = 0x00;
    temp[2] = 0x1F;
    tr_spi_wr_reg(TR_BANK1_IF_FREQ, temp, 3);

    temp[0] = 0x14;
    tr_spi_wr_reg(TR_BANK1_FDEV, temp, 1);

    tr_bank_switch(TR_Bank0);
    //  tr_delay_ms(2);
    tr_ce_high_pulse();
    //  tr_delay_ms(40);   //delay 150ms waitting for calibaration.
}

void tr_analog_init(void)      //HS6600 初始化
{
    uint8_t  temp[5];

    tr_ce_low();
    tr_bank_switch(TR_Bank0);

    temp[0] = 0x28;
    temp[1] = 0x32;
    temp[2] = 0x80;
    temp[3] = 0x06;
    temp[4] = 0x00;
    tr_spi_wr_reg(TR_BANK0_SETUP_VALUE, temp, 5);

    tr_bank_switch(TR_Bank1);

    temp[0] = 0x40;
    temp[1] = 0x01;
    temp[2] = 0x30;
    temp[3] = 0xE0;
    tr_spi_wr_reg(TR_BANK1_PLL_CTL0, temp, 4);

    temp[0] = 0x00;
    temp[1] = 0x42;
    temp[2] = 0x10;
    temp[3] = 0x00;
    tr_spi_wr_reg(TR_BANK1_PLL_CTL1, temp, 4);

    temp[0] = 0x29;
    temp[1] = 0x89;
    temp[2] = 0x75;
    temp[3] = 0x28;
    temp[4] = 0x50;
    tr_spi_wr_reg(TR_BANK1_CAL_CTL, temp, 5);

    temp[0] = 0xFF;
    temp[1] = 0x1F;
    temp[2] = 0x08;
    temp[3] = 0x29;
    tr_spi_wr_reg(TR_BANK1_FAGC_CTRL_1, temp, 4);

    temp[0] = 0x02;
    temp[1] = 0xC1;
    temp[2] = 0xCB;
    temp[3] = 0x1C;
    tr_spi_wr_reg(TR_BANK1_AGC_GAIN, temp, 4);

    temp[0] = 0x97;
    temp[1] = 0x64;
    temp[2] = 0x00;
    temp[3] = 0x01;
    tr_spi_wr_reg(TR_BANK1_RF_IVGEN, temp, 4);
    tr_bank_switch(TR_Bank0);
}

void tr_init(void)
{
    uint8_t  temp[5];

    HS_MAC6200->SPIRCON |= (TR_IRQ_RXFIFO_FULL | TR_IRQ_RXDATA_READY | TR_IRQ_TxFIFO_EMPTY |
                            TR_IRQ_TXFIFO_READY); //disable all spi interrupt

    tr_bank_switch(TR_Bank1);

    temp[0] = 0x0f;
    temp[1] = 0x28;
    temp[2] = 0x6f;
    temp[3] = 0x00;
    //rf_lld_Wr_Reg(MAC6200_BANK1_PLL_CTL1, temp, 5);
    tr_spi_wr_reg(TR_BANK1_PLL_CTL1, temp, 4);

    temp[0] = 0x20;
    temp[1] = 0x08;
    temp[2] = 0x54;
    temp[3] = 0x2b;
    temp[4] = 0x78;
    //rf_lld_Wr_Reg(MAC6200_BANK1_CAL_CTL, temp, 5);
    tr_spi_wr_reg(TR_BANK1_CAL_CTL, temp, 5);

    tr_bank_switch(TR_Bank0);
}

void tr_dump_rf_register()
{
    #if 1
    uint8_t  reg_value;
    uint8_t  temp[5];
    int8_t i;

    tr_phy_enable();

    tr_bank_switch(TR_Bank0);

    tr_spi_rd_reg(TR_BANK0_CONFIG, &reg_value, 1);
    printf("TR_BANK0_CONFIG: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_EN_AA, &reg_value, 1);
    printf("TR_BANK0_EN_AA: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_EN_RXADDR, &reg_value, 1);
    printf("TR_BANK0_EN_RXADDR: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_SETUP_AW, &reg_value, 1);
    printf("TR_BANK0_SETUP_AW: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_SETUP_RETR, &reg_value, 1);
    printf("TR_BANK0_SETUP_RETR: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_CH, &reg_value, 1);
    printf("TR_BANK0_CH: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RF_SETUP, &reg_value, 1);
    printf("TR_BANK0_RF_SETUP: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_STATUS, &reg_value, 1);
    printf("TR_BANK0_STATUS: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_OBSERVE_TX, &reg_value, 1);
    printf("TR_BANK0_OBSERVE_TX: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RPD, &reg_value, 1);
    printf("TR_BANK0_RPD: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_ADDR_P0, temp, 5);
    printf("TR_BANK0_RX_ADDR_P0: ");
    for (i = 4; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK0_RX_ADDR_P1, &reg_value, 1);
    printf("TR_BANK0_RX_ADDR_P1: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_ADDR_P2, &reg_value, 1);
    printf("TR_BANK0_RX_ADDR_P2: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_ADDR_P3, &reg_value, 1);
    printf("TR_BANK0_RX_ADDR_P3: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_ADDR_P4, &reg_value, 1);
    printf("TR_BANK0_RX_ADDR_P4: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_ADDR_P5, &reg_value, 1);
    printf("TR_BANK0_RX_ADDR_P5: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_TX_ADDR, temp, 5);
    printf("TR_BANK0_TX_ADDR: ");
    for (i = 4; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK0_RX_PW_P0, &reg_value, 1);
    printf("TR_BANK0_RX_PW_P0: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_PW_P1, &reg_value, 1);
    printf("TR_BANK0_RX_PW_P1: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_PW_P2, &reg_value, 1);
    printf("TR_BANK0_RX_PW_P2: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_PW_P3, &reg_value, 1);
    printf("TR_BANK0_RX_PW_P3: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_PW_P4, &reg_value, 1);
    printf("TR_BANK0_RX_PW_P4: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_RX_PW_P5, &reg_value, 1);
    printf("TR_BANK0_RX_PW_P5: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_FIFO_STATUS, &reg_value, 1);
    printf("TR_BANK0_FIFO_STATUS: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_DYNPD, &reg_value, 1);
    printf("TR_BANK0_DYNPD: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_FEATURE, &reg_value, 1);
    printf("TR_BANK0_FEATURE: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK0_SETUP_VALUE, temp, 5);
    printf("TR_BANK0_SETUP_VALUE: ");
    for (i = 4; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK0_PRE_GURD, &reg_value, 1);
    printf("TR_BANK0_PRE_GURD: %02x\r\n\n\n", (int)reg_value);

    tr_bank_switch(TR_Bank1);

    tr_spi_rd_reg(TR_BANK1_LINE, temp, 2);
    printf("TR_BANK1_LINE: ");
    for (i = 1; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_PLL_CTL0, temp, 4);
    printf("TR_BANK1_PLL_CTL0: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_PLL_CTL1, temp, 4);
    printf("TR_BANK1_PLL_CTL1: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_CAL_CTL, temp, 5);
    printf("TR_BANK1_CAL_CTL: ");
    for (i = 4; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_A_CNT_REG, &reg_value, 1);
    printf("TR_BANK1_A_CNT_REG: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_B_CNT_REG, &reg_value, 1);
    printf("TR_BANK1_B_CNT_REG: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_RESERVED1, &reg_value, 1);
    printf("TR_BANK1_RESERVED1: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_STATUS, &reg_value, 1);
    printf("TR_BANK1_STATUS: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_STATE, temp, 2);
    printf("TR_BANK1_STATE: ");
    for (i = 1; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_CHAN, temp, 4);
    printf("TR_BANK1_CHAN: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_IF_FREQ, temp, 3);
    printf("TR_BANK1_IF_FREQ: ");
    for (i = 2; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_AFC_COR, temp, 3);
    printf("TR_BANK1_AFC_COR: ");
    for (i = 2; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_FDEV, &reg_value, 1);
    printf("TR_BANK1_FDEV: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_DAC_RANGE, &reg_value, 1);
    printf("TR_BANK1_DAC_RANGE: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_DAC_IN, &reg_value, 1);
    printf("TR_BANK1_DAC_IN: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_CTUNING, temp, 2);
    printf("TR_BANK1_CTUNING: ");
    for (i = 1; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_FTUNING, temp, 2);
    printf("TR_BANK1_FTUNING: ");
    for (i = 1; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_RX_CTRL, temp, 4);
    printf("TR_BANK1_RX_CTRL: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_FAGC_CTRL, temp, 4);
    printf("TR_BANK1_FAGC_CTRL: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_FAGC_CTRL_1, temp, 4);
    printf("TR_BANK1_FAGC_CTRL_1: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_DAC_CAL_LOW, &reg_value, 1);
    printf("TR_BANK1_DAC_CAL_LOW: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_DAC_CAL_HI, &reg_value, 1);
    printf("TR_BANK1_DAC_CAL_HI: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_RESERVED3, &reg_value, 1);
    printf("TR_BANK1_RESERVED3: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_DOC_DACI, &reg_value, 1);
    printf("TR_BANK1_DOC_DACI: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_DOC_DACQ, &reg_value, 1);
    printf("TR_BANK1_DOC_DACQ: %02x\r\n", (int)reg_value);

    tr_spi_rd_reg(TR_BANK1_AGC_CTRL, temp, 4);
    printf("TR_BANK1_AGC_CTRL: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_AGC_GAIN, temp, 4);
    printf("TR_BANK1_AGC_GAIN: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_RF_IVGEN, temp, 4);
    printf("TR_BANK1_RF_IVGEN: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");

    tr_spi_rd_reg(TR_BANK1_TEST_PKDET, temp, 4);
    printf("TR_BANK1_TEST_PKDET: ");
    for (i = 3; i >= 0; i--) {
        printf("%02x ", (int)temp[i]);
    }
    printf("\r\n");
    tr_bank_switch(TR_Bank0);

    #endif
}

/*************for 2.4G rf adapte************/
/*Clear All Irq*/
void tr_clear_all_irq(void)
{
    uint8_t reg_val = 0;

    reg_val = 0x70;
    tr_spi_wr_reg(TR_BANK0_STATUS, &reg_val, 1);
}

/*read status*/
unsigned char tr_read_status(void)
{
    unsigned char status;
    tr_spi_rd_reg(TR_BANK0_STATUS, &status, 1);

    return status;
}

unsigned char tr_read_fifo_status(void)
{
    unsigned char status;
    tr_spi_rd_reg(TR_BANK0_FIFO_STATUS, &status, 1);

    return status;
}

void tr_write_status(unsigned char status)
{
    tr_spi_wr_reg(TR_BANK0_STATUS, &status, 1);
}

/*Mode_Switch*/
/*Caution: Before calling the function 'HS6206_ModeSwitch', */
/*              you must pull down the CE pin, just call the              */
/*              function HS6206_CE_Low()                                */

void tr_mode_switch(tr_mode_t mod)
{
    unsigned char tmp;

    tr_spi_rd_reg(TR_BANK0_CONFIG, &tmp, 1);
    if (mod == TR_PTX_MODE) {
        tmp &= 0xFE;
    } else {
        tmp |= 0x01;
    }
    tr_spi_wr_reg(TR_BANK0_CONFIG, &tmp, 1);
}

/*buf:data buffer; len:[1~32] ; cmd: HS6200_W_TX_PAYLOAD/HS6200_W_TX_PAYLOAD_NOACK*/
void tr_write(unsigned char *buf, unsigned char len)
{
    tr_write_tx_payload(buf, len);
    // CE high pulse to avoid:
    // 1. Sending retry forever with not receive ack
    // 2. In the max retry irq, the unexpected TXEN pulse will when clear the tx fifo.
    tr_ce_high_pulse();
}

void tr_write_no_ack(unsigned char *buf, unsigned char len)
{
    tr_write_tx_payload_no_ack(buf, len);
    // CE high pulse to avoid:
    // 1. Sending retry forever with not receive ack
    // 2. In the max retry irq, the unexpected TXEN pulse will when clear the tx fifo.
    tr_ce_high_pulse();
}

/*buf:data read from rx buffer*/
unsigned char tr_read(unsigned char *buf)
{
    unsigned char length;

    length = tr_read_rx_payload_width();
    if (length) {
        tr_read_rx_payload(buf, length);
    }

    return length;
}

/*Caution: Before calling the 'HS6206_ChangeAddr_Reg', */
/*              you should pull down the CE pin, using 'HS6206_CE_Low'*/
void tr_change_address(unsigned char *addrbuf, unsigned char len)
{
    tr_spi_wr_reg(TR_BANK0_RX_ADDR_P0, addrbuf, len);
    tr_spi_wr_reg(TR_BANK0_TX_ADDR, addrbuf, len);
}

void tr_set_freq(unsigned int freq)
{
    uint8_t ch_index = freq - 2400;
    tr_spi_wr_reg(TR_BANK0_CH, &ch_index, 1);

    // frequency
    CPM_ANA_CLK_ENABLE();
    REGW(&HS_DAIF->FREQ_CFG0, MASK_2REG(DAIF_FREQ_REG_ME, 1, DAIF_FREQ_REG_MO, freq));
    CPM_ANA_CLK_RESTORE();

}

void tr_change_ch(unsigned char ch_index)
{
    tr_set_freq(2400 + ch_index);
}

void HWradio_Enable(bool enable)
{
    if (enable) {

    } else {

    }
}

#if 0
/**
 * @brief rf_single_tone_enable()
 *
 * @param[in] enable  true or false
 * @param[in] freq  2402 ...
 * @param[in] payload
 *
 * @return None
 **/
void rf_single_tone_enable(bool enable, uint32_t freq, uint8_t payload)
{
    uint8_t temp[4];

    if (enable) {
        HWradio_Enable(true);

        // enable 2.4g phy
        tr_phy_enable();

        // enable single tone
        tr_bank_switch(TR_Bank1);
        tr_spi_rd_reg(TR_BANK1_AGC_CTRL, temp, 4);
        temp[3] = payload; // +250kHz in carry wave
        temp[2] |= 0x80;
        tr_spi_wr_reg(TR_BANK1_AGC_CTRL, temp, 4);
        tr_bank_switch(TR_Bank0);

        HS_DAIF->FREQ_CFG0 = freq;
        HS_DAIF->FREQ_CFG2 = 0;

        // CE High
        tr_ce_high();
    } else {
        // CE Low
        tr_ce_low();

        // diable single tone
        tr_bank_switch(TR_Bank1);
        tr_spi_rd_reg(TR_BANK1_AGC_CTRL, temp, 4);
        temp[2] &= 0x7F;
        tr_spi_wr_reg(TR_BANK1_AGC_CTRL, temp, 4);
        tr_bank_switch(TR_Bank0);

        // Disable 2.4g phy
        tr_phy_disable();

        HWradio_Enable(false);
    }
}

/**
 * @brief  rf carrier enable
 *
 * @param[in] enable  enable
 * @param[in] freq  2402MHz ... 2480MHz
 **/
void rf_carrier_enable(bool enable, uint32_t freq)
{
    CPM_ANA_CLK_ENABLE();

    if (enable) {
        // frequency
        REGW(&HS_DAIF->FREQ_CFG0, MASK_2REG(DAIF_FREQ_REG_ME, 1, DAIF_FREQ_REG_MO, freq));

        //        REGW(&HS_DAIF->PLL_CTRL0, MASK_2REG(DAIF_LDO_BUF,1, DAIF_LDO_BUF2,1));
        //
        //        REGW(&HS_DAIF->PD_CFG1, MASK_2REG(DAIF_PD_LOTX_ME,1, DAIF_PD_LOTX_MO,0));
        //        REGW(&HS_DAIF->TRX_SW_CFG, MASK_2REG(DAIF_TRX_EN_ME,1, DAIF_TRX_EN_MO,0));
        //
        //        REGW(&HS_DAIF->PD_CFG0, MASK_2REG(DAIF_PD_PA_ME,1, DAIF_PD_PA_MO,0)); //0:-53dB 1:-45dB
        //        REGW(&HS_DAIF->PD_CFG0, MASK_2REG(DAIF_PD_LDO_PA_ME,1, DAIF_PD_LDO_PA_MO,0));
        //
        //        // PA gain
        //        REGW(&HS_DAIF->PA_CNS, MASK_2REG(DAIF_PA_GAIN_IDX_SEL,0, DAIF_PA_GAIN_IDX_REG,18));

        // do TX
        REGW(&HS_DAIF->VCO_CTRL0, MASK_3REG(DAIF_TRX_DBG, 1, DAIF_RX_EN_MO, 0, DAIF_TX_EN_MO, 0));
        co_delay_10us(1);
        REGW(&HS_DAIF->VCO_CTRL0, MASK_3REG(DAIF_TRX_DBG, 1, DAIF_RX_EN_MO, 0, DAIF_TX_EN_MO, 1));
    } else {

    }

    CPM_ANA_CLK_RESTORE();
}



/**
 * @brief rf_full_rx_enable()
 *
 * @param[in] enable
 * @param[in] freq  2402 ...
 *
 * @return
 **/
void rf_full_rx_enable(bool enable, uint32_t freq)
{
    if (enable) {
        unsigned char tmp = 0x8F;

        HWradio_Enable(true);

        // pyh config
        //HS_PHY->RSSI_CAP_MODE = 1;
        //HS_PHY->RSSI_TONE_EST_MODE = 1;

        // enable 2.4g phy
        tr_phy_enable();

        // frequency cofig
        tr_set_freq(freq);

        // RX mode
        tr_spi_wr_reg(TR_BANK0_CONFIG, &tmp, 1);

        // CE High
        tr_ce_high();
    } else {
        // pyh config
        //HS_PHY->RSSI_CAP_MODE = 0;
        //HS_PHY->RSSI_TONE_EST_MODE = 0;

        // CE High
        tr_ce_low();

        // Disable 2.4g phy
        tr_phy_disable();

        HWradio_Enable(false);
    }
}
#endif
/*Configuration Reg*/
void tr_configure_reg(const unsigned char *dbuf)
{
    unsigned char cnt = 0, reg_addr, data_lenth;
    const unsigned char *p_data;
    while (1) {
        /*Get reg_addr*/
        reg_addr = dbuf[cnt];
        if (reg_addr == 0xFF) {
            break;
        }

        /*Get Data_Lenth*/
        cnt += 1;
        data_lenth = dbuf[cnt];

        /*Get Data address*/
        cnt += 1;
        p_data = &dbuf[cnt];

        /*Write buffer*/
        tr_spi_wr_reg(reg_addr, p_data, data_lenth);
        cnt += data_lenth;

        if (reg_addr == TR_BANK0_CH) {
            tr_change_ch(*p_data);
        }
    }
}

void tr_configure_reg_bank1(const unsigned char *dbuf)
{
    unsigned char cnt = 0, reg_addr, data_lenth;
    const unsigned char *p_data;

    tr_bank_switch(TR_Bank1);

    while (1) {
        /*Get reg_addr*/
        reg_addr = dbuf[cnt];
        if (reg_addr == 0xFF) {
            break;
        }

        /*Get Data_Lenth*/
        cnt += 1;
        data_lenth = dbuf[cnt];

        /*Get Data address*/
        cnt += 1;
        p_data = &dbuf[cnt];

        /*Write buffer*/
        tr_spi_wr_reg(reg_addr, p_data, data_lenth);
        cnt += data_lenth;
    }

    tr_bank_switch(TR_Bank0);
}

int8_t tr_read_rssi(void)
{
    return ((int8_t)HS_PHY->SIG_DBM_EST_O - 10);
}
#else

void rf_single_tone_enable(bool enable, uint32_t freq, uint8_t payload) {}
void rf_carrier_enable(bool enable, uint32_t freq) { }

#endif

/*-------------------------------------------End Of File---------------------------------------------*/

/** @} */

