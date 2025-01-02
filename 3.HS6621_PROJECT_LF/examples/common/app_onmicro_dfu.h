/**
 ****************************************************************************************
 *
 * @file app_onmicro_dfu.h
 *
 * @brief Application onmicro dfu profile entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_ONMICRO_DFU_H_
#define APP_ONMICRO_DFU_H_

/**
 ****************************************************************************************
 * @addtogroup APP_ONMICRO_DFU_PROFILE_H app_onmicro_dfu.h
 * @ingroup APP_ONMICRO_DFU
 *
 * @brief Application onmicro dfu profile entry point
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
/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Add a Service instance in the DB
 * @return void.
 ****************************************************************************************
 */
void app_onmicro_dfu_add_service(void);


/// @} APP

#endif // APP_ONMICRO_DFU_H_
