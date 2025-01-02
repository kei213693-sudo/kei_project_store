/**
 ****************************************************************************************
 *
 * @file app_tspp_server.h
 *
 * @brief Application Module entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_TSPP_SERVER_H_
#define APP_TSPP_SERVER_H_

/**
 ****************************************************************************************
 * @addtogroup APP_COMM_TSPP_H app_tspp_server.h
 * @ingroup APP_COMMON
 *
 * @brief Application Module entry point
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration
#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"         // Kernel Task Definition
#include "co_debug.h"         // Kernel Task Definition

/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// Application Module Environment Structure
struct app_tspp_server_env_tag
{
    /// Connection handle
    //uint8_t conidx[BLE_APP_TSPPS_CONNECT_MAX];
    uint8_t ntf_en[BLE_APP_TSPPS_CONNECT_MAX];
};

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// Application environment
extern struct app_tspp_server_env_tag app_tspp_server_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_tspp_server_handlers;

/*
 * FUNCTIONS DECLARATION
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
 * @brief Initialize Application Module
 ****************************************************************************************
 */
void app_tspp_server_init(void);

/**
 ****************************************************************************************
 * @brief Add a Service instance in the DB
 ****************************************************************************************
 */
void app_tspp_server_add_service(void);

/**
 ****************************************************************************************
 * @brief Enable the Service
 ****************************************************************************************
 */
void app_tspp_server_enable_prf(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Disable the Service
 ****************************************************************************************
 */
void app_tspp_server_disable_prf(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief send data
 ****************************************************************************************
 */
void app_tspp_server_send_data(uint8_t conidx, uint8_t* pdata, uint16_t len);

/**
 ****************************************************************************************
 * @brief set max mtu
 ****************************************************************************************
 */
// Some other functions

/// @} APP

#endif // APP_TSPP_SERVER_H_
