//copy by cc
/* 
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_spi_if.c
 * 摘要: 
 * 当前版本: V1.0.0 Chenjianbo 
 * Date: 2023-11-24
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_spi_if.h"
#include "hal_spi.h"
/**
* @brief  ：
 *                 spi初始化
 *
 *                  @param [in] channel          spi通道，模块自行定义通道及其映射关系
 *                  
 *                 
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：该接口主要是给应用层调用，是对底层初始化函数的在封装
 *              
 */
hal_result_t hal_spi_if_init(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_init(channel);
    return result;
}
/**
* @brief  ：
 *                 设置片选信息
 *
 *                 @param [in] channel          SPI通道，模块自行定义通道及其映射关系
 *                 @param [in] level         指定片选脚是高有效还是低有效
 *              
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_if_cs_levle(uint8_t channel, uint8_t level)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_cs_levle(channel, level);
    return result;
}

/**
* @brief  ：
 *
  * @param ：
 *                  @param [in] channel    lin通道，模块自行定义通道及其映射关系
 * @return ：hal_spi_result_et 0:FAIL 1:SUCCESS 2:SLAVE_WAIT_CLOCK
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_spi_result_et hal_spi_if_write(uint8_t channel, uint8_t *sendbuff, uint8_t *recvbuff, uint16_t len)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_write(channel, sendbuff,recvbuff,len);
    return result;
}

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
hal_result_t hal_spi_if_enabled(uint8_t channel, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_enabled(channel, enabled);
    return result;
}

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
hal_result_t hal_spi_if_set_role(uint8_t channel, hal_spi_role_et role_e)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_set_role(channel, role_e);
    return result;
}

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
hal_spi_role_et hal_spi_if_get_role(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_get_role(channel);
    return result;
}

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
hal_result_t hal_spi_if_set_order(uint8_t channel, hal_bit_order_et data_order)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_set_order(channel,data_order);
    return result;
}

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
hal_result_t hal_spi_if_write_abort(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_write_abort(channel);
    return result;
}

/**
* @brief  ：
 *                 spi中断配置
 *
 *     @param ：
 *                  @param [in] channel    SPI通道，模块自行定义通道及其映射关系
 *                  @param [in] isr_type   中断类型，可配置HAL_SPI_ISR_TXE，HAL_SPI_ISR_RXNE，HAL_SPI_ISR_ERR
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_isr_config(uint8_t channel, uint16_t isr_type, hal_enabled_et enabled)
{
	hal_result_t result = HAL_RESULT_FAIL;
	
	return result;
}

/**
* @brief  ：
 *                 spi中断配置
 *
 *                  @param [in] channel    SPI通道，模块自行定义通道及其映射关系
 *                  @param [in] isr_type   中断类型，可配置HAL_SPI_ISR_TXE，HAL_SPI_ISR_RXNE，HAL_SPI_ISR_ERR
 *                  @param [in] enabled    中断使能标签
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_if_isr_config(uint8_t channel, uint16_t isr_type, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_isr_config(channel,isr_type,enabled);
    return result;
}
