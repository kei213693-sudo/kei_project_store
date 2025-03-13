/*
 * bleManagerGATT.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "bleManagerGATT_cfg.h"
#include "app_api.h"

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
	bleMsgInfo_t bleMsgInfo;
	uint16_t handle;

    switch (pServerEvent->eventType)
    {
        case gEvtMtuChanged_c:
        {
        	shell_write("\r\ngEvtMtuChanged_c:%d\r\n",pServerEvent->eventData.mtuChangedEvent);
        }
        break;

        case gEvtHandleValueConfirmation_c:
        {
        	shell_write("\r\ngEvtHandleValueConfirmation_c\r\n.");
        }
        break;

        case gEvtAttributeWritten_c:
            //返回响应
            (void)GattServer_SendAttributeWrittenStatus(0, (uint16_t)value_SK_TX, (uint8_t)gAttErrCodeNoError_c);
        case gEvtAttributeWrittenWithoutResponse_c:
        {//收到了master发送的数据
            dbg_Printf("Ble received(type:0x%x) Air Message hex[%d]:",pServerEvent->eventType,pServerEvent->eventData.attributeWrittenEvent.cValueLength);
		    dbg_Printf_hex(pServerEvent->eventData.attributeWrittenEvent.aValue,pServerEvent->eventData.attributeWrittenEvent.cValueLength);
			handle = pServerEvent->eventData.attributeWrittenEvent.handle;
			(void)GattServer_SendAttributeWrittenStatus(deviceId, handle, (uint8_t)gAttErrCodeNoError_c);

            //处理逻辑
            nio_ble_message_protocol_t stMessage = {0};
            stMessage.u8Type = pServerEvent->eventData.attributeWrittenEvent.aValue[0];
            stMessage.u8ID = pServerEvent->eventData.attributeWrittenEvent.aValue[1];
            stMessage.u16Length = ((uint16_t)pServerEvent->eventData.attributeWrittenEvent.aValue[2] << 8) | ((uint16_t)pServerEvent->eventData.attributeWrittenEvent.aValue[3]);
            if((stMessage.u16Length + 4) > pServerEvent->eventData.attributeWrittenEvent.cValueLength)
            {
                dbg_Printf("len err:%d   %d:\r\n",stMessage.u16Length,pServerEvent->eventData.attributeWrittenEvent.cValueLength);
            }
            else
            {
                stMessage.pData = &pServerEvent->eventData.attributeWrittenEvent.aValue[4];
                dbg_Printf("NIO ble Message\r\nMType:0x%02x Mid:0x%02x\r\nPayload Length:%d bytes\r\nPayload[%d]:",stMessage.u8Type,stMessage.u8ID,stMessage.u16Length,stMessage.u16Length);
		        dbg_Printf_hex(stMessage.pData,stMessage.u16Length);

                if((0x01 == stMessage.u8Type) && (0x01 == stMessage.u8ID))
                {
                    uint8_t deviceId = 0;//一个角色默认为0
                    uint16_t valuehandle = (uint16_t)value_SK_RX;
                    uint16_t u16Length = 0;
                    bleResult_t stRet = 0;

                    memset(g_u8MessageData,0x00,sizeof(g_u8MessageData));
                    g_pStMessage = (nio_ble_message_protocol_t *)g_u8MessageData;
                    g_pStMessage->u8Type = 0x01;

                    if(0 == g_nio_ble_auth_flag)
                    {//返回成功
                        g_pStMessage->u8ID = 0x02;
                        u16Length = 42;//0x002A;
                        for(uint8_t i = 0; i < 42; i++)
                        {
                            g_u8MessageData[i + 4] = (i + 1);//从1~42赋值
                        }
                    }
                    else
                    {//返回失败
                        g_pStMessage->u8ID = 0x04;
			            u16Length = 1;
                        g_u8MessageData[4] = 0x01;
                    }

                    g_u8MessageData[2] = (uint8_t)(u16Length >> 8);
                    g_u8MessageData[3] = (uint8_t)(u16Length & 0x00ff);
                    //notify发送给SRF
                    stRet = GattServer_SendInstantValueNotification(deviceId,valuehandle,(u16Length + 4),(const uint8_t *)g_u8MessageData);
                    dbg_Printf("notify ret:0x%x\r\n",stRet);
                }
            }
        }
        break;

        case gEvtCharacteristicCccdWritten_c:
        {
        	shell_write("\r\ngEvtCharacteristicCccdWritten_c\r\n.");
        	bleCCCDFlag = !bleCCCDFlag;
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
            (void)Gap_Pair(serverDeviceId, &gPairingParameters);
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
                	bleMasterCCCDFlag = 1;
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
