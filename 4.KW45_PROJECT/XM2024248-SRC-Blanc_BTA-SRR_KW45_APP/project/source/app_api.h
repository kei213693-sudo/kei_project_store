/*
 * app_api.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef APP_APP_API_H_

#define APP_APP_API_H_

typedef struct
{
	uint8_t u8Type;
	uint8_t u8ID;
	uint16_t u16Length;
	uint8_t *pData;
}nio_ble_message_protocol_t;//nio ble通信协议结构

extern nio_ble_message_protocol_t *g_pStMessage;
extern uint8_t g_u8MessageData[512];
extern uint8_t g_nio_ble_auth_flag;

#endif /* APP_APP_API_H_ */
