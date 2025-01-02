/**
 * @file rwapp_config.h
 * @brief
 * @date Wed, Sep  5, 2018  5:44:25 PM
 * @author liqiang
 *
 * @addtogroup APP_SIMPLE_SERVER_CONFIG rwapp_config.h
 * @ingroup APP_SIMPLE_SERVER
 * @brief 使能profile，安全(sec)，白名单(whitelist)，隐私(privacy)
 * @details
 *
 * @{
 */

#ifndef __RWAPP_CONFIG_H__
#define __RWAPP_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "rwip_task.h"

/*********************************************************************
 * MACROS
 */
 #define BLE_APP_ONMICRO_DFU 1
 #define BLE_APP_SEC 1
 #define NVDS_SUPPORT 1
//#define BLE_BATT_SERVER 1
//#define BLE_APP_BATT 1
/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * EXTERN VARIABLES
 */

enum {
    TASK_ID_ONMICRO_DFU = TASK_ID_USER,
};
/*********************************************************************
 * EXTERN FUNCTIONS
 */


#ifdef __cplusplus
}
#endif

#endif

/** @} */

