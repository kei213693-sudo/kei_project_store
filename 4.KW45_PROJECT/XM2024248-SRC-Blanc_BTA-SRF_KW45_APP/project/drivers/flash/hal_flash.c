/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_flash.c
 * 摘要：
 *      文件概述：
 *                  文件主要描述flash模块功能实现，需要保证接口函数功能实现，本文件为模板文件，
 *              实现实体需要自行补充。
 *      文件调用：
 *                  文件应包含hal_flash_if.h hal_flash.h hal_flash_cfg.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

/*  Include     Begine      */
#include "hal_flash.h"
#include "hal_flash_cfg.h"
#include "hal_flash_if.h"
#include "fsl_flash.h"
#include "fsl_k4_flash.h"
#include "fsl_pm_core.h"
#include "hal_sys.h"
#include "uds_api.h"
#include "app_dtc.h"
#include "logger.h"

/*  Include     End         */

static flash_config_t code_flash_driver = {0};
static unsigned char fls_read_index = 0, fls_read_len = 0;

/**
 * 函 数 介 绍 ：
 *                 dataflash模块进行初始化
 *
 *  * 参       数 ：
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_pfls_init(void)
{
    hal_result_t result = HAL_RESULT_NO_SUPPORT;
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 dataflash模块擦除函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] datalen      擦除的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_pfls_erase(uint8_t index, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 dataflash模块读取函数
 *
 *  * 参       数 ：
 *                  [in] index     通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer    读取数据存放BUFF
 *                  [in] datalen       读取的长度
 * 返   回  值      hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，读取位置不能超出flash大小
 */
hal_result_t hal_pfls_read(uint8_t index, uint8_t *buffer, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 dataflash模块擦写函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        写入的数据
 *                  [in] l_u32_eraselen     擦除的长度
 *                  [in] datalen      写入的长度
 * 返   回  值  hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_pfls_erase_write(uint8_t index, const uint8_t *buffer, uint32_t l_u32_eraselen, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 dataflash模块写函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        写入的数据
 *                  [in] datalen      写入的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_pfls_write(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 dataflash模块对比函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        比较的数据
 *                  [in] datalen      比较的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_flash_compare_et hal_pfls_compare(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_flash_compare_et result = HAL_FLASH_COMPARE_ERROR;
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 dataflash在写之前检查整个flash是否已被清除
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *
 *                  [in] datalen      检查长度
 *
 * 返   回  值 hal_result_t   hal_result_t   =0 :空白检查成功，=-10 :空白检查失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_flash_check_et hal_pfls_blank_check(uint8_t index, uint32_t datalen)
{
    hal_flash_check_et result = HAL_FLASH_CHECK_ERROR;
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 codeflash模块初始化函数
 *
 *  * 参       数 ：

 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
*/
hal_result_t hal_cfls_init(void)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    if(kStatus_FLASH_Success != FLASH_Init(&code_flash_driver))
    {
    	result = HAL_RESULT_FAIL;
    }
    else
    {

    }

    return result;
}

/**
 * 函 数 介 绍 ：
 *                 cflash模块擦除函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *
 *                  [in] datalen      擦除的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_erase(uint8_t index, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    uint32_t len = 0;
	uint32_t *pg_cflash_cfg = NULL;
	hal_cflash_get_cfg(&pg_cflash_cfg,&len);
	uint32_t erase_sector_num = 0;

//	logPrintfImmedi("erase flash, index = %d, eraselen = %d\r\n", index, datalen);

	if((index < len) &&(NULL != pg_cflash_cfg))
	{
		hal_sys_enabled_irq(HAL_ENABLED_DISABLE);
		if(((pg_cflash_cfg[index] + datalen) >= HAL_CODEFLASH_START_ADDR) || ((pg_cflash_cfg[index] + datalen) < HAL_CODEFLASH_END_ADDR))
		{
			if(0 != datalen)
			{
				erase_sector_num = datalen / (HAL_FLASH_SECTOR_SIZE+1) + 1;
				for(uint32_t i = 0; i < erase_sector_num; ++i)
				{
					if((index + i) > HAL_CODEFLASH_ADDR_ARR_LEN)
					{
						break;
					}
					if(kStatus_FLASH_Success == FLASH_Erase(&code_flash_driver, FLASH, pg_cflash_cfg[index+i],HAL_FLASH_SECTOR_SIZE, kFLASH_ApiEraseKey))
					{

					}
					else
					{
						l_i8_result = HAL_RESULT_FAIL;
					}
				}
			}
		}
		else
		{
			l_i8_result = HAL_RESULT_PARAMETER_ERR;
		}
		hal_sys_enabled_irq(HAL_ENABLED_ENABLE);
	}
	else
	{
		l_i8_result = HAL_RESULT_PARAMETER_ERR;
	}

//	logPrintfImmedi("erase flash, l_i8_result = %d\r\n", l_i8_result);

    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 codeflash模块读取函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        读取的数据
 *                  [in] datalen      读取的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，读取位置不能超出flash大小
 */
hal_result_t hal_cfls_read(uint8_t index, uint8_t *buffer, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    uint32_t len = 0;
	uint32_t *pg_cflash_cfg = NULL;
	uint32_t addr = 0;

	fls_read_index = index;
	fls_read_len = datalen;

//	if(UDSAPI_FLASH_INDEX_DTC != index)
//	logPrintfImmedi("read flash, index = %d\r\n", index);

	hal_cflash_get_cfg(&pg_cflash_cfg,&len);
	if((index < len )&&(NULL != pg_cflash_cfg))
	{
		if(((pg_cflash_cfg[index] + datalen) >= HAL_CODEFLASH_START_ADDR) || ((pg_cflash_cfg[index] + datalen) < HAL_CODEFLASH_END_ADDR))
		{
			addr = pg_cflash_cfg[index];
			for(uint32_t i = 0; i < datalen ;++i)
			{
				buffer[i] = *((volatile uint8_t *)addr);
				addr++;
			}
		}
		else
		{
			l_i8_result = HAL_RESULT_PARAMETER_ERR;
		}
	}
	else
	{
		l_i8_result = HAL_RESULT_PARAMETER_ERR;
	}

//	if(UDSAPI_FLASH_INDEX_DTC != index)
//	logPrintfImmedi("read flash complete! l_i8_result = %d, index = %d, buffer[%d] = %02X %02X %02X %02X ...\r\n", l_i8_result, index, datalen, buffer[0], buffer[1], buffer[2], buffer[3]);

	fls_read_index = 0;
	fls_read_len = 0;

    return l_i8_result;
}

void BusFault_Handler(void)
{
#if 0
	unsigned char default_b[128] = {0};
	logPrintf("##################################################################\r\n");
	Uds_Api_fdl_Ereas_Write(126, default_b, 128);
	logPrintf("##################################################################\r\n");
	logPrintf("##################################################################\r\n");
#else
	if(0 != fls_read_index && 0 != fls_read_len)
	{
		logPrintf("read flash error! index = %d, fls_read_len = %d\r\n", fls_read_index, fls_read_len);
		hal_cfls_erase(fls_read_index, fls_read_len);
		logPrintf("##################################################################\r\n");
		logPrintf("##################################################################\r\n");
	}
	else
	{
		logPrintf("System Fault: BusFault_Handler!");
		while(1);
	}
#endif
}

/**
 * 函 数 介 绍 ：
 *                 codeflash模块擦写函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        写入的数据
 *                  [in] l_u32_eraselen     擦除的长度
 *                  [in] datalen      写入的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_erase_write(uint8_t index, const uint8_t *buffer, uint32_t l_u32_eraselen, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    uint32_t len = 0;
	uint32_t * pg_cflash_cfg = NULL;
	uint32_t erase_sector_num = 0;

	logPrintf("write flash, index = %d, eraselen = %d, buffer[%d] = %02X %02X %02X %02X ... \r\n", index, l_u32_eraselen, NULL != buffer?datalen:0, NULL != buffer?buffer[0]:0xFF, NULL != buffer?buffer[1]:0xFF, NULL != buffer?buffer[2]:0xFF, NULL != buffer?buffer[3]:0xFF);

	if(!(Get_PMState() == PMS_NORMAL))
	{
		logPrintf("System voltage is not normal! don't allow erase write falsh!\r\n");
		return HAL_RESULT_LOCK;
	}

	hal_cflash_get_cfg(&pg_cflash_cfg,&len);
	if((len > index) &&(pg_cflash_cfg)&&(NULL != buffer))
	{
		hal_sys_enabled_irq(HAL_ENABLED_DISABLE);
		if(((pg_cflash_cfg[index] + l_u32_eraselen) >= HAL_CODEFLASH_START_ADDR) || ((pg_cflash_cfg[index] + l_u32_eraselen) <= HAL_CODEFLASH_END_ADDR))
		{
			if(0 != l_u32_eraselen)
			{
				erase_sector_num = l_u32_eraselen / (HAL_FLASH_SECTOR_SIZE+1) + 1;
				for(uint8_t i = 0; i < erase_sector_num; ++i)
				{
					if((index + i) > HAL_CODEFLASH_ADDR_ARR_LEN)
					{
						break;
					}
					if(kStatus_FLASH_Success == FLASH_Erase(&code_flash_driver, FLASH, pg_cflash_cfg[index+i],HAL_FLASH_SECTOR_SIZE, kFLASH_ApiEraseKey))
					{

					}
					else
					{
						l_i8_result = HAL_RESULT_FAIL;
					}
				}
				if(kStatus_FLASH_Success == FLASH_Program(&code_flash_driver, FLASH, pg_cflash_cfg[index], (uint8_t *)buffer, datalen))
				{
					l_i8_result = HAL_RESULT_SUCCESS;
				}
				else
				{
					l_i8_result = HAL_RESULT_FAIL;
				}
			}
		}
		else
		{
			l_i8_result = HAL_RESULT_PARAMETER_ERR;
		}
		hal_sys_enabled_irq(HAL_ENABLED_ENABLE);
	}
	else
	{
		l_i8_result = HAL_RESULT_PARAMETER_ERR;
	}

	logPrintf("write flash complete! l_i8_result = %d\r\n", l_i8_result);

    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 codeflash模块写函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        写入的数据
 *                  [in] datalen      写入的长度
 * 返   回  值      hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_write(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    uint32_t len = 0;
	uint32_t * pg_cflash_cfg = NULL;
	hal_cflash_get_cfg(&pg_cflash_cfg,&len);
	if((len > index) &&(pg_cflash_cfg)&&(NULL != buffer))
	{
		hal_sys_enabled_irq(HAL_ENABLED_DISABLE);
		if(((pg_cflash_cfg[index] + datalen) >= HAL_CODEFLASH_START_ADDR) || ((pg_cflash_cfg[index] + datalen) < HAL_CODEFLASH_END_ADDR))
		{
			if(kStatus_FLASH_Success == FLASH_Program(&code_flash_driver, FLASH, pg_cflash_cfg[index], (uint8_t *)buffer, datalen))
			{

			}
			else
			{
				l_i8_result = HAL_RESULT_FAIL;
			}
		}
		else
		{
			l_i8_result = HAL_RESULT_PARAMETER_ERR;
		}
		hal_sys_enabled_irq(HAL_ENABLED_ENABLE);
	}
	else
	{
		l_i8_result = HAL_RESULT_FAIL;
	}
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 cflash模块对比函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        比较的数据
 *                  [in] datalen      比较的长度
 *
 * 返   回  值 ：比较结果
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_flash_compare_et hal_cfls_compare(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_flash_compare_et result = HAL_FLASH_COMPARE_ERROR;
    uint32_t len = 0;
	uint32_t * pg_cflash_cfg = NULL;
	hal_cflash_get_cfg(&pg_cflash_cfg,&len);
	volatile const uint8_t *pbuffer = NULL;
	if((len > index) &&(pg_cflash_cfg)&&(NULL != buffer))
	{
		if(((pg_cflash_cfg[index] + datalen) >= HAL_CODEFLASH_START_ADDR) || ((pg_cflash_cfg[index] + datalen) <= HAL_CODEFLASH_END_ADDR))
		{
			pbuffer = (volatile const uint8_t *)pg_cflash_cfg[index];
			result = HAL_FLASH_COMPARE_SAME;
			for(uint32_t i = 0; i < datalen; ++i)
			{
				if(buffer[i] != pbuffer[i])
				{
					result = HAL_FLASH_COMPARE_DIFF;
					break;
				}
			}
		}
		else
		{
			result = HAL_FLASH_COMPARE_ERROR;
		}
	}
	else
	{
		result = HAL_FLASH_COMPARE_ERROR;
	}
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 codeflash在写之前检查整个flash是否已被清除
 *
 *  * 参       数 ：
 *                  [in] index  要比较的位置（页数）
 *                  [in] datalen    比较的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :空白检查成功，=-10 :空白检查失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_blank_check(uint8_t index, uint32_t datalen)
{
    hal_flash_check_et result = HAL_FLASH_CHECK_ERROR;
    uint32_t len = 0;
	uint32_t * pg_cflash_cfg = NULL;
	hal_cflash_get_cfg(&pg_cflash_cfg,&len);
	volatile const uint8_t *pbuffer = NULL;
	if((len > index) &&(pg_cflash_cfg))
	{
		if(((pg_cflash_cfg[index] + datalen) >= HAL_CODEFLASH_START_ADDR) || ((pg_cflash_cfg[index] + datalen) <= HAL_CODEFLASH_END_ADDR))
		{
			pbuffer = (const uint8_t *)pg_cflash_cfg[index];
			result = HAL_FLASH_CHECK_OPT;
			for(uint32_t i = 0; i < datalen; ++i)
			{
				if(0xff != pbuffer[i])
				{
					result = HAL_FLASH_CHECK_NOT_OPT;
					break;
				}
			}
		}
		else
		{
			result = HAL_FLASH_CHECK_ERROR;
		}
	}
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 nvm模块初始化函数
 *
 *  * 参       数 ：
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_nvm_init(void)
{
	hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

	return l_i8_result;
}



/**
 * 函 数 介 绍 ：
 *                 nvm模块读函数
 *
 *  * 参       数 ：index               通过该索引号去数组（g_u32_nvm_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                 buffer               读取数据缓存指针
 *                 datalen            读取数据长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_nvm_read(uint8_t index, uint8_t *buffer, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 nvm模块写函数
 *
 *  * 参       数 ：index               通过该索引号去数组（g_u32_nvm_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                 buffer              写数据缓存指针
 *                 datalen            写数据长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_nvm_write(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
    return l_i8_result;
}

/* Public Function End              */
