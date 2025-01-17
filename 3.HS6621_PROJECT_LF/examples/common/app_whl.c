/**
 ****************************************************************************************
 *
 * @file app_whl.c
 * @brief about white list setting.
 * @date Mon, Jan  7, 2019  4:31:27 PM
 * @author chenzhiyuan
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_COMMON_WHL_C app_whl.c
 * @ingroup APP_COMMON
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRESENT)

#include <string.h>
#include "gapm_task.h"               // GAP Manager Task API
#include "app_task.h"                // Application task Definition
#include "app.h"                     // Application Definition
#include "gap.h"                     // GAP Definition
#include "gapc_task.h"               // GAP Controller Task API

#include "co_bt.h"                   // Common BT Definition
#include "co_math.h"                 // Common Maths Definition
#include "co_debug.h"
#if (BLE_APP_SEC)
#include "app_sec.h"                 // Application security Definition
#endif // (BLE_APP_SEC)

#if (NVDS_SUPPORT)
#include "nvds.h"                    // NVDS Definitions
#endif //(NVDS_SUPPORT)

#if (BLE_APP_WHITE_LIST)
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
struct app_whl_env_tag app_whl_env;
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief clear white list
 ****************************************************************************************
 */
//重新上电时要重写一遍白名单？
uint8_t appm_whl_set_list(void)
{
    uint8_t bond_num = 0; // when nvds modify, the bond num get from nvds
    uint8_t nvds_len = NVDS_TAG_BLE_LINK_KEY_LAST-NVDS_TAG_BLE_LINK_KEY_FIRST;
    // BD Address of the peer device
    struct gap_bdaddr peer_bd_addr[NVDS_TAG_BLE_LINK_KEY_LAST-NVDS_TAG_BLE_LINK_KEY_FIRST];

    if (app_whl_env.enable == 0)
    {
        return 0;
    }

    // get from nvds
    if (app_sec_get_bond_status())
    {
        struct app_sec_conn_info_tag info;
        nvds_tag_len_t length = sizeof(struct app_sec_conn_info_tag);
        uint8_t index = 0;
        
        // Get bond status from NVDS
        for(index = 0; index <=nvds_len; index++)
        {
            if (nvds_get(NVDS_TAG_BLE_LINK_KEY_LAST-index, &length, (uint8_t *)&info) == NVDS_OK)
            {
                #if (BLE_APP_PRIVACY)
                memcpy(&peer_bd_addr[bond_num], &info.irk.addr, sizeof(struct gap_bdaddr));
                #else
                memcpy(&peer_bd_addr[bond_num], &info.id_addr, sizeof(struct gap_bdaddr));
                #endif
                bond_num++;
            }
        }
    }

    if (bond_num == 0) return 0;

    struct gapm_list_set_wl_cmd* cmd = KE_MSG_ALLOC_DYN(GAPM_LIST_SET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_list_set_wl_cmd, sizeof(struct gap_bdaddr)*bond_num);
    cmd->operation = GAPM_SET_WL;

    cmd->size = bond_num;
    for(uint8_t i=0;i<bond_num;i++)
    {
        memcpy(&cmd->wl_info[i], &peer_bd_addr[i], sizeof(struct gap_bdaddr));
    }
    ke_msg_send(cmd);
    return bond_num;
}

/**
 ****************************************************************************************
 * @brief get white list size
 ****************************************************************************************
 */
void appm_whl_get_size(void)
{
    if (app_whl_env.enable == 0)
    {
        return;
    }

    struct gapm_get_dev_info_cmd* cmd = KE_MSG_ALLOC(GAPM_GET_DEV_INFO_CMD,
            TASK_GAPM, TASK_APP,
            gapm_get_dev_info_cmd);
    cmd->operation = GAPM_GET_WLIST_SIZE;
    ke_msg_send(cmd);
}

/**
 ****************************************************************************************
 * @brief set white list size
 ****************************************************************************************
 */
void appm_whl_set_size(uint8_t size)
{
    if (app_whl_env.enable == 0)
    {
        return;
    }

    app_whl_env.size = size;
}

/**
 ****************************************************************************************
 * @brief clear white list
 ****************************************************************************************
 */
void appm_whl_clear(void)
{
    if (app_whl_env.enable == 0)
    {
        return;
    }

    struct gapm_list_set_cmd* cmd = KE_MSG_ALLOC(GAPM_LIST_SET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_list_set_cmd);
    cmd->operation = GAPM_SET_WL;
    cmd->size = 0;
    ke_msg_send(cmd);

    struct gapm_list_set_ral_cmd* cmd1 = KE_MSG_ALLOC_DYN(GAPM_LIST_SET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_list_set_ral_cmd, sizeof(struct gap_ral_dev_info));
    cmd1->operation = GAPM_SET_RAL;
    cmd1->size = 0;
    ke_msg_send(cmd1);
}

/**
 ****************************************************************************************
 * @brief add device to white list for public
 * @param[in] device: struct gap_bdaddr
 ****************************************************************************************
 */
void appm_whl_add_device(uint8_t* device)
{
    if((app_whl_env.enable == 0) || (NULL == device))
    {
        return;
    }

    log_debug("whl public addr:%02x:%02x:%02x:%02x:%02x:%02x,%d\n", device[0], device[1],device[2],device[3],device[4],device[5],device[6]);
    
    //对端设备是公共地址情况直接将addr当作参数存入白名单列表
    struct gapm_list_set_wl_cmd* cmd = KE_MSG_ALLOC_DYN(GAPM_LIST_SET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_list_set_wl_cmd, sizeof(struct gap_bdaddr));
    cmd->operation = GAPM_SET_WL;
    cmd->size = 1;
    memcpy(&cmd->wl_info[0], (struct gap_bdaddr*)device, sizeof(struct gap_bdaddr));

    ke_msg_send(cmd);
}

/**
 ****************************************************************************************
 * @brief add device to white list for ral addr
 * @param[in] 
 ****************************************************************************************
 */
void appm_whl_add_device_ral(struct gap_ral_dev_info *pStData)
{
    if((app_whl_env.enable == 0) || (NULL == pStData))
    {
        return;
    }

    log_debug("priv_mode:%d",priv_mode);
    log_debug_array_ex("whl ral addr", pStData->addr.addr.addr,GAP_BD_ADDR_LEN);
    log_debug_array_ex("whl peer irk", pStData->peer_irk,GAP_KEY_LEN);
    log_debug_array_ex("whl local irk", pStData->local_irk,GAP_KEY_LEN);

    //对端设备是可解析随机地址情况
    struct gapm_list_set_ral_cmd* cmd = KE_MSG_ALLOC_DYN(GAPM_LIST_SET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_list_set_ral_cmd, sizeof(struct gap_ral_dev_info));
    cmd->operation = GAPM_SET_RAL;
    cmd->size = 1;//本次要添加的条数

    memcpy(&cmd->ral_info[0],pStData,sizeof(struct gap_ral_dev_info));

    ke_msg_send(cmd);
}

/**
 ****************************************************************************************
 * @brief Init white list
 *
 * @param[in] enable: 1:enable whitelist; 0:disable whitelist
 * ****************************************************************************************
 */
void appm_whl_init(uint8_t enable)
{
    memset(&app_whl_env, 0, sizeof(struct app_whl_env_tag ));
    app_whl_env.enable = enable;
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
int appm_whl_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapm_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    log_debug("appm_whl_cmp_evt_handler:%d\n",param->operation);

    if (app_whl_env.enable == 0)
    {
        return (KE_MSG_NO_FREE);
    }
    switch(param->operation)
    {
        case (GAPM_SET_DEV_CONFIG):
            {
                appm_whl_get_size();
            }
            break;

        case (GAPM_GET_WLIST_SIZE):
            {
                if(appm_whl_set_list() == 0)
                {
                    return (KE_MSG_NO_FREE);
                }
            }
            break;

        case (GAPM_SET_WL):
            {
                return (KE_MSG_NO_FREE);
            }
            //break;//不会执行，屏蔽消除告警
			
        default:
            break;
    }
	
    return (KE_MSG_CONSUMED);
}
#endif
#endif //(BLE_APP_PRESENT)

/// @} APP
