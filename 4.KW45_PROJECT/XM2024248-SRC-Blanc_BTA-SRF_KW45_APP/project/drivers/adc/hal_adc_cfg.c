/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_ADC.c
 * 摘要：
 *      文件概述：
 *                  文件主要描述ADC模块配置实现，本文件为模板文件，
 *              实现实体需要自行补充。
 *      文件调用：
 *                  文件应包含hal_ADC_cfg.h。
 *
 * 当前版本：
 * 历史版本：
*/

/*  Include     Begine      */
#include "hal_adc_cfg.h"
/*  Include     End         */

/*ADC初始化配置结构体实例化*/
static hal_adc_cfg_st g_adc_cfg_sa[HAL_ADC_CHANNELMAX] = {
    //0
		{HAL_ADC0_CHANNELA10,HAL_ADC_BIT,HAL_ENABLED_ENABLE,HAL_ADC_MODE_SINGLECHL,HAL_ADC_MODE_SINGLE,HAL_ADC_TRIG_ADC_TIM1_CC1,HAL_ADC_STATUS_UNINIT},
	//1
		{HAL_ADC0_CHANNELB6,HAL_ADC_BIT,HAL_ENABLED_ENABLE,HAL_ADC_MODE_SINGLECHL,HAL_ADC_MODE_SINGLE,HAL_ADC_TRIG_ADC_TIM1_CC1,HAL_ADC_STATUS_UNINIT}
};
/*ADC中断配置 初始化结构体*/
const hal_adc_isr_cfg_st g_adc_isr_cfg_sa[HAL_ADC_CHANNELMAX] =
{
    {HAL_ADC_IT_EOC,HAL_IRQ_PRIORTY_0,HAL_ENABLED_ENABLE}
};
/**
 * 函 数 介 绍 ：
 *                 获得ADC配置表
 *
 *  * 参       数 ：
 *                  adccfg      获得配置表指针
 *                  len         或者配置表长度
 * 返   回  值 ：
 * 备       注 ：
 *
 */
void hal_adc_get_cfg(hal_adc_cfg_st **adccfg, uint32_t *len)
{
      *len = HAL_ADC_CHANNELMAX;
      *adccfg = g_adc_cfg_sa;
}
