/*
 * hw_gpio.h
 *
 *  Created on: 2024年12月6日
 *	  Abstract: 用于stm32f103的通用gpio口配置（输入/输出）；stm32f103中，io外部中断线是0~15
 */
/*******************************头文件*******************************/
#include "hw_gpio.h"
#include "FreeRTOS.h"
#include "bsp_button.h"

/*******************************全局变量定义*******************************/


/*******************************内部变量定义*******************************/

/*******************************内部函数的声明*******************************/


/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：button相关引脚初始化
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void hw_gpio_button_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //开启GPIOA时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();               //开启GPIOC时钟
    
    GPIO_Initure.Pin=HW_GPIO_INDEX_BUTTON_0; 				//PC5
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING_FALLING;     //双边沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(HW_GPIO_TYPE_BUTTON_0,&GPIO_Initure);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1),0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    GPIO_Initure.Pin=HW_GPIO_INDEX_BUTTON_1; 				//PA15
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING_FALLING;     //双边沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(HW_GPIO_TYPE_BUTTON_1,&GPIO_Initure);
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1),0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);        	//使能中断线2

    GPIO_Initure.Pin=HW_GPIO_INDEX_BUTTON_2;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING_FALLING;      //双边沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(HW_GPIO_TYPE_BUTTON_2,&GPIO_Initure);
    HAL_NVIC_SetPriority(EXTI0_IRQn,(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1),0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //使能中断线0
}

/**************************************************************
*函数功能描述 ：button相关引脚去初始化
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void hw_gpio_button_deinit(void)
{
    //__HAL_RCC_GPIOA_CLK_DISABLE()不能随便调用
    //，这是针对整个GPIOA.x引脚全体的，其他引脚可能还要用到这个时钟
    HAL_GPIO_DeInit(HW_GPIO_TYPE_BUTTON_0,HW_GPIO_INDEX_BUTTON_0);
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    HAL_GPIO_DeInit(GPIOA,HW_GPIO_INDEX_BUTTON_1);
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
    HAL_GPIO_DeInit(GPIOA,HW_GPIO_INDEX_BUTTON_2);
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
}


//中断服务函数，HAL_GPIO_EXTI_IRQHandler会清空标志位，然后调用HAL_GPIO_EXTI_Callback（）
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);		//调用中断处理公用函数
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);		//调用中断处理公用函数
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);		//调用中断处理公用函数
}

//在HAL库中，所有的外部中断服务函数最后都会调用此函数
//GPIO_Pin:中断引脚号
//为什么按一次会进入两次
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if((HW_GPIO_INDEX_BUTTON_0 == GPIO_Pin)
        || (HW_GPIO_INDEX_BUTTON_1 == GPIO_Pin)
        || (HW_GPIO_INDEX_BUTTON_2 == GPIO_Pin))
    {//处理button
        bsp_button_handle(GPIO_Pin);
    }
    else
    {

    }
}


