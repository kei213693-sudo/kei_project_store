/**
 ****************************************************************************************************
 * @file        atk_ble03_uart.c
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

#include "atk_ble03_uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "bsp_task_events.h"

static UART_HandleTypeDef g_uart_handle; /* ATK-BLE03 UART */
static uint8_t g_u8RxTemp;//接收缓存，每次接收一个bytes，然后放入g_stAtkBle03UartCtl.rxBuf

typedef struct
{
    uint16_t finsh;/* 帧接收完成标志*/
    uint16_t len;/* 帧接收长度*/
    uint32_t irqCount;//中断计数标志
    uint8_t rxBuf[ATK_BLE03_UART_RX_BUF_SIZE];/* 帧接收区域 */

    uint8_t txBuf[ATK_BLE03_UART_TX_BUF_SIZE];/* ATK-BLE03 UART发送缓冲 */
}atk_ble03_uart_ctl_t;

static atk_ble03_uart_ctl_t g_stAtkBle03UartCtl = {0};/* ATK-BLE03 UART接收帧缓冲信息结构体 */

/**
 * @brief       ATK-BLE03 UART初始化
 * @param       baudrate: UART通讯波特率
 * @retval      无
 */
void atk_ble03_uart_init(uint32_t baudrate)
{
    g_uart_handle.Instance          = ATK_BLE03_UART_INTERFACE;     /* ATK-BLE03 UART */
    g_uart_handle.Init.BaudRate     = baudrate;                     /* 波特率 */
    g_uart_handle.Init.WordLength   = UART_WORDLENGTH_8B;           /* 数据位 */
    g_uart_handle.Init.StopBits     = UART_STOPBITS_1;              /* 停止位 */
    g_uart_handle.Init.Parity       = UART_PARITY_NONE;             /* 校验位 */
    g_uart_handle.Init.Mode         = UART_MODE_TX_RX;              /* 收发模式 */
    g_uart_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;          /* 无硬件流控 */
    g_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;         /* 过采样 */
    HAL_UART_Init(&g_uart_handle);                                  /* 使能ATK-BLE03 UART
                                                                     * HAL_UART_Init()会调用函数HAL_UART_MspInit()
                                                                     * 该函数定义在文件usart.c中
                                                                     */
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	HAL_UART_Receive_IT(&g_uart_handle, (uint8_t *)&g_u8RxTemp, 1);
}

#if 1
void UART5_IRQHandler(void)                	
{ 
	u32 timeout=0;
	HAL_UART_IRQHandler(&g_uart_handle);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&g_uart_handle) != HAL_UART_STATE_READY)//等待就绪
	{
		timeout++;//超时处理
		if(timeout>HAL_MAX_DELAY) break;
	}

	timeout=0;
	while(HAL_UART_Receive_IT(&g_uart_handle, (u8 *)&g_u8RxTemp, 1) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
		timeout++; //超时处理
		if(timeout>HAL_MAX_DELAY) break;
	}
}

//uart中断回调中调用
void ATK_BLE03_UART_RxCpltCallback(void)
{
	g_stAtkBle03UartCtl.irqCount = 0;//清空技术标志，重新计数
	
    if (g_stAtkBle03UartCtl.len < (ATK_BLE03_UART_RX_BUF_SIZE - 1))     /* 判断UART接收缓冲是否溢出
                                                                    * 留出一位给结束符'\0'*/
    {
        g_stAtkBle03UartCtl.rxBuf[g_stAtkBle03UartCtl.len] = g_u8RxTemp;      /* 将接收到的数据写入缓冲 */
        g_stAtkBle03UartCtl.len++;                                      /* 更新接收到的数据长度 */
    }
    else                                                            /* UART接收缓冲溢出，覆盖 */
    {
        g_stAtkBle03UartCtl.len = 0;                                    /* 覆盖之前收到的数据 */
        g_stAtkBle03UartCtl.rxBuf[g_stAtkBle03UartCtl.len] = g_u8RxTemp;      /* 将接收到的数据写入缓冲 */
        g_stAtkBle03UartCtl.len++;                                      /* 更新接收到的数据长度 */
    }
}

//rx irq检查操作，判断是否存在接收完成TIM中断中调用
void ATK_BLE03_UART_RxIrqCheck(void)
{
    if((0 != g_stAtkBle03UartCtl.len)
        && (1 != g_stAtkBle03UartCtl.finsh))
    {
        g_stAtkBle03UartCtl.irqCount++;
        if(g_stAtkBle03UartCtl.irqCount > ATK_BLE03_UART_RX_IRQ_TIMEOUT)
        {//10ms
            g_stAtkBle03UartCtl.irqCount = 0;
            g_stAtkBle03UartCtl.finsh = 1;
            //区分是否为AT指令，然后分两个分支，分别调用不同的irq相关api，去同步ble任务处理已经接受完的数据
            bsp_task_events_set_isr(APP_COMM_IRQ_BLE_EVENTS);
        }
    }
}

#else
/**
 * @brief       ATK-BLE03 UART中断回调函数
 * @param       无
 * @retval      无
 */
void ATK_BLE03_UART_IRQHandler(void)
{
    uint8_t tmp;
    
    if (__HAL_UART_GET_FLAG(&g_uart_handle, UART_FLAG_ORE) != RESET)        /* UART接收过载错误中断 */
    {
        __HAL_UART_CLEAR_OREFLAG(&g_uart_handle);                           /* 清除接收过载错误中断标志 */
        (void)g_uart_handle.Instance->SR;                                   /* 先读SR寄存器，再读DR寄存器 */
        (void)g_uart_handle.Instance->DR;
    }
    
    if (__HAL_UART_GET_FLAG(&g_uart_handle, UART_FLAG_RXNE) != RESET)       /* UART接收中断 */
    {
        HAL_UART_Receive(&g_uart_handle, &tmp, 1, HAL_MAX_DELAY);           /* UART接收数据 */
        
        if (g_stAtkBle03UartCtl.len < (ATK_BLE03_UART_RX_BUF_SIZE - 1))     /* 判断UART接收缓冲是否溢出
                                                                             * 留出一位给结束符'\0'
                                                                             */
        {
            g_stAtkBle03UartCtl.rxBuf[g_stAtkBle03UartCtl.len] = tmp;             /* 将接收到的数据写入缓冲 */
            g_stAtkBle03UartCtl.len++;                                      /* 更新接收到的数据长度 */
        }
        else                                                                /* UART接收缓冲溢出 */
        {
            g_stAtkBle03UartCtl.len = 0;                                    /* 覆盖之前收到的数据 */
            g_stAtkBle03UartCtl.rxBuf[g_stAtkBle03UartCtl.len] = tmp;             /* 将接收到的数据写入缓冲 */
            g_stAtkBle03UartCtl.len++;                                      /* 更新接收到的数据长度 */
        }
    }
    
    if (__HAL_UART_GET_FLAG(&g_uart_handle, UART_FLAG_IDLE) != RESET)       /* UART总线空闲中断 */
    {
        g_stAtkBle03UartCtl.finsh = 1;                                      /* 标记帧接收完成 */
        
        __HAL_UART_CLEAR_IDLEFLAG(&g_uart_handle);                          /* 清除UART总线空闲中断 */
        //区分是否为AT指令，然后分两个分支，分别调用不同的irq相关api，去同步ble任务处理已经接受完的数据
        //这里去comm任务中新增一个isr调用的event专门处理
        //bsp_task_events_set_isr(APP_COMM_IRQ_BLE_EVENTS);
    }
}
#endif

/**
 * @brief       ATK-BLE03 UART printf，AT格式输出指令
 * @param       fmt: 待打印的数据
 * @retval      无
 */
void atk_ble03_uart_printf(char *fmt, ...)
{
    va_list ap;
    uint16_t len;
    
    va_start(ap, fmt);
    vsprintf((char *)g_stAtkBle03UartCtl.txBuf, fmt, ap);
    va_end(ap);
    
    len = strlen((const char *)g_stAtkBle03UartCtl.txBuf);
    
    strcat("\r\n",(const char *)g_stAtkBle03UartCtl.txBuf);//这里会卡死
    len += 2;
    HAL_UART_Transmit(&g_uart_handle, g_stAtkBle03UartCtl.txBuf, len, HAL_MAX_DELAY);
}

/**
 * @brief       ATK-BLE03 UART重新开始接收数据
 * @param       无
 * @retval      无
 */
void atk_ble03_uart_rx_restart(void)
{
    g_stAtkBle03UartCtl.len     = 0;
    g_stAtkBle03UartCtl.finsh   = 0;
    g_stAtkBle03UartCtl.irqCount = 0;
    memset(&g_stAtkBle03UartCtl.rxBuf,0,ATK_BLE03_UART_RX_BUF_SIZE);//清空缓存
}

/**
 * @brief       获取ATK-BLE03 UART接收到的一帧数据
 * @param       无
 * @retval      NULL: 未接收到一帧数据
 *              其他: 接收到的一帧数据
 */
uint8_t *atk_ble03_uart_rx_get_frame(void)
{
    if(g_stAtkBle03UartCtl.finsh == 1)
    {
        g_stAtkBle03UartCtl.rxBuf[g_stAtkBle03UartCtl.len] = '\0';
        return g_stAtkBle03UartCtl.rxBuf;
    }
    else
    {//接收未完成，不返回地址
        return NULL;
    }
}

/**
 * @brief       获取ATK-BLE03 UART接收到的一帧数据的长度
 * @param       无
 * @retval      0   : 未接收到一帧数据
 *              其他: 接收到的一帧数据的长度
 */
uint16_t atk_ble03_uart_rx_get_frame_len(void)
{
    if(g_stAtkBle03UartCtl.finsh == 1)
    {
        return g_stAtkBle03UartCtl.len;
    }
    else
    {
        return 0;
    }
}

/**************************************************************
*函数功能描述 ：通过uart将数据发送给ble模块，阻塞式发送
*函数参数 ：pData，数据地址
            len，数据长度
            mstime，超时时间，ms
*函数返回值 ：0成功
*备注：
**************************************************************/
int32_t atk_ble03_uart_tx(uint8_t *pData, uint16_t len, uint32_t mstime)
{
	HAL_StatusTypeDef retStatus = HAL_ERROR;

    if((NULL != pData) && (0 != len) && (ATK_BLE03_UART_TX_BUF_SIZE >= len))
    {
        memset(g_stAtkBle03UartCtl.txBuf,0,ATK_BLE03_UART_TX_BUF_SIZE);
        memcpy(g_stAtkBle03UartCtl.txBuf,pData,len);
        retStatus = HAL_UART_Transmit(&g_uart_handle,g_stAtkBle03UartCtl.txBuf,len,mstime);
    }
    
	return (int)retStatus;
}

/**************************************************************
*函数功能描述 ：将串口接收到数据copy出来，并清空缓存信息，中断式接收
*函数参数 ：pData，数据地址，将rx buf拷贝到这里面
            pLen，数据大小信息
*函数返回值 ：
*备注：
**************************************************************/
int32_t atk_ble03_uart_rx(uint8_t *pData, uint16_t *pLen)
{
    int32_t i32ret = -1;

    if((NULL != pData) && (NULL != pLen))
    {
        if (g_stAtkBle03UartCtl.finsh == 1)
        {
            *pLen = g_stAtkBle03UartCtl.len;
            memcpy(pData,g_stAtkBle03UartCtl.rxBuf,g_stAtkBle03UartCtl.len);

            memset(&g_stAtkBle03UartCtl,0,sizeof(g_stAtkBle03UartCtl));//清空缓存
            i32ret = 0;
        }
    }

    return i32ret;
}


