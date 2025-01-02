#ifndef __M_LED_H__
#define __M_LED_H__

void led_init(void);
void led_on(void);
void led_off(void);
void led_on_duration(uint16_t timeout_ms);
void led_blink_start(uint16_t interval_ms);
void led_blink_stop(void);
void led_blink_duration(uint16_t interval_ms, uint16_t timeout_ms);
void led_blink_count(uint16_t interval_ms, uint16_t count);

#endif
