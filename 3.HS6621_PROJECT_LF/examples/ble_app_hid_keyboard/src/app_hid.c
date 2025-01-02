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
#include "peripheral.h"

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

/// Length of the HID keyboard Report
#define APP_HID_KEYBOARD_REPORT_LEN       (8)
/// Length of the Report Descriptor for an HID keyboard
#define APP_HID_KEYBOARD_REPORT_MAP_LEN   (sizeof(app_hid_keyboard_report_map))

/// Duration before connection update procedure if no report received (mouse is silent) - 20s
#define APP_HID_SILENCE_DURATION_1     (2000)
/// Duration before disconnection if no report is received after connection update - 60s
#define APP_HID_SILENCE_DURATION_2     (6000)

/// Number of reports that can be sent
#define APP_HID_NB_SEND_REPORT         (10)

#define APP_HID_TEST_SPEED_TIMER       (1000)

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
static const uint8_t app_hid_keyboard_report_map[] =
{
    // Report ID 1: Keyboard
    0x05, 0x01,                 // Usage Page (Generic Desktop)
    0x09, 0x06,                 // Usage (Keyboard)
    0xA1, 0x01,                 // Collection (Application)
    0x85, 0x01, 			    //	   Report ID (1)	
    0x05, 0x07,                 //     Usage Page (Key Codes)
    0x19, 0xe0,                 //     Usage Minimum (224)
    0x29, 0xe7,                 //     Usage Maximum (231)
    0x15, 0x00,                 //     Logical Minimum (0)
    0x25, 0x01,                 //     Logical Maximum (1)
    0x75, 0x01,                 //     Report Size (1)
    0x95, 0x08,                 //     Report Count (8)
    0x81, 0x02,                 //     Input (Data, Variable, Absolute)

    0x95, 0x01,                 //     Report Count (1)
    0x75, 0x08,                 //     Report Size (8)
    0x81, 0x01,                 //     Input (Constant) reserved byte(1)

    0x95, 0x05,                 //     Report Count (5)
    0x75, 0x01,                 //     Report Size (1)
    0x05, 0x08,                 //     Usage Page (Page# for LEDs)
    0x19, 0x01,                 //     Usage Minimum (1)
    0x29, 0x05,                 //     Usage Maximum (5)
    0x91, 0x02,                 //     Output (Data, Variable, Absolute), Led report
    0x95, 0x01,                 //     Report Count (1)
    0x75, 0x03,                 //     Report Size (3)
    0x91, 0x01,                 //     Output (Data, Variable, Absolute), Led report padding

    0x95, 0x06,                 //     Report Count (6)
    0x75, 0x08,                 //     Report Size (8)
    0x15, 0x00,                 //     Logical Minimum (0)
    0x25, 0x65,                 //     Logical Maximum (101)
    0x05, 0x07,                 //     Usage Page (Key codes)
    0x19, 0x00,                 //     Usage Minimum (0)
    0x29, 0x65,                 //     Usage Maximum (101)
    0x81, 0x00,                 //     Input (Data, Array) Key array(6 bytes)

    0x09, 0x05,                 //     Usage (Vendor Defined)
    0x15, 0x00,                 //     Logical Minimum (0)
    0x26, 0xFF, 0x00,           //     Logical Maximum (255)
    0x75, 0x08,                 //     Report Count (2)
    0x95, 0x02,                 //     Report Size (8 bit)
    0xB1, 0x02,                 //     Feature (Data, Variable, Absolute)

    0xC0,                         // End Collection (Application)

    0x05, 0x0C,                 // Usage Page (Consumer)
    0x09, 0x01,                 // Usage (Consumer Control)
    0xA1, 0x01,                 // Collection (Application)
    0x85, 0x02,  			    // Report ID (2)
    0x19, 0x01,  			    //	   Usage Minimum (Consumer Control) 		
    0x2A, 0xFF, 0x03,   	    //	   Usage Maximum 							 
    0x15, 0x01,  			    //	   Logical Minimum (1) 					
    0x26, 0xFF, 0x03,		    //	   Logical Maximum (1023) 					 
    0x95, 0x01,  			    //     Report Count (1) 						
    0x75, 0x10,  			    //     Report Size (16) 						
    0x81, 0x00,   			    //	   Input (Data,Ary,Abs) 					
    0xC0  	  				    // End Collection 	
};

#if (APP_HID_TEST_SPEED_TIMER)
static co_timer_t app_hid_timer;
static uint8_t key_code = 0x04;
static uint8_t key_modifier = 0;
static uint8_t key_player = 0;
static void app_hid_timer_handler(co_timer_t *timer, void *param)
{
    struct ps2_keyboard_msg report;
    
    if (key_modifier == 0 || key_code<=0x27) {
        // send std key press
        memset(&report, 0, sizeof(struct ps2_keyboard_msg));
        report.report_id = 1;
        report.key[0] = key_modifier;
        report.key[2] = key_code;
        app_hid_send_keyboard_report(report);

        // send std key release
        memset(&report, 0, sizeof(struct ps2_keyboard_msg));
        report.report_id = 1;
        app_hid_send_keyboard_report(report);

        key_code++;
        if (key_code > 0x27)
        {
            if (key_modifier != 0x02) {
                key_modifier = 0x02;
                key_code = 0x04;
            }
        }
    }
    else {
        memset(&report, 0, sizeof(struct ps2_keyboard_msg));
        report.report_id = 2;

        if (key_player ==0) {
            key_player = 1;
            // send multim key press
            report.key[0] = 0xCD;
            report.key[1] = 0x00; // play
            app_hid_send_keyboard_report(report);
        }
        else {
            // send multim key press
            report.key[0] = 0xB7;
            report.key[1] = 0x00; // stop
            
            key_player = 0;
            key_modifier = 0x00;
            key_code = 0x04;

        }
        app_hid_send_keyboard_report(report);

        // release
        memset(&report, 0, sizeof(struct ps2_keyboard_msg));
        report.report_id = 2;
        app_hid_send_keyboard_report(report);
    }

}
#endif

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

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
#if (APP_HID_TEST_SPEED_TIMER)
    co_timer_set(&app_hid_timer, APP_HID_TEST_SPEED_TIMER, TIMER_REPEAT, app_hid_timer_handler, NULL);
#endif
}

void app_hid_stop_keyboard(void)
{
#if (APP_HID_TEST_SPEED_TIMER)
    co_timer_del(&app_hid_timer);
#endif
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
    db_cfg->cfg[0].report_nb    = 3;
   /// db_cfg->cfg[0].report_id    = 1;
    db_cfg->cfg[0].report_id[0] = 1;

    // The report is an input report
    db_cfg->cfg[0].report_char_cfg[0] = HOGPD_CFG_REPORT_IN;

    db_cfg->cfg[0].report_id[1] = 2;

    // The report is an input report
    db_cfg->cfg[0].report_char_cfg[1] = HOGPD_CFG_REPORT_IN;

    db_cfg->cfg[0].report_id[2] = 1;

    // The report is an input report
    db_cfg->cfg[0].report_char_cfg[2] = HOGPD_CFG_REPORT_OUT;


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
    struct gapc_conn_param conn_param;

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
    // Notifications are enabled
    ntf_cfg         = HOGPD_CFG_REPORT_NTF_EN << 0/*report_idx*/;
    ntf_cfg        |= HOGPD_CFG_REPORT_NTF_EN << 1/*report_idx*/;

    // Go to Enabled state
    app_hid_env.state = APP_HID_READY;//APP_HID_ENABLED;

    req->ntf_cfg[conidx] = ntf_cfg;

    /*
     * Requested connection interval: 10ms
     * Latency: 25
     * Supervision Timeout: 2s
     */
    conn_param.intv_min = APP_CONN_INTERVAL_MIN;
    conn_param.intv_max = APP_CONN_INTERVAL_MAX;
    conn_param.latency  = APP_CONN_LATENCY;
    conn_param.time_out = APP_CONN_TIME_OUT;

    appm_update_param(&conn_param);

    // Send the message
    ke_msg_send(req);
}

void app_hid_disable_prf(uint8_t conidx)
{
    // Store the connection handle
    app_hid_env.conidx = GAP_INVALID_CONIDX;
    // Go to Enabled state
    app_hid_env.state = APP_HID_IDLE;

    app_hid_env.nb_report = APP_HID_NB_SEND_REPORT;

    // Allocate the message
    struct hogpd_enable_req * req = KE_MSG_ALLOC(HOGPD_ENABLE_REQ,
                                                 prf_get_task_from_id(TASK_ID_HOGPD),
                                                 TASK_APP,
                                                 hogpd_enable_req);

    // Fill in the parameter structure
    req->conidx     = conidx;
    // Notifications are disabled
    req->ntf_cfg[conidx] = 0;

    // Send the message
    ke_msg_send(req);
}

/*
 ****************************************************************************************
 * @brief Function called from PS2 driver
 *
 ****************************************************************************************
 */
void app_hid_send_keyboard_report(struct ps2_keyboard_msg report)
{
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
                req->report.length   = APP_HID_KEYBOARD_REPORT_LEN;
                memcpy(&req->report.value[0], &report.key[0], 8);

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
            struct gapc_conn_param conn_param;

            /*
             * Requested connection interval: 10ms
             * Latency: 25
             * Supervision Timeout: 2s
             */
            conn_param.intv_min = APP_CONN_INTERVAL_MIN;
            conn_param.intv_max = APP_CONN_INTERVAL_MAX;
            conn_param.latency  = APP_CONN_LATENCY;
            conn_param.time_out = APP_CONN_TIME_OUT;

            appm_update_param(&conn_param);

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
    log_debug("%s@%d,param->ntf_cfg[%d]=%x\n", __func__, __LINE__, param->conidx, param->ntf_cfg[param->conidx]);

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
    }

    return (KE_MSG_CONSUMED);
}

static int hogpd_report_req_ind_handler(ke_msg_id_t const msgid,
                                    struct hogpd_report_req_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    //log_debug("%s@%d,%x,type=%x,index=%x\n", __func__, __LINE__, param->operation,param->report.type,param->report.idx );

    if (param->operation == HOGPD_OP_REPORT_WRITE && param->report.type == HOGPD_REPORT)
    {
        // param->report.value[0]; out report. set led num lock| caps lock|scroll lock| compose|kana
        log_debug("set led value=%x\n", param->report.value[0]);
        
        struct hogpd_report_cfm *req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                                                        src_id, ///prf_get_task_from_id(TASK_ID_HOGPD),/* src_id */
                                                        dest_id, ///TASK_APP,
                                                        hogpd_report_cfm,
                                                        APP_HID_KEYBOARD_REPORT_MAP_LEN);

        req->conidx = app_hid_env.conidx; ///???
        /// Operation requested (read/write @see enum hogpd_op)
        req->operation = HOGPD_OP_REPORT_WRITE;
        /// Status of the request
        req->status = GAP_ERR_NO_ERROR;  ///???
        /// Report Info
        //req->report;
        /// HIDS Instance
        req->report.hid_idx = param->report.hid_idx;///   ???///app_hid_env.conidx; ///???
        /// type of report (@see enum hogpd_report_type)
        req->report.type = HOGPD_REPORT;
        /// Report Length (uint8_t)
        req->report.length = APP_HID_KEYBOARD_REPORT_MAP_LEN;
        /// Report Instance - 0 for boot reports and report map
        req->report.idx = 0;
        /// Report data
        memcpy(&req->report.value[0], &app_hid_keyboard_report_map[0], APP_HID_KEYBOARD_REPORT_MAP_LEN);

        // Send the message
        ke_msg_send(req);
    }
    else if ((param->operation == HOGPD_OP_REPORT_READ) && (param->report.type == HOGPD_REPORT_MAP))
    {
        struct hogpd_report_cfm *req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                                                        src_id, ///prf_get_task_from_id(TASK_ID_HOGPD),/* src_id */
                                                        dest_id, ///TASK_APP,
                                                        hogpd_report_cfm,
                                                        APP_HID_KEYBOARD_REPORT_MAP_LEN);

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
       req->report.length = APP_HID_KEYBOARD_REPORT_MAP_LEN;
        /// Report Instance - 0 for boot reports and report map
        req->report.idx = 0;
        /// Report data
         memcpy(&req->report.value[0], &app_hid_keyboard_report_map[0], APP_HID_KEYBOARD_REPORT_MAP_LEN);

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
            req->report.length = 8; //param->report.length;
            /// Report Instance - 0 for boot reports and report map
            req->report.idx = param->report.idx; //0:input; 1: output; 2:multim key
            /// Report data
            memset(&req->report.value[0], 0, 8); //???

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
    //log_debug("app_hid_env.conidx=%d, param->conidx=%d\n", app_hid_env.conidx, param->conidx);

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

    // log_debug("app_hid_env.nb_report=%d\n", app_hid_env.nb_report);

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
        struct gapc_conn_param conn_param;
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
        conn_param.intv_min = APP_CONN_INTERVAL_MIN;
        conn_param.intv_max = APP_CONN_INTERVAL_MAX;
        conn_param.latency  = 200;
        conn_param.time_out = 500;

        appm_update_param(&conn_param);

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
 * @brief Set the value of the Report Map Characteristic in the database
 ****************************************************************************************
 */
void app_hid_set_report_map(void);

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
