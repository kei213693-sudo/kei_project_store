#include "rwip_config.h" // RW SW configuration

#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "rwip.h"      // RW SW initialization
#include "dbg.h"
#include "peripheral.h"
#include "app.h"
#include "app_task.h"
#include "app_sec.h"
#include "app_hid.h"
#include "m_key.h"
#include "m_led.h"
#include "m_audio.h"
#include "m_ir.h"
#include "app_tspp_server.h"
#include "app_adv.h"
#include "ir_dma.h"

#if CONFIG_KEY_ENABLE

#if (APP_CUSTOMER_ID==1)
extern void appm_opentv_advdata_set(void);
extern void appm_pair_advdata_set(void);
#endif

#define KEY_PRESSED                        true
#define KEY_RELEASED                       false

#define KEY_KB_DATA_INDEX                  2
#define KEY_PROCESS_NUM                    2

static co_timer_t ota_mode_enter_timer;
#define OTA_MODE_ENTER_TID                 &ota_mode_enter_timer    //OTA timer id.
#define OTA_MODE_ENTER_LONG_PRESS_TIME     5000                             //5 seconds.

static co_timer_t bond_info_clear_timer;
#define BOND_INFO_CLEAR_TID                &bond_info_clear_timer    //Bond information clearing timer id.
#define BOND_INFO_CLEAR_LONG_PRESS_TIME    3000                             //3 seconds.

#if (APP_CUSTOMER_ID == 2 || APP_CUSTOMER_ID==3)
#include "app_atvv.h"
static uint16_t keys_matrix[KEY_ROW_COUNT * KEY_COL_COUNT] = KEY_MATRIX_CONSUMER;//KEY_MATRIX_KEYBOARD;
#else
static uint16_t keys_matrix[KEY_ROW_COUNT * KEY_COL_COUNT] = KEY_MATRIX_KEYBOARD;
#endif
static uint16_t keys_matrix_ir[KEY_ROW_COUNT * KEY_COL_COUNT] = KEY_MATRIX_IR;

static app_key_t keys[KEY_PROCESS_NUM];

static bool bond_info_clear_keys_pressed;
static bool ota_mode_enter_keys_pressed;
static bool audio_start_enabled;
static bool power_key_pressed;

static co_timer_t opentv_adv_auto_stop_timer;

// Clear the memory of keys
static void keys_data_clear(void)
{
    memset((uint8_t *)keys, 0, sizeof(app_key_t)*KEY_PROCESS_NUM);
}

#if (APP_CUSTOMER_ID == 2)
// Send consumer key value
static void con_key_report(app_key_t *keys)
{
    log_debug("[KEY]: Send consumer key value [%04X %04X]\n", keys[0].val, keys[1].val);
    // send std key press
    struct ps2_keyboard_msg report;
    memset(&report, 0, sizeof(struct ps2_keyboard_msg));
    report.report_id = 1;
    report.key[0] = keys[0].val & 0xFF;
    report.key[1] = keys[0].val >> 8;
    report.key[2] = keys[1].val & 0xFF;
    report.key[3] = keys[1].val >> 8;
    app_hid_send_keyboard_report(report);
    if(keys[0].val == 0x0221)
    {
        uint8_t data = 0x08;
        app_atvv_server_send_control(0, &data, 1);
    }
}
#elif (APP_CUSTOMER_ID == 3)
// Send consumer key value
static void con_key_report(uint16_t key_code)
{

    log_debug("[KEY]: Send consumer key value.%04x\n", key_code);
    // send std key press
    struct ps2_keyboard_msg report;
    memset(&report, 0, sizeof(struct ps2_keyboard_msg));
    report.report_id = 1;
    report.key[0] = key_code & 0xFF;
    report.key[1] = key_code >> 8;
    app_hid_send_keyboard_report(report);
    if(key_code == 0x0221)
    {
        uint8_t data = 0x08;
        app_atvv_server_send_control(0, &data, 1);
    }
}
#else
// Send keyboard key value
static void std_key_report(uint16_t key_code)
{
#if (BLE_APP_HID)
    log_debug("[KEY]: Send keyboard key value.%04x\n", key_code);
    struct ps2_keyboard_msg report;
    memset(&report, 0, sizeof(struct ps2_keyboard_msg));
    report.report_id = 1;
    report.key[2] = (uint8_t)key_code;
    app_hid_send_keyboard_report(report);
#endif
}
#endif
// Process the key pressed
static void key_code_process(uint16_t key_code)
{
    log_debug("[KEY]: key code = 0x%04X\n", key_code);
    if(ke_state_get(TASK_APP) == APPM_CONNECTED)
    {
#if (APP_CUSTOMER_ID == 2)
    con_key_report(keys);
#elif (APP_CUSTOMER_ID == 3)
    con_key_report(key_code);
#else
        std_key_report(key_code); 
#endif  
    }
    else
    {
#if CONFIG_IR_ENABLE
#if (APP_CUSTOMER_ID != 2)
        if(key_code)
        {
            led_on_duration(100);
            ir_dma_send(key_code);
        }
        else
            ir_dma_stop();
#else
        if(keys[0].val)
        {
            led_on_duration(100);
            ir_dma_send(keys[0].val);
        }
        else
            ir_dma_stop();
#endif
#else
        if(key_code)
        {
            led_on_duration(100);
        }
        
        if(key_code == IR_KEY_POWER)
            power_key_pressed = true;
#endif
    }
}

// Get the keys that be pressed, and process them.
static void key_code_get(uint8_t *p_data)
{
    //process no more than two keys at the same time
    if(keys[0].state==KEY_PRESSED && keys[1].state==KEY_PRESSED)
    {
        //keys[0] released
        if(!(p_data[keys[0].col] & BIT_MASK(keys[0].row)))
        {
            keys[0].state = KEY_RELEASED;
            keys[0].val = 0;
            key_code_process(0);
        }
        
        //keys[1] released
        if(!(p_data[keys[1].col] & BIT_MASK(keys[1].row)))
        {
            keys[1].state = KEY_RELEASED;
            keys[1].val = 0;
            key_code_process(0);
        }
        
        return;
    }
    
    //key scan
    for(int i=0; i<KEY_COL_COUNT; i++)
    {
        for(int j=0; j<KEY_ROW_COUNT; j++)
        {
            //skip the row and column of pressed key
            if(keys[0].state==KEY_PRESSED && keys[0].col==i && keys[0].row==j)
                continue;
            else if(keys[1].state==KEY_PRESSED && keys[1].col==i && keys[1].row==j)
                continue;
            
            if(p_data[i] & BIT_MASK(j))
            {
                if(keys[0].state == KEY_RELEASED)
                {
                    keys[0].state = KEY_PRESSED;
                    keys[0].col = i;
                    keys[0].row = j;
                    //get HID code or IR code, and store it in keys[0]
                    if(ke_state_get(TASK_APP) == APPM_CONNECTED)
                        keys[0].val = keys_matrix[keys[0].row * KEY_COL_COUNT + keys[0].col];
                    else
                        keys[0].val = keys_matrix_ir[keys[0].row * KEY_COL_COUNT + keys[0].col];
                    
                    key_code_process(keys[0].val);
                }
                else if(keys[1].state == KEY_RELEASED)
                {
                    keys[1].state = KEY_PRESSED;
                    keys[1].col = i;
                    keys[1].row = j;
                    //get HID code or IR code, and store it in keys[1]
                    if(ke_state_get(TASK_APP) == APPM_CONNECTED)
                        keys[1].val = keys_matrix[keys[1].row * KEY_COL_COUNT + keys[1].col];
                    else
                        keys[1].val = keys_matrix_ir[keys[1].row * KEY_COL_COUNT + keys[1].col];
                    
                    key_code_process(keys[1].val);
                }
            }
        }
    }
}

// Clearing bond information timer handler
static void bond_info_clear_timer_handler(co_timer_t *timer, void *param)
{
    app_sec_remove_bond();
    pmu_force_reboot();
}

// Clearing bond information check.
static void bond_info_clear_check(void)
{
    if((keys[0].val == IR_KEY_HOME && keys[1].val == IR_KEY_MENU) ||
            (keys[0].val == KB_KEY_HOME && keys[1].val == KB_KEY_MENU) ||
            (keys[0].val == IR_KEY_MENU && keys[1].val == IR_KEY_HOME) ||
            (keys[0].val == KB_KEY_MENU && keys[1].val == KB_KEY_HOME))
    {
        log_debug("Clearing pairing information timer start.\n");
        bond_info_clear_keys_pressed = true;
#if CONFIG_LED_ENABLE
        led_blink_stop();
#endif
        co_timer_set(BOND_INFO_CLEAR_TID, BOND_INFO_CLEAR_LONG_PRESS_TIME, TIMER_ONE_SHOT, bond_info_clear_timer_handler, NULL);
    }
    else
    {
        if(bond_info_clear_keys_pressed)
        {
            log_debug("Clearing bond information timer stop.\n");
            bond_info_clear_keys_pressed = false;
            co_timer_del(BOND_INFO_CLEAR_TID);
        }
    }
}

// Entering OTA mode timer handler
static void ota_mode_enter_timer_handler(co_timer_t *timer, void *param)
{
    led_on();
    #ifdef CONFIG_HS6621
    pmu_force_into_ota_isp_mode();
    #else
    pmu_force_reboot();
    #endif
}

// Entering OTA mode check
static void ota_mode_enter_check(void)
{
#if (APP_CUSTOMER_ID == 2)
    if((keys[0].val == IR_KEY_OK && keys[1].val == IR_KEY_VOL_N) ||
            (keys[0].val == KB_KEY_OK && keys[1].val == KB_KEY_VOLUME_DOWN) ||
            (keys[0].val == IR_KEY_VOL_N && keys[1].val == IR_KEY_OK) ||
            (keys[0].val == KB_KEY_VOLUME_DOWN && keys[1].val == KB_KEY_OK))
#else
    if((keys[0].val == IR_KEY_OK && keys[1].val == IR_KEY_VOLUME_DOWN) ||
            (keys[0].val == KB_KEY_OK && keys[1].val == KB_KEY_VOLUME_DOWN) ||
            (keys[0].val == IR_KEY_VOLUME_DOWN && keys[1].val == IR_KEY_OK) ||
            (keys[0].val == KB_KEY_VOLUME_DOWN && keys[1].val == KB_KEY_OK))
#endif
    {
        log_debug("Entering OTA mode timer start.\n");
        ota_mode_enter_keys_pressed = true;
#if CONFIG_LED_ENABLE
        led_blink_stop();
#endif
        co_timer_set(OTA_MODE_ENTER_TID, OTA_MODE_ENTER_LONG_PRESS_TIME, TIMER_ONE_SHOT, ota_mode_enter_timer_handler, NULL);
    }
    else
    {
        if(ota_mode_enter_keys_pressed)
        {
            log_debug("Entering OTA mode timer stop.\n");
            ota_mode_enter_keys_pressed = false;
            co_timer_del(OTA_MODE_ENTER_TID);
        }
    }
}

#if CONFIG_AUDIO_ENABLE
// Voice check
static void voice_check(void)
{
    if(keys[0].val==KB_KEY_VOICE || keys[1].val==KB_KEY_VOICE)
    {
    #if (APP_CUSTOMER_ID != 2 && APP_CUSTOMER_ID != 3)
        if(!audio_start_enabled)
        {
            audio_start_enabled = true;
            led_on();
            app_audio_start();
        }
    #endif
    }
    if(keys[0].val!=KB_KEY_VOICE && keys[1].val!=KB_KEY_VOICE)
    {
    #if (APP_CUSTOMER_ID != 2 &&APP_CUSTOMER_ID != 3)
        if(audio_start_enabled)
        {
            app_audio_stop();
            led_off();
            audio_start_enabled = false;
        }
    #endif
    }
}
#endif

static void opentv_adv_auto_stop_timer_handler(co_timer_t *timer, void *param)
{
    log_debug("opentv_adv_auto_stop_timer_handler\n");
    appm_adv_stop(1);
}

void keyscan_event_handler(kpp_sim_event_t event, const kpp_sim_data_t data)
{
    switch(event)
    {
        case KPP_SIM_EVENT_DEPRESS:
            log_debug_array_ex("\n[KEY]: key scan data ", data, KEY_COL_COUNT);
            key_code_get((uint8_t *)data);
#if CONFIG_AUDIO_ENABLE
            voice_check();
#endif
            bond_info_clear_check();
            ota_mode_enter_check();
            break;
        
        case KPP_SIM_EVENT_RELEASE:
            log_debug("\n[KEY]: All keys are released.\n");
            keys_data_clear();
            if(!audio_start_enabled)
                key_code_process(0);
#if CONFIG_AUDIO_ENABLE
            voice_check();
#endif
            co_timer_del(BOND_INFO_CLEAR_TID);
            co_timer_del(OTA_MODE_ENTER_TID);
            if(power_key_pressed)
            {
                log_debug("open tv\n");
                power_key_pressed = false;
                appm_adv_stop(1);
                appm_adv_set_mode(APP_ADV_MODE_FAST);
            #if (APP_CUSTOMER_ID == 1)
                appm_opentv_advdata_set();
            #endif
                appm_adv_start();
                co_timer_set(&opentv_adv_auto_stop_timer, 10000, TIMER_ONE_SHOT, opentv_adv_auto_stop_timer_handler, NULL);
            }
            else
            {
                if(ke_state_get(TASK_APP) == APPM_READY && app_sec_get_bond_status())
                {
                    log_debug("reconnect\n");
                    co_timer_del(&opentv_adv_auto_stop_timer);
                    appm_adv_stop(1);
                    appm_adv_set_mode(APP_ADV_MODE_DIRECTED);
                #if (APP_CUSTOMER_ID == 1)
                    appm_pair_advdata_set();
                #endif
                    appm_adv_start();
                }
            }
            break;
        
        default:
            break;
    }
}

#endif /* CONFIG_KBD_ENABLED */
