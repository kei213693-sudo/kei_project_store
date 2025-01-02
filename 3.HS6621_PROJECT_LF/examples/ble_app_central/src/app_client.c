/**
 ****************************************************************************************
 *
 * @file app_client.c
 *
 * @brief Application Module entry point
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

#include "rwip_config.h"     // SW configuration

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_client.h"       // Application Module Definitions
#include "app.h"                     // Application Definitions
#include "app_task.h"                // application task definitions
#include "ke_timer.h"
#include "co_bt.h"
#include "co_utils.h"
#include "prf_types.h"               // Profile common types definition
#include "arch.h"                    // Platform Definitions
#include "prf.h"
#include <string.h>
#include "tsppc_task.h"
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Application Module Environment Structure
struct app_client_env_tag app_client_env;
/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_client_init(void)
{
    // Reset the environment
    memset(&app_client_env, 0, sizeof(struct app_client_env_tag));

    // TODO: Initial something
}

#if (BLE_TSPP_COLLECTOR)
/**
 ****************************************************************************************
 * @brief
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_TSPPC).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_client_msg_tsppc_enable_rsp_handler(ke_msg_id_t const msgid,
                                     struct tsppc_enable_rsp const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(dest_id);
    uint8_t len = (BLE_MAX_OCTETS-4-3);
    uint8_t send_data[len];
    memset(send_data, len, 0x2A);
    //log_debug("%s@%d, status=%x\n", __func__, __LINE__,param->status);
    if (param->status == GAP_ERR_NO_ERROR)
    {
        memcpy(&app_client_env.tspps, &param->tspps, sizeof(struct tspps_content));
        ///Existing handle values tspps
        struct tsppc_wr_char_req * req = KE_MSG_ALLOC_DYN(TSPPC_WR_CHAR_REQ,
                                                        KE_BUILD_ID(prf_get_task_from_id(TASK_ID_TSPPC),conidx),
                                                        KE_BUILD_ID(TASK_APP, conidx),
                                                        tsppc_wr_char_req, len);

        // Fill in the parameter structure
        req->length             = len;
        memcpy(req->value, send_data, len);
        ke_msg_send(req);

    }
    return (KE_MSG_CONSUMED);
}

static int app_client_msg_tsppc_write_char_rsp_handler(ke_msg_id_t const msgid,
                                     struct tsppc_wr_char_rsp const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(dest_id);

    //log_debug("%s@%d, status=%x\n", __func__, __LINE__,param->status);
    if (param->status == GAP_ERR_NO_ERROR)
    {
        struct tsppc_cfg_indntf_req * req = KE_MSG_ALLOC(TSPPC_CFG_INDNTF_REQ,
                                                        KE_BUILD_ID(prf_get_task_from_id(TASK_ID_TSPPC),conidx),
                                                        KE_BUILD_ID(TASK_APP, conidx),
                                                        tsppc_cfg_indntf_req);

        req->cfg_val = PRF_CLI_START_NTF;
        // Fill in the parameter structure
        ke_msg_send(req);

    }
    return (KE_MSG_CONSUMED);
}
static int app_client_msg_tsppc_notify_ind_handler(ke_msg_id_t const msgid,
                                     struct tsppc_notify_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    static uint8_t num=0;

    uint8_t conidx = KE_IDX_GET(dest_id);
    log_debug("%s@%d,len=%d,num=%d\n", __func__, __LINE__, param->length, num);
    if (num ==200)
    {
        struct tsppc_cfg_indntf_req * req = KE_MSG_ALLOC(TSPPC_CFG_INDNTF_REQ,
                                                        KE_BUILD_ID(prf_get_task_from_id(TASK_ID_TSPPC),conidx),
                                                        KE_BUILD_ID(TASK_APP, conidx),
                                                        tsppc_cfg_indntf_req);

        req->cfg_val = PRF_CLI_STOP_NTFIND;
        // Fill in the parameter structure
        ke_msg_send(req);
        log_debug("stop\n");
    }
    num++;
    return (KE_MSG_CONSUMED);
}

#endif
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
static int app_client_msg_dflt_handler(ke_msg_id_t const msgid,
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
const struct ke_msg_handler app_client_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_client_msg_dflt_handler},
#if (BLE_TSPP_COLLECTOR)
    {TSPPC_ENABLE_RSP,              (ke_msg_func_t)app_client_msg_tsppc_enable_rsp_handler},
    {TSPPC_WR_CHAR_RSP,             (ke_msg_func_t)app_client_msg_tsppc_write_char_rsp_handler},
    {TSPPC_NOTIFY_IND,              (ke_msg_func_t)app_client_msg_tsppc_notify_ind_handler},
#endif
};

const struct app_subtask_handlers app_client_handlers = APP_HANDLERS(app_client);

void app_client_add_tsppc(void)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd);
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, NO_AUTH)|PERM(SVC_MI,ENABLE);
    req->prf_task_id = TASK_ID_TSPPC;
    req->app_task = TASK_APP;
    req->start_hdl = 0;
    // Send the message
    ke_msg_send(req);
}

void app_client_enable_prf(uint8_t conidx)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    //app_client_env.conidx = conidx;
    // Allocate the message
    #if (BLE_TSPP_COLLECTOR)
    ///Existing handle values tspps
    struct tsppc_enable_req * req = KE_MSG_ALLOC(TSPPC_ENABLE_REQ,
                                                KE_BUILD_ID(prf_get_task_from_id(TASK_ID_TSPPC),conidx),
                                                KE_BUILD_ID(TASK_APP, conidx),
                                                tsppc_enable_req);

    // Fill in the parameter structure
    req->con_type             = PRF_CON_DISCOVERY;
    ke_msg_send(req);

    #endif
}

void app_client_disable_prf(uint8_t conidx)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    //app_client_env.conidx = 0;
}

/**
 ****************************************************************************************
 * @brief discover all server
 ****************************************************************************************
 */
void app_client_disc_all_server(void)
{
}

/// @} APP
