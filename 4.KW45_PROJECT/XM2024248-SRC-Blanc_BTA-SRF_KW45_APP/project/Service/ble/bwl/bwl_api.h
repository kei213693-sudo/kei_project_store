/*
 * bwl_api.h
 *
 *  Created on: 2024年9月14日
 *      Author: F704012276_
 */

#ifndef BLEAPP_BWL_BWL_API_H_
#define BLEAPP_BWL_BWL_API_H_

#include "fsl_common.h"
#include "ble_cfgSignals.h"

typedef enum
{
	BLEAPI_FLASH_INDEX_WHITELIST = 127,
}BwlApi_flash_index_e;

typedef enum R_BLE_STATUS_T
{
    R_BLE_OK = 0,                /**< Operation terminated successfully */
    R_BLE_BUSY = 1,              /**< Operation is still ongoing */
    R_BLE_SUSPENDED = 2,         /**< Flash operation is suspended */
    R_BLE_ERR_CONFIGURATION = 3, /**< The FDL configuration (descriptor) was wrong */
    R_BLE_ERR_PARAMETER = 4,     /**< A parameter of the FDL function call was wrong */
    R_BLE_ERR_PROTECTION = 5,    /**< Operation blocked due to wrong parameters */
    R_BLE_ERR_REJECTED = 6,      /**< Flow error, e.g. another operation is still busy */
    R_BLE_ERR_WRITE = 7,         /**< Flash write error */
    R_BLE_ERR_ERASE = 8,         /**< Flash erase error */
    R_BLE_ERR_BLANKCHECK = 9,    /**< Flash blank check error */
    R_BLE_ERR_COMMAND = 10,      /**< Unknown command */
    R_BLE_ERR_ECC_SED = 11,      /**< Single bit error detected by ECC */
    R_BLE_ERR_ECC_DED = 12,      /**< Double bit error detected by ECC */
    R_BLE_ERR_INTERNAL = 13,     /**< Library internal error */
    R_BLE_CANCELLED = 14,         /**< Flash operation is cancelled */
	R_BLE_SLOT_INVALID = 0xff,
} r_ble_status_t;


void Bwl_Init(void);
void Bwl_AddWL(btmWhitelist_t *wl);
void Bwl_GetNumofRemainWL(uint8_t *count);
uint8_t Bwl_DeviceAvailable(btmWhitelist_t *wl);
void Bwl_UpdateTimeStamp(uint8_t *addr, bleCfg_timestamp_t* timestamp);

r_ble_status_t Ble_Api_fdl_Ereas_Write(uint8_t index, const uint8_t *databuf, uint32_t len);
r_ble_status_t Ble_Api_fdl_FlashRead(uint8_t index, uint8_t *databuf, uint32_t len);
void Bwl_GetKeyFobInfo(uint8_t* keyFobArray, uint8_t* num);
void Bwl_GetPhoneInfo(uint8_t* phoneArray, uint8_t* num);
void Bwl_WriteKeyFobLearningInfo(uint8_t* data);

uint8_t Bwl_CheckAndRemovePhoneAndKeyfob(uint8_t* device_addr);
uint8_t Bwl_CheckAndRevokeKeyfob(uint8_t* device_addr);
uint8_t Bwl_CheckAndRecoverkeyFob(uint8_t* device_addr);


#endif /* BLEAPP_BWL_BWL_API_H_ */
