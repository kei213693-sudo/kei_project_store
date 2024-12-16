/*
 * bsp_led.h
 *
 *  Created on: 2023年7月4日
 *	  Abstract: led处理模块，初始化io口和pwm的参数，提供pwm波相关操作接口，例如启动、停止、占空比变化
 */

#ifndef _BSP_LED_H_
#define _BSP_LED_H_
/*******************************头文件*******************************/

/*******************************宏定义*******************************/

/*******************************结构体、枚举等******************************/
typedef enum
{//输出通道枚举，决定到底要控制几个led灯
    BSP_LED_NAME_0 = 0,
    BSP_LED_NAME_1,

    BSP_LED_NAME_END
}BSP_LED_NAME_E;

typedef struct
{//led灯列表参数
    BSP_LED_NAME_E eLedName;//led灯通道
    void (*percent_set)(void *);//设置占空比，映射到hw层
    void (*light_on)(void);//点亮，映射到hw层
    void (*light_off)(void);//熄灭，映射到hw层
}BSP_LED_TAB_T;

typedef struct
{//提供给应用层的接口
void (*bsp_led_init)(void);
void (*bsp_led_deinit)(void);
void (*bsp_led_percent_set)(BSP_LED_NAME_E eLedName,uint8_t percent);
void (*bsp_led_light_on)(BSP_LED_NAME_E eLedName);
void (*bsp_led_light_off)(BSP_LED_NAME_E eLedName);
}BSP_LED_API_T;

/*******************************全局变量的外部声明*******************************/
extern const BSP_LED_API_T g_BspLedApi;

/*******************************全局函数的声明*******************************/
void bsp_led_init(void);
void bsp_led_deinit(void);
void bsp_led_percent_set(BSP_LED_NAME_E eLedName,uint8_t percent);
void bsp_led_light_on(BSP_LED_NAME_E eLedName);
void bsp_led_light_off(BSP_LED_NAME_E eLedName);

#endif /* APPLICATION_LED_BSP_LED_H_ */
