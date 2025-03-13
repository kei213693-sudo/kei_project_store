/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_ADC.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述ADC模块抽象层接口，不建议修改函数接口。
 *      文件调用：
 *                  文件应被应hal层内部调用。
 *                  文件应包含hal_ADC_typedef.h。
 * 当前版本：
 * 历史版本：
*/
#ifndef HAL_ADC_H
#define HAL_ADC_H

/*  Include     Begine      */
#include "hal_adc_typedef.h"
/*  Include     End         */


/**
 * 函 数 介 绍 ：
 *                 ADC模块进行初始化
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *
 *
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              ADC初始化配置，可以在hal_adc_cfg.c中修改g_adc_cfg_sa实例化来修改初始化配置参数。
 */
hal_result_t hal_adc_init(uint8_t channel);

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
hal_result_t hal_adc_set_extriger(uint8_t channel, hal_adc_trig_type_et trig_type);

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
hal_result_t hal_adc_get_stauts(uint8_t channel, hal_adc_status_et *adc_sta);

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
hal_result_t hal_adc_get_result(uint8_t channel, uint32_t *adc_val);
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
hal_result_t hal_adc_set_bit(uint8_t channel, uint8_t adc_bit);

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
hal_result_t hal_adc_enabled(uint8_t channel, hal_enabled_et enabled);
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
void hal_adc_conver_cbk(uint8_t channel, hal_adc_conver_func_t adc_cbk);

/**
 * 函 数 介 绍 ：
 *                 ADC中断配置
 *
 *  * 参       数 ：
 *                  [in] channel    adc通道，模块自行定义通道及其映射关系
 *                  [in] isr_cfg_s    是否使能
 *
 * 返   回  值 ：hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_adc_irq_config(uint8_t channel);

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
hal_result_t hal_adc_start_convert(uint8_t channel);

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
void hal_adc_deinit(void);
/*  Function    Begine      */

/*  Function    End         */


#endif
