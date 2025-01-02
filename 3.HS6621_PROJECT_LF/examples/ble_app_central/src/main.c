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

#include "app.h"

#include "peripheral.h"
#include "rw_uart.h"
#include "cfg.h"
#include "nvds.h"

/*********************************************************************
 * MACROS
 */
// IO PIN {{{
#ifdef CONFIG_ASIC
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
#else
#define PIN_WAKEUP_0                    3  // GPIO4@FPGA-PeripheralBoard
#define PIN_WAKEUP_1                    4  // KEY4@FPGA-PeripheralBoard (KEY2:RESET)
#define PIN_LED_0                       14
#define PIN_LED_1                       15
#define PIN_LED_2                       16
#define PIN_LED_3                       17
#define PIN_LED_4                       18
#define PIN_LED_5                       19
#define PIN_LED_6                       20
#define PIN_LED_7                       21
#define PIN_UART0_TX                    5
#define PIN_UART0_RX                    6
#endif
// }}}

#define HCI_UART_BAUDRATE               115200//240000 //
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
// Creation of uart external interface api
static rw_hci_uart_t rw_hci_uart;
static uint8_t       rw_hci_uart_pool[512];

static uint32_t pin_wakeup_mask = (BIT_MASK(PIN_WAKEUP_0) | BIT_MASK(PIN_WAKEUP_1));
//static uint32_t pin_leds_mask   = (BIT_MASK(PIN_LED_0)|BIT_MASK(PIN_LED_1)|
//                                   BIT_MASK(PIN_LED_2)|BIT_MASK(PIN_LED_3)|
//                                   BIT_MASK(PIN_LED_4)|BIT_MASK(PIN_LED_5)|
//                                   BIT_MASK(PIN_LED_6)|BIT_MASK(PIN_LED_7));

//static co_timer_t tspp_timer;

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void pinmux_init(void)
{
    // KEY
    pinmux_config(PIN_WAKEUP_0,  PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_WAKEUP_1,  PINMUX_GPIO_MODE_CFG);

    // UART0
    pinmux_config(PIN_UART0_TX, PINMUX_UART0_SDA_O_CFG);
	  pmu_pin_mode_set(BITMASK(PIN_UART0_TX), PMU_PIN_MODE_PP);
	  pmu_pin_mode_set(BITMASK(PIN_UART0_RX), PMU_PIN_MODE_PU);

#if 0
    // LEDS
    pinmux_config(PIN_LED_0, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_1, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_2, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_3, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_4, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_5, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_6, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_7, PINMUX_GPIO_MODE_CFG);
#endif
}

static void peripheral_init(void)
{
    // Init GPIO
    gpio_open();

    // Wakeup Pin
    gpio_set_direction(pin_wakeup_mask, GPIO_INPUT);
    gpio_set_interrupt(pin_wakeup_mask, GPIO_BOTH_EDGE);
//    gpio_set_interrupt_callback(wakeup_gpio_handler);

    // LEDS
    //gpio_write(pin_leds_mask, GPIO_LOW);
    //gpio_set_direction(pin_leds_mask, GPIO_OUTPUT);

    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
	
	  // Init UART
    rw_uart_open(HCI_UART_BAUDRATE, &rw_hci_uart, rw_hci_uart_pool, sizeof(rw_hci_uart_pool));

    // Set Priority, Higher than BB
    //NVIC_SetPriority(UART0_IRQn, 0);
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

    pmu_xtal32m_change_param(8);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(false);
}

//static void tspp_timer_handler(co_timer_t *timer, void *param)
//{
//    static uint32_t counter = 0;
//    log_debug("%d: %d\n", counter++, co_time());
//}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

int main(void)
{
	  ble_stack_config();

    hardware_init();
		
    rwip_init(RESET_NO_ERROR);
	
	  uint8_t addr[6] = {0x01, 0xbf, 0x33, 0x44, 0x55, 0x22};
	  nvds_put(NVDS_TAG_BD_ADDRESS, 6, addr);

	  log_debug("running\n");
	
    appm_init();

//    co_timer_set(&tspp_timer, 1000, TIMER_REPEAT, tspp_timer_handler, NULL);

    cfg_dump(printf);

    while(1)
    {
        rwip_schedule();
    }
}

/** @} */

