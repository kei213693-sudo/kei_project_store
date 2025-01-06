/**
 * @file rwapp_config.h
 * @brief
 * @date Wed, Sep  5, 2018  5:44:25 PM
 * @author liqiang
 *
 * @addtogroup APP_EXAMPLES_WATCH_CONFIG rwapp_config.h
 * @ingroup BLE_APP_WATCH
 * @brief the project config
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
#include "rwip_config.h" // RW SW configuration
#include "rwip_task.h"      // Task definitions
#include "ke_task.h"        // Kernel Task
#include "rwip.h"      // RW SW initialization


/*********************************************************************
 * MACROS
 */

#define DEBUG_ADDR "\xCC\xCC\xCC\xCC\xCC\xCC" // Address just for debug

/// Default Device Name
#define APP_DFLT_DEVICE_NAME            ("LF_")//广播名称前缀，后面还要加上sn码，例如“LF_000000007”
/// Default Device Name Length
#define APP_DFLT_DEVICE_NAME_LEN        (sizeof(APP_DFLT_DEVICE_NAME)-1)

/// uuid in adv
#define APP_ADV_DATA_MANU       "\x09\xFF\xF0\xFF\x01\x02\x03\x04\x05\x06"
/// uuid len
#define APP_ADV_DATA_MANU_LEN   (10)

/// appearance in adv
#define APP_ADV_DATA_APPEARANCE   "\x03\x19\xC2\x03"
/// appearance len
#define APP_ADV_DATA_APPEARANCE_LEN  (4)

/// scan resposne data
#define APP_SCNRSP_DATA         "\x03\x03\xE7\xFE\x03\x03\x12\x18\x11\x07\xd0\x00\x2d\x12\x1e\x4b\x0f\xa4\x99\x4e\xce\xb5\x31\xf4\x05\x79"
/// scan resposne data len
#define APP_SCNRSP_DATA_LEN     (26)



/// enable/disable security connect
#define BLE_APP_SEC          1//使能配对

/// WECHAT
#define BLE_APP_WECHAT       1
/// HID Application
#define BLE_APP_HID          1
///HID Device Role
#define BLE_HID_DEVICE       1
///TSPP
#define BLE_TSPP_SERVER      1
// 1: send data 20 Bytes;  
// 2: send data len via app. send data must be < mtu
// 3: send data len via mtu&data len of air
#define BLE_TSPP_SEND_TYPE   2
/// tspp connect max count
#define BLE_APP_TSPPS_CONNECT_MAX 1
/// tspp notify interval, if use timer
#define APP_NOTIFY_TIME_INTERVAL  1 // 10ms
/// tspp buffer size for send data
#define BLE_TSPPS_FIFO_BUF_SIZE   (1024*4) // 2,4,8,16,32....
///ANCS
#define BLE_APP_ANCSC        1
/// DFU
#define BLE_APP_ONMICRO_DFU   1

/// enable/disable white list
#define BLE_APP_WHITE_LIST   0//失能白名单机制
/// enable/disable privacy
#define BLE_APP_PRIVACY      0//失能随机地址

#if BLE_APP_ONMICRO_DFU
/// the dfu task id
#define TASK_ID_ONMICRO_DFU (TASK_ID_USER)
#endif // BLE_APP_ONMICRO_DFU

/// Security Application
#if (BLE_APP_HID || BLE_APP_ANCSC) && !BLE_APP_SEC
#undef BLE_APP_SEC
/// use security
#define BLE_APP_SEC          1
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

