#ifndef E2E_HAL_E2E_PROFILE1_H
#define E2E_HAL_E2E_PROFILE1_H

#include "E2EXf.h"

//protect id
typedef enum
{
	P01_PROTECTID_0X01,
	P01_PROTECTID_0X02,
	P01_PROTECTID_NUM
}p01_protectid_en;

//check id
typedef enum
{
	P01_CHECKID_0X01,
	P01_CHECKID_0X02,
	P01_CHECKID_NUM
}p01_checkid_en;

//初始化时调用
void hal_e2e_profile1_init(void);

//返回值为0时，说明转换成功，输入的数据pdata将会被插入crc和counter
uint8_t hal_e2e_profile1(p01_protectid_en id_index,uint8_t *pdata,uint32_t len);

//返回值为0时，说明验证成功，输入参数pdata数据可用；
//第一次接收到数据的时候，返回值不为0，
uint8_t hal_e2e_profile1_inv(p01_checkid_en id_index,uint8_t *pdata,uint32_t len);

#endif
