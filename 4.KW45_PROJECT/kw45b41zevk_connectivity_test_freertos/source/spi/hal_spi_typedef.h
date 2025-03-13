//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_spi_typedef.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述spi模块专用自定义类型，需要根据实际情况判定是否是要需改。
 *      文件调用：
 *                  文件应在模块内部产生直接调用，外部使用需通过hal_spi_if.h 或 hal_spi.h
 *              间接调用。
 *                  文件应包含hal_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

#ifndef HAL_spi_TYPEDEF_H
#define HAL_spi_TYPEDEF_H

/*  Include     Begine      */
#include "hal_typedef.h"
/*  Include     End         */

/*  Typedef Begine */

/**
 * SPI通道映射
 * HAL_SPI_CHL0             SPI硬件通道0映射
*/
typedef enum
{
    HAL_SPI_CHL0 = 0,
	HAL_SPI_CHL1 = 1,
    HAL_SPI_CHL_MAX
}hal_spi_channel_et;
/**
 * 返回结果类型
 * 
*/
typedef enum
{
    HAL_SPI_RESULT_FAIL = 0,
    HAL_SPI_RESULT_SUCCESS,
    HAL_SPI_RESULT_SLAVE_WAIT_CLOCK,
} hal_spi_result_et;

/**
 *  
 *  CPHA：决定数据的采样方式
 *   HAL_SPI_CPOL=0时，  CPHA=0，用上升沿采数；

 *               CPHA=1，用下降沿采数。

 *   HAL_SPI_CPOL=1时，CPHA=0，用下降沿采数；

 *               CPHA=1，用上升沿采数。
 *  CPHA=0：表示用第一个时钟沿采样；sample on the leading clock edge

 *   CPHA=1：表示用第二个时钟沿采样；sample on the trailing clock edge
 *
 *  如此即引申出一个隐含的要求：CPHA=0时，在第一个时钟沿来到之前，数据(和片选)必须在半个周期前已保持稳定。
 *
 *   SPI模式      CPOL     CPHA         空闲时SCL时钟            采样时刻
 *     0          0         0             低电平                奇数边沿采样，1、3、5......
 *     1          0         1             低电平                偶数边沿采样，2、4、6......
 *     2          1         0             高电平                奇数边沿采样，1、3、5......
 *     3          1         1             高电平                偶数边沿采样，2、4、6......
 * 
*/
typedef enum
{
    HAL_SPI_MODE_CPOL0_CPHA0 = 0,
    HAL_SPI_MODE_CPOL0_CPHA1,
    HAL_SPI_MODE_CPOL1_CPHA0,
    HAL_SPI_MODE_CPOL1_CPHA1,
} hal_spi_mode_et;

/**
 * SPI外设模式定义
 * HAL_SPI_ROLE_MASTER                       当前节点作为主机
 * HAL_SPI_ROLE_SLAVE                        当前节点作为从机 
*/
typedef enum
{
    HAL_SPI_ROLE_MASTER = 0,
    HAL_SPI_ROLE_SLAVE,
} hal_spi_role_et;



/*  Base Type  Begine      */

/*  Base Type  End         */

/*  Macro       Begine      */

/*  Macro       End         */

/*  Enum        Begine      */

/*  Enum        End         */

/*  Struct/union Begine      */

/*  Struct/union End         */

#endif
