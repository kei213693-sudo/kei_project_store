//------------------------------------------------------------------------------------
// HS6206_GPIO.h
//------------------------------------------------------------------------------------
// Copyright 2013, HunterSun Electronics Co., Ltd.
// Huiqing, Zheng
// 2014-02-08
//
// Program Description:
//	 GPIO APIs, including gpio32_xxx wrappers
//
//

#ifndef __HS6206_GPIO_H__
#define __HS6206_GPIO_H__

#define GPIO_DIR_OUT 0
#define GPIO_DIR_IN  1

/* gpio number: 0x17 means P1.7 */
#define gpio32_set_dir(gpio, dir)	gpio_set_dir((gpio) >> 4, (gpio) & 0x0f, (dir))
#define gpio32_get_input(gpio)		gpio_get_input((gpio) >> 4, (gpio) & 0x0f)
#define gpio32_set_output(gpio, val)		gpio_set_output((gpio) >> 4, (gpio) & 0x0f, val)

extern void gpio_set_dir(unsigned char port, unsigned char pin, unsigned char dir);
extern unsigned char gpio_get_input(unsigned char port, unsigned char pin);
extern void gpio_set_output(unsigned char port, unsigned char pin, unsigned char output);

#endif  /*__HS6206_GPIO_H__*/

/*-------------------------------------------End Of File---------------------------------------------*/
