/*
 * app_task.c
 *
 *  Created on: 2023年6月25日
 *      Author: F704011834_
 *	  Abstract: 用于rtos任务的管理（以freertos接口为例），负责任务创建、销毁、设置、获取等操作
 */

/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "app_task.h"

#include "portable.h"
#include "queue.h"
#include "timers.h"
#include "bsp_task_events.h"
#include "app_log.h"
#include "bsp_log.h"
#include "app_upgrade.h"
#include "app_ble.h"
#include "app_comm.h"
#include "app_button.h"

/*******************************全局变量定义*******************************/

/*******************************宏定义*******************************/

/*******************************内部变量定义*******************************/
static QueueHandle_t g_hTaskQueue = NULL;//队列句柄
static TaskHandle_t  g_TaskHandle = NULL;//任务句柄
static TimerHandle_t g_hTaskTimer = NULL;//定时器句柄

//这个表格需要考虑资源顺序，然后根据总长度除去单个字节长度就可以知道共定义使用到了几个模块
static app_task_module_tab_t g_stTaskModuleManagerTab[] = 
{
{APP_TASK_MODULE_TASK,              NULL,                  app_task_deinit,     &g_TaskHandle},
{APP_TASK_MODULE_LOG,               app_log_init,          app_log_deinit,      &g_LogTaskHandle},
{APP_TASK_MODULE_BLE,               app_ble_init,          app_ble_deinit,      &g_BleTaskHandle},
{APP_TASK_MODULE_LED,               NULL,          NULL,NULL},
{APP_TASK_MODULE_BUTTON,            app_button_init,       app_button_deinit,   &g_ButtonTaskHandle},
{APP_TASK_MODULE_COMM,              app_comm_init,         app_comm_deinit,     &g_CommTaskHandle},
{APP_TASK_MODULE_UPGRADE,           app_upgrade_init,      app_upgrade_deinit,  &g_UpgradeTaskHandle}
};

/*******************************内部函数的声明*******************************/
static void app_task(void* pvParameters);
static uint8_t app_task_get_index(APP_TASK_MODULE_E eModule);
static void app_task_vTimerCallback(TimerHandle_t xTimer);

static void app_task_sync(APP_TASK_MODULE_E eModule);
static void app_task_create(APP_TASK_MODULE_E eModule);
static void app_task_delete(APP_TASK_MODULE_E eModule);
static void app_task_suspend(APP_TASK_MODULE_E eModule);
static void app_task_resume(APP_TASK_MODULE_E eModule);
static void app_task_set_priority(APP_TASK_MODULE_E eModule,UBaseType_t uxNewPriority);
static void app_task_get_info(APP_TASK_MODULE_E eModule);
static void app_task_get_info_all(void);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：创建任务管理任务，然后在这个任务中创建其他任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：
**************************************************************/
int32_t app_task_init(void)
{
    int32_t i32ret = pdPASS;

    //创建队列
    g_hTaskQueue = xQueueCreate(APP_TASK_QUEUE_LEN,sizeof(APP_TASK_QUEUE_REC_T));
    if(NULL == g_hTaskQueue)
    {
        bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_WARNING,"Create g_hTaskQueue fail");
    }

    //创建事件组
    if(pdFAIL == bsp_task_events_init())
    {
        bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_WARNING,"Create g_hEventGroup fail");
    }

    //创建task定时器
    g_hTaskTimer = xTimerCreate("g_hTaskTimer",pdMS_TO_TICKS(APP_TASK_TIMEOUT)
                        ,pdTRUE//周期性
                        ,(void *)0//ID为0，该回调函数只分配给这一个句柄
                        ,app_task_vTimerCallback);
    if(NULL == g_hTaskTimer)
    {
        bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_WARNING,"Create g_hTaskTimer fail");
    }
    else
    {
        xTimerStart(g_hTaskTimer,0);
    }
    
    //创建任务
    i32ret = (int32_t)xTaskCreate(app_task
                                ,"app_task"
                                ,APP_TASK_STACK_DEPTH
                                ,NULL
                                ,APP_TASK_PRIORITY
                                ,&g_TaskHandle);
    if(pdPASS != i32ret)
    {
        bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_WARNING,"Create app_task fail");
        i32ret = pdFAIL;
    }
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：删除log任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：
**************************************************************/
int32_t app_task_deinit(void)
{
    int32_t i32ret = pdPASS;

    bsp_task_events_deinit();//删除事件组
    xTimerDelete(g_hTaskTimer,0);//删除定时器，不阻塞
    vQueueDelete(g_hTaskQueue);//删除队列
    vTaskDelete(g_TaskHandle);//删除任务
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：任务管理函数，也就是主函数，管理所有应用层任务的各类操作，还可以去处理喂狗、功能模式组合等
*函数参数 ：
*函数返回值 ：
*备注 ：通过接收队列进入阻塞状态
**************************************************************/
static void app_task(void* pvParameters)
{   
    APP_TASK_QUEUE_REC_T *pRec = NULL;
    APP_TASK_MODULE_E eModule = APP_TASK_MODULE_END;

    taskENTER_CRITICAL(); //进入临界区
    for(APP_TASK_MODULE_E eModule = APP_TASK_MODULE_TASK;eModule < APP_TASK_MODULE_END;eModule++)
    {//依次初始化所有模块任务，task已经创建，在这里不操作
        if(APP_TASK_MODULE_UPGRADE != eModule)
        {//这几个任务初始化阶段不创建
            app_task_create(eModule);
        }
    }
    taskEXIT_CRITICAL(); //退出临界区

    while(1)
    {
        if((pdPASS == xQueueReceive(g_hTaskQueue,(void *)&pRec,portMAX_DELAY))//阻塞等待队列有信息
            && (NULL != pRec))//获取的信息不为NULL
        {
            bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"app task opt:%d",pRec->eOpt);   

            switch (pRec->eOpt)
            {
                case APP_TASK_OPT_SYNC:
                {
                    if(NULL != pRec->pData)
                    {
                        eModule = (APP_TASK_MODULE_E)*pRec->pData;
                        app_task_sync(eModule);
                    }
                    break;
                }
                case APP_TASK_OPT_CREATE:
                {
                    if(NULL != pRec->pData)
                    {
                        eModule = (APP_TASK_MODULE_E)*pRec->pData;
                        app_task_create(eModule);
                    }
                    break;
                }
                case APP_TASK_OPT_DELETE:
                {
                    if(NULL != pRec->pData)
                    {
                        eModule = (APP_TASK_MODULE_E)*pRec->pData;
                        app_task_delete(eModule);
                    }
                    break;
                }
                case APP_TASK_OPT_SUSPEDN:
                {
                    if(NULL != pRec->pData)
                    {
                        eModule = (APP_TASK_MODULE_E)*pRec->pData;
                        app_task_suspend(eModule);
                    }
                    break;
                }
                case APP_TASK_OPT_RESUME:
                {
                    if(NULL != pRec->pData)
                    {
                        eModule = (APP_TASK_MODULE_E)*pRec->pData;
                        app_task_resume(eModule);
                    }
                    break;
                }
                case APP_TASK_OPT_SET_PRI:
                {
                    if(NULL != pRec->pData)
                    {
                        typedef struct
                        {
                            APP_TASK_MODULE_E eModule;
                            UBaseType_t uxNewPriority;
                        }Param_t;
                        Param_t stData;
                        memcpy((void *)&stData,(void *)pRec->pData,sizeof(Param_t));//将净荷数据copy到临时存储空间发然传递到信息队列中
                        app_task_set_priority(stData.eModule,stData.uxNewPriority);
                    }
                    break;
                }
                case APP_TASK_OPT_GET_INFO:
                {
                    if(NULL != pRec->pData)
                    {
                        eModule = (APP_TASK_MODULE_E)*pRec->pData;
                        app_task_get_info(eModule);
                    }
                    break;
                }
                case APP_TASK_OPT_GET_INFO_ALL:
                {
                    app_task_get_info_all();
                    break;
                }
                default:
                    bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_WARNING,"app task opt err:%d",pRec->eOpt);   
                    break;
            }

            //处理完成，释放信息节点
            if(NULL != pRec->pData)
            {
                vPortFree(pRec->pData);
            }
            vPortFree(pRec);
        }
    }
}

/**************************************************************
*函数功能描述 ：连续APP_UPGRADE_TIMEOUT（60s）没有接收到新的数据包就退出接收流程，并删除升级任务
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_task_vTimerCallback(TimerHandle_t xTimer)
{
    if(NULL != xTimer)
    {
        uint32_t ulCount;
        ulCount = ( uint32_t ) pvTimerGetTimerID(xTimer);//获取该定时器的ID
        ulCount++;
        vTimerSetTimerID( xTimer, ( void * ) ulCount);//修改该定时器的ID
        //bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"task count:%d",ulCount);
    }
}

/**************************************************************
*函数功能描述 ：信息入队列，供外部模块调用
*函数参数 ：eOpt，app层自定义操作码，不同的操作码对应不同的数据结构
            pData，待处理数据地址
            u32Len，待处理数据长度
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：因为需要pvPortMalloc（），所以不能用在中断里调用
**************************************************************/
int32_t app_task_opt_put(APP_TASK_OPT_E eOpt,uint8_t *pData,uint32_t u32Len)
{
    int32_t i32ret = pdFAIL;
    APP_TASK_QUEUE_REC_T *pRec = NULL;

    if((eOpt < APP_TASK_OPT_END) && (NULL != g_hTaskQueue))
    {
        pRec = pvPortMalloc(sizeof(APP_TASK_QUEUE_REC_T));//失败就在本函数释放，成功就在task中释放
        if(NULL != pRec)
        {
            pRec->eOpt = eOpt;
            if((NULL != pData) && (0 != u32Len))
            {
                pRec->u32Len = u32Len;
                pRec->pData = (uint8_t *)pvPortMalloc(u32Len);//失败就在本函数释放，成功就在task中释放
                if(NULL != pRec->pData)
                {
                    memcpy(pRec->pData,pData,u32Len);
                }
                else
                {
                    vPortFree(pRec);
                    return i32ret;
                }
            }
            else
            {
                pRec->u32Len = 0;
                pRec->pData = NULL;
            }

            //数据组包完成，放入队列，放满了就阻塞在这里
            (int32_t)xQueueSend(g_hTaskQueue,(void *)&pRec,portMAX_DELAY);
            i32ret = pdPASS;
        }
    }
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：根据任务类型获取索引
*函数参数 ：eModule，应用层自定义任务类型
*函数返回值 ：g_stTaskModuleManagerTab列表中的索引值
*备注 ：
**************************************************************/
static uint8_t app_task_get_index(APP_TASK_MODULE_E eModule)
{
    uint8_t index = UINT8_MAX;

    for(uint8_t i = 0;i < (sizeof(g_stTaskModuleManagerTab)/sizeof(g_stTaskModuleManagerTab[0]));i++)
    {//遍历管理列表查询eModule对应索引i
        if(eModule == g_stTaskModuleManagerTab[i].eModuleName)
        {
            index = i;
        }
    }

    return index;
}

/**************************************************************
*函数功能描述 ：任务同步，任务通知方式，中断调用
*函数参数 ：eModule，应用层自定义任务类型
*函数返回值 ：
*备注 ：供外部中断直接调用，不经过app_task处理
**************************************************************/
void app_task_sync_isr(APP_TASK_MODULE_E eModule)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t index = app_task_get_index(eModule);

    if(UINT8_MAX != index)
    {
        if(NULL != *g_stTaskModuleManagerTab[index].pTaskHandle)
        {//通过任务通知的方式模拟二值信号量
            vTaskNotifyGiveFromISR(*g_stTaskModuleManagerTab[index].pTaskHandle,&xHigherPriorityTaskWoken);//发送任务通知
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//检查是否有更高优先级的任务也在等待这个任务通知，如果是的话进行一次任务切换
        }
    }
}

/**************************************************************
*函数功能描述 ：任务同步，任务通知方式
*函数参数 ：eModule，应用层自定义任务类型
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_task_sync(APP_TASK_MODULE_E eModule)
{
    uint8_t index = app_task_get_index(eModule);

    if(UINT8_MAX != index)
    {
        if(NULL != *g_stTaskModuleManagerTab[index].pTaskHandle)
        {//通过任务通知的方式模拟二值信号量
            xTaskNotifyGive(*g_stTaskModuleManagerTab[index].pTaskHandle);//发送任务通知
        }
    }
}

/**************************************************************
*函数功能描述 ：创建某个任务及其专用信号量、队列等Freertos对象
*函数参数 ：eModule，应用层自定义任务类型
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_task_create(APP_TASK_MODULE_E eModule)
{
    uint8_t index = app_task_get_index(eModule);

    if(UINT8_MAX != index)
    {
        if(NULL != g_stTaskModuleManagerTab[index].CreateTaskFunc)
        {
            (*g_stTaskModuleManagerTab[index].CreateTaskFunc)();//创建各类任务
        }
    }
}

/**************************************************************
*函数功能描述 ：删除某个任务及其专用信号量、队列等Freertos对象
*函数参数 ：eModule，应用层自定义任务类型
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_task_delete(APP_TASK_MODULE_E eModule)
{
    uint8_t index = app_task_get_index(eModule);

    if(UINT8_MAX != index)
    {
        if(NULL != g_stTaskModuleManagerTab[index].DestroyTaskFunc)
        {
            (*g_stTaskModuleManagerTab[index].DestroyTaskFunc)();//删除各类任务
        }
    }
}

/**************************************************************
*函数功能描述 ：挂起某个任务
*函数参数 ：eModule，应用层自定义任务类型
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_task_suspend(APP_TASK_MODULE_E eModule)
{
    uint8_t index = app_task_get_index(eModule);

    if(UINT8_MAX != index)
    {
        if(NULL != *g_stTaskModuleManagerTab[index].pTaskHandle)
        {
            vTaskSuspend(*g_stTaskModuleManagerTab[index].pTaskHandle);
        }
    }
}

/**************************************************************
*函数功能描述 ：恢复挂起的某个任务
*函数参数 ：eModule，应用层自定义任务类型
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_task_resume(APP_TASK_MODULE_E eModule)
{
    uint8_t index = app_task_get_index(eModule);

    if(UINT8_MAX != index)
    {
        if(NULL != *g_stTaskModuleManagerTab[index].pTaskHandle)
        {
            vTaskResume(*g_stTaskModuleManagerTab[index].pTaskHandle);//有中断版本xTaskResumeFromISR（）
            //根据需求还可以调用xTaskAbortDelay（）强制任务进入就绪态
        }
    }
}

/**************************************************************
*函数功能描述 ：设置任务优先级
*函数参数 ：eModule，应用层自定义任务类型
            uxNewPriority，待配置的优先级
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_task_set_priority(APP_TASK_MODULE_E eModule,UBaseType_t uxNewPriority)
{
    uint8_t index = app_task_get_index(eModule);

    if((UINT8_MAX != index) && (uxNewPriority < configMAX_PRIORITIES))
    {
        if(NULL != *g_stTaskModuleManagerTab[index].pTaskHandle)
        {
            vTaskPrioritySet(*g_stTaskModuleManagerTab[index].pTaskHandle,uxNewPriority);
        }
    }
}

/**************************************************************
*函数功能描述 ：获取单个任务的信息
*函数参数 ：eModule，任务类型，通过这个获取任务句柄
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_task_get_info(APP_TASK_MODULE_E eModule)
{
    #if (1 == configUSE_TRACE_FACILITY)
    //获取指定某个任务的信息
    uint32_t u32Size = sizeof(TaskStatus_t);
    TaskStatus_t *pTaskStatus = pvPortMalloc(u32Size);//申请内存
    if(NULL != pTaskStatus)
    {//不获取堆栈剩余的历史最小大小，仅获取状态
        vTaskGetInfo(*g_stTaskModuleManagerTab[eModule].pTaskHandle,pTaskStatus,pdTRUE,eInvalid);
        bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"%s(%02d)-State(%d)\r\nPriority(%02d)-BasePriority(%02d)\r\nStackAddr(0x%08x)-StackSize(%05d)\r\nRunTime:%d\r\n"
                    ,pTaskStatus->pcTaskName,pTaskStatus->xTaskNumber,pTaskStatus->eCurrentState
                    ,pTaskStatus->uxCurrentPriority,pTaskStatus->uxBasePriority
                    ,pTaskStatus->pxStackBase,pTaskStatus->usStackHighWaterMark
                    ,pTaskStatus->ulRunTimeCounter);   
        vPortFree(pTaskStatus);//释放内存
    }

    //获取指定某个任务的状态
    eTaskState eStatus = eTaskGetState(*g_stTaskModuleManagerTab[eModule].pTaskHandle);
    bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"State:%d",eStatus);

    //获取某个任务堆栈剩余空间的历史最低值，单位字
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(*g_stTaskModuleManagerTab[eModule].pTaskHandle);
    bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"StackHighWaterMark:%d = %d bytes",uxHighWaterMark,uxHighWaterMark*4);

    //获取某个任务的优先级
    UBaseType_t uxPriority = uxTaskPriorityGet(*g_stTaskModuleManagerTab[eModule].pTaskHandle);
    bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"Priority:%d",uxPriority);
    #endif
}

/**************************************************************
*函数功能描述 ：获取全部任务的信息
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_task_get_info_all(void)
{
    #if (1 == configUSE_TRACE_FACILITY)
    //获取所有任务的信息——方法1
    uint32_t u32TotalRunTime = 0;
    uint32_t u32tasknum = uxTaskGetNumberOfTasks();//挂起/阻塞/就绪态/运行态的任务+空闲任务
    bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"task num:%d",u32tasknum);   
    uint32_t u32Size = sizeof(TaskStatus_t) * u32tasknum;

    TaskStatus_t *pTaskStatus = pvPortMalloc(u32Size);
    if(NULL != pTaskStatus)
    {
        UBaseType_t ret = uxTaskGetSystemState(pTaskStatus,(UBaseType_t)u32tasknum,&u32TotalRunTime);
        for(uint32_t i = 0;i < u32tasknum;i++)
        {
            bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"%s(%02d)-State(%d)\r\nPriority(%02d)-BasePriority(%02d)\r\nStackAddr(0x%08x)-StackSize(%05d)\r\nRunTime:%d\r\n"
                    ,(pTaskStatus + i)->pcTaskName,(pTaskStatus + i)->xTaskNumber,(pTaskStatus + i)->eCurrentState
                    ,(pTaskStatus + i)->uxCurrentPriority,(pTaskStatus + i)->uxBasePriority
                    ,(pTaskStatus + i)->pxStackBase,(pTaskStatus + i)->usStackHighWaterMark
                    ,(pTaskStatus + i)->ulRunTimeCounter);   
        }
        bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"u32TotalRunTime:%d",u32TotalRunTime);   
        vPortFree(pTaskStatus);//释放内存
    }

    //获取所有任务的信息——方法2
    u32Size = u32tasknum * 40;//每个任务给40bytes
    char *InfoBuffer = pvPortMalloc(u32Size);
    if(NULL != InfoBuffer)
    {
        //名称——状态——优先级——堆栈历史最小剩余大小——编号
        //状态X 是运行态，B 是阻塞态， R 是就绪态， S 是挂起态， D 是删除态
        vTaskList(InfoBuffer); //获取所有任务的信息
        printf("Task list:%d\r\n%s",strlen(InfoBuffer),InfoBuffer);   
        vPortFree(InfoBuffer);//释放内存
    }
    #endif

    #if (1 == configGENERATE_RUN_TIME_STATS)
    //获取所有任务运行时间信息
    u32Size = u32tasknum * 40;//每个任务给40bytes
    char *RunTimeInfo = pvPortMalloc(u32Size);
    if(NULL != RunTimeInfo)
    {
        //名称——运行时间——所占百分比
        //注意实际的运行时间要乘以定时器的周期，例如这里用TIM2的50us计数，若app_task是1664，就是运行了1664*50=83200us=83.2ms
        vTaskGetRunTimeStats(RunTimeInfo);
        printf("Task Time:%d\r\n%s",strlen(RunTimeInfo),RunTimeInfo);   
        vPortFree(RunTimeInfo);//释放内存
    }
    #endif

    //总的堆栈信息，当前可用空间，最小可用空间，bytes
    bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"Heap free:%d bytes,minifree:%d bytes",xPortGetFreeHeapSize(),xPortGetMinimumEverFreeHeapSize());   

    //调度器状态
    bsp_log_print(BSP_LOG_MODULE_TASK,BSP_LOG_TYPE_INFO,"scheduler:%d\r\n",(uint8_t)xTaskGetSchedulerState());   
}

