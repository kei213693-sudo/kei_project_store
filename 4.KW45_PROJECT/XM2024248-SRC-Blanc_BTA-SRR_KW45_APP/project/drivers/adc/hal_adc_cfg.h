/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_ADC.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述ADC模块配置接口，不建议修改。
 *      文件调用：
 *                  文件应被hal_ADC.c调用。
 *                  文件应包含hal_ADC_typedef.h。
 * 当前版本：
 * 历史版本：
*/
#ifndef HAL_ADC_CFG_H
#define HAL_ADC_CFG_H

/*  Include     Begine     */
#include "hal_adc_typedef.h"
/*  Include     End     */


/*ADC分辨率*/
#define HAL_ADC_BIT                 (12)

///*ADC0通道映射*/
//#define HAL_ADC_CHANNEL0            (0)
/*最多有几路ADC通道*/
#define HAL_ADC_CHANNELMAX          (2)



/*将要被使能或者失能的指定 ADC 中断源,开发者可根据芯片更改后面宏定义值*
*HAL_ADC_IT_EOC                     EOC 中断屏蔽
*HAL_ADC_IT_AWD                     AWDOG 中断屏蔽
*HAL_ADC_IT_JEOC                    JEOC 中断屏蔽
*/
#define HAL_ADC_IT_EOC              (0x01u)
#define HAL_ADC_IT_AWD              (0x02u)
#define HAL_ADC_IT_JEOC             (0x04u)


/**
 * ADC数据对齐方式
 * HAL_ADC_DataAlign_Right          ADC 数据右对齐
 * HAL_ADC_DataAlign_Left           ADC 数据左对齐
 * 宏定义值可以根据项目芯片去更改
*/
#define HAL_ADC_DataAlign_Right     (0xFFFFFFFFu)
#define HAL_ADC_DataAlign_Left      (0xEEEEEEEEu)

/**
*    ADC初始化结构体定义
*    m_enabled_e                        ADC功能使能标签，这个使能是是否需要打开或则关闭ADC功能，
                                        具体配置需要看芯片是否有支持直接打开或者关闭ADC功能的寄存器，
                                        如果没有相关寄存器则默认配置为HAL_ENABLED_ENABLE即可
*    m_u8_adc_bit                       ADC有多少位，由HAL_ADC_BIT决定，宏在hal_adc_cfg.h中定义，
                                        也可以调用hal_adc_bit（）函数去设置。
*    m_adc_scanConvmode_e               规定了模数转换工作在扫描模式（多通道）还是单次（单通道）模式
                                        可设置这个模式为HAL_ADC_MODE_SINGLECHL（单通道），可以设置HAL_ADC_MODE_MANYCHL（多通道）
     m_adc_continuousconvMode           规定了模数转换工作在连续还是单次模式，可以设置HAL_ADC_MODE_SINGLE（单次采集模式）
                                        HAL_ADC_MODE_CONTINU（连续采集模式）
*    m_trig_type_e                      设置外部触发类型
*    m_adcstatus_e                      设置ADC当前状态
     m_u32_adc_dataalign                指定ADC数据对齐方式，可配置HAL_ADC_DataAlign_Right、HAL_ADC_DataAlign_Left
    备注：以上为抽象接口定义，初始化时需要根据芯片手册具体配置到对应寄存器
*/
typedef struct
{
	hal_adc_channel_et m_adc_channel_e;
	uint8_t m_u8_adc_bit;
	hal_enabled_et m_enabled_e;
	hal_adc_mode_et m_adc_scanConvmode_e;
	 hal_adc_mode_et m_adc_continuousconvMode;
	hal_adc_trig_type_et m_trig_type_e;
	hal_adc_status_et   m_adcstatus_e;
	uint32_t m_u32_adc_dataalign;
}hal_adc_cfg_st;
/**
 *
 * ADC中断配置
 *
 * m_u8_irq_type                指定中断配置类型，可设置HAL_ADC_IT_EOC、HAL_ADC_IT_AWD、HAL_ADC_IT_JEOC
 * m_priorty_e                  指定中断优先级
 * m_enabled_e                  指定中断使能标签
*/
typedef struct
{
    uint16_t m_u8_irq_type;
    hal_irq_priorty_et m_priorty_e;
    hal_enabled_et m_enabled_e;
}hal_adc_isr_cfg_st;

/*  Function    Begine     */
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
void hal_adc_get_cfg(hal_adc_cfg_st **adccfg, uint32_t *len);
/*  Function    End     */

#endif
