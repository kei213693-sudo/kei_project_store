//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_pin_if.h
 * 摘要：
 *          文件主要描述pin模块对外接口
 * 当前版本：
 * 历史版本：
 */
#ifndef HAL_PIN_IF_H
#define HAL_PIN_IF_H

/*  Include  Begine */
#include "hal_pin_cfg.h"
#include "hal_pin_typedef.h"
/*  Include  End    */


/**
 * 函 数 介 绍 ：
 *                 pin初始化
 *
 *  * 参       数
 *
 * 返   回  值
 * 备       注 ：   根据pin_cfg配置表初始化引脚
 *
 */
hal_result_t hal_pin_if_init(void);

#if HAL_STD_OFF
/**
 * 函 数 介 绍 ：
 *                 引脚复用
 *
 *  * 参       数
 *                 pinindex： 引脚索引
 * 					afnum ： 复用号
 * 返   回  值
 * 备       注 ：   根据pin_cfg配置表初始化引脚
 *
 */
hal_result_t hal_pin_if_af(uint8_t pinindex, uint8_t afnum);
#endif
/**
 * 函 数 介 绍 ：
 *                 获得pin脚电平值
 *
 *  * 参       数
 *                 pinindex ： 引脚索引
 * 返   回  值 ：0：低电平；1：高电平
 * 备       注 ：
 *
 */
hal_pin_level_et hal_pin_if_read(uint32_t pinindex);

/**
 * 函 数 介 绍 ：
 *                 设置引脚输出电平
 *
 *  * 参       数
 *                 pinindex ： 引脚索引
 *                  level ：电平值
 * 返   回  值 ：0：成功，其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_pin_if_write(uint32_t pinindex, hal_pin_level_et level);

/**
 * 函 数 介 绍 ：
 *                 设置引脚中断
 *
 *  * 参       数
 *                 pinindex ： 引脚索引
 *                  priorty ：中断优先级
 *                  functioncbk ：中断回调函数
 * 返   回  值 ：0：成功，其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_pin_if_irq(uint32_t pinindex, hal_irq_priorty_et priorty, hal_pin_cbk_t functioncbk);

/**
 * 函 数 介 绍 ：
 *                 中断使能控制
 *
 *  * 参       数
 *                 pinindex ： 引脚索引
 *                 enabled ： 是否使能
 * 返   回  值 ：0：成功，其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_pin_if_irq_enabled(uint32_t pinindex, hal_enabled_et enabled);

#endif
