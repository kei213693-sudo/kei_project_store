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
#include "app_adv.h"
#if (BLE_APP_WHITE_LIST)
#include "app_whl.h"
#endif
#if (BLE_APP_PRIVACY)
#include "app_privacy.h"
#endif
/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximal length of the Device Name value
#define APP_DEVICE_NAME_MAX_LEN      (18)

#define APP_CONN_INTERVAL_MIN        (8)
#define APP_CONN_INTERVAL_MAX        (8)
#define APP_CONN_LATENCY             (25)
#define APP_CONN_TIME_OUT            (200)

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

/// Application environment structure
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

    /// Local device IRK
    uint8_t loc_irk[KEY_LEN];

    /// Counter used to generate IRK
    uint8_t rand_cnt;
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

/**
 ****************************************************************************************
 * @brief Initialize the BLE demo application.
 ****************************************************************************************
 */
void appm_init(void);

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


/// @} APP

#endif //(BLE_APP_PRESENT)

#endif // APP_H_
