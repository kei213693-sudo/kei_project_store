/**
 ****************************************************************************************
 *
 * @file app_hid.h
 *
 * @brief HID Application Module entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_HID_H_
#define APP_HID_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief HID Application Module entry point
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_HID)

#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"         // Kernel Task Definition

#if (PS2_SUPPORT)
#include "ps2.h"             // PS2 keyboard Driver
#endif //(PS2_SUPPORT)

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// HID Application Module Environment Structure
struct app_hid_env_tag
{
    /// Connection handle
    uint8_t conidx;
    /// Keyboard timeout value
    uint16_t timeout;
    /// Internal state of the module
    uint8_t state;
    /// Timer enabled
    bool timer_enabled;
    /// Number of report that can be sent
    uint8_t nb_report;
    /// report map
    uint8_t *report_map;
    /// report map len
    uint16_t report_map_len;
    uint8_t  packet_seq;
};

struct ps2_keyboard_msg
{
    uint8_t report_id;
    uint8_t key[8];
};

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// Table of message handlers
extern const struct app_subtask_handlers app_hid_handlers;

/*
 * GLOBAL FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * Health Thermometer Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize HID Application Module
 ****************************************************************************************
 */
void app_hid_init(void);

/**
 ****************************************************************************************
 * @brief Initialize the keyboard driver so that it can be used by the application
 ****************************************************************************************
 */
void app_hid_start_keyboard(void);

/**
 ****************************************************************************************
 * @brief Add a HID Service instance in the DB
 ****************************************************************************************
 */
void app_hid_add_hids(void);

/**
 ****************************************************************************************
 * @brief Enable the HID Over GATT Profile device role
 *
 * @param[in]:  conhdl - Connection handle for the connection
 ****************************************************************************************
 */
void app_hid_enable_prf(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Send a keboard report to the peer device
 *
 * @param[in]:  report - keboard report sent by the PS2 driver
 ****************************************************************************************
 */
void app_hid_send_keyboard_report(struct ps2_keyboard_msg report);

/**
 ****************************************************************************************
 * @brief Send data report to the peer device
 *
 * @param[in]:  pdata 
 * @param[in]:  len; max HOGPD_REPORT_MAX_LEN 
 ****************************************************************************************
 */
void app_hid_send_data_report(uint8_t *pdata, uint8_t len);

/**
 ****************************************************************************************
 * @brief set report map 
 *
 * @param[in]:  report map: thre report map, must be const
 * @param[in]:  report map len;
 ****************************************************************************************
 */
void app_hid_set_report_map(uint8_t *report_map, uint16_t report_map_len);

/**
 ****************************************************************************************
 * @brief Send comand report to the peer device
 *
 * @param[in]:  pdata 
 * @param[in]:  len; max 20 
 ****************************************************************************************
 */
void app_hid_send_cmd_report(uint8_t *pdata, uint8_t len);

void app_hid_send_control_report(uint16_t keycode);

#endif //(BLE_APP_HID)

/// @} APP

#endif // APP_HID_H_
