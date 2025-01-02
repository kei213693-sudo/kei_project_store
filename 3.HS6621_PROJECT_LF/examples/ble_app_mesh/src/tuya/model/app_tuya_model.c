/**
 ****************************************************************************************
 *
 * @file app_tuya_model.c
 *
 * @brief BLE mesh app tuya model register.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

/*********************************************************************
 * INCLUDES
 */
#include "rwip_config.h" // SW configuration
#include "app_tuya_model.h"

#if APP_MESH_TUYA

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/// Called by the mesh stack,upper application does not call
/// When register one model,please add it as follow
uint16_t mm_tuya_init(bool reset, void *p_env, const mm_cfg_t *p_cfg)
{
    // Environment initialization
    uint8_t *p_env_cursor = (uint8_t *)p_env;

    p_env_cursor += CO_ALIGN4_HI(mm_tuya_demo_init(reset, (void *)p_env_cursor, p_cfg));
    //TODO add when register new model

    // Return size of environment
    return ((uint32_t)p_env_cursor - (uint32_t)p_env);
}

/// Called by the mesh stack,upper application does not call
/// When register one model,please add it as follow
uint16_t mm_tuya_get_env_size(const mm_cfg_t *p_cfg)
{
    uint16_t total_env_size = 0;

    total_env_size += CO_ALIGN4_HI(mm_tuya_demo_get_env_size(p_cfg));
    //TODO add when register new model

    // Return size of environment
    return (total_env_size);
}

#endif /* APP_MESH_TUYA */