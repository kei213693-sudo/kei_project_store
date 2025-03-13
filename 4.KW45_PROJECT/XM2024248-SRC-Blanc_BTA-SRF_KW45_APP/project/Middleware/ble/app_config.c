/*! *********************************************************************************
* \addtogroup App Config
* @{
********************************************************************************** */
/*! *********************************************************************************
* \file app_config.c
*
* Copyright 2020-2023 NXP
*
* NXP Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from NXP.
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "gap_interface.h"
#include "ble_constants.h"
#include "gatt_db_handles.h"
#include "signals_api.h"
#include "app_config.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define smpEdiv                 0x1F99
#define mcEncryptionKeySize_c   16

#define mDefaultTxPower         gBleAdvTxPowerNoPreference_c
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
gapScanningParameters_t gScanParams =
{
    /* type */              gScanTypePassive_c,
    /* interval */          gGapScanIntervalDefault_d,
    /* window */            gGapScanWindowDefault_d,
    /* ownAddressType */    gBleAddrTypePublic_c,
    /* filterPolicy */      (uint8_t)gScanWithFilterAcceptList_c,
    /* scanning PHY */      (uint8_t)gLePhy1MFlag_c
};

/* Default Connection Request Parameters */
gapConnectionRequestParameters_t gConnReqParams =
{
    .scanInterval = 36,
    .scanWindow = 36,
    .filterPolicy = (uint8_t)gUseDeviceAddress_c,
    .ownAddressType = gBleAddrTypePublic_c,
    .connIntervalMin = gcConnectionIntervalMinDefault_c,
    .connIntervalMax = gcConnectionIntervalMinDefault_c,
    .connLatency = 0,
    .supervisionTimeout = 0xC8,
    .connEventLengthMin = 0,
    .connEventLengthMax = 0xFFFF,
    .initiatingPHYs = (uint8_t)gLePhy1MFlag_c,
};
#endif

#if (defined(BLE_ROLE_ADV) && (BLE_ROLE_ADV == 1U))

#if (defined(gAppNIOIBeacon_c) && (gAppNIOIBeacon_c == 1U))
/*----------------------------------------------I Beacon---------------------------------------------------*/
#define mAdvAppleId             0x4C, 00
#define mAdvAppleSubType        0x02
#define mAdvAppleSubTypeLength  0x15
#define mAdvIBeaconMajor        0x10, 0x00
#define mAdvIBeaconMinor        0x00, 0x01
#define mIBeaconUuid            0x9F, 0x08, 0x91, 0xD0, 0x03, 0xC9, 0x12, 0x7E, 0x76, 0xF5, 0x5B, 0x91, 0x5A, 0xBF, 0x49, 0x27

static uint8_t adData0_IBeacon[1] = {0x06};
static uint8_t adData1_IBeacon[25] = {
                       /* Apple Company Identifier */
                       mAdvAppleId,
                       /* Apple Sub Type */
                       mAdvAppleSubType,
                       /* Apple Sub Type Length */
                       mAdvAppleSubTypeLength,
                       /* iBeacon UUID */
                       mIBeaconUuid,
                       /* iBeacon Major */
                       mAdvIBeaconMajor,
                       /* iBeacon Major */
                       mAdvIBeaconMinor,
                       /* RSSI at 1m */
                       0xC5};

static gapAdStructure_t advScanStruct_IBeacon[] = {
	{
		.length = NumberOfElements(adData0_IBeacon) + 1,
		.adType = gAdFlags_c,
		.aData = (uint8_t*)adData0_IBeacon
	},
	{
		.length = NumberOfElements(adData1_IBeacon) + 1,
		.adType = gAdManufacturerSpecificData_c,
		.aData = (uint8_t*)adData1_IBeacon
	}
};

/* Default Legacy Advertising Parameters */
gapExtAdvertisingParameters_t gLegacyAdvParamsIBeacon =
{
	    /* SID */                       0xB, \
	    /* handle */                    gLegacyAdvSetHandle_c, \
	    /* minInterval */               159 /* 42 ms */, \
	    /* maxInterval */               159 /* 42 ms */, \
	    /* ownAddrType */               gBleAddrTypePublic_c,\
	    /* ownAddress */                {0, 0, 0, 0, 0, 0}, \
	    /* peerAddrType */              gBleAddrTypePublic_c,\
	    /* peerAddress */               {0, 0, 0, 0, 0, 0}, \
	    /* channelMap */                (gapAdvertisingChannelMapFlags_t) (gAdvChanMapFlag37_c | gAdvChanMapFlag38_c | gAdvChanMapFlag39_c), \
	    /* filterPolicy */              gProcessAll_c, \
	    /* extAdvProperties */          (bleAdvRequestProperties_t)(gAdvReqLegacy_c), \
	    /* TxPower */                   mDefaultTxPower, \
	    /* primaryPHY */                (gapLePhyMode_t)gLePhy1M_c, \
	    /* secondaryPHY */              (gapLePhyMode_t)gLePhy1M_c, \
	    /* secondaryAdvMaxSkip */       0, \
	    /* enableScanReqNotification*/  FALSE \
};

gapAdvertisingData_t gAppAdvertisingData_IBeacon =
{
	NumberOfElements(advScanStruct_IBeacon),
	(void*)advScanStruct_IBeacon
};

/*Default Application Advertising Parameters */
appExtAdvertisingParams_t gAppAdvParamsIBeacon =
{
    &gLegacyAdvParamsIBeacon,
    &gAppAdvertisingData_IBeacon,
    NULL,
    gLegacyAdvSetHandle_c,
    gBleExtAdvNoDuration_c,
    gBleExtAdvNoMaxEvents_c
};
#endif

#if (defined(gAppNIOAdvEnable_c) && (gAppNIOAdvEnable_c == 1U))
/*************************************************NIO Adv****************************************************/
/* Default Legacy Advertising Parameters */
gapExtAdvertisingParameters_t gLegacyAdvParams =
{
    /* SID */                       0xB, \
    /* handle */                    gLegacyAdvSetHandle_c, \
    /* minInterval */               gcAdvertisingIntervalCCC_1M_c /* 42 ms */, \
    /* maxInterval */               gcAdvertisingIntervalCCC_1M_c /* 42 ms */, \
    /* ownAddrType */               gBleAddrTypePublic_c,\
    /* ownAddress */                {0, 0, 0, 0, 0, 0}, \
    /* peerAddrType */              gBleAddrTypePublic_c,\
    /* peerAddress */               {0, 0, 0, 0, 0, 0}, \
    /* channelMap */                (gapAdvertisingChannelMapFlags_t) (gAdvChanMapFlag37_c | gAdvChanMapFlag38_c | gAdvChanMapFlag39_c), \
    /* filterPolicy */              gProcessAll_c, \
    /* extAdvProperties */          (bleAdvRequestProperties_t)(gAdvReqConnectable_c | gAdvReqScannable_c | gAdvReqLegacy_c), \
    /* TxPower */                   mDefaultTxPower, \
    /* primaryPHY */                (gapLePhyMode_t)gLePhy1M_c, \
    /* secondaryPHY */              (gapLePhyMode_t)gLePhy1M_c, \
    /* secondaryAdvMaxSkip */       0, \
    /* enableScanReqNotification*/  FALSE \
};

/*Default Application Advertising Parameters */
appExtAdvertisingParams_t gAppAdvParams =
{
    &gLegacyAdvParams,
    &gAppAdvertisingDataNio,
    &gAppScanRspDataNio,
    gLegacyAdvSetHandle_c,
    gBleExtAdvNoDuration_c,
    gBleExtAdvNoMaxEvents_c
};

/*-------------------------------------------------Start of adv data set----------------------------------------------------*/
static bleAdStructData0_t adData0_Nio = {gLeGeneralDiscoverableMode_c|gBrEdrNotSupported_c};
static bleAdStructData1_t adData1_Nio = {{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10}};
static bleAdStructData2_t adData2_Nio = {\
		.CompanyID = 0x0B48,
		.Reserved = 0,
		.deviceType = 0b10,
		.NIO_ADVersion = 0b00001,
		.SubType = 0x04,
		.PID[0] = 0x01,
		.PID[1] = 0x0,
		.PID[2] = 0x01,
		.BTA_Verion = 0x02,
};

static gapAdStructure_t NioAdvScanStruct[3] = {
	{
	.length = sizeof(bleAdStructData0_t) + 1,
	.adType = gAdFlags_c,
	.aData = (uint8_t*)&adData0_Nio
	},
	{
	.length = sizeof(bleAdStructData1_t) + 1,
	.adType = gAdComplete128bitServiceList_c,
	.aData = (uint8_t*)&adData1_Nio
	},
	{
	.length = sizeof(bleAdStructData2_t) + 1,
	.adType = gAdManufacturerSpecificData_c,
	.aData = (uint8_t*)&adData2_Nio
	}
};

static gapAdStructure_t NioAdvScanResponseStruct[1] = {
	{
		.length = 0x0D,
		.adType = gAdCompleteLocalName_c,
		.aData = "NIO KEY TEST"
	},
};

gapAdvertisingData_t gAppAdvertisingDataNio =
{
    NumberOfElements(NioAdvScanStruct),
    (void *)NioAdvScanStruct
};

gapScanResponseData_t gAppScanRspDataNio =
{
	NumberOfElements(NioAdvScanResponseStruct),
    (void *)NioAdvScanResponseStruct
};
/*-------------------------------------------------------End of adv data set--------------------------------------------------------------*/
#endif
#endif


/*----------------------------------------------------Start of Pairing Parameters-------------------------------------------------------*/
/* TODO BLE Pairing Parameters*/
gapPairingParameters_t gKeyFobPairingParameters = {
    .withBonding = (bool_t)gAppUseBonding_d,
    .securityModeAndLevel = gSecurityMode_1_Level_3_c,
    .maxEncryptionKeySize = mcEncryptionKeySize_c,
    .localIoCapabilities = gIoKeyboardOnly_c,
    .oobAvailable = FALSE,
    .centralKeys = (gapSmpKeyFlags_t) (gIrk_c|gLtk_c),
    .peripheralKeys = (gapSmpKeyFlags_t) (gIrk_c|gLtk_c),
    .leSecureConnectionSupported = FALSE,
    .useKeypressNotifications = FALSE,
};

gapPairingParameters_t gPhonePairingParameters = {
    .withBonding = (bool_t)gAppUseBonding_d,
    .securityModeAndLevel = gSecurityMode_1_Level_1_c,
    .maxEncryptionKeySize = mcEncryptionKeySize_c,
    .localIoCapabilities = gIoNone_c,
    .oobAvailable = FALSE,
    .centralKeys = (gapSmpKeyFlags_t) (gIrk_c|gLtk_c),
    .peripheralKeys = (gapSmpKeyFlags_t) (gIrk_c|gLtk_c),
    .leSecureConnectionSupported = TRUE,
    .useKeypressNotifications = FALSE,
};

/* LTK */
static uint8_t smpLtk[gcSmpMaxLtkSize_c] =
    {0xD6, 0x93, 0xE8, 0xA4, 0x23, 0x55, 0x48, 0x99,
     0x1D, 0x77, 0x61, 0xE6, 0x63, 0x2B, 0x10, 0x8E};

/* RAND*/
static uint8_t smpRand[gcSmpMaxRandSize_c] =
    {0x26, 0x1E, 0xF6, 0x09, 0x97, 0x2E, 0xAD, 0x7E};

/* IRK */
static uint8_t smpIrk[gcSmpIrkSize_c] = APP_SMP_IRK;

/* CSRK */
static uint8_t smpCsrk[gcSmpCsrkSize_c] =
    {0x90, 0xD5, 0x06, 0x95, 0x92, 0xED, 0x91, 0xD7,
     0xA8, 0x9E, 0x2C, 0xDC, 0x4A, 0x93, 0x5B, 0xF9};

gapSmpKeys_t gSmpKeys = {
    .cLtkSize = mcEncryptionKeySize_c,
    .aLtk = (void *)smpLtk,
    .aIrk = (void *)smpIrk,
    .aCsrk = (void *)smpCsrk,
    .aRand = (void *)smpRand,
    .cRandSize = gcSmpMaxRandSize_c,
    .ediv = smpEdiv,
    .addressType = gBleAddrTypePublic_c,
    .aAddress = NULL
};

/*----------------------------------------------------End of Parameters-------------------------------------------------------*/

/*----------------------------------------------------Start of Device security Requiements-----------------------------------------------*/
/*TODO Device Security Requirements */
static gapSecurityRequirements_t        deviceSecurity = gGapDefaultSecurityRequirements_d;
static gapServiceSecurityRequirements_t serviceSecurity[1] = {
  {
    .requirements = {
        .securityModeLevel = gSecurityMode_1_Level_1_c,
        .authorization = FALSE,
        .minimumEncryptionKeySize = gDefaultEncryptionKeySize_d
    },
    .serviceHandle = (uint16_t)SK_BLE_GATT
  }
};

gapDeviceSecurityRequirements_t deviceSecurityRequirements = {
    .pSecurityRequirements          = (void*)&deviceSecurity,
    .cNumServices                   = 3,
    .aServiceSecurityRequirements   = (void*)serviceSecurity
};

/*----------------------------------------------------End of Device security Requiements-----------------------------------------------*/
