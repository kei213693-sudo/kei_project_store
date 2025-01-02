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
#include "h4tl.h"
#include "peripheral.h"
#include "rw_uart.h"
#include "sysdump.h"

#include "shell.h"
#include "cmd.h"
#include "adc.h"
/*********************************************************************
 * MACROS
 */

#define USE_DEBUG_BUS                  ~0 //SYS_MON_CPM(0) //SYS_MON_PHY(0) //SYS_MON_DAIF(0)

// IO PIN {{{
#ifdef CONFIG_ASIC
#define PIN_WAKEUP_0                    3   // UART-RTS@EVB
#define PIN_WAKEUP_1                    24  // KEY1@EVB
#define PIN_UART0_TX                    5
#define PIN_UART0_RX                    6
#else
#define PIN_WAKEUP_0                    3  // GPIO4@FPGA-PeripheralBoard
#define PIN_WAKEUP_1                    4  // KEY4@FPGA-PeripheralBoard (KEY2:RESET)
#define PIN_UART0_TX                    5
#define PIN_UART0_RX                    6
#endif
// }}}

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

//static uint32_t pin_wakeup_mask = (BIT_MASK(PIN_WAKEUP_0) | BIT_MASK(PIN_WAKEUP_1));

/*
 * Shell commands you wish to use.
 */
static const ShellCommand commands[] = {
    TEST_HAL_CMD
    {NULL, NULL}
};

static const ShellConfig shell_cfg = {
    HS_UART0,
    commands
};

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void uart0_receive_handler(uint8_t data)
{
    app_shell_rx(data);
}

/*static void gpio_handler(uint32_t pin_mask)
{
    static int counter = 0;
    log_debug("gpio: %08X [%d]\n", pin_mask, counter++);
}

static void wakeup_gpio_handler(uint32_t pin_mask)
{
    static int counter = 0;
    log_debug("wake_gpio: %08X [%d]\n", pin_mask, counter++);
}
*/

static void pinmux_init(void)
{
    // KEY
    //pinmux_config(PIN_WAKEUP_0,  PINMUX_GPIO_MODE_CFG);
    //pinmux_config(PIN_WAKEUP_1,  PINMUX_GPIO_MODE_CFG);
    //pmu_pin_mode_set(pin_wakeup_mask, PMU_PIN_MODE_PU);

    // UART0
    pinmux_config(PIN_UART0_TX,  PINMUX_UART0_SDA_O_CFG);
    pinmux_config(PIN_UART0_RX,  PINMUX_UART0_SDA_I_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART0_TX), PMU_PIN_MODE_PP);
    pmu_pin_mode_set(BITMASK(PIN_UART0_TX), PMU_PIN_MODE_PP);
    pmu_pin_mode_set(BITMASK(PIN_UART0_RX), PMU_PIN_MODE_PU);
}

static void peripheral_init(void)
{
    // Init GPIO
    gpio_open();

    // Wakeup Pin
    //gpio_set_direction(pin_wakeup_mask, GPIO_INPUT);
    //gpio_set_interrupt(pin_wakeup_mask, GPIO_BOTH_EDGE);
    //gpio_set_interrupt_callback(gpio_handler);

    // wakeup
    //pmu_wakeup_pin_set(pin_wakeup_mask, PMU_PIN_WAKEUP_LOW_LEVEL);
    //pmu_wakeup_pin_register_callback(wakeup_gpio_handler, wakeup_gpio_handler);

    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, uart0_receive_handler);
}

static void peripheral_restore(void)
{
    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, uart0_receive_handler);
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
    co_power_ultra_sleep_mode_enable(false);

    pmu_xtal32m_change_param(8);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(false);
    //sc.rw_sleep_enable = false;
    
    // TIM need High clock
    pmu_xtal32m_fast_startup();
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void _adc_callback1(adc_event_t event, float adc_data)
{
    switch(event)
    {
        case ADC_EVENT_CH_T_DONE:
            log_debug("temperature = %f\n", adc_data);
            break;
        case ADC_EVENT_CH_V_DONE:
            log_debug("battery = %f\n", adc_data);
            break;
        case ADC_EVENT_CH_0_DONE:
        case ADC_EVENT_CH_1_DONE:
        case ADC_EVENT_CH_2_DONE:
        case ADC_EVENT_CH_3_DONE:
        case ADC_EVENT_CH_4_DONE:
        case ADC_EVENT_CH_5_DONE:
        case ADC_EVENT_CH_6_DONE:
        case ADC_EVENT_CH_7_DONE:
            log_debug("pin = %f\n", adc_data);
            break;
        case ADC_EVENT_DMA_DONE:
            log_debug("dma = %f\n", adc_data);
            break;
        default:
            log_debug("adc = %f\n", adc_data);
            break;
    }
}

int main(void)
{
    #ifdef CONFIG_HS6621
    pmu_pll_startup(96);
    #endif
    ble_stack_config();

    hardware_init();

    rwip_init(RESET_NO_ERROR);
    rwip_reset();

    co_power_register_user_status(power_sleep_status_handler);
    co_power_register_sleep_event(power_sleep_event_handler);

    log_debug("running %d\n", pmu_reboot_reason());
    
    app_shell_init(&shell_cfg);
    while(1)
    {
        rwip_schedule();
    }
}

/** @} */

// vim: fdm=marker
