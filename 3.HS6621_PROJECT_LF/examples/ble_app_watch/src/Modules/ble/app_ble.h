/*
 * app_ble.h
 *
 *  Created on: 2023年6月19日
 *	  Abstract: ble模块，实现ble应用逻辑
 */
#ifndef _APP_BLE_H_
#define _APP_BLE_H_

/*******************************头文件*******************************/
#include <stdint.h>
#include "co_timer.h"
#include "bsp_protocol.h"
#include "gap.h"

/*******************************宏定义*******************************/
#ifndef pdFAIL
#define pdFAIL                  (0)
#endif
#ifndef pdPASS
#define pdPASS                  (1)
#endif

#define APP_BLE_TASK_ID         TASK_USR_FIRST
#define APP_BLE_IDX_MAX         (1)//这个值到底是什么含义？？？

#define APP_BLE_SN_LEN 			(9)//SN码长度，bytes，小于等于16

#define APP_BLE_PAIR_TIMEROUT 	(180*1000)//允许配对的超时时间，ms
#define APP_BLE_PAIR_NUM_MAX 	(4)//最大允许配对个数

#define APP_BLE_RSSI_NUM        (64)//

#define APP_BLE_MSG_SWITCH      (0)//传递信息的方式，1直接内部调用，0重新传给ble task让其再处理

/*******************************结构体、枚举******************************/
typedef enum
{//app自定义操作
    //ble协议栈操作
    APP_BLE_OPT_START_ADV              = 0,
    APP_BLE_OPT_STOP_ADV,                  
    APP_BLE_OPT_UPDATA_PARAM,              //更新连接参数

    //自定义操作
    APP_BLE_OPT_DEBUGLOG,                  //处理调试指令
    APP_BLE_OPT_MCU_RX,                    //muc rx统一处理
    APP_BLE_OPT_MCU_TX,                    //muc tx统一处理

    //自定义协议
    APP_BLE_OPT_SN_SET,                    //SN设置
    APP_BLE_OPT_BOND_ADD,                  //启动配对模式   
    APP_BLE_OPT_BOND_DEL,                  //删除配对信息
    APP_BLE_OPT_PARAM_SET,                 //配置ble参数
    APP_BLE_OPT_RSSI_SEND,                 //传递RSSI给MCU

    APP_BLE_OPT_END
}APP_BLE_OPT_E;//在ke-rtos中用于APP_BLE_TASK_ID的MSG_ID

typedef enum
{//rssi操作枚举
    APP_BLE_RSSI_PERIOD_SET            = 0,//设置rssi获取周期
    APP_BLE_RSSI_START,                    //启动获取rssi
    APP_BLE_RSSI_UPDATA_DATA,              //更新rssi数值
    APP_BLE_RSSI_STOP,                     //停止获取rssi

    APP_BLE_RSSI_END
}APP_BLE_RSSI_E;

typedef struct
{//存储在内部flash的系统参数
    uint8_t sn[APP_BLE_SN_LEN];//序列号
    uint8_t res0[16 - APP_BLE_SN_LEN];//预留

    uint8_t administratorMAC[GAP_BD_ADDR_LEN];//管理员用户mac addr，用于权限认定
    uint8_t pairNum;//已配对手机个数
    uint8_t res1;//预留

    uint16_t AdvInterval;//广播间隔, *0.625ms
    uint8_t res2[2];//预留
}app_ble_sys_info_t;

typedef struct
{//自定义协议tx/rx控制结构体，将opt、cmd、实际函数指针联系起来
    APP_BLE_OPT_E enOpt;//app层事件

    PROTOCOL_CMD_E enCmdTx;//ble任务主动处理的指令
    void (*appBleFucTx)(void *);
    
    PROTOCOL_CMD_E enCmdRx;//ble任务被动接收的指令
    void (*appBleFucRx)(void *);
}APP_BLE_CMD_FUC_TAB_T;

typedef struct
{
    app_ble_sys_info_t stAppBleSysInfo;
    uint32_t u32Pincode;//配对码
    uint8_t  u8AdminFlag;//管理员标志
    uint8_t  u8RssiLen;//rssi接收长度
    uint16_t u16RssiGetPeriod;//获取rssi值的时间间隔，和连接间隔保持一致
    int8_t g_i8RssiBuf[APP_BLE_RSSI_NUM];//存储rssi数值，用于过滤
}app_ble_ctl_t;

/*******************************全局变量的外部声明*******************************/

/*******************************全局函数的声明*******************************/
extern int32_t app_ble_init(void);
extern int32_t app_ble_deinit(void);

extern uint16_t app_ble_adv_inter_get(void);
extern uint8_t *app_ble_sn_get(void);
extern uint32_t app_ble_pair_check(void);//控制u32Pincode
extern void app_ble_pair_succ_opt(uint8_t *pMacAddr);//控制u32Pincode，stAppBleSysInfo的administratorMAC、pairNum
extern void app_ble_admin_check(uint8_t *pMacAddr);//控制u8AdminFlag
extern uint8_t app_ble_admin_get(void);
extern uint8_t *app_ble_admin_mac_get(void);
extern void app_ble_rssi_opt(APP_BLE_RSSI_E enOpt,void *pData);//控制u8RssiLen、g_i8RssiBuf、u16RssiGetPeriod


#endif /* _APP_BLE_H_ */
