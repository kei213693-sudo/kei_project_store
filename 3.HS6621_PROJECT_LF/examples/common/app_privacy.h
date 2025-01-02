/**
 ****************************************************************************************
 *
 * @file app_privacy.h
 * @brief about privacy setting.
 * @date Mon, Jan  7, 2019  4:31:27 PM
 * @author chenzhiyuan
 *
 *
 ****************************************************************************************
 */

#ifndef APP_PRIVACY_H_
#define APP_PRIVACY_H_

/**
 ****************************************************************************************
 * @addtogroup APP_COMMON_PRIVACY app_privacy.h
 * @ingroup APP_COMMON
 * @brief about privacy setting.
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

#if (BLE_APP_PRIVACY)

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

/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Init white list
 *
 * @param[in] enable: 1: enable privacy; 0:disable privacy
 * ****************************************************************************************
 */
void appm_privacy_init(uint8_t enable);
/**
 ****************************************************************************************
 * @brief add device to resolving list
 * @param[in] device: struct gap_ral_dev_info
 ****************************************************************************************
 */
void appm_privacy_add_device(uint8_t* device);
/**
 ****************************************************************************************
 * @brief set resolving list size
 * @param[in] size: uint8 type, resolving list size;
 ****************************************************************************************
 */
void appm_privacy_set_size(uint8_t size);
/**
 ****************************************************************************************
 * @brief clear resolving list
 * @return void;
 ****************************************************************************************
 */
void appm_privacy_clear(void);

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
                            ke_task_id_t const src_id);
#endif

#endif //(BLE_APP_PRESENT)

/// @} APP

#endif // APP_PRIVACY_H_
