#ifndef E2E_HAL_E2E_PROFILE5_H
#define E2E_HAL_E2E_PROFILE5_H

#include "E2EXf.h"

//protect id
typedef enum
{
	P05_PROTECTID_0X01,
	P05_PROTECTID_0X02,
	P05_PROTECTID_NUM
}p05_protectid_en;

//check id
typedef enum
{
	P05_CHECKID_0X01,
	P05_CHECKID_0X02,
	P05_CHECKID_NUM
}p05_checkid_en;

//初始化时调用
void hal_e2e_profile5_init(void);
//返回值为0时，说明转换成功，输入的数据pdata将会被插入crc和counter
uint8_t hal_e2e_profile5(p05_protectid_en id_index,uint8_t *pdata,uint32_t len);
//返回值为0时，说明验证成功，输入参数pdata数据可用；
//第一次接收到数据的时候，返回值不为0，
uint8_t hal_e2e_profile5_inv(p05_checkid_en id_index,uint8_t *pdata,uint32_t len);
#endif
