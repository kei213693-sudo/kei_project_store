/*******************************头文件*******************************/
#include "hal_timer.h"
#include "peripheral.h"
#include "hal_uart.h"

/*******************************内部函数的声明*******************************/
static void hal_timer_tim0_handler(void);
static void hal_timer_tim1_handler(void);

/*******************************内部变量定义*******************************/

/*******************************全局变量定义*******************************/
/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：初始化timer
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void hal_timer_init(void)
{
    const tim_config_t timer_cfg =
    {
        .mode = TIM_TIMER_MODE,
        .config.timer =
        {
            .period_us = (HAL_UART_LOG_RX_IRQ_TIMEOUT * 1000),//中断时间，us
            .callback = hal_timer_tim0_handler,
        },
    };

    const tim_config_t timer_cfg1 =
    {
        .mode = TIM_TIMER_MODE,
        .config.timer =
        {
            .period_us = (HAL_UART_FM33LG_RX_IRQ_TIMEOUT *1000),//中断时间，us
            .callback = hal_timer_tim1_handler,
        },
    };

    // Timer init
    tim_init();
    tim_config(HAL_TIMER_TYPE_UART_LOG, &timer_cfg);
    tim_config(HAL_TIMER_TYPE_UART_FM33L, &timer_cfg1);
}

/**************************************************************
*函数功能描述 ：中断服务函数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void hal_timer_tim0_handler(void)
{
    //处理uart0
    hal_uart_log_RxIrqCheck();
}

/**************************************************************
*函数功能描述 ：中断服务函数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void hal_timer_tim1_handler(void)
{
    //处理uart1
    hal_uart_fm33lg_RxIrqCheck();
}
