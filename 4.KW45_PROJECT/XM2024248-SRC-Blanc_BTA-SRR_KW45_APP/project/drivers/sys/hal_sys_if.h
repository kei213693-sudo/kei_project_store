/***
 * @Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * @All rights reserved.
 * @FilePath: hal_sys_if.h
 * @abstract:
 * @Current_Version:
 * @History_Version: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#ifndef HAL_SYS_IF_H
#define HAL_SYS_IF_H
#include "hal_typedef.h"

/*
 * 函 数 介 绍: 系统初始化，主要是初始化tick
 * 参       数 {*}
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注: 需要调用所有用到的外设初始化函数
 */
hal_result_t hal_sys_if_tick_init(void);
/*
 * 函 数 介 绍: 配置tick时钟
 * 参       数 {*}
 * 返   回  值: null
 * 备       注: 单位hz
 */
void hal_sys_if_tickclk_config(uint32_t clockhz);
/*
 * 函 数 介 绍: 系统唤醒
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 接收到外部唤醒源，打开相应外设
 */
void hal_sys_if_wakeup(void);
/*
 * 函 数 介 绍: 系统睡眠
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 除唤醒源外，将其他外设置为低功耗状态，以节省功耗
 */
void hal_sys_if_sleep(void);
/*
 * 函 数 介 绍: 系统复位
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 执行复位操作
 */
void hal_sys_if_reset(void);
/*
 * 函 数 介 绍: 获取tick值
 * 参       数 {*}
 * 返   回  值: tick值
 * 备       注:
 */
uint32_t hal_sys_if_get_tickcnt(void);

#endif
