/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_uart_cfg.h
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#ifndef HAL_UART_CFG_H
#define HAL_UART_CFG_H
#include "hal_uart_typedef.h"

#define HAL_UART_CHANNEL0               (0)
#define HAL_UART_CHANNEL1				(1)
#define HAL_UART_CHANNEL_NUM            (2)


#define HAL_UART_MODE_RX                 (1)
#define HAL_UART_MODE_TX                 (2)
/**
 * UART初始化配置
 * m_baundrate_e                指定波特率
 * m_u8_parity                  指定有无奇偶校验,详见hal_uart_parity_et
 * m_u8_uart_mode               指定串口收发模式，可以配置HAL_UART_MODE_RX、HAL_UART_MODE_TX
 * m_bit_order_e                指定发数据先发高位还是低位
 * m_enable_e                   指定串口功能使能
 *
*/
typedef struct
{
    hal_uart_baudrate_et m_baundrate_e;
    hal_uart_parity_et m_u8_parity;
    uint8_t m_u8_uart_mode;
    hal_bit_order_et m_bit_order_e;
    hal_enabled_et m_enable_e;

} hal_uart_cfg_st;
/**
 * 函 数 介 绍 ：
 *                 获得UART配置表函数
 *
 *  * 参       数
 *                 uartcfg：配置表指针
 * 					len：：长度
 * 返   回  值
 * 备       注 ：
 *
 */
void hal_uart_get_cfg(hal_uart_cfg_st **uartcfg, uint32_t *len);

#endif
