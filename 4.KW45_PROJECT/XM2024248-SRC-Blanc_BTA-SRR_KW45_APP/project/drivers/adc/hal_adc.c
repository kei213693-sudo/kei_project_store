/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_ADC.c
 * 摘要：
 *      文件概述：
 *                  文件主要描述ADC模块功能实现，需要保证接口函数功能实现，本文件为模板文件，
 *              实现实体需要自行补充。
 *      文件调用：
 *                  文件应包含hal_ADC_if.h hal_ADC.h hal_ADC_cfg.h。
 * 当前版本：
 * 历史版本：
 */

/*  Include     Begine      */
#include "hal_adc.h"
#include "hal_adc_cfg.h"
#include "hal_adc_if.h"
#include "fsl_lpadc.h"
#include "fsl_vref.h"
/*  Include     End         */

typedef struct
{
	uint8_t index;
	uint8_t num;
	uint8_t channel[HAL_ADC0_NUM];
}st_adc_channel;


static st_adc_channel adc_channel = {0};
static uint8_t adc_init_flag = 0;
uint32_t adc_result[HAL_ADC0_NUM] = {0};
static void hal_add_channel(hal_adc_channel_et channel);
static void hal_change_channel(void);
void ADC0_IRQHandler(void)
{
	lpadc_conv_result_t g_LpadcResultConfigStruct = {0};
	LPADC_GetConvResult(ADC0, &g_LpadcResultConfigStruct, 0U);
	adc_result[adc_channel.channel[adc_channel.index]] = g_LpadcResultConfigStruct.convValue>>3;
	hal_change_channel();
}


static void hal_add_channel(hal_adc_channel_et channel)
{
	if(adc_channel.num < HAL_ADC0_NUM)
	{
		adc_channel.channel[adc_channel.num] = channel;
		adc_channel.num++;
	}
	else
	{

	}
}

static void hal_change_channel(void)
{
	lpadc_conv_command_config_t command_config = {0};
//	lpadc_conv_trigger_config_t trigger_config = {0};

	LPADC_GetDefaultConvCommandConfig(&command_config);
//	LPADC_GetDefaultConvTriggerConfig(&trigger_config);

	adc_channel.index++;
	adc_channel.index %= adc_channel.num;
	if(adc_channel.channel[adc_channel.index] <=HAL_ADC0_CHANNELA15)
	{
		command_config.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
		command_config.channelNumber = adc_channel.channel[adc_channel.index];
	}
	else
	{
		command_config.sampleChannelMode = kLPADC_SampleChannelSingleEndSideB;
		command_config.channelNumber = adc_channel.channel[adc_channel.index]-HAL_ADC0_CHANNELA15;
	}
	LPADC_SetConvCommandConfig(ADC0, 1, &command_config);
//	trigger_config.targetCommandId = 1;
//	trigger_config.enableHardwareTrigger = false;
//	LPADC_SetConvTriggerConfig(ADC0, 0, &trigger_config);
}
/**
 * 函 数 介 绍 ：
 *                 ADC模块进行初始化
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *                  [in] adc_cfg_s  ADC初始化配置，可以在hal_adc_cfg.c中修改g_adc_cfg_sa实例化来修改初始化配置参数。
 *
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_init(uint8_t channel)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    lpadc_config_t adc_config = {0};
    lpadc_conv_trigger_config_t trigger_config = {0};
    lpadc_conv_command_config_t command_config = {0};
	uint32_t len = 0;
	vref_config_t vrefConfig = {0};
	hal_adc_cfg_st *pg_adc_cfg_sa = NULL;
	hal_adc_get_cfg(&pg_adc_cfg_sa,&len);
	if (channel >= HAL_ADC_CHANNELMAX)
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else
    {
    	if(0 == adc_init_flag)
    	{
    		LPADC_GetDefaultConvCommandConfig(&command_config);
    		LPADC_GetDefaultConvTriggerConfig(&trigger_config);
			CLOCK_SetIpSrc(kCLOCK_Lpadc0, kCLOCK_IpSrcFro192M);
			CLOCK_SetIpSrcDiv(kCLOCK_Lpadc0, kSCG_SysClkDivBy10);

			 VREF_GetDefaultConfig(&vrefConfig);
			vrefConfig.bufferMode = kVREF_ModeBandgapOnly;
			 VREF_Init(VREF0, &vrefConfig);

//			 VREF_SetTrim21Val(VREF0, 8U);

			LPADC_GetDefaultConfig(&adc_config);
			adc_config.enableAnalogPreliminary = true;
			adc_config.referenceVoltageSource = kLPADC_ReferenceVoltageAlt1;
			adc_config.conversionAverageMode = kLPADC_ConversionAverage128;
			LPADC_Init(ADC0, &adc_config);
			LPADC_DoOffsetCalibration(ADC0);
			LPADC_DoAutoCalibration(ADC0);
			if(adc_channel.channel[adc_channel.index] <=HAL_ADC0_CHANNELA15)
			{
				command_config.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
				command_config.channelNumber = adc_channel.channel[adc_channel.index];
			}
			else
			{
				command_config.sampleChannelMode = kLPADC_SampleChannelSingleEndSideB;
				command_config.channelNumber = adc_channel.channel[adc_channel.index]-HAL_ADC0_CHANNELA15;
			}
	    	LPADC_SetConvCommandConfig(ADC0, 1, &command_config);
	    	trigger_config.targetCommandId = 1;
			trigger_config.enableHardwareTrigger = false;
			LPADC_SetConvTriggerConfig(ADC0, 0, &trigger_config);
			hal_add_channel(pg_adc_cfg_sa[channel].m_adc_channel_e);
			LPADC_DoSoftwareTrigger(ADC0,1);
			adc_init_flag = 1;
	    	LPADC_EnableInterrupts(ADC0, kLPADC_FIFO0WatermarkInterruptEnable);
	    	EnableIRQ(ADC0_IRQn);
    	}
    	else
    	{
    		hal_add_channel(pg_adc_cfg_sa[channel].m_adc_channel_e);
    	}
    }
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 ADC模块设置外部触发类型
 *
 *  * 参       数 ：
 *                  [in] channel      adc通道，模块自行定义通道及其映射关系
 *                  [in] trig_type    设置触发类型，触发类型详见hal_adc_trig_type_et[hal_adc_typedef.h]
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_set_extriger(uint8_t channel, hal_adc_trig_type_et trig_type)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    if (channel >= HAL_ADC_CHANNELMAX)
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else
    {
        /* add code */

    }
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 ADC模块当前状态
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *                  [in] adc_sta    获取ADC状态的指针,状态详见hal_adc_status_et[hal_adc_typedef.h]
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_get_stauts(uint8_t channel, hal_adc_status_et *adc_sta)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    if (channel >= HAL_ADC_CHANNELMAX)
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else if (NULL == adc_sta)
    {
        l_i8_result = HAL_RESULT_NULL;
    }
    else
    {
        /*
         *adcsta_e = 当前ADC状态；
         */
    }
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 ADC模块采样结果
 *
 *  * 参       数 ：
 *                  [in] channel        adc通道，模块自行定义通道及其映射关系
 *                  [in] adc_val        获取采样值的指针，该接口一般用于单次采样
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：用于单采样
 *
 */
hal_result_t hal_adc_get_result(uint8_t channel, uint32_t *adc_val)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
	uint32_t adc_channel = 0;
	uint32_t len = 0;
	hal_adc_cfg_st *pg_adc_cfg_sa = NULL;
	hal_adc_get_cfg(&pg_adc_cfg_sa,&len);
	if((NULL != pg_adc_cfg_sa) && (channel < len))
	{
		*adc_val = adc_result[pg_adc_cfg_sa[channel].m_adc_channel_e];
    }
	else
	{
		l_i8_result = HAL_RESULT_PARAMETER_ERR;
	}
    return l_i8_result;
}
#if 0
/**
 * 函 数 介 绍 ：
 *                 ADC模块采样结果数组
 *
 *  * 参       数 ：
 *                  [in] channel            adc通道，模块自行定义通道及其映射关系
 *                  [in] adc_array          存储采样结果的数组，该接口用于连续采样
 *                  [in] array_len          数组长度
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：用于连续采样
 *
 */
hal_result_t hal_adc_get_resultarray(uint8_t channel, uint32_t *adc_array, uint8_t array_len)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    if (channel >= HAL_ADC_CHANNELMAX)
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else if (NULL == adc_array)
    {
        l_i8_result = HAL_RESULT_NULL;
    }
    else
    {
        /*
         *p_u32_adc_arr = 当前ADC值;
         */
    }
    return l_i8_result;
}
#endif
/**
 * 函 数 介 绍 ：
 *                 设置ADC采样分辨率
 *
 *  * 参       数 ：.
 *                  [in] channel        adc通道，模块自行定义通道及其映射关系
 *                  [in] adc_bit        采样分辨率
 *                                      举例：如分辨率为10位，则bit = 10；如分辨率为12位，则bit =12
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_set_bit(uint8_t channel, uint8_t adc_bit)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    if (channel >= HAL_ADC_CHANNELMAX)
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else
    {
        /*
        采样分辨率 = adc_bit;
        */
    }
		return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 ADC功能使能
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *                  [in] enabled    功能使能，详见hal_enabled_et[hal_typedef.h]
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_enabled(uint8_t channel, hal_enabled_et enabled)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    if (channel >= HAL_ADC_CHANNELMAX)
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else
    {
        /*
        功能使能位 = enabled;
        */
    }
		return l_i8_result;
}
#if 0
/**
 * 函 数 介 绍 ：
 *                 多通道采样初始化
 *
 *  * 参       数 ：
 *                  [in] p_volt_curt_arr  p_volt_curt_arr[HAL_ADC_VOLTAGE]存放电压值，arr[HAL_ADC_CURRENT]存放电流值
 *                  [in] l_u8_arr_len     数组长度，传参固定值HAL_ADC_VOLT_CURT_MAX
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：配置是采集电压值还是电流值
 *
 */
hal_result_t hal_adc_multi_init(uint32_t *volt_curt_arr, uint8_t arr_len)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    if ((NULL == volt_curt_arr) || (arr_len > HAL_ADC_VOLT_CURT_MAX))
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else
    {
        /* 此处添加需增加的功能代码 */
    }

    return l_i8_result;
}
#endif
/**
 * 函 数 介 绍 ：
 *                 ADC采集回调函数
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *                  [in] adc_cbk      ADC采集回调函数
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *
 */
void hal_adc_conver_cbk(uint8_t channel, hal_adc_conver_func_t adc_cbk)
{
    // to be add user code
}

/**
 * 函 数 介 绍 ：
 *                 ADC中断配置
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *
 *
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              中断配置根据hal_adc_cfg.h中g_adc_isr_cfg_sa结构体内容来配置
 */
hal_result_t hal_adc_irq_config(uint8_t channel)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    if (channel >= HAL_ADC_CHANNELMAX)
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else
    {
        /* 此处添加功能代码 */
    }
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 ADC开始采集
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：如需暂停采集，可以调用使能接口函数hal_adc_enabled关闭ADC功能
 *
 */
hal_result_t hal_adc_start_convert(uint8_t channel)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    if (channel >= HAL_ADC_CHANNELMAX)
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else
    {
        /* 开始采集*/
    }
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 ADC去初始化
 *
 *  * 参       数 ：
 *
 * 返   回  值 ：
 * 备       注 ：关闭ADC功能
 *
 */
void hal_adc_deinit(void)
{
	adc_init_flag = 0;
	adc_channel.index = 0;
	adc_channel.num = 0;
	LPADC_Deinit(ADC0);
}

/* Public Function End              */
