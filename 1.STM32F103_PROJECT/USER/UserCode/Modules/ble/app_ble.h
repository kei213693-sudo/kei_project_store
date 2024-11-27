/*
 * app_ble.h
 *
 *  Created on: 2023年6月19日
 *      Author: F704011834_
 *	  Abstract: ble模块，实现ble应用逻辑
 */
#ifndef _APP_BLE_H_
#define _APP_BLE_H_

/*******************************头文件*******************************/
#include "bsp_task_events.h"
#include "bsp_protocol.h"

/*******************************宏定义*******************************/
#define APP_BLE_STACK_DEPTH         (128)//栈空间大小
#define APP_BLE_PRIORITY            (4)//栈优先级
#define APP_BLE_ALL_EVENTS          (APP_BLE_STACK_EVENTS | APP_BLE_TX_EVENTS | APP_BLE_RX_EVENTS)//事件组标志
#define APP_BLE_QUEUE_LEN           (4)//队列能够容纳的最大项目数

#define APP_BLE_TIME_PERIOD         (5000)//定时器定时事件，ms

#define APP_BLE_CON_NAME            "FRD1234567"//对端广播包名称，后续修改为从内部flash读取

#define APP_BLE_SEC_EN              (0)//应用层加密使能标志，收发需要解密/加密操作

#define APP_BLE_UPGRADE_LEN         (128)//升级数据包单包长度，两端要提前保持一致

/*******************************结构体、枚举******************************/
typedef enum
{//app自定义操作
    //ble协议栈操作
    //主动发起
    APP_BLE_OPT_START_ADV              = 0,
    APP_BLE_OPT_STOP_ADV,                    
    APP_BLE_OPT_START_SCAN,                  
    APP_BLE_OPT_STOP_SCAN,
    APP_BLE_OPT_SET_POWER,                 //配置rf射频功率
    APP_BLE_OPT_GAP_CON,                   //发起ble连接
    APP_BLE_OPT_GAP_DISCON,                //断开ble连接
    APP_BLE_OPT_GAP_RSSI,                  //获取RSSI
    APP_BLE_OPT_GATT_CLIENT_TX,            //客户端（主机）发送数据，有“指令Command、读/写请求Request”两种方式
    APP_BLE_OPT_GATT_SERVER_TX,            //服务器（从机）发送数据，有“通知Notify、读/写指示Indication”两种方式,9

    //被动响应
    APP_BLE_OPT_SCAN_REPORT,               //传递扫描到的广播包信息，决定是否要进行连接    
    APP_BLE_OPT_GATT_CLIENT_RX,            //客户端（主机）接收数据，对应APP_BLE_OPT_GATT_SERVER_TX
    APP_BLE_OPT_GATT_SERVER_RX,            //服务器（从机）接收数据，对应APP_BLE_OPT_GATT_CLIENT_TX

    //自定义协议
    APP_BLE_OPT_UPGRADE_START,             //升级起始包
    APP_BLE_OPT_UPGRADE_DATA,              //升级数据包
    APP_BLE_OPT_UPGRADE_STOP,              //升级停止包,15
    APP_BLE_OPT_BUTTON,                    //按键指令

    APP_BLE_OPT_END
}APP_BLE_OPT_E;

typedef struct
{
    APP_BLE_OPT_E enOpt; //app层自定义操作码
    uint32_t u32Len;//该数据长度
    uint8_t *pData;//指向实际要操作的数据
    uint8_t res[3];
}APP_BLE_QUEUE_REC_T;//传入队列的信息，根据不同的enOpt，可以随意扩展data格式

typedef struct
{//自定义协议tx/rx控制结构体，将opt、cmd、实际函数指针联系起来
    APP_BLE_OPT_E enOpt;//app层事件

    PROTOCOL_CMD_E enCmdTx;//ble任务主动处理的指令
    void (*appBleFucTx)(void *);//mcu将组包完成的数据给comm或直接通过io口操作ble芯片
    
    PROTOCOL_CMD_E enCmdRx;//ble任务被动接收的指令
    void (*appBleFucRx)(void *);//ble进程根据队列信息依次执行相应操作接口
}APP_BLE_CMD_FUC_TAB_T;

typedef struct
{//ble协议栈控制结构体，将opt、实际函数指针联系起来
    APP_BLE_OPT_E enOpt;//app层事件
    void (*appBleFuc)(void *);//mcu将组包完成的数据给comm或直接通过io口操作ble芯片
}APP_BLE_STACK_FUC_TAB_T;

/*******************************全局变量的外部声明*******************************/
extern TaskHandle_t g_BleTaskHandle;//任务句柄

/*******************************全局函数的声明*******************************/
extern int32_t app_ble_init(void);
extern int32_t app_ble_deinit(void);
extern int32_t app_ble_opt_put(EventBits_t events,APP_BLE_OPT_E enOpt,uint8_t *pData,uint32_t u32Len);

extern uint16_t app_ble_protocol_last_rsp(void);
extern void app_ble_protocol_tx(uint8_t *pBuf,uint32_t len);
extern void app_ble_protocol_rx(uint8_t *pBuf,uint32_t len);


#endif /* _APP_BLE_H_ */
