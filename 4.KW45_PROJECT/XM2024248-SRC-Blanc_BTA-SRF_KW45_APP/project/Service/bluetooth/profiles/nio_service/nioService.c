/*
 * nioService.c
 *
 *  Created on: 2024年8月28日
 *      Author: F704012276_
 */

/*#include "ble_general.h"
#include "gatt_db_app_interface.h"
#include "gatt_server_interface.h"
#include "gap_interface.h"
#include "nioService_cfg.h"
#include "signals_api.h"

static nioServiceConfig_t nioService;
static bleUuid_t uuid128_temp;

static void NioService_SendNotifications(uint8_t deviceId, uint16_t handle);
static void NioService_WriteCharacteristicValue
(
	uint8_t deviceId,
    uint16_t length,
    const uint8_t *pdata
);

void NioService_Init(void)
{
	memset(&nioService, 0, sizeof(nioServiceConfig_t));
}

void NioService_Transmit(
		uint8_t deviceId,
		uint8_t len,
		uint16_t serviceHandle,
		const uint8_t* data)
{
	bool_t    isNotifActive = FALSE;
    uint16_t valuehandle = gGattDbInvalidHandle_d;
    uint16_t handleCccd = gGattDbInvalidHandle_d;
    bleResult_t result = gBleSuccess_c;

    valuehandle = (uint16_t)value_SK_RX;
	if(maPeerInformation[deviceId].gapRole == gGapCentral_c)
	{
		 Use write feature to transmit
		NioService_WriteCharacteristicValue(deviceId, len, data);
	}
	else if(maPeerInformation[deviceId].gapRole == gGapPeripheral_c)
	{
		 Update characteristic value and send notification
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

! *********************************************************************************
 * \brief        Send the received uart stream over GATT
 *
 * \param[in]    pData              Pointer to the received stream.
 * \param[in]    streamSize         The number of bytes in the stream.
 **********************************************************************************
static void NioService_WriteCharacteristicValue
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
}*/
