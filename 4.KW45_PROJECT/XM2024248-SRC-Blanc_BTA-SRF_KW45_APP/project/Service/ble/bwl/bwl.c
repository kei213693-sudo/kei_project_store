/*
 * bwl.c
 *
 *  Created on: 2024年9月14日
 *      Author: F704012276_
 */

#include "ble_cfgSignals.h"
#include "bwl_api.h"

static whitelistofBtm_t whitelistStruct;
static btmWhitelist_t whitelistbuff;
static bleCfg_timestamp_t WLtimeStamp;
static uint8_t Bwl_FindAvailableSlot(void);
static uint8_t Bwl_RemoveOldestWL(void);

static uint8_t Bwl_FindSlotbyMacAddress(uint8_t *addr)
{
	for(uint8_t i = 0; i < BLE_WRITELIST_NUM; i++)
	{
		if( memcmp(addr, whitelistStruct.data.whitelistGroup.whitelist[i].BD_ADDR, 6) )
		{
			return i;
		}
	}

	for(uint8_t j = 0; j < BLE_WRITELIST_BUFF_NUM; j++)
	{
		if( memcmp(addr, whitelistStruct.data.whitelistGroup.whitelistbuff[j].BD_ADDR, 6) )
		{
			return j+BLE_WRITELIST_NUM;
		}
	}
	return R_BLE_SLOT_INVALID;
}

static uint8_t Bwl_FindAvailableSlot(void)
{
	for(uint8_t i = 0; i < BLE_WRITELIST_TOTAL_NUM; i++)
	{
		if(whitelistStruct.data.whitelistGroup.bits.IdleFlags & (uint32_t)(1<<i))
		{
			return i;
			break;
		}
	}
	return R_BLE_SLOT_INVALID;
}

static uint8_t Bwl_RemoveOldestWL(void)
{
	uint8_t slotNum = R_BLE_SLOT_INVALID;

	for(uint8_t i = 0; i < BLE_WRITELIST_NUM; i++)
	{
		if((WLtimeStamp.Timestamp_year < whitelistStruct.data.whitelistGroup.whitelist[i].timestamp.Timestamp_year) &&\
				(WLtimeStamp.Timestamp_Month < whitelistStruct.data.whitelistGroup.whitelist[i].timestamp.Timestamp_Month) &&\
				(WLtimeStamp.Timestamp_Day < whitelistStruct.data.whitelistGroup.whitelist[i].timestamp.Timestamp_Day) &&\
				(WLtimeStamp.Timestamp_Hour < whitelistStruct.data.whitelistGroup.whitelist[i].timestamp.Timestamp_Hour) &&\
				(WLtimeStamp.Timestamp_Minute < whitelistStruct.data.whitelistGroup.whitelist[i].timestamp.Timestamp_Minute) &&\
				(WLtimeStamp.Timestamp_Second < whitelistStruct.data.whitelistGroup.whitelist[i].timestamp.Timestamp_Second))
		{
			slotNum = i;
			memcpy(&WLtimeStamp, &whitelistStruct.data.whitelistGroup.whitelist[i].timestamp, sizeof(bleCfg_timestamp_t));
		}

	}

	for(uint8_t j = 0; j < BLE_WRITELIST_BUFF_NUM; j++)
	{
		if((WLtimeStamp.Timestamp_year < whitelistStruct.data.whitelistGroup.whitelistbuff[j].timestamp.Timestamp_year) &&\
					(WLtimeStamp.Timestamp_Month < whitelistStruct.data.whitelistGroup.whitelistbuff[j].timestamp.Timestamp_Month) &&\
					(WLtimeStamp.Timestamp_Day < whitelistStruct.data.whitelistGroup.whitelistbuff[j].timestamp.Timestamp_Day) &&\
					(WLtimeStamp.Timestamp_Hour < whitelistStruct.data.whitelistGroup.whitelistbuff[j].timestamp.Timestamp_Hour) &&\
					(WLtimeStamp.Timestamp_Minute < whitelistStruct.data.whitelistGroup.whitelistbuff[j].timestamp.Timestamp_Minute) &&\
					(WLtimeStamp.Timestamp_Second < whitelistStruct.data.whitelistGroup.whitelistbuff[j].timestamp.Timestamp_Second))
		{
			slotNum = j + BLE_WRITELIST_NUM;
			memcpy(&WLtimeStamp, &whitelistStruct.data.whitelistGroup.whitelistbuff[j].timestamp, sizeof(bleCfg_timestamp_t));
		}
	}

	return slotNum;
}

static uint8_t Bwl_DeleteWhitelistByAddress(uint8_t* device_addr)
{
	uint8_t slot;
	slot = Bwl_FindSlotbyMacAddress(device_addr);
	if(slot <= BLE_WRITELIST_NUM)
	{
		memset((uint8_t*)&whitelistStruct.data.whitelistGroup.whitelist[slot], 0, sizeof(btmWhitelist_t));
	}
	else if(slot <= BLE_WRITELIST_TOTAL_NUM)
	{
		memset((uint8_t*)&whitelistStruct.data.whitelistGroup.whitelistbuff[slot], 0, sizeof(btmWhitelist_t));
	}
	else{}

	return R_BLE_OK;
}

static uint8_t Bwl_DeleteKeyFobLearningInfoByAddress(uint8_t* device_addr)
{
	for(uint8_t i = 0; i < 6; i++)
	{
		if(memcmp(device_addr, &whitelistStruct.keyFobLearningData[i].deviceAddr, 6))
		{
			memset((uint8_t*)&whitelistStruct.keyFobLearningData[i], 0, sizeof(KeyFobLearningData_t));
			return R_BLE_OK;
		}
	}
	return R_BLE_BUSY;
}

static uint8_t Bwl_MoveLearningInfo2Whitelist(uint8_t* device_addr)
{
	if(Bwl_FindSlotbyMacAddress(device_addr) != R_BLE_SLOT_INVALID)
	{
		return R_BLE_OK;
	}

	for(uint8_t i = 0; i < 6; i++)
	{
		if(memcmp(device_addr, whitelistStruct.keyFobLearningData[i].deviceAddr, 6))
		{
			memcpy(whitelistbuff.BD_ADDR, whitelistStruct.keyFobLearningData[i].deviceAddr, 6+16);

			Bwl_AddWL((btmWhitelist_t*)&whitelistbuff);
			break;
			return R_BLE_OK;
		}
	}

	return R_BLE_BUSY;
}

void Bwl_Init(void)
{
	/*load whitelist from NVM*/
	Ble_Api_fdl_FlashRead(BLEAPI_FLASH_INDEX_WHITELIST, (uint8_t *)&whitelistStruct, sizeof(whitelistofBtm_t));
	memset(&WLtimeStamp, 0xff, sizeof(bleCfg_timestamp_t));
}

void Bwl_AddWL(btmWhitelist_t *wl)
{
	uint8_t slotNum = Bwl_FindAvailableSlot();
	if(slotNum == R_BLE_SLOT_INVALID)
	{
		slotNum = Bwl_RemoveOldestWL();
	}
	else
	{
		/*TODO */
	}

	if(slotNum < 20)
	{
		memcpy(&whitelistStruct.data.whitelistGroup.whitelist[slotNum], wl, sizeof(btmWhitelist_t));
	}
	else
	{
		memcpy(&whitelistStruct.data.whitelistGroup.whitelistbuff[slotNum], wl, sizeof(btmWhitelist_t));
	}

	whitelistStruct.data.whitelistGroup.bits.IdleFlags ^= (1 << slotNum);
	Ble_Api_fdl_Ereas_Write(BLEAPI_FLASH_INDEX_WHITELIST, (uint8_t *)&whitelistStruct, sizeof(whitelistofBtm_t));
}

void Bwl_UpdateTimeStamp(uint8_t *addr, bleCfg_timestamp_t* timestamp)
{
	uint8_t slotIndex;
	slotIndex = Bwl_FindSlotbyMacAddress(addr);
	memcpy(&whitelistStruct.data.whitelistGroup.whitelist[slotIndex].timestamp, timestamp, sizeof(bleCfg_timestamp_t));
}

uint8_t Bwl_DeviceAvailable(btmWhitelist_t *wl)
{
	uint8_t deviceIsLegacy = 0;
	return deviceIsLegacy;
}

void Bwl_GetNumofRemainWL(uint8_t *count)
{

}

void Bwl_GetConnectedMAC_IRKS(void)
{

}

void Bwl_GetKeyFobInfo(uint8_t* keyFobArray, uint8_t* num)
{
	uint8_t offset = 2;
	for(uint8_t i = 0; i < BLE_WRITELIST_NUM; i++)
	{
		if(whitelistStruct.data.whitelistGroup.whitelist[i].bleType == BTMWhitelistKeyfobType)
		{
			offset = offset + 22*i;
			memcpy(&keyFobArray[offset], whitelistStruct.data.whitelistGroup.whitelist[i].BD_ADDR, 23);
			*num++;
		}
	}

	for(uint8_t j = BLE_WRITELIST_NUM; j < BLE_WRITELIST_TOTAL_NUM; j++)
	{
		if(whitelistStruct.data.whitelistGroup.whitelist[j].bleType == BTMWhitelistKeyfobType)
		{
			offset = offset + 22*j;
			memcpy(&keyFobArray[offset], whitelistStruct.data.whitelistGroup.whitelistbuff[j].BD_ADDR, 23);
			*num++;
		}
	}
	keyFobArray[0] = 0;
	keyFobArray[1] = *num;
}

void Bwl_GetPhoneInfo(uint8_t* phoneArray, uint8_t* num)
{
	uint8_t offset = 2;
	for(uint8_t i = 0; i < BLE_WRITELIST_NUM; i++)
	{
		if(whitelistStruct.data.whitelistGroup.whitelist[i].bleType == BTMWhitelistPhoneType)
		{
			offset = offset + 6*i;
			memcpy(&phoneArray[offset], whitelistStruct.data.whitelistGroup.whitelist[i].BD_ADDR, 6);
			*num++;
		}
	}

	for(uint8_t j = BLE_WRITELIST_NUM; j < BLE_WRITELIST_TOTAL_NUM; j++)
	{
		if(whitelistStruct.data.whitelistGroup.whitelist[j].bleType == BTMWhitelistPhoneType)
		{
			offset = offset + 6*j;
			memcpy(&phoneArray[offset], whitelistStruct.data.whitelistGroup.whitelistbuff[j].BD_ADDR, 6);
			*num++;
		}
	}
	phoneArray[0] = 0;
	phoneArray[1] = *num;
}

void Bwl_WriteKeyFobLearningInfo(uint8_t* data)
{
	for(uint8_t i = 0; i < 6; i++)
	{
		if(whitelistStruct.keyFobLearningData[i].idleFlag == KeyFobLearningDataBlockIdle)
		{
			whitelistStruct.keyFobLearningData[i].idleFlag = KeyFobLearningDataBlockOccupied;
			memcpy(whitelistStruct.keyFobLearningData, data, sizeof(KeyFobLearningData_t)-1);
			break;
		}
	}
}


/* MType 0x84 MID 0x03(DeviceManageRequest) */
uint8_t Bwl_CheckAndRemovePhoneAndKeyfob(uint8_t* device_addr)
{
	uint8_t slot;
	slot = Bwl_FindSlotbyMacAddress(device_addr);
	if(slot <= BLE_WRITELIST_NUM)
	{
		memset((uint8_t*)&whitelistStruct.data.whitelistGroup.whitelist[slot], 0, sizeof(btmWhitelist_t));
	}
	else if(slot <= BLE_WRITELIST_TOTAL_NUM)
	{
		memset((uint8_t*)&whitelistStruct.data.whitelistGroup.whitelistbuff[slot], 0, sizeof(btmWhitelist_t));
	}
	else{}
}

/* MType 0x84 MID 0x03(DeviceManageRequest) */
uint8_t Bwl_CheckAndRevokeKeyfob(uint8_t* device_addr)
{
	return Bwl_DeleteWhitelistByAddress(device_addr);
}

/* MType 0x84 MID 0x03(DeviceManageRequest) */
uint8_t Bwl_CheckAndRecoverkeyFob(uint8_t* device_addr)
{
	return Bwl_MoveLearningInfo2Whitelist(device_addr);
}
