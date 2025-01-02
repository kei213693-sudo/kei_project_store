#include "rwip_config.h" // RW SW configuration

#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "rwip.h"      // RW SW initialization
#include "dbg.h"
#include "peripheral.h"


#if CONFIG_LED_ENABLE

#define PIN_LED_MASK              BIT_MASK(PIN_LED_0)
static co_timer_t led_blink_timer;
static co_timer_t led_blink_duration_timer;
static co_timer_t led_on_duration_timer;

#define LED_BLINK_TID             &led_blink_timer
#define LED_BLINK_DURATION_TID    &led_blink_duration_timer
#define LED_ON_DURATION_TID       &led_on_duration_timer

static bool led_blink_enable;
static bool led_blink_count_enable;

void led_init(void)
{
    pinmux_config(PIN_LED_0, PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(PIN_LED_MASK, PMU_PIN_MODE_PP);

    gpio_set_direction(PIN_LED_MASK, GPIO_OUTPUT);
    gpio_write(PIN_LED_MASK, ~CONFIG_LED_LIGHT);
}

void led_on(void)
{
    if(led_blink_enable)
        return;
    gpio_write(PIN_LED_MASK, CONFIG_LED_LIGHT);
}

void led_off(void)
{
    if(led_blink_enable)
        return;
    gpio_write(PIN_LED_MASK, ~CONFIG_LED_LIGHT);
}

static void led_on_timer_handler(co_timer_t *timer, void *param)
{
    co_timer_del(LED_ON_DURATION_TID);
    led_off();
}

void led_on_duration(uint16_t timeout_ms)
{
    led_on();
    co_timer_set(LED_ON_DURATION_TID, timeout_ms, TIMER_ONE_SHOT, led_on_timer_handler, NULL);
}

static void led_blink_timer_handler(co_timer_t* timer, void *param)
{
    gpio_toggle(PIN_LED_MASK);
}

void led_blink_start(uint16_t interval_ms)
{
    led_blink_enable = true;
    led_on();
    co_timer_set(LED_BLINK_TID, interval_ms, TIMER_REPEAT, led_blink_timer_handler, NULL);
}

void led_blink_stop(void)
{
    led_blink_enable = false;
    co_timer_del(LED_BLINK_TID);
    led_off();
}

static void led_duration_blink_timer_handler(co_timer_t* timer, void *param)
{
    co_timer_del(LED_BLINK_DURATION_TID);
    led_blink_stop();
    if(led_blink_count_enable)
    {
        led_blink_count_enable = false;
    }
}

void led_blink_duration(uint16_t interval_ms, uint16_t timeout_ms)
{
    led_blink_start(interval_ms);
    co_timer_set(LED_BLINK_DURATION_TID, timeout_ms, TIMER_ONE_SHOT, led_duration_blink_timer_handler, NULL);
}

void led_blink_count(uint16_t interval_ms, uint16_t count)
{ 
    uint16_t timeout_ms = interval_ms*count*2;
    led_blink_count_enable = true;
    led_blink_duration(interval_ms, timeout_ms);
}

#endif
