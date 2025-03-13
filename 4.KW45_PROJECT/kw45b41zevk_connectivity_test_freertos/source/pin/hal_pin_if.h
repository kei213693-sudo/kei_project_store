//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_pin_if.h
 * 摘要：
 *          文件主要描述pin模块对外接口
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */
#ifndef HAL_CLOCK_IF_H
#define HAL_CLOCK_IF_H

/*  Include  Begine */
#include "hal_pin_cfg.h"
#include "hal_pin_typedef.h"
/*  Include  End    */

/*  Typedef Begine */



/*  Typedef End */

/*  Mirco Begine*/

/**
 * PIN index
 */

/*  Mirco End*/

/*  Function Begine   */

/**
* @brief  ：
 *                 所有PIN脚进行初始化
 *                 通过函数接口 hal_pin_get_cfg(hal_pin_cfg_st *pincfg,uint32_t *len) 获取配置表信息，
 *                 按照配置表信息逐行初始化。外部调用时直接调用本函数即可
 *
  * @param ：
 *
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
hal_result_t hal_pin_if_init(void);

#if HAL_STD_OFF
/**
* @brief  ：
 *                 设置PIN index对应的PIN脚的复用功能
 *
  * @param ：
 *                  [in] pinindex pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_cfg.h:PIN index 宏定义组
 *                  @param [in] afnum    pin对应功能的复用号，可以通过宏获取该号码或直接使用宏定义，参考hal_pin_cfg.h
 *                                可以通过#define HAL_PIN_GET_MULTIPLE(PINX_X,FUNCITON)
 *                                获取复用定义中的值，也可以直接调用复用定义中的宏定义。
* @return hal_result_t   @ref           HAL_RESULT_SUCCESS   
 * @note ：
 *
 */
hal_result_t hal_pin_if_af(uint8_t pinindex, uint8_t afnum);
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
__IO hal_pin_level_et hal_pin_if_read(uint32_t pinindex);

/**
* @brief  ：
 *                 设置Pin脚电平状态
 *
  * @param ：
 *                  [in] pinindex   pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *                  [in] level      所得电平状态
 * @return ：hal_result_t    =0 :操作成功
 *                              <0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * @note ：
 *
 */
hal_result_t hal_pin_if_write(uint32_t pinindex, hal_pin_level_et level);

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
hal_result_t hal_pin_if_irq(uint32_t pinindex, hal_irq_priorty_et priorty, hal_pin_cbk_t functioncbk);

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
hal_result_t hal_pin_if_irq_enabled(uint32_t pinindex, hal_enabled_et enabled);

#endif
