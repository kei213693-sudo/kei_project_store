/***
 * @Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * @All rights reserved.
 * @FilePath: hal_sys.h
 * @abstract:  system configuration function header file implementation
 * @Current_Version: V1.0.0 Chenjianbo
 * @Date: 2023-09-18 10:33:01
 * @History_Version: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#ifndef HAL_SYS_H
#define HAL_SYS_H

#include "hal_sys_type.h"
/*
 * 函 数 介 绍: 系统初始化，主要是初始化tick
 * 参       数 {*}
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注: 需要调用所有用到的外设初始化函数
 */
hal_result_t hal_sys_tick_init(void);
/*
 * 函 数 介 绍: 配置tick时钟
 * 参       数
 *              [in]clockhz             指定tick时钟频率，单位hz
 * 返   回  值: null
 * 备       注: 单位hz
 */
void hal_sys_tickclk_config(uint32_t clockhz);
/*
 * 函 数 介 绍: 系统唤醒
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 接收到外部唤醒源，打开相应外设
 */
void hal_sys_wakeup(void);
/*
 * 函 数 介 绍: 系统睡眠
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 除唤醒源外，将其他外设置为低功耗状态，以节省功耗
 */
void hal_sys_sleep(void);
/*
 * 函 数 介 绍: 系统复位
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 执行复位操作
 */
void hal_sys_reset(void);
/*
 * 函 数 介 绍: 获取tick值
 * 参       数 {*}
 * 返   回  值: tick值
 * 备       注: 返回毫秒数，1就是1毫秒
 */
uint32_t hal_sys_get_tickcnt(void);

/*
 * 函 数 介 绍: 全局中断使能函数
 * 参       数 {*}
 *              en:使能状态
 * 返   回  值:
 * 备       注:
 */
void hal_sys_enabled_irq(hal_enabled_et en);
void sys_sleep_init(void);
void hal_sys_wakeupsource_disable(void);
#endif
