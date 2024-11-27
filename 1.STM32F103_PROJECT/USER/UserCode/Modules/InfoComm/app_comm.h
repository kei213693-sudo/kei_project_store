/*
 * app_comm.h
 *
 *  Created on: 2023年6月26日
 *      Author: F704011834_
 *	  Abstract: 用于数据的队列管理，实现所有通信渠道的数据收发中转、发送失败重试的功能
 */

#ifndef _APP_COMM_H_
#define _APP_COMM_H_
/*******************************头文件*******************************/
#include "bsp_task_events.h"
#include "bsp_comm.h"

/*******************************宏定义*******************************/
#define APP_COMM_STACK_DEPTH            (200)//
#define APP_COMM_PRIORITY               (5)//数字越大，优先级越高，优先级要高于对应ble、wifi、uwb等任务
#define APP_COMM_ALL_EVENTS             (APP_COMM_TX_EVENTS | APP_COMM_RX_EVENTS | APP_COMM_IRQ_BLE_EVENTS)//事件组标志
#define APP_COMM_QUEUE_LEN              (4)//队列能够容纳的最大项目数

/*******************************结构体、枚举等******************************/
typedef struct
{
    uint8_t queueIndex;//传入数据索引
    uint8_t res[3];
}APP_COMM_QUEUE_REC_T;//传入队列的信息，根据不同的enOpt，可以随意扩展data格式

typedef enum
{//数据传输通道选择
    APP_COMM_CHANNEL_BLE   = 0,//wifi传递的信息
    APP_COMM_CHANNEL_WIFI     ,//ble传递的信息
    APP_COMM_CHANNEL_END
}APP_COMM_CHANNEL_E;

/*******************************全局变量的外部声明*******************************/
extern TaskHandle_t g_CommTaskHandle;

/*******************************全局函数的声明*******************************/
extern int32_t app_comm_init(void);
extern int32_t app_comm_deinit(void);
extern int32_t app_comm_txEnQueue(uint8_t *buf,uint32_t len,uint8_t channel,uint16_t waitcmd);
extern int32_t app_comm_rxEnQueue(uint8_t *buf,uint32_t len,uint8_t channel);


#endif /* APPLICATION_COMM_APP_COMM_H_ */
