/**
 ****************************************************************************************
 *
 * @file app_ancsc.c
 *
 * @brief Ancsc Application Module entry point
 *
 * Copyright (C) 2020-2030 OnMicro Limited. All rights reserved.
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_COMMON_ANCSC_C app_ancsc.c
 * @ingroup APP_COMMON
 * @{
 ****************************************************************************************
 */
#include "rwip_config.h"     // SW configuration

#if BLE_APP_ANCSC

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "attm.h"
#include "app.h"                    // Application Definitions
#include "app_task.h"               // application task definitions
#include "ke_task.h"
#include "prf_types.h"
#include "prf_utils.h"
#include "co_utils.h"        		// common utility definition
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// ANCS Application Module Environment Structure
struct app_ancsc_env_tag app_ancsc_env;

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_ancsc_init(void)
{
    ancs_debug("%s\n",__func__);
    // Reset the environment
    memset(&app_ancsc_env, 0, sizeof(struct app_ancsc_env_tag));
}

void app_ancs_add_ancsc(void)
{
    ancs_debug("%s\n",__func__);
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd);
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, ENABLE);
    req->prf_task_id = TASK_ID_ANCSC;
    req->app_task = TASK_APP;
    req->start_hdl = 0;
    // Send the message
    ke_msg_send(req);
}

void app_ancsc_enable_prf(uint16_t conidx)
{
    // Allocate the message
    struct ancsc_enable_req * req = KE_MSG_ALLOC(ANCSC_ENABLE_REQ,
                                                KE_BUILD_ID(prf_get_task_from_id(TASK_ID_ANCSC), conidx), TASK_APP,
                                                ancsc_enable_req);
    // Fill in the parameter structure
    req->con_type    = PRF_CON_DISCOVERY;
    // Send the message
    ke_msg_send(req);
}

void app_ancsc_start(void)
{
    ancsc_encrypted_cmd(app_env.conidx);
}

static int app_ancsc_msg_dflt_handler(ke_msg_id_t const msgid,
                                     void const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    // Drop the message
    ancs_debug("%s, msgid:0x%04x\n",__func__, msgid);
    return (KE_MSG_CONSUMED);
}

static int app_ancsc_enable_rsp_handler(ke_msg_id_t const msgid,
                                     struct ancsc_enable_rsp const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    ancs_debug("ANCS enabled: status = 0x%x\n", param->status);
    return (KE_MSG_CONSUMED);
}

static int app_ancsc_notification_ind_handler(ke_msg_id_t const msgid,
                                     struct ancsc_notification_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    //ancs_debug("APP get ANCS notification, uid=0x%08x\n", param->notification_uid);
    if(param->event_id == ANCS_EVENT_ID_NOTIFICATION_ADDED && param->event_flags){
        struct ancsc_get_ntf_attr_cmd *cmd = KE_MSG_ALLOC(ANCSC_GET_NTF_ATTR_CMD,
                                                    src_id, dest_id,
                                                    ancsc_get_ntf_attr_cmd);
        cmd->uid = param->notification_uid;
        ke_msg_send(cmd);
    }
    APP_HANDLER(ancs_recv_evt, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

static int app_ancsc_ntf_attr_ind_handler(ke_msg_id_t const msgid,
                                     struct ancsc_ntf_attr_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
	if(0){// delete the notification on the phone
        param->app_id[param->app_id_len] = 0;
        ancs_debug("APP get ancs notify attribute !\n");
        ancs_debug("\t App_id: %s\n", param->app_id);
        ancs_debug("\t Date  : %s\n", param->date);
	}
    APP_HANDLER(ancs_recv_msg, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_ancsc_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
	{KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_ancsc_msg_dflt_handler},
	{ANCSC_ENABLE_RSP,              (ke_msg_func_t)app_ancsc_enable_rsp_handler},
	{ANCSC_NOTIFICATION_IND,        (ke_msg_func_t)app_ancsc_notification_ind_handler},
	{ANCSC_NTF_ATTR_IND,            (ke_msg_func_t)app_ancsc_ntf_attr_ind_handler},
};

const struct app_subtask_handlers app_ancsc_handlers =
    {&app_ancsc_msg_handler_list[0], (sizeof(app_ancsc_msg_handler_list)/sizeof(struct ke_msg_handler))};

void app_ancsc_proform(uint32_t notif_uid, uint8_t act_id)
{
    struct ancsc_perform_act_cmd *cmd = KE_MSG_ALLOC(ANCSC_PERFORM_ACT_CMD,
                                                KE_BUILD_ID(prf_get_task_from_id(TASK_ID_ANCSC), app_env.conidx), TASK_APP,
                                                ancsc_perform_act_cmd);
    cmd->uid = notif_uid;
    cmd->act_id = act_id;
    ke_msg_send(cmd);
}
#endif /* BLE_APP_ANCSC */
/// @} APP
