//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_pin_cfg.h
 * 摘要：
 *          文件主要描述timer模块对外接口
 * 当前版本：
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
 * m_irq_mode_e     配置中断类型
 * m_irq_enabled_e  配置是否使能中断
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

#define HAL_PINB_3_AF_SPI1SOUT		kPORT_MuxAlt2
//#define HAL_PINB_3_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PINB_4_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PINB_5_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PINA_4_AF_ADC0A10			kPORT_MuxAsGpio
//#define HAL_PINA_4_AF_GPIO				kPORT_MuxAsGpio

//#define HAL_PINA_16_AF_SPI0PCS0		kPORT_MuxAlt2
#define HAL_PINA_16_AF_GPIO				kPORT_MuxAsGpio

//#define HAL_PINA_17_AF_SPI0SIN		kPORT_MuxAlt2
#define HAL_PINA_17_AF_GPIO				kPORT_MuxAsGpio

//#define HAL_PINA_18_AF_SPI0SOUT	kPORT_MuxAlt2
#define HAL_PINA_18_AF_GPIO				kPORT_MuxAsGpio

//#define HAL_PINA_19_AF_SPI0SCK		kPORT_MuxAlt2
#define HAL_PINA_19_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PINA_20_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PINA_21_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PIND_1_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PIND_2_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PIND_3_AF_ADC0B6			kPORT_PinDisabledOrAnalog

#define HAL_PINC_0_AF_CAN0TX			kPORT_MuxAlt3

#define HAL_PINC_1_AF_CAN0RX			kPORT_MuxAlt3

#define HAL_PINC_2_AF_UART1RX		kPORT_MuxAlt3
//#define HAL_PINC_2_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PINC_3_AF_UART1TX		kPORT_MuxAlt3
#define HAL_PINC_4_AF_GPIO				kPORT_MuxAsGpio
#define HAL_PINC_5_AF_GPIO			kPORT_MuxAsGpio

#define HAL_PINC_6_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PINC_7_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PINB_0_AF_SPI1PCS0		kPORT_MuxAlt2
#define HAL_PINB_0_AF_GPIO				kPORT_MuxAsGpio

#define HAL_PINB_1_AF_SPI1SIN			kPORT_MuxAlt2


#define HAL_PINB_2_AF_SPI1SCK			kPORT_MuxAlt2

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
 * 函 数 介 绍 ：
 *                 获得pin配置表
 *
 *  * 参       数 pincfg            获得配置表指针
 *                 len              获得配置表长度
 * 返   回  值
 * 备       注 ：
 *
 */
void hal_pin_get_cfg(hal_pin_cfg_st **pincfg, uint32_t *len);

/* Function End*/
#endif
