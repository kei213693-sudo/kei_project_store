/**
 * @file app_tr_ble_sim.c
 * @brief 
 * @date Fri Dec 23 13:13:43 2022
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
#include "co.h"
#include "peripheral.h"
#include "hw_transceiver.h"
#include "tr_ble_sim.h"
#include "timer_pwm_input.h"
#include "rwip_config.h"     // SW configuration

/*********************************************************************
 * MACROS
 */
#define DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE()      gpio_toggle(BITMASK(PIN_LED_0))

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */
static co_timer_t simple_timer;

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */
/**
 * @brief  tr switch role ex
 *
 * @param[in] role  role
 **/
static void tr_switch_role_ex(unsigned char role)
{
    unsigned char tmp = role==TR_PRX_MODE ? 0x03 : 0x02;
    tr_spi_wr_reg(TR_BANK0_CONFIG, &tmp, 1);
    tr_flush_tx_fifo();
    tr_flush_rx_fifo();
}

/**
 * @brief  tr ble txrx once
 **/
static void tr_ble_txrx_once(uint32_t send_on_dwt_cnt)
{
    uint8_t status = 0x00;
    uint8_t payload_len = 0;
    uint32_t tr_tx_length;
    uint8_t tr_tx_payload[32];
    uint8_t tr_rx_payload[32];
    int loop_n = 0;
    int rx_length;
    uint32_t rx_start_time;
    uint32_t rx_timeout;
    bool rx_synced;
    uint32_t cpu_mhz = cpm_get_clock(CPM_CPU_CLK)/1000000;

    const uint8_t ble_payload[] = {
        0x24, 0x24, 0x21, 0x66, 0xBF, 0x01,
        0x03, 0x09, 0x58, 0x58
    };

    DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();

    tr_ble_enable(true);

    tr_set_freq(2402);

    tr_ble_aa_set(0x8E89BED6);

    tr_tx_length = tr_ble_pdu_generate(0x555555, 37, 0x40, ble_payload, sizeof(ble_payload), tr_tx_payload);
    tr_write_tx_payload(tr_tx_payload, tr_tx_length);

    // start TX
    while(DWT->CYCCNT < send_on_dwt_cnt);
    tr_ce_high_pulse();
    DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();

    while (1)
    {
        /*wait data send finished*/
        status = tr_read_byte(TR_BANK0_STATUS);
        if (TR_STATUS_TX_DS & status)
        {
            DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();
            tr_switch_role_ex(TR_PRX_MODE);
            tr_ce_high();
            while(TR_BLE_SYNC_DETECTED()); // polling AGC is running (agc idle is sync detected)
            tr_clear_all_irq();
            DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();
            rx_start_time= DWT->CYCCNT;
            rx_synced = false;
            while (1)
            {
                status = tr_read_byte(TR_BANK0_STATUS);
                if (TR_STATUS_RX_DR & status)
                {
                    DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();
                    tr_ce_low();
                    payload_len = tr_read_rx_payload_width();
                    tr_read_rx_payload(tr_rx_payload, payload_len);
                    // remove first invalid byte
                    DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();
                    rx_length = tr_ble_packet_parse(0x555555, 37, tr_rx_payload, payload_len);
                    DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();
                    if (rx_length > 0)
                        log_debug_array_ex("RX", tr_rx_payload, rx_length);
                    else
                        log_debug("RX: error\n");
                    break;
                }

                if (!rx_synced)
                {
                    // agc idle is sync detected
                    if (TR_BLE_SYNC_DETECTED())
                    {
                        DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();
                        rx_synced = true;
                        DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();
                    }
                }

                // The time from rx_start_time(agc running) to sync_detect is about 91us (payload is 12bytes)
                // The time from rx_start_time to RX_done is about 248us (payload is 12bytes)
                if (rx_synced)
                    rx_timeout = (TR_BLE_RX_FIXED_LENGTH+1+4+2+3) * 8 + 150 - 50;
                else
                    rx_timeout = (1+4) * 8 + 150 - 90;

                if ((DWT->CYCCNT - rx_start_time) / cpu_mhz > rx_timeout)
                {
                    DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();
                    tr_ce_low();
                    break;
                }
            }
            tr_clear_all_irq();
            tr_switch_role_ex(TR_PTX_MODE);
            break;
        }
    }

    tr_ble_enable(false);

    DEBUG_APP_TR_BLE_SIM_GPIO_TOGGLE();
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 * @brief  simple timer handler
 *
 * @param[in] timer  timer
 * @param[in] param  param
 **/
//void simple_timer_handler(co_timer_t *timer, void *param)
//{
//    CO_DISABLE_IRQ();
//    for (int i=0; i<10; ++i)
//    {
//        // current send delay: 143us
//        tr_ble_txrx_once(DWT->CYCCNT + 160*64);
//        //co_delay_us(100);
//    }
//    CO_RESTORE_IRQ();
//}

/**
 * @brief  tr ble gpio handler
 *
 * @param[in] level  level
 **/
void tr_ble_gpio_handler(uint32_t level)
{
    uint16_t cnt = HS_TIM0->CNT;
    uint16_t delay_cycle;

    if (level)
        delay_cycle = cnt - (uint16_t)HS_TIM0->CCR[1];
    else
        delay_cycle = cnt - (uint16_t)HS_TIM0->CCR[2];

    if (delay_cycle/64 > 20)
    {
        log_debug("gpio delay too larger: %dus\n", delay_cycle/64);
        return;
    }

    CO_DISABLE_IRQ();
    tr_ble_txrx_once(DWT->CYCCNT - delay_cycle + 180*64);
    for (int i=0; i<10; ++i)
    {
        tr_ble_txrx_once(DWT->CYCCNT + 160*64);
        //co_delay_us(100);
    }
    CO_RESTORE_IRQ();

    log_debug("gpio delay: %dus\n", delay_cycle/64);
}

/**
 * @brief  tr ble test
 **/
void tr_ble_test(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    rf_txrx_pin_enable(true, PIN_LED_6, PIN_LED_7, 0);

    tr_ble_init();

    // IO input capture init
    pinmux_config(2, PINMUX_TIMER0_IO_0_CFG);
    tim_pwm_input_start(HS_TIM0, 0);

#if 0 // PWM test
    pinmux_config(19, PINMUX_TIMER1_IO_0_CFG);

    const tim_config_t pwm_cfg =
    {
        .mode = TIM_PWM_MODE,
        .config.pwm =
        {
            // pwm_frequency = count_freq/period_count (2000Hz)
            .count_freq = 1000,
            .period_count = 200,
            .channel =
            {
                // duty cycle = pulse_count/period_count
                {true/*enable*/, {TIM_PWM_POL_HIGH2LOW, 100}}, // channel0: duty cycle = 1/5
            },
            .callback = NULL,
        },
    };

    // PWM Init
    tim_init();
    tim_config(HS_TIM1, &pwm_cfg);
    tim_start(HS_TIM1);
#endif

#if 0
    // SYNC_DETECT debug bus
    pinmux_config(29, PINMUX_DBG_MODE_CFG);
    HS_SYS->MON = SYS_MON_PHY(0);
#endif

//    co_timer_set(&simple_timer, 100, TIMER_REPEAT, simple_timer_handler, NULL);
}

/** @} */

