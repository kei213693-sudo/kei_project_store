/*
 * bsp_task_events.c
 *
 *  Created on: 2024年4月18日
 *      Author: lijing
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "bsp_task_events.h"

/*******************************全局变量定义*******************************/

/*******************************内部变量定义*******************************/
static EventGroupHandle_t g_hEventGroup = NULL;

/*******************************内部函数的声明*******************************/

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：创建事件组
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
int32_t bsp_task_events_init(void)
{
    int32_t i32ret = pdPASS;
    g_hEventGroup = xEventGroupCreate();
    if(g_hEventGroup == NULL)
    {
        i32ret = pdFAIL;
    }
    return i32ret;
}

/**************************************************************
*函数功能描述 ：删除事件组
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
int32_t bsp_task_events_deinit(void)
{
    int32_t i32ret = pdPASS;
    if(NULL != g_hEventGroup)
    {
        vEventGroupDelete(g_hEventGroup);
    }
    return i32ret;
}

/**************************************************************
*函数功能描述 ：给事件组置位
*函数参数 ：events，待置位事件组标志
*函数返回值 ：
*备注 ：
**************************************************************/
void bsp_task_events_set(EventBits_t events)
{
    xEventGroupSetBits(g_hEventGroup,events);
}

/**************************************************************
*函数功能描述 ：给事件组置位，中断调用
*函数参数 ：events，待置位事件组标志
*函数返回值 ：
*备注 ：
**************************************************************/
void bsp_task_events_set_isr(EventBits_t events)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(pdTRUE == xEventGroupSetBitsFromISR(g_hEventGroup,events,&xHigherPriorityTaskWoken))
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//检查是否有更高优先级的任务也在等待这个信号量且准备允许了
    }
}

/**************************************************************
*函数功能描述 ：等待特定位的事件组
*函数参数 ：AllEvents，待等待的标志
            WaitTimeMS，等待时间，ms，最大为portMAX_DELAY（建议用，让任务处于阻塞状态）
*函数返回值 ：被置位事件组标志，超时返回为0
*备注 ：
**************************************************************/
EventBits_t bsp_task_events_wait(EventBits_t AllEvents,uint32_t WaitTimeMS)
{
    const TickType_t xTicksToWait = WaitTimeMS / portTICK_PERIOD_MS;

    return xEventGroupWaitBits(g_hEventGroup,AllEvents
                ,pdTRUE//调用后对应位会被自动清空
                ,pdFALSE//任意一个被置位
                ,xTicksToWait);
}

/**************************************************************
*函数功能描述 ：等待特定位的事件组，组合等待，通常用于同步多个任务
                ，或者叫做任务集合，每个任务必须等待其他任务到达同步点才能继续
                ；默认对应位会被自动清空
*函数参数 ：SetEvents，某一任务调用时，要去设置的位
            WaitEvents，需要满足的所有标志位
            WaitTimeMS，等待时间，ms，最大为portMAX_DELAY
*函数返回值 ：被置位事件组标志，超时返回为0
*备注 ：
**************************************************************/
EventBits_t bsp_task_events_wait_snyc(EventBits_t SetEvents,EventBits_t WaitEvents,uint32_t WaitTimeMS)
{
    const TickType_t xTicksToWait = WaitTimeMS / portTICK_PERIOD_MS;

    return xEventGroupSync(g_hEventGroup,SetEvents,WaitEvents,xTicksToWait);
}

/**************************************************************
*函数功能描述 ：获取事件标志组数值
*函数参数 ：
*函数返回值 ：事件组标志总值
*备注 ：
**************************************************************/
EventBits_t bsp_task_events_get(void)
{
    return xEventGroupGetBits(g_hEventGroup);
}

