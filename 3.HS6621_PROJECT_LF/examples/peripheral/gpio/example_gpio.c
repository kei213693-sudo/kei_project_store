/**
 * @file example_gpio.c
 * @brief example for GPIO
 * @date 2017/07/12 17:53:23
 * @author liqiang
 *
 * @defgroup EXAMPLE_GPIO GPIO Example
 * @ingroup EXAMPLE
 * @brief example for GPIO
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"


/*********************************************************************
 * MACROS
 */
#define BUTTON_PIN  24
#define LEDS_PIN    8

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/**
 * @brief button_handler()
 *
 * @param[in] pin_mask  
 *
 * @return 
 **/
static void button_handler(uint32_t pin_mask)
{
    log_debug("button: %08X\n", pin_mask);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_gpio(void)
{
    // Pinmux
    pinmux_config(BUTTON_PIN, PINMUX_GPIO_MODE_CFG);
    pinmux_config(LEDS_PIN,   PINMUX_GPIO_MODE_CFG);

    // gpio mode
    pmu_pin_mode_set(BITMASK(BUTTON_PIN), PMU_PIN_MODE_PU);
    pmu_pin_mode_set(BITMASK(LEDS_PIN), PMU_PIN_MODE_PP);

    // gpio init
    gpio_open();

    // Button
    gpio_set_direction(BITMASK(BUTTON_PIN), GPIO_INPUT);
    gpio_set_interrupt(BITMASK(BUTTON_PIN), GPIO_BOTH_EDGE);
    gpio_set_interrupt_callback(button_handler);

    // LED
    gpio_write(BITMASK(LEDS_PIN), GPIO_HIGH);
    gpio_set_direction(BITMASK(LEDS_PIN), GPIO_OUTPUT);

    // Toggle
    while(1)
    {
        gpio_toggle(BITMASK(LEDS_PIN));
        co_delay_ms(100);
    }
}

/** @} */

