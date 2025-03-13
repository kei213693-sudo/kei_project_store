//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_pin_cfg.h
 * 摘要：
 *          文件主要描述timer模块对外接口
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */
#ifndef HAL_PIN_CFG_H
#define HAL_PIN_CFG_H

/* Include Begine*/
#include "hal_pin_typedef.h"
#include "hal_typedef.h"
/* Include End*/

/* Typedef Begine*/

/**
 * pin脚配置属性
 * m_u32_port       配置端口号，可以为单片机原有定义直接映射
 * m_u32_pin        配置PIN脚号，可以为单片机原有定义直接映射
 * m_direction_et   配置输入输出，详情参考hal_pin_direction_et
 * m_mode_et        配置默认PIN脚模式，如上拉等 详情参考hal_pin_mode_et
 * m_u32_alternativenum 配置中复用号,可以通过#define HAL_PIN_GET_MULTIPLE(PINX_X,FUNCITON)
 *                  获取复用定义中的值，也可以直接调用复用定义中的宏定义。如果未使用复用则直接
 *                  调用HAL_PIN_AF_NULL
 * m_initstatus_et  配置初始化状态，是否被初始化
 * hal_pin_init()   初始化调用时，按照配置表逐行初始化,使用#define HAL_PIN_MAXCOUNT 定义需要初始化的数量
 */
typedef struct
{
    uint32_t m_u32_port;
    uint32_t m_u32_pin;
    hal_pin_direction_et m_direction_et;
    hal_pin_mode_et m_mode_et;
    uint32_t m_u32_alternativenum;
	hal_pin_irq_mode_et  m_irq_mode_e;
	hal_enabled_et	m_irq_enabled_e;
} hal_pin_cfg_st;

/* Typedef End*/


/* Mirco Begine*/

/**
 * 复用定义
 * 格式为PINXX_XX__MULTIPLEX_XXX 举例 HAL_PIN0_1_AF_CAN0RX        2
 * 获取索引方式通过##连接符实现
 * #define HAL_PIN_GET_MULTIPLE(PINX_X,FUNCITON)        (PINX_X##_MULPTIPLE_FUNCITON)
 * 2 == HAL_PIN_GET_MULTIPLE(PIN0_1,CAN0RX)
 * 同一芯片使用时应在此基础上不断新增，逐步完善所有PIN脚复用关系
 */
#define HAL_PIN_GET_MULTIPLE(PINX_X, FUNCITON) (HAL_##PINX_X##_AF_##FUNCITON)

/**
 * 不使用复用功能
 */
#define HAL_PIN_AF_NULL 0

/**	
 * CAN 列举
 * CAN FUNCTION ：
 *      CANXRX
 *      CANXTX
 * #define HAL_PIN0_1_AF_CAN0RX       2
 * 按芯片实际需求增加定义，使用时通过HAL_PIN_GET_MULTIPLE获取对应的复用号
 */
//#define HAL_PIN0_1_AF_CAN0RX (2)

/**
 * LIN 列举
 * LIN FUNCTION ：
 *      LINXRX
 *      LINXTX
 * #define PIN0_1_MULTIPLE_LIN0RX       2
 * 按芯片实际需求增加定义，使用时通过HAL_PIN_GET_MULTIPLE获取对应的复用号
 */

/**
 * SPI 列举
 * SPI FUNCTION ：
 *      SPIXDI
 *      SPIXDO
 *      SPIXCLK
 *      SPIXCS
 * #define PIN0_1_MULTIPLE_SPIXDO       2
 * 按芯片实际需求增加定义，使用时通过HAL_PIN_GET_MULTIPLE获取对应的复用号
 */

/**
 * UART 列举
 * UART FUNCTION ：
 *      UARTXTX
 *      UARTXRX
 * #define PIN0_1_MULTIPLE_UARTXTX       2
 * 按芯片实际需求增加定义，使用时通过HAL_PIN_GET_MULTIPLE获取对应的复用号
 */

#define HAL_PIN_MAXCOUNT (HAL_PIN_INDEX_MAX)

/* Mirco End*/
/* Pramameter Begine*/

/* Pramameter End*/
/* Function Begine*/

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
void hal_pin_get_cfg(hal_pin_cfg_st **pincfg, uint32_t *len);

/* Function End*/
#endif
