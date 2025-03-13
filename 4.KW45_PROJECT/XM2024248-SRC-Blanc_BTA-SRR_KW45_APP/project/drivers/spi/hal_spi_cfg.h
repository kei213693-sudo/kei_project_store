//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_spi.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述xx模块配置接口，不建议修改。
 *      文件调用：
 *                  文件应被hal_xx.c调用。
 *                  文件应包含hal_xx_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */
#ifndef HAL_SPI_CFG_H
#define HAL_SPI_CFG_H

/*  Include     Begine     */
#include "hal_spi_typedef.h"

/*SPI硬件通道0映射*/
#define HAL_SPI_CHANNEL0            (0U)
#define HAL_SPI_CHANNEL1			  (1U)
/*芯片支持的SPI通道数*/
#define HAL_SPI_CHANNEL_NUM         (2U)

/*配置SPI一帧数据的长度，可配置参数有8bit和16bit*/
#define HAL_SPI_DATASIZE            (8U)
/*配置片选线高有效还是低有效，0：低有效，1：高有效*/
#define HAL_SPI_CSLEVEL             (1U)
/*SPI时钟频率设置，单位hz*/
#define HAL_SP_CLOCKHZ              (1000000U)

/*发送完成中断*/
#define HAL_SPI_ISR_TXE             (0)
/*接收缓存非空中断*/
#define HAL_SPI_ISR_RXNE            (1)
/*错误中断*/
#define HAL_SPI_ISR_ERR             (2)

/*  Include     End     */
/**
 * SPI初始化配置
 * m_spi_sampmode_e                 指定SPI采样方式
 * m_spi_role_e                     指定该节点是主机还是从机
 * m_spi_data_order_e               指定高位先传输，还是低位先传输
 * m_enabled_e                      指定SPI功能是否使能
 * m_u8_datasize                    指定SPI一帧数据的长度，可配置参数为8位或者16位,由HAL_SPI_DATASIZE宏定义决定
 * m_u8_cs_level                    指定片选脚是高电平1有效，还是低电平0有效，由HAL_SPI_CSLEVEL宏定义决定
 * m_u32_spi_clockhz                指定SPI时钟频率,由宏定义HAL_SP_CLOCKHZ决定
*/
typedef struct
{
    hal_spi_mode_et m_spi_mode_e;
    hal_spi_role_et m_spi_role_e;
    hal_bit_order_et m_spi_data_order_e;
    hal_enabled_et m_enabled_e;
    uint8_t m_u8_datasize;
    uint8_t m_u8_cs_level;
    uint32_t m_u32_spi_clockhz;
}hal_spi_cfg_st;

/*SPI初始化数据配置结构体数组定义*/
//const hal_spi_cfg_st g_spi_cfg_sa[HAL_SPI_CHANNEL_NUM];
/*  Function    Begine     */
/**
 * 函 数 介 绍 ：
 *                 获得SPI配置表函数
 *
 *  * 参       数
 *                 spicfg：配置表指针
 * 					len：：长度
 * 返   回  值
 * 备       注 ：
 *
 */
void hal_spi_get_cfg(hal_spi_cfg_st **spicfg, uint32_t *len);
/*  Function    End     */

#endif
