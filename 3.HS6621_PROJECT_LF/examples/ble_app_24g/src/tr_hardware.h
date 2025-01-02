/**
 * @file rf_hardware.h
 * @brief
 * @date Thu 09 Oct 2017 16:26:13 PM CST
 * @author shangzhou.hu@huntersun.com
 *
 * @addtogroup
 * @ingroup
 * @details
 *
 * @{
 */

#ifndef __RF_HARDWARE_H__
#define __RF_HARDWARE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

void tr_hardware_init(void); 
void rf_set_address(uint8_t *addr);

#endif
