/*
 * bsp_gpio.h
 *
 *  Created on: 2023年6月21日
 *	  Abstract: button处理模块，初始化io口参数，传递某一个按键操作（短按、长按、长按超时）传递给app层
 */

#ifndef BSP_BUTTON_H_
#define BSP_BUTTON_H_
/*******************************头文件*******************************/
#include "timer.h"

/*******************************宏定义*******************************/
#define BSP_BUTTON_ELIMINATE_COUNT ((50*1000)/TIMR_TIM3_BASE_US)//消抖时间，50ms

#define BSP_BUTTON_LONG_ENBLE      (0)//长按使能标志
#if (1 == BSP_BUTTON_LONG_ENBLE)
#define BSP_BUTTON_LONG_COUNT_MIN  ((3*1000*1000)/TIMR_TIM3_BASE_US)//长按计数,最小时间数值3s
#define BSP_BUTTON_LONG_COUNT_MAX  ((10*1000*1000)/TIMR_TIM3_BASE_US)//长按超时计数,最大时间数值10s
#endif
/*******************************结构体、枚举等******************************/
typedef void (*appButtonCB_t)(void *);

typedef enum
{//按键名称，也是按键键值，用一个bit表示，u32能够表现32个io按键数值
    BSP_BUTTON_NAME_0            = 0x00000001,
    BSP_BUTTON_NAME_1            = 0x00000002,
    BSP_BUTTON_NAME_2            = 0x00000004,
    BSP_BUTTON_NAME_3            = 0x00000008,

    BSP_BUTTON_NAME_END
}BSP_BUTTON_NAME_E;

typedef enum
{//按键类型
    //长按事件通过宏定义BSP_BUTTON_LONG_ENBLE启用，兼容短按事件
    //，开启后不是超过消抖时间就传递短按事件，而是松开后再传递，这里要注意
    BSP_BUTTON_TYPE_SHORT          = 0,//短按事件，过了消抖就传递
    BSP_BUTTON_TYPE_LONG,              //长按事件，必须持续按下超过一定时间
    BSP_BUTTON_TYPE_TIMEOUT,           //长按超时，持续按下过长

    BSP_BUTTON_TYPE_END
}BSP_BUTTON_TYPE_E;

typedef struct
{//当前按键信息，传递给app层的信息
    BSP_BUTTON_NAME_E eButtonName;//4bytes
    BSP_BUTTON_TYPE_E eButtonType;//区分长按还是短按
}BSP_BUTTON_INFO_T;

typedef struct
{//控制/列表结构体
    BSP_BUTTON_NAME_E eButtonName;//按键名称
    uint16_t u16PinIndex;//gpio索引
    uint8_t u8ActiveLevel;//有效电平，0或1，代表按下时的电平
    //以上三个参数需要初始化阶段配置好然后固定不变，下面两个参数会动态变化
    uint8_t u8Flag;//按键按下标志，1表示被按下了
    uint32_t u32Count;//计数标志，用于消抖、长按等时间判断
}BSP_BUTTON_TAB_T;

typedef struct
{//提供给应用层的接口
    void (*bsp_button_init)(appButtonCB_t pCallback);
    void (*bsp_button_deinit)(void);
    void (*bsp_button_info_get)(BSP_BUTTON_INFO_T *pStData);
}BSP_BUTTON_API_T;


/*******************************全局变量的外部声明*******************************/
extern const BSP_BUTTON_API_T g_BspButtonApi;

/*******************************全局函数的声明*******************************/
extern void bsp_button_init(appButtonCB_t pCallback);
extern void bsp_button_deinit(void);
extern void bsp_button_info_get(BSP_BUTTON_INFO_T *pStData);

extern void bsp_button_handle(uint16_t u16Pin);
extern void bsp_button_IrqCheck(void);



#endif /* APPLICATION_PERIPHERAL_GPIO_BSP_GPIO_H_ */
