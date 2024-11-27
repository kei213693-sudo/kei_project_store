/*
 * bsp_task_events.h
 *
 *  Created on: 2024年4月18日
 *      Author: lijing
 */
/*******************************预编译指令*******************************/
#ifndef _BSP_TASK_EVENTS_H_
#define _BSP_TASK_EVENTS_H_

/*******************************头文件*******************************/
#include "FreeRTOS.h"
#include "event_groups.h"

/*******************************宏定义*******************************/
//事件组位类型，在freertos中，如果是32bit变量的话，高8bit（ID_24~ID_31）是系统用的，用户只能用ID_00~ID_23
#define BSP_TASK_EVENTS_ID_00 (0x00000001)
#define BSP_TASK_EVENTS_ID_01 (0x00000002)
#define BSP_TASK_EVENTS_ID_02 (0x00000004)
#define BSP_TASK_EVENTS_ID_03 (0x00000008)
#define BSP_TASK_EVENTS_ID_04 (0x00000010)
#define BSP_TASK_EVENTS_ID_05 (0x00000020)
#define BSP_TASK_EVENTS_ID_06 (0x00000040)
#define BSP_TASK_EVENTS_ID_07 (0x00000080)
#define BSP_TASK_EVENTS_ID_08 (0x00000100)
#define BSP_TASK_EVENTS_ID_09 (0x00000200)
#define BSP_TASK_EVENTS_ID_10 (0x00000400)
#define BSP_TASK_EVENTS_ID_11 (0x00000800)
#define BSP_TASK_EVENTS_ID_12 (0x00001000)
#define BSP_TASK_EVENTS_ID_13 (0x00002000)
#define BSP_TASK_EVENTS_ID_14 (0x00004000)
#define BSP_TASK_EVENTS_ID_15 (0x00008000)
#define BSP_TASK_EVENTS_ID_16 (0x00010000)
#define BSP_TASK_EVENTS_ID_17 (0x00020000)
#define BSP_TASK_EVENTS_ID_18 (0x00040000)
#define BSP_TASK_EVENTS_ID_19 (0x00080000)
#define BSP_TASK_EVENTS_ID_20 (0x00100000)
#define BSP_TASK_EVENTS_ID_21 (0x00200000)
#define BSP_TASK_EVENTS_ID_22 (0x00400000)
#define BSP_TASK_EVENTS_ID_23 (0x00800000)
#define BSP_TASK_EVENTS_ID_24 (0x01000000)
#define BSP_TASK_EVENTS_ID_25 (0x02000000)
#define BSP_TASK_EVENTS_ID_26 (0x04000000)
#define BSP_TASK_EVENTS_ID_27 (0x08000000)
#define BSP_TASK_EVENTS_ID_28 (0x10000000)
#define BSP_TASK_EVENTS_ID_29 (0x20000000)
#define BSP_TASK_EVENTS_ID_30 (0x40000000)
#define BSP_TASK_EVENTS_ID_31 (0x80000000)
#define BSP_TASK_EVENTS_ID_NONE (0)

//各模块要用的bit定义
#define APP_BLE_STACK_EVENTS            (BSP_TASK_EVENTS_ID_00)//协议栈事件
#define APP_BLE_TX_EVENTS               (BSP_TASK_EVENTS_ID_01)//自定义协议发送事件
#define APP_BLE_RX_EVENTS               (BSP_TASK_EVENTS_ID_02)//自定义协议接收事件

#define APP_COMM_TX_EVENTS              (BSP_TASK_EVENTS_ID_03)
#define APP_COMM_RX_EVENTS              (BSP_TASK_EVENTS_ID_04)
#define APP_COMM_IRQ_BLE_EVENTS         (BSP_TASK_EVENTS_ID_05)
/*******************************结构体、枚举******************************/

/*******************************全局变量的外部声明extern*******************************/

/*******************************全局函数的声明*******************************/
extern int32_t bsp_task_events_init(void);
extern int32_t bsp_task_events_deinit(void);

extern void bsp_task_events_set(EventBits_t events);
extern void bsp_task_events_set_isr(EventBits_t events);
extern EventBits_t bsp_task_events_wait(EventBits_t AllEvents,uint32_t WaitTimeMS);
extern EventBits_t bsp_task_events_wait_snyc(EventBits_t SetEvents,EventBits_t WaitEvents,uint32_t WaitTimeMS);
extern EventBits_t bsp_task_events_get(void);

#endif /* _BSP_TASK_EVENTS_H_ */
