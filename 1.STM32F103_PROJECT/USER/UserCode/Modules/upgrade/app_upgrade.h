/*
 * app_upgrade.h
 *
 *  Created on: 2024年10月28日
 *      Author: lijing
 *	  Abstract: 程序刷写模块，实现多途径程序接收、校验、存储、刷写的流程，app层处理"传输方式、通信协议解析、传输过程加解密"，其他交给bsp层
 */
/*******************************预编译指令*******************************/
#ifndef _APP_UPGRADE_H_
#define _APP_UPGRADE_H_

/*******************************头文件*******************************/
#include "FreeRTOS.h"
#include "task.h"

/*******************************宏定义*******************************/
#define APP_UPGRADE_STACK_DEPTH         (160)//栈空间大小，字,实测最多用到94*4bytes，所以这里给总空间160 * 4 = 640bytes
#define APP_UPGRADE_PRIORITY            (configMAX_PRIORITIES - 2)//栈优先级，次高

#define APP_UPGRADE_QUEUE_LEN           (4)//队列能够容纳的最大项目数

#define APP_UPGRADE_SEC_EN              (0)//传输加解密使能标志，使能后发送/接收数据都要加解密一遍，常用于无线传输且不带自定加密的场景
#define APP_UPGRADE_BACKGROUND_EN       (0)//后台运行使能标志，使能后接收数据包的过程中还能执行其他操作，否则只能等待升级流程结束

#define APP_UPGRADE_SINGLE_LEN          (128)//自定义协议数据包单次传输有效数据长度
#define APP_UPGRADE_TIMEOUT             (5*60*1000)//超时失败时间，ms

/*******************************结构体、枚举******************************/
typedef enum
{//app自定义事件，在这里区分传输方式、通信协议
    APP_UPGRADE_OPT_YMODEM            = 0,//Ymodem协议传输升级包
    APP_UPGRADE_OPT_START,                //起始包
    APP_UPGRADE_OPT_DATA,                 //数据包
    APP_UPGRADE_OPT_STOP,                 //终止包
    APP_UPGRADE_OPT_END
}APP_UPGRADE_OPT_E;

typedef struct
{
    APP_UPGRADE_OPT_E eOpt;//app层自定义操作码
    uint32_t u32Len;//该数据长度
    uint8_t *pData;//指向实际要操作的数据，每个操作码都可以自定义格式
    uint8_t res[3];
}APP_UPGRADE_QUEUE_REC_T;//传入队列的信息，根据不同的enOpt，可以随意扩展data格式

/*******************************全局变量的外部声明extern*******************************/
extern TaskHandle_t g_UpgradeTaskHandle;//任务句柄

/*******************************全局函数的声明*******************************/
extern int32_t app_upgrade_init(void);
extern int32_t app_upgrade_deinit(void);

extern int32_t app_upgrade_opt_put(APP_UPGRADE_OPT_E eOpt,uint8_t *pData,uint32_t u32Len);

#endif /* _APP_UPGRADE_H_ */
