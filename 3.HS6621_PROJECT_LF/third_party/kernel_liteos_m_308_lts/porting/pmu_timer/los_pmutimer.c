#include "los_pmutimer.h"

#if (LOSCFG_USE_SYSTICK == 0)
#include "los_tick.h"
#include "los_sched.h"
#include "los_context.h"
#include "los_arch_interrupt.h"
#include "los_debug.h"

STATIC UINT32 g_period;
STATIC UINT32 g_value;
STATIC VOID (*g_tickHandler)(VOID);

STATIC void pmuTimerHandler(void)
{
    if (g_tickHandler) {
        g_tickHandler();
    }
}

UINT32 HalTickStart(OS_TICK_HANDLER handler)
{
    if ((OS_SYS_CLOCK == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return LOS_ERRNO_TICK_CFG_INVALID;
    }

#if (LOSCFG_USE_SYSTEM_DEFINED_INTERRUPT == 1)
#if (OS_HWI_WITH_ARG == 1)
    OsSetVector(PMU_TIMER_IRQn, (HWI_PROC_FUNC)handler, NULL);
#else
    OsSetVector(PMU_TIMER_IRQn, (HWI_PROC_FUNC)handler);
#endif
#endif

    g_sysClock = OS_SYS_CLOCK;
    g_cyclesPerTick = OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND;

    // Init PMU RTOS timer and close Systick
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    g_tickHandler = handler;
    pmu_timer_init();
    pmu_rtos_timer_register(pmuTimerHandler);
    g_period = LOSCFG_BASE_CORE_TICK_RESPONSE_MAX;
    g_value = (UINT32)pmu_timer_time();
    pmu_rtos_timer_start(g_value + g_period);

    return LOS_OK;
}

VOID HalSysTickReload(UINT64 nextResponseTime)
{
    pmu_rtos_timer_stop();
    g_value = (UINT32)pmu_timer_time();
    g_period = (UINT32)nextResponseTime;
    pmu_rtos_timer_start(g_value + g_period - 1U);
}

UINT64 HalGetTickCycle(UINT32 *period)
{
    UINT32 curValue, consumeValue;
    UINT32 intSave = LOS_IntLock();

    curValue = pmu_timer_time();
    if (period) {
        *period = (UINT32)g_period;
    }
    consumeValue = curValue - g_value;
    if (consumeValue >= g_period) {
        consumeValue = 0;
    }
    LOS_IntRestore(intSave);
    return consumeValue;
}

VOID HalTickLock(VOID)
{
    g_value = pmu_timer_time() - g_value;
    pmu_rtos_timer_stop();
}

VOID HalTickUnlock(VOID)
{
    HalSysTickReload(g_period - g_value);
}

#endif /* _LOS_UP_PMU_TIMER_ */
