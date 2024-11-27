#include <string.h>
#include "main.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "iwdg.h"
#include "bsp_upgrade.h"
#include "bsp_ymodem.h"

bsp_upgrade_ctl_t g_stBspUpgradeCtl = {0};
uint8_t g_dataCode[BSP_INFLASH_SECTOR_SIZE] = {0};
bsp_ymodem_ctl_t g_sttBspYmodemCtl = {0};

static void app_upgrade_handle(uint8_t u8Force);

//跳转到app
void boot_jump_to_app(void)
{
	typedef        void (*pFunction)(void);
	pFunction      AppResetFunction;//app的复位函数地址

	if (((*(__IO uint32_t*)APP_START_ADDR) & 0x2FFE0000) == 0x20000000)
	{//判断堆栈地址是否正确指向RAM空间
		printf("Jump to App!\r\n");
		delay_ms(10);
		__set_FAULTMASK(1);//禁止所有中断
		AppResetFunction = (pFunction)(*(__IO uint32_t*) (APP_START_ADDR + 4));//指向app的中断复位函数
		__set_MSP(*(__IO uint32_t*) APP_START_ADDR);//设置app的栈顶指针,可以用MSR_MSP(*(__IO uint32_t*)APP_START_ADDR)替代
		//SCB->VTOR = APP_START_ADDR;//设置app的中断向量表地址，这里设置了没有用，app还是默认值
		AppResetFunction();//跳转到app去执行复位函数
	}
	else
	{
		printf("stack top err:0x%x\r\n",((*(__IO uint32_t*)APP_START_ADDR) & 0x2FFE0000));
	}
}

/***注意：本工程和教程中的新建工程3.3小节对应***/
int main(void)
{   
	uint8_t u8Force = 0;
	uint32_t u32JumpCount = 0;
	SCB->VTOR = BOOT_START_ADDR;//设置中断向量表的偏移量
	__set_FAULTMASK(0);//使能所有中断

	uint32_t StartUpMode = RCC->CSR;//重启源，通过寄存器RCC_CSR获取，具体查看对应的芯片手册
    __HAL_RCC_CLEAR_RESET_FLAGS();//清除重启标志
	
    HAL_Init();                    	 			//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   			//设置时钟,72M
	delay_init(72);
	uart_init(115200);
	#if BOOT_WDT_ENABLE
	IWDG_Init(IWDG_PRESCALER_64,(2500));//分频系数64时，每1000就是1.6s左右，这里定时4s
	#endif
	W25QXX_Init();//初始化外部flash
	
	//打印基本信息
	printf("**************************************************************\r\n");
	printf("StartUpMode:0x%x\r\n",StartUpMode);
	printf("Version data:%s\r\n",BOOT_VERSION_DATE);
	printf("Version time:%s\r\n",BOOT_VERSION_TIME);
	printf("BOOT version:%d.%d.%d\r\n", BOOT_VERSION_MAIN, BOOT_VERSION_SUB1,BOOT_VERSION_SUB2);
	printf("BOOT Start\r\n");
	printf("**************************************************************\r\n");
	
	//初始化程序升级组件
	g_pStBspYmodemCtl = &g_sttBspYmodemCtl;
	g_pStBspUpgradeCtl = &g_stBspUpgradeCtl;
	g_pDataCode = g_dataCode;
	bsp_upgrade_init(BSP_UPGRADE_AREA_APP);
	
	while(1)
	{
		while(u32JumpCount < BOOT_JUMP_COUNT)
		{
			#if BOOT_WDT_ENABLE
			IWDG_Feed();
			#endif
			u32JumpCount++;
			//1.判定app是否需要升级并进行相应操作
			u8Force = (u32JumpCount < BOOT_JUMP_COUNT) ? 0 : 1;
			app_upgrade_handle(u8Force);//连续BOOT_JUMP_COUNT次跳转失败，强制重新刷写程序
			
			//2.跳转APP
			boot_jump_to_app();
			printf("jump fail count:%d\r\n",u32JumpCount);
			delay_ms(1000);
		}
				
		//4.失败，进入uart升级模式
		bsp_ymodem_recv(0);//程序多出5648bytes
		delay_ms(1000);//延时1s再次进入uart升级模式
	}
}

/**************************************************************
*函数功能描述 ：适用于裸机环境的ymodem接收api
*函数参数 ：u8Force，为1的时候u8UpFlag为0也要强制刷写代码
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_upgrade_handle(uint8_t u8Force)
{
	uint8_t u8UpFlag = g_pStBspUpgradeCtl->stInfo.u8UpFlag;
	//printf("u8UpFlag:%d\r\n",u8UpFlag);
		
	if(((0 == u8UpFlag) && (1 == u8Force)) 
		|| (1 == u8UpFlag))
	{
		//刷写代码，完成后再跳转
		bsp_upgrade_copy_area_code(APP_START_ADDR,APP_AREA_SIZE);
	}
	else if(2 == u8UpFlag)
	{//擦除升级包存储区域，将现有程序重新拷贝过去，重置该标志信息，完成后再跳转
		bsp_upgrade_copy_area_store(APP_START_ADDR,APP_AREA_SIZE);
	}
}

