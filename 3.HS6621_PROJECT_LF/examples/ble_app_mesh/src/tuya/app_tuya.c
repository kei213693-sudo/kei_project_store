/**
 ****************************************************************************************
 *
 * @file app_tuya.c
 *
 * @brief BLE mesh app TUYA.
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
// TODO Test for Key Update
static tuya_mesh_net_app_upd_t upd_info = {
        .netkey_lid         = 1,    // Use Only one NetKey in Stack
        .net_key            = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
        .appkey_id          = 0,    // From PTS case APPKey Add
        .app_key            = {10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25},
        .iv_index           = 22,
};
// TODO Test for custom beacon
static uint8_t tuya_ble_bcn_dat[M_ADV_BCN_LEN_MAX];

/*
 * LOCAL FUNCTION DEFINITIONS
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
 * @brief Init TUYA app
 * @return
 */

void tuya_app_init(void)
{
    // TODO Test for set mac address
    uint8_t mac_addr[BD_ADDR_LEN] = {0x33,0x22,0x11,0x4d,0x23,0xdc};

    // TODO Test for set node params
    app_mesh_node_id_t nid = {
        .uuid           = {0xDC,0x23,0x4D,0x11,0x22,0x33,0x10,0x15,0x76,0x61,0x35,0x62,0x64,0x76,0x79,0x74},
        .compamy_id     = 0x4321,
        .product_id     = 0x5432,
        .version_id     = 0x6543
    };
    tuya_mesh_node_params_t node_info = {
        .relay_en           = 1,
        .proxy_en           = 1,
        .friend_en          = 0,
        .lpn_en             = 0,
        .nid                = nid
    };
    //tuya_mesh_node_params_init(&node_info);

    // TODO Test for set mac address
    app_mesh_set_mac_addr(mac_addr);

    // TODO Test for custom beacon
    tuya_ble_bcn_dat[0] = 0x03; // Length
    tuya_ble_bcn_dat[1] = 0xff; // Type
    tuya_ble_bcn_dat[2] = 0x55; // Dat0
    tuya_ble_bcn_dat[3] = 0x77; // Dat1
    //tuya_ble_beacon_send_set(tuya_ble_bcn_dat,4);

    // TODO Test for beacon ctrl
    //tuya_mesh_beacon_ctrl(1,1);
}

/**
 * @brief Mesh stack reset finish handler
 *
 * @param [in] pov_state true:porved;false:unproved
 *
 * @return
 */

void tuya_mesh_stack_reset_handler(bool prov_state)
{
#if 0
    // TODO Test for subscription list
    uint16_t g_subs_list[10];

    // TODO Test for publish data
    uint8_t pub_data[6] = {0x11,0x22,0x33,0x44,0x77,0x99};

    // TODO Test for primary address set
    uint16_t addr;

    // TODO Test for net params init
    tuya_mesh_net_params_t net_info = {
        .send_ttl           = 0x33,
        .net_transmit_cnt   = 4,
        .net_transmit_step  = 0x40
    };
    tuya_mesh_net_params_init(&net_info);

    // TODO Print Mesh Info Log
    //tuya_mesh_info_log();

    // TODO Add app key and binding model automatic
    //tuya_mesh_add_apk_self_bind_model(1,0,upd_info.app_key);

    // TODO Test for Key Update
    tuya_mesh_device_update(upd_info.net_key);
    tuya_mesh_network_params_update(&upd_info);

    // TODO Test for subscription list
    tuya_mesh_subs_list_del(0x1000,0xc000);
    tuya_mesh_subs_list_set(0x1000,0xc005);
    tuya_mesh_subs_list_set(0x1000,0xc001);
    tuya_mesh_subs_list_get(0x1000,(uint8_t *)g_subs_list);

    // TODO Test for primary address set
    tuya_set_primary_ele_addr(0x1234);
    addr = tuya_get_primary_ele_addr();
    addr = addr;

    // TODO Test for publish data
    tuya_mesh_model_publish(0x1000,0,0xc003,0x0482,0,pub_data,sizeof(pub_data));

#endif
    // TODO You should add User Code when stack reset finish !!
    // TODO User Code ......
}
#endif /* APP_MESH_TUYA */

#endif //(BLE_APP_PRESENT)