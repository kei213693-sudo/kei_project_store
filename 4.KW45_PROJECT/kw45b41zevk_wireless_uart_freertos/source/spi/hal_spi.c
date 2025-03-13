//copy by cc
/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_spi.c
 * 摘要：
 *      文件概述：
 *                  文件主要描述spi模块功能实现，需要保证接口函数功能实现，本文件为模板文件，
 *              实现实体需要自行补充。
 *      文件调用：
 *                  文件应包含hal_spi_if.h hal_spi.h hal_spi_cfg.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

/*  Include     Begine      */
#include "hal_spi.h"
#include "hal_spi_cfg.h"
#include "hal_spi_if.h"
#include "fsl_lpspi.h"
#include "fsl_clock.h"
/*  Include     End         */

//kLPSPI_Pcs0 kLPSPI_Pcs1 kLPSPI_Pcs2 kLPSPI_Pcs3

#define SPI0_PCSn	kLPSPI_Pcs0
#define SPI1_PCSn	kLPSPI_Pcs0

static LPSPI_Type * const spi_array[HAL_SPI_CHANNEL_NUM] = {LPSPI0,LPSPI1};
static lpspi_which_pcs_t const spi_pcs_array[HAL_SPI_CHANNEL_NUM] = {kLPSPI_Pcs0,kLPSPI_Pcs0};
static clock_ip_name_t const spi_clock_array[HAL_SPI_CHANNEL_NUM] = {kCLOCK_Lpspi0,kCLOCK_Lpspi1};
static enum _lpspi_transfer_config_flag_for_master const transfer_array[4] = {kLPSPI_MasterPcs0,kLPSPI_MasterPcs1,kLPSPI_MasterPcs2,kLPSPI_MasterPcs3};
static enum _lpspi_transfer_config_flag_for_master const spi_transfer_array[HAL_SPI_CHANNEL_NUM] = {transfer_array[SPI0_PCSn],transfer_array[SPI1_PCSn]};
/**
* @brief  ：
 *                 spi初始化
 *
 *     @param ：
 *                  @param [in] channel          spi通道，模块自行定义通道及其映射关系
 *
 *
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：g_spi_cfg_sa初始化配置结构体,配置主从模式，采样方式等，详见hal_spi_cfg_st[hal_spi_cfg.h]
 *
 */
static void hal_spi_mode(lpspi_master_config_t *spi_config,hal_spi_mode_et spi_mode)
{
	switch(spi_mode)
	{
	case HAL_SPI_MODE_CPOL0_CPHA0:
		spi_config->cpol = kLPSPI_ClockPolarityActiveHigh;
		spi_config->cpha = kLPSPI_ClockPhaseFirstEdge;
		break;
	case HAL_SPI_MODE_CPOL0_CPHA1:
		spi_config->cpol = kLPSPI_ClockPolarityActiveHigh;
		spi_config->cpha = kLPSPI_ClockPhaseSecondEdge;
		break;
	case HAL_SPI_MODE_CPOL1_CPHA0:
		spi_config->cpol = kLPSPI_ClockPolarityActiveLow;
		spi_config->cpha = kLPSPI_ClockPhaseFirstEdge;
		break;
	case HAL_SPI_MODE_CPOL1_CPHA1:
		spi_config->cpol = kLPSPI_ClockPolarityActiveLow;
		spi_config->cpha = kLPSPI_ClockPhaseSecondEdge;
		break;
	default:
		spi_config->cpol = kLPSPI_ClockPolarityActiveHigh;
		spi_config->cpha = kLPSPI_ClockPhaseFirstEdge;
		break;
	}
}
static void hal_spi_bitorder(lpspi_master_config_t *spi_config,hal_bit_order_et bit_order)
{
	switch(bit_order)
	{
	case HAL_BIT_ORDER_LSB:
		spi_config->direction = kLPSPI_LsbFirst;
		break;
	case HAL_BIT_ORDER_MSB:
		spi_config->direction = kLPSPI_MsbFirst;
		break;
	default:
		spi_config->direction = kLPSPI_MsbFirst;
		break;
	}
}
static void hal_spi_pcs(lpspi_master_config_t * spi_config,uint8_t level)
{
	switch(level)
	{
	case 0:
		spi_config->pcsActiveHighOrLow = kLPSPI_PcsActiveLow;
		break;
	case 1:
		spi_config->pcsActiveHighOrLow = kLPSPI_PcsActiveHigh;
		break;
	default:
		spi_config->pcsActiveHighOrLow = kLPSPI_PcsActiveLow;
		break;
	}
}
hal_result_t hal_spi_init(uint8_t channel)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
	hal_spi_cfg_st *pg_spi_cfg_sa = NULL;
	lpspi_master_config_t spi_config = {0};
	uint32_t len = 0;
	hal_spi_get_cfg(&pg_spi_cfg_sa,&len);
    if ((channel < HAL_SPI_CHANNEL_NUM)&&(NULL != pg_spi_cfg_sa))
    {
    	if(HAL_ENABLED_ENABLE == pg_spi_cfg_sa[channel].m_enabled_e)
    	{
    		CLOCK_SetIpSrc(spi_clock_array[channel], kCLOCK_IpSrcFro192M);
    		CLOCK_SetIpSrcDiv(spi_clock_array[channel], kSCG_SysClkDivBy16);
			LPSPI_MasterGetDefaultConfig(&spi_config);
			spi_config.baudRate = pg_spi_cfg_sa[channel].m_u32_spi_clockhz;
			spi_config.bitsPerFrame = pg_spi_cfg_sa[channel].m_u8_datasize;
			spi_config.whichPcs = spi_pcs_array[channel];
			spi_config.pinCfg = kLPSPI_SdiInSdoOut;
			spi_config.dataOutConfig = kLpspiDataOutRetained;
			hal_spi_mode(&spi_config, pg_spi_cfg_sa[channel].m_spi_mode_e);
			hal_spi_bitorder(&spi_config, pg_spi_cfg_sa[channel].m_spi_data_order_e);
			hal_spi_pcs(&spi_config, pg_spi_cfg_sa[channel].m_u8_cs_level);
			LPSPI_MasterInit(spi_array[channel], &spi_config, CLOCK_GetIpFreq(spi_clock_array[channel]));
    	}
    	else
    	{
    		LPSPI_Reset(spi_array[channel]);
    	}
	}
    else
    {
      l_i8_result = HAL_RESULT_PARAMETER_ERR;  
    }
    return l_i8_result;
}

/**
* @brief  ：
 *                 SPI时钟配置
 *
 *     @param ：
 *                  @param [in] channel            SPI通道，模块自行定义通道及其映射关系
 *                  @param [in] clockhz            指定时钟频率
 * @return ：  hal_result_t   * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_set_clock(uint8_t channel, uint32_t clockhz)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
//	hal_spi_cfg_st *pg_spi_cfg_sa = NULL;
//	uint32_t len = 0;
//	hal_spi_get_cfg(&pg_spi_cfg_sa,&len);
//    if ((0 < len)&&(NULL != pg_spi_cfg_sa)&&(clockhz > 0))
//    {
//
//    }
//    else
//    {
//        l_i8_result = HAL_RESULT_PARAMETER_ERR;
//    }
    return l_i8_result;
}

/**
* @brief  ：
 *                 设置SPI数据采样方式
 *
 *     @param ：
 *                  @param [in] channel          SPI通道，模块自行定义通道及其映射关系
 *                  [in] sampmode         设置采样模式
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_set_sampmode(uint8_t channel, hal_spi_mode_et sampmode)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
//	hal_spi_cfg_st *pg_spi_cfg_sa = NULL;
//	uint32_t len = 0;
//	hal_spi_get_cfg(&pg_spi_cfg_sa,&len);
//    if ((0 < len)&&(NULL != pg_spi_cfg_sa))
//    {
//
//	}
//	else
//	{
//		l_i8_result = HAL_RESULT_PARAMETER_ERR;
//	}
    return l_i8_result;
}

/**
* @brief  ：
 *                 设置片选信息
 *
 *     @param ：
 *                  @param [in] channel          SPI通道，模块自行定义通道及其映射关系
 *                  [in] level         指定片选脚是高有效还是低有效
 *
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_cs_levle(uint8_t channel, uint8_t level)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
//    if (channel >= HAL_SPI_CHL_MAX)
//    {
//        l_i8_result = HAL_RESULT_PARAMETER_ERR;
//    }
//    else
//    {
//        /* 设置片选信息*/
//    }
    return l_i8_result;
}

/**
* @brief  ：
 *                 spi cs level
 *
 *     @param ：
 *                  @param [in] channel    lin通道，模块自行定义通道及其映射关系
 *                  [in] mode
 * @return ：hal_spi_result_et 0:FAIL 1:SUCCESS 2:SLAVE_WAIT_CLOCK
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_spi_result_et hal_spi_write(uint8_t channel, uint8_t *sendbuff, uint8_t *recvbuff, uint16_t len)
{
    hal_spi_result_et result = HAL_SPI_RESULT_SUCCESS;
    lpspi_transfer_t spi_transfer = {0};
	hal_spi_cfg_st *pg_spi_cfg_sa = NULL;
	uint32_t plen = 0;
	uint16_t i = 0;
	hal_spi_get_cfg(&pg_spi_cfg_sa,&plen);
    if ((channel < HAL_SPI_CHANNEL_NUM)&&(NULL != pg_spi_cfg_sa))
    {
    	spi_transfer.txData = (uint8_t *)sendbuff;
    	spi_transfer.rxData = recvbuff;
    	spi_transfer.dataSize = len;
    	spi_transfer.configFlags = spi_transfer_array[channel] | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
    	LPSPI_MasterTransferBlocking(spi_array[channel],&spi_transfer);
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
    return result;
}

/**
* @brief  ：
 *                 spi模块使能
 *
 *     @param ：
 *                  @param [in] channel    spi通道，模块自行定义通道及其映射关系
 *                  @param [in] enabled    是否使能
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_enabled(uint8_t channel, hal_enabled_et enabled)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
//	hal_spi_cfg_st *pg_spi_cfg_sa = NULL;
//	uint32_t len = 0;
//	hal_spi_get_cfg(&pg_spi_cfg_sa,&len);
//    if ((0 < len)&&(NULL != pg_spi_cfg_sa))
//    {
//
//	}
//	else
//	{
//
//	}
    return l_i8_result;
}

/**
* @brief  ：
 *                 spi设置主从
 *
 *     @param ：
 *                  @param [in] channel    SPI通道，模块自行定义通道及其映射关系
 *                  [in] role_e     主从机模式
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_set_role(uint8_t channel, hal_spi_role_et role_e)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
//	hal_spi_cfg_st *pg_spi_cfg_sa = NULL;
//	uint32_t len = 0;
//	hal_spi_get_cfg(&pg_spi_cfg_sa,&len);
//    if ((0 < len)&&(NULL != pg_spi_cfg_sa))
//    {
//
//	}
//    else
//    {
//      l_i8_result = HAL_RESULT_PARAMETER_ERR;
//    }
    return l_i8_result;
}

/**
* @brief  ：
 *                 spi获取当前角色
 *
 *     @param ：
 *                  @param [in] channel    SPI通道，模块自行定义通道及其映射关系
 *
 * @return ：hal_spi_role_et = HAL_SPI_ROLE_MASTER:主机模式， = HAL_SPI_ROLE_SLAVE ：从机模式
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_spi_role_et hal_spi_get_role(uint8_t channel)
{
    // to be add user code
	hal_spi_role_et spi_role = HAL_RESULT_NO_SUPPORT;
//	hal_spi_cfg_st *pg_spi_cfg_sa = NULL;
//	uint32_t len = 0;
//	hal_spi_get_cfg(&pg_spi_cfg_sa,&len);
//	if ((0 < len)&&(NULL != pg_spi_cfg_sa))
//	{
//
//	}
//	else
//	{
//
//	}
	return spi_role;
}

/**
* @brief  ：
 *                 设置发数据时高位先发还是低位先发
 *
 *     @param ：
 *                  @param [in] channel        SPI通道，模块自行定义通道及其映射关系
 *                  [in] data_order     高位先发，还是低位先发
  * @return hal_result_t   @ref           HAL_RESULT_SUCCESS  
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_set_order(uint8_t channel, hal_bit_order_et data_order)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;
//    if (channel >= HAL_SPI_CHL_MAX)
//    {
//        l_i8_result = HAL_RESULT_PARAMETER_ERR;
//    }
//    else
//    {
//        /* 配置发数据时高位先发还是低位先发*/
//    }
    return l_i8_result;
}

/**
* @brief  ：
 *                 spi停止写数据
 *
 *     @param ：
 *                  @param [in] channel    SPI通道，模块自行定义通道及其映射关系
 *
 * @return ：hal_spi_result_et 0:FAIL 1:SUCCESS 2:SLAVE_WAIT_CLOCK
 * @note ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_spi_write_abort(uint8_t channel)
{
    // to be add user code
	hal_result_t result = HAL_RESULT_NO_SUPPORT;

	return result;
}


/* Public Function End              */
