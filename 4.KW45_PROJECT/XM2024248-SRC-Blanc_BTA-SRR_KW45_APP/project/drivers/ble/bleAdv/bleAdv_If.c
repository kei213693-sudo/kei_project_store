/*
 * bleAdv_If.c
 *
 *  Created on: 2024年7月4日
 *      Author: F704012276_
 */

#include "bleAdv_cfg.h"

#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
static appScanningParams_t appScanParams = {
    &gScanParams,
	gGapDuplicateFilteringDisable_c,
    gGapScanContinuously_d,
    gGapScanPeriodicDisabled_d
};
#endif

void BleAppIf_NioAdvertise(void);

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleAppIf_Start(void)
{
	BleAppIf_StartScan();
	BleAppIf_StartAdv();
}

void BleAppIf_StartNonConAdv(void)
{
	gAppAdvParams.pGapExtAdvParams->extAdvProperties = (bleAdvRequestProperties_t)(gAdvReqScannable_c | gAdvReqLegacy_c);
	// gAppScanRspDataNio.aAdStructures->aData[1] = 'X';
	// gAppScanRspDataNio.aAdStructures->aData[2] = 'N';
	BleAppIf_Start();
}

void BleAppIf_StartConAdv(void)
{
	gAppAdvParams.pGapExtAdvParams->extAdvProperties = (bleAdvRequestProperties_t)(gAdvReqConnectable_c | gAdvReqScannable_c | gAdvReqLegacy_c);
	// gAppScanRspDataNio.aAdStructures->aData[1] = 'I';
	// gAppScanRspDataNio.aAdStructures->aData[2] = 'O';
	BleAppIf_Start();
}

/* Start Scan*/
void BleAppIf_StartScan(void)
{
#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
    if(!Get_bit_bleScanState())
    {
        /* Start scanning for non-CCC key fobs */
        (void)BluetoothLEHost_StartScanning(&appScanParams, BleApp_ScanningCallback);
    }
#endif /* (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U)) */
}

void BleAppIf_StartAdv(void)
{
#if (defined(BLE_ROLE_ADV) && (BLE_ROLE_ADV == 1))
    if(!Get_bit_bleAdvState())
    {
    	gCurrentAdvHandle = (uint8_t)gLegacyAdvSetHandle_c;
//    	BleAppIf_BeaconAdvertise();
    	BleAppIf_NioAdvertise();
    }
#endif
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will start after
*               the parameters are set.
*
********************************************************************************** */
void BleAppIf_NioAdvertise(void)
{
#if (defined(BLE_ROLE_ADV) && (BLE_ROLE_ADV == 1U))
#if (defined(gAppNIOAdvEnable_c) && (gAppNIOAdvEnable_c == 1U))
    (void)BluetoothLEHost_StartExtAdvertising(&gAppAdvParams, BleApp_AdvertisingCallback, BleApp_ConnectionCallback);
#endif
#endif
}

void BleAppIf_BeaconAdvertise(void)
{
#if (defined(BLE_ROLE_ADV) && (BLE_ROLE_ADV == 1U))
#if (defined(gAppNIOIBeacon_c) && (gAppNIOIBeacon_c == 1U))
	/* Set advertising parameters*/
	extern appExtAdvertisingParams_t gAppAdvParamsIBeacon;
	gAppAdvParamsIBeacon.pGapExtAdvParams->handle = 0x01;
	gAppAdvParamsIBeacon.handle = 0x01;
    (void)BluetoothLEHost_StartExtAdvertising(&gAppAdvParamsIBeacon, BleApp_AdvertisingCallback, NULL/*, NULL, BleApp_ConnectionCallback*/);
#endif
#endif
}
