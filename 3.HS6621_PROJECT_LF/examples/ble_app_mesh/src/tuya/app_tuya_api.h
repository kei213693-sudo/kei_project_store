/**
 ****************************************************************************************
 *
 * @file app_tuya_api.h
 *
 * @brief BLE Mesh TUYA api header file.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

#ifndef __APP_TUYA_API__
#define __APP_TUYA_API__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "app_mesh.h"
#include "app_tuya_model.h"

/*********************************************************************
 * MACROS
 */
// mesh adv interval min 10ms
#define M_TB_NET_ADV_INTV_MIN       (0x20)

// mesh adv interval max 10.24ms
#define M_TB_NET_ADV_INTV_MAX       (0x4000)

// mesh adv retransmit number min 1
#define M_TB_NET_ADV_NUM_MIN        (1)

// mesh adv retransmit number max 8
#define M_TB_NET_ADV_NUM_MAX        (8)

// clear up adv params
#define M_TB_STATE_NET_TX(count, intv_slots)                                            \
    ((count - 1) | (((intv_slots >> 4) - 1) << 3))

// receive ble beacon advdata pdu type
#define M_ADV_NONCONN_IND           (0x09) /* GAPM_REPORT_TYPE_ADV_LEG | GAPM_REPORT_INFO_COMPLETE_BIT */
#define M_ADV_IND                   (0x39) /* GAPM_REPORT_TYPE_ADV_LEG | GAPM_REPORT_INFO_COMPLETE_BIT
                                                | GAPM_REPORT_INFO_CONN_ADV_BIT | GAPM_REPORT_INFO_SCAN_ADV_BIT */

// mesh advdata beacon length default limit
#define M_DEF_ADV_LEN_MIN           (4)
#define M_DEF_ADV_LEN_MAX           (31)
/*********************************************************************
 *  ENUM
 */
typedef enum
{
    /// No Error
    TUYA_MESH_API_NO_ERROR,
    /// Invalid length
    TUYA_MESH_API_INVALID_LEN,
    /// Invalid params
    TUYA_MESH_API_INVALID_PARAMS,
    /// Can't execute when unproved
    TUYA_MESH_API_UNPORVED_STATE_FAIL,
    /// Model register Fail
    TUYA_MESH_API_MDL_REG_FAIL,
    /// Model publish data Fail
    TUYA_MESH_API_MDL_PUB_FAIL,
    /// Access send data Fail
    TUYA_MESH_API_ACCESS_SEND_FAIL,
    /// Default TTL set Fail
    TUYA_MESH_API_TTL_SET_FAIL,
    /// Network Key GET Fail
    TUYA_MESH_API_NET_KEY_GET_FAIL,
    /// APP Key ADD Fail
    TUYA_MESH_API_APP_KEY_ADD_FAIL,
    /// APP Key GET Fail
    TUYA_MESH_API_APP_KEY_GET_FAIL,
    /// Device Key Update Fail
    TUYA_MESH_API_DEV_KEY_UPD_FAIL,
    /// Network Key Update Fail
    TUYA_MESH_API_NET_KEY_UPD_FAIL,
    /// Node Only Support Netkey One
    TUYA_MESH_API_NET_KEY_NUM_FAIL,
    /// Node Publish Need Least One APP Key
    TUYA_MESH_API_APP_KEY_NUM_FIAL,
    /// Node Publish Check Binding Fail
    TUYA_MESH_API_CHECK_BIND_FIAL,
    /// Set Subscription Address Fail
    TUYA_MESH_API_SET_SUBS_ADR_FAIL,
    /// Get Subscription Address Fail
    TUYA_MESH_API_GET_SUBS_ADR_FAIL,
    /// Delete Subscription Address Fail
    TUYA_MESH_API_DEL_SUBS_ADR_FAIL,
    /// Cannot Find APP KEY ID
    TUYA_MESH_API_FIND_APPKEY_ID_FAIL,
    /// Cannot Update APP KEY
    TUYA_MESH_API_UPD_APPKEY_FAIL,

}tuya_api_state_t;

/*********************************************************************
 * TYPEDEFS
 */
typedef struct tuya_mesh_node_params
{
    /// relay switch
    uint8_t relay_en;

    /// proxy switch
    uint8_t proxy_en;

    /// friend switch
    uint8_t friend_en;

    /// low power node switch
    uint8_t lpn_en;

    /// node id
    app_mesh_node_id_t nid;

}tuya_mesh_node_params_t;

typedef struct tuya_mesh_net_params
{
    /// Default TTL
    uint8_t send_ttl;

    /// retransmit number of network layer
    uint8_t net_transmit_cnt;

    /// retransimt step of network layer (1 = 0.625ms)
    uint16_t net_transmit_step;

}tuya_mesh_net_params_t;

typedef struct tuya_mesh_net_app_upd
{
    /// network key
    uint8_t net_key[MESH_KEY_LEN];

    /// network key index
    uint8_t netkey_lid;

    /// flags
    uint8_t flags;

    /// iv index
    uint32_t iv_index;

    /// unicast address
    uint16_t unicast_address;

    /// appkey id
    uint16_t appkey_id;

    /// appkey
    uint8_t app_key[MESH_KEY_LEN];

}tuya_mesh_net_app_upd_t;

/*********************************************************************
 * EXTERN FUNCTIONS
 */
/**
 * @brief Init mesh node capablity
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_node_params_init(tuya_mesh_node_params_t *node_info);

/**
 * @brief Init mesh node network info
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_net_params_init(tuya_mesh_net_params_t *net_info);

/**
 * @brief Used to know if the device have been provisioned successfully
 * @return prov state true:proved,false:unproved
 */
uint8_t tuya_get_if_prov_success(void);

/**
 * @brief Used to sest the primary element addr of the node
 * @param[in] addr unicast addr
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_set_primary_ele_addr(uint16_t addr);

/**
 * @brief Used to get the primary element addr of the node
 * @return mesh addr
 */
uint16_t tuya_get_primary_ele_addr(void);

/**
 * @brief This function can reset the device.
 * @return none
 */
void tuya_mesh_network_reset(void);

/**
 * @brief Init mesh node capablity
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_device_update(uint8_t *key);

/**
 * @brief Add the APPKey Manually and Automatic binding model
 * @param [in] net_key_lid local identifier of network key
 * @param [in] app_key_id identifier of appkey
 * @param [in] app_key app key to add
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_add_apk_self_bind_model(m_lid_t net_key_lid,uint16_t app_key_id,uint8_t *app_key);

/**
 * @brief Init mesh network params update
 * @param [in] upd_info update info of network
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_network_params_update(tuya_mesh_net_app_upd_t *upd_info);

/**
 * @brief Set mesh tx power
 * @param tx_power [-20,7]
 */
void tuya_mesh_rf_power_set(int tx_power);

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
tuya_api_state_t tuya_mesh_model_publish(uint32_t model_id,uint32_t appkey_id,uint16_t pub_adr,uint32_t opcode,bool vendor,uint8_t *p_data,uint16_t p_data_len);

/**
 * @brief BLE get connect state
 * @param
 * @return 1:connected 0:disconnected
 */
uint8_t tuya_get_ble_connect_state(void);

/**
 * @brief BLE disconnect
 * @param
 * @return
 */
void tuya_ble_disconnect(void);

/**
 * @brief BLE beacon recevice callback handler
 * @param [in] p_data      receive data address
 * @param [in] p_data_len  receive data lenght
 * @return
 */
tuya_api_state_t tuya_ble_beacon_rec_handler(uint8_t pdu_type, uint8_t *p_data,uint8_t p_data_len);

/**
 * @brief BLE beacon send set
 * @param [in] p_data      send data address
 * @param [in] p_data_len  send data lenght
 * @return
 */
void tuya_ble_beacon_send_set(uint8_t *p_data,uint8_t p_data_len);

/**
 * @brief BLE beacon send set
 * @param [in] unporv_en   switch unprovision beacon
 * @param [in] proxy_en    switch proxy beacon
 * @return
 */
void tuya_mesh_beacon_ctrl(bool unprov_en,bool proxy_en);

/**
 * @brief BLE mesh set group address to subscription list
 * @param [in] model_id    model identifier
 * @param [in] sub_adr     subscription group address
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_subs_list_set(uint32_t model_id,uint16_t sub_adr);

/**
 * @brief BLE mesh get group address to subscription list
 * @param [in] model_id    model identifier
 * @param [in] sub_adr     subscription group address list
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_subs_list_get(uint32_t model_id,uint8_t *sub_adr);

/**
 * @brief BLE mesh delete group address of subscription list
 * @param [in] model_id    model identifier
 * @param [in] sub_adr     subscription group address list
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_subs_list_del(uint32_t model_id,uint16_t sub_adr);

/**
 * @brief BLE mesh delete all group address of subscription list
 * @param [in] model_id    model identifier
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_subs_list_del_all(uint32_t model_id);

/**
 * @brief BLE mesh print LOG for debug
 * @return tuya_api_state_t
 */
tuya_api_state_t tuya_mesh_info_log(void);

/*********************************************************************
 * EXTERN STACK FUNCTION
 */
/**
 ****************************************************************************************
 * @brief Open The GATT Provisioning bearer type custom
 ****************************************************************************************
 */
void m_prov_bearer_gatt_start_custom(uint8_t *p_data,uint8_t p_data_len);


#ifdef __cplusplus
}
#endif

#endif /* __APP_TUYA_API__ */

/** @} */

