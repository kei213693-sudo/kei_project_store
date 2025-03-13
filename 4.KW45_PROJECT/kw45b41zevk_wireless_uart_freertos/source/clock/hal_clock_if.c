/* 
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_clock_if.c
 * 摘要: 
 * 当前版本: V1.0.0 Chenjianbo 
 * Date: 2023-11-23
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_clock_if.h"
#include "hal_clock.h"



/**
 * @brief  初始化所有时钟
 * 
 * @return hal_result_t 
 */
hal_result_t hal_clock_if_init(void)
{
	hal_result_t result = HAL_RESULT_SUCCESS;
	result = hal_clock_init();
    return result;
}
/**
 * @brief  
 *                 设置相关模块状态，使能或关闭对应的时钟
 *
 * @param：
 *         @param      [in] hal_clock_module_t module,需要设置使能的模块  @ref HAL_CLOCK_MODULE_PLL
 *            @param   [in] hal_enabled_t @ref hal_enabled_t 
 * @return :  @ref           HAL_RESULT_SUCCESS
 *                              hal_result_t定义
 * @note
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_clock_if_enabled(hal_clock_module_et module, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_clock_enabled(module, enabled);
//    assert(result != HAL_RESULT_SUCCESS);
    return result;
}
/**
 * @brief  
 *                 获取相关时钟模块状态
 *
 * @param：
 *       @param        [in] @ref hal_clock_module_t module,需要设置使能的模块
 * @return  ： @ref hal_enabled_t
 * @note
 *              调用前必须先完成相关初始化
 */
hal_enabled_et hal_clock_if_getstatus(hal_clock_module_et module)
{
    hal_enabled_et enable = HAL_ENABLED_ENABLE;
    enable = hal_clock_getstatus(module);
    return enable;
}
/**
 * @brief  
 *                 获取tick计数，每 1ms 计数一次
 *
 * @param：
 * @return  ： uint32_t tick计数次数，精度为1ms
 * @note
 *              调用前必须先完成相关初始化
 */
uint32_t hal_clock_if_gettick(void)
{
    uint32_t tick_val = 0;
    tick_val = hal_clock_gettick();
    return tick_val;
}

/**
 * @brief  
 *                  clock初始化函数，里面包括初始化PLL/Tick/MCU/CAN/LIN/IIC/UART/ADC/DAC等
 *              相关时钟的初始化。参数来源为 hal_clock_cfg.h中的宏定义。
 *
 * @param：
 * @return :  @ref           HAL_RESULT_SUCCESS
 * @note
 *              1.tick步长定义为1ms。
 *              2.里面初始化的具体数值应参考hal_clock_cfg.h。一般情况代
 *                码运行过程中禁止改变其值。
 *              3.初始化完成后时钟默认为使能状态，若其他模块需要切换状态
 *                请参考函数：hal_clock_enabled
 */
