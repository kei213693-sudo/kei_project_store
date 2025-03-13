/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_clock_typedef.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述clock模块专用自定义类型，需要根据实际情况判定是否是要需改。
 *      文件调用：
 *                  文件应在模块内部产生直接调用，外部使用需通过hal_clock_if.h 或 hal_clock.h
 *              间接调用。
 *                  文件应包含hal_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

#ifndef HAL_CLOCK_TYPEDEF_H
#define HAL_CLOCK_TYPEDEF_H

/*  Include     Begine      */
#include "hal_typedef.h"
/*  Include     End         */

/*  Base Type  Begine      */

/*  Base Type  End         */

/*  Macro       Begine      */

/*  Macro       End         */

/*  Enum        Begine      */

/*  Enum        End         */

/*  Struct/union Begine      */
/**
 * 参数介绍：
 *          时钟控制配置结构体，用于存放时钟配置初始值
 * 关联参数：
 *
 * 备    注：
 *
 */
typedef struct
{
    uint32_t m_u32_clockpll;
    uint32_t m_u32_clockmcu;
    uint32_t m_u32_clockcan;
    uint32_t m_u32_clocklin;
    uint32_t m_u32_clockspi;
    uint32_t m_u32_clockiic;
    uint32_t m_u32_clockuart;
    uint32_t m_u32_clocktick;
} hal_clock_cfg_st;

typedef enum
{
    HAL_CLOCK_MODULE_PLL = 0,
    HAL_CLOCK_MODULE_MCU,
    HAL_CLOCK_MODULE_TICK,
    HAL_CLOCK_MODULE_CAN,
    HAL_CLOCK_MODULE_SPI,
    HAL_CLOCK_MODULE_IIC,
    HAL_CLOCK_MODULE_UART,
    HAL_CLOCK_MODULE_LIN,
    HAL_CLOCK_MODULE_ADC,
    HAL_CLOCK_MODULE_DAC
} hal_clock_module_et;
/*  Struct/union End         */

#endif