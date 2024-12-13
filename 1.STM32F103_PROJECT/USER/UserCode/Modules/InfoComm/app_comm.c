/*
 * app_comm.c
 *
 *  Created on: 2023年6月26日
 *      Author: F704011834_
 *	  Abstract: 用于数据的队列管理，实现所有通信渠道的数据收发中转、发送失败重试的功能
 */

/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "app_comm.h"
#include "portable.h"
#include "bsp_task_events.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "bsp_log.h"
#include "app_ble.h"
#include "atk_ble03_uart.h"

/*******************************全局变量定义*******************************/
TaskHandle_t g_CommTaskHandle = NULL;//任务句柄

/*******************************内部变量定义*******************************/
static QueueHandle_t g_hCommTxQueue = NULL;
static QueueHandle_t g_hCommRxQueue = NULL;

static TimerHandle_t g_hCommTxWaitTimer[BSP_COMM_QUEUE_SIZE] = {0};//定时重发

const BSP_COMM_API_T *g_pBspCommApi = &g_BspCommApi;

/*******************************内部函数的声明*******************************/
static void app_comm_task(void* pvParameters);
static void app_comm_processTxMsg(APP_COMM_QUEUE_REC_T stRec);
static void app_comm_processRxMsg(APP_COMM_QUEUE_REC_T stRec);

static void app_comm_cb(void *pData);
static void app_comm_vTimerCallback(TimerHandle_t xTimer);

//和rtos类型无关的接口
static void app_comm_tx_hw(uint8_t *buf,uint32_t len,uint8_t channel);
static void app_comm_rx_app(uint8_t *buf,uint32_t len,uint8_t channel);
static uint16_t app_comm_last_rsp(uint8_t channel);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：创建comm任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_comm_init(void)
{
    int32_t i32ret = pdPASS;

    //初始化bsp层
    g_pBspCommApi->bsp_comm_init(app_comm_cb,app_comm_tx_hw,app_comm_rx_app,app_comm_last_rsp);

    //申请内存
    g_pStBspCommCtl = pvPortMalloc(sizeof(bsp_comm_ctl_t));
    if(NULL == g_pStBspCommCtl)
    {
        bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_ERROR,"g_pStBspCommCtl malloc fail:%d bytes",sizeof(bsp_comm_ctl_t));
    }

    //创建队列
    g_hCommTxQueue = xQueueCreate(APP_COMM_QUEUE_LEN,sizeof(APP_COMM_QUEUE_REC_T));
    g_hCommRxQueue = xQueueCreate(APP_COMM_QUEUE_LEN,sizeof(APP_COMM_QUEUE_REC_T));
    if((NULL == g_hCommTxQueue) || (NULL == g_hCommRxQueue))
    {
        bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_ERROR,"Create g_hCommQueue fail");
    }

    //创建定时器
    for(uint8_t i = 0;i < BSP_COMM_QUEUE_SIZE;i++)
    {
        g_hCommTxWaitTimer[i] = xTimerCreate("CommTimer",pdMS_TO_TICKS(BSP_COMM_TIMEOUT_TIME),pdFALSE,(void *)i,app_comm_vTimerCallback);
        if(NULL == g_hCommTxWaitTimer[i])
        {
            bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_ERROR,"Create g_hCommTxWaitTimer[%d] fail",i);
        }
    }

    //创建任务
    i32ret = (int32_t)xTaskCreate(app_comm_task
                                ,"app_comm_task"
                                ,APP_COMM_STACK_DEPTH
                                ,NULL
                                ,APP_COMM_PRIORITY
                                ,&g_CommTaskHandle);
    if(i32ret != pdPASS)
    {
        bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_ERROR,"Create app_comm_task fail");
        i32ret = pdFAIL;
    }

    return i32ret;
}

/**************************************************************
*函数功能描述 ：销毁comm任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_comm_deinit(void)
{
	int32_t i32ret = pdPASS;

    for(uint8_t i = 0;i < BSP_COMM_QUEUE_SIZE;i++)
    {
        xTimerDelete(g_hCommTxWaitTimer[i],0);//删除定时器，不阻塞
    }
    vPortFree(g_pStBspCommCtl);//释放内存空间
    vQueueDelete(g_hCommTxQueue);//删除队列
    vQueueDelete(g_hCommRxQueue);//删除队列
    vTaskDelete(g_CommTaskHandle);//删除任务

    return i32ret;
}

/**************************************************************
*函数功能描述 ：comm进程任务
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_comm_task(void* pvParameters)
{
    EventBits_t events = 0;
    APP_COMM_QUEUE_REC_T stRec;

    while(1)
    {
        memset(&stRec,0,sizeof(APP_COMM_QUEUE_REC_T));

        events = bsp_task_events_wait(APP_COMM_ALL_EVENTS,portMAX_DELAY);//阻塞在这里
        if(events)
        {
            if((events & APP_COMM_TX_EVENTS)
                && (NULL != g_hCommTxQueue))
			{//一直读取队列数据，然后处理，直到队列为空
                while(uxQueueMessagesWaiting(g_hCommTxQueue))//返回当前队列有效元素数目
                {
                    if(pdPASS == xQueueReceive(g_hCommTxQueue,(void *)&stRec,0))
                    {//不阻塞,接收数据成功后自动从队列中删除项目
                        app_comm_processTxMsg(stRec);
                    }
                }
            }

            if((events & APP_COMM_RX_EVENTS)
                && (NULL != g_hCommRxQueue))
			{//一直读取队列数据，然后处理，直到队列为空
                while(uxQueueMessagesWaiting(g_hCommRxQueue))//返回当前队列有效元素数目
                {
                    if(pdPASS == xQueueReceive(g_hCommRxQueue,(void *)&stRec,0))
                    {//不阻塞,接收数据成功后自动从队列中删除项目
                        app_comm_processRxMsg(stRec);
                    }
                }
            }

            if(events & APP_COMM_IRQ_BLE_EVENTS)
            {
                uint8_t *u8Buf = atk_ble03_uart_rx_get_frame();
                if(NULL != u8Buf)
                {
                    uint16_t u16Len = atk_ble03_uart_rx_get_frame_len();
                    app_comm_rxEnQueue(u8Buf,u16Len,APP_COMM_CHANNEL_BLE);
                    atk_ble03_uart_rx_restart();//重置接收参数
                }
                else
                {
                    bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_ERROR,"atk_ble03 uart data get fail!");
                }
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：将待处理的操作和对应的数据传递给任务,供外部调用
*函数参数 ：events，事件组标志
            queueIndex，发送数据索引
*函数返回值 ：
*备注：中断无法调用
**************************************************************/
int32_t app_comm_put(EventBits_t events,uint8_t queueIndex)
{
    int32_t i32ret = pdFAIL;
    APP_COMM_QUEUE_REC_T stRec;
    memset(&stRec,0,sizeof(APP_COMM_QUEUE_REC_T));

    if(events & APP_COMM_ALL_EVENTS)
    {
        stRec.queueIndex = queueIndex;

        //数据组包完成，放入队列，放满了就阻塞在这里
        if((events & APP_COMM_TX_EVENTS) && (NULL != g_hCommTxQueue))
        {
            xQueueSend(g_hCommTxQueue,(void *)&stRec,portMAX_DELAY);//放入队列，中断服务中不能调用
        }
        else if((events & APP_COMM_RX_EVENTS) && (NULL != g_hCommRxQueue))
        {
            xQueueSend(g_hCommRxQueue,(void *)&stRec,portMAX_DELAY);//放入队列，中断服务中不能调用
        }
        else
        {}

        bsp_task_events_set(events);//传递事件
        i32ret = pdPASS;
    }
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：开始重传计时，app层供bsp层调用的回调函数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_comm_cb(void *pData)
{
	if(NULL != pData)
	{
		uint8_t queueIndex = 0;
		memcpy((void*)&queueIndex,pData,sizeof(queueIndex));
		if(pdFAIL == xTimerIsTimerActive(g_hCommTxWaitTimer[queueIndex]))
		{//不在运行过程中就启动
			xTimerStart(g_hCommTxWaitTimer[queueIndex],0);
		}
		else
		{
			bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_INFO,"app_comm_cb is buys:%d",queueIndex);
		}
	}
}

/**************************************************************
*函数功能描述 ：comm任务的定时器中断
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_comm_vTimerCallback(TimerHandle_t xTimer)
{
    if(NULL != xTimer)
    {
        for(uint8_t i = 0;i < BSP_COMM_QUEUE_SIZE;i++)
        {
            if(xTimer == g_hCommTxWaitTimer[i])
            {
                bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_INFO,"g_hCommTxWaitTimer[%d] running",i);

                app_comm_put(APP_COMM_TX_EVENTS,i);
            }
        } 
    }

    //xTimerStop( xTimer, 0 );
    //xTimerStart( xTimer, 0 );
}

/**************************************************************
*函数功能描述 ：comm tx自定义事件处理函数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_comm_processTxMsg(APP_COMM_QUEUE_REC_T stRec)
{
    if(NULL == g_pStBspCommCtl)
    {
        return;
    }

    bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_INFO,"App comm tx index:%d,channel:%d",stRec.queueIndex,g_pStBspCommCtl->txQueue[stRec.queueIndex].channel);

    if(0 != g_pStBspCommCtl->txQueue[stRec.queueIndex].len)
    {
        bsp_log_print_hex(BSP_LOG_MODULE_COMM,"comm tx data",g_pStBspCommCtl->txQueue[stRec.queueIndex].buf,g_pStBspCommCtl->txQueue[stRec.queueIndex].len);
        g_pBspCommApi->bsp_comm_do_tx(stRec.queueIndex);
    }
    else
    {
        bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_INFO,"tx cancel:%d",stRec.queueIndex);
        xTimerStop(g_hCommTxWaitTimer[stRec.queueIndex],0);
    }
}

/**************************************************************
*函数功能描述 ：comm rx自定义事件处理函数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_comm_processRxMsg(APP_COMM_QUEUE_REC_T stRec)
{
    if(NULL == g_pStBspCommCtl)
    {
        return;
    }

    bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_INFO,"App comm rx index:%d,channel:%d",stRec.queueIndex,g_pStBspCommCtl->rxQueue[stRec.queueIndex].channel);

    bsp_log_print_hex(BSP_LOG_MODULE_COMM,"comm rx data",g_pStBspCommCtl->rxQueue[stRec.queueIndex].buf,g_pStBspCommCtl->rxQueue[stRec.queueIndex].len);
    g_pBspCommApi->bsp_comm_do_rx(stRec.queueIndex);
}

/******************以下代码和rtos类型无关***************************************************************************/

/**************************************************************
*函数功能描述 ：本质就是调用实际接口将数据发送出去，例如从app到bsp到hw
*函数参数 ：buf，指向数据指针
            len，数据长度
            channel，数据传输通道
*函数返回值 ：命令码
*备注：
**************************************************************/
static void app_comm_tx_hw(uint8_t *buf,uint32_t len,uint8_t channel)
{
    if(APP_COMM_CHANNEL_BLE == channel)
    {
        app_ble_protocol_tx(buf,len);
    }
    else if(APP_COMM_CHANNEL_WIFI == channel)
    {
    }
}

/**************************************************************
*函数功能描述 ：将数据传递给对应的进程模块，让对应模块自己去解析处理
*函数参数 ：buf，指向数据指针
            len，数据长度
            channel，数据传输通道
*函数返回值 ：命令码
*备注：
**************************************************************/
static void app_comm_rx_app(uint8_t *buf,uint32_t len,uint8_t channel)
{
    if(APP_COMM_CHANNEL_BLE == channel)
    {
        app_ble_protocol_rx(buf,len);
    }
    else if(APP_COMM_CHANNEL_WIFI == channel)
    {
    }
}

/**************************************************************
*函数功能描述 ：根据通道获取当前时间最后一包数据的响应指令码
*函数参数 ：channel，数据传输通道
*函数返回值 ：
*备注：
**************************************************************/
static uint16_t app_comm_last_rsp(uint8_t channel)
{
    uint16_t u16Waitflag = 0;

    if(APP_COMM_CHANNEL_BLE == channel)
    {
        u16Waitflag = app_ble_protocol_last_rsp();
    }
    else if(APP_COMM_CHANNEL_WIFI == channel)
    {
    }
    
    return u16Waitflag;
}

/**************************************************************
*函数功能描述 ：comm tx传入任务，替代bsp层comm_enter_txQueue
*函数参数 ：buf，指向缓存地址
            len，实际数据长度
            channel，传输数据的通道
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_comm_txEnQueue(uint8_t *buf,uint32_t len,uint8_t channel,uint16_t waitcmd)
{
    int32_t i32ret = pdFAIL;
    uint8_t queueIndex = 0;

    if((NULL != buf) && (len <= BSP_COMM_PACKET_SIZE) && (channel < APP_COMM_CHANNEL_END))
    {
        queueIndex = g_pBspCommApi->bsp_comm_enter_txQueue(buf,len,channel,waitcmd);
        if(queueIndex < BSP_COMM_QUEUE_SIZE)
        {
            //bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_INFO,"tx enter queue Index:%d",queueIndex);
            app_comm_put(APP_COMM_TX_EVENTS,queueIndex);
            i32ret = pdPASS;
        }
        else
        {
            bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_ERROR,"tx queue is full!");
        }
    }
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：comm rx传入任务，替代bsp层comm_enter_rxQueue
*函数参数 ：buf，指向缓存地址
            len，实际数据长度
            channel，传输数据长度
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_comm_rxEnQueue(uint8_t *buf,uint32_t len,uint8_t channel)
{
    int32_t i32ret = pdFAIL;
    uint8_t queueIndex = 0;

    if((NULL != buf) && (len <= BSP_COMM_PACKET_SIZE) && (channel < APP_COMM_CHANNEL_END))
    {
        queueIndex = g_pBspCommApi->bsp_comm_enter_rxQueue(buf,len,channel);
        if(queueIndex < BSP_COMM_QUEUE_SIZE)
        {//将索引index传到任务队列，让comm_task处理接收操作
            //bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_INFO,"rx enter queue Index:%d",queueIndex);
            app_comm_put(APP_COMM_RX_EVENTS,queueIndex);
            i32ret = pdPASS;
        }
        else
        {
            bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_ERROR,"rx queue is full!");
        }
    }
    
    return i32ret;
}
