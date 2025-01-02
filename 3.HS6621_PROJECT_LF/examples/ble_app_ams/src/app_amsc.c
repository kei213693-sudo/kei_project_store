/**
 ****************************************************************************************
 *
 * @file app_ams.c
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
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */
#include "rwip_config.h"     // SW configuration


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

/// AMS Application Module Environment Structure
struct app_amsc_env_tag app_amsc_env;

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_amsc_init(void)
{
    ams_debug("%s\n",__func__);
    // Reset the environment
    memset(&app_amsc_env, 0, sizeof(struct app_amsc_env_tag));
}

const struct prf_task_cbs* amsc_prf_itf_get(void);
void app_ams_add_amsc(void)
{
    struct prf_itf_pair itf_pair = {
        TASK_ID_AMSC, amsc_prf_itf_get(),
    };
    prf_itf_register(&itf_pair, 1);

    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd);
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, UNAUTH);
    req->prf_task_id = TASK_ID_AMSC;
    req->app_task = TASK_APP;
    req->start_hdl = 0;
    // Send the message
    ke_msg_send(req);
}

void app_amsc_enable_prf(uint16_t conidx)
{
    // Allocate the message
    struct amsc_enable_req * req = KE_MSG_ALLOC(AMSC_ENABLE_REQ,
                                                KE_BUILD_ID(prf_get_task_from_id(TASK_ID_AMSC), conidx), TASK_APP,
                                                amsc_enable_req);
    // Fill in the parameter structure
    req->con_type    = PRF_CON_DISCOVERY;
    // Subscribe entities
    req->ams_subscribe |= (1<<PlayerAttributeIDName);         // Localized name of the app
    req->ams_subscribe |= (1<<PlayerAttributeIDPlaybackInfo); // PlaybackInfo: State,Rate,ElapsedTime
    req->ams_subscribe |= (1<<PlayerAttributeIDVolume);       // Volume
    req->ams_subscribe |= (1<<QueueAttributeIDIndex);         // Playlist index
    req->ams_subscribe |= (1<<QueueAttributeIDCount);         // Playlist total number
    req->ams_subscribe |= (1<<QueueAttributeIDShuffleMode);   // ShuffleMode @ref enum AMS_ShuffleMode_values
    req->ams_subscribe |= (1<<QueueAttributeIDRepeatMode);    // RepeatMode @ref enum AMS_RepeatMode_values 
    req->ams_subscribe |= (1<<TrackAttributeIDArtist);        // Name of the artist & Lyrics
    req->ams_subscribe |= (1<<TrackAttributeIDAlbum);         // Name of the album
    req->ams_subscribe |= (1<<TrackAttributeIDTitle);         // Title of the track.
    req->ams_subscribe |= (1<<TrackAttributeIDDuration);      // Total duration of the track
    // Send the message
    ke_msg_send(req);
}

static int app_amsc_msg_dflt_handler(ke_msg_id_t const msgid,
                                     void const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    // Drop the message
    return (KE_MSG_CONSUMED);
}

static int app_amsc_enable_rsp_handler(ke_msg_id_t const msgid,
                                     struct amsc_enable_rsp const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    ams_debug("AMS enabled: status = 0x%x\n", param->status);
    return (KE_MSG_CONSUMED);
}

static int app_amsc_rem_ctrl_ind_handler(ke_msg_id_t const msgid,
                                     struct amsc_rem_ctrl_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    log_debug_array_ex("All command can be sent:", param->value, param->len);
    return (KE_MSG_CONSUMED);
}

static int app_amsc_entity_att_ind_handler(ke_msg_id_t const msgid,
                                     struct amsc_entity_att_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    //log_debug("Entity:%d, flag:%d, data", param->subscribe_bit, param->flag);
    //log_debug_array_ex("", param->data, param->len);
    switch(param->subscribe_bit)
    {
        case PlayerAttributeIDName:
            log_debug("Localized name of the app");
            break;
        case PlayerAttributeIDPlaybackInfo:
            log_debug("State,Rate,ElapsedTime");
            break;
        case PlayerAttributeIDVolume:
            log_debug("Volume");
            break;
        case QueueAttributeIDIndex:
            log_debug("Playlist index");
            break;
        case QueueAttributeIDCount:
            log_debug("Playlist total");
            break;
        case QueueAttributeIDShuffleMode:
            log_debug("Shuffle mode");
            break;
        case QueueAttributeIDRepeatMode:
            log_debug("Repeat mode");
            break;
        case TrackAttributeIDArtist:
            log_debug("Name of the artist");
            break;
        case TrackAttributeIDAlbum:
            log_debug("Name of the album");
            break;
        case TrackAttributeIDTitle:
            log_debug("Title of the track.");
            break;
        case TrackAttributeIDDuration:
            log_debug("Total duration of the track");
            break;
    }
    log_debug(" :");
    for(int i=0;i<param->len;i++){ log_debug("%c", param->data[i]); }
    log_debug("\n");
    return (KE_MSG_CONSUMED);
}

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_amsc_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
	{KE_MSG_DEFAULT_HANDLER,       (ke_msg_func_t)app_amsc_msg_dflt_handler},
	{AMSC_ENABLE_RSP,              (ke_msg_func_t)app_amsc_enable_rsp_handler},
	{AMSC_REM_CTRL_IND,            (ke_msg_func_t)app_amsc_rem_ctrl_ind_handler},
	{AMSC_ENTITY_ATT_IND,          (ke_msg_func_t)app_amsc_entity_att_ind_handler},
};

const struct app_subtask_handlers app_amsc_handlers =
    {&app_amsc_msg_handler_list[0], (sizeof(app_amsc_msg_handler_list)/sizeof(struct ke_msg_handler))};

/// @} APP
