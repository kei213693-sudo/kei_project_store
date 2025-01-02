/**
 ****************************************************************************************
 *
 * @file tmall_timer.c
 *
 * @brief BLE mesh tmall timer header file.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

#ifndef __TMALL_TIMER_H__
#define __TMALL_TIMER_H__

/*********************************************************************
 * INCLUDES
 */

#include <stdint.h>

/*********************************************************************
 * MACROS
 */
/**< The number of tmall alarm timers, 40 is required in V2.01. */
#define TMALL_TIMER_NUM       4//40
/**< The number of attr_type to be operated for a alarm timer. */
#define TMALL_TIMER_OP_NUM    2

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
extern "C"
{
#endif

    void tmall_init_timer(void);
    void tmall_set_unix_time(uint8_t *p_attr_value, uint8_t *p_len_value);
    void tmall_del_timer(uint8_t *p_attr_value, uint8_t *p_len_value);
    void tmall_set_timer(uint8_t * p_attr_value, uint8_t * p_len_value,uint8_t timer_type);


#ifdef __cplusplus
}
#endif

#endif

/** @} */


