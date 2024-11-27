/**
 ****************************************************************************************************
 * @file        atk_ble03.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-06-01
 * @brief       ATK-BLE03模块驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 MiniSTM32 V4开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 基本参数说明：
 * 支持BLE5.1 + SPP2.1双模，但是同时只能使用一种，功耗0.1uA~9mA
 * ble通信距离40m，速度20kb/s；spp通信距离80m，速度40kb/s
 * 只能做slave，广播名称默认"BLE03_BLE/SPP"，广播状态蓝灯闪烁，连接状态常量
 * 一个服务，三个特征值
 * mtu默认20，最多支持512，根据对端设备指令最终决定
 * 硬件控制该模组的启用和关闭接口是ATK_BLE03_LDO（）
 ****************************************************************************************************
 */

#include "atk_ble03.h"
#include "delay.h"
#include <string.h>
#include <stdio.h>


/**
 * @brief       ATK-BLE03硬件初始化
 * @param       无
 * @retval      无
 */
void atk_ble03_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* 使能时钟 */
    ATK_BLE03_STA_GPIO_CLK_ENABLE();
    ATK_BLE03_LDO_GPIO_CLK_ENABLE();
    
    /* 初始化STA引脚 */
    gpio_init_struct.Pin    = ATK_BLE03_STA_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLDOWN;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_BLE03_STA_GPIO_PORT, &gpio_init_struct);
    
    /* 初始化LDO引脚 */
    gpio_init_struct.Pin    = ATK_BLE03_LDO_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_BLE03_LDO_GPIO_PORT, &gpio_init_struct);
    
    ATK_BLE03_LDO(1);/*开启LDO供电*/
}

/**
 * @brief       获取ATK-BLE03连接状态
 * @param       无
 * @retval      ATK_BLE03_CONNECTED   : 已连接
 *              ATK_BLE03_DISCONNECTED: 未连接
 */
atk_ble03_conn_sta_t atk_ble03_get_conn_sta(void)
{
    if (ATK_BLE03_READ_STA() != 0)
    {
        return ATK_BLE03_CONNECTED;    
    }
    
    return ATK_BLE03_DISCONNECTED;
}

/**
 * @brief       ATK-BLE03发送AT指令
 * @param       cmd    : 待发送的AT指令
 *              ack    : 等待的响应
 *              timeout: 等待超时时间
 * @retval      ATK_BLE03_EOK     : 函数执行成功
 *              ATK_BLE03_ETIMEOUT: 等待期望应答超时，函数执行失败
 */
uint8_t atk_ble03_send_at_cmd(char *cmd, char *ack, uint32_t timeout)
{
    uint8_t *ret = NULL;
    
    atk_ble03_uart_rx_restart();
    atk_ble03_uart_printf("%s\r\n", cmd);
    
    if ((ack == NULL) || (timeout == 0))
    {
        return ATK_BLE03_EOK;
    }
    else
    {
        while (timeout > 0)
        {
            ret = atk_ble03_uart_rx_get_frame();
            if (ret != NULL)
            {
                if (strstr((const char *)ret, ack) != NULL)
                {
                    return ATK_BLE03_EOK;
                }
                else
                {
                    atk_ble03_uart_rx_restart();
                }
            }
            timeout--;
            delay_ms(1);
        }
        
        return ATK_BLE03_ETIMEOUT;
    }
}

