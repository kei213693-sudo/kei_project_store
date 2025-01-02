//------------------------------------------------------------------------------------
// HS6206_GPIO.c
//------------------------------------------------------------------------------------
// Copyright 2014, HunterSun Electronics Co., Ltd.
// Huiqing, Zheng
// 2014-02-08
//
// Program Description:
//
//
//

#ifdef HS6206_GPIO

#include "HS6206_Regs.h"
#include <HS6206_Types.h>
#include <gpio.h>
               
void gpio_set_dir(unsigned char port, unsigned char pin, unsigned char dir)
{
    switch (port)
      {
        case 0:
           CLR_GPIO_DIR_PIN(0, pin);
           SET_GPIO_DIR_PIN(0, pin, dir);
	   break;
        case 1:
           CLR_GPIO_DIR_PIN(1, pin);
           SET_GPIO_DIR_PIN(1, pin, dir);
	   break;
        case 2:
           CLR_GPIO_DIR_PIN(2, pin);
           SET_GPIO_DIR_PIN(2, pin, dir);
	   break;
      default:
           CLR_GPIO_DIR_PIN(0, pin);
           SET_GPIO_DIR_PIN(0, pin, dir);
      }
}

unsigned char gpio_get_input(unsigned char port, unsigned char pin)
{
    unsigned char ret_value;
    switch (port)
      {
        case 0:
	   ret_value = GET_GPIO_PIN_CFG(P, 0, pin);
	   break;
        case 1:
	   ret_value = GET_GPIO_PIN_CFG(P, 1, pin);
	   break;
        case 2:
	   ret_value = GET_GPIO_PIN_CFG(P, 2, pin);
	   break;
      default:
	   ret_value = GET_GPIO_PIN_CFG(P, 0, pin);
      }
    return ret_value;
}

void gpio_set_output(unsigned char port, unsigned char pin, unsigned char level)
{
    switch (port)
      {
        case 0:
	   if (! level)
	     P0 &= ~(1 << pin);
	   else
	     P0 |= (1 << pin);
	   break;
        case 1:
	   if (! level)
	     P1 &= ~(1 << pin);
	   else
	     P1 |= (1 << pin);
	   break;
        case 2:
	   if (! level)
	     P2 &= ~(1 << pin);
	   else
	     P2 |= (1 << pin);
	   break;
      default:
	   if (! level)
	     P0 &= ~(1 << pin);
	   else
	     P0 |= (1 << pin);
      }
}

#endif

/*-------------------------------------------End Of File---------------------------------------------*/

