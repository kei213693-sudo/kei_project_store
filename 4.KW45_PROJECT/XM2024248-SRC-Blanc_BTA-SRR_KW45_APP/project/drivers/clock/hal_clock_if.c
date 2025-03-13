/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_clock_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_clock_if.h"
#include "hal_clock.h"
#include "board.h"

/*
 * 函 数 介 绍: 时钟初始化
 * 参       数
 *
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_clock_if_init(void)
{
	hal_result_t result = HAL_RESULT_SUCCESS;
	BOARD_InitHardware();
    return result;
}

/*
 * 函 数 介 绍: 时钟使能
 * 参       数
 *
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_clock_if_enabled(hal_clock_module_et module, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_clock_enabled(module, enabled);
    return result;
}
/*
 * 函 数 介 绍: 获得时钟状态
 * 参       数
 *              module 时钟模块选择
 * 返   回  值: 0：未使能；1：使能
 * 备       注:
 */
hal_enabled_et hal_clock_if_getstatus(hal_clock_module_et module)
{
    hal_enabled_et enable = HAL_ENABLED_ENABLE;
    enable = hal_clock_getstatus(module);
    return enable;
}
/*
 * 函 数 介 绍: 获得tick计数
 * 参       数
 *
 * 返   回  值: tick值
 * 备       注:
 */
uint32_t hal_clock_if_gettick(void)
{
    uint32_t tick_val = 0;
    tick_val = hal_clock_gettick();
    return tick_val;
}

