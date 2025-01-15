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
#include "app_ble.h"
#include "ke_timer.h"

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
    log_debug("%s@%d\n", __func__, __LINE__);
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
    log_debug("----------%s@%d,conhdl:%d,Addr Type:%d,", __func__, __LINE__,param->conhdl,param->peer_addr_type);
    log_debug_array_ex("ADDR", &param->peer_addr, 6);

    ke_timer_set(APP_BLE_OPT_UPDATA_PARAM, APP_BLE_TASK_ID, 300);//3s后检查，是否要主动发送连接参数更新请求
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
    log_debug("----------%s@%d,conhdl:%d,reason:0x%02x\n", __func__, __LINE__,param->conhdl, param->reason);
    app_ble_admin_check(NULL);//清空管理员权限标志
    app_ble_rssi_opt(APP_BLE_RSSI_STOP,NULL);//停止定时获取rssi
    app_ble_adv_updata_check();//检查是否要修改连接参数，用于配对模式结束后恢复广播间隔
}

/**
 ****************************************************************************************
 * @brief handle gap param update event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 * 连接间隔, *1.25ms
 ****************************************************************************************
 */
void app_gap_param_update_ind_handler(struct gapc_param_updated_ind const *param)
{
    log_debug("%s@%d,interval:%d ms,latency:%d,timeout:%d ms\n",__func__, __LINE__
        ,(param->con_interval)*5/4,param->con_latency,(param->sup_to)*10);

    // if((ke_timer_active(APP_BLE_OPT_UPDATA_PARAM, APP_BLE_TASK_ID)) //定时器准备运行
    //     && (param->sup_to <= 200) && (param->con_interval <= 40))//已经收到连接参数更新指令，且timeout <= 2s
    // {//关闭定时器,不再发送请求
    //     ke_timer_clear(APP_BLE_OPT_UPDATA_PARAM, APP_BLE_TASK_ID);
    // }

    if(0 != app_ble_admin_get())
    {//回连成功了
        app_ble_rssi_opt(APP_BLE_RSSI_STOP,NULL);//停止定时获取rssi
        app_ble_rssi_opt(APP_BLE_RSSI_PERIOD_SET,(void *)param);//更新定时获取周期
        app_ble_rssi_opt(APP_BLE_RSSI_START,NULL);//重新启动
    }
}

/**
 ****************************************************************************************
 * @brief handle smp encrypted event 
 *
 * @param[in] param     Pointer to the parameters of the event.
 *
 * @return void
 * 快速回连成功调用
 ****************************************************************************************
 */
void app_smp_encrypted_handler(uint8_t const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);

    app_ble_admin_check((uint8_t *)param);//先检查
    if(0 != app_ble_admin_get())//再判定
    {
        app_ble_rssi_opt(APP_BLE_RSSI_STOP,NULL);//停止定时获取rssi
        app_ble_rssi_opt(APP_BLE_RSSI_START,NULL);//快速回连成功，启动定时获取rssi任务
    }
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

