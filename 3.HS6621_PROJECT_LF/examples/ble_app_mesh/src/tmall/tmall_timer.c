/**
 ****************************************************************************************
 *
 * @file app.c
 *
 * @brief BLE mesh tmall timer entry.
 *
 * Copyright (C) OnMicro 2021
 *
 * Author: OnMicro SW Team
 *
 ****************************************************************************************
 */

/*********************************************************************
 * INCLUDES
 */

#include <stdint.h>
#include <string.h>
#include <time.h>
#include "co.h"
#include "rtc.h"
#include "tmall_timer.h"
#include "app.h"
#include "tmall_led_demo.h"

#if APP_MESH_TMALL
/*********************************************************************
 * MACROS
 */
#define CUR_MS                          (co_time() * 5 / 16)

/*********************************************************************
 * TYPEDEFS
 */

enum {
    TMALL_TIMER_TYPE_ONESHOT = 1,
    TMALL_TIMER_TYPE_PERIODIC,
};

typedef struct {
    uint16_t attr_type;
    uint16_t attr_value;
} tmall_timer_op_t;

typedef struct {
    ///timer index defined by tmall-genie
    uint8_t  timer_index;
    ///0xff: deleted  1: oneshot  2: periodic
    uint8_t  timer_type;
    uint8_t  schd_week;
    uint8_t  op_count;
    tmall_timer_op_t timer_ops[TMALL_TIMER_OP_NUM];
    union {
        uint32_t unix_sec;
        uint16_t schd_min;
    };
} tmall_attr_timer_t;

/*********************************************************************
 * LOCAL VARIABLES
 */
static tmall_attr_timer_t m_tmall_timers[TMALL_TIMER_NUM];
static uint8_t m_tmall_timer_index_pending = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void rtc_alarm_callback(const struct tm *timp);

static int _tmall_attr_value_bytes(uint16_t attr_type)
{
    /* porting: support more attr type here. */
    if ((attr_type == ATTR_TYPE_ONOFF) ||
        (attr_type == ATTR_TYPE_MAIN_LAMP_ONOFF) ||
        (attr_type == ATTR_TYPE_BG_LAMP_ONOFF)) {
        return 1;
    }
    else if ((attr_type == ATTR_TYPE_LIGHTNESS) ||
             (attr_type == ATTR_TYPE_CCT) ||
             (attr_type == ATTR_TYPE_HSL)) {
        return 2;
    }
    /* Not support attr type */
    return -1;
}

static void _tmall_timer_emit(void)
{
    int index,index_periodic;
    uint32_t oneshot_min,oneshot_unix_sec = 0;

    /* Disable the current hw alarm timer. */
    m_tmall_timer_index_pending = 0;
    rtc_alarm_set(NULL,NULL);

    /* Get the latest oneshot timer. */
    for(index = 0; index < TMALL_TIMER_NUM; index++)
    {
        if(TMALL_TIMER_TYPE_ONESHOT == m_tmall_timers[index].timer_type){
            oneshot_unix_sec = m_tmall_timers[index].unix_sec;
            break;
        }
    }
    if(oneshot_unix_sec != 0)
    {
        struct tm datetime;
        localtime_r((const time_t *)&oneshot_unix_sec,&datetime);
        /* Get the total minutes in a day */
        oneshot_min = datetime.tm_min + datetime.tm_hour * 60;
        if(datetime.tm_sec)oneshot_min += 1;
    }
    else{
        oneshot_min = 24*60;
    }

    /* Enumerate periodic timers. */
    index_periodic = 0;
    while(TMALL_TIMER_TYPE_PERIODIC == m_tmall_timers[index_periodic].timer_type) {
        struct tm datetime;
        rtc_time_get(&datetime);
        uint16_t curr_min = datetime.tm_hour * 60 + datetime.tm_min;
        /* .tm_wday: the number of days since Sunday,in the range 0 to 6. */
        uint8_t wday = datetime.tm_wday;
        /* schedule: bit6=Sunday, bit0=Monday */
        if (wday == 0) wday = 6;
        else           wday--;
        /* Check the periodic timer is earlier than the nearest oneshot time */
        if ((m_tmall_timers[index_periodic].schd_week & (1 << wday)) &&
            /* Skip it if this periodic timer is elapsed today. */
            (m_tmall_timers[index_periodic].schd_min > curr_min) &&
            /* Skip it if this periodic timer is later than the nearest oneshot timer. */
            (m_tmall_timers[index_periodic].schd_min < oneshot_min)) {
            datetime.tm_hour = m_tmall_timers[index_periodic].schd_min / 60;
            datetime.tm_min  = m_tmall_timers[index_periodic].schd_min % 60;
            datetime.tm_sec  = 0;
            /* Emit the periodic alarm timer. */
            //printf("[%07lu] set alarm %d at %s\n",CUR_MS,m_tmall_timers[index_periodic].timer_index,asctime(&datetime));
            rtc_alarm_set(&datetime,rtc_alarm_callback);
            m_tmall_timer_index_pending = m_tmall_timers[index_periodic].timer_index;
            index_periodic++;
            return;
        }
        index_periodic++;
    }
    /* Emit the oneshot alarm timer. */
    if(oneshot_unix_sec != 0){
        struct tm datetime;
        localtime_r((const time_t *)&oneshot_unix_sec,&datetime);
        __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Tmall Timing finish: %s\n",asctime(&datetime));
        //printf("[%07lu] set alarm %d at %lx %s\n",CUR_MS,m_tmall_timers[index].timer_index,oneshot_unix_sec,asctime(&datetime));
        rtc_alarm_set(&datetime,rtc_alarm_callback);
        m_tmall_timer_index_pending = m_tmall_timers[index].timer_index;
    }
}

static int _tmall_timer_preserve(int index)
{
    if(index >= TMALL_TIMER_NUM){
        /* Always overwrite the most far one. */
        index = TMALL_TIMER_NUM - 1;
    }
    if(index < TMALL_TIMER_NUM - 1) {
        /* Move the trail timers. */
        memmove(&m_tmall_timers[index + 1], &m_tmall_timers[index], (TMALL_TIMER_NUM - 1 - index) * sizeof(tmall_attr_timer_t));
    }
    /* This index is preserved as a spare one. */
    return index;
}

static int _tmall_timer_get(uint8_t timer_index)
{
    int index;
    for(index = 0; index < TMALL_TIMER_NUM; index++) {
        if (timer_index == m_tmall_timers[index].timer_index) {
            return index;
        }
    }
    return -1;
}

static int _tmall_timer_delete(uint8_t timer_index)
{
    int index;
    for(index = 0; index < TMALL_TIMER_NUM; index++)
    {
        if (timer_index == m_tmall_timers[index].timer_index){
            if (index < TMALL_TIMER_NUM - 1){
                memmove(&m_tmall_timers[index], &m_tmall_timers[index + 1],(TMALL_TIMER_NUM - 1 - index) * sizeof(tmall_attr_timer_t));
                memset(&m_tmall_timers[TMALL_TIMER_NUM - 1], 0xff, sizeof(tmall_attr_timer_t));
            }
            else{
                memset(&m_tmall_timers[index], 0xff, sizeof(tmall_attr_timer_t));
            }
            return index;
        }
    }
    /* Not found timer index. */
    return -1;
}

static int _tmall_timer_insert(uint8_t timer_index,uint32_t unix_sec)
{
    int low, high, this;

    /* Delete the existing timer and resort. */
    _tmall_timer_delete(timer_index);

    /* Binary search in the sorted array: 2 logn + 2n + 1 accesses */
    low = 0;
    high = TMALL_TIMER_NUM - 1;
    while(low < high){
        this = (low + high) / 2;
        if (m_tmall_timers[this].unix_sec == unix_sec){
            return this;
        }
        else if(m_tmall_timers[this].unix_sec > unix_sec){
            high = this - 1;
        }
        else{
            low = this + 1;
        }
    }

    if(m_tmall_timers[low].unix_sec == unix_sec){
        return low;
    }
    else if(m_tmall_timers[low].unix_sec > unix_sec){
        return _tmall_timer_preserve(low);
    }
    else{
        return _tmall_timer_preserve(low + 1);
    }
}

static void _rtc_irq_lazy_handler(void)
{
    uint8_t resp[8];
    int offset = 0;

    int index = _tmall_timer_get(m_tmall_timer_index_pending);
    if (index < 0) return;

    resp[offset++] = 0x00; // TID field is set in vendor model
    co_write16p(&resp[offset], ATTR_TYPE_EVENT);
    offset += 2;
    resp[offset++] = 0x11; // timer is completed
    resp[offset++] = m_tmall_timer_index_pending;
    for(int i = 0; i < m_tmall_timers[index].op_count; i++){
        uint16_t attr_type = m_tmall_timers[index].timer_ops[i].attr_type;
        if(ATTR_TYPE_ONOFF == attr_type){
            bool onoff = m_tmall_timers[index].timer_ops[i].attr_value;
            board_set_attr(ATTR_TYPE_ONOFF,&onoff);
            break;
        }
    }

    // publish tmall indication

    if(TMALL_TIMER_TYPE_ONESHOT == m_tmall_timers[index].timer_type){
        _tmall_timer_delete(m_tmall_timer_index_pending);
    }
    _tmall_timer_emit();
}

static void rtc_alarm_callback(const struct tm * timp)
{
    if(0 != m_tmall_timer_index_pending){
        _rtc_irq_lazy_handler();
        m_tmall_timer_index_pending = 0;
    }
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

void tmall_set_timer(uint8_t * p_attr_value, uint8_t * p_len_value,uint8_t timer_type)
{
    uint8_t *p_in   = p_attr_value;
    uint8_t *p_out  = p_attr_value;
    uint32_t unix_sec;
    uint16_t time;
    uint8_t week,i,timer_index,op_count,offset = 0;
    int index;

    /* timer index */
    timer_index = *(p_in + offset);
    offset += 1;

    /* timer's time */
    if(TMALL_TIMER_TYPE_ONESHOT == timer_type){
        struct tm datetime;
        rtc_time_get(&datetime);
        uint32_t curr_sec = (uint32_t)mktime(&datetime);
        unix_sec = co_read32p(p_in + offset);
        offset += 4;
        /* Timer accuracy is minute. */
        op_count = unix_sec % 60;
        unix_sec = unix_sec / 60 * 60;
        /* UTC + 8 */
        unix_sec += 8*60*60;
        if(unix_sec < curr_sec){
            return;
        }
    }
    else {
        time = co_read16p(p_in + offset);
        offset += 2;
        week = (*p_in + offset);
        offset += 1;
        /* time unit in minute. */
        op_count = time >> 12;
        time = time & 0x0fff;
    }

    /* timer's location */
    if(TMALL_TIMER_TYPE_ONESHOT == timer_type){
        index = _tmall_timer_insert(timer_index,unix_sec);
    }
    else{
        index = _tmall_timer_insert(timer_index,time);
    }
    if(index < 0){
        return;
    }

    m_tmall_timers[index].timer_index = timer_index;
    m_tmall_timers[index].timer_type = timer_type;
    if(TMALL_TIMER_TYPE_ONESHOT == timer_type){
        m_tmall_timers[index].unix_sec = unix_sec;
    }
    else{
        m_tmall_timers[index].unix_sec = time;
        m_tmall_timers[index].schd_week = week;
    }
    if(op_count > TMALL_TIMER_OP_NUM) op_count = TMALL_TIMER_OP_NUM;
    m_tmall_timers[index].op_count = op_count;

    /* timer's attr pair {type,value} */
    for(i = 0; i < op_count; i++){
        int bytes;
        uint16_t attr_type = co_read16p(p_in + offset);
        offset += 2;
        m_tmall_timers[index].timer_ops[i].attr_type = attr_type;

        bytes = _tmall_attr_value_bytes(attr_type);
        if(1 == bytes){
            m_tmall_timers[index].timer_ops[i].attr_value = *(p_in + offset);
            offset += 1;
        }
        else if(2 == bytes){
            m_tmall_timers[index].timer_ops[i].attr_value = co_read16p(p_in + offset);
            offset += 2;
        }
        else{
            _tmall_timer_delete(timer_index);
            return;
        }
    }

    /* Prepare upstream for all oneshot/periodic timers. */
    offset = 0;
    for(index = 0; index < TMALL_TIMER_NUM; index++){
        if(timer_type == m_tmall_timers[index].timer_type) {
            /* timer index */
            *(p_out + offset) = m_tmall_timers[index].timer_index;
            offset += 1;

            /* timer's time */
            if (TMALL_TIMER_TYPE_ONESHOT == timer_type){
                co_write32p(p_out + offset, m_tmall_timers[index].unix_sec);
                offset += 4;
            }
            else{
                co_write16p(p_out + offset, m_tmall_timers[index].schd_min);
                offset += 2;
                *(p_out + offset) = m_tmall_timers[index].schd_week;
                offset += 1;
            }

            /* timer's attr pair {type,value} */
            for(int i = 0;i < m_tmall_timers[index].op_count;i++){
                uint16_t attr_type = m_tmall_timers[index].timer_ops[i].attr_type;
                co_write16p(p_out + offset, attr_type);
                offset += 2;
                if(1 == _tmall_attr_value_bytes(attr_type)){
                    *(p_out + offset) = m_tmall_timers[index].timer_ops[i].attr_value;
                    offset += 1;
                }
                else{
                    co_write16p(p_out + offset, m_tmall_timers[index].timer_ops[i].attr_value);
                    offset += 2;
                }
            }
        }
    }
    *p_len_value = offset;

    _tmall_timer_emit();
}

void tmall_del_timer(uint8_t *p_attr_value, uint8_t *p_len_value)
{
    uint8_t *p_in = p_attr_value;
    uint8_t timer_index = *(p_in + 0);
    int index;

    if(timer_index == 0xff){
        __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Tmall Delete all Timing \n");
        /* delete all timers. */
        for(index = 0; index < TMALL_TIMER_NUM; index++){
            memset(m_tmall_timers,0xff,sizeof(m_tmall_timers));
        }
        *p_len_value = 1;
    }
    else{
        /* Delete specific timer. */
        index = _tmall_timer_delete(timer_index);
        if(index < 0){
            *p_len_value = 0;
        }
        else{
            /* TBD: support multiply timer index to be deleted. */
            *p_len_value = 1;
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Tmall Delete %d Timing\n",index);
        }
    }
    _tmall_timer_emit();
}

void tmall_set_unix_time(uint8_t * p_attr_value, uint8_t * p_len_value)
{
    uint32_t unix_sec = co_read32p(p_attr_value);
    struct tm datetime;

    /* UTC + 8 */
    unix_sec += 8*60*60;
    localtime_r((const time_t *)&unix_sec,&datetime);
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Tmall Timing start: %s\n",asctime(&datetime));
    rtc_time_set(&datetime,NULL);
    *p_len_value = 4;
}

void tmall_init_timer(void)
{
    rtc_init();

    /* Invalidate all tmall timers */
    memset(m_tmall_timers,0xff,sizeof(m_tmall_timers));
    /* FIXME: use nvds */
    _tmall_timer_emit();
}

#endif /* APP_MESH_TMALL */