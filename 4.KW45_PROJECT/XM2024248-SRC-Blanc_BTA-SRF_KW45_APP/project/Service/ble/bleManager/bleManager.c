/*
 * bleManager.c
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#include "bleManager_cfg.h"

static void BluetoothLEHostGeneralCallback_Debug(uint32_t EventType);
static void BluetoothLEHostAdvertisementCallback_Debug(uint32_t EventType);
static void BluetoothLEHostScanCallback_Debug(uint32_t EventType);
static void BluetoothLEHostConnectionCallback_Debug(uint32_t EventType);
static void BluetoothLEHostGattServerCallback_Debug(uint32_t EventType);
static void BluetoothLEHostClientProcedureMsgCallback_Debug(uint32_t EventType);
static void BluetoothLEHostGattClientNotificationMsgCallback_Debug(uint16_t handler);
static void BluetoothLEHostL2caLeDataMsgCallback_Debug(uint16_t channelId);
static void BluetoothLEHostL2caLeControlMsgCallback_Debug(uint32_t EventType);


static void App_HandleHostMessageInput
(
    appMsgFromHost_t* pMsg
);
static bool_t App_HandleHostMessageInput_Gap
(
    appMsgFromHost_t* pMsg
);
static bool_t App_HandleHostMessageInput_Gatt
(
    appMsgFromHost_t* pMsg
);
static bool_t App_HandleHostMessageInput_L2ca
(
    appMsgFromHost_t* pMsg
);

/*! *********************************************************************************
*\fn           void BluetoothLEHost_HandleMessages(void)
*\brief        This function is responsible for consuming all events coming from the
*              Bluetooth LE stack.
*
*\param  [in]  none.
*
*\retval       void.
********************************************************************************** */
void BluetoothLEHost_HandleMessages(void)
{
#ifdef SDK_OS_FREE_RTOS
    osa_event_flags_t event = 0U;
    (void)OSA_EventWait((osa_event_handle_t)mAppEvent,
                        osaEventFlagsAll_c,
                        FALSE,
                        gAppTaskWaitTimeout_ms_c ,
                        &event);
#endif /* SDK_OS_FREE_RTOS */

    /* Check for existing messages in queue */
    if (MSG_QueueGetHead(&mHostAppInputQueue) != NULL)
    {
        /* Pointer for storing the messages from host. */
        appMsgFromHost_t *pMsgIn = MSG_QueueRemoveHead(&mHostAppInputQueue);

        if (pMsgIn != NULL)
        {
            /* Process it */
            App_HandleHostMessageInput(pMsgIn);

            /* Messages must always be freed. */
            (void)MSG_Free(pMsgIn);
        }
    }

    /* Check for existing messages in queue */
    if (MSG_QueueGetHead(&mAppCbInputQueue) != NULL)
    {
        /* Pointer for storing the callback messages. */
        appMsgCallback_t *pMsgIn = MSG_QueueGetHead(&mAppCbInputQueue);

        if (pMsgIn != NULL)
        {
            /* Execute callback handler */
            if (pMsgIn->handler != NULL)
            {
                pMsgIn->handler(pMsgIn->param);
            }

            /* Messages must always be freed. */
            (void)MSG_Free(pMsgIn);
        }
    }

#ifdef SDK_OS_FREE_RTOS
    /* Signal the main_thread again if there are more messages pending */
    event = (MSG_QueueGetHead(&mHostAppInputQueue) != NULL) ? gAppEvtMsgFromHostStack_c : 0U;
    event |= (MSG_QueueGetHead(&mAppCbInputQueue) != NULL) ? gAppEvtAppCallback_c : 0U;
    if (event != 0U)
    {
    	(void)OSA_EventSet((osa_event_handle_t)mAppEvent, gAppEvtAppCallback_c);
    }
#endif /* SDK_OS_FREE_RTOS */
}

/*! *********************************************************************************
*\private
*\fn           void App_HandleHostMessageInput(appMsgFromHost_t* pMsg)
*\brief        Handles all messages received from the host task.
*
*\param  [in]  pMsg    Pointer to the mesage received from the host task.
*
*\retval       void.
********************************************************************************** */
static void App_HandleHostMessageInput
(
    appMsgFromHost_t* pMsg
)
{
    bool_t matchFound = App_HandleHostMessageInput_Gap(pMsg);

    if (!matchFound)
    {
        matchFound = App_HandleHostMessageInput_Gatt(pMsg);
    }

    if (!matchFound)
    {
        matchFound = App_HandleHostMessageInput_L2ca(pMsg);
    }
}

/*! *********************************************************************************
*\private
*\fn           bool_t App_HandleHostMessageInput_Gap(appMsgFromHost_t* pMsg)
*\brief        Handles all messages received from the host task if the message
*              addresses GAP functionality. Called by App_HandleHostMessageInput().
*
*\param  [in]  pMsg    Pointer to the mesage received from the host task.
*
*\retval       TRUE     If the message type matched and a handler function was called
*\retval       FALSE    Message was not GAP layer related
********************************************************************************** */
static bool_t App_HandleHostMessageInput_Gap
(
    appMsgFromHost_t* pMsg
)
{
    bool_t matchFound = TRUE;
    switch ( pMsg->msgType )
    {
        case (uint32_t)gAppGapGenericMsg_c:
        {
        	//TODO cyl 添加generic Message的EventType
        	BluetoothLEHostGeneralCallback_Debug(pMsg->msgData.genericMsg.eventType);
            if (mpfGenericHandler != NULL)
            {
                mpfGenericHandler(&pMsg->msgData.genericMsg);
            }
            break;
        }
        case (uint32_t)gAppGapAdvertisementMsg_c:
        {
        	//TODO cyl 添加GAP Advertisement的EventType
        	BluetoothLEHostAdvertisementCallback_Debug(pMsg->msgData.advMsg.eventType);
            if (pfAdvCallback != NULL)
            {
                pfAdvCallback(&pMsg->msgData.advMsg);
            }
            break;
        }
        case (uint32_t)gAppGapScanMsg_c:
        {
        	//TODO cyl 添加GAP Scan的EventType
        	BluetoothLEHostScanCallback_Debug(pMsg->msgData.scanMsg.eventType);
            if (pfScanCallback != NULL)
            {
                pfScanCallback(&pMsg->msgData.scanMsg);
            }
            break;
        }
        case (uint32_t)gAppGapConnectionMsg_c:
        {
        	//TODO cyl 添加GAP Connection的EventType
        	BluetoothLEHostConnectionCallback_Debug(pMsg->msgData.connMsg.connEvent.eventType);
            if (pfConnCallback != NULL)
            {
                pfConnCallback(pMsg->msgData.connMsg.deviceId,
                               &pMsg->msgData.connMsg.connEvent);
            }
            break;
        }
        default:
        {
            matchFound = FALSE;
                        break;
        }
    }

    return matchFound;
}

/*! *********************************************************************************
*\private
*\fn           bool_t App_HandleHostMessageInput_Gatt(appMsgFromHost_t* pMsg)
*\brief        Handles all messages received from the host task if the message
*              addresses GATT functionality. Called by App_HandleHostMessageInput().
*
*\param  [in]  pMsg    Pointer to the mesage received from the host task.
*
*\retval       TRUE     If the message type matched and a handler function was called
*\retval       FALSE    Message was not GATT layer related
********************************************************************************** */
static bool_t App_HandleHostMessageInput_Gatt
(
    appMsgFromHost_t* pMsg
)
{
    bool_t matchFound = TRUE;
    switch ( pMsg->msgType )
    {
        case (uint32_t)gAppGattServerMsg_c:
        {
        	//TODO cyl 添加GATT Server的EventType
        	BluetoothLEHostGattServerCallback_Debug(pMsg->msgData.gattServerMsg.serverEvent.eventType);
            if (pfGattServerCallback != NULL)
            {
                pfGattServerCallback(pMsg->msgData.gattServerMsg.deviceId,
                                     &pMsg->msgData.gattServerMsg.serverEvent);
            }
            break;
        }
        case (uint32_t)gAppGattClientProcedureMsg_c:
        {
        	//TODO cyl 添加GATT gAppGattClientProcedureMsg_c的Type
        	BluetoothLEHostClientProcedureMsgCallback_Debug(pMsg->msgData.gattClientProcMsg.procedureType);
            if (pfGattClientProcCallback != NULL)
            {
                pfGattClientProcCallback(
                    pMsg->msgData.gattClientProcMsg.deviceId,
                    pMsg->msgData.gattClientProcMsg.procedureType,
                    pMsg->msgData.gattClientProcMsg.procedureResult,
                    pMsg->msgData.gattClientProcMsg.error);
            }
            break;
        }
        case (uint32_t)gAppGattClientNotificationMsg_c:
        {
            if (pfGattClientNotifCallback != NULL)
            {
            	//TODO cyl 添加GATT gAppGattClientNotificationMsg_c的handler
            	BluetoothLEHostGattClientNotificationMsgCallback_Debug(pMsg->msgData.gattClientNotifIndMsg.characteristicValueHandle);
                pfGattClientNotifCallback(
                    pMsg->msgData.gattClientNotifIndMsg.deviceId,
                    pMsg->msgData.gattClientNotifIndMsg.characteristicValueHandle,
                    pMsg->msgData.gattClientNotifIndMsg.aValue,
                    pMsg->msgData.gattClientNotifIndMsg.valueLength);
            }
            break;
        }
        case (uint32_t)gAppGattClientIndicationMsg_c:
        {
            if (pfGattClientIndCallback != NULL)
            {
            	//TODO cyl 添加GATT gAppGattClientIndicationMsg_c的Handler
            	BluetoothLEHostGattClientNotificationMsgCallback_Debug(pMsg->msgData.gattClientNotifIndMsg.characteristicValueHandle);
                pfGattClientIndCallback(
                    pMsg->msgData.gattClientNotifIndMsg.deviceId,
                    pMsg->msgData.gattClientNotifIndMsg.characteristicValueHandle,
                    pMsg->msgData.gattClientNotifIndMsg.aValue,
                    pMsg->msgData.gattClientNotifIndMsg.valueLength);
            }
            break;
        }
        default:
        {
            matchFound = FALSE;
            break;
        }
    }
    return matchFound;
}

/*! *********************************************************************************
*\private
*\fn           bool_t App_HandleHostMessageInput_L2ca(appMsgFromHost_t* pMsg)
*\brief        Handles all messages received from the host task if the message
*              addresses L2CAP functionality. Called by App_HandleHostMessageInput().
*
*\param  [in]  pMsg    Pointer to the mesage received from the host task.
*
*\retval       TRUE     If the message type matched and a handler function was called
*\retval       FALSE    Message was not L2CAP layer related
********************************************************************************** */
static bool_t App_HandleHostMessageInput_L2ca
(
    appMsgFromHost_t* pMsg
)
{
    bool_t matchFound = TRUE;
    switch ( pMsg->msgType )
    {
        case (uint32_t)gAppL2caLeDataMsg_c:
        {
        	//TODO cyl 添加L2caLe Data Msg的channel id
//			BluetoothLEHostL2caLeDataMsgCallback_Debug(pMsg->msgData.l2caLeCbDataMsg.channelId);
            if (pfL2caLeCbDataCallback != NULL)
            {
                pfL2caLeCbDataCallback(
                    pMsg->msgData.l2caLeCbDataMsg.deviceId,
                    pMsg->msgData.l2caLeCbDataMsg.channelId,
                    pMsg->msgData.l2caLeCbDataMsg.aPacket,
                    pMsg->msgData.l2caLeCbDataMsg.packetLength);
            }
            break;
        }
        case (uint32_t)gAppL2caLeControlMsg_c:
        {
        	//TODO cyl 添加L2caLe control msg的Message type
//			BluetoothLEHostL2caLeControlMsgCallback_Debug(pMsg->msgData.l2caLeCbControlMsg.messageType);
            if (pfL2caLeCbControlCallback != NULL)
            {
                pfL2caLeCbControlCallback(&pMsg->msgData.l2caLeCbControlMsg);
            }
            break;
        }
        default:
        {
            matchFound = FALSE;
            break;
        }
    }
    return matchFound;
}

/************************************************************************************
 *  void BluetoothLEHost_Debug()
 *
 * *********************************************************************************/
static void BluetoothLEHostGeneralCallback_Debug(uint32_t EventType)
{
	switch (EventType)
	{
	case gInitializationComplete_c:
		shell_write("\r\n ----gInitializationComplete_c\r\n");
		break;
	case gInternalError_c:
		shell_write("\r\n ----gInternalError_c\r\n");
		break;
	case gAdvertisingSetupFailed_c:
		shell_write("\r\n ----gAdvertisingSetupFailed_c \r\n");
		break;
	case gAdvertisingParametersSetupComplete_c:
		shell_write("\r\n ----gAdvertisingParametersSetupComplete_c \r\n");
		break;
	case gAdvertisingDataSetupComplete_c:
		shell_write("\r\n ----gAdvertisingDataSetupComplete_c \r\n");
		break;
	case gFilterAcceptListSizeRead_c:
		shell_write("\r\n ----gFilterAcceptListSizeRead_c \r\n");
		break;
	case gDeviceAddedToFilterAcceptList_c:
		shell_write("\r\n ----gDeviceAddedToFilterAcceptList_c \r\n");
		break;
	case gDeviceRemovedFromFilterAcceptList_c:
		shell_write("\r\n ----gDeviceRemovedFromFilterAcceptList_c \r\n");
		break;
	case gFilterAcceptListCleared_c:
		shell_write("\r\n ----gFilterAcceptListCleared_c \r\n");
		break;
	case gRandomAddressReady_c:
		shell_write("\r\n ----gRandomAddressReady_c \r\n");
		break;
	case gCreateConnectionCanceled_c:
		shell_write("\r\n ----gCreateConnectionCanceled_c \r\n");
		break;
	case gPublicAddressRead_c:
		shell_write("\r\n ----gPublicAddressRead_c \r\n");
		break;
	case gAdvTxPowerLevelRead_c:
		shell_write("\r\n ----gAdvTxPowerLevelRead_c \r\n");
		break;
	case gPrivateResolvableAddressVerified_c:
		shell_write("\r\n ----gPrivateResolvableAddressVerified_c \r\n");
		break;
	case gRandomAddressSet_c:
		shell_write("\r\n ----gRandomAddressSet_c \r\n");
		break;
	case gLeScPublicKeyRegenerated_c:
		shell_write("\r\n gLeScPublicKeyRegenerated_c \r\n");
		break;
	case gLeScLocalOobData_c:
		shell_write("\r\n ----gLeScLocalOobData_c \r\n");
		break;
	case gHostPrivacyStateChanged_c:
		shell_write("\r\n ----gHostPrivacyStateChanged_c \r\n");
		break;
	case gControllerPrivacyStateChanged_c:
		shell_write("\r\n ----gControllerPrivacyStateChanged_c \r\n");
		break;
	case gControllerTestEvent_c:
		shell_write("\r\n ----gControllerTestEvent_c \r\n");
		break;
	case gTxPowerLevelSetComplete_c:
		shell_write("\r\n ----gTxPowerLevelSetComplete_c \r\n");
		break;
	case gLePhyEvent_c:
		shell_write("\r\n ----gLePhyEvent_c \r\n");
		break;
	case gControllerNotificationEvent_c:
		shell_write("\r\n ----gControllerNotificationEvent_c \r\n");
		break;
	case gGetConnParamsComplete_c:
		shell_write("\r\n ----gGetConnParamsComplete_c \r\n");
		break;
	case gBondCreatedEvent_c:
		shell_write("\r\n ----gBondCreatedEvent_c \r\n");
		break;
	case gChannelMapSet_c:
		shell_write("\r\n ----gChannelMapSet_c \r\n");
		break;


	case gExtAdvertisingParametersSetupComplete_c:
		shell_write("\r\n ----gExtAdvertisingParametersSetupComplete_c \r\n");
		break;
	case gExtAdvertisingDataSetupComplete_c:
		shell_write("\r\n ----gExtAdvertisingDataSetupComplete_c \r\n");
		break;
	case gExtAdvertisingSetRemoveComplete_c:
		shell_write("\r\n ----gExtAdvertisingSetRemoveComplete_c \r\n");
		break;
	case gPeriodicAdvParamSetupComplete_c:
		shell_write("\r\n ----gPeriodicAdvParamSetupComplete_c \r\n");
		break;
	case gPeriodicAdvDataSetupComplete_c:
		shell_write("\r\n ----gPeriodicAdvDataSetupComplete_c \r\n");
		break;
	case gPeriodicAdvertisingStateChanged_c:
		shell_write("\r\n ----gPeriodicAdvertisingStateChanged_c \r\n");
		break;
	case gPeriodicAdvListUpdateComplete_c:
		shell_write("\r\n ----gPeriodicAdvListUpdateComplete_c \r\n");
		break;
	case gPeriodicAdvCreateSyncCancelled_c:
		shell_write("\r\n ----gPeriodicAdvCreateSyncCancelled_c \r\n");
		break;
	case gTxEntryAvailable_c:
		shell_write("\r\n ----gTxEntryAvailable_c \r\n");
		break;
	case gControllerLocalRPARead_c:
		shell_write("\r\n ----gControllerLocalRPARead_c \r\n");
		break;


	case gConnectionlessCteTransmitParamsSetupComplete_c:
		shell_write("\r\n ----gConnectionlessCteTransmitParamsSetupComplete_c \r\n");
		break;
	case gConnectionlessCteTransmitStateChanged_c:
		shell_write("\r\n ----gConnectionlessCteTransmitStateChanged_c \r\n");
		break;
	case gConnectionlessIqSamplingStateChanged_c:
		shell_write("\r\n ----gConnectionlessIqSamplingStateChanged_c \r\n");
		break;
	case gAntennaInformationRead_c:
		shell_write("\r\n ----gAntennaInformationRead_c \r\n");
		break;


	case gModifiedSleepClockAccuracy_c:
		shell_write("\r\n ----gModifiedSleepClockAccuracy_c \r\n");
		break;


	case gPeriodicAdvRecvEnableComplete_c:
		shell_write("\r\n ----gPeriodicAdvRecvEnableComplete_c \r\n");
		break;
	case gPeriodicAdvSyncTransferComplete_c:
		shell_write("\r\n ----gPeriodicAdvSyncTransferComplete_c \r\n");
		break;
	case gPeriodicAdvSetInfoTransferComplete_c:
		shell_write("\r\n ----gPeriodicAdvSetInfoTransferComplete_c \r\n");
		break;
	case gSetPeriodicAdvSyncTransferParamsComplete_c:
		shell_write("\r\n ----gSetPeriodicAdvSyncTransferParamsComplete_c \r\n");
		break;
	case gSetDefaultPeriodicAdvSyncTransferParamsComplete_c:
		shell_write("\r\n ----gSetDefaultPeriodicAdvSyncTransferParamsComplete_c \r\n");
		break;
	case gPeriodicAdvSyncTransferSucceeded_c:
		shell_write("\r\n ----gPeriodicAdvSyncTransferSucceeded_c \r\n");
		break;
	case gPeriodicAdvSyncTransferFailed_c:
		shell_write("\r\n ----gPeriodicAdvSyncTransferFailed_c \r\n");
		break;
	case gConnEvtLeGenerateDhKeyComplete_c:
		shell_write("\r\n ----gConnEvtLeGenerateDhKeyComplete_c \r\n");
		break;


	case gHandoverGetComplete_c:
		shell_write("\r\n ----gHandoverGetComplete_c \r\n");
		break;
	case gHandoverSetComplete_c:
		shell_write("\r\n ----gHandoverSetComplete_c \r\n");
		break;
	case gHandoverGetCsLlContextComplete_c:
		shell_write("\r\n ----gHandoverGetCsLlContextComplete_c \r\n");
		break;
	case gHandoverSetCsLlContextComplete_c:
		shell_write("\r\n ----gHandoverSetCsLlContextComplete_c \r\n");
		break;
	case gHandoverGetTime_c:
		shell_write("\r\n ----gHandoverGetTime_c \r\n");
		break;
	case gHandoverSuspendTransmitComplete_c:
		shell_write("\r\n ----gHandoverSuspendTransmitComplete_c \r\n");
		break;
	case gHandoverResumeTransmitComplete_c:
		shell_write("\r\n ----gHandoverResumeTransmitComplete_c \r\n");
		break;
	case gHandoverAnchorNotificationStateChanged_c:
		shell_write("\r\n ----gHandoverAnchorNotificationStateChanged_c \r\n");
		break;
	case gHandoverAnchorSearchStarted_c:
		shell_write("\r\n ----gHandoverAnchorSearchStarted_c \r\n");
		break;
	case gHandoverAnchorSearchStopped_c:
		shell_write("\r\n ----gHandoverAnchorSearchStopped_c \r\n");
		break;
	case gHandoverTimeSyncTransmitStateChanged_c:
		shell_write("\r\n ----gHandoverTimeSyncTransmitStateChanged_c \r\n");
		break;
	case gHandoverTimeSyncReceiveComplete_c:
		shell_write("\r\n ----gHandoverTimeSyncReceiveComplete_c \r\n");
		break;
	case gHandoverAnchorMonitorEvent_c:
		shell_write("\r\n ----gHandoverAnchorMonitorEvent_c \r\n");
		break;
	case gHandoverTimeSyncEvent_c:
		shell_write("\r\n ----gHandoverTimeSyncEvent_c \r\n");
		break;
	case gHandoverConnParamUpdateEvent_c:
		shell_write("\r\n ----gHandoverConnParamUpdateEvent_c \r\n");
		break;


	case gRemoteVersionInformationRead_c:
		shell_write("\r\n ----gRemoteVersionInformationRead_c \r\n");
		break;
	case gLlSkdReportEvent_c:
		shell_write("\r\n ----gLlSkdReportEvent_c \r\n");
		break;
	case gLeSetSchedulerPriorityComplete_c:
		shell_write("\r\n ----gLeSetSchedulerPriorityComplete_c \r\n");
		break;
	case gDeInitializationComplete_c:
		shell_write("\r\n ----gDeInitializationComplete_c \r\n");
		break;
	case gHandoverAnchorMonitorPacketEvent_c:
		shell_write("\r\n ----gHandoverAnchorMonitorPacketEvent_c \r\n");
		break;
	case gHandoverAnchorMonitorPacketContinueEvent_c:
		shell_write("\r\n ----gHandoverAnchorMonitorPacketContinueEvent_c \r\n");
		break;
	case gHandoverFreeComplete_c:
		shell_write("\r\n ----gHandoverFreeComplete_c \r\n");
		break;
	case gHandoverUpdateConnParamsComplete_c:
		shell_write("\r\n ----gHandoverUpdateConnParamsComplete_c \r\n");
		break;

	default:
		shell_write("\r\n ----Host General Callback No Matching... \r\n");
		break;
	}

}

/************************************************************************************
 *  void BluetoothLEHost_Debug()
 *
 * *********************************************************************************/
static void BluetoothLEHostAdvertisementCallback_Debug(uint32_t EventType)
{
	switch (EventType)
	{
	case gAdvertisingStateChanged_c:
		shell_write("\r\n ----gAdvertisingStateChanged_c \r\n");
		break;
	case gAdvertisingCommandFailed_c:
		shell_write("\r\n ----gAdvertisingCommandFailed_c \r\n");
		break;
	case gExtAdvertisingStateChanged_c:
		shell_write("\r\n ----gExtAdvertisingStateChanged_c \r\n");
		break;
	case gAdvertisingSetTerminated_c:
		shell_write("\r\n ----gAdvertisingSetTerminated_c \r\n");
		break;
	case gExtScanNotification_c:
		shell_write("\r\n ----gExtScanNotification_c \r\n");
		break;
	default:
		shell_write("\r\n ----Advertisement No Matching... \r\n");
		break;
	}
}

/************************************************************************************
 *  void BluetoothLEHost_Debug()
 *
 * *********************************************************************************/
static void BluetoothLEHostScanCallback_Debug(uint32_t EventType)
{
	switch (EventType)
	{
	case gScanStateChanged_c:
		shell_write("\r\n ----gScanStateChanged_c \r\n");
		break;
	case gScanCommandFailed_c:
		shell_write("\r\n ----gScanCommandFailed_c \r\n");
		break;
	case gDeviceScanned_c:
		shell_write("\r\n ----gDeviceScanned_c \r\n");
		break;
	case gExtDeviceScanned_c:
		shell_write("\r\n ----gExtDeviceScanned_c \r\n");
		break;
	case gPeriodicDeviceScanned_c:
		shell_write("\r\n ----gPeriodicDeviceScanned_c \r\n");
		break;
	case gPeriodicAdvSyncEstablished_c:
		shell_write("\r\n ----gPeriodicAdvSyncEstablished_c \r\n");
		break;
	case gPeriodicAdvSyncLost_c:
		shell_write("\r\n ----gPeriodicAdvSyncLost_c \r\n");
		break;
	case gPeriodicAdvSyncTerminated_c:
		shell_write("\r\n ----gPeriodicAdvSyncTerminated_c \r\n");
		break;
	case gConnectionlessIqReportReceived_c:
		shell_write("\r\n ----gConnectionlessIqReportReceived_c \r\n");
		break;
	default:
		shell_write("\r\n ----Scan Callback No Matching... \r\n");
		break;
	}
}

/************************************************************************************
 *  void BluetoothLEHost_Debug()
 *
 * *********************************************************************************/
static void BluetoothLEHostConnectionCallback_Debug(uint32_t EventType)
{
	switch (EventType)
	{
	case gConnEvtConnected_c:
		shell_write("\r\n ----gConnEvtConnected_c \r\n");
		break;
	case gConnEvtPairingRequest_c:
		shell_write("\r\n ----gConnEvtPairingRequest_c \r\n");
		break;
	case gConnEvtPeripheralSecurityRequest_c:
		shell_write("\r\n ----gConnEvtPeripheralSecurityRequest_c \r\n");
		break;
	case gConnEvtPairingResponse_c:
		shell_write("\r\n ----gConnEvtPairingResponse_c \r\n");
		break;
	case gConnEvtAuthenticationRejected_c:
		shell_write("\r\n ----gConnEvtAuthenticationRejected_c \r\n");
		break;
	case gConnEvtPasskeyRequest_c:
		shell_write("\r\n ----gConnEvtPasskeyRequest_c \r\n");
		break;
	case gConnEvtOobRequest_c:
		shell_write("\r\n ----gConnEvtOobRequest_c \r\n");
		break;
	case gConnEvtPasskeyDisplay_c:
		shell_write("\r\n ----gConnEvtPasskeyDisplay_c \r\n");
		break;
	case gConnEvtKeyExchangeRequest_c:
		shell_write("\r\n ----gConnEvtKeyExchangeRequest_c \r\n");
		break;
	case gConnEvtKeysReceived_c:
		shell_write("\r\n ----gConnEvtKeysReceived_c \r\n");
		break;
	case gConnEvtLongTermKeyRequest_c:
		shell_write("\r\n ----gConnEvtLongTermKeyRequest_c \r\n");
		break;
	case gConnEvtEncryptionChanged_c:
		shell_write("\r\n ----gConnEvtEncryptionChanged_c \r\n");
		break;
	case gConnEvtPairingComplete_c:
		shell_write("\r\n ----gConnEvtPairingComplete_c \r\n");
		break;
	case gConnEvtDisconnected_c:
		shell_write("\r\n ----gConnEvtDisconnected_c \r\n");
		break;
	case gConnEvtRssiRead_c:
//		shell_write("\r\n ----gConnEvtRssiRead_c \r\n");
		break;
	case gConnEvtTxPowerLevelRead_c:
		shell_write("\r\n ----gConnEvtTxPowerLevelRead_c \r\n");
		break;
	case gConnEvtPowerReadFailure_c:
		shell_write("\r\n ----gConnEvtPowerReadFailure_c \r\n");
		break;
	case gConnEvtParameterUpdateRequest_c:
		shell_write("\r\n ----gConnEvtParameterUpdateRequest_c \r\n");
		break;
	case gConnEvtParameterUpdateComplete_c:
		shell_write("\r\n ----gConnEvtParameterUpdateComplete_c \r\n");
		break;
	case gConnEvtLeDataLengthChanged_c:
		shell_write("\r\n ----gConnEvtLeDataLengthChanged_c \r\n");
		break;
	case gConnEvtLeScOobDataRequest_c:
		shell_write("\r\n ----gConnEvtLeScOobDataRequest_c \r\n");
		break;
	case gConnEvtLeScDisplayNumericValue_c:
		shell_write("\r\n ----gConnEvtLeScDisplayNumericValue_c \r\n");
		break;
	case gConnEvtLeScKeypressNotification_c:
		shell_write("\r\n ----gConnEvtLeScKeypressNotification_c \r\n");
		break;
	case gConnEvtChannelMapRead_c:
		shell_write("\r\n ----gConnEvtChannelMapRead_c \r\n");
		break;
	case gConnEvtChannelMapReadFailure_c:
		shell_write("\r\n ----gConnEvtChannelMapReadFailure_c \r\n");
		break;
	case gConnEvtChanSelectionAlgorithm2_c:
		shell_write("\r\n ----gConnEvtChanSelectionAlgorithm2_c \r\n");
		break;
	case gConnEvtPairingNoLtk_c:
		shell_write("\r\n ----gConnEvtPairingNoLtk_c \r\n");
		break;
	case gConnEvtPairingAlreadyStarted_c:
		shell_write("\r\n ----gConnEvtPairingAlreadyStarted_c \r\n");
		break;
	case gConnEvtIqReportReceived_c:
		shell_write("\r\n ----gConnEvtIqReportReceived_c \r\n");
		break;
	case gConnEvtCteRequestFailed_c:
		shell_write("\r\n ----gConnEvtCteRequestFailed_c \r\n");
		break;
	case gConnEvtCteReceiveParamsSetupComplete_c:
		shell_write("\r\n ----gConnEvtCteReceiveParamsSetupComplete_c \r\n");
		break;
	case gConnEvtCteTransmitParamsSetupComplete_c:
		shell_write("\r\n ----gConnEvtCteTransmitParamsSetupComplete_c \r\n");
		break;
	case gConnEvtCteReqStateChanged_c:
		shell_write("\r\n ----gConnEvtCteReqStateChanged_c \r\n");
		break;
	case gConnEvtCteRspStateChanged_c:
		shell_write("\r\n ----gConnEvtCteRspStateChanged_c \r\n");
		break;
	case gConnEvtPathLossThreshold_c:
		shell_write("\r\n ----gConnEvtPathLossThreshold_c \r\n");
		break;
	case gConnEvtTransmitPowerReporting_c:
		shell_write("\r\n ----gConnEvtTransmitPowerReporting_c \r\n");
		break;
	case gConnEvtEnhancedReadTransmitPowerLevel_c:
		shell_write("\r\n ----gConnEvtEnhancedReadTransmitPowerLevel_c \r\n");
		break;
	case gConnEvtPathLossReportingParamsSetupComplete_c:
		shell_write("\r\n ----gConnEvtPathLossReportingParamsSetupComplete_c \r\n");
		break;
	case gConnEvtPathLossReportingStateChanged_c:
		shell_write("\r\n ----gConnEvtPathLossReportingStateChanged_c \r\n");
		break;
	case gConnEvtTransmitPowerReportingStateChanged_c:
		shell_write("\r\n ----gConnEvtTransmitPowerReportingStateChanged_c \r\n");
		break;
	case gConnEvtEattConnectionRequest_c:
		shell_write("\r\n ----gConnEvtEattConnectionRequest_c \r\n");
		break;
	case gConnEvtEattConnectionComplete_c:
		shell_write("\r\n ----gConnEvtEattConnectionComplete_c \r\n");
		break;
	case gConnEvtEattChannelReconfigureResponse_c:
		shell_write("\r\n ----gConnEvtEattChannelReconfigureResponse_c \r\n");
		break;
	case gConnEvtEattBearerStatusNotification_c:
		shell_write("\r\n ----gConnEvtEattBearerStatusNotification_c \r\n");
		break;
	case gConnEvtHandoverConnected_c:
		shell_write("\r\n ----gConnEvtHandoverConnected_c \r\n");
		break;
	case gHandoverDisconnected_c:
		shell_write("\r\n ----gHandoverDisconnected_c \r\n");
		break;
	case gConnEvtLeSetDataLengthFailure_c:
		shell_write("\r\n ----gConnEvtLeSetDataLengthFailure_c \r\n");
		break;

	default:
		shell_write("\r\n ----Connection Callback No Matching... \r\n");
		break;
	}
}

/************************************************************************************
 *  void BluetoothLEHost_Debug()
 *
 * *********************************************************************************/
static void BluetoothLEHostGattServerCallback_Debug(uint32_t EventType)
{
	switch (EventType)
	{
	case gEvtMtuChanged_c:
		shell_write("\r\n ----gEvtMtuChanged_c \r\n");
		break;
	case gEvtHandleValueConfirmation_c:
		shell_write("\r\n ----gEvtHandleValueConfirmation_c \r\n");
		break;
	case gEvtAttributeWritten_c:
		shell_write("\r\n ----gEvtAttributeWritten_c \r\n");
		break;

	case gEvtCharacteristicCccdWritten_c:
		shell_write("\r\n ----gEvtCharacteristicCccdWritten_c \r\n");
		break;
	case gEvtAttributeWrittenWithoutResponse_c:
		shell_write("\r\n ----gEvtAttributeWrittenWithoutResponse_c \r\n");
		break;

	case gEvtError_c:
		shell_write("\r\n ----gEvtError_c \r\n");
		break;
	case gEvtLongCharacteristicWritten_c:
		shell_write("\r\n ----gEvtLongCharacteristicWritten_c \r\n");
		break;
	case gEvtAttributeRead_c:
		shell_write("\r\n ----gEvtAttributeRead_c \r\n");
		break;

	case gEvtInvalidPduReceived_c:
		shell_write("\r\n ----gEvtInvalidPduReceived_c \r\n");
		break;


	default:
		shell_write("\r\n ----Gatt Server Callback No Matching... \r\n");
		break;
	}
}

/************************************************************************************
 *  void BluetoothLEHost_Debug()
 *
 * *********************************************************************************/
static void BluetoothLEHostClientProcedureMsgCallback_Debug(uint32_t EventType)
{
	switch (EventType)
	{
	case gGattProcExchangeMtu_c:
		shell_write("\r\n ----gGattProcExchangeMtu_c \r\n");
		break;
	case gGattProcDiscoverAllPrimaryServices_c:
		shell_write("\r\n ----gGattProcDiscoverAllPrimaryServices_c \r\n");
		break;
	case gGattProcDiscoverPrimaryServicesByUuid_c:
		shell_write("\r\n ----gGattProcDiscoverPrimaryServicesByUuid_c \r\n");
		break;
	case gGattProcFindIncludedServices_c:
		shell_write("\r\n ----gGattProcFindIncludedServices_c \r\n");
		break;
	case gGattProcDiscoverAllCharacteristics_c:
		shell_write("\r\n ----gGattProcDiscoverAllCharacteristics_c \r\n");
		break;
	case gGattProcDiscoverCharacteristicByUuid_c:
		shell_write("\r\n ----gGattProcDiscoverCharacteristicByUuid_c \r\n");
		break;
	case gGattProcDiscoverAllCharacteristicDescriptors_c:
		shell_write("\r\n ----gGattProcDiscoverAllCharacteristicDescriptors_c \r\n");
		break;
	case gGattProcReadCharacteristicValue_c:
		shell_write("\r\n ----gGattProcReadCharacteristicValue_c \r\n");
		break;
	case gGattProcReadUsingCharacteristicUuid_c:
		shell_write("\r\n ----gGattProcReadUsingCharacteristicUuid_c \r\n");
		break;
	case gGattProcReadMultipleCharacteristicValues_c:
		shell_write("\r\n ----gGattProcReadMultipleCharacteristicValues_c \r\n");
		break;
	case gGattProcWriteCharacteristicValue_c:
		shell_write("\r\n ----gGattProcWriteCharacteristicValue_c \r\n");
		break;
	case gGattProcReadCharacteristicDescriptor_c:
		shell_write("\r\n ----gGattProcReadCharacteristicDescriptor_c \r\n");
		break;
	case gGattProcWriteCharacteristicDescriptor_c:
		shell_write("\r\n ----gGattProcWriteCharacteristicDescriptor_c \r\n");
		break;
	case gGattProcReadMultipleVarLengthCharValues_c:
		shell_write("\r\n ----gGattProcReadMultipleVarLengthCharValues_c \r\n");
		break;

	default:
		shell_write("\r\n ----No Gatt Client procedure Callback NoMatching... \r\n");
		break;
	}
}

/************************************************************************************
 *  void BluetoothLEHost_Debug()
 *
 * *********************************************************************************/
static void BluetoothLEHostGattClientNotificationMsgCallback_Debug(uint16_t handler)
{
	shell_write("\r\n ----Handler is: %d", handler);
}

/************************************************************************************
 *  void BluetoothLEHost_Debug()
 *
 * *********************************************************************************/
static void BluetoothLEHostL2caLeDataMsgCallback_Debug(uint16_t channelId)
{
	shell_write("\r\n ----Handler is: %d", channelId);
}

/************************************************************************************
 *  void BluetoothLEHost_Debug()
 *
 * *********************************************************************************/
static void BluetoothLEHostL2caLeControlMsgCallback_Debug(uint32_t EventType)
{
	switch (EventType)
	{
	case gL2ca_LePsmConnectRequest_c:
		shell_write("\r\n ----gL2ca_LePsmConnectRequest_c \r\n");
		break;
	case gL2ca_LePsmConnectionComplete_c:
		shell_write("\r\n ----gL2ca_LePsmConnectionComplete_c \r\n");
		break;
	case gL2ca_LePsmDisconnectNotification_c:
		shell_write("\r\n ----gL2ca_LePsmDisconnectNotification_c \r\n");
		break;
	case gL2ca_NoPeerCredits_c:
		shell_write("\r\n ----gL2ca_NoPeerCredits_c \r\n");
		break;
	case gL2ca_LowPeerCredits_c:
		shell_write("\r\n ----gL2ca_LowPeerCredits_c \r\n");
		break;
	case gL2ca_LocalCreditsNotification_c:
		shell_write("\r\n ----gL2ca_LocalCreditsNotification_c \r\n");
		break;
	case gL2ca_Error_c:
		shell_write("\r\n ----gL2ca_Error_c \r\n");
		break;
	case gL2ca_ChannelStatusNotification_c:
		shell_write("\r\n ----gL2ca_ChannelStatusNotification_c \r\n");
		break;
	case gL2ca_LePsmEnhancedConnectRequest_c:
		shell_write("\r\n ----gL2ca_LePsmEnhancedConnectRequest_c \r\n");
		break;
	case gL2ca_LePsmEnhancedConnectionComplete_c:
		shell_write("\r\n ----gL2ca_LePsmEnhancedConnectionComplete_c \r\n");
		break;
	case gL2ca_EnhancedReconfigureRequest_c:
		shell_write("\r\n ----gL2ca_EnhancedReconfigureRequest_c \r\n");
		break;
	case gL2ca_EnhancedReconfigureResponse_c:
		shell_write("\r\n ----gL2ca_EnhancedReconfigureResponse_c \r\n");
		break;
	case gL2ca_HandoverConnectionComplete_c:
		shell_write("\r\n ----gL2ca_HandoverConnectionComplete_c \r\n");
		break;

	default:
		shell_write("\r\n ----L2caLe Callback No Matching... \r\n");
		break;
	}
}

