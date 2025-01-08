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
 * @addtogroup APP_EXAMPLE_WATCH_APP_H app.h
 * @ingroup BLE_APP_WATCH
 *
 * @brief the watch application functions
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration
#include "gapc_task.h"            // GAP Controller Task API
#include "gapm_task.h"            // GAP Manager Task API
#include "gattc_task.h"

//#if (BLE_APP_PRESENT)

#include <stdint.h>          // Standard Integer Definition
#include <co_bt.h>           // Common BT Definitions
#include "arch.h"            // Platform Definitions
#include "gapc.h"            // GAPC Definitions

#if (NVDS_SUPPORT)
#include "nvds.h"
#endif // (NVDS_SUPPORT)

#include "app_adv.h"
#if (BLE_APP_WHITE_LIST)
#include "app_whl.h"
#endif
#if (BLE_APP_PRIVACY)
#include "app_privacy.h"
#endif
#if (BLE_APP_ONMICRO_DFU)
#include "onmicro_dfu.h"
#include "app_onmicro_dfu.h"
#endif
#include "wechat_task.h"
#include "hogpd_task.h"             // HID Over GATT Profile Device Role Functions
#include "tspps_task.h"             // health thermometer functions
#include "app_task.h"               // application task definitions

/*
 * DEFINES
 ****************************************************************************************
 */
void app_audio_rx_cmd_handler(struct app_audio_rx_cmd_t const *param);
void app_gap_connected_handler(struct gapc_connection_req_ind const *param);
void app_gap_disconnected_handler(struct gapc_disconnect_ind const *param);
void app_gap_param_update_ind_handler(struct gapc_param_updated_ind const *param);
void app_smp_encrypted_handler(uint8_t const *param);
void app_gatt_mtu_changed_handler(struct gattc_mtu_changed_ind const *param);
void app_tspp_empty_handler(void const *param);
void app_tspp_full_handler(void const *param);
void app_tspp_recv_handler(struct tspp_server_write_ind const *param);
void app_tspp_error_handler(struct tspp_server_error_evt const *param);
#if BLE_APP_ANCSC
void app_ancs_recv_evt_handler(struct ancsc_notification_ind const *param);
void app_ancs_recv_msg_handler(struct ancsc_ntf_attr_ind const *param);
#endif

/// the handler event function
#define APP_HANDLER(handler_name, msgid, param, dest, src) app_##handler_name##_handler(param)

/// Maximal length of the Device Name value
#define APP_DEVICE_NAME_MAX_LEN      (18)
/// the connect interval min
#define APP_CONN_INTERVAL_MIN        (8)
/// the connect interval max
#define APP_CONN_INTERVAL_MAX        (8)
/// the connect latency
#define APP_CONN_LATENCY             (25)
/// the conn timeout
#define APP_CONN_TIME_OUT            (200)

/**
 * Advertising Parameters
 */
#define APP_ADV_FAST_INTERVAL     0x0320//默认500ms 0x0028   /**< Fast advertising interval (in units of 0.625 ms. This value corresponds to 25 ms.). */
#define APP_ADV_SLOW_INTERVAL     0x0C80   /**< Slow advertising interval (in units of 0.625 ms. This value corrsponds to 2 seconds). */

#define APP_ADV_FAST_TIMEOUT      0       /**< The duration of the fast advertising period (in seconds). max 655, or 0 is no timeout*/
#define APP_ADV_SLOW_TIMEOUT      0       /**< The duration of the slow advertising period (in seconds). max 655. or 0 is no timeout*/

#define APP_ADV_DIRECTED_TIMEOUT  0       /**< The duration of the fast advertising period (in seconds). mas 655. or 0 is high duty cycle*/


#define APP_VERSION_DATE      (__DATE__)
#define APP_VERSION_TIME      (__TIME__)

/*
 * MACROS
 ****************************************************************************************
 */

/// the profile task handler list
#define APP_HANDLERS(subtask)    {&subtask##_msg_handler_list[0], ARRAY_LEN(subtask##_msg_handler_list)}

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

#if (NVDS_SUPPORT)
/// List of Application NVDS TAG identifiers
enum app_nvds_tag
{
    NVDS_TAG_APP_FIRST = NVDS_TAG_APP_SPECIFIC_FIRST,

    // tag
    NVDS_TAG_MOUSE_SAMPLE_RATE,
    NVDS_TAG_MOUSE_NTF_CFG,
    NVDS_TAG_MOUSE_TIMEOUT,
    NVDS_TAG_MOUSE_ENERGY_SAFE,

    // tag Length
    NVDS_LEN_MOUSE_SAMPLE_RATE = 1,
    NVDS_LEN_MOUSE_NTF_CFG     = 2,
    NVDS_LEN_MOUSE_TIMEOUT     = 2,
    NVDS_LEN_MOUSE_SAFE_ENERGY = 2,
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

///@struct app_env_tag 
///Application environment structure
struct app_env_tag
{
    /// Connection handle
    uint16_t conhdl;
    /// Connection Index
    uint8_t  conidx;

    /// Last initialized profile
    uint8_t next_svc;

    /// Device Name length
    nvds_tag_len_t dev_name_len;
    /// Device Name
    uint8_t dev_name[APP_DEVICE_NAME_MAX_LEN];
    /// addr
    bd_addr_t addr;
    /// Local device IRK
    uint8_t loc_irk[KEY_LEN];

    /// Counter used to generate IRK
    uint8_t rand_cnt;
};

/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */

extern struct app_env_tag app_env; /// Application environment

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void appm_recv_data(uint8_t conidx, uint8_t *p_pdata, uint16_t len);

/**
 ****************************************************************************************
 * @brief Initialize the BLE demo application.
 * @return void.
 ****************************************************************************************
 */
void appm_init(void);

/**
 ****************************************************************************************
 * @brief Register service profile interface
 ****************************************************************************************
 */
void appm_reg_svc_itf(void);

/**
 ****************************************************************************************
 * @brief Add a required service in the database
 * @return true: add server success; false: add server fail;
 ****************************************************************************************
 */
bool appm_add_svc(void);

/**
 ****************************************************************************************
 * @brief Send to request to update the connection parameters
 * @param[in] conn_param: the connect param. see struct gapc_conn_param
 * @return void.
 ****************************************************************************************
 */
void appm_update_param(struct gapc_conn_param *conn_param);

/**
 ****************************************************************************************
 * @brief Send a disconnection request
 * @param[in] conidx: the connect index.
 * @return void.
 ****************************************************************************************
 */
void appm_disconnect(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Retrieve device name
 *
 * @param[out] name: device name
 *
 * @return name length
 ****************************************************************************************
 */
uint8_t appm_get_dev_name(uint8_t* name);


void appm_get_rssi(void);

void appm_advertising_init(void);

//#endif //(BLE_APP_PRESENT)

/// @} APP

#endif // APP_H_
