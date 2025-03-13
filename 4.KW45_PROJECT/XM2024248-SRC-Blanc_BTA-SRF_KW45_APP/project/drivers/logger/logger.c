/**
  ******************************************************************************
  * @file    logger.c
  * @brief   Debug log output utility implementation.
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

/* Includes ------------------------------------------------------------------*/

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "hal_uart_if.h"
#include "logger.h"
#include "fsl_os_abstraction.h"
#include "ringbuff.h"
#include "hal_rte.h"
#include "hal_can_if.h"
#include "app_dtc.h"
#include "uds_api.h"
#include "ComBusoff.h"
#include "ComSM.h"
#include "canbus_trans.h"
#include "E2Para.h"

uint8_t logcontent[LOG_PAYLOAD_LENTH];
uint8_t logLenth = 0;
uint8_t log_os_start_complete = 0;
#if (USE_LOGGER == 1)
#define MAX_HEX_STR         1
#define LOG_BUFFER_SIZE 65
#define MAX_HEX_STR_LENGTH  (LOG_BUFFER_SIZE*2)
char hexStr[MAX_HEX_STR][MAX_HEX_STR_LENGTH];
uint8_t hexStrIdx = 0;

static osa_msgq_handle_t log_xQueue = NULL;
static uint32_t log_lost_count = 0;
static uint8_t log_can_init_complete = 0;
static uint8_t log_uart_init_complete = 0;

#define LOG_QUEUE_LEN	(16U)

typedef unsigned char logstr[LOG_BUFFER_SIZE];

#if LOGGER_CAN
RING_BUFF_STRUCT_TYPEDEF ringbuff;
uint8_t bufflog[1024];
#endif

#endif /* #if USE_LOGGER == 1 */

/**
  *****************************************************************************
  * @brief  This function initalize the UART handle and UART IP.
  *
  * @param[in,out]	husart : handle to USART HW
  *
  * @return None
  *****************************************************************************
  */
void logUsartInit(void)
{
#if (USE_LOGGER == 1)
#if LOGGER_CAN
	if(0 == log_can_init_complete)
	{
		RingBuff_Init(&ringbuff, bufflog, sizeof(bufflog));
		log_can_init_complete = 1;
	}
#endif
#if LOGGER_UART
	if(0 == log_uart_init_complete)
	{
		if(log_os_start_complete)
		{
			if(KOSA_StatusSuccess != OSA_MsgQCreate(&log_xQueue, LOG_QUEUE_LEN, sizeof(logstr)))
			{
				logPrintf("Log create queue fail.\r\n");
				while(1);
			}
			log_lost_count = 0;
			log_uart_init_complete = 1;
		}
	}
#endif
#endif
}

/**
  *****************************************************************************
  * @brief  This function Transmit data via USART
  *
  * @param[in]	data    : data to be transmitted
  * @param[in]	dataLen : length of data to be transmitted
  *
  * @retval ERR_INVALID_HANDLE : in case the UART HW is not initalized yet
  * @retval others             : HAL status
  *****************************************************************************
  */
uint8_t logUsartTxCAN(uint8_t *data, uint16_t dataLen)
{
 #if (USE_LOGGER == 1)
  {
	#if LOGGER_CAN
	uint16_t wrdataLen;
	if(log_os_start_complete)
	{
		OSA_InterruptDisable();
	}
	wrdataLen = RingBuff_WriteData(&ringbuff, data, dataLen);

	if(log_os_start_complete)
	{
		OSA_InterruptEnable();
	}
	if(wrdataLen < dataLen)
	{
		/* buff full deal */
		;;;
	}
	#endif
  }
  #else
  {
    return 0;
  }
  #endif /* #if USE_LOGGER == 1 */
}

/**
  *****************************************************************************
  * @brief  This function Transmit data via USART
  *
  * @param[in]	data    : data to be transmitted
  * @param[in]	dataLen : length of data to be transmitted
  *
  * @retval ERR_INVALID_HANDLE : in case the UART HW is not initalized yet
  * @retval others             : HAL status
  *****************************************************************************
  */
uint8_t logUsartTxUART(uint8_t *data, uint16_t dataLen)
{
 #if (USE_LOGGER == 1)
  {
	#if LOGGER_UART
    return hal_uart_if_writebuff(1, data, dataLen);
	#endif
  }
  #else
  {
    return 0;
  }
  #endif /* #if USE_LOGGER == 1 */
}

/**
  *****************************************************************************
  * @brief  This function Transmit data via USART
  *
  * @param[in]	data    : data to be transmitted
  * @param[in]	dataLen : length of data to be transmitted
  *
  * @retval ERR_INVALID_HANDLE : in case the UART HW is not initalized yet
  * @retval others             : HAL status
  *****************************************************************************
  */
uint16_t logUsartGet(uint8_t *data, uint16_t dataLen)
{
	#if (USE_LOGGER == 1)
	#if LOGGER_CAN
	uint16_t rddataLen;
	if(log_os_start_complete)
	{
		OSA_InterruptDisable();
	}
	rddataLen = RingBuff_ReadData(&ringbuff, data, dataLen);
	if(log_os_start_complete)
	{
		OSA_InterruptEnable();
	}
	if(rddataLen < dataLen)
	{
		/* buff current is empty deal */
		;;;
	}
	return rddataLen;
	#else
	return 0;
	#endif
	#else
	return 0;
	#endif
}

/**
  *****************************************************************************
  * @brief  This function is used to write a formated string via the UART interface.
  *
  * @param[in]	format : data to be transmitted
  *
  * @return Number of data sent
  *****************************************************************************
  */
int logUsartQue(const char* format, ...)
{
	#if (USE_LOGGER == 1)
	char buf[LOG_BUFFER_SIZE];
	va_list argptr;
	va_start(argptr, format);
	int cnt = vsnprintf(buf, LOG_BUFFER_SIZE, format, argptr);
	va_end(argptr);

	#if LOGGER_UART
	/* */
	if(NULL != log_xQueue)
	{
	  QueueHandle_t handler    = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)(&log_xQueue));
	  if((uxQueueMessagesWaiting(handler) + 1) == LOG_QUEUE_LEN)
	  {
		  sprintf(buf + strlen(buf), "~~~ Log xQueue is full! ~~~ log lost count:%d\r\n", log_lost_count);
	  }

	  if(KOSA_StatusSuccess != OSA_MsgQPut(&log_xQueue, (osa_msg_handle_t)buf))
	  {
	//			  logUsart("Log xQueue PutMsg is fail! %s\r\n", buf);
		  log_lost_count ++;
		  cnt = -1;
	  }
	}
	else
	{
	//		  logUsart("Log xQueue is NULL!\r\n");
	  cnt = -1;
	}
	#endif

	#if LOGGER_CAN
	logUsartTxCAN((uint8_t*)buf, strlen(buf));
	#endif
	return cnt;
	#else
	{
	  return 0;
	}
	#endif /* #if USE_LOGGER == 1 */
}

/**
  *****************************************************************************
  * @brief  This function is used to write a formated string via the UART interface.
  *
  * @param[in]	format : data to be transmitted
  *
  * @return Number of data sent
  *****************************************************************************
  */
void logUsartQueProce(void)
{
	#if (USE_LOGGER == 1)
	#if LOGGER_CAN
	#define CAN_LOG_MSG_ID	0x493U
	static uint8_t CAN_Log_msg[64];
	static uint32_t canlogSndErrorCounter = 0;
	static hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
	if(CanSM_GetAppTxMsgIsEnable() && (Get_canBusoffFlag() == 0) && ( UdsApi_get_normal_tx_sta()) && (Get_PMState() == PMS_NORMAL))
	{
		if(RingBuff_GetDataLength(&ringbuff) > 0)
		{
			static hal_can_pdu_st pdu={0};
			pdu.m_u32_canid = CAN_LOG_MSG_ID;
			pdu.m_id_type_e = HAL_CAN_ID_TYPE_StAND;
			pdu.m_frame_type_e = HAL_CAN_FRAME_TYPE_CANFD;
			pdu.m_u8_pdata = CAN_Log_msg;
			pdu.m_u8_dlc = sizeof(CAN_Log_msg);
			if(HAL_RESULT_SUCCESS == l_i8_result)
			{
				memset(CAN_Log_msg, 0, sizeof(CAN_Log_msg));
				logUsartGet(CAN_Log_msg, sizeof(CAN_Log_msg));
			}
			OSA_InterruptDisable();
			l_i8_result = hal_can_if_write(0, &pdu, 1);
			OSA_InterruptEnable();
			if(HAL_RESULT_SUCCESS != l_i8_result)
			{
				canlogSndErrorCounter ++;
				//logPrintf("hal_can_if_write(1) return result:%d, canlogSndErrorCounter = %d\r\n", l_i8_result, canlogSndErrorCounter);
			}
			else
			{
				canlogSndErrorCounter = 0;
			}
		}
	}
	#endif
	OSA_TimeDelay(5);
	#if LOGGER_UART
	static logstr logContext;
	if(KOSA_StatusSuccess == OSA_MsgQGet(&log_xQueue, (osa_msg_handle_t)logContext, osaWaitNone_c))
	{
		logUsartTxUART(logContext, strlen(logContext));
	}
	else
	{
		//logUsart("Log MsgGet is fail!\r\n");
	}
	#endif
	#else
	vTaskSuspend(NULL);
	#endif /* #if USE_LOGGER == 1 */
}

/**
  *****************************************************************************
  * @brief  This function is used to write a formated string via the UART interface.
  *
  * @param[in]	format : data to be transmitted
  *
  * @return Number of data sent
  *****************************************************************************
  */
int logUsart(const char* format, ...)
{
  #if (USE_LOGGER == 1)
  {
    char buf[LOG_BUFFER_SIZE];
    va_list argptr;
    va_start(argptr, format);
    int cnt = vsnprintf(buf, LOG_BUFFER_SIZE, format, argptr);
    va_end(argptr);

#if LOGGER_CAN
    logUsartTxCAN((uint8_t*)buf, strlen(buf));
#endif
#if LOGGER_UART
    logUsartTxUART((uint8_t*)buf, strlen(buf));
#endif
    return cnt;
  }
  #else
  {
    return 0;
  }
  #endif /* #if USE_LOGGER == 1 */
}

/**
  *****************************************************************************
  * @brief  helper to convert hex data into formated string
  *
	* @param[in]	data : pointer to buffer to be dumped.
	* @param[in]	dataLen : buffer length
  *
  * @return pointer to converted data
  *****************************************************************************
  */
char* hex2Str(unsigned char * data, size_t dataLen)
{
  #if (USE_LOGGER == 1)
  {
    unsigned char * pin = data;
    const char * hex = "0123456789ABCDEF";
    char * pout = hexStr[hexStrIdx];
    uint8_t i = 0;
    uint8_t idx = hexStrIdx;

    if( dataLen > (MAX_HEX_STR_LENGTH/2) )
    {
      dataLen = (MAX_HEX_STR_LENGTH/2);
    }

    if(dataLen == 0)
    {
      pout[0] = 0;
    }
    else
    {
      for(; i < dataLen - 1; ++i)
      {
          *pout++ = hex[(*pin>>4)&0xF];
          *pout++ = hex[(*pin++)&0xF];
      }
      *pout++ = hex[(*pin>>4)&0xF];
      *pout++ = hex[(*pin)&0xF];
      *pout = 0;
    }

    hexStrIdx++;
    hexStrIdx %= MAX_HEX_STR;

    return hexStr[idx];
  }
  #else
  {
    return NULL;
  }
  #endif /* #if USE_LOGGER == 1 */
}

static void printch(char ch)
{
		if ((logLenth >= 0) && (logLenth < 63))
		{
			logcontent[logLenth] = ch;
			logLenth++;
		}
}

static void printdec(int dec)
{
		static uint8_t cnt = 0;
		if (dec == 0)
		{
				if (cnt == 0)
				{
						printch('0');
				}
				cnt = 0;
				return;
		}
		if ((logLenth >= 0) && (logLenth < 63))
		{
				cnt++;
				printdec(dec/10);
				printch((char)(dec%10 + '0'));
		}
}

static void printflt(double flt)
{
		int tmpint = 0;

		if ((logLenth >= 0) && (logLenth < 63))
		{
				tmpint = (int)flt;
				if (tmpint == 0)
				{
						printch('0');
				}
				else
				{
						printdec(tmpint);
				}
				printch('.');
				flt = flt - tmpint;
				if (flt < 0.1)
				{
						printch('0');
				}
				tmpint = (int)(flt * 1000);
				printdec(tmpint);
		}
}

static void printstr(char* str)
{
		uint8_t len = strlen(str);
		if ((logLenth >= 0) && (logLenth < 63) && ((logLenth + len) <= 63))
		{
				memcpy(&logcontent[logLenth], str, len);
				logLenth+=len;
		}
}

static void printbin(int bin)
{
		if (bin == 0)
		{
				printstr("0b");
				return;
		}

		if ((logLenth >= 0) && (logLenth < 63))
		{
				printbin(bin/2);
				printch((char)(bin%2 + '0'));
		}
}

static void printhex(int hex)
{
		if (hex == 0)
		{
				printstr("0x");
				return;
		}

		if ((logLenth >= 0) && (logLenth < 63))
		{
				printhex(hex/16);
				if ((hex%16) < 10)
				{
						printch((char)(hex%16 + '0'));
				}
				else
				{
						printch((char)(hex%16 - 10 + 'a'));
				}
		}
}

void canlogPrintf(char *fmt, ...)
{
		double	vargFlt = 0;
		int			vargInt	=	0;
		char*		vargpch	=	NULL;
		char		vargch	=	0;
		char*		pfmt		=	NULL;
		bool		rfEnabled = false;
		bool		bleEnabled = false;
		bool		uwbEnabled = false;

		va_list	vp;

		va_start(vp, fmt);
		pfmt = fmt;
		memset(logcontent, 0x00, LOG_PAYLOAD_LENTH);

		while (*pfmt)
		{
				if (*pfmt == '%')
				{
						switch (*(++pfmt))
						{
							case 'c':
								vargch = va_arg(vp, int);
								printch(vargch);
								break;

							case 'd':
							case 'i':
								vargInt = va_arg(vp, int);
								printdec(vargInt);
								break;

							case 'f':
								vargFlt = va_arg(vp, double);
								printflt(vargFlt);
								break;

							case 's':
								vargpch = va_arg(vp, char*);
								printstr(vargpch);
								break;

							case 'b':
							case 'B':
								vargInt = va_arg(vp, int);
								printbin(vargInt);
								break;

							case 'e':
							case 'E':
								if (canlog_Eflag & 0x1)
								{
									// enable send ble log
									bleEnabled = true;
								}
								break;

							case 'r':
							case 'R':
								if (canlog_Eflag & 0x2)
								{
									// enable send rf log
									rfEnabled = true;
								}
								break;

							case 'u':
							case 'U':
								if (canlog_Eflag & 0x4)
								{
									// enable send uwb log
									uwbEnabled = true;
								}
								break;

							case 'x':
							case 'X':
								vargInt = va_arg(vp, int);
								printhex(vargInt);
								break;

							default:
								break;
						}
						pfmt++;
				}
				else
				{
						printch(*pfmt++);
				}
		}
		logLenth = 0;
		va_end(vp);
		if ((rfEnabled == true) || (bleEnabled == true) || (uwbEnabled == true))
		{
			canbusTransSendMsgFrame(0x490, LOG_PAYLOAD_LENTH, logcontent);
		}
		else
		{
			return;
		}

}
