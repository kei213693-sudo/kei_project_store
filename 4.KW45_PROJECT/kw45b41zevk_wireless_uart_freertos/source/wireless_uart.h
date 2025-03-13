/*! *********************************************************************************
 * \defgroup Wireless UART Application
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2021 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Wireless UART application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef WIRELESS_UART_H
#define WIRELESS_UART_H

#include "hal_can_typedef.h"

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/* Profile Parameters */
#define gScanningTime_c        10   /* 10 s*/

/* Enable/Disable Controller Adv/Scan/Connection Notifications */
#ifndef gUseControllerNotifications_c
#define gUseControllerNotifications_c 1
#endif

/* Receive Notifications in a callback registered in the Controller
   instead of Host GAP Generic callback */
#ifndef gUseControllerNotificationsCallback_c
#define gUseControllerNotificationsCallback_c 0
#endif

#if (defined(gAppButtonCnt_c) && (gAppButtonCnt_c == 1))
  /* switch press timer timeout */
  #ifndef gSwitchPressTimeout_c
  #define gSwitchPressTimeout_c    (1000UL)
  #endif
  /* switch press threshold (number of key presses to toggle the GAP role) */
  #ifndef gSwitchPressThreshold_c
  #define gSwitchPressThreshold_c    (2)
  #endif
#endif

#define gLinkMonitorNumMax  4//最大监控数量
#define GENFSKBLECONINFOCMD     (0x5A)//识别有效连接信息的句柄

#define BTLE_NONE        0
#define BTLE_CONNECT     1
#define BTLE_DISCONN     2

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern gapConnectionRequestParameters_t gConnReqParams;
extern gapScanningParameters_t          gScanParams;
extern gapAdvertisingData_t             gAppAdvertisingData;
extern gapScanResponseData_t            gAppScanRspData;
extern gapAdvertisingParameters_t       gAdvParams;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

extern void app_can_rx_handle(const hal_can_pdu_st * pdata);

#endif /* _APP_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
