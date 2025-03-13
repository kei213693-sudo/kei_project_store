//copy by cc
/* 
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_pin_typedef.h
 * 摘要: 
 * 当前版本: V1.0.0 Chenjianbo 
 * Date: 2023-11-23
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#ifndef HAL_PIN_TYPEDEF_H
#define HAL_PIN_TYPEDEF_H
#include "hal_typedef.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

typedef uint8_t hal_pin_multiplex_t;
typedef void (*hal_pin_cbk_t)(void);


/**
 * 初始化状态
 */
typedef enum
{
	HAL_PIN_UNINIT = 0,
	HAL_PIN_INIT
}hal_pin_initstatus_et;

/**
 * pin等级
 */
typedef enum
{
    HAL_PIN_LEVEL_LOW = 0,
    HAL_PIN_LEVEL_HIGH
} hal_pin_level_et;

/**
 * 中断模式
 */
typedef enum
{
    HAL_PIN_IRQ_MODE_FALLEDGE = 0, 
    HAL_PIN_IRQ_MODE_RISEEDGE,
    HAL_PIN_IRQ_MODE_RISE_FALL,
    HAL_PIN_IRQ_MODE_HIGH,
    HAL_PIN_IRQ_MODE_LOW
} hal_pin_irq_mode_et;

/**
 * pin 模式
 */
typedef enum
{
    HAL_PIN_MODE_IN_ANALOG = 0,
    HAL_PIN_MODE_IN_FLOAT,
    HAL_PIN_MODE_IN_UP,
    HAL_PIN_MODE_IN_DOWN,
    HAL_PIN_MODE_OUT_PUSH_PULL,
    HAL_PIN_MODE_OUT_OPEN_DRAIN,
	HAL_PIN_MODE_AF
} hal_pin_mode_et;
/**
 * Pin脚输入输出方向配置
 * HAL_PIN_DIRECTION_IN             指定pin脚为输入脚
 * HAL_PIN_DIRECTION_OUT            指定pin脚为输出脚
*/
typedef enum
{
    HAL_PIN_DIRECTION_IN = 0,
    HAL_PIN_DIRECTION_OUT
} hal_pin_direction_et;

typedef enum
{
	HAL_PORTA,
	HAL_PORTB,
	HAL_PORTC,
	HAL_PORTD
}hal_pin_port_et;
/**
 * pin 索引，对应数组为hal_pin_cfg.c:sc_pin_init_cfg_sa[HAL_PIN_INDEX_MAX]
 * 注意：HAL_PIN_INDEX_MAX 无实质映射，用来表示PIN脚数量，只能添加到枚举最后
 *      初值为0，其余不需要定义值
 */
typedef enum
{
    HAL_PIN_INDEX0 = 0,
	HAL_PIN_INDEX1,
	HAL_PIN_INDEX2,
	HAL_PIN_INDEX3,
    HAL_PIN_INDEX_MAX
} hal_pin_index_et;

#endif
