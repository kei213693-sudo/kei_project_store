/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_tim.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述tim模块配置接口，不建议修改。
 *      文件调用：
 *                  文件应被hal_tim.c调用。
 *                  文件应包含hal_tim_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */
#ifndef HAL_TIM_CFG_H
#define HAL_TIM_CFG_H

/*  Include     Begine     */
#include "hal_tim_typedef.h"
/*  Include     End     */

/*  Macro       Begine      */

/*
 *本宏定义是对实质TIM通道的映射
 *后续方便初始化或设置相关参数
 */
#define HAL_TIM_CHANNEL0     (0U)
#define HAL_TIM_CHANNEL1		(1U)
#define HAL_TIM_CHANNEL2     (0U)
#define HAL_TIM_CHANNEL3		(1U)
/*支持TIM的通道数*/
#define HAL_TIM_CHANNEL_NUM     (4U)
/**
 * 参数介绍：
 *          打开按ms计时的宏开关
 * 关联参数：
 *
 * 备    注：
 *          默认为按频率设置，此开关功能打开后需要模块实现对应的ms转频率换算
 *
 */
#define HAL_TIM_CLOCK_MS_DEFINE HAL_STD_OFF

/*  Macro       End         */

/* Typedef Begine */



/**
 * TIM使能类型枚举
 * HAL_TIM_DISABLE        不使能
 * HAL_TIM_ENABLE         使能
 */
typedef enum
{
    HAL_TIM_DISABLE = 0,
    HAL_TIM_ENABLE
}hal_tim_enabled_et;

/**
 * TIM优先级类型枚举，数值大小和优先级关系和具体芯片相关
 * HAL_TIM_0_PRIORTY        优先级0
 * HAL_TIM_1_PRIORTY        优先级1
 * HAL_TIM_MAX_PRIORTY      优先级最大值限制
 */
//typedef enum
//{
//    HAL_TIM_0_PRIORTY = 0,
//    HAL_TIM_1_PRIORTY,
//    HAL_TIM_MAX_PRIORTY
//}hal_tim_priorty_et;

/**
 * TIM中断类型枚举
 * 本枚举是对实质TIM通道的映射
 * 后续方便初始化或设置相关参数
 * HAL_TIM_0_IRQ_TYPE           TIM 内部触发 0
 * HAL_TIM_1_IRQ_TYPE           TIM 内部触发 1
 * HAL_TIM_2_IRQ_TYPE           TIM 内部触发 2
 * HAL_TIM_3_IRQ_TYPE           TIM 内部触发 3
 * HAL_TIM_MAX_IRQ_TYPE         中断类型最大值限制
 */
typedef enum
{
    HAL_TIM_0_IRQ_TYPE = 0,
    HAL_TIM_1_IRQ_TYPE,
    HAL_TIM_2_IRQ_TYPE,
    HAL_TIM_3_IRQ_TYPE,
    HAL_TIM_MAX_IRQ_TYPE
}hal_tim_irq_type_et;

/**
 * TIM初始化参数配置
 * m_32_tim_hz                  频率，根据频率计算所需要延时的时间
 * m_tim_priorty_e              优先级
 * m_tim_irq_type_e             中断类型，选择是否配置中断，如果需要配置中断，自行添加中断函数
 * m_tim_enabled_e              使能功能
 */
typedef struct
{
    uint32_t            m_32_tim_hz;
    hal_irq_priorty_et  m_tim_priorty_e;
    hal_tim_irq_type_et m_tim_irq_type_e;
    hal_enabled_et      m_tim_enabled_e;
} hal_tim_cfg_st;

/* Typedef End */

/*  Function    Begine     */
/**
 * 函 数 介 绍 ：
 *                 获得TIM配置表函数
 *
 *  * 参       数
 *                 timcfg：配置表指针
 * 					len：：长度
 * 返   回  值
 * 备       注 ：
 *
 */
void hal_tim_get_cfg(hal_tim_cfg_st **timcfg, uint32_t *len);
/*  Function    End     */

#endif
