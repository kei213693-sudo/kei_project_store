/**
 ****************************************************************************************
 *
 * @file app_wechat.h
 *
 * @brief wechat Application Module entry point
 *
 * Copyright (C) 2020-2030 OnMicro Limited. All rights reserved.
 *
 *
 ****************************************************************************************
 */
#ifndef APP_WECHAT_H_
#define APP_WECHAT_H_
/**
 ****************************************************************************************
 * @addtogroup APP_COMM_WECHAT_H app_wechat.h
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
#include "gap.h"         // Kernel Task Definition

/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// fff0s Application Module Environment Structure
struct app_wechat_env_tag
{
    /// Connection handle
    uint8_t conidx;
    /// the bluetooth address
    bd_addr_t addr;
};

/// the pkt length
#define PKT_LENGTH      120

/// the wechat recv pkt
typedef struct
{
    /// the packet buffer  
    uint8_t* buf;
    /// the buffer len
    uint8_t len;
    /// the offset of pakcet
    uint8_t offset;
}WeChat_Recv_Pkt;

/// the wechat send pkt
typedef struct
{
    /// the buffer
    uint8_t buf[PKT_LENGTH];
    /// the send data len
    uint8_t len;
    /// the send data offset
    uint8_t offset;
}WeChat_Send_Pkt;



/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// fff0s Application environment
extern struct app_wechat_env_tag app_wechat_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_wechat_handlers;
/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize wechat Application Module
 ****************************************************************************************
 */
void app_wechat_init(void);
/**
 ****************************************************************************************
 * @brief Add a wechat Service instance in the DB
 ****************************************************************************************
 */
void app_wechat_add_wechat(void);
/**
 ****************************************************************************************
 * @brief Enable the wechat Service
 ****************************************************************************************
 */
void app_wechat_enable_prf(uint8_t conidx, bd_addr_t *mac_addr);
/**
 ****************************************************************************************
 * @brief Send a wechat data value
 ****************************************************************************************
 */
void app_wechat_indicate_data(uint8_t* buf, uint8_t len);
/**
 ****************************************************************************************
 * @brief Get device mac address
 ****************************************************************************************
 */
void app_wechat_get_mac_addr(uint8_t *mac_address);

/// @} APP

#endif // APP_WECHAT_H_
