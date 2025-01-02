/**
 ****************************************************************************************
 *
 * @file app_tuya_model.h
 *
 * @brief BLE Mesh TUYA model header file.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

#ifndef __APP_TUYA_MODEL__
#define __APP_TUYA_MODEL__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "m_tb.h"
#include "mm_tb.h"
#include "app_tuya_model_demo.h"

/*********************************************************************
 * MACROS
 */
/// Head length for buffers allocated by the SIG Model
#define TUYA_MM_SIG_HEAD_LEN          (2)
/// Head length for buffers allocated by the Vendor Model
#define TUYA_MM_VENDOR_HEAD_LEN       (3)

/*********************************************************************
 *  ENUM
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * EXTERN VARIABLES
 */

/*********************************************************************
 * EXTERN FUNCTIONS
 */

/**
 ****************************************************************************************
 * @brief Initialize the BLE mesh model for stack use.
 ****************************************************************************************
 */
uint16_t mm_tuya_init(bool reset, void *p_env, const mm_cfg_t *p_cfg);

/**
 ****************************************************************************************
 * @brief Initialize the BLE mesh model size for stack use.
 ****************************************************************************************
 */
uint16_t mm_tuya_get_env_size(const mm_cfg_t *p_cfg);

#ifdef __cplusplus
}
#endif

#endif /* __APP_TUYA_MODEL__ */

/** @} */

