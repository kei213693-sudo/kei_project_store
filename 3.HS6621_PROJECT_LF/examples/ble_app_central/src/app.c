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

#if (BLE_APP_SEC)
#include "app_sec.h"                 // Application security Definition
#endif // (BLE_APP_SEC)

#if (NVDS_SUPPORT)
#include "nvds.h"                    // NVDS Definitions
#endif //(NVDS_SUPPORT)
#include "app_client.h"

/*
 * DEFINES
 ****************************************************************************************
 */

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
#if (BLE_TSPP_COLLECTOR)
    APPM_SVC_TSPPC,
#endif
    APPM_SVC_LIST_STOP,
};
/*
 * LOCAL VARIABLES DEFINITIONS
 ****************************************************************************************
 */
/// Application Task Descriptor
extern const struct ke_task_desc TASK_DESC_APP;
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Application Environment Structure
struct app_env_tag app_env;

#if (BLE_APP_USE_CLIENT_TASK)
/// List of functions used to create the database
static const appm_add_svc_func_t appm_add_svc_func_list[APPM_SVC_LIST_STOP] =
{
#if (BLE_TSPP_COLLECTOR)
    (appm_add_svc_func_t)app_client_add_tsppc,
#endif
};
#endif
/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
static void appm_send_gapm_reset_cmd(void)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    // Reset the stack
    struct gapm_reset_cmd *p_cmd = KE_MSG_ALLOC(GAPM_RESET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_reset_cmd);

    p_cmd->operation = GAPM_RESET;

    ke_msg_send(p_cmd);
}

uint8_t appm_connect_by_conhdl(uint8_t conhdl)
{
    uint8_t i=0;
    for(i=0;i<BLE_APP_CENTRAL_CONNECT_MAX;i++)
    {
        if(app_env.init[i].conhdl == conhdl)
        {
            break;
        }
    }
    return i;
}

uint8_t appm_connect_by_addr(struct gap_bdaddr addr)
{
    uint8_t i=0;
    for(i=0;i<BLE_APP_CENTRAL_CONNECT_MAX;i++)
    {
        if(memcmp(&(app_env.init[i].trans_addr),&addr, sizeof(struct gap_bdaddr)) == 0)
        {
            break;
        }
    }
    return i;
}

uint8_t appm_connect_by_actv_idx(uint8_t idx)
{
    uint8_t i=0;
    for(i=0;i<BLE_APP_CENTRAL_CONNECT_MAX;i++)
    {
        if(app_env.init[i].init_actv_idx == idx)
        {
            break;
        }
    }
    return i;
}

uint8_t appm_connect_by_actv_state(uint8_t state)
{
    uint8_t i=0;
    for(i=0;i<BLE_APP_CENTRAL_CONNECT_MAX;i++)
    {
        if(app_env.init[i].init_state == state)
        {
            break;
        }
    }
    return i;
}

static void appm_start_init(uint8_t idx)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_START_CMD message
    struct gapm_activity_start_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_START_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_start_cmd);

    p_cmd->operation = GAPM_START_ACTIVITY;
    p_cmd->actv_idx = app_env.init[idx].init_actv_idx;
    p_cmd->u_param.init_param.scan_param_1m.scan_intv = SCAN_INTERVAL_DFT;
    p_cmd->u_param.init_param.scan_param_1m.scan_wd = SCAN_WINDOW_DFT;
    p_cmd->u_param.init_param.scan_param_coded.scan_intv = SCAN_INTERVAL_DFT;
    p_cmd->u_param.init_param.scan_param_coded.scan_wd = SCAN_WINDOW_DFT;
    
    p_cmd->u_param.init_param.type = GAPM_INIT_TYPE_DIRECT_CONN_EST;
    p_cmd->u_param.init_param.prop = GAPM_INIT_PROP_1M_BIT|GAPM_INIT_PROP_2M_BIT|GAPM_INIT_PROP_CODED_BIT;
    p_cmd->u_param.init_param.conn_to = 0;
    p_cmd->u_param.init_param.conn_param_1m.conn_intv_min = 0x0c;
    p_cmd->u_param.init_param.conn_param_1m.conn_intv_max = 0x0c;
    p_cmd->u_param.init_param.conn_param_1m.conn_latency = 0;
    p_cmd->u_param.init_param.conn_param_1m.supervision_to = 100;

    p_cmd->u_param.init_param.conn_param_2m.conn_intv_min = 0x0c;
    p_cmd->u_param.init_param.conn_param_2m.conn_intv_max = 0x0c;
    p_cmd->u_param.init_param.conn_param_2m.conn_latency = 0;
    p_cmd->u_param.init_param.conn_param_2m.supervision_to = 100;

    p_cmd->u_param.init_param.conn_param_coded.conn_intv_min = 0x0c;
    p_cmd->u_param.init_param.conn_param_coded.conn_intv_max = 0x0c;
    p_cmd->u_param.init_param.conn_param_coded.conn_latency = 0;
    p_cmd->u_param.init_param.conn_param_coded.supervision_to = 100;
    //p_cmd->u_param.init_param.conn_param_1m.ce_len_min = 0x100;
    //p_cmd->u_param.init_param.conn_param_1m.ce_len_max = 0x100;
    memcpy(&(p_cmd->u_param.init_param.peer_addr), &(app_env.init[idx].trans_addr), sizeof(struct gap_bdaddr));

    // Send the message
    ke_msg_send(p_cmd);

    // Keep the current operation
    app_env.init[idx].init_state = APP_INIT_STATE_STARTING;
    // And the next expected operation code for the command completed event
    app_env.init[idx].init_op = GAPM_START_ACTIVITY;
}

void appm_stop_init(uint8_t idx)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_STOP_CMD message
    struct gapm_activity_stop_cmd *cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_STOP_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_stop_cmd);

    // Fill the allocated kernel message
    cmd->operation = GAPM_STOP_ACTIVITY;
    cmd->actv_idx = app_env.init[idx].init_actv_idx;

    // Send the message
    ke_msg_send(cmd);

    // Update advertising state
    app_env.init[idx].init_state = APP_INIT_STATE_STOPPING;
    // And the next expected operation code for the command completed event
    app_env.init[idx].init_op = GAPM_STOP_ACTIVITY;
}

void appm_create_init(void)
{
    uint8_t idx = 0;

    if (app_env.w4connect <=0) return;

    idx = app_env.w4connect-1;
    //log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_CREATE_CMD message
    struct gapm_activity_create_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_CREATE_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_create_cmd);

    // Set operation code
    p_cmd->operation = GAPM_CREATE_INIT_ACTIVITY;

    // Fill the allocated kernel message
    p_cmd->own_addr_type = GAPM_STATIC_ADDR;

    // Send the message
    ke_msg_send(p_cmd);

    // Keep the current operation
    app_env.init[idx].init_state = APP_INIT_STATE_CREATING;
    // And the next expected operation code for the command completed event
    app_env.init[idx].init_op = GAPM_CREATE_INIT_ACTIVITY;
}

void appm_delete_init(uint8_t idx)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_CREATE_CMD message
    struct gapm_activity_delete_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_DELETE_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_delete_cmd);

    // Set operation code
    p_cmd->operation = GAPM_DELETE_ACTIVITY;

    // Send the message
    ke_msg_send(p_cmd);

    // Keep the current operation
    app_env.init[idx].init_state = APP_INIT_STATE_DELETING;
    // And the next expected operation code for the command completed event
    app_env.init[idx].init_op = GAPM_DELETE_ACTIVITY;
}

static void appm_create_scan(void)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_CREATE_CMD message
    struct gapm_activity_create_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_CREATE_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_create_cmd);

    // Set operation code
    p_cmd->operation = GAPM_CREATE_SCAN_ACTIVITY;

    // Fill the allocated kernel message
    p_cmd->own_addr_type = GAPM_STATIC_ADDR;

    // Send the message
    ke_msg_send(p_cmd);

    // Keep the current operation
    app_env.scan_state = APP_SCAN_STATE_CREATING;
    // And the next expected operation code for the command completed event
    app_env.scan_op = GAPM_CREATE_SCAN_ACTIVITY;
}

void appm_delete_scan(void)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_CREATE_CMD message
    struct gapm_activity_delete_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_DELETE_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_delete_cmd);

    // Set operation code
    p_cmd->operation = GAPM_DELETE_ACTIVITY;

    // Send the message
    ke_msg_send(p_cmd);

    // Keep the current operation
    // Keep the current operation
    app_env.scan_state = APP_SCAN_STATE_DELETING;
    // And the next expected operation code for the command completed event
    app_env.scan_op = GAPM_DELETE_ACTIVITY;
}

static void appm_start_scan(void)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_START_CMD message
    struct gapm_activity_start_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_START_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_start_cmd);

    p_cmd->operation = GAPM_START_ACTIVITY;
    p_cmd->actv_idx = app_env.scan_actv_idx;
    p_cmd->u_param.scan_param.type = GAPM_SCAN_TYPE_GEN_DISC;
    p_cmd->u_param.scan_param.prop = GAPM_SCAN_PROP_PHY_1M_BIT|GAPM_SCAN_PROP_PHY_CODED_BIT|GAPM_SCAN_PROP_ACTIVE_1M_BIT|GAPM_SCAN_PROP_ACTIVE_CODED_BIT|GAPM_SCAN_PROP_ACCEPT_RPA_BIT|GAPM_SCAN_PROP_FILT_TRUNC_BIT;
    p_cmd->u_param.scan_param.dup_filt_pol = GAPM_DUP_FILT_EN;
    p_cmd->u_param.scan_param.scan_param_1m.scan_intv = 0x200;
    p_cmd->u_param.scan_param.scan_param_1m.scan_wd = 0x100;
    p_cmd->u_param.scan_param.scan_param_coded.scan_intv = 0x200;
    p_cmd->u_param.scan_param.scan_param_coded.scan_wd = 0x100;
    p_cmd->u_param.scan_param.duration = GAP_TMR_GEN_DISC_SCAN;
    p_cmd->u_param.scan_param.period = 0;
    // Send the message
    ke_msg_send(p_cmd);

    // Keep the current operation
    app_env.scan_state = APP_SCAN_STATE_STARTING;
    // And the next expected operation code for the command completed event
    app_env.scan_op = GAPM_START_ACTIVITY;
}

void appm_stop_scan(void)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_STOP_CMD message
    struct gapm_activity_stop_cmd *cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_STOP_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_stop_cmd);

    // Fill the allocated kernel message
    cmd->operation = GAPM_STOP_ACTIVITY;
    cmd->actv_idx = app_env.scan_actv_idx;

    // Send the message
    ke_msg_send(cmd);

    // Update advertising state
    app_env.scan_state = APP_SCAN_STATE_STOPPING;
    // And the next expected operation code for the command completed event
    app_env.scan_op = GAPM_STOP_ACTIVITY;
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

    /*------------------------------------------------------
     * INITIALIZE ALL MODULES
     *------------------------------------------------------*/

    // load device information:

#if (BLE_APP_SEC)
    // Security Module
    app_sec_init();
#endif // (BLE_APP_SEC)

    app_client_init();

    // Reset the stack
    appm_send_gapm_reset_cmd();

    app_env.prf_env.app_task = (ke_task_id_t)(TASK_APP | PERM(PRF_MI, ENABLE));
    app_env.prf_env.prf_task = (ke_task_id_t)(TASK_APP | PERM(PRF_MI, ENABLE));
}

bool appm_add_svc(void)
{
    // Indicate if more services need to be added in the database
    bool more_svc = false;
#if (BLE_APP_USE_CLIENT_TASK)
    // Check if another should be added in the database
    if (app_env.next_svc != APPM_SVC_LIST_STOP)
    {
        ASSERT_INFO(appm_add_svc_func_list[app_env.next_svc] != NULL, app_env.next_svc, 1);

        // Call the function used to add the required service
        appm_add_svc_func_list[app_env.next_svc]();

        // Select following service to add
        app_env.next_svc++;
        more_svc = true;
    }
#endif
    return more_svc;
}

void appm_disconnect(uint8_t conidx)
{
    //log_debug("%s@%d\n", __func__, __LINE__);

    struct gapc_disconnect_cmd *cmd = KE_MSG_ALLOC(GAPC_DISCONNECT_CMD,
            KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
            gapc_disconnect_cmd);

    cmd->operation = GAPC_DISCONNECT;
    cmd->reason    = CO_ERROR_REMOTE_USER_TERM_CON;

    // Send the message
    ke_msg_send(cmd);
}

void appm_connect_fsm_next(bool next, uint8_t operation)
{
    //log_debug("%s@%d, next=%d, oper=%x\n", __func__, __LINE__, next, operation);

    uint8_t i=0;
    if (operation == GAPM_CREATE_INIT_ACTIVITY) {
        for(i=0;i<BLE_APP_CENTRAL_CONNECT_MAX;i++)
        {
            if(app_env.init[i].init_state == APP_INIT_STATE_CREATING || app_env.init[i].init_state == APP_INIT_STATE_CREATED)
            {
                appm_start_init(i);
                break;
            }
        }
    }
    else if (operation == GAPM_START_ACTIVITY) {
        for(i=0;i<BLE_APP_CENTRAL_CONNECT_MAX;i++)
        {
            if(app_env.init[i].init_state == APP_INIT_STATE_STARTING)
            {
                if (next) {
                    app_env.init[i].init_state = APP_INIT_STATE_STARTED;
                }
                else
                {
                    appm_delete_init(i);
                }
                break;
            }
        }
    }
    else if (operation == GAPM_STOP_ACTIVITY) {
        for(i=0;i<BLE_APP_CENTRAL_CONNECT_MAX;i++)
        {
            if(app_env.init[i].init_state == APP_INIT_STATE_STOPPING || app_env.init[i].init_state == APP_INIT_STATE_STOPPED)
            {
                appm_delete_init(i);
                break;
            }
        }
    }
    else if (operation == GAPM_DELETE_ACTIVITY) {
        for(i=0;i<BLE_APP_CENTRAL_CONNECT_MAX;i++)
        {
            if(app_env.init[i].init_state == APP_SCAN_STATE_DELETING)
            {
                memset(&app_env.init[i], 0, sizeof(struct app_connecting_device));
                if (app_env.w4connect>0) {
                    appm_create_init();
                }
                else {
                    ke_state_set(TASK_APP, APPM_READY);
                }
                break;
            }
        }
    }
}

void appm_scan_fsm_next(bool next)
{
    (void)next;
    //log_debug("%s@%d,state=%x\n", __func__, __LINE__, app_env.scan_state);

    switch (app_env.scan_state)
    {
        case (APP_SCAN_STATE_IDLE):
            {
                // Create 
                appm_create_scan();
                ke_state_set(TASK_APP, APPM_SCAN);
            }
            break;

        case (APP_SCAN_STATE_CREATING):
        case (APP_SCAN_STATE_CREATED):
            {
                // Start activity
                appm_start_scan();
            } 
            break;

        case (APP_SCAN_STATE_STARTING):
            {
                // Go to started state
                app_env.scan_state = APP_SCAN_STATE_STARTED;
            } 
            break;

        case (APP_SCAN_STATE_STARTED):
            {
                // Stop activity
                appm_stop_scan();
            } 
            break;

        case (APP_SCAN_STATE_STOPPING):
            {
                // Go created state
                app_env.scan_state = APP_SCAN_STATE_CREATED;
                // restart scan
                appm_start_scan();
            } 
            break;

        case APP_SCAN_STATE_STOPPED:
            appm_delete_scan();
            break;
        case APP_SCAN_STATE_DELETING:
            app_env.scan_state = APP_SCAN_STATE_IDLE;
            if (app_env.w4connect)
            {
                //app_env.w4connect = 0;
                appm_create_init();
                ke_state_set(TASK_APP, APPM_CONNECT);
            }
            break;
        default:
            {
                ASSERT_ERR(0);
            }
            break;
    }
}

void appm_fsm_next(bool next, uint8_t operation)
{
    ke_state_t state = ke_state_get(TASK_APP);
    //log_debug("app state=%x,next=%d\n", state,next);
    switch(state)
    {
        case APPM_READY:
        case APPM_SCAN:
            appm_scan_fsm_next(next);
            break;
        case APPM_CONNECT:
            appm_connect_fsm_next(next, operation);
            break;
        default:
            break;
    }
}

#endif //(BLE_APP_PRESENT)

/// @} APP
