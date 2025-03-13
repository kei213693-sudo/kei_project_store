/*
 * bleSrv.c
 *
 *  Created on: 2024年8月8日
 *      Author: F704012276_
 */
#include "bleSrv_api.h"
#include "app_preinclude.h"
#include "bleSrv_cfg.h"

#define UseForTest		1

static bleParams_t bleParams;

static void Ble_InitState(void);
static void Ble_ReadParameters(bleParams_t * param);
static void Ble_ConfigAdvParameters(bleParams_t * param);
static void Ble_ConfigScanParameters(bleParams_t * param);
static void Ble_ConfigConnParameters(bleParams_t * param);

static void Ble_InitState(void)
{
	Set_bit_bleScanState(FALSE);
	Set_bit_bleAdvState(FALSE);
	Set_bit_bleConfigDoneFlag(0);
	Set_validConnectedKeyNum(0);
	Set_validConnectedKeyFobNum(0);
}

static void Ble_ReadParameters(bleParams_t * param)
{
#if UseForTest
	static uint8_t InitFlag = 0;
	uint8_t temp_data[16] = APP_VEHICLE_ID;
	uint8_t temp_data1[6] = APP_BD_ADDR;

	/* Init Config parameter */
	memcpy(param->bytes.bytes.preSet.params.vehicle_id, temp_data, sizeof(temp_data));
	memcpy(param->bytes.bytes.preSet.params.device_address, temp_data1, sizeof(temp_data1));

	param->bytes.bytes.adv.params.T_BTM_Adv_Interval_p = (uint32_t)(T_BTM_Adv_Interval/0.625);
	param->bytes.bytes.conn.params.T_Conn_Interval_Min_p = (uint32_t)(Conn_Interval_Min/1.25);
	param->bytes.bytes.conn.params.T_Conn_Interval_Max_p = (uint32_t)(Conn_Interval_Max/1.25);

	param->bytes.bytes.scan.params.C_BTM_Slave_Latency_p = (uint32_t)(Conn_Interval_Max/0.625);
	param->bytes.bytes.scan.params.T_BTM_Fscan_Interval_p = (uint32_t)(T_BTM_Fscan_Interval/0.625);
	param->bytes.bytes.scan.params.T_BTM_Scan_Duration_p = (uint32_t)(T_BTM_Sscan_Interval/0.625);
	param->bytes.bytes.scan.params.T_BTM_Scan_Window_p = (uint32_t)(T_BTM_Scan_Window/0.625);
	param->bytes.bytes.scan.params.T_BTM_Sscan_Interval_p = (uint32_t)(T_BTM_Scan_Duration/0.625);

//	if(InitFlag == 0)
//	{
//		InitFlag = 1;
//		Gap_AddDeviceToFilterAcceptList(gBleAddrTypePublic_c, param->bytes.bytes.preSet.params.device_address);
//	}
#else
	/*TODO */
#endif
}

static void Ble_ConfigAdvParameters(bleParams_t * param)
{
	uint8_t tempData[20] = {0};
	uint8_t retData[32] = {0};
	uint8_t adv_random[] = {0x88, 0xbe, 0x12, 0x2c};

	gCurrentAdvHandle = (uint8_t)gLegacyAdvSetHandle_c;
#if (defined(BLE_ROLE_ADV) && (BLE_ROLE_ADV == 1))
#if (defined(gAppNIOAdvEnable_c) && (gAppNIOAdvEnable_c == 1))
	/* 1.CONCATENATE vehicle id and the last random num */
	memcpy(tempData, param->bytes.bytes.preSet.params.vehicle_id, 16);
	memcpy(&tempData[16], adv_random, 4);

	/* 2.Calculate the concatenate string, get the hash value */
	SHA256_Hash(tempData, 20, retData);

	/* 3.Put hash value into adv parameters*/
	memcpy(gAppAdvertisingDataNio.aAdStructures[1].aData, &(retData[20]), 12);
	memcpy(&(gAppAdvertisingDataNio.aAdStructures[1].aData[12]), adv_random, 4);

	/* 4.Update Adv Parameters */
	gAppAdvParams.pGapAdvData = &gAppAdvertisingDataNio;
	gAppAdvParams.pScanResponseData = &gAppScanRspDataNio;

	gAppAdvParams.pGapExtAdvParams->minInterval = param->bytes.bytes.adv.params.T_BTM_Adv_Interval_p;
	gAppAdvParams.pGapExtAdvParams->maxInterval = param->bytes.bytes.adv.params.T_BTM_Adv_Interval_p;

#endif

#if (defined(gAppNIOIBeacon_c) && (gAppNIOIBeacon_c == 1))
	/* 1. Update I_Beacon Parameters */
	/* TODO */
//	mAppAdvParams_IBeacon
#endif
#endif
}

static void Ble_ConfigScanParameters(bleParams_t * param)
{
	/* 1.Update Scan Parameters */
#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
	gScanParams.filterPolicy = /*gScanAll_c*/gScanWithFilterAcceptList_c;
	gScanParams.interval = param->bytes.bytes.scan.params.T_BTM_Fscan_Interval_p;
	gScanParams.window = param->bytes.bytes.scan.params.T_BTM_Scan_Window_p;
#endif
}

static void Ble_ConfigConnParameters(bleParams_t * param)
{
#if (defined(gAppScanNonCCC_d) && (gAppScanNonCCC_d == 1U))
	gConnReqParams.connIntervalMin = param->bytes.bytes.conn.params.T_Conn_Interval_Min_p;
	gConnReqParams.connIntervalMax = param->bytes.bytes.conn.params.T_Conn_Interval_Max_p;
#endif
}

void Ble_Init(void)
{
	Ble_InitState();
	Ble_ReadParameters(&bleParams);
	Ble_ConfigAdvParameters(&bleParams);
	Ble_ConfigScanParameters(&bleParams);
	Ble_ConfigConnParameters(&bleParams);
}

uint8_t Get_DeviceIdbyMacAddress(const uint8_t* macAddr)
{
	for(uint8_t id = 0; id < gAppMaxConnections_c; id++)
	{
		if(memcmp(maPeerInformation[id].maPeerDeviceAddress_IRK, macAddr, 6))
		{
			return id;
		}
	}
}

bleResult_t Ble_GapDisconnect(uint8_t deviceId)
{
	if(maPeerInformation[deviceId].appState != mAppIdle_c)
	{
		return Gap_Disconnect(deviceId);
	}
}


