/**
 * @file examples/ble_app_simple_server/src/rwapp_config.h
 * @brief
 * @date Wed, Sep  5, 2018  5:44:25 PM
 * @author liqiang
 *
 * @addtogroup APP_SIMPLE_SERVER_CONFIG rwapp_config.h
 * @ingroup APP_SIMPLE_SERVER
 * @brief all profile application，Security，whitelist，privacy setting
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


/*********************************************************************
 * MACROS
 */

/// enable/disable security connect
#define BLE_APP_SEC           1

/// enable/disable simple server profile
#define BLE_APP_SIMPLE_SERVER 1

/// enable/disable white list
#define BLE_APP_WHITE_LIST   0
/// enable/disable privacy
#define BLE_APP_PRIVACY      0

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * EXTERN VARIABLES
 */


/*********************************************************************
 * EXTERN FUNCTIONS
 */


#ifdef __cplusplus
}
#endif

#endif

/** @} */

