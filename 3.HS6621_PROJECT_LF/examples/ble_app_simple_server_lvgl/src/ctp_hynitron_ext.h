#ifndef CTP_HYNITRON_EXT_H__
#define CTP_HYNITRON_EXT_H__


#include <stdint.h>
#include "peripheral.h"

#define PIN_TP_RST                      31
#define PIN_TP_SCL                      32
#define PIN_TP_SDA                      33
#define PIN_TP_INT                      30


#define CTP_SET_RESET_PIN_OUTPUT       gpio_set_direction(BITMASK(PIN_TP_RST), GPIO_OUTPUT)
#define CTP_SET_RESET_PIN_HIGH         gpio_write(BITMASK(PIN_TP_RST), GPIO_HIGH)
#define CTP_SET_RESET_PIN_LOW          gpio_write(BITMASK(PIN_TP_RST), GPIO_LOW)

#define CTP_SET_I2C_DATA_OUTPUT        gpio_set_direction_ex(BITMASK(PIN_TP_SDA - 32), GPIO_OUTPUT)
#define CTP_SET_I2C_DATA_INPUT         gpio_set_direction_ex(BITMASK(PIN_TP_SDA - 32), GPIO_INPUT)
#define CTP_SET_I2C_DATA_HIGH          gpio_write_ex(BITMASK(PIN_TP_SDA - 32), GPIO_HIGH)
#define CTP_SET_I2C_DATA_LOW           gpio_write_ex(BITMASK(PIN_TP_SDA - 32), GPIO_LOW)
#define CTP_GET_I2C_DATA_BIT           gpio_read_ex(BITMASK(PIN_TP_SDA - 32))

#define CTP_SET_I2C_CLK_OUTPUT         gpio_set_direction_ex(BITMASK(PIN_TP_SCL - 32), GPIO_OUTPUT)
#define CTP_SET_I2C_CLK_INPUT          gpio_set_direction_ex(BITMASK(PIN_TP_SCL - 32), GPIO_INPUT)
#define CTP_SET_I2C_CLK_HIGH           gpio_write_ex(BITMASK(PIN_TP_SCL - 32), GPIO_HIGH)
#define CTP_SET_I2C_CLK_LOW            gpio_write_ex(BITMASK(PIN_TP_SCL - 32), GPIO_LOW)
#define CTP_GET_I2C_CLK_BIT            gpio_read_ex(BITMASK(PIN_TP_SCL - 32))

#define CTP_GET_INT_BIT()              gpio_read(BITMASK(PIN_TP_INT))



extern void ctp_hynitron_init(void);
extern bool ctp_hynitron_is_pressed(void);
extern void ctp_hynitron_read_pos(int16_t *x, int16_t *y);

extern void cst816s_wakeup(void);
extern void cst816s_enter_sleepmode(void);

#endif /*CTP_HYNITRON_EXT_H__*/


