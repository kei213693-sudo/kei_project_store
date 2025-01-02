/**
 * @file app_tr_ble_sim.h
 * @brief 
 * @date Fri 23 Dec 2022 01:14:08 PM CST
 * @author Onmicro Software
 *
 * @defgroup 
 * @ingroup 
 * @brief 
 * @details 
 *
 * @{
 */

#ifndef __APP_TR_BLE_SIM_H__
#define __APP_TR_BLE_SIM_H__

#ifdef __cplusplus
extern "C"
{ /*}*/
#endif

/*********************************************************************
 * INCLUDES
 */


/*********************************************************************
 * MACROS
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
 * @brief  tr ble test
 **/
void tr_ble_test(void);

/**
 * @brief  tr ble gpio handler
 *
 * @param[in] level  level
 **/
void tr_ble_gpio_handler(uint32_t level);

#ifdef __cplusplus
/*{*/ }
#endif

#endif

/** @} */

