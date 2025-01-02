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

#if (BLE_APP_CENTRAL_SUPPORT)
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_task.h"                // application task definitions
#include "app.h"                     // Application Definitions
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
#include "app_central.h"
#include "app.h"
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
    uint8_t index = app_central_find_by_conidx(conidx);
    
    log_debug("%s@%d, conidx=%d, index=%x\n", __func__, __LINE__, conidx, index);
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
                app_env.central.connect[index].write_handle==0 && (val_prop&ATT_CHAR_PROP_WR)== ATT_CHAR_PROP_WR)
            {
                app_env.central.connect[index].write_handle = val_hdl;
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
                app_env.central.connect[index].notify_handle==0 && uuid == ATT_DESC_CLIENT_CHAR_CFG)
            {
                app_env.central.connect[index].notify_handle = char_hdl;
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
    uint8_t index = app_central_find_by_conidx(conidx);
    log_debug("%s@%d, opra=%x,status=%x, index=%x\n", __func__, __LINE__, param->operation, param->status, index);
    switch(param->operation)
    {
        case GATTC_WRITE:
        case GATTC_WRITE_NO_RESPONSE:
            {
            }
            break;
        case GATTC_SDP_DISC_SVC_ALL:
            {
                if (param->status == GAP_ERR_NO_ERROR && app_env.central.connect[index].notify_handle !=0)
                {
                    log_debug("start notify, handle=%x(%d)\n", app_env.central.connect[index].notify_handle, conidx);
                    prf_gatt_write_ntf_ind(&app_env.central.prf_env, 
                                            conidx,app_env.central.connect[index].notify_handle, 
                                            PRF_CLI_START_NTF);
                }
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
    log_debug("%s@%d, type=%x,len=%x\n", __func__, __LINE__, param->type, param->length);

    if(param->type == GATTC_NOTIFY)
    {
        app_central_recv_handler(conidx, param->handle, param->length, (uint8_t*)param->value);
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

uint16_t app_gattc_send_data(uint16_t conidx, uint8_t *p_pdata, uint16_t len)
{
    uint8_t index = app_central_find_by_conidx(conidx);

    log_debug("gattc send data %x->%x, %d\r\n", conidx, index, len);
    
    uint16_t templen = app_gattc_perfect_once_tx_length(app_env.central.connect[index].mtu, 
                                                        app_env.central.connect[index].tx_len, 
                                                        APP_GATTC_UPLOAD_MAX_LEN);
    templen = co_min(len, templen);
    prf_gatt_write(&app_env.central.prf_env, 
                    app_env.central.connect[index].conidx,
                    app_env.central.connect[index].write_handle, 
                    p_pdata, 
                    templen, 
                    GATTC_WRITE);
    return templen;
}

#endif
/// @} APP
