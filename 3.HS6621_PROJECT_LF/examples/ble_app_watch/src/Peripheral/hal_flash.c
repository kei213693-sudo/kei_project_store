/*******************************头文件*******************************/
#include "hal_flash.h"
#include "peripheral.h"

/*******************************内部函数的声明*******************************/

/*******************************内部变量定义*******************************/

/*******************************全局变量定义*******************************/
/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：初始化flash
*函数参数 ：
*函数返回值 ：
*备注：HS_SF是内部flash，cs=0，HS_SF1和HS_SF2控制外部flash
**************************************************************/
void hal_flash_init(void)
{
    uint8_t result = 0;
	//uint32_t flash_id = 0;
	sf_config_t inside_sf_cfg = {
		.freq_hz = 1000000,//工作频率，hz
		.width = SF_WIDTH_4LINE,//4线工作模式
		.delay = 5,
	};
	
	//初始化flash
	sf_enable(HS_SF, HAL_FLASH_CS);
	sf_config(HS_SF, HAL_FLASH_CS, &inside_sf_cfg);
	
	//检测是否存在flash	
	result = sf_detect(HS_SF, HAL_FLASH_CS);
	if(result != true)
	{
		log_debug("bsp_flash_init err!\n");
	}

	//调用了也没用，pmu还是显示SF1 SF2的clock被使能，但是实际没有用到，感觉是使能了一个SF，所有的clock都会被启用
	// sf_disable(HS_SF1, 0);
	// sf_disable(HS_SF2, 0);
	
	//相同型号芯片id相同，例如HS6621CG-C就是0x00856014
	// uint32_t flash_id = sf_read_id(HS_SF, HAL_FLASH_CS);
	// log_debug("flash_id:0x%08x\n",flash_id);
}

/**************************************************************
*函数功能描述 ：去初始化flash
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void hal_flash_deinit(void)
{
    sf_disable(HS_SF, HAL_FLASH_CS);
}

/**************************************************************
*函数功能描述 ：擦除
*函数参数 ：
*函数返回值 ：
*备注：内部flash一个扇区大小是4k
**************************************************************/
void hal_flash_erase(uint32_t addr,uint32_t size)
{
    sf_status_t sf_sts = sf_status(HS_SF, HAL_FLASH_CS);//检查flash状态

	if((SF_STATUS_PRESENT == sf_sts) && (size > HAL_FLASH_CS)
		&& (addr >= HAL_FLASH_USER_DATA_START) && ((addr + size) <= HAL_FLASH_USER_DATA_END))
	{
		sf_erase(HS_SF, HAL_FLASH_CS, addr, size);
	}
}

/**************************************************************
*函数功能描述 ：读取
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void hal_flash_read(uint32_t addr,uint8_t *pData,uint32_t len)
{
    sf_status_t sf_sts = sf_status(HS_SF, HAL_FLASH_CS);

	if((SF_STATUS_PRESENT == sf_sts) && (pData != NULL) && (len > HAL_FLASH_CS)
		&& (addr >= HAL_FLASH_USER_DATA_START) && ((addr + len) <= HAL_FLASH_USER_DATA_END))
	{
		sf_read(HS_SF, HAL_FLASH_CS, addr, pData, len);
	}
}

/**************************************************************
*函数功能描述 ：写入
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void hal_flash_write(uint32_t addr,uint8_t *pData,uint32_t len)
{
    sf_status_t sf_sts = sf_status(HS_SF, HAL_FLASH_CS);

	if((SF_STATUS_PRESENT == sf_sts) && (pData != NULL) && (len > HAL_FLASH_CS)
		&& (addr >= HAL_FLASH_USER_DATA_START) && ((addr + len) <= HAL_FLASH_USER_DATA_END))
	{
		sf_write(HS_SF, HAL_FLASH_CS, addr, pData, len);
	}
}


