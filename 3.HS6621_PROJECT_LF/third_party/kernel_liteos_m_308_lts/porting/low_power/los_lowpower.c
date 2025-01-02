#include "target_config.h"
#include "pmu.h"
#include "los_tick.h"
#include "los_sched.h"
#include "los_pm.h"
#include "los_pmutimer.h"


#if (LOSCFG_USE_SYSTICK == 0 && LOSCFG_KERNEL_PM == 1)
/* Constants required to manipulate the VFP. */
#define LOS_UP_FPCCR                                ( ( volatile uint32_t * ) 0xe000ef34UL ) /* Floating point context control register. */
#define LOS_UP_ASPEN_AND_LSPEN_BITS                 ( 0x3UL << 30UL )

#define LOS_UP_NVIC_SHPR3_REG                       ( *( ( volatile uint32_t * ) 0xe000ed20 ) )

/* The lowest interrupt priority that can be used in a call to a "set priority" function. */
#define LOS_UP_LIBRARY_LOWEST_INTERRUPT_PRIORITY    15
#define LOS_UP_PRIO_BITS                            4
#define LOS_UP_KERNEL_INTERRUPT_PRIORITY            ( LOS_UP_LIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - LOS_UP_PRIO_BITS) )
#define LOS_UP_NVIC_PENDSV_PRI                      ( ( ( uint32_t ) LOS_UP_KERNEL_INTERRUPT_PRIORITY ) << 16UL )

STATIC UINT32 LOS_LpSleep(VOID)
{
    UINT32 intSave = LOS_IntLock();
    co_power_manage_rtos();
    /* run here, it has waked */
    LOS_UP_NVIC_SHPR3_REG |= LOS_UP_NVIC_PENDSV_PRI;
    /* Enable VFP by enable CP10 and CP11 coprocessors */
    SCB->CPACR |= (0xf << 20);
    *( LOS_UP_FPCCR ) |= LOS_UP_ASPEN_AND_LSPEN_BITS;
    LOS_IntRestore(intSave);
    return LOS_OK;
}

STATIC LosPmSysctrl g_pmuSysctrl = {
    .early = NULL,
    .late = NULL,
    .normalSuspend = LOS_LpSleep,
    .normalResume = NULL,
    .lightSuspend = LOS_LpSleep,
    .lightResume = NULL,
    .deepSuspend = LOS_LpSleep,
    .deepResume = NULL,
    .shutdownSuspend = NULL,
    .shutdownResume = NULL,
};

UINT32 LOS_LpInit(void)
{
    if (LOS_PmRegister(LOS_PM_TYPE_SYSCTRL, &g_pmuSysctrl) != LOS_OK) {
        return LOS_NOK;
    }
    return LOS_OK;
}


#endif
