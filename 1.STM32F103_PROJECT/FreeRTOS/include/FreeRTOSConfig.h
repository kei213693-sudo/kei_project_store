/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "main.h"

#if APP_SYSVIEW_ENABLE
#include "SEGGER_SYSVIEW_FreeRTOS.h"//用于SystemView可视化调试
#endif

//针对不同的编译器调用不同的stdint.h文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    extern uint32_t SystemCoreClock;
#endif

/***************************************************************************************************************/
/*                                        FreeRTOS基础配置配置选项                                              */
/***************************************************************************************************************/
//内核对象配置
#define configUSE_QUEUE_SETS					1                       //为1时启用队列
#define configUSE_RECURSIVE_MUTEXES				1                       //为1时启用递归互斥信号量
#define configUSE_COUNTING_SEMAPHORES			1                       //为1时启用计数信号量
#define configUSE_MUTEXES						1                       //为1时启用互斥信号量
#define configQUEUE_REGISTRY_SIZE				8                       //不为0时表示启用队列记录，具体的值是可以
                                                                        //记录的队列和信号量最大数目。

//时钟配置
#define configCPU_CLOCK_HZ						(SystemCoreClock)       //CPU频率
#define configTICK_RATE_HZ						(1000)                  //时钟节拍频率，这里设置为1000，周期就是1ms，设置为20，周期就是50ms
#define configUSE_16_BIT_TICKS					0                       //系统节拍计数器变量数据类型，
                                                                        //1表示为16位无符号整形（8bit可用），0表示为32位无符号整形（24bit可用）

//任务基本配置
#define configMAX_TASK_NAME_LEN					(16)                    //任务名字字符串长度
#define configMAX_PRIORITIES					(32)                    //可使用的最大优先级
//#define configNUM_THREAD_LOCAL_STORAGE_POINTERS       //设置每个任务的本地存储指针数组大小，任务控制块中有本地存储数组指针，用户应用程序可以在这些本地存储中存入一些数据

//任务调度配置
#define configUSE_PREEMPTION					1                       //1使用抢占式内核，0使用协程
#define configUSE_TIME_SLICING					1						//1使能时间片调度(默认式使能的)，允许多个任务可共用一个优先级，只有相同优先级任务才用到时间片调度
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1                       //1启用特殊方法来选择下一个要运行的任务
                                                                        //一般是硬件计算前导零指令，如果所使用的
                                                                        //MCU没有这些硬件指令的话此宏应该设置为0！
#define configUSE_TASK_NOTIFICATIONS            1                       //为1时开启任务通知功能，默认开启，关闭后每个任务每个数组索引可以节省8bytes
//#define configTASK_NOTIFICATION_ARRAY_ENTRIES 1//决定了每项任务的任务通知数组中的索引数，默认是1（对应索引0），可以通过这个宏来扩充，然后就可以调用后缀为Indexed的api
#define configUSE_TICKLESS_IDLE					0                       //1启用低功耗tickless模式
#define configIDLE_SHOULD_YIELD					0                       //为1时空闲任务放弃CPU使用权给其他同优先级的用户任务使用时间片

/***************************************************************************************************************/
/*                                FreeRTOS与运行时间和任务状态收集有关的配置选项                                 */
/***************************************************************************************************************/
#define configUSE_TRACE_FACILITY				1                       //为1启用可视化跟踪调试
#define configUSE_STATS_FORMATTING_FUNCTIONS	1                       //与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数
                                                                        //prvWriteNameToBuffer(),vTaskList(),
                                                                        //vTaskGetRunTimeStats()

#define configGENERATE_RUN_TIME_STATS	        1                       //为1时启用运行时间统计功能
#if (1 == configGENERATE_RUN_TIME_STATS)
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ConfigureTimeForRunTimeStats()//使用一个Tim提供时基
#define portGET_RUN_TIME_COUNTER_VALUE() FreeRTOSRunTimeTicks
#endif

/***************************************************************************************************************/
/*                                FreeRTOS与内存申请有关配置选项                                                */
/***************************************************************************************************************/
#define configSUPPORT_DYNAMIC_ALLOCATION        1                       //支持动态内存申请
#define configTOTAL_HEAP_SIZE					((size_t)(24*1024))     //系统所有总的堆大小，buyes
#define configMINIMAL_STACK_SIZE				((unsigned short)130)   //空闲任务使用的堆栈大小，单位是字，不是字节，也就是130——》520bytes
// #define configAPPLICATION_ALLOCATED_HEAP                             //用于内存动态分配,0：交给编译器自动分配；1：用户可自定义堆内存
// #define configSUPPORT_STATIC_ALLOCATION                  //使能静态创建任务，当此宏定义为 1，在创建一些内核对象的时候需要用户指定 RAM，当为 0 的时候就会自使用 heap.c 中的动态内存管理函数来自动的申请 RAM

/***************************************************************************************************************/
/*                                FreeRTOS与软件定时器有关的配置选项                                            */
/***************************************************************************************************************/
#define configUSE_TIMERS				        1                               //为1时启用软件定时器
#define configTIMER_TASK_PRIORITY		        (configMAX_PRIORITIES-1)        //软件定时器优先级
#define configTIMER_QUEUE_LENGTH		        5                               //软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH	        (configMINIMAL_STACK_SIZE*2)    //软件定时器任务堆栈大小

/***************************************************************************************************************/
/*                                FreeRTOS与中断有关的配置选项                                                  */
/***************************************************************************************************************/
//stm32f103移植时报错，必须使用4，不能使用4U（__NVIC_PRIO_BITS）
//，因为不能修改stm32f103xe.h，所以这里写死为4
#define configPRIO_BITS       		4//MCU优先级bit数，stm32用的是4bit，共16级，从低到高是15~0

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15                      //中断最低优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5                       //系统可管理的最高中断优先级，BASEPRI寄存器的阈值
                                                                                //；这里就是5~15归freertos管理，可以调用相应后缀为FromISR的api
                                                                                //，高于这个优先级的（0~4）不归Freertos管理
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )//左移4bit到高位，0xf0才是寄存器实际的数值；PenSV、系统心跳时钟的中断优先级也是这个
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )//左移4bit到高位，0x50才是寄存器实际的数值；高于此优先级（0~4）的中断不能调用FreeRTOS API，而5~15可以调用

/***************************************************************************************************************/
/*                                FreeRTOS与中断服务函数有关的配置选项                                          */
/***************************************************************************************************************/
//中断优先级是最低的configKERNEL_INTERRUPT_PRIORITY，在xPortStartScheduler()中设置
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler

/***************************************************************************************************************/
/*                                FreeRTOS与钩子函数有关的配置选项                                              */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK						0                       //1，使用空闲钩子；0，不使用
#define configUSE_TICK_HOOK						0                       //1，使用时间片钩子；0，不使用
#define configCHECK_FOR_STACK_OVERFLOW			0                       //大于0时启用堆栈溢出检测功能，如果使用此功能
                                                                        //用户必须提供一个栈溢出钩子函数，如果使用的话
                                                                        //此值可以为1\2\3，因为有3种栈溢出检测方法。
#define configUSE_MALLOC_FAILED_HOOK			0                       //1使用内存申请失败钩子函数
#define configUSE_APPLICATION_TASK_TAG			0                       //决定函数configUSE_APPLICATION_TASK_TAGF()和xTaskCallApplicationTaskHook()是否被编译
// #define configUSE_DAEMON_TASK_STARTUP_HOOK                           //守护任务钩子函数，配合configUSE_TIMERS使用，使能钩子函数vApplicationDaemonTaskStartupHook()，用户实现

/***************************************************************************************************************/
/*                                FreeRTOS与协程有关的配置选项                                                  */
/***************************************************************************************************************/
//协程基本用不上
#define configUSE_CO_ROUTINES 			        0                       //为1时启用协程，启用协程以后必须添加文件croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )                   //协程的有效优先级数目

/***************************************************************************************************************/
/*                                FreeRTOS可选函数配置选项                                                      */
/***************************************************************************************************************/
//本质就是可选api的使能标志，用于条件编译
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_vTaskPrioritySet		        1
#define INCLUDE_uxTaskPriorityGet		        1
#define INCLUDE_vTaskDelete				        1
#define INCLUDE_vTaskCleanUpResources	        1
#define INCLUDE_vTaskSuspend			        1
#define INCLUDE_vTaskDelayUntil			        1
#define INCLUDE_vTaskDelay				        1
#define INCLUDE_eTaskGetState			        1
#define INCLUDE_xTimerPendFunctionCall	        1
#define INCLUDE_uxTaskGetStackHighWaterMark	    1

#if APP_SYSVIEW_ENABLE
#define INCLUDE_xTaskGetIdleTaskHandle          1
#define INCLUDE_pxTaskGetStackStart             1
#endif

/***************************************************************************************************************/
/*                                        FreeRTOS断言函数的实现                                              */
/***************************************************************************************************************/
#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)//类似于c标准库的assert()


#endif /* FREERTOS_CONFIG_H */

