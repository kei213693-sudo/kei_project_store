#ifndef _LOS_UP_PMU_TIMER_
#define _LOS_UP_PMU_TIMER_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#include "los_compiler.h"

#define PMU_TIMER_TICK_COMPENSATE   5U

extern void LOS_PmuTimerTickInit(void);
extern BOOL LOS_PmuTimerTickExceed(void);
extern UINT32 LOS_PmuTimerGetLoadVal(UINT32 *period);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_UP_PMU_TIMER_ */
