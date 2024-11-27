#include "main.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "iwdg.h"

#include "app_task.h"
#include "bsp_log.h"
#include "bsp_time.h"

#if APP_SYSVIEW_ENABLE
#include "SEGGER_SYSVIEW.h"
#endif

#include "hw_w25qxx.h"
#include "atk_ble03.h"

/*
//跳转到boot，带了freertos为什么跳不过去？软硬件重启是可以的
void app_jump_to_boot(void)
{
	typedef        void (*pFunction)(void);
	pFunction      BootResetFunction;//boot的复位函数地址

	if (((*(__IO uint32_t*)BOOT_START_ADDR) & 0x2FFE0000) == 0x20000000)
	{//判断堆栈地址是否正确指向RAM空间
		printf("Jump to Boot!\r\n");
		__set_FAULTMASK(1);//禁止所有中断
		BootResetFunction = (pFunction)(*(__IO uint32_t*) (BOOT_START_ADDR + 4));//指向boot的中断复位函数
		//__set_PSP(*(__IO uint32_t*)BOOT_START_ADDR); 
		//__set_CONTROL(0);
		__set_MSP(*(__IO uint32_t*) BOOT_START_ADDR);//设置app的栈顶指针,可以用MSR_MSP(*(__IO uint32_t*)BOOT_START_ADDR)替代
		BootResetFunction();//跳转到boot去执行复位函数
	}
	else
	{
		printf("stack top err:0x%x\r\n",((*(__IO uint32_t*)BOOT_START_ADDR) & 0x2FFE0000));
	}
}
*/

void led_init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();           	//开启GPIOD时钟
	
    GPIO_Initure.Pin=GPIO_PIN_8; 				//PA8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_2; 				//PD2
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);		//PA8置1 
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);		//PA8置0

	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);		//PD2置1  			
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);		//PD2置0
}

/**************************************************************
*函数功能描述 ：各类外设初始化
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_peripheral_init(void)
{
    uart_init(115200);
	delay_init(72);//初始化延时和freertos滴答计时器
	#if APP_WDT_ENABLE
	IWDG_Init(IWDG_PRESCALER_64,(2500));//分频系数64时，每1000就是1.6s左右，这里定时4s	
    #endif
    led_init();
}

/**************************************************************
*函数功能描述 ：各类组件初始化
*函数参数 ：
*函数返回值 ：
*备注：有些可能包含部分外设初始化，例如时间信息会初始化rtc模块
**************************************************************/
static void app_components_init(void)
{
    bsp_time_init();//时间信息组件初始化，包含rtc
	W25QXX_Init();//初始化外部flash
}

int main(void)
{
	#if APP_BOOT_ENABLE
	SCB->VTOR = APP_START_ADDR;//设置中断向量表的偏移量
	__set_FAULTMASK(0);//使能所有中断
	#endif

	uint32_t StartUpMode = RCC->CSR;//重启源，通过寄存器RCC_CSR获取，具体查看对应的芯片手册
    __HAL_RCC_CLEAR_RESET_FLAGS();//清除重启标志

    HAL_Init();//初始化HAL库
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);//设置系统中断优先级分组4，4bit全为抢占优先级，没有子优先级了
    Stm32_Clock_Init(RCC_PLL_MUL9);//设置时钟,72M
	app_peripheral_init();
    app_components_init();

    #if APP_SYSVIEW_ENABLE
    SEGGER_SYSVIEW_Conf();//用于SystemView可视化调试
	#endif
	
	if(pdPASS == app_task_init())
    {
        //打印基本信息
        bsp_log_print(BSP_LOG_MODULE_NONE,BSP_LOG_TYPE_INFO,"**************************************************************");
        bsp_log_print(BSP_LOG_MODULE_NONE,BSP_LOG_TYPE_INFO,"StartUpMode:0x%x",StartUpMode);
        bsp_log_print(BSP_LOG_MODULE_NONE,BSP_LOG_TYPE_INFO,"Version data:%s",APP_VERSION_DATE);
        bsp_log_print(BSP_LOG_MODULE_NONE,BSP_LOG_TYPE_INFO,"Version time:%s",APP_VERSION_TIME);
        bsp_log_print(BSP_LOG_MODULE_NONE,BSP_LOG_TYPE_INFO,"APP version:%d.%d.%d", APP_VERSION_MAIN, APP_VERSION_SUB1,APP_VERSION_SUB2);
        bsp_log_print(BSP_LOG_MODULE_NONE,BSP_LOG_TYPE_INFO,"APP Start");
        bsp_log_print(BSP_LOG_MODULE_NONE,BSP_LOG_TYPE_INFO,"**************************************************************");
	
        vTaskStartScheduler();
    }

    while(1)
    {
        bsp_log_print(BSP_LOG_MODULE_NONE,BSP_LOG_TYPE_ERROR,"Init fail,Please restart!!!");
        delay_xms(1*1000);
    }
}
