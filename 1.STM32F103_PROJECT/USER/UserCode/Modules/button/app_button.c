/*
 * app_button.c
 *
 *  Created on: 2023年5月30日
 *	  Abstract: button处理模块，将bsp传递的单个按键操作扩展为更复杂的应用，例如“组合按键（双击、三击、复合）、顺序按键”等操作
 */

/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "app_button.h"
#include "bsp_button.h"
#include "bsp_log.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/*******************************全局变量定义*******************************/
TaskHandle_t g_ButtonTaskHandle = NULL;//任务句柄

/*******************************内部变量定义*******************************/
static QueueHandle_t g_hButtonQueue = NULL;
static TimerHandle_t g_hButtonTimer = NULL;//组合按键定时器
static TimerHandle_t g_hButtonCombTimer = NULL;//顺序按键超时定时器

static const BSP_BUTTON_API_T* const g_pButtonApi = &g_BspButtonApi;

static app_button_ctl_t g_stAppButtonCtl;

/*******************************内部函数的声明*******************************/
static void app_button_task(void* pvParameters);
static void app_button_app_cb(void *pData);
static void app_button_processGet(void);
static void app_button_processOptHandle(APP_BUTTON_OPT_E eOpt);
static void app_button_vTimerCallback(TimerHandle_t xTimer);
static void app_button_combination_vTimerCallback(TimerHandle_t xTimer);
static void app_button_combination_updata(BSP_BUTTON_NAME_E enButtonName);
static void app_button_combination_reset(BSP_BUTTON_NAME_E enButtonName);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：创建button任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_button_init(void)
{
    int32_t i32ret = pdPASS;

	memset(&g_stAppButtonCtl,0,sizeof(app_button_ctl_t));
    g_pButtonApi->bsp_button_init(app_button_app_cb);//初始化按键

    //创建队列
    g_hButtonQueue = xQueueCreate(APP_BUTTON_QUEUE_LEN,sizeof(APP_BUTTON_QUEUE_REC_T));
    if(NULL == g_hButtonQueue)
    {
        bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_ERROR,"Create g_hButtonQueue fail");
    }

    //创建定时器
    g_hButtonTimer = xTimerCreate("ButtonTimer",pdMS_TO_TICKS(APP_BUTTON_CONTINUOUS_TIME),pdFALSE,NULL,app_button_vTimerCallback);
    g_hButtonCombTimer = xTimerCreate("ButtonCombTimer",pdMS_TO_TICKS(APP_BUTTON_COMBINATION_TIME),pdFALSE,NULL,app_button_combination_vTimerCallback);
    if((NULL == g_hButtonTimer) || (NULL == g_hButtonCombTimer))
    {
        bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_ERROR,"Create g_hButtonTimer or g_hButtonCombTimer fail");
    }

    //创建任务
    i32ret = (int32_t)xTaskCreate(app_button_task
                                ,"app_button_task"
                                ,APP_BUTTON_STACK_DEPTH
                                ,NULL
                                ,APP_BUTTON_PRIORITY
                                ,&g_ButtonTaskHandle);
    if(i32ret != pdPASS)
    {
        bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_ERROR,"Create app_button_task fail");
        i32ret = pdFAIL;
    }

    return i32ret;
}

/**************************************************************
*函数功能描述 ：销毁button任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_button_deinit(void)
{
	int32_t i32ret = pdPASS;

    vTaskDelete(g_ButtonTaskHandle);//删除任务
    xTimerDelete(g_hButtonCombTimer,0);//删除定时器，不阻塞
    xTimerDelete(g_hButtonTimer,0);//删除定时器，不阻塞
    vQueueDelete(g_hButtonQueue);//删除队列
    g_pButtonApi->bsp_button_deinit();//button去初始化

    return i32ret;
}

/**************************************************************
*函数功能描述 ：button进程任务，处理button业务层逻辑
*函数参数 ：
*函数返回值 ：
*备注：使用队列处理
**************************************************************/
static void app_button_task(void* pvParameters)
{
	APP_BUTTON_QUEUE_REC_T stRec;

    while (1)
    {
		memset(&stRec,0,sizeof(APP_BUTTON_QUEUE_REC_T));
		
        if(pdPASS == xQueueReceive(g_hButtonQueue,(void *)&stRec,portMAX_DELAY))//阻塞等待队列有信息
        {
            if(APP_BUTTON_E_GET == stRec.eOpt)
            {
                app_button_processGet();
            }
            else
            {
                //opt已经确定，清空控制结构体
                memset(&g_stAppButtonCtl,0,sizeof(app_button_ctl_t));
                app_button_processOptHandle(stRec.eOpt);
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：button信息入队列
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_button_opt_put(APP_BUTTON_OPT_E eOpt)
{
    int32_t i32ret = pdFAIL;
    APP_BUTTON_QUEUE_REC_T stRec;
	memset(&stRec,0,sizeof(APP_BUTTON_QUEUE_REC_T));

    if((eOpt > APP_BUTTON_E_GET) && (eOpt < APP_BUTTON_E_END))
    {
        stRec.eOpt = eOpt;
        //数据组包完成，放入队列
        //供定时器调用，所以这里不能阻塞
        (int32_t)xQueueSend(g_hButtonQueue,(void *)&stRec,0);
        i32ret = pdPASS;
    }
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：供bsp层在中断服务中调用，告知app层去获取按键操作
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_button_app_cb(void *pData)
{
    APP_BUTTON_QUEUE_REC_T stRec = {APP_BUTTON_E_GET};
    (int32_t)xQueueSendFromISR(g_hButtonQueue,(void *)&stRec,NULL);
}

/**************************************************************
*函数功能描述 ：按键值传递处理，将bsp层单个按键操作（短按、长按）转换为
            ，app层按键事件（单按、长按、组合按（双击、三击、复合）、顺序按）等
            ，然后再传递给button task处理
*函数参数 ：
*函数返回值 ：
*备注：需要结合两个定时函数app_button_vTimerCallback和app_button_combination_vTimerCallback完成转换的功能
**************************************************************/
static void app_button_processGet(void)
{
    BSP_BUTTON_INFO_T stBspButtonInfo;
	memset(&stBspButtonInfo,0,sizeof(BSP_BUTTON_INFO_T));
    g_pButtonApi->bsp_button_info_get(&stBspButtonInfo);//获取按键信息
    bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"Button Upload Name:0x%x, Type:%d",stBspButtonInfo.enButtonName,stBspButtonInfo.enButtonType);

    if(1 == g_stAppButtonCtl.u8CombFlag)
    {//先判定是否是组合按键
        if(APP_BUTTON_E_COMBINATION_UPDATA == g_stAppButtonCtl.eButtonOpt)
        {
            app_button_combination_updata(stBspButtonInfo.enButtonName);
        }
        else if(APP_BUTTON_E_COMBINATION_RESET == g_stAppButtonCtl.eButtonOpt)
        {
            app_button_combination_reset(stBspButtonInfo.enButtonName);
        }
    }
    else
    {
        if(BSP_BUTTON_TYPE_LONG == stBspButtonInfo.enButtonType)
        {//长按，直接上报事件给任务
            if(BSP_BUTTON_NAME_0 == stBspButtonInfo.enButtonName)
            {
                g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_LONG_0;
            }
            else if(BSP_BUTTON_NAME_1 == stBspButtonInfo.enButtonName)
            {
                g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_LONG_1;
            }
            else if(BSP_BUTTON_NAME_2 == stBspButtonInfo.enButtonName)
            {
                g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_LONG_2;
            }
            app_button_opt_put(g_stAppButtonCtl.eButtonOpt);
        }
        else if(BSP_BUTTON_TYPE_TIMEOUT == stBspButtonInfo.enButtonType)
        {//长按超时
        }
        else if(BSP_BUTTON_TYPE_SHORT == stBspButtonInfo.enButtonType)
        {//短按
            if(g_stAppButtonCtl.u8Count < APP_BUTTON_CONTINUOUS_NUM)
            {
                g_stAppButtonCtl.u8ButtonNameTab[g_stAppButtonCtl.u8Count] = (uint8_t)stBspButtonInfo.enButtonName;
                g_stAppButtonCtl.u8Count++;
                g_stAppButtonCtl.u32ButtonState |= stBspButtonInfo.enButtonName;
            }
            else
            {
                bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_WARNING,"Button Upload full!!");
            }

            //定时器操作
            if(pdFALSE != xTimerIsTimerActive(g_hButtonTimer))
            {//已激活，重置计时
                xTimerReset(g_hButtonTimer,0);
            }
            else
            {//未激活，启动
                xTimerStart(g_hButtonTimer,0);
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：定时时间到，按键复合处理
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_button_vTimerCallback(TimerHandle_t xTimer)
{
    switch(g_stAppButtonCtl.u8Count)
    {
        case 1:
        {//单次按下
            if(BSP_BUTTON_NAME_0 == g_stAppButtonCtl.u8ButtonNameTab[0])
            {
                g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_SINGLE_0;
            }
            else if(BSP_BUTTON_NAME_1 == g_stAppButtonCtl.u8ButtonNameTab[0])
            {
                g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_SINGLE_1;
            }
            else if(BSP_BUTTON_NAME_2 == g_stAppButtonCtl.u8ButtonNameTab[0])
            {
                g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_SINGLE_2;
            }
            app_button_opt_put(g_stAppButtonCtl.eButtonOpt);
            break;
        }

        case 2:
        {//连续按下2次
            if(g_stAppButtonCtl.u8ButtonNameTab[0] == g_stAppButtonCtl.u8ButtonNameTab[1])
            {//同一按键双击
                if(BSP_BUTTON_NAME_0 == g_stAppButtonCtl.u8ButtonNameTab[0])
                {
                    g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_DOUBLE_0;
                }
                else if(BSP_BUTTON_NAME_1 == g_stAppButtonCtl.u8ButtonNameTab[0])
                {
                    g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_DOUBLE_1;
                }
                else if(BSP_BUTTON_NAME_2 == g_stAppButtonCtl.u8ButtonNameTab[0])
                {
                    g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_DOUBLE_2;
                }
                app_button_opt_put(g_stAppButtonCtl.eButtonOpt);
            }
            else if(g_stAppButtonCtl.u32ButtonState == (BSP_BUTTON_NAME_0 | BSP_BUTTON_NAME_1))
            {//组合按键——复合
                g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_COMBINATION_LEARN;
                app_button_opt_put(g_stAppButtonCtl.eButtonOpt);
            }
            else
            {//组合按键——顺序
                if(g_stAppButtonCtl.u32ButtonState == (BSP_BUTTON_NAME_0 | BSP_BUTTON_NAME_2))
                {
                    g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_COMBINATION_UPDATA;
                    bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"upgrade check start");
                    //启动组合按键判定定时计数4s，需要依次按下
                    g_stAppButtonCtl.u8CombFlag = 1;
                    g_stAppButtonCtl.u8Count = 0;
                    //启动定时
                    xTimerStart(g_hButtonCombTimer,0);
                }
                else if(g_stAppButtonCtl.u32ButtonState == (BSP_BUTTON_NAME_1 | BSP_BUTTON_NAME_2))
                {
                    g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_COMBINATION_RESET;
                    bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"reset check start");
                    //启动组合按键判定定时计数4s，需要依次按下
                    g_stAppButtonCtl.u8CombFlag = 1;
                    g_stAppButtonCtl.u8Count = 0;
                    //启动定时
                    xTimerStart(g_hButtonCombTimer,0);
                }
            }
            break;
        }

        case 3:
        {//连续按下3次
            if((g_stAppButtonCtl.u8ButtonNameTab[0] == g_stAppButtonCtl.u8ButtonNameTab[1]) 
                && (g_stAppButtonCtl.u8ButtonNameTab[0] == g_stAppButtonCtl.u8ButtonNameTab[2]))
            {//同一按键三击
                if(BSP_BUTTON_NAME_0 == g_stAppButtonCtl.u8ButtonNameTab[0])
                {
                    g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_THREE_0;
                }
                else if(BSP_BUTTON_NAME_1 == g_stAppButtonCtl.u8ButtonNameTab[0])
                {
                    g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_THREE_1;
                }
                else if(BSP_BUTTON_NAME_2 == g_stAppButtonCtl.u8ButtonNameTab[0])
                {
                    g_stAppButtonCtl.eButtonOpt = APP_BUTTON_E_THREE_2;
                }
                app_button_opt_put(g_stAppButtonCtl.eButtonOpt);
            }
            //预留后续扩展
            break;
        }

        default:
            // Do nothing.
            break;
    }
}

/**************************************************************
*函数功能描述 ：定时时间到，清空组合按键控制结构体
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_button_combination_vTimerCallback(TimerHandle_t xTimer)
{
    memset(&g_stAppButtonCtl,0,sizeof(app_button_ctl_t));//清空控制结构体
    bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_WARNING,"combination check timeout");
}

/**************************************************************
*函数功能描述 ：顺序按键——upgrade规律判定
*函数参数 ：enButtonName，当前按键名称
*函数返回值 ：
*备注：
**************************************************************/
static void app_button_combination_updata(BSP_BUTTON_NAME_E enButtonName)
{
    //升级事件判定，短按0和2启动顺序组合按键判定，黄灯闪烁后
    //，4s内依次按下0、1、2、0，就进入升级模式
    BSP_BUTTON_NAME_E eButtonUpgradeName[APP_BUTTON_COMBINATION_UPDATA_COUNT] = 
    {
        BSP_BUTTON_NAME_0,
        BSP_BUTTON_NAME_1,
        BSP_BUTTON_NAME_2,
        BSP_BUTTON_NAME_0
    };

    bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"upgrade check:%d",enButtonName);
    if(enButtonName < BSP_BUTTON_NAME_END)
    {
        if(eButtonUpgradeName[g_stAppButtonCtl.u8Count] == enButtonName)
        {//按照顺序判断相等就继续判定
            g_stAppButtonCtl.u8Count++;
            if(APP_BUTTON_COMBINATION_UPDATA_COUNT == g_stAppButtonCtl.u8Count)
            {//组合按键校验成功
                g_stAppButtonCtl.u8CombFlag = 0;
                g_stAppButtonCtl.u8Count = 0;
                //执行相应操作
                app_button_opt_put(g_stAppButtonCtl.eButtonOpt);
                xTimerStop(g_hButtonCombTimer,0);
            }
        }
        else
        {//有一个不一样就退出
            g_stAppButtonCtl.u8CombFlag = 0;
            g_stAppButtonCtl.u8Count = 0;
            xTimerStop(g_hButtonCombTimer,0);
        }
    }
}

/**************************************************************
*函数功能描述 ：顺序按键——reset规律判定
*函数参数 ：enButtonName，当前按键名称
*函数返回值 ：
*备注：
**************************************************************/
static void app_button_combination_reset(BSP_BUTTON_NAME_E enButtonName)
{
    //初始化事件判定，短按0和2启动顺序组合按键判定，黄灯闪烁后
    //，4s内依次按下2、0、1，就初始化系统
    BSP_BUTTON_NAME_E g_eButtonResetName[APP_BUTTON_COMBINATION_RESET_COUNT] = 
    {
        BSP_BUTTON_NAME_2,
        BSP_BUTTON_NAME_0,
        BSP_BUTTON_NAME_1
    };

    bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"reset check:%d",enButtonName);
    if(enButtonName < BSP_BUTTON_NAME_END)
    {
        if(g_eButtonResetName[g_stAppButtonCtl.u8Count] == enButtonName)
        {//按照顺序判断相等就继续判定
            g_stAppButtonCtl.u8Count++;
            if(APP_BUTTON_COMBINATION_RESET_COUNT == g_stAppButtonCtl.u8Count)
            {//组合按键校验成功
                g_stAppButtonCtl.u8CombFlag = 0;
                g_stAppButtonCtl.u8Count = 0;
                //执行相应操作
                app_button_opt_put(g_stAppButtonCtl.eButtonOpt);
                xTimerStop(g_hButtonCombTimer,0);
            }
        }
        else
        {//有一个不一样就退出
            g_stAppButtonCtl.u8CombFlag = 0;
            g_stAppButtonCtl.u8Count = 0;
            xTimerStop(g_hButtonCombTimer,0);
        }
    }
}


/**************************************************************
*函数功能描述 ：app层按键事件处理
*函数参数 ：eOpt，app层自定义按键事件
*函数返回值 ：
*备注：最终将按键操作转换为具体的按键事件，然后执行相应操作，与其他模块相耦合的地方就在这里
**************************************************************/
static void app_button_processOptHandle(APP_BUTTON_OPT_E eOpt)
{
    bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"app button opt:%d",eOpt);

    switch(eOpt)
    {
        case APP_BUTTON_E_SINGLE_0:
        {
            bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"single lock");

            break;
        }
        case APP_BUTTON_E_SINGLE_1:
        {
            bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"single horn");

            break;
        }
        case APP_BUTTON_E_SINGLE_2:
        {
            bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"single unlock");
            break;
        }

        case APP_BUTTON_E_DOUBLE_0:
        {
            bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"double lock");
            break;
        }
        case APP_BUTTON_E_DOUBLE_1:
        {
            bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"double unlock");
            break;
        }
        case APP_BUTTON_E_DOUBLE_2:
        {
            bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"double trunk");
            break;
        }

        case APP_BUTTON_E_COMBINATION_LEARN:
        {//学习模式
            bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"learn mode");
            //待添加功能接口
            break;
        }

        case APP_BUTTON_E_COMBINATION_UPDATA:
        {//upgrade升级模式
            bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"upgrade mode");
            //待添加功能接口
            break;
        }

        case APP_BUTTON_E_COMBINATION_RESET:
        {//初始化系统
            bsp_log_print(BSP_LOG_MODULE_BUTTON,BSP_LOG_TYPE_INFO,"reset sys");
            //待添加功能接口
            break;
        }

        default:
            break;
    }
}

