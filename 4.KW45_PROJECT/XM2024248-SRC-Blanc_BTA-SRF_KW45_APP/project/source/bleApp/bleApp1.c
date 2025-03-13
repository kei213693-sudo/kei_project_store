/*
 * bleApp1.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "fsl_component_mem_manager.h"
#include "gap_interface.h"
#include "bleAppEvent.h"
#include "signals_api.h"
#include "bleDrv_api.h"
#include "bleAppMsg.h"
#include "Uds_api.h"
#include "drv_api.h"
#include "srv_api.h"
#include "app_api.h"
#include "app.h"

/*!*************************************************************************************************
* \fn     uint8_t APP_UserInterfaceEventHandler(uint8_t *pData)
* \brief  This function is used to handle User Interface events from BLE
*         To register another function use BleApp_RegisterUserInterfaceEventHandler
*
* \param  [in]   pData - pointer to appEventData_t data;
 ***************************************************************************************************/
void APP_AppMessageHandler(void *pData)
{
    appEventData_t *pEventData = (appEventData_t *)pData;
    appMsgBleCanData_t* appMsgDataPtr = (appMsgBleCanData_t*)pEventData->eventData.pData;

    uint8_t deviceId = appMsgDataPtr->deviceId;

    switch(pEventData->appEvent)
    {
        case mAppEvt_CanMsgRev_c:
        	CanTp_RxMainFunction();
        	break;

        case mAppEvt_BleMsgRev_c:
        	BleTp_RxMainFunction(deviceId);
        	break;

        case mAppEvt_CanMsgTransmit_c:
        	CanTp_TxMainFunction();
        	break;

        case mAppEvt_BleMsgTransmit_c:
        	BleTp_TxMainFunction(deviceId);
        	break;

        case mAppEvt_CanMsgRevCompleteHandle_c:
        	CanTp_MsgRevHandle(deviceId);
        	break;

        case mAppEvt_CanMsgSendCompleteHandle_c:
        	CanTp_MsgSendHandle();
        	break;

        case mAppEvt_BleMsgRevCompleteHandle_c:
        	BleTp_MsgRevHandle(deviceId);
        	break;

        case mAppEvt_BleMsgSendCompleteHandle_c:
        	BleTp_MsgSendHandle();
        	break;

        default:
        {
            ; /* No action required */
        }
        break;
    }

    (void)MEM_BufferFree(pData);
    pData = NULL;
}

/*!*************************************************************************************************
* \fn     uint8_t APP_UserInterfaceEventHandler(uint8_t *pData)
* \brief  This function is used to handle User Interface events from BLE
*         To register another function use BleApp_RegisterUserInterfaceEventHandler
*
* \param  [in]   pData - pointer to appEventData_t data;
 ***************************************************************************************************/
void APP_AppEventHandler(void *pData)
{
    /* Here custom code can be added to handle user interface update based on application events */
    appEventData_t *pEventData = (appEventData_t *)pData;

    switch(pEventData->appEvent)
    {
    	case mAppEvt_BleCmd_c:
    	{

    	}
    	break;

        case mAppEvt_AuthenStart_c:
        {

        }
        break;

        case mAppEvt_Authening_c:
        {

        }
        break;

        case mAppEvt_AuthenPrefailed_c:
        case mAppEvt_AuthenFail_c:
        {
        	App_BleAuthPreFailureHandle(pEventData);
        }
        break;

        case mAppEvt_AuthenPass_c:
        {
        	BleAppIf_Start();
        }
        break;

        case mAppEvt_TimeOut_c:
        {
        	App_TimeOutHandle(pEventData);
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }

    (void)MEM_BufferFree(pData);
    pData = NULL;
}


/*!*************************************************************************************************
* \fn     uint8_t APP_UserInterfaceEventHandler(uint8_t *pData)
* \brief  This function is used to handle User Interface events from BLE
*         To register another function use BleApp_RegisterUserInterfaceEventHandler
*
* \param  [in]   pData - pointer to appEventData_t data;
 ***************************************************************************************************/
void APP_UserInterfaceEventHandler(void *pData)
{
    /* Here custom code can be added to handle user interface update based on application events */
    appEventData_t *pEventData = (appEventData_t *)pData;
    static uint8_t secondAdvFlag = 0;

    switch(pEventData->appEvent)
    {
        case mAppEvt_PeerConnected_c:
        {
        	App_PeerConnectedHandle(pEventData);
        }
        break;

        case mAppEvt_PsmChannelCreated_c:
        {
            shell_write("\r\nL2CAP PSM Connection Complete.\r\n");
        }
        break;

        case mAppEvt_PairingPeerOobDataRcv_c:
        {
            shell_write("\r\nReceived First_Approach_RQ.\r\n");
        }
        break;

        case mAppEvt_PeerDisconnected_c:
        {
        	App_PeerDisconnectedHandle(pEventData);
        }
        break;

        case mAppEvt_PairingLocalOobData_c:
        {
            shell_write("\r\nSending First_Approach_RS\r\n");
        }
        break;

        case mAppEvt_EncryptionChanged_c:
        {
        	App_EncryptionChangedHandle(pEventData);
        }
        break;

        case mAppEvt_PairingComplete_c:
        {
        	App_PairingCompleteHandle(pEventData);
        }
        break;

        case mAppEvt_PairingReqRcv_c:
        {
        	App_PairingReqRcvHandle(pEventData);
        }
        break;

        case mAppEvt_SPAKERequestSent_c:
        {
            shell_write("\r\nSPAKE Request sent.\r\n");
        }
        break;

        case mAppEvt_SPAKEVerifySent_c:
        {
            shell_write("\r\nSPAKE Verify sent.\r\n");
        }
        break;

        case mAppEvt_AdvertisingStartedLegacy_c:
        {
            shell_write("\r\nAdvertising started - Legacy.\r\n");
//            if(secondAdvFlag == 0)
//            {
//            	secondAdvFlag = 1;
//            	BleAppIf_NioAdvertise();
//            }
        }
        break;

        case mAppEvt_AdvertisingStartedExtendedLR_c:
        {
            shell_write("Advertising started - Extended LR.\r\n");
        }
        break;

        case mAppEvt_AdvertisingStopped_c:
        {
        	secondAdvFlag = 0;
            shell_write("Advertising stopped - All PHYs.\r\n");
        }
        break;

        case mAppEvt_BleConfigDone_c:
        {
        	/*TODO Initial Adv/Scan Parameters*/
        	AppEvt_BleConfigDoneHandle();
            shell_write("\r\nDigital Key Car Anchor.\r\n");

        }
        break;

        case mAppEvt_BleScanning_c:
        {
        	ble_SetSleepFlag(1);
            shell_write("Scanning...\r\n");
        }
        break;

        case mAppEvt_BleScanStopped_c:
        {
            shell_write("Scan stopped.\r\n");
        }
        break;

        case mAppEvt_BleConnectingToDevice_c:
        {
        	App_BleConnectingHandle(pEventData);
        }
        break;

        case mAppEvt_LePhyEvent_c:
        {
#if defined(gAppUseShellInApplication_d) && (gAppUseShellInApplication_d == 1)
//            AppPrintLePhyEvent((gapPhyEvent_t *)pEventData->eventData.pData);
#endif
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }

    (void)MEM_BufferFree(pData);
    pData = NULL;
}

/*!*************************************************************************************************
 *\fn     uint8_t APP_BleEventHandler(uint8_t *pData)
 *\brief  This function is used to handle events from BLE
 *        To register another function use BleApp_RegisterEventHandler
 *
 *\param  [in]   pData - pointer to appEventData_t data;
 ***************************************************************************************************/
void APP_BleEventHandler(void *pData)
{
    appEventData_t *pEventData = (appEventData_t *)pData;

    switch(pEventData->appEvent)
    {
        case mAppEvt_GenericCallback_LeScLocalOobData_c:
        {
#if defined(gAppUseShellInApplication_d) && (gAppUseShellInApplication_d == 1)
            /* Code for CCC demo application */
            (void)App_HandleLeScLocalOobDataCallback(pEventData);
#else
            /*
             * To custom handle OOB data, add your code here and set gAppUseShellInApplication_d to 0 in app_preinclude.h
             * The shell will be disabled too.
             * The OOB data is received in pEventData->eventData.pData as gapLeScOobData_t
             */
            /* Do Nothing! */
#endif
        }
        break;

        case mAppEvt_GenericCallback_BondCreatedEvent_c:
        {
#if defined(gAppUseShellInApplication_d) && (gAppUseShellInApplication_d == 1)

#if (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U))
            A2A_HandleGenericCallbackBondCreatedEvent(pEventData);
#else
            App_HandleGenericCallbackBondCreatedEvent(pEventData);
#endif /* (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U)) */
#else
            /*TODO */
            App_HandleGenericCallbackBondCreatedEvent(pEventData);
            /*
             * To custom handle bond created event, add your code here and set gAppUseShellInApplication_d to 0 in app_preinclude.h
             * The shell will be disabled too.
             * The bond created event is received in pEventData->eventData.pData as bleBondCreatedEvent_t
             */
#endif
        }
        break;

        case mAppEvt_L2capPsmDataCallback_c:
        {
#if defined(gAppUseShellInApplication_d) && (gAppUseShellInApplication_d == 1)
            App_HandleL2capPsmDataCallback(pEventData);
#else
             /*
             * To custom handle L2CAP data packet, add your code here and set gAppUseShellInApplication_d to 0 in app_preinclude.h
             * The shell will be disabled too.
             * The L2CAP data packet is received in pEventData->eventData.pData as appEventL2capPsmData_t
             */
#endif
        }
        break;

        case mAppEvt_FactoryReset_c:
        {
#if defined(gAppUseShellInApplication_d) && (gAppUseShellInApplication_d == 1)
        	//TODO FFFF
//            BleApp_FactoryReset();
#else
            /*
             * To custom handle factory reset data packet, add your code here and set gAppUseShellInApplication_d to 0 in app_preinclude.h
             * The shell will be disabled too.
             */
#endif
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }

    (void)MEM_BufferFree(pData);
    pData = NULL;
}

/* *Phone
 * Public Address:  d0 41 6a 61 23 24
 * Identity Resolving Key:  4b 23 7a 56 50 1f c8 c9 5b 31 ab ff ef d2 fd 75
*/

/* *SRF
#define APP_BD_ADDR     {0xC5, 0x00, 0x00, 0x00, 0x00, 0xC4}
#define APP_VEHICLE_ID	{0x80, 0x39, 0x46, 0x36, 0x18, 0xa7, 0xe3, 0xd6, 0xb7, 0xca, 0x2f, 0xd4, 0x43, 0x1d, 0xba, 0xbc}
#define APP_SMP_IRK     {0x0A, 0x2D, 0xF4, 0x65, 0xE3, 0xBD, 0x7B, 0x49, 0x1E, 0xB4, 0xC0, 0x95, 0x95, 0x13, 0x46, 0x73}

*/

#if 0
static uint8_t hashValue[3] = {0};
static uint8_t rpAddress[3] = {0x69, 0x18, 0xFA};
static uint8_t tempIRK[16] = {0x0A, 0x2D, 0xF4, 0x65, \
		0xE3, 0xBD, 0x7B, 0x49, \
		0x1E, 0xB4, 0xC0, 0x95, \
		0x95, 0x13, 0x46, 0x73};
static uint8_t tempIRK1[16] = {0x73, 0x46, 0x13, 0x95, \
		0x95, 0xC0, 0xB4, 0x1E, \
		0x49, 0x7B, 0xBD, 0xE3, \
		0x65, 0xF4, 0x2D, 0x0A};
//static uint8_t tempIRK[16] = {0x4b, 0x23, 0x7a, 0x56, \
//		0x50, 0x1f, 0xc8, 0xc9, \
//		0x5b, 0x31, 0xab, 0xff, \
//		0xef, 0xd2, 0xfd, 0x75};
#endif

void APP_Ble10ms_Proc(void)
{
	static uint8_t cycleTicks = 0;
	static uint8_t deviceId = 0;
	static uint8_t oldAdvCmd = 0;
	static uint8_t oldScanCmd = 0;
	static uint8_t oldScanIndex = 0;

	cycleTicks++;
	if(cycleTicks >= 10)
	{
		cycleTicks = 0;
		for (uint8_t deviceId = 0; deviceId < gAppMaxConnections_c; deviceId++)
		{
			if(maPeerInformation[deviceId].appState == mAppRunning_c)
			{
				Ble_UpdataIRK(deviceId);
				Gap_ReadRssi(deviceId);
			}
		}
	}

	Set_Did_4A56_Value(UdsApi_get_BleTurnOnState());

	if(oldAdvCmd != Get_Did_4A56_Value())
	{
		if(Get_Did_4A56_Value() == DV_BLE_OPEN_ENABLE)
		{
			/* 打开广播 */
			if(!Get_bit_bleAdvState())
			{
				gCurrentAdvHandle = (uint8_t)gLegacyAdvSetHandle_c;
				BleAppIf_NioAdvertise();
			}
		}
		else if(Get_Did_4A56_Value() == DV_BLE_OPEN_DISABLE)
		{
			/* 关闭广播 */
			if(Get_bit_bleAdvState())
			{
				gCurrentAdvHandle = (uint8_t)gNoAdvSetHandle_c;
				(void)Gap_StopExtAdvertising(0xff);
			}
		}
		oldAdvCmd = Get_Did_4A56_Value();
	}

	if(EcuM_GetCurrentState() != ECUM_State_Normal)
	{
		/* 关闭广播 */
		if(Get_bit_bleAdvState())
		{
			(void)Gap_StopExtAdvertising(0xff);
		}
	}
#if 0
	static uint8_t delayCnt = 0;
	delayCnt++;
	if(delayCnt >= 100)
	{
		delayCnt = 90;
		SecLib_VerifyBluetoothAh(hashValue, tempIRK, rpAddress);
	}

#endif

#if 0
	static uint8_t sendCycle = 0;
	if(sendCycle >= 50)
	{
		sendCycle = 0;
		for(uint8_t i = 0; i < 6; i++)
		{
			bleMsgCanBuff.MAC_IRK[i] = i;
		}
		bleMsgCanBuff.pPacket[0] = 0x01;
		bleMsgCanBuff.pPacket[1] = 0x02;
		bleMsgCanBuff.pPacket[2] = 0;
		bleMsgCanBuff.pPacket[3] = 0x10;
		bleMsgCanBuff.packetLength = 0x14;
		Tp_Transmit(0x4E4, &bleMsgCanBuff);
	}
#endif
}

void App_BleClearPeerInfomation(uint8_t deviceId)
{
	memset(&maPeerInformation[deviceId], 0, sizeof(appPeerInfo_t));
}

