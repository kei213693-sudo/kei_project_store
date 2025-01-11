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


/*********************************************************************
 * MACROS
 */

#ifdef CONFIG_ASIC
#define PIN_WAKEUP_1                    24  // KEY1@EVB，需要接上靠近按键KEY1的跳线帽，按下按键才有效
#endif

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */

static uint32_t pin_wakeup_mask = (BIT_MASK(PIN_WAKEUP_1));

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void gpio_handler(uint32_t pin_mask)
{
    static int counter = 0;
    log_debug("gpio: %08X [%d]\n", pin_mask, counter++);
}

static void wakeup_gpio_handler(uint32_t pin_mask)
{
    static int counter = 0;
    log_debug("wake_gpio: %08X [%d]\n", pin_mask, counter++);
}

//gpio pmu部分从sleep唤醒后不需要重新初始化
static void pinmux_init(void)
{
    // KEY1
    pinmux_config(PIN_WAKEUP_1,  PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(pin_wakeup_mask, PMU_PIN_MODE_PU);
}

static void peripheral_init(void)
{
    // Init GPIO
    gpio_open();

    // Wakeup Pin
    gpio_set_direction(pin_wakeup_mask, GPIO_INPUT);
    gpio_set_interrupt(pin_wakeup_mask, GPIO_BOTH_EDGE);
    gpio_set_interrupt_callback(gpio_handler);

    // wakeup
    pmu_wakeup_pin_set(pin_wakeup_mask, PMU_PIN_WAKEUP_LOW_LEVEL);
    pmu_wakeup_pin_register_callback(wakeup_gpio_handler, NULL);//sleep唤醒回调注册

    hal_uart_log_init();
    hal_uart_fm33lg_init();
    hal_flash_init();
	hal_wdt_init();
}

//休眠唤醒后重新初始化各类外设
//GPIO, Timer, UART, I2C, SPI, WDT, Encoder, KPP, I2S,DMA, ADC
static void peripheral_restore(void)
{
    // Init UART
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

    peripheral_init();//外设
}

static void power_sleep_event_handler(co_power_sleep_state_t sleep_state, co_power_status_t power_status)
{
    switch(sleep_state)
    {
        case POWER_SLEEP_ENTRY://进入休眠
			log_debug("e\n");
            break;

        case POWER_SLEEP_LEAVE_TOP_HALF:
            peripheral_restore();
            break;

        case POWER_SLEEP_LEAVE_BOTTOM_HALF://退出休眠
            log_debug("l\n");
            break;
    }
}

//状态改变后就会进入这个回调
co_power_status_t power_sleep_status_handler(void)
{
    co_power_status_t pmu_status;

    pmu_status = pmu_power_status();//直接查询状态值

//    return MIN(POWER_SLEEP, pmu_status);
    return pmu_status;
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
    co_power_sleep_enable(false);//失能sleep模式
    //co_power_sleep_enable(true);//使能sleep模式，此时无法进入调试状态
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

int main(void)
{
    ble_stack_config();
    hardware_init();
    rwip_init(RESET_NO_ERROR);
    //co_power_register_user_status(power_sleep_status_handler);
    co_power_register_sleep_event(power_sleep_event_handler);

	app_ble_init();//先初始化自定义任务
    appm_init();//再初始化ble协议参数

    log_debug("Version data:%s\n",APP_VERSION_DATE);
	log_debug("Version time:%s\n",APP_VERSION_TIME);
    log_debug("reboot reason:%d\n", pmu_reboot_reason());//0掉电，1深度睡眠唤醒，2ota升级重启，3主动重启，4看门狗重启，5低电压重启
	log_debug("APP start\n");//从上电到这里最少要花1000ms

    while(1)
    {
        rwip_schedule();//包含任务调度和电源管理
    }
}


