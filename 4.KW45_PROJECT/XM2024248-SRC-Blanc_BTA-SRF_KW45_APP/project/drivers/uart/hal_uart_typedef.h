/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_uart_typedef.h
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#ifndef HAL_UART_TYPEDEF_H
#define HAL_UART_TYPEDEF_H
#include "hal_typedef.h"


/**
 * 串口奇偶校验配置
 * HAL_UART_PARITY_NO                   无奇偶校验
 * HAL_UART_PARITY_EVEN                 偶校验
 * HAL_UART_PARITY_ODD                  奇校验
*/
typedef enum
{
    HAL_UART_PARITY_NO = 0,
    HAL_UART_PARITY_EVEN,
    HAL_UART_PARITY_ODD

}hal_uart_parity_et;

/**
 * uart硬件通道映射
 * HAL_UART_CHL0            uart硬件通道0映射
*/
typedef enum
{
    HAL_UART_CHL0,
	HAL_UART_CHL1,
    HAL_UART_CHL_MAX
}hal_uart_channel_et;
/**
 * UART传输状态
 * HAL_UART_TRANS_STA_NO_INIT              未初始化状态
 * HAL_UART_TRANS_STA_NO_START             未开始状态
 * HAL_UART_TRANS_STA_SENDING              传输中
 * HAL_UART_TRANS_STA_RX_DONE              接收完成
 * HAL_UART_TRANS_STA_TX_DONE              发送完成
 */
typedef enum
{
    HAL_UART_TRANS_STA_NO_INIT,
    HAL_UART_TRANS_STA_NO_START,
    HAL_UART_TRANS_STA_SENDING,
    HAL_UART_TRANS_STA_RX_DONE,
    HAL_UART_TRANS_STA_TX_DONE
} hal_uart_trans_status_et;
/**
 * UART传输错误类型
 * HAL_UART_ERR_TYPE_ERR_TX                 发送错误
 * HAL_UART_ERR_TYPE_ERR_RX                 接收错误
 *
*/
typedef enum
{
    HAL_UART_ERR_TYPE_ERR_TX,
    HAL_UART_ERR_TYPE_ERR_RX
} hal_uart_error_type_et;
/**
 * 波特率设置
 * HAL_UART_BAUDRATE_0                 指定波特率为9600
 * HAL_UART_BAUDRATE_1                 指定波特率为115200
*/
typedef enum
{
    HAL_UART_BAUDRATE_9600 = 0,
    HAL_UART_BAUDRATE_115200 = 1
} hal_uart_baudrate_et;

/***
 * 串口中断类型
 * HAL_UART_ISR_TYPE_PE                 奇偶错误中断
 * HAL_UART_ISR_TYPE_TXE                发送中断
 * HAL_UART_ISR_TYPE_TC                 传输完成中断
 * HAL_UART_ISR_TYPE_RXNE               接收中断
 * HAL_UART_ISR_TYPE_IDLE               空闲总线中断
 * HAL_UART_ISR_TYPE_ERR                错误中断
 */
typedef enum
{
    HAL_UART_ISR_TYPE_PE = 0,
    HAL_UART_ISR_TYPE_TXE,
    HAL_UART_ISR_TYPE_TC,
    HAL_UART_ISR_TYPE_RXNE,
    HAL_UART_ISR_TYPE_IDLE,
    HAL_UART_ISR_TYPE_ERR
}hal_uart_isr_type_et;
/*接收回调函数*/
typedef void (*hal_uart_rxcbk)(uint8_t *data, uint16_t len);
/*发送完成回调函数 */
typedef void (*hal_uart_txdonecbk)(void);

#endif
