//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_typedef.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述底层通用自定义类型，需要根据实际情况判定是否是要增加。
 *      文件调用：
 *                  文件为基础类型，底层模块所有文件均可调用
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
*/

#ifndef HAL_TYPEDEF_H
#define HAL_TYPEDEF_H

/*  Include     Begine      */
#include <stdint.h>
#include "assert.h" 
/*  Include     End         */



/*  Base Type  Begine      */
#define SYS_CLOCK_HZ    80000000
#define SYS_CLOCK_M_HZ  80
#define SYS_XTAL_HZ		16000000
#define SYS_XTAL_M_HZ	16
/**
 * 参数介绍：
 *          操作结果返回
 * 关联参数：
 *          参考后续宏定义中的错误码
 * 备    注：
 * 
*/
typedef int8_t        hal_result_t;



/*  Base Type  End         */



/*  Macro       Begine      */

/**
 * 参数介绍：
 *          宏开关状态组
 * 关联参数：
 *          
 * 备    注：
 *          值                  名称                        描述
 *          0               HAL_STD_OFF                     关闭
 *          1               HAL_STD_ON                      打开
*/
#define HAL_STD_ON          1
#define HAL_STD_OFF         0

/**
 * 参数介绍：
 *          错误码组
 * 关联参数：
 *          
 * 备    注：如下没有开发者需要的错误码可根据下面规则酌情增加
 *          值                  名称                        描述
 *          0               HAL_RESULT_SUCCESS             无错误，处理结果成功
 *          -1              HAL_RESULT_FAIL                操作未知错误
 *          -2              HAL_RESULT_BUSY                系统繁忙中,可稍后重试
 *          -3              HAL_RESULT_FIFO_FULL           FIFO已满
 *          -4              HAL_RESULT_NO_SUPPORT          操作不支持
 *          -5              HAL_RESULT_PARAMETER_ERR       参数错误
 *          -6              HAL_RESULT_CONDITION_ERR       条件错误
 *          -7              HAL_RESULT_UNINIT              模块未初始化
 *          -8              HAL_RESULT_LOCK                功能锁定
 *          -9              HAL_RESULT_NULL                指针为空
 *          -10             HAL_RESULT_BLANK_CHK_FAIL      空白检查失败
*/
 #define   HAL_RESULT_SUCCESS                   ( 0)
 #define   HAL_RESULT_FAIL                      (-1)
 #define   HAL_RESULT_BUSY                      (-2)
 #define   HAL_RESULT_FIFO_FULL                 (-3)
 #define   HAL_RESULT_NO_SUPPORT                (-4)
 #define   HAL_RESULT_PARAMETER_ERR             (-5)
 #define   HAL_RESULT_CONDITION_ERR             (-6)
 #define   HAL_RESULT_UNINIT                    (-7)
 #define   HAL_RESULT_LOCK                      (-8)
 #define   HAL_RESULT_NULL                      (-9)
/*  Macro       End         */



/*  Enum        Begine      */

/**
 * 参数介绍：
 *          使能状态
 * 关联参数：
 *          
 * 备    注：
 *          值                  名称                        描述
 *          0           HAL_ENABLED_DISABLE                未使能
 *          1           HAL_ENABLED_ENABLE                  使能
*/
typedef enum{
    HAL_ENABLED_DISABLE     =   0,
    HAL_ENABLED_ENABLE
}hal_enabled_et;

/**
 * 参数介绍：
 *          设置状态
 * 关联参数：
 *          
 * 备    注：
 *          值                  名称                        描述
 *          0           HAL_SET_RESET                       未设置
 *          1           HAL_SET                               设置
*/
typedef enum{
    HAL_SET_RESET       =   0,
    HAL_SET             
}hal_set_et;

/**
 * 参数介绍：
 *          中断优先级，目前设定至少分为四个等级，尾坠值越小，优先级越高
 * 关联参数：
 *          
 * 备    注：
 *          值                  名称                        描述
 *          0           HAL_IRQ_PRIORTY_0                   优先级0
 *          1           HAL_IRQ_PRIORTY_1                   优先级1
 *          2           HAL_IRQ_PRIORTY_2                   优先级2
 *          3           HAL_IRQ_PRIORTY_3                   优先级3
*/
typedef enum
{
    HAL_IRQ_PRIORTY_0 = 0,
    HAL_IRQ_PRIORTY_1,
    HAL_IRQ_PRIORTY_2,
    HAL_IRQ_PRIORTY_3,
    HAL_IRQ_PRIORTY_4,
    HAL_IRQ_PRIORTY_5,
    HAL_IRQ_PRIORTY_6,
    HAL_IRQ_PRIORTY_7,
    HAL_IRQ_PRIORTY_8,
    HAL_IRQ_PRIORTY_9,
    HAL_IRQ_PRIORTY_10,
	HAL_IRQ_PRIORTY_MAX
} hal_irq_priorty_et;

typedef enum
{
    HAL_BIT_ORDER_LSB,
    HAL_BIT_ORDER_MSB
}hal_bit_order_et;
/*  Enum        End         */



/*  Struct/union Begine      */

/*  Struct/union End         */

#endif
