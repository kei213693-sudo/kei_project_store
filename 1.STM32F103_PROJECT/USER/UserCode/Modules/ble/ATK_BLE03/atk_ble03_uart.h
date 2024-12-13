/**
 ****************************************************************************************************
 * @file        atk_ble03_uart.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-06-01
 * @brief       ATK-BLE03模块UART接口驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 MiniSTM32 V4开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATK_BLE03_UART_H
#define __ATK_BLE03_UART_H

#include "sys.h"
#include "timer.h"

/* 引脚定义 */
#define ATK_BLE03_UART_TX_GPIO_PORT         GPIOC
#define ATK_BLE03_UART_TX_GPIO_PIN          GPIO_PIN_12
#define ATK_BLE03_UART_TX_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

#define ATK_BLE03_UART_RX_GPIO_PORT         GPIOD
#define ATK_BLE03_UART_RX_GPIO_PIN          GPIO_PIN_2
#define ATK_BLE03_UART_RX_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)

#define ATK_BLE03_UART_INTERFACE            UART5
#define ATK_BLE03_UART_IRQn                 UART5_IRQn
#define ATK_BLE03_UART_IRQHandler           UART5_IRQHandler
#define ATK_BLE03_UART_CLK_ENABLE()         do{ __HAL_RCC_UART5_CLK_ENABLE(); }while(0)

/* UART收发缓冲大小 */
#define ATK_BLE03_UART_RX_BUF_SIZE          512//ble5.1 mtu最多扩展到512
#define ATK_BLE03_UART_TX_BUF_SIZE          128

#define ATK_BLE03_UART_RX_IRQ_TIMEOUT       ((5*1000)/TIMR_TIM3_BASE_US)//rx irq超时时间，5ms


/* 操作函数 */
void atk_ble03_uart_init(uint32_t baudrate);    /* ATK-BLE03 UART初始化 */
void ATK_BLE03_UART_RxCpltCallback(void);
void ATK_BLE03_UART_RxIrqCheck(void);
void atk_ble03_uart_rx_restart(void);           /* ATK-BLE03 UART重新开始接收数据 */
uint8_t *atk_ble03_uart_rx_get_frame(void);     /* 获取ATK-BLE03 UART接收到的一帧数据 */
uint16_t atk_ble03_uart_rx_get_frame_len(void); /* 获取ATK-BLE03 UART接收到的一帧数据的长度 */

int32_t atk_ble03_uart_tx(uint8_t *data, uint16_t len, uint32_t mstime);
int32_t atk_ble03_uart_rx(uint8_t *pData, uint16_t *pLen);

//下面由AT指令调用，字符串格式处理
void atk_ble03_uart_printf(char *fmt, ...);     /* ATK-BLE03 UART printf *///不用这个

#endif
