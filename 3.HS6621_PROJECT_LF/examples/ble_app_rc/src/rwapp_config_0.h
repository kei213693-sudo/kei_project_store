/**
 * @file rwapp_config_0.h
 * @brief
 * @date Wed, Sep  5, 2018  5:44:25 PM
 * @author liqiang
 *
 * @addtogroup APP_EXAMPLES_WATCH_CONFIG_0 rwapp_config_0.h
 * @ingroup BLE_APP_RC
 * @brief the remote control config
 * @details
 *
 * @{
 */

#ifndef __RWAPP_CONFIG_0_H__
#define __RWAPP_CONFIG_0_H__

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
/* ----------------------------------- customer Module ----------------------------------- */
/// Default Device Name
#define APP_DFLT_DEVICE_NAME            ("Onmicro-RC")

/// Default Device Name Length
#define APP_DFLT_DEVICE_NAME_LEN        (sizeof(APP_DFLT_DEVICE_NAME)-1)

/// uuid in adv
#define APP_ADV_DATA_MANU       "\x04\xFF\x01\xBF\x00"
/// uuid len
#define APP_ADV_DATA_MANU_LEN        (5)

/// appearance in adv
#define APP_ADV_DATA_APPEARANCE   "\x03\x19\x80\x01"
/// appearance len
#define APP_ADV_DATA_APPEARANCE_LEN         (4)

/// scan resposne data
#define APP_SCNRSP_DATA         "\x03\x03\xE7\xFE\x03\x03\x12\x18\x11\x07\xd0\x00\x2d\x12\x1e\x4b\x0f\xa4\x99\x4e\xce\xb5\x31\xf4\x05\x79"
/// scan resposne data len
#define APP_SCNRSP_DATA_LEN     (26)
/// the adv mode
#define BLE_APP_ADV_DISC_MODE GAPM_ADV_MODE_LIM_DISC
/// adv tx power -127-20
#define BLE_APP_ADV_TX_POWER 20
/// enable/disable security connect
#define BLE_APP_SEC          1

/// enable/disable white list
#define BLE_APP_WHITE_LIST   0
/// enable/disable privacy
#define BLE_APP_PRIVACY      0

/// WECHAT
#define BLE_APP_WECHAT       0
/// HID Application
#define BLE_APP_HID          1
#if (BLE_APP_HID)
///HID Device Role
#define BLE_HID_DEVICE       1
#endif

/// dis application
#define BLE_APP_DIS          1
#if (BLE_APP_DIS)
///Device Information Service Server Role
#define BLE_DIS_SERVER       1

/// Manufacturer Name Value
#define APP_DIS_MANUFACTURER_NAME       ("OnMicro")
/// Manufacturer Name Value size 
#define APP_DIS_MANUFACTURER_NAME_LEN   (sizeof(APP_DIS_MANUFACTURER_NAME))

/// Model Number String Value
#define APP_DIS_MODEL_NB_STR            ("Model Nbr 0.2")
/// Model Number String Value len
#define APP_DIS_MODEL_NB_STR_LEN        (sizeof(APP_DIS_MODEL_NB_STR))

/// Serial Number
#define APP_DIS_SERIAL_NB_STR           ("SerialNum0.2")
/// Serial Number len
#define APP_DIS_SERIAL_NB_STR_LEN       (sizeof(APP_DIS_SERIAL_NB_STR))

/// Firmware Revision
#define APP_DIS_FIRM_REV_STR            ("6621A3")
/// Firmware Revision len
#define APP_DIS_FIRM_REV_STR_LEN        (5)

/// System ID Value - LSB -> MSB
#define APP_DIS_SYSTEM_ID               ("\x00\x00\x01\x02\x00\x03\x04\x05")
/// System ID Value len
#define APP_DIS_SYSTEM_ID_LEN           (8)

/// Hardware Revision String
#define APP_DIS_HARD_REV_STR           ("1.3")
/// Hardware Revision String len
#define APP_DIS_HARD_REV_STR_LEN       (sizeof(APP_DIS_HARD_REV_STR))

/// Software Revision String
#define APP_DIS_SW_REV_STR              ("1.0.1.2")
/// Software Revision String len
#define APP_DIS_SW_REV_STR_LEN          (sizeof(APP_DIS_SW_REV_STR))

/// IEEE
#define APP_DIS_IEEE                    ("\x52\x54\x4B\x42\x65\x65\x49\x45\x45\x45\x44\x61\x74\x61\x6C\x69\x73\x74\x00")
/// IEEE len
#define APP_DIS_IEEE_LEN                (19)

/**
 * PNP ID Value - LSB -> MSB
 *      Vendor ID Source : 0x02 (USB Implementer’s Forum assigned Vendor ID value)
 *      Vendor ID : 0x045E      (Microsoft Corp)
 *      Product ID : 0x0040
 *      Product Version : 0x0300
 */
#define APP_DIS_PNP_ID               ("\x01\xBF\x01\xB8\x32\x79\x49")
/// dis pnp id len
#define APP_DIS_PNP_ID_LEN           (7)
#endif
/// the dis features
#define APP_DIS_FEATURES             (DIS_ALL_FEAT_SUP)

/// battery appliaction
#define BLE_APP_BATT         1
#if (BLE_APP_BATT)
///Battery Service Server Role
#define BLE_BATT_SERVER      1
#endif

///TSPP
#define BLE_TSPP_SERVER      1
// 1: send data 20 Bytes;  
// 2: send data len via app. send data must be < mtu
// 3: send data len via mtu&data len of air
#define BLE_TSPP_SEND_TYPE   3
/// tspp connect max
#define BLE_APP_TSPPS_CONNECT_MAX 1
/// tspp notify interval when use timer
#define APP_NOTIFY_TIME_INTERVAL  1 // 10ms
/// tspp fifo buffer size
#define BLE_TSPPS_FIFO_BUF_SIZE   (1024*4) // 2,4,8,16,32....

///ATVV enable/disable
#define BLE_ATVV_SERVER      0
/// ATVV connect max
#define BLE_APP_ATVVS_CONNECT_MAX 1
/// ATVV fifo buffer size
#define BLE_ATVVS_FIFO_BUF_SIZE   (1024*16) // 2,4,8,16,32....

///ANCS
#define BLE_APP_ANCSC        0
/// DFU
#define BLE_APP_ONMICRO_DFU   1

#if BLE_APP_ONMICRO_DFU
/// the dfu task id
#define TASK_ID_ONMICRO_DFU (TASK_ID_USER)
#endif // BLE_APP_ONMICRO_DFU

/// Security Application
#if (BLE_APP_HID || BLE_APP_ANCSC) && !BLE_APP_SEC
#undef BLE_APP_SEC
/// security enable/disable
#define BLE_APP_SEC          1
#endif

//#define APP_AUDIO_USE_DIG_MIC

/* ----------------------------------- UART Module ----------------------------------- */
///  UART Configuration
#define CONFIG_UART_ENABLE              1
/// uart tx pin
#define PIN_UART0_TX                    7 //5
/// uart rx pin
#define PIN_UART0_RX                    9 //6
/// uart baudrate
#define DEFALUT_UART_BAUDRATE           115200

/* ----------------------------------- LED Module ----------------------------------- */
///  LED Configuration
#define CONFIG_LED_ENABLE               1
/// led pin
#define PIN_LED_0                       8   // PA8@EVB

/// <0> low <~0> high
#define CONFIG_LED_LIGHT                (~0)
/* ----------------------------------- KEY Module ----------------------------------- */
///  Key Configuration
#define CONFIG_KEY_ENABLE               1

/// Keyboard Row Count <0-8>
#define KEY_ROW_COUNT                   3
/// Keyboard Column Count <0-8>
#define KEY_COL_COUNT                   4

/// Keyboard Rows Pin Set
///  Row 0 Pin
///  <0xFF=>Disable
///  <0=>GPIO0   <1=>GPIO1   <2=>GPIO2   <3=>GPIO3   <4=>GPIO4   <5=>GPIO5   <6=>GPIO6   <7=>GPIO7
///  <8=>GPIO8   <9=>GPIO9   <10=>GPIO10 <11=>GPIO11 <12=>GPIO12 <13=>GPIO13 <14=>GPIO14 <15=>GPIO15
///  <16=>GPIO16 <17=>GPIO17 <18=>GPIO18 <19=>GPIO19 <20=>GPIO20 <21=>GPIO21 <22=>GPIO22 <23=>GPIO23
///  <24=>GPIO24 <25=>GPIO25 <26=>GPIO26 <27=>GPIO27 <28=>GPIO28 <29=>GPIO29 <30=>GPIO30 <31=>GPIO31
/**@brief Matrix Keyboard: Row 0 Pin */
#define PIN_KEY_ROW_0           26          /**< key row 0*/
#define PIN_KEY_ROW_1           27          /**< key row 1*/
#define PIN_KEY_ROW_2           28          /**< key row 2*/
#define PIN_KEY_ROW_3           0xFF        /**< key row 3*/
#define PIN_KEY_ROW_4           0xFF        /**< key row 4*/
#define PIN_KEY_ROW_5           0xFF        /**< key row 5*/
#define PIN_KEY_ROW_6           0xFF        /**< key row 6*/
#define PIN_KEY_ROW_7           0xFF        /**< key row 7*/

/**@brief Matrix Keyboard: Column 0 Pin */
#define PIN_KEY_COL_0           29      /**< key col 0*/
#define PIN_KEY_COL_1           30      /**< key col 1*/
#define PIN_KEY_COL_2           18      /**< key col 2*/
#define PIN_KEY_COL_3           19      /**< key col 3*/
#define PIN_KEY_COL_4           0xFF    /**< key col 4*/
#define PIN_KEY_COL_5           0xFF    /**< key col 5*/
#define PIN_KEY_COL_6           0xFF    /**< key col 6*/
#define PIN_KEY_COL_7           0xFF    /**< key col 7*/

/* ----------------------------------- Audio Module ----------------------------------- */
///  Audio Configuration
#define CONFIG_AUDIO_ENABLE         1

///  MIC Positive Pin
///  <0xFF=>Disable
///  <0=>MIC_P   <2=>GPIO2   <3=>GPIO3  <7=>GPIO7   <8=>GPIO8 
#define PIN_MIC_P                   0 //3

///  MIC Nagative Pin
///  <0xFF=>Disable
///  <0xFF=>Disable
///  <0=>MIC_N   <2=>GPIO2   <3=>GPIO3  <7=>GPIO7   <8=>GPIO8 
#define PIN_MIC_N                   0 //0xFF

///  DMIC Clocd
///  <0xFF=>Disable
///  <0=>GPIO0   <1=>GPIO1   <2=>GPIO2   <3=>GPIO3   <4=>GPIO4   <5=>GPIO5   <6=>GPIO6   <7=>GPIO7
///  <8=>GPIO8   <9=>GPIO9   <10=>GPIO10 <11=>GPIO11 <12=>GPIO12 <13=>GPIO13 <14=>GPIO14 <15=>GPIO15
///  <16=>GPIO16 <17=>GPIO17 <18=>GPIO18 <19=>GPIO19 <20=>GPIO20 <21=>GPIO21 <22=>GPIO22 <23=>GPIO23
///  <24=>GPIO24 <25=>GPIO25 <26=>GPIO26 <27=>GPIO27 <28=>GPIO28 <29=>GPIO29 <30=>GPIO30 <31=>GPIO31
#define PIN_DMIC_CLOCK                  0xFF

///  DMIC Data
///  <0xFF=>Disable
///  <0=>GPIO0   <1=>GPIO1   <2=>GPIO2   <3=>GPIO3   <4=>GPIO4   <5=>GPIO5   <6=>GPIO6   <7=>GPIO7
///  <8=>GPIO8   <9=>GPIO9   <10=>GPIO10 <11=>GPIO11 <12=>GPIO12 <13=>GPIO13 <14=>GPIO14 <15=>GPIO15
///  <16=>GPIO16 <17=>GPIO17 <18=>GPIO18 <19=>GPIO19 <20=>GPIO20 <21=>GPIO21 <22=>GPIO22 <23=>GPIO23
///  <24=>GPIO24 <25=>GPIO25 <26=>GPIO26 <27=>GPIO27 <28=>GPIO28 <29=>GPIO29 <30=>GPIO30 <31=>GPIO31
#define PIN_DMIC_DATA                   0xFF

///  Audio Control Pin
///  <0xFF=>Disable
///  <0=>GPIO0   <1=>GPIO1   <2=>GPIO2   <3=>GPIO3   <4=>GPIO4   <5=>GPIO5   <6=>GPIO6   <7=>GPIO7
///  <8=>GPIO8   <9=>GPIO9   <10=>GPIO10 <11=>GPIO11 <12=>GPIO12 <13=>GPIO13 <14=>GPIO14 <15=>GPIO15
///  <16=>GPIO16 <17=>GPIO17 <18=>GPIO18 <19=>GPIO19 <20=>GPIO20 <21=>GPIO21 <22=>GPIO22 <23=>GPIO23
///  <24=>GPIO24 <25=>GPIO25 <26=>GPIO26 <27=>GPIO27 <28=>GPIO28 <29=>GPIO29 <30=>GPIO30 <31=>GPIO31
#define PIN_AUDIO_CTRL              7

/// audio codec select
/// <0xFF=>Disable
/// <0> sbc <1> MI <2>adpcm
#define CONFIG_AUDIO_CODEC_SEL    0

/// select send audio path
/// <0xFF> disable
/// <0> tspp <1> hid <2>atvv
#define CONFIG_AUDIO_SEND_PATH 0

/* ----------------------------------- IR Module ----------------------------------- */
///  IR Configuration
#define CONFIG_IR_ENABLE 1

///  IR Control Pin
///  <0xFF=>Disable
///  <0=>GPIO0   <1=>GPIO1   <2=>GPIO2   <3=>GPIO3   <4=>GPIO4   <5=>GPIO5   <6=>GPIO6   <7=>GPIO7
///  <8=>GPIO8   <9=>GPIO9   <10=>GPIO10 <11=>GPIO11 <12=>GPIO12 <13=>GPIO13 <14=>GPIO14 <15=>GPIO15
///  <16=>GPIO16 <17=>GPIO17 <18=>GPIO18 <19=>GPIO19 <20=>GPIO20 <21=>GPIO21 <22=>GPIO22 <23=>GPIO23
///  <24=>GPIO24 <25=>GPIO25 <26=>GPIO26 <27=>GPIO27 <28=>GPIO28 <29=>GPIO29 <30=>GPIO30 <31=>GPIO31
#define PIN_IR_CONTROL  9

/// IR Format
/// uPD6121 is NEC
/// <1=>TC9012 <2=>uPD6121
#define IR_FORMAT 1

/// IR User Code <0-0xFFFF>
#define IR_USER_CODE 0x1818


/* ----------------------------------- Battery Monitor Module ----------------------------------- */
///  Battery Monitor Configuration
#define CONFIG_BATTERY_MONITOR_ENABLE 0


/**
 * Advertising Parameters
 */
#define APP_ADV_FAST_INTERVAL     0x0032   /**< Fast advertising interval (in units of 0.625 ms. This value corresponds to 25 ms.). */
#define APP_ADV_SLOW_INTERVAL     0x01F4   /**< Slow advertising interval (in units of 0.625 ms. This value corrsponds to 2 seconds). */

#define APP_ADV_FAST_TIMEOUT      30      /**< The duration of the fast advertising period (in seconds). max 655, or 0 is no timeout*/
#define APP_ADV_SLOW_TIMEOUT      60      /**< The duration of the slow advertising period (in seconds). max 655. or 0 is no timeout*/

#define APP_ADV_DIRECTED_TIMEOUT      5       /**< The duration of the fast advertising period (in seconds). mas 655. or 0 is high duty cycle*/

/// connection interval min
#define BLE_CONN_INTERVAL_MIN       6       //unit: 1.25 ms
/// connection interval max
#define BLE_CONN_INTERVAL_MAX       16      //unit: 1.25 ms
/// connection latency
#define BLE_CONN_LATENCY            133
/// connection timeout
#define BLE_CONN_TIMEOUT            400     //unit: 10 ms
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

