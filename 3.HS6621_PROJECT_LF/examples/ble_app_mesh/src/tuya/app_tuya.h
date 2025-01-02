/**
 ****************************************************************************************
 *
 * @file app_tuya.h
 *
 * @brief BLE Mesh TUYA app header file.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

#ifndef __APP_TUYA__
#define __APP_TUYA__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * MACROS
 */
/// ble advdata length maximum
#define M_ADV_BCN_LEN_MAX   (28)

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
 * @brief Init TUYA app
 * @return
 */
void tuya_app_init(void);

/**
 * @brief Mesh stack reset finish handler
 *
 * @param [in] pov_state true:porved;false:unproved
 *
 * @return
 */
void tuya_mesh_stack_reset_handler(bool prov_state);

#ifdef __cplusplus
}
#endif

#endif /* __APP_TUYA_API__ */

/** @} */
