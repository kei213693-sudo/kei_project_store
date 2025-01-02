#include "target_config.h"
#include "pmu.h"
#include "los_tick.h"
#include <stdint.h>
#include "los_pmutimer.h"

#if (LOSCFG_USE_SYSTICK == 0)

static UINT32 PmuTickStart(HWI_PROC_FUNC handler);
static UINT64 PmuTickCycleGet(UINT32 *period);
static UINT64 PmuTickReload(UINT64 nextResponseTime);
static void PmuTickLock(void);
static void PmuTickUnlock(void);

static uint32_t g_period;
static uint32_t g_value;

static ArchTickTimer g_pmuTimer = {
    .freq = OS_SYS_CLOCK,
    .irqNum = PMU_TIMER_IRQn,
    .periodMax = LOSCFG_BASE_CORE_TICK_RESPONSE_MAX,
    .init = PmuTickStart,
    .getCycle = PmuTickCycleGet,
    .reload = PmuTickReload,
    .lock = PmuTickLock,
    .unlock = PmuTickUnlock,
    .tickHandler = NULL,
};

static void PmuTimerHandler(void)
{
    if (g_pmuTimer.tickHandler) {
        g_pmuTimer.tickHandler();
    }
}

static UINT32 PmuTickStart(HWI_PROC_FUNC handler)
{
    g_pmuTimer.tickHandler = handler;
    pmu_timer_init();
    pmu_rtos_timer_register(PmuTimerHandler);
    g_period = LOSCFG_BASE_CORE_TICK_RESPONSE_MAX;
    g_value = (uint32_t)pmu_timer_time();
    pmu_rtos_timer_start(g_value + g_period);
    return LOS_OK;
}

static UINT64 PmuTickCycleGet(UINT32 *period)
{
    uint32_t curValue, consumeValue;
    uint32_t intSave = LOS_IntLock();

    *period = (UINT32)g_period;
    curValue = (uint32_t)pmu_timer_time();
    consumeValue = curValue - g_value;
    if (consumeValue >= g_period) {
        consumeValue = 0;
    }
    LOS_IntRestore(intSave);
    return (UINT64)consumeValue;
}

static UINT64 PmuTickReload(UINT64 nextResponseTime)
{
    uint32_t intSave = LOS_IntLock();

    if (nextResponseTime > g_pmuTimer.periodMax) {
        nextResponseTime = g_pmuTimer.periodMax;
    }
    g_period = (uint32_t)nextResponseTime;
    g_value = (uint32_t)pmu_timer_time();
    pmu_rtos_timer_start(g_value + g_period - 1U);
    LOS_IntRestore(intSave);
    return nextResponseTime;
}

static void PmuTickLock(void)
{
    uint32_t intSave = LOS_IntLock();
    pmu_rtos_timer_stop();
    g_value = (uint32_t)pmu_timer_time();
    NVIC_ClearPendingIRQ(PMU_TIMER_IRQn);
    LOS_IntRestore(intSave);
}

static void PmuTickUnlock(void)
{
    uint32_t intSave = LOS_IntLock();
    g_period -= g_value;
    pmu_rtos_timer_start(pmu_timer_time() + g_period - g_value);
    LOS_IntRestore(intSave);
}

void LOS_PmuTimerTickInit(void)
{
    LOS_TickTimerRegister(&g_pmuTimer, OsTickHandler);
}

#endif
