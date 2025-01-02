//------------------------------------------------------------------------------------
// app_hs6230_test.c
//------------------------------------------------------------------------------------
// Copyright 2019, HunterSun Electronics Co., Ltd.
// crazy_code@yeah.net
// 2019-07-17
//
// Program Description:
//
//
//


#include <stdio.h>
#include "command_line.h"
#include "queue.h"
#include "hw_transceiver.h"
#include "tr_hardware.h"
#include "peripheral.h"
#include <string.h>
#include "arch.h"      // architectural platform definitions

static uint8_t j = 1;
static uint32_t error_bit = 0;
static uint32_t error_count = 0;
static uint32_t right_count = 0;
static uint8_t tr_tx_payload[32];
static uint8_t tr_rx_payload[32];
static volatile bool max_try_flag = false;
static volatile bool tx_irq_flag = false;
static volatile bool rx_irq_ack_en = false;

static uint8_t bitcount(uint8_t n)
{
    uint8_t count = 0 ;
    while (n) {
        count++ ;
        n &= (n - 1) ;
    }
    return count;
}

void send_rf_packet_test(uint8_t ack_en)
{

    uint16_t i, j = 1;
    uint8_t status = 0x00;
    uint8_t payload_len = 0;
    uint16_t ack_count = 0;
    uint16_t max_rty_count = 0;
    uint16_t total_count = 300;

    tr_hardware_init();

    tr_write_byte(TR_BANK0_FEATURE, ack_en ? 0x07 : 0x00);
    tr_write_byte(TR_BANK0_DYNPD, ack_en ? 0x3f : 0x00);
    tr_write_byte(TR_BANK0_EN_AA, ack_en ? 0x3f : 0x00);
    tr_write_byte(TR_BANK0_SETUP_AW, 0x03);
    tr_write_byte(TR_BANK0_CONFIG, ack_en ? 0x0E : 0x0E);
    tr_set_freq(2480);

    tr_dump_rf_register();
    for (i = 0; i < 32; i++) {
        tr_tx_payload[i] = i + 1;
    }

    while (total_count--) {
        tr_ce_low();

        tr_flush_tx_fifo();

        tr_clear_all_irq();

        if (ack_en) {
            tr_write(tr_tx_payload, j);
        } else {
            tr_write(tr_tx_payload, 32);
        }

        /*wait data send finished*/
        while (1) {
            status = tr_read_byte(TR_BANK0_STATUS);
            if ((TR_STATUS_TX_DS & status) || (TR_STATUS_MAX_RT & status)) {
                tr_write_byte(TR_BANK0_STATUS, status);
                if (TR_STATUS_TX_DS & status) {
                    if (ack_en) {
                        ack_count++;
                        payload_len = tr_read_rx_payload_width(); //read ack length.
                        tr_read_rx_payload(tr_rx_payload, payload_len);
                    }

                    tr_flush_rx_fifo();
                    tr_flush_tx_fifo();
                    tr_clear_all_irq();
                    if (ack_en) {
                        log_debug("ACK: ");
                        for (i = 0x00; i < payload_len; i++) {
                            log_debug("%02d ", tr_rx_payload[i]);
                        }
                        log_debug("\r\n");
                    } else {
                        if ((total_count + 1) % 16 == 0) {
                            log_debug("\r\n");
                        }
                        log_debug("#");
                    }
                } else {
                    tr_flush_rx_fifo();
                    tr_flush_tx_fifo();
                    tr_clear_all_irq();
                    if (ack_en) {
                        max_rty_count++;
                        log_debug("Max_Retry\r\n");
                    }
                }
                status = 0;
				payload_len = 0;
				for (i = 0x00; i < 50; i++) {
                    tr_rx_payload[i] = 0;
                }
                break;
            }
        }
        co_delay_ms(100);   //Wait for the receiver to print
        j++;
        if (j > 32) {
            j = 1;
        }

    }
    if (ack_en) {
        log_debug("T:%d ACK:%d MAX_RT:%d \r\n", (ack_count + max_rty_count), ack_count, max_rty_count);
    }
    log_debug("\r\nsend 100th done\r\n");
    tr_phy_disable();
}


void receive_rf_packet_test(uint8_t ack_en)
{

    uint16_t i, j = 1;
    uint8_t status = 0x00;
    uint8_t payload_len = 0;
    uint32_t error_bit = 0;
    uint32_t error_count = 0;
    uint32_t right_count = 0;
    uint32_t timeout = 0;
    bool error_flag = false;

    tr_hardware_init();

    tr_write_byte(TR_BANK0_FEATURE, ack_en ? 0x07 : 0x00);
    tr_write_byte(TR_BANK0_DYNPD, ack_en ? 0x3f : 0x00);
    tr_write_byte(TR_BANK0_EN_AA, ack_en ? 0x3f : 0x00);
    tr_write_byte(TR_BANK0_CONFIG, ack_en ? 0x0F : 0x0F);
    tr_set_freq(2480);

    tr_bank_switch(TR_Bank0);

    tr_dump_rf_register();

    tr_ce_low();

    tr_flush_rx_fifo();
    tr_flush_tx_fifo();
    tr_clear_all_irq();

    for (i = 0; i < 32; i++) {
        tr_tx_payload[i] = i + 1;
    }

    if (ack_en) {
		tr_flush_tx_fifo();
        tr_write_ack_payload(0, tr_tx_payload, 1);
    }

    tr_ce_high();

    while (1) {
        status = tr_read_byte(TR_BANK0_STATUS);
        if ((TR_STATUS_RX_DR & status)) {

            tr_ce_low();

            payload_len = tr_read_rx_payload_width(); //read rx payload length.
            tr_read_rx_payload(tr_rx_payload, payload_len);
            tr_flush_rx_fifo();
            // tr_flush_tx_fifo();
            tr_clear_all_irq();
            tr_ce_high();
            for (i = 0x00; i < payload_len; i++) {
                //log_debug("%02d ", tr_rx_payload[i]);
                if ((i + 1) != tr_rx_payload[i]) {
                    error_flag = true;
                    error_bit += bitcount(i ^ tr_rx_payload[i]);
                }
            }
			log_debug("%02d ", tr_rx_payload[payload_len - 1]);

            if (error_flag) {
                error_count++;
            } else {
                right_count++;
            }

            if (error_flag) {
                error_flag = false;
                log_debug("err Pkt:");
                for (i = 0; i < payload_len; i++) {
                    log_debug("%d ", tr_rx_payload[i]);
                }
                log_debug("\r\n");
            } else {
                //log_debug("\nT:%d  E:%d  R:%d  BitError:%d\r\n", (error_count + right_count), error_count, right_count, error_bit);
            }
            log_debug("\r\n");
			co_delay_ms(10);
			if (ack_en) {
                j++;
                if (j > 32) {
                    j = 1;
                }
				tr_flush_tx_fifo();
                tr_write_ack_payload(0, tr_tx_payload, j);
            }

            status = 0;
            timeout = 0;
        }
        timeout++;
        if(timeout>1000000)
            break;
    }
	log_debug("\nT:%d  E:%d  R:%d  BitError:%d\r\n", (error_count + right_count), error_count, right_count, error_bit);
	tr_ce_low();
    log_debug("T:%d \r\n", right_count);
    tr_phy_disable();
}

static void rf_rx_int_handle(uint8_t ack_en)
{
    uint16_t i;
    uint8_t payload_len = 0;
    bool error_flag = false;
    
    tr_ce_low();
    payload_len = tr_read_rx_payload_width(); //read rx payload length.
    tr_read_rx_payload(tr_rx_payload, payload_len);
    tr_flush_rx_fifo();
    tr_clear_all_irq();
    
    for (i = 0x00; i < payload_len; i++) {
        //log_debug("%02d ", tr_rx_payload[i]);
        if ((i + 1) != tr_rx_payload[i]) {
            error_flag = true;
            error_bit += bitcount(i ^ tr_rx_payload[i]);
        }
    }
    if (error_flag) {
        error_count++;
    } else {
        right_count++;
    }
    if (error_flag) {
        error_flag = false;
        log_debug("err Pkt:");
        for (i = 0; i < payload_len; i++) {
            log_debug("%d ", tr_rx_payload[i]);
        }
        log_debug("\r\n");
    } else {
        log_debug("\nT:%d  E:%d  R:%d  BitError:%d\r\n", (error_count + right_count), error_count, right_count, error_bit);
    }
    log_debug("\r\n");
    if (ack_en) {
        j++;
        if (j > 32) {
            j = 1;
        }
        tr_write_ack_payload(0, tr_tx_payload, j);
    }
    tr_ce_high();
}



void receive_rf_packet_int_test(uint8_t ack_en)
{
    uint16_t i;

    rx_irq_ack_en = ack_en;
    tr_hardware_init();
    tr_write_byte(TR_BANK0_FEATURE, ack_en ? 0x07 : 0x00);
    tr_write_byte(TR_BANK0_DYNPD, ack_en ? 0x3f : 0x00);
    tr_write_byte(TR_BANK0_EN_AA, ack_en ? 0x3f : 0x00);
    tr_write_byte(TR_BANK0_CONFIG, ack_en ? 0x8F : 0x83);
    tr_set_freq(2418);
    tr_bank_switch(TR_Bank0);
    //tr_dump_rf_register();
    tr_ce_low();
    tr_flush_rx_fifo();
    tr_flush_tx_fifo();
    tr_clear_all_irq();

    for (i = 0; i < 32; i++) {
        tr_tx_payload[i] = i + 1;
    }
    if (ack_en) {
        tr_write_ack_payload(0, tr_tx_payload, 2);
        tr_ce_high();
    }
    tr_ce_high();
    NVIC_ClearPendingIRQ(MAC6200_RF_IRQn);
    NVIC_SetPriority(MAC6200_RF_IRQn, IRQ_PRIORITY_NORMAL);
    NVIC_EnableIRQ(MAC6200_RF_IRQn);
    //while(1);
}

void test_rf_irq(void)
{
    uint32_t timeout = 0;
    tr_hardware_init();

    tr_write_byte(TR_BANK0_FEATURE,  0x07);
    tr_write_byte(TR_BANK0_DYNPD, 0x3f);
    tr_write_byte(TR_BANK0_EN_AA, 0x3f);
    tr_write_byte(TR_BANK0_SETUP_AW, 0x03);
    tr_write_byte(TR_BANK0_CONFIG, 0x82);
    tr_set_freq(2408);

    NVIC_ClearPendingIRQ(MAC6200_RF_IRQn);
    NVIC_SetPriority(MAC6200_RF_IRQn, IRQ_PRIORITY_NORMAL);
    NVIC_EnableIRQ(MAC6200_RF_IRQn);

    tr_ce_low();
    tr_flush_tx_fifo();
    tr_clear_all_irq();

    tr_write(tr_tx_payload, 1);

    while (1) {
        timeout++;
        if (timeout > 1000000) {
            log_debug("test max retry interrupt timeout \r\n");
            break;

        }
        if (max_try_flag) {
            log_debug("test max retry interrupt success \r\n");
            max_try_flag = false;
            break;
        }
    }

    timeout = 0;
    tr_ce_low();
    tr_flush_tx_fifo();

    tr_write_no_ack(tr_tx_payload, 1);

    while (1) {
        timeout++;
        if (timeout > 1000000) {
            log_debug("test irq interrupt timeout \r\n");
            break;

        }
        if (tx_irq_flag) {
            log_debug("test irq interrupt success \r\n");
            tx_irq_flag = false;
            break;
        }
    }
    NVIC_DisableIRQ(MAC6200_RF_IRQn);
    tr_phy_disable();
}

uint8_t CLI_TRTest(void *para, uint8_t len)
{
    uint8_t *ptemp;
    uint8_t result = FALSE;
    ptemp = (uint8_t *)para;
    if ((0 < len) && (NULL != ptemp)) {
        ptemp++; /* skip a blank space */
        if ('1' == *ptemp) {
            log_debug("HS6621 2.4G reciver test (no ack, nocrc, spl)\r\n");
            receive_rf_packet_test(0);
            NL1();
            DBG("\r\ntest PASS\r\n");
        } else if ('0' == *ptemp) {
            /* irq test */
            test_rf_irq();
            NL1();
            if (result == TRUE) {
                DBG("\r\ntest PASS\r\n");
            } else {
                DBG("\r\ntest FAILE\r\n");
            }
        } else if ('2' == *ptemp) {
            log_debug("HS6621 2.4G send test (no ack, nocrc, spl)\r\n");
            send_rf_packet_test(0);
            NL1();
            DBG("\r\ntest PASS\r\n");
        } else if ('3' == *ptemp) {
            log_debug("HS6621 2.4G reciver test (ack, crc, dpl)\r\n");
            receive_rf_packet_test(1);
            NL1();
            DBG("\r\ntest PASS\r\n");
        } else if ('4' == *ptemp) {
            log_debug("HS6621 2.4G send test (ack, crc, dpl)\r\n");
            send_rf_packet_test(1);
            NL1();
            DBG("\r\ntest PASS\r\n");
        } else {
            /* para. wrong */
            return FALSE;
        }
    }

    /* led command ok */
    return TRUE;
}

void MAC6200_RF_IRQHandler(void)
{
    uint8_t status = 0;
    status = tr_read_byte(TR_BANK0_STATUS);
    //log_debug("status=%02x\r\n", status);
    if ((TR_STATUS_TX_DS & status)) {
        tx_irq_flag = true;
    }
    if (TR_STATUS_MAX_RT & status) {
        max_try_flag = true;
    }
    if (TR_STATUS_RX_DR & status) {
        rf_rx_int_handle(rx_irq_ack_en);
    }
    tr_clear_all_irq();
}

