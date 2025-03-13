/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_sys_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_sys_if.h"
#include "hal_sys.h"
/*
 * 函 数 介 绍: 系统初始化，主要是初始化tick
 * 参       数 {*}
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注: 需要调用所有用到的外设初始化函数
 */
hal_result_t hal_sys_if_tick_init(void)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_sys_tick_init();
    return result;
}
/*
 * 函 数 介 绍: 配置tick时钟
 * 参       数 {*}
 * 返   回  值: null
 * 备       注: 单位hz
 */
void hal_sys_if_tickclk_config(uint32_t clockhz)
{
    hal_sys_tickclk_config(clockhz);
}
/*
 * 函 数 介 绍: 系统唤醒
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 接收到外部唤醒源，打开相应外设
 */
void hal_sys_if_wakeup(void)
{
    hal_sys_wakeup();
}
/*
 * 函 数 介 绍: 系统睡眠
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 除唤醒源外，将其他外设置为低功耗状态，以节省功耗
 */
void hal_sys_if_sleep(void)
{
    hal_sys_sleep();
}
/*
 * 函 数 介 绍: 系统复位
 * 参       数 {*}
 * 返   回  值: {*}
 * 备       注: 执行复位操作
 */
void hal_sys_if_reset(void)
{
    hal_sys_reset();
}
/*
 * 函 数 介 绍: 获取tick值
 * 参       数 {*}
 * 返   回  值: tick值
 * 备       注: 返回毫秒数，1就是1毫秒
 */
uint32_t hal_sys_if_get_tickcnt(void)
{
    uint32_t tick_counter = 0;
    tick_counter = hal_sys_if_get_tickcnt();
    return tick_counter;
}
