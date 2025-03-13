///*
// * notify_info_interface.h
// *
// *  Created on: 2024年4月19日
// *      Author: F704012276_
// */
//
//#ifndef PROFILES_TEST_NOTIFY_WRITE_NOTIFY_INTERFACE_H_
//#define PROFILES_TEST_NOTIFY_WRITE_NOTIFY_INTERFACE_H_
//
//#include "ble_general.h"
///*--------------------------------------------------
// * Include
// * ------------------------------------------------*/
//
///*--------------------------------------------------
// * Public constants & macros
// * ------------------------------------------------*/
//
///*--------------------------------------------------
// * Public type definitions
// * ------------------------------------------------*/
//
//typedef struct utf8s_tag
//{
//    uint16_t    stringLength;
//    char        *pUtf8s;
//}utf8s_t;
//
///*! test config Service - Configuration */
//typedef struct testConfig_tag
//{
//	uint8_t 	deviceId;
//    uint16_t    serviceHandle;
//    utf8s_t     manufacturerName;
//    uint8_t	    NotifyData[1500];
//    bool_t*     aValidSubscriberList;
//    uint8_t     validSubscriberListSize;
//} testConfig_t;
//
//bleResult_t TestService_Start(testConfig_t *pServiceConfig);
//bleResult_t TestService_Stop(testConfig_t *pServiceConfig);
//bleResult_t TestService_Subscribe(testConfig_t *pServiceConfig, deviceId_t clientDeviceId);
//bleResult_t TestService_Unsubscribe(testConfig_t *pServiceConfig, deviceId_t clientDeviceId);
//bleResult_t NotifyService_SendData(testConfig_t *pServiceConfig, uint16_t len);
//
//
//
//#endif /* PROFILES_TEST_NOTIFY_WRITE_NOTIFY_INTERFACE_H_ */
