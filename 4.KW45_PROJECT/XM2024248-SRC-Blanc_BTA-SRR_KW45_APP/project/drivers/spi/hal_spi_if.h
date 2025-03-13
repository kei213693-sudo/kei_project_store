//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_spi_if.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述spi模块抽象层接口，不建议修改函数接口。
 *      文件调用：
 *                  文件应被应用调用，不应被hal层调用。如果hal层内部需要调用请参考
 *              文件hal_spi.h
 *                  文件应包含hal_spi_typedef.h。
 * 当前版本：
 * 历史版本：
 */
#ifndef HAL_SPI_IF_H
#define HAL_SPI_IF_H

/*  Include     Begine      */
#include "hal_spi_typedef.h"

/*  Include     End         */


/**
 * 函 数 介 绍 ：
 *                 spi初始化
 *
 *  * 参       数
 *                 channel：spi通道
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_if_init(uint8_t channel);
/**
 * 函 数 介 绍 ：
 *                 spi片选电平设置
 *
 *  * 参       数
 *              channel：spi通道
 *              level： 电平值
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_if_cs_levle(uint8_t channel, uint8_t level);
/**
 * 函 数 介 绍 ：
 *                 spi发送数据函数
 *
 *  * 参       数
 *                 channel：spi通道
 *                  sendbuff：发送缓冲区
 *                  recvbuff：接收缓冲区
 *                  len：数据长度
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_spi_result_et hal_spi_if_write(uint8_t channel, uint8_t *sendbuff, uint8_t *recvbuff, uint16_t len);

/**
 * 函 数 介 绍 ：
 *                 spi使能函数
 *
 *  * 参       数
 *                 channel：spi通道
 *                  enabled：使能状态
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_if_enabled(uint8_t channel, hal_enabled_et enabled);

/**
 * 函 数 介 绍 ：
 *                 spi主从模式设置
 *
 *  * 参       数
 *                 channel：spi通道
 *                  role_e：spi模式
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_if_set_role(uint8_t channel, hal_spi_role_et role_e);

/**
 * 函 数 介 绍 ：
 *                 获取spi模式
 *
 *  * 参       数
 *                 channel：spi通道
 * 返   回  值 ：0：主模式；1：从模式
 * 备       注 ：
 *
 */
hal_spi_role_et hal_spi_if_get_role(uint8_t channel);

/**
 * 函 数 介 绍 ：
 *                 SPI发送字节序设置
 *
 *  * 参       数
 *                 channel：spi通道
 *                  data_order：0：lsb；1：msb
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_if_set_order(uint8_t channel, hal_bit_order_et data_order);

/**
 * 函 数 介 绍 ：
 *                 spi停止发送数据
 *
 *  * 参       数
 *                 channel：spi通道
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_if_write_abort(uint8_t channel);
/**
 * 函 数 介 绍 ：
 *                 spi中断配置
 *
 *  * 参       数
 *                 channel：spi通道
 *                  isr_type:中断类型
 *                  enabled：使能状态
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_if_isr_config(uint8_t channel, uint16_t isr_type, hal_enabled_et enabled);
/**
 * 函 数 介 绍 ：
 *                 spi去初始化
 *
 *  * 参       数
 *
 * 返   回  值 ：
 * 备       注 ：
 *
 */
void hal_spi_if_deinit(void);
/*  Function    End         */

#endif
