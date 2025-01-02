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
/// Default Device Name
#define APP_DFLT_DEVICE_NAME            ("BLE-TSPP")
#define APP_DFLT_DEVICE_NAME_LEN        (sizeof(APP_DFLT_DEVICE_NAME)-1)

//#define APP_DEVICE_NAME_1            ("okey00017310")
#define APP_DEVICE_NAME_1            ("HS66xx") // ("TSPP_1")
#define APP_DEVICE_NAME_1_LEN        (sizeof(APP_DEVICE_NAME_1)-1)


#define BLE_APP_CENTRAL_CONNECT_MAX 2 // max BLE_CONNECTION_MAX


#define BLE_APP_SEC           1
#if (BLE_APP_SEC)
#define BLE_APP_SEC_CON       0
#endif

#define BLE_APP_USE_CLIENT_TASK 0      // use tsppc,or **c task
#define BLE_TSPP_COLLECTOR    1


#define BLE_APP_TEST_AUTO_CALLBACK 1  // auto send 1->1024 to tspp server,
#if (BLE_APP_TEST_AUTO_CALLBACK)
#define BLE_APP_TEST_SEND_MAX_LEN 10 //1024
#define BLE_APP_TEST_SEND_LOOP    1//3
#endif
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

