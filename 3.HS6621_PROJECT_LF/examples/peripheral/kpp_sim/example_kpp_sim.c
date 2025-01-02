/**
 * @file example_kpp_sim.c
 * @brief example for KPP
 * @date 2017/07/12 16:10:54
 * @author liqiang
 *
 * @defgroup EXAMPLE_KPP KPP sim Example
 * @ingroup EXAMPLE
 * @brief example for KPP
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/**
 * @brief  kpp test gpio handler
 *
 * @param[in] pin_mask  pin mask
 **/
static void kpp_test_gpio_handler(uint32_t pin_mask)
{
    kpp_sim_gpio_handler(pin_mask);
}

/**
 * @brief  kpp sim event handler
 *
 * @param[in] event  event
 * @param[in] data  data
 **/
static void kpp_sim_event_handler(kpp_sim_event_t event, const kpp_data_t data)
{
    if(event == KPP_SIM_EVENT_DEPRESS)
        log_debug_array_ex("Depress", data, sizeof(kpp_data_t));
    else
        log_debug("Release\n");
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void example_kpp_sim(void)
{
    const kpp_sim_config_t config =
    {
        .row_pin_mask = (1<<14) | (1<<16) | (1<<27) | (1<<29),
        .col_pin_mask = (1<<15) | (1<<17) | (1<<28) | (1<<30),
        .col_output_delay = 10,
        .scan_interval  = 8,
        .callback = kpp_sim_event_handler,
    };

    pinmux_config(14, PINMUX_GPIO_MODE_CFG); // row
    pinmux_config(16, PINMUX_GPIO_MODE_CFG); // row
    pinmux_config(27, PINMUX_GPIO_MODE_CFG); // row
    pinmux_config(29, PINMUX_GPIO_MODE_CFG); // row
    pinmux_config(15, PINMUX_GPIO_MODE_CFG); // col
    pinmux_config(17, PINMUX_GPIO_MODE_CFG); // col
    pinmux_config(28, PINMUX_GPIO_MODE_CFG); // col
    pinmux_config(30, PINMUX_GPIO_MODE_CFG); // col
//    pmu_pin_mode_set((1<<14) | (1<<16) | (1<<27) | (1<<29), PMU_PIN_MODE_PU);
//    pmu_pin_mode_set((1<<15) | (1<<17) | (1<<28) | (1<<30), PMU_PIN_MODE_OD);

    gpio_open();
    gpio_set_interrupt_callback(kpp_test_gpio_handler);

    kpp_sim_config(&config);

    pmu_wakeup_pin_register_callback(kpp_test_gpio_handler, NULL);

    log_debug("kpp sim running\n");
}

/** @} */

