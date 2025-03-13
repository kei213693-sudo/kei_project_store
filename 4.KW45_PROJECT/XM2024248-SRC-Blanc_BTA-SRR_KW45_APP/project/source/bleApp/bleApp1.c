/*
 * bleApp1.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "fsl_component_mem_manager.h"
#include "UdsItf_WriteDid.h"
#include "UdsItf_ReadDid.h"
#include "gap_interface.h"
#include "bleAppEvent.h"
#include "signals_api.h"
#include "bleDrv_api.h"
#include "autosar_nm.h"
#include "drv_api.h"
#include "srv_api.h"
#include "app_api.h"

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
//        	CanTp_RxMainFunction();
        	break;

        case mAppEvt_BleMsgRev_c:
        	BleTp_RxMainFunction(deviceId);
        	break;

        case mAppEvt_CanMsgTransmit_c:
//        	CanTp_TxMainFunction();
        	break;

        case mAppEvt_BleMsgTransmit_c:
        	BleTp_TxMainFunction(deviceId);
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
        	BleAppIf_StartConAdv();
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
            dbg_Printf("Please enter 'Auth YES' or 'Auth NO'.\r\n");
//			BleAppIf_BeaconAdvertise();
//			ble_SetSleepFlag(0);
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

void APP_GapStopConnection(void)
{
	if(Get_BLE_DV_ConnectingSts() == BLE_DV_IS_CONNECTING)
	{
		Gap_Disconnect(0);
	}
}

void APP_Gap_StopAdv(void)
{
	if(Get_bit_bleAdvState() == 1)
	{
		gCurrentAdvHandle = (uint8_t)gNoAdvSetHandle_c;
		Gap_StopExtAdvertising(0xff);
	}
}

/* 100ms */
void APP_APPModeControl(void)
{
	static uint8_t oldKeyAddressIndex = 0;
	static uint8_t resetflag = 0;
	static uint8_t resetDelayCnt = 0;
	static uint16_t delayCnt = 0;
	Nm_StateType nmStatePtr;
	Nm_ModeType nmModePtr;
	CanNm_GetState(0, &nmStatePtr, &nmModePtr);

	UDS_itf_readDid_4AFF(&oldKeyAddressIndex);

	if(oldKeyAddressIndex != Get_BLE_AddressIndex())
	{
		UDS_itf_writeDid_4AFF(&Get_BLE_AddressIndex());
		oldKeyAddressIndex = Get_BLE_AddressIndex();
		resetflag = 1;
	}

	if(resetflag == 1)
	{
		resetDelayCnt++;
		if(resetDelayCnt >= 25)
		{
			resetDelayCnt = 0;
			resetflag = 0;
			hal_sys_if_reset();
		}
	}

//	if(nmModePtr == NM_PREPAREBUSSLEEP)
//	{
//		APP_Gap_StopAdv();
//	}
}
