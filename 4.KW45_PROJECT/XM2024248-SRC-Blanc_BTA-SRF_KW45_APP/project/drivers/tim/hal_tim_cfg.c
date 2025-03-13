/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_tim.c
 * 摘要：
 *      文件概述：
 *                  文件主要描述tim模块配置实现，本文件为模板文件，
 *              实现实体需要自行补充。
 *      文件调用：
 *                  文件应包含hal_tim_cfg.h。
 *
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

/*  Include     Begine      */
#include "hal_tim_cfg.h"
/*  Include     End         */

/**
 * TIM初始化参数配置
 * m_32_tim_hz                  频率，根据频率计算所需要延时的时间
 * m_tim_priorty_e              中断优先优先级
 * m_tim_irq_type_e             中断类型，选择是否配置中断，如果需要配置中断，自行添加中断函数
 * m_tim_enabled_e              使能功能
 */
static hal_tim_cfg_st tim_cfg_st[HAL_TIM_CHANNEL_NUM] =
{
    {
        1000,                   /* 频率，根据频率计算所需要延时的时间*/
        HAL_IRQ_PRIORTY_0,      /*中断优先优先级*/
        HAL_TIM_0_IRQ_TYPE,     /* 中断类型，选择是否配置中断，如果需要配置中断，自行添加中断函数*/
        HAL_ENABLED_DISABLE     /*使能功能*/
    },
		{
        100,                   /* 频率，根据频率计算所需要延时的时间*/
        HAL_IRQ_PRIORTY_0,      /*中断优先优先级*/
        HAL_TIM_0_IRQ_TYPE,     /* 中断类型，选择是否配置中断，如果需要配置中断，自行添加中断函数*/
        HAL_ENABLED_DISABLE     /*使能功能*/
    },
    {
        1000,                   /* 频率，根据频率计算所需要延时的时间*/
        HAL_IRQ_PRIORTY_0,      /*中断优先优先级*/
        HAL_TIM_0_IRQ_TYPE,     /* 中断类型，选择是否配置中断，如果需要配置中断，自行添加中断函数*/
        HAL_ENABLED_DISABLE     /*使能功能*/
    },
		{
        100,                   /* 频率，根据频率计算所需要延时的时间*/
        HAL_IRQ_PRIORTY_0,      /*中断优先优先级*/
        HAL_TIM_0_IRQ_TYPE,     /* 中断类型，选择是否配置中断，如果需要配置中断，自行添加中断函数*/
        HAL_ENABLED_DISABLE     /*使能功能*/
    }
};

/* Private Struct/union End         */

/* Private Function Begine          */

/* Private Function End             */

/* Public Function Begine           */
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
void hal_tim_get_cfg(hal_tim_cfg_st **timcfg, uint32_t *len)
{
      *len = HAL_TIM_CHANNEL_NUM;
      *timcfg = tim_cfg_st;
}
/* Public Function End              */
