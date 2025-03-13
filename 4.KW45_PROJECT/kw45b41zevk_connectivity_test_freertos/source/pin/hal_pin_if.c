//copy by cc
/* 
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_pin_if.c
 * 摘要: 
 * 当前版本: V1.0.0 Chenjianbo 
 * Date: 2023-11-23
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_pin_if.h"
#include "hal_pin.h"
/**
* @brief  ：
 *                 所有PIN脚进行初始化
 *                 按照配置表信息逐行初始化。外部调用时直接调用本函数即可
 *
  * @param ： @ref hal_pin_cfg_st 
 *@param  len 配置表长度·
* @return hal_result_t   @ref           HAL_RESULT_SUCCESS   
 * @note ：
 *              原始参数来源于sc_pin_init_cfg_sa中的配置
 *               hal_pin_cfg_st * pincfg脚配置属性
 *              m_u32_port       配置端口号，可以为单片机原有定义直接映射
 *              m_u32_pin        配置PIN脚号，可以为单片机原有定义直接映射
 *              m_direction_et   配置输入输出，详情参考hal_pin_direction_et
 *              m_mode_et        配置默认PIN脚模式，如上拉等 详情参考hal_pin_mode_et
 *              m_u32_alternativenum 配置中复用号,可以通过#define HAL_PIN_GET_MULTIPLE(PINX_X,FUNCITON)
 *                              获取复用定义中的值，也可以直接调用复用定义中的宏定义。如果未使用复用则直接
 *                              调用HAL_PIN_AF_NULL
 */
hal_result_t hal_pin_if_init(void)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pin_init();
    return result;
}

#if HAL_STD_OFF
/**
* @brief  ：
 *                 设置PIN index对应的PIN脚的复用功能
 *
 *             @param     [in] pinindex pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_cfg.h:PIN index 宏定义组
 *                  @param [in] afnum    pin对应功能的复用号，可以通过宏获取该号码或直接使用宏定义，参考hal_pin_cfg.h
 *                                可以通过#define HAL_PIN_GET_MULTIPLE(PINX_X,FUNCITON)
 *                                获取复用定义中的值，也可以直接调用复用定义中的宏定义。
* @return hal_result_t   @ref           HAL_RESULT_SUCCESS   
 * @note ：
 *
 */
hal_result_t hal_pin_if_af(uint32_t pinindex, uint32_t afnum)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pin_af(pinindex, afnum);
    return result;
}
#endif
/**
* @brief  ：
 *                 获取Pin脚电平状态
 *
  * @param ：
 *                  [in] pinindex pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *
 * @return ：hal_result_t   >=0 :操作成功 详情参考 hal_pin_if.h:hal_pin_level_et 枚举
 *                              <0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * @note ：
 *
 */
 __IO hal_pin_level_et hal_pin_if_read(uint32_t pinindex)
{
    hal_pin_level_et result = HAL_RESULT_SUCCESS;
    result = hal_pin_read(pinindex);
    return result;
}

/**
* @brief  ：
 *                 写Pin脚电平状态
 *
  * @param ：
 *                  [in] pinindex   pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *                  [in] level      所得电平状态
 * @return ：hal_result_t    =0 :操作成功
 *                              <0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * @note ：
 *
 */
hal_result_t hal_pin_if_write(uint32_t pinindex, hal_pin_level_et level)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pin_write(pinindex, level);
    return result;
}

/**
* @brief  ：
 *                 设置PIN脚中断
 *
  * @param ：
 *                  [in] pinindex   pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *                  [in] priorty    设置中断优先级
 *                  [in] functioncbk    设置中断回调函数
 * @return ：hal_result_t    =0 :操作成功
 *                              <0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * @note ：
 *              需完成初始化后才允许调用
 */
hal_result_t hal_pin_if_irq(uint32_t pinindex, hal_irq_priorty_et priorty, hal_pin_cbk_t functioncbk)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pin_irq(pinindex, priorty, functioncbk);
    return result;
}

/**
* @brief  ：
 *                 设置PIN中断使能状态
 *
  * @param ：
 *                  [in] pinindex   pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *                  @param [in] enabled    设置中断使能状态
 * @return ：hal_result_t    =0 :操作成功
 *                              <0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * @note ：
 *              需完成初始化后才允许调用
 */
hal_result_t hal_pin_if_irq_enabled(uint32_t pinindex, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pin_irq_enabled(pinindex, enabled);
    return result;
}
