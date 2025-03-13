/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_clock_if.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述clock模块抽象层接口，不建议修改函数接口。
 *      文件调用：
 *                  文件应被应用调用，不应被hal层调用。如果hal层内部需要调用请参考
 *              文件hal_clock.h
 *                  文件应包含hal_clock_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
*/
#ifndef HAL_CLOCK_IF_H
#define HAL_CLOCK_IF_H

/*  Include     Begine      */
#include "hal_clock_typedef.h"
/*  Include     End         */

/**
 * @brief  
 *                 设置相关模块状态，使能或关闭对应的时钟
 *
 * @param：
 *              [in] hal_clock_module_t module,需要设置使能的模块
 *                           HAL_CLOCK_MODULE_PLL = 0,
 *                           HAL_CLOCK_MODULE_MCU    ,
 *                           HAL_CLOCK_MODULE_TICK   ,
 *                           HAL_CLOCK_MODULE_CAN    ,
 *                           HAL_CLOCK_MODULE_SPI    ,
 *                           HAL_CLOCK_MODULE_IIC    ,
 *                           HAL_CLOCK_MODULE_UART   ,
 *                           HAL_CLOCK_MODULE_LIN    ,
 *                           HAL_CLOCK_MODULE_ADC    ,
 *                           HAL_CLOCK_MODULE_DAC
 *              [in] hal_enabled_t enabled,
 *                           HAL_ENABLED_DISABLE     =   0,
 *                           HAL_ENABLED_ENABLE
 @return :  @ref           HAL_RESULT_SUCCESS
 *                              hal_result_t定义
 * @note
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_clock_if_enabled(hal_clock_module_et module, hal_enabled_et enabled);
/**
 * @brief  
 *                 获取相关时钟模块状态
 *
 * @param：
 *              [in] hal_clock_module_t module,需要设置使能的模块
 *                           HAL_CLOCK_MODULE_PLL = 0,
 *                           HAL_CLOCK_MODULE_MCU    ,
 *                           HAL_CLOCK_MODULE_TICK   ,
 *                           HAL_CLOCK_MODULE_CAN    ,
 *                           HAL_CLOCK_MODULE_SPI    ,
 *                           HAL_CLOCK_MODULE_IIC    ,
 *                           HAL_CLOCK_MODULE_UART   ,
 *                           HAL_CLOCK_MODULE_LIN    ,
 *                           HAL_CLOCK_MODULE_ADC    ,
 *                           HAL_CLOCK_MODULE_DAC
 *
  * @return  ：hal_enabled_t
 *                           HAL_ENABLED_DISABLE     =   0,
 *                           HAL_ENABLED_ENABLE
 * @note
 *              调用前必须先完成相关初始化
 */
hal_enabled_et hal_clock_if_getstatus(hal_clock_module_et module);
/**
 * @brief  
 *                 获取tick计数，每 1ms 计数一次
 *
 * @param：
  * @return  ： uint32_t tick计数次数，精度为1ms
 * @note
 *              调用前必须先完成相关初始化
 */
uint32_t hal_clock_if_gettick(void);

/**
 * @brief  
 *                  clock初始化函数，里面包括初始化PLL/Tick/MCU/CAN/LIN/IIC/UART/ADC/DAC等
 *              相关时钟的初始化。参数来源为 hal_clock_cfg.h中的宏定义。
 *
 * @param：
 @return :  @ref           HAL_RESULT_SUCCESS
 * @note
 *              1.tick步长定义为1ms。
 *              2.里面初始化的具体数值应参考hal_clock_cfg.h。一般情况代
 *                码运行过程中禁止改变其值。
 *              3.初始化完成后时钟默认为使能状态，若其他模块需要切换状态
 *                请参考函数：hal_clock_enabled
 */
hal_result_t hal_clock_if_init(void);

/*  Function    Begine      */

/*  Function    End         */

#endif