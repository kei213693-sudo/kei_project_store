/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_ADC_typedef.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述ADC模块专用自定义类型，需要根据实际情况判定是否是要需改。
 *      文件调用：
 *                  文件应在模块内部产生直接调用，外部使用需通过hal_ADC_if.h 或 hal_ADC.h
 *              间接调用。
 *                  文件应包含hal_typedef.h。
 * 当前版本：
 * 历史版本：
 */

#ifndef HAL_ADC_TYPEDEF_H
#define HAL_ADC_TYPEDEF_H

/*  Include     Begine      */
#include "hal_typedef.h"
/*  Include     End         */

// ADC采集回调函数重定义
typedef void (*hal_adc_conver_func_t)(uint16_t *conver_buf,uint16_t length);


// ADC通道枚举
typedef enum
{
	HAL_ADC0_CHANNELA0 = 0,			//ADC0_A0
	HAL_ADC0_CHANNELA5= 1,			//ADC0_A5
	HAL_ADC0_CHANNELA6 = 2,			//ADC0_A6
	HAL_ADC0_CHANNELA7 = 3,			//ADC0_A7
	HAL_ADC0_CHANNELA8 = 5,			//ADC0_A8
	HAL_ADC0_CHANNELA9 = 6,			//ADC0_A9
	HAL_ADC0_CHANNELA10 = 7,		//ADC0_A10
	HAL_ADC0_CHANNELA11 = 8,		//ADC0_A11
	HAL_ADC0_CHANNELA12 = 9,		//ADC0_A12
	HAL_ADC0_CHANNELA13 = 10,		//ADC0_A13
	HAL_ADC0_CHANNELA14 = 11,		//ADC0_A14
	HAL_ADC0_CHANNELA15 = 12,		//ADC0_A15
	HAL_ADC0_CHANNELB5 = 13,		//ADC0_B5
	HAL_ADC0_CHANNELB6 = 14,    	//ADC0_B6
	HAL_ADC0_CHANNELB9 = 18,		//ADC0_B9
	HAL_ADC0_CHANNELB10 = 19,		//ADC0_B10
	HAL_ADC0_CHANNELB11 = 20,		//ADC0_B11
	HAL_ADC0_CHANNELB12 = 21,		//ADC0_B12
	HAL_ADC0_CHANNELB13 = 22,		//ADC0_B13
	HAL_ADC0_NUM
}hal_adc_channel_et;

/**
 * 采样模式
 * HAL_ADC_MODE_SINGLECHL            单通道采集
 * HAL_ADC_MODE_MANYCHL              多通道采集
 *
 * HAL_ADC_MODE_SINGLE               单次采样
 * HAL_ADC_MODE_CONTINU              连续采样
 */
typedef enum
{
    HAL_ADC_MODE_SINGLECHL = 0,
    HAL_ADC_MODE_MANYCHL = 1,
    HAL_ADC_MODE_SINGLE ,
    HAL_ADC_MODE_CONTINU,
} hal_adc_mode_et;
/**
 * ADC采集外部触发类型
 * HAL_ADC_TRIG_ADC_TIM1_CC1                先择定时器1的捕获比较1作为转换外部触发
 * HAL_ADC_TRIG_ADC_TIM1_CC2                选择定时器 1 的捕获比较 2 作为转换外部触发
 * HAL_ADC_TRIG_ADC_TIM2_CC2                选择定时器 2 的捕获比较 2 作为转换外部触发
 * HAL_ADC_TRIG_ADC__Ext_IT11               选择外部中断线 11 事件作为转换外部触发
 * ADC_PWM                                  PWM波触发
 * ADC_SW                                   转换由软件而不是外部触发启动
 * */
typedef enum
{
    HAL_ADC_TRIG_ADC_TIM1_CC1 = 0,
    HAL_ADC_TRIG_ADC_TIM1_CC2,
    HAL_ADC_TRIG_ADC_TIM2_CC2,
    HAL_ADC_TRIG_ADC__Ext_IT11,
    HAL_ADC_TRIG_ADC_PWM,
    HAL_ADC_TRIG_ADC_SW,
} hal_adc_trig_type_et;

/**
 * ADC数据对齐方式
 *
 *
*/
/**
 * ADC采集状态
 * HAL_ADC_STATUS_CONVERT_DONE      采集完成
 * HAL_ADC_STATUS_BUSY              采集中
 * UNSTART                          未开始采集
 * UNINIT                           未初始化
 * ARRAY_CONVERT_DONE               数组采集完成
 */
typedef enum
{
    HAL_ADC_STATUS_UNINIT = 0,
    HAL_ADC_STATUS_IDLE,
    HAL_ADC_STATUS_BUSY
} hal_adc_status_et;
/**
 *
*/
typedef enum
{
    HAL_ADC_VOLTAGE,
    HAL_ADC_CURRENT,
    HAL_ADC_VOLT_CURT_MAX
}hal_adc_volt_curt_et;
/*  Enum        End         */

/*  Struct/union Begine      */
/**
 *
 * ADC采集结构体定义
 * */
typedef struct
{
    uint16_t m_u16_len;
    uint32_t *m_u32_pdata;

} hal_adc_conver_st;
/*  Struct/union End         */

#endif
