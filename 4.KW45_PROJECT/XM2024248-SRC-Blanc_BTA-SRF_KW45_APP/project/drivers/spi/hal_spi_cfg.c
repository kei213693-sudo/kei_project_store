//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_spi.c
 * 摘要：
 *      文件概述：
 *                  文件主要描述xx模块配置实现，本文件为模板文件，
 *              实现实体需要自行补充。
 *      文件调用：
 *                  文件应包含hal_spi_cfg.h。
 *
 * 当前版本：
 * 历史版本：
 */

/*  Include     Begine      */
#include "hal_spi_cfg.h"
/*  Include     End         */


/*SPI初始化数据配置结构体数组定义*/
static hal_spi_cfg_st g_spi_cfg_sa[HAL_SPI_CHANNEL_NUM] =
{
    {
	 HAL_SPI_MODE_CPOL1_CPHA0,
	 HAL_SPI_ROLE_MASTER,
	 HAL_BIT_ORDER_MSB,
	 HAL_ENABLED_ENABLE,
	 HAL_SPI_DATASIZE,
	 0,
	 HAL_SP_CLOCKHZ
	},
	{
	HAL_SPI_MODE_CPOL1_CPHA0,
	 HAL_SPI_ROLE_MASTER,
	 HAL_BIT_ORDER_MSB,
	 HAL_ENABLED_ENABLE,
	 HAL_SPI_DATASIZE,
	 0U,
	 1000000U
	}
};
/**
 * 函 数 介 绍 ：
 *                 获得SPI配置表函数
 *
 *  * 参       数
 *                 spicfg：配置表指针
 * 					len：：长度
 * 返   回  值
 * 备       注 ：
 *
 */
void hal_spi_get_cfg(hal_spi_cfg_st **spicfg, uint32_t *len)
{
	*len = HAL_SPI_CHANNEL_NUM;
	*spicfg = g_spi_cfg_sa;
}

/* Public Function End              */
