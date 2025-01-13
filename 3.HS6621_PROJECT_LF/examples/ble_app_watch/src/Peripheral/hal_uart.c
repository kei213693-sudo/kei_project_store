/*******************************头文件*******************************/
#include "hal_uart.h"
#include "peripheral.h"
#include "ke_timer.h"
#include "app_ble.h"
#include "pmu.h"

/*******************************内部函数的声明*******************************/
static void hal_uart_log_rx_callback(uint8_t data);
static void hal_uart_log_timer_handler(co_timer_t *timer, void *param);

static void hal_uart_fm33lg_rx_callback(uint8_t data);
static void hal_uart_fm33lg_timer_handler(co_timer_t *timer, void *param);
static void hal_uart_fm33lg_tx_callback(void);

/*******************************内部变量定义*******************************/
static hal_uart_log_ctl_t g_stUartLogCtl = {0};
static hal_uart_fm33lg_ctl_t g_stUartFm33lgCtl = {0};

static co_timer_t g_uart_log_timer;
static co_timer_t g_uart_fm33lg_timer;

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
	if(0 == g_stUartLogCtl.finsh)
	{
		g_stUartLogCtl.rxBuf[g_stUartLogCtl.rxLen] = data;
		g_stUartLogCtl.rxLen++;
		if(g_stUartLogCtl.rxLen >= HAL_UART_LOG_LEN)
		{//超过最大值，覆盖之前的数据
			g_stUartLogCtl.rxLen = 0;
		}

        co_timer_del(&g_uart_log_timer);//先取消定时器任务
		co_timer_set(&g_uart_log_timer, HAL_UART_LOG_RX_IRQ_TIMEOUT
                    , TIMER_ONE_SHOT, hal_uart_log_timer_handler, NULL);//再启动
        if(!(PMU_LP_UART0 & pmu_lowpower_prevent_status()))//判定没被置位就置位
        //if(g_stUartFm33lgCtl.rxLen == 1)
        {
            pmu_lowpower_prevent(PMU_LP_UART0);//避免进入休眠
        }
    }
}

/**************************************************************
*函数功能描述 ：rx irq检查操作，判断是否存在接收完成
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void hal_uart_log_timer_handler(co_timer_t *timer, void *param)
{
    g_stUartLogCtl.finsh = 1;
    ke_timer_set(APP_BLE_OPT_DEBUGLOG, APP_BLE_TASK_ID, 0);//让log task去处理uart数据,此时src_id就是0xff
    if(PMU_LP_UART0 & pmu_lowpower_prevent_status())
    {//处理完毕，允许休眠
        pmu_lowpower_allow(PMU_LP_UART0);
    }
}

//获取rxBuf地址
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
//获取rxBuf长度
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
	memset(g_stUartLogCtl.rxBuf,0,HAL_UART_LOG_LEN);
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
	if(0 == g_stUartFm33lgCtl.finsh)
	{
		g_stUartFm33lgCtl.rxBuf[g_stUartFm33lgCtl.rxLen] = data;
		g_stUartFm33lgCtl.rxLen++;
		
		if(g_stUartFm33lgCtl.rxLen >= HAL_UART_FM33LG_LEN)
		{//超过最大值，覆盖之前的数据
			g_stUartFm33lgCtl.rxLen = 0;
		}

        co_timer_del(&g_uart_fm33lg_timer);//先取消定时器任务
		co_timer_set(&g_uart_fm33lg_timer, HAL_UART_FM33LG_RX_IRQ_TIMEOUT
                    , TIMER_ONE_SHOT, hal_uart_fm33lg_timer_handler, NULL);//再启动
        if(!(PMU_LP_UART1 & pmu_lowpower_prevent_status()))//判定没被置位就置位
        //if(g_stUartFm33lgCtl.rxLen == 1)
        {//不要每次都调用，该函数会失能中断
            pmu_lowpower_prevent(PMU_LP_UART1);//避免进入休眠
        }
	}
}

/**************************************************************
*函数功能描述 ：rx irq检查操作，判断是否存在接收完成
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void hal_uart_fm33lg_timer_handler(co_timer_t *timer, void *param)
{
    g_stUartFm33lgCtl.finsh = 1;
    ke_timer_set(APP_BLE_OPT_MCU_RX, APP_BLE_TASK_ID, 0);//让log task去处理uart数据
    if(PMU_LP_UART1 & pmu_lowpower_prevent_status())
    {//处理完毕，允许休眠
        pmu_lowpower_allow(PMU_LP_UART1);
    }
}

//获取rxBuf地址
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
//获取rxBuf长度
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
	memset(g_stUartFm33lgCtl.rxBuf,0,HAL_UART_FM33LG_LEN);
}

/**************************************************************
*函数功能描述 ：非阻塞式发送数据完毕回调
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void hal_uart_fm33lg_tx_callback(void)
{
	//log_debug("uart fm33lg tx complete.\n");
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
        log_debug_array_ex("mcu tx hex",g_stUartFm33lgCtl.txBuf,g_stUartFm33lgCtl.txLen);

		//非阻塞式发送
		uart_send_noblock(HAL_UART_FM33LG_TYPE,(uint8_t **)&g_stUartFm33lgCtl.txBuf,(unsigned *)&g_stUartFm33lgCtl.txLen,hal_uart_fm33lg_tx_callback);
	}
}

