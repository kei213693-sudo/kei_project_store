/*
 * bleAppUser.c
 *
 *  Created on: 2024年7月11日
 *      Author: F704012276_
 */

#include "signals_api.h"
#include "autosar_nm.h"
#include "UWB_CAN.h"
#include "app_api.h"
#include "drv_api.h"
#include "srv_api.h"

static void Ble_UpdataIRK(uint8_t deviceId);

static void T_Start_Pair_Timeout_callback(void *param);
static void T_Start_Pair_Complete_Timeout_callback(void *param);
static void T_Pending_for_Adv_Timeout_callback(void *param);
static void T_SP_Auth_Tmot_Timeout_callback(void *param);
static void T_SKF_Auth_Tmot_Timeout_callback(void *param);
static void T_Pending_for_Adv_Scan_Timeout_callback(void *param);
static void T_Auth_Valid_Timeout_callback(void *param);
static void T_BLEImmo_Timeout_callback(void *param);
static void T_PhoneKey_SupervisionTO_Timeout_callback(void *param);
static void T_KeyFob_SupervisionTO_Timeout_callback(void *param);
static void T_AuthFail_RestartAdv_Timeout_callback(void *param);
static void T_NioAdv_App_Timeout_callback(void *param);
static void T_NioAdv_Ibeacon_Timeout_callback(void *param);
static void T_BleCanTp_Timeout_callback(void *param);

void AppEvt_BleConfigDoneHandle(void)
{
	Ble_Init();
	BleAppIf_Start();
}

void App_PeerConnectedHandle(appEventData_t* event)
{
	uint8_t deviceId;

	deviceId = event->eventData.peerDeviceId;
//	ble_SetSleepFlag(0);
//	Start_BleTimer(event->eventData.peerDeviceId, T_Start_Pair);
//	Start_BleTimer(event->eventData.peerDeviceId, T_Start_Pair_Complete);
	Set_BLE_DV_ConnectingSts(BLE_DV_IS_CONNECTING);
	maPeerInformation[deviceId].appStage = pAppConnected_c;
	gCurrentAdvHandle = (uint8_t)gNoAdvSetHandle_c;
	(void)Gap_StopExtAdvertising(0xFF);
	canMsg1ccRx.newMsgRecv_flg = 1;
	canMsg1ccRx.msg.sig.scanCMD = 1;

	//BleAppIf_StartNonConAdv();//建立连接后启动不可连接广播
	shell_write("\r\nConnected!\r\n");
	/* trigger event */
}

void App_PeerDisconnectedHandle(appEventData_t* event)
{
	Nm_StateType nmStatePtr;
	Nm_ModeType nmModePtr;
	CanNm_GetState(0, &nmStatePtr, &nmModePtr);

	uint8_t deviceId;

	deviceId = event->eventData.peerDeviceId;
	/**/
	bleCCCDFlag = 0;
	bleMasterCCCDFlag = 0;
	bleDisconnectFlag = 1;

	Set_bit_bleAdvState(FALSE);
	Set_bit_bleScanState(FALSE);
	Reset_BleTimer(deviceId);
	gCurrentAdvHandle = (uint8_t)gNoAdvSetHandle_c;
	//(void)Gap_StopExtAdvertising(0xFF);
	Set_BLE_DV_ConnectingSts(BLE_DV_NO_CONNECTION);
	maPeerInformation[deviceId].appStage = pAppDisconnected_c;

	//canMsg1ccRx.newMsgRecv_flg = 1;
	//canMsg1ccRx.msg.sig.scanCMD = 2;
	shell_write("Disconnected!\r\n");
	BleAppIf_StartConAdv();//重新启动可连接广播

    if(nmModePtr == NM_NETWORK)
    {
    	Start_BleTimer(event->eventData.peerDeviceId, T_Pending_for_Adv);
    }
}

void App_PairingCompleteHandle(appEventData_t* event)
{
	uint8_t deviceId;
	deviceId = event->eventData.peerDeviceId;
/*	if(maPeerInformation[deviceId].appStage == pAppPairing_c)
	{
		maPeerInformation[deviceId].appStage = pAppPaired_c;
		Clear_BleTimer(deviceId, T_Start_Pair_Complete);

		if(maPeerInformation[deviceId].gapRole == gGapPeripheral_c)
		{
			Start_BleTimer(deviceId, T_SP_Auth_Tmot);
		}
		else if(maPeerInformation[deviceId].gapRole == gGapCentral_c)
		{
			Start_BleTimer(deviceId, T_SKF_Auth_Tmot);
		}
		else
		{
			 Do Nothing
		}
	}*/
	gCurrentAdvHandle = (uint8_t)gNoAdvSetHandle_c;
	(void)Gap_StopExtAdvertising(0xFF);
	//BleAppIf_StartConAdv();
	Ble_UpdataIRK(deviceId);
    shell_write("\r\nPairing successful.\r\n");
}

void App_PairingReqRcvHandle(appEventData_t* event)
{
	/* Clean pairing timer */
	Clear_BleTimer(event->eventData.peerDeviceId, T_Start_Pair);
	maPeerInformation[event->eventData.peerDeviceId].appStage = pAppPairing_c;
    shell_write("\r\nPairing...\r\n");
}

void App_BleConnectingHandle(appEventData_t* event)
{
	maPeerInformation[event->eventData.peerDeviceId].appStage = pAppConnecting_c;
    shell_write("Connecting...\r\n");
}

void App_BleAuthingHandle(appEventData_t* event)
{
	maPeerInformation[event->eventData.peerDeviceId].appStage = pAppAuthening_c;
	shell_write("Authening_c...\r\n");
}

void App_BleAuthPassHandle(appEventData_t* event)
{
	if(maPeerInformation[event->eventData.peerDeviceId].appStage = pAppAuthening_c)
	{
		maPeerInformation[event->eventData.peerDeviceId].appStage = pAppAuthenedPassed_c;
	}
	shell_write("Authentic Passed...\r\n");
}

void App_BleAuthPreFailureHandle(appEventData_t* event)
{
	if(maPeerInformation[event->eventData.peerDeviceId].appStage = pAppAuthening_c)
	{
		maPeerInformation[event->eventData.peerDeviceId].appStage = pAppAuthenedPreFailure_c;
		maPeerInformation[event->eventData.peerDeviceId].retryCnt--;
	}

	/* fail three times in row in T_SP_Auth_Tmot_timeout/T_SKF_Auth_Tmot_timeout */
	if(maPeerInformation[event->eventData.peerDeviceId].retryCnt != 0)
	{
		/* TODO Restart Authentication*/
		maPeerInformation[event->eventData.peerDeviceId].appStage = pAppAuthening_c;
	}
	else
	{
		/*restart */
		maPeerInformation[event->eventData.peerDeviceId].appStage = pAppAuthenFailure_c;
		maPeerInformation[event->eventData.peerDeviceId].retryCnt = 3;
		App_BleAuthFailHandle(event);
	}
}

void App_BleAuthFailHandle(appEventData_t* event)
{
	if(maPeerInformation[event->eventData.peerDeviceId].appStage = pAppAuthenFailure_c)
	{
		maPeerInformation[event->eventData.peerDeviceId].retryCnt = 3;

		/*TODO 主动断开连接，并且需要5S之后才能广播*/
		Gap_Disconnect(event->eventData.peerDeviceId);
		Start_BleTimer(event->eventData.peerDeviceId, T_AuthFail_RestartAdv);
	}
	shell_write("Authentic failure...\r\n");
}

void App_TimeOutHandle(appEventData_t* pEventData)
{
	bleAppTimerEvent_t* bleAppTimerEvent = ((bleAppTimerEvent_t*)pEventData->eventData.pData);

	switch(bleAppTimerEvent->timeout_flag)
	{
	case T_Start_Pair:
		T_Start_Pair_Timeout_callback(bleAppTimerEvent);
		break;
	case T_Start_Pair_Complete:
		T_Start_Pair_Complete_Timeout_callback(bleAppTimerEvent);
		break;
	case T_Pending_for_Adv:
		T_Pending_for_Adv_Timeout_callback(bleAppTimerEvent);
		break;
	case T_SP_Auth_Tmot:
		T_SP_Auth_Tmot_Timeout_callback(bleAppTimerEvent);
		break;
	case T_SKF_Auth_Tmot:
		T_SKF_Auth_Tmot_Timeout_callback(bleAppTimerEvent);
		break;
	case T_Pending_for_Adv_Scan:
		T_Pending_for_Adv_Scan_Timeout_callback(bleAppTimerEvent);
		break;
	case T_Auth_Valid:
		T_Auth_Valid_Timeout_callback(bleAppTimerEvent);
		break;
	case T_BLEImmo:
		T_BLEImmo_Timeout_callback(bleAppTimerEvent);
		break;
	case T_PhoneKey_SupervisionTO:
		T_PhoneKey_SupervisionTO_Timeout_callback(bleAppTimerEvent);
		break;
	case T_KeyFob_SupervisionTO:
		T_KeyFob_SupervisionTO_Timeout_callback(bleAppTimerEvent);
		break;
	case T_AuthFail_RestartAdv:
		T_AuthFail_RestartAdv_Timeout_callback(bleAppTimerEvent);
		break;
	case T_NioAdv_App:
		T_NioAdv_App_Timeout_callback(bleAppTimerEvent);
		break;
	case T_NioAdv_Ibeacon:
		break;
	case T_BleCanTp_Timeout:
		T_BleCanTp_Timeout_callback(bleAppTimerEvent);
		break;

	default:
		break;
	}

	bleAppTimerEvent = NULL;	/* Let the Pointer point to data */
}

static void T_Start_Pair_Timeout_callback(void *param)
{
	bleAppTimerEvent_t* bleAppTimerEvent = param;
	Clear_BleTimer(bleAppTimerEvent->device_id, T_Start_Pair_Complete);
	Gap_Disconnect(bleAppTimerEvent->device_id);
}

static void T_Start_Pair_Complete_Timeout_callback(void *param)
{
	bleAppTimerEvent_t* bleAppTimerEvent = param;
	Gap_Disconnect(bleAppTimerEvent->device_id);
}

static void T_Pending_for_Adv_Timeout_callback(void *param)
{
	bleAppTimerEvent_t* bleAppTimerEvent = param;
	BleAppIf_StartConAdv();
	Clear_BleTimer(bleAppTimerEvent->device_id, T_SP_Auth_Tmot);
}

static void T_SP_Auth_Tmot_Timeout_callback(void *param)
{
	bleAppTimerEvent_t* bleAppTimerEvent = param;

	Gap_Disconnect(bleAppTimerEvent->device_id);
	Clear_BleTimer(bleAppTimerEvent->device_id, T_SP_Auth_Tmot);
}

static void T_SKF_Auth_Tmot_Timeout_callback(void *param)
{
	bleAppTimerEvent_t* bleAppTimerEvent = param;
	Gap_Disconnect(bleAppTimerEvent->device_id);
	Clear_BleTimer(bleAppTimerEvent->device_id, T_SKF_Auth_Tmot);
}

static void T_Pending_for_Adv_Scan_Timeout_callback(void *param)
{

}

static void T_Auth_Valid_Timeout_callback(void *param)
{

}

static void T_BLEImmo_Timeout_callback(void *param)
{

}

static void T_PhoneKey_SupervisionTO_Timeout_callback(void *param)
{

}

static void T_KeyFob_SupervisionTO_Timeout_callback(void *param)
{

}

/* Time is up, start ADV and clear the timer*/
static void T_AuthFail_RestartAdv_Timeout_callback(void *param)
{
	bleAppTimerEvent_t* bleAppTimerEvent = param;
	Clear_BleTimer(bleAppTimerEvent->device_id, bleAppTimerEvent->timeout_flag);
	BleAppIf_Start();
}

static void T_NioAdv_App_Timeout_callback(void *param)
{

}

static void T_NioAdv_Ibeacon_Timeout_callback(void *param)
{

}

static void T_BleCanTp_Timeout_callback(void *param)
{
/*TODO */
//	CanTp_RxMainFunction();
//	CanTp_TxMainFunction();
}

static void Ble_UpdataIRK(uint8_t deviceId)
{

}
