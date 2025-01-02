/**
 * @file wdt.h
 * @brief watch dog driver
 * @date Thu 04 May 2017 05:37:07 PM CST
 * @author liqiang
 *
 * @defgroup WDT WDT
 * @ingroup PERIPHERAL
 * @brief Watch dog driver
 * @details WDT driver
 *
 * @{
 *
 * @example example_wdt.c
 * This is an example of how to use the wdt
 *
 */

#ifndef __WDT_H__
#define __WDT_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

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
 * @brief watch dog keepalive
 *
 * @return None
 **/
void wdt_keepalive(void);

/**
 * @brief enable watch dog
 *
 * @param[in] timeout  timeout with second, 0 to disable It
 *
 * @return None
 **/
void wdt_enable(uint32_t timeout);

/**
 * @brief  wdt reactivate
 **/
void wdt_reactivate(void);

#ifdef __cplusplus
}
#endif

#endif

/** @} */

