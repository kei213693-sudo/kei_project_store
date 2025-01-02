/**
 ****************************************************************************************
 *
 * @file app_gattc.c
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
#include "app.h"                     // Application Definitions
#include "app_task.h"                // application task definitions
#include "ke_timer.h"
#include "co_bt.h"
#include "co_utils.h"
#include "prf_types.h"               // Profile common types definition
#include "arch.h"                    // Platform Definitions
#include "prf.h"
#include <string.h>
#include "gattc_task.h"
#include "app_gattc.h"
#include "prf_utils.h"
/*
 * DEFINES
 ****************************************************************************************
 */
#define APP_GATTC_MTU_TO_NTF_WRTCMD_LEN(n)   ((n) - 3)
#define APP_GATTC_MTO_TO_NTF_WRTCMD_LEN(n)   ((n) - 7)
#define APP_GATTC_UPLOAD_MAX_LEN            (BLE_MAX_OCTETS-4-3)
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
#if (BLE_APP_TEST_AUTO_CALLBACK)
struct tapp_gattc_auto_test
{
    uint16_t tx_len;
    uint16_t tx_pos;
    uint16_t tx_loop_count;
    uint16_t rx_len;
    uint8_t  rx_buffer[BLE_APP_TEST_SEND_MAX_LEN];
    uint8_t  tx_buffer[BLE_APP_TEST_SEND_MAX_LEN];
};
static struct tapp_gattc_auto_test app_gattc_auto_test[BLE_APP_CENTRAL_CONNECT_MAX];
#else
static uint16_t app_notify_num = 0;
#endif
/// Application Module Environment Structure
/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_SDP_SVC_IND_HANDLER message.
 * The handler stores the found service details for service discovery.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
__STATIC int app_gattc_sdp_svc_ind_handler(ke_msg_id_t const msgid,
                                             struct gattc_sdp_svc_ind const *ind,
                                             ke_task_id_t const dest_id,
                                             ke_task_id_t const src_id)
{
    uint8_t fnd_att = 0;
    uint8_t conidx = KE_IDX_GET(src_id);
    uint16_t service_uuid = ((ind->uuid[1]<<8)|ind->uuid[0]);

    log_debug("%s@%d, conidx=%d\n", __func__, __LINE__, conidx);
    log_debug("uuid=0x%02x%02x,start=%x,end=%x\n", ind->uuid[1],ind->uuid[0], ind->start_hdl, ind->end_hdl);

    for (fnd_att=0; fnd_att< (ind->end_hdl - ind->start_hdl); fnd_att++)
    {
        if(ind->info[fnd_att].att_type == GATTC_SDP_ATT_CHAR)
        {
            uint16_t char_hdl = ind->start_hdl+ 1 + fnd_att;
            uint16_t val_hdl  = ind->info[fnd_att].att_char.handle;
            uint8_t  val_prop = ind->info[fnd_att].att_char.prop;

            log_debug("char =0x%02x, value_handle=0x%04x,prop=%x\n", char_hdl, val_hdl,val_prop);

            if (service_uuid != ATT_SVC_GENERIC_ACCESS && 
                service_uuid != ATT_SVC_GENERIC_ATTRIBUTE &&
                app_env.connected[conidx].write_handle==0 && (val_prop&ATT_CHAR_PROP_WR)== ATT_CHAR_PROP_WR)
            {
                app_env.connected[conidx].write_handle = val_hdl;
                log_debug("write handle=%x\n", val_hdl);
            }
        }
        else if(ind->info[fnd_att].att_type == GATTC_SDP_ATT_VAL)
        {
            uint16_t char_hdl = ind->start_hdl+ 1 + fnd_att;
            uint16_t val_hdl  = ind->info[fnd_att].att_char.handle;
            uint8_t  val_prop = ind->info[fnd_att].att_char.prop;

            log_debug("value=0x%02x,         uuid=0x%04x,prop=%x,uuid=%02x%02x\n", char_hdl, val_hdl, 
                        val_prop,ind->info[fnd_att].att.uuid[1],ind->info[fnd_att].att.uuid[0]);
        }
        else if(ind->info[fnd_att].att_type == GATTC_SDP_ATT_DESC)
        {
            uint16_t char_hdl = ind->start_hdl+ 1 + fnd_att;
            uint16_t val_hdl  = ind->info[fnd_att].att_char.handle;
            uint8_t  val_prop = ind->info[fnd_att].att_char.prop;
            uint16_t uuid = ((ind->info[fnd_att].att.uuid[1]<<8)|ind->info[fnd_att].att.uuid[0]);

            log_debug("desc= 0x%02x,         uuid=0x%04x,prop=%x,uuid=%02x%02x\n", char_hdl, val_hdl, 
                        val_prop,ind->info[fnd_att].att.uuid[1],ind->info[fnd_att].att.uuid[0]);
            if (service_uuid != ATT_SVC_GENERIC_ACCESS && 
                service_uuid != ATT_SVC_GENERIC_ATTRIBUTE &&
                app_env.connected[conidx].notify_handle==0 && uuid == ATT_DESC_CLIENT_CHAR_CFG)
            {
                app_env.connected[conidx].notify_handle = char_hdl;
                log_debug("notify handle=%x\n", char_hdl);
            }
        }
    }
    log_debug("\n\n");

    return (KE_MSG_CONSUMED);
}

/**
 * @brief app_gattc_perfect_once_tx_length()
 *
 * @param[in] mtu  
 * @param[in] mto  
 * @param[in] char_len  
 *
 * @return 
 **/
uint16_t app_gattc_perfect_once_tx_length(uint16_t mtu, uint16_t mto, uint16_t char_len)
{
    uint16_t mtux = co_min(APP_GATTC_MTU_TO_NTF_WRTCMD_LEN(mtu),char_len);
    uint16_t mtox = APP_GATTC_MTO_TO_NTF_WRTCMD_LEN(mto);

    return (mtux > mtox) ? (mtox + mto * ((mtux-mtox) / mto)) : (mtux);
}
#if (BLE_APP_TEST_AUTO_CALLBACK)
__STATIC int app_gattc_send_data(uint8_t conidx)
{
    uint16_t i=0;
    uint16_t len = app_gattc_perfect_once_tx_length(app_env.connected[conidx].mtu, 
                                                    app_env.connected[conidx].tx_len, 
                                                    APP_GATTC_UPLOAD_MAX_LEN);

    //log_error("tx len=%d, rx len%d, tx_pos=%d\n", app_gattc_auto_test[conidx].tx_len,
    //                                              app_gattc_auto_test[conidx].rx_len,
    //                                              app_gattc_auto_test[conidx].tx_pos);

    if (app_gattc_auto_test[conidx].rx_len == app_gattc_auto_test[conidx].tx_len)
    {
        app_gattc_auto_test[conidx].tx_len++;
        app_gattc_auto_test[conidx].tx_pos = 0;
        app_gattc_auto_test[conidx].rx_len = 0;
        if (app_gattc_auto_test[conidx].tx_len > BLE_APP_TEST_SEND_MAX_LEN) {
            app_gattc_auto_test[conidx].tx_len = 1;
            app_gattc_auto_test[conidx].tx_loop_count++;
            if (BLE_APP_TEST_SEND_LOOP !=0 && BLE_APP_TEST_SEND_LOOP == app_gattc_auto_test[conidx].tx_loop_count)
            {
                memset(&app_gattc_auto_test[conidx], 0, sizeof(struct tapp_gattc_auto_test));
                return (KE_MSG_CONSUMED);
            }
        }
        for(i=0;i<app_gattc_auto_test[conidx].tx_len; i++)
        {
            app_gattc_auto_test[conidx].tx_buffer[i] = i+1;
        }
        len = co_min(len,app_gattc_auto_test[conidx].tx_len);
        app_gattc_auto_test[conidx].tx_pos += len;
        prf_gatt_write(&app_env.prf_env, conidx,
                app_env.connected[conidx].write_handle, app_gattc_auto_test[conidx].tx_buffer, len, GATTC_WRITE);

    }
    else if(app_gattc_auto_test[conidx].tx_pos < app_gattc_auto_test[conidx].tx_len) 
    {
        len = co_min(len,(app_gattc_auto_test[conidx].tx_len-app_gattc_auto_test[conidx].tx_pos));
        prf_gatt_write(&app_env.prf_env, conidx,
                app_env.connected[conidx].write_handle, app_gattc_auto_test[conidx].tx_buffer+app_gattc_auto_test[conidx].tx_pos, len, GATTC_WRITE);
        app_gattc_auto_test[conidx].tx_pos += len;
    }

    return (KE_MSG_CONSUMED);
}

__STATIC int app_gattc_recv_data(uint8_t* value, uint16_t len, uint8_t conidx)
{
    memcpy(app_gattc_auto_test[conidx].rx_buffer+app_gattc_auto_test[conidx].rx_len, value, len);
    app_gattc_auto_test[conidx].rx_len += len;
    if (app_gattc_auto_test[conidx].rx_len == app_gattc_auto_test[conidx].tx_len)
    {
        if(memcmp(app_gattc_auto_test[conidx].rx_buffer, app_gattc_auto_test[conidx].tx_buffer, app_gattc_auto_test[conidx].tx_len)==0)
        {
            log_error("[%d]%d----ok\n", conidx,app_gattc_auto_test[conidx].tx_len);
            app_gattc_send_data(conidx);
        }
        else
        {
            log_error("[%d]%d----fail\n", conidx,app_gattc_auto_test[conidx].tx_len);
            log_debug_array_ex("send:", app_gattc_auto_test[conidx].tx_buffer, app_gattc_auto_test[conidx].tx_len);
            log_debug_array_ex("recv:", app_gattc_auto_test[conidx].rx_buffer, app_gattc_auto_test[conidx].rx_len);
        }
    }
    return (KE_MSG_CONSUMED);
}
#endif
/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_CMP_EVT message.
 * This generic event is received for different requests, so need to keep track.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
__STATIC int app_gattc_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gattc_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);

    //log_debug("%s@%d, opra=%x,status=%x\n", __func__, __LINE__, param->operation, param->status);
    switch(param->operation)
    {
        case GATTC_WRITE:
        case GATTC_WRITE_NO_RESPONSE:
            {
#if (BLE_APP_TEST_AUTO_CALLBACK)
                app_gattc_send_data(conidx);
#else
                if (param->status == GAP_ERR_NO_ERROR && app_env.connected[conidx].notify_handle !=0 && app_env.connected[conidx].notify_state ==0)
                {
                    app_env.connected[conidx].notify_state =1;
                    app_notify_num = 0;
                    prf_gatt_write_ntf_ind(&app_env.prf_env, conidx, app_env.connected[conidx].notify_handle, PRF_CLI_START_NTF);
                }
#endif
            }
            break;
        case GATTC_SDP_DISC_SVC_ALL:
            {
#if (BLE_APP_TEST_AUTO_CALLBACK)
                if (param->status == GAP_ERR_NO_ERROR && app_env.connected[conidx].notify_handle !=0 && app_env.connected[conidx].notify_state ==0)
                {
                    log_debug("start notify, handle=%x(%d)\n", app_env.connected[conidx].notify_handle, conidx);
                    memset(&app_gattc_auto_test[conidx], 0, sizeof(struct tapp_gattc_auto_test[conidx]));
                    prf_gatt_write_ntf_ind(&app_env.prf_env, conidx, app_env.connected[conidx].notify_handle, PRF_CLI_START_NTF);
                }
#else
                uint8_t temp_data[30];
                uint16_t len = sizeof(temp_data);
                memset(temp_data, 0x5a, len);
                if (param->status == GAP_ERR_NO_ERROR && app_env.connected[conidx].write_handle !=0)
                {
                    prf_gatt_write(&app_env.prf_env, conidx,
                            app_env.connected[conidx].write_handle, temp_data, len, GATTC_WRITE);
                }
#endif
            }
            break;
        default:
            break;
    }
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_READ_IND message.
 * Generic event received after every simple read command sent to peer server.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
__STATIC int app_gattc_read_ind_handler(ke_msg_id_t const msgid,
                                    struct gattc_read_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_EVENT_IND message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
__STATIC int app_gattc_event_ind_handler(ke_msg_id_t const msgid,
                                        struct gattc_event_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);
    //log_debug("%s@%d, type=%x,len=%x\n", __func__, __LINE__, param->type, param->length);

    if(param->type == GATTC_NOTIFY)
    {

#if (BLE_APP_TEST_AUTO_CALLBACK)
        app_gattc_recv_data((uint8_t*)param->value, param->length, conidx);
#else
        app_notify_num++;
        if (app_notify_num ==20)
        {
            prf_gatt_write_ntf_ind(&app_env.prf_env, conidx, app_env.connected[conidx].notify_handle, PRF_CLI_STOP_NTFIND);
        }
#endif
    }
    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GATTC).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_gattc_msg_dflt_handler(ke_msg_id_t const msgid,
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
const struct ke_msg_handler app_gattc_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_gattc_msg_dflt_handler},
    {GATTC_SDP_SVC_IND,             (ke_msg_func_t)app_gattc_sdp_svc_ind_handler},
    {GATTC_CMP_EVT,                 (ke_msg_func_t)app_gattc_cmp_evt_handler},
    {GATTC_EVENT_IND,               (ke_msg_func_t)app_gattc_event_ind_handler},
    {GATTC_READ_IND,                (ke_msg_func_t)app_gattc_read_ind_handler},

};

const struct app_subtask_handlers app_gattc_handlers = APP_HANDLERS(app_gattc);

/// @} APP
