#ifndef _USER_PORT_H
#define _USER_PORT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#include "peripheral.h"

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
    /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
#define LOS_UP_PRIO_BITS                            __NVIC_PRIO_BITS
#else
#define LOS_UP_PRIO_BITS                            4        /* 15 priority levels */
#endif
/* The lowest interrupt priority that can be used in a call to a "set priority" function. */
#define LOS_UP_LIBRARY_LOWEST_INTERRUPT_PRIORITY    15
/* Interrupt priorities used by the kernel port layer itself.
   These are generic to all Cortex-M ports, and do not rely on any particular library functions. */
#define LOS_UP_KERNEL_INTERRUPT_PRIORITY            ( LOS_UP_LIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - LOS_UP_PRIO_BITS) )
#define LOS_UP_NVIC_PENDSV_PRI                      ( ( ( uint32_t ) LOS_UP_KERNEL_INTERRUPT_PRIORITY ) << 16UL )
#define LOS_UP_NVIC_SYSTICK_PRI                     ( ( ( uint32_t ) LOS_UP_KERNEL_INTERRUPT_PRIORITY ) << 24UL )
/* Constants required to manipulate the VFP. */
#define LOS_UP_FPCCR                                ( ( volatile uint32_t * ) 0xe000ef34UL ) /* Floating point context control register. */
#define LOS_UP_ASPEN_AND_LSPEN_BITS                 ( 0x3UL << 30UL )

/* Constants required to manipulate the core.  Registers first... */
#define LOS_UP_NVIC_SYSTICK_CTRL_REG                ( *( ( volatile uint32_t * ) 0xe000e010 ) )
#define LOS_UP_NVIC_SYSTICK_LOAD_REG                ( *( ( volatile uint32_t * ) 0xe000e014 ) )
#define LOS_UP_NVIC_SYSTICK_CURRENT_VALUE_REG       ( *( ( volatile uint32_t * ) 0xe000e018 ) )
#define LOS_UP_NVIC_SHPR3_REG                       ( *( ( volatile uint32_t * ) 0xe000ed20 ) )
/* ...then bits in the registers. */
#define LOS_UP_NVIC_SYSTICK_INT_BIT                 ( 1UL << 1UL )
#define LOS_UP_NVIC_SYSTICK_ENABLE_BIT              ( 1UL << 0UL )
#define LOS_UP_NVIC_SYSTICK_COUNT_FLAG_BIT          ( 1UL << 16UL )
#define LOS_UP_NVIC_PENDSVCLEAR_BIT                 ( 1UL << 27UL )
#define LOS_UP_NVIC_PEND_SYSTICK_CLEAR_BIT          ( 1UL << 25UL )

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _USER_PORT_H */
