/*******************************头文件*******************************/
#include "hal_uart.h"
#include "peripheral.h"
#include "ke_timer.h"
#include "app_ble.h"

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
			ke_timer_set(APP_BLE_OPT_DEBUGLOG, APP_BLE_TASK_ID, 0);//让log task去处理uart数据,此时src_id就是0xff
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
uint16_t hal_uart_log_rx_buf_len_get(void)
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
//重置参数
void hal_uart_log_rx_buf_reset(void)
{
	g_stUartLogCtl.finsh = 0;
	g_stUartLogCtl.rxLen = 0;
	g_stUartLogCtl.irqCount = 0;
	memset(g_stUartLogCtl.rxBuf,0,sizeof(g_stUartLogCtl.rxBuf));
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
			ke_timer_set(APP_BLE_OPT_MCU_RX, APP_BLE_TASK_ID, 0);//让log task去处理uart数据
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
//重置参数
void hal_uart_fm33lg_rx_buf_reset(void)
{
	g_stUartFm33lgCtl.finsh = 0;
	g_stUartFm33lgCtl.rxLen = 0;
	g_stUartFm33lgCtl.irqCount = 0;
	memset(g_stUartFm33lgCtl.rxBuf,0,sizeof(g_stUartFm33lgCtl.rxBuf));
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

