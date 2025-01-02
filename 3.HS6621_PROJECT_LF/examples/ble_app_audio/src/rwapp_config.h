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


/*********************************************************************
 * MACROS
 */

//#define APP_TEST_AUDIO_PLAY
//#define APP_AUDIO_USE_DIG_MIC

#define DEBUG_ADDR "\x01\xBF\x66\x21\x55\x55" // Address just for debug

/// Default Device Name
#define APP_DFLT_DEVICE_NAME            ("Onmicro-Audio")
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
#define BLE_APP_SEC          0

/// WECHAT
#define BLE_APP_WECHAT       0
/// HID Application
#define BLE_APP_HID          0
///HID Device Role
#define BLE_HID_DEVICE       0
///TSPP
#define BLE_TSPP_SERVER      1
// 1: send data 20 Bytes;  
// 2: send data len via app. send data must be < mtu
// 3: send data len via mtu&data len of air
#define BLE_TSPP_SEND_TYPE   2
#define BLE_APP_TSPPS_CONNECT_MAX 1
#define APP_NOTIFY_TIME_INTERVAL  1 // 10ms
#ifdef APP_TEST_AUDIO_PLAY
#define BLE_TSPPS_FIFO_BUF_SIZE   (1024*2)
#else
#define BLE_TSPPS_FIFO_BUF_SIZE   (1024*16) // 2,4,8,16,32....
#endif
///ANCS
#define BLE_APP_ANCSC        0

/// enable/disable white list
#define BLE_APP_WHITE_LIST   0
/// enable/disable privacy
#define BLE_APP_PRIVACY      0

/// Security Application
#if (BLE_APP_HID || BLE_APP_ANCSC) && !BLE_APP_SEC
#undef BLE_APP_SEC
#define BLE_APP_SEC          1
#endif
//#define APP_AUDIO_USE_DIG_MIC

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

