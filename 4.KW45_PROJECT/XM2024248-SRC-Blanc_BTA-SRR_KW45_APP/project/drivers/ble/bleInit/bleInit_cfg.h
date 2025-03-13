/*
 * bleInit_cfg.h
 *
 *  Created on: 2024年6月12日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLEINIT_CFG_H_
#define DRV_BLE_BLEINIT_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "board.h"

#if (defined(gUseHciTransportDownward_d) && (gUseHciTransportDownward_d == 1)) || \
    (defined(gUseHciTransportUpward_d) && (gUseHciTransportUpward_d == 1))
#include "hci_transport.h"
#endif /* gUseHciTransportDownward_d || gUseHciTransportUpward_d */

#if !defined(gUseHciTransportDownward_d) || (!gUseHciTransportDownward_d)
#include "controller_interface.h"

#if defined(K32W232H_SERIES)
#include "nxp2p4_xcvr.h"
#endif

#include "fsl_adapter_flash.h"
#endif /* gUseHciTransportDownward_d */

/************************************************************************************
*************************************************************************************
* Public macros - Do not modify directly! Override in app_preinclude.h if needed.
*************************************************************************************
************************************************************************************/
#ifndef gBleXcvrInitRetryCount_c
#define gBleXcvrInitRetryCount_c (10U)
#endif /* gBleXcvrInitRetryCount_c */

/*
 * This enables/disables the HCI Reset command sent by the Host at init sequence
 * Default value is disabled, only for gUseHciTransportDownward_d is required
 */
#ifndef gHostInitResetController_c
    #if defined(gUseHciTransportDownward_d) && gUseHciTransportDownward_d
        #define gHostInitResetController_c TRUE
    #else /* gUseHciTransportDownward_d */
        #define gHostInitResetController_c FALSE
    #endif /* gUseHciTransportDownward_d */
#endif /* gHostInitResetController_c */

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
#if !defined(gUseHciTransportUpward_d) || (!gUseHciTransportUpward_d)
extern bool_t gHostInitResetController;
#endif /* gUseHciTransportUpward_d */

#endif /* DRV_BLE_BLEINIT_CFG_H_ */
