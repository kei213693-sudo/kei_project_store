/*
 * app_log.c
 *
 *  Created on: 2024年4月18日
 *      Author: lijing
 *	  Abstract: 用于处理上位机调试指令
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "app_log.h"
#include "bsp_log.h"
#include "usart.h"

/*******************************全局变量定义*******************************/
TaskHandle_t g_LogTaskHandle = NULL;//任务句柄

/*******************************内部变量定义*******************************/

/*******************************内部函数的声明*******************************/
static void app_log_task(void* pvParameters);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：创建log任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：
**************************************************************/
int32_t app_log_init(void)
{
    int32_t i32ret = pdPASS;

    i32ret = (int32_t)xTaskCreate(app_log_task
                                ,"app_log_task"
                                ,APP_LOG_STACK_DEPTH
                                ,NULL
                                ,APP_LOG_PRIORITY
                                ,&g_LogTaskHandle);
    if(pdPASS != i32ret)
    {
        bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_WARNING,"Create app_log_task fail.\r\n");
    }

    return i32ret;
}

/**************************************************************
*函数功能描述 ：删除log任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：
**************************************************************/
int32_t app_log_deinit(void)
{
    int32_t i32ret = pdPASS;

    vTaskDelete(g_LogTaskHandle);//删除任务
    bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"Del app_log_task suc\r\n");

    return i32ret;
}

/**************************************************************
*函数功能描述 ：log任务函数
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_log_task(void* pvParameters)
{
    while(1)
    {
        if(1 == ulTaskNotifyTake(pdTRUE,portMAX_DELAY))//相当于二值信号量接收
        {
            uint8_t *pData = uart_rx_buf_addr_get();
            if(NULL != pData)
            {
                uint16_t u16Len = uart_rx_buf_len_get();
                bsp_log_cmd_handle(pData,u16Len);//处理各类串口指令
                uart_rx_irq_restart();
            }
        }
    }
}

