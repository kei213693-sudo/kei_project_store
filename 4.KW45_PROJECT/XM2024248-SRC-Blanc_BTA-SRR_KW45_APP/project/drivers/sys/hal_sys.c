/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * @FilePath: hal_sys.c
 * @abstract:
 * @Current_Version: V1.0.0 Chenjianbo
 * @Date: 2023-09-18 10:32:49
 * @History_Version: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_sys.h"
#include "hal_pin.h"

#include "fsl_common.h"
#include "fsl_pm_core.h"
#include "fsl_pm_board.h"
#include "fsl_cmc.h"
#include "fsl_spc.h"
#include "fsl_wuu.h"
#include "fsl_lptmr.h"

#define SYS_SLEEP_S		(10)		//睡眠5秒后，定时唤醒
#define SYS_SPLEEP_TICKS (32000 *SYS_SLEEP_S)

#define APP_SLEEP_CONSTRAINTS                                                                                        \
    9U, PM_RESC_CTCM0_DEEPSLEEP, PM_RESC_CTCM1_DEEPSLEEP, PM_RESC_STCM0_DEEPSLEEP, PM_RESC_STCM1_DEEPSLEEP,          \
        PM_RESC_STCM2_DEEPSLEEP, PM_RESC_STCM3_DEEPSLEEP, PM_RESC_STCM4_DEEPSLEEP, PM_RESC_MAIN_PD_PERI_OPERATIONAL, \
        PM_RESC_WAKE_PD_PERI_ACTIVE

#define APP_DEEP_SLEEP_CONSTRAINTS                                                                          \
     9U, PM_RESC_CTCM0_ACTIVE, PM_RESC_CTCM1_ACTIVE, PM_RESC_STCM0_ACTIVE, PM_RESC_STCM1_ACTIVE, \
        PM_RESC_STCM2_ACTIVE, PM_RESC_STCM3_ACTIVE, PM_RESC_STCM4_ACTIVE,                          \
        PM_RESC_MAIN_PD_PERI_STATE_RETENTION, PM_RESC_WAKE_PD_PERI_OPERATIONAL

#define APP_POWER_DOWN_CONSTRAINTS                                                                          \
    8U, PM_RESC_CTCM0_DEEPSLEEP, PM_RESC_CTCM1_DEEPSLEEP, PM_RESC_STCM0_DEEPSLEEP, PM_RESC_STCM1_DEEPSLEEP, \
        PM_RESC_STCM2_DEEPSLEEP, PM_RESC_STCM3_DEEPSLEEP, PM_RESC_STCM4_DEEPSLEEP,                          \
        PM_RESC_WAKE_PD_PERI_STATE_RETENTION
#define APP_DEEP_POWER_DOWN_CONSTRAINTS 0U

AT_ALWAYS_ON_DATA(pm_handle_t g_pmHndle);
AT_ALWAYS_ON_DATA(uint8_t g_targetPowerMode);
AT_ALWAYS_ON_DATA(uint32_t g_irqMask);

uint32_t m_warmboot_stack_end;

static void hal_board_deinit(void);
static void hal_sys_wakeupsource_init(void);

static status_t APP_EccReInitCallback(pm_event_type_t eventType, uint8_t powerState, void *data);
static status_t APP_ControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data);

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify0) = {
    .notifyCallback = APP_ControlCallback,
    .data           = NULL,
};

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify2) = {
    .notifyCallback = APP_EccReInitCallback,
    .data           = NULL,
};

AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_lptmr0WakeupSource);
AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_PTC6WakeupSource);

void WUU0_IRQHandler(void)
{
	status_t status = PM_HandleWakeUpEvent();
	WUU_ClearExternalWakeupPinFlag(WUU0, 8);
}

//uint32_t m_warmboot_stack_end;
void hal_sys_enabled_irq(hal_enabled_et en)
{
	if(HAL_ENABLED_ENABLE == en)
	{
		EnableGlobalIRQ(g_irqMask);
	}
	else if(HAL_ENABLED_DISABLE == en)
	{
		g_irqMask = DisableGlobalIRQ();
	}
	else
	{

	}
}

status_t APP_EccReInitCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if ((eventType == kPM_EventExitingSleep) && (powerState == PM_LP_STATE_DEEP_POWER_DOWN))
    {
        uint32_t ramAddress = 0x4000000UL;
        while (ramAddress < 0x4004000UL)
        {
            memset((void *)(uint32_t *)ramAddress, 0UL, sizeof(uint32_t));
            ramAddress = ramAddress + 4UL;
        }
        ramAddress = 0x20000000;
        while (ramAddress < 0x20010000)
        {
            memset((void *)(uint32_t *)ramAddress, 0UL, sizeof(uint32_t));
            ramAddress = ramAddress + 4UL;
        }
    }
    return kStatus_Success;
}

status_t APP_ControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if (eventType == kPM_EventEnteringSleep)
    {
    	hal_board_deinit();
    }
    else
    {
        if ((CMC_GetSystemResetStatus(CMC0) & kCMC_WakeUpReset) != 0UL)
        {
            /* Close ISO. */
            SPC_ClearPeriphIOIsolationFlag(SPC0);
        }
    }
    return kStatus_Success;
}

void APP_Lptmr0WakeupService(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}

void APP_StartLptmr(uint64_t timeOutTickes)
{
    const lptmr_config_t DEMO_LPTMR_config = {.timerMode            = kLPTMR_TimerModeTimeCounter,
                                              .pinSelect            = kLPTMR_PinSelectInput_0,
                                              .pinPolarity          = kLPTMR_PinPolarityActiveHigh,
                                              .enableFreeRunning    = false,
                                              .bypassPrescaler      = true,
                                              .prescalerClockSource = kLPTMR_PrescalerClock_2,
                                              .value                = kLPTMR_Prescale_Glitch_0};

    LPTMR_Init(LPTMR0, &DEMO_LPTMR_config);
    LPTMR_SetTimerPeriod(LPTMR0, (uint32_t)timeOutTickes);
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(LPTMR0);
}

void APP_StopLptmr(void)
{
    LPTMR_StopTimer(LPTMR0);
}
/*
 * 函 数 介 绍: 系统初始化，主要是初始化tick
 * 参       数 {*}
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注: 需要调用所有用到的外设初始化函数
 */
hal_result_t hal_sys_tick_init(void)
{
	return HAL_RESULT_SUCCESS;
}

/*
 * 函 数 介 绍: 配置tick时钟
 * 参       数 {*}
 * 返   回  值: null
 * 备       注: 单位hz
 */
void hal_sys_tickclk_config(uint32_t clockhz)
{

}
/*
 * 函 数 介 绍: 系统唤醒
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 接收到外部唤醒源，打开相应外设
 */
void hal_sys_wakeup(void)
{

}

static void hal_board_deinit(void)
{

}
void hal_sys_wakeupsource_disable(void)
{
	WUU_ClearExternalWakeupPinFlag(WUU0, 8);
}

static void hal_sys_wakeupsource_init(void)
{

	//外部中断唤醒
	wuu_external_wakeup_pin_config_t wakekup_config = {0};
	port_pin_config_t port_config = {0};
	gpio_pin_config_t pin_config = {0};

	pin_config.pinDirection = 0;
	pin_config.outputLogic = 0;

	port_config.pullSelect = kPORT_PullUp;
	port_config.pullValueSelect = kPORT_LowPullResistor;
	port_config.slewRate = kPORT_FastSlewRate;
	port_config.passiveFilterEnable = kPORT_PassiveFilterDisable;
	port_config.openDrainEnable = kPORT_OpenDrainDisable;
	port_config.driveStrength = kPORT_LowDriveStrength;
	port_config.driveStrength1 = kPORT_NormalDriveStrength;
	port_config.mux = kPORT_MuxAsGpio;
	port_config.lockRegister = kPORT_UnlockRegister;

	GPIO_PinInit(GPIOC,1,&pin_config);
	PORT_SetPinConfig(PORTC,1,&port_config);

	wakekup_config.edge = kWUU_ExternalPinFallingEdge;
	wakekup_config.event = kWUU_ExternalPinInterrupt;
	wakekup_config.mode = kWUU_ExternalPinActiveDSPD;

	WUU_SetExternalWakeUpPinsConfig(WUU0,8,&wakekup_config);
//	WUU_SetInternalWakeUpModulesConfig(WUU0, 3, kWUU_InternalModuleInterrupt);
	WUU_SetInternalWakeUpModulesConfig(WUU0, 2, kWUU_InternalModuleDMATrigger);
	NVIC_EnableIRQ(WUU0_IRQn);

	//定时中断唤醒
	//PM_InitWakeupSource(&g_lptmr0WakeupSource, PM_WSID_LPTMR0, APP_Lptmr0WakeupService, true);
	//PM_RegisterTimerController(&g_pmHndle, APP_StartLptmr, APP_StopLptmr, NULL, NULL);
}

static void APP_SetConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case PM_LP_STATE_SLEEP:
        {
            PM_SetConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_DEEP_SLEEP:
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_POWER_DOWN:
        {
            PM_SetConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_DEEP_POWER_DOWN:
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        default:
        {
            /* This branch will never be hit. */
            break;
        }
    }
}

static void APP_ReleaseConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case PM_LP_STATE_SLEEP:
        {
            PM_ReleaseConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_SLEEP:
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_POWER_DOWN:
        {
            PM_ReleaseConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_POWER_DOWN:
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        default:
        {
            /* This branch should never be hit. */
            break;
        }
    }
}
/*
 * 函 数 介 绍: 系统睡眠
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 除唤醒源外，将其他外设置为低功耗状态，以节省功耗
 */
void hal_sys_sleep(void)
{
	uint64_t tick = SYS_SPLEEP_TICKS;
	APP_SetConstraints(PM_LP_STATE_DEEP_SLEEP);
	hal_sys_enabled_irq(HAL_ENABLED_DISABLE);
	PM_EnablePowerManager(true);
	PM_EnterLowPower(tick);
	PM_EnablePowerManager(false);
	hal_sys_enabled_irq(HAL_ENABLED_ENABLE);
	APP_ReleaseConstraints(PM_LP_STATE_DEEP_SLEEP);
}

void sys_sleep_init(void)
{
	CMC_EnableDebugOperation(CMC0, false);
	PM_CreateHandle(&g_pmHndle);
	PM_RegisterNotify(kPM_NotifyGroup0, &g_notify0);
	PM_RegisterNotify(kPM_NotifyGroup2, &g_notify2);
	hal_sys_wakeupsource_init();
}

/*
 * 函 数 介 绍: 系统复位
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 执行复位操作
 */
void hal_sys_reset(void)
{
    // to be add user code
	__NVIC_SystemReset();
}
/*
 * 函 数 介 绍: 获取tick值
 * 参       数 {*}
 * 返   回  值: tick值
 * 备       注: 返回毫秒数，1就是1毫秒
 */
uint32_t hal_sys_get_tickcnt(void)
{
	return 0;
}
