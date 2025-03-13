/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_rtc.c
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2023-10-18
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_rtc.h"
#include "fsl_rtc.h"
#include "fsl_ccm32k.h"

static hal_rtc_alarm_cbk_t rtc_alarm_cbk = NULL;

void RTC_Alarm_IRQHandler(void)
{
    uint32_t status = RTC_GetStatusFlags(RTC);

    if (status & kRTC_AlarmFlag)
    {
        /* Clear alarm flag */
    	if(NULL != rtc_alarm_cbk)
    	{
    		rtc_alarm_cbk();
    	}
        RTC_ClearStatusFlags(RTC, kRTC_AlarmInterruptEnable);
    }
    else if (status & kRTC_TimeInvalidFlag)
    {
        /* Clear timer invalid flag */
        RTC_ClearStatusFlags(RTC, kRTC_TimeInvalidFlag);
    }
    else
    {
    }
}

/*
 * 函 数 介 绍: rtc初始化
 * 参       数
 *              [in]enabled         功能使能标签
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
static void rtc_ccm32k_osc_config(void)
{
	ccm32k_osc_config_t osc32kconfig =
	{
		.enableInternalCapBank = true,
		.xtalCap = kCCM32K_OscXtal0pFCap,
		.extalCap = kCCM32K_OscExtal16pFCap,
		.coarseAdjustment = kCCM32K_OscCoarseAdjustmentRange0
	};
	CCM32K_Set32kOscConfig(CCM32K, kCCM32K_Enable32kHzCrystalOsc, &osc32kconfig);
	CCM32K_SelectClockSource(CCM32K, kCCM32K_ClockSourceSelectOsc32k);
}

hal_result_t hal_rtc_init(void)
{
	hal_result_t result = HAL_RESULT_SUCCESS;
	rtc_datetime_t data = {0};
	rtc_config_t rtcconfig = {0};

	RTC_GetDefaultConfig(&rtcconfig);
	RTC_Init(RTC, &rtcconfig);

	data.year = 2024;
	data.month = 1;
	data.day = 1;
	data.hour = 0;
	data.minute = 0;
	data.second = 0;

	RTC_StopTimer(RTC);
	RTC_SetDatetime(RTC, &data);

	RTC_StartTimer(RTC);

	// add user code
	return result;
}

/**
* @brief : 配置年月日
 *   @param           [in] @ref RTC_DateTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t  hal_rtc_set_date(RTC_DateTypeDef *date)
{
	rtc_datetime_t data = {0};
	RTC_StopTimer(RTC);
	RTC_GetDatetime(RTC, &data);
	data.year = date->Year;
	data.month = date->Month;
	data.day = date->Date;
	RTC_SetDatetime(RTC, &data);
	RTC_StartTimer(RTC);
	return HAL_RESULT_SUCCESS;
}

/**
* @brief : 获取年月日
 *   @param           [in] @ref RTC_DateTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t  hal_rtc_get_date(RTC_DateTypeDef *date)
{
	rtc_datetime_t data = {0};
	RTC_StopTimer(RTC);
	RTC_GetDatetime(RTC, &data);

	date->Year = data.year;
	date->Month = data.month;
	date->Date = data.day;

	RTC_SetDatetime(RTC, &data);
	RTC_StartTimer(RTC);
	return HAL_RESULT_SUCCESS;
}

/**
* @brief : 配置时分秒
 *   @param           [in] @ref RTC_TimeTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t   hal_rtc_set_time(RTC_TimeTypeDef *time)
{
	rtc_datetime_t data = {0};
	RTC_StopTimer(RTC);
	RTC_GetDatetime(RTC, &data);

	data.hour = time->Hours;
	data.minute = time->Minutes;
	data.second = time->Seconds;

	RTC_SetDatetime(RTC, &data);
	RTC_StartTimer(RTC);
	return HAL_RESULT_SUCCESS;
}


/**
* @brief : 获取时分秒
 *   @param           [in] @ref RTC_TimeTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t hal_rtc_get_time(RTC_TimeTypeDef *time)
{
	rtc_datetime_t data = {0};
	RTC_GetDatetime(RTC, &data);

	time->Hours = data.hour;
	time->Minutes = data.minute;
	time->Seconds = data.second;

	return HAL_RESULT_SUCCESS;
}
/**
* @brief : rtc使能
 *               @param [in] enabled             功能使能标签
 * @return: {*}
 * @note
 */
void hal_rtc_enabled(uint8_t enabled)
{
	if(HAL_ENABLED_DISABLE == enabled)
	{
		RTC_StartTimer(RTC);
	}
	else
	{
		RTC_StopTimer(RTC);
	}
}
/**
* @brief : rtc alarm中断配置
 *    @param          [in]    priorty 设置中断优先级
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t  hal_rtc_alarm_isr_config(hal_irq_priorty_et priorty)
{
	return HAL_RESULT_NO_SUPPORT;
}



/**
 * @brief 中断使能与否
 *
 * @param enabled
 */
void hal_rtc_alarm_isr_enabled(hal_enabled_et enabled)
{
	if(HAL_ENABLED_DISABLE == enabled)
	{
		DisableIRQ(RTC_Alarm_IRQn);
		RTC_DisableInterrupts(RTC, kRTC_AlarmInterruptEnable);
	}
	else
	{
		RTC_EnableInterrupts(RTC, kRTC_AlarmInterruptEnable);
		EnableIRQ(RTC_Alarm_IRQn);
	}
}
/**
 * @brief 设置闹钟
 *
 * @param date  @ref RTC_DateTypeDef
 * @param time  @ref RTC_TimeTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 */
hal_result_t hal_rtc_set_alarm(RTC_DateTypeDef *date,RTC_TimeTypeDef *time)
{
	rtc_datetime_t data = {0};
	data.year = date->Year;
	data.month = date->Month;
	data.day = date->Date;
	data.hour = time->Hours;
	data.minute = time->Minutes;
	data.second = time->Seconds;
	RTC_SetAlarm(RTC, &data);
	return HAL_RESULT_SUCCESS;
}


/**
 * @brief 获取闹钟
 *
 * @param date  @ref RTC_DateTypeDef
 * @param time  @ref RTC_TimeTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 */
hal_result_t hal_rtc_get_alarm(RTC_DateTypeDef *date,RTC_TimeTypeDef *time)
{
	rtc_datetime_t data = {0};
	RTC_GetAlarm(RTC, &data);

	date->Year = data.year;
	date->Month = data.month;
	date->Date = data.day;

	time->Hours = data.hour;
	time->Minutes = data.minute;
	time->Seconds = data.second;
	return HAL_RESULT_SUCCESS;
}
/**
 * @brief 设置闹钟回调函数
 *
 * @param cbk @ref hal_rtc_alarm_cbk_t
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 */
 hal_result_t hal_rtc_alarm_cbk(hal_rtc_alarm_cbk_t cbk)
 {
	 rtc_alarm_cbk = cbk;
	 return HAL_RESULT_SUCCESS;
 }

 void hal_rtc_deinit(void)
 {
	 RTC_Deinit(RTC);
}


