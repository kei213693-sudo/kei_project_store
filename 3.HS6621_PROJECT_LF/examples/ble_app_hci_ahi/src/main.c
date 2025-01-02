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
#include "h4tl.h"
#include "peripheral.h"
#include "rw_uart.h"
#include "ke_msg.h"
#include "hci.h"
#include "sysdump.h"

#if BLE_MESH
#include "mesh_log.h"
#endif

/**
 * If use DTM 2wire mode, please define macro CONFIG_DTM_USE_2WIRE_MODE.
 * And add lib/keil/dtm2wire/dtm2wire.lib(ARMCC) or lib/keil/dtm2wire/dtm2wire_armclang.lib(ARMCLANG) to project.
 */
//#define CONFIG_DTM_USE_2WIRE_MODE
#ifdef CONFIG_DTM_USE_2WIRE_MODE
#include "rw_uart_2wire.h"
#endif

/*********************************************************************
 * MACROS
 */

// IO PIN {{{
#define PIN_WAKEUP_0                    24  // UART-RTS@EVB
#define PIN_WAKEUP_1                    24  // KEY1@EVB
#define PIN_LED_0                       8
#define PIN_LED_1                       9
#define PIN_LED_2                       23
#define PIN_LED_3                       25
#define PIN_LED_4                       13
#define PIN_LED_5                       14
#define PIN_LED_6                       14
#define PIN_LED_7                       14
#define PIN_UART0_TX                    7
#define PIN_UART1_TX                    5
#define PIN_UART1_RX                    6
#define PIN_UART1_RTS                   3
#define PIN_UART1_CTS                   2

// }}}

#define HCI_UART_BAUDRATE               115200
#define DEBUG_UART_BAUDRATE             115200

//#define HCI_UART_FLOW_CONTROL
//#define ADV_START

#ifdef CONFIG_DTM_USE_2WIRE_MODE
#define rw_uart_open                    rw_uart_2wire_open
#define rw_uart_read                    rw_uart_2wire_read
#define rw_uart_write                   rw_uart_2wire_write
#define rw_uart_flow_on                 rw_uart_2wire_flow_on
#define rw_uart_flow_off                rw_uart_2wire_flow_off
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

static uint32_t pin_wakeup_mask = (BIT_MASK(PIN_WAKEUP_0) | BIT_MASK(PIN_WAKEUP_1));
static uint32_t pin_leds_mask   = (BIT_MASK(PIN_LED_0)|BIT_MASK(PIN_LED_1)|
                                   BIT_MASK(PIN_LED_2)|BIT_MASK(PIN_LED_3)|
                                   BIT_MASK(PIN_LED_4)|BIT_MASK(PIN_LED_5)|
                                   BIT_MASK(PIN_LED_6)|BIT_MASK(PIN_LED_7));

// Creation of uart external interface api
const static struct rwip_eif_api uart_eif = {rw_uart_read, rw_uart_write, rw_uart_flow_on, rw_uart_flow_off};
static rw_hci_uart_t rw_hci_uart;
static uint8_t       rw_hci_uart_pool[512];

//static co_timer_t simple_timer;

/*********************************************************************
 * GLOBAL VARIABLES
 */
extern uint16_t rwip_app_main_task;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void adv_start(void)
{
#ifdef ADV_START
    struct hci_le_set_adv_param_cmd *adv_param = KE_MSG_ALLOC(HCI_COMMAND, TASK_LLM, HCI_LE_SET_ADV_PARAM_CMD_OPCODE, hci_le_set_adv_param_cmd);
    struct hci_le_set_adv_data_cmd *adv_data = KE_MSG_ALLOC(HCI_COMMAND, TASK_LLM, HCI_LE_SET_ADV_DATA_CMD_OPCODE, hci_le_set_adv_data_cmd);
    struct hci_le_set_scan_rsp_data_cmd *scan_rsp_data = KE_MSG_ALLOC(HCI_COMMAND, TASK_LLM, HCI_LE_SET_SCAN_RSP_DATA_CMD_OPCODE, hci_le_set_scan_rsp_data_cmd);
    struct hci_le_set_adv_en_cmd *adv_en = KE_MSG_ALLOC(HCI_COMMAND, TASK_LLM, HCI_LE_SET_ADV_EN_CMD_OPCODE, hci_le_set_adv_en_cmd);
//    struct hci_le_tx_test_cmd *tx_test = KE_MSG_ALLOC(HCI_COMMAND, TASK_LLM, HCI_LE_ENH_TX_TEST_CMD_OPCODE, hci_le_tx_test_cmd);

    // ADV param
    adv_param->adv_intv_min = 0xA0;
    adv_param->adv_intv_max = 0xA0;
    adv_param->adv_type = ADV_NONCONN_UNDIR;//ADV_NONCONN_UNDIR;
    adv_param->own_addr_type = ADDR_PUBLIC;
    adv_param->peer_addr_type = ADDR_PUBLIC;
    adv_param->adv_chnl_map = ADV_ALL_CHNLS_EN;
    adv_param->adv_filt_policy = ADV_ALLOW_SCAN_ANY_CON_ANY;

    // ADV data
    adv_data->adv_data_len = 17;
    memcpy(adv_data->data.data, "\x02\x01\x06\x0d\x08\x51\x51\x51\x51\x51\x51\x51\x20\x54\x45\x53\x54", 17);
    scan_rsp_data->scan_rsp_data_len = 17;
    memcpy(scan_rsp_data->data.data, "\x02\x01\x06\x0d\x08\x53\x53\x53\x53\x53\x53\x53\x20\x54\x45\x53\x54", 17);

    // ADV enable
    adv_en->adv_en = ADV_EN;

    // Send it
    ke_msg_send(adv_param);
    ke_msg_send(adv_data);
    ke_msg_send(scan_rsp_data);
    ke_msg_send(adv_en);
#endif
}

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
	#ifdef HCI_UART_FLOW_CONTROL
    pinmux_config(PIN_UART1_CTS, PINMUX_UART1_CTS_I_N_CFG);
    pinmux_config(PIN_UART1_RTS, PINMUX_UART1_RTS_O_N_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART1_RTS), PMU_PIN_MODE_PP);
    pmu_pin_mode_set(BITMASK(PIN_UART1_CTS), PMU_PIN_MODE_PD);
    #endif

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

    // Init UART
    rw_uart_open(HCI_UART_BAUDRATE, &rw_hci_uart, rw_hci_uart_pool, sizeof(rw_hci_uart_pool));
	
	// Enable flow control
	#ifdef HCI_UART_FLOW_CONTROL
    HS_UART1->MCR = UART_MCR_RTS | UART_MCR_AFCE;
    #endif
}

static void peripheral_restore(void)
{
    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);

    // Init UART
    rw_uart_open(HCI_UART_BAUDRATE, &rw_hci_uart, rw_hci_uart_pool, sizeof(rw_hci_uart_pool));
	
	// Enable flow control
    #ifdef HCI_UART_FLOW_CONTROL
    HS_UART1->MCR = UART_MCR_RTS | UART_MCR_AFCE;
    #endif
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

co_power_status_t power_sleep_status_handler(void)
{
    co_power_status_t pmu_status;

    pmu_status = pmu_power_status();

//    return MIN(POWER_SLEEP, pmu_status);
    return pmu_status;
}

//static void simple_timer_handler(co_timer_t *timer, void *param)
//{

//}

static void ble_stack_config(void)
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
    co_power_ultra_sleep_mode_enable(true);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(false);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

int main(void)
{
    ble_stack_config();

    hardware_init();

#if BLE_MESH
    mesh_log_init(0xffffffff, LOG_LEVEL_DBG2);
#endif

    gpio_toggle(pin_leds_mask);

    rwip_init(RESET_NO_ERROR);
    h4tl_init(0, &uart_eif);
    rwip_reset();
    adv_start();

#if AHI_TL_SUPPORT
    rwip_app_main_task = TASK_AHI;
#endif

    co_power_register_user_status(power_sleep_status_handler);
    co_power_register_sleep_event(power_sleep_event_handler);

    log_debug("running %d\n", pmu_reboot_reason());

    gpio_toggle(pin_leds_mask);

    //co_timer_set(&simple_timer, 100, TIMER_REPEAT, simple_timer_handler, NULL);

    while(1)
    {
        rwip_schedule();
    }
}

/** @} */

// vim: fdm=marker
