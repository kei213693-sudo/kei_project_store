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
#define IR_HEADER_TIMER           9000
#define IR_HEADER_DELAY_TIMER     4500
#define IR_HEADER_REPEAT_TIMER    2250
#define IR_DATA_CARRIER_TIMER     560
#define IR_DATA_1_DELAY_TIMER     1690
#define IR_DATA_0_DELAY_TIMER     560
#define IR_TAIL_TIMER             560


#define IR_SEND_REPEAT_TIMER       110
/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */
static co_timer_t ir_send_repeat_timer;;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void ir_delay_timer(uint32_t timer)
{
    uint32_t st=timer;
    switch(timer)
    {
        case IR_DATA_0_DELAY_TIMER:
            st = 485;
            break;
        case IR_DATA_1_DELAY_TIMER:
            st = 1486;
            break;
        case IR_HEADER_REPEAT_TIMER:
            st = 2000;
            break;
        case IR_HEADER_DELAY_TIMER:
            st = 3986;
            break;
       default:
            st = st*6/13;
            break;
    }
   
    co_delay_us(st);
}

static void ir_send_carrier(uint32_t utime)
{
    uint32_t icount = 0;
    uint32_t unum = utime*10/264; // 1/38k
       
    while (1)
    {
        if (icount == unum)
            break;
 
        gpio_write(BIT_MASK(PIN_IR_CONTROL), GPIO_HIGH);
        __NOP();
        co_delay_us(6); //delay 13us
        gpio_write(BIT_MASK(PIN_IR_CONTROL), GPIO_LOW);
        co_delay_us(6); // delay 13.4us
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        icount++;
 
    }
    gpio_write(BIT_MASK(PIN_IR_CONTROL), GPIO_LOW);
}

static void ir_send_bit(uint8_t byte)
{
    ir_send_carrier(IR_DATA_CARRIER_TIMER); 
 
	ir_delay_timer(byte == 0x00 ? (IR_DATA_0_DELAY_TIMER) : (IR_DATA_1_DELAY_TIMER));
}

static void ir_send_byte(uint8_t u8keydata)
{
	uint8_t u8Mask = 0x01;
 
	while (u8Mask)
	{
		ir_send_bit(u8keydata & u8Mask);
		u8Mask <<= 1;
	}
}

static void ir_send_repeat_timer_handler(co_timer_t *timer, void *param)
{
    ir_send_carrier(IR_HEADER_TIMER); //delay 9ms
    ir_delay_timer(IR_HEADER_REPEAT_TIMER);
    ir_send_carrier(IR_TAIL_TIMER);
}

static void ir_nec_send(uint8_t keycode)
{
    uint8_t u8datOpposite = 0;
    u8datOpposite =~ keycode;

    co_timer_set(&ir_send_repeat_timer, IR_SEND_REPEAT_TIMER, TIMER_REPEAT, ir_send_repeat_timer_handler, NULL);
    
    ir_send_carrier(IR_HEADER_TIMER); //delay 9ms

    ir_delay_timer(IR_HEADER_DELAY_TIMER); //delay 4.5 ms

    ir_send_byte((uint8_t)(IR_USER_CODE&0x00FF));
    ir_send_byte((uint8_t)((IR_USER_CODE&0xFF00)>>8));

    ir_send_byte(keycode);
    ir_send_byte(u8datOpposite);
    ir_send_carrier(IR_TAIL_TIMER);
}

static void ir_nec_stop(void)
{
    co_timer_del(&ir_send_repeat_timer);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
void ir_init(void)
{
    pinmux_config(PIN_IR_CONTROL, PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(BIT_MASK(PIN_IR_CONTROL), PMU_PIN_MODE_PP);

    gpio_set_direction(BIT_MASK(PIN_IR_CONTROL), GPIO_OUTPUT);
    gpio_write(BIT_MASK(PIN_IR_CONTROL), GPIO_LOW);
}

void ir_send(uint8_t key_code)
{
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
}

#endif
