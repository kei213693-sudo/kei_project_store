/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_tim_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_tim_if.h"
#include "hal_tim.h"
/**
 * 函 数 介 绍 ：
 *                timer 初始化，若之前该通道已经完成初始化则直接覆盖之前设置的相关参数
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    endbled  是否使能
 *              [in]    tim_cnk  回调函数
 * 返   回  值 ：hal_result_t 初始化结果，默认均为成功
 * 备       注 ：
 *              需提前设置好tim时钟 hal_tim_clock
 */
hal_result_t hal_tim_if_init(hal_channel_handle_t channel, hal_enabled_et enabled, hal_tim_cbk_t tim_cbk)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_tim_init(channel,enabled,tim_cbk);
    return result;
}

/**
 * 函 数 介 绍 ：
 *                设置tim触发频率
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    hz       设置时钟触发频率
 * 返   回  值 ：hal_result_t 初始化结果，默认均为成功
 * 备       注 ：确认hz<1时定义方式
 */
hal_result_t hal_tim_if_clock(hal_channel_handle_t channel, uint32_t hz)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_tim_clock(channel,hz);
    return result;

}

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
hal_result_t hal_tim_if_clock_ms(hal_channel_handle_t channel, uint32_t ms)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_tim_clock_ms(channel,ms);
    return result;
}

/**
 * 函 数 介 绍 ：
 *               设置tim 使能状态
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    endbled  是否使能
 * 返   回  值 ：hal_result_t 初始化结果，默认均为成功
 * 备       注 ：
 */
hal_result_t hal_tim_if_enabled(hal_channel_handle_t channel, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_tim_enabled(channel,enabled);
    return result;
}

/**
 * 函 数 介 绍 ：
 *               设置tim 回调函数
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    tim_cbk  回调函数
 * 返   回  值 ：hal_result_t 回调函数设置结果
 * 备       注 ：
 *              当通道回调函数正在使用中时，设置回调函数应返回HAL_RESULT_BUSY
 */
hal_result_t hal_tim_if_cbk(hal_channel_handle_t channel, hal_tim_cbk_t tim_cbk)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_tim_cbk(channel,tim_cbk);
    return result;
}

/**
 * 函 数 介 绍 ：
 *               设置tim 中断参数
 *
 * 参       数 ：
 *              [in]    channel  timer通道，模块自行定义通道及其映射关系
 *              [in]    irq_type 中断类型
 *              [in]    priorty 设置中断优先级
 *              [in]    endbled  是否使能
 * 返   回  值 ：hal_result_t 回调函数设置结果
 * 备       注 ：
 *              当通道回调函数正在使用中时，设置回调函数应返回HAL_RESULT_BUSY
 */
hal_result_t hal_tim_if_irq_cfg(hal_channel_handle_t channel, hal_tim_irq_type_et type, hal_irq_priorty_et priorty, hal_enabled_et enabled)
{
   hal_result_t result = HAL_RESULT_SUCCESS;
   result = hal_tim_irq_cfg(channel,type,priorty,enabled);
   return result;
}

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
hal_result_t hal_tim_if_recount(hal_channel_handle_t channel)
{
	 hal_result_t result = HAL_RESULT_SUCCESS;
	result = hal_tim_recount(channel);
	return result;
}
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
void hal_tim_if_deinit(void)
{
	hal_tim_deinit();
}
