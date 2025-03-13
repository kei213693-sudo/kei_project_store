/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_rtc.h
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2023-10-18
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#ifndef HAL_RTC_H
#define HAL_RTC_H
#include "hal_typedef.h"
#include "hal_rtc_typedef.h"

hal_result_t hal_rtc_init(void);
/**
* @brief : 配置年月日
 *   @param           [in] @ref RTC_DateTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t  hal_rtc_set_date(RTC_DateTypeDef *date);

/**
* @brief : 获取年月日
 *   @param           [in] @ref RTC_DateTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t  hal_rtc_get_date(RTC_DateTypeDef *date);

/**
* @brief : 配置时分秒
 *   @param           [in] @ref RTC_TimeTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t   hal_rtc_set_time(RTC_TimeTypeDef *time);

/**
* @brief : 获取时分秒
 *   @param           [in] @ref RTC_TimeTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t hal_rtc_get_time(RTC_TimeTypeDef *time);
/**
* @brief : rtc使能
 *               @param [in] enabled             功能使能标签
 * @return: {*}
 * @note
 */
void hal_rtc_enabled(uint8_t enabled);
/**
* @brief : rtc alarm中断配置
 *    @param          [in]    priorty 设置中断优先级
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 * @note
 */
hal_result_t  hal_rtc_alarm_isr_config(hal_irq_priorty_et priorty);
/**
 * @brief 中断使能与否
 *
 * @param enabled
 */
void hal_rtc_alarm_isr_enabled(hal_enabled_et enabled);
/**
 * @brief 设置闹钟
 *
 * @param date  @ref RTC_DateTypeDef
 * @param time  @ref RTC_TimeTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 */
hal_result_t hal_rtc_set_alarm(RTC_DateTypeDef *date,RTC_TimeTypeDef *time);

/**
 * @brief 获取闹钟
 *
 * @param date  @ref RTC_DateTypeDef
 * @param time  @ref RTC_TimeTypeDef
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 */
hal_result_t hal_rtc_get_alarm(RTC_DateTypeDef *date,RTC_TimeTypeDef *time);
/**
 * @brief 设置闹钟回调函数
 *
 * @param cbk @ref hal_rtc_alarm_cbk_t
 * @return hal_result_t @ref HAL_RESULT_SUCCESS
 */
 hal_result_t hal_rtc_alarm_cbk(hal_rtc_alarm_cbk_t cbk);

 void hal_rtc_deinit(void);

#endif
