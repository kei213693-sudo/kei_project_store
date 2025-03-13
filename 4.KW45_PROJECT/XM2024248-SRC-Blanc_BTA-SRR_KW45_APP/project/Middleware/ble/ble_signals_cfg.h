/*
 * ble_signals_cfg.h
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#ifndef SIGNALS_BLE_SIGNALS_CFG_H_
#define SIGNALS_BLE_SIGNALS_CFG_H_

/*! *********************************************************************************
 * \defgroup Digital Key Car Anchor
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* \file digital_key_car_anchor.h
*
* Copyright 2020, 2022-2023 NXP
*
* NXP Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from NXP.
********************************************************************************** */

#ifndef DIGITAL_KEY_CAR_ANCHOR_H
#define DIGITAL_KEY_CAR_ANCHOR_H

#include "fsl_component_timer_manager.h"

#include "gatt_types.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "EmbeddedTypes.h"
#include "ble_general.h"
#include "gap_types.h"
#include "gap_handover_types.h"
#include "fsl_component_messaging.h"
#include "fsl_os_abstraction.h"
#include "fsl_component_mem_manager.h"
#include <test_service_interface.h>
#include "gatt_db_handles.h"
#include "app_preinclude.h"


/* Profile / Services */
#include "digital_key_interface.h"

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef void* appCallbackParam_t;
typedef void (*appCallbackHandler_t)(appCallbackParam_t param);
typedef void (*gapConnManagerHandler_t)
(
    deviceId_t            peerDeviceId,
    gapConnectionEvent_t* pConnectionEvent
);
typedef void (*appAdvertiserHandler_t)(gapGenericEventType_t  eventType);

typedef struct appMsgCallback_tag
{
    appCallbackHandler_t   handler;
    appCallbackParam_t     param;
} appMsgCallback_t;


/* Host to Application Messages Types */
typedef enum {
    gAppGapGenericMsg_c = 0,
    gAppGapConnectionMsg_c,
    gAppGapAdvertisementMsg_c,
    gAppGapScanMsg_c,
    gAppGattServerMsg_c,
    gAppGattClientProcedureMsg_c,
    gAppGattClientNotificationMsg_c,
    gAppGattClientIndicationMsg_c,
    gAppL2caLeDataMsg_c,
    gAppL2caLeControlMsg_c,
    gAppSecLibMultiplyMsg_c,
} appHostMsgType_t;

typedef struct appScanningParams_tag
{
    gapScanningParameters_t *pHostScanParams;         /*!< Pointer to host scan structure */
    gapFilterDuplicates_t enableDuplicateFiltering;   /*!< Duplicate filtering mode */
    uint16_t duration;                                /*!< scan duration  */
    uint16_t period;                                  /*!< scan period  */
} appScanningParams_t;

typedef struct appAdvertisingParams_tag
{
    gapAdvertisingParameters_t *pGapAdvParams;        /*!< Pointer to the GAP advertising parameters */
    const gapAdvertisingData_t *pGapAdvData;          /*!< Pointer to the GAP advertising data  */
    const gapScanResponseData_t *pScanResponseData;   /*!< Pointer to the scan response data */
} appAdvertisingParams_t;

typedef struct appExtAdvertisingParams_tag
{
    gapExtAdvertisingParameters_t *pGapExtAdvParams;  /*!< Pointer to the GAP extended advertising parameters */
    gapAdvertisingData_t *pGapAdvData;                /*!< Pointer to the GAP advertising data  */
    gapScanResponseData_t *pScanResponseData;         /*!< Pointer to the scan response data */
    uint8_t                     handle;
    uint16_t                    duration;
    uint8_t                     maxExtAdvEvents;
} appExtAdvertisingParams_t;

/* Host to Application GATT Server Message */
typedef struct gattServerMsg_tag
{
    deviceId_t          deviceId;
    gattServerEvent_t   serverEvent;
} gattServerMsg_t;

/* Host to Application GATT Client Procedure Message */
typedef struct gattClientProcMsg_tag
{
    deviceId_t              deviceId;
    gattProcedureType_t     procedureType;
    gattProcedureResult_t   procedureResult;
    bleResult_t             error;
} gattClientProcMsg_t;

/* Host to Application GATT Client Notification/Indication Message */
typedef struct gattClientNotifIndMsg_tag
{
    uint8_t*    aValue;
    uint16_t    characteristicValueHandle;
    uint16_t    valueLength;
    deviceId_t  deviceId;
} gattClientNotifIndMsg_t;

/* L2ca to Application Data Message */
typedef struct l2caLeCbDataMsg_tag
{
    deviceId_t  deviceId;
    uint16_t    channelId;
    uint16_t    packetLength;
    uint8_t     aPacket[1];
} l2caLeCbDataMsg_t;

/* Host to Application Connection Message */
typedef struct connectionMsg_tag
{
    deviceId_t              deviceId;
    gapConnectionEvent_t    connEvent;
} connectionMsg_t;

/* Sec Lib to Application Data Message */
typedef struct secLibMsgData_tag
{
    computeDhKeyParam_t *pData;
} secLibMsgData_t;

typedef struct appMsgFromHost_tag
{
    uint32_t    msgType;
    union {
        gapGenericEvent_t       genericMsg;
        gapAdvertisingEvent_t   advMsg;
        connectionMsg_t         connMsg;
        gapScanningEvent_t      scanMsg;
        gattServerMsg_t         gattServerMsg;
        gattClientProcMsg_t     gattClientProcMsg;
        gattClientNotifIndMsg_t gattClientNotifIndMsg;
        l2caLeCbDataMsg_t       l2caLeCbDataMsg;
        l2capControlMessage_t   l2caLeCbControlMsg;
        secLibMsgData_t         secLibMsgData;
    } msgData;
} appMsgFromHost_t;

/*! Callback for notifying application upon Bluetooth LE stack initialization */
typedef void (*appBluetoothLEInitCompleteCallback_t)(void);

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/*! Enable/disable use NV flash procedures for operations triggered by the host stack
    Do not modify directly. Redefine it in the app_preinclude.h file*/
#ifndef gAppUseNvm_d
#define gAppUseNvm_d                    (FALSE)
#endif /* gAppUseNvm_d */

/* Application Events */
#define gAppEvtMsgFromHostStack_c       (1U << 0U)
#define gAppEvtAppCallback_c            (1U << 1U)

#define gAppEvtService_c				(1U << 0U)
#define gAppEvtMsgService_c				(1U << 1U)

#if (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U))
#define App_NvmWriteLocalIRK(pLocalKey)   App_NvmWriteLocalKeys(0U, pLocalKey)
#define App_NvmWriteLocalCSRK(pLocalKey)  App_NvmWriteLocalKeys(1U, pLocalKey)
#define App_NvmReadLocalIRK(pLocalKey)    App_NvmReadLocalKeys(0U, pLocalKey)
#define App_NvmReadLocalCSRK(pLocalKey)   App_NvmReadLocalKeys(1U, pLocalKey)
#endif /* (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U)) */


/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/* Profile Parameters */
#define gReducedPowerMinAdvInterval_c   1600 /* 1 s */
#define gReducedPowerMaxAdvInterval_c   4000 /* 2.5 s */

#define gAdvTime_c                      30 /* 30 s*/
#define gGoToSleepAfterDataTime_c       5 /* 5 s*/

#ifndef gAppDeepSleepMode_c
#define gAppDeepSleepMode_c 1
#endif
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum messageSource_tag{
	messageSource_air_c,
	messageSource_can_c,
	messageDestination_air_c,
	messageDestination_can_c
}messageSource_t;

typedef enum appHandoverError_tag {
    mAppHandover_NoActiveConnection_c,
    mAppHandover_TimeSyncTx_c,
    mAppHandover_AnchorSearchStartFailed_c,
    mAppHandover_PeerBondingDataInvalid_c,
    mAppHandover_UnexpectedError_c,
    mAppHandover_AnchorSearchFailedToSync_c,
    mAppHandover_OutOfMemory_c,
} appHandoverError_t;

typedef enum appA2BError_tag {
    mAppA2B_E2EKeyDerivationFailiure_c,
    mAppA2B_E2ELocalIrkSyncFailiure_c,
    mAppA2B_UnexpectedError_c,
} appA2BError_t;

typedef enum appEvent_tag{
    mAppEvt_ConfigComplete_c,
    mAppEvt_GenericCallback_PeerDisconnected_c,
    mAppEvt_GenericCallback_LePhyEvent_c,
    mAppEvt_GenericCallback_LeScLocalOobData_c,
    mAppEvt_GenericCallback_BondCreatedEvent_c,
    mAppEvt_AdvertisingCallback_ExtAdvertisingStateChanged_c,
    mAppEvt_ConnectionCallback_EvtLeDataLengthChanged_c,
    mAppEvt_L2capPsmDataCallback_c,
    mAppEvt_ScanningCallback_ExtDeviceScanned_c,
    mAppEvt_ScanningCallback_ScanStateChanged_c,
    mAppEvt_Shell_Reset_Command_c,
    mAppEvt_Shell_FactoryReset_Command_c,
    mAppEvt_Shell_ShellStartDiscoveryOP_Command_c,
    mAppEvt_Shell_ShellStartDiscoveryPE_Command_c,
    mAppEvt_Shell_StopDiscovery_Command_c,
    mAppEvt_Shell_Disconnect_Command_c,
    mAppEvt_Shell_TriggerTimeSync_Command_c,
    mAppEvt_Shell_SetBondingData_Command_c,
    mAppEvt_Shell_ListBondedDev_Command_c,
    mAppEvt_Shell_RemoveBondedDev_Command_c,
    mAppEvt_AdvertisingStartedLegacy_c,
    mAppEvt_AdvertisingStartedExtendedLR_c,
    mAppEvt_AdvertisingStopped_c,
    mAppEvt_BleConfigDone_c,
    mAppEvt_BleScanning_c,
    mAppEvt_BleScanStopped_c,
    mAppEvt_BleConnectingToDevice_c,
    mAppEvt_SPAKERequestSent_c,
    mAppEvt_SPAKEVerifySent_c,
    mAppEvt_LePhyEvent_c,
    mAppEvt_PeerConnected_c,
    mAppEvt_PeerDisconnected_c,
    mAppEvt_EncryptionChanged_c,
    mAppEvt_PairingComplete_c,
    mAppEvt_PairingReqRcv_c,
    mAppEvt_PairingLocalOobData_c,
    mAppEvt_PairingPeerOobDataReq_c,
    mAppEvt_PairingPeerOobDataRcv_c,
    mAppEvt_ServiceDiscoveryComplete_c,
    mAppEvt_ServiceDiscoveryFailed_c,
    mAppEvt_GattProcComplete_c,
    mAppEvt_GattProcError_c,
    mAppEvt_ReadCharacteristicValueComplete_c,
    mAppEvt_PsmChannelCreated_c,
    mAppEvt_OwnerPairingRequestReceived_c,
    mAppEvt_ReceivedSPAKEResponse_c,
    mAppEvt_BlePairingReady_c,
    mAppEvt_FactoryReset_c,

	mAppEvt_AuthenStart_c,
	mAppEvt_Authening_c,
	mAppEvt_AuthenPrefailed_c,
	mAppEvt_AuthenFail_c,
	mAppEvt_AuthenPass_c,

	mAppEvt_CanMsgRev_c,
	mAppEvt_CanMsgTransmit_c,
	mAppEvt_BleMsgRev_c,
	mAppEvt_BleMsgTransmit_c,

	mAppEvt_TimeOut_c,

#if defined(gHandoverDemo_d) && (gHandoverDemo_d == 1)
    mAppEvt_Shell_HandoverTimeSync_Command_c,
    mAppEvt_Shell_Handover_Command_c,
    mAppEvt_Shell_HandoverSendL2cap_Command_c,
    mAppEvt_Shell_HandoverStartAnchorMonitor_Command_c,
    mAppEvt_Shell_HandoverStopAnchorMonitor_Command_c,
    mAppEvt_Shell_HandoverStarted_c,
    mAppEvt_Shell_AnchorMonitorEventReceived_c,
    mAppEvt_Shell_PacketMonitorEventReceived_c,
    mAppEvt_Shell_PacketMonitorContinueEventReceived_c,
    mAppEvt_Shell_HandoverCompleteConnected_c,
    mAppEvt_Shell_HandoverCompleteDisconnected_c,
    mAppEvt_Shell_HandoverError_c,
#endif /* defined(gHandoverDemo_d) && (gHandoverDemo_d == 1) */
#if defined(gA2BEnabled_d) && (gA2BEnabled_d == 1)
    mAppEvt_Shell_A2BKeyDerivationComplete_c,
    mAppEvt_Shell_A2BLocalIrkSyncComplete_c,
    mAppEvt_Shell_A2BError_c,
#endif /* defined(gA2BEnabled_d) && (gA2BEnabled_d == 1) */

}appEvent_t;

typedef struct appEventL2capPsmData_tag
{
    deviceId_t     deviceId;
    uint16_t       lePsm;
    uint16_t       packetLength;
    uint8_t*       pPacket;
}appEventL2capPsmData_t;

typedef struct appMsgBleAirData_tag
{
    deviceId_t     deviceId;
    uint16_t       packetLength;
    uint8_t*       pPacket;
}appMsgBleAirData_t;

typedef struct appMsgBleCanData_tag
{
    deviceId_t     deviceId;
/*    uint16_t       packetLength;
    uint8_t*       pPacket;*/
}appMsgBleCanData_t;

typedef struct appEventLeDataLengthChanged_tag
{
    deviceId_t peerDeviceId;
    gapConnLeDataLengthChanged_t pLeDataLengthChangedEvent;
}appEventLeDataLengthChanged_t;

typedef struct appConnectionCallbackEventData_tag
{
    deviceId_t peerDeviceId;
    gapConnectedEvent_t pConnectedEvent;
}appConnectionCallbackEventData_t;

typedef struct appEventData_tag
{
    appEvent_t   appEvent;              /*!< Event type. */
    union {
        void*                                       pData;
        deviceId_t                                  peerDeviceId;
        appHandoverError_t                          handoverError;
        appA2BError_t                               a2bError;
        handoverAnchorMonitorEvent_t                anchorMonitorEvent;
        handoverAnchorMonitorPacketEvent_t          anchorPacketEvent;
        handoverAnchorMonitorPacketContinueEvent_t  anchorPacketContinueEvent;
        bool_t                                      handoverTimeSync;
        bleHandoverAnchorSearchMode_t               monitorMode;
    } eventData;                        /*!< Event data, selected according to event type. */
}appEventData_t;

typedef enum appStage_tag
{
	pAppUnInitialized_c,
	pAppIdle_c,
	pAppPreWork_c,
	pAppWaitWork_c,
	pAppAdvScan_c,
	pAppConnecting_c,
	pAppConnected_c,
	pAppDisconnected_c,
	pAppPairing_c,
	pAppPaired_c,
	pAppPairFailure_c,
	pAppAuthening_c,
	pAppAuthenedPassed_c,
	pAppAuthenedPreFailure_c,
	pAppAuthenFailure_c,
	pAppInstructionComplete_c,
}appStage_t;

/*! Temperature Client - Configuration */
typedef struct clientCfg_tag
{
	uint16_t	cccdUuid16;
	uint16_t	cccdHandle;

	uint16_t 	valueUuid16;
	uint16_t 	valueHandle;
} clientCfg_t;

typedef struct appCustomInfo_tag
{
    uint16_t     	psmChannelId;
    clientCfg_t		clientCfg;
    /* Add persistent information here */
}appCustomInfo_t;

typedef enum appState_tag{
    mAppIdle_c,
    mAppExchangeMtu_c,
    mAppServiceDisc_c,
    mAppCCCWaitingForOwnerPairingRequest_c,
    mAppCCCPhase2WaitingForResponse_c, /* certificate exchange */
    mAppCCCPhase2WaitingForVerify_c,
    mAppCCCReadyForPairing_c,
    mAppPair_c,
    mAppRunning_c
}appState_t;

typedef struct appPeerInfo_tag
{
    deviceId_t          deviceId;
    gapRole_t           gapRole;
    appCustomInfo_t     customInfo;
    bool_t              isBonded;
    bool_t              isLinkEncrypted;
    appState_t          appState;
    appStage_t			appStage;
    uint8_t				retryCnt;
    uint8_t				authenFailCnt;
    gapLeScOobData_t    oobData;
    gapLeScOobData_t    peerOobData;
    uint8_t				maPeerDeviceAddressType;
    uint8_t				maPeerDeviceAddress_IRK[16];
}appPeerInfo_t;

typedef struct{
    deviceId_t deviceId;
    uint8_t destination;
    uint8_t source;
    uint16_t canId;
    uint16_t	handle;
    uint16_t valueHandle;
    uint8_t* pPacket;
    uint16_t packetLength;	/* packet length = MID + MType + payload length + payload*/
}bleMsgInfo_t;

/* APP -  pointer to function for BLE events*/
typedef void (*pfBleCallback_t)(void* pData);
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern gapAdvertisingData_t             gAppAdvertisingData;
extern gapAdvertisingData_t             gAppAdvertisingDataNio;
extern gapAdvertisingData_t             gAppAdvertisingData_IBeacon;
extern gapAdvertisingData_t             gAppAdvertisingDataEmpty;
extern gapScanResponseData_t            gAppScanRspData;
extern gapScanResponseData_t            gAppScanRspDataNio;
extern gapExtAdvertisingParameters_t    gExtAdvParams;
extern gapExtAdvertisingParameters_t    gLegacyAdvParams;
extern gapExtAdvertisingParameters_t 	gLegacyAdvParams_IBeacon;
extern appExtAdvertisingParams_t        gAppAdvParams;
extern appExtAdvertisingParams_t        gAppAdvParams_IBeacon;

extern appAdvertisingParams_t mAppAdvParams_IBeacon;

#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
extern gapScanningParameters_t          gScanParams;
extern gapConnectionRequestParameters_t gConnReqParams;
#endif

typedef struct{
	uint8_t validConnectedKeyNum;
	uint8_t validConnectedKeyFobNum;
	uint8_t bleAdvState : 1;
	uint8_t bleScanState : 1;
	uint8_t bleConfigDoneFlag : 1;
	uint8_t bleReserved : 5;
}bleStateStruct_t;

#endif /* DIGITAL_KEY_CAR_ANCHOR_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */


#endif /* SIGNALS_BLE_SIGNALS_CFG_H_ */
