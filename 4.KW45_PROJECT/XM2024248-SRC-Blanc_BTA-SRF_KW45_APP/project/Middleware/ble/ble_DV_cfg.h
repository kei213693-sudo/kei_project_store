/*
 * bleDV_Cfg.h
 *
 *  Created on: 2024年6月25日
 *      Author: F704012276_
 */

#ifndef SIGNALS_BLE_BLE_DV_CFG_H_
#define SIGNALS_BLE_BLE_DV_CFG_H_

void Set_BlePublicAddress(uint8_t* address);
void Get_BlePublicAddress(uint8_t* address);


extern uint8_t bleInitFailureFlag;
#define Update_uint8_bleInitFailureFlag()		(bleInitFailureFlag++)
#define Set_uint8_bleInitFailureFlag(x)			(bleInitFailureFlag = x)
#define Get_uint8_bleInitFailureFlag()			(bleInitFailureFlag)
#define BLE_INIT_FAILURE_FLAG					1
#define BLE_INIT_SUCCESS_FLAG					0

extern uint8_t NBUInitFailureFlag;
#define Update_uint8_NBUInitFailureFlag()		(NBUInitFailureFlag++)
#define Set_uint8_NBUInitFailureFlag(x)			(NBUInitFailureFlag = x)
#define Get_uint8_NBUInitFailureFlag()			(NBUInitFailureFlag)
#define BLE_NBU_INIT_FAILURE_FLAG					1
#define BLE_NBU_INIT_SUCCESS_FLAG					0

extern uint8_t BLE_ADV_Cmd;
#define Get_uint8_BLE_ADV_Cmd()					(BLE_ADV_Cmd)
#define Set_uint8_BLE_ADV_Cmd(x)				(BLE_ADV_Cmd = x)
#define BLE_ADV_CMD_OPEN						1
#define BLE_ADV_CMD_CLOSE						0

extern uint8_t ble_rssi_value;
#define Get_uint8_bleRssiValue()				(ble_rssi_value)
#define Set_uint8_bleRssiValue(x)				(ble_rssi_value = x)

extern uint8_t Did4A56_Value;
#define Get_Did_4A56_Value()					(Did4A56_Value)
#define Set_Did_4A56_Value(x)					(Did4A56_Value = x)
#define DV_BLE_OPEN_INVALID						0xff
#define DV_BLE_OPEN_ENABLE						1
#define DV_BLE_OPEN_DISABLE						0

extern uint8_t BLE_SleepFlag;
#define Get_BLE_SleepFlag()						(BLE_SleepFlag)
#define Set_BLE_SleepFlag(x)					(BLE_SleepFlag = x)
#define DV_BLE_Sleep_Allow						1
#define DV_BLE_Sleep_Disallow					0

extern uint8_t BLE_ErrInfo;
#define Get_BLE_ErrInfo()						(BLE_ErrInfo)
#define Set_BLE_ErrInfo(x)						(BLE_ErrInfo = x)
#define BLE_ErrInfo_NO_DATA						2
#define BLE_ErrInfo_WRONG_DATA					1
#define BLE_ErrInfo_SUCCESS						0

extern uint8_t emcBleSlaveRecvFlag;
#define Get_emcBleSlaveRecvFlag()				(emcBleSlaveRecvFlag)
#define Set_emcBleSlaveRecvFlag(x)				(emcBleSlaveRecvFlag = x)
#define EMC_BLE_FAIL_WRONGE_DATA				1
#define EMC_BLE_FAIL_NO_DATA					2
#define EMC_BLE_SUCCESS							0
#define EMC_BLE_DEFAULT							0xff

extern uint8_t BLE_State;
#define Get_BLE_State()							(BLE_State)
#define Set_BLE_State(x)						(BLE_State = x)
#define BLE_DV_STATE_ERROR						1
#define BLE_DV_STATE_NORMAL						0


extern uint8_t BLE_DataErrCnt;
#define Get_BLE_DataErrCnt()					(BLE_DataErrCnt)
#define Set_BLE_DataErrCnt(x)					(BLE_DataErrCnt = x)

extern uint8_t BLE_RecvErrCnt;
#define Get_BLE_RecvErrCnt()					(BLE_RecvErrCnt)
#define Set_BLE_RecvErrCnt(x)					(BLE_RecvErrCnt = x)

extern uint8_t BLE_DV_Master_State;
#define Get_BLE_DV_Master_State()				(BLE_DV_Master_State)
#define Set_BLE_DV_Master_State(x)				(BLE_DV_Master_State = x)
#define BLE_EMC_MASTER_RECEIVE_SUCCESS			0
#define BLE_EMC_MASTER_RECEIVE_WRONG_DATA		1
#define BLE_EMC_MASTER_NO_RECEIVE				2

#endif /* SIGNALS_BLE_BLE_DV_CFG_H_ */
