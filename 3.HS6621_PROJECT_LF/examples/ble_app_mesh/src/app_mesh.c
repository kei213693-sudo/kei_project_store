/**
 ****************************************************************************************
 *
 * @file app_mesh.c
 *
 * @brief BLE mesh app entry.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

/*********************************************************************
 * INCLUDES
 */
#include "rwip_config.h" // SW configuration

#include "app.h"
#include "m_tb.h"
#include "peripheral.h"
#include "prf_utils.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static app_mesh_node_id_t mesh_device_id = {
    .uuid       = DEFAULT_UUID,
    .compamy_id = DEFAULT_CID,
    .product_id = DEFAULT_PID,
    .version_id = DEFAULT_VID,
};

static app_mesh_auth_data_t mesh_auth_data;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
extern const struct prf_task_cbs *mal_prf_itf_get(void);

void app_mesh_add_profile(void)
{
    struct mesh_cfg* db_cfg;
    // Allocate the BASS_CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd, sizeof(struct mesh_cfg));

    // Mesh profile interface pair
    struct prf_itf_pair itf_pair = {
        TASK_ID_MESH, mal_prf_itf_get(),
    };
    // Profile instance & Number
    prf_itf_register(&itf_pair, 1);

    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl     = PERM(SVC_AUTH, NO_AUTH);
    req->prf_task_id = TASK_ID_MESH;
    req->app_task    = TASK_APP;
    req->start_hdl   = 0;

    // Set parameters
    db_cfg = (struct mesh_cfg *) req->param;

    /// Mask of supported features (@see enum m_api_feat)
    db_cfg->prf_cfg.features = M_FEAT_RELAY_NODE_SUP |
                               M_FEAT_PROXY_NODE_SUP |
                               M_FEAT_FRIEND_NODE_SUP |
                               M_FEAT_LOW_POWER_NODE_SUP |
                               M_FEAT_MSG_API_SUP |
                               M_FEAT_PB_GATT_SUP |
                               M_FEAT_DYN_BCN_INTV_SUP;
    /// 16-bit company identifier assigned by the Bluetooth SIG
    db_cfg->prf_cfg.cid = app_mesh_node_id_get_cid();
    /// 16-bit vendor-assigned product identifier
    db_cfg->prf_cfg.pid = app_mesh_node_id_get_pid();
    /// 16-bit vendor-assigned product version identifier
    db_cfg->prf_cfg.vid = app_mesh_node_id_get_vid();
    /// Localization descriptor
    db_cfg->prf_cfg.loc = 0x0100;
    /// Number of pages in the Composition Data
    db_cfg->prf_cfg.nb_cdata_page = 0x01;
    /// Receive window in milliseconds when Friend feature is supported
    db_cfg->prf_cfg.frd_rx_window_ms = 0;
    /// Queue size when Friend feature is supported
    db_cfg->prf_cfg.frd_queue_size = 0;
    /// Subscription list size when Friend feature is supported
    db_cfg->prf_cfg.frd_subs_list_size = 0;

    /// Mesh Model Configuration
    db_cfg->model_cfg.nb_models = 1;

    // Send the message
    ke_msg_send(req);
}

void app_mesh_enable(void)
{
    // Allocate the message
    struct m_api_cmd * req = KE_MSG_ALLOC(MESH_API_CMD,
                                            prf_get_task_from_id(TASK_ID_MESH),
                                            TASK_APP,
                                            m_api_cmd);

    /// Command code (@see enum m_api_cmd_code)
    req->cmd_code = M_API_ENABLE;

    // Send the message
    ke_msg_send(req);
}

void app_mesh_disable(void)
{
    // Allocate the message
    struct m_api_cmd * req = KE_MSG_ALLOC(MESH_API_CMD,
                                            prf_get_task_from_id(TASK_ID_MESH),
                                            TASK_APP,
                                            m_api_cmd);

    /// Command code (@see enum m_api_cmd_code)
    req->cmd_code = M_API_DISABLE;

    // Send the message
    ke_msg_send(req);
}

void app_mesh_node_id_init(app_mesh_node_id_t nid)
{
    memcpy(mesh_device_id.uuid,nid.uuid,MESH_DEV_UUID_LEN);
    mesh_device_id.compamy_id = nid.compamy_id;
    mesh_device_id.product_id = nid.product_id;
    mesh_device_id.version_id = nid.version_id;
}

void app_mesh_node_id_set_uuid(uint8_t *uuid)
{
    memcpy(mesh_device_id.uuid,uuid,MESH_DEV_UUID_LEN);
}

void app_mesh_node_id_get_uuid(uint8_t *uuid)
{
    memcpy(uuid,mesh_device_id.uuid,MESH_DEV_UUID_LEN);
}

uint16_t app_mesh_node_id_get_cid(void)
{
    return mesh_device_id.compamy_id;
}

uint16_t app_mesh_node_id_get_pid(void)
{
    return mesh_device_id.product_id;
}

uint16_t app_mesh_node_id_get_vid(void)
{
    return mesh_device_id.version_id;
}

void app_mesh_prov_auth_data_set(app_mesh_auth_data_t auth_data)
{
    memcpy(mesh_auth_data,auth_data,M_OOB_AUTH_DATA_LEN);
}

app_mesh_auth_data_t* app_mesh_prov_auth_data_get(void)
{
    return &mesh_auth_data;
}

#if (BLE_MESH_GATT_PROXY)
void app_mesh_proxy_service_enable(void)
{
    // Allocate the message
    struct m_api_proxy_ctl_cmd * req = KE_MSG_ALLOC(MESH_API_CMD,
                                            prf_get_task_from_id(TASK_ID_MESH),
                                            TASK_APP,
                                            m_api_proxy_ctl_cmd);
    /// Command code (@see enum m_api_cmd_code)
    req->cmd_code = M_API_PROXY_CTL;
    req->enable   = MESH_PROXY_ADV_CTL_START_NET;

    // Send the message
    ke_msg_send(req);
}
#else
void app_mesh_proxy_service_enable(void) { }
#endif

#if (NVDS_SUPPORT)
void app_mesh_storage_load(void)
{
    // Allocate the message
    struct m_api_storage_load_cmd * req = KE_MSG_ALLOC(MESH_API_CMD,
                                            prf_get_task_from_id(TASK_ID_MESH),
                                            TASK_APP,
                                            m_api_storage_load_cmd);

    /// Command code (@see enum m_api_storage_load_cmd_t)
    req->cmd_code = M_API_STORAGE_LOAD;
    req->length   = 0;

    // Send the message
    ke_msg_send(req);
}

void app_mesh_storage_save(void)
{
    // Allocate the message
    struct m_api_cmd * req = KE_MSG_ALLOC(MESH_API_CMD,
                                            prf_get_task_from_id(TASK_ID_MESH),
                                            TASK_APP,
                                            m_api_cmd);

    /// Command code (@see enum m_api_cmd_code)
    req->cmd_code = M_API_STORAGE_SAVE;

    // Send the message
    ke_msg_send(req);
}

void app_mesh_remove_bond(void)
{
    // Counter
    uint8_t cnt;

    // Clean all NVDS tags related to mesh
    for (cnt = 0; cnt < NVDS_TAG_MESH_NB; cnt++)
    {
        nvds_del(M_TB_STORE_GET_NVDS_TAG(cnt));
    }
    /// Force reboot
    pmu_force_reboot();
}

void app_mesh_set_mac_addr(uint8_t *addr)
{
    uint8_t addr_get[GAP_BD_ADDR_LEN];
	nvds_tag_len_t length = GAP_BD_ADDR_LEN;
    if((NVDS_OK == nvds_get(PARAM_ID_BD_ADDRESS,&length,addr_get)) && (0 == memcmp(addr_get,addr,GAP_BD_ADDR_LEN))) {
        return;
    }
    nvds_put(PARAM_ID_BD_ADDRESS,GAP_BD_ADDR_LEN,addr);
}
#else

void app_mesh_storage_load(void){}
void app_mesh_storage_save(void){}
void app_mesh_remove_bond(void){}
void app_mesh_set_mac_addr(uint8_t *addr){}

#endif //(NVDS_SUPPORT)

#if BLE_MESH_LPN

void app_mesh_lpn_start(void)
{
    // Allocate the message
    struct m_api_lpn_start_cmd *req = KE_MSG_ALLOC(MESH_API_CMD,
                                                    prf_get_task_from_id(TASK_ID_MESH),
                                                    TASK_APP,
                                                    m_api_lpn_start_cmd);
    /// Command code (@see enum m_api_cmd_code)
    req->cmd_code = M_API_LPN_START;

    /// Initial value of PollTimeout timer (in multiple of 100ms),range[0x0A,0x34BBFF]
    req->poll_timeout = 50;

    /// Poll interval (in milliseconds)
    req->poll_intv_ms = 300;

    /// Unicast address of the primary element of the previous friend
    req->prev_addr = M_UNASSIGNED_ADDR;

    /// Receive delay (in units of 1ms)
    req->rx_delay = 200;

    /// RSSI factor
    req->rssi_factor = M_RSSI_FACTOR_2;

    /// Receive window factor
    req->rx_window_factor = M_RX_WINDOW_FACTOR_2;

    ///  Minimum queue size (log value)
    req->min_queue_size_log = M_MIN_CACHE_SIZE_LOG_32;

    // Send the message
    ke_msg_send(req);
}

void app_mesh_lpn_stop(void)
{
    // Allocate the message
    struct m_api_cmd * req = KE_MSG_ALLOC(MESH_API_CMD,
                                            prf_get_task_from_id(TASK_ID_MESH),
                                            TASK_APP,
                                            m_api_cmd);

    /// Command code (@see enum m_api_cmd_code)
    req->cmd_code = M_API_LPN_STOP;

    // Send the message
    ke_msg_send(req);
}

void app_mesh_lpn_select(uint16_t friend_addr)
{
   // Allocate the message
    struct m_api_lpn_select_friend_cmd *req = KE_MSG_ALLOC(MESH_API_CMD,
                                                    prf_get_task_from_id(TASK_ID_MESH),
                                                    TASK_APP,
                                                    m_api_lpn_select_friend_cmd);
    /// Command code (@see enum m_api_cmd_code)
    req->cmd_code = M_API_LPN_SELECT_FRIEND;

    /// Friend node address
    req->friend_addr = friend_addr;

    // Send the message
    ke_msg_send(req);
}

#else

void app_mesh_lpn_start(void) {}
void app_mesh_lpn_stop(void) {}
void app_mesh_lpn_select(uint16_t friend_addr) {}

#endif //(BLE_MESH_LPN)

/** @} */

