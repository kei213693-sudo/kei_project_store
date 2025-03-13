/*
 * bleSrv_cfg.h
 *
 *  Created on: 2024年8月8日
 *      Author: F704012276_
 */

#ifndef BLE_BLESRV_CFG_H_
#define BLE_BLESRV_CFG_H_

#define T_BTM_Adv_Interval			100
#define Conn_Interval_Min			25
#define Conn_Interval_Max			25
#define C_BTM_Slave_Latency			0
#define T_BTM_Sscan_Interval		1*1000
#define T_BTM_Fscan_Interval		260
#define T_BTM_Scan_Window			205
#define T_BTM_Scan_Duration			0


typedef struct{
	union{
		uint8_t data[71];
		struct{
			union{
				uint32_t data[1];
				struct{
					uint32_t T_BTM_Adv_Interval_p;
				}params;
			}adv;

			union{
				uint32_t data[5];
				struct{
					uint32_t C_BTM_Slave_Latency_p;
					uint32_t T_BTM_Sscan_Interval_p;
					uint32_t T_BTM_Fscan_Interval_p;
					uint32_t T_BTM_Scan_Window_p;
					uint32_t T_BTM_Scan_Duration_p;
				}params;
			}scan;

			union{
				uint32_t data[2];
				struct{
					uint32_t T_Conn_Interval_Min_p;
					uint32_t T_Conn_Interval_Max_p;
				}params;
			}conn;

			union{
				uint8_t data[39];
				struct{
					uint8_t device_address[6];
					uint8_t device_id[16];
					uint8_t vehicle_id[16];
					uint8_t device_type;
				}params;
			}preSet;

		}bytes;
	}bytes;
}bleParams_t;

#endif /* BLE_BLESRV_CFG_H_ */
