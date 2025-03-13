//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_pin_cfg.c
 * 摘要：
 *          文件主要描述clock模块
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

/* Include Begine*/
#include "hal_pin_cfg.h"
/* Include End*/

/* Parameter Begine*/

/**
 * pin脚配置属性
 * m_u32_port       配置端口号，可以为单片机原有定义直接映射
 * m_u32_pin        配置PIN脚号，可以为单片机原有定义直接映射
 * m_direction_et   配置输入输出，详情参考hal_pin_direction_et
 * m_mode_et        配置默认PIN脚模式，如上拉等 详情参考hal_pin_mode_et
 * m_u32_alternativenum 配置中复用号,可以通过#define HAL_PIN_GET_MULTIPLE(PINX_X,FUNCITON)
 *                  获取复用定义中的值，也可以直接调用复用定义中的宏定义。如果未使用复用则直接
 *                  调用HAL_PIN_AF_NULL
 * hal_pin_init()   初始化调用时，按照配置表逐行初始化,使用#define HAL_PIN_MAXCOUNT 定义需要初始化的数量
 */
static hal_pin_cfg_st sc_pin_init_cfg_sa[HAL_PIN_INDEX_MAX] = {
		//PIN测试
//	{HAL_PORTD,1,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_IN_UP,kPORT_MuxAsGpio,0,HAL_ENABLED_DISABLE},
//	{HAL_PORTC,6,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_IN_UP,kPORT_MuxAsGpio,0,HAL_ENABLED_DISABLE},
//	{HAL_PORTA,16,HAL_PIN_DIRECTION_OUT,HAL_PIN_MODE_OUT_PUSH_PULL,kPORT_MuxAsGpio,0,HAL_ENABLED_DISABLE},
//	{HAL_PORTD,1,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_IN_FLOAT,kPORT_MuxAsGpio,HAL_PIN_IRQ_MODE_FALLEDGE,HAL_ENABLED_DISABLE}
		//UART测试
//	{HAL_PORTA,16,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_AF,kPORT_MuxAlt6,0,HAL_ENABLED_DISABLE},
//	{HAL_PORTA,17,HAL_PIN_DIRECTION_OUT,HAL_PIN_MODE_AF,kPORT_MuxAlt6,0,HAL_ENABLED_DISABLE},
//	{HAL_PORTC,2,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_AF,kPORT_MuxAlt3,0,HAL_ENABLED_DISABLE},
//	{HAL_PORTC,3,HAL_PIN_DIRECTION_OUT,HAL_PIN_MODE_AF,kPORT_MuxAlt3,0,HAL_ENABLED_DISABLE}
		//SPI0测试
//	{HAL_PORTA,16,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_AF,kPORT_MuxAlt2,0,HAL_ENABLED_DISABLE},	//SPI0_PCS0
//	{HAL_PORTA,17,HAL_PIN_DIRECTION_OUT,HAL_PIN_MODE_AF,kPORT_MuxAlt2,0,HAL_ENABLED_DISABLE},	//SPI0_SIN
//	{HAL_PORTA,18,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_AF,kPORT_MuxAlt2,0,HAL_ENABLED_DISABLE},	//SPI0_SOUT
//	{HAL_PORTA,19,HAL_PIN_DIRECTION_OUT,HAL_PIN_MODE_AF,kPORT_MuxAlt2,0,HAL_ENABLED_DISABLE}	//SPI0_SCK
		//CAN测试
	{HAL_PORTC,4,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_AF,kPORT_MuxAlt3,0,HAL_ENABLED_DISABLE},
	{HAL_PORTC,5,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_AF,kPORT_MuxAlt3,0,HAL_ENABLED_DISABLE},
	{HAL_PORTD,1,HAL_PIN_DIRECTION_IN,HAL_PIN_MODE_IN_UP,kPORT_MuxAsGpio,0,HAL_ENABLED_DISABLE},
	{HAL_PORTB,0,HAL_PIN_DIRECTION_OUT,HAL_PIN_MODE_OUT_PUSH_PULL,kPORT_MuxAsGpio,0,HAL_ENABLED_DISABLE}
};

/* Parameter End*/

/*  Function Begine  */

/**
* @brief  ：
 *                 获取配置表信息
 *                 PIN初始化时按照返回的表和长度逐一初始化
  * @param ：
 *                  [out] pincfg 配置表首地址指针
 *                  [out] len    返回表格长度
 * @return ：
 * @note ：
 *
 */
void hal_pin_get_cfg(hal_pin_cfg_st **pincfg, uint32_t *len)
{
      *len = HAL_PIN_MAXCOUNT;
      *pincfg = sc_pin_init_cfg_sa;
}
/*  Function End  */
