/**
 * @file example_uart.c
 * @brief example for UART
 * @date 2017/07/12 17:53:16
 * @author liqiang
 *
 * @defgroup EXAMPLE_UART UART Example
 * @ingroup EXAMPLE
 * @brief example for UART
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
static void receive_handler(uint8_t data)
{
    // Got data
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_uart(void)
{
    uint8_t send_data[2] = {1, 2};

    // Pinmux
    pinmux_config(5,  PINMUX_UART1_SDA_O_CFG);
    pinmux_config(6,  PINMUX_UART1_SDA_I_CFG);

    // UART Init
    uart_open(HS_UART1, 115200, UART_FLOW_CTRL_DISABLED, receive_handler);

    // Send data with block
    uart_send_block(HS_UART1, send_data, 2);

    while(1);
}

/** @} */

