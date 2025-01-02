/**
 ****************************************************************************************
 *
 * @file app_event.c
 *
 * @brief handler all event
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EXAMPLE_WATCH_EVENT_C app_event.c
 * @ingroup BLE_APP_WATCH
 * @{
 ****************************************************************************************
 */


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
#include "app_sec.h"
#include "app_tspp_server.h"
#include "app_ancsc.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*********************************************************************
 * EXTERN FUNCTIONS
 */

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void app_audio_rx_cmd_handler(struct app_audio_rx_cmd_t const *param)
{
}

/**
 ****************************************************************************************
 * @brief handle gap connected event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_gap_connected_handler(struct gapc_connection_req_ind const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    log_debug_array_ex("ADDR", &param->peer_addr, 6);
    if(1){ // is android devices
        //app_sec_send_security_req(app_env.conidx);
    }
}

/**
 ****************************************************************************************
 * @brief handle gap disconnected event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_gap_disconnected_handler(struct gapc_disconnect_ind const *param)
{
    log_debug("%s@%d:0x%02x\n", __func__, __LINE__, param->reason);
}

/**
 ****************************************************************************************
 * @brief handle gap param update event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_gap_param_update_ind_handler(struct gapc_param_updated_ind const *param)
{
    log_debug("%s@%d:%d/%d/%d\n", __func__, __LINE__, param->con_interval, param->con_latency, param->sup_to);
}

/**
 ****************************************************************************************
 * @brief handle smp encrypted event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_smp_encrypted_handler(void const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

/**
 ****************************************************************************************
 * @brief handle gatt mtu changed event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_gatt_mtu_changed_handler(struct gattc_mtu_changed_ind const *param)
{
    log_debug("%s@%d:%d\n", __func__, __LINE__, param->mtu);
}

/**
 ****************************************************************************************
 * @brief handle tspp send buffer empty event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_tspp_empty_handler(void const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

/**
 ****************************************************************************************
 * @brief handle tspp send buffer full event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_tspp_full_handler(void const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

/**
 ****************************************************************************************
 * @brief handle tspp recv data event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_tspp_recv_handler(struct tspp_server_write_ind const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

/**
 ****************************************************************************************
 * @brief handle tspp error event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_tspp_error_handler(struct tspp_server_error_evt const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

#if BLE_APP_ANCSC
/**
 ****************************************************************************************
 * @brief handle ancs  recv  event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_ancs_recv_evt_handler(struct ancsc_notification_ind const *param)
{
    log_debug("ANCS Report event: id(%d),category(%d)\n", param->event_id, param->category_id);
}

/**
 ****************************************************************************************
 * @brief handle ancs  recv  message  
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 ****************************************************************************************
 */
void app_ancs_recv_msg_handler(struct ancsc_ntf_attr_ind const *param)
{
    log_debug("ANCS Report mesage: date(%s). %s \n", param->date, param->data);
}
#endif


/// @} APP

