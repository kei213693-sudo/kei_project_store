/**
 * @file main.c
 * @brief
 * @date Wed, Sep  5, 2018  5:19:05 PM
 * @author liqiang
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
#include "rwip_config.h" // RW SW configuration

#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "rwip.h"      // RW SW initialization
#include "dbg.h"
#include "peripheral.h"
#include "sysdump.h"
#include "hw_transceiver.h"
#include "tr_hardware.h"

/*********************************************************************
 * MACROS
 */

#define PIN_WAKEUP_0                    2   // UART-RTS@EVB
#define PIN_WAKEUP_1                    24  // KEY1@EVB
#define PIN_LED_0                       8
#define PIN_LED_1                       9
#define PIN_LED_2                       10
#define PIN_LED_3                       25
#define PIN_LED_4                       13
#define PIN_LED_5                       14
#define PIN_LED_6                       14
#define PIN_LED_7                       14
#define PIN_UART0_TX                    5
#define PIN_UART0_RX                    6

#define DEBUG_UART_BAUDRATE             115200
#define SWITCH_1M_TO_2M                 0   // 1:2M     0:1M
#define FREQ_NUM                        2402
#define RF_CARRIER_OPEN                 0  // 1:send carrier test   0:rx sensitivity test 
#define POWER_NUM                       0  // 0~18
/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */

static uint32_t pin_wakeup_mask = (BIT_MASK(PIN_WAKEUP_0) | BIT_MASK(PIN_WAKEUP_1));
static uint32_t pin_leds_mask   = (BIT_MASK(PIN_LED_0) | BIT_MASK(PIN_LED_1) |
                                   BIT_MASK(PIN_LED_2) | BIT_MASK(PIN_LED_3) |
                                   BIT_MASK(PIN_LED_4) | BIT_MASK(PIN_LED_5) |
                                   BIT_MASK(PIN_LED_6) | BIT_MASK(PIN_LED_7));

//static co_timer_t simple_timer;

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void gpio_handler(uint32_t pin_mask)
{
    //    static int counter = 0;
    //    log_debug("gpio: %08X [%d]\n", pin_mask, counter++);
}

static void wakeup_gpio_handler(uint32_t pin_mask)
{
    //    static int counter = 0;
    //    log_debug("wake_gpio: %08X [%d]\n", pin_mask, counter++);
}

static void pinmux_init(void)
{
    // KEY
    pinmux_config(PIN_WAKEUP_0,  PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_WAKEUP_1,  PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(pin_wakeup_mask, PMU_PIN_MODE_PU);

    // UART0
    pinmux_config(PIN_UART0_TX, PINMUX_UART0_SDA_O_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART0_TX), PMU_PIN_MODE_PP);
    pinmux_config(PIN_UART0_RX, PINMUX_UART0_SDA_I_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART0_RX), PMU_PIN_MODE_FLOAT);

    // LEDS
    pinmux_config(PIN_LED_0, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_1, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_2, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_3, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_4, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_5, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_6, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_7, PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(pin_leds_mask, PMU_PIN_MODE_PP);
}

static void peripheral_init(void)
{
    // Init GPIO
    gpio_open();

    // Wakeup Pin
    gpio_set_direction(pin_wakeup_mask, GPIO_INPUT);
    gpio_set_interrupt(pin_wakeup_mask, GPIO_BOTH_EDGE);
    gpio_set_interrupt_callback(gpio_handler);

    // LEDS
    gpio_write(pin_leds_mask, GPIO_HIGH);
    gpio_set_direction(pin_leds_mask, GPIO_OUTPUT);

    // wakeup
    pmu_wakeup_pin_set(pin_wakeup_mask, PMU_PIN_WAKEUP_LOW_LEVEL);
    pmu_wakeup_pin_register_callback(wakeup_gpio_handler, wakeup_gpio_handler);

    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
}

static void peripheral_restore(void)
{
    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
}

static void hardware_init(void)
{
    pmu_pin_mode_set(0xFFFFFFFF, PMU_PIN_MODE_PU);
    #ifndef CONFIG_GPIO1_ABSENT
    pmu_pin_mode_set_ex(0xFF, PMU_PIN_MODE_PU);
    #endif

    pinmux_init();
    peripheral_init();
}

static void power_sleep_event_handler(co_power_sleep_state_t sleep_state, co_power_status_t power_status)
{
    switch (sleep_state) {
        case POWER_SLEEP_ENTRY:
            //            log_debug("sleep entry\n");
            break;

        case POWER_SLEEP_LEAVE_TOP_HALF:
            peripheral_restore();
            break;

        case POWER_SLEEP_LEAVE_BOTTOM_HALF:
            //            log_debug("sleep leave\n");
            break;
    }
}

co_power_status_t power_sleep_status_handler(void)
{
    co_power_status_t pmu_status;

    pmu_status = pmu_power_status();

    //    return MIN(POWER_SLEEP, pmu_status);
    return pmu_status;
}

static void simple_timer_handler(co_timer_t *timer, void *param)
{
    //sysdump();
}

static void ble_stack_config(void)
{
    // Disable WDT
    wdt_enable(0);

    pmu_dcdc_enable(true);

    // Power down xtal32k in deep sleep mode
    pmu_32k_enable_in_deep_sleep(false);

    // Select 32k clock for stack
    //pmu_xtal32k_change_param(15, 1);
    //    pmu_select_32k(PMU_32K_SEL_32768HZ_XTAL);
    pmu_select_32k(PMU_32K_SEL_RC);

    // ultra sleep mode enable
    co_power_ultra_sleep_mode_enable(true);

    pmu_xtal32m_change_param(25);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(false);
    //sc.rw_sleep_enable = false;
}

static uint8_t bitcount(uint8_t n)
{
    uint8_t count = 0 ;
    while (n) {
        count++ ;
        n &= (n - 1) ;
    }
    return count;
}

static void receive_rf_packet_test(void)
{
    uint8_t tr_rx_payload[32];
    uint16_t i;
    uint8_t status = 0x00;
    uint8_t payload_len = 0;
    uint32_t error_bit = 0;
    uint32_t error_count = 0;
    uint32_t right_count = 0;
    bool error_flag = false;

    tr_hardware_init();

    tr_write_byte(TR_BANK0_FEATURE,  0x00);
    tr_write_byte(TR_BANK0_DYNPD,  0x00);
    tr_write_byte(TR_BANK0_EN_AA,  0x00);
    tr_write_byte(TR_BANK0_CONFIG, 0x83);
    #if SWITCH_1M_TO_2M
    tr_write_byte(TR_BANK0_RF_SETUP, 0x4F);  //2M
    #else
    tr_write_byte(TR_BANK0_RF_SETUP, 0x47);
    #endif
    tr_set_freq(FREQ_NUM);

    tr_bank_switch(TR_Bank1);
    tr_spi_rd_reg(TR_BANK1_CAL_CTL, tr_rx_payload, 3);
    tr_rx_payload[2] &= ~0x10; //Scramble_en = 0;
    tr_spi_wr_reg(TR_BANK1_CAL_CTL, tr_rx_payload, 3);
    tr_bank_switch(TR_Bank0);

    tr_dump_rf_register();

    tr_ce_low();

    tr_flush_rx_fifo();
    tr_flush_tx_fifo();
    tr_clear_all_irq();

    tr_ce_high();

    while (1) {
        status = tr_read_byte(TR_BANK0_STATUS);
        if ((TR_STATUS_RX_DR & status)) {
            tr_ce_low();

            payload_len = tr_read_rx_payload_width(); //read rx payload length.
            tr_read_rx_payload(tr_rx_payload, payload_len);

            tr_flush_rx_fifo();
            //          tr_flush_tx_fifo();
            tr_clear_all_irq();

            for (i = 0x00; i < payload_len; i++) {
                //              log_debug("%02d ", tr_rx_payload[i]);
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
            gpio_toggle(BIT_MASK(PIN_LED_4));
            tr_ce_high();
            status = 0;
        }
    }
    log_debug("T:%d \r\n", right_count);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
int main(void)
{
    ble_stack_config();

    hardware_init();

    rwip_init(RESET_NO_ERROR);
    #if 0
    rf_tx_power_set(false, 0);
    CPM_ANA_AHB_CLK_ENABLE();
    REGW(&HS_DAIF->PA_CNS, MASK_1REG( DAIF_PA_GAIN_IDX_REG, POWER_NUM)); //0~18 Gear:Power description in calib.c
    CPM_ANA_CLK_DISABLE();
    #else
    //rf_tx_power_set(false,9); //max power 9dbm
    #endif
    co_power_register_user_status(power_sleep_status_handler);
    co_power_register_sleep_event(power_sleep_event_handler);
    log_debug("running %d\n", pmu_reboot_reason());

    #if RF_CARRIER_OPEN
    rf_carrier_enable(true, FREQ_NUM);
    while (1);
    #endif
    receive_rf_packet_test();

    while (1);
}

/** @} */

// vim: fdm=marker
