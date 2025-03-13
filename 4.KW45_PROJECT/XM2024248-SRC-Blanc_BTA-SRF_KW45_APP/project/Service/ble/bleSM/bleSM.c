/*
 * bleSM.c
 *
 *  Created on: 2024年7月4日
 *      Author: F704012276_
 */

#include "bleSM_cfg.h"
#include "signals_api.h"
#include "drv_api.h"

/**/
#define T_Start_Pair_timeout				10 * 1000
//#define T_Start_Pair_Complete_timeout		30 * 1000 * 1000
//#define T_Pending_for_Adv_timeout			2  * 1000 * 1000
//#define T_SP_Auth_Tmot_timeout				10 * 1000 * 1000
//#define T_SKF_Auth_Tmot_timeout				5  * 1000 * 1000
//#define T_Pending_for_Adv_Scan_timeout		200* 1000
//#define T_Auth_Valid_timeout				10 * 1000 * 1000
//#define T_BLEImmo_timeout					2  * 60   * 1000 * 1000
//#define T_PhoneKey_SupervisionTO_timeout	3  * 1000 * 1000
//#define T_KeyFob_SupervisionTO_timeout		4  * 1000 * 1000
//
//
//
//
TIMER_MANAGER_HANDLE_DEFINE(T_Start_Pair0);						/* timer for pair start timeout after conn*/
//TIMER_MANAGER_HANDLE_DEFINE(T_Start_Pair_Complete);				/* timer for pair complete after conn*/
//TIMER_MANAGER_HANDLE_DEFINE(T_Pending_for_Adv);					/* time for BTA1 to restart advertising after the Bluetooth connection is broken*/
//TIMER_MANAGER_HANDLE_DEFINE(T_SP_Auth_Tmot);					/*Time for BTA1 wait for phone key authentication result*/
//TIMER_MANAGER_HANDLE_DEFINE(T_SKF_Auth_Tmot);					/*Time for BTA1 wait for keyfob authentication result*/
//TIMER_MANAGER_HANDLE_DEFINE(T_Pending_for_Adv_Scan);			/*Time for BTA1 to restart scanning/advertising if authentication failed.*/
//TIMER_MANAGER_HANDLE_DEFINE(T_Auth_Valid);						/*Time for BTA1/VDF clear caching data after disconnection.*/
//TIMER_MANAGER_HANDLE_DEFINE(T_BLEImmo);							/*time for BTA1 to overwrite valid key status*/
//TIMER_MANAGER_HANDLE_DEFINE(T_PhoneKey_SupervisionTO);			/*Bluetooth disconnect if no communication between master and slave device over a certain period of time*/
//TIMER_MANAGER_HANDLE_DEFINE(T_KeyFob_SupervisionTO);			/*supervision timeout for ble connection of keyfob and BTA1*/


static uint8_t bleState[gAppMaxConnections_c] = {
		pAppIdle_c,
		pAppIdle_c,
		pAppIdle_c,
		pAppIdle_c,
		pAppIdle_c
};
static uint8_t bleOldStage[gAppMaxConnections_c];

static void T_Start_Pair_callback(uint8_t id);
static void T_Start_Pair_Complete_callback(uint8_t id);
static void T_Pending_for_Adv_callback(uint8_t id);
static void T_SP_Auth_Tmot_callback(void* param);
static void T_SKF_Auth_Tmot_callback(uint8_t id);
static void T_Pending_for_Adv_Scan_callback(uint8_t id);
static void T_Auth_Valid_callback(uint8_t id);
static void T_BLEImmo_callback(uint8_t id);
static void T_PhoneKey_SupervisionTO_callback(uint8_t id);
static void T_KeyFob_SupervisionTO_callback(uint8_t id);

static void BleSM_MainInit(void);
static void BleSM_MainNormal(void);
static void BleSM_MainSleep(void);

void BleSM_Init(void)
{
//	BleSM_MainInit();
//
//	/*Initialized Timer*/
//	TM_Open(T_Start_Pair0);
//	(void)TM_InstallCallback((timer_handle_t)T_Start_Pair0, T_SP_Auth_Tmot_callback, NULL);
//	(void)TM_Start((timer_handle_t)T_Start_Pair0, (uint8_t)kTimerModeIntervalTimer | (uint8_t)kTimerModeSetMicrosTimer, T_Start_Pair_timeout);

//	TM_Open(T_Start_Pair_Complete);
//	TM_Open(T_Pending_for_Adv);
//	TM_Open(T_SP_Auth_Tmot);
//	TM_Open(T_SKF_Auth_Tmot);
//	TM_Open(T_Pending_for_Adv_Scan);
//	TM_Open(T_Auth_Valid);
//	TM_Open(T_BLEImmo);
//	TM_Open(T_PhoneKey_SupervisionTO);
//	TM_Open(T_KeyFob_SupervisionTO);
}

void BleSM_MainFunction(void)
{

//	for(uint8_t i = 0; i < gAppMaxConnections_c; i++)
//	{
//		BleSM_DeviceMainFunction(i);
//	}
}

void BleSM_DeviceMainFunction(uint8_t index)
{
//	static uint8_t whiteListAddFlag = 0;
//	static uint8_t advScanStartFlag = 0;
//
//	switch(maPeerInformation[index].appStage)
//	{
//	case pAppUnInitialized_c:
//		break;
//	case pAppIdle_c:
//		if(whiteListAddFlag != 1)
//		{
//			Gap_AddDeviceToFilterAcceptList(gBleAddrTypePublic_c, keyFobData.device_address);
//			whiteListAddFlag = 1;
//		}
//		maPeerInformation[index].appStage = pAppPreWork_c;
//		break;
//	case pAppPreWork_c:
//		if(advScanStartFlag != 1)
//		{
//			BleSM_PreWork();
//			advScanStartFlag = 1;
//		}
//		break;
//	case pAppAdvScan_c:			/*Start scan*/
//		break;
//	case pAppConnecting_c:		/*Start connecting */
//		break;
//	case pAppConnected_c:		/* Device Connected */
//		maPeerInformation[index].appStage = pAppInstructionComplete_c;
//		break;
//	case pAppDisconnected_c:	/* Device Disconnected */
//		break;
//	case pAppPairing_c:			/* BTA Start Pairing */
//		break;
//	case pAppPaired_c:
//		(void)TM_Stop((timer_handle_t)T_Start_Pair);
//		(void)TM_Stop((timer_handle_t)T_Start_Pair_Complete);
//
//		if(maPeerInformation[index].gapRole == gGapCentral_c)	/*Connected device is Keyfob*/
//		{
//			(void)TM_InstallCallback((timer_handle_t)T_SKF_Auth_Tmot, T_SKF_Auth_Tmot_callback, &(maPeerInformation[index].deviceId));
//			(void)TM_Start((timer_handle_t)T_SKF_Auth_Tmot, (uint8_t)kTimerModeSingleShot | (uint8_t)kTimerModeSetMicrosTimer, T_SKF_Auth_Tmot_timeout);
//		}
//		else if(maPeerInformation[index].gapRole == gGapPeripheral_c)	/*Connected device is Phone*/
//		{
//			(void)TM_InstallCallback((timer_handle_t)T_SP_Auth_Tmot, T_SP_Auth_Tmot_callback, &(maPeerInformation[index].deviceId));
//			(void)TM_Start((timer_handle_t)T_SP_Auth_Tmot, (uint8_t)kTimerModeSingleShot | (uint8_t)kTimerModeSetMicrosTimer, T_SP_Auth_Tmot_timeout);
//		}
//		else
//		{
//			/*Do Nothing */
//		}
//		maPeerInformation[index].appStage = pAppInstructionComplete_c;
//
//		break;
//	case pAppPairFailure_c:
//
//		break;
//	case pAppAuthening_c:
//		break;
//	case pAppAuthened_c:
//		BleSM_MainSleep();
//		break;
//	case pAppAuthenFailure_c:
//		break;
//	case pAppInstructionComplete_c:
//		break;
//
//	default:
//		break;
//	}
}

static void BleSM_PreWork(void)
{
//	/* Update Adv Parameters */
//#if (defined(gAppNIOAdvEnable_c) && (gAppNIOAdvEnable_c == 1))
//	gAppAdvParams.pGapAdvData = &gAppAdvertisingDataNio;
//	gAppAdvParams.pScanResponseData = &gAppScanRspDataNio;
//
//	gLegacyAdvParams.minInterval = 0x147;
//	gLegacyAdvParams.maxInterval = 0x147;
//#endif
//
//	/* Update Scan Parameters */
//#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
//	appScanParams.pHostScanParams->filterPolicy = gScanWithFilterAcceptList_c;
//#endif
//
//	/* Start Adv/Scan*/
//	BleAppIf_Start();
}
static void BleSM_MainInit(void)
{
//	uint8_t tempData[20] = {0};
//	uint8_t retData[32] = {0};
//	uint8_t adv_random[] = {0x88, 0xbe, 0x12, 0x2c};
//
//	gCurrentAdvHandle = (uint8_t)gLegacyAdvSetHandle_c;
//    Set_bit_bleScanState(FALSE);
//    Set_bit_bleAdvState(FALSE);
//	Set_bit_bleConfigDoneFlag(0);
//	Set_validConnectedKeyNum(0);
//	Set_validConnectedKeyFobNum(0);
//
//	/* 1.CONCATENATE vehicle id and the last random num */
//	FLib_MemCpy(tempData, keyFobData.vehicle_id, 16);
//	FLib_MemCpy(&tempData[16], adv_random, 4);
//
//	/* 2.Calculate the concatenate string, get the hash value */
//	SHA256_Hash(tempData, 20, retData);
//
//	/*3.Put hash value into adv parameters*/
//	FLib_MemCpy(gAppAdvertisingDataNio.aAdStructures[1].aData, &(retData[20]), 12);
//	FLib_MemCpy(&(gAppAdvertisingDataNio.aAdStructures[1].aData[12]), adv_random, 4);


}

static void BleSM_MainNormal(void)
{
//	static uint8_t bleIdleCnt = 0;
//	static uint8_t bleIdleFlag = 0;
//	static uint16_t bleStateCnt = 0;
//	static uint8_t bleState = enum_bleSts_Normal;
//
//	for(uint8_t i = 0; i < gAppMaxConnections_c; i++)
//	{
//		if((maPeerInformation[i].appState == mAppIdle_c) || \
//				(maPeerInformation[i].appState == mAppRunning_c) ||\
//				(maPeerInformation[i].appState == mAppCCCWaitingForOwnerPairingRequest_c))
//		{
//			bleIdleCnt++;
//			if(bleIdleCnt >= gAppMaxConnections_c)
//			{
//				bleIdleFlag = 1;
//			}
//		}
//	}
//
//	switch(bleState)
//	{
//	case enum_bleSts_Normal:
//	case enum_bleSts_WaitWork:
//		bleStateCnt++;
//		if(bleStateCnt > 10)
//		{
//			bleStateCnt = 0;
//			bleState = enum_bleSts_Work;
//		}
//		break;
//	case enum_bleSts_Work:
//		if(bleIdleFlag == 1)
//		{
//			if((Get_validConnectedKeyNum() < gAppMaxConnections_c) \
//				&& (Get_bit_bleConfigDoneFlag() == 1))	/* No connection + valid key < 5 + key fob < 3*/
//			{
//				BleAppIf_StartAdv();
//				if(Get_validConnectedKeyFobNum() < 3)
//				{
//					BleAppIf_StartScan();
//				}
//			}
//			bleState = enum_bleSts_WaitWork;
//		}
//		break;
//	default:
//		break;
//	}
//
//	bleIdleCnt = 0;
//	bleIdleFlag = 0;

}

static void BleSM_MainSleep(void)
{

}

static void T_Start_Pair_callback(uint8_t id)
{
//	if(maPeerInformation[id].appStage == pAppConnected_c)
//	{
//		Gap_Disconnect(id);
//	}
}

static void T_Start_Pair_Complete_callback(uint8_t id)
{
//	if(maPeerInformation[id].appStage == pAppPairing_c)
//	{
//		Gap_Disconnect(id);
//	}
}

static void T_Pending_for_Adv_callback(uint8_t id)
{

}

static void T_SP_Auth_Tmot_callback(void* param)
{
	static uint16_t cnt = 0;
	cnt++;
//	/* Authening failure within T_SP_Auth_Tmot*/
//	if(maPeerInformation[id].appStage == pAppAuthening_c)
//	{
//		Gap_Disconnect(id);
//	}
}

static void T_SKF_Auth_Tmot_callback(uint8_t id)
{
//	/* Authening failure within T_SKF_Auth_Tmot*/
//	if(maPeerInformation[id].appStage == pAppAuthening_c)
//	{
//		Gap_Disconnect(id);
//	}
}

static void T_Pending_for_Adv_Scan_callback(uint8_t id)
{

}

static void T_Auth_Valid_callback(uint8_t id)
{

}

static void T_BLEImmo_callback(uint8_t id)
{

}

static void T_PhoneKey_SupervisionTO_callback(uint8_t id)
{

}

static void T_KeyFob_SupervisionTO_callback(uint8_t id)
{

}
