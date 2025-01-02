/**
 ****************************************************************************************
 *
 * @file rwapp_config.h
 *
 * @brief BLE Mesh User Config For Whole Project.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

#ifndef __RWAPP_CONFIG_H__
#define __RWAPP_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "rwip_task.h"
#include "app_tmall.h"
#include "app_tuya.h"
#include "mesh_sniffer.h"

/*********************************************************************
 * APP MACROS
 */

// Enable tuya app
#ifndef APP_MESH_TUYA
#define APP_MESH_TUYA       0
#endif

// Enable tmall app
#ifndef APP_MESH_TMALL
#define APP_MESH_TMALL      0
#endif

/*********************************************************************
 *  SYS MACROS !Don't Modfiy!
 */
#undef  CONFIG_XIP_ROM_ALL

#ifndef CONFIG_XIP_FLASH_ALL
#define CONFIG_XIP_FLASH_ALL 1
#endif

#if !APP_MESH_TUYA
#define CFG_BLE_MESH_MDL 1
#endif

#ifndef CFG_BLE_MESH
    #define CFG_BLE_MESH 1
    #define CFG_SEC_CON 1
    #define CFG_BLE_MESH_MSG_API 1
    #define CFG_BLE_MESH_RELAY 1
    #define CFG_BLE_MESH_GATT_PROXY 1
    #define CFG_BLE_MESH_GATT_PROV 1
    //#define CFG_BLE_MESH_FRIEND 1
    //#define CFG_BLE_MESH_LPN 1
    #define CFG_BLE_MESH_STORAGE_NVDS 1

    #if CFG_BLE_MESH_MDL
        #define CFG_BLE_MESH_MDL_GENS 1
        #define CFG_BLE_MESH_MDL_LIGHTS 1
        #define CFG_BLE_MESH_MDL_SENSS 1
        #define CFG_BLE_MESH_MDL_TSCNS 1
    #endif /* #if CFG_BLE_MESH_MDL */
#endif /* #ifndef CFG_BLE_MESH */

// Enable DEBUG
#define CONFIG_DEBUG 1
#define CONFIG_LOG_OUTPUT 1
#define CONFIG_LOG_LEVEL 4

// Use RW BLE host
#define CONFIG_USE_RW_HOST 1

// Use OnMicro DFU
#define BLE_APP_ONMICRO_DFU 0

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * EXTERN VARIABLES
 */
enum {
    TASK_ID_ONMICRO_DFU = TASK_ID_USER,
};

/*********************************************************************
 * EXTERN FUNCTIONS
 */

#ifdef __cplusplus
}
#endif

#endif

/** @} */

