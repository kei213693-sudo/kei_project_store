#include "timer.h"
#include "usart.h"
#include "atk_ble03_uart.h"
#include "bsp_button.h"

//stm32f103rc就三个TIM（1、2、3）能用

//stm32的定时器除了TIM6和7。其他的定时器都可以用来产生PWM输出。其中高级定
//时器TIM1和TIM8可以同时产生多达7路的PWM输出。而通用定时器也能同时产生多达4路的PWM输出，

TIM_HandleTypeDef 	TIM1_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef 	TIM1_CH1Handler;	//定时器1通道1句柄

TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 
volatile unsigned long long FreeRTOSRunTimeTicks;//FreeRTOS 时间统计所用的节拍计数器

TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 

//TIM1 PWM部分初始化 
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//stm32中CCRx决定占空比，AAR决定输出频率
void TIM1_PWM_Init(u16 arr,u16 psc)
{  
    TIM1_Handler.Instance=TIM1;         	//定时器1
    TIM1_Handler.Init.Prescaler=psc;       	//定时器分频
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM1_Handler.Init.Period=arr;          	//自动重装载值
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM1_Handler);       	//初始化PWM
    
    TIM1_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM1_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM1_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低，此时cnt < CCRx/Pulse输出1
    TIM1_CH1Handler.OCIdleState=TIM_OCIDLESTATE_SET; //空闲状态电平，配置为高，灯熄灭;这里如果不配置，pwm停止后电平状态不确定，可能为低，导致亮灯

    HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH1Handler,TIM_CHANNEL_1);//配置TIM1通道1
}

void TIM1_PWM_deInit(void)
{
    HAL_TIM_PWM_DeInit(&TIM1_Handler);
}

//定时器1中断服务函数
void TIM1_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);
}

//初始化 TIM2 使其为 FreeRTOS 的时间统计提供时基
void ConfigureTimeForRunTimeStats(void)
{
    //定时器 2 初始化，定时器时钟为 72M，分频系数为 72-1，所以定时器 2 的频率
    //为 72M/72=1M，自动重装载为 50-1，那么定时器周期就是 50us
    //tick是1ms，所以是其20倍
    FreeRTOSRunTimeTicks=0;

    TIM2_Handler.Instance=TIM2;                          //通用定时器2
    TIM2_Handler.Init.Prescaler=(72-1);                     //分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=(TIMR_TIM2_BASE_US-1);                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);

    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器2和定时器2更新中断：TIM_IT_UPDATE   
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
//定时器3初始化，默认系统时钟为72Mhz，第一个入参决定计时时间，单位ms
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    /*
    因为TIM_TimeBaseInit会导致update事件。如果这个时候中断是使能的就会立即进入中断。
    这是TIM_TimeBaseInit函数最后一条语句
    Generate an update event to reload the Prescaler
    and the repetition counter(only for TIM1 and TIM8) value immediately
    TIMx->EGR = TIM_PSCReloadMode_Immediate;
    所以初始化之后必须清空标志位，避免第一次启动马上就触发进入中断服务
    */
    //__HAL_TIM_CLEAR_IT(&TIM3_Handler,TIM_IT_UPDATE);//非周期性定时调用
	HAL_TIM_Base_Start_IT(&TIM3_Handler); //周期性产生中断
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
//调用HAL_TIM_PWM_Init（）进入
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();			//使能定时器1
    }
}

//定时器底层驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
//调用HAL_TIM_Base_Init（）进入
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
	{
        //做freertos时基的TIM中断优先级要最高
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY),3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断   
	}
    else if(htim->Instance==TIM3)
	{
        //TIM3中断优先级要最高要低于uart1
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3),3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}
}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM2_Handler))
    {
        #if (1 == configGENERATE_RUN_TIME_STATS)
        FreeRTOSRunTimeTicks++; //freertos运行时间统计时基计数器加一
        #endif
    }
	else if(htim==(&TIM3_Handler))
    {
        //处理uart1
        LOG_UART_RxIrqCheck();

        //处理uart5
        ATK_BLE03_UART_RxIrqCheck();

        //处理button0、1、2
        bsp_button_IrqCheck();
    }
}

//设置TIM1通道1的占空比
//*pCompare:比较值
void hw_tim_tim1PwmChannel1_compare_set(void *pData)
{
    if(NULL != pData)
    {
        u32 *pCompare = (u32 *)pData;
	    TIM1->CCR1 = *pCompare;
    }

    //另外一种设置方法
    // TIM1_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    // TIM1_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    // TIM1_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低，此时cnt < CCRx/Pulse输出1
    // HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH1Handler,TIM_CHANNEL_1);//配置TIM1通道1
}

void hw_tim_tim1PwmChannel1_start(void)
{
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);//开启PWM通道1
}

void hw_tim_tim1PwmChannel1_stop(void)
{
    HAL_TIM_PWM_Stop(&TIM1_Handler,TIM_CHANNEL_1);//关闭PWM通道1
}
