/**
 ****************************************************************************************
 *
 * @file app.h
 *
 * @brief Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_H_
#define APP_H_

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
#include "prf.h"
#include "app_central.h"
#endif
#include "app_adv.h"
#if (BLE_APP_WHITE_LIST)
#include "app_whl.h"
#endif
#if (BLE_APP_PRIVACY)
#include "app_privacy.h"
#endif
#include "tspps_task.h"             // health thermometer functions
#if (BLE_APP_ONMICRO_DFU)
#include "onmicro_dfu.h"
#include "app_onmicro_dfu.h"
#endif

/*
 * DEFINES
 ****************************************************************************************
 */


#define APP_HANDLER(handler_name, msgid, param, dest, src) app_##handler_name##_handler(param)
/*
 * MACROS
 ****************************************************************************************
 */

#define APP_HANDLERS(subtask)    {&subtask##_msg_handler_list[0], ARRAY_LEN(subtask##_msg_handler_list)}

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

#if (NVDS_SUPPORT)
/// List of Application NVDS TAG identifiers
enum app_nvds_tag
{
    // tag
    NVDS_TAG_APP_FIRST = NVDS_TAG_APP_SPECIFIC_FIRST,

    // tag length
    NVDS_LEN_APP_FIRST        = 1,
};
#endif // (NVDS_SUPPORT)

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Structure containing information about the handlers for an application subtask
struct app_subtask_handlers
{
    /// Pointer to the message handler table
    const struct ke_msg_handler *p_msg_handler_tab;
    /// Number of messages handled
    uint16_t msg_cnt;
};

/// Application environment structure
struct app_env_tag
{
    /// Connection handle
    //uint16_t conhdl;
    /// Connection Index
    uint16_t  conidx[BLE_APP_TSPPS_CONNECT_MAX];
    uint8_t  connect_num;

    /// Last initialized profile
    uint8_t next_svc;

    /// Device Name length
    nvds_tag_len_t dev_name_len;
    /// Device Name
    uint8_t dev_name[APP_DEVICE_NAME_MAX_LEN];

    /// Local device IRK
    uint8_t loc_irk[KEY_LEN];

    /// Counter used to generate IRK
    uint8_t rand_cnt;

#if (BLE_APP_CENTRAL_SUPPORT)
    struct app_central_tag central;
#endif
};

/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */

/// Application environment
extern struct app_env_tag app_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void app_gap_connected_handler(struct gapc_connection_req_ind const *param);
void app_gap_disconnected_handler(struct gapc_disconnect_ind const *param);
void app_gap_disconnected_compelete_handler(struct gapc_cmp_evt const *param);
void app_scan_error_hander(uint8_t error);
void app_tspp_empty_handler(void const *param);
void app_tspp_full_handler(void const *param);
void app_tspp_recv_handler(struct tspp_server_write_ind const *param);
void app_tspp_error_handler(struct tspp_server_error_evt const *param);
void app_uart1_rx_callback(uint8_t data);
#if (BLE_APP_CENTRAL_SUPPORT)
void app_central_recv_handler(uint8_t conidx, uint16_t handle, uint16_t len ,uint8_t *pdata);
#endif

/**
 ****************************************************************************************
 * @brief Initialize the BLE demo application.
 ****************************************************************************************
 */
void appm_init(void);


/**
 ****************************************************************************************
 * @brief register all service to host
 ****************************************************************************************
 */
void appm_reg_svc_itf(void);

/**
 ****************************************************************************************
 * @brief Add a required service in the database
 ****************************************************************************************
 */
bool appm_add_svc(void);

/**
 ****************************************************************************************
 * @brief Send to request to update the connection parameters
 ****************************************************************************************
 */
void appm_update_param(struct gapc_conn_param *conn_param);

/**
 ****************************************************************************************
 * @brief Send a disconnection request
 ****************************************************************************************
 */
void appm_disconnect(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Retrieve device name
 *
 * @param[out] device name
 *
 * @return name length
 ****************************************************************************************
 */
uint8_t appm_get_dev_name(uint8_t* name);

/**
 ****************************************************************************************
 * @brief Start/stop advertising
 *
 * @param[in] start     True if advertising has to be started, else false
 ****************************************************************************************
 */
void appm_update_adv_state(bool start);

/**
 ****************************************************************************************
 * @brief delete advertising
 *
 * @param[in] none
 ****************************************************************************************
 */

void appm_delete_advertising(void);

/**
 ****************************************************************************************
 * @brief
 *
 ****************************************************************************************
 */
void app_recv_data(uint16_t conidx, uint8_t *p_pdata, uint16_t len);


/**
 ****************************************************************************************
 * @brief when role is device, call the connect setting
 *
 * @param[in] connect index 
 *
 * @return index
 ****************************************************************************************
 */
uint8_t app_dev_set_connect(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief when role is device, call the disconnect setting
 *
 * @param[in] connect index 
 *
 * @return index
 ****************************************************************************************
 */
uint8_t app_dev_set_disconnect(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief when role is device, call the get index
 *
 * @param[in] connect index 
 *
 * @return index
 ****************************************************************************************
 */
uint8_t app_dev_get_by_conidx(uint8_t conidx);

/// @} APP

#endif //(BLE_APP_PRESENT)

#endif // APP_H_
