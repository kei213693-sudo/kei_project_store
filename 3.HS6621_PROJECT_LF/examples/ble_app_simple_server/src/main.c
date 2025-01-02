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
#include "app.h"

/*********************************************************************
 * MACROS
 */

#define PIN_WAKEUP_0                    2   // UART-RTS@EVB
#define PIN_WAKEUP_1                    24  // KEY1@EVB
#define PIN_LED_0                       8
#define PIN_LED_1                       9
#define PIN_LED_2                       23
#define PIN_LED_3                       25
#define PIN_LED_4                       13
#define PIN_LED_5                       14
#define PIN_LED_6                       14
#define PIN_LED_7                       14
#define PIN_UART0_TX                    7
#define PIN_UART1_TX                    5
#define PIN_UART1_RX                    6

#define HCI_UART_BAUDRATE               115200
#define DEBUG_UART_BAUDRATE             115200

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
static uint32_t pin_leds_mask   = (BIT_MASK(PIN_LED_0)|BIT_MASK(PIN_LED_1)|
                                   BIT_MASK(PIN_LED_2)|BIT_MASK(PIN_LED_3)|
                                   BIT_MASK(PIN_LED_4)|BIT_MASK(PIN_LED_5)|
                                   BIT_MASK(PIN_LED_6)|BIT_MASK(PIN_LED_7));

static co_timer_t simple_timer;

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void gpio_handler(uint32_t pin_mask)
{
    static int counter = 0;
    log_debug("gpio: %08X [%d]\n", pin_mask, counter++);
}

static void wakeup_gpio_handler(uint32_t pin_mask)
{
    static int counter = 0;
    log_debug("wake_gpio: %08X [%d]\n", pin_mask, counter++);
}

static void pinmux_init(void)
{
    // KEY
    pinmux_config(PIN_WAKEUP_0,  PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_WAKEUP_1,  PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(pin_wakeup_mask, PMU_PIN_MODE_PU);

    // UART0
    pinmux_config(PIN_UART0_TX, PINMUX_UART0_SDA_O_CFG);
    // UART1
    pinmux_config(PIN_UART1_TX,  PINMUX_UART1_SDA_O_CFG);
    pinmux_config(PIN_UART1_RX,  PINMUX_UART1_SDA_I_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART1_TX), PMU_PIN_MODE_PP);
    pmu_pin_mode_set(BITMASK(PIN_UART1_TX), PMU_PIN_MODE_PP);
    pmu_pin_mode_set(BITMASK(PIN_UART1_RX), PMU_PIN_MODE_PU);

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
    switch(sleep_state)
    {
        case POWER_SLEEP_ENTRY:
            break;

        case POWER_SLEEP_LEAVE_TOP_HALF:
            peripheral_restore();
            break;

        case POWER_SLEEP_LEAVE_BOTTOM_HALF:
            break;
    }
}

static void simple_timer_handler(co_timer_t *timer, void *param)
{
    //sysdump();
}

static void ble_stack_config(void)
{
    // Disable WDT
    wdt_enable(0);

    // Enable DCDC
    pmu_dcdc_enable(true);

    // Power down xtal32k in deep sleep mode
    pmu_32k_enable_in_deep_sleep(false);

    // Select 32k clock for stack
    //pmu_xtal32k_change_param(15, 1);
    pmu_select_32k(PMU_32K_SEL_RC);

    // xtal32m param
    pmu_xtal32m_change_param(15);

    // ultra sleep mode enable
    co_power_ultra_sleep_mode_enable(true);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(true);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

int main(void)
{
    ble_stack_config();

    hardware_init();

    rwip_init(RESET_NO_ERROR);

    co_power_register_sleep_event(power_sleep_event_handler);

    log_debug("running %d\n", pmu_reboot_reason());

    appm_init();

    co_timer_set(&simple_timer, 100, TIMER_REPEAT, simple_timer_handler, NULL);

    while(1)
    {
        rwip_schedule();
    }
}

/** @} */

// vim: fdm=marker
