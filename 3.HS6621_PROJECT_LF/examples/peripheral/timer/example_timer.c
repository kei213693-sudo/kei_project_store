/**
 * @file example_timer.c
 * @brief example for timer
 * @date 2017/07/12 16:10:54
 * @author liqiang
 *
 * @defgroup EXAMPLE_TIMER Timer Example
 * @ingroup EXAMPLE
 * @brief example for timer
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
 * @brief tim_timer_handler()
 *
 * @return 
 **/
void tim_timer_handler(void)
{
    log_debug("tick\n");
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void example_timer(void)
{
    const tim_config_t timer_cfg =
    {
        .mode = TIM_TIMER_MODE,
        .config.timer =
        {
            .period_us = 1000*1000,
            .callback = tim_timer_handler,
        },
    };

    // Timer init
    tim_init();
    tim_config(HS_TIM0, &timer_cfg);
    tim_start(HS_TIM0);

    while(1);
}

/** @} */

