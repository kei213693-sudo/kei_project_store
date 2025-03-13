/*
 * bleManagerGATT.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "bleManagerGATT_cfg.h"
#include "signals_api.h"

static gattAttribute_t      *mpCharProcBuffer = NULL;

static void BleApp_StoreServiceHandles
(
    deviceId_t      peerDeviceId,
    gattService_t   *pService
);

/*! *********************************************************************************
* \brief        Handles GATT server callback from host stack.
*
* \param[in]    deviceId        Peer device ID.
* \param[in]    pServerEvent    Pointer to gattServerEvent_t.
********************************************************************************** */
void BleApp_GattServerCallback (deviceId_t deviceId, gattServerEvent_t* pServerEvent)
{
	uint16_t handle;
	static bleMsgRevInfo_t bleMsgRxInfo;

    switch (pServerEvent->eventType)
    {
        case gEvtMtuChanged_c:
        {
        	shell_write("\r\ngEvtMtuChanged_c\r\n.");
        }
        break;

        case gEvtHandleValueConfirmation_c:
        {
        	shell_write("\r\ngEvtHandleValueConfirmation_c\r\n.");
        }
        break;

        case gEvtAttributeWritten_c:
        {
        	/* Receive message from Phone */
        	shell_write("\r\n BLE Received Air Message! \r\n.");
			shell_write("\r\n SRF Receive data: %d", pServerEvent->eventData.attributeWrittenEvent.aValue[0]);
			shell_write("\r\n SRF Receive data length: %d", pServerEvent->eventData.attributeWrittenEvent.cValueLength);

			handle = pServerEvent->eventData.attributeWrittenEvent.handle;
			bleMsgRxInfo.source = messageSource_air_c;
			bleMsgRxInfo.len = pServerEvent->eventData.attributeWrittenEvent.cValueLength;
			bleMsgRxInfo.data = pServerEvent->eventData.attributeWrittenEvent.aValue;
			bleMsgRxInfo.deviceId = deviceId;

			BleIf_Receive(&bleMsgRxInfo);
			(void)GattServer_SendAttributeWrittenStatus(deviceId, handle, (uint8_t)gAttErrCodeNoError_c);
        }
        break;

        case gEvtCharacteristicCccdWritten_c:
        {
        	maPeerInformation[deviceId].bleChanflags = BLE_DEVICE_CHANNEL_ENABLE;
        	shell_write("\r\ngEvtCharacteristicCccdWritten_c\r\n.");
        }
        break;

        case gEvtAttributeWrittenWithoutResponse_c:
        {
        	shell_write("\r\ngEvtAttributeWrittenWithoutResponse_c\r\n.");
			handle = pServerEvent->eventData.attributeWrittenEvent.handle;
			(void)GattServer_SendAttributeWrittenStatus(deviceId, handle, (uint8_t)gAttErrCodeNoError_c);
        }
        break;

        case gEvtError_c:
        {
        	shell_write("\r\ngEvtError_c\r\n.");
        }
        break;

        case gEvtLongCharacteristicWritten_c:
        {
        	shell_write("\r\ngEvtLongCharacteristicWritten_c\r\n.");
        }
        break;

        case gEvtAttributeRead_c:
        {
        	shell_write("\r\ngEvtAttributeRead_c\r\n.");
			handle = pServerEvent->eventData.attributeWrittenEvent.handle;
			(void)GattServer_SendAttributeReadStatus(deviceId, handle, (uint8_t)gAttErrCodeNoError_c);
        }
        break;

        case gEvtInvalidPduReceived_c:
        {
        	shell_write("\r\ngEvtInvalidPduReceived_c\r\n.");
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Handles GATT client callback from host stack.
*
* \param[in]    serverDeviceId      GATT Server device ID.
* \param[in]    procedureType       Procedure type.
* \param[in]    procedureResult     Procedure result.
* \param[in]    error               Callback result.
********************************************************************************** */
void BleApp_GattClientCallback(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
)
{
    if (procedureResult == gGattProcError_c)
    {
        attErrorCode_t attError = (attErrorCode_t)(uint8_t)(error);

        if (attError == gAttErrCodeInsufficientEncryption_c     ||
            attError == gAttErrCodeInsufficientAuthorization_c  ||
            attError == gAttErrCodeInsufficientAuthentication_c)
        {
            /* Start Pairing Procedure */
            (void)Gap_Pair(serverDeviceId, &gKeyFobPairingParameters);
        }

        BleApp_StateMachineHandler(serverDeviceId, mAppEvt_GattProcError_c);
    }
    else
    {
        if (procedureResult == gGattProcSuccess_c)
        {
            appEvent_t appEvt = mAppEvt_GattProcComplete_c;

            switch(procedureType)
            {

                case gGattProcReadCharacteristicValue_c:
                {
                    appEvt = mAppEvt_ReadCharacteristicValueComplete_c;
                }
                break;

                case gGattProcWriteCharacteristicDescriptor_c:
                {
                	/*TODO Release message buffer */
                }
                break;

                default:
                {
                    ; /* No action required */
                }
                break;
            }

            BleApp_StateMachineHandler(serverDeviceId, appEvt);
        }
    }

    /* Signal Service Discovery Module */
    BleServDisc_SignalGattClientEvent(serverDeviceId, procedureType, procedureResult, error);
}

/*! *********************************************************************************
* \brief        Handles discovered services.
*
* \param[in]    peerDeviceId        Peer device ID.
* \param[in]    pEvent              Pointer to servDiscEvent_t.
********************************************************************************** */
void BleApp_ServiceDiscoveryCallback(deviceId_t peerDeviceId, servDiscEvent_t* pEvent)
{
    switch(pEvent->eventType)
    {
        /* Store the discovered handles for later use. */
        case gServiceDiscovered_c:
        {
        	BleApp_StoreServiceHandles(peerDeviceId, pEvent->eventData.pService);
        }
        break;

        /* Service discovery has finished, run the state machine. */
        case gDiscoveryFinished_c:
        {
            if (pEvent->eventData.success)
            {
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_ServiceDiscoveryComplete_c);
            }
            else
            {
                BleApp_StateMachineHandler(peerDeviceId, mAppEvt_ServiceDiscoveryFailed_c);
            }
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief        Enable temperature notifications by writing peer's CCCD of the
                Temperature characteristic.
*
* \return       gBleSuccess_c or gBleOutOfMemory_c
********************************************************************************** */
bleResult_t BleApp_ConfigureNotifications
(
    deviceId_t      peerDeviceId
)
{
    bleResult_t result = gBleSuccess_c;
    uint16_t value = (uint16_t)gCccdNotification_c;

    /* Allocate buffer for the write operation */
    if( mpCharProcBuffer == NULL )
    {
        mpCharProcBuffer = MEM_BufferAlloc(sizeof(gattAttribute_t) + gAttDefaultMtu_c);
    }

    if( mpCharProcBuffer != NULL )
    {
    	/*TODO cyl config descriptor*/
        /* Populate the write request */
        mpCharProcBuffer->handle = (uint16_t)cccd_SK_RX/*maPeerInformation[peerDeviceId].customInfo.clientCfg.cccdHandle*/;
        /*memcpy(mpCharProcBuffer->uuid.uuid128, tempData, 16);*/
        mpCharProcBuffer->valueLength = 0;
        mpCharProcBuffer->uuidType = 0x01;
        (void)GattClient_WriteCharacteristicDescriptor(peerDeviceId,
                                                       mpCharProcBuffer,
                                                       (uint16_t)sizeof(value), (void*)&value);
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}

/*! *********************************************************************************
* \brief        Handles GATT client notification callback from host stack.
*
* \param[in]    serverDeviceId              GATT Server device ID.
* \param[in]    characteristicValueHandle   Handle.
* \param[in]    aValue                      Pointer to value.
* \param[in]    valueLength                 Value length.
********************************************************************************** */
void BleApp_GattNotificationCallback
(
deviceId_t  serverDeviceId,
uint16_t    characteristicValueHandle,
uint8_t*    aValue,
uint16_t    valueLength
)
{
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
    if (maPeerInformation[serverDeviceId].isBonded)
    {
#endif
    	shell_write("Master received notify message!\r\n");
        if (characteristicValueHandle == (uint16_t)value_SK_RX/*maPeerInformation[serverDeviceId].customInfo.clientCfg.valueHandle*/)
        {
        	/* Set Event when message was received from KeyFob */
			if(mpfBleAppMessageEventHandler != NULL)
			{
				 appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(appMsgBleAirData_t) + (uint32_t)valueLength);
				 if(pEventData != NULL)
				 {
					 pEventData->appEvent = mAppEvt_BleMsgRev_c;
					 pEventData->eventData.pData = &pEventData[1];
					 appMsgBleAirData_t* appMsgBleAirData = pEventData->eventData.pData;
					 appMsgBleAirData->deviceId = serverDeviceId;
					 appMsgBleAirData->packetLength = valueLength;
					 appMsgBleAirData->pPacket = aValue;
					 FLib_MemCpy(appMsgBleAirData->pPacket, aValue, valueLength);
					 appMsgBleAirData = NULL;
					 if (gBleSuccess_c != BleApp_MessageCallback(mpfBleAppEventHandler, appMsgBleAirData))
					 {
						 (void)MEM_BufferFree(pEventData);
					 }
				 }
			}
        }
#if defined(gAppUseBonding_d) && (gAppUseBonding_d)
    }
#endif
}

/*! *********************************************************************************
* \brief        Stores handles for the specified service.
*
* \param[in]    peerDeviceId   Peer device ID.
* \param[in]    pService       Pointer to gattService_t.
********************************************************************************** */
static void BleApp_StoreServiceHandles
(
    deviceId_t      peerDeviceId,
    gattService_t   *pService
)
{
    if (pService->uuidType == gBleUuidType128_c)
    {
        /* Found Temperature Service */
    	if(pService->aCharacteristics->aDescriptors->handle == (uint16_t)cccd_SK_RX)
    	{
    		maPeerInformation[peerDeviceId].customInfo.clientCfg.cccdUuid16 = pService->aCharacteristics->aDescriptors->uuid.uuid16;
    		maPeerInformation[peerDeviceId].customInfo.clientCfg.cccdHandle = (uint16_t)cccd_SK_RX;
    		maPeerInformation[peerDeviceId].customInfo.clientCfg.valueHandle = pService->aCharacteristics->value.handle;
    		maPeerInformation[peerDeviceId].customInfo.clientCfg.valueUuid16 = pService->aCharacteristics->value.uuid.uuid16;
    	}
    }
}
