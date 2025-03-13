/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_tim.c
 * 摘要：
 *      文件概述：
 *                  文件主要描述tim模块功能实现，需要保证接口函数功能实现，本文件为模板文件，
 *              实现实体需要自行补充。
 *      文件调用：
 *                  文件应包含hal_tim_if.h hal_tim.h hal_tim_cfg.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

/*  Include     Begine      */
#include "hal_tim.h"
#include "hal_tim_cfg.h"
#include "hal_tim_if.h"
#include "fsl_lpit.h"

/*  Include     End         */
static hal_tim_cbk_t timer_cbk_array[4] = {0};
static uint8_t lpitinit_flag = 0;

void LPIT0_IRQHandler(void)
{
	if(kLPIT_Channel0TimerFlag == LPIT_GetStatusFlags(LPIT0))
	{
	    LPIT_ClearStatusFlags(LPIT0, kLPIT_Channel0TimerFlag);
	    if(NULL != timer_cbk_array[0])
	    {
	    	timer_cbk_array[0]();
	    }
	}
	if(kLPIT_Channel1TimerFlag == LPIT_GetStatusFlags(LPIT0))
	{
	    LPIT_ClearStatusFlags(LPIT0, kLPIT_Channel1TimerFlag);
	    if(NULL != timer_cbk_array[1])
		{
			timer_cbk_array[1]();
		}
	}
	if(kLPIT_Channel2TimerFlag == LPIT_GetStatusFlags(LPIT0))
	{
		LPIT_ClearStatusFlags(LPIT0, kLPIT_Channel2TimerFlag);
		 if(NULL != timer_cbk_array[2])
		{
			timer_cbk_array[2]();
		}
	}
	if(kLPIT_Channel3TimerFlag == LPIT_GetStatusFlags(LPIT0))
	{
		LPIT_ClearStatusFlags(LPIT0, kLPIT_Channel3TimerFlag);
		 if(NULL != timer_cbk_array[3])
		{
			timer_cbk_array[3]();
		}
	}
}
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
hal_result_t hal_tim_init(uint8_t channel, hal_enabled_et enabled_e, hal_tim_cbk_t tim_cbk)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
	hal_tim_cfg_st *ptim_cfg_st = NULL;
    lpit_config_t lpitConfig = {0};
    lpit_chnl_params_t lpitChannelConfig = {0};
	uint32_t len = 0;
    uint32_t time_cnt = 0;
	hal_tim_get_cfg(&ptim_cfg_st,&len);
    if (channel < HAL_TIM_CHANNEL_NUM)
    {
    	if(NULL != ptim_cfg_st)
    	{
    		if(0 == lpitinit_flag)
    		{
    			CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcSoscClk);
    			LPIT_GetDefaultConfig(&lpitConfig);
    			LPIT_Init(LPIT0, &lpitConfig);
    			EnableIRQ(LPIT0_IRQn);
    			lpitinit_flag = 1;
    		}
    		if(enabled_e == HAL_ENABLED_ENABLE)
			{
    		    lpitChannelConfig.chainChannel          = false;
    		    lpitChannelConfig.enableReloadOnTrigger = false;
    		    lpitChannelConfig.enableStartOnTrigger  = false;
    		    lpitChannelConfig.enableStopOnTimeout   = false;
    		    lpitChannelConfig.timerMode             = kLPIT_PeriodicCounter;
    		    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    		    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;
    		    LPIT_SetupChannel(LPIT0,channel, &lpitChannelConfig);
    		    time_cnt = 1000000 / ptim_cfg_st[channel].m_32_tim_hz;
    		    LPIT_SetTimerPeriod(LPIT0, channel, USEC_TO_COUNT(time_cnt, CLOCK_GetFreq(kCLOCK_ScgSysOscClk)));
    		    LPIT_EnableInterrupts(LPIT0, 1<<channel);
    		    timer_cbk_array[channel] = tim_cbk;
    		    LPIT_StartTimer(LPIT0, channel);
			}
    		else
    		{

    		}
    	}
    	else
    	{
    		result = HAL_RESULT_PARAMETER_ERR;
    	}
    }
    else
    {
        result = HAL_RESULT_PARAMETER_ERR;
    }

    return result;
}

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
hal_result_t hal_tim_clock(uint8_t channel, uint32_t hz)
{
    hal_result_t result = HAL_RESULT_NO_SUPPORT;

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
#if HAL_TIM_CLOCK_MS_DEFINE
hal_result_t hal_tim_clock_ms(uint8_t channel, uint32_t ms)
{

    return hal_tim_clock(channel, 1000 / ms);
}
#else
hal_result_t hal_tim_clock_ms(uint8_t channel, uint32_t ms)
{
    return HAL_RESULT_NO_SUPPORT;
}
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
hal_result_t hal_tim_enabled(uint8_t channel, hal_enabled_et enabled_e)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
	hal_tim_cfg_st *ptim_cfg_st = NULL;
	lpit_chnl_params_t lpitChannelConfig = {0};
	uint32_t len = 0;
	uint32_t time_cnt = 0;
	hal_tim_get_cfg(&ptim_cfg_st,&len);
	if (channel < HAL_TIM_CHANNEL_NUM)
	{
		if(NULL != ptim_cfg_st)
		{
			if(HAL_ENABLED_ENABLE == enabled_e)
			{
					lpitChannelConfig.chainChannel          = false;
					lpitChannelConfig.enableReloadOnTrigger = false;
					lpitChannelConfig.enableStartOnTrigger  = false;
					lpitChannelConfig.enableStopOnTimeout   = false;
					lpitChannelConfig.timerMode             = kLPIT_PeriodicCounter;
					lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
					lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;
					LPIT_SetupChannel(LPIT0,channel, &lpitChannelConfig);
					time_cnt = 1000000 / ptim_cfg_st[channel].m_32_tim_hz;
					LPIT_SetTimerPeriod(LPIT0, channel, USEC_TO_COUNT(time_cnt, CLOCK_GetFreq(kCLOCK_ScgSysOscClk)));
					LPIT_EnableInterrupts(LPIT0, 1<<channel);
					LPIT_StartTimer(LPIT0, channel);
			}
			else
			{
				LPIT_StopTimer(LPIT0, channel);
			}
		}
		else
		{
			result = HAL_RESULT_PARAMETER_ERR;
		}
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
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
 *              如果使用RTOS系统，当通道回调函数正在使用中时，设置回调函数应返回HAL_RESULT_BUSY
 */
hal_result_t hal_tim_cbk(uint8_t channel, hal_tim_cbk_t tim_cbk)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
	hal_tim_cfg_st *ptim_cfg_st = NULL;
	uint32_t len = 0;
	hal_tim_get_cfg(&ptim_cfg_st,&len);
    if ((channel < HAL_TIM_CHANNEL_NUM)&&(NULL != ptim_cfg_st))
    {
    	timer_cbk_array[channel] = tim_cbk;
    }
    else
    {
        result = HAL_RESULT_PARAMETER_ERR;
    }

    return result;
}

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
hal_result_t hal_tim_irq_cfg(uint8_t channel, hal_tim_irq_type_et type, hal_irq_priorty_et priorty, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_NO_SUPPORT;

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
hal_result_t hal_tim_recount(hal_channel_handle_t channel)
{
    hal_result_t result = HAL_RESULT_NO_SUPPORT;

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
void hal_tim_deinit(void)
{
	lpitinit_flag = 0;
	LPIT_Deinit(LPIT0);
}
/* Public Function End              */

