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
#include "prf.h"
#include "att.h"
#if (NVDS_SUPPORT)
#include "nvds.h"
#endif // (NVDS_SUPPORT)

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximal length of the Device Name value
//#define APP_DEVICE_NAME_MAX_LEN      (18)

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

/// scan state machine
enum app_scan_state
{
    /// activity does not exists
    APP_SCAN_STATE_IDLE = 0,
    /// Creating activity
    APP_SCAN_STATE_CREATING,

    /// activity created
    APP_SCAN_STATE_CREATED,
    /// Starting activity
    APP_SCAN_STATE_STARTING,
    /// activity started
    APP_SCAN_STATE_STARTED,
    /// Stopping activity
    APP_SCAN_STATE_STOPPING,
    /// Stopped activity
    APP_SCAN_STATE_STOPPED,
    /// delete activity
    APP_SCAN_STATE_DELETING,
};

/// scan state machine
enum app_init_state
{
    /// activity does not exists
    APP_INIT_STATE_IDLE = 0,
    /// Creating activity
    APP_INIT_STATE_CREATING,

    /// activity created
    APP_INIT_STATE_CREATED,
    /// Starting activity
    APP_INIT_STATE_STARTING,
    /// activity started
    APP_INIT_STATE_STARTED,
    /// Stopping activity
    APP_INIT_STATE_STOPPING,
    /// Stopped activity
    APP_INIT_STATE_STOPPED,
    /// delete activity
    APP_INIT_STATE_DELETING,
};

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

struct app_connecting_device
{
    /// Connection handle
    uint16_t conhdl;

    struct gap_bdaddr trans_addr;
    /// activity index
    uint8_t init_actv_idx;
    /// Current init state (@see enum app_init_state)
    uint8_t init_state;
    /// Next expected operation completed event
    uint8_t init_op;
};

struct app_connected_device
{
    uint16_t mtu;
    uint16_t tx_len;

    uint16_t write_handle;
    uint16_t notify_handle;
    uint16_t notify_state;
};

/// Application environment structure
struct app_env_tag
{
    /// Connection handle
    //uint16_t conhdl;
    /// Connection Index
    //uint8_t  conidx;

    /// activity index
    uint8_t scan_actv_idx;
    /// Current scan state (@see enum app_scan_state)
    uint8_t scan_state;
    /// Next expected operation completed event
    uint8_t scan_op;

    /// Last initialized profile
    uint8_t next_svc;

    /// Local device IRK
    uint8_t loc_irk[KEY_LEN];

    /// Counter used to generate IRK
    uint8_t rand_cnt;

    prf_env_t prf_env;

    struct app_connecting_device init[BLE_APP_CENTRAL_CONNECT_MAX];
    struct app_connected_device connected[BLE_APP_CENTRAL_CONNECT_MAX];
    uint8_t w4connect;
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
 * @brief
 ****************************************************************************************
 */
void appm_fsm_next(bool next, uint8_t operation);

/**
 ****************************************************************************************
 * @brief
 ****************************************************************************************
 */
void appm_create_init(void);

/**
 ****************************************************************************************
 * @brief
 ****************************************************************************************
 */

void appm_delete_init(uint8_t idx);
/**
 ****************************************************************************************
 * @brief
 ****************************************************************************************
 */

uint8_t appm_connect_by_addr(struct gap_bdaddr addr);
/**
 ****************************************************************************************
 * @brief
 ****************************************************************************************
 */
uint8_t appm_connect_by_conhdl(uint8_t conhdl);
/**
 ****************************************************************************************
 * @brief
 ****************************************************************************************
 */
uint8_t appm_connect_by_actv_idx(uint8_t idx);
/**
 ****************************************************************************************
 * @brief
 ****************************************************************************************
 */

uint8_t appm_connect_by_actv_state(uint8_t state);
/**
 ****************************************************************************************
 * @brief stop scan
 *
 * @param[in] none
 ****************************************************************************************
 */

void appm_stop_scan(void);

/**
 ****************************************************************************************
 * @brief Send a disconnection request
 ****************************************************************************************
 */
void appm_disconnect(uint8_t conidx);

/// @} APP

#endif //(BLE_APP_PRESENT)

#endif // APP_H_
