//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_pin.c
 * 摘要：
 *          文件主要描述clock模块
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
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

void hal_pin_irqhandler(GPIO_Type * gpio,hal_pin_cbk_t *cbk_array,uint8_t pinnum)
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

void GPIOA_INT0_IRQHandlerhandle(void)
{
	hal_pin_irqhandler(GPIOA,porta_cbk_array,PORTA_PINNUM);
}

// void GPIOA_INT0_IRQHandler(void)
// {
// 	hal_pin_irqhandler(GPIOA,porta_cbk_array,PORTA_PINNUM);
// }

// void GPIOA_INT1_IRQHandler(void)
// {
// }

// void GPIOB_INT0_IRQHandler(void)
// {
// 	hal_pin_irqhandler(GPIOB,portb_cbk_array,PORTB_PINNUM);
// }

// void GPIOB_INT1_IRQHandler(void)
// {

// }

// void GPIOC_INT0_IRQHandler(void)
// {
// 	hal_pin_irqhandler(GPIOC,portc_cbk_array,PORTC_PINNUM);
// }

// void GPIOC_INT1_IRQHandler(void)
// {

// }

// void GPIOD_INT0_IRQHandler(void)
// {

// 	hal_pin_irqhandler(GPIOD,portd_cbk_array,PORTD_PINNUM);
// }

// void GPIOD_INT1_IRQHandler(void)
// {

// }
/**
* @brief  ：
 *                 所有PIN脚进行初始化
 *                 通过函数接口 hal_pin_get_cfg(hal_pin_cfg_st *pincfg,uint32_t *len) 获取配置表信息，
 *                 按照配置表信息逐行初始化
 *
  * @param ：
 *
* @return hal_result_t   @ref           HAL_RESULT_SUCCESS   
 * @note ：
 *              原始参数来源于sc_pin_init_cfg_sa中的配置
 *               hal_pin_cfg_st * pincfg脚配置属性
 *              m_u32_port       配置端口号，可以为单片机原有定义直接映射
 *              m_u32_pin        配置PIN脚号，可以为单片机原有定义直接映射
 *              m_direction_et   配置输入输出，详情参考hal_pin_direction_et
 *              m_mode_et        配置默认PIN脚模式，如上拉等 详情参考hal_pin_mode_et
 *              m_u32_alternativenum 配置中复用号,可以通过#define HAL_PIN_GET_MULTIPLE(PINX_X,FUNCITON)
 *                              获取复用定义中的值，也可以直接调用复用定义中的宏定义。如果未使用复用则直接
 *                              调用HAL_PIN_AF_NULL
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

/**
* @brief  ：
 *                 恢复PIN index对应的PIN脚及其参数 参考sc_pin_init_cfg_sa中的配置
 *
  * @param ：
 *                  [in] pinindex pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index宏定义组
* @return hal_result_t   @ref           HAL_RESULT_SUCCESS   
 * @note ：
 *
 */
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
* @brief  ：
 *                 设置PIN index对应的PIN脚的复用功能
 *
  * @param ：
 *                  [in] pinindex pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *                  @param [in] afnum    pin对应功能的复用号，可以通过宏获取该号码或直接使用宏定义，参考hal_pin_cfg.h
 *                                :复用定义 宏定义组
* @return hal_result_t   @ref           HAL_RESULT_SUCCESS   
 * @note ：
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
* @brief  ：
 *                 获取Pin脚电平状态
 *
  * @param ：
 *                  [in] pinindex pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *
 * @return ：hal_result_t   >=0 :操作成功 详情参考 hal_pin_if.h:hal_pin_level_et 枚举
 *                              <0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * @note ：
 *
 */
__IO hal_pin_level_et hal_pin_read(uint8_t pinindex)
{
    // to be add user code
	__IO hal_pin_level_et hal_pin_level = HAL_PIN_LEVEL_LOW;
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
* @brief  ：
 *                 获取Pin脚电平状态
 *
  * @param ：
 *                  [in] pinindex   pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *                  [in] level      所得电平状态
 * @return ：hal_result_t    =0 :操作成功
 *                              <0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * @note ：
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


/**
* @brief  ：
 *                 设置PIN脚中断
 *
  * @param ：
 *                  [in] pinindex       pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *                  [in] priorty        设置中断优先级
 *                  [in] functioncbk    设置中断回调函数
 * @return ：hal_result_t    =0 :操作成功
 *                              <0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * @note ：
 *              需完成初始化后才允许调用
 */

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

//HAL_PIN_IRQ_MODE_FALLEDGE = 0,
//HAL_PIN_IRQ_MODE_RISEEDGE,
//HAL_PIN_IRQ_MODE_RISE_FALL,
//HAL_PIN_IRQ_MODE_HIGH,
//HAL_PIN_IRQ_MODE_LOW
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
* @brief  ：
 *                 设置PIN中断使能状态
 *
  * @param ：
 *                  [in] pinindex   pin在sc_pin_init_cfg_sa中的索引号 参考hal_pin_if.h:PIN index 宏定义组
 *                  @param [in] enabled    设置中断使能状态
 * @return ：hal_result_t    =0 :操作成功
 *                              <0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * @note ：
 *              需完成初始化后才允许调用
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
