/**
 * @file rwapp_config.h
 * @brief 
 * @date Wed, Sep  5, 2018  5:44:25 PM
 * @author liqiang
 *
 * @defgroup 
 * @ingroup 
 * @brief 
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

//#define DEBUG_ADDR "\xC1\xC1\xC1\xC1\xC1\xC1" // Address just for debug

/// HID Application
#define BLE_APP_HID          1
/// DIS Application
#define BLE_APP_DIS          1
/// Battery Service Application
#define BLE_APP_BATT         0
/// Security Application
#define BLE_APP_SEC          1
///Device Information Service Server Role
#define BLE_DIS_SERVER       1
///Battery Service Server Role
#define BLE_BATT_SERVER      0
///HID Device Role
#define BLE_HID_DEVICE       1
/// white list
#define BLE_APP_WHITE_LIST   0
// privacy
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

