/**
 * @file main.c
 * @brief 
 * @date Wed, Sep  5, 2018  5:19:05 PM
 * @author zhiyuan.chen
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
#include "dbg.h"
#include "co.h"
#include "peripheral.h"

#if CONFIG_IR_ENABLE
#include "m_ir.h"

/*********************************************************************
 * MACROS
 */
#define IR_PWM_TIMER_ID           HS_TIM0
#define IR_TIMER_ID               HS_TIM1

#define IR_HEADER_TIMER           9000
#define IR_HEADER_DELAY_TIMER     4500
#define IR_HEADER_REPEAT_TIMER    2250
#define IR_DATA_CARRIER_TIMER     560
#define IR_DATA_1_DELAY_TIMER     1690
#define IR_DATA_0_DELAY_TIMER     560
#define IR_TAIL_TIMER             560
#define IR_PWM_UINT               26.3


#define IR_SEND_REPEAT_TIMER       (110)
/*********************************************************************
 * TYPEDEFS
 */
enum ir_state_type
{
    IR_STATE_IDLE,
    IR_STATE_SEND_HEADER,
    IR_STATE_SEND_PATTERN,
    IR_STATE_SEND_REPEAT,
};

enum ir_sub_state_type
{
    IR_SUB_STATE_CARRIER,
    IR_SUB_STATE_DELAY,
};

struct ir_env_tag
{
    co_timer_t send_repeat_timer;
    uint32_t low_timer;
    uint32_t send_pattern;
    uint32_t send_pattern_pos;
    volatile uint16_t send_carrier_count;
    uint8_t state;
    uint8_t substate;
};

static void ir_pwm_handler(void);
static void ir_timer_us_handler(void);
static void ir_send_repeat_timer_handler(co_timer_t *timer, void *param);
static void ir_delay_timer_handler(void);

/*********************************************************************
 * CONSTANTS
 */
const tim_config_t ir_pwm_cfg =
{
    .mode = TIM_PWM_MODE,
    .config.pwm =
    {
        .count_freq = 76000,
        .period_count = 2,
        .channel =
        {
            {true, {TIM_PWM_POL_LOW2HIGH, 1}},
            {false, {TIM_PWM_POL_HIGH2LOW, 2}},
            {false, {TIM_PWM_POL_HIGH2LOW, 3}},
            {false, {TIM_PWM_POL_HIGH2LOW, 4}},
        },
        .callback = ir_pwm_handler,
    },
};

const tim_config_t ir_timer_cfg =
{
    .mode = TIM_TIMER_MODE,
    .config.timer =
    {
        .period_us = 16,
        .callback = ir_timer_us_handler,
    },
};

/*********************************************************************
 * LOCAL VARIABLES
 */
static struct ir_env_tag m_ir_env;
/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
 static void ir_send_carrier(uint32_t utime)
{
    m_ir_env.send_carrier_count = utime/26.3;
    tim_start(IR_PWM_TIMER_ID);
}

static void ir_send_pattern(void)
{
    if(m_ir_env.send_pattern_pos ==32) { // send stop
        m_ir_env.send_pattern_pos++;
        ir_send_carrier(IR_TAIL_TIMER);
        m_ir_env.state = IR_STATE_IDLE;
        return;
    }
    
    if(m_ir_env.send_pattern_pos >32) {
        return;
    }
    
    if(m_ir_env.substate == IR_SUB_STATE_CARRIER) {
        ir_send_carrier(IR_DATA_CARRIER_TIMER+IR_PWM_UINT);
        m_ir_env.substate = IR_SUB_STATE_DELAY;
    }
    else
    {
        if(m_ir_env.send_pattern&(1<<m_ir_env.send_pattern_pos))
        {
            //send 1
            m_ir_env.low_timer = IR_DATA_1_DELAY_TIMER/16-2;
            tim_start(IR_TIMER_ID);
        }
        else
        {
            //send 0
            m_ir_env.low_timer = IR_DATA_0_DELAY_TIMER/16-3;
            tim_start(IR_TIMER_ID);
        }
        m_ir_env.send_pattern_pos++;
        m_ir_env.substate = IR_SUB_STATE_CARRIER;
    }
    
}

static void ir_send_repeat_timer_handler(co_timer_t *timer, void *param)
{
    m_ir_env.state = IR_STATE_SEND_REPEAT;
    ir_send_carrier(IR_HEADER_TIMER); //delay 9ms
}


static void ir_delay_timer_handler(void)
{
    switch(m_ir_env.state)
    {
        case IR_STATE_IDLE:
            break;
        case IR_STATE_SEND_HEADER:
            m_ir_env.state = IR_STATE_SEND_PATTERN;
            m_ir_env.send_pattern_pos = 0;
            m_ir_env.substate = IR_SUB_STATE_CARRIER;
            ir_send_pattern();
            break;
        case IR_STATE_SEND_PATTERN:
            ir_send_pattern();
            break;
        case IR_STATE_SEND_REPEAT:
            ir_send_carrier(IR_TAIL_TIMER);
            m_ir_env.state = IR_STATE_IDLE;
            break;
        default:
            break;
    }

}

static void ir_send_carrier_next(void)
{
    switch(m_ir_env.state)
    {
        case IR_STATE_IDLE:
            break;
        case IR_STATE_SEND_HEADER:
            m_ir_env.low_timer = IR_HEADER_DELAY_TIMER/16-2;
            tim_start(IR_TIMER_ID);
            break;
        case IR_STATE_SEND_PATTERN:
            ir_send_pattern();
            break;
        case IR_STATE_SEND_REPEAT:
            m_ir_env.low_timer = IR_HEADER_REPEAT_TIMER/16-2;
            tim_start(IR_TIMER_ID);
            break;
        default:
            break;
    }
}

static void ir_pwm_handler(void)
{
    m_ir_env.send_carrier_count--;
    
    if(m_ir_env.send_carrier_count == 0) {
        tim_stop(IR_PWM_TIMER_ID);
        ir_send_carrier_next();
    }
}

static void ir_timer_us_handler(void)
{
   m_ir_env.low_timer--;
   if(m_ir_env.low_timer == 0) {
       tim_stop(IR_TIMER_ID);
       ir_delay_timer_handler();
   }
   //gpio_toggle(BIT_MASK(4));
}

static void ir_nec_stop(void)
{
    co_timer_del(&m_ir_env.send_repeat_timer);
    tim_stop(IR_PWM_TIMER_ID);
}

static void ir_nec_send(uint8_t keycode)
{
    m_ir_env.send_pattern = ((~ keycode)<<24|keycode<<16 |IR_USER_CODE);
    m_ir_env.send_pattern_pos = 0;

    if (m_ir_env.state != IR_STATE_IDLE)
    {
        ir_nec_stop();
    }
    
    co_timer_set(&m_ir_env.send_repeat_timer, 
                IR_SEND_REPEAT_TIMER, 
                TIMER_REPEAT, 
                ir_send_repeat_timer_handler, NULL);
    
    ir_send_carrier(IR_HEADER_TIMER+IR_PWM_UINT); //delay 9ms
    m_ir_env.state = IR_STATE_SEND_HEADER;
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void ir_init(void)
{
    m_ir_env.send_carrier_count = 0;
    m_ir_env.state = IR_STATE_IDLE;

    // Pinmux        
    pinmux_config(PIN_IR_CONTROL, PINMUX_TIMER0_IO_0_CFG);
}

void ir_send(uint8_t key_code)
{
    pmu_lowpower_prevent(PMU_LP_TIM0);
    pmu_lowpower_prevent(PMU_LP_TIM1);
    
    tim_init();
    // timer init
    tim_config(IR_TIMER_ID, &ir_timer_cfg);
    // PWM Init
    tim_config(IR_PWM_TIMER_ID, &ir_pwm_cfg);
    //tim_start(IR_PWM_TIMER_ID);
    //tim_stop(IR_PWM_TIMER_ID);
    
    if(IR_FORMAT == 1)
    {
        ir_nec_send(key_code);
    }
}

void ir_stop(void)
{
    if(IR_FORMAT == 1)
    {
        ir_nec_stop();
    }
    pmu_lowpower_allow(PMU_LP_TIM0);
    pmu_lowpower_allow(PMU_LP_TIM1);
}

#endif
