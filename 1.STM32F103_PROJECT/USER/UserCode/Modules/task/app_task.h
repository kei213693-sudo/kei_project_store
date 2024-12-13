/*
 * app_task.h
 *
 *  Created on: 2023年6月25日
 *      Author: lj
 *	  Abstract: 用于rtos任务的管理（以freertos接口为例），负责任务创建、销毁、设置、获取等操作
 */
#ifndef _APP_TASK_H_
#define _APP_TASK_H_
/*******************************头文件*******************************/
#include "FreeRTOS.h"
#include "task.h"

/*******************************宏定义*******************************/
#define APP_TASK_STACK_DEPTH         (200)//栈空间大小，字，200*4 = 800bytes
#define APP_TASK_PRIORITY            (configMAX_PRIORITIES - 1)//栈优先级，最高

#define APP_TASK_QUEUE_LEN           (4)//队列能够容纳的最大项目数
#define APP_TASK_TIMEOUT             (1*1000)//超时失败时间，ms

//开发环境确定
// #define APP_TASK_MCU_TYPE           MCU_TYPE_SNJ32W103
// #define APP_TASK_OS_TYPE            OS_TYPE_FREERTOS
/*******************************结构体、枚举等******************************/

//模块列表枚举: 这里列举的是该系统所有的可用任务模块
//，而不同型号的产品可以根据宏定义作为开关选择使用部分模块
//，程序运行时，也可以动态控制部分任务销毁和创建。
typedef enum
{ 
    APP_TASK_MODULE_TASK              = 0,//任务管理任务
    APP_TASK_MODULE_LOG,                  //调试日志任务
    APP_TASK_MODULE_BLE,                  //蓝牙管理任务
    APP_TASK_MODULE_LED,                  //LED灯任务
    APP_TASK_MODULE_BUTTON,               //按键任务
    APP_TASK_MODULE_COMM,                 //comm任务
    APP_TASK_MODULE_UPGRADE,             //程序刷写任务
    //根据项目实际扩充
    APP_TASK_MODULE_END
}APP_TASK_MODULE_E;

//模块对应任务创建、消除: 实际初始化时就根据这个表格从开头到最后依次初始化各个模块并创建相应任务;
//而在休眠前则从最后到开头依次释放 (去初始化)各个模块，这里还要注意有些模块休眠不能释放，
//例如“低功耗管理”，具体要根据实际的功能决定
typedef struct app_task_module_tab_t
{
    APP_TASK_MODULE_E eModuleName;//模块类别
    int32_t (*CreateTaskFunc)(void);//任务和其他资源创建
    int32_t (*DestroyTaskFunc)(void);//任务和其他资源删除
    TaskHandle_t *pTaskHandle;//任务句柄，用于任务通知、信息获取等
}app_task_module_tab_t;

typedef enum
{//app自定义事件
    APP_TASK_OPT_SYNC                 = 0,//任务同步
    APP_TASK_OPT_CREATE,                  //任务初始化
    APP_TASK_OPT_DELETE,                  //任务去初始化
    APP_TASK_OPT_SUSPEDN,                 //任务挂起
    APP_TASK_OPT_RESUME,                  //任务恢复
    APP_TASK_OPT_SET_PRI,                 //任务优先级设置
    APP_TASK_OPT_GET_INFO,                //单个任务信息获取
    APP_TASK_OPT_GET_INFO_ALL,            //所有任务信息获取

    APP_TASK_OPT_END
}APP_TASK_OPT_E;

typedef struct
{
    APP_TASK_OPT_E eOpt;//app层自定义操作码
    uint32_t u32Len;//该数据长度
    uint8_t *pData;//指向实际要操作的数据，每个操作码都可以自定义格式
    uint8_t res[3];
}APP_TASK_QUEUE_REC_T;//传入队列的信息，根据不同的enOpt，可以随意扩展data格式

/*******************************全局变量的外部声明extern*******************************/
/*******************************全局函数的声明*******************************/
extern int32_t app_task_init(void);
extern int32_t app_task_deinit(void);

extern int32_t app_task_opt_put(APP_TASK_OPT_E eOpt,uint8_t *pData,uint32_t u32Len);
extern void app_task_sync_isr(APP_TASK_MODULE_E eModule);

#endif /* APPLICATION_TASK__APP_TASK_H_ */
