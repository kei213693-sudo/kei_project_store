/**
 ****************************************************************************************
 *
 * @file app_atvv.h
 *
 * @brief Application Module entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_ATVV_SERVER_H_
#define APP_ATVV_SERVER_H_

/**
 ****************************************************************************************
 * @addtogroup APP_COMMON_ATVV_H app_atvv.h
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
#if (BLE_ATVV_SERVER)
/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// Application Module Environment Structure
struct app_atvv_server_env_tag {
    /// Connection handle
    //uint8_t conidx[BLE_APP_ATVVS_CONNECT_MAX];
    uint8_t ntf_en[BLE_APP_ATVVS_CONNECT_MAX][2];
    uint16_t seq_num;
};

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// Application environment
extern struct app_atvv_server_env_tag app_atvv_server_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_atvv_server_handlers;

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
void app_atvv_server_init(void);

/**
 ****************************************************************************************
 * @brief Add a Service instance in the DB
 ****************************************************************************************
 */
void app_atvv_server_add_service(void);

/**
 ****************************************************************************************
 * @brief Enable the Service
 ****************************************************************************************
 */
void app_atvv_server_enable_prf(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Disable the Service
 ****************************************************************************************
 */
void app_atvv_server_disable_prf(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief send data
 ****************************************************************************************
 */
void app_atvv_server_send_data(uint8_t conidx, uint8_t* pdata, uint8_t len);

/**
 ****************************************************************************************
 * @brief send audio to remote device
 ****************************************************************************************
 */
void app_atvv_notify_audio_frame(uint8_t *input, uint8_t len, int16_t prevsample, uint16_t previndex);
/**
 ****************************************************************************************
 * @brief send control
 ****************************************************************************************
 */
void app_atvv_server_send_control(uint8_t conidx, uint8_t* pdata, uint16_t len);

/**
 ****************************************************************************************
 * @brief set max mtu
 ****************************************************************************************
 */
// Some other functions

#endif
/// @} APP

#endif // APP_ATVV_SERVER_H_
