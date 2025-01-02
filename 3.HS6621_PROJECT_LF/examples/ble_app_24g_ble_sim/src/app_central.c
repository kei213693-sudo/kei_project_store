/**
 ****************************************************************************************
 *
 * @file app_central.c
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

#if (BLE_APP_CENTRAL_SUPPORT)

#include <string.h>

#include "prf.h"
#include "app_central.h"
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
#include "prf_utils.h"
/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * ENUMERATIONS
 ****************************************************************************************
 */


/*
 * LOCAL VARIABLES DEFINITIONS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */


/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
static uint8_t app_central_find_free(void)
{
    uint8_t i =0;
    for(i=0; i< BLE_APP_CENTRAL_SUPPORT; i++)
    {
        if (app_env.central.connect[i].state == APP_CENTRAL_SUB_STATE_IDLE)
            break;
    }
    return i;
}

uint8_t app_central_find_by_conidx(uint8_t conidx)
{
    uint8_t i =0;
    for(i=0; i< BLE_APP_CENTRAL_SUPPORT; i++)
    {
        if (app_env.central.connect[i].conidx == conidx)
            break;
    }
    return i;
}

static uint8_t app_central_find_by_actv_idx(uint8_t actv_idx)
{
    uint8_t i =0;
    for(i=0; i< BLE_APP_CENTRAL_SUPPORT; i++)
    {
        if (app_env.central.connect[i].actv_idx == actv_idx)
            break;
    }
    return i;
}

static void app_central_free_node(uint8_t index)
{
    if (index >= BLE_APP_CENTRAL_SUPPORT) return;
    
    memset(&app_env.central.connect[index], 0, sizeof(struct app_central_node_tag));
    app_env.central.connect[index].conidx = 0xFF;
}

static void app_central_create_init(uint8_t index)
{
    log_debug("%s@%d\n", __func__, __LINE__);
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
    app_env.central.connect[index].state = APP_CENTRAL_SUB_STATE_CREATING;
}

static void app_central_delete_init(uint8_t index)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_CREATE_CMD message
    struct gapm_activity_delete_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_DELETE_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_delete_cmd);

    // Set operation code
    p_cmd->operation = GAPM_DELETE_ACTIVITY;
    p_cmd->actv_idx = app_env.central.connect[index].actv_idx;

    // Send the message
    ke_msg_send(p_cmd);

    // Keep the current operation
    app_env.central.connect[index].state = APP_CENTRAL_SUB_STATE_DELETING;
}

static void app_central_start_init(uint8_t index)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_START_CMD message
    struct gapm_activity_start_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_START_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_start_cmd);

    p_cmd->operation = GAPM_START_ACTIVITY;
    p_cmd->actv_idx = app_env.central.connect[index].actv_idx;
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
    memcpy(&(p_cmd->u_param.init_param.peer_addr), &(app_env.central.connect[index].addr), sizeof(struct gap_bdaddr));

    // Send the message
    ke_msg_send(p_cmd);

    // Keep the current operation
    app_env.central.connect[index].state = APP_CENTRAL_SUB_STATE_STARTING;
}

static void app_central_stop_init(uint8_t index)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_STOP_CMD message
    struct gapm_activity_stop_cmd *cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_STOP_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_stop_cmd);

    // Fill the allocated kernel message
    cmd->operation = GAPM_STOP_ACTIVITY;
    cmd->actv_idx = app_env.central.connect[index].actv_idx;

    // Send the message
    ke_msg_send(cmd);

    // Update advertising state
    app_env.central.connect[index].state = APP_CENTRAL_SUB_STATE_STOPPING;
}

static void app_central_create_scan(void)
{
    log_debug("%s@%d\n", __func__, __LINE__);
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
    app_env.central.scan_state = APP_CENTRAL_SUB_STATE_CREATING;
    app_env.central.scan_operation = GAPM_CREATE_SCAN_ACTIVITY;
}
#if 0
static void app_central_delete_scan(void)
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
    app_env.central.scan_state = APP_CENTRAL_SUB_STATE_DELETING;
    app_env.central.scan_operation = GAPM_DELETE_ACTIVITY;
}
#endif
static void app_central_start_scan(void)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_START_CMD message
    
    struct gapm_activity_start_cmd *p_cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_START_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_start_cmd);

    p_cmd->operation = GAPM_START_ACTIVITY;
    p_cmd->actv_idx = app_env.central.scan_actv_idx;
    p_cmd->u_param.scan_param.type = GAPM_SCAN_TYPE_CONN_DISC;
    p_cmd->u_param.scan_param.prop = GAPM_SCAN_PROP_PHY_1M_BIT|GAPM_SCAN_PROP_ACTIVE_1M_BIT|GAPM_SCAN_PROP_ACCEPT_RPA_BIT;
    p_cmd->u_param.scan_param.dup_filt_pol = GAPM_DUP_FILT_EN;
    p_cmd->u_param.scan_param.scan_param_1m.scan_intv = 0x200;
    p_cmd->u_param.scan_param.scan_param_1m.scan_wd = 0x100;
    p_cmd->u_param.scan_param.scan_param_coded.scan_intv = 0x200;
    p_cmd->u_param.scan_param.scan_param_coded.scan_wd = 0x100;
    p_cmd->u_param.scan_param.duration = GAPM_SCAN_TYPE_GEN_DISC;
    p_cmd->u_param.scan_param.period = 0;
    // Send the message
    ke_msg_send(p_cmd);

    // Keep the current operation
    app_env.central.scan_state = APP_CENTRAL_SUB_STATE_STARTING;
    app_env.central.scan_operation = GAPM_START_ACTIVITY;
}

static void app_central_stop_scan(void)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    // Prepare the GAPM_ACTIVITY_STOP_CMD message
    struct gapm_activity_stop_cmd *cmd = KE_MSG_ALLOC(GAPM_ACTIVITY_STOP_CMD,
            TASK_GAPM, TASK_APP,
            gapm_activity_stop_cmd);

    // Fill the allocated kernel message
    cmd->operation = GAPM_STOP_ACTIVITY;
    cmd->actv_idx = app_env.central.scan_actv_idx;

    // Send the message
    ke_msg_send(cmd);

    app_env.central.scan_state = APP_CENTRAL_SUB_STATE_STOPPING;
    app_env.central.scan_operation = GAPM_STOP_ACTIVITY;
}

static void app_central_update_scan_state(uint8_t opera, uint8_t actv_idx, uint8_t reason)
{
    (void)opera;
    log_debug("scan state=%x, todo=%x,reason=%x\r\n", app_env.central.scan_state, app_env.central.scan_todo, reason);
    switch(app_env.central.scan_state)
    {
        case APP_CENTRAL_SUB_STATE_IDLE:
            break;
        case APP_CENTRAL_SUB_STATE_CREATING:
            app_env.central.scan_actv_idx = actv_idx;
            app_env.central.scan_state = APP_CENTRAL_SUB_STATE_CREATED;
            break;
        case APP_CENTRAL_SUB_STATE_CREATED:
            {
                if (reason !=0)
                {
                    app_env.central.scan_state = APP_CENTRAL_SUB_STATE_IDLE;
                    app_env.central.scan_todo = 0;
                    app_scan_error_hander(reason);
                }
                else if (app_env.central.scan_todo != 2)
                {
                    app_central_start_scan();
                }
            }
            break;
        case APP_CENTRAL_SUB_STATE_STARTING:
            {
                if (reason !=0)
                {
                    app_env.central.scan_state = APP_CENTRAL_SUB_STATE_IDLE;
                    app_env.central.scan_todo = 0;
                    app_scan_error_hander(reason);
                }
                else if (app_env.central.scan_todo == 2)
                {
                    app_central_stop_scan();
                }
                else
                {
                    app_env.central.scan_state = APP_CENTRAL_SUB_STATE_STARTED;
                    app_env.central.scan_todo = 0;
                }
            }
            break;
        case APP_CENTRAL_SUB_STATE_STARTED:
            if (reason != 0)
            {
                app_env.central.scan_state = APP_CENTRAL_SUB_STATE_CREATED;
                app_env.central.scan_todo = 0;
            }
            break;
        case APP_CENTRAL_SUB_STATE_STOPPING:
            if (app_env.central.scan_todo == 1)
            {
                app_env.central.scan_todo = 0;
                app_central_start_scan();
            }
            else
            {
                app_env.central.scan_state = APP_CENTRAL_SUB_STATE_STOPPED;
            }
            break;
        case APP_CENTRAL_SUB_STATE_STOPPED:
            {
                if (reason !=0)
                {
                    app_env.central.scan_state = APP_CENTRAL_SUB_STATE_IDLE;
                    app_env.central.scan_todo = 0;
                    app_scan_error_hander(reason);
                }
                else if (app_env.central.scan_todo == 1)
                {
                    app_env.central.scan_todo = 0;
                    app_central_start_scan();
                }
                else
                {
                    app_env.central.scan_todo = 0;
                    app_env.central.scan_state = APP_CENTRAL_SUB_STATE_CREATED;
                }
            }
            break;
        case APP_CENTRAL_SUB_STATE_DELETING:
//            log_debug("case APP_CENTRAL_SUB_STATE_DELETING:\n");
            app_env.central.scan_state = APP_CENTRAL_SUB_STATE_IDLE;
            if (app_env.central.scan_todo == 1)
            {
                app_env.central.scan_todo = 0;
                app_central_start_scan();
            }
            break;
        default:
            break;
    }
}

static void app_central_update_init_state(uint8_t opera, uint8_t actv_idx, uint8_t reason)
{
    uint8_t index = BLE_APP_CENTRAL_SUPPORT;
    log_debug("init %x,%x,%x,%x\r\n", opera, actv_idx, reason, app_env.central.w4connect);
    if (app_env.central.w4connect)
    {
        index = app_env.central.w4connect - 1;
    }
    else {
        index = app_central_find_by_actv_idx(actv_idx);
    }
    if (index >= BLE_APP_CENTRAL_SUPPORT) return;

    log_debug("init [%d] state=%x\r\n", index, app_env.central.connect[index].state);

    if (opera == GAPM_DELETE_ACTIVITY && app_env.central.connect[index].state != APP_CENTRAL_SUB_STATE_DELETING)
    {
        return;
    }
    switch(app_env.central.connect[index].state)
    {
        case APP_CENTRAL_SUB_STATE_IDLE:
            break;
        case APP_CENTRAL_SUB_STATE_CREATING:
            app_env.central.connect[index].actv_idx = actv_idx;
            app_env.central.connect[index].state = APP_CENTRAL_SUB_STATE_CREATED;
            break;
        case APP_CENTRAL_SUB_STATE_CREATED:
            if (reason)
            {
                app_central_free_node(index);
                app_env.central.w4connect = 0;
            }
            else
            {
                app_central_start_init(index);
            }
            break;
        case APP_CENTRAL_SUB_STATE_STARTING:
            app_env.central.connect[index].state = APP_CENTRAL_SUB_STATE_STARTED;
            //app_env.central.w4connect = 0;
            break;
        case APP_CENTRAL_SUB_STATE_STARTED:
            if (reason != 0)
                app_env.central.connect[index].state = APP_CENTRAL_SUB_STATE_STOPPING;
            break;
        case APP_CENTRAL_SUB_STATE_STOPPING:
        case APP_CENTRAL_SUB_STATE_STOPPED:
            app_central_delete_init(index);
            app_central_free_node(index);
            app_env.central.w4connect = 0;
            break;
        case APP_CENTRAL_SUB_STATE_DELETING:
            app_central_free_node(index);
            break;
        default:
            break;
    }
}

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void app_central_init(void)
{
    uint8_t i =0;
    for(i=0; i< BLE_APP_CENTRAL_SUPPORT; i++)
    {
        app_env.central.connect[i].conidx = 0xFF;
        app_env.central.connect[i].conhdl = 0xFF;
    }
    app_env.central.prf_env.app_task = (ke_task_id_t)(TASK_APP | PERM(PRF_MI, ENABLE));
    app_env.central.prf_env.prf_task = (ke_task_id_t)(TASK_APP | PERM(PRF_MI, ENABLE));
}

void app_central_update_state(uint8_t opera, uint8_t actv_idx, uint8_t reason)
{
    log_debug("app_central_update_state=%x, index=%x, reason=%x\r\n", opera, actv_idx, reason);
    if (opera == GAPM_ACTV_TYPE_SCAN || opera == app_env.central.scan_operation)
    {
        if(opera != GAPM_ACTV_TYPE_SCAN) app_env.central.scan_operation = 0;
        
        app_central_update_scan_state(opera, actv_idx, reason);
    }
    else // if (opera == GAPM_ACTV_TYPE_INIT)
    {
        app_central_update_init_state(opera, actv_idx, reason);
    }
}

uint8_t app_central_scan_start(void)
{
    uint8_t ret = APP_CENTRAL_ERROR_NO_ERROR;
    switch(app_env.central.scan_state)
    {
        case APP_CENTRAL_SUB_STATE_IDLE:
            app_central_create_scan();
            break;
        case APP_CENTRAL_SUB_STATE_CREATED:
            app_central_start_scan();
            break;
        case APP_CENTRAL_SUB_STATE_CREATING:
        case APP_CENTRAL_SUB_STATE_STARTING:
            break;        
        case APP_CENTRAL_SUB_STATE_STARTED:
            ret = APP_CENTRAL_ERROR_SCAN_STARTED;
            break;
        case APP_CENTRAL_SUB_STATE_STOPPING:
        case APP_CENTRAL_SUB_STATE_STOPPED:
        case APP_CENTRAL_SUB_STATE_DELETING:
            app_env.central.scan_todo = 1;
            break;
        default:
            break;
    }
    return ret;
}

uint8_t app_central_scan_stop(void)
{
    uint8_t ret = APP_CENTRAL_ERROR_NO_ERROR;
    switch(app_env.central.scan_state)
    {
        case APP_CENTRAL_SUB_STATE_IDLE:
            break;
        case APP_CENTRAL_SUB_STATE_CREATING:
        case APP_CENTRAL_SUB_STATE_CREATED:
        case APP_CENTRAL_SUB_STATE_STARTING:
            app_env.central.scan_todo = 2;
            break;        
        case APP_CENTRAL_SUB_STATE_STARTED:
            app_central_stop_scan();
            break;
        case APP_CENTRAL_SUB_STATE_STOPPING:
        case APP_CENTRAL_SUB_STATE_STOPPED:
        case APP_CENTRAL_SUB_STATE_DELETING:
            break;
        default:
            break;
    }
    return ret;
}

uint8_t app_central_connect(struct gap_bdaddr addr)
{
    uint8_t ret = APP_CENTRAL_ERROR_NO_ERROR;

    if (app_env.central.w4connect)
    {
        return APP_CENTRAL_ERROR_CONN_CONNECTING;
    }
    
    uint8_t index = app_central_find_free();
    if (index < BLE_APP_CENTRAL_SUPPORT)
    {
        memcpy(&app_env.central.connect[index].addr, &addr, sizeof(struct gap_bdaddr));
        app_central_create_init(index);
        app_env.central.w4connect = index+1;
    }
    else {
        ret = APP_CENTRAL_ERROR_CONN_NO_FREE;
    }
    return ret;
}

/**
 ****************************************************************************************
 * @brief cancle current connecting
 *
 * @return status of init @ref eapp_central_error
 ****************************************************************************************
 */
uint8_t app_central_connect_cancel(void)
{
    uint8_t index = app_env.central.w4connect;
    if (index > 0 && 
       app_env.central.connect[index-1].state == APP_CENTRAL_SUB_STATE_STARTED)
    {
        app_central_stop_init(index-1);
        return APP_CENTRAL_ERROR_NO_ERROR;
    }
    return APP_CENTRAL_ERROR_CONN_FAIL;
}

/**
 ****************************************************************************************
 * @brief disconnect by handle
 *
 * @return status of scan @ref eapp_central_error
 ****************************************************************************************
 */
uint8_t app_central_disconnect(uint8_t conidx)
{
    uint8_t index = app_central_find_by_conidx(conidx);
    if (index < BLE_APP_CENTRAL_SUPPORT)
    {
        app_central_delete_init(index);
        app_central_free_node(index);
        return APP_CENTRAL_ERROR_NO_ERROR;
    }
    return APP_CENTRAL_ERROR_CONN_FAIL;
}


void app_central_set_conn_handle(uint8_t conidx, uint16_t conhdl)
{
    uint8_t index = app_env.central.w4connect-1;
    log_debug("init set conn index=%x\r\n", app_env.central.w4connect);
    app_env.central.w4connect = 0;
    app_env.central.connect[index].conidx = conidx;
    app_env.central.connect[index].conhdl = conhdl;
    app_env.central.connect[index].mtu = GAP_MAX_LE_MTU;
    app_env.central.connect[index].tx_len = BLE_MAX_OCTETS;
}

void app_central_set_max_mtu(uint8_t conidx, uint16_t mtu)
{
    log_debug("app_central_set_max_mtu conidx=%x, mtu=%d\r\n", conidx, mtu);
    uint8_t index = app_central_find_by_conidx(conidx);
    if (index >= BLE_APP_CENTRAL_SUPPORT) return;

    if  (app_env.central.connect[index].mtu != mtu) {
        app_env.central.connect[index].mtu = mtu;
        prf_disc_svc_all_send(&(app_env.central.prf_env), conidx);
    }
}

void app_central_set_tx_len(uint8_t conidx, uint16_t tx_len)
{
    log_debug("app_central_set_tx_len conidx=%x, len=%d\r\n", conidx, tx_len);
    
    uint8_t index = app_central_find_by_conidx(conidx);
    if (index >= BLE_APP_CENTRAL_SUPPORT) return;
    
    app_env.central.connect[index].tx_len = tx_len;
}

uint8_t app_central_get_scan_state(void)
{
    return app_env.central.scan_state;
}
#endif //(BLE_APP_PRESENT)

/// @} APP
