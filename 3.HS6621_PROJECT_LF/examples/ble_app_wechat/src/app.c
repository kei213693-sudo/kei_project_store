/**
 ****************************************************************************************
 *
 * @file app.c
 *
 * @brief Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRESENT)

#include <string.h>
#include "co_utils.h"
#include "app_task.h"                // Application task Definition
#include "app.h"                     // Application Definition
#include "gap.h"                     // GAP Definition
#include "gapm_task.h"               // GAP Manager Task API
#include "gapc_task.h"               // GAP Controller Task API

#include "co_bt.h"                   // Common BT Definition
#include "co_math.h"                 // Common Maths Definition
#include "co_endian.h"

#include "nvds.h"                    // NVDS Definitions
#include "app_sec.h"                     // sec Definitions
#include "app_wechat.h"


/*
 * DEFINES
 ****************************************************************************************
 */

/// Default Device Name
#define APP_DFLT_DEVICE_NAME            ("RW-BLE-WECHAT")
#define APP_DFLT_DEVICE_NAME_LEN        (sizeof(APP_DFLT_DEVICE_NAME)-1)

#define APP_WECHAT_ADV_DATA_UUID        "\x03\x03\xe7\xfe"
#define APP_WECHAT_ADV_DATA_UUID_LEN    (4)
#define APP_WECHAT_ADV_DATA_VENDOR_SPEC "\x09\xFF\xFF\xFF\x00\x00\x00\x00\x00\x00"
#define APP_WECHAT_ADV_DATA_VENDOR_SPEC_LEN (sizeof(APP_WECHAT_ADV_DATA_VENDOR_SPEC)-1)

/**
 * Default Scan response data
 * --------------------------------------------------------------------------------------
 * x09                             - Length
 * xFF                             - Vendor specific advertising type
 * x00\x60\x52\x57\x2D\x42\x4C\x45 - "RW-BLE"
 * --------------------------------------------------------------------------------------
 */
#define APP_SCNRSP_DATA         ""
#define APP_SCNRSP_DATA_LEN     (0)

/**
 * Advertising Parameters
 */
#define APP_ADV_FAST_INTERVAL     0x0028   /**< Fast advertising interval (in units of 0.625 ms. This value corresponds to 25 ms.). */
#define APP_ADV_SLOW_INTERVAL     0x0C80   /**< Slow advertising interval (in units of 0.625 ms. This value corrsponds to 2 seconds). */

#define APP_ADV_FAST_TIMEOUT      0 //30       /**< The duration of the fast advertising period (in seconds). max 655, or 0 is no timeout*/
#define APP_ADV_SLOW_TIMEOUT      0 //180      /**< The duration of the slow advertising period (in seconds). max 655. or 0 is no timeout*/

#define APP_ADV_DIRECTED_TIMEOUT  0       /**< The duration of the fast advertising period (in seconds). mas 655. or 0 is high duty cycle*/

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef void (*appm_add_svc_func_t)(void);

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// List of service to add in the database
enum appm_svc_list
{
    APPM_SVC_WECHAT,

    APPM_SVC_LIST_STOP,
};

/*
 * LOCAL VARIABLES DEFINITIONS
 ****************************************************************************************
 */

/// Application Task Descriptor
extern const struct ke_task_desc TASK_DESC_APP;

/// List of functions used to create the database
static const appm_add_svc_func_t appm_add_svc_func_list[APPM_SVC_LIST_STOP] =
{
    (appm_add_svc_func_t)app_wechat_add_wechat,
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Application Environment Structure
struct app_env_tag app_env;

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static void appm_build_adv_data(uint16_t max_length, uint16_t *p_length, uint8_t *p_buf)
{
    // Remaining Length
    uint8_t rem_len = max_length;
    // Set list of UUIDs
    memcpy(p_buf, APP_WECHAT_ADV_DATA_UUID, APP_WECHAT_ADV_DATA_UUID_LEN);
    *p_length += APP_WECHAT_ADV_DATA_UUID_LEN;
    p_buf += APP_WECHAT_ADV_DATA_UUID_LEN;

    // Set vendor spec
#if (NVDS_SUPPORT)
    nvds_tag_len_t len=6;
    if (nvds_get(NVDS_TAG_BD_ADDRESS, &len, &app_env.mac_addr.addr.addr[0]) != NVDS_OK)
    {
        memcpy(&app_env.mac_addr.addr.addr[0], &co_default_bdaddr,sizeof(co_default_bdaddr));
    }
#endif //(NVDS_SUPPORT)

    memcpy(p_buf, APP_WECHAT_ADV_DATA_VENDOR_SPEC, APP_WECHAT_ADV_DATA_VENDOR_SPEC_LEN);
    co_bswap((uint8_t*)&p_buf[4], &app_env.mac_addr.addr.addr[0], GAP_BD_ADDR_LEN);
    *p_length += APP_WECHAT_ADV_DATA_VENDOR_SPEC_LEN;
    p_buf += APP_WECHAT_ADV_DATA_VENDOR_SPEC_LEN;

    // Sanity check
    ASSERT_ERR(rem_len >= max_length);

    // Get remaining space in the Advertising Data - 2 bytes are used for name length/flag
    rem_len -= *p_length;

    // Check if additional data can be added to the Advertising data - 2 bytes needed for type and length
    if (rem_len > 2)
    {
        uint8_t dev_name_length = co_min(app_env.dev_name_len, (rem_len - 2));

        // Device name length
        *p_buf = dev_name_length + 1;
        // Device name flag (check if device name is complete or not)
        *(p_buf + 1) = (dev_name_length == app_env.dev_name_len) ? '\x08' : '\x09';
        // Copy device name
        memcpy(p_buf + 2, app_env.dev_name, dev_name_length);

        // Update advertising data length
        *p_length += (dev_name_length + 2);
    }
}

static void appm_advertising_init(void)
{
    struct app_adv_modes_config_tag adv_config;

    adv_config.whitelist_enabled = false;
    adv_config.directed_enabled = false;
    adv_config.directed_timeout = APP_ADV_DIRECTED_TIMEOUT;
    adv_config.fast_enabled = true;
    adv_config.fast_interval = APP_ADV_FAST_INTERVAL;
    adv_config.fast_timeout = APP_ADV_FAST_TIMEOUT;
    adv_config.slow_enabled = false;
    adv_config.slow_interval = APP_ADV_SLOW_INTERVAL;
    adv_config.slow_timeout = APP_ADV_SLOW_TIMEOUT;

    appm_adv_init(NULL, &adv_config);

    uint8_t default_adv_data[ADV_DATA_LEN - 3];
    uint16_t adv_len = 0;
    appm_build_adv_data(ADV_DATA_LEN - 3, &adv_len, (uint8_t*)default_adv_data);
    appm_adv_set_adv_data((uint8_t*)default_adv_data, adv_len);

    //uint8_t res_data[APP_ADV_MAX_LEN];
    //uint8_t res_data_len = APP_SCNRSP_DATA_LEN;
    //memcpy(res_data, APP_SCNRSP_DATA, sizeof(APP_SCNRSP_DATA));
    //appm_adv_set_res_data(res_data, res_data_len);
}


static void appm_send_gapm_reset_cmd(void)
{
    // Reset the stack
    struct gapm_reset_cmd *p_cmd = KE_MSG_ALLOC(GAPM_RESET_CMD,
                                                TASK_GAPM, TASK_APP,
                                                gapm_reset_cmd);

    p_cmd->operation = GAPM_RESET;

    ke_msg_send(p_cmd);
}

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void appm_init()
{
    // Reset the application manager environment
    memset(&app_env, 0, sizeof(app_env));

    // Create APP task
    ke_task_create(TASK_APP, &TASK_DESC_APP);

    // Initialize Task state
    ke_state_set(TASK_APP, APPM_INIT);

    // Get the Device Name to add in the Advertising Data (Default one or NVDS one)
    #if (NVDS_SUPPORT)
    app_env.dev_name_len = APP_DEVICE_NAME_MAX_LEN;
    if (nvds_get(NVDS_TAG_DEVICE_NAME, &(app_env.dev_name_len), app_env.dev_name) != NVDS_OK)
    #endif //(NVDS_SUPPORT)
    {
        // Get default Device Name (No name if not enough space)
        memcpy(app_env.dev_name, APP_DFLT_DEVICE_NAME, APP_DFLT_DEVICE_NAME_LEN);
        app_env.dev_name_len = APP_DFLT_DEVICE_NAME_LEN;

        // TODO update this value per profiles
    }


    /*------------------------------------------------------
     * INITIALIZE ALL MODULES
     *------------------------------------------------------*/

    // load device information:

    #if (DISPLAY_SUPPORT)
    app_display_init();
    #endif //(DISPLAY_SUPPORT)

    #if (BLE_APP_SEC)
    // Security Module
    app_sec_init();
    #endif // (BLE_APP_SEC)

    #if (BLE_APP_HT)
    // Health Thermometer Module
    app_ht_init();
    #endif //(BLE_APP_HT)

    #if (BLE_APP_DIS)
    // Device Information Module
    app_dis_init();
    #endif //(BLE_APP_DIS)

    #if (BLE_APP_WECHAT)
    // Device Information Module
    app_wechat_init();
    #endif //(BLE_APP_WECHAT)

    // Reset the stack
    appm_send_gapm_reset_cmd();

    // start adversting
    appm_advertising_init();
}

bool appm_add_svc(void)
{
    // Indicate if more services need to be added in the database
    bool more_svc = false;

    // Check if another should be added in the database
    if (app_env.next_svc != APPM_SVC_LIST_STOP)
    {
        ASSERT_INFO(appm_add_svc_func_list[app_env.next_svc] != NULL, app_env.next_svc, 1);

        // Call the function used to add the required service
        appm_add_svc_func_list[app_env.next_svc]();

        // Select following service to add
        app_env.next_svc++;
        more_svc = true;

        // Go to the create db state
        ke_state_set(TASK_APP, APPM_CREATE_DB);
    }

    return more_svc;
}

void appm_disconnect(uint8_t conidx)
{
    struct gapc_disconnect_cmd *cmd = KE_MSG_ALLOC(GAPC_DISCONNECT_CMD,
                                                   KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                                                   gapc_disconnect_cmd);

    cmd->operation = GAPC_DISCONNECT;
    cmd->reason    = CO_ERROR_REMOTE_USER_TERM_CON;

    // Send the message
    ke_msg_send(cmd);
}

void appm_update_param(struct gapc_conn_param *conn_param)
{
    // Prepare the GAPC_PARAM_UPDATE_CMD message
    struct gapc_param_update_cmd *cmd = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD,
                                                     KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                                                     gapc_param_update_cmd);

    cmd->operation  = GAPC_UPDATE_PARAMS;
    cmd->intv_min   = conn_param->intv_min;
    cmd->intv_max   = conn_param->intv_max;
    cmd->latency    = conn_param->latency;
    cmd->time_out   = conn_param->time_out;

    // not used by a slave device
    cmd->ce_len_min = 0xFFFF;
    cmd->ce_len_max = 0xFFFF;

    // Send the message
    ke_msg_send(cmd);
}

uint8_t appm_get_dev_name(uint8_t* name)
{
    // copy name to provided pointer
    memcpy(name, app_env.dev_name, app_env.dev_name_len);
    // return name length
    return app_env.dev_name_len;
}

void appm_get_mac_addr(void)
{
    struct gapm_get_dev_info_cmd *req = ke_msg_alloc(GAPM_GET_DEV_INFO_CMD,
                                                    TASK_GAPM, TASK_APP,
                                                    sizeof(struct gapm_get_dev_info_cmd));
    req->operation = GAPM_GET_DEV_BDADDR;
    ke_msg_send(req);
}

#endif //(BLE_APP_PRESENT)

/// @} APP
