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

#include "app_task.h"                // Application task Definition
#include "app.h"                     // Application Definition
#include "gap.h"                     // GAP Definition
#include "gapm_task.h"               // GAP Manager Task API
#include "gapc_task.h"               // GAP Controller Task API

#include "co_bt.h"                   // Common BT Definition
#include "co_math.h"                 // Common Maths Definition
#include "prf.h"
#if (BLE_APP_SEC)
#include "app_sec.h"                 // Application security Definition
#endif // (BLE_APP_SEC)

#if (BLE_APP_HT)
#include "app_ht.h"                  // Health Thermometer Application Definitions
#endif //(BLE_APP_HT)

#if (BLE_APP_DIS)
#include "app_dis.h"                 // Device Information Service Application Definitions
#include "diss.h"
#endif //(BLE_APP_DIS)

#if (BLE_APP_BATT)
#include "app_batt.h"                // Battery Application Definitions
#include "bass.h"
#endif //(BLE_APP_DIS)

#if (BLE_APP_HID)
#include "app_hid.h"                 // HID Application Definitions
#include "hogpd.h"
#endif //(BLE_APP_HID)

#if (DISPLAY_SUPPORT)
#include "app_display.h"             // Application Display Definition
#endif //(DISPLAY_SUPPORT)

#if (BLE_APP_AM0)
#include "app_am0.h"                 // Audio Mode 0 Application
#endif //(BLE_APP_AM0)

#if (NVDS_SUPPORT)
#include "nvds.h"                    // NVDS Definitions
#endif //(NVDS_SUPPORT)

#include "att.h"
/*
 * DEFINES
 ****************************************************************************************
 */

/// Default Device Name
#if (BLE_APP_HID)
// HID Mouse
#define APP_DFLT_DEVICE_NAME            ("Hid Mouse")
#else
#define APP_DFLT_DEVICE_NAME            ("BLE-DEV")
#endif
#define APP_DFLT_DEVICE_NAME_LEN        (sizeof(APP_DFLT_DEVICE_NAME)-1)


/**
 * Default Adv response data
 */
#define APP_DEFAULT_ADV_DATA         "\x03\x03\x12\x18\x03\x19\xC2\x03\x0a\x09\x48\x69\x64\x20\x4d\x6f\x75\x73\x65"
#define APP_DEFAULT_ADV_DATA_LEN     sizeof(APP_DEFAULT_ADV_DATA)

/**
 * Default Scan response data
 * --------------------------------------------------------------------------------------
 * x09                             - Length
 * xFF                             - Vendor specific advertising type
 * x00\x60\x52\x57\x2D\x42\x4C\x45 - "RW-BLE"
 * --------------------------------------------------------------------------------------
 */
#define APP_SCNRSP_DATA         "\x09\xFF\x01\xBF\x48\x53\x2D\x42\x4C\x45"
#define APP_SCNRSP_DATA_LEN     (10)

/**
 * Advertising Parameters
 */
#if (BLE_APP_HID)

#define APP_ADV_FAST_INTERVAL     0x0028   /**< Fast advertising interval (in units of 0.625 ms. This value corresponds to 25 ms.). */
#define APP_ADV_SLOW_INTERVAL     0x0C80   /**< Slow advertising interval (in units of 0.625 ms. This value corrsponds to 2 seconds). */

#define APP_ADV_FAST_TIMEOUT      30       /**< The duration of the fast advertising period (in seconds). max 655, or 0 is no timeout*/
#define APP_ADV_SLOW_TIMEOUT      180      /**< The duration of the slow advertising period (in seconds). max 655. or 0 is no timeout*/

#define APP_ADV_DIRECTED_TIMEOUT  0       /**< The duration of the fast advertising period (in seconds). mas 655. or 0 is high duty cycle*/
#endif //(BLE_APP_HID)

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
    #if (BLE_APP_HT)
    APPM_SVC_HTS,
    #endif //(BLE_APP_HT)
    #if (BLE_APP_DIS)
    APPM_SVC_DIS,
    #endif //(BLE_APP_DIS)
    #if (BLE_APP_BATT)
    APPM_SVC_BATT,
    #endif //(BLE_APP_BATT)
    #if (BLE_APP_HID)
    APPM_SVC_HIDS,
    #endif //(BLE_APP_HID)
    #if (BLE_APP_AM0)
    APPM_SVC_AM0_HAS,
    #endif //(BLE_APP_AM0)

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
    #if (BLE_APP_HT)
    (appm_add_svc_func_t)app_ht_add_hts,
    #endif //(BLE_APP_HT)
    #if (BLE_APP_DIS)
    (appm_add_svc_func_t)app_dis_add_dis,
    #endif //(BLE_APP_DIS)
    #if (BLE_APP_BATT)
    (appm_add_svc_func_t)app_batt_add_bas,
    #endif //(BLE_APP_BATT)
    #if (BLE_APP_HID)
    (appm_add_svc_func_t)app_hid_add_hids,
    #endif //(BLE_APP_HID)
    #if (BLE_APP_AM0)
    (appm_add_svc_func_t)app_am0_add_has,
    #endif //(BLE_APP_AM0)
};

static void appm_reg_svc_itf(void)
{
    struct prf_itf_pair itf_pair[] = {
        #if (BLE_APP_DIS)
        { TASK_ID_DISS, diss_prf_itf_get() },
        #endif //(BLE_APP_DIS)
        #if (BLE_APP_HID)
        { TASK_ID_HOGPD, hogpd_prf_itf_get() },
        #endif //(BLE_APP_HID)
    };
    prf_itf_register(itf_pair, sizeof(itf_pair)/sizeof(itf_pair[0]));
}
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

static void appm_send_gapm_reset_cmd(void)
{
    // Reset the stack
    struct gapm_reset_cmd *p_cmd = KE_MSG_ALLOC(GAPM_RESET_CMD,
                                                TASK_GAPM, TASK_APP,
                                                gapm_reset_cmd);

    p_cmd->operation = GAPM_RESET;

    ke_msg_send(p_cmd);
}

static void appm_advertising_init(void)
{
    struct app_adv_modes_config_tag adv_config;

    adv_config.whitelist_enabled = true;
    adv_config.directed_enabled = true;
    adv_config.directed_timeout = APP_ADV_DIRECTED_TIMEOUT;
    adv_config.fast_enabled = true;
    adv_config.fast_interval = APP_ADV_FAST_INTERVAL;
    adv_config.fast_timeout = APP_ADV_FAST_TIMEOUT;
    adv_config.slow_enabled = true;
    adv_config.slow_interval = APP_ADV_SLOW_INTERVAL;
    adv_config.slow_timeout = 0; //APP_ADV_SLOW_TIMEOUT;

#if 0
    struct app_adv_data_tag adv_data;

    adv_data.include = APP_ADV_DATA_INCLUDE_APPEARANCE|APP_ADV_DATA_INCLUDE_NAME|APP_ADV_DATA_INCLUDE_UUID;
    adv_data.appearance = 0x03C2; // HID+mouse
    adv_data.name_type = APP_ADV_DATATYPE_COMPLETE_LOCAL_NAME;
    adv_data.uuid.type = APP_ADV_DATATYPE_16BIT_SERVICE_UUID_COMPLETE;
    adv_data.uuid.len = ATT_UUID_16_LEN;
    adv_data.uuid.uuid[0] = (ATT_SVC_HID&0xFF);
    adv_data.uuid.uuid[1] = ((ATT_SVC_HID>>8)&0xFF);

    appm_adv_init(&adv_data, &adv_config);
#else
    appm_adv_init(NULL, &adv_config);

    appm_adv_set_adv_data((uint8_t*)(APP_DEFAULT_ADV_DATA), APP_DEFAULT_ADV_DATA_LEN);
#endif
    uint8_t res_data[APP_ADV_MAX_LEN];
    uint8_t res_data_len = APP_SCNRSP_DATA_LEN;
    memcpy(res_data, APP_SCNRSP_DATA, APP_SCNRSP_DATA_LEN);
    appm_adv_set_res_data(res_data, res_data_len);
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

    #if (BLE_APP_HID)
    // HID Module
    app_hid_init();
    #endif //(BLE_APP_HID)

    #if (BLE_APP_BATT)
    // Battery Module
    app_batt_init();
    #endif //(BLE_APP_BATT)

    #if (BLE_APP_AM0)
    // Audio Mode 0 Module
    app_am0_init();
    #endif //(BLE_APP_AM0)

    // Reset the stack
    appm_send_gapm_reset_cmd();

    appm_advertising_init();
    
    appm_reg_svc_itf();
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
#endif //(BLE_APP_PRESENT)

/// @} APP
