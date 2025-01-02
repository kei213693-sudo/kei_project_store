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
#include "cmsis_os2.h"

/*********************************************************************
 * MACROS
 */

// IO PIN {{{
#ifdef CONFIG_ASIC
#define PIN_WAKEUP_0                    2   ///< UART-RTS@EVB
#define PIN_WAKEUP_1                    24  ///< KEY1@EVB
#define PIN_LED_0                       8   ///< LED 0
#define PIN_LED_1                       9   ///< LED 1
#define PIN_LED_2                       23  ///< LED 2
#define PIN_LED_3                       25  ///< LED 3
#define PIN_LED_4                       13  ///< LED 4
#define PIN_LED_5                       14  ///< LED 5
#define PIN_LED_6                       14  ///< LED 6
#define PIN_LED_7                       14  ///< LED 7
#define PIN_UART0_TX                    5   ///< UART0 TX
#define PIN_UART1_TX                    7   ///< UART1 TX
#define PIN_UART1_RX                    6   ///< UART1 RX
#else
/// @cond
#define PIN_WAKEUP_0                    3  // GPIO4@FPGA-PeripheralBoard
#define PIN_WAKEUP_1                    4  // KEY4@FPGA-PeripheralBoard (KEY2:RESET)
#define PIN_LED_0                       14
#define PIN_LED_1                       15
#define PIN_LED_2                       16
#define PIN_LED_3                       17
#define PIN_LED_4                       18
#define PIN_LED_5                       19
#define PIN_LED_6                       20
#define PIN_LED_7                       21
#define PIN_UART0_TX                    13
#define PIN_UART1_TX                    5
#define PIN_UART1_RX                    6
/// @endcond
#endif
// }}}

#define DEBUG_UART_BAUDRATE             115200 ///< debug uart baudrate

/// Wakeup pin
static uint32_t pin_wakeup_mask = (BIT_MASK(PIN_WAKEUP_0) | BIT_MASK(PIN_WAKEUP_1));

/// LED pin
static uint32_t pin_leds_mask   = (BIT_MASK(PIN_LED_0)|BIT_MASK(PIN_LED_1)|
                                   BIT_MASK(PIN_LED_2)|BIT_MASK(PIN_LED_3)|
                                   BIT_MASK(PIN_LED_4)|BIT_MASK(PIN_LED_5)|
                                   BIT_MASK(PIN_LED_6)|BIT_MASK(PIN_LED_7));


/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */
static osSemaphoreId_t sem;

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
    // UART1
    pinmux_config(PIN_UART1_TX,  PINMUX_UART1_SDA_O_CFG);
    pinmux_config(PIN_UART1_RX,  PINMUX_UART1_SDA_I_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART1_TX), PMU_PIN_MODE_PP);
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
#ifdef CONFIG_ASIC
    gpio_write(pin_leds_mask, GPIO_HIGH);
#else
    gpio_write(pin_leds_mask, GPIO_LOW);
#endif
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
            break;

        case POWER_SLEEP_LEAVE_TOP_HALF:
            peripheral_restore();
            break;

        case POWER_SLEEP_LEAVE_BOTTOM_HALF:
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
static void vTimerCallback(void *argument)
{
    static uint32_t i = 0;
    log_debug("soft timer timeout %d.\n", i++);
}

/**
 * @brief  test schedule task1
 *
 * @param[in] pvParameters  pv parameters
 **/
static void vTestTask1(void *argument)
{
    osTimerId_t xTimer;

    xTimer = osTimerNew(vTimerCallback, osTimerPeriodic, NULL, NULL);
    osTimerStart(xTimer, 1000 * osKernelGetTickFreq() / 1000);
    for (;;)
    {
        osSemaphoreAcquire(sem, osWaitForever);
        gpio_toggle(BIT_MASK(PIN_LED_3));
    }
}

/**
 * @brief  test schedule task2
 *
 * @param[in] pvParameters  pv parameters
 **/
static void vTestTask2(void *argument)
{
    for (;;)
    {
        osSemaphoreRelease(sem);
        osDelay(500 * osKernelGetTickFreq() / 1000);
    }
}

/**
 * @brief  v start test task
 **/
static void vStartTestTask(void)
{
    // Create semaphore
    sem = osSemaphoreNew (1, 0, NULL);
    // Create ble Task
    osThreadNew(vTestTask1, NULL, NULL);
    osThreadNew(vTestTask2, NULL, NULL);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

int main(void)
{
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

    // Initialize CMSIS-RTOS
    osKernelInitialize();

    // Start bluetooth task
    vStartBluetoothTask();

    // Test task
    vStartTestTask();

    // Start thread execution
    if (osKernelGetState() == osKernelReady)
        osKernelStart();

    for( ;; );
}

/** @} */

