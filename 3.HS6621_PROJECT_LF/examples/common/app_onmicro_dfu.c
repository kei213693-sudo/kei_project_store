/**
 ****************************************************************************************
 *
 * @file app_onmicro_dfu.c
 *
 * @brief Application Module entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_ONMICRO_DFU_PROFILE_C app_onmicro_dfu.c
 * @ingroup APP_ONMICRO_DFU
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_onmicro_dfu.h"       // Application Module Definitions
#include "onmicro_dfu.h"       // Application Module Definitions
#include "app.h"                     // Application Definitions
#include "app_task.h"                // application task definitions
#include "dfu_task.h"               // health thermometer functions
#include "ke_timer.h"
#include "co_bt.h"
#include "co_utils.h"
#include "prf_types.h"               // Profile common types definition
#include "arch.h"                    // Platform Definitions
#include "prf.h"
#include <string.h>
#include "peripheral.h"
#include "dfu.h"

#if BLE_APP_ONMICRO_DFU
/*
 * DEFINES
 ****************************************************************************************
 */
bool onmicro_dfu_reset_flag;
/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

const struct prf_task_cbs* onmicro_dfu_prf_itf_get(void);
void app_onmicro_dfu_add_service(void)
{
//    struct onmicro_dfu_db_cfg* db_cfg;

    struct prf_itf_pair itf_pair = {
        TASK_ID_ONMICRO_DFU, onmicro_dfu_prf_itf_get(),
    };
    prf_itf_register(&itf_pair, 1);
   
    // Allocate the CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd, 0);
    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl     = PERM(SVC_AUTH, NO_AUTH);
    req->prf_task_id = TASK_ID_ONMICRO_DFU;
    req->app_task    = TASK_APP;
    req->start_hdl   = 0;

    // Send the message
    ke_msg_send(req);
}


void app_onmicro_dfu_update_start_ind_handler(uint8_t status, void *p)
{
    log_debug("onmicro_dfu_update_start, updating connection parameter\n");
    struct gapc_conn_param conn_param = {0x0C, 0x0C, 0, 200};
    appm_update_param(&conn_param);
}

void app_onmicro_dfu_update_prog_ind_handler(uint8_t status, void *p)
{
    //log_debug("prog %d %d.\n", *(uint32_t*)p, *((uint32_t*)p+1));
}

void app_onmicro_dfu_update_end_ind_handler(uint8_t status, void *p)
{
    log_debug("onmicro_dfu_update_end. status: %d\n", status);
    if(status == DFU_UPDATE_ST_SUCCESS)
    {
#if CONFIG_HS6621C_VROM
        do {
            uint16_t ctrl_flag = *(uint16_t*)p;
            if(ctrl_flag & DFU_CTRL_BIT_MORE_IMG){
                struct vrom_dfu_info {
                    uint8_t bdaddr[6];
                    uint8_t addr_type;
                    int8_t load_capacitance;
                    uint16_t dfu_att_handle;
                };
                extern int vrom_set_dfu_info(struct vrom_dfu_info* p_dfu_info);
                struct vrom_dfu_info dfu_info;
                memcpy(dfu_info.bdaddr, app_env.bdaddr, sizeof(app_env.bdaddr));
                dfu_info.addr_type = app_env.addr_type;
                dfu_info.dfu_att_handle = app_env.dfu_att_handle;
                dfu_info.load_capacitance = app_env.load_capacitance;
                int res = vrom_set_dfu_info(&dfu_info);
                co_assert(res >= 0);
            }
        } while(0);
#endif
        ke_state_t state = ke_state_get(TASK_APP);
        if(state != APPM_CONNECTED){
            log_debug("REBOOT !!\n");
            pmu_force_reboot();
        }else{
            log_debug("Disconnecting.\n");
            appm_disconnect(0);
            onmicro_dfu_reset_flag = true;
        }
    }
    else
    {
        struct gapc_conn_param conn_param = {0x20, 0x40, 10, 200};
        appm_update_param(&conn_param);
    }
}

uint8_t app_onmicro_dfu_user_check_handler(uint8_t img_type, uint32_t img_size, uint32_t img_version)
{
    log_debug("DFU Image: Type-0x%02X, Size-0x%X(%d), Version-0x%08X\n", img_type, img_size, img_size, img_version);
#if 1
    #define FLASH_BASE    0x00403000
    #define ROM_ID_OFFSET 0x34
    uint32_t rom_id = *(uint32_t*)(uintptr_t)(FLASH_BASE + ROM_ID_OFFSET);
    if(img_type == IMAGE_TYPE_APP) {
        if(img_version != rom_id) {
            log_debug("DFU Image check failed(version not match): Current-0x%08X, Imgae-0x%08X\n", rom_id, img_version);
            return DFU_VERSION_NOT_MATCH;
        }
    }
#endif
    return DFU_SUCCESS;
}
#endif /* #if BLE_APP_ONMICRO_DFU */


/// @} APP
