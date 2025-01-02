/**
 ****************************************************************************************
 *
 * @file app.c
 *
 * @brief Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_SIMPLE_SERVER_APP_C app.c
 * @ingroup APP_SIMPLE_SERVER
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRESENT)

#include <string.h>

#include "app_task.h"                // Application task Definition
#include "app.h"                     // Application Definition
#include "gap.h"                     // GAP Definition
#include "gapm_task.h"               // GAP Manager Task API
#include "gapc_task.h"               // GAP Controller Task API

#include "co.h"
#include "co_utils.h"
#include "co_bt.h"                   // Common BT Definition
#include "co_math.h"                 // Common Maths Definition

#if (BLE_APP_SEC)
#include "app_sec.h"                 // Application security Definition
#endif // (BLE_APP_SEC)

#if (NVDS_SUPPORT)
#include "nvds.h"                    // NVDS Definitions
#endif //(NVDS_SUPPORT)
#include "prf.h"
#if (BLE_APP_WECHAT)
#include "app_wechat.h"
#include "wechat.h"
#endif

#if (BLE_APP_DIS)
#include "app_dis.h"                 // Device Information Service Application Definitions
#include "diss.h"
#endif
#if (BLE_APP_BATT)
#include "app_batt.h"                // Battery Application Definitions
#include "bass.h"
#endif
#if (BLE_APP_HID)
#include "app_hid.h"                 // HID Application Definitions
#include "hogpd.h"
#endif
#if (BLE_TSPP_SERVER)
#include "app_tspp_server.h"
#endif
#if (BLE_ATVV_SERVER)
#include "app_atvv.h"
#endif

#if (BLE_APP_ANCSC)
#include "ancsc.h"
#include "app_ancsc.h"
#endif
#if BLE_APP_ONMICRO_DFU
#include "onmicro_dfu.h"
#include "app_onmicro_dfu.h"
#endif //BLE_APP_ONMICRO_DFU


/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/// typdef appm_add_svc_func_t
typedef void (*appm_add_svc_func_t)(void);

/*
 * ENUMERATIONS
 ****************************************************************************************
 */
#if 0//(BLE_APP_HID)
/// HID Keyboard Report Descriptor
static const uint8_t app_hid_report_map[] =
{
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,        // Usage (Keyboard)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x01,        //   Report ID (1)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
    0x19, 0xE0,        //   Usage Minimum (0xE0)
    0x29, 0xE7,        //   Usage Maximum (0xE7)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x08,        //   Report Count (8)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x08,        //   Report Size (8)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x05,        //   Report Count (5)
    0x75, 0x01,        //   Report Size (1)
    0x05, 0x08,        //   Usage Page (LEDs)
    0x19, 0x01,        //   Usage Minimum (Num Lock)
    0x29, 0x05,        //   Usage Maximum (Kana)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x03,        //   Report Size (3)
    0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x06,        //   Report Count (6)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x28,        //   Logical Minimum (40)
    0x25, 0xFE,        //   Logical Maximum (-2)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
    0x19, 0x28,        //   Usage Minimum (0x28)
    0x29, 0xFE,        //   Usage Maximum (0xFE)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x0C,        //   Usage Page (Consumer)
    0x09, 0x01,        //   Usage (Consumer Control)
    0xA1, 0x01,        //   Collection (Application)
    0x85, 0xF1,        //     Report ID (-15)
    0x15, 0x00,        //     Logical Minimum (0)
    0x25, 0x01,        //     Logical Maximum (1)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x18,        //     Report Count (24)
    0x09, 0xB5,        //     Usage (Scan Next Track)
    0x09, 0xB6,        //     Usage (Scan Previous Track)
    0x09, 0xB7,        //     Usage (Stop)
    0x09, 0xCD,        //     Usage (Play/Pause)
    0x09, 0xE2,        //     Usage (Mute)
    0x09, 0xE5,        //     Usage (Bass Boost)
    0x09, 0xE7,        //     Usage (Loudness)
    0x09, 0xE9,        //     Usage (Volume Increment)
    0x09, 0xEA,        //     Usage (Volume Decrement)
    0x0A, 0x52, 0x01,  //     Usage (Bass Increment)
    0x0A, 0x53, 0x01,  //     Usage (Bass Decrement)
    0x0A, 0x54, 0x01,  //     Usage (Treble Increment)
    0x0A, 0x55, 0x01,  //     Usage (Treble Decrement)
    0x0A, 0x83, 0x01,  //     Usage (AL Consumer Control Configuration)
    0x0A, 0x8A, 0x01,  //     Usage (AL Email Reader)
    0x0A, 0x92, 0x01,  //     Usage (AL Calculator)
    0x0A, 0x94, 0x01,  //     Usage (AL Local Machine Browser)
    0x0A, 0x21, 0x02,  //     Usage (AC Search)
    0x0A, 0x23, 0x02,  //     Usage (AC Home)
    0x0A, 0x24, 0x02,  //     Usage (AC Back)
    0x0A, 0x25, 0x02,  //     Usage (AC Forward)
    0x0A, 0x26, 0x02,  //     Usage (AC Stop)
    0x0A, 0x27, 0x02,  //     Usage (AC Refresh)
    0x0A, 0x2A, 0x02,  //     Usage (AC Bookmarks)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xC0,              // End Collection

    0x06, 0x00, 0xFF,      // Usage Page (Vendor Defined 0xFF00)
    0x09, 0x00,            // Usage (0x00)
    0xA1, 0x01,            // Collection (Application)
    0xA1, 0x02,            //   Collection (Logical)
    0x85, 0xAA,            //     Report ID
    0x09, 0x01,            //     Usage (0x01)
    0x15, 0x00,            //     Logical Minimum (0)
    0x25, 0xFF,            //     Logical Maximum (255)
    0x75, 0x08,            //     Report Size (8)
    0x95, 0x3C,            //     Report Count (19)
    0x81, 0x22,            //     Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0xC0,                  //   End Collection
    0xC0,                  // End Collection
    
    // 147 bytes172
};
#endif
/// List of service to add in the database
enum appm_svc_list
{
    #if (BLE_APP_SIMPLE_SERVER)
    APPM_SVC_SIMPLE_SERVER,
    #endif // BLE_APP_SIMPLE_SERVER
    #if (BLE_APP_DIS)
    APPM_SVC_DIS,
    #endif //(BLE_APP_DIS)
    #if (BLE_APP_BATT)
    APPM_SVC_BATT,
    #endif //(BLE_APP_BATT)
    #if (BLE_TSPP_SERVER)
    APPM_SVC_TSPP_SERVER,
    #endif //(BLE_TSPP_SERVER)
    #if (BLE_ATVV_SERVER)
    APPM_SVC_ATVV_SERVER,
    #endif //(BLE_ATVV_SERVER)
    #if (BLE_APP_HID)
    APPM_SVC_HIDS,
    #endif //(BLE_APP_HID)
    #if BLE_APP_WECHAT
    APPM_SVC_WECHAT,
    #endif
    #if (BLE_APP_ANCSC)
    APPM_SVC_ANCSC,
    #endif //(BLE_APP_ANCSC)
    #if (BLE_APP_ONMICRO_DFU)
    APPM_SVC_ONMICRO_DFU,
    #endif //(BLE_APP_ONMICRO_DFU)

    APPM_SVC_LIST_STOP,
};

/*
 * LOCAL VARIABLES DEFINITIONS
 ****************************************************************************************
 */

/// Application Task Descriptor
extern const struct ke_task_desc TASK_DESC_APP;

/// List of functions used to create the database
static const appm_add_svc_func_t appm_add_svc_func_list[APPM_SVC_LIST_STOP] =
{
    #if (BLE_APP_SIMPLE_SERVER)
    (appm_add_svc_func_t)app_simple_server_add_service,
    #endif // BLE_APP_SIMPLE_SERVER
    #if (BLE_APP_DIS)
    (appm_add_svc_func_t)app_dis_add_dis,
    #endif //(BLE_APP_DIS)
    #if (BLE_APP_BATT)
    (appm_add_svc_func_t)app_batt_add_bas,
    #endif //(BLE_APP_BATT)
    #if (BLE_TSPP_SERVER)
    (appm_add_svc_func_t)app_tspp_server_add_service,
    #endif //(BLE_TSPP_SERVER)
    #if (BLE_ATVV_SERVER)
    (appm_add_svc_func_t)app_atvv_server_add_service,
    #endif //(BLE_ATVV_SERVER)
    #if (BLE_APP_HID)
    (appm_add_svc_func_t)app_hid_add_hids,
    #endif //(BLE_APP_HID)
    #if BLE_APP_WECHAT
    (appm_add_svc_func_t)app_wechat_add_wechat,
    #endif
    #if (BLE_APP_ANCSC)
    (appm_add_svc_func_t)app_ancs_add_ancsc,
    #endif //(BLE_APP_ANCSC)
    #if (BLE_APP_ONMICRO_DFU)
    (appm_add_svc_func_t)app_onmicro_dfu_add_service,
    #endif //(BLE_APP_ONMICRO_DFU)
};


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Application Environment Structure
struct app_env_tag app_env;
extern const struct prf_task_cbs* tspp_server_prf_itf_get(void);
extern const struct prf_task_cbs* atvv_server_prf_itf_get(void);
/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void appm_opentv_advdata_set(void)
{
    uint8_t advdata_len = 28;
    
    uint8_t advdata[28] = {0x03, 0xff, 0x00, 0x01, 0x06, 0x08, 0x4d, 0x49, 0x20, 0x52, 0x43, 0x03, 0x03, 
                           0x12, 0x18, 0x04, 0x0d, 0x04, 0x05, 0x00, 0x02, 0x0a, 0x00, 0x04, 0xfe, 0x00, 0x00, 0x00};

    #if (NVDS_SUPPORT)
    struct app_sec_conn_info_tag info;
    nvds_tag_len_t length = sizeof(struct app_sec_conn_info_tag);
    uint8_t index = 0;
    uint8_t nvds_len = NVDS_TAG_BLE_LINK_KEY_LAST-NVDS_TAG_BLE_LINK_KEY_FIRST;

    // Get bond status from NVDS
    for(index =0; index <= nvds_len; index++)
    {
        if (nvds_get(NVDS_TAG_BLE_LINK_KEY_FIRST+index, &length, (uint8_t *)&info) != NVDS_OK)
        {
            break;
        }
        else {
            advdata[25] = info.id_addr.addr.addr[0];
            advdata[26] = info.id_addr.addr.addr[1];
            advdata[27] = info.id_addr.addr.addr[2];
            break;
        }
    }
    #endif //(NVDS_SUPPORT)

    appm_adv_set_adv_data(advdata, advdata_len);

    uint8_t srdata_len = 29;
    uint8_t srdata[29] = {0x1c, 0x09, 0xe5, 0xb0, 0x8f, 0xe7, 0xb1, 0xb3, 0xe8, 0x93, 0x9d, 0xe7, 0x89, 0x99, 0xe8, 
                          0xaf, 0xad, 0xe9, 0x9f, 0xb3, 0xe9, 0x81, 0xa5, 0xe6, 0x8e, 0xa7, 0xe5, 0x99, 0xa8};

    appm_adv_set_res_data(srdata, srdata_len);
}
#if (APP_CUSTOMER_ID==1) 
void appm_pair_advdata_set(void)
{
    uint8_t advdata_len = 23;
    uint8_t advdata[23] = {0x03, 0xff, 0x00, 0x00, 0x06, 0x08, 0x4d, 0x49, 0x20, 0x52, 
                           0x43, 0x03, 0x03, 0x12, 0x18, 0x04, 0x0d, 0x04, 0x05, 0x00, 0x02, 0x0a, 0x00};
    appm_adv_set_adv_data(advdata, advdata_len);
    uint8_t srdata_len = 29;
    uint8_t srdata[29] = {0x1c, 0x09, 0xe5, 0xb0, 0x8f, 0xe7, 0xb1, 0xb3, 0xe8, 0x93, 0x9d, 0xe7, 0x89, 0x99, 0xe8, 
                          0xaf, 0xad, 0xe9, 0x9f, 0xb3, 0xe9, 0x81, 0xa5, 0xe6, 0x8e, 0xa7, 0xe5, 0x99, 0xa8};
    appm_adv_set_res_data(srdata, srdata_len);
}
#endif
#if (APP_CUSTOMER_ID==2 || APP_CUSTOMER_ID==3)
void appm_pair_advdata_set(void)
{
    uint8_t advdata[] = {
//        0x02, 0x01, 0x05,
        0x03, 0x19, 0x80, 0x01,
        0x03, 0x02, 0x12, 0x18,
        0x13, 0xFF, 0x46, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x43, 0x52, 0x4B, 0x54, 0x4D,
    };
    appm_adv_set_adv_data(advdata, sizeof(advdata));
    
    uint8_t srdata[] = {
        0x0E, 0x09, 'T', 'V', ' ', 'B', 'L', 'E', ' ', 'R', 'e', 'm', 'o', 't', 'e',
    };
    appm_adv_set_res_data(srdata, sizeof(srdata));
}
#endif
#if (APP_CUSTOMER_ID==0)
static void appm_build_adv_data(uint16_t max_length, uint16_t *p_length, uint8_t *p_buf)
{
    // Remaining Length
    uint8_t rem_len = max_length;
    // Set appearance
    memcpy(p_buf, APP_ADV_DATA_APPEARANCE, APP_ADV_DATA_APPEARANCE_LEN);
    *p_length += APP_ADV_DATA_APPEARANCE_LEN;
    p_buf += APP_ADV_DATA_APPEARANCE_LEN;

    // Set list of UUIDs
    memcpy(p_buf, APP_ADV_DATA_MANU, APP_ADV_DATA_MANU_LEN);
    *p_length += APP_ADV_DATA_MANU_LEN;
    p_buf += APP_ADV_DATA_MANU_LEN;

    // Sanity check
    //ASSERT_ERR(rem_len< max_length);

    // Get remaining space in the Advertising Data - 2 bytes are used for name length/flag
    rem_len -= *p_length;

    // Check if additional data can be added to the Advertising data - 2 bytes needed for type and length
    if (rem_len > 2)
    {
        uint8_t dev_name_length = co_min(app_env.dev_name_len, (rem_len - 2));

        // Device name length
        *p_buf = dev_name_length + 1;
        // Device name flag (check if device name is complete or not)
        *(p_buf + 1) = (dev_name_length == app_env.dev_name_len) ? '\x09' : '\x08';
        // Copy device name
        memcpy(p_buf + 2, app_env.dev_name, dev_name_length);

        // Update advertising data length
        *p_length += (dev_name_length + 2);
    }
}
#endif


static void appm_advertising_init(void)
{
    struct app_adv_modes_config_tag adv_config;
    #if(BLE_APP_WHITE_LIST)
    adv_config.whitelist_enabled = true;
    #else
    adv_config.whitelist_enabled = false;
    #endif
    adv_config.directed_enabled = true;
    adv_config.directed_timeout = APP_ADV_DIRECTED_TIMEOUT;
    adv_config.fast_enabled = true;
    adv_config.fast_interval = APP_ADV_FAST_INTERVAL;
    adv_config.fast_timeout = APP_ADV_FAST_TIMEOUT;
    adv_config.slow_enabled = true;
    adv_config.slow_interval = APP_ADV_SLOW_INTERVAL;
    adv_config.slow_timeout = APP_ADV_SLOW_TIMEOUT;

    appm_adv_init(NULL, &adv_config);
#if (APP_CUSTOMER_ID != 0)
    appm_pair_advdata_set();
#else
    uint8_t default_adv_data[ADV_DATA_LEN - 3];
    uint16_t adv_len = 0;
    appm_build_adv_data(ADV_DATA_LEN - 3, &adv_len, (uint8_t*)default_adv_data);
    appm_adv_set_adv_data((uint8_t*)default_adv_data, adv_len);

    uint8_t res_data[APP_ADV_MAX_LEN];
    memcpy(res_data, APP_SCNRSP_DATA, APP_SCNRSP_DATA_LEN);
    appm_adv_set_res_data(res_data, APP_SCNRSP_DATA_LEN);
#endif
}

static void appm_send_gapm_reset_cmd(void)
{
    // Reset the stack
    struct gapm_reset_cmd *p_cmd = KE_MSG_ALLOC(GAPM_RESET_CMD,
                                                TASK_GAPM, TASK_APP,
                                                gapm_reset_cmd);

    p_cmd->operation = GAPM_RESET;

    ke_msg_send(p_cmd);
}

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void appm_init(void)
{
    // Reset the application manager environment
    memset(&app_env, 0, sizeof(app_env));

    // Create APP task
    ke_task_create(TASK_APP, &TASK_DESC_APP);

    // Initialize Task state
    ke_state_set(TASK_APP, APPM_INIT);

    // Get the Device Name to add in the Advertising Data (Default one or NVDS one)
    #if (NVDS_SUPPORT)
    app_env.dev_name_len = APP_DEVICE_NAME_MAX_LEN;
    if (nvds_get(NVDS_TAG_DEVICE_NAME, &(app_env.dev_name_len), app_env.dev_name) != NVDS_OK)
    #endif //(NVDS_SUPPORT)
    {
        // Get default Device Name (No name if not enough space)
        memcpy(app_env.dev_name, APP_DFLT_DEVICE_NAME, APP_DFLT_DEVICE_NAME_LEN);
        app_env.dev_name_len = APP_DFLT_DEVICE_NAME_LEN;

        // TODO update this value per profiles
    }

    #if (NVDS_SUPPORT)
    nvds_tag_len_t len=6;
    if (nvds_get(NVDS_TAG_BD_ADDRESS, &len, &app_env.addr.addr[0]) == NVDS_OK)
    {
    }
    else
    #endif //(NVDS_SUPPORT)
    {
        memcpy(&app_env.addr.addr[0], &co_default_bdaddr,sizeof(co_default_bdaddr));
    }
    #if defined(DEBUG_ADDR)
        memcpy(&app_env.addr.addr[0], DEBUG_ADDR, 6); // !!  JUST FOR DEBUG  !!
    #endif

    /*------------------------------------------------------
     * INITIALIZE ALL MODULES
     *------------------------------------------------------*/

    // load device information:

    #if (BLE_APP_SEC)
    // Security Module
    app_sec_init();
    #endif // (BLE_APP_SEC)

    #if (BLE_APP_DIS)
    // Device Information Module
    app_dis_init();
    #endif //(BLE_APP_DIS)

    #if (BLE_APP_SIMPLE_SERVER)
    // simple_server Module
    app_simple_server_init();
    #endif //(BLE_APP_SIMPLE_SERVER)

    #if (BLE_APP_WECHAT)
    // Device Information Module
    app_wechat_init();
    #endif //(BLE_APP_WECHAT)

    #if (BLE_APP_ANCSC)
    // ANCS Module
    app_ancsc_init();
    #endif //(BLE_APP_ANCSC)
    #if (BLE_APP_HID)
    app_hid_init();
    //app_hid_set_report_map((uint8_t*)(&app_hid_report_map[0]), sizeof(app_hid_report_map));
    #endif //(BLE_APP_HID)
    #if (BLE_ATVV_SERVER)
    // simple_server Module
    app_atvv_server_init();
    #endif //(BLE_ATVV_SERVER)
    // Reset the stack
    appm_send_gapm_reset_cmd();

    // start adversting
    appm_advertising_init();
}

void appm_reg_svc_itf(void)
{
    struct prf_itf_pair itf_pair[] = {
        #if BLE_APP_SIMPLE_SERVER
        { TASK_ID_SIMPLE_SERVER, simple_server_prf_itf_get() },
        #endif
        #if BLE_APP_WECHAT
        { TASK_ID_WECHAT, wechat_prf_itf_get() },
        #endif // BLE_APP_WECHAT
        #if (BLE_APP_DIS)
        { TASK_ID_DISS, diss_prf_itf_get() },
        #endif //(BLE_APP_DIS)
        #if (BLE_APP_BATT)
        { TASK_ID_BASS, bass_prf_itf_get() },
        #endif //(BLE_APP_BATT)
        #if (BLE_APP_HID)
        { TASK_ID_HOGPD, hogpd_prf_itf_get() },
        #endif //(BLE_APP_HID)
        #if (BLE_TSPP_SERVER)
        { TASK_ID_TSPPS, tspp_server_prf_itf_get() },
        #endif //(BLE_TSPP_SERVER)
        #if (BLE_ATVV_SERVER)
        { TASK_ID_ATVVS, atvv_server_prf_itf_get() },
        #endif //(BLE_ATVV_SERVER)
        #if (BLE_APP_ANCSC)
        { TASK_ID_ANCSC, ancsc_prf_itf_get() },
        #endif //(BLE_APP_ANCSC)
    };
    prf_itf_register(itf_pair, sizeof(itf_pair)/sizeof(itf_pair[0]));
}

bool appm_add_svc(void)
{
    // Indicate if more services need to be added in the database
    bool more_svc = false;

    // Check if another should be added in the database
    if (app_env.next_svc != APPM_SVC_LIST_STOP)
    {
        ASSERT_INFO(appm_add_svc_func_list[app_env.next_svc] != NULL, app_env.next_svc, 1);

        // Call the function used to add the required service
        appm_add_svc_func_list[app_env.next_svc]();

        // Select following service to add
        app_env.next_svc++;
        more_svc = true;

        // Go to the create db state
        ke_state_set(TASK_APP, APPM_CREATE_DB);
    }

    return more_svc;
}

void appm_disconnect(uint8_t conidx)
{
    struct gapc_disconnect_cmd *cmd = KE_MSG_ALLOC(GAPC_DISCONNECT_CMD,
            KE_BUILD_ID(TASK_GAPC, conidx), KE_BUILD_ID(TASK_APP, conidx),
            gapc_disconnect_cmd);

    cmd->operation = GAPC_DISCONNECT;
    cmd->reason    = CO_ERROR_REMOTE_USER_TERM_CON;

    // Send the message
    ke_msg_send(cmd);
}

void appm_update_param(struct gapc_conn_param *conn_param)
{
    // Prepare the GAPC_PARAM_UPDATE_CMD message
    struct gapc_param_update_cmd *cmd = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CMD,
            KE_BUILD_ID(TASK_GAPC, app_env.conidx), KE_BUILD_ID(TASK_APP, app_env.conidx),
            gapc_param_update_cmd);

    cmd->operation  = GAPC_UPDATE_PARAMS;
    cmd->intv_min   = conn_param->intv_min;
    cmd->intv_max   = conn_param->intv_max;
    cmd->latency    = conn_param->latency;
    cmd->time_out   = conn_param->time_out;

    // not used by a slave device
    cmd->ce_len_min = 0xFFFF;
    cmd->ce_len_max = 0xFFFF;

    // Send the message
    ke_msg_send(cmd);
}

uint8_t appm_get_dev_name(uint8_t* name)
{
    // copy name to provided pointer
    memcpy(name, app_env.dev_name, app_env.dev_name_len);
    // return name length
    return app_env.dev_name_len;
}

//void appm_recv_data(uint8_t conidx, uint8_t *p_pdata, uint16_t len)
//{
//    log_debug("recv to slave len(%x)\n", len);
    // TOOD: Process received data
//}
#endif //(BLE_APP_PRESENT)

/// @} APP
