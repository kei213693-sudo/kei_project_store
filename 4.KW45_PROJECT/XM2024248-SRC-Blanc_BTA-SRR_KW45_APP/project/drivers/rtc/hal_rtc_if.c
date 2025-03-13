/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_rtc_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_rtc_if.h"
#include "hal_rtc.h"
/**
 * 函 数 介 绍 ：
 *                 rtc初始化
 *
 *  * 参       数
 *
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_rtc_if_init(void)
{
    hal_result_t result = hal_rtc_init();
    return result;
}
/**
 * 函 数 介 绍 ：
 *                 设置年月日
 *
 *  * 参       数
 *                 date：设置的年月日
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t  hal_rtc_if_set_date(RTC_DateTypeDef *date)
{
	hal_result_t result = hal_rtc_set_date(date);
	return result;
}

/**
 * 函 数 介 绍 ：
 *                 获取年月日
 *
 *  * 参       数
 *                 date：获得的年月日
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t  hal_rtc_if_get_date(RTC_DateTypeDef *date)
{
	hal_result_t result = hal_rtc_get_date(date);
	return result;
}

/**
 * 函 数 介 绍 ：
 *                 设置时分秒
 *
 *  * 参       数
 *                 time：设置的时分秒
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t   hal_rtc_if_set_time(RTC_TimeTypeDef *time)
{
	hal_result_t result = hal_rtc_set_time(time);
	return result;
}


/**
 * 函 数 介 绍 ：
 *                 获取时分秒
 *
 *  * 参       数
 *                 time：获得的时分秒
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_rtc_if_get_time(RTC_TimeTypeDef *time)
{
	hal_result_t result = hal_rtc_get_time(time);
	return result;
}
/**
 * 函 数 介 绍 ：
 *                 rtc使能
 *
 *  * 参       数
 *                 enabled：使能状态
 * 返   回  值
 * 备       注 ：
 *
 */
void hal_rtc_if_enabled(uint8_t enabled)
{
	hal_rtc_enabled(enabled);
}
/**
 * 函 数 介 绍 ：
 *                 alarm中断优先级设置
 *
 *  * 参       数
 *                priorty：中断优先级
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t  hal_rtc_if_alarm_isr_config(hal_irq_priorty_et priorty)
{
	hal_result_t result = hal_rtc_alarm_isr_config(priorty);
	return result;
}



/**
 * 函 数 介 绍 ：
 *                 alarm中断使能
 *
 *  * 参       数
 *                 enabled：使能状态
 * 返   回  值
 * 备       注 ：
 *
 */
void hal_rtc_if_alarm_isr_enabled(hal_enabled_et enabled)
{
	hal_rtc_alarm_isr_enabled(enabled);
}
/**
 * 函 数 介 绍 ：
 *                 设置闹钟
 *
 *  * 参       数
 *                 date：设置的年月日
 * 				   time：设置的时分秒
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_rtc_if_set_alarm(RTC_DateTypeDef *date,RTC_TimeTypeDef *time)
{
	hal_result_t result = hal_rtc_set_alarm(date, time);
	return result;
}


/**
 * 函 数 介 绍 ：
 *                 获得设置的闹钟时间
 *
 *  * 参       数
 *                 date：获得的年月日
 * 				  time：获得的时分秒
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_rtc_if_get_alarm(RTC_DateTypeDef *date,RTC_TimeTypeDef *time)
{
	hal_result_t result = hal_rtc_get_alarm(date, time);
	return result;
}
/**
 * 函 数 介 绍 ：
 *                 设置闹钟回调函数
 *
 *  * 参       数
 *                 cbk：闹钟的回调函数
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
 hal_result_t hal_rtc_if_alarm_cbk(hal_rtc_alarm_cbk_t cbk)
 {
	 hal_result_t result = hal_rtc_alarm_cbk(cbk);
	 return result;
 }
/**
 * 函 数 介 绍 ：
 *                 rtc去初始化
 *
 *  * 参       数
 *
 * 返   回  值
 * 备       注 ：
 *
 */
 void hal_rtc_if_deinit(void)
 {
	 hal_rtc_deinit();
}

