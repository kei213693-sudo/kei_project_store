/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_adc_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "hal_adc_if.h"
#include "hal_adc_cfg.h"
#include "hal_adc.h"
/**
 * 函 数 介 绍 ：
 *                 ADC模块进行初始化
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *
 *
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：该接口是对hal_adc_init接口的再次封装，供应用层调用
 *
 */
hal_result_t hal_adc_if_init(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;

    result = hal_adc_init(channel);

    return result;
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
hal_result_t hal_adc_if_set_extriger(uint8_t channel, hal_adc_trig_type_et trig_type)
{
    hal_result_t result = HAL_RESULT_SUCCESS;

    result = hal_adc_set_extriger(channel, trig_type);

    return result;
}

/**
 * 函 数 介 绍 ：
 *                 ADC模块当前状态
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *                      adc_sta_e    获取ADC状态的指针,状态详见hal_adc_status_et[hal_adc_typedef.h]
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_if_get_stauts(uint8_t channel, hal_adc_status_et *adc_sta)
{
    hal_result_t result = HAL_RESULT_SUCCESS;

    result = hal_adc_if_get_stauts(channel,adc_sta);

    return result;
}

/**
 * 函 数 介 绍 ：
 *                 ADC模块采样结果
 *
 *  * 参       数 ：
 *                  [in] channel        adc通道，模块自行定义通道及其映射关系
 *                  [in] adc_val        获取采样值的指针
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *
 */
hal_result_t hal_adc_if_get_result(uint8_t channel, uint32_t *adc_val)
{
    hal_result_t result = HAL_RESULT_SUCCESS;

    result = hal_adc_get_result(channel, adc_val);

    return result;
}
#if 0
/**
 * 函 数 介 绍 ：
 *                 ADC模块采样结果数组
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *                  [in] p_u32_adc_arr      存储采样结果的数组，该接口用于连续采样
 *                  [in] l_u8_arrlen        数组长度
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：用于连续采样
 *
 */
hal_result_t hal_adc_get_resultarray(uint8_t channel, uint32_t *p_u32_adc_arr, uint8_t l_u8_arrlen);
#endif
/**
 * 函 数 介 绍 ：
 *                 设置ADC采样位数
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *                  [in] bit
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_if_set_bit(uint8_t channel, uint8_t adc_bit)
{
    hal_result_t result = HAL_RESULT_SUCCESS;

    result = hal_adc_set_bit(channel, adc_bit);

    return result;
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
hal_result_t hal_adc_if_enabled(uint8_t channel, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;

    result = hal_adc_enabled(channel, enabled);

    return result;
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
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_multi_init(uint32_t *p_volt_curt_arr, uint8_t l_u8_arr_len);
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
void hal_adc_if_conver_cbk(uint8_t channel, hal_adc_conver_func_t adc_cbk)
{
    hal_adc_conver_cbk(channel,adc_cbk);
}

/**
 * 函 数 介 绍 ：
 *                 ADC中断配置
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *                  [in] isr_cfg    是否使能
 *
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_if_irq_config(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;

    result = hal_adc_irq_config(channel);

    return result;
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
hal_result_t hal_adc_if_start_convert(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;

    result = hal_adc_start_convert(channel);

    return result;
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
void hal_adc_if_deinit(void)
{
	hal_adc_deinit();
}
