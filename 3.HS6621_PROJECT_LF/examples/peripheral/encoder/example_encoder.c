/**
 * @file example_encoder.c
 * @brief example for encoder
 * @date 2017/07/12 15:54:27
 * @author liqiang
 *
 * @defgroup EXAMPLE_ENCODER Encoder Example
 * @ingroup EXAMPLE
 * @brief example for encoder
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * STATIC FUNCTIONS
 */

static void encoder_event_handler(HS_ENCODER_Type *encoder, uint32_t cnt)
{
    log_debug("encoder: %d\n", cnt);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_encoder(void)
{
    encoder_config_t e;

    // config
    e.a_pin = 23;
    e.b_pin = 24;
    e.clk = ENCODER_CLK_32K;
    e.mode = ENCODER_COUNT_ON_B_EDGE,
    e.pol_a = ENCODER_POL_NORMAL,
    e.pol_b = ENCODER_POL_NORMAL,
    e.counter_max = 256,
    e.callback = encoder_event_handler;

    encoder_config(HS_ENCODER0, &e);

    // start
    encoder_start(HS_ENCODER0);

    while(1);
}

/** @} */

