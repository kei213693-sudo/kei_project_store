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
static lpspi_pin_config_t const spi_pin_array[HAL_SPI_CHANNEL_NUM] = {kLPSPI_SdoInSdiOut,kLPSPI_SdiInSdoOut};
static clock_ip_name_t const spi_clock_array[HAL_SPI_CHANNEL_NUM] = {kCLOCK_Lpspi0,kCLOCK_Lpspi1};
static enum _lpspi_transfer_config_flag_for_master const transfer_array[4] = {kLPSPI_MasterPcs0,kLPSPI_MasterPcs1,kLPSPI_MasterPcs2,kLPSPI_MasterPcs3};
static enum _lpspi_transfer_config_flag_for_master const spi_transfer_array[HAL_SPI_CHANNEL_NUM] = {kLPSPI_MasterPcs0,kLPSPI_MasterPcs0};


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
/**
 * 函 数 介 绍 ：
 *                 spi初始化
 *
 *  * 参       数
 *                 channel：spi通道
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
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
			spi_config.pinCfg = spi_pin_array[channel];
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
 * 函 数 介 绍 ：
 *                 spi通信速率设置
 *
 *  * 参       数
 *                 channel：spi通道
 * 					clockhz：设置的速率
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_set_clock(uint8_t channel, uint32_t clockhz)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 spi采样模式设置
 *
 *  * 参       数
 *                 channel：spi通道
 * 					sampmode：采样模式
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_set_sampmode(uint8_t channel, hal_spi_mode_et sampmode)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 spi片选电平设置
 *
 *  * 参       数
 *              channel：spi通道
 *              level： 电平值
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_cs_levle(uint8_t channel, uint8_t level)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 spi发送数据函数
 *
 *  * 参       数
 *                 channel：spi通道
 *                  sendbuff：发送缓冲区
 *                  recvbuff：接收缓冲区
 *                  len：数据长度
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_spi_result_et hal_spi_write(uint8_t channel, uint8_t *sendbuff, uint8_t *recvbuff, uint16_t len)
{
    hal_spi_result_et result = HAL_SPI_RESULT_SUCCESS;
    lpspi_transfer_t spi_transfer = {0};
	hal_spi_cfg_st *pg_spi_cfg_sa = NULL;
	uint32_t plen = 0;
	uint16_t i = 0;
	//uint32_t lene = len*8;

	hal_spi_get_cfg(&pg_spi_cfg_sa,&plen);

    if ((channel < HAL_SPI_CHANNEL_NUM)&&(NULL != pg_spi_cfg_sa))
    {
    	spi_transfer.txData = (uint8_t *)sendbuff;
    	spi_transfer.rxData = recvbuff;
    	spi_transfer.dataSize = len;
    	spi_transfer.configFlags = spi_transfer_array[channel] | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
    	//LPSPI_SetFrameSize(spi_array[channel], lene);   /* 设置发送数据的长度 */
    	 status_t ret = LPSPI_MasterTransferBlocking(spi_array[channel],&spi_transfer);
    	 if(kStatus_Success != ret)
		 {
				 result = HAL_RESULT_FAIL;
		 }
    	 else
    	 {
    		 while ((LPSPI_GetStatusFlags(spi_array[channel]) & (uint32_t)kLPSPI_TransferCompleteFlag) == 0U);
    	 }
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 spi使能函数
 *
 *  * 参       数
 *                 channel：spi通道
 *                  enabled：使能状态
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_enabled(uint8_t channel, hal_enabled_et enabled)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 spi主从模式设置
 *
 *  * 参       数
 *                 channel：spi通道
 *                  role_e：spi模式
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_set_role(uint8_t channel, hal_spi_role_et role_e)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 获取spi模式
 *
 *  * 参       数
 *                 channel：spi通道
 * 返   回  值 ：0：主模式；1：从模式
 * 备       注 ：
 *
 */
hal_spi_role_et hal_spi_get_role(uint8_t channel)
{
    // to be add user code
	hal_spi_role_et spi_role = HAL_RESULT_NO_SUPPORT;

	return spi_role;
}

/**
 * 函 数 介 绍 ：
 *                 SPI发送字节序设置
 *
 *  * 参       数
 *                 channel：spi通道
 *                  data_order：0：lsb；1：msb
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_set_order(uint8_t channel, hal_bit_order_et data_order)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 spi停止发送数据
 *
 *  * 参       数
 *                 channel：spi通道
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_write_abort(uint8_t channel)
{
    // to be add user code
	hal_result_t result = HAL_RESULT_NO_SUPPORT;

	return result;
}
/**
 * 函 数 介 绍 ：
 *                 spi中断配置
 *
 *  * 参       数
 *                 channel：spi通道
 *                  isr_type:中断类型
 *                  enabled：使能状态
 * 返   回  值 ：0：成功；其他值：失败
 * 备       注 ：
 *
 */
hal_result_t hal_spi_isr_config(uint8_t channel, uint16_t isr_type, hal_enabled_et enabled)
{
    hal_result_t result = HAL_RESULT_NO_SUPPORT;

    return result;
}
/**
 * 函 数 介 绍 ：
 *                 spi去初始化
 *
 *  * 参       数
 *
 * 返   回  值 ：
 * 备       注 ：
 *
 */
void hal_spi_deinit(void)
{
	//LPSPI_Deinit(LPSPI0);
	LPSPI_Deinit(LPSPI1);
}

/* Public Function End              */
