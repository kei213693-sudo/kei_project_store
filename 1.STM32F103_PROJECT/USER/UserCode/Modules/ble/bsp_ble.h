/*
 * bsp_ble.h
 *
 *  Created on: 2023年6月19日
 *      Author: F704011834_
 *	  Abstract: ble驱动模块，实现ble底层驱动接口
 */
#ifndef _BSP_BLE_H_
#define _BSP_BLE_H_

/*******************************头文件*******************************/

/*******************************宏定义*******************************/
#define BSP_BLE_TX_TIMEOUT          (200)//数据发送超时时间

/*******************************结构体、枚举******************************/

/*******************************全局变量的外部声明extern*******************************/

/*******************************全局函数的声明*******************************/
void bsp_ble_init(uint8_t u8Mode);
void bsp_ble_deinit(void);

void bsp_ble_start_adv(void *pData);
void bsp_ble_stop_adv(void *pData);
void bsp_ble_start_scan(void *pData);
void bsp_ble_stop_scan(void *pData);
void bsp_ble_connect(void *pData);
void bsp_ble_disconnect(void *pData);
void bsp_ble_con_param_updata(void *pData);
void bsp_ble_con_param_updata_res(void *pData);
void bsp_ble_get_rssi(void *pData);
void bsp_ble_set_power(void *pData);
void bsp_ble_enble_notify(void *pData);
void bsp_ble_send_data_notify(void *pData);
void bsp_ble_send_data_command(void *pData);


#endif /* _BSP_BLE_H_ */


