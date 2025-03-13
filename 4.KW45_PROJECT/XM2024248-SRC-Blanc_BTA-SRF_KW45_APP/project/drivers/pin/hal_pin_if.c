//copy by cc
/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_pin_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_pin_if.h"
#include "hal_pin.h"

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
 * 函 数 介 绍 ：
 *                 获得pin脚电平值
 *
 *  * 参       数
 *                 pinindex ： 引脚索引
 * 返   回  值 ：0：低电平；1：高电平
 * 备       注 ：
 *
 */
hal_pin_level_et hal_pin_if_read(uint32_t pinindex)
{
    hal_pin_level_et result = HAL_RESULT_SUCCESS;
    result = hal_pin_read(pinindex);
    return result;
}

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
hal_result_t hal_pin_if_write(uint32_t pinindex, hal_pin_level_et level)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pin_write(pinindex, level);
    return result;
}

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
hal_result_t hal_pin_if_irq(uint32_t pinindex, hal_irq_priorty_et priorty, hal_pin_cbk_t functioncbk)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pin_irq(pinindex, priorty, functioncbk);
    return result;
}

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
hal_result_t hal_pin_if_irq_enabled(uint32_t pinindex, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pin_irq_enabled(pinindex, enabled);
    return result;
}
