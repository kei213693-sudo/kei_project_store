/**
 ****************************************************************************************
 *
 * @file app_tmall.h
 *
 * @brief BLE Mesh User Config For APP Tmall.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

#ifndef __APP_TMALL_H__
#define __APP_TMALL_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "rwapp_config.h"

/*********************************************************************
 * MACROS
 */
#if APP_MESH_TMALL

// Enable tmall indication feature
// Enable will increase broadcast storm, do not enable unless necessary
#define CONFIG_TMALL_IND        0

// Enable tmall indication relay feature
// Enable will increase broadcast storm more, do not enable unless necessary
#define CONFIG_TMALL_IND_RELAY  0

#endif /* APP_MESH_TMALL */

/*********************************************************************
 *  SYS MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

typedef enum
{
    ATTR_TYPE_ERROR_CODE      = 0x0000,
    ATTR_TYPE_VERSION_NUMBER  = 0xFF01,
    ATTR_TYPE_DEVICE_FEATURE  = 0xFF02,
    ATTR_TYPE_FLASH_CAPACITY  = 0xFF03,
    ATTR_TYPE_FLASH_USED      = 0xFF04,
    ATTR_TYPE_FLASH_FREE      = 0xFF05,
    ATTR_TYPE_ENGINEER_MODE   = 0xFF06,

    ATTR_TYPE_WORK_STATUS     = 0xF001,
    ATTR_TYPE_USER_ID         = 0xF002,
    ATTR_TYPE_DEVICE_NAME     = 0xF003,
    ATTR_TYPE_SCENE_MODE      = 0xF004,
    ATTR_TYPE_ONOFF_PLAN      = 0xF008,
    ATTR_TYPE_EVENT           = 0xF009,
    ATTR_TYPE_RSSI            = 0xF00A,
    ATTR_TYPE_ADJUST_ATTR     = 0xF00B,
    ATTR_TYPE_ELEMENT_NUM     = 0xF00C,
    ATTR_TYPE_SWITCH_ATTR     = 0xF00D,
    ATTR_TYPE_REMOTE_ADDR     = 0xF00E,
    ATTR_TYPE_PERIPHERAL_RSSI = 0xF00F,
    ATTR_TYPE_TIMED_ONESHOT   = 0xF010,
    ATTR_TYPE_TIMED_PERIODIC  = 0xF011,
    ATTR_TYPE_DEL_TIMER       = 0xF012,
    ATTR_TYPE_REQ_TIMER       = 0xF013,
    ATTR_TYPE_UNIX_TIME       = 0xF01F,
    ATTR_TYPE_ENABLE_NVDS     = 0xF021,

    ATTR_TYPE_ONOFF           = 0x0100,
    ATTR_TYPE_LEVEL           = 0x010A,
    ATTR_TYPE_LIGHTNESS       = 0x0121,
    ATTR_TYPE_CCT             = 0x0122,
    ATTR_TYPE_HSL             = 0x0123,
    ATTR_TYPE_DUV             = 0x01ff,

    ATTR_TYPE_MAIN_LAMP_ONOFF = 0x0534,
    ATTR_TYPE_BG_LAMP_ONOFF   = 0x0533,
    ATTR_TYPE_FAN_DIRECTION   = 0x0521,
} tmall_device_attr_type_t;

typedef enum
{
    TMALL_ERROR_CODE_SUCCESS                 = 0x00,
    TMALL_ERROR_CODE_SIG_MESH_FAULT_MASK     = 0x7F,
    TMALL_ERROR_CODE_DEV_NOT_READY           = 0x80,
    TMALL_ERROR_CODE_ATTR_TYPE_NOT_SUPPORT   = 0x81,
    TMALL_ERROR_CODE_OPERATION_NOT_SUPPORT   = 0x82,
    TMALL_ERROR_CODE_ATTR_PARAMS_ERR         = 0x83,
    TMALL_ERROR_CODE_DEV_STATE_ERR           = 0x84,
    TMALL_ERROR_CODE_INDEX_NOT_FOUND         = 0x85,
    TMALL_ERROR_CODE_STORAGE_IS_FULL         = 0x86,
    TMALL_ERROR_CODE_FORMAT_ERR              = 0x87,
    TMALL_ERROR_CODE_INTERNAL_ERR            = 0xF0,
} tmall_device_error_code_t;

/// Tmall-genie tripple data, which comes from https://iot.aligenie.com
typedef struct {
    uint32_t product_id;
    uint8_t  secret[16];
    uint8_t  mac_addr[6];
} tmall_tripple_t;

/// Tmall-genie attribute handler
typedef tmall_device_error_code_t (*app_tmall_attr_op_t)(tmall_device_attr_type_t attr_type, uint8_t *p_attr_value, uint8_t *p_len_value);
typedef tmall_device_error_code_t (*app_tmall_cfm_op_t)(uint8_t *p_attr_value);

/*********************************************************************
 * EXTERN VARIABLES
 */
/// Tmall-genie defines the format of device UUID, and static OOB derived from tripple data.
extern tmall_tripple_t mesh_tmall_tripple;

/*********************************************************************
 * EXTERN FUNCTIONS
 */
/**
 ****************************************************************************************
 * @brief APP Tmall init
 ****************************************************************************************
 */
void appm_tmall_init(void);

/**
 ****************************************************************************************
 * @brief APP Tmall register attribute handler
 ****************************************************************************************
 */
void mm_vnd_tmall_register_attr_handler(app_tmall_attr_op_t cb_get, app_tmall_attr_op_t cb_set,app_tmall_cfm_op_t cb_cfm);

/**
 ****************************************************************************************
 * @brief APP Tmall indication
 ****************************************************************************************
 */
bool mm_vnd_tmall_indication(tmall_device_attr_type_t attr_type, uint8_t tid, uint8_t *p_attr_value);

/**
 ****************************************************************************************
 * @brief APP Tmall indication delay ms
 ****************************************************************************************
 */
uint16_t appm_tmall_ind_delay_ms(uint32_t ms);

/**
 ****************************************************************************************
 * @brief  APP Tmall binding
 ****************************************************************************************
 */
void appm_tmall_self_bind(uint8_t app_key_lid);

/**
 ****************************************************************************************
 * @brief Add a required service in the database
 ****************************************************************************************
 */
void tmall_nvds_loaded_handler(void);

/**
 ****************************************************************************************
 * @brief Add a required service in the database
 ****************************************************************************************
 */
void tmall_nvds_store_handler(void);

#endif /* __APP_TMALL_H__ */

/** @} */

