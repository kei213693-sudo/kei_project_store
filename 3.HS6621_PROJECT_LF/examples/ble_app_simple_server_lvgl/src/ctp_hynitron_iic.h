/**
 *Name        : ctp_hynitron_iic.h
 *Author      : gary
 *Version     : V1.0
 *Create      : 2017-11-23
 *Copyright   : zxzz
 */


#ifndef __CTP_HYNITRON_IIC_H
#define __CTP_HYNITRON_IIC_H

#include <stdbool.h>
#include <stdint.h>

extern void hctp_i2c_init(uint8_t slave_addr, uint16_t iic_speed_k);
extern void hctp_i2c_power_on(void);
extern bool hctp_write_bytes(uint16_t reg, const uint8_t *data, uint16_t len,uint8_t regLen);
extern bool hctp_read_bytes(uint16_t reg,  uint8_t *value, uint16_t len,uint8_t regLen);


#endif /*CTP_HYNITRON_IIC_H__*/


/* End Of File -------------------------------------------------------------- */

