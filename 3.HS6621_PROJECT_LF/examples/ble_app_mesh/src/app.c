/**
 ****************************************************************************************
 *
 * @file app.c
 *
 * @brief BLE mesh app entry.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h" // SW configuration

#if (BLE_APP_PRESENT)

#include "app.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "app_task.h"
#include "app_mesh.h"

#if BLE_APP_ONMICRO_DFU
#include "app_onmicro_dfu.h"
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
#if APP_MESH_TUYA
/// Default Device Name
#define APP_DFLT_DEVICE_NAME            ("TUYA-MESH   ")
#else
/// Default Device Name
#define APP_DFLT_DEVICE_NAME            ("OM-MESH   ")
#endif /* #if APP_MESH_TUYA */

/// Default Device Name Length
#define APP_DFLT_DEVICE_NAME_LEN        (sizeof(APP_DFLT_DEVICE_NAME)-1)

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
    APPM_SVC_MESH,

#if BLE_APP_ONMICRO_DFU
	APPM_SVC_ONMICRO_DFU,
#endif

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
    (appm_add_svc_func_t)app_mesh_add_profile,

#if BLE_APP_ONMICRO_DFU
	(appm_add_svc_func_t)app_onmicro_dfu_add_service,
#endif

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
/// Gapm reset cmd
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
/// Appm Init
void appm_init(void)
{
#if APP_MESH_TMALL
    appm_tmall_init();
#endif

    // Reset the application manager environment
    memset(&app_env, 0, sizeof(app_env));

    // Create APP task
    ke_task_create(TASK_APP, &TASK_DESC_APP);

    // Initialize Task state
    ke_state_set(TASK_APP, APPM_INIT);

    // Get the Device Name to add in the Advertising Data (Default one or NVDS one)
#if (NVDS_SUPPORT)
    app_env.dev_name_len = APP_DEVICE_NAME_MAX_LEN;
    if (nvds_get(PARAM_ID_DEVICE_NAME, (nvds_tag_len_t *)&(app_env.dev_name_len), app_env.dev_name) != PARAM_OK)
    {
        // Get default Device Name (No name if not enough space)
        memcpy(app_env.dev_name, APP_DFLT_DEVICE_NAME, APP_DFLT_DEVICE_NAME_LEN);
        app_env.dev_name_len = APP_DFLT_DEVICE_NAME_LEN;
    }
#endif

#if APP_MESH_TUYA
    // Concat the last byte of BDA to bt device name
    if (app_env.dev_name[app_env.dev_name_len-3] == ' ')
    {
        uint8_t lo, hi;
        uint8_t addr[GAP_BD_ADDR_LEN];
        nvds_tag_len_t length = GAP_BD_ADDR_LEN;
#if NVDS_SUPPORT
        if (nvds_get(PARAM_ID_BD_ADDRESS, &length, addr) != PARAM_OK) {
            memcpy(addr, &co_default_bdaddr, sizeof(co_default_bdaddr));
        }
#else
        memcpy(addr, &co_default_bdaddr, sizeof(co_default_bdaddr));
#endif /* #if NVDS_SUPPORT */
        lo = addr[0] & 0x0f;
        hi = addr[0] >> 4;
        app_env.dev_name[app_env.dev_name_len-1] = lo <= 9 ? '0' + lo : 'A' + lo-0xA;
        app_env.dev_name[app_env.dev_name_len-2] = hi <= 9 ? '0' + hi : 'A' + hi-0xA;
        app_env.dev_name[app_env.dev_name_len-3] = ':';
    }
#endif /* #if APP_MESH_TUYA */

#if APP_MESH_TMALL
    // Concat the last byte of BDA to bt device name
    if (app_env.dev_name[app_env.dev_name_len-3] == ' ')
    {
        uint8_t lo, hi;
        lo = mesh_tmall_tripple.mac_addr[0] & 0x0f;
        hi = mesh_tmall_tripple.mac_addr[0] >> 4;
        app_env.dev_name[app_env.dev_name_len-1] = lo <= 9 ? '0' + lo : 'A' + lo-0xA;
        app_env.dev_name[app_env.dev_name_len-2] = hi <= 9 ? '0' + hi : 'A' + hi-0xA;
        app_env.dev_name[app_env.dev_name_len-3] = ':';
    }
#endif /* #if APP_MESH_TMALL */

    /*------------------------------------------------------
     * INITIALIZE ALL MODULES
     *------------------------------------------------------*/

    // load device information:

    #if (BLE_APP_SEC)
    // Security Module
    app_sec_init();
    #endif // (BLE_APP_SEC)

    #if (BLE_APP_DIS)
    // Device Information Module
    app_dis_init();
    #endif //(BLE_APP_DIS)

    // Reset the stack
    appm_send_gapm_reset_cmd();
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
