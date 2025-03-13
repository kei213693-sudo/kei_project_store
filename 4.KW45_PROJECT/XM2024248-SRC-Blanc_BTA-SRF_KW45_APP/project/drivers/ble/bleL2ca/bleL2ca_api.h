/*
 * bleL2ca_api.h
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLEL2CA_BLEL2CA_API_H_
#define DRV_BLE_BLEL2CA_BLEL2CA_API_H_

#include "bleL2ca_cfg.h"

bleResult_t App_RegisterLeCbCallbacks
(
    l2caLeCbDataCallback_t      pCallback,
    l2caLeCbControlCallback_t   pCtrlCallback
);

#endif /* DRV_BLE_BLEL2CA_BLEL2CA_API_H_ */
