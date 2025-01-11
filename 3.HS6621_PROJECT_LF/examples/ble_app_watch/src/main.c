/**
 * @file main.c
 * @brief 
 * @date Wed, Sep  5, 2018  5:19:05 PM
 * @author liqiang
 *
 * @addtogroup 
 * @ingroup 
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "rwip_config.h" // RW SW configuration

#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include <stdio.h>
#include "rwip.h"      // RW SW initialization
#include "dbg.h"
#include "peripheral.h"
#include "sysdump.h"
#include "app.h"

#include "hal_uart.h"
#include "hal_flash.h"
#include "hal_wdt.h"
#include "app_ble.h"

#define PIN_KEY_1                    24  // KEY1@EVB，需要接上靠近按键KEY1的跳线帽，按下按键才有效
#define APP_WAKEUP_TIMEOUT          (300)//休眠唤醒超时时间，ms

static co_timer_t g_wakeup_timer;

static uint32_t pin_wakeup_mask = (BIT_MASK(PIN_KEY_1) | BIT_MASK(HAL_UART_LOG_RX_PIN) | BIT_MASK(HAL_UART_FM33LG_RX_PIN));

/**************************************************************
*函数功能描述 ：唤醒超时，强制休眠
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_wakeup_timer_handler(co_timer_t *timer, void *param)
{
    log_debug("lp status:%08X\n",pmu_lowpower_prevent_status());

    if(PMU_LP_UART0 & pmu_lowpower_prevent_status())
    {
        pmu_lowpower_allow(PMU_LP_UART0);
    }

    if(PMU_LP_UART1 & pmu_lowpower_prevent_status())
    {
        pmu_lowpower_allow(PMU_LP_UART1);
    }
}

static void app_lowpower_wakeup_handler(uint32_t pin_mask)
{
    log_debug("wake_gpio: %08X\n", pin_mask);

    if(0 != (pin_mask & BIT_MASK(PIN_KEY_1)))
    {
        log_debug("key1 Wakeup\n");
    }

    if(0 != (pin_mask & BIT_MASK(HAL_UART_LOG_RX_PIN)))
    {
        log_debug("uart log Wakeup\n");
        pmu_lowpower_prevent(PMU_LP_UART0);//先禁止休眠
    }

    if(0 != (pin_mask & BIT_MASK(HAL_UART_FM33LG_RX_PIN)))
    {
        log_debug("uart fm33lg Wakeup\n");
        pmu_lowpower_prevent(PMU_LP_UART1);//先禁止休眠
    }

    co_timer_set(&g_wakeup_timer, APP_WAKEUP_TIMEOUT//等待接收指令的时间长度，暂定300ms
            , TIMER_ONE_SHOT, app_wakeup_timer_handler, NULL);
}

static void gpio_handler(uint32_t pin_mask)
{
    static int counter = 0;
    log_debug("gpio: %08X [%d]\n", pin_mask, counter++);
}

//pmu部分从sleep唤醒后不需要重新初始化
static void pinmux_init(void)
{
    pmu_wakeup_pin_set(pin_wakeup_mask, PMU_PIN_WAKEUP_LOW_LEVEL);
    pmu_wakeup_pin_register_callback(app_lowpower_wakeup_handler, NULL);//sleep唤醒回调注册
}

//通用外设初始化
static void peripheral_init(void)
{
    // Init GPIO
    gpio_open();

    // key1初始化
    pinmux_config(PIN_KEY_1,  PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(BITMASK(PIN_KEY_1), PMU_PIN_MODE_PU);
    gpio_set_direction(BITMASK(PIN_KEY_1), GPIO_INPUT);
    gpio_set_interrupt(BITMASK(PIN_KEY_1), GPIO_BOTH_EDGE);
    gpio_set_interrupt_callback(gpio_handler);//通用gpio中断回调函数

    hal_uart_log_init();
    hal_uart_fm33lg_init();
    hal_flash_init();
	hal_wdt_init();
}

static void hardware_init(void)
{
    pmu_pin_mode_set(0xFFFFFFFF, PMU_PIN_MODE_PU);
    #ifndef CONFIG_GPIO1_ABSENT
    pmu_pin_mode_set_ex(0xFF, PMU_PIN_MODE_PU);
    #endif
	
	pinmux_init();

    peripheral_init();
}

static void power_sleep_event_handler(co_power_sleep_state_t sleep_state, co_power_status_t power_status)
{
    switch(sleep_state)
    {
        case POWER_SLEEP_ENTRY://进入休眠
			log_debug("e\n");
            break;

        case POWER_SLEEP_LEAVE_TOP_HALF:
            //休眠唤醒后重新初始化各类外设
            //GPIO, Timer, UART, I2C, SPI, WDT, Encoder, KPP, I2S,DMA, ADC
            peripheral_init();
            log_debug("h\n");//为什么这个打印不出来？？
            break;

        case POWER_SLEEP_LEAVE_BOTTOM_HALF://退出休眠
            log_debug("l\n");
            break;
    }
}

static void ble_stack_config(void)
{
    //电源使用dcdc模式
	pmu_dcdc_enable(true);
	
    // Power down xtal32k in deep sleep mode
    pmu_32k_enable_in_deep_sleep(false);//失能32k，不使用deep sleep模式

    // Select 32k clock for stack
    //pmu_xtal32k_change_param(15, 1);
	pmu_select_32k(PMU_32K_SEL_32768HZ_XTAL);//外部晶振
    //pmu_select_32k(PMU_32K_SEL_RC);//内部rc震荡

    // ultra sleep mode enable
    co_power_ultra_sleep_mode_enable(false);//不使用ultra deep sleep模式

    pmu_xtal32m_change_param(1);//修改负载电容参数

    // Enable sleep, SWD will be closed.
    //co_power_sleep_enable(false);//失能sleep模式
    co_power_sleep_enable(true);//使能sleep模式，此时无法进入调试状态

    co_power_register_sleep_event(power_sleep_event_handler);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

int main(void)
{
    ble_stack_config();
    hardware_init();
    rwip_init(RESET_NO_ERROR);

	app_ble_init();//先初始化自定义任务
    appm_init();//再初始化ble参数和任务

    log_debug("Version data:%s\n",APP_VERSION_DATE);
	log_debug("Version time:%s\n",APP_VERSION_TIME);
    log_debug("reboot reason:%d\n", pmu_reboot_reason());//0掉电，1深度睡眠唤醒，2ota升级重启，3主动重启，4看门狗重启，5低电压重启
	log_debug("APP start\n");//从上电到这里最少要花1000ms

    while(1)
    {
        rwip_schedule();//包含任务调度和电源管理
    }
}


