/*
 * bleAppData.h
 *
 *  Created on: 2024年8月22日
 *      Author: F704012276_
 */

#ifndef BLEAPP_BLEAPPMSG_H_
#define BLEAPP_BLEAPPMSG_H_

void CanTp_MsgRevHandle(uint8_t deviceId);
void CanTp_MsgSendHandle(void);
void BleTp_MsgRevHandle(uint8_t deviceId);
void BleTp_MsgSendHandle(void);

uint8_t bleMsgRx_0x01_0x03_IndAppFunc(uint8_t deviceId, void* pBleMsg);
uint8_t bleMsgRx_0x04_0x0B_IndAppFunc(uint8_t deviceId, void* pBleMsg);
uint8_t bleMsgRx_0x40_0x01_IndAppFunc(uint8_t deviceId, void* pBleMsg);
uint8_t bleMsgRx_0x84_0x01_IndAppFunc(uint8_t deviceId, void* pBleMsg);
uint8_t bleMsgRx_0x84_0x03_IndAppFunc(uint8_t deviceId, void* pBleMsg);
uint8_t bleMsgRx_0x84_0x05_IndAppFunc(uint8_t deviceId, void* pBleMsg);
uint8_t bleMsgRx_0x84_0x07_IndAppFunc(uint8_t deviceId, void* pBleMsg);
uint8_t bleMsgRx_0x84_0x09_IndAppFunc(uint8_t deviceId, void* pBleMsg);

#endif /* BLEAPP_BLEAPPMSG_H_ */
