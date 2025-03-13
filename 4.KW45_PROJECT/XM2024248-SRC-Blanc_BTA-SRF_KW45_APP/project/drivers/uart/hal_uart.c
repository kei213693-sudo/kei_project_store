/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_uart.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_uart.h"
#include "fsl_lpuart.h"

static LPUART_Type * const uart_array[HAL_UART_CHANNEL_NUM] = {LPUART0,LPUART1};
static IRQn_Type const uart_irq_array[HAL_UART_CHANNEL_NUM] = {LPUART0_IRQn,LPUART1_IRQn};
static clock_ip_name_t const uart_clock_arrayp[HAL_UART_CHANNEL_NUM] = {kCLOCK_Lpuart0,kCLOCK_Lpuart1};
static hal_uart_rxcbk uart_rx_cbk_array[HAL_UART_CHANNEL_NUM] = {0};

void LPUART0_IRQHandler(void)
{
	uint8_t data = 0;
	if((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(LPUART0))
	{
		data = LPUART_ReadByte(LPUART0);
		if(NULL != uart_rx_cbk_array[0])
		{
			uart_rx_cbk_array[0](&data,1);
		}
		else
		{

		}
	}
	else
	{

	}
}

void LPUART1_IRQHandler(void)
{
	uint8_t data = 0;
	if((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(LPUART1))
	{
		data = LPUART_ReadByte(LPUART1);
		if(NULL != uart_rx_cbk_array[1])
		{
			uart_rx_cbk_array[1](&data,1);
		}
		else
		{

		}
	}
	else
	{

	}
}

static void hal_uart_baudrate(lpuart_config_t *uart_config,hal_uart_baudrate_et baudrate)
{
	switch(baudrate)
	{
	case HAL_UART_BAUDRATE_9600:
		uart_config->baudRate_Bps = 9600;
		break;
	case HAL_UART_BAUDRATE_115200:
		uart_config->baudRate_Bps = 115200;
		break;
	default:
		uart_config->baudRate_Bps = 9600;
		break;

	}
}

static void hal_uart_parity(lpuart_config_t *uart_config,hal_uart_parity_et parity)
{
	switch(parity)
	{
	case HAL_UART_PARITY_NO:
		uart_config->parityMode = kLPUART_ParityDisabled;
		break;
	case HAL_UART_PARITY_EVEN:
		uart_config->parityMode = kLPUART_ParityEven;
		break;
	case HAL_UART_PARITY_ODD:
		uart_config->parityMode = kLPUART_ParityOdd;
		break;
	default:
		uart_config->parityMode = kLPUART_ParityDisabled;
		break;
	}
}

static void hal_uart_mode(lpuart_config_t *uart_config,uint8_t uart_mode)
{
	switch(uart_mode)
	{
	case HAL_UART_MODE_RX:
		uart_config->enableRx = true;
		uart_config->enableTx = false;
		break;
	case HAL_UART_MODE_TX:
		uart_config->enableRx = false;
		uart_config->enableTx = true;
		break;
	case (HAL_UART_MODE_RX|HAL_UART_MODE_TX):
		uart_config->enableRx = true;
		uart_config->enableTx = true;
		break;
	default:
		uart_config->enableRx = true;
		uart_config->enableTx = true;
		break;
	}
}

static void hal_uart_bitorder(lpuart_config_t *uart_config,hal_bit_order_et bit_order)
{
	switch(bit_order)
	{
	case HAL_BIT_ORDER_LSB:
		uart_config->isMsb = false;
		break;
	case HAL_BIT_ORDER_MSB:
		uart_config->isMsb = true;
		break;
	default:
		uart_config->isMsb = false;
		break;
	}
}
/*
 * 函 数 介 绍: UART初始化，设置串口的波特率、数据位、停止位、校验位等参数,
 * 参       数
 *              [in]channel                         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注: 初始化配置调用g_uart_cfg_sa结构体进行配置
 */
hal_result_t hal_uart_init(uint8_t channel)
{
    hal_result_t l_i8_result = HAL_RESULT_FAIL;
    hal_uart_cfg_st *pg_uart_cfg_sa = NULL;
    uint32_t len = 0;
    lpuart_config_t uart_config = {0};
    if(channel >= HAL_UART_CHANNEL_NUM)
    {
    	l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else
    {
    	hal_uart_get_cfg(&pg_uart_cfg_sa, &len);
    	if((NULL != pg_uart_cfg_sa)&&(channel < HAL_UART_CHANNEL_NUM)&&(HAL_ENABLED_ENABLE == pg_uart_cfg_sa[channel].m_enable_e))
		{
    		CLOCK_SetIpSrc(uart_clock_arrayp[channel], kCLOCK_IpSrcFro6M);
    		LPUART_GetDefaultConfig(&uart_config);
    		hal_uart_baudrate(&uart_config, pg_uart_cfg_sa[channel].m_baundrate_e);
    		hal_uart_parity(&uart_config, pg_uart_cfg_sa[channel].m_u8_parity);
    		hal_uart_mode(&uart_config, pg_uart_cfg_sa[channel].m_u8_uart_mode);
    		hal_uart_bitorder(&uart_config, pg_uart_cfg_sa[channel].m_bit_order_e);
    		LPUART_Init(uart_array[channel], &uart_config, CLOCK_GetFreq(kCLOCK_ScgSircClk));
    		//LPUART_EnableInterrupts(uart_array[channel], kLPUART_RxDataRegFullInterruptEnable);
    		EnableIRQ(uart_irq_array[channel]);
    		l_i8_result = HAL_RESULT_SUCCESS;
		}
    	else
    	{
    		l_i8_result = HAL_RESULT_PARAMETER_ERR;
    	}
    }
    return l_i8_result;
}
/*
 * 函 数 介 绍:UART功能使能
 * 参       数 {
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]enabled         串口使能标签}
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */

hal_result_t hal_uart_enabled(uint8_t channel, hal_enabled_et enabled)
{
    hal_result_t l_i8_result = HAL_RESULT_FAIL;
	hal_uart_cfg_st *pg_uart_cfg_sa = NULL;
	uint32_t len = 0;
    if (channel >= HAL_UART_CHANNEL_NUM)
    {
        l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    else
    {
    	hal_uart_get_cfg(&pg_uart_cfg_sa, &len);
		if((NULL != pg_uart_cfg_sa)&&(channel < HAL_UART_CHANNEL_NUM))
		{
			if(HAL_ENABLED_ENABLE == enabled)
			{
				hal_uart_init(channel);
			}
			else
			{
				DisableIRQ(uart_irq_array[channel]);
				LPUART_Deinit(uart_array[channel]);
			}
		}
		else
		{

		}
    }
    return l_i8_result;
}
/*
 * 函 数 介 绍: 获取串口传输状态
 * 参       数
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 * 返   回  值: 返回传输状态，状态详见hal_uart_trans_status_et
 * 备       注:
 */
hal_uart_trans_status_et hal_uart_get_status(uint8_t channel)
{
    // 添加获取串口传输状态逻辑
    return HAL_UART_TRANS_STA_NO_START;
}
/*
 * 函 数 介 绍: UART 发送一个字节
 * 参       数
 *              [in]channel             串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]byte                发送的字节
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_writebyte(uint8_t channel, uint8_t byte)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    uint16_t delaytime = 0;
	if (channel >= HAL_UART_CHANNEL_NUM)
	{
		l_i8_result = HAL_RESULT_PARAMETER_ERR;
	}
	else
	{
		while(!(kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(uart_array[channel])))
		{
			if(delaytime > 1000)
			{
				break;
			}
			delaytime++;
		}
		if(delaytime >= 1000)
		{
			l_i8_result = HAL_RESULT_BUSY;
		}
		else
		{
			LPUART_WriteByte(uart_array[channel], byte);
		}
	}
    return l_i8_result;
}
/*
 * 函 数 介 绍: UART发送字节流
 * 参       数
 *              [in]channel                 串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]buffer                  字节流
 *              [in]len                     字节流长度
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_writebuff(uint8_t channel, const uint8_t *buffer, uint16_t len)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    hal_result_t result = HAL_RESULT_SUCCESS;
    uint16_t i = 0;
    if(NULL != buffer)
    {
    	//阻塞发送
//    	LPUART_WriteBlocking(uart_array[channel], buffer, len);
    	//非阻塞发送
    	for(i = 0; i < len; ++i)
    	{
    		result = hal_uart_writebyte(channel, buffer[i]);
    		if(result != HAL_RESULT_SUCCESS)
    		{
    			l_i8_result = HAL_RESULT_BUSY;
    		}
    	}
    }
    else
    {
    	l_i8_result = HAL_RESULT_PARAMETER_ERR;
    }
    return l_i8_result;
}
/*
 * 函 数 介 绍: 串口设置波特率
 * 参       数
 *              [in]channel                 串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]baudrate                串口波特率，详见hal_uart_baudrate_et}
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_set_baudrate(uint8_t channel, hal_uart_baudrate_et baudrate)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    uint32_t len = 0;
	hal_uart_cfg_st *pg_uart_cfg_sa = NULL;
	if (channel >= HAL_UART_CHANNEL_NUM)
	{
		l_i8_result = HAL_RESULT_PARAMETER_ERR;
	}
	else
	{
		/* 添加配置波特率逻辑 */
			hal_uart_get_cfg(&pg_uart_cfg_sa,&len);
			if((NULL != pg_uart_cfg_sa)&&(channel < HAL_UART_CHANNEL_NUM))
			{
				l_i8_result = hal_uart_enabled(channel,HAL_ENABLED_DISABLE);
				if(HAL_RESULT_SUCCESS == l_i8_result)
				{
					switch(baudrate)
					{
						case HAL_UART_BAUDRATE_9600:
							pg_uart_cfg_sa[channel].m_baundrate_e = HAL_UART_BAUDRATE_9600;
							l_i8_result = hal_uart_init(channel);
							break;
						case HAL_UART_BAUDRATE_115200:
							pg_uart_cfg_sa[channel].m_baundrate_e = HAL_UART_BAUDRATE_115200;
							l_i8_result = hal_uart_init(channel);
							break;
						default:
							l_i8_result = HAL_RESULT_PARAMETER_ERR;
							break;
					}
				}
				else
				{

				}
			}
			else
			{
				l_i8_result = HAL_RESULT_PARAMETER_ERR;
			}

	}
    return l_i8_result;
}
/*
 * 函 数 介 绍: 串口接收回调函数设置
 * 参       数
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]rx_func         接收回调函数}
 * 返   回  值: {*}
 * 备       注:
 */
void hal_uart_rx_cbk(uint8_t channel, hal_uart_rxcbk rx_func)
{
	if((channel < HAL_UART_CHANNEL_NUM)&&(NULL != rx_func))
	{
		uart_rx_cbk_array[channel] = rx_func;
	}
}
/*
 * 函 数 介 绍: 串口发送完成回调函数配置
 * 参       数
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]tx_func         发送完成回调函数}
 * 返   回  值: {*}
 * 备       注: 串口发送中断调用此函数接口
 */
void hal_uart_tx_done_cbk(uint8_t channel, hal_uart_txdonecbk tx_func)
{

}
/*
 * 函 数 介 绍:停止串口发送
 * 参       数 {channel             串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0}
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_tx_abort(uint8_t channel)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/*
 * 函 数 介 绍: 串口中断配置
 * 参       数
 *              [in]channel         串口通道号，通过hal_uart_cfg.h文件中通道宏定义组来配置，例如：HAL_UART_CHANNEL0映射uart0
 *              [in]isr_type        串口波特率，详见hal_uart_baudrate_et
 *              [in]enabled         中断使能标签
 * 返   回  值: hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注:
 */
hal_result_t hal_uart_isr_config(uint8_t channel, hal_uart_isr_type_et isr_type, hal_irq_priorty_et priorty, hal_enabled_et enabled)
{
    hal_result_t l_i8_result = HAL_RESULT_NO_SUPPORT;

    return l_i8_result;
}
/**
 * 函 数 介 绍 ：
 *               uart去初始化函数
 *
 * 参       数 ：
 *
 * 返   回  值 ：
 * 备       注 ：
 *
 */
void hal_uart_deinit(void)
{
	LPUART_Deinit(LPUART0);
	LPUART_Deinit(LPUART1);
}
