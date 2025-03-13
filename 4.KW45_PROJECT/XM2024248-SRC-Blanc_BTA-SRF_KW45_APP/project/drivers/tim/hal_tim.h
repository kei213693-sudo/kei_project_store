/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_tim.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述tim模块抽象层接口，不建议修改函数接口。
 *      文件调用：
 *                  文件应被应hal层内部调用。
 *                  文件应包含hal_tim_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */
#ifndef HAL_tim_H
#define HAL_tim_H

/*  Include     Begine      */
#include "hal_tim_typedef.h"
#include "hal_tim_cfg.h"
/*  Include     End         */

/*  Function    Begine      */

/**
 * 函 数 介 绍 ：
 *                timer 初始化，若之前该通道已经完成初始化则直接覆盖之前设置的相关参数
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    endbled  是否使能
 *              [in]    tim_cbk  回调函数
 * 返   回  值 ：hal_result_t 初始化结果，默认均为成功
 * 备       注 ：
 *              需提前设置好tim时钟 hal_tim_clock
 */
hal_result_t hal_tim_init(hal_channel_handle_t channel, hal_enabled_et enabled_e, hal_tim_cbk_t tim_cbk);

/**
 * 函 数 介 绍 ：
 *                设置tim触发频率
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    hz       设置时钟触发频率
 * 返   回  值 ：hal_result_t 触发频率设置结果
 * 备       注 ：
 */
hal_result_t hal_tim_clock(hal_channel_handle_t channel, uint32_t hz);

/**
 * 函 数 介 绍 ：
 *                设置tim触发时间
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    ms       设置时钟触发时间
 * 返   回  值 ：hal_result_t 初始化结果，默认均为成功
 * 备       注 ：如果hal_tim.h 中宏开关 HAL_TIM_CLOCK_MS_DEFINE 处于关闭，则直接返回HAL_RESULT_NO_SUPPORT
 */
#if HAL_TIM_CLOCK_MS_DEFINE
hal_result_t hal_tim_clock_ms(hal_channel_handle_t channel, uint32_t ms);
#else
hal_result_t hal_tim_clock_ms(hal_channel_handle_t channel, uint32_t ms);
#endif

/**
 * 函 数 介 绍 ：
 *               设置tim 使能状态
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    endbled  是否使能
 * 返   回  值 ：hal_result_t 使能状态设置结果
 * 备       注 ：
 */
hal_result_t hal_tim_enabled(hal_channel_handle_t channel, hal_enabled_et enabled_e);

/**
 * 函 数 介 绍 ：
 *               设置tim 回调函数
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    tim_cbk  回调函数
 * 返   回  值 ：hal_result_t 回调函数设置结果
 * 备       注 ：
 *              如果使用RTOS系统，当通道回调函数正在使用中时，设置回调函数应返回HAL_RESULT_BUSY
 */
hal_result_t hal_tim_cbk(hal_channel_handle_t channel, hal_tim_cbk_t tim_cbk);

/**
 * 函 数 介 绍 ：
 *               设置tim 中断参数
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    irq_type 中断类型 参考hal_typedef.h中定义
 *              [in]    priorty 设置中断优先级 参考hal_typedef.h中定义
 *              [in]    endbled  是否使能
 * 返   回  值 ：hal_result_t 回调函数设置结果
 * 备       注 ：
 *              当通道回调函数正在使用中时，设置回调函数应返回HAL_RESULT_BUSY
 */
hal_result_t hal_tim_irq_cfg(hal_channel_handle_t channel, hal_tim_irq_type_et type, hal_irq_priorty_et priorty, hal_enabled_et enabled);

/**
 * 函 数 介 绍 ：
 *               复位tim计数
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 * 返   回  值 ：hal_result_t 回调函数设置结果
 * 备       注 ：
 *
 */
hal_result_t hal_tim_recount(hal_channel_handle_t channel);
/**
 * 函 数 介 绍 ：
 *               tim去初始化函数
 *
 * 参       数 ：
 *
 * 返   回  值 ：
 * 备       注 ：
 *
 */
void hal_tim_deinit(void);

/*  Function    End         */

#endif
