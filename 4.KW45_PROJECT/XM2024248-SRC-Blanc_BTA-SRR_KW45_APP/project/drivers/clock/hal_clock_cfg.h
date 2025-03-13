/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_clock.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述clock模块配置接口，不建议修改。
 *      文件调用：
 *                  文件应被hal_clock.c调用。
 *                  文件应包含hal_clock_typedef.h。
 * 当前版本：
 * 历史版本：
*/
#ifndef HAL_CLOCK_CFG_H
#define HAL_CLOCK_CFG_H

/*  Include     Begine     */
#include "hal_clock_typedef.h"
/*  Include     End     */

/*  Macro       Begine      */
/**
 * 参数介绍：
 *          时钟参数通过宏定义
 * 关联参数：
 *
 * 备    注：
 *          1.如果值设置为0时，默认功能关闭，不使用
 *          2.单位为频率
 *          3.HAL_CLOCK_TICK_CFG设置应按照1ms及频率使用为1000Hz
 *
*/
// #define HAL_CLOCK_PLL_CFG                   0
// #define HAL_CLOCK_MCU_CFG                   0
// #define HAL_CLOCK_CAN_CFG                   0
// #define HAL_CLOCK_LIN_CFG                   0
// #define HAL_CLOCK_UART_CFG                  0
// #define HAL_CLOCK_SPI_CFG                   0
// #define HAL_CLOCK_IIC_CFG                   0
// #define HAL_CLOCK_ADC_CFG                   0
// #define HAL_CLOCK_DAC_CFG                   0
// #define HAL_CLOCK_TICK_CFG                  0

#define BOARD_XTAL0_CLK_HZ                         32000000U  /*!< Board xtal0 frequency in Hz */
#define BOARD_BOOTCLOCKRUN_CORE_CLOCK              96000000U  /*!< Core clock frequency: 96000000Hz */
#define BOARD_BOOTCLOCKRUN_ROSC_CLOCK                 32768U  /*!< ROSC clock frequency: 32768Hz */



#endif
