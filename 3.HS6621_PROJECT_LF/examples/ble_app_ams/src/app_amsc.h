/**
 ****************************************************************************************
 *
 * @file app_amsc.h
 *
 * @brief AMS Application Module entry point
 *
 * Copyright (C) 2020-2030 OnMicro Limited. All rights reserved.
 *
 *
 ****************************************************************************************
 */

#ifndef APP_AMSC_H_
#define APP_AMSC_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup
 *
 * @brief AMS Application Module entry point
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

/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// AMS Application Module Environment Structure
struct app_amsc_env_tag
{
    /// Connection handle
    uint16_t conhdl;
};

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// AMS Application environment
extern struct app_amsc_env_tag app_amsc_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_amsc_handlers;

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * AMS Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize AMS Application Module
 ****************************************************************************************
 */
void app_amsc_init(void);

/**
 ****************************************************************************************
 * @brief Add a AMS Service instance in the DB
 ****************************************************************************************
 */
void app_ams_add_amsc(void);

/**
 ****************************************************************************************
 * @brief Enable the AMS Service
 ****************************************************************************************
 */
void app_amsc_enable_prf(uint16_t conidx);

/**
 ****************************************************************************************
 * @brief Send a AMS cfg value
 ****************************************************************************************
 */
void app_amsc_send_ntf_cfg(uint16_t cfg);

void app_amsc_send_data_cfg(uint16_t cfg);



/// @} APP

#endif // APP_AMSC_H_
