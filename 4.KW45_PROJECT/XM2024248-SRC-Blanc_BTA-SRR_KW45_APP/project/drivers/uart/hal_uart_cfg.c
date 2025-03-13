/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_uart_cfg.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_uart_cfg.h"

/*串口初始化结构体数组，开发者需要配置的初始化信息可以在下面进行配置然后调用初始化接口*/
static hal_uart_cfg_st g_uart_cfg_sa[HAL_UART_CHANNEL_NUM] = {
    {HAL_UART_BAUDRATE_9600,HAL_UART_PARITY_NO,HAL_UART_MODE_RX |HAL_UART_MODE_TX,HAL_BIT_ORDER_LSB,HAL_ENABLED_ENABLE},
	{HAL_UART_BAUDRATE_115200,HAL_UART_PARITY_NO,HAL_UART_MODE_RX |HAL_UART_MODE_TX,HAL_BIT_ORDER_LSB,HAL_ENABLED_ENABLE}
};
/**
 * 函 数 介 绍 ：
 *                 获得UART配置表函数
 *
 *  * 参       数
 *                 uartcfg：配置表指针
 * 					len：：长度
 * 返   回  值
 * 备       注 ：
 *
 */
void hal_uart_get_cfg(hal_uart_cfg_st **uartcfg, uint32_t *len)
{
	*len = HAL_UART_CHANNEL_NUM;
	*uartcfg = g_uart_cfg_sa;
}
