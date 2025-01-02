/**
 * @file example_pwm.c
 * @brief example for PWM
 * @date 2017/07/12 16:10:54
 * @author liqiang
 *
 * @defgroup EXAMPLE_PWM PWM Example
 * @ingroup EXAMPLE
 * @brief example for PWM
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
 * @brief tim_pwm_handler()
 *
 * @return 
 **/
void tim_pwm_handler(void)
{
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void example_pwm(void)
{
    const tim_config_t pwm_cfg =
    {
        .mode = TIM_PWM_MODE,
        .config.pwm =
        {
            // pwm_frequency = count_freq/period_count (2000Hz)
            .count_freq = 10000,
            .period_count = 5,
            .channel =
            {
                // duty cycle = pulse_count/period_count
                {true/*enable*/, {TIM_PWM_POL_HIGH2LOW, 1}}, // channel0: duty cycle = 1/5
                {true/*enable*/, {TIM_PWM_POL_HIGH2LOW, 2}}, // channel1: duty cycle = 2/5
                {true/*enable*/, {TIM_PWM_POL_HIGH2LOW, 3}}, // channel2: duty cycle = 3/5
                {true/*enable*/, {TIM_PWM_POL_HIGH2LOW, 4}}, // channel3: duty cycle = 4/5
            },
            .callback = tim_pwm_handler,
        },
    };

    // Pinmux
    pinmux_config(8, PINMUX_TIMER1_IO_0_CFG);
    pinmux_config(9, PINMUX_TIMER1_IO_1_CFG);
    pinmux_config(10, PINMUX_TIMER1_IO_2_CFG);
    pinmux_config(25, PINMUX_TIMER1_IO_3_CFG);

    // PWM Init
    tim_init();
    tim_config(HS_TIM1, &pwm_cfg);
    tim_start(HS_TIM1);
    while(1);
}

/** @} */

