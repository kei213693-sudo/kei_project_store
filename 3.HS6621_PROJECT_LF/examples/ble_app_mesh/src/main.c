/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief BLE mesh main.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"
#include "rwip_config.h"
#include "mesh_log.h"
#include "rwip.h"
#include "app.h"

#if APP_MESH_TMALL
#include "tmall_led_demo.h"
#endif

#if APP_MESH_TUYA
#include "app_tuya.h"
#endif

#define M_DBG_TIMER		0

#if M_DBG_TIMER
#include "sysdump.h"
co_timer_t dbg_timer;
#endif

/*********************************************************************
 * MACROS
 */
#define PIN_UART0_TX            (5)
#define DEF_UART_BAUDRATE       (115200)

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
/// BLE Config dependent
static void ble_stack_config(void)
{
    // Disable WDT
    wdt_enable(0);

    // Enable DCDC
    pmu_dcdc_enable(true);

    // Power down xtal32k in deep sleep mode
    pmu_32k_enable_in_deep_sleep(false);

    // Select 32k clock for stack
    pmu_select_32k(PMU_32K_SEL_RC);

    // xtal32m param
    pmu_xtal32m_change_param(19);

    // ultra sleep mode enable
    co_power_ultra_sleep_mode_enable(false);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(false);
}

/// Peripheral
void peripheral_init(void)
{
    // Enable 64M
    //pmu_xtal32m_x2_startup();

    // Init GPIO
    gpio_open();

    // Set pin mode
    pmu_pin_mode_set(0xFFFFFFFF, PMU_PIN_MODE_PU);

#ifndef CONFIG_GPIO1_ABSENT
    pmu_pin_mode_set_ex(0xFF, PMU_PIN_MODE_PU);
#endif

    // UART0
    pinmux_config(PIN_UART0_TX, PINMUX_UART0_SDA_O_CFG);
    uart_open(HS_UART0, DEF_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
}

#if M_DBG_TIMER
static void dbg_timer_handler(co_timer_t *timer, void *param)
{
    sch_arb_dump(printf);
}
#endif
/*********************************************************************
 * PUBLIC FUNCTIONS
 */

int main(void)
{
    peripheral_init();

    ble_stack_config();

    //mesh_log_init(0xffffffff, LOG_LEVEL_DBG2);

    rwip_init(RESET_NO_ERROR);
    log_debug("runing\r\n");
    appm_init();

#if APP_MESH_TMALL
    tmall_app_init();
#endif

#if APP_MESH_TUYA
    tuya_app_init();
#endif

#if M_DBG_TIMER
    co_timer_set(&dbg_timer, 4000, TIMER_REPEAT, dbg_timer_handler, NULL);
#endif

    __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "Mesh App Running...\n");

    while(1)
    {
        rwip_schedule();
    }
}

/** @} */

