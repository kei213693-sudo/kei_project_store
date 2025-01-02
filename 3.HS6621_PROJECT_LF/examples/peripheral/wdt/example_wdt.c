/**
 * @file example_wdt.c
 * @brief example for WDT
 * @date 2017/07/12 16:10:54
 * @author liqiang
 *
 * @defgroup EXAMPLE_WDT WDT Example
 * @ingroup EXAMPLE
 * @brief example for WDT
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
static co_timer_t simple_timer;

/**
 * @brief  simple timer handler
 *
 * @param[in] timer  timer
 * @param[in] param  param
 **/
static void simple_timer_handler(co_timer_t *timer, void *param)
{
    wdt_keepalive();
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void example_wdt(void)
{
    // Enable WDT with 10s
    wdt_enable(10);

    // Keep alive timer
    co_timer_set(&simple_timer, 5000, TIMER_REPEAT, simple_timer_handler, NULL);
}

/** @} */

