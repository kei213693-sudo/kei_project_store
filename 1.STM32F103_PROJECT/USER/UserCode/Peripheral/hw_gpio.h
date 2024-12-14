/*
 * hw_gpio.h
 *
 *  Created on: 2024年12月6日
 *	  Abstract: 用于stm32f103的通用gpio口配置（输入/输出）；stm32f103中，io外部中断线是0~15
 */

#ifndef _HW_GPIO_H_
#define _HW_GPIO_H_
/*******************************头文件*******************************/
#include "sys.h"

/*******************************宏定义*******************************/ 
//按键GPIO参数
#define HW_GPIO_INDEX_BUTTON_0        GPIO_PIN_5//引脚索引
#define HW_GPIO_TYPE_BUTTON_0         GPIOC//引脚组/类型
#define HW_GPIO_LEVEL_BUTTON_0        HAL_GPIO_ReadPin(HW_GPIO_TYPE_BUTTON_0,HW_GPIO_INDEX_BUTTON_0)//KEY0按键PC5

#define HW_GPIO_INDEX_BUTTON_1        GPIO_PIN_15//引脚索引
#define HW_GPIO_TYPE_BUTTON_1         GPIOA//引脚组/类型
#define HW_GPIO_LEVEL_BUTTON_1        HAL_GPIO_ReadPin(HW_GPIO_TYPE_BUTTON_1,HW_GPIO_INDEX_BUTTON_1)//KEY1按键PA15

#define HW_GPIO_INDEX_BUTTON_2        GPIO_PIN_0//引脚索引
#define HW_GPIO_TYPE_BUTTON_2         GPIOA//引脚组/类型
#define HW_GPIO_LEVEL_BUTTON_2        HAL_GPIO_ReadPin(HW_GPIO_TYPE_BUTTON_2,HW_GPIO_INDEX_BUTTON_2)//WKUP按键PA0

//LED灯GPIO参数
#define HW_GPIO_INDEX_LED_0           GPIO_PIN_8//引脚索引
#define HW_GPIO_TYPE_LED_0            GPIOA//引脚组/类型

#define HW_GPIO_INDEX_LED_1           GPIO_PIN_2//引脚索引
#define HW_GPIO_TYPE_LED_1            GPIOD//引脚组/类型

#define HW_GPIO_PD2_ENBLE             0//这里要注意，atk_ble03模组串口通信用到了PD2做RX，所以使用ble模组的时候需要关闭该引脚的使用

/*******************************结构体、枚举等******************************/

/*******************************全局变量的外部声明*******************************/

/*******************************全局函数的声明*******************************/
extern void hw_gpio_button_init(void);
extern void hw_gpio_button_deinit(void);

extern void hw_gpio_led_init(void);
extern void hw_gpio_led_deinit(void);
void hw_gpio_led1_reset(void);
void hw_gpio_led1_set(void);

#endif /* APPLICATION_PERIPHERAL_GPIO_BSP_GPIO_H_ */
