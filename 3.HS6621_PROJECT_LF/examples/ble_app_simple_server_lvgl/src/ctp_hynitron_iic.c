/**
 *Name        : ctp_hynitron_iic.c
 *Author      : gary
 *Version     : V1.0
 *Create      : 2017-11-23
 *Copyright   : zxzz
 */

#include "ctp_hynitron_iic.h"
#include "ctp_hynitron_ext.h"

/*****************************************************************/
static uint8_t ctp_device_addr;
static uint16_t ctp_i2c_delay_us;

/*****************************************************************/
/*
 *hctp_delay_us
 */

static inline void hctp_delay_us(uint16_t us)
{
    co_delay_us(us);
}

/*
 *hctp_i2c_send_byte
 */
static bool hctp_i2c_send_byte(const uint8_t send_byte)
{
    uint8_t     i;
    uint8_t   ack;

    // send byte
    for(i=7; i>=0U; i--) {
        if (send_byte & (1<<i)) {
            CTP_SET_I2C_DATA_HIGH;
        } else {
            CTP_SET_I2C_DATA_LOW;
        }
        hctp_delay_us(ctp_i2c_delay_us);
        CTP_SET_I2C_CLK_HIGH;
        hctp_delay_us(ctp_i2c_delay_us);
        CTP_SET_I2C_CLK_LOW;
        if (i == 0) {
            break;
            //CTP_SET_I2C_DATA_HIGH;
            //CTP_SET_I2C_DATA_INPUT;
        }
        hctp_delay_us(ctp_i2c_delay_us);
    }

    CTP_SET_I2C_DATA_HIGH;
    CTP_SET_I2C_DATA_INPUT;
    CTP_SET_I2C_CLK_HIGH;
    CTP_SET_I2C_CLK_INPUT;
    hctp_delay_us(ctp_i2c_delay_us);

    for(i=0; i<100; i++) {
        if (CTP_GET_I2C_CLK_BIT == 1) {
            break;
        }
        hctp_delay_us(10);
    }
    CTP_SET_I2C_CLK_OUTPUT;

    for(i=0; i<20; i++) {
        if (CTP_GET_I2C_DATA_BIT==0) {
            ack = true;
            break;
        } else {
            ack = false;
        }
        hctp_delay_us(5);
    }
    CTP_SET_I2C_CLK_LOW;
    CTP_SET_I2C_DATA_OUTPUT;
    hctp_delay_us(ctp_i2c_delay_us);

    return ack;
}

/*
 *hctp_i2c_get_byte
 */
static uint8_t hctp_i2c_get_byte(uint8_t sendAck)
{
    uint8_t             i;
    uint8_t    get_byte=0;

    CTP_SET_I2C_DATA_INPUT;
    hctp_delay_us(ctp_i2c_delay_us);

    for (i=7;i>=0;i--) {
        CTP_SET_I2C_CLK_LOW;
        hctp_delay_us(ctp_i2c_delay_us);
        CTP_SET_I2C_CLK_HIGH;
        hctp_delay_us(ctp_i2c_delay_us);
        get_byte |= CTP_GET_I2C_DATA_BIT ? (1<<i) : 0;
        if(i==0U) {
            break;
        }
        hctp_delay_us(ctp_i2c_delay_us);
    }
    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_CLK_LOW;
    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_DATA_OUTPUT;

    if (sendAck) {
        CTP_SET_I2C_DATA_LOW;
    } else {
        CTP_SET_I2C_DATA_HIGH;
    }

    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_CLK_HIGH;

    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_CLK_LOW;

    hctp_delay_us(ctp_i2c_delay_us);
    CTP_SET_I2C_DATA_LOW;

    return get_byte;
}

/*
 *hctp_i2c_start
 */
static void hctp_i2c_start(void)
{
    CTP_SET_I2C_CLK_OUTPUT;
    CTP_SET_I2C_DATA_OUTPUT;
    CTP_SET_I2C_CLK_HIGH;
    CTP_SET_I2C_DATA_HIGH;
    hctp_delay_us(ctp_i2c_delay_us*10);
    CTP_SET_I2C_DATA_LOW;
    hctp_delay_us(ctp_i2c_delay_us*10);
    CTP_SET_I2C_CLK_LOW;
    hctp_delay_us(ctp_i2c_delay_us*10);
}

/*
 *hctp_i2c_stop
 */
static void hctp_i2c_stop(void)
{
    CTP_SET_I2C_CLK_OUTPUT;
    CTP_SET_I2C_DATA_OUTPUT;
    CTP_SET_I2C_CLK_LOW;
    CTP_SET_I2C_DATA_LOW;

    hctp_delay_us(ctp_i2c_delay_us*10);
    CTP_SET_I2C_CLK_HIGH;
    hctp_delay_us(ctp_i2c_delay_us*10);
    CTP_SET_I2C_DATA_HIGH;
    hctp_delay_us(ctp_i2c_delay_us*10);
}

/*
 *hctp_i2c_init
 */
void hctp_i2c_init(uint8_t slave_addr,uint16_t iic_speed_k)
{
    CTP_SET_I2C_DATA_OUTPUT;//GPIO_OUTPUT
    CTP_SET_I2C_CLK_OUTPUT;//GPIO_OUTPUT

    CTP_SET_I2C_DATA_OUTPUT;
    CTP_SET_I2C_DATA_HIGH;
    CTP_SET_I2C_CLK_OUTPUT;
    CTP_SET_I2C_CLK_HIGH;

    CTP_SET_RESET_PIN_OUTPUT;
    CTP_SET_RESET_PIN_LOW;
    co_delay_ms(5);
    CTP_SET_RESET_PIN_HIGH;
    co_delay_ms(30);
    ctp_device_addr = slave_addr;

    ctp_i2c_delay_us = 1000/(iic_speed_k+1)/3;
}

/*
 *hctp_i2c_power_on
 */
void hctp_i2c_power_on(void)
{
    /*
    pmu_set_vcam_d_sel(VCAM_D_SEL_2_8);     //<==neen set VDD = 2.8V
    pmu_set_vcam_d_en(CTP_TRUE);
    kal_sleep_task(1);
    */
}

/*
 *hctp_write_bytes
 */
bool hctp_write_bytes(uint16_t reg, const uint8_t *data, uint16_t len, uint8_t regLen)
{
    hctp_i2c_start();
    if(!hctp_i2c_send_byte((ctp_device_addr)))
    {
        hctp_i2c_stop();
        return false;
    }
    if (regLen == 2)
    {
        hctp_delay_us(ctp_i2c_delay_us);
        if(!hctp_i2c_send_byte(reg>>8)) {
            hctp_i2c_stop();
            return false;
        }
    }
    hctp_delay_us(ctp_i2c_delay_us);
    if(!hctp_i2c_send_byte(reg)) {
        hctp_i2c_stop();
        return false;
    }
    while(len--) {
        hctp_delay_us(ctp_i2c_delay_us);
        if(!hctp_i2c_send_byte(*data++)) {
            hctp_i2c_stop();
            return false;
        }
    }
    hctp_delay_us(ctp_i2c_delay_us);
    hctp_i2c_stop();
    return true;
}

/*
 *hctp_read_bytes
 */
bool hctp_read_bytes(uint16_t reg, uint8_t *value, uint16_t len,uint8_t regLen)
{
    hctp_i2c_start();
    if(!hctp_i2c_send_byte((ctp_device_addr))) {
        hctp_i2c_stop();
        return false;
    }
    if (regLen == 2) {
        hctp_delay_us(ctp_i2c_delay_us);
        if(!hctp_i2c_send_byte(reg>>8)) {
            hctp_i2c_stop();
            return false;
        }
    }
    hctp_delay_us(ctp_i2c_delay_us);
    if(!hctp_i2c_send_byte(reg)){
        hctp_i2c_stop();
        return false;
    }
    hctp_delay_us(ctp_i2c_delay_us);
    hctp_i2c_stop();

    hctp_delay_us(ctp_i2c_delay_us);

    hctp_i2c_start();
    if(!hctp_i2c_send_byte((ctp_device_addr)+1)) {
        hctp_i2c_stop();
        return false;
    }
    while(len--) {
        *value++ = hctp_i2c_get_byte(len);
    }
    hctp_i2c_stop();

    return true;
}



