/*
 * bsp_led.c
 *
 *  Created on: 2023年7月4日
 *	  Abstract: led处理模块，初始化io口和pwm的参数，提供pwm波相关操作接口，例如启动、停止、占空比变化
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "bsp_led.h"
#include "hw_gpio.h"
#include "timer.h"

/*******************************内部函数的声明*******************************/

/*******************************内部变量定义*******************************/
static BSP_LED_TAB_T g_BspLedTab[] = 
{//led灯数量按照表格扩展
{BSP_LED_NAME_0,    hw_tim_tim1PwmChannel1_compare_set,   hw_tim_tim1PwmChannel1_start,   hw_tim_tim1PwmChannel1_stop},//pwm波控制
{BSP_LED_NAME_1,    NULL,   hw_gpio_led1_reset,   hw_gpio_led1_set}//io高低电平控制
};

/*******************************全局变量定义*******************************/
const BSP_LED_API_T g_BspLedApi =
{
    bsp_led_init,
    bsp_led_deinit,
    bsp_led_percent_set,
    bsp_led_light_on,
    bsp_led_light_off
};

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：led初始化
*函数参数 ：pCallback，应用层回调函数
*函数返回值 ：0表示成功，-1表示失败
*备注：
**************************************************************/
void bsp_led_init(void)
{
    hw_gpio_led_init();//gpio配置
    TIM1_PWM_Init(TIMR_TIM1_PWM_ARR_MAX,0);//pwm配置，不分频。PWM频率=72000/(899+1)=80Khz 
}

/**************************************************************
*函数功能描述 ：led去初始化
*函数参数 ：
*函数返回值 ：0表示成功，-1表示失败
*备注：
**************************************************************/
void bsp_led_deinit(void)
{
    hw_gpio_led_deinit();
    TIM1_PWM_deInit();
}

/**************************************************************
*函数功能描述 ：设定pwm占空比
*函数参数 ：eLedName，led灯的通道
        percent，占空比/百分比，值越大灯越亮
*函数返回值 ：
*备注：
**************************************************************/
void bsp_led_percent_set(BSP_LED_NAME_E eLedName,uint8_t percent)
{
    uint32_t percentNum = 0;//百分比转换实际寄存器的数值

    if((percent < 100) && (eLedName < BSP_LED_NAME_END))
    {
        for(uint8_t i = 0; i < (sizeof(g_BspLedTab)/sizeof(g_BspLedTab[0])); i++)
        {
            if((eLedName == g_BspLedTab[i].eLedName)
                && (NULL != g_BspLedTab[i].percent_set))
            {
                percentNum  = (uint32_t) ((TIMR_TIM1_PWM_ARR_MAX * percent) / 100);//占空比数值计算公式
                g_BspLedTab[i].percent_set((void*)&percentNum);
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：点亮某个led灯
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void bsp_led_light_on(BSP_LED_NAME_E eLedName)
{
    if(eLedName < BSP_LED_NAME_END)
    {
        for(uint8_t i = 0; i < (sizeof(g_BspLedTab)/sizeof(g_BspLedTab[0])); i++)
        {
            if((eLedName == g_BspLedTab[i].eLedName)
                && (NULL != g_BspLedTab[i].light_on))
            {
                g_BspLedTab[i].light_on();
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：熄灭某个led灯
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void bsp_led_light_off(BSP_LED_NAME_E eLedName)
{
    if(eLedName < BSP_LED_NAME_END)
    {
        for(uint8_t i = 0; i < (sizeof(g_BspLedTab)/sizeof(g_BspLedTab[0])); i++)
        {
            if((eLedName == g_BspLedTab[i].eLedName)
                && (NULL != g_BspLedTab[i].light_on))
            {
                g_BspLedTab[i].light_off();
            }
        }
    }
}
