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
#include "USB2SPI.h"
#include "USB_ISR.h"

/*  OM6621P_SPIM  HS6206_SPIS   SPI
    PAD_GPIO<13>	P1.2	    CSN
    PAD_GPIO<14>	P2.1	    MISO
    PAD_GPIO<15>	P2.0	    SCK
    PAD_GPIO<16>	P1.3	    MOSI
    PAD_GPIO<17>	P1.0	    IRQ
*/
/*********************************************************************
 * MACROS
 */

#define PIN_WAKEUP_0                    2   // UART-RTS@EVB
#define PIN_WAKEUP_1                    17  // KEY1@EVB
#define PIN_LED_0                       8
#define PIN_LED_1                       9
#define PIN_LED_2                       23
#define PIN_LED_3                       25
#define PIN_LED_4                       23
#define PIN_LED_5                       24
#define PIN_LED_6                       24
#define PIN_LED_7                       24
#define PIN_UART0_TX                    5
#define PIN_UART1_TX                    7
#define PIN_UART1_RX                    6

#define HCI_UART_BAUDRATE               115200
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
static uint32_t pin_leds_mask   = (BIT_MASK(PIN_LED_0)|BIT_MASK(PIN_LED_1)|
                                   BIT_MASK(PIN_LED_2)|BIT_MASK(PIN_LED_3)|
                                   BIT_MASK(PIN_LED_4)|BIT_MASK(PIN_LED_5)|
                                   BIT_MASK(PIN_LED_6)|BIT_MASK(PIN_LED_7));

static co_timer_t usb_report_timer;

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

static void peripheral_init(void)
{
    // Init GPIO
    gpio_open();

    // Wakeup Pin
    gpio_set_direction(pin_wakeup_mask, GPIO_INPUT);
    gpio_set_interrupt(pin_wakeup_mask, GPIO_BOTH_EDGE);
    gpio_set_interrupt_callback(gpio_handler);

    // LEDS
    gpio_write(pin_leds_mask, GPIO_HIGH);
    gpio_set_direction(pin_leds_mask, GPIO_OUTPUT);

    // wakeup
    pmu_wakeup_pin_set(pin_wakeup_mask, PMU_PIN_WAKEUP_LOW_LEVEL);
    pmu_wakeup_pin_register_callback(wakeup_gpio_handler, wakeup_gpio_handler);

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
            break;

        case POWER_SLEEP_LEAVE_TOP_HALF:
            peripheral_restore();
            break;

        case POWER_SLEEP_LEAVE_BOTTOM_HALF:
            break;
    }
}


extern void build_mouse_report(uint8_t button,uint8_t x, uint8_t y, uint8_t vwheel, uint8_t hwheel);
extern void send_packet(unsigned char report_id);
static void usb_report_timer_handler(co_timer_t *timer, void *param)
{
    static int8_t x = 0;
    static int8_t dir = 1;
    
    if(g_USB_State != DEV_CONFIGURED)
        return;

    x += dir;
    if (x > 10)
        dir = -dir;
    if (x < -10)
        dir = -dir;
    build_mouse_report(0, x, 0, 0, 0); //button, x, y, wheel
    send_packet(0); //ReportID = 0
}

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
    co_power_ultra_sleep_mode_enable(false);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(false);
}

uint8_t usb_spi_rw(uint8_t value)
{
    uint8_t rxdata;
    gpio_toggle(BIT_MASK(PIN_LED_0));
    spi_master_exchange(HS_SPI0, &value, &rxdata, 1);
    gpio_toggle(BIT_MASK(PIN_LED_0));
    return rxdata;
}
extern void usb_interrupt_service(uint32_t pin_mask);
void usb_irq_init(void)
{
    pinmux_config(PIN_USB_INTERRUPT,  PINMUX_GPIO_MODE_CFG);
    gpio_set_direction(BIT_MASK(PIN_USB_INTERRUPT), GPIO_INPUT);
    pmu_pin_mode_set(BIT_MASK(PIN_USB_INTERRUPT), PMU_PIN_MODE_PU);
    gpio_set_interrupt(BIT_MASK(PIN_USB_INTERRUPT), GPIO_FALLING_EDGE);
    gpio_set_interrupt_callback(usb_interrupt_service);    
}
void usb_spi_init(void)
{
    // Pinmux
    pinmux_config(PIN_USB_SPI_MISO,  PINMUX_SPI0_MST_SDA_I_CFG); /*MISO*/
    pinmux_config(PIN_USB_SPI_MOSI,  PINMUX_SPI0_MST_SDA_O_CFG); /*MOSI*/
    pinmux_config(PIN_USB_SPI_SCK,   PINMUX_SPI0_MST_SCK_CFG);   /*SCLK*/
    pinmux_config(PIN_USB_SPI_CSN,   PINMUX_SPI0_MST_CSN_CFG);   /*CSN */
    pinmux_config(PIN_USB_INTERRUPT, PINMUX_GPIO_MODE_CFG);      /*IRQ */
    
    // SPI Init
    spi_open(HS_SPI0, SPI_MODE_MASTER, SPI_TRANS_MODE_2, 8000000);
}
void usb_spi_csn_low(void)
{
    spi_master_cs_low(HS_SPI0);   
}
void usb_spi_csn_high(void)
{
    spi_master_cs_high(HS_SPI0);
}

/**
 * @brief example_pwm_common()
 *
 * @return 
 **/
static void example_pwm_common(HS_TIM_Type *tim, uint8_t ch, bool complementary)
{
    co_assert((HS_TIM0 == tim) || (HS_TIM1 == tim) || (HS_TIM2 == tim));
    co_assert(!(3 == ch && true == complementary));
    
    pinmux_t timer_ch;
    pinmux_t timer_ch_toggle;
    
    tim_config_t pwm_cfg =
    {
        .mode = TIM_PWM_MODE,
        .config.pwm =
        {
            /* pwm_frequency = count_freq/period_count  */
            /* t = (psc + 1)*(arr + 1)/clock    psc = (clock/count_freq) - 1    arr = period_count - 1 */
            .count_freq = 32000000, 
            .period_count = 2,
            .channel =
            {
                /* duty cycle = pulse_count/period_count */
                {false/*enable*/, {TIM_PWM_POL_HIGH2LOW, 1}}, /* channel0: duty cycle = 1/5 */
                {false/*enable*/, {TIM_PWM_POL_HIGH2LOW, 2}}, /* channel1: duty cycle = 2/5 */
                {false/*enable*/, {TIM_PWM_POL_HIGH2LOW, 3}}, /* channel2: duty cycle = 3/5 */
                {false/*enable*/, {TIM_PWM_POL_HIGH2LOW, 4}}, /* channel3: duty cycle = 4/5 */ 
            },
            .callback = NULL,
        },
    };      
    
    pwm_cfg.config.pwm.channel[ch].enable = true;

    /* PWM Init */
    tim_config(tim, &pwm_cfg);
    tim_start(tim);
}


/**
  * @brief  example_pwm 
  * @param  None
  * @retval None
  */
void open_hs6206_16m_clock(void)
{
    pinmux_config(19,  PINMUX_TIMER0_IO_0_CFG);
    example_pwm_common(HS_TIM0,0,false);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

int main(void)
{
//    cpm_set_clock_div(CPM_SF0_CLK,2);
//    cpm_set_clock_div(CPM_SF1_CLK,2);
//    cpm_set_clock_div(CPM_SF2_CLK,2);
//    cpm_set_clock_div(CPM_CPU_CLK,1);
//    cpm_set_clock_div(CPM_TIM0_CLK,1);
//    pmu_pll_startup(128);
//	
    ble_stack_config();

    hardware_init();

    rwip_init(RESET_NO_ERROR);

    co_power_register_sleep_event(power_sleep_event_handler);

    log_debug("[OM6621PT] hero usb demo is built at %s %s\n", __TIME__, __DATE__);
    log_debug("pmu reboot reason %d\n", pmu_reboot_reason());

    appm_init();
    open_hs6206_16m_clock();
#ifndef USB_APP_CDC
    co_timer_set(&usb_report_timer, 4, TIMER_REPEAT, usb_report_timer_handler, NULL);
#endif    
    usb_init();

    while(1)
    {
        rwip_schedule();
    }
}

/** @} */

// vim: fdm=marker
