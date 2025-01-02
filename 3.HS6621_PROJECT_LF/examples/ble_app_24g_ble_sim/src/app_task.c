/**
 ****************************************************************************************
 *
 * @file appm_task.c
 *
 * @brief RW APP Task implementation
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APPTASK
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"          // SW configuration

#if (BLE_APP_PRESENT)

#include "app_task.h"             // Application Manager Task API
#include "app.h"                  // Application Manager Definition
#include "gapc_task.h"            // GAP Controller Task API
#include "gapm_task.h"            // GAP Manager Task API
#include "arch.h"                 // Platform Definitions
#include <string.h>
#include "co_utils.h"
#include "ke_timer.h"             // Kernel timer
#include "co_debug.h"
#include "gattc_task.h"

#if (BLE_APP_SEC)
#include "app_sec.h"              // Security Module Definition
#endif //(BLE_APP_SEC)
#include "prf_utils.h"
#include "app_gattc.h"

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static uint8_t app_get_handler(const struct app_subtask_handlers *handler_list_desc,
                               ke_msg_id_t msgid,
                               void *param,
                               ke_task_id_t src_id)
{
    // Counter
    uint8_t counter;

    // Get the message handler function by parsing the message table
    for (counter = handler_list_desc->msg_cnt; 0 < counter; counter--)
    {
        struct ke_msg_handler *handler
                = (struct ke_msg_handler*)((handler_list_desc->p_msg_handler_tab + counter - 1));

        if ((handler->id == msgid) ||
            (handler->id == KE_MSG_DEFAULT_HANDLER))
        {
            // If handler is NULL, message should not have been received in this state
            ASSERT_ERR(handler->func);

            return (uint8_t)(handler->func(msgid, param, TASK_APP, src_id));
        }
    }

    // If we are here no handler has been found, drop the message
    return (KE_MSG_CONSUMED);
}

/*
 * MESSAGE HANDLERS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles GAPM_ACTIVITY_CREATED_IND event
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_activity_created_ind_handler(ke_msg_id_t const msgid,
                                             struct gapm_activity_created_ind const *p_param,
                                             ke_task_id_t const dest_id,
                                             ke_task_id_t const src_id)
{
    log_debug("%s@%d, type=%x, index=%x\n", __func__, __LINE__, p_param->actv_type, p_param->actv_idx);

    if (p_param->actv_type == GAPM_ACTV_TYPE_ADV) {
        appm_adv_create_ind_handler((void*)p_param);
    }
    #if (BLE_APP_CENTRAL_SUPPORT)
    else if (p_param->actv_type == GAPM_ACTV_TYPE_SCAN || p_param->actv_type == GAPM_ACTV_TYPE_INIT)
    {
        app_central_update_state(p_param->actv_type, p_param->actv_idx, 0);
    }
    #endif

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles GAPM_ACTIVITY_STOPPED_IND event.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_activity_stopped_ind_handler(ke_msg_id_t const msgid,
                                             struct gapm_activity_stopped_ind const *p_param,
                                             ke_task_id_t const dest_id,
                                             ke_task_id_t const src_id)
{
    log_debug("%s@%d, index=%x, reason=%x. type=%x\n", __func__, __LINE__, p_param->actv_idx, p_param->reason, p_param->actv_type);
    if (p_param->actv_type == GAPM_ACTV_TYPE_ADV) {
        appm_adv_stopped_ind_handler((void*)p_param);
    }
#if (BLE_APP_CENTRAL_SUPPORT)
    else if (p_param->actv_type == GAPM_ACTV_TYPE_SCAN || p_param->actv_type == GAPM_ACTV_TYPE_INIT)
    {
        app_central_update_state(p_param->actv_type, p_param->actv_idx, p_param->reason);
    }
#endif

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles GAPM_PROFILE_ADDED_IND event
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_profile_added_ind_handler(ke_msg_id_t const msgid,
                                          struct gapm_profile_added_ind *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    // Current State
    ke_state_t state = ke_state_get(dest_id);

    if (state == APPM_CREATE_DB)
    {
        switch (param->prf_task_id)
        {

            default: /* Nothing to do */ break;
        }
    }
    else
    {
        ASSERT_INFO(0, state, src_id);
    }
    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles GAPM_EXT_ADV_REPORT_IND event.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_ext_adv_report_ind_handler(ke_msg_id_t const msgid,
                                             struct gapm_ext_adv_report_ind const *p_param,
                                             ke_task_id_t const dest_id,
                                             ke_task_id_t const src_id)
{
    #if (BLE_APP_CENTRAL_SUPPORT)
    uint8_t cursor = 0;
    uint8_t is_find = 0;

    log_debug("%02x:%02x:%02x:%02x:%02x:%02x\n", p_param->trans_addr.addr.addr[0],
                                                        p_param->trans_addr.addr.addr[1],
                                                        p_param->trans_addr.addr.addr[2],
                                                        p_param->trans_addr.addr.addr[3],
                                                        p_param->trans_addr.addr.addr[4],
                                                        p_param->trans_addr.addr.addr[5]);

    // Parse advertising data and look for ad_type
    while (cursor < p_param->length)
    {
        // Check if it's AD Type flag data
        // find "BLE-TSPP"
        if ((p_param->data[cursor + 1] == GAP_AD_TYPE_COMPLETE_NAME || p_param->data[cursor + 1] == GAP_AD_TYPE_SHORTENED_NAME)
                && (p_param->data[cursor]-1) == APP_SCAN_DEVICE_NAME_LEN)
        {
            if (memcmp(APP_SCAN_DEVICE_NAME, &p_param->data[cursor + 2], APP_SCAN_DEVICE_NAME_LEN)== 0)
            {
                is_find = 1;
                break;
            }
        }

        // Go to next advertising info
        cursor += (p_param->data[cursor] + 1);
    }

    if (is_find)
    {
        //log_debug("%s@%d\n", __func__, __LINE__);
        log_debug("%02x:%02x:%02x:%02x:%02x:%02x\n", p_param->trans_addr.addr.addr[0],
                                                    p_param->trans_addr.addr.addr[1],
                                                    p_param->trans_addr.addr.addr[2],
                                                    p_param->trans_addr.addr.addr[3],
                                                    p_param->trans_addr.addr.addr[4],
                                                    p_param->trans_addr.addr.addr[5]);
        //if (app_env.central.w4connect == 0) {
        //    memcpy(&(app_env.central.trans_addr), &(p_param->trans_addr), sizeof(struct gap_bdaddr));
        //    app_env.central.w4connect = 1;
        //    app_central_update_state(2, 0, 0);
        //}
    }
    #endif
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles GAP manager command complete events.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapm_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    #if ((NVDS_SUPPORT) && (BLE_APP_SEC))
    nvds_tag_len_t key_len = KEY_LEN;
    #endif //(NVDS_SUPPORT)
    //log_debug("%s@%d, operation=%x,status=%x\n", __func__, __LINE__, param->operation, param->status);

    switch(param->operation)
    {
        // Reset completed
        case (GAPM_RESET):
        {

            if(param->status == GAP_ERR_NO_ERROR)
            {
                #if (NVDS_SUPPORT)
                nvds_tag_len_t len=6;
                #endif //(NVDS_SUPPORT)
                #if (BLE_APP_HID)
                app_hid_start_mouse();
                #endif //(BLE_APP_HID)

                // Set Device configuration
                struct gapm_set_dev_config_cmd* cmd = KE_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,
                                                                   TASK_GAPM, TASK_APP,
                                                                   gapm_set_dev_config_cmd);
                // Set the operation
                cmd->operation = GAPM_SET_DEV_CONFIG;
                // Set the device role - Peripheral
                cmd->role      = GAP_ROLE_ALL;

                #if (BLE_APP_SEC_CON)
                // The Max MTU is increased to support the Public Key exchange
                // HOWEVER, with secure connections enabled you cannot sniff the
                // LEAP and LEAS protocols
                cmd->max_mtu = 160;
                cmd->pairing_mode = GAPM_PAIRING_SEC_CON | GAPM_PAIRING_LEGACY;
                #else // !(BLE_APP_SEC_CON)
                // Do not support secure connections
                #if (BLE_APP_SEC)
                cmd->pairing_mode = GAPM_PAIRING_SEC_CON | GAPM_PAIRING_LEGACY;
                #else
                cmd->pairing_mode = GAPM_PAIRING_DISABLE;
                #endif
                #endif //(BLE_APP_SEC_CON)

                // Set Data length parameters
                cmd->sugg_max_tx_octets = BLE_MAX_OCTETS;
                cmd->sugg_max_tx_time   = BLE_MAX_TIME;

                /// Preferred LE PHY for data transmission (@see enum gap_phy)
                //cmd->tx_pref_phy = GAP_PHY_LE_2MBPS;
                /// Preferred LE PHY for data reception (@see enum gap_phy)
                //cmd->rx_pref_phy = GAP_PHY_LE_2MBPS;

                // Maximal MTU/MPS acceptable for device
                cmd->max_mtu = BLE_MAX_OCTETS;
                cmd->max_mps = GAP_MAX_LE_MTU;

                // Enable Slave Preferred Connection Parameters present
                cmd->att_cfg = GAPM_MASK_ATT_SLV_PREF_CON_PAR_EN;

                // Host privacy enabled by default
                #if 0
                cmd->privacy_cfg = 0;
                #else
                cmd->privacy_cfg = GAPM_PRIV_CFG_PRIV_ADDR_BIT;
                #endif
                
                #if (NVDS_SUPPORT)
                if (nvds_get(NVDS_TAG_BD_ADDRESS, &len, &cmd->addr.addr[0]) == NVDS_OK)
                {
                    // Check if address is a static random address
                    if (cmd->addr.addr[5] & 0xC0)
                    {
                        // Host privacy enabled by default
                        cmd->privacy_cfg |= GAPM_PRIV_CFG_PRIV_ADDR_BIT;
                    }
                }
                #endif //(NVDS_SUPPORT)

                #if (BLE_APP_AM0)
                cmd->audio_cfg   = GAPM_MASK_AUDIO_AM0_SUP;
                cmd->att_cfg    |= GAPM_MASK_ATT_SVC_CHG_EN;
                #endif //(BLE_APP_AM0)


                #if ((NVDS_SUPPORT) && (BLE_APP_SEC))

                if ((app_sec_get_bond_status()==true) &&
                    (nvds_get(NVDS_TAG_LOC_IRK, &key_len, app_env.loc_irk) == NVDS_OK))
                {
                    memcpy(cmd->irk.key, app_env.loc_irk, 16);
                }
                else
                #endif //(NVDS_SUPPORT)
                {
                    memset((void *)&cmd->irk.key[0], 0x00, KEY_LEN);
                }
                // Send message
                ke_msg_send(cmd);
            }
            else
            {
                ASSERT_ERR(0);
            }
        }
        break;

        case (GAPM_PROFILE_TASK_ADD):
        {
            if (appm_add_svc()) break;
            
            #if(BLE_APP_SEC)
            if (app_sec_get_bond_status()==true)
            {
                #if ((NVDS_SUPPORT) && (BLE_APP_SEC))
                // If Bonded retrieve the local IRK from NVDS
                if (nvds_get(NVDS_TAG_LOC_IRK, &key_len, app_env.loc_irk) == NVDS_OK)
                {
                    // Set the IRK in the GAP
                    struct gapm_set_irk_cmd *cmd = KE_MSG_ALLOC(GAPM_SET_IRK_CMD,
                                                                TASK_GAPM, TASK_APP,
                                                                gapm_set_irk_cmd);
                    ///  - GAPM_SET_IRK:
                    cmd->operation = GAPM_SET_IRK;
                    memcpy(&cmd->irk.key[0], &app_env.loc_irk[0], KEY_LEN);
                    ke_msg_send(cmd);
                }
                else
                #endif //(NVDS_SUPPORT)
                {
                     // If cannot read IRK from NVDS ASSERT
                     ASSERT_ERR(0);
                }
            }
            else // Need to start the generation of new IRK
            #endif
            {
                struct gapm_gen_rand_nb_cmd *cmd = KE_MSG_ALLOC(GAPM_GEN_RAND_NB_CMD,
                                                                TASK_GAPM, TASK_APP,
                                                                gapm_gen_rand_nb_cmd);
                cmd->operation   = GAPM_GEN_RAND_NB;
                app_env.rand_cnt = 1;
                ke_msg_send(cmd);
            }
        }
        break;

        case (GAPM_GEN_RAND_NB) :
        {
            if (app_env.rand_cnt == 1)
            {
                // Generate a second random number
                app_env.rand_cnt++;
                struct gapm_gen_rand_nb_cmd *cmd = KE_MSG_ALLOC(GAPM_GEN_RAND_NB_CMD,
                                                                TASK_GAPM, TASK_APP,
                                                                gapm_gen_rand_nb_cmd);
                cmd->operation = GAPM_GEN_RAND_NB;
                ke_msg_send(cmd);
            }
            else
            {
                struct gapm_set_irk_cmd *cmd = KE_MSG_ALLOC(GAPM_SET_IRK_CMD,
                                                        TASK_GAPM, TASK_APP,
                                                        gapm_set_irk_cmd);
                app_env.rand_cnt=0;
                ///  - GAPM_SET_IRK
                cmd->operation = GAPM_SET_IRK;
                memcpy(&cmd->irk.key[0], &app_env.loc_irk[0], KEY_LEN);
                ke_msg_send(cmd);
            }
        }
        break;

        case (GAPM_SET_IRK):
        {
            // ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->operation, param->status);

            // If not Bonded already store the generated value in NVDS
            #if(BLE_APP_SEC)
            if (app_sec_get_bond_status()==false)
            {
                #if ((NVDS_SUPPORT) && (BLE_APP_SEC))
                if (nvds_put(NVDS_TAG_LOC_IRK, KEY_LEN, (uint8_t *)&app_env.loc_irk) != NVDS_OK)
                #endif //(NVDS_SUPPORT)
                {
                    ASSERT_INFO(0, 0, 0);
                }
            }
            #endif

            app_env.rand_cnt = 0;
            // Add the next requested service
            if (!appm_add_svc())
            {
                // Go to the ready state
                ke_state_set(TASK_APP, APPM_READY);

                // No more service to add, start advertising
                #if (NVDS_SUPPORT)
                nvds_tag_len_t len=6;
                bd_addr_t s_temp_addr;
                if (nvds_get(NVDS_TAG_BD_ADDRESS, &len, &s_temp_addr.addr[0]) == NVDS_OK)
                {
                    gapm_set_bdaddr(&s_temp_addr);
                }
                #endif //(NVDS_SUPPORT)
                //appm_adv_start();
            }
        }
        break;

        // Device Configuration updated
        case (GAPM_SET_DEV_CONFIG):
        {
            ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->operation, param->status);

            // Go to the create db state
            ke_state_set(TASK_APP, APPM_CREATE_DB);
            appm_reg_svc_itf();
            // Add the first required service in the database
            // and wait for the PROFILE_ADDED_IND
            appm_add_svc();
        }
        break;

        case (GAPM_CREATE_ADV_ACTIVITY):
        case (GAPM_SET_ADV_DATA):
        case (GAPM_SET_SCAN_RSP_DATA):
            appm_adv_cmp_evt_handler((void*)param);
            break;
        case (GAPM_STOP_ACTIVITY):
        case (GAPM_START_ACTIVITY):
        case (GAPM_DELETE_ACTIVITY):
        {
            // Sanity checks
            //ASSERT_INFO(app_env.adv_op == param->operation, app_env.adv_op, param->operation);
            //ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->status, app_env.adv_op);
            //if (param->status !=0) break;
            // Perform next operation
            appm_adv_cmp_evt_handler((void*)param);
            #if (BLE_APP_CENTRAL_SUPPORT)
            app_central_update_state(param->operation, 0, param->status);
            #endif
        } break;

#if (BLE_APP_CENTRAL_SUPPORT)
        case (GAPM_CREATE_SCAN_ACTIVITY):
        case (GAPM_CREATE_INIT_ACTIVITY):
        {
            app_central_update_state(param->operation, 0, param->status);
        }break;
#endif

        case (GAPM_DELETE_ALL_ACTIVITIES) :
        {
            appm_adv_cmp_evt_handler((void*)param);
            app_central_update_state(param->operation, 0, param->status);
        } break;


        default:
        {
            // Drop the message
        }
        break;
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of random number generated message
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_gem_max_data_len_ind_handler(ke_msg_id_t const msgid, struct gapm_max_data_len_ind *param,
                                        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    log_debug("gapm:max tx=%d, time=0x%x, rx=%d, time=0x%x\n", param->suppted_max_tx_octets, param->suppted_max_tx_time,
                                                        param->suppted_max_rx_octets, param->suppted_max_rx_time);
    tspp_server_set_data_len(KE_IDX_GET(src_id), param->suppted_max_tx_octets);
    return KE_MSG_CONSUMED;
}

#if 0
static void app_tspp_get_max_tx_len(void)
{
    struct gapm_get_dev_info_cmd *req = ke_msg_alloc(GAPM_GET_DEV_INFO_CMD,
                                                    TASK_GAPM, TASK_APP,
                                                    sizeof(struct gapm_get_dev_info_cmd));
    req->operation = GAPM_GET_MAX_LE_DATA_LEN;
    ke_msg_send(req);
}
#endif

static int gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_get_dev_info_req_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    switch(param->req)
    {
        case GAPC_DEV_NAME:
        {
            struct gapc_get_dev_info_cfm * cfm = KE_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,
                                                    src_id, dest_id,
                                                    gapc_get_dev_info_cfm, APP_DEVICE_NAME_MAX_LEN);
            cfm->req = param->req;
            cfm->info.name.length = appm_get_dev_name(cfm->info.name.value);

            // Send message
            ke_msg_send(cfm);
        } break;

        case GAPC_DEV_APPEARANCE:
        {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                                                             src_id, dest_id,
                                                             gapc_get_dev_info_cfm);
            cfm->req = param->req;
            // No appearance
            cfm->info.appearance = 0;

            // Send message
            ke_msg_send(cfm);
        } break;

        case GAPC_DEV_SLV_PREF_PARAMS:
        {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                    src_id, dest_id,
                                                            gapc_get_dev_info_cfm);
            cfm->req = param->req;
            // Slave preferred Connection interval Min
            cfm->info.slv_pref_params.con_intv_min = 0x0c;
            // Slave preferred Connection interval Max
            cfm->info.slv_pref_params.con_intv_max = 0x0c;
            // Slave preferred Connection latency
            cfm->info.slv_pref_params.slave_latency  = 0;
            // Slave preferred Link supervision timeout
            cfm->info.slv_pref_params.conn_timeout    = 200;  // 2s (500*10ms)

            // Send message
            ke_msg_send(cfm);
        } break;

        default: /* Do Nothing */ break;
    }


    return (KE_MSG_CONSUMED);
}
/**
 ****************************************************************************************
 * @brief Handles GAPC_SET_DEV_INFO_REQ_IND message.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_set_dev_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_set_dev_info_req_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    // Set Device configuration
    struct gapc_set_dev_info_cfm* cfm = KE_MSG_ALLOC(GAPC_SET_DEV_INFO_CFM, src_id, dest_id,
                                                     gapc_set_dev_info_cfm);
    // Reject to change parameters
    cfm->status = GAP_ERR_REJECTED;
    cfm->req = param->req;
    // Send message
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles connection complete event from the GAP. Enable all required profiles
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_connection_req_ind_handler(ke_msg_id_t const msgid,
                                           struct gapc_connection_req_ind const *param,
                                           ke_task_id_t const dest_id,
                                           ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);
    #if(BLE_APP_SEC)
    bool is_bond = app_sec_get_bond_status_by_addr(param->peer_addr);
    #endif

    //app_env.conidx[conidx] = conidx;
    log_debug("Device type(%d) conidx(%d) handle(%x) role=%d,connected, ", param->peer_addr_type, conidx, param->conhdl, param->role);
    log_debug_array_ex("ADDR", &param->peer_addr, 6);
    // Check if the received Connection Handle was valid
    if (conidx != GAP_INVALID_CONIDX)
    {
#if (BLE_APP_CENTRAL_SUPPORT)
        if (param->role == 0) {// master
            app_central_set_conn_handle(conidx, param->conhdl);
            // Send connection confirmation
            struct gapc_connection_cfm *cfm = KE_MSG_ALLOC(GAPC_CONNECTION_CFM,
                    src_id, dest_id,
                    gapc_connection_cfm);

#if(BLE_APP_SEC)
            cfm->auth      = is_bond ? GAP_AUTH_REQ_MITM_BOND : GAP_AUTH_REQ_MITM_NO_BOND; // TODO [FBE] restore valid data
#else // !(BLE_APP_SEC)
            cfm->auth      = GAP_AUTH_REQ_NO_MITM_NO_BOND;
#endif // (BLE_APP_SEC)
            // Send the message
            ke_msg_send(cfm);

#if (BLE_APP_SEC)
            if (is_bond == false)
            {
                // Ask for the peer device to either start encryption
                app_sec_bond_req(conidx);
            }
            else
#endif
            {
            #if(BLE_APP_SEC)
                app_sec_encrypt_req(conidx, param->peer_addr);
            #endif
                struct gattc_exc_mtu_cmd * mtu_cmd = KE_MSG_ALLOC_DYN(GATTC_EXC_MTU_CMD,
                        KE_BUILD_ID(TASK_GATTC, conidx), dest_id,
                        gattc_exc_mtu_cmd, 0);

                mtu_cmd->operation = GATTC_MTU_EXCH;

                // Send message
                ke_msg_send(mtu_cmd);
            }
        }
        else 
#endif
        {
            // Retrieve the connection info from the parameters
            app_dev_set_connect(conidx);

            // Send connection confirmation
            struct gapc_connection_cfm *cfm = KE_MSG_ALLOC(GAPC_CONNECTION_CFM,
                    src_id, dest_id,
                    gapc_connection_cfm);

#if(BLE_APP_SEC)
            cfm->auth      = is_bond ? GAP_AUTH_REQ_MITM_BOND : GAP_AUTH_REQ_MITM_NO_BOND; // TODO [FBE] restore valid data
#else // !(BLE_APP_SEC)
            cfm->auth      = GAP_AUTH_REQ_NO_MITM_NO_BOND;
#endif // (BLE_APP_SEC)
            // Send the message
            ke_msg_send(cfm);

            /*--------------------------------------------------------------
             * ENABLE REQUIRED PROFILES
             *--------------------------------------------------------------*/

#if (BLE_TSPP_SERVER)
            // Enable TSPP_SERVER Service
            app_tspp_server_enable_prf(conidx);
#endif //(BLE_APP_TSPP_SERVER)

            // We are now in connected State
            ke_state_set(dest_id, APPM_CONNECTED);

#if (BLE_APP_SEC)
            if (is_bond)
            {
                // Ask for the peer device to either start encryption
                app_sec_send_security_req(conidx);
            }
#endif // (BLE_APP_SEC)
        }
    }
    else
    {
        // No connection has been established, restart advertising
        //appm_adv_update_state(true);
        APP_HANDLER(gap_connected, msgid, param, dest_id, src_id); 
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles GAP controller command complete events.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapc_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    switch(param->operation)
    {
        case (GAPC_DISCONNECT):
            APP_HANDLER(gap_disconnected_compelete, msgid, param, dest_id, src_id); 
            break;
        case (GAPC_UPDATE_PARAMS):
        {
            if (param->status != GAP_ERR_NO_ERROR)
            {
//                appm_disconnect();
            }
        } break;
#if (BLE_APP_CENTRAL_SUPPORT)
#if (BLE_APP_SEC_CON)
        case (GAPC_BOND):
        {
            if (param->status == GAP_ERR_NO_ERROR)
            {
                uint8_t conidx = KE_IDX_GET(src_id);
                struct gattc_exc_mtu_cmd * mtu_cmd = KE_MSG_ALLOC_DYN(GATTC_EXC_MTU_CMD,
                        KE_BUILD_ID(TASK_GATTC, conidx), dest_id,
                        gattc_exc_mtu_cmd, 0);

                mtu_cmd->operation = GATTC_MTU_EXCH;

                // Send message
                ke_msg_send(mtu_cmd);
            }
        }
#endif
#endif
        default:
        {
        } break;
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles disconnection complete event from the GAP.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_disconnect_ind_handler(ke_msg_id_t const msgid,
                                      struct gapc_disconnect_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);
    log_debug("Disconnectde.num=%d, reason=%d,conidx=%x, handle=%x\n", app_env.connect_num, param->reason, conidx, param->conhdl);
#if (BLE_APP_CENTRAL_SUPPORT)
    app_central_disconnect(conidx);
#endif
    {
        if(app_dev_set_disconnect(conidx) < BLE_APP_TSPPS_CONNECT_MAX)
        {
            app_tspp_server_disable_prf(conidx);
            // Go to the ready state
            ke_state_set(TASK_APP, APPM_READY);
            APP_HANDLER(gap_disconnected, msgid, param, dest_id, src_id); // APP layer Callback

            // Restart Advertising
            //appm_adv_start();
        }
    }

    return (KE_MSG_CONSUMED);
}

/**
  * @brief Handles connect param update(>4.1) event from the GAP.
  *
  * @param[in] msgid	 Id of the message received.
  * @param[in] param	 Pointer to the parameters of the message.
  * @param[in] dest_id	 ID of the receiving task instance (TASK_GAP).
  * @param[in] src_id	 ID of the sending task instance.
  *
  * @return If the message was consumed or not.
  ****************************************************************************************
  */
static int gapc_param_update_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_param_update_req_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    //uint8_t conidx = KE_IDX_GET(src_id);
    // Prepare the GAPC_PARAM_UPDATE_CMD message
    struct gapc_param_update_cfm *cfm = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM,
            src_id, dest_id,
            gapc_param_update_cfm);
    log_debug("%s@%d\n", __func__, __LINE__);
    log_debug("interval=%d,%d, latency=%d,time=%d\n", param->intv_min, param->intv_max, param->latency, param->time_out);
    /// True to accept slave connection parameters, False else.
    cfm->accept = true;
    /// Minimum Connection Event Duration
    cfm->ce_len_min = 0x0;
    /// Maximum Connection Event Duration
    cfm->ce_len_max = 0xFFFF;

    // Send the message
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

static int gapc_pkt_size_ind_handler(ke_msg_id_t const msgid,  struct gapc_le_pkt_size_ind const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);

    log_debug("gapc:(%x) max tx=%d, time=0x%x, rx=%d, time=0x%x\n",
                conidx,
                param->max_tx_octets, 
                param->max_tx_time,
                param->max_rx_octets, 
                param->max_rx_time);
#if (BLE_APP_CENTRAL_SUPPORT)
    app_central_set_tx_len(conidx, param->max_tx_octets);
#endif
    if (app_dev_get_by_conidx(conidx) < BLE_APP_TSPPS_CONNECT_MAX) {
        tspp_server_set_data_len(conidx, param->max_tx_octets);
    }
    
    return KE_MSG_CONSUMED;
}
 /**
 ****************************************************************************************
 * @brief Handles reception of all messages sent from the lower layers to the application
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int appm_msg_handler(ke_msg_id_t const msgid,
                            void *param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
    // Retrieve identifier of the task from received message
    ke_task_id_t src_task_id = MSG_T(msgid);
    // Message policy
    uint8_t msg_pol = KE_MSG_CONSUMED;

    switch (src_task_id)
    {
        case (TASK_ID_GAPC):
        {
            #if (BLE_APP_SEC)
            if ((msgid >= GAPC_BOND_CMD) &&
                (msgid <= GAPC_SECURITY_IND))
            {
                // Call the Security Module
                msg_pol = app_get_handler(&app_sec_handlers, msgid, param, src_id);
            }
            #endif //(BLE_APP_SEC)
            // else drop the message
        } break;

        case (TASK_ID_GATTC):
        {
            // Service Changed - Drop
#if (BLE_APP_CENTRAL_SUPPORT)
            msg_pol = app_get_handler(&app_gattc_handlers, msgid, param, src_id);
#endif
        } break;

        #if (BLE_APP_DIS)
        case (TASK_ID_DISS):
        {
            // Call the Device Information Module
            msg_pol = app_get_handler(&app_dis_handlers, msgid, param, src_id);
        } break;
        #endif //(BLE_APP_DIS)

        #if (BLE_TSPP_SERVER)
        case (TASK_ID_TSPPS):
        {
            // Call the Audio Mode 0 Module
            msg_pol = app_get_handler(&app_tspp_server_handlers, msgid, param, src_id);
        } break;

        #endif //(BLE_TSPP_SERVER)

        default:
        {
        } break;
    }

    return (msg_pol);
}

/**
 ****************************************************************************************
 * @brief Handles reception of random number generated message
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_gen_rand_nb_ind_handler(ke_msg_id_t const msgid, struct gapm_gen_rand_nb_ind *param,
                                        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if (app_env.rand_cnt==1)      // First part of IRK
    {
        memcpy(&app_env.loc_irk[0], &param->randnb.nb[0], 8);
    }
    else if (app_env.rand_cnt==2) // Second part of IRK
    {
        memcpy(&app_env.loc_irk[8], &param->randnb.nb[0], 8);
    }

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of random number generated message
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_mtu_changed_ind_handler(ke_msg_id_t const msgid, struct gattc_mtu_changed_ind *param,
                                        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    uint8_t conidx = KE_IDX_GET(src_id);
    log_debug("conidx=%x,mtu=%d\n", conidx, param->mtu);
#if (BLE_APP_CENTRAL_SUPPORT)
    app_central_set_max_mtu(conidx, param->mtu);
#endif
    {
        if (app_dev_get_by_conidx(conidx) < BLE_APP_TSPPS_CONNECT_MAX)
            tspp_server_set_max_mtu(conidx, param->mtu);
    }
    return KE_MSG_CONSUMED;
}

/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */

/* Default State handlers definition. */
KE_MSG_HANDLER_TAB(appm)
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,    (ke_msg_func_t)appm_msg_handler},

    // GAPM messages
    {GAPM_PROFILE_ADDED_IND,    (ke_msg_func_t)gapm_profile_added_ind_handler},
    {GAPM_ACTIVITY_CREATED_IND, (ke_msg_func_t)gapm_activity_created_ind_handler},
    {GAPM_ACTIVITY_STOPPED_IND, (ke_msg_func_t)gapm_activity_stopped_ind_handler},
    {GAPM_CMP_EVT,              (ke_msg_func_t)gapm_cmp_evt_handler},
    {GAPM_GEN_RAND_NB_IND,      (ke_msg_func_t)gapm_gen_rand_nb_ind_handler},
    {GAPM_MAX_DATA_LEN_IND,     (ke_msg_func_t)gapm_gem_max_data_len_ind_handler},
    {GAPM_EXT_ADV_REPORT_IND,   (ke_msg_func_t)gapm_ext_adv_report_ind_handler},

    // GAPC messages
    {GAPC_GET_DEV_INFO_REQ_IND, (ke_msg_func_t)gapc_get_dev_info_req_ind_handler},
    {GAPC_SET_DEV_INFO_REQ_IND, (ke_msg_func_t)gapc_set_dev_info_req_ind_handler},
    {GAPC_CONNECTION_REQ_IND,   (ke_msg_func_t)gapc_connection_req_ind_handler},
    {GAPC_CMP_EVT,              (ke_msg_func_t)gapc_cmp_evt_handler},
    {GAPC_DISCONNECT_IND,       (ke_msg_func_t)gapc_disconnect_ind_handler},
    {GAPC_PARAM_UPDATE_REQ_IND, (ke_msg_func_t)gapc_param_update_req_ind_handler},
    {GAPC_LE_PKT_SIZE_IND,      (ke_msg_func_t)gapc_pkt_size_ind_handler},

    // gattc message
    {GATTC_MTU_CHANGED_IND,     (ke_msg_func_t)gattc_mtu_changed_ind_handler},
};

/* Defines the place holder for the states of all the task instances. */
ke_state_t appm_state[APP_IDX_MAX];

// Application task descriptor
const struct ke_task_desc TASK_DESC_APP = {appm_msg_handler_tab, appm_state, APP_IDX_MAX, ARRAY_LEN(appm_msg_handler_tab)};

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
