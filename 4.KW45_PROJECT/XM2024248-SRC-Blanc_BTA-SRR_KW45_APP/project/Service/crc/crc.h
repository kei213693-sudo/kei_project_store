#ifndef _CRC_H_
#define _CRC_H_

#include "stdint.h"

uint32_t crc32_check (const unsigned char *buf, int len, unsigned int init);

uint16_t my_crc16_with_init_value(uint8_t *data, uint32_t size,uint16_t initial_crc_value);
uint16_t my_crc16(uint8_t *data, uint32_t size);

uint16_t my_crc8_with_init_value(uint8_t *data,uint32_t size,uint16_t initial_crc_value);
uint16_t my_crc8(uint8_t *data, uint32_t size);

uint8_t TmpsCalculateCRC8(uint8_t *ptr, uint8_t len, uint8_t poly, uint8_t init);

#endif
