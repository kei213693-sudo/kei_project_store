

#include "ctp_hynitron_iic.h"
#include "ctp_hynitron_ext.h"
#include "cst816s_fw.h"

#define PER_LEN       512
#define REG_LEN_1B      1
#define REG_LEN_2B      2

#define CTP_HYNITRON_EXT        1 // enable update
#define CTP_USE_SW_I2C          1
#define SUPPORT_TP_UPDATA       0

int TP_int;

void tp_int_cb(unsigned int pin_mask)
{
		TP_int = 1;
}


#if (SUPPORT_TP_UPDATA)
static int cst816s_enter_bootmode(void)
{
    uint32_t retry_cnt = 10U;

    CTP_SET_I2C_DATA_OUTPUT;
    CTP_SET_I2C_DATA_HIGH;
    CTP_SET_I2C_CLK_OUTPUT;
    CTP_SET_I2C_CLK_HIGH;
    CTP_SET_RESET_PIN_LOW;
    co_delay_ms(15);
    CTP_SET_RESET_PIN_HIGH;
    co_delay_ms(5);

    while(retry_cnt--) {
        uint8_t cmd[3];
        cmd[0] = 0xAB;//0xAA:cts716;0xAB:cts816,cts716se-b
        if(false == hctp_write_bytes(0xA001, cmd, 1, REG_LEN_2B)) {  // enter program mode
             co_delay_ms(2); // 4ms
             continue;
        }
        if(false == hctp_read_bytes(0xA003, cmd, 1, REG_LEN_2B)) { // read flag
             co_delay_ms(2); // 4ms
             continue;
        } else {
            if(cmd[0] != 0x55) {
                co_delay_ms(2); // 4ms
                continue;
            } else {
                log_debug("enter tp bootmode\r\n");
              return 0;
           }
        }
    }
    return -1;
}

static int cst816s_update(uint16_t start_addr, uint16_t len, const uint8_t *fw_data)
{
    uint16_t            count;
    uint16_t       update_len;
    uint8_t           cmd[10];
    uint8_t             a[10];

    co_assert(len);
    co_assert(fw_data);

    if (cst816s_enter_bootmode() == -1) {
        return -1;
    }
    count =0;

    do {
        cmd[0] = start_addr & 0xFF;
        cmd[1] = start_addr >> 8U;
        hctp_write_bytes(0xA014, cmd, 2, REG_LEN_2B);
        hctp_read_bytes(0xA018, a, 2, REG_LEN_2B);
        if(len >= PER_LEN) {
            update_len = PER_LEN;
        } else {
            update_len = len;
        }
        len -= update_len;
        hctp_write_bytes(0xA018, &(fw_data[count]), update_len, REG_LEN_2B);

        cmd[0] = 0xEE;
        hctp_write_bytes(0xA004, cmd, 1, REG_LEN_2B);
        co_delay_ms(100);
        {
            uint8_t retry_cnt = 50;
            while(retry_cnt--) {
                cmd[0] = 0;
                hctp_read_bytes(0xA005, cmd, 1, REG_LEN_2B);
                if(cmd[0] == 0x55){
                    break;
                }
                co_delay_ms(20);
            }
        }
        start_addr += PER_LEN;
        count      += update_len;
        log_debug("cst816s update len = 0x%x\r\n", len);
    } while(len);

    cmd[0] = 0x00;
    hctp_write_bytes(0xA003, cmd, 1, REG_LEN_2B);

    return 0;
}

static int cst816s_read_checksum(uint16_t startAddr,uint16_t len)
{
    union {
       uint16_t sum;
       uint8_t buf[2];
    } checksum;
    char cmd[3];

    log_debug("checksum 0\r\n");

    if(cst816s_enter_bootmode() == -1) {
        log_debug("checksum 1\r\n");
        return -1;
    }
    cmd[0] = 0;
    if (false == hctp_write_bytes(0xA003,(uint8_t *)cmd,1,REG_LEN_2B)) {
        log_debug("checksum 2\r\n");
        return -1;
    }
    co_delay_ms(1000);
    {
        int retry = 100;
        while(--retry) {
            char temp = 9;
            if (false == hctp_read_bytes(0xA000, (uint8_t *)&temp, 1, REG_LEN_2B)) {
                log_debug("checksum 3\r\n");
                return -1;
            }
            if (temp == 0) {
                co_delay_ms(100);
            } else {
                break;
            }
        }
    }
    checksum.sum = 0;
    if (false == hctp_read_bytes(0xA008, checksum.buf, 2, REG_LEN_2B)) {
        log_debug("checksum 4\r\n");
        return -1;
    }

    log_debug("checksum = 0x%x\r\n", checksum.sum);
    return checksum.sum;
}

bool ctp_hynitron_update(void)
{
    const uint8_t     *ctp_fw;

    ctp_fw = cst816s_fw;
    hctp_i2c_init(0x6A<<1,50);
    if(cst816s_enter_bootmode() == 0) {
        uint16_t      start_addr;
        uint16_t          length;
        uint16_t       check_sum;

        start_addr = (ctp_fw[1] << 8) + ctp_fw[0];
        length     = (ctp_fw[3] << 8) + ctp_fw[2];
        check_sum  = (ctp_fw[5] << 8) + ctp_fw[4];

        if(cst816s_read_checksum(start_addr, length) != check_sum) {
            log_debug("cst816s update start>>>\r\n");
            cst816s_update(start_addr, length, ctp_fw);
        }
        log_debug("check sum = 0x%x\r\n", check_sum);

        CTP_SET_RESET_PIN_LOW;
        co_delay_ms(15);
        CTP_SET_RESET_PIN_HIGH;
        co_delay_ms(30);

        return true;
    }

    return false;
}
#endif  /* SUPPORT_TP_UPDATA */

void ctp_hynitron_init(void)
{
    uint8_t chip_id =0;

    //TP
    pinmux_config(PIN_TP_RST,      PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(BITMASK(PIN_TP_RST), PMU_PIN_MODE_PP);
    pinmux_config(PIN_TP_SCL,      PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set_ex(BITMASK(PIN_TP_SCL - 32), PMU_PIN_MODE_PU);
    pinmux_config(PIN_TP_SDA,      PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set_ex(BITMASK(PIN_TP_SDA - 32), PMU_PIN_MODE_PU);
    pinmux_config(PIN_TP_INT,      PINMUX_GPIO_MODE_CFG);
    pmu_pin_mode_set(BITMASK(PIN_TP_INT), PMU_PIN_MODE_PU);
	
	//ADD CALLBACK
		gpio_set_interrupt_callback(tp_int_cb);  		//??????
		gpio_set_interrupt(BITMASK(PIN_TP_INT), GPIO_FALLING_EDGE); //??????????

     //detect update
#if SUPPORT_TP_UPDATA
    ctp_hynitron_update();
#endif
    hctp_i2c_init(0x2D<<1,100);
     //check tp is here 
    hctp_read_bytes(0xA7, &chip_id, 1, REG_LEN_1B);
    log_debug("ctp_hynitron_init: chip ID = 0x%x\r\n",chip_id);

#if 0 //Yao
    //pmu_wakeup_pin_set_ex(Bitmask, PMU_PIN_WAKEUP_LOW_LEVEL);
#endif
    //gpio_register_event_cb(tp_event_handler);
}

bool ctp_hynitron_is_pressed(void)
{
    if(TP_int) {
			  TP_int = 0;
        // log_debug("ctp is pressed\r\n");
        return true;
    } else {
        return false;
    }
}

void ctp_hynitron_read_pos(int16_t *x, int16_t *y)
{
    uint8_t pos[8]={0};

    hctp_read_bytes(0x01, pos, 6, REG_LEN_1B);

    *x = (pos[2] & 0x3f) << 8 | pos[3];
    *y = (pos[4] & 0x0f) << 8 | pos[5];

    // log_debug("ctp_hynitron postion x=%d,y=%d\r\n", *x, *y);
}

#if 0
/*************************************************************************
**************************creat poll timer********************************
**************************creat poll timer********************************
**************************creat poll timer********************************
**************************************************************************/
//static co_timer_t touch_timer;
static osTimerId_t touch_timer_id;

static bool touch_poll_start=false;
static int touch_cnt=0;
static int long_touch_cnt=0;
static tevent_t press_event;
static tevent_t touch_event;
static bool long_touch=false;
//-----------------------------------------------------------------------------
// Global Variable Declaration
//-----------------------------------------------------------------------------
static void touch_timer_stop(void)
{
    //co_timer_del(&touch_timer);
    osTimerStop(touch_timer_id);
}

static void touch_timer_handler(co_timer_t *timer, void *param)
{
    int x,y=0;
    uint8_t pos[4]={0};

    hctp_read_bytes(0x03,pos,4,REG_LEN_1B);
    x=(pos[0]&0x0f) << 8 | pos[1];
    y=(pos[2]&0x0f) << 8 | pos[3];
    // log_debug("ctp_hynitron postion x=%x,y=%x!!!\r\n",x,y);
    if(touch_cnt) {
        touch_cnt=0;
        touch_event.x=x;
        touch_event.y=y;
        touch_event.pressed=true;
    } else {
        x = touch_event.x - press_event.x;
        y = touch_event.y - press_event.y;
        touch_event.policy = TE_POLICY_UNKNOW;
        if(ABS(x)<= TOUCH_SLIDE_OFFSET && ABS(y)<= TOUCH_SLIDE_OFFSET && long_touch==false) {
            touch_event.policy |= TE_POLICY_PRESS;
        } else if(ABS(x)<=ABS(y)) {
            if (y > 0 && ABS(y) >= 50){
                    touch_event.policy |= TE_POLICY_DOWN_SLIDE;
            } else if(ABS(y) >= 50){
                    touch_event.policy |= TE_POLICY_UP_SLIDE;
            }else{
                  touch_event.policy = TE_POLICY_UP;
            }
        } else if(ABS(x) >= 50) {
            if (x > 0) {
                    touch_event.policy |= TE_POLICY_RIGHT_SLIDE;
            } else {
                    touch_event.policy |= TE_POLICY_LEFT_SLIDE;
            }
        } else {
            touch_event.policy = TE_POLICY_UP;
        }
        press_event.pressed = false;
        touch_event.pressed=false;
        touch_poll_start=false;
        long_touch_cnt=0;
        long_touch=false;
        UI_HandleKeyEvent(&touch_event);
        touch_timer_stop();
        log_debug("ctp_hynitron touch end!!!\r\n");

        return;
    }
    if(touch_event.pressed && !press_event.pressed) {
        press_event.pressed=true;
        press_event.x=touch_event.x;
        press_event.y=touch_event.y;
        touch_event.policy = TE_POLICY_DOWN;
        UI_HandleKeyEvent(&touch_event);//tp is pressed
        log_debug("ctp_hynitron touch press!!!\r\n");
        return;
    }
    if(press_event.pressed) {
        x = touch_event.x - press_event.x;
        y = touch_event.y - press_event.y;
        if (ABS(x) <= TOUCH_LONG_OFFSET && ABS(y) <= TOUCH_LONG_OFFSET) {
            long_touch_cnt++;
            if((long_touch_cnt >30) && (long_touch==false)) {
                long_touch=true;
                touch_event.policy = TE_POLICY_LONG_TOUCH_PRESS;
                UI_HandleKeyEvent(&touch_event);
                log_debug("ctp_hynitron touch long press>>>>>\r\n");
                return;
            }
        } else {
            long_touch_cnt=0;
        }
        touch_event.policy = TE_POLICY_MOVE;
        UI_HandleKeyEvent(&touch_event);
        return;
    }
}

static void touch_timer_start(void)
{
    //co_timer_set(&touch_timer, 50, TIMER_REPEAT, touch_timer_handler, NULL);
    osTimerStart(touch_timer_id, 50U);
}

void tp_event_handler(uint32_t pin_mask)
{
    touch_cnt++;
    if(touch_poll_start == false) {
        touch_poll_start = true;
        touch_timer_start();
    } else {
        if(touch_cnt > 12) {
            touch_cnt = 0;
            touch_poll_start=false;
            log_debug("ctp_hynitron touch state error!!!\r\n"); 
        }
    }
}
#endif

void cst816s_enter_sleepmode(void)
{
    uint8_t cmd=0x03;
    hctp_write_bytes(0xA5, &cmd, 1, REG_LEN_1B);
#if 0    //check state
     uint8_t chip_id=0;
     co_delay_ms(150);
     hctp_read_bytes(0xA7, &chip_id, 1, REG_LEN_1B);
    if(chip_id!=0xB3) {
        log_debug("ctp_hynitron sleep success!!!\r\n");
    }
#endif
}

void cst816s_wakeup(void)
{
    //touch_poll_start=false;
    //touch_cnt=0;
    CTP_SET_RESET_PIN_LOW;
    co_delay_ms(15);
    CTP_SET_RESET_PIN_HIGH;
    co_delay_ms(30);
}


/* End Of File -------------------------------------------------------------- */

