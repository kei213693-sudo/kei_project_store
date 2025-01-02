/**
 * @file os_main.c
 * @brief 
 * @date Thu 03 Dec 2020 04:58:51 PM CST
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

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "lvgl.h"
#include "lv_port_hs662x.h"
#include "flash_outside.h"
#include "lv_om_animation.h"
/*********************************************************************
 * MACROS
 */

// IO PIN {{{
#define PIN_WAKEUP_0                    2   ///< UART-RTS@EVB
#define PIN_WAKEUP_1                    24  ///< KEY1@EVB
#define PIN_LED_0                       8   ///< LED 0
#define PIN_LED_1                       9   ///< LED 1
#define PIN_LED_2                       14  ///< LED 2
#define PIN_LED_3                       14  ///< LED 3
#define PIN_LED_4                       13  ///< LED 4
#define PIN_LED_5                       14  ///< LED 5
#define PIN_LED_6                       14  ///< LED 6
#define PIN_LED_7                       14  ///< LED 7
#define PIN_UART0_TX                    7   ///< UART0 TX
#define PIN_UART1_TX                    5   ///< UART1 TX
#define PIN_UART1_RX                    6   ///< UART1 RX
#define TEST_PIN_0  10
#define TEST_PIN_1  16
#define TEST_PIN_2  17
#define TEST_PIN_3  18
#define TEST_PIN_4  19

#define DEBUG_UART_BAUDRATE             115200 ///< debug uart baudrate

/// Wakeup pin
static uint32_t pin_wakeup_mask = (BIT_MASK(PIN_WAKEUP_0) | BIT_MASK(PIN_WAKEUP_1));

/// LED pin
static uint32_t pin_leds_mask   = (BIT_MASK(PIN_LED_0)|BIT_MASK(PIN_LED_1)|
                                   BIT_MASK(TEST_PIN_0)|BIT_MASK(TEST_PIN_1)| BIT_MASK(TEST_PIN_2)| BIT_MASK(TEST_PIN_3)| BIT_MASK(TEST_PIN_4)|
                                   BIT_MASK(PIN_LED_2)|BIT_MASK(PIN_LED_3)|
                                   BIT_MASK(PIN_LED_4)|BIT_MASK(PIN_LED_5)|
                                   BIT_MASK(PIN_LED_6)|BIT_MASK(PIN_LED_7));

extern void lv_demo_start(void);

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */


/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */
void vStartBluetoothTask(void);

/**
 * @brief  gpio handler
 *
 * @param[in] pin_mask  pin mask
 **/
static void gpio_handler(uint32_t pin_mask)
{
    static int counter = 0;
    log_debug("gpio: %08X [%d]\n", pin_mask, counter++);
}

/**
 * @brief  wakeup gpio handler
 *
 * @param[in] pin_mask  pin mask
 **/
static void wakeup_gpio_handler(uint32_t pin_mask)
{
    static int counter = 0;
    log_debug("wake_gpio: %08X [%d]\n", pin_mask, counter++);
}

/**
 * @brief  pinmux init
 **/
static void pinmux_init(void)
{
    // KEY
    pinmux_config(PIN_WAKEUP_0,  PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_WAKEUP_1,  PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(pin_wakeup_mask, PMU_PIN_MODE_PU);

    // UART0
    pinmux_config(PIN_UART0_TX, PINMUX_UART0_SDA_O_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART0_TX), PMU_PIN_MODE_PP);
    // UART1
    pinmux_config(PIN_UART1_TX,  PINMUX_UART1_SDA_O_CFG);
    pinmux_config(PIN_UART1_RX,  PINMUX_UART1_SDA_I_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART1_TX), PMU_PIN_MODE_PP);
    pmu_pin_mode_set(BITMASK(PIN_UART1_RX), PMU_PIN_MODE_PU);

    // LEDS
    pinmux_config(PIN_LED_0, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_1, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_2, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_3, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_4, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_5, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_6, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_7, PINMUX_GPIO_MODE_CFG);
    pinmux_config(TEST_PIN_1, PINMUX_GPIO_MODE_CFG);
    pinmux_config(TEST_PIN_0, PINMUX_GPIO_MODE_CFG);
    pinmux_config(TEST_PIN_2, PINMUX_GPIO_MODE_CFG);
    pinmux_config(TEST_PIN_3, PINMUX_GPIO_MODE_CFG);
    pinmux_config(TEST_PIN_4, PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(pin_leds_mask, PMU_PIN_MODE_PP);
}

/**
 * @brief  peripheral init
 **/
static void peripheral_init(void)
{
    // Init GPIO
    gpio_open();

    // Wakeup Pin
    gpio_set_direction(pin_wakeup_mask, GPIO_INPUT);
    gpio_set_interrupt(pin_wakeup_mask, GPIO_BOTH_EDGE);
    gpio_set_interrupt_callback(gpio_handler);

    // LEDS
    gpio_write(pin_leds_mask, GPIO_LOW);
    gpio_set_direction(pin_leds_mask, GPIO_OUTPUT);

    // wakeup
    pmu_wakeup_pin_set(pin_wakeup_mask, PMU_PIN_WAKEUP_LOW_LEVEL);
    pmu_wakeup_pin_register_callback(wakeup_gpio_handler, wakeup_gpio_handler);

    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
}

/**
 * @brief  peripheral restore
 **/
static void peripheral_restore(void)
{
    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
}

/**
 * @brief  hardware init
 **/
static void hardware_init(void)
{
    pmu_pin_mode_set(0xFFFFFFFF, PMU_PIN_MODE_PU);
#ifndef CONFIG_GPIO1_ABSENT
    pmu_pin_mode_set_ex(0xFF, PMU_PIN_MODE_PU);
#endif

    pinmux_init();

    peripheral_init();
}

/**
 * @brief  power sleep event handler
 *
 * @param[in] sleep_state  sleep state
 * @param[in] power_status  power status
 **/
static void power_sleep_event_handler(co_power_sleep_state_t sleep_state, co_power_status_t power_status)
{
    switch(sleep_state)
    {
        case POWER_SLEEP_ENTRY:
//            log_debug("sleep entry\n");
            break;

        case POWER_SLEEP_LEAVE_TOP_HALF:
            peripheral_restore();
            break;

        case POWER_SLEEP_LEAVE_BOTTOM_HALF:
//            log_debug("sleep leave\n");
            break;
    }
}

/**
 * @brief  system config
 **/
static void system_config(void)
{
    // Disable WDT
    wdt_enable(0);

    // Enable DCDC
    pmu_dcdc_enable(true);

    // Power down xtal32k in deep sleep mode
    pmu_32k_enable_in_deep_sleep(false);

    // Select 32k clock for stack
    //pmu_xtal32k_change_param(15, 1);
    pmu_select_32k(PMU_32K_SEL_RC);

    // xtal32m param
    pmu_xtal32m_change_param(15);

    // ultra sleep mode enable
    co_power_ultra_sleep_mode_enable(false);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(true);
}

/**
 * @brief  v timer callback
 *
 * @param[in] pxTimer  px timer
 **/
static void vTimerCallback( TimerHandle_t pxTimer )
{
    // Optionally do something if the pxTimer parameter is NULL.
    configASSERT( pxTimer );
}

/**
 * @brief  test schedule task
 *
 * @param[in] pvParameters  pv parameters
 **/
static void vTestTask(void *pvParameters)
{
    TimerHandle_t xTimer;

    xTimer = xTimerCreate("Timer",              // Just a text name, not used by the kernel.
                          portTICK_PERIOD_MS_N(2000), // The timer period in ticks.
                          pdTRUE,               // The timers will auto-reload themselves when they expire.
                          ( void * ) 1,         // Assign each timer a unique id equal to its array index.
                          vTimerCallback);      // Each timer calls the same callback when it expires.
    xTimerStart(xTimer, 0 );

    for (;;)
    {
#if (CONFIG_LVGL)
        lv_task_handler();
#endif
        vTaskDelay(portTICK_PERIOD_MS_N(1));
    }
}

/**
 * @brief  v start test task
 **/
static void vStartTestTask(void)
{
    // Create ble Task
    xTaskCreate(vTestTask, "TEST", configMINIMAL_STACK_SIZE+512, NULL, configMAX_PRIORITIES, NULL);
}
#include "glcd.h"
/*********************************************************************
 * PUBLIC FUNCTIONS
 */
   lv_obj_t * img;

int main(void)
{
    cpm_set_clock_div(CPM_SF0_CLK, 2);
    cpm_set_clock_div(CPM_SF1_CLK, 2);
    cpm_set_clock_div(CPM_SF2_CLK, 2);
    cpm_set_clock_div(CPM_CPU_CLK, 1);
    pmu_pll_startup(128);
    // system config
    system_config();

    // hardware init
    hardware_init();

    // BLE stack init
    rwip_init(RESET_NO_ERROR);

    // sleep event register
    co_power_register_sleep_event(power_sleep_event_handler);

    // log
    log_debug("running %d\n", pmu_reboot_reason());
    pmu_lowpower_prevent(PMU_LP_USER);
    flash_out_init();

    //lvgl init
    lv_port_init();
    
    extern const unsigned char om_gif_1 [937605];
    img = lv_om_gif_create(lv_scr_act());
    
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_om_gif_set_src(img, om_gif_1, 937605 );

    // lv_demo_start();

    // Start bluetooth task
    vStartBluetoothTask();

    // Test task
    vStartTestTask();

    // Start the scheduler.
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    for( ;; );
}

void vApplicationTickHook( void )
{
#if (CONFIG_LVGL)
    lv_tick_inc(1U);    //lvgl tick
#endif
}

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

/** @} */

