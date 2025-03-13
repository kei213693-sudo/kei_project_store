/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_tim_typedef.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述tim模块专用自定义类型，需要根据实际情况判定是否是要需改。
 *      文件调用：
 *                  文件应在模块内部产生直接调用，外部使用需通过hal_tim_if.h 或 hal_tim.h
 *              间接调用。
 *                  文件应包含hal_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
*/

#ifndef HAL_TIM_TYPEDEF_H
#define HAL_TIM_TYPEDEF_H

/*  Include     Begine      */
#include "hal_typedef.h"
/*  Include     End         */


typedef enum
{
	HAL_TIM_UNINIT = 0,
	HAL_TIM_INIT
}hal_tim_initstatus_et;

typedef uint8_t hal_channel_handle_t;

/*  Base Type  Begine      */
/**
 * 参数介绍：
 *          中断回调函数
 * 关联参数：
 *
 * 备    注：
 *          主要用于设置tim中断时的回调函数
 *
*/
typedef void(* hal_tim_cbk_t)(void);

/*  Base Type  End         */
//typedef enum
//{
//    TYPE1,
//    TYPE2
//}hal_tim_irq_type_et;

/*  Macro       Begine      */
/**
 * 参数介绍：
 *          打开按ms计时的宏开关
 * 关联参数：
 *
 * 备    注：
 *          默认为按频率设置，此开关功能打开后需要模块实现对应的ms转频率换算
 *
*/
#define HAL_TIM_CLOCK_MS_DEFINE                    HAL_STD_OFF

/*  Macro       End         */



/*  Enum        Begine      */

/*  Enum        End         */



/*  Struct/union Begine      */

/*  Struct/union End         */

#endif
