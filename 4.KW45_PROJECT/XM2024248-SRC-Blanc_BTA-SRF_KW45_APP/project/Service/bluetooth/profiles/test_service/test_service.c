///*
// * notify_info_service.c
// *
// *  Created on: 2024年4月19日
// *      Author: F704012276_
// */
//
///*------------include--------------*/
//#include <test_service_interface.h>
//#include "ble_general.h"
//#include "gatt_db_app_interface.h"
//#include "gatt_server_interface.h"
//#include "gap_interface.h"
//
//
///*--------------------------------------------------
// * private constants & macros
// * ------------------------------------------------*/
//
///*--------------------------------------------------
// * Private type definitions
// * ------------------------------------------------*/
//
///*--------------------------------------------------
// * Private memory declarations
// * ------------------------------------------------*/
//
///*--------------------------------------------------
// * Private functions prototypes
// * ------------------------------------------------*/
//static void Notify_SendNotifications(testConfig_t *pServiceConfig, uint16_t handle);
//static bleResult_t Dis_SetManufacturerName(uint16_t serviceHandle, utf8s_t manufacturerName);
//
///*--------------------------------------------------
// * Public functions
// * ------------------------------------------------*/
//static bleUuid_t uuid128_temp;
//uint8_t tempData[16] = {0x3E, 0xA2, 0x32, 0x86, 0x15, 0xDD, 0x53, 0xB2, 0x80, 0x98, 0x90, 0x2E, 0x0E, 0xD6, 0x3D, 0xDE};
////uint8_t tempData[] = {0xDE, 0x3D, 0xD6, 0x0E, 0x2E, 0x90, 0x98, 0x80, 0xB2, 0x53, 0xDD, 0x15, 0x86, 0x32, 0xA2, 0x3E};
//
///*
// * des: start the TestService
// * */
//bleResult_t TestService_Start(testConfig_t *pServiceConfig)
//{
//    uint8_t mClientId = 0U;
//    uint8_t arr[] = {'A', 'B', 'C'};
//
//    pServiceConfig->manufacturerName.stringLength = 3;
//    pServiceConfig->manufacturerName.pUtf8s = arr;
//
//    /* reset all slots for valid subscribers */
//    for (mClientId = 0; mClientId < pServiceConfig->validSubscriberListSize; mClientId++)
//    {
//        pServiceConfig->aValidSubscriberList[mClientId] = FALSE;
//    }
//
//	if(pServiceConfig->manufacturerName.stringLength != 0 )
//	{
//		Dis_SetManufacturerName(pServiceConfig->serviceHandle, pServiceConfig->manufacturerName);
//	}
//
//    /* Record initial battery level measurement */
//    return NotifyService_SendData(pServiceConfig, 2);
//}
//
///*
// * des: stop the TestService
// * */
//bleResult_t TestService_Stop(testConfig_t *pServiceConfig)
//{
//    uint8_t mClientId = 0U;
//
//    /* reset all slots for valid subscribers */
//    for (mClientId = 0; mClientId < pServiceConfig->validSubscriberListSize; mClientId++)
//    {
//        pServiceConfig->aValidSubscriberList[mClientId] = FALSE;
//    }
//
//    return gBleSuccess_c;
//}
//
///*
// * des: subscribe the service for some devices
// * */
//bleResult_t TestService_Subscribe(testConfig_t *pServiceConfig, deviceId_t clientDeviceId)
//{
//    bleResult_t result = gBleSuccess_c;
//
//    if (clientDeviceId >= pServiceConfig->validSubscriberListSize)
//    {
//        result = gBleInvalidParameter_c;
//    }
//    else
//    {
//        pServiceConfig->aValidSubscriberList[clientDeviceId] = TRUE;
//    }
//
//    return result;
//}
//
///*
// * des: Unsubscribe the Service for some devices
// * */
//bleResult_t TestService_Unsubscribe(testConfig_t *pServiceConfig, deviceId_t clientDeviceId)
//{
//    bleResult_t result = gBleSuccess_c;
//
//    if (clientDeviceId >= pServiceConfig->validSubscriberListSize)
//    {
//        result = gBleInvalidParameter_c;
//    }
//    else
//    {
//        pServiceConfig->aValidSubscriberList[clientDeviceId] = FALSE;
//    }
//
//    return result;
//}
//
///*
// * des: use the function to send data to the master
// * called by: generally could be called by any function
// * */
//bleResult_t NotifyService_SendData(testConfig_t *pServiceConfig, uint16_t len)
//{
//    uint16_t  handle = gGattDbInvalidHandle_d;
//    bleResult_t result = gBleSuccess_c;
//
//    memcpy(uuid128_temp.uuid128, tempData, 16);
//    /* Get handle of  characteristic */
//    result = GattDb_FindCharValueHandleInService(pServiceConfig->serviceHandle,
//    		gBleUuidType128_c, &uuid128_temp, &handle);
//
//    if (result == gBleSuccess_c)
//    {
//        /* Update characteristic value and send notification */
//        result = GattDb_WriteAttribute(handle, (uint16_t)len, &(pServiceConfig->NotifyData[0]));
//
//        if (result == gBleSuccess_c)
//        {
//        	Notify_SendNotifications(pServiceConfig, handle);
//        }
//    }
//
//    return result;
//}
//
///*
// * des: slave use the Notification to send data to the Master
// * called by: called by  NotifyService_SendData()
// * */
//static void Notify_SendNotifications
//(
//	testConfig_t *pServiceConfig,
//    uint16_t handle
//)
//{
//    uint16_t  handleCccd = gGattDbInvalidHandle_d;
//    bool_t    isNotifActive = FALSE;
//    uint8_t   mClientId = 0U;
//
//    /* Get handle of CCCD */
//    if (GattDb_FindCccdHandleForCharValueHandle(handle, &handleCccd) == gBleSuccess_c)
//    {
//        for (mClientId = 0U; mClientId < pServiceConfig->validSubscriberListSize; mClientId++)
//        {
//            if (pServiceConfig->aValidSubscriberList[mClientId])
//            {
//                if (gBleSuccess_c == Gap_CheckNotificationStatus
//                    (mClientId, handleCccd, &isNotifActive) &&
//                    TRUE == isNotifActive)
//                {
//                    (void)GattServer_SendNotification(mClientId, handle);
//                }
//            }
//        }
//    }
//}
//
///*--------------------------------------------------
// * Private functions
// * ------------------------------------------------*/
//static bleResult_t Dis_SetManufacturerName(uint16_t serviceHandle, utf8s_t manufacturerName)
//{
//    uint16_t  handle = gGattDbInvalidHandle_d;
//    bleResult_t result = gBleSuccess_c;
//    bleUuid_t uuid = Uuid16(gBleSig_ManufacturerNameString_d);
//
//    /* Get handle of characteristic */
//    result = GattDb_FindCharValueHandleInService(serviceHandle,
//             gBleUuidType16_c, &uuid, &handle);
//
//    if (result == gBleSuccess_c)
//    {
//        /* Update characteristic value */
//        result = GattDb_WriteAttribute(handle, manufacturerName.stringLength, (void *)manufacturerName.pUtf8s);
//    }
//
//    return result;
//}
