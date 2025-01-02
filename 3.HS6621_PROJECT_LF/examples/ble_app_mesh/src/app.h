/**
 ****************************************************************************************
 *
 * @file app.h
 *
 * @brief BLE mesh app header file.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

#ifndef APP_H_
#define APP_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"  // SW configuration

#if (BLE_APP_PRESENT)

#include "app_adv.h"
#include "app_mesh.h"
#include "m_tb.h"

/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/// List of Application NVDS TAG identifiers
#if (NVDS_SUPPORT)
enum app_nvds_tag
{
    NVDS_TAG_APP_FIRST           = NVDS_TAG_APP_SPECIFIC_FIRST,

    // tag
    NVDS_TAG_MESH_TMALL_MAC      = PARAM_ID_BD_ADDRESS,
    NVDS_TAG_MESH_TMALL_PID      = NVDS_TAG_APP_FIRST,
    NVDS_TAG_MESH_TMALL_SEC      = NVDS_TAG_MESH_TMALL_PID + 1,
    NVDS_TAG_MESH_HW_ATTR        = NVDS_TAG_MESH_TMALL_SEC + 1,

    // tag Length
    NVDS_LEN_MESH_TMALL_MAC      = 6,
    NVDS_LEN_MESH_TMALL_PID      = 4,
    NVDS_LEN_MESH_TMALL_SEC      = 16,
    NVDS_LEN_MESH_HW_ATTR        = 12,

};
#endif

/*
 * MACROS
 ****************************************************************************************
 */
#define APP_HANDLERS(subtask)    {&subtask##_msg_handler_list[0], ARRAY_LEN(subtask##_msg_handler_list)}

/// Maximal length of the Device Name value
#define APP_DEVICE_NAME_MAX_LEN      (18)

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
