/**
 ****************************************************************************************
 *
 * @file app_tspp_server.c
 *
 * @brief Application Module entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_tspp_server.h"       // Application Module Definitions
#include "app_task.h"                // application task definitions
#include "app.h"                     // Application Definitions
#include "tspps_task.h"               // health thermometer functions
#include "ke_timer.h"
/**
 ****************************************************************************************
 * @addtogroup APP_COMM_TSPP_C app_tspp_server.c
 * @ingroup APP_COMMON
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#include "co_bt.h"
#include "co_utils.h"
#include "prf_types.h"               // Profile common types definition
#include "arch.h"                    // Platform Definitions
#include "prf.h"
#include <string.h>
#if (BLE_TSPP_SERVER)

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/// Application Module Environment Structure
struct app_tspp_server_env_tag app_tspp_server_env;
uint8_t app_tspp_trans_data_buffer[BLE_TSPPS_FIFO_BUF_SIZE];
/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_tspp_server_init(void)
{
    // Reset the environment
    memset(&app_tspp_server_env, 0, sizeof(struct app_tspp_server_env_tag));
}

extern const struct prf_task_cbs* tspp_server_prf_itf_get(void);

void app_tspp_server_add_service(void)
{
    struct tspp_server_db_cfg* db_cfg;
    // Allocate the CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
            TASK_GAPM, TASK_APP,
            gapm_profile_task_add_cmd, sizeof(struct tspp_server_db_cfg));
    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl     = PERM(SVC_AUTH, NO_AUTH)|PERM(SVC_MI,ENABLE);
    req->prf_task_id = TASK_ID_TSPPS;
    req->app_task    = TASK_APP;
    req->start_hdl   = 0;

    // Set parameters and add some parameter if needed
    db_cfg = (struct tspp_server_db_cfg* ) req->param;
    db_cfg->fifo_size = BLE_TSPPS_FIFO_BUF_SIZE;
    db_cfg->fifo_buffer = app_tspp_trans_data_buffer;
    db_cfg->connect_num = BLE_APP_TSPPS_CONNECT_MAX; // max connect num
    db_cfg->svc_type= 1; // 128 uuid ,use to bt audio
    // Send the message
    ke_msg_send(req);
}

void app_tspp_server_enable_prf(uint8_t conidx)
{
    //    app_tspp_server_env.conidx[conidx] = conidx;
#if 0
    struct gapc_conn_param conn_param;
#if 0 // interval 15ms iphone6s 12.3kBps; iphone8x 68kBps, mate9 13.2kBps
    conn_param.intv_min = 0x0c;
    conn_param.intv_max = 0x0c;
    conn_param.latency  = 0;
    conn_param.time_out = 100;
#else // android interval 7.5ms, huawei6x 4.9kBps; mate9 8.89kBps
    conn_param.intv_min = 0x06;
    conn_param.intv_max = 0x06;
    conn_param.latency  = 0;
    conn_param.time_out = 0x07D0;
#endif
    appm_update_param(&conn_param);
#endif
    // Allocate the message
    struct tspp_server_enable_req * req = KE_MSG_ALLOC(TSPP_SERVER_ENABLE_REQ,
            KE_BUILD_ID(prf_get_task_from_id(TASK_ID_TSPPS), conidx),
            KE_BUILD_ID(TASK_APP,conidx),
            tspp_server_enable_req);

    // Fill in the parameter structure
    req->conidx             = conidx;

    // Send the message
    ke_msg_send(req);
}

void app_tspp_server_disable_prf(uint8_t conidx)
{
    //    app_tspp_server_env.conidx[conidx] = GAP_INVALID_CONIDX;

    uint8_t is_find=0;

    app_tspp_server_env.ntf_en[conidx] = 0;
    for(uint8_t i=0;i<BLE_APP_TSPPS_CONNECT_MAX;i++)
    {
        if (app_tspp_server_env.ntf_en[i])
        {
            is_find++;
        }
    }
    if (is_find == 0)
    {
        ke_timer_clear(TSPP_SERVER_TIMEOUT_TIMER, TASK_APP);
    }
}

static int tspp_server_enable_rsp_handler(ke_msg_id_t const msgid,
        struct tspp_server_enable_rsp const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

static int tspp_server_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
        struct tspp_server_ntf_cfg_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    log_debug("%s@%d,src=%d,dest=%d\n", __func__, __LINE__, KE_IDX_GET(src_id), KE_IDX_GET(dest_id));

    uint8_t conidx = KE_IDX_GET(src_id);
    uint8_t is_find=0;
    for(uint8_t i=0;i<BLE_APP_TSPPS_CONNECT_MAX;i++)
    {
        if (app_tspp_server_env.ntf_en[i])
        {
            is_find++;
        }
    }

    if(param->ntf_cfg == PRF_CLI_START_NTF || param->ntf_cfg == PRF_CLI_START_IND){
        app_tspp_server_env.ntf_en[conidx] = 1;
    }else if(param->ntf_cfg == PRF_CLI_STOP_NTFIND){
        app_tspp_server_env.ntf_en[conidx] = 0;
    }
    return (KE_MSG_CONSUMED);
}

static int tspp_server_send_empty_handler(ke_msg_id_t const msgid,
        struct tspp_server_buffer_empty_evt const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    //uint8_t conidx = KE_IDX_GET(src_id);
    APP_HANDLER(tspp_empty, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

static int tspp_server_send_full_handler(ke_msg_id_t const msgid,
        struct tspp_server_buffer_empty_evt const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    APP_HANDLER(tspp_full, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

static int tspp_server_write_ind_handler(ke_msg_id_t const msgid,
        struct tspp_server_write_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    //uint8_t conidx = KE_IDX_GET(src_id);
    //log_debug("%s@%d\n", __func__, __LINE__);
    //log_debug_array_ex("write data", param->value, param->length);
    //appm_recv_data(conidx, (uint8_t*)param->value, param->length);
    APP_HANDLER(tspp_recv, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

static int tspp_server_error_handler(ke_msg_id_t const msgid,
        struct tspp_server_error_evt const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    APP_HANDLER(tspp_error, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_tspp_server_msg_dflt_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    // Drop the message

    return (KE_MSG_CONSUMED);
}

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_tspp_server_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_tspp_server_msg_dflt_handler},
    {TSPP_SERVER_ENABLE_REQ,        (ke_msg_func_t)tspp_server_enable_rsp_handler},
    {TSPP_SERVER_NTF_CFG_IND,       (ke_msg_func_t)tspp_server_ntf_cfg_ind_handler},
    //{TSPP_SERVER_TIMEOUT_TIMER,     (ke_msg_func_t)tspp_server_timeout_timer_handler},
    {TSPP_SERVER_BUFFER_EMPTY,      (ke_msg_func_t)tspp_server_send_empty_handler},
    {TSPP_SERVER_BUFFER_FULL,       (ke_msg_func_t)tspp_server_send_full_handler},
    {TSPP_SERVER_WRITE_IND,         (ke_msg_func_t)tspp_server_write_ind_handler},
    {TSPP_SERVER_ERROR,             (ke_msg_func_t)tspp_server_error_handler},
};

const struct app_subtask_handlers app_tspp_server_handlers = APP_HANDLERS(app_tspp_server);


void app_tspp_server_send_data(uint8_t conidx, uint8_t* pdata, uint16_t len)
{
    // Allocate the message
    struct tspp_server_send_ntf_cmd * cmd = KE_MSG_ALLOC_DYN(TSPP_SERVER_SEND_NTF_CMD,
            KE_BUILD_ID(prf_get_task_from_id(TASK_ID_TSPPS), conidx),
            KE_BUILD_ID(TASK_APP, conidx),
            tspp_server_send_ntf_cmd,
            len);
    cmd->conidx = conidx;
    cmd->length = len;
    memcpy(cmd->value, pdata, len);
    // Send the message
    ke_msg_send(cmd);
}
#endif
/// @} APP
