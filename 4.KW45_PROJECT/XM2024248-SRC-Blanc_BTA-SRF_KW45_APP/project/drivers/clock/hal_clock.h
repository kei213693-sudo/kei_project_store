/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_clock.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述clock模块抽象层接口，不建议修改函数接口。
 *      文件调用：
 *                  文件应被应hal层内部调用。
 *                  文件应包含hal_clock_typedef.h。
 * 当前版本：
 * 历史版本：
 */
#ifndef HAL_CLOCK_H
#define HAL_CLOCK_H

/*  Include     Begine      */
#include "hal_clock_typedef.h"
/*  Include     End         */

/*
 * 函 数 介 绍: 时钟使能
 * 参       数
 *
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_clock_enabled(hal_clock_module_et module, hal_enabled_et enabled);
/*
 * 函 数 介 绍: 获得时钟状态
 * 参       数
 *              module 时钟模块选择
 * 返   回  值: 0：未使能；1：使能
 * 备       注:
 */
hal_enabled_et hal_clock_getstatus(hal_clock_module_et module);

/*
 * 函 数 介 绍: 获得tick计数
 * 参       数
 *
 * 返   回  值: tick值
 * 备       注:
 */
uint32_t hal_clock_gettick(void);

/*
 * 函 数 介 绍: 时钟初始化
 * 参       数
 *
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_clock_init(void);
/*  Function    Begine      */

/*  Function    End         */

#endif
