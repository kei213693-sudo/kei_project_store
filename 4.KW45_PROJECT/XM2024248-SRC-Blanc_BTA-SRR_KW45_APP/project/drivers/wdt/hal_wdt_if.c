/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_wdt_if.c
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2023-11-24
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_wdt_if.h"
#include "hal_wdt.h"
/**
 * 函 数 介 绍 ：
 *                 wdt模块初始化
 *
 *  * 参       数 ：
 *                  [in] channel_e      WDT通道，可配置HAL_WDT_CHANNEL0、HAL_WDT_CHANNEL1.....
 *                  [in] feedspeed      喂狗速率
 * 返   回  值 ：
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_wdt_if_init(uint8_t channel,hal_wdt_speed_et feedspeed)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_wdt_init(channel,feedspeed);
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 喂狗
 *
 *  * 参       数 ：
 *                  [in] channel    WDT通道，可配置HAL_WDT_CHANNEL0、HAL_WDT_CHANNEL1.....
 * 返   回  值 ：
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
void hal_wdt_if_feed(uint8_t channel)
{
    hal_wdt_feed(channel);
}

/**
 * 函 数 介 绍 ：
 *                 wdt配置时钟信息
 *
 *  * 参       数 ：
 *                  [in] channel            WDT通道，可配置HAL_WDT_CHANNEL0、HAL_WDT_CHANNEL1.....
 *                  [in] feedspeed          看门狗复位速率
 * 返   回  值 ：
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
void hal_wdt_if_clock(uint8_t channel, hal_wdt_speed_et feedspeed)
{
    hal_wdt_clock(channel,feedspeed);
}
/**
 * 函 数 介 绍 ：
 *                 配置WDT是否使能
 *
 *  * 参       数 ：
 *                  [in] channel            WDT通道，可配置HAL_WDT_CHANNEL0、HAL_WDT_CHANNEL1.....
 *                  [in] enable             看门狗使能标签
 * 返   回  值 ：
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
void hal_wdt_if_enable(uint8_t channel ,hal_enabled_et enable)
{
    hal_wdt_enable(channel,enable);
}
