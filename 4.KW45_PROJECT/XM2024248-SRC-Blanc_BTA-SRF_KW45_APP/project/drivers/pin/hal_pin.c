//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_pin.c
 * 摘要：
 *          文件主要描述pin模块
 * 当前版本：
 * 历史版本：
 */

/* Include Begine*/
#include "hal_pin.h"
#include "hal_pin_cfg.h"

/* Include End*/

/* Function Begine*/

#define PORTA_PINNUM	22
#define PORTB_PINNUM	6
#define PORTC_PINNUM	8
#define	PORTD_PINNUM	6

static  GPIO_Type * const gpio_array[4] = {GPIOA,GPIOB,GPIOC,GPIOD};
static  PORT_Type * const port_array[4] = {PORTA,PORTB,PORTC,PORTD};

static hal_pin_cbk_t porta_cbk_array[PORTA_PINNUM] = {0};
static hal_pin_cbk_t portb_cbk_array[PORTB_PINNUM] = {0};
static hal_pin_cbk_t portc_cbk_array[PORTC_PINNUM] = {0};
static hal_pin_cbk_t portd_cbk_array[PORTD_PINNUM] = {0};

static void hal_pin_irqhandler(GPIO_Type * gpio,hal_pin_cbk_t *cbk_array,uint8_t pinnum)
{
	uint32_t flag = 0;
	uint8_t i = 0;
	flag = GPIO_GpioGetInterruptChannelFlags(gpio,0);
	for(i = 0; i < pinnum; i++)
	{
		if((flag&0x01) == 0x01)
		{
			if(NULL != cbk_array[i])
			{
				cbk_array[i]();
			}
			else
			{

			}
			GPIO_GpioClearInterruptFlags(gpio, 1U<<i);
		}
		else
		{

		}
		flag=flag>>1U;
	}
}

void GPIOA_INT0_IRQHandler(void)
{
	hal_pin_irqhandler(GPIOA,porta_cbk_array,PORTA_PINNUM);
}

void GPIOA_INT1_IRQHandler(void)
{

}

void GPIOB_INT0_IRQHandler(void)
{
	hal_pin_irqhandler(GPIOB,portb_cbk_array,PORTB_PINNUM);
}

void GPIOB_INT1_IRQHandler(void)
{

}

void GPIOC_INT0_IRQHandler(void)
{
	hal_pin_irqhandler(GPIOC,portc_cbk_array,PORTC_PINNUM);
}

void GPIOC_INT1_IRQHandler(void)
{

}

void GPIOD_INT0_IRQHandler(void)
{

	hal_pin_irqhandler(GPIOD,portd_cbk_array,PORTD_PINNUM);
}

void GPIOD_INT1_IRQHandler(void)
{

}
/**
 * 函 数 介 绍 ：
 *                 pin初始化
 *
 *  * 参       数
 *
 * 返   回  值
 * 备       注 ：   根据pin_cfg配置表初始化引脚
 *
 */
hal_result_t hal_pin_init()
{
    // to be add user code
	hal_result_t result = HAL_RESULT_SUCCESS;
	hal_result_t res = HAL_RESULT_SUCCESS;
	uint32_t len = 0;
	uint32_t i = 0;
	hal_pin_cfg_st * psc_pin_init_cfg_sa = NULL;
	hal_pin_get_cfg(&psc_pin_init_cfg_sa, &len);
	CLOCK_EnableClock(kCLOCK_GpioA);
	CLOCK_EnableClock(kCLOCK_GpioB);
	CLOCK_EnableClock(kCLOCK_GpioC);
	if((0 < len)&&(NULL != psc_pin_init_cfg_sa))
	{
		for(i = 0; i < len; i++)
		{
			res = hal_pin_init_index(i);
			if(res != HAL_RESULT_SUCCESS)
			{
				result = res;
				break;
			}
			else
			{

			}
		}
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
	EnableIRQ(GPIOA_INT0_IRQn);
	EnableIRQ(GPIOB_INT0_IRQn);
	EnableIRQ(GPIOC_INT0_IRQn);
	EnableIRQ(GPIOD_INT0_IRQn);
	return result;
}


static void hal_pin_mode_in_analog(gpio_pin_config_t *pin_config,port_pin_config_t *port_config)
{
	pin_config->pinDirection = 0;
	pin_config->outputLogic = 0;

	port_config->pullSelect = kPORT_PullDisable;
	port_config->pullValueSelect = kPORT_LowPullResistor;
	port_config->slewRate = kPORT_FastSlewRate;
	port_config->passiveFilterEnable = kPORT_PassiveFilterDisable;
	port_config->openDrainEnable = kPORT_OpenDrainDisable;
	port_config->driveStrength = kPORT_LowDriveStrength;
	port_config->driveStrength1 = kPORT_NormalDriveStrength;
	port_config->mux = kPORT_PinDisabledOrAnalog;
	port_config->lockRegister = kPORT_UnlockRegister;
}
static void hal_pin_mode_in_float(gpio_pin_config_t *pin_config,port_pin_config_t *port_config)
{
	pin_config->pinDirection = kGPIO_DigitalInput;
	pin_config->outputLogic = 0;

	port_config->pullSelect = kPORT_PullDisable;
	port_config->pullValueSelect = kPORT_LowPullResistor;
	port_config->slewRate = kPORT_FastSlewRate;
	port_config->passiveFilterEnable = kPORT_PassiveFilterDisable;
	port_config->openDrainEnable = kPORT_OpenDrainDisable;
	port_config->driveStrength = kPORT_LowDriveStrength;
	port_config->driveStrength1 = kPORT_NormalDriveStrength;
	port_config->mux = kPORT_MuxAsGpio;
	port_config->lockRegister = kPORT_UnlockRegister;
}
static void hal_pin_mode_in_up(gpio_pin_config_t *pin_config,port_pin_config_t *port_config)
{
	pin_config->pinDirection = kGPIO_DigitalInput;
	pin_config->outputLogic = 0;

	port_config->pullSelect = kPORT_PullUp;
	port_config->pullValueSelect = kPORT_LowPullResistor;
	port_config->slewRate = kPORT_FastSlewRate;
	port_config->passiveFilterEnable = kPORT_PassiveFilterDisable;
	port_config->openDrainEnable = kPORT_OpenDrainDisable;
	port_config->driveStrength = kPORT_LowDriveStrength;
	port_config->driveStrength1 = kPORT_NormalDriveStrength;
	port_config->mux = kPORT_MuxAsGpio;
	port_config->lockRegister = kPORT_UnlockRegister;
}
static void hal_pin_mode_in_down(gpio_pin_config_t *pin_config,port_pin_config_t *port_config)
{
	pin_config->pinDirection = kGPIO_DigitalInput;
	pin_config->outputLogic = 0;

	port_config->pullSelect = kPORT_PullDown;
	port_config->pullValueSelect = kPORT_LowPullResistor;
	port_config->slewRate = kPORT_FastSlewRate;
	port_config->passiveFilterEnable = kPORT_PassiveFilterDisable;
	port_config->openDrainEnable = kPORT_OpenDrainDisable;
	port_config->driveStrength = kPORT_LowDriveStrength;
	port_config->driveStrength1 = kPORT_NormalDriveStrength;
	port_config->mux = kPORT_MuxAsGpio;
	port_config->lockRegister = kPORT_UnlockRegister;
}
static void hal_pin_mode_out_push_pull(gpio_pin_config_t *pin_config,port_pin_config_t *port_config)
{
	pin_config->pinDirection = kGPIO_DigitalOutput;
	pin_config->outputLogic = 0;

	port_config->pullSelect = kPORT_PullDisable;
	port_config->pullValueSelect = kPORT_LowPullResistor;
	port_config->slewRate = kPORT_FastSlewRate;
	port_config->passiveFilterEnable = kPORT_PassiveFilterDisable;
	port_config->openDrainEnable = kPORT_OpenDrainDisable;
	port_config->driveStrength = kPORT_LowDriveStrength;
	port_config->driveStrength1 = kPORT_NormalDriveStrength;
	port_config->mux = kPORT_MuxAsGpio;
	port_config->lockRegister = kPORT_UnlockRegister;
}
static void hal_pin_mode_out_open_drain(gpio_pin_config_t *pin_config,port_pin_config_t *port_config)
{
	pin_config->pinDirection = kGPIO_DigitalOutput;
	pin_config->outputLogic = 0;

	port_config->pullSelect = kPORT_PullDisable;
	port_config->pullValueSelect = kPORT_LowPullResistor;
	port_config->slewRate = kPORT_FastSlewRate;
	port_config->passiveFilterEnable = kPORT_PassiveFilterDisable;
	port_config->openDrainEnable = kPORT_OpenDrainEnable;
	port_config->driveStrength = kPORT_LowDriveStrength;
	port_config->driveStrength1 = kPORT_NormalDriveStrength;
	port_config->mux = kPORT_MuxAsGpio;
	port_config->lockRegister = kPORT_UnlockRegister;
}
static void hal_pin_mode_af(gpio_pin_config_t *pin_config,port_pin_config_t *port_config,port_mux_t alternativenum)
{
	pin_config->pinDirection = 0;
	pin_config->outputLogic = 0;

	port_config->pullSelect = kPORT_PullDisable;
	port_config->pullValueSelect = kPORT_LowPullResistor;
	port_config->slewRate = kPORT_FastSlewRate;
	port_config->passiveFilterEnable = kPORT_PassiveFilterDisable;
	port_config->openDrainEnable = kPORT_OpenDrainDisable;
	port_config->driveStrength = kPORT_LowDriveStrength;
	port_config->driveStrength1 = kPORT_NormalDriveStrength;
	port_config->mux = alternativenum;
	port_config->lockRegister = kPORT_UnlockRegister;
}
/**
 * 函 数 介 绍 ：
 *                 pin初始化
 *
 *  * 参       数
 *                pinindex：引脚索引
 * 返   回  值
 * 备       注 ：   根据pin_cfg配置表初始化引脚
 *
 */
hal_result_t hal_pin_init_index(uint8_t pinindex)
{
    // to be add user code
	hal_result_t result = HAL_RESULT_FAIL;
	uint32_t len = 0;
	gpio_pin_config_t pin_config = {0};
	port_pin_config_t port_config = {0};
	hal_pin_cfg_st * psc_pin_init_cfg_sa = NULL;
	hal_pin_get_cfg(&psc_pin_init_cfg_sa, &len);
	if((pinindex < len)&&(NULL != psc_pin_init_cfg_sa))
	{
		result = HAL_RESULT_SUCCESS;
		switch(psc_pin_init_cfg_sa[pinindex].m_mode_et)
		{
		case HAL_PIN_MODE_IN_ANALOG:
			hal_pin_mode_in_analog(&pin_config,&port_config);
			PORT_SetPinConfig(port_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &port_config);
			break;
		case HAL_PIN_MODE_IN_FLOAT:
			hal_pin_mode_in_float(&pin_config,&port_config);
			GPIO_PinInit(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &pin_config);
			PORT_SetPinConfig(port_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &port_config);
			break;
		case HAL_PIN_MODE_IN_UP:
			hal_pin_mode_in_up(&pin_config,&port_config);
			GPIO_PinInit(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &pin_config);
			PORT_SetPinConfig(port_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &port_config);
			break;
		case HAL_PIN_MODE_IN_DOWN:
			hal_pin_mode_in_down(&pin_config,&port_config);
			GPIO_PinInit(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &pin_config);
			PORT_SetPinConfig(port_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &port_config);
			break;
		case HAL_PIN_MODE_OUT_PUSH_PULL:
			hal_pin_mode_out_push_pull(&pin_config,&port_config);
			GPIO_PinInit(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &pin_config);
			PORT_SetPinConfig(port_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &port_config);
			break;
		case HAL_PIN_MODE_OUT_OPEN_DRAIN:
			hal_pin_mode_out_open_drain(&pin_config,&port_config);
			GPIO_PinInit(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &pin_config);
			PORT_SetPinConfig(port_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &port_config);
			break;
		case HAL_PIN_MODE_AF:
			hal_pin_mode_af(&pin_config,&port_config,psc_pin_init_cfg_sa[pinindex].m_u32_alternativenum);
			PORT_SetPinConfig(port_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &port_config);
			break;
		default:
			result = HAL_RESULT_PARAMETER_ERR;
			break;
		}
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
	return result;
}

/**
 * 函 数 介 绍 ：
 *                 引脚复用
 *
 *  * 参       数
 *                 pinindex： 引脚索引
 * 					afnum ： 复用号
 * 返   回  值
 * 备       注 ：   根据pin_cfg配置表初始化引脚
 *
 */
hal_result_t hal_pin_af(uint8_t pinindex, uint8_t afnum)
{
    // to be add user code
	hal_result_t result = HAL_RESULT_FAIL;
	uint32_t len = 0;
	gpio_pin_config_t pin_config = {0};
	port_pin_config_t port_config = {0};
	hal_pin_cfg_st * psc_pin_init_cfg_sa = NULL;
	hal_pin_get_cfg(&psc_pin_init_cfg_sa, &len);
	if((pinindex < len)&&(NULL != psc_pin_init_cfg_sa))
	{
		hal_pin_mode_af(&pin_config,&port_config,afnum);
		PORT_SetPinConfig(port_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, &port_config);
		result = HAL_RESULT_SUCCESS;
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
	return result;
}

/**
 * 函 数 介 绍 ：
 *                 获得pin脚电平值
 *
 *  * 参       数
 *                 pinindex ： 引脚索引
 * 返   回  值 ：0：低电平；1：高电平
 * 备       注 ：
 *
 */
hal_pin_level_et hal_pin_read(uint8_t pinindex)
{
    // to be add user code
	hal_pin_level_et hal_pin_level = HAL_PIN_LEVEL_LOW;
	uint32_t len = 0;
	hal_pin_cfg_st * psc_pin_init_cfg_sa = NULL;
	hal_pin_get_cfg(&psc_pin_init_cfg_sa, &len);
	if((pinindex < len)&&(NULL != psc_pin_init_cfg_sa))
	{
		hal_pin_level = GPIO_PinRead(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin);
	}
	else
	{

	}
	return hal_pin_level;
}

/**
 * 函 数 介 绍 ：
 *                 设置引脚输出电平
 *
 *  * 参       数
 *                 pinindex ： 引脚索引
 *                  level ：电平值
 * 返   回  值 ：0：成功，其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_pin_write(uint8_t pinindex, hal_pin_level_et level)
{
    // to be add user code
	hal_result_t result = HAL_RESULT_FAIL;
	uint32_t len = 0;
	hal_pin_cfg_st * psc_pin_init_cfg_sa = NULL;
	hal_pin_get_cfg(&psc_pin_init_cfg_sa, &len);
	if((pinindex < len)&&(NULL != psc_pin_init_cfg_sa))
	{
		if(HAL_PIN_LEVEL_HIGH == level)
		{
			GPIO_PinWrite(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, 1);
			result = HAL_RESULT_SUCCESS;
		}
		else if(HAL_PIN_LEVEL_LOW == level)
		{
			GPIO_PinWrite(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, 0);
			result = HAL_RESULT_SUCCESS;
		}
		else
		{
			result = HAL_RESULT_PARAMETER_ERR;
		}
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
	return result;
}

static hal_result_t hal_pin_irq_setportcbk(uint32_t pin,hal_pin_cbk_t *cbk_array,uint32_t pinnum,hal_pin_cbk_t cbk)
{
	hal_result_t result = HAL_RESULT_FAIL;
	if(pin < pinnum)
	{
		cbk_array[pin] = cbk;
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
	return result;
}

static hal_result_t hal_pin_irq_setcbk(uint32_t portx,uint32_t pin,hal_pin_cbk_t cbk)
{
	hal_result_t result = HAL_RESULT_FAIL;
	switch(portx)
	{
	case HAL_PORTA:
		result = hal_pin_irq_setportcbk(pin,porta_cbk_array,PORTA_PINNUM,cbk);
		break;
	case HAL_PORTB:
		result = hal_pin_irq_setportcbk(pin,portb_cbk_array,PORTB_PINNUM,cbk);
		break;
	case HAL_PORTC:
		result = hal_pin_irq_setportcbk(pin,portc_cbk_array,PORTC_PINNUM,cbk);
		break;
	case HAL_PORTD:
		result = hal_pin_irq_setportcbk(pin,portd_cbk_array,PORTD_PINNUM,cbk);
		break;
	default:
		break;
	}
	return result;
}

/**
 * 函 数 介 绍 ：
 *                 设置引脚中断
 *
 *  * 参       数
 *                 pinindex ： 引脚索引
 *                  priorty ：中断优先级
 *                  functioncbk ：中断回调函数
 * 返   回  值 ：0：成功，其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_pin_irq(uint8_t pinindex, hal_irq_priorty_et priorty, hal_pin_cbk_t functioncbk)
{
    // to be add user code
	hal_result_t result = HAL_RESULT_FAIL;
	uint32_t len = 0;
	hal_pin_cfg_st * psc_pin_init_cfg_sa = NULL;
	hal_pin_get_cfg(&psc_pin_init_cfg_sa, &len);
	if((pinindex < len)&&(NULL != psc_pin_init_cfg_sa))
	{
		result = hal_pin_irq_setcbk(psc_pin_init_cfg_sa[pinindex].m_u32_port,psc_pin_init_cfg_sa[pinindex].m_u32_pin, functioncbk);
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
	return result;
}

/**
 * 函 数 介 绍 ：
 *                 中断使能控制
 *
 *  * 参       数
 *                 pinindex ： 引脚索引
 *                 enabled ： 是否使能
 * 返   回  值 ：0：成功，其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_pin_irq_enabled(uint8_t pinindex, hal_enabled_et enabled)
{
    // to be add user code
	hal_result_t result = HAL_RESULT_FAIL;
	uint32_t len = 0;
	hal_pin_cfg_st * psc_pin_init_cfg_sa = NULL;
	hal_pin_get_cfg(&psc_pin_init_cfg_sa, &len);
	if((pinindex < len)&&(NULL != psc_pin_init_cfg_sa))
	{
		result = HAL_RESULT_SUCCESS;
		if(HAL_ENABLED_ENABLE == enabled)
		{
			switch(psc_pin_init_cfg_sa[pinindex].m_irq_mode_e)
			{
			case HAL_PIN_IRQ_MODE_FALLEDGE:
				GPIO_SetPinInterruptConfig(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, kGPIO_InterruptFallingEdge);
				break;
			case HAL_PIN_IRQ_MODE_RISEEDGE:
				GPIO_SetPinInterruptConfig(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, kGPIO_InterruptRisingEdge);
				break;
			case HAL_PIN_IRQ_MODE_RISE_FALL:
				GPIO_SetPinInterruptConfig(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, kGPIO_InterruptEitherEdge);
				break;
			case HAL_PIN_IRQ_MODE_HIGH:
				GPIO_SetPinInterruptConfig(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, kGPIO_InterruptLogicOne);
				break;
			case HAL_PIN_IRQ_MODE_LOW:
				GPIO_SetPinInterruptConfig(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, kGPIO_InterruptLogicZero);
				break;
			default:
				result = HAL_RESULT_PARAMETER_ERR;
				break;
			}
		}
		else
		{
			GPIO_SetPinInterruptConfig(gpio_array[psc_pin_init_cfg_sa[pinindex].m_u32_port], psc_pin_init_cfg_sa[pinindex].m_u32_pin, kGPIO_InterruptStatusFlagDisabled);
		}
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
	return result;
}
/* Function End*/
