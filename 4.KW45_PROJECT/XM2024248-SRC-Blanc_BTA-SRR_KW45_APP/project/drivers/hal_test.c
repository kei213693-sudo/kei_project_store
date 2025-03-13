#include "drv_api.h"
#include "hal_test.h"
//#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "hal_rte.h"
#include "hal_sys.h"
#include "uds_api.h"
#include "autosar_nm.h"
#include "ComBusoff.h"
#include "canbus_drv.h"
#include "app.h"
#include "canbus_trans.h"
#include "hal_pin.h"
#include "UWB_CAN.h"
#include "hal_uart.h"
#include "autosar_nm.h"

uint16_t BAT_V_Count = 0;
uint16_t DTC_V_Count = 0;
void deleytime(void)
{
	uint32_t x=0,y=0;
	for(x = 0;x <= 1000;x++)
	{
		for(y = 0;y <= 10000; y++)
		{

		}
	}
}

uint8_t CANIf_Transmit(uint8_t channelnum, uint32_t id, uint8_t dlc, uint8_t *data)
{
#if 0
	hal_can_pdu_st pdu={0};
	pdu.m_u32_canid = id;
	pdu.m_id_type_e = HAL_CAN_ID_TYPE_StAND;
	pdu.m_u8_dlc = dlc;
	pdu.m_u8_pdata = data;
	hal_can_if_write(channelnum, &pdu);
#endif
	if(UdsApi_get_nm_tx_sta())
	{
		canbusTransSendMsgFrame(id, dlc, data);
	}
}

hal_pin_level_et read_level = HAL_PIN_LEVEL_LOW;
uint32_t ad_value = 0;
uint8_t spi1_senddata[16] = {0};
uint8_t spi1_readdata[16] = {0};
uint8_t spi0_senddata[16] = {0};
uint8_t spi0_readdata[16] = {0};
uint8_t can0_readdata[64];
uint8_t can0_senddata[64];
static uint16_t nm_counter = 0;
void hal_can_cbk(const hal_can_pdu_st * pdata)
{
	uint8_t udsResult = 0;
	Nm_StateType nmStatePtr;
	Nm_ModeType nmModePtr;

	CanNm_GetState(0, &nmStatePtr, &nmModePtr);
	if(pdata != NULL)
	{
		if(Get_PMState() == PMS_NORMAL)
		{

			if(UdsApi_get_nm_rx_sta())
			{
				/*网络被动唤醒报文接收处理*/
				if((pdata->m_u32_canid >= 0x500) && (pdata->m_u32_canid <= 0x57F))
				{
					//dbg_Printf("Start 3 !\r\n");
					nm_counter = 0;
					CanNm_RxIndication(0,pdata->m_u32_canid,pdata->m_u8_dlc,pdata->m_u8_pdata);
					return ;
				}
			}

			if(nmModePtr != NM_BUS_SLEEP)        /**/
			{
				if(((uint32_t)(pdata->m_u32_canid) == 0x673) && (pdata->m_u8_pdata[0] == 0x04) && (pdata->m_u8_pdata[1] == 0x2E) &&\
					(pdata->m_u8_pdata[2] == 0x4A) && (pdata->m_u8_pdata[3] == 0xff))
				{
					Set_BLE_AddressIndex(pdata->m_u8_pdata[4]);
				}
				if(!UdsApi_ReciveData((uint32_t)(pdata->m_u32_canid), (uint8_t)(pdata->m_u8_dlc), (uint8_t *)(pdata->m_u8_pdata)))/* handle uds Message*/
				{
					return ;
				}
				else/*Handle App Message*/
				{
					CANIf_Indication(pdata);
					return ;
				}
			}
		}
	}
	else
	{
	}
	return ;
}

// void can_timeout_test(void)
// {
// 	nm_counter++;
// 	if(nm_counter >= 10000)
// 	{
// 		CanNm_NetworkRelease(0);
// 		nm_counter = 0;
// 	}
// 	else
// 	{}
// }
void uart1_cbk(uint8_t *data, uint16_t len)
{
	hal_uart_if_writebyte(DEBUG_UART_INDEX, *data);
}


void hal_tim0_cbk(void)
{
	if(0 != g_u16UartRxLen)
	{
		g_u8UartRxCount++;
		if(g_u8UartRxCount > 5)
		{
			g_u8UartRxCount = 0;
			g_u8UartRxFlag = 1;
			//传递信号量给task让其处理uart数据
			extern SemaphoreHandle_t xSemaphore_debugLog;
			BaseType_t xHigherPriorityTaskWoken;
			xSemaphoreGiveFromISR(xSemaphore_debugLog, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}
}


void hal_tim1_cbk(void)
{

}

void SwTimer1msCallback(TimerHandle_t xTimer)
{
	BAT_V_Count++;
	DTC_V_Count++;
	if(BAT_V_Count > 5000)
	{
		BAT_V_Count = 0;
	}
	static uint8_t UdsTaskCnt_10ms = 0;
	UdsTaskCnt_10ms++;

	CanNm_Timer();
	UdsApi_TimeoutTimeer();
	Busoff_timer();
	BleAppTimerCallback();
	can_busoff_proc();
	if(UdsTaskCnt_10ms >= 10)
	{
		UdsTaskCnt_10ms = 0;
		UdsApi_10ms_Proc();
	}
	canbusTxService();
	CanNm_MainFunction();
}

void SwTimer10msCallback(TimerHandle_t xTimer)
{
	static uint8_t cycliCnt = 0;
	cycliCnt++;
	if(cycliCnt >= 5)
	{
		//cycliCnt = 0;
		APP_APPModeControl();
	}

	LPADC_DoSoftwareTrigger(ADC0,1);
}

void SystemosIint(void)
{
	hal_clock_if_init();
	hal_pin_if_init();
	hal_pin_if_write(HAL_PIN_INDEX21, HAL_PIN_LEVEL_HIGH);
	hal_pin_if_write(HAL_PIN_INDEX10, HAL_PIN_LEVEL_HIGH);	/* Open AD switch*/
	hal_adc_if_init(1);
	/*Flash Init*/
	hal_cfls_if_init();
	/*WatchDog Init*/
	hal_wdt_if_init(0, HAL_WDT_SPEED_FAST);
	hal_pin_if_write(CANSTB_INDEX, HAL_PIN_LEVEL_LOW);
	/*Timer Init*/
	hal_tim_if_init(0, HAL_ENABLED_ENABLE, hal_tim0_cbk);
	//hal_tim_if_init(1, HAL_ENABLED_ENABLE, hal_tim1_cbk);
	hal_spi_if_init(J29D6_SPI_INDEX);
	hal_uart_if_init(1);
	hal_uart_if_rx_cbk(1,hal_uart_rxCallBack);//注册回调函数

	/*CAN Init*/
	canbusServiceInit();
	/*CAN NM Init*/
	CanNm_Init();
	/*UDS Init*/
	UdsApi_Uds_Initialize();
	dbg_Printf("Start Sending !\r\n");
}
void Sleepwakeupinit(void)
{
	//hal_pin_if_write(HAL_PIN_INDEX21, HAL_PIN_LEVEL_HIGH);
	hal_pin_if_write(HAL_PIN_INDEX10, HAL_PIN_LEVEL_HIGH);	/* Open AD switch*/
	hal_pin_init_index(HAL_PIN_INDEX12);
	hal_adc_if_init(1);
	//hal_cfls_if_init();
	//hal_wdt_if_init(0, HAL_WDT_SPEED_FAST);
	hal_pin_init_index(HAL_PIN_INDEX14);
	hal_pin_if_write(CANSTB_INDEX, HAL_PIN_LEVEL_LOW);
	hal_spi_if_init(J29D6_SPI_INDEX);
	//canbusServiceInit();
	UdsApi_Uds_Initialize();
}

void DiagMange(void)
{
	hal_wdt_if_feed(0);
	UdsApi_Uds_Process();
}
// void hal_test(void)
// {
// 	hal_wdt_if_feed(0);
// 	CanNm_MainFunction();
// 	can_timeout_test();
// 	UdsApi_Uds_Process();
// }



