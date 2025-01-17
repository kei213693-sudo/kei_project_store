/**
 ****************************************************************************************
 *
 * @file app_whl.h
 * @brief about white list setting.
 * @date Mon, Jan  7, 2019  4:31:27 PM
 * @author chenzhiyuan
 *
 *
 ****************************************************************************************
 */

#ifndef APP_WHL_H_
#define APP_WHL_H_

/**
 ****************************************************************************************
 * @addtogroup APP_COMMON_WHL app_whl.h
 * @ingroup APP_COMMON
 *
 * @brief about white list setting.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_PRESENT)

#include <stdint.h>          // Standard Integer Definition
#include <co_bt.h>           // Common BT Definitions
#include "arch.h"            // Platform Definitions
#include "gapc.h"            // GAPC Definitions

#if (NVDS_SUPPORT)
#include "nvds.h"
#endif // (NVDS_SUPPORT)

#if (BLE_APP_WHITE_LIST)

#include "ke_msg.h"
#include "gapm_task.h"
/*
 * DEFINES
 ****************************************************************************************
 */
/*
 * MACROS
 ****************************************************************************************
 */


/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/// the white list env information
struct app_whl_env_tag
{
	/// enable or disable white list function
    uint8_t enable;
	/// the white list size
    uint8_t size;
};
/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */
extern struct app_whl_env_tag app_whl_env;


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Init white list
 *
 * @param[in] enable: 1:enable whitelist; 0:disable whitelist
 * ****************************************************************************************
 */
void appm_whl_init(uint8_t enable);
/**
 ****************************************************************************************
 * @brief add device to white list
 * @param[in] device: struct gap_bdaddr
 ****************************************************************************************
 */
void appm_whl_add_device(uint8_t* device);

void appm_whl_add_device_ral(struct gap_ral_dev_info *pStData);

/**
 ****************************************************************************************
 * @brief set white list size
 * @return void
 ****************************************************************************************
 */
void appm_whl_set_size(uint8_t size);

void appm_whl_get_size(void);

/**
 ****************************************************************************************
 * @brief clear white list
 * @return void
 ****************************************************************************************
 */
void appm_whl_clear(void);

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
                            ke_task_id_t const src_id);
#endif

#endif //(BLE_APP_PRESENT)

/// @} APP

#endif // APP_WHL_H_
