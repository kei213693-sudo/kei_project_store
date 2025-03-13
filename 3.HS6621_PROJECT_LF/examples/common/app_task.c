/**
 ****************************************************************************************
 *
 * @file examples/common/app_task.c
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
 * @addtogroup APP_SIMPLE_SERVER_TASK_C app_task.c
 * @ingroup APP_SIMPLE_SERVER
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"          // SW configuration

#include "app_task.h"             // Application Manager Task API
#include "app.h"                  // Application Manager Definition
#include "gapc_task.h"            // GAP Controller Task API
#include "gapm_task.h"            // GAP Manager Task API
#include "arch.h"                 // Platform Definitions
#include <string.h>
#include "co_utils.h"
#include "ke_timer.h"             // Kernel timer
#include "co_debug.h"

#if (BLE_APP_SEC)
#include "app_sec.h"              // Security Module Definition
#endif //(BLE_APP_SEC)

#if (BLE_APP_DIS)
#include "app_dis.h"              // Device Information Module Definition
#include "diss_task.h"
#endif //(BLE_APP_DIS)

#if (BLE_APP_WECHAT)
#include "app_wechat.h"             // Wechat Module Definition
#include "wechat_task.h"
#endif //(BLE_APP_WECHAT)

#if (BLE_APP_BATT)
#include "app_batt.h"             // Battery Module Definition
#include "bass_task.h"
#endif //(BLE_APP_BATT)

#if (BLE_APP_HID)
#include "app_hid.h"              // HID Module Definition
#include "hogpd_task.h"
#endif //(BLE_APP_HID)

#if (BLE_TSPP_SERVER)
#include "app_tspp_server.h"              // TSPP Module Definition
#endif //(BLE_TSPP_SERVER)
#if (BLE_ATVV_SERVER)
#include "app_atvv.h"              // TSPP Module Definition
#endif //(BLE_ATVV_SERVER)

#include "app_ble.h"

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
                = (struct ke_msg_handler*)(handler_list_desc->p_msg_handler_tab + counter - 1);

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

#if(BLE_APP_SEC)
/**
 ****************************************************************************************
 * @brief set local irk
 *
 * @return void
 ****************************************************************************************
 */
void app_gap_set_loc_irk(void) 
{
    nvds_tag_len_t key_len = KEY_LEN;
    if (app_sec_get_bond_status()==true)
    {
        #if (NVDS_SUPPORT)
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
             //ASSERT_ERR(0);
        }
    }
    else // Need to start the generation of new IRK
    {
        struct gapm_gen_rand_nb_cmd *cmd = KE_MSG_ALLOC(GAPM_GEN_RAND_NB_CMD,
                                                        TASK_GAPM, TASK_APP,
                                                        gapm_gen_rand_nb_cmd);
        cmd->operation   = GAPM_GEN_RAND_NB;
        app_env.rand_cnt = 1;
        ke_msg_send(cmd);
    }
}
#endif

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
    nvds_tag_len_t key_len = KEY_LEN;//16
    #endif //(NVDS_SUPPORT)
    switch(param->operation)
    {
        // Reset completed
        case (GAPM_RESET):
        {
            if(param->status == GAP_ERR_NO_ERROR)
            {
                #if (BLE_APP_HID)
                //app_hid_start_mouse();
                #endif //(BLE_APP_HID)
                // Set Device configuration
                struct gapm_set_dev_config_cmd* cmd = KE_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,
                                                                TASK_GAPM, TASK_APP,
                                                                gapm_set_dev_config_cmd);
                // Set the operation
                cmd->operation = GAPM_SET_DEV_CONFIG;
                // Set the device role - Peripheral
                cmd->role      = GAP_ROLE_PERIPHERAL;
                // Do not support secure connections
                cmd->pairing_mode = GAPM_PAIRING_LEGACY;
                // Set Data length parameters
                cmd->sugg_max_tx_octets = BLE_MAX_OCTETS;
                cmd->sugg_max_tx_time   = BLE_MAX_TIME;
                //cmd->max_mtu            = 200;
                /// Preferred LE PHY for data transmission (@see enum gap_phy)
                cmd->tx_pref_phy = GAP_PHY_LE_1MBPS;
                /// Preferred LE PHY for data reception (@see enum gap_phy)
                cmd->rx_pref_phy = GAP_PHY_LE_1MBPS;
                // Maximal MTU/MPS acceptable for device
                cmd->max_mtu = 251;
                cmd->max_mps = GAP_MAX_LE_MTU;

                #if (BLE_APP_HID)
                // Enable Slave Preferred Connection Parameters present
                cmd->att_cfg = GAPM_MASK_ATT_SLV_PREF_CON_PAR_EN;
                #endif //(BLE_APP_HID)
                
                #if ((NVDS_SUPPORT) && (BLE_APP_SEC))
                if ((app_sec_get_bond_status()==true) &&//已经绑定过最少一个设备
                    (nvds_get(NVDS_TAG_LOC_IRK, &key_len, app_env.loc_irk) == NVDS_OK))
                {
                    memcpy(cmd->irk.key, app_env.loc_irk, 16);
                }
                else
                #endif //(NVDS_SUPPORT)
                {
                    memset((void *)&cmd->irk.key[0], 0x00, KEY_LEN);
                }
                #if (BLE_APP_PRIVACY)
                cmd->privacy_cfg |= GAPM_PRIV_CFG_PRIV_EN_BIT;//配置为随机地址
                cmd->renew_dur = 0x0384;//更改随机地址的时间
                #else
                cmd->privacy_cfg = GAPM_PRIV_CFG_PRIV_ADDR_BIT;;//静态随机地址参数
                memcpy(cmd->addr.addr, &app_env.addr.addr[0], 6);//配置静态随机地址参数
                #endif
                // Send message
                ke_msg_send(cmd);
            }
            else
            {
                ASSERT_ERR(0);
            }
        }
        break;

        // Device Configuration updated
        #if (BLE_APP_WHITE_LIST || BLE_APP_PRIVACY)
        case (GAPM_SET_DEV_CONFIG):
        {
            #if (BLE_APP_WHITE_LIST)
            appm_whl_cmp_evt_handler(msgid, param, dest_id, src_id);
            #endif
            
            #if (BLE_APP_PRIVACY)
            appm_privacy_cmp_evt_handler(msgid, param, dest_id, src_id);
            #endif
            
            #if(BLE_APP_SEC)
            app_gap_set_loc_irk();
            #endif
        }
        break;
        #else
        case (GAPM_SET_DEV_CONFIG):
        {
            ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->operation, param->status);

            #if(BLE_APP_SEC)
            app_gap_set_loc_irk();
            #endif
            
            appm_reg_svc_itf();
            // Add the first required service in the database
            // and wait for the PROFILE_ADDED_IND
            appm_add_svc();
        }
        break;
        #endif

        #if (BLE_APP_WHITE_LIST)
        case (GAPM_GET_WLIST_SIZE):
        case (GAPM_SET_WL):
        {
            if(appm_whl_cmp_evt_handler(msgid, param, dest_id, src_id) != KE_MSG_CONSUMED)
            {
                ke_state_t state = ke_state_get(dest_id);

                if (state < APPM_READY) {
                    appm_reg_svc_itf();
                    appm_add_svc();
                }
            }
        }
        break;
        #endif
        case (GAPM_PROFILE_TASK_ADD):
        {
            //log_debug("GAPM_PROFILE_TASK_ADD, st:%d\n", param->status);
            // Add the next requested service
            if (!appm_add_svc())
            {
                // Go to the ready state
                ke_state_set(TASK_APP, APPM_READY);
                // No more service to add, start advertising
                //appm_adv_update_state(true);
            #if (APP_CUSTOMER_ID == 1)
                extern void appm_pair_advdata_set(void);
                appm_pair_advdata_set();
            #endif
                if(0 == app_ble_no_pair_check())
                {//如果没有配对过，且不再配对模式了，不广播，接收到启动绑定指令后再广播
                    appm_adv_start();//上电初始化时调用
                }
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
                #if (NVDS_SUPPORT)
                if (nvds_put(NVDS_TAG_LOC_IRK, KEY_LEN, (uint8_t *)&app_env.loc_irk) != NVDS_OK)
                #endif //(NVDS_SUPPORT)
                {
                    ASSERT_INFO(0, 0, 0);
                }
            }
            #endif

            app_env.rand_cnt = 0;
        }
        break;

        #if (BLE_APP_PRIVACY)
        case (GAPM_GET_RAL_SIZE):
        case (GAPM_SET_RAL):
            appm_privacy_cmp_evt_handler(msgid, param, dest_id, src_id);
            break;
        #endif

        case (GAPM_CREATE_ADV_ACTIVITY):
        case (GAPM_STOP_ACTIVITY):
        case (GAPM_START_ACTIVITY):
        case (GAPM_DELETE_ACTIVITY):
        case (GAPM_SET_ADV_DATA):
        case (GAPM_SET_SCAN_RSP_DATA):
        case (GAPM_DELETE_ALL_ACTIVITIES) :
        {
            appm_adv_cmp_evt_handler((void*)param);
        } break;

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
            cfm->info.slv_pref_params.con_intv_min = 8;
            // Slave preferred Connection interval Max
            cfm->info.slv_pref_params.con_intv_max = 10;
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
    app_env.conidx = KE_IDX_GET(src_id);
    #if(BLE_APP_SEC)
    bool is_bond = app_sec_get_bond_status_by_addr(param->peer_addr);//判断是否是已经绑定过的设备
    #endif

	// log_debug("Device type(%d) conidx(%d) connected, ", param->peer_addr_type, app_env.conidx);
	// log_debug_array_ex("ADDR", &param->peer_addr, 6);

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
		//权限设定，走Legacy Pairing模式，启动MIMT保护，允许绑定
        //；不支持Secure Connections Pairing模式
        cfm->auth      = is_bond ? GAP_AUTH_REQ_MITM_BOND : GAP_AUTH_REQ_MITM_BOND;
        //cfm->auth      = GAP_AUTH_REQ_MITM_BOND;
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

        #if (BLE_APP_HID)
        // Enable HID Service
        app_hid_enable_prf(app_env.conidx);
        #endif //(BLE_APP_HID)

        #if (BLE_APP_SIMPLE_SERVER)
        // Enable SIMPLE_SERVER Service
        app_simple_server_enable_prf(app_env.conidx);
        #endif //(BLE_APP_SIMPLE_SERVER)

        #if (BLE_APP_WECHAT)
        //TODO: bd_addr_t mac_addr should be defined
        app_wechat_enable_prf(app_env.conidx, &app_env.addr);
        #endif //(BLE_APP_WECHAT)

        #if (BLE_TSPP_SERVER)
        // Enable TSPP_SERVER Service
        app_tspp_server_enable_prf(app_env.conidx);
        #endif //(BLE_APP_TSPP_SERVER)
        
        #if (BLE_ATVV_SERVER)
        // Enable TSPP_SERVER Service
        app_atvv_server_enable_prf(app_env.conidx);
        #endif //(BLE_APP_TSPP_SERVER)

        #if (BLE_APP_ANCSC)
        app_ancsc_enable_prf(app_env.conidx);
        #endif //(BLE_APP_ANCSC)
        
        // We are now in connected State
        ke_state_set(dest_id, APPM_CONNECTED);
        ke_state_set(APP_BLE_TASK_ID, APPM_CONNECTED);

        #if (BLE_APP_SEC)
        if(is_bond)
        {
            log_debug("is bonded\n");
        }
        else
        {
            app_sec_send_security_req(app_env.conidx);//安全连接请求
            //app_sec_bond_req(app_env.conidx);//绑定请求，对ios手机没用
            log_debug("send sec req\n");
            //启动定时器判定绑定是否成功，不成功就断开连接
            // co_timer_set(&bond_check_timer, 5000, TIMER_ONE_SHOT, bond_check_timer_handler, &app_env.conidx);
        }
        #endif // (BLE_APP_SEC)
        //调用app_gap_connected_handler打印对端信息
        APP_HANDLER(gap_connected, msgid, param, dest_id, src_id); // APP layer Callback
    }
    else
    {
    #if (APP_CUSTOMER_ID == 1)
        extern void appm_pair_advdata_set(void);
        appm_pair_advdata_set();
    #endif
        if(0 == app_ble_no_pair_check())
        {//如果没有配对过，不广播，接收到启动绑定指令后再广播
            // No connection has been established, restart advertising
            appm_adv_update_state(true);
        }
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
#if (BLE_APP_SEC)
    app_sec_gapc_disconnect_ind_handler(msgid, (void*)param, dest_id, src_id);
#endif
    //log_debug("Disconnected.\n");
    #if (BLE_APP_SIMPLE_SERVER)
    app_simple_server_disable_prf(KE_IDX_GET(src_id));
    #endif //(BLE_APP_SIMPLE_SERVER)
    #if (BLE_TSPP_SERVER)
    app_tspp_server_disable_prf(KE_IDX_GET(src_id));
    #endif

    #if (BLE_ATVV_SERVER)
    app_atvv_server_disable_prf(KE_IDX_GET(src_id));
    #endif
    
    // Go to the ready state
    ke_state_set(TASK_APP, APPM_READY);
    ke_state_set(APP_BLE_TASK_ID, APPM_READY);

    APP_HANDLER(gap_disconnected, msgid, param, dest_id, src_id); // APP layer Callback

    // Restart Advertising
    //appm_adv_update_state(true);
#if (APP_CUSTOMER_ID == 1)
    extern void appm_pair_advdata_set(void);
    appm_pair_advdata_set();
#endif
    if(0 == app_ble_no_pair_check())
    {//如果没有配对过，不广播，接收到启动绑定指令后再广播
        appm_adv_start();//断开连接后，重新启动广播
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
	 // Prepare the GAPC_PARAM_UPDATE_CMD message
	 struct gapc_param_update_cfm *cfm = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM,
                                                    KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                                                    gapc_param_update_cfm);

    /// True to accept slave connection parameters, False else.
    cfm->accept = true;//不论什么参数都接受
    /// Minimum Connection Event Duration
    cfm->ce_len_min = 0x0;
    /// Maximum Connection Event Duration
    cfm->ce_len_max = 0xFFFF;

    // Send the message
    ke_msg_send(cfm);
    
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
static int gapc_param_update_ind_handler(ke_msg_id_t const msgid,
                                    struct gapc_param_updated_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    APP_HANDLER(gap_param_update_ind, msgid, param, dest_id, src_id);
    return (KE_MSG_CONSUMED);
}

 /**
  * @brief Handles i2s rx event from the APP.
  *
  * @param[in] msgid	 Id of the message received.
  * @param[in] param	 Pointer to the parameters of the message.
  * @param[in] dest_id	 ID of the receiving task instance (TASK_GAP).
  * @param[in] src_id	 ID of the sending task instance.
  *
  * @return If the message was consumed or not.
  ****************************************************************************************
  */                              
static int app_i2s_rx_data_handler(ke_msg_id_t const msgid,
                                     void const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    APP_HANDLER(audio_rx_cmd, msgid, param, dest_id, src_id); // APP layer Callback

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

        #if (BLE_TSPP_SERVER)
        case (TASK_ID_TSPPS):
            {
                // Call the Audio Mode 0 Module
                msg_pol = app_get_handler(&app_tspp_server_handlers, msgid, param, src_id);
            } break;
        #endif //(BLE_TSPP_SERVER)

        #if (BLE_ATVV_SERVER)
        case (TASK_ID_ATVVS):
            {
                // Call the Audio Mode 0 Module
                msg_pol = app_get_handler(&app_atvv_server_handlers, msgid, param, src_id);
            } break;
        #endif //(BLE_TSPP_SERVER)

        #if (BLE_APP_WECHAT)
        case (TASK_ID_WECHAT):
        {
            // Call the Wechat Module
            msg_pol = app_get_handler(&app_wechat_handlers, msgid, param, src_id);
        } break;
        #endif //(BLE_APP_WECHAT)

        #if (BLE_APP_ANCSC)
        case (TASK_ID_ANCSC):
        {
            // Call the Audio Mode 0 Module
            msg_pol = app_get_handler(&app_ancsc_handlers, msgid, param, src_id);
        } break;

        #endif //(BLE_APP_ANCSC)

        #if (BLE_APP_HID)
        case (TASK_ID_HOGPD):
        {
            // Call the HID Module
            msg_pol = app_get_handler(&app_hid_handlers, msgid, param, src_id);
        } break;
        #endif //(BLE_APP_HID)

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
    #if (BLE_TSPP_SERVER)
    tspp_server_set_max_mtu(conidx, param->mtu);
    #endif
    #if (BLE_ATVV_SERVER)
    atvv_server_set_max_mtu(conidx, param->mtu);
    #endif

    APP_HANDLER(gatt_mtu_changed, msgid, param, dest_id, src_id); // APP layer Callback

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
static int gapm_gem_max_data_len_ind_handler(ke_msg_id_t const msgid, struct gapm_max_data_len_ind *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
    log_debug("gapm:max tx=%d, time=0x%x, rx=%d, time=0x%x\n", param->suppted_max_tx_octets, param->suppted_max_tx_time,
            param->suppted_max_rx_octets, param->suppted_max_rx_time);
    #if (BLE_TSPP_SERVER)
    tspp_server_set_data_len(KE_IDX_GET(src_id), param->suppted_max_tx_octets);
    #endif
    #if (BLE_ATVV_SERVER)
    atvv_server_set_data_len(KE_IDX_GET(src_id), param->suppted_max_tx_octets);
    #endif
    return KE_MSG_CONSUMED;
}

static int gapc_pkt_size_ind_handler(ke_msg_id_t const msgid,  struct gapc_le_pkt_size_ind const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);

    log_debug("gapc:max tx=%d, time=0x%x, rx=%d, time=0x%x\n", param->max_tx_octets, param->max_tx_time,
            param->max_rx_octets, param->max_rx_time);
    #if (BLE_TSPP_SERVER)
    tspp_server_set_data_len(conidx, param->max_tx_octets);
    #endif
    #if (BLE_ATVV_SERVER)
    atvv_server_set_data_len(conidx, param->max_tx_octets);
    #endif
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
    log_debug("white list get size=%d\n", param->size);
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

static int gapc_get_rssi_ind_handler(ke_msg_id_t const msgid,struct gapc_con_rssi_ind const *param,
                                    ke_task_id_t const dest_id,ke_task_id_t const src_id)
{
    if(NULL != param)
    {
        app_ble_rssi_opt(APP_BLE_RSSI_UPDATA_DATA,(void *)&param->rssi);
    }

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
    {GAPM_MAX_DATA_LEN_IND,     (ke_msg_func_t)gapm_gem_max_data_len_ind_handler},
    {GAPM_LIST_SIZE_IND,        (ke_msg_func_t)gapm_list_size_ind_handler},
    {GAPM_DEV_BDADDR_IND,       (ke_msg_func_t)gapm_dev_bdaddr_ind_handler},

    // GAPC messages
    {GAPC_GET_DEV_INFO_REQ_IND, (ke_msg_func_t)gapc_get_dev_info_req_ind_handler},
    {GAPC_SET_DEV_INFO_REQ_IND, (ke_msg_func_t)gapc_set_dev_info_req_ind_handler},
    {GAPC_CONNECTION_REQ_IND,   (ke_msg_func_t)gapc_connection_req_ind_handler},//建立连接
    {GAPC_CMP_EVT,              (ke_msg_func_t)gapc_cmp_evt_handler},
    {GAPC_DISCONNECT_IND,       (ke_msg_func_t)gapc_disconnect_ind_handler},//断开连接
    {GAPC_PARAM_UPDATE_REQ_IND, (ke_msg_func_t)gapc_param_update_req_ind_handler},//主/从，收到“连接参数更新指令”回调函数，需要返回确定
    {GAPC_PARAM_UPDATED_IND,    (ke_msg_func_t)gapc_param_update_ind_handler},//从机收到连接参数更新最终确认指令后进入处理
    {GAPC_LE_PKT_SIZE_IND,      (ke_msg_func_t)gapc_pkt_size_ind_handler},
    {GATTC_MTU_CHANGED_IND,     (ke_msg_func_t)gattc_mtu_changed_ind_handler},//mtu交换完成
    {APP_AUDIO_RX_CMD,          (ke_msg_func_t)app_i2s_rx_data_handler},
    {GAPC_CON_RSSI_IND,         (ke_msg_func_t)gapc_get_rssi_ind_handler}//获取RSSI值回调
};

/**< Defines the place holder for the states of all the task instances. */
ke_state_t appm_state[APP_IDX_MAX];//状态列表

/// Application task descriptor
const struct ke_task_desc TASK_DESC_APP = {appm_msg_handler_tab, appm_state, APP_IDX_MAX, ARRAY_LEN(appm_msg_handler_tab)};

/// @} APPTASK
