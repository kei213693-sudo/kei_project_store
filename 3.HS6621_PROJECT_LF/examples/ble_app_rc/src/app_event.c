/*********************************************************************
 * INCLUDES
 */
#include "rwip_config.h" // RW SW configuration
#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "rwip.h"      // RW SW initialization
#include "app.h"
#include "peripheral.h"
#include "m_audio.h"
#include "app_adv.h"
#if (BLE_ATVV_SERVER)
#include "app_atvv.h"             // atv voice task
#endif
#if (BLE_APP_HID)
#include "app_hid.h"             // HID Over GATT Profile Device Role Functions
#endif
//APP_HANDLER(gatt_mtu_changed, msgid, param, dest_id, src_id);
static co_timer_t conn_params_update_timer;
//#if (BLE_ATVV_SERVER)
//static co_timer_t send_ctl_timer;
//static co_timer_t send_voice_timer;
//#endif
static uint8_t app_dfu_start = 0;
/*********************************************************************
 * EXTERN FUNCTIONS
 */
void appm_update_param(struct gapc_conn_param *conn_param);
void app_sec_send_security_req(uint8_t conidx);
void app_tspp_server_send_data(uint8_t conidx, uint8_t* pdata, uint16_t len);
void app_ancsc_start(void); // Call this when link encrypted
void app_ancsc_proform(uint32_t notif_uid, uint8_t act_id); /* ANCS_ACTION_ID_POSITIVE / NEGATIVE */

/*****************************************************************************************/
/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
static void conn_params_update_timer_handler(co_timer_t *timer, void *param)
{
    if (GAP_INVALID_CONIDX == app_env.conidx) {
        return;
    }
    
#if (APP_CUSTOMER_ID != 1)
    {
    struct gapc_conn_param conn_param;
    // interval 15ms iphone6s 12.3kBps; iphone8x 68kBps, mate9 13.2kBps
    conn_param.intv_min = 0x06;//0x10;
    conn_param.intv_max = 0x06;//0x10;
    if (app_dfu_start)
    {
        conn_param.latency  = 0;
    }
    else
    {
       conn_param.latency  = 133;//100;
    }
    conn_param.time_out = 300;

    appm_update_param(&conn_param);
    }
#endif
}

// gap handlers
void app_gap_connected_handler(struct gapc_connection_req_ind const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    log_debug_array_ex("ADDR", &param->peer_addr, 6);
    
    if(0){ // is android devices
        app_sec_send_security_req(0);
    }
    app_dfu_start = 0;
    co_timer_set(&conn_params_update_timer, 10000, TIMER_ONE_SHOT, conn_params_update_timer_handler, NULL);
}

/**
 * Disconnected event handler
 */
void app_gap_disconnected_handler(struct gapc_disconnect_ind const *param)
{
    log_debug("%s@%d:0x%02x\n", __func__, __LINE__, param->reason);
    app_audio_stop();//in case of disconnect when using audio
}

/**
 * Connection parameters updated event handler
 */
void app_gap_param_update_ind_handler(struct gapc_param_updated_ind const *param)
{
    log_debug("%s@%d:%d/%d/%d\n", __func__, __LINE__, param->con_interval, param->con_latency, param->sup_to);
    
    //>32 is to prevent data loss, <(BLE_CONN_INTERVAL*BLE_CONN_LATENCY) is to prevent excessive current
    if(app_dfu_start == 0) {
        if(CONFIG_AUDIO_CODEC_SEL == 2)
        {
            if(param->con_interval > BLE_CONN_INTERVAL_MAX || (param->con_interval * param->con_latency) < (BLE_CONN_INTERVAL_MIN * BLE_CONN_LATENCY))
            {
                co_timer_set(&conn_params_update_timer, 3000, TIMER_ONE_SHOT, conn_params_update_timer_handler, NULL);
            }
        }
    }
}

/**
 * SMP encrypted event handler
 */
void app_smp_encrypted_handler(void const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

/**
 * MTU exchanged event handler
 */
void app_gatt_mtu_changed_handler(struct gattc_mtu_changed_ind const *param)
{
    log_debug("%s@%d:%d\n", __func__, __LINE__, param->mtu);
}

#if (BLE_TSPP_SERVER)
// tspp handlers
void app_tspp_empty_handler(void const *param)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
}

void app_tspp_full_handler(void const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

extern uint8_t is_audio_test_mode;
void app_tspp_recv_handler(struct tspp_server_write_ind const *param)
{
    //uint8_t conidx = 0;
    log_debug("[tspp] recv (%x)\n", param->length);
    if(param->value != NULL)// && param->length == 4)
    {
        if(param->value[0] == 0x01)
        {
            app_audio_start();
        }
        else if(param->value[0] == 0x00)
        {
            app_audio_stop();

            // send end frame
            //uint8_t data[4]={0x11, 0x22, 0x33, 0x44};
            //app_tspp_server_send_data(conidx, data, 4);
        }
        else if(param->value[0] == 0xAA)
        {
            log_debug("Enter testing mode\n");
            is_audio_test_mode = 1;
        }
    }
}

void app_tspp_error_handler(struct tspp_server_error_evt const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    log_debug("operation=%x, status=%x\n", param->operation, param->status);
}
#endif

#if (BLE_ATVV_SERVER)
#if 0
static uint8_t voice_start = 0;
static void send_voice_timer_handler(co_timer_t *timer, void *param)
{
    log_debug("voice_timer=%d\r\n", voice_start);
    if(voice_start)
    {
        voice_start = 0;
        app_audio_start();
        co_timer_set(&send_voice_timer, 5000, TIMER_ONE_SHOT, send_voice_timer_handler, NULL);
    }
    else
    {
        app_audio_stop();
        app_hid_send_control_report(0);
    }
}

static void send_ctl_timer_handler(co_timer_t *timer, void *param)
{
#if (BLE_APP_HID)
    app_hid_send_control_report(0x221);
#endif
    uint8_t ctl = 0x08;
    app_atvv_server_send_control(app_env.conidx, &ctl, 1);

   if(voice_start == 0)
   {
    voice_start = 1;
    co_timer_set(&send_voice_timer, 5000, TIMER_ONE_SHOT, send_voice_timer_handler, NULL);
   }
}
#endif
// atv voice handlers
void app_atvv_empty_handler(void const *param)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
}

void app_atvv_full_handler(void const *param)
{
    //log_debug("%s@%d\n", __func__, __LINE__);
}
/*
void app_atvv_recv_handler(struct atvv_server_write_ind const *param)
{
    //uint8_t conidx = 0;
    log_debug("[atvv] recv (%x)\n", param->length);
    if(param->value != NULL)// && param->length == 4)
    {
        #if (APP_CUSTOMER_ID == 2)
        {
            if(param->value[0] == 0x0a && param->value[1] == 0x00 && param->value[2] == 0x04
              && param->value[3] == 0x00 && param->value[4] == 0x03)
            {
                uint8_t caps[] = { 
                    0x0b,   
                    0x00, 0x04, // VER:0.4  
                    0x00, 0x03,     // CODEC: ADPCM/8k/16 & 16k/16)     
                    0x00, 0x86, // 134 bytes/frame  
                    0x00, 0x14  // 20 bytes/characteristic  
                }; 
                app_atvv_server_send_control(param->conidx, &caps[0], 9);
                co_timer_set(&send_ctl_timer, 4000, TIMER_ONE_SHOT, send_ctl_timer_handler, NULL);

            }
           else if (param->value[0] == 0x0c && param->value[1] == 0x00 && param->value[2] == 0x02)
           {
                if(voice_start==0) {
                    app_audio_start();
                    voice_start = 0;
                    co_timer_set(&send_voice_timer, 5000, TIMER_ONE_SHOT, send_voice_timer_handler, NULL);
                }
           }
           else if(param->value[0] == 0x0D)
           {
                app_audio_stop();
           }
        }
        #endif
        #if (APP_CUSTOMER_ID == 0)
        {
            if(param->value[0] == 0x01)
            {
                app_audio_start();
            }
            else if(param->value[0] == 0x00)
            {
                app_audio_stop();
            }
        }
        #endif
    }
}
*/
void app_atvv_error_handler(struct atvv_server_error_evt const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    log_debug("operation=%x, status=%x\n", param->operation, param->status);
}
#endif

#if BLE_APP_ANCSC
// ancs handlers
void app_ancs_recv_evt_handler(struct ancsc_notification_ind const *param)
{
    log_debug("ANCS Report event: id(%d),category(%d)\n", param->event_id, param->category_id);
}

// ancs handlers
void app_ancs_recv_msg_handler(struct ancsc_ntf_attr_ind const *param)
{
    log_debug("ANCS Report mesage: date(%s). %s \n", param->date, param->data);
}
#endif
