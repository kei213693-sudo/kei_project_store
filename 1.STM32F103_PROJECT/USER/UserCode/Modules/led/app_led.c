/*
 * app_led.c
 *
 *  Created on: 2023年7月4日
*	  Abstract: led处理模块，实现单个led灯的“亮一次、快闪、亮起一定时间、常亮”功能，然后根据特定操作码扩展更复杂多样的led灯状态
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "app_led.h"
#include "bsp_led.h"
#include "bsp_log.h"
#include "queue.h"
#include "timers.h"

/*******************************全局变量定义*******************************/
TaskHandle_t g_LedTaskHandle = NULL;//任务句柄

/*******************************内部变量定义*******************************/
static QueueHandle_t g_hLedQueue = NULL;
static TimerHandle_t g_hLedTimer[BSP_LED_NAME_END];//定时器

static app_led_ctl_t g_stLedCtl[BSP_LED_NAME_END];//灯控制结构体

static const BSP_LED_API_T *g_pBspLedApi = &g_BspLedApi;

/*******************************内部函数的声明*******************************/
static void app_led_task(void* pvParameters);
static void app_led_vTimerCallback(TimerHandle_t xTimer);

static void app_led_execute_once(BSP_LED_NAME_E eLedName);
static void app_led_execute_flicker(BSP_LED_NAME_E eLedName,uint16_t u16FlickerCount);
static void app_led_execute_constant(BSP_LED_NAME_E eLedName);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：创建led任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_led_init(void)
{
    int32_t i32ret = pdPASS;

	memset(&g_stLedCtl,0,sizeof(g_stLedCtl));
    g_pBspLedApi->bsp_led_init();

    //创建队列
    g_hLedQueue = xQueueCreate(APP_LED_QUEUE_LEN,sizeof(APP_LED_QUEUE_REC_T));
    if(NULL == g_hLedQueue)
    {
        bsp_log_print(BSP_LOG_MODULE_LED,BSP_LOG_TYPE_ERROR,"Create g_hLedQueue fail");
    }

    //创建定时器
    for(uint8_t i = 0; i < (sizeof(g_hLedTimer)/sizeof(g_hLedTimer[0])); i++)
    {
        g_hLedTimer[i] = xTimerCreate("LedTimer",pdMS_TO_TICKS(APP_LED_ONCE_TIME),pdFALSE,NULL,app_led_vTimerCallback);
        if(NULL == g_hLedTimer[i])
        {
            bsp_log_print(BSP_LOG_MODULE_LED,BSP_LOG_TYPE_ERROR,"Create g_hLedTimer[%d] fail",i);
        }
    }

    //创建任务
    i32ret = (int32_t)xTaskCreate(app_led_task
                                ,"app_led_task"
                                ,APP_LED_STACK_SIZE
                                ,NULL
                                ,APP_LED_PRIORITY
                                ,&g_LedTaskHandle);
    if(i32ret != pdPASS)
    {
        bsp_log_print(BSP_LOG_MODULE_LED,BSP_LOG_TYPE_ERROR,"Create app_led_task fail");
        i32ret = pdFAIL;
    }
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：销毁led任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_led_deinit(void)
{
    int32_t i32ret = pdPASS;

    for(uint8_t i = 0; i < (sizeof(g_hLedTimer)/sizeof(g_hLedTimer[0])); i++)
    {
        xTimerDelete(g_hLedTimer[i],0);//删除定时器，不阻塞
    }
    vTaskDelete(g_LedTaskHandle);//删除任务
    vQueueDelete(g_hLedQueue);//删除队列
    g_pBspLedApi->bsp_led_deinit();
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：led进程任务
*函数参数 ：
*函数返回值 ：
*备注：使用队列阻塞
**************************************************************/
static void app_led_task(void* pvParameters)
{
	APP_LED_QUEUE_REC_T stRec;

    while(1)
    {
		memset(&stRec,0,sizeof(APP_LED_QUEUE_REC_T));

        if(pdPASS == xQueueReceive(g_hLedQueue,(void *)&stRec,portMAX_DELAY))//阻塞等待队列有信息
        {
            bsp_log_print(BSP_LOG_MODULE_LED,BSP_LOG_TYPE_INFO,"app led opt:%d",stRec.eOpt);

            switch(stRec.eOpt)
            {
                case APP_LED_E_BUTTON:
                {//红灯亮一次
                    app_led_execute_once(BSP_LED_NAME_0);
                    break;
                }
                case APP_LED_E_CHARGE:
                {//红灯持续闪烁
                    app_led_execute_flicker(BSP_LED_NAME_0,20);//闪烁10次
                    break;
                }
                case APP_LED_E_CHARGE_ERR:
                {//红灯亮起一定时间
                    app_led_execute_constant(BSP_LED_NAME_0);
                    break;
                }
                case APP_LED_E_BATTERY_FULL:
                {//红灯常亮
                    xTimerStop(g_hLedTimer[BSP_LED_NAME_0],0);//关闭定时器，且不启用
                    g_pBspLedApi->bsp_led_percent_set(BSP_LED_NAME_0,APP_LED_PERCENT);
                    g_pBspLedApi->bsp_led_light_on(BSP_LED_NAME_0);
                    break;
                }
                case APP_LED_E_BUTTON_TIMEOUT:
                {//红灯熄灭
                    xTimerStop(g_hLedTimer[BSP_LED_NAME_0],0);//关闭定时器，且不启用
                    g_pBspLedApi->bsp_led_light_off(BSP_LED_NAME_0);
                    break;
                }

                case APP_LED_E_LEARN_MODE:
                {//绿灯亮一次
                    app_led_execute_once(BSP_LED_NAME_1);
                    break;
                }
                case APP_LED_E_OTA_MODE:
                {//绿灯持续闪烁
                    app_led_execute_flicker(BSP_LED_NAME_1,(0xff + 1));//一直闪烁不停
                    break;
                }
                case APP_LED_E_RESET_MODE:
                {//绿灯亮起一定时间
                    app_led_execute_constant(BSP_LED_NAME_1);
                    break;
                }
                case APP_LED_E_SYS_MODE:
                {//绿灯常亮
                    xTimerStop(g_hLedTimer[BSP_LED_NAME_1],0);//关闭定时器，且不启用
                    g_pBspLedApi->bsp_led_percent_set(BSP_LED_NAME_1,APP_LED_PERCENT);
                    g_pBspLedApi->bsp_led_light_on(BSP_LED_NAME_1);
                    break;
                }

                case APP_LED_E_DOUSE:
                {//灯全灭
                    //停止所有定时器
                    for(uint8_t i = 0; i < (sizeof(g_hLedTimer)/sizeof(g_hLedTimer[0])); i++)
                    {
                        if(pdFALSE != xTimerIsTimerActive(g_hLedTimer[i]))
                        {//已激活，停止工作
                            xTimerStop(g_hLedTimer[i],0);
                        }
                    }
                    //关闭所有灯
                    for(uint8_t i = 0; i < BSP_LED_NAME_END; i++)
                    {
                        g_pBspLedApi->bsp_led_light_off((BSP_LED_NAME_E)i);
                    }
                    break;
                }
                default:
                // Do nothing.
                    break;
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：led信息入队列
*函数参数 ：eOpt，操作码
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：供其他模块调用控制led模式
**************************************************************/
int32_t app_led_opt_put(APP_LED_OPT_E eOpt)
{
    int32_t i32ret = pdFAIL;

    if(eOpt < APP_LED_E_END)
    {
        APP_LED_QUEUE_REC_T stRec;
	    memset(&stRec,0,sizeof(APP_LED_QUEUE_REC_T));
        stRec.eOpt = eOpt;
        //数据组包完成，放入队列
        //供定时器调用，所以这里不能阻塞
        (int32_t)xQueueSend(g_hLedQueue,(void *)&stRec,0);
        i32ret = pdPASS;
    }
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：led状态转换处理
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_led_vTimerCallback(TimerHandle_t xTimer)
{
    if(NULL != xTimer)
    {
        for(uint8_t i = 0;i < BSP_LED_NAME_END;i++)
        {
            if(xTimer == g_hLedTimer[i])
            {
                bsp_log_print(BSP_LOG_MODULE_COMM,BSP_LOG_TYPE_INFO,"g_hLedTimer[%d] running",i);

                if(APP_LED_STATE_FLICKER == g_stLedCtl[i].eLedState)
                {
                    g_stLedCtl[i].u8Count++;
                    if(g_stLedCtl[i].u8Count <= g_stLedCtl[i].u16FlickerCount)
                    {
                        if(0 == (g_stLedCtl[i].u8Count % 2))
                        {//亮
                            g_pBspLedApi->bsp_led_percent_set((BSP_LED_NAME_E)i,APP_LED_PERCENT);
                            g_pBspLedApi->bsp_led_light_on((BSP_LED_NAME_E)i);
                        }
                        else
                        {//灭
                            g_pBspLedApi->bsp_led_light_off((BSP_LED_NAME_E)i);
                        }
                        //继续启动一次
                        xTimerStart(g_hLedTimer[i],0);
                    }
                    else
                    {
                        g_pBspLedApi->bsp_led_light_off((BSP_LED_NAME_E)i);
                    }
                }
                else
                {
                    g_pBspLedApi->bsp_led_light_off((BSP_LED_NAME_E)i);
                }
            }
        } 
    }
}

/**************************************************************
*函数功能描述 ：led灯亮一次
*函数参数 ：eLedName，灯通道
*函数返回值 ：
*备注：不启动定时器，一直亮起
**************************************************************/
static void app_led_execute_once(BSP_LED_NAME_E eLedName)
{
    if(eLedName < BSP_LED_NAME_END)
    {
        //点亮led灯
        g_pBspLedApi->bsp_led_percent_set(eLedName,APP_LED_PERCENT);
        g_pBspLedApi->bsp_led_light_on(eLedName);

        //配置控制结构体参数
        g_stLedCtl[eLedName].eLedState = APP_LED_STATE_ONCE;

        xTimerChangePeriod(g_hLedTimer[eLedName],pdMS_TO_TICKS(APP_LED_ONCE_TIME),0);
        if(pdFALSE != xTimerIsTimerActive(g_hLedTimer[eLedName]))
        {//已激活，重置计时
            xTimerReset(g_hLedTimer[eLedName],0);
        }
        else
        {//未激活，启动
            xTimerStart(g_hLedTimer[eLedName],0);
        }
    }
}

/**************************************************************
*函数功能描述 ：led灯执行闪烁
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_led_execute_flicker(BSP_LED_NAME_E eLedName,uint16_t u16FlickerCount)
{
    if(eLedName < BSP_LED_NAME_END)
    {
        //点亮led灯
        g_pBspLedApi->bsp_led_percent_set(eLedName,APP_LED_PERCENT);
        g_pBspLedApi->bsp_led_light_on(eLedName);

        //配置控制结构体参数
        g_stLedCtl[eLedName].eLedState = APP_LED_STATE_FLICKER;
        g_stLedCtl[eLedName].u16FlickerCount = u16FlickerCount;
        g_stLedCtl[eLedName].u8Count = 0;

        //修改定时时间并启动定时
        xTimerChangePeriod(g_hLedTimer[eLedName],pdMS_TO_TICKS(APP_LED_FLICKER_TIME),0);
        if(pdFALSE != xTimerIsTimerActive(g_hLedTimer[eLedName]))
        {//已激活，重置计时
            xTimerReset(g_hLedTimer[eLedName],0);
        }
        else
        {//未激活，启动
            xTimerStart(g_hLedTimer[eLedName],0);
        }
    }
}

/**************************************************************
*函数功能描述 ：led常量一段时间
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_led_execute_constant(BSP_LED_NAME_E eLedName)
{
    if(eLedName < BSP_LED_NAME_END)
    {
        //点亮led灯
        g_pBspLedApi->bsp_led_percent_set(eLedName,APP_LED_PERCENT);
        g_pBspLedApi->bsp_led_light_on(eLedName);

        //配置控制结构体参数
        g_stLedCtl[eLedName].eLedState = APP_LED_STATE_CONSTANT;

        //修改定时时间并启动定时
        xTimerChangePeriod(g_hLedTimer[eLedName],pdMS_TO_TICKS(APP_LED_CONSTANT_TIME),0);
        if(pdFALSE != xTimerIsTimerActive(g_hLedTimer[eLedName]))
        {//已激活，重置计时
            xTimerReset(g_hLedTimer[eLedName],0);
        }
        else
        {//未激活，启动
            xTimerStart(g_hLedTimer[eLedName],0);
        }
    }
}
