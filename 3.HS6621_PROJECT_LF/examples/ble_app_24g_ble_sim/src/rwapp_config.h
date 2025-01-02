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
/// when role is central. connect device count 
#define BLE_APP_CENTRAL_SUPPORT 3 // max BLE_CONNECTION_MAX
    
/// when role is peripheral£¬ can be connected count
#define BLE_APP_TSPPS_CONNECT_MAX 2 // max BLE_CONNECTION_MAX


#define BLE_APP_SEC           1
#if (BLE_APP_SEC)
#define BLE_APP_SEC_CON       1
#endif
#define BLE_TSPP_SERVER       1
// 1: send data 20 Bytes;  
// 2: send data len via app. send data must be < mtu
// 3: send data len via mtu&data len of air
#define BLE_TSPP_SEND_TYPE   3
#define BLE_TSPPS_FIFO_BUF_SIZE   (1024*4) // 2,4,8,16,32....

/// enable/disable white list
#define BLE_APP_WHITE_LIST   0
/// enable/disable privacy
#define BLE_APP_PRIVACY      0

/// Maximal length of the Device Name value
#define APP_DEVICE_NAME_MAX_LEN      (18)

/// Default Device Name
#define APP_DFLT_DEVICE_NAME            ("BLE-TSPP")
#define APP_DFLT_DEVICE_NAME_LEN        (sizeof(APP_DFLT_DEVICE_NAME)-1)

//#define APP_ADV_DATA_UUID       "\x03\x03\x12\x18"
//#define APP_ADV_DATA_UUID_LEN   (4)

#define APP_ADV_DATA_APPEARANCE   "\x03\x19\xC2\x03"
#define APP_ADV_DATA_APPEARANCE_LEN  (4)

#define APP_SCNRSP_DATA         ""
#define APP_SCNRSP_DATA_LEN     (0)

/**
 * Advertising Parameters
 */
/// Advertising channel map - 37, 38, 39
#define APP_ADV_CHMAP           (0x07)
/// Advertising minimum interval - 40ms (64*0.625ms)
#define APP_ADV_INT_MIN         (64)
/// Advertising maximum interval - 40ms (64*0.625ms)
#define APP_ADV_INT_MAX         (64)
/// Fast advertising interval
#define APP_ADV_FAST_INT        (32)


// --------------------------central----------------------------------
#if (BLE_APP_CENTRAL_SUPPORT)
#define APP_SCAN_DEVICE_NAME            ("HS66xx")
#define APP_SCAN_DEVICE_NAME_LEN        (sizeof(APP_SCAN_DEVICE_NAME)-1)
#endif

// IO PIN {{{
#ifdef CONFIG_ASIC
#define PIN_WAKEUP_0                    2   // UART-RTS@EVB
#define PIN_WAKEUP_1                    24  // KEY1@EVB
#define PIN_LED_0                       8
#define PIN_LED_1                       9
#define PIN_LED_2                       10
#define PIN_LED_3                       25
#define PIN_LED_4                       14
#define PIN_LED_5                       14
#define PIN_LED_6                       12
#define PIN_LED_7                       13
#define PIN_UART0_TX                    5
#define PIN_UART1_TX                    7
#define PIN_UART1_RX                    6
#else
#define PIN_WAKEUP_0                    3  // GPIO4@FPGA-PeripheralBoard
#define PIN_WAKEUP_1                    4  // KEY4@FPGA-PeripheralBoard (KEY2:RESET)

#define PIN_UART0_TX                    13
#define PIN_UART1_TX                    5
#define PIN_UART1_RX                    6
#endif
// }}}

#define HCI_UART_BAUDRATE               115200
#define DEBUG_UART_BAUDRATE             115200

/// DFU
#define BLE_APP_ONMICRO_DFU   1

#if BLE_APP_ONMICRO_DFU
/// the dfu task id
#define TASK_ID_ONMICRO_DFU (TASK_ID_USER)
#endif // BLE_APP_ONMICRO_DFU


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

