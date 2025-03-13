/*
 * Copyright (C) 2024,宁波福尔达智能科技股份有限公司（重庆）
 * All rights reserved.
 * 文件名称：dbg_printf.c
 * 摘要：日志打印
 * 当前版本：V1.0.0,TaoWenzhuo,2024/5/11,初版创建
 * 历史版本：Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_uart_if.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "dbg_printf.h"
/* Private macro -------------------------------------------------------------*/
#define MAX_PRNT_BUF_LEN 200u

/* Private variables ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * 函 数 介 绍 : 格式化打印输出到串口
 * 参       数 : format [in]
 * 返   回  值 : 无
 * 备       注 : 无
 */
void dbg_Printf(char *format, ...)
{
    static char cur_printf_str_i8a[MAX_PRNT_BUF_LEN];
    va_list ap;
    uint8_t cur_print_str_len_u8 = 0;
    uint8_t i_u8 = 0;

    for (i_u8 = 0; i_u8 < MAX_PRNT_BUF_LEN; i_u8++)
    {
        cur_printf_str_i8a[i_u8] = '\0';
    }

    /* 格式化字符串处理 */
    va_start(ap, format);
    (void)vsprintf(cur_printf_str_i8a, (const char *)format, ap);
    va_end(ap);
    cur_printf_str_i8a[MAX_PRNT_BUF_LEN - 1] = '\0';
    for (i_u8 = 0; i_u8 < MAX_PRNT_BUF_LEN; i_u8++)
    {
        if ('\0' == cur_printf_str_i8a[i_u8])
        {
            break;
        }
    }
    cur_print_str_len_u8 = i_u8;

    for (i_u8 = 0; i_u8 < cur_print_str_len_u8; i_u8++)
    {
        hal_uart_if_writebyte(1, cur_printf_str_i8a[i_u8]);
    }
}

//hex打印
void dbg_Printf_hex(char *pData, int len)
{
    if((NULL != pData) && (0 != len))
    {
        char data = 0;
        for(int i = 0; i < len; i++)
        {
        	if(1 == (i % 50))
        	{
        		hal_wdt_if_feed(0);//太长时间不喂狗会重启
        	}
            data = *(pData + i);
            dbg_Printf("%02x ",data);
        }
        dbg_Printf("\r\n");
    }
}
