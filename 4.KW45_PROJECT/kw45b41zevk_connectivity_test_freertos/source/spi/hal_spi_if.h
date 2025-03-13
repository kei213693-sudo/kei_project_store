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
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */
#ifndef HAL_spi_IF_H
#define HAL_spi_IF_H

/*  Include     Begine      */
#include "hal_spi_typedef.h"

/*  Include     End         */

/*  Typedef End */

/*  Function    Begine      */
/**
* @brief  ：
 *                 spi初始化
 *
  * @param ：
 *                  @param [in] channel          spi通道，模块自行定义通道及其映射关系
 *                  
 *                 
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：该接口主要是给应用层调用，是对底层初始化函数的在封装
 *              
 */
hal_result_t hal_spi_if_init(uint8_t channel);
/**
* @brief  ：
 *                 设置片选信息
 *
  * @param ：
 *                  @param [in] channel          SPI通道，模块自行定义通道及其映射关系
 *                  [in] level         指定片选脚是高有效还是低有效
 *              
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_if_cs_levle(uint8_t channel, uint8_t level);
/**
* @brief  ：
 *                 spi cs level
 *
  * @param ：
 *                  @param [in] channel    lin通道，模块自行定义通道及其映射关系
 *                  [in] mode
 * @return ：hal_spi_result_et 0:FAIL 1:SUCCESS 2:SLAVE_WAIT_CLOCK
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_spi_result_et hal_spi_if_write(uint8_t channel, uint8_t *sendbuff, uint8_t *recvbuff, uint16_t len);

/**
* @brief  ：
 *                 spi模块使能
 *
  * @param ：
 *                  @param [in] channel    spi通道，模块自行定义通道及其映射关系
 *                  @param [in] enabled    是否使能
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_if_enabled(uint8_t channel, hal_enabled_et enabled);

/**
* @brief  ：
 *                 spi设置主从
 *
  * @param ：
 *                  @param [in] channel    SPI通道，模块自行定义通道及其映射关系
 *                  [in] role_e     主从机模式
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_if_set_role(uint8_t channel, hal_spi_role_et role_e);

/**
* @brief  ：
 *                 spi获取当前角色
 *
  * @param ：
 *                  @param [in] channel    SPI通道，模块自行定义通道及其映射关系
 *                 
 * @return ：hal_spi_role_et = HAL_SPI_ROLE_MASTER:主机模式， = HAL_SPI_ROLE_SLAVE ：从机模式
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_spi_role_et hal_spi_if_get_role(uint8_t channel);

/**
* @brief  ：
 *                 设置发数据时高位先发还是低位先发
 *
  * @param ：
 *                  @param [in] channel        SPI通道，模块自行定义通道及其映射关系
 *                  [in] data_order     高位先发，还是低位先发
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_if_set_order(uint8_t channel, hal_bit_order_et data_order);

/**
* @brief  ：
 *                 spi停止写数据
 *
  * @param ：
 *                  @param [in] channel    SPI通道，模块自行定义通道及其映射关系
 *                  
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_if_write_abort(uint8_t channel);
/**
* @brief  ：
 *                 spi中断配置
 *
 *     @param ：
 *                  @param [in] channel    SPI通道，模块自行定义通道及其映射关系
 *                  @param [in] isr_type   中断类型，可配置HAL_SPI_ISR_TXE，HAL_SPI_ISR_RXNE，HAL_SPI_ISR_ERR
 *                  @param [in] enabled    中断使能标签
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_if_isr_config(uint8_t channel, uint16_t isr_type, hal_enabled_et enabled);
/*  Function    End         */

#endif
