/*
 * bleIF.c
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#include "ble_general.h"
#include "bleMessage_cfg.h"
#include "bleMsg_par.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "signals_api.h"
#include "bleTP_api.h"

static bleUuid_t uuid128_temp;

static void SK_TX_WriteCharacteristicValue
(
	uint8_t deviceId,
    uint16_t length,
    const uint8_t *pdata
);

void BleIf_Receive(bleMsgRevInfo_t *bleMsgInfo)
{
    Tp_Receive(bleMsgInfo);
}

void BleIf_Init(void)
{
	/* TODO */
}

void BleIf_Transmit(
		uint8_t deviceId,
		uint8_t len,
		uint16_t serviceHandle,
		const uint8_t* data)
{
	bool_t    isNotifActive = FALSE;
    uint16_t valuehandle = gGattDbInvalidHandle_d;
    uint16_t handleCccd = gGattDbInvalidHandle_d;
    bleResult_t result = gBleSuccess_c;

	if((maPeerInformation[deviceId].gapRole == gGapCentral_c) &&\
			(maPeerInformation[deviceId].appState == mAppRunning_c))
	{
		/* Use SK_TX to transmit to KeyFob*/
		SK_TX_WriteCharacteristicValue(deviceId, len, data);
	}
	else if((maPeerInformation[deviceId].gapRole == gGapPeripheral_c) &&\
			(maPeerInformation[deviceId].appState == mAppRunning_c))
	{
		/***************************************************
		 * 1. Use notify to send data to phone
		 * 2. Update characteristic value and send notification
		 * *************************************************/
		valuehandle = (uint16_t)value_SKP_RX;
		result = GattDb_WriteAttribute(valuehandle, (uint16_t)len, data);

		if (result == gBleSuccess_c)
		{
			if (GattDb_FindCccdHandleForCharValueHandle(valuehandle, &handleCccd) == gBleSuccess_c)
			{
				if (gBleSuccess_c == Gap_CheckNotificationStatus
					(deviceId, handleCccd, &isNotifActive) &&
					TRUE == isNotifActive)
				{
					(void)GattServer_SendNotification(deviceId, valuehandle);
				}
			}
		}
	}
}

/*! *********************************************************************************
 * \brief        Send the received uart stream over GATT
 *
 * \param[in]    pData              Pointer to the received stream.
 * \param[in]    streamSize         The number of bytes in the stream.
 ********************************************************************************** */
static void SK_TX_WriteCharacteristicValue
(
	uint8_t deviceId,
    uint16_t length,
    const uint8_t *pdata
)
{
    gattCharacteristic_t characteristic = {(uint8_t)gGattCharPropNone_c, {0}, 0, 0};
    uint8_t mPeerId = 0;

	characteristic.value.handle = (uint16_t)value_SK_TX;
	(void)GattClient_WriteCharacteristicValue(deviceId, &characteristic, \
			length, pdata, FALSE, \
			FALSE, FALSE, NULL);
}
