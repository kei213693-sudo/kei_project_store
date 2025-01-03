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

/*******************************宏定义*******************************/
#ifndef pdFAIL
#define pdFAIL                  (0)
#endif
#ifndef pdPASS
#define pdPASS                  (1)
#endif

#define APP_BLE_TASK_ID         TASK_USR_FIRST
#define APP_BLE_IDX_MAX         (1)//这个值到底是什么含义？？？

/*******************************结构体、枚举******************************/
typedef enum
{//app自定义操作
    //ble协议栈操作
    APP_BLE_OPT_START_ADV              = 0,
    APP_BLE_OPT_STOP_ADV,                    
    APP_BLE_OPT_SET_POWER,                 //配置rf射频功率
    APP_BLE_OPT_GAP_DISCON,                //断开ble连接
    APP_BLE_OPT_GAP_RSSI,                  //获取RSSI
    APP_BLE_OPT_SMP_DEL,                   //删除配对信息

    //自定义操作
    APP_BLE_OPT_DEBUGLOG,                  //处理调试指令
    APP_BLE_OPT_MCU_RX,                    //处理从mcu接收到的数据

    APP_BLE_OPT_END
}APP_BLE_OPT_E;//在ke-rtos中用于APP_BLE_TASK_ID的MSG_ID

/*******************************全局变量的外部声明*******************************/

/*******************************全局函数的声明*******************************/
extern int32_t app_ble_init(void);
extern int32_t app_ble_deinit(void);

#endif /* _APP_BLE_H_ */
