/**
 ****************************************************************************************
 *
 * @file app_gattc.h
 *
 * @brief Application Module entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_GATTC_H_
#define APP_GATTC_H_

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

/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// Application environment

/// Table of message handlers
extern const struct app_subtask_handlers app_gattc_handlers;

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * Application Functions
 *
 ****************************************************************************************
 */
uint16_t app_gattc_send_data(uint16_t conidx, uint8_t *p_pdata, uint16_t len);
/**
 ****************************************************************************************
 * @brief Initialize Application Module
 ****************************************************************************************
 */

// Some other functions

/// @} APP

#endif // APP_CLIENT_H_
