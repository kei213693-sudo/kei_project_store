/**
 ****************************************************************************************
 *
 * @file app_atvv.c
 *
 * @brief Application ATVV module entry point
 *
 * Copyright (C) Onmicro 2020
 *
 ****************************************************************************************
 */
 
/**
 ****************************************************************************************
 * @addtogroup APP_COMM_ATVV_C app_atvv.c
 * @ingroup APP_COMMON
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <string.h>

#include "co_bt.h"
#include "co_utils.h"
#include "prf_types.h"               // Profile common types definition
#include "arch.h"                    // Platform definitions
#include "prf.h"
#include "co_timer_pmu.h"

#include "rwip_config.h"            // SW configuration

#include "ke_timer.h"
#include "app_atvv.h"               // Application ATVV module definitions
#include "app_task.h"               // Application task definitions
#include "app.h"                    // Application definitions
#include "atvvs_task.h"             // Application ATVV service definitions
#include "m_audio.h"


#if (BLE_ATVV_SERVER)
/*
 * DEFINES
 ****************************************************************************************
 */
//Command Type
#define CMD_ATVV_GET_CAPS                       0x0A
#define CMD_ATVV_MIC_OPEN                       0x0C
#define CMD_ATVV_MIC_CLOSE                      0x0D
#define CMD_ATVV_SEARCH_END                     0x0E

//Control Type
#define CTL_AUDIO_STOP                          0x00
#define CTL_AUDIO_START                         0x04
#define CTL_SEARCH_START                        0x08
#define CTL_AUDIO_SYNC                          0x0A
#define CTL_GET_CAPS_RESP                       0x0B
#define CTL_MIC_OPEN_ERROR                      0x0C

#define INVALID_CODEC                           0x0F01
#define REMOTE_TIMEOUT_FOR_MIC_OPEN             0x0F02
#define INVALID_STATE                           0x0F03


/*
 * LOCAL VARIABLES DEFINITION
 ****************************************************************************************
 */
static co_timer_t audio_delay_stop_timer;

/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */
/// Application ATVV Module Environment Structure
struct app_atvv_server_env_tag app_atvv_server_env;
uint8_t app_atvv_trans_data_buffer[BLE_ATVVS_FIFO_BUF_SIZE];
/*
 * EXTERN FUNCTIONS DECLARATION
 ****************************************************************************************
 */
extern const struct prf_task_cbs* atvv_server_prf_itf_get(void);


/*
 * LOCAL FUNCTIONS DECLARATION
 ****************************************************************************************
 */
static void _atvv_get_caps_respose(uint8_t conidx);
static void _atvv_mic_open(uint8_t conidx);
static void _atvv_mic_close(uint8_t conidx);
static void _atvv_search_end(void);


/*
 * GLOBAL FUNCTIONS
 ****************************************************************************************
 */
void app_atvv_recv_handler(struct atvv_server_write_ind const *param)
{
    uint8_t data[32];
    
    memcpy(data, param->value, param->length);
    log_debug_array_ex("[ATVV] atvv received data", data, param->length);
    
    switch(data[0])
    {
        case CMD_ATVV_GET_CAPS:
            log_debug("[ATVV] CMD_ATVV_GET_CAPS\r\n");
            _atvv_get_caps_respose(param->conidx);
            break;
        
        case CMD_ATVV_MIC_OPEN:
            log_debug("[ATVV] CMD_ATVV_MIC_OPEN\r\n");
            _atvv_mic_open(param->conidx);
            break;
        
        case CMD_ATVV_MIC_CLOSE:
            log_debug("[ATVV] CMD_ATVV_MIC_CLOSE\r\n");
            _atvv_mic_close(param->conidx);
            break;
        
        case CMD_ATVV_SEARCH_END:
            log_debug("[ATVV] CMD_ATVV_SEARCH_END\r\n");
            _atvv_search_end();
            break;
        
        default:
            log_debug("[ATVV] unkown command\r\n");
            break;
    }
}

void app_atvv_notify_audio_frame(uint8_t *input, uint8_t len, int16_t prevsample, uint16_t previndex)
{
    uint8_t buf[200];

    buf[0] = app_atvv_server_env.seq_num >> 8;
    buf[1] = app_atvv_server_env.seq_num & 0x00FF;
    buf[2] = 0x01;
    buf[3] = (uint8_t)(prevsample >> 8);
    buf[4] = prevsample & 0x00FF;
    buf[5] = previndex & 0x00FF;
    
    memcpy(buf+6, input, len);
    app_atvv_server_send_data(0, buf, len+6);
    
    app_atvv_server_env.seq_num++;
}


/*
 * LOCAL FUNCTIONS
 ****************************************************************************************
 */
static void _atvv_get_caps_respose(uint8_t conidx)
{
    uint8_t caps[] = { 
        CTL_GET_CAPS_RESP,
        0x00, 0x04, // VER:0.4
        0x00, 0x01, // CODEC: ADPCM/8k/16 & 16k/16)
        0x00, 0x86, // 134 bytes/frame  
        0x00, 0x14  // 20 bytes/characteristic
    }; 
    app_atvv_server_send_control(conidx, &caps[0], 9);

    log_debug_array_ex("[ATVV] Get capability response ", caps, 9);
}

static void audio_delay_stop_timer_handler(co_timer_t *timer, void *param)
{
    app_audio_stop();
}

static void _atvv_mic_open(uint8_t conidx)
{
    uint8_t buf = CTL_AUDIO_START;
    app_atvv_server_send_control(conidx, &buf, sizeof(buf));
    co_timer_set(&audio_delay_stop_timer, 30000, TIMER_ONE_SHOT, audio_delay_stop_timer_handler, NULL);
    
    app_atvv_server_env.seq_num = 0;
    app_audio_start();
}

static void _atvv_mic_close(uint8_t conidx)
{
    app_audio_stop();
    
    uint8_t buf = CTL_AUDIO_STOP;
    app_atvv_server_send_control(conidx, &buf, 1);
}

static void _atvv_search_end(void)
{
    log_debug("[ATVV] Search end.\r\n");
}

void app_atvv_server_init(void)
{
    // Reset the environment
    memset(&app_atvv_server_env, 0, sizeof(struct app_atvv_server_env_tag));
}


void app_atvv_server_add_service(void)
{
    struct atvv_server_db_cfg* db_cfg;
    // Allocate the CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
            TASK_GAPM, TASK_APP,
            gapm_profile_task_add_cmd, sizeof(struct atvv_server_db_cfg));
    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl     = PERM(SVC_AUTH, NO_AUTH)|PERM(SVC_MI,ENABLE);
    req->prf_task_id = TASK_ID_ATVVS;
    req->app_task    = TASK_APP;
    req->start_hdl   = 0;

    // Set parameters and add some parameter if needed
    db_cfg = (struct atvv_server_db_cfg* ) req->param;
    db_cfg->fifo_buffer = app_atvv_trans_data_buffer;
    db_cfg->fifo_size = BLE_ATVVS_FIFO_BUF_SIZE;
    db_cfg->connect_num = BLE_APP_ATVVS_CONNECT_MAX; // max connect num
    db_cfg->svc_type= 1; // 128 uuid ,use to bt audio
    // Send the message
    ke_msg_send(req);
}

void app_atvv_server_enable_prf(uint8_t conidx)
{
    //    app_atvv_server_env.conidx[conidx] = conidx;
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
    struct atvv_server_enable_req * req = KE_MSG_ALLOC(ATVV_SERVER_ENABLE_REQ,
            KE_BUILD_ID(prf_get_task_from_id(TASK_ID_ATVVS), conidx),
            KE_BUILD_ID(TASK_APP,conidx),
            atvv_server_enable_req);

    // Fill in the parameter structure
    req->conidx             = conidx;
    req->en = 1;

    // Send the message
    ke_msg_send(req);
}

void app_atvv_server_disable_prf(uint8_t conidx)
{
    //    app_atvv_server_env.conidx[conidx] = GAP_INVALID_CONIDX;

    uint8_t is_find=0;

    app_atvv_server_env.ntf_en[conidx][0] = 0;
    app_atvv_server_env.ntf_en[conidx][1] = 0;
    for(uint8_t i=0;i<BLE_APP_ATVVS_CONNECT_MAX;i++)
    {
        if (app_atvv_server_env.ntf_en[i])
        {
            is_find++;
        }
    }
    if (is_find == 0)
    {
        ke_timer_clear(ATVV_SERVER_TIMEOUT_TIMER, TASK_APP);
    }

    struct atvv_server_enable_req * req = KE_MSG_ALLOC(ATVV_SERVER_ENABLE_REQ,
            KE_BUILD_ID(prf_get_task_from_id(TASK_ID_ATVVS), conidx),
            KE_BUILD_ID(TASK_APP,conidx),
            atvv_server_enable_req);

    // Fill in the parameter structure
    req->conidx             = conidx;
    req->en =0;

    // Send the message
    ke_msg_send(req);
}

static int atvv_server_enable_rsp_handler(ke_msg_id_t const msgid,
        struct atvv_server_enable_rsp const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

static int atvv_server_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
        struct atvv_server_ntf_cfg_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    log_debug("%s@%d,src=%d,dest=%d\n", __func__, __LINE__, KE_IDX_GET(src_id), KE_IDX_GET(dest_id));

    uint8_t conidx = KE_IDX_GET(src_id);
    uint8_t is_find=0;
    for(uint8_t i=0;i<BLE_APP_ATVVS_CONNECT_MAX;i++)
    {
        if (app_atvv_server_env.ntf_en[i])
        {
            is_find++;
        }
    }

    if(param->ntf_cfg == PRF_CLI_START_NTF || param->ntf_cfg == PRF_CLI_START_IND){
        app_atvv_server_env.ntf_en[conidx][param->type] = 1;
    }else if(param->ntf_cfg == PRF_CLI_STOP_NTFIND){
        app_atvv_server_env.ntf_en[conidx][param->type] = 0;
    }
    return (KE_MSG_CONSUMED);
}

static int atvv_server_send_empty_handler(ke_msg_id_t const msgid,
        struct atvv_server_buffer_empty_evt const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    //uint8_t conidx = KE_IDX_GET(src_id);
    APP_HANDLER(atvv_empty, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

static int atvv_server_send_full_handler(ke_msg_id_t const msgid,
        struct atvv_server_buffer_empty_evt const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    APP_HANDLER(atvv_full, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

static int atvv_server_write_ind_handler(ke_msg_id_t const msgid,
        struct atvv_server_write_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    //uint8_t conidx = KE_IDX_GET(src_id);
    //log_debug("%s@%d\n", __func__, __LINE__);
    //log_debug_array_ex("write data", param->value, param->length);
    //appm_recv_data(conidx, (uint8_t*)param->value, param->length);
    APP_HANDLER(atvv_recv, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

static int atvv_server_error_handler(ke_msg_id_t const msgid,
        struct atvv_server_error_evt const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    APP_HANDLER(atvv_error, msgid, param, dest_id, src_id);
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
static int app_atvv_server_msg_dflt_handler(ke_msg_id_t const msgid,
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
const struct ke_msg_handler app_atvv_server_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_atvv_server_msg_dflt_handler},
    {ATVV_SERVER_ENABLE_RSP,        (ke_msg_func_t)atvv_server_enable_rsp_handler},
    {ATVV_SERVER_NTF_CFG_IND,       (ke_msg_func_t)atvv_server_ntf_cfg_ind_handler},
    //{ATVV_SERVER_TIMEOUT_TIMER,     (ke_msg_func_t)atvv_server_timeout_timer_handler},
    {ATVV_SERVER_BUFFER_EMPTY,      (ke_msg_func_t)atvv_server_send_empty_handler},
    {ATVV_SERVER_BUFFER_FULL,       (ke_msg_func_t)atvv_server_send_full_handler},
    {ATVV_SERVER_WRITE_IND,         (ke_msg_func_t)atvv_server_write_ind_handler},
    {ATVV_SERVER_ERROR,             (ke_msg_func_t)atvv_server_error_handler},
};

const struct app_subtask_handlers app_atvv_server_handlers = APP_HANDLERS(app_atvv_server);


void app_atvv_server_send_data(uint8_t conidx, uint8_t* pdata, uint8_t len)
{
    atvv_server_send_data(conidx, pdata, len);
#if 0
    // Allocate the message
    if(atvv_server_get_free_size(conidx)< len)  
    {
        return;
    }
    struct atvv_server_send_ntf_cmd * cmd = KE_MSG_ALLOC_DYN(ATVV_SERVER_SEND_NTF_CMD,
            KE_BUILD_ID(prf_get_task_from_id(TASK_ID_ATVVS), conidx),
            KE_BUILD_ID(TASK_APP, conidx),
            atvv_server_send_ntf_cmd,
            len);
    
    cmd->conidx = conidx;
    cmd->length = len;
    memcpy(cmd->value, pdata, len);
    // Send the message
    ke_msg_send(cmd);
    #endif
}

void app_atvv_server_send_control(uint8_t conidx, uint8_t* pdata, uint16_t len)
{
    // Allocate the message
    struct atvv_server_send_ctl_cmd * cmd = KE_MSG_ALLOC_DYN(ATVV_SERVER_SEND_CTL_CMD,
            KE_BUILD_ID(prf_get_task_from_id(TASK_ID_ATVVS), conidx),
            KE_BUILD_ID(TASK_APP, conidx),
            atvv_server_send_ctl_cmd,
            len);
    cmd->conidx = conidx;
    cmd->length = len;
    memcpy(cmd->value, pdata, len);
    // Send the message
    ke_msg_send(cmd);
}
#endif
/// @} APP


