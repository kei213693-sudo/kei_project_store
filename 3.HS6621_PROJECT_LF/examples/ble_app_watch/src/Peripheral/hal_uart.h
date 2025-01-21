#ifndef  _HAL_UART_H_
#define  _HAL_UART_H_
/*******************************头文件*******************************/
#include <stdint.h>
#include "hal_timer.h"

/*******************************宏定义*******************************/
//log调试日志串口
#define HAL_UART_LOG_TYPE               HS_UART0
#define HAL_UART_LOG_TX_PIN             5
#define HAL_UART_LOG_RX_PIN             6
#define HAL_UART_LOG_BAUDRATE           115200
#define HAL_UART_LOG_LEN    	        (32)
#define HAL_UART_LOG_RX_IRQ_TIMEOUT     (3)//rx irq超时时间，5ms		//115200传输一个bytes需要8/115200 = 69.4us的时间，所以定时1ms没有数据传来就判定一次数据传输完成

//MCU通信串口
#define HAL_UART_FM33LG_TYPE            HS_UART1//uart1支持DMA、16深度FIFO，uart0都不支持
#define HAL_UART_FM33LG_TX_PIN          10
#define HAL_UART_FM33LG_RX_PIN          9
#define HAL_UART_FM33LG_BAUDRATE        115200
#define HAL_UART_FM33LG_LEN    	        (128)//tx+rx最大数据长度，确保 >= PROTOCOL_BUFF_SIZE
#define HAL_UART_FM33LG_RX_IRQ_TIMEOUT  (12)//rx irq超时时间，128bytes以115200传输耗时11ms，所以这里定义12ms
#define HAL_UART_FM33LG_DMA             (0)//1启动DMA模式

#define HAL_UART_TIME_TYPE              (1)//1用外设timer，0用ble stack的定时任务

/*******************************结构体、枚举等******************************/
typedef struct
{
    uint8_t finsh;/*收完成标志*/
    uint8_t res[3];
    uint32_t irqCount;//中断计数标志
    uint16_t rxLen;/*接收长度*/
    uint8_t  rxBuf[HAL_UART_LOG_LEN];/* 帧接收区域 */
}hal_uart_log_ctl_t;

typedef struct
{
    uint8_t finsh;/*收完成标志*/
    uint8_t res[3];
    uint32_t irqCount;//中断计数标志
    uint16_t rxLen;/*接收长度*/
    uint16_t txLen;/*发送长度*/
    uint8_t  rxBuf[HAL_UART_FM33LG_LEN];/* 帧接收区域 */
    uint8_t  txBuf[HAL_UART_FM33LG_LEN];/* 帧发送区域 */
}hal_uart_fm33lg_ctl_t;

/*******************************全局变量的外部声明extern*******************************/

/*******************************全局函数的声明*******************************/
void hal_uart_log_init(void);
#if (HAL_UART_TIME_TYPE)
void hal_uart_log_RxIrqCheck(void);
#endif
uint8_t *hal_uart_log_rx_buf_addr_get(void);
uint16_t hal_uart_log_rx_buf_len_get(void);
void hal_uart_log_rx_buf_reset(void);


void hal_uart_fm33lg_init(void);
#if (HAL_UART_TIME_TYPE)
void hal_uart_fm33lg_RxIrqCheck(void);
#endif
uint8_t *hal_uart_fm33lg_rx_buf_addr_get(void);
uint8_t hal_uart_fm33lg_rx_buf_len_get(void);
void hal_uart_fm33lg_rx_buf_reset(void);
void hal_uart_fm33lg_tx_buf_set(uint8_t *pData,uint8_t u8Len);

#endif
