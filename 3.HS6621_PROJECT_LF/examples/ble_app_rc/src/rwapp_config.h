/**
 * @file rwapp_config.h
 * @brief
 * @date Wed, Sep  5, 2018  5:44:25 PM
 * @author liqiang
 *
 * @addtogroup APP_EXAMPLES_WATCH_CONFIG rwapp_config.h
 * @ingroup BLE_APP_RC
 * @brief the remote control config
 * @details
 *
 * @{
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

/*********************************************************************
 * MACROS
 */
/* ----------------------------------- customer Module ----------------------------------- */
/// <0> phone <1> mi <2> ximi <3> myself
#define APP_CUSTOMER_ID               1

/// Default Device Name
#if (APP_CUSTOMER_ID == 1)
#include "rwapp_config_1.h"

#define BLE_CONN_INTERVAL_MIN       6       //unit: 1.25 ms
#define BLE_CONN_INTERVAL_MAX       16      //unit: 1.25 ms
#define BLE_CONN_LATENCY            133
#define BLE_CONN_TIMEOUT            400     //unit: 10 ms
#elif (APP_CUSTOMER_ID == 2)
#include "rwapp_config_2.h"
#elif (APP_CUSTOMER_ID == 3)
#include "rwapp_config_3.h"
#else
#include "rwapp_config_0.h"
#endif

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * EXTERN VARIABLES
 */


/*********************************************************************
 * EXTERN FUNCTIONS
 */

#ifdef __cplusplus
}
#endif

#endif

/** @} */

