/**
 ****************************************************************************************
 *
 * @file app_hid.c
 *
 * @brief HID Application Module entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"            // SW configuration
#include "co_timer.h"
#include <stdio.h>
#include <string.h>

#if (BLE_APP_HID)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app.h"                    // Application Definitions
#include "app_sec.h"                // Application Security Module API
#include "app_task.h"               // Application task definitions
#include "app_hid.h"                // HID Application Module Definitions
#include "hogpd_task.h"             // HID Over GATT Profile Device Role Functions
#include "prf_types.h"              // Profile common types Definition
#include "arch.h"                    // Platform Definitions
#include "prf.h"
#include "ke_timer.h"

#if (NVDS_SUPPORT)
#include "nvds.h"                   // NVDS Definitions
#endif //(NVDS_SUPPORT)

#if (DISPLAY_SUPPORT)
#include "app_display.h"            // Application Display Module
#endif //(DISPLAY_SUPPORT)

#include "co_utils.h"               // Common functions

#if (KE_PROFILING)
#include "ke_mem.h"
#endif //(KE_PROFILING)

/*
 * DEFINES
 ****************************************************************************************
 */
#define HOGPD_REPORT_CMD_MAX_LEN          20
#define HOGPD_REPORT_MAX_LEN              60
/// Length of the HID keyboard Report
#define APP_HID_KEYBOARD_REPORT_LEN       (8)
/// Length of the Report Descriptor for an HID keyboard
#define APP_HID_KEYBOARD_REPORT_MAP_LEN   (sizeof(app_hid_keyboard_report_map))

/// Duration before connection update procedure if no report received (mouse is silent) - 20s
#define APP_HID_SILENCE_DURATION_1     (2000)
/// Duration before disconnection if no report is received after connection update - 60s
#define APP_HID_SILENCE_DURATION_2     (6000)

/// Number of reports that can be sent
#define APP_HID_NB_SEND_REPORT         (100)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// States of the Application HID Module
enum app_hid_states
{
    /// Module is disabled (Service not added in DB)
    APP_HID_DISABLED,
    /// Module is idle (Service added but profile not enabled)
    APP_HID_IDLE,
    /// Module is enabled (Device is connected and the profile is enabled)
    APP_HID_ENABLED,
    /// The application can send reports
    APP_HID_READY,
    /// Waiting for a report
    APP_HID_WAIT_REP,

    APP_HID_STATE_MAX,
};

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// HID Application Module Environment Structure
static struct app_hid_env_tag app_hid_env;

/// HID Keyboard Report Descriptor
#if (APP_CUSTOMER_ID == 2 || APP_CUSTOMER_ID==3)
static const uint8_t app_hid_keyboard_report_map[] =
{
    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x01,        //   Report ID (1)
    0x19, 0x00,        //   Usage Minimum (Unassigned)
    0x2A, 0x9C, 0x02,  //   Usage Maximum (AC Distribute Vertically)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0x9C, 0x02,  //   Logical Maximum (668)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x10,        //   Report Size (16)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
    
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,        // Usage (Keyboard)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x02,        //   Report ID (2)
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
    0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x05,        //   Report Count (5)
    0x75, 0x01,        //   Report Size (1)
    0x05, 0x08,        //   Usage Page (LEDs)
    0x19, 0x01,        //   Usage Minimum (Num Lock)
    0x29, 0x05,        //   Usage Maximum (Kana)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x03,        //   Report Size (3)
    0x91, 0x03,        //   Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x06,        //   Report Count (6)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0xFF,        //   Logical Maximum (-1)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x29, 0xFF,        //   Usage Maximum (0xFF)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
    
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,        // Usage (Mouse)
    0xA1, 0x01,        // Collection (Application)
    0x09, 0x01,        //   Usage (Pointer)
    0xA1, 0x00,        //   Collection (Physical)
    0x85, 0x03,        //     Report ID (3)
    0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x15, 0x80,        //     Logical Minimum (-128)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x02,        //     Report Count (2)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x09,        //     Usage Page (Button)
    0x19, 0x01,        //     Usage Minimum (0x01)
    0x29, 0x05,        //     Usage Maximum (0x05)
    0x15, 0x00,        //     Logical Minimum (0)
    0x25, 0x01,        //     Logical Maximum (1)
    0x95, 0x05,        //     Report Count (5)
    0x75, 0x01,        //     Report Size (1)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x01,        //     Report Count (1)
    0x75, 0x03,        //     Report Size (3)
    0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xC0,              // End Collection
    
    0x06, 0x01, 0xFF,  // Usage Page (Vendor Defined 0xFF01)
    0x09, 0x02,        // Usage (0x02)
    0xA1, 0x02,        // Collection (Logical)
    0x85, 0x05,        //   Report ID (5)
    0x09, 0x14,        //   Usage (0x14)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x14,        //   Report Count (20)
    0x15, 0x80,        //   Logical Minimum (-128)
    0x25, 0x7F,        //   Logical Maximum (127)
    0x81, 0x22,        //   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
    0x85, 0x04,        //   Report ID (4)
    0x09, 0x04,        //   Usage (0x04)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x01,        //   Report Count (1)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              // End Collection

    // 174 bytes
};
#else
static const uint8_t app_hid_keyboard_report_map[] =
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
    0x85, 0x03,        //     Report ID (-15)
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
#if 0
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
#endif
    // 147 bytes172
};
#endif
/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
static co_timer_t conn_params_update_timer;
static void conn_params_update_timer_handler(co_timer_t *timer, void *param)
{
    if (GAP_INVALID_CONIDX == app_env.conidx) {
        return;
    }
    
    struct gapc_conn_param conn_param;
    conn_param.intv_min = 0x18;
    conn_param.intv_max = 0x18;
    conn_param.latency  = 0x3B;
    conn_param.time_out = 0x05DC;
    
    appm_update_param(&conn_param);
}

void app_hid_init(void)
{
    // Length of the mouse timeout value
    //nvds_tag_len_t length = NVDS_LEN_MOUSE_TIMEOUT;

    // Reset the environment
    memset(&app_hid_env, 0, sizeof(app_hid_env));

    app_hid_env.nb_report = APP_HID_NB_SEND_REPORT;

    /*
     * Get the timeout value from the NVDS - This value is used each time a report is received
     * from the PS2 driver, store it in the environment.
     */
    //if (nvds_get(NVDS_TAG_MOUSE_TIMEOUT, &length, (uint8_t *)&app_hid_env.timeout) != NVDS_OK)
    //{
    //    app_hid_env.timeout = APP_HID_SILENCE_DURATION_1;
    //}
}


/*
 ****************************************************************************************
 * @brief Function called when get GAP manager command complete events.
 *
 ****************************************************************************************
 */
void app_hid_start_keyboard(void)
{
    /*-----------------------------------------------------------------------------------
     * CONFIGURE THE Keyboard
     *----------------------------------------------------------------------------------*/
}

void app_hid_add_hids(void)
{
    struct hogpd_db_cfg *db_cfg;
    // Prepare the HOGPD_CREATE_DB_REQ message
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                   TASK_GAPM, TASK_APP,
                                                   gapm_profile_task_add_cmd, sizeof(struct hogpd_db_cfg));

    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl     = PERM(SVC_AUTH, UNAUTH);
    req->prf_task_id = TASK_ID_HOGPD;
    req->app_task    = TASK_APP;
    req->start_hdl   = 0;

    // Set parameters
    db_cfg = (struct hogpd_db_cfg* ) req->param;

    // Only one HIDS instance is useful
    db_cfg->hids_nb = 1;

    // The device is a keyboard
    db_cfg->cfg[0].svc_features = HOGPD_CFG_KEYBOARD|HOGPD_CFG_BOOT_KB_WR|HOGPD_CFG_PROTO_MODE;

    // Only one Report Characteristic is requested
    db_cfg->cfg[0].report_nb    = 12;

    db_cfg->cfg[0].report_id[0] = 1;
    db_cfg->cfg[0].report_char_cfg[0] = HOGPD_CFG_REPORT_IN;

    db_cfg->cfg[0].report_id[1] = 2;
    db_cfg->cfg[0].report_char_cfg[1] = HOGPD_CFG_REPORT_OUT;

    db_cfg->cfg[0].report_id[2] = 3;
    db_cfg->cfg[0].report_char_cfg[2] = HOGPD_CFG_REPORT_IN;

    db_cfg->cfg[0].report_id[3] = 4;
    db_cfg->cfg[0].report_char_cfg[3] = HOGPD_CFG_REPORT_FEAT;
    db_cfg->cfg[0].report_id[4] = 5;
    db_cfg->cfg[0].report_char_cfg[4] = HOGPD_CFG_REPORT_FEAT|HOGPD_CFG_REPORT_NI;
    db_cfg->cfg[0].report_id[5] = 6;
    db_cfg->cfg[0].report_char_cfg[5] = HOGPD_CFG_REPORT_FEAT|HOGPD_CFG_REPORT_NI;
    db_cfg->cfg[0].report_id[6] = 7;
    db_cfg->cfg[0].report_char_cfg[6] = HOGPD_CFG_REPORT_FEAT|HOGPD_CFG_REPORT_NI; 
    db_cfg->cfg[0].report_id[7] = 8;
    db_cfg->cfg[0].report_char_cfg[7] = HOGPD_CFG_REPORT_FEAT|HOGPD_CFG_REPORT_NI;

#if 1 
    db_cfg->cfg[0].report_id[8] = 0xe0;
    db_cfg->cfg[0].report_char_cfg[8] = HOGPD_CFG_REPORT_FEAT;
    db_cfg->cfg[0].report_id[9] = 0xe1;
    db_cfg->cfg[0].report_char_cfg[9] = HOGPD_CFG_REPORT_FEAT;
    db_cfg->cfg[0].report_id[10] = 0xe2;
    db_cfg->cfg[0].report_char_cfg[10] = HOGPD_CFG_REPORT_FEAT;
    db_cfg->cfg[0].report_id[11] = 0xe3;
    db_cfg->cfg[0].report_char_cfg[11] = HOGPD_CFG_REPORT_FEAT;
#endif

    // HID Information
    db_cfg->cfg[0].hid_info.bcdHID       = 0x0111;         // HID Version 1.11
    db_cfg->cfg[0].hid_info.bCountryCode = 0x00;
    db_cfg->cfg[0].hid_info.flags        = HIDS_REMOTE_WAKE_CAPABLE | HIDS_NORM_CONNECTABLE;

    // Send the message
    ke_msg_send(req);
}




/*
 ****************************************************************************************
 * @brief Function called when get connection complete event from the GAP
 *
 ****************************************************************************************
 */
void app_hid_enable_prf(uint8_t conidx)
{
    // Requested connection parameters
    //struct gapc_conn_param conn_param;
    //log_debug("%s@%d,%x\n", __func__, __LINE__, conidx);

    uint16_t ntf_cfg;

    // Store the connection handle
    app_hid_env.conidx = conidx;

    // Allocate the message
    struct hogpd_enable_req * req = KE_MSG_ALLOC(HOGPD_ENABLE_REQ,
                                                 prf_get_task_from_id(TASK_ID_HOGPD),
                                                 TASK_APP,
                                                 hogpd_enable_req);

    // Fill in the parameter structure
    req->conidx     = conidx;
    // Notifications are disabled
    ntf_cfg         = 0;

    // Go to Enabled state
    app_hid_env.state = APP_HID_ENABLED;
    app_hid_env.packet_seq = 0;
    
#if (NVDS_SUPPORT)
    // Length of the value read in NVDS
    nvds_tag_len_t length   = NVDS_LEN_MOUSE_NTF_CFG;
    // Notification configuration
    
    if (nvds_get(NVDS_TAG_MOUSE_NTF_CFG, &length, (uint8_t *)&ntf_cfg) == NVDS_OK)
    {
        // CCC enable notification
        if ((ntf_cfg & HOGPD_CFG_REPORT_NTF_EN ) != 0)
        {
            // The device is ready to send reports to the peer device
            app_hid_env.state = APP_HID_READY;
            app_hid_env.nb_report = APP_HID_NB_SEND_REPORT;
            // Restart the mouse timeout timer if needed
            if (app_hid_env.timeout != 0)
            {
                ke_timer_set(APP_HID_MOUSE_TIMEOUT_TIMER, TASK_APP, (uint16_t)(app_hid_env.timeout));
                app_hid_env.timer_enabled = true;
            }
        }
    }
#endif //(NVDS_SUPPORT)

    req->ntf_cfg[conidx] = ntf_cfg;

    /*
     * Requested connection interval: 10ms
     * Latency: 25
     * Supervision Timeout: 2s
     */
    //conn_param.intv_min = APP_CONN_INTERVAL_MIN;
    //conn_param.intv_max = APP_CONN_INTERVAL_MAX;
    //conn_param.latency  = APP_CONN_LATENCY;
    //conn_param.time_out = APP_CONN_TIME_OUT;

    //appm_update_param(&conn_param);

    // Send the message
    ke_msg_send(req);
}

void app_hid_send_cmd_report(uint8_t *pdata, uint8_t len)
{
    switch (app_hid_env.state)
    {
        case (APP_HID_READY):
        {
            // Check if the report can be sent
            if (app_hid_env.nb_report)
            {
                //uint8_t temp_len = HOGPD_REPORT_MAX_LEN;//(len>HOGPD_REPORT_MAX_LEN?HOGPD_REPORT_MAX_LEN:len);
                // Allocate the HOGPD_REPORT_UPD_REQ message
                struct hogpd_report_upd_req * req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_UPD_REQ,
                                                                  prf_get_task_from_id(TASK_ID_HOGPD),
                                                                  TASK_APP,
                                                                  hogpd_report_upd_req,
                                                                  HOGPD_REPORT_CMD_MAX_LEN);

                req->conidx  = app_hid_env.conidx;
                //now fill report
                req->report.hid_idx  = app_hid_env.conidx;
                req->report.type     = HOGPD_REPORT; 
                req->report.idx      = 4;
                req->report.length   = HOGPD_REPORT_CMD_MAX_LEN;
                memset(&req->report.value[0], 0,HOGPD_REPORT_CMD_MAX_LEN);
                if(len > HOGPD_REPORT_CMD_MAX_LEN) {
                    memcpy(&req->report.value[0], pdata, HOGPD_REPORT_CMD_MAX_LEN);
                    len -= HOGPD_REPORT_CMD_MAX_LEN;
                }
                else
                {
                    memcpy(&req->report.value[0], pdata, len);
                    len = 0;
                }

                ke_msg_send(req);

                app_hid_env.nb_report--;

                // Restart the mouse timeout timer if needed
                if (app_hid_env.timeout != 0)
                {
                    ke_timer_set(APP_HID_MOUSE_TIMEOUT_TIMER, TASK_APP, (uint16_t)(app_hid_env.timeout));
                    app_hid_env.timer_enabled = true;
                }
            }
        } break;

        case (APP_HID_WAIT_REP):
        {
            // Go back to the ready state
            app_hid_env.state = APP_HID_READY;
        } break;

        case (APP_HID_IDLE):
        {
            // Try to restart advertising if needed
            //appm_adv_update_state(true);
        } break;

        // DISABLE and ENABLED states
        default:
        {
            // Drop the message
        } break;
    }
}

void app_hid_send_data_report(uint8_t *pdata, uint8_t len)
{
    switch (app_hid_env.state)
    {
        case (APP_HID_READY):
        {
            // Check if the report can be sent
            if (app_hid_env.nb_report)
            {
                //uint8_t temp_len = HOGPD_REPORT_MAX_LEN;//(len>HOGPD_REPORT_MAX_LEN?HOGPD_REPORT_MAX_LEN:len);
                // Allocate the HOGPD_REPORT_UPD_REQ message
                struct hogpd_report_upd_req * req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_UPD_REQ,
                                                                  prf_get_task_from_id(TASK_ID_HOGPD),
                                                                  TASK_APP,
                                                                  hogpd_report_upd_req,
                                                                  HOGPD_REPORT_MAX_LEN);

                req->conidx  = app_hid_env.conidx;
                //now fill report
                req->report.hid_idx  = app_hid_env.conidx;
                req->report.type     = HOGPD_REPORT; 
                req->report.idx      = 5+(app_hid_env.packet_seq%3);
                req->report.length   = HOGPD_REPORT_MAX_LEN;
                memset(&req->report.value[0], 0,HOGPD_REPORT_MAX_LEN);
                if(len > HOGPD_REPORT_MAX_LEN) {
                    memcpy(&req->report.value[0], pdata, HOGPD_REPORT_MAX_LEN);
                    len -= HOGPD_REPORT_MAX_LEN;
                }
                else
                {
                    memcpy(&req->report.value[0], pdata, len);
                    len = 0;
                }
                
                app_hid_env.packet_seq++;
                if (app_hid_env.packet_seq >= 3) 
                {
                    app_hid_env.packet_seq = 0;
                }

                ke_msg_send(req);

                app_hid_env.nb_report--;

                // Restart the mouse timeout timer if needed
                if (app_hid_env.timeout != 0)
                {
                    ke_timer_set(APP_HID_MOUSE_TIMEOUT_TIMER, TASK_APP, (uint16_t)(app_hid_env.timeout));
                    app_hid_env.timer_enabled = true;
                }
            }
        } break;

        case (APP_HID_WAIT_REP):
        {
            // Go back to the ready state
            app_hid_env.state = APP_HID_READY;
        } break;

        case (APP_HID_IDLE):
        {
            // Try to restart advertising if needed
            //appm_adv_update_state(true);
        } break;

        // DISABLE and ENABLED states
        default:
        {
            // Drop the message
        } break;
    }
}

/*
 ****************************************************************************************
 * @brief Function called from PS2 driver
 *
 ****************************************************************************************
 */
void app_hid_send_keyboard_report(struct ps2_keyboard_msg report)
{
    //log_debug("%s@%d,%x\n", __func__, __LINE__, app_hid_env.state);
    switch (app_hid_env.state)
    {
        case (APP_HID_READY):
        {
            // Check if the report can be sent
            if (app_hid_env.nb_report)
            {
                // Allocate the HOGPD_REPORT_UPD_REQ message
                struct hogpd_report_upd_req * req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_UPD_REQ,
                                                                  prf_get_task_from_id(TASK_ID_HOGPD),
                                                                  TASK_APP,
                                                                  hogpd_report_upd_req,
                                                                  APP_HID_KEYBOARD_REPORT_LEN);

                req->conidx  = app_hid_env.conidx;
                //now fill report
                req->report.hid_idx  = app_hid_env.conidx;
                req->report.type     = HOGPD_REPORT; //HOGPD_BOOT_MOUSE_INPUT_REPORT;//
                req->report.idx      = report.report_id-1; //0 for boot reports and report map
				#if (APP_CUSTOMER_ID == 2 || APP_CUSTOMER_ID==3)
				req->report.length   = 2;//APP_HID_KEYBOARD_REPORT_LEN;
                memcpy(&req->report.value[0], &report.key[0], 2);
				#else
                req->report.length   = APP_HID_KEYBOARD_REPORT_LEN;
                memcpy(&req->report.value[0], &report.key[0], 8);
				#endif

                ke_msg_send(req);

                app_hid_env.nb_report--;

                // Restart the mouse timeout timer if needed
                if (app_hid_env.timeout != 0)
                {
                    ke_timer_set(APP_HID_MOUSE_TIMEOUT_TIMER, TASK_APP, (uint16_t)(app_hid_env.timeout));
                    app_hid_env.timer_enabled = true;
                }
            }
        } break;

        case (APP_HID_WAIT_REP):
        {
            // Requested connection parameters
            //struct gapc_conn_param conn_param;

            /*
             * Requested connection interval: 10ms
             * Latency: 25
             * Supervision Timeout: 2s
             */
            //conn_param.intv_min = APP_CONN_INTERVAL_MIN;
            //conn_param.intv_max = APP_CONN_INTERVAL_MAX;
            //conn_param.latency  = APP_CONN_LATENCY;
            //conn_param.time_out = APP_CONN_TIME_OUT;

            //appm_update_param(&conn_param);

            // Restart the mouse timeout timer if needed
            if (app_hid_env.timeout != 0)
            {
                ke_timer_set(APP_HID_MOUSE_TIMEOUT_TIMER, TASK_APP, (uint16_t)(app_hid_env.timeout));
                app_hid_env.timer_enabled = true;
            }

            // Go back to the ready state
            app_hid_env.state = APP_HID_READY;
        } break;

        case (APP_HID_IDLE):
        {
            // Try to restart advertising if needed
            //appm_adv_update_state(true);
        } break;

        // DISABLE and ENABLED states
        default:
        {
            // Drop the message
        } break;
    }
}

void app_hid_send_control_report(uint16_t keycode)
{
    //log_debug("%s@%d,%x\n", __func__, __LINE__, app_hid_env.state);
    switch (app_hid_env.state)
    {
        case (APP_HID_READY):
        {
            // Check if the report can be sent
            if (app_hid_env.nb_report)
            {
                // Allocate the HOGPD_REPORT_UPD_REQ message
                struct hogpd_report_upd_req * req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_UPD_REQ,
                                                                  prf_get_task_from_id(TASK_ID_HOGPD),
                                                                  TASK_APP,
                                                                  hogpd_report_upd_req,
                                                                  3);

                req->conidx  = app_hid_env.conidx;
                //now fill report
                req->report.hid_idx  = app_hid_env.conidx;
                req->report.type     = HOGPD_REPORT; //HOGPD_BOOT_MOUSE_INPUT_REPORT;//
                req->report.idx      = 2; //0 for boot reports and report map
                req->report.length   = 3;
                memcpy(&req->report.value[0], &keycode, 2);

                ke_msg_send(req);

                app_hid_env.nb_report--;

                // Restart the mouse timeout timer if needed
                if (app_hid_env.timeout != 0)
                {
                    ke_timer_set(APP_HID_MOUSE_TIMEOUT_TIMER, TASK_APP, (uint16_t)(app_hid_env.timeout));
                    app_hid_env.timer_enabled = true;
                }
            }
        } break;

        case (APP_HID_WAIT_REP):
        {
            // Restart the mouse timeout timer if needed
            if (app_hid_env.timeout != 0)
            {
                ke_timer_set(APP_HID_MOUSE_TIMEOUT_TIMER, TASK_APP, (uint16_t)(app_hid_env.timeout));
                app_hid_env.timer_enabled = true;
            }

            // Go back to the ready state
            app_hid_env.state = APP_HID_READY;
        } break;

        case (APP_HID_IDLE):
        {
            // Try to restart advertising if needed
            //appm_adv_update_state(true);
        } break;

        // DISABLE and ENABLED states
        default:
        {
            // Drop the message
        } break;
    }
}

/*
 * MESSAGE HANDLERS
 ****************************************************************************************
 */


static int hogpd_ctnl_pt_ind_handler(ke_msg_id_t const msgid,
                                     struct hogpd_ctnl_pt_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    //log_debug("%s@%d,%x\n", __func__, __LINE__, param->hid_ctnl_pt);

    if( param->hid_ctnl_pt ) {
        // hid host wake up(ext suspend)
    }
    else {
        // hid host sleep(suspend)
    }
    return (KE_MSG_CONSUMED);
}




static int hogpd_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                                     struct hogpd_ntf_cfg_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    //log_debug("%s@%d,%x\n", __func__, __LINE__, param->ntf_cfg[param->conidx]);

    if (app_hid_env.conidx == param->conidx)
    {
        app_hid_env.state = APP_HID_ENABLED;

        if (param->ntf_cfg[param->conidx] & HOGPD_CFG_KEYBOARD)
        {
            // keboard notify enable;
            app_hid_env.state = APP_HID_READY;
        }
        if (param->ntf_cfg[param->conidx] & HOGPD_CFG_PROTO_MODE)
        {
            // boot proto mode notify enable;
            app_hid_env.state = APP_HID_READY;
        }
        if (param->ntf_cfg[param->conidx] & HOGPD_CFG_BOOT_KB_WR)
        {
            // boot keboard notify enable;
            app_hid_env.state = APP_HID_READY;
        }
        if (param->ntf_cfg[param->conidx] & HOGPD_CFG_REPORT_NTF_EN)
        {
            // report index= 0; notify enable;
            app_hid_env.state = APP_HID_READY;
        }
        if ((param->ntf_cfg[param->conidx]>>1) & HOGPD_CFG_REPORT_NTF_EN)
        {
            // report index= 1; notify enable;
            app_hid_env.state = APP_HID_READY;
        }
        // Store the notification configuration in the database
        if (nvds_put(NVDS_TAG_MOUSE_NTF_CFG, NVDS_LEN_MOUSE_NTF_CFG,
                     (uint8_t *)&param->ntf_cfg[param->conidx]) != NVDS_OK)
        {
            // Should not happen
            ASSERT_ERR(0);
        }
    }

    return (KE_MSG_CONSUMED);
}

static int hogpd_report_req_ind_handler(ke_msg_id_t const msgid,
                                    struct hogpd_report_req_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
   // log_debug("%s@%d,%x,type=%x,index=%x\n", __func__, __LINE__, param->operation,param->report.type,param->report.idx );

    if (param->operation == HOGPD_OP_REPORT_WRITE && param->report.type == HOGPD_REPORT)
    {
        // param->report.value[0]; out report. set led num lock| caps lock|scroll lock| compose|kana
        //log_debug("set led value=%x\n", param->report.value[0]);
        struct hogpd_report_cfm *req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                                                        prf_get_task_from_id(TASK_ID_HOGPD),/* src_id */
                                                        TASK_APP,
                                                        hogpd_report_cfm,
                                                        0/*param->report.length*/);

        req->conidx = param->conidx; ///app_hid_env.conidx; ///???
        /// Operation requested (read/write @see enum hogpd_op)
        req->operation = HOGPD_OP_REPORT_WRITE;
        /// Status of the request
        req->status = GAP_ERR_NO_ERROR;  ///???
        /// Report Info
        //req->report;
        /// HIDS Instance
        req->report.hid_idx = app_hid_env.conidx; ///???
        /// type of report (@see enum hogpd_report_type)
        req->report.type = param->report.type;//-1;//outside 
        /// Report Length (uint8_t)
        req->report.length = 0; //param->report.length;
        /// Report Instance - 0 for boot reports and report map
        req->report.idx = param->report.idx; //0:input; 1: output; 2:multim key

        // Send the message
        ke_msg_send(req);
    }
    else if ((param->operation == HOGPD_OP_REPORT_READ) && (param->report.type == HOGPD_REPORT_MAP))
    {
        uint16_t map_len = APP_HID_KEYBOARD_REPORT_MAP_LEN;
        if (app_hid_env.report_map_len != 0)
        {
            map_len = app_hid_env.report_map_len;
        }
        struct hogpd_report_cfm *req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                                                        src_id, ///prf_get_task_from_id(TASK_ID_HOGPD),/* src_id */
                                                        dest_id, ///TASK_APP,
                                                        hogpd_report_cfm,
                                                        map_len);

        req->conidx = app_hid_env.conidx; ///???
        /// Operation requested (read/write @see enum hogpd_op)
        req->operation = HOGPD_OP_REPORT_READ;
        /// Status of the request
        req->status = GAP_ERR_NO_ERROR;  ///???
        /// Report Info
        //req->report;
        /// HIDS Instance
        req->report.hid_idx = param->report.hid_idx;///   ???///app_hid_env.conidx; ///???
        /// type of report (@see enum hogpd_report_type)
        req->report.type = HOGPD_REPORT_MAP;
        /// Report Length (uint8_t)
       req->report.length = map_len;
        /// Report Instance - 0 for boot reports and report map
        req->report.idx = 0;
        /// Report data
        if (app_hid_env.report_map_len != 0)
        {
            memcpy(&req->report.value[0], app_hid_env.report_map, app_hid_env.report_map_len);
        }
        else
        {
            memcpy(&req->report.value[0], &app_hid_keyboard_report_map[0], map_len);
        }

        // Send the message
        ke_msg_send(req);
    }
    else if (param->operation == HOGPD_OP_REPORT_READ)
    {
        if (param->report.type == HOGPD_BOOT_KEYBOARD_INPUT_REPORT)
        { //request of boot report
            struct hogpd_report_cfm *req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                                                            prf_get_task_from_id(TASK_ID_HOGPD),/* src_id */
                                                            TASK_APP,
                                                            hogpd_report_cfm,
                                                            0/*param->report.length*/);

            req->conidx = param->conidx; ///app_hid_env.conidx; ///???
            /// Operation requested (read/write @see enum hogpd_op)
            req->operation = HOGPD_OP_REPORT_READ;
            /// Status of the request
            req->status = GAP_ERR_NO_ERROR;  ///???
            /// HIDS Instance
            req->report.hid_idx = app_hid_env.conidx; ///???
            /// type of report (@see enum hogpd_report_type)
            req->report.type = param->report.type;//-1;//outside 
            /// Report Length (uint8_t)
            req->report.length = 0; //param->report.length;
            /// Report Instance - 0 for boot reports and report map
            req->report.idx = param->report.idx; //0;
            /// Report data

            // Send the message
            ke_msg_send(req);
        }
        else if (param->report.type == HOGPD_REPORT)
        { //request of  report
            uint8_t len = 12;
            uint8_t val[12] = {0x27,0x17,0x32,0xB8,0x10,0x16,0x64,0x19,0xA0,0x00,0x00,0x00};
            if (param->report.idx == 8)
            {
                len = 12;
            }
            else if (param->report.idx == 9)
            {
                memset(val, 0, len);
                len = 6;
                val[5] = 0x20;
            }
            else if (param->report.idx == 11)
            {
                memset(val, 0, len);
                len = 1;
                val[0] = 0x02;
                
                co_timer_set(&conn_params_update_timer, 1000, TIMER_ONE_SHOT, conn_params_update_timer_handler, NULL);
            }
            else
            {
                memset(val, 0, len);
                len = 8;
            }
            struct hogpd_report_cfm *req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                                                            prf_get_task_from_id(TASK_ID_HOGPD),/* src_id */
                                                            TASK_APP,
                                                            hogpd_report_cfm,
                                                            len/*param->report.length*/);

            req->conidx = param->conidx; ///app_hid_env.conidx; ///???
            /// Operation requested (read/write @see enum hogpd_op)
            req->operation = HOGPD_OP_REPORT_READ;
            /// Status of the request
            req->status = GAP_ERR_NO_ERROR;  ///???
            /// Report Info
            //req->report;
            /// HIDS Instance
            req->report.hid_idx = app_hid_env.conidx; ///???
            /// type of report (@see enum hogpd_report_type)
            req->report.type = param->report.type;//-1;//outside 
            /// Report Length (uint8_t)
            req->report.length = len; //param->report.length;
            /// Report Instance - 0 for boot reports and report map
            req->report.idx = param->report.idx; //0:input; 1: output; 2:multim key
            /// Report data
            //memset(&req->report.value[0], 0, len); //???
            memcpy(&req->report.value[0], val, len);

            // Send the message
            ke_msg_send(req);
        }
        else if (param->report.type ==  HOGPD_BOOT_KEYBOARD_OUTPUT_REPORT)
        {
            struct hogpd_report_cfm *req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                                                            prf_get_task_from_id(TASK_ID_HOGPD),/* src_id */
                                                            TASK_APP,
                                                            hogpd_report_cfm,
                                                            8/*param->report.length*/);

            req->conidx = param->conidx; ///app_hid_env.conidx; ///???
            /// Operation requested (read/write @see enum hogpd_op)
            req->operation = HOGPD_OP_REPORT_READ;
            /// Status of the request
            req->status = GAP_ERR_NO_ERROR;  ///???
            /// Report Info
            //req->report;
            /// HIDS Instance
            req->report.hid_idx = app_hid_env.conidx; ///???
            /// type of report (@see enum hogpd_report_type)
            req->report.type = param->report.type;//-1;//outside 
            /// Report Length (uint8_t)
            req->report.length = 1; //param->report.length;
            /// Report Instance - 0 for boot reports and report map
            req->report.idx = param->report.idx; //0;
            /// Report data
            req->report.value[0] = 0x00;
            // Send the message
            ke_msg_send(req);
        }
        else {
            log_debug("not handler\n");
        }
    }

    return (KE_MSG_CONSUMED);
}

static int hogpd_proto_mode_req_ind_handler(ke_msg_id_t const msgid,
                                        struct hogpd_proto_mode_req_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    //log_debug("%s@%d,%x,proto=%x\n", __func__, __LINE__, param->operation,param->proto_mode );

    if ((param->conidx == app_hid_env.conidx) && (param->operation == HOGPD_OP_PROT_UPDATE))
    {

        //make use of param->proto_mode
        struct hogpd_proto_mode_cfm *req = KE_MSG_ALLOC_DYN(HOGPD_PROTO_MODE_CFM,
                                                        prf_get_task_from_id(TASK_ID_HOGPD),/* src_id */
                                                        TASK_APP,
                                                        hogpd_proto_mode_cfm,
                                                        0);
        /// Connection Index
        req->conidx = app_hid_env.conidx; 
        /// Status of the request
        req->status = GAP_ERR_NO_ERROR;
        /// HIDS Instance
        req->hid_idx = app_hid_env.conidx;
        /// New Protocol Mode Characteristic Value
        req->proto_mode = param->proto_mode;
        

        // Send the message
        ke_msg_send(req);
    }
    else
    {
        struct hogpd_proto_mode_cfm *req = KE_MSG_ALLOC_DYN(HOGPD_PROTO_MODE_CFM,
                                                        prf_get_task_from_id(TASK_ID_HOGPD),/* src_id */
                                                        TASK_APP,
                                                        hogpd_proto_mode_cfm,
                                                        0);
        /// Status of the request
        req->status = ATT_ERR_APP_ERROR;

        /// Connection Index
        req->conidx = app_hid_env.conidx;
        /// HIDS Instance
        req->hid_idx = app_hid_env.conidx;
        /// New Protocol Mode Characteristic Value
        req->proto_mode = param->proto_mode;
        
        // Send the message
        ke_msg_send(req);
    }
    return (KE_MSG_CONSUMED);
}


static int hogpd_report_upd_handler(ke_msg_id_t const msgid,
                                   struct hogpd_report_upd_rsp const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    //log_debug("%s@%d,%x\n", __func__, __LINE__, param->status );

    if (app_hid_env.conidx == param->conidx)
    {
        if (GAP_ERR_NO_ERROR == param->status)
        {
            if (app_hid_env.nb_report < APP_HID_NB_SEND_REPORT)
            {
                app_hid_env.nb_report++;
            }
        }
        else
        {
            // we get this message if error occur while sending report
            // most likely - disconnect
            // Go back to the ready state
            //app_hid_env.state = APP_HID_IDLE;
            // change mode
            // restart adv
            // Try to restart advertising if needed
            //appm_adv_update_state(true);

            //report was not success - need to restart???
        }
    }
    return (KE_MSG_CONSUMED);
}

static int hogpd_enable_rsp_handler(ke_msg_id_t const msgid,
                                     struct hogpd_enable_rsp const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    //log_debug("%s@%d,%x\n", __func__, __LINE__, param->status );

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Function called when the APP_HID_MOUSE_TIMEOUT_TIMER expires.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_hid_keyboard_timeout_timer_handler(ke_msg_id_t const msgid,
                                               void const *param,
                                               ke_task_id_t const dest_id,
                                               ke_task_id_t const src_id)
{
    //log_debug("%s@%d\n", __func__, __LINE__);

    app_hid_env.timer_enabled = false;

    if (app_hid_env.state == APP_HID_READY)
    {
        // Requested connection parameters
        //struct gapc_conn_param conn_param;
        // Length
        nvds_tag_len_t length = NVDS_LEN_MOUSE_TIMEOUT;
        // Timer value
        uint16_t timer_val;

        /*
         * Request an update of the connection parameters
         * Requested connection interval: 10ms
         * Latency: 200
         * Supervision Timeout: 5s
         */
        //conn_param.intv_min = APP_CONN_INTERVAL_MIN;
        //conn_param.intv_max = APP_CONN_INTERVAL_MAX;
        //conn_param.latency  = 200;
        //conn_param.time_out = 500;

        //appm_update_param(&conn_param);

        // Go to the Wait for Report state
        app_hid_env.state = APP_HID_WAIT_REP;
        // Get the timer value from the NVDS
        if (nvds_get(NVDS_TAG_MOUSE_ENERGY_SAFE, &length, (uint8_t *)&timer_val) != NVDS_OK)
        {
            timer_val = APP_HID_SILENCE_DURATION_2;
        }

        // Relaunch the timer
        ke_timer_set(APP_HID_MOUSE_TIMEOUT_TIMER, TASK_APP, timer_val);
        app_hid_env.timer_enabled = true;
    }
    else if (app_hid_env.state == APP_HID_WAIT_REP)
    {
      // Disconnect the link with the device
        appm_disconnect(0);

        //log_debug("%s@%d, disconnect\n", __func__, __LINE__);

        // Go back to the ready state
        app_hid_env.state = APP_HID_IDLE;
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_hid_msg_dflt_handler(ke_msg_id_t const msgid,
                                    void const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    // Drop the message

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief set report map 
 *
 * @param[in]:  report map: thre report map, must be const
 * @param[in]:  report map len;
 ****************************************************************************************
 */
void app_hid_set_report_map(uint8_t *report_map, uint16_t report_map_len)
{
    app_hid_env.report_map = report_map;
    app_hid_env.report_map_len = report_map_len;
}

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_hid_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_hid_msg_dflt_handler},

    {HOGPD_ENABLE_RSP,              (ke_msg_func_t)hogpd_enable_rsp_handler},
    {HOGPD_NTF_CFG_IND,             (ke_msg_func_t)hogpd_ntf_cfg_ind_handler},
    {HOGPD_REPORT_REQ_IND,          (ke_msg_func_t)hogpd_report_req_ind_handler},
    {HOGPD_PROTO_MODE_REQ_IND,      (ke_msg_func_t)hogpd_proto_mode_req_ind_handler},
    {HOGPD_CTNL_PT_IND,             (ke_msg_func_t)hogpd_ctnl_pt_ind_handler},
    {HOGPD_REPORT_UPD_RSP,          (ke_msg_func_t)hogpd_report_upd_handler},

    {APP_HID_MOUSE_TIMEOUT_TIMER,   (ke_msg_func_t)app_hid_keyboard_timeout_timer_handler},
};

const struct app_subtask_handlers app_hid_handlers = APP_HANDLERS(app_hid);

#endif //(BLE_APP_HID)

/// @} APP
