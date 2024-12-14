/*
 * app_led.h
 *
 *  Created on: 2023年7月4日
 *	  Abstract: led处理模块，实现单个led灯的“亮一次、快闪、亮起一定时间、常亮”功能，然后根据特定操作码扩展更复杂多样的led灯状态
 */

#ifndef _APP_LED_H_
#define _APP_LED_H_
/*******************************头文件*******************************/
#include "FreeRTOS.h"
#include "task.h"

/*******************************宏定义*******************************/
#define APP_LED_STACK_SIZE                 (128)
#define APP_LED_PRIORITY                   (6)//数字越大，优先级越高
#define APP_LED_QUEUE_LEN                  (4)//队列能够容纳的最大项目数

#define APP_LED_ONCE_TIME                  (600)//闪烁一次时间，单位ms
#define APP_LED_FLICKER_TIME               (300)//快闪间隔，单位ms
#define APP_LED_CONSTANT_TIME              (3000)//常量持续间隔，单位ms
#define APP_LED_PERCENT                    (50)//100//占空比

/*******************************结构体、枚举等******************************/
typedef enum
{//app自定义事件
    APP_LED_E_BUTTON      = 0,//按键事件，红灯亮一次
    APP_LED_E_CHARGE         ,//充电事件，红灯定时闪烁
    APP_LED_E_CHARGE_ERR     ,//充电错误事件，红灯亮起一定时间
    APP_LED_E_BATTERY_FULL   ,//充满事件，红灯常亮
    APP_LED_E_BUTTON_TIMEOUT ,//长按超时事件，红灯熄灭

    APP_LED_E_LEARN_MODE     ,//学习事件，绿灯亮一次
    APP_LED_E_OTA_MODE       ,//ota升级事件，绿灯定时闪烁
    APP_LED_E_RESET_MODE     ,//重置事件，绿灯亮起一定时间
    APP_LED_E_SYS_MODE       ,//系统事件，绿灯常亮

    APP_LED_E_DOUSE          ,//全灭

    APP_LED_E_END                  
}APP_LED_OPT_E;

typedef struct
{
    APP_LED_OPT_E eOpt; //app层自定义操作码
}APP_LED_QUEUE_REC_T;

typedef enum
{//单个led灯状态，可以组合为“闪烁一次、持续闪烁、常亮、常灭”
    APP_LED_STATE_ONCE        = 0,//闪烁一次
    APP_LED_STATE_FLICKER,       //闪烁，次数可选
    APP_LED_STATE_CONSTANT,      //亮起一定时间

    APP_LED_STATE_END                  
}APP_LED_STATE_E;

typedef struct
{//定时处理控制结构体
    APP_LED_STATE_E eLedState;
    uint16_t u16FlickerCount;//快闪次数限制，超过0xff就是一直闪烁不停下,10就是闪烁5次
    uint8_t u8Count;//快闪计数
    uint8_t res;
}app_led_ctl_t;
/*******************************全局变量的外部声明*******************************/
extern TaskHandle_t g_LedTaskHandle;//任务句柄

/*******************************全局函数的声明*******************************/
extern int32_t app_led_init(void);
extern int32_t app_led_deinit(void);
extern int32_t app_led_opt_put(APP_LED_OPT_E eOpt);


#endif /* APPLICATION_LED_APP_LED_H_ */
