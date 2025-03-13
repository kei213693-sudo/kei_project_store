/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_xx.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述xx模块抽象层接口，不建议修改函数接口。
 *      文件调用：
 *                  文件应被应hal层内部调用。
 *                  文件应包含hal_xx_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */
#ifndef HAL_WDT_H
#define HAL_WDT_H

/*  Include     Begine      */
#include "hal_wdt_typedef.h"
/*  Include     End         */

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
hal_result_t hal_wdt_init(uint8_t channel,hal_wdt_speed_et feedspeed);

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
void hal_wdt_feed(uint8_t channel);

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
void hal_wdt_clock(uint8_t channel, hal_wdt_speed_et feedspeed);
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
void hal_wdt_enable(uint8_t channel ,hal_enabled_et enable);
/*  Function    Begine      */

/*  Function    End         */

#endif
