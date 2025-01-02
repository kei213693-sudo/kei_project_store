/**
 ****************************************************************************************
 *
 * @file app_central.h
 *
 * @brief Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_CENTRAL_H_
#define APP_CENTRAL_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_PRESENT)

#include <stdint.h>          // Standard Integer Definition
#include <co_bt.h>           // Common BT Definitions
#include "arch.h"            // Platform Definitions
#include "gapc.h"            // GAPC Definitions

#if (NVDS_SUPPORT)
#include "nvds.h"
#endif // (NVDS_SUPPORT)

#if (BLE_APP_CENTRAL_SUPPORT)
/*
 * DEFINES
 ****************************************************************************************
 */
enum eapp_central_substate
{
    /// activity does not exists
    APP_CENTRAL_SUB_STATE_IDLE = 0,
    /// Creating activity
    APP_CENTRAL_SUB_STATE_CREATING,

    /// activity created
    APP_CENTRAL_SUB_STATE_CREATED,
    /// Starting activity
    APP_CENTRAL_SUB_STATE_STARTING,
    /// activity started
    APP_CENTRAL_SUB_STATE_STARTED,
    /// Stopping activity
    APP_CENTRAL_SUB_STATE_STOPPING,
    /// Stopped activity
    APP_CENTRAL_SUB_STATE_STOPPED,
    /// delete activity
    APP_CENTRAL_SUB_STATE_DELETING,
};

enum eapp_central_state
{
    APP_CENTRAL_STATE_IDLE = 0,
    APP_CENTRAL_STATE_SCAN,
    APP_CENTRAL_STATE_INIT,
};

enum eapp_central_error
{
    APP_CENTRAL_ERROR_NO_ERROR         = 0,
    APP_CENTRAL_ERROR_SCAN_STARTED        ,
    APP_CENTRAL_ERROR_SCAN_FAIL           ,
    APP_CENTRAL_ERROR_CONN_NO_FREE        ,
    APP_CENTRAL_ERROR_CONN_CONNECTING     ,
    APP_CENTRAL_ERROR_CONN_FAIL           ,
};

struct app_central_node_tag
{
    struct gap_bdaddr addr;
    
    uint16_t mtu;
    uint16_t tx_len;
    uint16_t write_handle;
    uint16_t notify_handle;
    uint16_t conhdl;
    
    uint8_t conidx;
    uint8_t actv_idx;
    uint8_t state;
};

/// central tag: scan only one, but connect have BLE_APP_CENTRAL_SUPPORT;
struct app_central_tag
{
    prf_env_t prf_env;

    //uint8_t state;

    uint8_t scan_operation;
    /// for scan donging.
    uint8_t scan_state;
    uint8_t scan_actv_idx;
    // 1: start; 2: stop
    uint8_t scan_todo;
    
    uint8_t w4connect;
    struct app_central_node_tag connect[BLE_APP_CENTRAL_SUPPORT];
};
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief central init
 *
 * @return void
 ****************************************************************************************
 */
void app_central_init(void);

void app_central_update_state(uint8_t opera, uint8_t actv_idx, uint8_t reason);

/**
 ****************************************************************************************
 * @brief start scan
 *
 * @return status of scan @ref eapp_central_error
 ****************************************************************************************
 */
uint8_t app_central_scan_start(void);

/**
 ****************************************************************************************
 * @brief stop scan
 *
 * @return status of scan @ref eapp_central_error
 ****************************************************************************************
 */
uint8_t app_central_scan_stop(void);

/**
 ****************************************************************************************
 * @brief connect by addr
 *
 * @return status of scan @ref eapp_central_error
 ****************************************************************************************
 */
uint8_t app_central_connect(struct gap_bdaddr addr);

/**
 ****************************************************************************************
 * @brief disconnect by handle
 *
 * @return status of scan @ref eapp_central_error
 ****************************************************************************************
 */
uint8_t app_central_disconnect(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief cancle current connecting
 *
 * @return status of init @ref eapp_central_error
 ****************************************************************************************
 */
uint8_t app_central_connect_cancel(void);

/**
 ****************************************************************************************
 * @brief set connect handle
 *
 * @return void
 ****************************************************************************************
 */
void app_central_set_conn_handle(uint8_t conidx, uint16_t conhdl);

/**
 ****************************************************************************************
 * @brief get connect index
 *
 * @return void
 ****************************************************************************************
 */
uint8_t app_central_find_by_conidx(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief set max mtu
 *
 * @return void
 ****************************************************************************************
 */
void app_central_set_max_mtu(uint8_t conidx, uint16_t mtu);

/**
 ****************************************************************************************
 * @brief set tx_len
 *
 * @return void
 ****************************************************************************************
 */
void app_central_set_tx_len(uint8_t conidx, uint16_t tx_len);

/**
 ****************************************************************************************
 * @brief get scan state
 *
 * @param[in] void
 * @return the scan state @eapp_central_substate
 * ****************************************************************************************
 */
uint8_t app_central_get_scan_state(void);

/// @} APP

#endif // (BLE_APP_CENTRAL_SUPPORT)
#endif //(BLE_APP_PRESENT)

#endif // APP_CENTRAL_H_
