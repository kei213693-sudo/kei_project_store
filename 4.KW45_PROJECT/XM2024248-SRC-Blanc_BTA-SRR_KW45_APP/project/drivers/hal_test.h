#ifndef _HAL_TEST_H
#define _HAL_TEST_H

#include "hal_pin_if.h"
#include "hal_uart_if.h"
#include "hal_tim_if.h"
#include "hal_spi_if.h"
#include "hal_can_if.h"
#include "hal_rtc_if.h"
#include "hal_flash_if.h"
#include "hal_wdt_if.h"
#include "hal_sys_if.h"
#include "hal_adc_if.h"
#include "hal_clock_if.h"
#include "dbg_printf.h"
#include <string.h>
void hal_test(void);
void SystemosIint(void);
void ComMange(void);
void DiagMange(void);
void Sleepwakeupinit(void);
uint8_t CANIf_Transmit(uint8_t channelnum, uint32_t id, uint8_t dlc, uint8_t *data);
void hal_can_cbk(const hal_can_pdu_st * pdata);

#endif

