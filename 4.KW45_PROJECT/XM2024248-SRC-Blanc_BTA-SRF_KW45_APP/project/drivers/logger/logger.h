/**
  ******************************************************************************
  * @file    logger.h
  * @brief   Header for logger module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LOGGER_H
#define LOGGER_H

#include "dbg_cfg.h"

#define logPrintf 		logUsartQue
#define logPrintfImmedi	logUsart

#define shell_write		logPrintf
#define dbg_Printf		logPrintf

#define LOG_PAYLOAD_LENTH			64
#define LOG_MESSAGE_LENTH			60
#define LOG_STORAGE_LENTH			8
#define LOG_ERRORCODE_LENTH		4

/*!
 *****************************************************************************
 *  \brief  Writes out a formated string via UART interface
 *
 *  This function is used to write a formated string via the UART interface.
 *
 *****************************************************************************
 */
void logUsartInit(void);

/*!
 *****************************************************************************
 *  \brief  Writes out a formated string via UART interface
 *
 *  This function is used to write a formated string via the UART interface.
 *
 *****************************************************************************
 */
int logUsartQue(const char* format, ...);

/*!
 *****************************************************************************
 *  \brief  Writes out a formated string via UART interface
 *
 *  This function is used to write a formated string via the UART interface.
 *
 *****************************************************************************
 */
void logUsartQueProce(void);

/*!
 *****************************************************************************
 *  \brief  Writes out a formated string via UART interface
 *
 *  This function is used to write a formated string via the UART interface.
 *
 *****************************************************************************
 */
int logUsart(const char* format, ...);


///*!
// *****************************************************************************
// *  \brief  Writes out a formated string via UART interface
// *
// *  This function is used to write a formated string via the UART interface.
// *
// *****************************************************************************
// */
//uint16_t logUsartGet(uint8_t *data, uint16_t dataLen);

/*!
 *****************************************************************************
 *  \brief  helper to convert hex data into formated string
 *
 *  \param[in] data : pointer to buffer to be dumped.
 *
 *  \param[in] dataLen : buffer length
 *
 *  \return hex formated string
 *
 *****************************************************************************
 */
char* hex2Str(unsigned char * data, size_t dataLen);

typedef struct
{
	char buf[65];
	char len;
}canlogDef;
void canlogPrintf(char *fmt, ...);
#endif /* LOGGER_H */

