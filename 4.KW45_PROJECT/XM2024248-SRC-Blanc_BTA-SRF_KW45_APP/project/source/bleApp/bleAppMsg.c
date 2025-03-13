/*
 * bleAppData.c
 *
 *  Created on: 2024年8月22日
 *      Author: F704012276_
 */

#include "app.h"
#include "signals_api.h"
#include "bleAppMsg.h"
#include "bleMessage_cfg.h"

uint8_t bleMsgRx_0x01_0x03_IndAppFunc(uint8_t deviceId, void* pBleMsg)
{
	/* Key Fob Authentication */
	blePayloads_t* bufPtr = pBleMsg;
	if(bufPtr->bytes.msg.data[0] == KEYFOB_AUTHEN_FAILED)
	{
		/*TODO 开启定时器*/
		Ble_GapDisconnect(deviceId);
		Start_BleTimer(deviceId, T_Pending_for_Adv_Scan);
	}
}

uint8_t bleMsgRx_0x04_0x0B_IndAppFunc(uint8_t deviceId, void* pBleMsg)
{
	/* Phone Auth Failure*/ 	/*TODO 开启定时器*/
	blePayloads_t* bufPtr = pBleMsg;
	Ble_GapDisconnect(deviceId);
	Start_BleTimer(deviceId, T_Pending_for_Adv_Scan);
}

uint8_t bleMsgRx_0x40_0x01_IndAppFunc(uint8_t deviceId, void* pBleMsg)
{
	/* TODO Smart Key Postion*/
}

uint8_t bleMsgRx_0x84_0x01_IndAppFunc(uint8_t deviceId, void* pBleMsg)
{
	/* Ble Msg Disconnect request*/
	Gap_Disconnect(0);
}

uint8_t bleMsgRx_0x84_0x03_IndAppFunc(uint8_t deviceId, void* pBleMsg)
{
	/*TODO DeviceManageRequest Device Remove/Revoke/Recover*/
	blePayloads_t* bufPtr = pBleMsg;
	if(bufPtr->bytes.msg.data[0] == 0x01)	/*Remove*/
	{
		if((bufPtr->bytes.msg.data[1] == 0x01) || \
				(bufPtr->bytes.msg.data[1] == 0x02))	/*Key FOb*/
		{
			Bwl_CheckAndRemovePhoneAndKeyfob(&bufPtr->bytes.data[2]);
		}
	}
	else if(bufPtr->bytes.msg.data[0] == 0x02)	/*Revoke*/
	{
		if(bufPtr->bytes.msg.data[1] == 0x01)
		{
			Bwl_CheckAndRevokeKeyfob(&bufPtr->bytes.data[2]);
		}
	}
	else if(bufPtr->bytes.msg.data[0] == 0x03)	/*Recover*/
	{
		if(bufPtr->bytes.msg.data[1] == 0x01)
		{
			Bwl_CheckAndRecoverkeyFob(&bufPtr->bytes.data[2]);
		}
	}

}

uint8_t bleMsgRx_0x84_0x05_IndAppFunc(uint8_t deviceId, void* pBleMsg)
{
	/*KeyFobLearningDataWrite*/
	blePayloads_t* bufPtr = pBleMsg;
	Bwl_WriteKeyFobLearningInfo(bufPtr->bytes.msg.data);
	/*KeyFobLearningDataWriteRsp*/
	bleMsgCanBuff.pPacket[0] = 0x84;
	bleMsgCanBuff.pPacket[1] = 0x06;
	bleMsgCanBuff.pPacket[2] = 1;
	bleMsgCanBuff.pPacket[3] = 0;
	bleMsgCanBuff.pPacket[4] = 1;
	Tp_Transmit(0x4E4, &bleMsgCanBuff);
}

uint8_t bleMsgRx_0x84_0x07_IndAppFunc(uint8_t deviceId, void* pBleMsg)
{
	/*Valid Phone Key Synchronization*/
	/*PhoneKeyDeviceListGet*/
	/*PhoneKeyDeviceListGetRsp*/
	uint8_t count;
	Bwl_GetPhoneInfo(&bleMsgCanBuff.pPacket[4], &count);
	bleMsgCanBuff.pPacket[0] = 0x84;
	bleMsgCanBuff.pPacket[1] = 0x08;
	bleMsgCanBuff.pPacket[2] = (uint8_t)(1 + 1 + count*6);
	bleMsgCanBuff.pPacket[3] = (uint8_t)((1 + 1 + count*6) >> 8);
	bleMsgCanBuff.packetLength = 4+2+count*6;	/*MType + MID + LEN + result + devicecount + devicelist*/

	Tp_Transmit(0x4E4, &bleMsgCanBuff);
}

uint8_t bleMsgRx_0x84_0x09_IndAppFunc(uint8_t deviceId, void* pBleMsg)
{
	/*KeyFobDeviceListGet*/
	/*KeyFobDeviceListGetRsp*/
	uint8_t count;
	Bwl_GetKeyFobInfo(&bleMsgCanBuff.pPacket[4], &count);
	bleMsgCanBuff.pPacket[0] = 0x84;
	bleMsgCanBuff.pPacket[1] = 0x0A;
	bleMsgCanBuff.pPacket[2] = (uint8_t)(1 + 1 + count*23);
	bleMsgCanBuff.pPacket[3] = (uint8_t)((1 + 1 + count*23) >> 8);
	bleMsgCanBuff.packetLength = 4+2+count*6;	/*MType + MID + LEN + result + devicecount + devicelist*/

	Tp_Transmit(0x4E4, &bleMsgCanBuff);
}

void CanTp_MsgRevHandle(uint8_t deviceId)
{
	static canPayloads_t* bufPtr;
	bufPtr = (canPayloads_t *)bleMsgCanBuff.pPacket;
	memcpy(bleMsgAirBuff.pPacket, bleMsgCanBuff.pPacket, bleMsgCanBuff.packetLength);
	bleMsgAirBuff.packetLength = bleMsgCanBuff.packetLength;

	for(uint8_t i = 0; i < enum_bleMsg_total_num; i++)
	{
		if((bufPtr->MType == bleMsgRxHandle[i][1]) &&\
				(bufPtr->MID == bleMsgRxHandle[i][2]))
		{
			if(bleRx_IndAppFuncPtr != NULL)
			{
				bleRx_IndAppFuncPtr[i](deviceId, bleMsgAirBuff.pPacket);
				return;
			}
		}
	}

	/*Router data to BLE*/
	{
		Tp_Transmit(INVALID_CAN_ID, &bleMsgAirBuff);
	}
}
void CanTp_MsgSendHandle(void)
{

}
void BleTp_MsgRevHandle(uint8_t deviceId)
{
	static blePayloads_t* bufPtr;
	bufPtr = (blePayloads_t *)bleMsgAirBuff.pPacket;
	memcpy(bleMsgCanBuff.pPacket, bleMsgAirBuff.pPacket, bleMsgAirBuff.packetLength);
	bleMsgCanBuff.packetLength = bleMsgAirBuff.packetLength;

	for(uint8_t i = 0; i < enum_bleMsg_total_num; i++)
	{
		if((bufPtr->bytes.msg.MType == bleMsgRxHandle[i][1]) &&\
				(bufPtr->bytes.msg.MID == bleMsgRxHandle[i][2]))
		{
			/*handle*/
			if(bleRx_IndAppFuncPtr != NULL)
			{
				bleRx_IndAppFuncPtr[i](deviceId, bleMsgCanBuff.pPacket);
				return;
			}
		}
	}

	/*Router data to CAN*/
	{
		bleMsgCanBuff.deviceId = deviceId;
		Tp_Transmit(0x4E4, &bleMsgCanBuff);
	}
}
void BleTp_MsgSendHandle(void)
{

}

