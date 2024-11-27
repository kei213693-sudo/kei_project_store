/*
 * bsp_time.c
 *
 *  Created on: 2024年9月11日
 *      Author: lj
 *	  Abstract: 用于时间信息处理，一般是基于rtc实现，代码基于c库<time.h>实现
 */
#include <stdint.h>
#include <string.h>
#include <time.h>
//要点就是time_t(u32计数值)和struct tm（格式化日历值）两种类型的值相互转化

// time_t这种类型就是用来存储从1970年到现在经过了多少秒，就是时间戳stamp

// tm结构具体内容如下，和bsp_time_calendar_t配合使用
// struct tm {
//    int tm_sec;         /* 秒，范围从 0 到 59        */
//    int tm_min;         /* 分，范围从 0 到 59        */
//    int tm_hour;        /* 小时，范围从 0 到 23        */
//    int tm_mday;        /* 一月中的第几天，范围从 1 到 31    */
//    int tm_mon;         /* 月，范围从 0 到 11        */
//    int tm_year;        /* 自 1900 年起的年数        */
//    int tm_wday;        /* 一周中的第几天，范围从 0 到 6    */
//    int tm_yday;        /* 一年中的第几天，范围从 0 到 365    */
//    int tm_isdst;       /* 夏令时                */
// };

//调用api要注意
//time获取Unix、gmtime转换tm、asctime转换str，所表示的时间都是UTC时间，只是数据类型不一样
//mktime获取Unix、localtime转换tm、ctime转换str，所表示的时间都是经过时区转换后的本地时间

//灵活运用strftime来自定义时间格式，参数参考下列信息
// %a 星期几的简写
// %A 星期几的全称
// %b 月分的简写
// %B 月份的全称
// %c 标准的日期的时间串
// %C 年份的后两位数字
// %d 十进制表示的每月的第几天
// %D 月/天/年
// %e 在两字符域中，十进制表示的每月的第几天
// %F 年-月-日
// %g 年份的后两位数字，使用基于周的年
// %G 年分，使用基于周的年
// %h 简写的月份名
// %H 24小时制的小时
// %I 12小时制的小时
// %j 十进制表示的每年的第几天
// %m 十进制表示的月份
// %M 十时制表示的分钟数
// %n 新行符
// %p 本地的AM或PM的等价显示
// %r 12小时的时间
// %R 显示小时和分钟：hh:mm
// %S 十进制的秒数
// %t 水平制表符
// %T 显示时分秒：hh:mm:ss
// %u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）
// %U 第年的第几周，把星期日做为第一天（值从0到53）
// %V 每年的第几周，使用基于周的年
// %w 十进制表示的星期几（值从0到6，星期天为0）
// %W 每年的第几周，把星期一做为第一天（值从0到53）
// %x 标准的日期串
// %X 标准的时间串
// %y 不带世纪的十进制年份（值从0到99）
// %Y 带世纪部分的十制年份
// %z，%Z 时区名称，如果不能得到时区名称则返回空字符。
// %% 百分号

#include "bsp_time.h"
#include "rtc.h"
#include "FreeRTOS.h"
/*
如果是裸机需要自己参考下列信息定义返回宏
#define pdFALSE                                  ( ( BaseType_t ) 0 )
#define pdTRUE                                   ( ( BaseType_t ) 1 )

#define pdPASS                                   ( pdTRUE )
#define pdFAIL                                   ( pdFALSE )
*/

/*******************************全局变量定义*******************************/

/*******************************内部变量定义*******************************/
static char g_fmStr[BSP_TIME_FORMAT_BUF_LEN] = {0};//存储字符串形式的时间信息

/*******************************内部函数的声明*******************************/
static int32_t bsp_time_timezone(void);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：时间信息模块初始化
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：
**************************************************************/
int32_t bsp_time_init(void)
{
    int32_t i32ret = pdPASS;

    RTC_Init();//rtc初始化

    //printf("bsp_time_clock:%d\r\n",clock());//打印处理器时钟所使用的时间

    return i32ret;
}

/**************************************************************
*函数功能描述 ：时间信息模块去初始化
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：
**************************************************************/
int32_t bsp_time_deinit(void)
{
    int32_t i32ret = pdPASS;

    //RTC_DeInit();//rtc去初始化

    return i32ret;
}

/**************************************************************
*函数功能描述 ：时区校准处理，东区+，西区-
*函数参数 ：
*函数返回值 ：时区校准值
*备注 ：
**************************************************************/
static int32_t bsp_time_timezone(void)
{
    return (BSP_TIME_TIMEZONE_TYPE * BSP_TIME_TIMEZONE_UNIT);
}

/**************************************************************
*函数功能描述 ：时间信息设置，通过bsp_time_calendar_t
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：先将bsp_time_calendar_t格式数据转换为time_t格式，然后再转换为struct tm格式，最后写入到rtc计数寄存器中;
这个函数没有时区问题，直接按照设定时间配置
**************************************************************/
int32_t bsp_time_set_caledar(bsp_time_calendar_t stCalendar)
{
    int32_t i32ret = pdPASS;
    
    time_t time_t_value;     //定义time_t类型的设置时间结构体
    struct tm tm_value;      //定义struct tm结构的设置时间结构体

    tm_value.tm_year = (int)(stCalendar.year - 1900);  //从2000年开始算起
    tm_value.tm_mon  = (int)(stCalendar.month - 1);    //月
    tm_value.tm_mday = (int)stCalendar.day;            //日
    tm_value.tm_hour = (int)stCalendar.hour;           //时
    tm_value.tm_min  = (int)stCalendar.min;            //分
    tm_value.tm_sec  = (int)stCalendar.sec;            //秒

    time_t_value = mktime(&tm_value);//得到计数初值，将struct tm转换为time_t的格式

    if(time_t_value != 0xFFFFFFFF)
    {//设置RTC计数器的值，并重新启动计数
        RTC_SetCounter(time_t_value);
    }
    else
    {
        i32ret = pdFAIL;
    }

    return i32ret;
}

/**************************************************************
*函数功能描述 ：时间信息设置，通过u32Stamp时间戳数值
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：time_t就是时间戳，但是要注意时区问题
**************************************************************/
int32_t bsp_time_set_stamp(uint32_t u32Stamp)
{
    int32_t i32ret = pdPASS;

    time_t time_t_value = (time_t)u32Stamp;     //定义time_t类型的设置时间结构体
    if(time_t_value != 0xFFFFFFFF)
    {//设置RTC计数器的值，并重新启动计数
        time_t_value += bsp_time_timezone();//时区进行校准，考虑零时区和本地时间的差别
        RTC_SetCounter(time_t_value);
    }
    else
    {
        i32ret = pdFAIL;
    }

    return i32ret;
}

/**************************************************************
*函数功能描述 ：时间信息获取，以bsp_time_calendar_t格式传递出去
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：这个函数没有时区问题，直接按照设定时间获取
**************************************************************/
int32_t bsp_time_get_caledar(bsp_time_calendar_t *pStCalendar)
{
    int32_t i32ret = pdPASS;

    if(NULL != pStCalendar)
    {
        time_t time_t_value;     //定义time_t类型的设置时间结构体
        struct tm *p_tm_value;      //定义struct tm结构的设置时间结构体

        time_t_value = RTC_GetCounter();//获取当前RTC计数值

        p_tm_value = localtime(&time_t_value);//把从1970-1-1零点零分到当前时间系统所偏移的秒数时间转换为本地日历时间，将time_t转换为struct tm的格式
        if(NULL != p_tm_value)
        {
            pStCalendar->year  = (uint16_t)(p_tm_value->tm_year + 1900);  //从1900年起
            pStCalendar->month = (uint8_t)(p_tm_value->tm_mon + 1);         //月
            pStCalendar->day   = (uint8_t)p_tm_value->tm_mday;            //日
            pStCalendar->hour  = (uint8_t)p_tm_value->tm_hour;            //时
            pStCalendar->min   = (uint8_t)p_tm_value->tm_min;             //分
            pStCalendar->sec   = (uint8_t)p_tm_value->tm_sec;             //秒
        }
        else
        {
            i32ret = pdFAIL;
        }
    }
    else
    {
        i32ret = pdFAIL;
    }

    return i32ret;
}

/**************************************************************
*函数功能描述 ：时间信息获取，以时间戳格式传递出去
*函数参数 ：
*函数返回值 ：时间戳
*备注 ：
**************************************************************/
uint32_t bsp_time_get_stamp(void)
{
    uint32_t u32Stamp = 0;

    u32Stamp = (uint32_t)RTC_GetCounter();//获取当前RTC计数值
    // u32Stamp += bsp_time_timezone();//时区进行校准，考虑零时区和本地时间的差别

    return u32Stamp;
}

/**************************************************************
*函数功能描述 ：获取sting格式的时间信息
*函数参数 ：
*函数返回值 ：存储时间字符串的指针
*备注 ：建议调用strftime来自定义时间格式，asctime和ctime返回的字符串格式是固定的"Sat Jan 12 01:25:29 2008"
**************************************************************/
char *bsp_time_get_string(void)
{
    time_t time_t_value;
    struct tm *p_tm_value;

    time_t_value = RTC_GetCounter();//获取当前RTC计数值
    p_tm_value = localtime(&time_t_value);

    if(NULL != p_tm_value)
    {
        memset(g_fmStr,0x00,BSP_TIME_FORMAT_BUF_LEN);
        strftime(g_fmStr,BSP_TIME_FORMAT_BUF_LEN,"%Y-%m-%d %H:%M:%S",p_tm_value);
    }

    return g_fmStr;
}

/**************************************************************
*函数功能描述 ：星期信息获取
*函数参数 ：
*函数返回值 ：星期数，星期一到星期日对应从0到6
*备注 ：
**************************************************************/
uint8_t bsp_time_get_week(void)
{
    uint8_t u8Week = 0xff;
    time_t time_t_value;
    struct tm *p_tm_value;

    time_t_value = RTC_GetCounter();//获取当前RTC计数值
    p_tm_value = localtime(&time_t_value);

    if(NULL != p_tm_value)
    {
        u8Week = p_tm_value->tm_wday;
    }

    return u8Week;
}

/**************************************************************
*函数功能描述 ：当年天数获取
*函数参数 ：
*函数返回值 ：当年的天数，范围从 0 到 365
*备注 ：
**************************************************************/
uint16_t bsp_time_get_ydays(void)
{
    uint16_t u16Ydays = 0xffff;
    time_t time_t_value;
    struct tm *p_tm_value;

    time_t_value = RTC_GetCounter();//获取当前RTC计数值
    p_tm_value = localtime(&time_t_value);

    if(NULL != p_tm_value)
    {
        u16Ydays = p_tm_value->tm_yday;
    }

    return u16Ydays;
}

/**************************************************************
*函数功能描述 ：获取年份类型，判定是否是闰年
*函数参数 ：
*函数返回值 ：1闰年，0平年
*备注 ：
**************************************************************/
uint8_t bsp_time_get_year_type(uint16_t year)
{
	uint8_t u8ret = 0;
	
    if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)
            {
                u8ret = 1;//如果以00结尾,还要能被400整除
            }   
			else
            {
                u8ret = 0;
            }
		}
        else
        {
            u8ret = 1;
        }
	}
    else
    {
        u8ret = 0;
    }
	
	return u8ret;
}
