/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_rte.h
 * 摘要：
 *          文件主要描述rte模块对外接口
 * 当前版本：
 * 历史版本：
 */
#ifndef HAL_RTE_H
#define HAL_RTE_H

#include "hal_pin_cfg.h"
#include"hal_spi_cfg.h"

//pin_index
#define CANID1_INDEX        HAL_PIN_INDEX8
//PD2  HAL_PIN_INDEX11
#define CANID2_INDEX        HAL_PIN_INDEX11
//PB4 HAL_PIN_INDEX1
#define LED1_INDEX			HAL_PIN_INDEX1
//PB5 HAL_PIN_INDEX2
#define CANSTB_INDEX		HAL_PIN_INDEX2
//PD1 HAL_PIN_INDEX4
#define SW_12V_INDEX		HAL_PIN_INDEX10
//PC0 HAL_PIN_INDEX6
#define UWB_RST_INDEX		HAL_PIN_INDEX17
//PC6 HAL_PIN_INDEX11
#define UWB_RDY_INDEX		HAL_PIN_INDEX19
//PC7 HAL_PIN_INDEX12
#define UWB_INT0_INDEX		HAL_PIN_INDEX20
//PB0 HAL_PIN_INDEX13
#define J29D6_CS_INDEX      HAL_PIN_INDEX21
//spi_index
#define J29D6_SPI_INDEX		HAL_SPI_CHANNEL1
//#define K2910_SPI_INDEX		HAL_SPI_CHANNEL0

//adc_index
#define DB29_ADC_INDEX			0
#define BAT_ADC_INDEX				1

//can_index
#define CANFD_INDEX				0

//uart_index
#define DEBUG_UART_INDEX		1

#endif

