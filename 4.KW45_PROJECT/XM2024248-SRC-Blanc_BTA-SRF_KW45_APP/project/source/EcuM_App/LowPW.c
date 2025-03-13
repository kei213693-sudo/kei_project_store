/*
 * LowPW.c
 *
 *  Created on: 2024年7月27日
 *      Author: F704011769_
 */
#include "LowPW.h"
#include "hal_sys.h"
#include "KW45B41Z83.h"
#include "hal_pin_if.h"
#include "hal_rte.h"
#include "hal_sys.h"
#include "hal_can_if.h"
#include "hal_spi.h"
#include "hal_test.h"
#include "hal_uart_if.h"
#include "hal_sys.h"
#include "NCK2910.h"

uint32_t  CAN_Stb_CloseWait = 0;
uint8_t CAN_Stb_DisableFlag = 0;
/*****************************************************************************
  * 函数:MCU_LowPW
  * 功能:完成对进入低功耗之前、之后的处理。
  * 参数:void
  * 返回:null
  * 说明:1，使用唤醒源的唤醒电平信号进行唤醒，包括端口的电平、中断标志位的电平等等。MCU 唤醒与模块中断使能无关，
 *			但是需要使能全局中断；
 *		2，中断标志位为置位状态时，会造成 MCU 无法进入低功耗模式；
  *		3，在中断处理程序中，不得出现关闭唤醒源的操作，包括潜在形式的。避免无法唤醒 MCU 的情况；
  *		4，为了保证MCU能够可靠地进入休眠和被唤醒，根据车厂的能够随时唤醒MCU的要求，和 MCU进入低功耗
  * 		前需要清除所有中断标志的要求，综合过往的经验，总结出：
  * 		A，低功耗的处理顺序：所有的外设进入低功耗状态、清除所有的标志位、设置和使能唤醒源、执行进入低
  * 			功耗模式的指令、恢复所有的外设到工作状态、关闭唤醒源、使能IRQ总中断；
  * 		B，在中断处理程序中，不得出现关闭唤醒源的操作，包括潜在形式的；
  * 		C，在 BootLoader 和 APP 中没有使能的模块，进入低功耗模式之前，如果清除它们的标志位，需要先使能模块，
  *				避免进入 HardFault 中断。
 ****************************************************************************/
void MCU_lowPW(void)
{
	/* 第一步，MCU进入低功耗模式之前的操作。  所有的外设进入低功耗状态。*/
	//所有悬空角依据芯片特性置输出，无关外设关闭。
	Sleep_IOC();

	/* 第二步，清除 MCU 的所有中断标志位。*/
	//TODO 清除不受系统控制模块控制的功能模块的所有中断标志位。 后续需依据功能是否设计相关中断增加或删减
	Clear_Isr();
	hal_adc_if_deinit();
	hal_spi_deinit();
	/* 第三步，设置唤醒源。 */
	sys_sleep_init();

	hal_sys_sleep();

	/* 第四步，MCU 执行进入低功耗模式的指令 */


	/* 第五步， 恢复现场，恢复所有的外设到工作状态。 */
	CAN_Stb_DisableFlag = 0;
	Sleepwakeupinit();

	Enable_Isr();
	/* 第六步， 关闭唤醒源，避免产生无效的中断 */
	// 确认需要关闭的唤醒源 后续需依据启用的唤醒源进行处理
	hal_sys_wakeupsource_disable();
}


/*****************************************************************************
  * 函数:PreSleep_Judge
  * 功能:在预休眠状态中，需要判断唤醒源，若存在唤醒则需要进行唤醒处理。
  * 参数:void
  * 返回:null
  * 说明:\
 ****************************************************************************/
uint8_t PreSleep_Judge()
{
	static uint8_t cnt = 0;
	cnt++;
	if(cnt >= 5)
	{
		cnt = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************
  * 函数:UWBSleep_timeout_Judge
  * 功能:在预休眠状态中，需要判断唤醒源，若存在唤醒则需要进行唤醒处理。
  * 参数:void
  * 返回:null
  * 说明:\
 ****************************************************************************/
uint8_t UWBSleep_timeout_Judge()
{
	static uint8_t cnt = 0;
	cnt++;
	if(cnt >= 20)
	{
		cnt = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************
  * 函数:Sleep_IOC
  * 功能:在休眠前需要对相关的IO口做处理，悬空的IO需要依据芯片特性执行，并关闭相关外设。
  * 参数:void
  * 返回:null
  * 说明:\
 ****************************************************************************/
void Sleep_IOC(void)
{
	hal_pin_if_write(HAL_PIN_INDEX10, HAL_PIN_LEVEL_LOW);
	hal_pin_if_write(CANSTB_INDEX, HAL_PIN_LEVEL_HIGH);
}

/*****************************************************************************
  * 函数:Clear_Isr
  * 功能:在休眠前需要对相关的IO口做处理，悬空的IO需要依据芯片特性执行，并关闭相关外设。
  * 参数:void
  * 返回:null
  * 说明:\
 ****************************************************************************/
void Clear_Isr(void)
{
	DisableIRQ(ADC0_IRQn);
	DisableIRQ(GPIOB_INT0_IRQn);
}

void Enable_Isr(void)
{
	EnableIRQ(ADC0_IRQn);
	EnableIRQ(GPIOB_INT0_IRQn);
}

/*****************************************************************************
  * 函数:NM_SleepPre_Judge
  * 功能:NM休眠判定，当NM网络状态进入休眠状态时，需要使能can中断脚。
  * 参数:void
  * 返回:null
  * 说明:\
 ****************************************************************************/
void CAN_RX_IRQ_Callback(void)
{
	hal_pin_init_index(14);  //PTC1 CAN RX Int
	GPIO_SetPinInterruptConfig(GPIOC, 1, kGPIO_InterruptStatusFlagDisabled); //CAN RX Int
	hal_pin_if_write(CANSTB_INDEX, HAL_PIN_LEVEL_LOW); //CAN-STB Enable
}


void NM_SleepPre_Judge(void)
{
	gpio_pin_config_t pin_config = {0};
	port_pin_config_t port_config = {0};
	gpio_interrupt_config_t interrupt_config = {0};
	OSA_InterruptDisable();
	hal_pin_if_write(CANSTB_INDEX, HAL_PIN_LEVEL_HIGH); //CAN-STB Disable
	pin_config.pinDirection = kGPIO_DigitalInput;
	port_config.pullSelect = HAL_PIN_MODE_IN_UP;
	port_config.mux = kPORT_MuxAsGpio;
	GPIO_PinInit(GPIOC, 1, &pin_config);
	PORT_SetPinConfig(PORTC, 1, &port_config);
	hal_pin_if_irq(14, 0, CAN_RX_IRQ_Callback); //CAN RX Int
	GPIO_SetPinInterruptConfig(GPIOC, 1, kGPIO_InterruptFallingEdge); //CAN RX Int
	OSA_InterruptEnable();
}
