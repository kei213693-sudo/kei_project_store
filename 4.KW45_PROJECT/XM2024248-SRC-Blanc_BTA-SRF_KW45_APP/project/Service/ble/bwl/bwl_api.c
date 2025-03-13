/*
 * bwl_api.c
 *
 *  Created on: 2024年9月18日
 *      Author: F704012276_
 */
#include "ble_cfgSignals.h"
#include "hal_flash_if.h"
#include "bwl_api.h"

/*
 * 函数介绍:flash擦除写入封装函数
 * 参数: index：    需要操作的flash地址序列号
 *       databuf：  写入的数据
 *       len    ：  写入的长度
 * 返回值: R_FDL_OK：操作成功
 * 备注:
 */
r_ble_status_t Ble_Api_fdl_Ereas_Write(uint8_t index, const uint8_t *databuf, uint32_t len)
{

    r_ble_status_t result = R_BLE_OK;

    if (databuf != NULL)
    {
        result = hal_cfls_if_erase_write(index, databuf, len, len);
    }
    else
    {
        result = R_BLE_ERR_PARAMETER;
    }

    return result;
}
/*
 * 函数介绍: flash读操作函数接口
 * 参数:  index：    需要操作的flash地址序列号
 *       databuf：  写入的数据
 *       len    ：  写入的长度
 * 返回值: R_FDL_OK：操作成功
 * 备注:
 */
r_ble_status_t Ble_Api_fdl_FlashRead(uint8_t index, uint8_t *databuf, uint32_t len)
{

	r_ble_status_t result = R_BLE_OK;

    if (databuf != NULL)
    {
        result = hal_cfls_if_read(index, databuf, len);
    }
    else
    {
        result = R_BLE_ERR_PARAMETER;
    }

    return result;
}
