/**
 * @file timer_pwm_input.h
 * @brief 
 * @date Sat 24 Dec 2022 07:41:20 PM CST
 * @author Onmicro
 *
 * @defgroup 
 * @ingroup 
 * @brief 
 * @details 
 *
 * @{
 */

#ifndef __TIMER_PWM_INPUT_H__
#define __TIMER_PWM_INPUT_H__

#ifdef __cplusplus
extern "C"
{ /*}*/
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
 * @brief  tim pwm input start
 *
 * @param[in] tim  tim
 * @param[in] psc  psc
 *
 * timer cnt: (uint16_t)tim->CNT;
 * edge cnt: (uint16_t)tim->CCR[1], (uint16_t)tim->CCR[2];
 *
 **/
void tim_pwm_input_start(HS_TIM_Type *tim, uint32_t psc);

#ifdef __cplusplus
/*{*/ }
#endif

#endif

/** @} */

