/*
 * bsp_time.h
 *
 *  Created on: 2024年9月11日
 *      Author: lj
 *	  Abstract: 用于时间信息处理，一般是基于rtc实现，代码基于c库<time.h>实现
 */
#ifndef _BSP_TIME_H_
#define _BSP_TIME_H_

/*******************************头文件*******************************/

/*******************************宏定义*******************************/
#define BSP_TIME_FORMAT_BUF_LEN          80//存储strinng格式的时间信息

//时区类型，共24个，东12+西12，东区+，西区-
#define BSP_TIME_TIMEZONE_UNIT           3600//60*60，一小时的s数
#define BSP_TIME_TIMEZONE_TYPE           (0)//(8)//默认北京时间，东8时区，Unix需要+28800（8*60*60）

/*******************************结构体、枚举******************************/
typedef struct bsp_time_calendar_t
{//日历结构存储时间的结构体
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  min;
    uint8_t  sec;
    uint8_t  res;//预留
}bsp_time_calendar_t;

/*******************************全局变量的外部声明*******************************/

/*******************************全局函数的声明*******************************/
extern int32_t bsp_time_init(void);
extern int32_t bsp_time_deinit(void);

extern int32_t bsp_time_set_caledar(bsp_time_calendar_t stCalendar);
extern int32_t bsp_time_set_stamp(uint32_t u32Stamp);

extern int32_t bsp_time_get_caledar(bsp_time_calendar_t *pStCalendar);
extern uint32_t bsp_time_get_stamp(void);

extern char *bsp_time_get_string(void);
extern uint8_t bsp_time_get_week(void);
extern uint16_t bsp_time_get_ydays(void);
extern uint8_t bsp_time_get_year_type(uint16_t year);

#endif /* _BSP_TIME_H_ */

