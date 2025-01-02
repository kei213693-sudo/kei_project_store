/*
 * @project   Pazell
 * @copyright Copyright (c) 2021 OnMicro Corp.
 * @author    wei.lu@onmicro.com.cn
 * @license   SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file   gzll_port.h
 * @brief  Nordic Gazell porting header.
 *
 * @addtogroup Gazell
 * @{
 */
#ifndef _GZLL_MACROS_H_
#define _GZLL_MACROS_H_

//#include "om62xx.h"
//#include "panacea.h"

/**
 *  OM6229 is not C51.
 */
#if defined(__arm__)
#define code const
#define xdata
#define pdata
//#define data 
#define reentrant
#define __no_init
#endif

#endif
