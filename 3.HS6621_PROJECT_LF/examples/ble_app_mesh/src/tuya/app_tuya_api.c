/**
 ****************************************************************************************
 *
 * @file app_tuya_api.c
 *
 * @brief BLE mesh app TUYA API.
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
#include "rwip_config.h" // SW configuration

#if (BLE_APP_PRESENT)

#include "m_bcn.h"
#include "peripheral.h"
#include "app.h"
#include "app_mesh.h"
#include "app_task.h"
#include "app_tuya_api.h"

#if APP_MESH_TUYA
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLES DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 * @brief Init mesh node capablity
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_node_params_init(tuya_mesh_node_params_t *node_info)
{
    // relay switch
    m_relay_feature_en(node_info->relay_en);

    // proxy switch
    m_proxy_feature_en(node_info->proxy_en);

    // friend switch
    m_friend_feature_en(node_info->friend_en);

    // low power node switch
    m_lpn_feature_en(node_info->lpn_en);

    // uuid/cid/pid/vid
    app_mesh_node_id_init(node_info->nid);

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief Init mesh node network
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_net_params_init(tuya_mesh_net_params_t *net_info)
{
    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    // ttl
    if(net_info->send_ttl != m_tb_state_get_default_ttl()) {

        if(MESH_ERR_NO_ERROR != m_tb_state_set_default_ttl(net_info->send_ttl)) {
            return TUYA_MESH_API_TTL_SET_FAIL;
        }
    }

    // net_transmit_cnt & net_transmit_step
    if((net_info->net_transmit_cnt > M_TB_NET_ADV_NUM_MAX) || (net_info->net_transmit_cnt < M_TB_NET_ADV_NUM_MIN)) {
        return TUYA_MESH_API_INVALID_PARAMS;
    }

    if((net_info->net_transmit_step > M_TB_NET_ADV_INTV_MAX) || (net_info->net_transmit_step < M_TB_NET_ADV_INTV_MIN)) {
        return TUYA_MESH_API_INVALID_PARAMS;
    }

    if(M_TB_STATE_NET_TX(net_info->net_transmit_cnt,net_info->net_transmit_step) == m_tb_state_get_net_tx_state()) {
        return TUYA_MESH_API_NO_ERROR;
    }

    m_tb_state_set_net_tx_state(M_TB_STATE_NET_TX(net_info->net_transmit_cnt,net_info->net_transmit_step));

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief Used to know if the device have been provisioned successfully
 * @return prov state true:proved,false:unproved
 */
uint8_t tuya_get_if_prov_success(void)
{
    if(M_TB_STATE_PROV_STATE_PROV == m_tb_state_get_prov_state()) {
        return true;
    }
    return false;
}

/**
 * @brief Used to set the primary element addr of the node
 * @param[in] addr unicast addr
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_set_primary_ele_addr(uint16_t addr)
{
    uint8_t model_lid = 0;
    uint8_t model_bound_nb = 0;
    uint8_t appkey_lid = M_TB_KEY_MAX_NB_NET + M_TB_KEY_MAX_NB_DEV;

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    if(!M_IS_UNICAST_ADDR(addr)) {
        return TUYA_MESH_API_INVALID_PARAMS;
    }

    if(addr == tuya_get_primary_ele_addr()) {
        return TUYA_MESH_API_NO_ERROR;
    }

    // Execute
    m_tb_mio_set_prim_addr(addr);

    // Rebinding & Delete subscription and publish params
    for(model_lid = 0; model_lid < m_tb_mio_get_nb_model(); model_lid++) {

        for(model_bound_nb = 0;model_bound_nb < m_tb_mio_get_nb_bound_app(model_lid); model_bound_nb++) {

            for(uint8_t i = 0;i < m_tb_key_get_nb_app_keys(true); i++) {

                m_tb_key_model_unbind(appkey_lid + i,model_lid);
                m_tb_mio_unbind(model_lid,appkey_lid + i);
                m_tb_key_model_bind(appkey_lid + i,model_lid);
                m_tb_mio_bind(model_lid);

                if(m_tb_mio_get_subscription_list_size(model_lid)) {
                    m_tb_mio_delete_all_subscription(model_lid);
                }
            }
        }
    }

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief Used to get the primary element addr of the node
 * @return mesh addr
 */
uint16_t tuya_get_primary_ele_addr(void)
{
    return m_tb_mio_get_prim_addr();
}

/**
 * @brief This function can reset the device.
 * @return none
 */
void tuya_mesh_network_reset(void)
{
    app_mesh_remove_bond();
}

/**
 * @brief Init mesh node capablity
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_device_update(uint8_t *key)
{
    // Get device key information
    const m_tb_key_dev_t *p_key_dev;

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    if(MESH_ERR_NO_ERROR != m_tb_key_dev_get(&p_key_dev)) {
        return TUYA_MESH_API_DEV_KEY_UPD_FAIL;
    }

    if(0 == memcmp(key,&p_key_dev->key[0],MESH_KEY_LEN)) {
        return TUYA_MESH_API_NO_ERROR;
    }

    m_tb_key_dev_delete();
    if(MESH_ERR_NO_ERROR != m_tb_key_dev_add(key)) {
        return TUYA_MESH_API_DEV_KEY_UPD_FAIL;
    }

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief Add the APPKey Manually and Automatic binding model
 * @param [in] net_key_lid local identifier of network key
 * @param [in] app_key_id identifier of appkey
 * @param [in] app_key app key to add
 * @return tuya_api_state_t
 */
static void tuya_mesh_app_key_added_handler(uint16_t status, m_lid_t app_key_lid)
{
    uint8_t model_lid;

    if(MESH_ERR_NO_ERROR == status) {
        for(model_lid = 1; model_lid < m_tb_mio_get_nb_model(); model_lid++) {
                m_tb_key_model_bind(app_key_lid,model_lid);
                m_tb_mio_bind(model_lid);
        }
    }
}

tuya_api_state_t tuya_mesh_add_apk_self_bind_model(m_lid_t net_key_lid,uint16_t app_key_id,uint8_t *app_key)
{
    // Network key information
    const m_tb_key_net_t *p_key_net;
    // AppKey Lid
    m_lid_t appkey_lid;

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    // Get Netkey key
    if (MESH_ERR_NO_ERROR != m_tb_key_net_get(net_key_lid, &p_key_net, false)) {
        return TUYA_MESH_API_NET_KEY_GET_FAIL;
    }

    // Check app key id has added
    if(MESH_ERR_NO_ERROR == m_tb_key_app_find(app_key_id,&appkey_lid)) {
        return TUYA_MESH_API_APP_KEY_GET_FAIL;
    }

    // APP Key Add
    if(MESH_ERR_NO_ERROR != m_tb_key_app_add(app_key_id,app_key,net_key_lid,tuya_mesh_app_key_added_handler)) {
        return TUYA_MESH_API_APP_KEY_ADD_FAIL;
    }

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief Init mesh network params update
 * @param [in] upd_info update info of network
 * @return tuya_api_state_t
 */
static tuya_mesh_net_app_upd_t *upd_info_bak = NULL;

static void tuya_mesh_app_key_updated_handler(uint16_t status, m_lid_t app_key_lid)
{
    if(MESH_ERR_NO_ERROR == status) {
        m_tb_key_net_use_new(upd_info_bak->netkey_lid);
        m_tb_key_net_revoke_old(upd_info_bak->netkey_lid);
    }
}

static void tuya_mesh_net_key_updated_handler(uint16_t status, m_lid_t net_key_lid)
{
    if(MESH_ERR_NO_ERROR == status) {
        if(0 == m_tb_key_get_nb_app_keys(upd_info_bak->netkey_lid)) {
            m_tb_key_net_use_new(upd_info_bak->netkey_lid);
            m_tb_key_net_revoke_old(upd_info_bak->netkey_lid);
        }
        else {
            m_tb_key_app_update(upd_info_bak->appkey_id,upd_info_bak->app_key,
                                upd_info_bak->netkey_lid,tuya_mesh_app_key_updated_handler);
        }
    }
}

tuya_api_state_t tuya_mesh_network_params_update(tuya_mesh_net_app_upd_t *upd_info)
{
    // Network key information
    const m_tb_key_net_t *p_key_net;

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    // Get Netkey key
    if (MESH_ERR_NO_ERROR != m_tb_key_net_get(upd_info->netkey_lid, &p_key_net, false)) {
        return TUYA_MESH_API_NET_KEY_UPD_FAIL;
    }

    // Check
    if(0 == memcmp(upd_info->net_key,&p_key_net->key[0],MESH_KEY_LEN)) {
        return TUYA_MESH_API_NET_KEY_UPD_FAIL;
    }

    // net key update
    m_tb_key_net_update(upd_info->netkey_lid,upd_info->net_key,tuya_mesh_net_key_updated_handler);

    // Set bak for app key update use
    upd_info_bak = upd_info;

    // Set IV
    m_tb_key_set_iv_seq(upd_info->iv_index, 0);

    // Inform the storage manager
    m_tb_store_update_ind(M_TB_STORE_UPD_TYPE_STATE, M_TB_STORE_TYPE_IV_SEQ);

    // send msg to nvds when indicate save
    app_mesh_storage_save();

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief Set mesh tx power
 * @param tx_power [-20,7]
 */
void tuya_mesh_rf_power_set(int tx_power)
{
    rf_tx_power_set(false,tx_power);
}

/**
 * @brief Publish data.
 * @param[in] model_id      model identifier
 * @param[in] appkey_id     appkey identifier
 * @param[in] pub_adr       publish address
 * @param[in] opcode
 * @param[in] vendor
 * @param[in] p_data        publish data address
 * @param[in] p_data_len    publish data length
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_model_publish(uint32_t model_id,uint32_t appkey_id,uint16_t pub_adr,uint32_t opcode,bool vendor,uint8_t *p_data,uint16_t p_data_len)
{
    uint16_t get_pub_adr;
    uint8_t  get_appkey_lid;
    uint8_t  get_ttl;

    mesh_tb_buf_t *p_buf;
    m_lid_t appkey_lid;

    m_lid_t model_lid = m_tb_mio_get_model_lid(model_id);

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    if(MESH_INVALID_LID == model_lid) {
        return TUYA_MESH_API_MDL_PUB_FAIL;
    }

    if(true != m_tb_key_get_nb_net_keys()) {
        return TUYA_MESH_API_NET_KEY_NUM_FAIL;
    }

    if(0 == m_tb_key_get_nb_app_keys(true)) {
        return TUYA_MESH_API_APP_KEY_NUM_FIAL;
    }

    if(MESH_ERR_NO_ERROR != m_tb_key_app_find(appkey_id,&appkey_lid)) {
        return TUYA_MESH_API_APP_KEY_NUM_FIAL;
    }

    if (MESH_ERR_NO_ERROR != m_tb_key_model_bind_check(appkey_lid,model_lid)) {
        return TUYA_MESH_API_CHECK_BIND_FIAL;
    }

    m_tb_mio_get_publi_param(model_lid,&get_pub_adr,NULL,&get_appkey_lid,&get_ttl,NULL,NULL,NULL);

    if(get_pub_adr != pub_adr || get_appkey_lid != appkey_lid || get_ttl != m_tb_state_get_default_ttl()) {
        m_tb_mio_set_publi_param(model_lid,pub_adr,NULL,appkey_lid,m_tb_state_get_default_ttl(),0,0,0);
    }

    if(MESH_ERR_NO_ERROR == mesh_tb_buf_alloc(&p_buf,vendor == true ? TUYA_MM_VENDOR_HEAD_LEN : TUYA_MM_SIG_HEAD_LEN,
                            p_data_len,false)) {

        uint8_t *p_msg = MESH_TB_BUF_DATA(p_buf);
        memcpy(p_msg, p_data, p_data_len);

        // Publish the message
        if(MESH_ERR_NO_ERROR != m_api_model_publish(model_lid, opcode, false, p_buf, false)) {
            m_api_buf_release(p_msg);
        }
    }
    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief BLE get connect state
 * @param
 * @return 1:connected 0:disconnected
 */
uint8_t tuya_get_ble_connect_state(void)
{
    if(ke_state_get(TASK_APP) == APPM_CONNECTED) {
        return true;
    }
    return false;
}

/**
 * @brief BLE disconnect
 * @param
 * @return
 */
void tuya_ble_disconnect(void)
{
    appm_disconnect(0);
}

/**
 * @brief BLE beacon recevice callback handler
 * @param [in] pdu_type    receive data type
 * @param [in] p_data      receive data address
 * @param [in] p_data_len  receive data lenght
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_ble_beacon_rec_handler(uint8_t pdu_type, uint8_t *p_data,uint8_t p_data_len)
{
    //TODO Check length
    if(p_data_len > M_DEF_ADV_LEN_MAX) return TUYA_MESH_API_INVALID_LEN;
    if(p_data_len < M_DEF_ADV_LEN_MIN) return TUYA_MESH_API_INVALID_LEN;

    //TODO Check type
    if((pdu_type != M_ADV_IND) && (pdu_type != M_ADV_NONCONN_IND))return TUYA_MESH_API_INVALID_PARAMS;

    // TODO ......

    //__LOG_XB(LOG_SRC_APP, LOG_LEVEL_INFO,"RecBeacon:",p_data,p_data_len);

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief BLE beacon send set
 * @param [in] p_data      send data address
 * @param [in] p_data_len  send data lenght
 * @return
 */
void tuya_ble_beacon_send_set(uint8_t *p_data,uint8_t p_data_len)
{
    m_prov_bearer_gatt_start_custom(p_data,p_data_len);
}

/**
 * @brief BLE beacon send set
 * @param [in] unporv_en   switch unprovision beacon
 * @param [in] proxy_en    switch proxy beacon
 * @return
 */
void tuya_mesh_beacon_ctrl(bool unprov_en,bool proxy_en)
{
    m_bcn_unprov_tx_en(unprov_en);
    m_bcn_proxy_tx_en(proxy_en);
}

/**
 * @brief BLE mesh set group address to subscription list
 * @param [in] model_id    model identifier
 * @param [in] sub_adr     subscription group address
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_subs_list_set(uint32_t model_id,uint16_t sub_adr)
{
    m_lid_t model_lid = m_tb_mio_get_model_lid(model_id);
    uint16_t subs_list[M_TB_MIO_SUBS_LIST_SIZE];

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    if(MESH_INVALID_LID == model_lid) {
        return TUYA_MESH_API_SET_SUBS_ADR_FAIL;
    }

    if(m_tb_mio_get_subscription_list_size(model_lid) > M_TB_MIO_SUBS_LIST_SIZE) {
        return TUYA_MESH_API_SET_SUBS_ADR_FAIL;
    }

    m_tb_mio_get_subscription_list(model_lid, (uint8_t*)subs_list, false);

    for(uint8_t i = 0; i < m_tb_mio_get_subscription_list_size(model_lid); i ++) {
        if(sub_adr == subs_list[i]) {
            return TUYA_MESH_API_NO_ERROR;
        }
    }

    if(MESH_ERR_NO_ERROR != m_tb_mio_add_subscription(model_lid,sub_adr)) {
        return TUYA_MESH_API_SET_SUBS_ADR_FAIL;
    }

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief BLE mesh get group address from subscription list
 * @param [in] model_id    model identifier
 * @param [in] sub_adr     subscription group address list
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_subs_list_get(uint32_t model_id,uint8_t *sub_adr)
{
    m_lid_t model_lid = m_tb_mio_get_model_lid(model_id);

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    if(MESH_INVALID_LID == model_lid) {
        return TUYA_MESH_API_GET_SUBS_ADR_FAIL;
    }

    m_tb_mio_get_subscription_list(model_lid,sub_adr,false);

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief BLE mesh delete group address of subscription list
 * @param [in] model_id    model identifier
 * @param [in] sub_adr     subscription group address list
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_subs_list_del(uint32_t model_id,uint16_t sub_adr)
{
    m_lid_t model_lid = m_tb_mio_get_model_lid(model_id);

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    if(MESH_INVALID_LID == model_lid) {
        return TUYA_MESH_API_DEL_SUBS_ADR_FAIL;
    }

    if(false == m_tb_mio_get_subscription_list_size(model_lid)) {
        return TUYA_MESH_API_NO_ERROR;
    }

    m_tb_mio_delete_subscription(model_lid,sub_adr);

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief BLE mesh delete all group address of subscription list
 * @param [in] model_id    model identifier
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_subs_list_del_all(uint32_t model_id)
{
    m_lid_t model_lid = m_tb_mio_get_model_lid(model_id);

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    if(MESH_INVALID_LID == model_lid) {
        return TUYA_MESH_API_DEL_SUBS_ADR_FAIL;
    }

    if(false == m_tb_mio_get_subscription_list_size(model_lid)) {
        return TUYA_MESH_API_NO_ERROR;
    }

    m_tb_mio_delete_all_subscription(model_lid);

    return TUYA_MESH_API_NO_ERROR;
}

/**
 * @brief BLE mesh print LOG for debug
 * @param [in] model_id    model identifier
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_info_log(void)
{
    m_lid_t mdl_num = 0;
    m_lid_t app_key_num = 0;
    uint32_t iv = 0;
    // Get device key information
    const m_tb_key_dev_t *p_key_dev;
    // Network key information
    const m_tb_key_net_t *p_key_net;
    // Application key information
    const m_tb_key_app_t *p_key_app;

    if(!tuya_get_if_prov_success()) {
        return TUYA_MESH_API_UNPORVED_STATE_FAIL;
    }

    // Model Info
    mdl_num = m_tb_mio_get_nb_model();
    printf("\n\n");
    __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "[ Mesh Info Start ]\n");
    __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "> Model Number: %d\n",mdl_num);
    __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "> Model ID: ");
    for(m_lid_t i = 0; i < mdl_num; i++) {
        printf("0x%x  ",m_tb_mio_get_model_id(i,NULL));
    }
    printf("\n");

    // Device Key
    m_tb_key_dev_get(&p_key_dev);
    __LOG_XB(LOG_SRC_API, LOG_LEVEL_INFO, "> Device Key",&p_key_dev->key[0],MESH_KEY_LEN);

    // Network Key
    m_tb_key_net_get(true, &p_key_net, false);
    __LOG_XB(LOG_SRC_API, LOG_LEVEL_INFO, "> Network Key",&p_key_net->key[0],MESH_KEY_LEN);

    // APP Key
    app_key_num = m_tb_key_get_nb_app_keys(true);
    for(m_lid_t i = 0; i < app_key_num; i++) {
        m_tb_key_app_get(i + M_TB_KEY_MAX_NB_NET + M_TB_KEY_MAX_NB_DEV, &p_key_app, false);
        __LOG_XB(LOG_SRC_API, LOG_LEVEL_INFO, "> APP Key",&p_key_app->key[0],MESH_KEY_LEN);
    }

    // TTL and IV
    m_tb_key_get_iv_seq(NULL,&iv);
    __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "> TTL Default 0x%x,IV Index: 0x%x\n",m_tb_state_get_default_ttl(),iv);

    // END
    __LOG(LOG_SRC_API, LOG_LEVEL_INFO, "[ Mesh Info End ]\n");
    printf("\n\n");

    return TUYA_MESH_API_NO_ERROR;
}
#endif /* APP_MESH_TUYA */

#endif //(BLE_APP_PRESENT)
