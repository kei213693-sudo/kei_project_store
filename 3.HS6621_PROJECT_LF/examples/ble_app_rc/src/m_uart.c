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


#if CONFIG_UART_ENABLE

void uart_init(void)
{
	pinmux_config(PIN_UART0_TX, PINMUX_UART0_SDA_O_CFG);
	pinmux_config(PIN_UART0_RX, PINMUX_UART0_SDA_I_CFG);
	
	uart_open(HS_UART0, DEFALUT_UART_BAUDRATE, UART_FLOW_CTRL_DISABLED, NULL);
}

#endif
