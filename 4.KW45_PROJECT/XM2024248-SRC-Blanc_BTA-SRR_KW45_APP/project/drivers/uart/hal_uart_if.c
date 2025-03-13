/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_uart_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_uart_if.h"
#include "hal_uart.h"
/*
 * 函 数 介 绍: UART初始化，设置串口的波特率、数据位、停止位、校验位等参数,
 * 参       数
 *              [in]channel                         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注: 该接口主要给应用层调用，是对底层初始化函数的再次封装
 */
hal_result_t hal_uart_if_init(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_uart_init(channel);
    return result;
}
/*
 * 函 数 介 绍:UART功能使能
 * 参       数 {
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]enabled         串口使能标签}
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_if_enabled(uint8_t channel, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_uart_enabled(channel, enabled);
    return result;
}
/*
 * 函 数 介 绍:UART功能使能
 * 参       数 {
}
 *
 * 函 数 介 绍: 获取串口传输状态
 * 参       数
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 * 返   回  值: 返回传输状态，状态详见hal_uart_trans_status_et
 * 备       注:
 */
hal_uart_trans_status_et hal_uart_if_get_status(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_uart_get_status(channel);
    return result;
}
/*
 * 函 数 介 绍: UART 发送一个字节
 * 参       数
 *              [in]channel             串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]byte                发送的字节
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_if_writebyte(uint8_t channel, uint8_t byte)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_uart_writebyte(channel, byte);
    return result;
}
/*
 * 函 数 介 绍: UART发送字节流
 * 参       数
 *              [in]channel                 串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]buffer                  字节流
 *              [in]len                     字节流长度
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_if_writebuff(uint8_t channel, const uint8_t *buffer, uint16_t len)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_uart_writebuff(channel, buffer, len);
    return result;
}
/*
 * 函 数 介 绍: 串口接收回调函数设置
 * 参       数
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]rx_func         接收回调函数}
 * 返   回  值: {*}
 * 备       注:
 */
void hal_uart_if_rx_cbk(uint8_t channel, hal_uart_rxcbk rx_func)
{
    hal_uart_rx_cbk(channel, rx_func);
}
/*
 * 函 数 介 绍: 串口发送完成回调函数配置
 * 参       数
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]tx_func         发送完成回调函数}
 * 返   回  值: {*}
 * 备       注: 串口发送中断调用此函数接口
 */
void hal_uart_if_txdonecbk(uint8_t channel, hal_uart_txdonecbk tx_func)
{
    hal_uart_tx_done_cbk(channel, tx_func);
}
/*
 * 函 数 介 绍:停止串口发送
 * 参       数 {channel             串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0}
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_if_tx_abort(uint8_t channel)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_uart_tx_abort(channel);
    return result;
}
/*
 * 函 数 介 绍: 串口中断配置
 * 参       数
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]isr_type        串口波特率，详见hal_uart_baudrate_et[hal_uart_typedef.h]
 *              [in]enabled         中断使能标签
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_if_isr_config(uint8_t channel, hal_uart_isr_type_et isr_type, hal_irq_priorty_et priorty, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_uart_isr_config(channel, isr_type, priorty, enabled);
    return result;
}
/**
 * 函 数 介 绍 ：
 *               uart去初始化函数
 *
 * 参       数 ：
 *
 * 返   回  值 ：
 * 备       注 ：
 *
 */
void hal_uart_if_deinit(void)
{
	hal_uart_deinit();
}
