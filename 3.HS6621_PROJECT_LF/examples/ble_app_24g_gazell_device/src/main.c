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
#include "gzll.h"
#include "gzp.h"
#include "hal_nrf.h"

/*********************************************************************
 * MACROS
 */

#define PIN_WAKEUP_0                    24   // UART-RTS@EVB
#define PIN_WAKEUP_1                    24  // KEY1@EVB
#define PIN_LED_0                       8
#define PIN_LED_1                       9
#define PIN_LED_2                       10
#define PIN_LED_3                       25
#define PIN_LED_4                       14
#define PIN_LED_5                       14
#define PIN_LED_6                       14
#define PIN_LED_7                       14
#define PIN_UART0_TX                    5
#define PIN_UART0_RX                    6
#define DEBUG_UART_BAUDRATE             115200

#ifdef GZP_CRYPT_DISABLE
#error This example project uses gzp_crypt, please remove the definition "GZP_CRYPT_DISABLE".
#endif

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * CONSTANTS
 */
const unsigned char tr_init_data[]=
{
    /*Register Addr             DataLenth   Data*/

    TR_BANK0_CONFIG,        1,      0x02,//0x8e,
    TR_BANK0_RX_PW_P0,      1,      0x20,
    TR_BANK0_DYNPD,         1,      0x3f,
    TR_BANK0_FEATURE,       1,      0x07,
    TR_BANK0_SETUP_VALUE,   5,      0x40,0x3c,0xff,0x06,0x44,
    TR_BANK0_EN_AA,         1,      0x00,
    TR_BANK0_EN_RXADDR,     1,      0x3F,
    TR_BANK0_SETUP_AW,      1,      0x03,
    TR_BANK0_SETUP_RETR,    1,      0x00,
    TR_BANK0_CH,            1,      0x26,
    TR_BANK0_RF_SETUP,      1,      0x47,
//    TR_BANK0_RX_ADDR_P0,    5,      0x46, 0x0b, 0xAF, 0x43, 0x98,
//    TR_BANK0_TX_ADDR,       5,      0x46, 0x0b, 0xAF, 0x43, 0x98,
	  TR_BANK0_RX_ADDR_P0,    5,      0x36, 0x2b, 0xAD, 0x58, 0x96,
    TR_BANK0_TX_ADDR,       5,      0x36, 0x2b, 0xAD, 0x58, 0x96,

    0xFF    /*Addr=0xFF,complete flag*/
};


const unsigned char tr_analog_data[]=
{
    TR_BANK1_PLL_CTL0,      4,  0x40,0x01,0x30,0xE1,
//    TR_BANK1_PLL_CTL1,      4,  0x00,0x8a,0x10,0x07,
    TR_BANK1_PLL_CTL1,      4,  0x00,0x7a,0x10,0x07,
    TR_BANK1_CAL_CTL,       5,  0x29,0x89,0x67,0x28,0x28,
    TR_BANK1_RX_CTRL,       4,  0x52,0xC2,0x09,0xAC,
    TR_BANK1_FAGC_CTRL_1,   4,  0x80,0x14,0x08,0x29,
    TR_BANK1_RF_IVGEN,      4,  0x1F,0x64,0x00,0x11,

    0xFF    /*Addr=0xFF,complete flag*/
};

/*********************************************************************
 * LOCAL VARIABLES
 */

static uint32_t pin_wakeup_mask = (BIT_MASK(PIN_WAKEUP_0) | BIT_MASK(PIN_WAKEUP_1));
static uint32_t pin_leds_mask   = (BIT_MASK(PIN_LED_0)|BIT_MASK(PIN_LED_1)|
                                   BIT_MASK(PIN_LED_2)|BIT_MASK(PIN_LED_3)|
                                   BIT_MASK(PIN_LED_4)|BIT_MASK(PIN_LED_5)|
                                   BIT_MASK(PIN_LED_6)|BIT_MASK(PIN_LED_7));

static co_timer_t simple_timer;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void gpio_handler(uint32_t pin_mask)
{
//    static int counter = 0;
//    log_debug("gpio: %08X [%d]\n", pin_mask, counter++);
}

static void wakeup_gpio_handler(uint32_t pin_mask)
{
//    static int counter = 0;
//    log_debug("wake_gpio: %08X [%d]\n", pin_mask, counter++);
}

static void pinmux_init(void)
{
    // KEY
    pinmux_config(PIN_WAKEUP_0,  PINMUX_GPIO_MODE_CFG);
    pinmux_config(PIN_WAKEUP_1,  PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(pin_wakeup_mask, PMU_PIN_MODE_PU);

    // UART0
    pinmux_config(PIN_UART0_TX, PINMUX_UART0_SDA_O_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART0_TX), PMU_PIN_MODE_PP);
    pinmux_config(PIN_UART0_RX, PINMUX_UART0_SDA_I_CFG);
    pmu_pin_mode_set(BITMASK(PIN_UART0_RX), PMU_PIN_MODE_FLOAT);

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
    __enable_irq();
}

static void peripheral_restore(void)
{
    // Init UART
    uart_open(HS_UART0, DEBUG_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
    __enable_irq();
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

static void simple_timer_handler(co_timer_t *timer, void *param)
{
    gzll_timer_isr_function();
}

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
    co_power_ultra_sleep_mode_enable(true);

    pmu_xtal32m_change_param(25);

    // Enable sleep, SWD will be closed.
    co_power_sleep_enable(false);
    //sc.rw_sleep_enable = false;
}
static uint8_t bitcount(uint8_t n)
{
    uint8_t count = 0 ;
    while (n) {
        count++ ;
        n &= (n - 1) ;
    }
    return count;
}

static void mcu_init(void)
{
    ble_stack_config();
    hardware_init();
    rwip_init(RESET_NO_ERROR);
    rwip_reset();
	  co_power_register_user_status(power_sleep_status_handler);
    co_power_register_sleep_event(power_sleep_event_handler);
    log_debug("running %d\n", pmu_reboot_reason());

	  hal_nrf_phy_enable();
    hal_nrf_ce_low();
    hal_nrf_configure_reg_bank1(tr_analog_data);
	  hal_nrf_configure_reg(tr_init_data);
    NVIC_DisableIRQ(MAC6200_RF_IRQn);  
}
/*********************************************************************
 * PUBLIC FUNCTIONS
 */
int main(void)
{
		
	  uint8_t payload[GZLL_MAX_PAYLOAD_LENGTH];
	  uint8_t i;
	  bool state = false;
		mcu_init();
		gzll_init();
    hal_nrf_dump_rf_register();
		for(i=0; i<32; i++) {
		    payload[i] = i+1;
    }
	  while(1)
		{      
			// If gazell link layer idle
			if(gzll_get_state() == GZLL_IDLE)
			{

				// Transmits payload[] to pipe 0 address
			  state = gzll_tx_data(payload, GZLL_MAX_FW_PAYLOAD_LENGTH, 0);
        if(state){
					  log_debug("send packet:");
						for(i = 0; i<GZLL_MAX_FW_PAYLOAD_LENGTH; i++) {
								log_debug(" %d",payload[i]);
						}
						log_debug("\n");
				}else{
				  log_debug("send error\n");
				}
			  
			  //hal_nrf_dump_rf_register();
        co_delay_ms(1000);			
			}
		}
}

/* gzll_radio_isr.c */
void MAC6200_RF_IRQHandler(void)
{
  gzll_radio_isr_function();
}

void gzll_set_timer_period(uint16_t period/*us*/)
{
  /* RX_PERIOD=1600us: 625Hz, 51.2 rtc clocks. */
  co_timer_set(&simple_timer, period, TIMER_REPEAT, simple_timer_handler, NULL);
}

void gzp_host_chip_id_read(uint8_t *dst, uint8_t n)
{
  uint8_t i;
  // Read n bytes of chip ID (System address?) from info page
  for(i = 0; i < n; i++)
  {
//    *(dst + i) = *(volatile uint8_t *)(OTP_CPU_BASE_C+0x3FFB+i);
  }

  /* for test */
  dst[0] = 0x0a;
  dst[1] = 0x04;
  dst[2] = 0x8e;
  dst[3] = 0xf2;
}

/** @} */

// vim: fdm=marker
