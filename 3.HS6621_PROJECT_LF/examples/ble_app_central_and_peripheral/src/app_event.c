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
#include "co_debug.h"
#include "app.h"
#include "peripheral.h"
//APP_HANDLER(gatt_mtu_changed, msgid, param, dest_id, src_id);

/*********************************************************************
 * EXTERN FUNCTIONS
 */
void app_tspp_server_send_data(uint8_t conidx, uint8_t* pdata, uint16_t len);

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

// gap handlers
void app_gap_connected_handler(struct gapc_connection_req_ind const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    log_debug_array_ex("ADDR", &param->peer_addr, 6);
}

void app_gap_disconnected_handler(struct gapc_disconnect_ind const *param)
{
    log_debug("%s@%d:0x%02x\n", __func__, __LINE__, param->reason);
}

void app_gap_disconnected_compelete_handler(struct gapc_cmp_evt const *param)
{
    log_debug("%s@%d:0x%02x\n", __func__, __LINE__, param->status);
}

void app_scan_error_hander(uint8_t error)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

// tspp handlers
void app_tspp_empty_handler(void const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

void app_tspp_full_handler(void const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

void app_tspp_recv_handler(struct tspp_server_write_ind const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
    app_tspp_server_send_data(param->conidx, (uint8_t*)param->value, param->length);
}

void app_tspp_error_handler(struct tspp_server_error_evt const *param)
{
    log_debug("%s@%d\n", __func__, __LINE__);
}

extern void gapm_set_bdaddr(bd_addr_t *addr);
void app_set_address(bd_addr_t *addr)
{
    #if (NVDS_SUPPORT)
    nvds_put(NVDS_TAG_BD_ADDRESS, 6, addr->addr);
    #endif
    // after adv and scan sop, call gapm_set_bdaddr
    gapm_set_bdaddr(addr);
}

#if (BLE_APP_CENTRAL_SUPPORT)
void app_central_recv_handler(uint8_t conidx, uint16_t handle, uint16_t len, uint8_t *pdata)
{
    log_debug("central recv, conidx=%x, handle=%x, len=%x\n", conidx, handle, len);
}
#endif

void app_uart1_rx_callback(uint8_t data)
{
    uint32_t c = atoi((char*)(&data));
    #if (BLE_APP_CENTRAL_SUPPORT)
    switch(c)
    {
        case 1:
            // central scan start
            app_central_scan_start();
            break;
        case 2:
            // scan stop
            app_central_scan_stop();
            break;
        case 3:
            {
                // connect remote by addr
                struct gap_bdaddr addr;
                addr.addr_type = 1;
                addr.addr.addr[0] = 0x01;
                addr.addr.addr[1] = 0xbf;
                addr.addr.addr[2] = 0x66;
                addr.addr.addr[3] = 0x21;
                addr.addr.addr[4] = 0x33;
                addr.addr.addr[5] = 0xC2;
                app_central_connect(addr);
            }
            break;
        case 4:
            {
                // connect remote by addr
                struct gap_bdaddr addr;
                addr.addr_type = 0;
                addr.addr.addr[0] = 0x01;
                addr.addr.addr[1] = 0xbf;
                addr.addr.addr[2] = 0x33;
                addr.addr.addr[3] = 0x44;
                addr.addr.addr[4] = 0x55;
                addr.addr.addr[5] = 0x33;
                app_central_connect(addr);
            }
            break;
        case 5:
            // disconnect
            appm_disconnect(0);
            break;
        case 6:
            {
                // send data to slave
                uint8_t send_data[] = {0x01, 0x02, 0x03, 0x04, 0x05,0x06};
                app_gattc_send_data(0, send_data, sizeof(send_data));
            }
            break;
        case 7:
            {
                appm_adv_stop(1);
                // change addr and adv start
                bd_addr_t addr;
                addr.addr[0] = 0x01;
                addr.addr[1] = 0xBF;
                addr.addr[2] = 0x66;
                addr.addr[3] = 0x21;
                addr.addr[4] = 0x33;
                addr.addr[5] = 0xC2;
                app_set_address(&addr);
                appm_adv_start();
            }
            break;
        case 8:
            {
                // if remote device don not advertiser, need call cancel connect, 
                // before connect other device.
                app_central_connect_cancel();
            }
            break;
        default:
            break;
    }
    #endif
}


