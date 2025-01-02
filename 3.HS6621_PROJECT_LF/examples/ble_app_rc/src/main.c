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
#include "rwip.h"      // RW SW initialization
#include "dbg.h"
#include "peripheral.h"
#include "sysdump.h"
#include "app.h"
#include "m_uart.h"
#include "m_led.h"
#include "m_key.h"
#include "m_audio.h"
#include "m_ir.h"
/*********************************************************************
 * MACROS
 */
//#define APP_LOG_DEBUG
/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * LOCAL VARIABLES
 */
#ifdef APP_LOG_DEBUG
static co_timer_t simple_timer;
#endif
/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void pinmux_init(void)
{
}

static void peripheral_init(void)
{
    // Init GPIO
    gpio_open();

    // led 
    #if CONFIG_LED_ENABLE
    led_init();
    #endif
    // Init UART
    #if CONFIG_UART_ENABLE
    uart_init();
    #endif
    #if CONFIG_KEY_ENABLE
    key_init();
    #endif
    #if CONFIG_IR_ENABLE
    //ir_init();
    #endif
}

static void peripheral_restore(void)
{
    // led 
    #if CONFIG_LED_ENABLE
    //led_init();
    #endif
    // Init UART
    #if CONFIG_UART_ENABLE
    uart_init();
    #endif
    #if CONFIG_KEY_ENABLE
    //key_init();
    #endif
    #if CONFIG_IR_ENABLE
    //ir_init();
    #endif
}

static void hardware_init(void)
{
    // close ldo, 100uA-->20uA
#if (APP_CUSTOMER_ID == 2)
    pmu_pin_mode_set(0x208, PMU_PIN_MODE_PP);  // use GPIO3 capture battery,PB1 use ir
    pmu_pin_mode_set(0x10080, PMU_PIN_MODE_PD); // PB0 use to IR
    pmu_pin_mode_set(0xFFFEFD77, PMU_PIN_MODE_PU);
#else
    pmu_pin_mode_set(0x200, PMU_PIN_MODE_PP); 
    pmu_pin_mode_set(0x80, PMU_PIN_MODE_PD); 
    pmu_pin_mode_set(0xFFFFFD7F, PMU_PIN_MODE_PU);
#endif
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
#ifdef APP_LOG_DEBUG
static void simple_timer_handler(co_timer_t *timer, void *param)
{
    sysdump();
}
#endif

static void ble_stack_config(void)
{
    // Disable WDT
    wdt_enable(0);
    pmu_dcdc_enable(true);

    // Power down xtal32k in deep sleep mode
    pmu_32k_enable_in_deep_sleep(true);

    // Select 32k clock for stack
    //pmu_xtal32k_change_param(15, 1);
    pmu_select_32k(PMU_32K_SEL_32768HZ_XTAL);
    //pmu_select_32k(PMU_32K_SEL_RC);
       
    pmu_xtal32m_change_param(12);
    
    // ultra sleep mode enable
    co_power_ultra_sleep_mode_enable(true);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(true);
    //sc.rw_sleep_enable = false;
    // close invaild ram block
    #ifdef CONFIG_HS6621
    pmu_ram_power_off_invalid_block();
    #endif
    // need the xtal vary good.
    #if 0
    sc.lpclk_drift = 20;
    #endif
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

int main(void)
{
    ble_stack_config();

    rwip_init(RESET_NO_ERROR);

    hardware_init();

    co_power_register_user_status(power_sleep_status_handler);
    co_power_register_sleep_event(power_sleep_event_handler);

    log_debug("running %d\n", pmu_reboot_reason());
    
#ifndef CONFIG_HS6621 // when audio,must set PIN2&PIN3 float--(v03)
    pmu_pin_mode_set(BITMASK(2), PMU_PIN_MODE_FLOAT);
    pmu_pin_mode_set(BITMASK(3), PMU_PIN_MODE_FLOAT);    
#endif

#if (NVDS_SUPPORT)
    uint8_t pub_addr[6] = {0x01,0xBF,0x66,0x21,0xCC,0xCC};
    nvds_put(NVDS_TAG_BD_ADDRESS, 6, pub_addr);
#endif

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

#ifdef APP_LOG_DEBUG
    co_timer_set(&simple_timer, 1000, TIMER_REPEAT, simple_timer_handler, NULL);
#endif
#if 0
    pinmux_config(2,  PINMUX_ANALOG_PIN2_CFG);
    adc_cal_table_t cal_table;
    adc_calibrate(&cal_table);
    adc_set_calibarate_param(&cal_table);

    float vbat = adc_battery_voltage_read(NULL);
    float vbat2 = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_PIN2, ADC_PGA_GAIN_0P125, NULL, 10);
#endif

    while(1)
    {
        rwip_schedule();
#if 0
        vbat = adc_battery_voltage_read(NULL);
        vbat2 = adc_battery_voltage_read_by_single_pin(ADC_CHANNEL_EXTERN_PIN2, ADC_PGA_GAIN_0P125, NULL, 10);
        log_debug("%f,%f\r\n", vbat, vbat2);
#endif
    }
}

/** @} */

// vim: fdm=marker
