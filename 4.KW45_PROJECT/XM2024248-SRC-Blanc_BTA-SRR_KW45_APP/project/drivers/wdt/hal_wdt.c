/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_xx.c
 * 摘要：
 *      文件概述：
 *                  文件主要描述xx模块功能实现，需要保证接口函数功能实现，本文件为模板文件，
 *              实现实体需要自行补充。
 *      文件调用：
 *                  文件应包含hal_xx_if.h hal_xx.h hal_xx_cfg.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

/*  Include     Begine      */
#include "hal_wdt.h"
#include "hal_wdt_cfg.h"
#include "hal_wdt_if.h"
#include "hal_wdt_typedef.h"
#include "fsl_wdog32.h"
/*  Include     End         */

static WDOG_Type * wdt_array[HAL_WDT_NUM] = {WDOG0,WDOG1};
AT_QUICKACCESS_SECTION_DATA(static wdog32_config_t config[HAL_WDT_NUM]);

/**
 * 函 数 介 绍 ：
 *                 wdt模块初始化
 *
 * 参       数 ：
 *                  [in] channel_e      WDT通道，可配置HAL_WDT_CHANNEL0、HAL_WDT_CHANNEL1.....
 *                  [in] feedspeed      喂狗速率
 * 返   回  值 ：
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_wdt_init(uint8_t channel, hal_wdt_speed_et feedspeed)
{
	hal_result_t result = HAL_RESULT_SUCCESS;
	if(channel < HAL_WDT_NUM)
	{
		/* When system is boot up, WDOG32 is disabled. We must wait for at least 2.5
		 * periods of wdog32 clock to reconfigure wodg32. So Delay a while to wait for
		 * the previous configuration taking effect. */
		__NOP();
		__NOP();
		__NOP();

		WDOG32_GetDefaultConfig(&config[channel]);
		config[channel].clockSource = kWDOG32_ClockSource0;
		config[channel].prescaler = kWDOG32_ClockPrescalerDivide256;
		//0.01075ms
		if(HAL_WDT_SPEED_FAST == feedspeed)
		{
			config[channel].timeoutValue = 9300;	//100ms
		}
		else if(HAL_WDT_SPEED_MEDI == feedspeed)
		{
			config[channel].timeoutValue = 23250;	//250ms
		}
		else if(HAL_WDT_SPEED_SLOW == feedspeed)
		{
			config[channel].timeoutValue = 46500;	//500ms
		}
		else
		{
			config[channel].timeoutValue = 65535;
		}
		WDOG32_Init(wdt_array[channel], &config[channel]);
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
	return result;
}

/**
 * 函 数 介 绍 ：
 *                 喂狗
 *
 * 参       数 ：
 *                  [in] channel    WDT通道，可配置HAL_WDT_CHANNEL0、HAL_WDT_CHANNEL1.....
 * 返   回  值 ：
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
void hal_wdt_feed(uint8_t channel)
{
	 WDOG32_Refresh(wdt_array[channel]);
}

/**
 * 函 数 介 绍 ：
 *                 wdt配置时钟信息
 *
 * 参       数 ：
 *                  [in] channel            WDT通道，可配置HAL_WDT_CHANNEL0、HAL_WDT_CHANNEL1.....
 *                  [in] feedspeed          看门狗复位速率
 * 返   回  值 ：
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
void hal_wdt_clock(uint8_t channel, hal_wdt_speed_et feedspeed)
{

}
/**
 * 函 数 介 绍 ：
 *                 配置WDT是否使能
 *
 * 参       数 ：
 *                  [in] channel            WDT通道，可配置HAL_WDT_CHANNEL0、HAL_WDT_CHANNEL1.....
 *                  [in] enable             看门狗使能标签
 * 返   回  值 ：
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
void hal_wdt_enable(uint8_t channel, hal_enabled_et enable)
{
	if(channel < HAL_WDT_NUM)
	{
		WDOG32_Unlock(wdt_array[channel]);
		if(HAL_ENABLED_ENABLE == enable)
		{

		}
		else if(HAL_ENABLED_DISABLE == enable)
		{
			WDOG32_Deinit(wdt_array[channel]);
		}
		else
		{

		}
	}
	else
	{

	}
}

/* Public Function End              */
