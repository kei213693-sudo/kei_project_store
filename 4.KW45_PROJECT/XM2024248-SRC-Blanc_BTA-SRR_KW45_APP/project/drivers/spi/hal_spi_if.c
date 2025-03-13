//copy by cc
/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_spi_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_spi_if.h"
#include "hal_spi.h"
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
hal_result_t hal_spi_if_init(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_init(channel);
    return result;
}
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
hal_result_t hal_spi_if_cs_levle(uint8_t channel, uint8_t level)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_cs_levle(channel, level);
    return result;
}

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
hal_spi_result_et hal_spi_if_write(uint8_t channel, uint8_t *sendbuff, uint8_t *recvbuff, uint16_t len)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_write(channel, sendbuff,recvbuff,len);
    return result;
}

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
hal_result_t hal_spi_if_enabled(uint8_t channel, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_enabled(channel, enabled);
    return result;
}

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
hal_result_t hal_spi_if_set_role(uint8_t channel, hal_spi_role_et role_e)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_set_role(channel, role_e);
    return result;
}

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
hal_spi_role_et hal_spi_if_get_role(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_get_role(channel);
    return result;
}

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
hal_result_t hal_spi_if_set_order(uint8_t channel, hal_bit_order_et data_order)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_set_order(channel,data_order);
    return result;
}

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
hal_result_t hal_spi_if_write_abort(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_write_abort(channel);
    return result;
}

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
hal_result_t hal_spi_if_isr_config(uint8_t channel, uint16_t isr_type, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_spi_isr_config(channel,isr_type,enabled);
    return result;
}
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
void hal_spi_if_deinit(void)
{
	hal_spi_deinit();
}

