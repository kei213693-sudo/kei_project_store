/**
 * @file example_rtc.c
 * @brief example for RTC
 * @date 2017/07/12 16:12:53
 * @author liqiang
 *
 * @defgroup EXAMPLE_RTC RTC Example
 * @ingroup EXAMPLE
 * @brief example for RTC
 * @details 
 *
 * @{
 */

/*********************************************************************
 * INCLUDES
 */
#include "peripheral.h"

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/**
 * @brief rtc_second_handler()
 *
 * @param[in] tm  
 *
 * @return 
 **/
void rtc_second_handler(const struct tm *tm)
{
    log_debug("time: %s\n", asctime(tm));
}

/**
 * @brief rtc_alarm_handler()
 *
 * @param[in] _tm  
 *
 * @return 
 **/
void rtc_alarm_handler(const struct tm *_tm)
{
    log_debug("alarm: %s\n", asctime(_tm));
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void example_rtc(void)
{
    struct tm tm = {0};

    // Select 32k clock source
    pmu_select_32k(PMU_32K_SEL_32768HZ_XTAL);

    // Check reboot reason
    if(pmu_reboot_reason() == PMU_REBOOT_FROM_ULTRA_DEEP_SLEEP)
    {
        /*
         * If wakeup from ultra-deep-sleep, RTC should be running
         *
         * If rtc_time_set(&tm, NULL) and pmu_xtal32k_enable_in_deep_sleep(true), RTC can come into deep-sleep mode
         */

        // restore alarm callback
        rtc_alarm_set(NULL, rtc_alarm_handler);
        // Run
        rtc_start();

        // Get reboot time
        rtc_time_get(&tm);
        log_debug("reboot_time: %s\n", asctime(&tm));
    }
    else
    {
        // 2017.6.13 20:15:00
        tm.tm_year = 2017 - 1900;
        tm.tm_mon  = 6 - 1;
        tm.tm_mday = 13;
        tm.tm_hour = 20;
        tm.tm_min  = 15;
        tm.tm_sec  = 0;

        // Init RTC
        rtc_init();

        //Set time
        rtc_time_set(&tm, NULL);
//        rtc_time_set(&tm, rtc_second_handler);

        // Set alarm
        tm.tm_sec  = 30;
        rtc_alarm_set(&tm, rtc_alarm_handler);

        // Run
        rtc_start();
    }
}

/** @} */

