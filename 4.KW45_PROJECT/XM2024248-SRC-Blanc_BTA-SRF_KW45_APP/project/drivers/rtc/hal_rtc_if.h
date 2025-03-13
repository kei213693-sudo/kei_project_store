/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_rtc_if.h
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#ifndef HAL_RTC_IF_H
#define HAL_RTC_IF_H
#include "hal_rtc_typedef.h"
#include "hal_typedef.h"

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
hal_result_t hal_rtc_if_init(void);
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
hal_result_t  hal_rtc_if_set_date(RTC_DateTypeDef *date);

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
hal_result_t  hal_rtc_if_get_date(RTC_DateTypeDef *date);

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
hal_result_t   hal_rtc_if_set_time(RTC_TimeTypeDef *time);

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
hal_result_t hal_rtc_if_get_time(RTC_TimeTypeDef *time);
/**
 * 函 数 介 绍 ：
 *                 rtc使能
 *
 *  * 参       数
 *                 enabled：使能状态
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
void hal_rtc_if_enabled(uint8_t enabled);
/**
 * 函 数 介 绍 ：
 *                 alarm中断优先级设置
 *
 *  * 参       数
 *                priorty：中断优先级
 * 返   回  值 ：
 * 备       注 ：
 *
 */
hal_result_t  hal_rtc_if_alarm_isr_config(hal_irq_priorty_et priorty);
/**
 * 函 数 介 绍 ：
 *                 alarm中断使能
 *
 *  * 参       数
 *                 enabled：使能状态
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
void hal_rtc_if_alarm_isr_enabled(hal_enabled_et enabled);
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
hal_result_t hal_rtc_if_set_alarm(RTC_DateTypeDef *date,RTC_TimeTypeDef *time);

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
hal_result_t hal_rtc_if_get_alarm(RTC_DateTypeDef *date,RTC_TimeTypeDef *time);
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
 hal_result_t hal_rtc_if_alarm_cbk(hal_rtc_alarm_cbk_t cbk);
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
 void hal_rtc_if_deinit(void);

#endif
