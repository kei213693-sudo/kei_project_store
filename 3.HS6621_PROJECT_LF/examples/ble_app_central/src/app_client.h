/**
 ****************************************************************************************
 *
 * @file app_client.h
 *
 * @brief Application Module entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_CLIENT_H_
#define APP_CLIENT_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
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

#if (BLE_TSPP_COLLECTOR)
#include "tsppc_task.h"
#endif
/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// Application Module Environment Structure
struct app_client_env_tag
{
    /// Connection handle
    //uint8_t conidx;
    /// att max mtu
    uint16_t mtu[BLE_APP_CENTRAL_CONNECT_MAX];
    #if (BLE_TSPP_COLLECTOR)
    ///Existing handle values tspps
    struct tspps_content tspps;
    #endif
};

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// Application environment
extern struct app_client_env_tag app_client_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_client_handlers;

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
void app_client_init(void);

/**
 ****************************************************************************************
 * @brief Add a TSPP Service instance in the DB
 ****************************************************************************************
 */
void app_client_add_tsppc(void);

/**
 ****************************************************************************************
 * @brief discover all server
 ****************************************************************************************
 */
void app_client_disc_all_server(void);

/**
 ****************************************************************************************
 * @brief Enable the Client
 ****************************************************************************************
 */
void app_client_enable_prf(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Disenable the Client
 ****************************************************************************************
 */
void app_client_disable_prf(uint8_t conidx);
// Some other functions

/// @} APP

#endif // APP_CLIENT_H_
