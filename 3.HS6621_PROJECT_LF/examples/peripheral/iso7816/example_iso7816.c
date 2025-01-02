/**
 * @file example_iso7816.c
 * @brief example for iso7816
 * @date 2017/07/12 15:59:03
 * @author liqiang
 *
 * @defgroup EXAMPLE_ISO7816 ISO7816 Example
 * @ingroup EXAMPLE
 * @brief example for iso7816
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void iso7816_receive_handler(uint8_t data)
{
    log_debug("RX: 0x%02X\n", data);
}


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_iso7816(void)
{
    // Pinmux
    pinmux_config(8, PINMUX_GPIO_MODE_CFG);
    pinmux_config(5,  PINMUX_ISO7816_IO_CFG);
    pinmux_config(6,  PINMUX_ISO7816_CLK_CFG);

    // gpio init
    gpio_open();
    gpio_set_direction(1<<8, GPIO_OUTPUT);
    pmu_pin_mode_set((1<<8) | (1<<5) | (1<<6), PMU_PIN_MODE_PU);

    // 7816 init
    iso7816_init(4000000, 372, iso7816_receive_handler);

    while(1)
    {
        // Reset smart card, Maybe return: 0x3B,0x6B,0x00,0x00,0xFF,0x86,0x88,0x62,0x6A,0x6E,0x73,0x31,0x32,0x21,0x22
        gpio_write(1<<8, GPIO_LOW);
        co_delay_ms(1000);
        gpio_write(1<<8, GPIO_HIGH);
        co_delay_ms(100);
        log_debug("error: %04X\n", HS_UART1->ISO7816.ERROR_CNT);

        // Send 'PTS' command to change speed, Shoudl be returned: FF1111FF(support) or FF1011FF(not-support)
        iso7816_send_block((const uint8_t *)"\xFF\x11\x11\xFF", 4);
        co_delay_ms(100);
        log_debug("error: %04X\n", HS_UART1->ISO7816.ERROR_CNT);
    }
}
/** @} */

