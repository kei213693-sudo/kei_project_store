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
#include "co.h"

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

/*********************************************************************
 * MACROS
 */

// IO PIN {{{
#ifdef CONFIG_ASIC
#define PIN_WAKEUP_0                    14//2   // UART-RTS@EVB
#define PIN_WAKEUP_1                    14 //2//24  // KEY1@EVB
/*
#define PIN_LED_0                       8
#define PIN_LED_1                       9
//#define PIN_LED_2                       10
#define PIN_LED_3                       14//25
#define PIN_LED_4                       14//13
#define PIN_LED_5                       14
#define PIN_LED_6                       14
#define PIN_LED_7                       14
*/
#define PIN_UART0_TX                    5
#define PIN_UART1_TX                    13
#define PIN_UART1_RX                    6
#else
#define PIN_WAKEUP_0                    3  // GPIO4@FPGA-PeripheralBoard
#define PIN_WAKEUP_1                    4  // KEY4@FPGA-PeripheralBoard (KEY2:RESET)
#define PIN_LED_0                       14
#define PIN_LED_1                       15
//#define PIN_LED_2                       16
#define PIN_LED_3                       17
#define PIN_LED_4                       18
#define PIN_LED_5                       19
#define PIN_LED_6                       20
#define PIN_LED_7                       21
#define PIN_UART0_TX                    13
#define PIN_UART1_TX                    5
#define PIN_UART1_RX                    6
#endif
// }}}
#if 0
#define AUDIO_PIN_ANA_MIC           3
#endif
#ifdef APP_AUDIO_USE_DIG_MIC
#define AUDIO_PIN_DIG_MIC_CLK       19
#define AUDIO_PIN_DIG_MIC_DATA      21
#endif

#ifdef APP_TEST_AUDIO_PLAY
#define AUDIO_PIN_I2C_CLK           20//25 //33
#define AUDIO_PIN_I2C_DAT           21//18
#endif

#define AUDIO_PIN_I2S_TX_WS         22//16
#define AUDIO_PIN_I2S_TX_CLK        23//29
#define AUDIO_PIN_I2S_TX_DAT        24//27

//#define HCI_UART_BAUDRATE               115200
#define DEBUG_UART_BAUDRATE             115200

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
//static uint32_t pin_leds_mask   = (BIT_MASK(PIN_LED_0)|BIT_MASK(PIN_LED_1)|
//                                   BIT_MASK(PIN_LED_3)|
//                                   BIT_MASK(PIN_LED_4)|BIT_MASK(PIN_LED_5)|
//                                   BIT_MASK(PIN_LED_6)|BIT_MASK(PIN_LED_7));

//static co_timer_t simple_timer;

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */
#if 0
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
#endif
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

#if 0
    // LEDS
    pinmux_config(PIN_LED_0, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_1, PINMUX_GPIO_MODE_CFG);
    //pinmux_config(PIN_LED_2, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_3, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_4, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_5, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_6, PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_LED_7, PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(pin_leds_mask, PMU_PIN_MODE_PP);
#endif

#ifdef APP_AUDIO_USE_DIG_MIC
    // dig mic
    /// 1. dmic pin setting
    pinmux_config(15,  PINMUX_TIMER0_BKIN_CFG);
    pinmux_config(17,  PINMUX_TIMER0_BKIN_CFG);
    pinmux_config(20,  PINMUX_TIMER0_BKIN_CFG);
    pinmux_config(AUDIO_PIN_DIG_MIC_CLK, PINMUX_DMIC_CLK_CFG);
    pinmux_config(AUDIO_PIN_DIG_MIC_DATA, PINMUX_DMIC_IN_CFG);
    if (AUDIO_PIN_DIG_MIC_CLK > 32)
    {
        #ifndef CONFIG_GPIO1_ABSENT
        pmu_pin_driven_current_set_ex(BIT_MASK(AUDIO_PIN_DIG_MIC_CLK-32), PMU_PIN_DRIVEN_CURRENT_2MA);
        #endif
    }
    else {
        pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_DIG_MIC_CLK), PMU_PIN_DRIVEN_CURRENT_2MA);
    }
    if (AUDIO_PIN_DIG_MIC_DATA > 32)
    {
        #ifndef CONFIG_GPIO1_ABSENT
        pmu_pin_driven_current_set_ex(BIT_MASK(AUDIO_PIN_DIG_MIC_DATA-32), PMU_PIN_DRIVEN_CURRENT_2MA);
        #endif
    }
    else {
        pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_DIG_MIC_DATA), PMU_PIN_DRIVEN_CURRENT_2MA);
    }
#else
#if 0//#ifdef CONFIG_HS6621
    // ana mic
    pmu_pin_mode_set(BITMASK(AUDIO_PIN_ANA_MIC), PMU_PIN_MODE_FLOAT);
    REGW0(&HS_PMU->GPIO_IE_CTRL, BITMASK((AUDIO_PIN_ANA_MIC)));
    pmu_pin_driven_current_set(BITMASK(AUDIO_PIN_ANA_MIC), PMU_PIN_DRIVEN_CURRENT_2MA);
#endif
#endif
#ifdef APP_TEST_AUDIO_PLAY
/// wm8753 pin setting
    pinmux_config(AUDIO_PIN_I2C_CLK, PINMUX_I2C_MST_SCK_CFG);
    pinmux_config(AUDIO_PIN_I2C_DAT, PINMUX_I2C_MST_SDA_CFG);
#if 0 //#ifndef CONFIG_GPIO1_ABSENT
    pmu_pin_driven_current_set_ex(BIT_MASK(AUDIO_PIN_I2C_CLK-32), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_mode_set_ex(BIT_MASK(AUDIO_PIN_I2C_CLK-32), PMU_PIN_MODE_OD);
#else
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2C_CLK), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_mode_set(BIT_MASK(AUDIO_PIN_I2C_CLK), PMU_PIN_MODE_OD);
#endif
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2C_DAT), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_mode_set(BIT_MASK(AUDIO_PIN_I2C_DAT), PMU_PIN_MODE_OD);
    
    /// codec i2s pin setting
    pinmux_config(AUDIO_PIN_I2S_TX_DAT, PINMUX_I2S_SDO_0_CFG);
    pinmux_config(AUDIO_PIN_I2S_TX_WS,  PINMUX_I2S_TX_WS_CFG);
    pinmux_config(AUDIO_PIN_I2S_TX_CLK,  PINMUX_I2S_TX_SCLK_CFG);
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2S_TX_DAT), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2S_TX_WS), PMU_PIN_DRIVEN_CURRENT_2MA);
    pmu_pin_driven_current_set(BIT_MASK(AUDIO_PIN_I2S_TX_CLK), PMU_PIN_DRIVEN_CURRENT_2MA);
#endif
}

static void peripheral_init(void)
{
    // Init GPIO
    gpio_open();

    // Wakeup Pin
    gpio_set_direction(pin_wakeup_mask, GPIO_INPUT);
    gpio_set_interrupt(pin_wakeup_mask, GPIO_BOTH_EDGE);
//    gpio_set_interrupt_callback(gpio_handler);

#if 0
    // LEDS
#ifdef CONFIG_ASIC
    gpio_write(pin_leds_mask, GPIO_HIGH);
#else
    gpio_write(pin_leds_mask, GPIO_LOW);
#endif
    gpio_set_direction(pin_leds_mask, GPIO_OUTPUT);
#endif

    // wakeup
    pmu_wakeup_pin_set(pin_wakeup_mask, PMU_PIN_WAKEUP_LOW_LEVEL);
    //pmu_wakeup_pin_register_callback(wakeup_gpio_handler, wakeup_gpio_handler);

    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
}

static void peripheral_restore(void)
{
    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
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

/*static void simple_timer_handler(co_timer_t *timer, void *param)
{
    //sysdump();
}
*/

static void ble_stack_config(void)
{
    // Disable WDT
    wdt_enable(0);

    pmu_dcdc_enable(true);

    // Power down xtal32k in deep sleep mode
    pmu_32k_enable_in_deep_sleep(false);

    // Select 32k clock for stack
    //pmu_xtal32k_change_param(15, 1);
//    pmu_select_32k(PMU_32K_SEL_32768HZ_XTAL);
    pmu_select_32k(PMU_32K_SEL_RC);

    // ultra sleep mode enable
    co_power_ultra_sleep_mode_enable(false);

    pmu_xtal32m_change_param(12);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(false);
}

void *appm_malloc(int size)
{
    return co_malloc(size);
}

void appm_free(void *p)
{
    co_free(p);
}
/*********************************************************************
 * PUBLIC FUNCTIONS
 */
 #if 0
static void show_audio_reg(void)
{
    log_debug("HS_PSO->AUDIO_CFG=%08x\r\n", HS_PSO->AUDIO_CFG);
    log_debug("HS_PSO->I2S_CFG=%08x\r\n", HS_PSO->I2S_CFG);
    log_debug("HS_DAIF->ANA_TST_CTRL=%08x\r\n", HS_DAIF->ANA_TST_CTRL);
    #ifdef CONFIG_HS6621
    log_debug("HS_DAIF->SYSPLL_CNS0=%08x\r\n", HS_DAIF->SYSPLL_CNS0);
    #endif
    log_debug("HS_AUDIO->CODEC_ANA_PWR_1=%08x\r\n", HS_AUDIO->CODEC_ANA_PWR_1);
    log_debug("HS_AUDIO->CODEC_ANA_CTRL_1=%08x\r\n", HS_AUDIO->CODEC_ANA_CTRL_1);
    log_debug("HS_AUDIO->CODEC_ANA_CTRL_2=%08x\r\n", HS_AUDIO->CODEC_ANA_CTRL_2);
    log_debug("HS_AUDIO->ADC_VOL_CTRL=%08x\r\n", HS_AUDIO->ADC_VOL_CTRL);
    log_debug("HS_AUDIO->CODEC_CLK_CTRL_2=%08x\r\n", HS_AUDIO->CODEC_CLK_CTRL_2);
    log_debug("HS_AUDIO->CODEC_CLK_CTRL_3=%08x\r\n", HS_AUDIO->CODEC_CLK_CTRL_3);
}
#endif
int main(void)
{
    //pmu_pll_startup(96);
    ble_stack_config();

    hardware_init();
#ifndef CONFIG_HS6621 // when audio,must set PIN2&PIN3 float
    pmu_pin_mode_set(BITMASK(2), PMU_PIN_MODE_FLOAT);
    pmu_pin_mode_set(BITMASK(3), PMU_PIN_MODE_FLOAT);    
#endif
    rwip_init(RESET_NO_ERROR);

    co_power_register_user_status(power_sleep_status_handler);
    co_power_register_sleep_event(power_sleep_event_handler);

    log_debug("running %d\n", pmu_reboot_reason());

#if defined(NORDIC_DFU_DEBUG)
    uint32_t addr, size, res;
    sfs_enable();
    res = mbr_read_part(PART_TYPE_APP, &addr, &size, NULL);
    if(!res){
        dfu_debug("Running @ flash address: 0x%08x, size:0x%08x(%d)\n", addr, size, size);
    }else{
        dfu_debug("Read PART_TYPE_APP address failed: %d\n", res);
    }
    res = mbr_read_part(PART_TYPE_PATCH, &addr, &size, NULL);
    if(!res){
        dfu_debug("patch address: 0x%08x, size:0x%08x(%d)\n", addr, size, size);
    }else{
        dfu_debug("Read PART_TYPE_PATCH address failed: %d\n", res);
    }
    res = mbr_read_part(PART_TYPE_CFG, &addr, &size, NULL);
    if(!res){
        dfu_debug("config address: 0x%08x, size:0x%08x(%d)\n", addr, size, size);
    }else{
        dfu_debug("Read PART_TYPE_CFG address failed: %d\n", res);
    }
#endif

    appm_init();

    //co_timer_set(&simple_timer, 100, TIMER_REPEAT, simple_timer_handler, NULL);
#if 0
    pinmux_config(2,  PINMUX_ANALOG_CH2_PIN2_CFG);
    adc_cal_table_t cal_table;
    adc_calibrate(&cal_table);
    adc_set_calibarate_param(&cal_table);

    float vbat = adc_battery_voltage_read(NULL);
    float vbat2 = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH2, ADC_PGA_GAIN_0P125, NULL, 10);
#endif
    //show_audio_reg();
    
#ifdef APP_TEST_AUDIO_PLAY
    app_audio_open();
#endif
    if (0) /// debug audio adc
    {
        HS_SYS->MON = 0x0801;

        pinmux_config(15, PINMUX_DBG_MODE_CFG);
        pinmux_config(16, PINMUX_DBG_MODE_CFG);
        pinmux_config(17, PINMUX_DBG_MODE_CFG);
        pinmux_config(22, PINMUX_DBG_MODE_CFG);
        pmu_pin_driven_current_set(BITMASK(15), PMU_PIN_DRIVEN_CURRENT_2MA);
        pmu_pin_driven_current_set(BITMASK(16), PMU_PIN_DRIVEN_CURRENT_2MA);
        pmu_pin_driven_current_set(BITMASK(17), PMU_PIN_DRIVEN_CURRENT_2MA);
        pmu_pin_driven_current_set(BITMASK(22), PMU_PIN_DRIVEN_CURRENT_2MA);
    }
    //static uint16_t ttt = 0;
    while(1)
    {
        rwip_schedule();
        /*
        ttt++;
        if(ttt==1000) {
            app_audio_close();
            show_audio_reg();
        }
        else if(ttt>2000) 
        {
            ttt = 0;
            app_audio_open();
        }*/
#if 0
        vbat = adc_battery_voltage_read(NULL);
        vbat2 = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_CH2, ADC_PGA_GAIN_0P125, NULL, 10);
        log_debug("%f,%f\r\n", vbat, vbat2);
#endif
    }
}

/** @} */

// vim: fdm=marker
