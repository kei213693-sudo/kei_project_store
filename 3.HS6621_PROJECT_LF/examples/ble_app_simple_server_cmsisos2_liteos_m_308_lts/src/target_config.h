/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**@defgroup los_config System configuration items
 * @ingroup kernel
 */

#ifndef _TARGET_CONFIG_H
#define _TARGET_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#include "peripheral.h"

/*=============================================================================
                    PMU Timer configuration
=============================================================================*/
#define PMU_TIMER_CLOCK                                     ( 32768 )
#define PMU_TIMER_DELAY_MAX                                 ( 0xFFFFFFFF >> 1 )
/*=============================================================================
                    Low power configuration
=============================================================================*/
#define LOSCFG_KERNEL_PM                                    1
/*=============================================================================
                    System clock module configuration
=============================================================================*/
#define LOSCFG_USE_SYSTICK                                  0
#if LOSCFG_USE_SYSTICK
#define OS_SYS_CLOCK                                        ( ( unsigned long ) cpm_get_clock(CPM_CPU_CLK) )
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                    ( 1000 )
#define LOSCFG_BASE_CORE_TICK_RESPONSE_MAX                  ( 0xFFFFFF )
void SysTick_Handler(void);
#define OsTickHandler                                       SysTick_Handler
#else
#define OS_SYS_CLOCK                                        PMU_TIMER_CLOCK
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                    ( 512 )
#define LOSCFG_BASE_CORE_TICK_RESPONSE_MAX                  PMU_TIMER_DELAY_MAX
#endif
#define LOSCFG_BASE_CORE_TICK_HW_TIME                       0
/*=============================================================================
                Hardware interrupt module configuration
=============================================================================*/
#define LOSCFG_PLATFORM_HWI                                 0
#define LOSCFG_PLATFORM_HWI_LIMIT                           128
#define LOSCFG_USE_SYSTEM_DEFINED_INTERRUPT                 0
/*=============================================================================
                        Task module configuration
=============================================================================*/
#define LOSCFG_BASE_CORE_TSK_LIMIT                          16
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE                (ALIGN(1024U, 4))
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE             (ALIGN(1024U, 4))
#define LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE                 (ALIGN(256U , 4))
#define LOSCFG_BASE_CORE_TSK_MONITOR                        0

/* lospriority range: 0~31,
   cmmsis priority = osPriorityNormal + LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO - losPriority,
   when default 15, cmsis priority range: 8~39 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO                   15
#define LOSCFG_BASE_CORE_TIMESLICE                          1
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT                  20000
/*=============================================================================
                        Semaphore module configuration
=============================================================================*/
#define LOSCFG_BASE_IPC_SEM                                 1
#define LOSCFG_BASE_IPC_SEM_LIMIT                           8
/*=============================================================================
                        Mutex module configuration
=============================================================================*/
#define LOSCFG_BASE_IPC_MUX                                 1
#define LOSCFG_BASE_IPC_MUX_LIMIT                           8
/*=============================================================================
                        Queue module configuration
=============================================================================*/
#define LOSCFG_BASE_IPC_QUEUE                               1
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                         8
/*=============================================================================
                        Software timer module configuration
=============================================================================*/
#define LOSCFG_BASE_CORE_SWTMR                              1
#define LOSCFG_BASE_CORE_SWTMR_ALIGN                        1
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                        8
/*=============================================================================
                        Memory module configuration
=============================================================================*/
#define LOSCFG_SYS_EXTERNAL_HEAP                            0
#define LOSCFG_SYS_HEAP_SIZE                                (0x40000U)
#define LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK                1
#define LOSCFG_BASE_MEM_NODE_SIZE_CHECK                     1
#define LOSCFG_MEM_MUL_POOL                                 0
#define OS_SYS_MEM_NUM                                      80
#define LOSCFG_TASK_MEM_USED                                0

/*=============================================================================
                        TestSuite configuration
=============================================================================*/
#define LOSCFG_BACKTRACE_TYPE                               0
#define LOSCFG_KERNEL_BACKTRACE                             0
#define LOSCFG_BACKTRACE_DEPTH                              15
#define LOSCFG_EXC_HARDWARE_STACK_PROTECTION                0
/*=============================================================================
                                       Exception module configuration
=============================================================================*/
#define LOSCFG_PLATFORM_EXC                                 0
/*=============================================================================
                                       system printf configuration
=============================================================================*/
#define LOSCFG_KERNEL_PRINTF                                0

/*=============================================================================
                                       kernel debug hook configuration
=============================================================================*/
#define LOSCFG_DEBUG_HOOK                                   0

/*=============================================================================
                                       core cpup configuration
=============================================================================*/
#define LOSCFG_BASE_CORE_CPUP                               0

/*=============================================================================
                                       kernel signal configuration
=============================================================================*/
#define LOSCFG_KERNEL_SIGNAL                                0

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _TARGET_CONFIG_H */
