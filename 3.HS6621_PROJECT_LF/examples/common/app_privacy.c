/**
 ****************************************************************************************
 *
 * @file app_privacy.c
 * @brief about privacy setting.
 * @date Mon, Jan  7, 2019  4:31:27 PM
 * @author chenzhiyuan
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_COMMON_PRIVACY_C app_privacy.c
 * @ingroup APP_COMMON
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

#if (BLE_APP_PRIVACY)
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**@struct app_privacy_env_tag 
 * @brief privacy env information. */
struct app_privacy_env_tag
{
	/// enable or disable privacy function
    uint8_t enable;
	/// the resolving list size
    uint8_t size;
};
/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/*
 * LOCAL VARIABLES DEFINITIONS
 ****************************************************************************************
 */
static struct app_privacy_env_tag app_privacy_env; // the privacy env var.
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
 * @brief clear resolving list
 ****************************************************************************************
 */
uint8_t appm_privacy_set_list(void)
{
    uint8_t bond_num = 0; // when nvds modify, the bond num get from nvds
    uint8_t nvds_len = NVDS_TAG_BLE_LINK_KEY_LAST-NVDS_TAG_BLE_LINK_KEY_FIRST;
    struct gapc_irk irk[NVDS_TAG_BLE_LINK_KEY_LAST-NVDS_TAG_BLE_LINK_KEY_FIRST];
    uint8_t loc_irk[GAP_KEY_LEN];
    struct gapm_list_set_ral_cmd* cmd;

    if (app_privacy_env.enable == 0)
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
                memcpy(&irk[bond_num], &info.irk, sizeof(struct gapc_irk));
                bond_num++;
            }
        }

        length = GAP_KEY_LEN;
        if(nvds_get(NVDS_TAG_LOC_IRK, &length, loc_irk) != NVDS_OK)
        {
            memset(loc_irk, 0x00, GAP_KEY_LEN);
        }
    }

    if (bond_num == 0)
    {
        cmd = KE_MSG_ALLOC_DYN(GAPM_LIST_SET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_list_set_ral_cmd, sizeof(struct gap_ral_dev_info));
        cmd->operation = GAPM_SET_RAL;
        cmd->size = 0;
        memset(&cmd->ral_info[0], 0, sizeof(sizeof(struct gap_ral_dev_info)));
        cmd->ral_info[0].priv_mode = 1;
        
  
    }
    else
    {
        cmd = KE_MSG_ALLOC_DYN(GAPM_LIST_SET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_list_set_ral_cmd, sizeof(struct gap_ral_dev_info)*bond_num);
        cmd->operation = GAPM_SET_RAL;
        cmd->size = bond_num;

        for(uint8_t i=0;i<bond_num;i++)
        {
            memcpy(&cmd->ral_info[i].addr, &irk[i].addr, sizeof(struct gap_bdaddr));
            cmd->ral_info[i].priv_mode = 1;
            memcpy(cmd->ral_info[i].peer_irk, irk[i].irk.key, GAP_KEY_LEN);
            memcpy(cmd->ral_info[i].local_irk, loc_irk, GAP_KEY_LEN);
        }
    }
    ke_msg_send(cmd);
    return bond_num;
}

/**
 ****************************************************************************************
 * @brief get list size
 ****************************************************************************************
 */
void appm_privacy_get_size(void)
{
    if (app_privacy_env.enable == 0)
    {
        return;
    }

    struct gapm_get_dev_info_cmd* cmd = KE_MSG_ALLOC(GAPM_GET_DEV_INFO_CMD,
            TASK_GAPM, TASK_APP,
            gapm_get_dev_info_cmd);
    cmd->operation = GAPM_GET_RAL_SIZE;
    ke_msg_send(cmd);
}

/**
 ****************************************************************************************
 * @brief set list size
 ****************************************************************************************
 */
void appm_privacy_set_size(uint8_t size)
{
    if (app_privacy_env.enable == 0)
    {
        return;
    }

    app_privacy_env.size = size;
}

/**
 ****************************************************************************************
 * @brief clear list
 ****************************************************************************************
 */
void appm_privacy_clear(void)
{
    if (app_privacy_env.enable == 0)
    {
        return;
    }

    struct gapm_list_set_cmd* cmd = KE_MSG_ALLOC(GAPM_LIST_SET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_list_set_cmd);
    cmd->operation = GAPM_SET_RAL;
    cmd->size = 0;
    ke_msg_send(cmd);
}

/**
 ****************************************************************************************
 * @brief add device to list
 ****************************************************************************************
 */
void appm_privacy_add_device(uint8_t* device)
{
    if (app_privacy_env.enable == 0)
    {
        return;
    }

    struct gapm_list_set_ral_cmd* cmd = KE_MSG_ALLOC_DYN(GAPM_LIST_SET_CMD,
            TASK_GAPM, TASK_APP,
            gapm_list_set_ral_cmd, sizeof(struct gap_ral_dev_info));
    cmd->operation = GAPM_SET_RAL;

    cmd->size = 1;

    memcpy(&cmd->ral_info[0], (struct gap_ral_dev_info*)device, sizeof(struct gap_ral_dev_info));
    ke_msg_send(cmd);
}

/**
 ****************************************************************************************
 * @brief Init white list
 *
 * @param[in] enable: 1: enable privacy; 0:disable privacy
 * ****************************************************************************************
 */
void appm_privacy_init(uint8_t enable)
{
    memset(&app_privacy_env, 0, sizeof(struct app_privacy_env_tag ));
    app_privacy_env.enable = enable;
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
int appm_privacy_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapm_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    if (app_privacy_env.enable == 0)
    {
        return (KE_MSG_NO_FREE);
    }
    
    switch(param->operation)
    {
        case (GAPM_SET_DEV_CONFIG):
            {
                appm_privacy_set_list();
            }
            break;
        case (GAPM_GET_RAL_SIZE):
            break;
        case (GAPM_SET_RAL):
            {
                return (KE_MSG_NO_FREE);
            }
            break;
        default:
            break;
    }
    
    return (KE_MSG_CONSUMED);
}
#endif
#endif //(BLE_APP_PRESENT)

/// @} APP
