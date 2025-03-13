/*
 * dbg_cfg.h
 *
 *  Created on: 2024年8月26日
 *      Author: F980002587_
 */

#ifndef COMMON_DBG_CFG_H_
#define COMMON_DBG_CFG_H_

#define APP_DEBUG	0

#define USE_LOGGER 	1
#define LOGGER_CAN	0
#define LOGGER_UART	1

#if APP_DEBUG
#define DEBUG_CANBUS_MSG	1
//#define NM_TEST_FLAG 		1
#define ENABLE_WDT 			0
#else
#define DEBUG_CANBUS_MSG	0
//#define NM_TEST_FLAG 		0
#define ENABLE_WDT 			1
#endif

#endif /* COMMON_DBG_CFG_H_ */
