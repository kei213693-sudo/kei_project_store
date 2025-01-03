/*******************************头文件*******************************/
#include "hal_uart.h"
#include "peripheral.h"
#include "hal_flash.h"

/*******************************内部函数的声明*******************************/
static void hal_uart_log_rx_callback(uint8_t data);

static void hal_uart_fm33lg_rx_callback(uint8_t data);
static void hal_uart_fm33lg_tx_callback(void);

/*******************************内部变量定义*******************************/
static hal_uart_log_ctl_t g_stUartLogCtl = {0};
static hal_uart_fm33lg_ctl_t g_stUartFm33lgCtl = {0};

/*******************************全局变量定义*******************************/
/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：初始化调试日志uart相关参数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void hal_uart_log_init(void)
{
    pinmux_config(HAL_UART_LOG_TX_PIN,  PINMUX_UART0_SDA_O_CFG);
    pinmux_config(HAL_UART_LOG_RX_PIN,  PINMUX_UART0_SDA_I_CFG);
    pmu_pin_mode_set(BITMASK(HAL_UART_LOG_TX_PIN), PMU_PIN_MODE_PP);
    pmu_pin_mode_set(BITMASK(HAL_UART_LOG_RX_PIN), PMU_PIN_MODE_PU);
	
	uart_open(HAL_UART_LOG_TYPE, HAL_UART_LOG_BAUDRATE, UART_FLOW_CTRL_DISABLED, hal_uart_log_rx_callback);
}

/**************************************************************
*函数功能描述 ：uart单字节接收回调处理，非dma模式
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void hal_uart_log_rx_callback(uint8_t data)
{
	if(0 != g_stUartLogCtl.finsh)
	{
		log_debug("cmd is running!\n");
	}
	else
	{
		g_stUartLogCtl.irqCount = 0;//每接收一次清空一次计数标志
		
		g_stUartLogCtl.rxBuf[g_stUartLogCtl.rxLen] = data;
		g_stUartLogCtl.rxLen++;
		if(g_stUartLogCtl.rxLen >= sizeof(g_stUartLogCtl.rxBuf))
		{//超过最大值，覆盖之前的数据
			g_stUartLogCtl.rxLen = 0;
		}
	}
}

/**************************************************************
*函数功能描述 ：rx irq检查操作，判断是否存在接收完成
*函数参数 ：
*函数返回值 ：
*备注：TIM中断中调用
**************************************************************/
void hal_uart_log_RxIrqCheck(void)
{
    if((0 != g_stUartLogCtl.rxLen) && (1 != g_stUartLogCtl.finsh))
    {
        g_stUartLogCtl.irqCount++;
        if(g_stUartLogCtl.irqCount >= HAL_UART_LOG_RX_IRQ_TIMEOUT)
        {//1ms
            g_stUartLogCtl.irqCount = 0;
            g_stUartLogCtl.finsh = 1;
            //app_task_sync_isr(APP_TASK_MODULE_LOG);//让log task去处理uart数据
        }
    }
}

//获取rxbuf地址
uint8_t *hal_uart_log_rx_buf_addr_get(void)
{
	if(g_stUartLogCtl.finsh == 1)
    {
        return g_stUartLogCtl.rxBuf;
    }
    else
    {//接收未完成，不返回地址
        return NULL;
    }
}
//获取rxbuf长度
uint8_t hal_uart_log_rx_buf_len_get(void)
{
	if(g_stUartLogCtl.finsh == 1)
    {
        return g_stUartLogCtl.rxLen;
    }
    else
    {
        return 0;
    }
}

/**************************************************************
*函数功能描述 ：初始化和主控MCU FM33LG通信uart相关参数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void hal_uart_fm33lg_init(void)
{
    pinmux_config(HAL_UART_FM33LG_TX_PIN,  PINMUX_UART1_SDA_O_CFG);
    pinmux_config(HAL_UART_FM33LG_RX_PIN,  PINMUX_UART1_SDA_I_CFG);
    pmu_pin_mode_set(BITMASK(HAL_UART_FM33LG_TX_PIN), PMU_PIN_MODE_PP);
    pmu_pin_mode_set(BITMASK(HAL_UART_FM33LG_RX_PIN), PMU_PIN_MODE_PU);
	
	uart_open(HAL_UART_FM33LG_TYPE, HAL_UART_FM33LG_BAUDRATE, UART_FLOW_CTRL_DISABLED, hal_uart_fm33lg_rx_callback);
}

/**************************************************************
*函数功能描述 ：uart单字节接收回调处理，非dma模式
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void hal_uart_fm33lg_rx_callback(uint8_t data)
{
	//log_debug("uart revice:0x%x\n",data);
	if(0 != g_stUartFm33lgCtl.finsh)
	{
		log_debug("comm is running!\n");
	}
	else
	{
		g_stUartFm33lgCtl.irqCount = 0;

		g_stUartFm33lgCtl.rxBuf[g_stUartFm33lgCtl.rxLen] = data;
		g_stUartFm33lgCtl.rxLen++;
		if(g_stUartFm33lgCtl.rxLen >= sizeof(g_stUartFm33lgCtl.rxBuf))
		{//超过最大值，覆盖之前的数据
			g_stUartFm33lgCtl.rxLen = 0;
		}
	}
}

/**************************************************************
*函数功能描述 ：rx irq检查操作，判断是否存在接收完成
*函数参数 ：
*函数返回值 ：
*备注：TIM中断中调用
**************************************************************/
void hal_uart_fm33lg_RxIrqCheck(void)
{
    if((0 != g_stUartFm33lgCtl.rxLen) && (1 != g_stUartFm33lgCtl.finsh))
    {
        g_stUartFm33lgCtl.irqCount++;
        if(g_stUartFm33lgCtl.irqCount >= HAL_UART_FM33LG_RX_IRQ_TIMEOUT)
        {//5ms
            g_stUartFm33lgCtl.irqCount = 0;
            g_stUartFm33lgCtl.finsh = 1;
            //app_task_sync_isr(APP_TASK_MODULE_LOG);//让log task去处理uart数据
        }
    }
}

//获取rxbuf地址
uint8_t *hal_uart_fm33lg_rx_buf_addr_get(void)
{
	if(g_stUartFm33lgCtl.finsh == 1)
    {
        return g_stUartFm33lgCtl.rxBuf;
    }
    else
    {//接收未完成，不返回地址
        return NULL;
    }
}
//获取rxbuf长度
uint8_t hal_uart_fm33lg_rx_buf_len_get(void)
{
	if(g_stUartFm33lgCtl.finsh == 1)
    {
        return g_stUartFm33lgCtl.rxLen;
    }
    else
    {
        return 0;
    }
}

/**************************************************************
*函数功能描述 ：非阻塞式发送数据完毕回调
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void hal_uart_fm33lg_tx_callback(void)
{
	log_debug("uart fm33lg tx complete.\n");
}

//设置待发送数据，并启动发送回调
void hal_uart_fm33lg_tx_buf_set(uint8_t *pData,uint8_t u8Len)
{
	if((NULL != pData) && (0 != u8Len))
	{
		uart_wait_send_finish(HAL_UART_FM33LG_TYPE);//等待发送完成

		g_stUartFm33lgCtl.txLen = u8Len;
		memset(g_stUartFm33lgCtl.txBuf,0,HAL_UART_FM33LG_LEN);
		memcpy(g_stUartFm33lgCtl.txBuf,pData,u8Len);

		//非阻塞式发送
		uart_send_noblock(HAL_UART_FM33LG_TYPE,(uint8_t **)&g_stUartFm33lgCtl.txBuf,(unsigned *)&g_stUartFm33lgCtl.txLen,hal_uart_fm33lg_tx_callback);
	}
}


/**************************************************************
*函数功能描述 ：处理上位机通过uart传递的数据
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void hal_uart_log_cmd_handle(void)
{
	//调试用，后续放在app task中选择是否执行
	if(0 == g_stUartLogCtl.finsh)
	{
		return;
	}
	else
	{
		g_stUartLogCtl.finsh = 0;
	}

	log_debug("uart rx:%s\n",g_stUartLogCtl.rxBuf);
	log_debug("uart rx hex[%d]:",g_stUartLogCtl.rxLen);
	log_debug_array_ex("", g_stUartLogCtl.rxBuf, g_stUartLogCtl.rxLen);
	
	if(0 == strncmp("LED",(const char*)g_stUartLogCtl.rxBuf,3))
	{//
		uint8_t u8Opt = g_stUartLogCtl.rxBuf[3] - 0x30;
		log_debug("LED opt:%d\n",u8Opt);
	}
	else if(0 == strncmp("flashtest",(const char*)g_stUartLogCtl.rxBuf,9))
	{//flash测试
		uint8_t u8Opt = g_stUartLogCtl.rxBuf[9] - 0x30;
		uint8_t data[128] = {0};
		uint8_t data2[128] = {0};
		uint8_t i = 0;
		for(i = u8Opt;i<(128 + u8Opt);i++)
		{
			data[i-u8Opt] = i;
		}
		
		hal_flash_erase(HAL_FLASH_USER_DATA_START,SF_SECTOR_SIZE);//先擦除
		hal_flash_write(HAL_FLASH_USER_DATA_START,data,128);//先写入
		hal_flash_read(HAL_FLASH_USER_DATA_START,data2,128);//再读取
		log_debug_array_ex("flash data",data2,128);//打印待写入的数据
	}
	else
	{
	}
	
	//清空接收缓存
	g_stUartLogCtl.finsh = 0;
	g_stUartLogCtl.rxLen = 0;
	memset(g_stUartLogCtl.rxBuf,0,sizeof(g_stUartLogCtl.rxBuf));
}

