/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_clock.c
 * 摘要：
 *      文件概述：
 *                  文件主要描述clock模块功能实现，需要保证接口函数功能实现，本文件为模板文件，
 *              实现实体需要自行补充。
 *      文件调用：
 *                  文件应包含hal_clock_if.h hal_clock.h hal_clock_cfg.h。
 * 当前版本：
 * 历史版本：
 */

/*  Include     Begine      */
#include "hal_clock.h"
#include "hal_clock_cfg.h"
#include "hal_clock_if.h"
#include "hal_clock_typedef.h"
#include "fsl_ccm32k.h"
#include "fsl_spc.h"
/*  Include     End         */

static const scg_firc_trim_config_t frc_trim_config =
{
    .trimMode = kSCG_FircTrimUpdate,              /* FIRC trim is enabled and trim value update is enabled */
    .trimSrc = kSCG_FircTrimSrcSysOsc,            /* Trim source is System OSC */
    .trimDiv = 31U,                               /* Divided by 32 */
    .trimCoar = 0U,                               /* Trim value, see Reference Manual for more information */
    .trimFine = 0U,                               /* Trim value, see Reference Manual for more information */
};
const scg_sys_clk_config_t sys_clk_config =
{
    .divSlow = (uint32_t)kSCG_SysClkDivBy4,       /* Slow Clock Divider: divided by 4 */
    .divBus = (uint32_t)kSCG_SysClkDivBy1,        /* Bus Clock Divider: divided by 1 */
    .divCore = (uint32_t)kSCG_SysClkDivBy1,       /* Core Clock Divider: divided by 1 */
    .src = (uint32_t)kSCG_SysClkSrcFirc,          /* Fast IRC is selected as System Clock Source */
};
const scg_sosc_config_t sosc_config =
{
    .freq = 32000000U,                            /* System Oscillator frequency: 32000000Hz */
    .monitorMode = kSCG_SysOscMonitorDisable,     /* System OSC Clock Monitor is disabled */
    .enableMode = kSCG_SoscEnable,                /* System OSC Enable */
};
const scg_sirc_config_t sirc_config =
{
    .enableMode = kSCG_SircDisableInSleep,        /* Slow IRC is disabled in sleep modes */
};
const scg_firc_config_t firc_config =
{
    .enableMode = kSCG_FircEnable,                /* Fast IRC is enabled */
    .range = kSCG_FircRange96M,                   /* 96 Mhz FIRC clock selected */
    .trimConfig = &frc_trim_config,
};
static const ccm32k_osc_config_t osc_config =
{
    .coarseAdjustment = kCCM32K_OscCoarseAdjustmentRange0,/* ESR_Range0 */
    .enableInternalCapBank = true,                /* Internal capacitance bank is enabled */
    .xtalCap = kCCM32K_OscXtal8pFCap,             /* 8 pF */
    .extalCap = kCCM32K_OscExtal8pFCap,           /* 8 pF */
};


static void CLOCK_CONFIG_FircSafeConfig(const scg_firc_config_t *fircConfig)
{
    scg_sys_clk_config_t curConfig;
    scg_sys_clk_config_t sysClkSafeConfigSource = {
         .divSlow = (uint32_t)kSCG_SysClkDivBy4, /* Slow clock divider */
         .divCore = (uint32_t)kSCG_SysClkDivBy1, /* Core clock divider */
         .src = (uint32_t)kSCG_SysClkSrcSirc,    /* System clock source */
    };
    /* Change to use SIRC as system clock source to prepare to change FIRCCFG register */
    CLOCK_SetRunModeSysClkConfig(&sysClkSafeConfigSource);
    /* Wait for clock source switch finished */
    do
    {
        CLOCK_GetCurSysClkConfig(&curConfig);
    } while (curConfig.src != sysClkSafeConfigSource.src);

    /* Init Firc */
    (void)CLOCK_InitFirc(fircConfig);
}

static void CLOCK_CONFIG_SetScgOutSel(clock_clkout_src_t setting)
{
     SCG0->CLKOUTCNFG = SCG_CLKOUTCNFG_CLKOUTSEL(setting);
}

/*
 * 函 数 介 绍: 时钟使能
 * 参       数
 *
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_clock_enabled(hal_clock_module_et module, hal_enabled_et enabled)
{
    hal_result_t res = HAL_RESULT_NO_SUPPORT;
#if 0
    switch (module)
    {
    case HAL_CLOCK_MODULE_PLL:
        // to be add user code
        break;
    case HAL_CLOCK_MODULE_MCU:
        // to be add user code
        break;
    case HAL_CLOCK_MODULE_TICK:
        // to be add user code
        break;

    case HAL_CLOCK_MODULE_CAN:
        // to be add user code
        break;

    case HAL_CLOCK_MODULE_SPI:
        // to be add user code
        break;

    case HAL_CLOCK_MODULE_IIC:
        // to be add user code
        break;

    case HAL_CLOCK_MODULE_UART:
        // to be add user code
        break;

    case HAL_CLOCK_MODULE_LIN:
        // to be add user code
        break;

    case HAL_CLOCK_MODULE_ADC:
        // to be add user code
        break;

    case HAL_CLOCK_MODULE_DAC:
        // to be add user code
        break;

    default:
        res = HAL_RESULT_PARAMETER_ERR;
        break;
    }
		#endif
    return res;
}

/*
 * 函 数 介 绍: 获得时钟状态
 * 参       数
 *              module 时钟模块选择
 * 返   回  值: 0：未使能；1：使能
 * 备       注:
 */
hal_enabled_et hal_clock_getstatus(hal_clock_module_et module)
{
    hal_enabled_et enable = HAL_ENABLED_DISABLE;
    // to be add user code
    return enable;
}

/*
 * 函 数 介 绍: 获得tick计数
 * 参       数
 *
 * 返   回  值: tick值
 * 备       注:
 */
uint32_t hal_clock_gettick(void)
{
    // to be add user code
	uint32_t result = 0;
	return result;
}

/*
 * 函 数 介 绍: 时钟初始化
 * 参       数
 *
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_clock_init(void)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
#if 0
    uint32_t coreFreq;
	scg_sys_clk_config_t curConfig;
	spc_active_mode_core_ldo_option_t ldoOption;

	/* Unlock FIRC, SIRC, ROSC and SOSC control status registers */
	CLOCK_UnlockFircControlStatusReg();
	CLOCK_UnlockSircControlStatusReg();
	CLOCK_UnlockRoscControlStatusReg();
	CLOCK_UnlockSysOscControlStatusReg();

	/* Get the CPU Core frequency */
	coreFreq = CLOCK_GetSysClkFreq(kSCG_SysClkCore);

	if (coreFreq <= BOARD_BOOTCLOCKRUN_CORE_CLOCK) {
		/* Set the LDO_CORE VDD regulator level */
		ldoOption.CoreLDOVoltage = kSPC_CoreLDO_NormalVoltage;
		ldoOption.CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength;
		(void)SPC_SetActiveModeCoreLDORegulatorConfig(SPC0, &ldoOption);
		/* Configure Flash to support different voltage level and frequency */
		FMU0->FCTRL = (FMU0->FCTRL & ~((uint32_t)FMU_FCTRL_RWSC_MASK)) | (FMU_FCTRL_RWSC(0x2U));
		/* Specifies the operating voltage for the SRAM's read/write timing margin */
		SPC_SetSRAMOperateVoltage(SPC0, kSPC_SRAM_OperatVoltage1P1V);
	}

	/* Config 32k Crystal Oscillator */
	CCM32K_Set32kOscConfig(CCM32K, kCCM32K_Enable32kHzCrystalOsc, &osc_config);
	/* Monitor is disabled */
	CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
	/* Wait for the 32kHz crystal oscillator to be stable */
	while ((CCM32K_GetStatusFlag(CCM32K) & CCM32K_STATUS_OSC32K_RDY_MASK) == 0UL)
	{
	}
	/* OSC32K clock output is selected as clock source */
	CCM32K_SelectClockSource(CCM32K, kCCM32K_ClockSourceSelectOsc32k);
	/* Disable the FRO32K clock */
	CCM32K_Enable32kFro(CCM32K, false);
	/* Wait for RTC Oscillator to be Valid */
	while (!CLOCK_IsRoscValid())
	{
	}

	CLOCK_SetXtal32Freq(BOARD_BOOTCLOCKRUN_ROSC_CLOCK);

	/* Init FIRC */
	CLOCK_CONFIG_FircSafeConfig(&firc_config);
	/* Set SCG to FIRC mode */
	CLOCK_SetRunModeSysClkConfig(&sys_clk_config);
	/* Wait for clock source switch finished */
	do
	{
		CLOCK_GetCurSysClkConfig(&curConfig);
	} while (curConfig.src != sys_clk_config.src);
	/* Initializes SOSC according to board configuration */
	(void)CLOCK_InitSysOsc(&sosc_config);
	/* Set the XTAL0 frequency based on board settings */
	CLOCK_SetXtal0Freq(sosc_config.freq);
	/* Init SIRC */
	(void)CLOCK_InitSirc(&sirc_config);
	/* Set SystemCoreClock variable */
	SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;

	if (coreFreq > BOARD_BOOTCLOCKRUN_CORE_CLOCK) {
		/* Configure Flash to support different voltage level and frequency */
		FMU0->FCTRL = (FMU0->FCTRL & ~((uint32_t)FMU_FCTRL_RWSC_MASK)) | (FMU_FCTRL_RWSC(0x2U));
		/* Specifies the operating voltage for the SRAM's read/write timing margin */
		SPC_SetSRAMOperateVoltage(SPC0, kSPC_SRAM_OperatVoltage1P1V);
		/* Set the LDO_CORE VDD regulator level */
		ldoOption.CoreLDOVoltage = kSPC_CoreLDO_NormalVoltage;
		ldoOption.CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength;
		(void)SPC_SetActiveModeCoreLDORegulatorConfig(SPC0, &ldoOption);
	}

	/* Set SCG CLKOUT selection. */
	CLOCK_CONFIG_SetScgOutSel(kClockClkoutSelScgSlow);
#else
			scg_sys_clk_config_t curConfig;
			spc_active_mode_core_ldo_option_t ldoOption;

			/* Unlock FIRC, SIRC, ROSC and SOSC control status registers */
			CLOCK_UnlockFircControlStatusReg();
			CLOCK_UnlockSircControlStatusReg();
			CLOCK_UnlockRoscControlStatusReg();
			CLOCK_UnlockSysOscControlStatusReg();

			/* Monitor is disabled */
			CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);

			CLOCK_SetXtal32Freq(BOARD_BOOTCLOCKRUN_ROSC_CLOCK);

			/* Init FIRC */
			CLOCK_CONFIG_FircSafeConfig(&firc_config);
			/* Set SCG to FIRC mode */
			CLOCK_SetRunModeSysClkConfig(&sys_clk_config);
			/* Wait for clock source switch finished */
			do
			{
				CLOCK_GetCurSysClkConfig(&curConfig);
			} while (curConfig.src != sys_clk_config.src);
			/* Initializes SOSC according to board configuration */
			(void)CLOCK_InitSysOsc(&sosc_config);
			/* Set the XTAL0 frequency based on board settings */
			CLOCK_SetXtal0Freq(sosc_config.freq);
			/* Init SIRC */
			(void)CLOCK_InitSirc(&sirc_config);
			/* Set SystemCoreClock variable */
			SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;

			/* Configure Flash to support higher voltage level and frequency */
			FMU0->FCTRL = (FMU0->FCTRL & ~((uint32_t)FMU_FCTRL_RWSC_MASK)) | (FMU_FCTRL_RWSC(0x1U));
			/* Configure SRAM to support lower voltage level */
			SPC_SetSRAMOperateVoltage(SPC0, kSPC_SRAM_OperatVoltage1P0V);
			/* Decrease voltage to new level */
			ldoOption.CoreLDOVoltage = kSPC_CoreLDO_MidDriveVoltage;
			ldoOption.CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength;
			(void)SPC_SetActiveModeCoreLDORegulatorConfig(SPC0, &ldoOption);

			/* Set SCG CLKOUT selection. */
			CLOCK_CONFIG_SetScgOutSel(kClockClkoutSelScgSlow);
#endif
    return result;
}

/* Public Function End              */
