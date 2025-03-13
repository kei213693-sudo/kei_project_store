/*
 * ble_comStack_cfg.h
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#ifndef SIGNALS_BLE_BLE_COMSTACK_CFG_H_
#define SIGNALS_BLE_BLE_COMSTACK_CFG_H_

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

typedef struct blePduR_tag
{
    appCallbackHandler_t   handler;
    appCallbackParam_t     param;
} blePduR_t;

#endif /* SIGNALS_BLE_BLE_COMSTACK_CFG_H_ */
