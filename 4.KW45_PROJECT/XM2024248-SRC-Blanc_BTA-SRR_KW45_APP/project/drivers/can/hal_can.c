/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_can.c
 * 摘要：
 *          文件主要描述clock模块
 * 当前版本：
 * 历史版本：
 */

/*  Include Begine  */
#include "hal_can.h"
#include "hal_can_if.h"
#include "clock_config.h"
#include "fsl_flexcan.h"

#define CAN_TX_BMIDX 0
static uint8_t dlc_to_len(uint8_t dlc);

typedef void (* pcan_callback)(CAN_Type * base, flexcan_handle_t * handle, status_t status, uint64_t result, void *userData);
static void can0_callback(CAN_Type * base, flexcan_handle_t * handle, status_t status, uint64_t result, void *userData);
static clock_ip_name_t can_clock_array[HAL_CAN_CHANNEL_NUM] = {kCLOCK_Can0};
static CAN_Type * can_array[HAL_CAN_CHANNEL_NUM] = {CAN0};
static pcan_callback can_cbk_array[HAL_CAN_CHANNEL_NUM] = {can0_callback};
static flexcan_handle_t can_handle_arry[HAL_CAN_CHANNEL_NUM];
static hal_can_recv_cbk_t can_rx_cbk[HAL_CAN_CHANNEL_NUM] = {NULL};
static hal_can_tx_cbk_t can_tx_cbk[HAL_CAN_CHANNEL_NUM] = {NULL};
static hal_can_busoff_cbk_t can_busoff_cbk[HAL_CAN_CHANNEL_NUM] = {NULL};
static flexcan_enhanced_rx_fifo_config_t rxEhFifoConfig[HAL_CAN_CHANNEL_NUM] = {0};
static flexcan_fd_frame_t txframe[HAL_CAN_CHANNEL_NUM] = {0};
static flexcan_mb_transfer_t txXfer[HAL_CAN_CHANNEL_NUM] = {0};
static flexcan_fifo_transfer_t rxFifoXfer[HAL_CAN_CHANNEL_NUM] = {0};
static flexcan_fd_frame_t rxframe[HAL_CAN_CHANNEL_NUM][1] = {0};
static uint32_t rxfifofilter[HAL_CAN_CHANNEL_NUM][HAL_CAN_FILTER0_LEN] = {0};
static uint8_t can_receive_data[HAL_CAN_CHANNEL_NUM][64] = {0};
static hal_can_pdu_st can_receive_pdu[HAL_CAN_CHANNEL_NUM] = {{.m_u8_pdata = can_receive_data[0]}};
extern hal_can_filter_st g_canfilter0_arr_sa[HAL_CAN_FILTER0_LEN];
static uint32_t can_baudrate[HAL_CAN_CHANNEL_NUM][2] = {{CAN0_BAUDRATE,CAN0FD_BAUDRATE}};

//result 对应注册的邮箱
static void can0_callback(CAN_Type * base, flexcan_handle_t * handle, status_t status, uint64_t result, void *userData)
{
	uint8_t i = 0;

	switch (status)
	{
	//接收中断
	case kStatus_FLEXCAN_RxIdle:
		break;
	case kStatus_FLEXCAN_RxFifoIdle:
		if(NULL != can_rx_cbk[0])
		{
			if(HAL_CAN_FILTER0_LEN > rxframe[0][0].idhit)
			{
				if(kFLEXCAN_FrameFormatStandard == rxframe[0][0].format)
				{
					can_receive_pdu[0].m_id_type_e = HAL_CAN_ID_TYPE_StAND;
				}
				else
				{
					can_receive_pdu[0].m_id_type_e = HAL_CAN_ID_TYPE_EXTEND;
				}
				can_receive_pdu[0].m_u32_canid = rxframe[0][0].id >> CAN_ID_STD_SHIFT;
				can_receive_pdu[0].m_u8_dlc = dlc_to_len(rxframe[0][0].length);
				for(i = 0; i <can_receive_pdu[0].m_u8_dlc/4; ++i)
				{
					can_receive_pdu[0].m_u8_pdata[4*i+0] = rxframe[0][0].dataWord[i]>>24;
					can_receive_pdu[0].m_u8_pdata[4*i+1] = (rxframe[0][0].dataWord[i]>>16)&0xff;
					can_receive_pdu[0].m_u8_pdata[4*i+2] = (rxframe[0][0].dataWord[i]>>8)&0xff;
					can_receive_pdu[0].m_u8_pdata[4*i+3] = rxframe[0][0].dataWord[i]&0xff;
				}
				can_rx_cbk[0](&can_receive_pdu[0]);
			}
		}
		FLEXCAN_TransferReceiveEnhancedFifoNonBlocking(can_array[0], &can_handle_arry[0], &rxFifoXfer[0]);
		break;
	//发送完成中断
	case kStatus_FLEXCAN_TxIdle:
		if(NULL != can_tx_cbk[0])
		{
			can_tx_cbk[0](txframe[0].id >> CAN_ID_STD_SHIFT);
		}
		break;
	case kStatus_FLEXCAN_WakeUp:
		break;
	case kStatus_FLEXCAN_ErrorStatus:
			if(NULL != can_busoff_cbk[0])
			{
				can_busoff_cbk[0]();
			}
		break;
	case kStatus_FLEXCAN_TxBusy:
		break;
	default:
		break;
	}
}

static void hal_can_setfifofig(uint8_t channel,hal_can_cfg_st *pcan_cfg)
{
	uint8_t len = 0;
	FLEXCAN_SetFDTxMbConfig(can_array[channel], CAN_TX_BMIDX, true);
	for(len = 0; len < pcan_cfg->m_u16_filtarrlen; ++len)
	{
		rxfifofilter[channel][len] = FLEXCAN_ENHANCED_RX_FIFO_STD_MASK_AND_FILTER(pcan_cfg->m_filter_s[len].m_u32_code,0,pcan_cfg->m_filter_s[len].m_u32_mask,0);
	}
	rxEhFifoConfig[channel].idFilterTable = rxfifofilter[channel];
	rxEhFifoConfig[channel].idFilterPairNum = sizeof(rxfifofilter[channel]) / sizeof(rxfifofilter[channel][0]) / 2;
	rxEhFifoConfig[channel].extendIdFilterNum = 0;
	rxEhFifoConfig[channel].fifoWatermark = 0;
	rxEhFifoConfig[channel].dmaPerReadLength = kFLEXCAN_19WordPerRead;
	rxEhFifoConfig[channel].priority = kFLEXCAN_RxFifoPrioHigh;
	FLEXCAN_SetEnhancedRxFifoConfig(can_array[channel], &rxEhFifoConfig[channel], true);
	rxFifoXfer[channel].framefd = &rxframe[channel][0];
	rxFifoXfer[channel].frameNum = 1;
	FLEXCAN_TransferReceiveEnhancedFifoNonBlocking(can_array[channel],&can_handle_arry[channel],&rxFifoXfer[channel]);
}
/*
 * 函 数 介 绍: CAN初始化接口
 * 参       数 [in]channel      CAN通道号
 *
 *
 * 返   回  值: = 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注: 1、通道号需要对应映射到CAN硬件通道号上，初始化配置在hal_can_cfg.c中g_cancfg_arr_sa去配置。
 *              2、CAN波特率根据采样点和CAN时钟来计算，fCAN由时钟配置那边决定，这里只需配置好tq就行。
 *              举例：NTS2=16; NTS1=63; NSJW=16; NBRP=0，Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/4/5 = 2Mbps
 */

hal_result_t hal_can_init(uint8_t channel)
{
	hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
	hal_result_t result = HAL_RESULT_SUCCESS;
	uint32_t len = 0;
	flexcan_config_t can_config = {0};
	flexcan_timing_config_t timing_config = {0};
	hal_can_cfg_st * pg_cancfg_arr_sa = NULL;
	hal_can_get_cfg(&pg_cancfg_arr_sa,&len);
	if((channel < HAL_CAN_CHANNEL_NUM)&&(NULL != pg_cancfg_arr_sa)&&(channel < len))
	{
		CLOCK_SetIpSrc(can_clock_array[channel], kCLOCK_IpSrcSoscClk);
		CLOCK_SetIpSrcDiv(can_clock_array[channel], kSCG_SysClkDivBy1);
		FLEXCAN_GetDefaultConfig(&can_config);
		can_config.baudRate = can_baudrate[channel][0];
		can_config.baudRateFD = can_baudrate[channel][1];

#ifdef USE_TIMINGCONFIG
		if(FLEXCAN_FDCalculateImprovedTimingValues(can_array[channel], can_config.bitRate, can_config.baudRateFD, CLOCK_GetFreq(kCLOCK_ScgSysOscClk), &timing_config))
		{
			memcpy(&can_config.timingConfig, &timing_config, sizeof(flexcan_timing_config_t));
		}
#else
		can_config.timingConfig.preDivider = pg_cancfg_arr_sa[channel].m_atqstr_s->m_u16_brp;
		can_config.timingConfig.rJumpwidth = pg_cancfg_arr_sa[channel].m_atqstr_s->m_u8_sjw;
		can_config.timingConfig.phaseSeg1 = pg_cancfg_arr_sa[channel].m_atqstr_s->m_u8_seg1;
		can_config.timingConfig.phaseSeg2 = pg_cancfg_arr_sa[channel].m_atqstr_s->m_u8_seg2;
		can_config.timingConfig.propSeg = pg_cancfg_arr_sa[channel].m_atqstr_s->m_u8_procseg;

		can_config.timingConfig.fpreDivider = pg_cancfg_arr_sa[channel].m_dtqstr_s->m_u16_brp;
		can_config.timingConfig.frJumpwidth = pg_cancfg_arr_sa[channel].m_dtqstr_s->m_u8_sjw;
		can_config.timingConfig.fphaseSeg1 = pg_cancfg_arr_sa[channel].m_dtqstr_s->m_u8_seg1;
		can_config.timingConfig.fphaseSeg2 = pg_cancfg_arr_sa[channel].m_dtqstr_s->m_u8_seg2;
		can_config.timingConfig.fpropSeg = pg_cancfg_arr_sa[channel].m_dtqstr_s->m_u8_procseg;
#endif

		FLEXCAN_FDInit(can_array[channel], &can_config, CLOCK_GetFreq(kCLOCK_ScgSysOscClk), kFLEXCAN_64BperMB, true);
		//配置callback
		FLEXCAN_TransferCreateHandle(can_array[channel], &can_handle_arry[channel], can_cbk_array[channel], NULL);
		//配置筛选器
		hal_can_setfifofig(channel,&pg_cancfg_arr_sa[channel]);
	}
	else
	{
			result = HAL_RESULT_PARAMETER_ERR;
	}

    return l_i8_result;
}
#ifdef HAL_CAN_FDFRAME
/*
 * 函 数 介 绍: CANFD设置TDC
 * 参       数  [in]channel    CAN通道号
 *              [in]TDC_offset TDC偏移量，具体寄存器需要根据芯片手册配置}
 *              [in]TDC_enable TDC使能标签，这里定义0:Disable,1:enable;具体寄存器配置需要根据芯片手册配置
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:
 */
hal_result_t hal_can_fd_set_tdc(uint8_t channel, uint8_t tdc_offset, hal_enabled_et tdc_enabled)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
#endif
/*
 * 函 数 介 绍: 配置仲裁域tq
 * 参       数
 *              [in]channel         CAN通道号
 *
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:                 1、波特率根据传入的tq配置来计算
 *                              举例：NTS2=16; NTS1=63; NSJW=16; NBRP=0，Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/4/5 = 2Mbps
 *                              2、仲裁域tq可通过hal_can_cfg.h中g_canatq_arr_sa结构体数据进行配置，实现过程中直接使用该结构体内容即可
 */

hal_result_t hal_can_set_arbitratetq(uint8_t channel)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: 配置数据域tq
 * 参       数
 *              [in]channel           CAN通道号
 *
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:                 1、波特率根据传入的tq配置来计算
 *                              举例：NTS2=16; NTS1=63; NSJW=16; NBRP=0，Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/4/5 = 2Mbps
 *                              2、仲裁域tq可通过hal_can_cfg.h中g_candtq_arr_sa结构体数据进行配置，实现过程中直接使用该结构体内容即可
 */
hal_result_t hal_can_set_datatq(uint8_t channel, hal_can_tq_st *can_dtq)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: CAN过滤配置
 * 参       数
 *              [in]channel            CAN通道号
 *              [in]can_filter_arr     过滤数组指针，详情请参考hal_can_filter_st
 *              [in]filterarr_len   过滤数组长度,由宏定义HAL_CAN_FILTER[n]_LEN决定，在hal_can_cfg.h中定义
 *                                   n与CAN通道号保持一致，举例CAN0,n=0}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:每个CAN通道号对应一个hal_can_filter_st数组和数组长度宏定义，传参的时候注意对应起来
 */
hal_result_t hal_can_filter_config(uint8_t channel, hal_can_filter_st *can_filter_arr, uint16_t filterarr_len)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: CAN 发送接口
 * 参       数
 *             [in]channel              CAN通道号
 *             [in]can_trans_pdu        CAN发送结构体，怎样填充详见hal_can_pdu_st}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 *
 * 备       注:
 */
static uint8_t  CAN_FD_DLC_8[8u] = {8u,9u,10u,11u,12u,13u,14u,15u};
static uint8_t  CAN_FD_DLC_64[8u] = {8u,12u,16u,20u,24u,32u,48u,64u};
hal_result_t hal_can_write(uint8_t channel, hal_can_pdu_st *can_trans_pdu)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    uint8_t i = 0;
    uint8_t dlc = 0;

    uint32_t len = 0;
    hal_can_cfg_st * pg_cancfg_arr_sa = NULL;
    hal_can_get_cfg(&pg_cancfg_arr_sa,&len);
    if((NULL != can_trans_pdu)&&(channel < len)&&(NULL != pg_cancfg_arr_sa))
    {
    	txframe[channel].id =FLEXCAN_ID_STD(can_trans_pdu->m_u32_canid);
    	if(HAL_CAN_ID_TYPE_EXTEND == can_trans_pdu->m_id_type_e)
    	{
    		txframe[channel].format = (uint8_t)kFLEXCAN_FrameFormatExtend;
    	}
    	else
    	{
    		txframe[channel].format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    	}
    	txframe[channel].type = (uint8_t)kFLEXCAN_FrameTypeData;

		for (i = 0; i < 8; i++)
		{
			if (can_trans_pdu->m_u8_dlc <= CAN_FD_DLC_64[i])
			{
				dlc = CAN_FD_DLC_8[i];
				break;
			}
		}
//    	 for ( i = 0; i < can_trans_pdu->m_u8_dlc/4; i++)
//		 {
//    		 txframe.dataWord[i] = (can_trans_pdu->m_u8_pdata[i*4 + 3]<<24) + (can_trans_pdu->m_u8_pdata[i*4 + 2]<<16) +
//								 (can_trans_pdu->m_u8_pdata[i*4 + 1]<<8) + (can_trans_pdu->m_u8_pdata[i*4 + 0]);
//		 }
		 for ( i = 0; i < can_trans_pdu->m_u8_dlc/4; i++)
		 {
			 txframe[channel].dataWord[i] = (can_trans_pdu->m_u8_pdata[i*4 + 0]<<24) + (can_trans_pdu->m_u8_pdata[i*4 + 1]<<16) +
								 (can_trans_pdu->m_u8_pdata[i*4 + 2]<<8) + (can_trans_pdu->m_u8_pdata[i*4 + 3]);
		 }
    	 txframe[channel].length = dlc;
    	 txframe[channel].brs = 1;
    	 txframe[channel].edl = 1;

    	txXfer[channel].mbIdx = CAN_TX_BMIDX;
    	txXfer[channel].framefd = &txframe[channel];
    	status_t status = FLEXCAN_TransferFDSendNonBlocking(can_array[channel], &can_handle_arry[channel], &txXfer[channel]);
    	if(kStatus_Success == status)
    	{
    		l_i8_result = HAL_RESULT_SUCCESS;
    	}
    	else if(kStatus_FLEXCAN_TxBusy == status)
    	{
    		l_i8_result = HAL_RESULT_BUSY;
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

    return l_i8_result;
}
/*
 * 函 数 介 绍: CAN接收接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]can_recv_pdu        CAN接收结构体，怎样填充详见hal_can_pdu_st}}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:
 */
hal_result_t hal_can_read(uint8_t channel, hal_can_pdu_st *can_recv_pdu)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: CAN接收回调接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]recv_func           回调函数，传参参考hal_can_recv_cbk_t[hal_can_typedef.h]}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: 该接口需要在CAN接收地方调用
 */
hal_result_t hal_can_recv_callback(uint8_t channel, hal_can_recv_cbk_t recv_func)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    uint32_t len = 0;
    hal_can_cfg_st * pg_cancfg_arr_sa = NULL;
    hal_can_get_cfg(&pg_cancfg_arr_sa,&len);
    if((NULL != pg_cancfg_arr_sa) && (channel < len ))
    {
    	can_rx_cbk[channel] = recv_func;
        l_i8_result = HAL_RESULT_SUCCESS;
    }
    else
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    return l_i8_result;
}
/*
 * 函 数 介 绍: busoff回调函数配置
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]busoff_func         busoff回调函数
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: busoff触发后调用该函数
 */
hal_result_t hal_can_busoff_callback(uint8_t channel, hal_can_busoff_cbk_t busoff_func)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    uint32_t len = 0;
    hal_can_cfg_st * pg_cancfg_arr_sa = NULL;
    hal_can_get_cfg(&pg_cancfg_arr_sa,&len);
    if((NULL != pg_cancfg_arr_sa) && (channel < len ))
    {
    	can_busoff_cbk[channel] = busoff_func;
    	l_i8_result = HAL_RESULT_SUCCESS;
    }
    else
    {
      l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    return l_i8_result;
}
/*
 * 函 数 介 绍: 配置传输模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]trcvmode            传输模式,详见hal_can_trcvmode_et
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_set_trcv_mode(uint8_t channel, hal_can_trcvmode_et trcvmode)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: 获取CAN传输模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]trcvmode            CAN传输模式指针}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_get_trcvmode(uint8_t channel, hal_can_trcvmode_et *trcvmode)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: 配置CAN控制模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]ctrl_mode           控制模式，详见hal_can_ctrmode_et
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: 实际修改对应寄存器的值还请对照芯片数据手册来配置
 */
hal_result_t hal_can_set_ctrlmode(uint8_t channel, hal_can_ctrmode_et ctrlmode)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: 获取CAN控制模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]ctrmode             CAN控制模式指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_get_ctrlmode(uint8_t channel, hal_can_ctrmode_et *ctrmode)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: 获取CAN发送错误计数器计数
 * 参       数
 *              [in]channel          CAN通道号
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_get_tec(uint8_t channel, uint8_t *tec)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: 获取CAN接收错误计数器计数
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]p_u8_rec            获取CAN接收错误计数器的指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_get_rec(uint8_t channel, uint8_t *rec)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}


static uint8_t dlc_to_len(uint8_t dlc)
{
	uint8_t ret;
	uint8_t j = (dlc & 7);
    if(dlc < 8)
    {
        return dlc;
    }
	if (dlc > 12)
    {
        ret = 12 * (j - 4) + 4 * j;
    }
	else
    {
        ret = 8 + 4 * j;
    }
	return ret;
}
/*
 * 函 数 介 绍: 配置CAN发送完成回调函数
 * 参       数
 *              [in]channel          CAN通道号
 *              [in]tx_cbk           回调函数指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_tx_callback(uint8_t channel, hal_can_tx_cbk_t tx_cbk)
{
	hal_result_t result = HAL_RESULT_SUCCESS;
	if(channel < HAL_CAN_CHANNEL_NUM)
	{
		can_tx_cbk[channel] = tx_cbk;
	}
	else
	{
		result = HAL_RESULT_PARAMETER_ERR;
	}
	return result;
}
/*
 * 函 数 介 绍: CAN去初始化函数
 * 参       数
 *
 * 返   回  值:
 * 备       注:
 */
void hal_can_deinit(void)
{
	FLEXCAN_Deinit(CAN0);
}
