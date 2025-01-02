/**
 ****************************************************************************************
 *
 * @file app_task.c
 *
 * @brief BLE mesh app task entry.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"          // SW configuration

#if (BLE_APP_PRESENT)

#include "app.h"
#include "app_task.h"
#include "gapc_task.h"
#include "gapm_task.h"
#include "gattc_task.h"

#include "app_mesh.h"
#include "m_tb.h"

#if BLE_MESH_DBG
#include "m_dbg.h"
#endif

#if APP_MESH_TMALL
#include "tmall_led_demo.h"
#endif

#if BLE_APP_ONMICRO_DFU
#include "peripheral.h"
#include "onmicro_dfu.h"
#endif

#include "prf_utils.h"
/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
#if APP_MESH_TUYA
static bool g_proxy_feature_en = true;
static bool g_relay_feature_en = true;

#if CFG_BLE_MESH_FRIEND
static bool g_friend_feature_en = false;
#endif

#if CFG_BLE_MESH_LPN
static bool g_lpn_feature_en = false;
#endif

void m_proxy_feature_en(bool en)
{
    g_proxy_feature_en = en;
}

void m_relay_feature_en(bool en)
{
    g_relay_feature_en = en;
}

#if CFG_BLE_MESH_FRIEND
void m_friend_feature_en(bool en)
{
    g_friend_feature_en = en;
}
#else
void m_friend_feature_en(bool en) {}
#endif

#if CFG_BLE_MESH_LPN
void m_lpn_feature_en(bool en)
{
    g_lpn_feature_en = en;
}
#else
void m_lpn_feature_en(bool en){}
#endif

#endif

#if (BLE_APP_SEC || BLE_APP_DIS || BLE_APP_SIMPLE_SERVER)
static uint8_t app_get_handler(const struct app_subtask_handlers *handler_list_desc,
                               ke_msg_id_t msgid,
                               void *param,
                               ke_task_id_t src_id)
{
    // Counter
    uint8_t counter;

    // Get the message handler function by parsing the message table
    for (counter = handler_list_desc->msg_cnt; 0 < counter; counter--) {
        struct ke_msg_handler *handler
                = (struct ke_msg_handler*)(handler_list_desc->p_msg_handler_tab + counter - 1);

        if ((handler->id == msgid) ||
            (handler->id == KE_MSG_DEFAULT_HANDLER)) {
            // If handler is NULL, message should not have been received in this state
            ASSERT_ERR(handler->func);
            return (uint8_t)(handler->func(msgid, param, TASK_APP, src_id));
        }
    }

    // If we are here no handler has been found, drop the message
    return (KE_MSG_CONSUMED);
}
#endif

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
    if (p_param->actv_type == GAPM_ACTV_TYPE_ADV) {
        appm_adv_create_ind_handler((void*)p_param);
    }

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
    if (p_param->actv_type == GAPM_ACTV_TYPE_ADV) {
        appm_adv_stopped_ind_handler((void*)p_param);
    }

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

    if (state == APPM_CREATE_DB) {
        switch (param->prf_task_id) {
            default: /* Nothing to do */ break;
        }
    }
    else {
        ASSERT_INFO(0, state, src_id);
    }

    return KE_MSG_CONSUMED;
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

    switch(param->operation)
    {
        // Reset completed
        case (GAPM_RESET):
        {
            if(param->status == GAP_ERR_NO_ERROR)
            {
                #if (BLE_APP_HID)
                app_hid_start_mouse();
                #endif //(BLE_APP_HID)

                // Set Device configuration
                struct gapm_set_dev_config_cmd* cmd = KE_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,
                                                                   TASK_GAPM, TASK_APP,
                                                                   gapm_set_dev_config_cmd);
                // Set the operation
                cmd->operation = GAPM_SET_DEV_CONFIG;
                // Set the device role - Observer & Broadcaster
                cmd->role      = GAP_ROLE_ALL;

                #if (BLE_APP_SEC_CON)
                // The Max MTU is increased to support the Public Key exchange
                // HOWEVER, with secure connections enabled you cannot sniff the
                // LEAP and LEAS protocols
                cmd->max_mtu = 160;
                cmd->pairing_mode = GAPM_PAIRING_SEC_CON | GAPM_PAIRING_LEGACY;
                #else // !(BLE_APP_SEC_CON)
                // Do not support secure connections
                cmd->pairing_mode = GAPM_PAIRING_LEGACY;
                cmd->max_mtu = 247;
                #endif //(BLE_APP_SEC_CON)

                // Set Data length parameters
                cmd->sugg_max_tx_octets = BLE_MIN_OCTETS;
                cmd->sugg_max_tx_time   = BLE_MIN_TIME;

                #if (BLE_APP_HID)
                // Enable Slave Preferred Connection Parameters present
                cmd->att_cfg = GAPM_MASK_ATT_SLV_PREF_CON_PAR_EN;
                #endif //(BLE_APP_HID)

                #if APP_MESH_TMALL
                // Static Public Address for Tmall-genie, and GAPM will omit it if privacy_cfg is zero.
                memcpy(cmd->addr.addr, &mesh_tmall_tripple.mac_addr[0], GAP_BD_ADDR_LEN);
                cmd->privacy_cfg = GAPM_PRIV_CFG_PRIV_ADDR_BIT;

                #elif (NVDS_SUPPORT)

                nvds_tag_len_t length = GAP_BD_ADDR_LEN;
                if (nvds_get(PARAM_ID_BD_ADDRESS, &length, cmd->addr.addr) != PARAM_OK)
                {
                    memcpy(cmd->addr.addr, &co_default_bdaddr, sizeof(co_default_bdaddr));
                }
                cmd->privacy_cfg = GAPM_PRIV_CFG_PRIV_ADDR_BIT;
                #endif

                // Host privacy enabled by default
                #if (BLE_APP_PRIVACY)
                cmd->privacy_cfg |= GAPM_PRIV_CFG_PRIV_EN_BIT;
                cmd->renew_dur = 0x0384; // 15min
                #endif

                #if (BLE_APP_AM0)
                cmd->audio_cfg   = GAPM_MASK_AUDIO_AM0_SUP;
                cmd->att_cfg    |= GAPM_MASK_ATT_SVC_CHG_EN;
                #endif //(BLE_APP_AM0)

                // Enable Gatt Service change indication
                cmd->att_cfg    |= GAPM_MASK_ATT_SVC_CHG_EN;

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
            if (!appm_add_svc())
            {
                // Go to the ready state
                ke_state_set(TASK_APP, APPM_READY);

                // To enable mesh after the advertising above is started
                app_mesh_enable();
                // Load mesh paras begin
                app_mesh_storage_load();
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
            #if (BLE_APP_PRIVACY)
            appm_privacy_init(1);
            #endif
        }
        break;

        case (GAPM_SET_IRK):
        {
            // ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->operation, param->status);
            // If not Bonded already store the generated value in NVDS
            #if (BLE_APP_SEC)
            if (app_sec_get_bond_status()==false)
            {
                #if (NVDS_SUPPORT)
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
            }
        }
        break;

        // Device Configuration updated
        #if (BLE_APP_WHITE_LIST)
        case (GAPM_SET_DEV_CONFIG):
        case (GAPM_GET_WLIST_SIZE):
        case (GAPM_SET_WL):
        {
            if(appm_whl_cmp_evt_handler(msgid, param, dest_id, src_id) != KE_MSG_CONSUMED)
            {
                ke_state_t state = ke_state_get(dest_id);

                if (state < APPM_READY) {
                    appm_add_svc();
                }
            }
        }
        break;
        #else
        case (GAPM_SET_DEV_CONFIG):
        {
            ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->operation, param->status);
            appm_add_svc();
        }
        break;
        #endif

        #if (BLE_APP_PRIVACY)
        case (GAPM_GET_RAL_SIZE):
        case (GAPM_SET_RAL):
            appm_privacy_cmp_evt_handler(msgid, param, dest_id, src_id);
            break;
        #endif

        // Drive fsm to start advertising
        case (GAPM_CREATE_ADV_ACTIVITY):
        case (GAPM_STOP_ACTIVITY):
        case (GAPM_DELETE_ACTIVITY):
        case (GAPM_SET_ADV_DATA):
        case (GAPM_SET_SCAN_RSP_DATA):
        case (GAPM_DELETE_ALL_ACTIVITIES) :
        {
            appm_adv_cmp_evt_handler((void*)param);
        } break;

        case (GAPM_START_ACTIVITY):
        {
            break;
        }

        default:
        {
            // Drop the message
        }
        break;
    }
    return (KE_MSG_CONSUMED);
}

static int gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_get_dev_info_req_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    switch(param->req)
    {
        case GAPC_DEV_NAME:
        {
            struct gapc_get_dev_info_cfm * cfm =
                KE_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,
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
            cfm->info.slv_pref_params.con_intv_min = 8;
            // Slave preferred Connection interval Max
            cfm->info.slv_pref_params.con_intv_max = 10;
            // Slave preferred Connection latency
            cfm->info.slv_pref_params.slave_latency = 0;
            // Slave preferred Link supervision timeout
            cfm->info.slv_pref_params.conn_timeout = 200;  // 2s (500*10ms)

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
    app_env.conidx = KE_IDX_GET(src_id);
    #if(BLE_APP_SEC)
    bool is_bond = app_sec_get_bond_status_by_addr(param->peer_addr);
    #endif

    // Check if the received Connection Handle was valid
    if (app_env.conidx != GAP_INVALID_CONIDX)
    {
        // Retrieve the connection info from the parameters
        app_env.conhdl = param->conhdl;

        // Send connection confirmation
        struct gapc_connection_cfm *cfm = KE_MSG_ALLOC(GAPC_CONNECTION_CFM,
                KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                gapc_connection_cfm);

        #if(BLE_APP_SEC)
        cfm->auth      = is_bond ? GAP_AUTH_REQ_NO_MITM_BOND : GAP_AUTH_REQ_NO_MITM_NO_BOND; // TODO [FBE] restore valid data
        #else // !(BLE_APP_SEC)
        cfm->auth      = GAP_AUTH_REQ_NO_MITM_NO_BOND;
        #endif // (BLE_APP_SEC)
        // Send the message
        ke_msg_send(cfm);

        /*--------------------------------------------------------------
         * ENABLE REQUIRED PROFILES
         *--------------------------------------------------------------*/

        #if (BLE_APP_BATT)
        // Enable Battery Service
        app_batt_enable_prf(app_env.conidx);
        #endif //(BLE_APP_BATT)

        #if (BLE_APP_SIMPLE_SERVER)
        // Enable SIMPLE_SERVER Service
        app_simple_server_enable_prf(app_env.conidx);
        #endif //(BLE_APP_SIMPLE_SERVER)

        // We are now in connected State
        ke_state_set(dest_id, APPM_CONNECTED);

        #if (BLE_APP_SEC)
        if (is_bond)
        {
            // Ask for the peer device to either start encryption
            app_sec_send_security_req(app_env.conidx);
        }
        #endif // (BLE_APP_SEC)
    }
    else
    {
        // No connection has been established, restart advertising
        //appm_adv_update_state(true);
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
        case (GAPC_UPDATE_PARAMS):
        {
            if (param->status != GAP_ERR_NO_ERROR)
            {
//                appm_disconnect();
            }
        } break;

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
    //log_debug("Disconnectde.\n");
    #if (BLE_APP_SIMPLE_SERVER)
    app_simple_server_disable_prf(KE_IDX_GET(src_id));
    #endif //(BLE_APP_SIMPLE_SERVER)
    // Go to the ready state
    ke_state_set(TASK_APP, APPM_READY);

#if BLE_APP_ONMICRO_DFU
    dfu_reset(DFU_UPDATE_ST_DISCONNECT); //Cleanup DFU cache
    extern bool onmicro_dfu_reset_flag;
    if(onmicro_dfu_reset_flag)
    {
        log_debug("REBOOT !\n");
        pmu_force_reboot();
    }
#endif

    // Restart Advertising
    //appm_adv_update_state(true);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles connect param update(>4.1) event from the GAP.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_param_update_req_ind_handler(ke_msg_id_t const msgid,
                                      struct gapc_param_update_cmd const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Prepare the GAPC_PARAM_UPDATE_CMD message
    struct gapc_param_update_cfm *cfm = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM,
                                                     KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                                                     gapc_param_update_cfm);

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
        } break;

        #if (BLE_APP_DIS)
        case (TASK_ID_DISS):
        {
            // Call the Device Information Module
            msg_pol = app_get_handler(&app_dis_handlers, msgid, param, src_id);
        } break;
        #endif //(BLE_APP_DIS)

        #if (BLE_APP_SIMPLE_SERVER)
        case (TASK_ID_SIMPLE_SERVER):
        {
            // Call the Audio Mode 0 Module
            msg_pol = app_get_handler(&app_simple_server_handlers, msgid, param, src_id);
        } break;

        #endif //(BLE_APP_SIMPLE_SERVER)

        case (TASK_ID_MESH):
            switch (msgid)
            {
                case (MESH_API_PROV_STATE_IND):
                {
                    m_api_prov_state_ind_t *p_ind = (m_api_prov_state_ind_t *)param;
                    if (M_PROV_SUCCEED == p_ind->state)
                    {
                        __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "Successfully provisioned, Node Address: 0x%04X\n", m_tb_mio_get_prim_addr());

                        #if APP_MESH_TUYA
                        if(g_relay_feature_en) {
                            // enable relay state
                            m_tb_state_set_relay_state(true,false);
                        }
                        if(g_proxy_feature_en) {
                            // enable gatt proxy
                            m_tb_state_set_gatt_proxy_state(true);
                            app_mesh_proxy_service_enable();
                        }
                        #if CFG_BLE_MESH_LPN
                        if(g_lpn_feature_en) {
                            app_mesh_lpn_start();
                        }
                        #endif /* #if CFG_BLE_MESH_LPN */
                        #endif /* #if APP_MESH_TUYA */

                        #if APP_MESH_TMALL
                        #if CFG_BLE_MESH_RELAY
                        // enable relay state
                        m_tb_state_set_relay_state(true,false);
                        #endif
                        #if CFG_BLE_MESH_GATT_PROXY
                        // enable gatt proxy
                        m_tb_state_set_gatt_proxy_state(true);
                        app_mesh_proxy_service_enable();
                        #endif
                        // clear unprov timer
                        tmall_ind_prov_state_handler(false);
                        #endif
                    }
                    else
                    {
                        __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "provision state=0x%x(enum m_prov_state) status=0x%x(enum mesh_error)\n",
                              p_ind->state, p_ind->status);
                    }
                } break;

                case (MESH_API_ATTENTION_UPDATE_IND):
                {
                    m_api_attention_update_ind_t *p_ind = (m_api_attention_update_ind_t *)param;
                    __LOG(LOG_SRC_API, LOG_LEVEL_DBG2, "attention state=0x%x\n", p_ind->attention_state);
                } break;

                case (MESH_API_UPDATE_IND):
                {
                    m_api_update_ind_t *p_ind = (m_api_update_ind_t *)param;
                    //__LOG(LOG_SRC_API, LOG_LEVEL_DBG1, "config update type 0x%x(enum m_tb_store_upd_type)in %d-byte\n", p_ind->upd_type, p_ind->length);
                    if (M_TB_STORE_UPD_TYPE_APP_KEY_UPDATED == p_ind->upd_type)
                    {
                        #if APP_MESH_TMALL
                        /* Self bind for Vendor Tmall Model, regardless UUID version is 0 or 1. */
                        m_lid_t app_key_lid;
                        memcpy(&app_key_lid, &p_ind->data[0], co_min(sizeof(m_lid_t), p_ind->length));
                        appm_tmall_self_bind(app_key_lid);
                        #endif
                    }
                    /* send msg to nvds when indicate save*/
                    app_mesh_storage_save();
                } break;

                case (MESH_API_NODE_RESET_IND):
                {
                    __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "Successfully Unbind and Reset\n");
                    app_mesh_remove_bond();
                } break;

                case (MESH_API_PROXY_ADV_UPDATE_IND):
                case (MESH_API_FAULT_PERIOD_IND):
                {

                }break;

                #if CFG_BLE_MESH_LPN
                case MESH_API_LPN_OFFER_IND:
                {
                    m_api_lpn_offer_ind_t *p_ind = (m_api_lpn_offer_ind_t *)param;

                    __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "LPN Rec Offer,addr=0x%x,rssi=%d\n",p_ind->addr,p_ind->rssi);

                    app_mesh_lpn_select(p_ind->addr);
                }
                break;

                case MESH_API_LPN_STATUS_IND:
                {
                    m_api_lpn_status_ind_t *p_ind = (m_api_lpn_status_ind_t *)param;
                    switch (p_ind->status)
                    {
                        case MESH_ERR_NO_ERROR:
                            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "LPN SetUp FriendShip Success,Friend Address: 0x%x\n",p_ind->friend_addr);
                            break;
                        case MESH_ERR_LPN_ESTAB_FAILED:
                            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "LPN SetUp FriendShip Failed After Several Attempts.\n");
                            break;
                        case MESH_ERR_LPN_ESTAB_FAILED_KEY:
                            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "LPN SetUp FriendShip Failed of Friend Keys.\n");
                            break;
                        case MESH_ERR_LPN_FRIEND_LOST_LOCAL:
                            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "LPN FriendShip Stopped due to Local Request.\n");
                            break;
                        case MESH_ERR_LPN_FRIEND_LOST_TIMEOUT:
                            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "LPN FriendShip Lost due to Request Timeout.\n");
                            break;
                        default:
                            break;
                    }
                    if(MESH_ERR_NO_ERROR != p_ind->status) {
                        app_mesh_lpn_stop();
                    }
                }
                break;
                #endif /* #if CFG_BLE_MESH_LPN */

                #if CFG_BLE_MESH_FRIEND
                case MESH_DBG_FRIEND_LPN_IND:
                {
                #if BLE_MESH_DBG

                    m_dbg_friend_lpn_ind_t *p_ind = (m_dbg_friend_lpn_ind_t *)param;
                    switch(p_ind->event)
                    {
                        case M_TB_FRIEND_EVENT_DISC:
                            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "Friend New LPN Discovered,LPN address: 0x%x\n",p_ind->lpn_addr);
                            break;
                        case M_TB_FRIEND_EVENT_ESTAB:
                            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "Friend FriendShip Established,LPN address: 0x%x\n",p_ind->lpn_addr);
                            break;
                        case M_TB_FRIEND_EVENT_LOST:
                            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "Friend FriendShip Lost,LPN address: 0x%x\n",p_ind->lpn_addr);
                            break;
                        default:
                            break;
                    }

                #endif /* BLE_MESH_DBG */
                }
                break;
                #endif /* #if CFG_BLE_MESH_FRIEND */

                case MESH_MDL_API_IND:
                {
                    mm_api_ind_t *p_ind = (mm_api_ind_t *)param;
                    __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "Mesh Model Ind: Elmt_lid %x,Opc 0x%x,Dat: %x,%x,%x\n",
                    p_ind->elmt_lid,p_ind->ind_code,p_ind->ind_dat[0],p_ind->ind_dat[1],p_ind->ind_dat[2]);
                    break;
                }

                default:
                    log_debug("[mesh] Not handle mesh msgid=0x%08X\n", msgid);
                    break;
            }
            break;

        default:
            log_debug("[mesh] Not cover comm msgid=0x%08X\n", msgid);
            break;
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
 * @brief Handles list size ind
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_list_size_ind_handler(ke_msg_id_t const msgid, struct gapm_list_size_ind *param,
                                        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    #if (BLE_APP_WHITE_LIST)
    //log_debug("white list size=%d\n", param->size);
    if (param->operation == GAPM_GET_WLIST_SIZE) {
        appm_whl_set_size(param->size);
    }
    #endif
    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles GAPM_DEV_BDADDR_IND
 *
 * @param[in] msgid 	Id of the message received.
 * @param[in] param 	Pointer to the parameters of the message.
 * @param[in] dest_id	ID of the receiving task instance
 * @param[in] src_id	ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_dev_bdaddr_ind_handler(ke_msg_id_t const msgid, struct gapm_dev_bdaddr_ind *param,
                                        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    log_debug_array_ex("addr ", param->addr.addr.addr, 6);
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
    return KE_MSG_CONSUMED;
}

/**
 * @brief gapc_pkt_size_ind_handler()
 *
 * @param[in] msgid
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return
 **/
static int gapc_pkt_size_ind_handler(ke_msg_id_t const msgid,  struct gapc_le_pkt_size_ind const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

/**
 * @brief gapc_param_update_ind_handler()
 *
 * @param[in] msgid
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return
 **/
static int gapc_param_update_ind_handler(ke_msg_id_t const msgid,
                                      struct gapc_param_updated_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

/**
 * @brief mesh_api_cmp_evt_handler()
 *
 * @param[in] msgid
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return
 **/
static int mesh_api_cmp_evt_handler(ke_msg_id_t const msgid,
                                    struct mesh_api_cmp_evt const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    #if APP_MESH_TUYA
    uint8_t uuid[MESH_DEV_UUID_LEN];
    #endif

    ASSERT_ERR(param->status == MESH_ERR_NO_ERROR);

    switch (param->cmd_code)
    {
        case M_API_ENABLE:
            break;

        case (M_API_STORAGE_LOAD):

            #if APP_MESH_TUYA
            // Print UUID
            app_mesh_node_id_get_uuid(uuid);
            __LOG_XB(LOG_SRC_API, LOG_LEVEL_INFO, "Device UUID", uuid, MESH_DEV_UUID_LEN);
            #if CFG_BLE_MESH_FRIEND
            if(g_friend_feature_en) {
                m_tb_state_set_friend_state(true);
            }
            #endif /* #if CFG_BLE_MESH_FRIEND */
            #endif /* #if APP_MESH_TUYA */

            // Load Finish
            if (0 == param->status)
            {
                uint16_t node_address = m_tb_mio_get_prim_addr();
                if ((0x0000 != node_address) && (0xFFFF != node_address))
                {
                    __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "Already provisioned, Node Address: 0x%04X\n",
                          node_address);

                    #if APP_MESH_TUYA

                    // Inform APP mesh stack reset,proved
                    tuya_mesh_stack_reset_handler(true);

                    if(g_relay_feature_en) {
                        uint8_t retx_state  = 0;
                        uint8_t relay_state = 0;

                        relay_state = m_tb_state_get_relay_state(&retx_state);
                        if((relay_state == true) && (retx_state == false)) {
                        }
                        else {
                            // enable relay state
                            m_tb_state_set_relay_state(true,false);
                        }
                    }
                    if(g_proxy_feature_en) {
                        if(m_tb_state_get_gatt_proxy_state() == false) {
                            // enable gatt proxy
                            m_tb_state_set_gatt_proxy_state(true);
                        }
                        app_mesh_proxy_service_enable();
                    }

                    #if CFG_BLE_MESH_LPN
                    if(g_lpn_feature_en) {
                        app_mesh_lpn_start();
                    }
                    #endif /* #if CFG_BLE_MESH_LPN */

                    #endif /* #if APP_MESH_TUYA */

                    #if APP_MESH_TMALL
                    #if CFG_BLE_MESH_RELAY
                    m_tb_state_set_relay_state(true,false);
                    #endif
                    #if CFG_BLE_MESH_GATT_PROXY
                    m_tb_state_set_gatt_proxy_state(true);
                    app_mesh_proxy_service_enable();
                    #endif
                    /* indicate provisioned */
                    tmall_ind_prov_state_handler(false);
                    /* Loaded tag finish */
                    tmall_nvds_loaded_handler();
                    #endif
                }
                else{
                    #if APP_MESH_TUYA
                    // Inform APP mesh stack reset,unproved
                    tuya_mesh_stack_reset_handler(false);
                    #endif

                    #if APP_MESH_TMALL
                    tmall_ind_prov_state_handler(true);
                    #endif
                }
            }
            else {
                 __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "Load Params Error:%x\n",param->status);
            }
            break;

        case (M_API_STORAGE_SAVE):
            break;

        case (M_API_PROXY_CTL):
            break;
#if CFG_BLE_MESH_LPN
        case (M_API_LPN_START):
            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "LPN Started.\n");
            break;

        case (M_API_LPN_STOP):
            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "LPN Stoped.\n");
            break;

        case (M_API_LPN_SELECT_FRIEND):
            __LOG(LOG_SRC_APP, LOG_LEVEL_DBG2, "LPN Selected Friend.\n");
            break;
#endif /* #if CFG_BLE_MESH_LPN */
        default:
            __LOG(LOG_SRC_API, LOG_LEVEL_DBG1, "command=0x%x is completed with status=0x%x\n",
                  param->cmd_code, param->status);
            break;
    }

    return (KE_MSG_CONSUMED);
}

/**
 * @brief mesh_api_prov_param_req_ind_handler()
 *
 * @param[in] msgid
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return
 **/
static int mesh_api_prov_param_req_ind_handler(ke_msg_id_t const msgid,
                                      void const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    uint8_t uuid[MESH_DEV_UUID_LEN];

    // Prepare the GAPC_PARAM_UPDATE_CMD message
    struct m_api_prov_param_cfm *cfm = KE_MSG_ALLOC(MESH_API_PROV_PARAM_CFM,
                                                    src_id, TASK_APP,
                                                    m_api_prov_param_cfm);

    /// Device UUID
    app_mesh_node_id_get_uuid(uuid);
    memcpy(cfm->dev_uuid, uuid, MESH_DEV_UUID_LEN);
    /// URI hash
    cfm->uri_hash = 0;
    /// OOB information
    cfm->oob_info = 0;
    /// Public key OOB information available
    cfm->pub_key_oob = 0;
#if APP_MESH_TMALL
    /// Static OOB information available
    cfm->static_oob = 1;
#else
    cfm->static_oob = 0;
#endif
    /// Maximum size of Output OOB supported
    cfm->out_oob_size = 0;
    /// Supported Output OOB Actions (@see enum m_prov_out_oob)
    cfm->out_oob_action = 0;
    /// Maximum size in octets of Input OOB supported
    cfm->in_oob_size = 0;
    /// Supported Input OOB Actions (@see enum m_prov_in_oob)
    cfm->in_oob_action = 0;
    /// Number of elements
    cfm->nb_elt = 1;
    /// Bit field providing additional information (@see enum m_prov_info)
    cfm->info = 0;

    // Send the message
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/**
 * @brief The handler of message Mesh Provisioning Authentication Data Request.
 *
 * @param[in] msgid
 * @param[in] param
 * @param[in] dest_id
 * @param[in] src_id
 *
 * @return
 **/
static int mesh_api_prov_auth_data_req_ind_handler(ke_msg_id_t const msgid,
                                                   void const *param,
                                                   ke_task_id_t const dest_id,
                                                   ke_task_id_t const src_id)
{
    uint8_t auth_size = 0;
    struct m_api_prov_auth_data_cfm *cfm;
    m_api_prov_auth_data_req_ind_t *p_ind = (m_api_prov_auth_data_req_ind_t *)param;

    switch (p_ind->auth_method) {
    case M_PROV_AUTH_NO_OOB:
        auth_size = 0;
        break;

    case M_PROV_AUTH_STATIC_OOB:
        auth_size = M_OOB_AUTH_DATA_LEN;
        break;

    case M_PROV_AUTH_OUTPUT_OOB:
        break;

    case M_PROV_AUTH_INPUT_OOB:
        break;

    default:
        /* TBD: calculate the factual auth_size for Output/Input OOB auth. */
        auth_size = co_min(p_ind->auth_size, M_OOB_AUTH_DATA_LEN);
        break;
    }

    // Prepare the MESH_API_PROV_AUTH_DATA_CFM message
    cfm = KE_MSG_ALLOC_DYN(MESH_API_PROV_AUTH_DATA_CFM,
                           src_id, TASK_APP,
                           m_api_prov_auth_data_cfm, auth_size);

    if(auth_size) {
        /// Authentication data
        memcpy(cfm->auth_data, app_mesh_prov_auth_data_get(), auth_size);
    }

    cfm->accept = 1;
    cfm->auth_size = auth_size;

    // Send the message
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */

/**< Default State handlers definition. */
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
    {GAPM_LIST_SIZE_IND,        (ke_msg_func_t)gapm_list_size_ind_handler},
    {GAPM_DEV_BDADDR_IND,       (ke_msg_func_t)gapm_dev_bdaddr_ind_handler},

    // GAPC messages
    {GAPC_GET_DEV_INFO_REQ_IND, (ke_msg_func_t)gapc_get_dev_info_req_ind_handler},
    {GAPC_SET_DEV_INFO_REQ_IND, (ke_msg_func_t)gapc_set_dev_info_req_ind_handler},
    {GAPC_CONNECTION_REQ_IND,   (ke_msg_func_t)gapc_connection_req_ind_handler},
    {GAPC_CMP_EVT,              (ke_msg_func_t)gapc_cmp_evt_handler},
    {GAPC_DISCONNECT_IND,       (ke_msg_func_t)gapc_disconnect_ind_handler},
    {GAPC_PARAM_UPDATE_REQ_IND, (ke_msg_func_t)gapc_param_update_req_ind_handler},

    {GAPC_LE_PKT_SIZE_IND,      (ke_msg_func_t)gapc_pkt_size_ind_handler},
    {GAPC_PARAM_UPDATED_IND,    (ke_msg_func_t)gapc_param_update_ind_handler},
    {GATTC_MTU_CHANGED_IND,     (ke_msg_func_t)gattc_mtu_changed_ind_handler},

    // Mesh messages
    {MESH_API_CMP_EVT,            (ke_msg_func_t)mesh_api_cmp_evt_handler},
    {MESH_API_PROV_PARAM_REQ_IND, (ke_msg_func_t)mesh_api_prov_param_req_ind_handler},
    {MESH_API_PROV_AUTH_DATA_REQ_IND, (ke_msg_func_t)mesh_api_prov_auth_data_req_ind_handler},
};

/**< Defines the place holder for the states of all the task instances. */
ke_state_t appm_state[APP_IDX_MAX];

/// Application task descriptor
const struct ke_task_desc TASK_DESC_APP = {appm_msg_handler_tab, appm_state, APP_IDX_MAX, ARRAY_LEN(appm_msg_handler_tab)};

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
