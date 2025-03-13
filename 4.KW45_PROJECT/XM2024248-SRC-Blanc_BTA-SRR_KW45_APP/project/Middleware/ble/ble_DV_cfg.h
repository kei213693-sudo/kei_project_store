/*
 * bleDV_Cfg.h
 *
 *  Created on: 2024年6月25日
 *      Author: F704012276_
 */

#ifndef SIGNALS_BLE_BLE_DV_CFG_H_
#define SIGNALS_BLE_BLE_DV_CFG_H_

extern uint8_t						bleTagAddressGroup[6];

extern uint8_t BLE_AddressIndex;
#define Get_BLE_AddressIndex()				(BLE_AddressIndex)
#define Set_BLE_AddressIndex(x)				(BLE_AddressIndex = x)

extern uint8_t BLE_DV_ConnectingSts;
#define Get_BLE_DV_ConnectingSts()			(BLE_DV_ConnectingSts)
#define Set_BLE_DV_ConnectingSts(x)			(BLE_DV_ConnectingSts = x)
#define BLE_DV_NO_CONNECTION		0
#define BLE_DV_IS_CONNECTING		1

extern uint8_t BLE_ErrInfo;
#define Get_BLE_ErrInfo()					(BLE_ErrInfo)
#define Set_BLE_ErrInfo(x)					(BLE_ErrInfo = x)
#define BLE_ErrInfo_NO_DATA						2
#define BLE_ErrInfo_WRONG_DATA					1
#define BLE_ErrInfo_SUCCESS						0

extern uint8_t emcBleSlaveRecvFlag;
#define Get_emcBleSlaveRecvFlag()					(emcBleSlaveRecvFlag)
#define Set_emcBleSlaveRecvFlag(x)				(emcBleSlaveRecvFlag = x)
#define EMC_BLE_FAIL_WRONGE_DATA				1
#define EMC_BLE_FAIL_NO_DATA					2
#define EMC_BLE_SUCCESS							0
#define EMC_BLE_DEFAULT							0xff

extern uint8_t BLE_State;
#define Get_BLE_State()							(BLE_State)
#define Set_BLE_State(x)						(BLE_State = x)
#define BLE_DV_STATE_ERROR				1
#define BLE_DV_STATE_NORMAL				0


extern uint8_t BLE_DataErrCnt;
#define Get_BLE_DataErrCnt()					(BLE_DataErrCnt)
#define Set_BLE_DataErrCnt(x)					(BLE_DataErrCnt = x)

extern uint8_t BLE_RecvErrCnt;
#define Get_BLE_RecvErrCnt()					(BLE_RecvErrCnt)
#define Set_BLE_RecvErrCnt(x)					(BLE_RecvErrCnt = x)

extern uint8_t BLE_DV_Master_State;
#define Get_BLE_DV_Master_State()					(BLE_DV_Master_State)
#define Set_BLE_DV_Master_State(x)					(BLE_DV_Master_State = x)
#define BLE_EMC_MASTER_RECEIVE_SUCCESS			0
#define BLE_EMC_MASTER_RECEIVE_WRONG_DATA		1
#define BLE_EMC_MASTER_NO_RECEIVE				2

#endif /* SIGNALS_BLE_BLE_DV_CFG_H_ */
