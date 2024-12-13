/*
 * task_button.h
 *
 *  Created on: 2023年5月30日
 *	  Abstract: button处理模块，将bsp传递的单个按键操作扩展为更复杂的应用，例如“组合按键（双击、三击、复合）、顺序按键”等操作
*/

#ifndef _APP_BUTTON_H_
#define _APP_BUTTON_H_
/*******************************头文件*******************************/
#include "FreeRTOS.h"
#include "task.h"

/*******************************宏定义*******************************/
#define APP_BUTTON_STACK_DEPTH                 (192)//
#define APP_BUTTON_PRIORITY                    (6)//数字越大，优先级越高
#define APP_BUTTON_QUEUE_LEN                   (4)//队列能够容纳的最大项目数

#define APP_BUTTON_NAME_TAB_MAX                4//单次操作，存储按键值最大上限

#define APP_BUTTON_CONTINUOUS_TIME             500//连按限制时间，ms
#define APP_BUTTON_CONTINUOUS_NUM              3//连按限制次数

#define APP_BUTTON_COMBINATION_TIME            4*1000//顺序按键限制时间，ms

#define APP_BUTTON_COMBINATION_UPDATA_COUNT    4//ota升级事件要判定的按键数，0、1、2、0
#define APP_BUTTON_COMBINATION_RESET_COUNT     3//初始化事件要判定的按键数，2、0、1
//后续有什么组合按键就依次扩展

/*******************************结构体、枚举等******************************/

typedef enum
{//app自定义按键事件
    //获取按键事件
    APP_BUTTON_E_GET = 1,
    //单击事件
    APP_BUTTON_E_SINGLE_0,
    APP_BUTTON_E_SINGLE_1,
    APP_BUTTON_E_SINGLE_2,
    APP_BUTTON_E_SINGLE_3,
    //长按事件
    APP_BUTTON_E_LONG_0,
    APP_BUTTON_E_LONG_1,
    APP_BUTTON_E_LONG_2,
    APP_BUTTON_E_LONG_3,
    //组合按键——双击事件
    APP_BUTTON_E_DOUBLE_0,
    APP_BUTTON_E_DOUBLE_1,
    APP_BUTTON_E_DOUBLE_2,
    APP_BUTTON_E_DOUBLE_3,
    //组合按键——三击事件
    APP_BUTTON_E_THREE_0,
    APP_BUTTON_E_THREE_1,
    APP_BUTTON_E_THREE_2,
    APP_BUTTON_E_THREE_3,
    //组合按键——复合事件
    APP_BUTTON_E_COMBINATION_LEARN,
    //顺序按键
    APP_BUTTON_E_COMBINATION_UPDATA,
    APP_BUTTON_E_COMBINATION_RESET,

    APP_BUTTON_E_END
}APP_BUTTON_OPT_E;

typedef struct
{
    APP_BUTTON_OPT_E eOpt; //app层自定义操作码
}APP_BUTTON_QUEUE_REC_T;

typedef struct
{//按键模块控制结构体
    APP_BUTTON_OPT_E eButtonOpt;//按键事件opt
	uint32_t u32ButtonState;//存储按键数值，每bit代表一个按键，通过键值BSP_BUTTON_NAME_E获取，用于组合按键处理
    uint8_t u8ButtonNameTab[APP_BUTTON_NAME_TAB_MAX];//依次存储连按的键值
    uint8_t u8Count;//存储连按的次数，也是u8ButtonNameTab[]的索引
    uint8_t u8CombFlag;//启动顺序按键标志
    uint8_t res[2];
}app_button_ctl_t;

/*******************************全局变量的外部声明*******************************/
extern TaskHandle_t g_ButtonTaskHandle;//任务句柄

/*******************************全局函数的声明*******************************/
extern int32_t app_button_init(void);
extern int32_t app_button_deinit(void);
extern int32_t app_button_opt_put(APP_BUTTON_OPT_E eOpt);

#endif
