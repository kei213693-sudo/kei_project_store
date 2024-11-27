/**
 ****************************************************************************************************
 * @file        atk_ble03.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-06-01
 * @brief       ATK-BLE03模块驱动代码
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

#ifndef __ATK_BLE03_H
#define __ATK_BLE03_H

#include "sys.h"
#include "atk_ble03_uart.h"

/* 引脚定义 */
#define ATK_BLE03_LDO_GPIO_PORT             GPIOC
#define ATK_BLE03_LDO_GPIO_PIN              GPIO_PIN_4
#define ATK_BLE03_LDO_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

#define ATK_BLE03_STA_GPIO_PORT            GPIOA
#define ATK_BLE03_STA_GPIO_PIN             GPIO_PIN_4
#define ATK_BLE03_STA_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

/* IO操作 */
#define ATK_BLE03_READ_STA()                HAL_GPIO_ReadPin(ATK_BLE03_STA_GPIO_PORT, ATK_BLE03_STA_GPIO_PIN)
#define ATK_BLE03_LDO(x)                   do{ x ?                                                                                     \
                                                HAL_GPIO_WritePin(ATK_BLE03_LDO_GPIO_PORT, ATK_BLE03_LDO_GPIO_PIN, GPIO_PIN_SET) :    \
                                                HAL_GPIO_WritePin(ATK_BLE03_LDO_GPIO_PORT, ATK_BLE03_LDO_GPIO_PIN, GPIO_PIN_RESET);   \
                                            }while(0)

/* 连接状态枚举 */
typedef enum
{
    ATK_BLE03_CONNECTED = 0x00,             /* 已连接 */
    ATK_BLE03_DISCONNECTED,                 /* 未连接 */
} atk_ble03_conn_sta_t;

/* 波特率范围 */
#define ATK_BLE03_BAUD_MIN                  1
#define ATK_BLE03_BAUD_MAX                  16

/* 串口波特率枚举 */
typedef enum
{

    ATK_BLE03_UART_BAUDRATE_2400   = 0x01,                   /* 2400    */
    ATK_BLE03_UART_BAUDRATE_4800,                            /* 4800    */
    ATK_BLE03_UART_BAUDRATE_9600,                            /* 9600    */
    ATK_BLE03_UART_BAUDRATE_19200,                           /* 19200   */
    ATK_BLE03_UART_BAUDRATE_31250,                           /* 31250   */
    ATK_BLE03_UART_BAUDRATE_38400,                           /* 38400   */
    ATK_BLE03_UART_BAUDRATE_57600,                           /* 57600   */
    ATK_BLE03_UART_BAUDRATE_115200,                          /* 115200  */
    ATK_BLE03_UART_BAUDRATE_230400,                          /* 230400  */
    ATK_BLE03_UART_BAUDRATE_256000,                          /* 256000  */
    ATK_BLE03_UART_BAUDRATE_460800,                          /* 460800  */
    ATK_BLE03_UART_BAUDRATE_512000,                          /* 512000  */
    ATK_BLE03_UART_BAUDRATE_750000,                          /* 750000  */
    ATK_BLE03_UART_BAUDRATE_800000,                          /* 800000  */
    ATK_BLE03_UART_BAUDRATE_921600,                          /* 921600  */
    ATK_BLE03_UART_BAUDRATE_1000000,                         /* 1000000 */
} atk_ble03_uart_baudrate_t;


/* 错误代码 */
#define ATK_BLE03_EOK      0                /* 没有错误 */
#define ATK_BLE03_ERROR    1                /* 错误 */
#define ATK_BLE03_ETIMEOUT 2                /* 超时错误 */
#define ATK_BLE03_EINVAL   3                /* 参数错误 */

/* 操作函数 */
void atk_ble03_hw_init(void);/* ATK-BLE03初始化 */
atk_ble03_conn_sta_t atk_ble03_get_conn_sta(void);/* 获取ATK-BLE03连接状态 */
uint8_t atk_ble03_send_at_cmd(char *cmd, char *ack, uint32_t timeout);/* ATK-BLE03发送AT指令 */

#endif
