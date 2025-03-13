#include "hal_test.h"
//#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "hal_rte.h"
#include "hal_sys.h"
#include "uds_api.h"
#include "autosar_nm.h"
#include "canbus_drv.h"
#include "canbus_app.h"
#include "app.h"
#include "RFData_Operation_If.h"
#include "fsl_lpadc.h"
#include "canbus_trans.h"
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"
#include "hal_pin.h"

extern uint8_t receiveRFflag;
uint16_t BAT_V_Count = 0;
uint16_t DTC_V_Count = 0;
uint16_t Comlost_Count = 0;
uint32_t wol2min_counter = 0;
uint32_t tim1_cnt = 0;
extern uint32_t send293;
extern uint8_t wakenmFlag;
uint32_t start_tick = 0;
uint32_t end_tick = 0;
uint8_t  delay_cnt = 0;
uint8_t D6VersionCnt = 0;
extern uint8_t RFReleaseflag;
extern void UdsApi_10ms_Proc(void);
extern uint8_t log_os_start_complete;
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
					if(!UdsApi_ReciveData((uint32_t)(pdata->m_u32_canid), (uint8_t)(pdata->m_u8_dlc), (uint8_t *)(pdata->m_u8_pdata)))/* handle uds Message*/
					{
						return ;
					}
					else        /*Handle App Message*/
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


void can_timeout_test(void)
{
	nm_counter++;
	if(nm_counter >= 10000)
	{
		CanNm_NetworkRelease(0);
		nm_counter = 0;
	}
	else
	{}
}
void uart1_cbk(uint8_t *data, uint16_t len)
{
	hal_uart_if_writebyte(DEBUG_UART_INDEX, *data);
}


void SwTimer1msCallback(TimerHandle_t xTimer)
{
#if 0
	BtaOutputRfData_msg message;

	if (true == isMsg293Send)
	{
		delay_cnt++;
		if (delay_cnt >= 4)
		{
			delay_cnt = 0;
			end_tick = SysTimerGetCurrentTime();
			BtaOutputRfData1_t.msg.sig.FrameTime = msg293_sendTime + (end_tick - start_tick);
			BtaOutputRfData1_t.msg.sig.RFFrameTimeRC = BtaOutputRfData_t.msg.sig.RFSignalRC;
			canbusTransSendMsgFrame(0x294, BtaOutputRfData1_DLC,BtaOutputRfData1_t.msg.data);
			msg293_sendTime = 0;
			isMsg293Send = false;
		}
	}
#endif
	BAT_V_Count++;
	DTC_V_Count++;
	Comlost_Count++;
	if(BAT_V_Count > 5000)
	{
		BAT_V_Count = 0;
	}

	if(Comlost_Count > 5000)
	{
		Comlost_Count = 0;
	}

#if 1
	if(wakenmFlag == 1)
	{
		tim1_cnt++;
		if(tim1_cnt>=500)
		{
			if(!RFData_if_QueueIsEmpty(&RF_DataQueue))
			{
				if (RFData_if_QueuePop(&RF_DataQueue, &BtaOutputRfData_t))
				{
					canbusTransSendMsgFrame(0x293, BtaOutputRfData_DLC,BtaOutputRfData_t.msg.data);

					uint32_t rf_proc_time = SysTimerDelayTime(BtaOutputRfData_t.MsgTimeOut_PreT);
					if(rf_proc_time >= 500) rf_proc_time-= 500;
					BtaOutputRfData1_t.msg.sig.FrameTime = rf_proc_time > 255?255:rf_proc_time;
					BtaOutputRfData1_t.msg.sig.RFFrameTimeRC = BtaOutputRfData_t.msg.sig.RFSignalRC;
					canbusTransSendMsgFrame(0x294, BtaOutputRfData1_DLC,BtaOutputRfData1_t.msg.data);

					wakenmFlag = 0;
				}
			}
		}
	}
	else
	{
		if(!RFData_if_QueueIsEmpty(&RF_DataQueue))
		{
			if (RFData_if_QueuePop(&RF_DataQueue, &BtaOutputRfData_t))
			{
				canbusTransSendMsgFrame(0x293, BtaOutputRfData_DLC,BtaOutputRfData_t.msg.data);

				uint32_t rf_proc_time = SysTimerDelayTime(BtaOutputRfData_t.MsgTimeOut_PreT);
				if(rf_proc_time >= 500) rf_proc_time-= 500;
				BtaOutputRfData1_t.msg.sig.FrameTime = rf_proc_time > 255?255:rf_proc_time;
				BtaOutputRfData1_t.msg.sig.RFFrameTimeRC = BtaOutputRfData_t.msg.sig.RFSignalRC;
				canbusTransSendMsgFrame(0x294, BtaOutputRfData1_DLC,BtaOutputRfData1_t.msg.data);
			}
		}
	}
#else
	if(wakenmFlag == 1)
	{
		tim1_cnt++;
		//wol2min_counter++;
		if(tim1_cnt>=500)
		{
			if ((false == RFData_if_QueueIsEmpty(&RF_DataQueue)) && (false == isMsg293Send))
			{
				if (RFData_if_QueuePop(&RF_DataQueue, &BtaOutputRfData_t))
				{
					start_tick = SysTimerGetCurrentTime();
					canbusTransSendMsgFrame(0x293, BtaOutputRfData_DLC,BtaOutputRfData_t.msg.data);
					isMsg293Send = true;
				}
			}
		}
	}
	else
	{
		if ((false == RFData_if_QueueIsEmpty(&RF_DataQueue)) && (false == isMsg293Send))
		{
			if (RFData_if_QueuePop(&RF_DataQueue, &BtaOutputRfData_t))
			{
				start_tick = SysTimerGetCurrentTime();
				canbusTransSendMsgFrame(0x293, BtaOutputRfData_DLC,BtaOutputRfData_t.msg.data);
				isMsg293Send = true;
			}
		}
	}
#endif

	if(RFReleaseflag == 1)
	{
		wol2min_counter++;
		if(wol2min_counter >= 120*1000)
		{
			CanNm_NetworkRelease(0);
			rf_SetSleepFlag(1);
			tim1_cnt = 0;
			wol2min_counter = 0;
			wakenmFlag = 0;
			RFReleaseflag = 0;
		}
	}
	CanNm_Timer();
	UdsApi_TimeoutTimeer();
	Busoff_timer();
	//BleAppTimerCallback();

	canbusTxService();
	can_busoff_proc();

#if USE_OWNER_BLETP
#else
	app_transmit_entry();
#endif
	CanNm_MainFunction();

}

extern bool_t D6VersionFlag;
void SwTimer10msCallback(TimerHandle_t xTimer)
{
	LPADC_DoSoftwareTrigger(ADC0,1);

	UdsApi_10ms_Proc();

	NCK2910_Rf10msProc();

	if(D6VersionCnt < 200)
	{
		D6VersionCnt++;
		if(D6VersionCnt == 190)
		{
			uciMsgCustom_GetProductInfoCMD();
		}
	}

	/* SyRS_823 */
	extern uint32_t ChrgrGunGateOpenReq_On_KEEP_TIME_cnt;
	if(ChrgrGunGateOpenReq_On_KEEP_TIME_cnt > 0) ChrgrGunGateOpenReq_On_KEEP_TIME_cnt --;
	else
	{
		if(BTA_SRF_295_t.msg.sig.ChrgrGunGateOpenReq == BASICONOFFSWTSTSON_493)
		{
			BTA_SRF_295_t.msg.sig.ChrgrGunGateOpenReq = BASICONOFFSWTSTSOFF_492;
#if (defined(RF_DEBUG) && (RF_DEBUG == 1))
			dbg_Printf("Charging Pile Msg Recv Timeout, Set Basicon to OFF!\r\n");
#endif
		}
	}
	extern uint8_t RF_initvalue_flag;
	if(RF_initvalue_flag == 1)
	{
		RF_initvalue_flag = 0;
		Dtc_Recove(&DFC_RF_CHIP_COM_ERROR);
	}
}

void SystemosIint(void)
{
	hal_clock_if_init();
	//BleTimer_Init();
	hal_pin_if_init();
	hal_pin_if_write(HAL_PIN_INDEX10, HAL_PIN_LEVEL_HIGH);	/* Open AD switch*/
	hal_pin_if_write(HAL_PIN_INDEX20, HAL_PIN_LEVEL_HIGH);
	hal_pin_if_write(HAL_PIN_INDEX23, HAL_PIN_LEVEL_LOW);
	hal_adc_if_init(1);
	hal_cfls_if_init();
	hal_uart_if_init(1);
	hal_wdt_if_init(0, HAL_WDT_SPEED_SLOW);
	hal_pin_if_write(CANSTB_INDEX, HAL_PIN_LEVEL_LOW);
	hal_spi_if_init(J29D6_SPI_INDEX);
	CanNm_Init();
	canbusServiceInit();
	UdsApi_Uds_Initialize();
	logPrintfImmedi("Start Sending !\r\n");
}
void Sleepwakeupinit(void)
{
	//hal_pin_if_write(HAL_PIN_INDEX20, HAL_PIN_LEVEL_LOW);
	hal_pin_if_write(HAL_PIN_INDEX10, HAL_PIN_LEVEL_HIGH);	/* Open AD switch*/
	hal_pin_init_index(HAL_PIN_INDEX12);
	hal_adc_if_init(1);

	hal_pin_init_index(HAL_PIN_INDEX14);
	hal_pin_if_write(CANSTB_INDEX, HAL_PIN_LEVEL_LOW);
	hal_spi_if_init(J29D6_SPI_INDEX);
	UdsApi_Uds_Initialize();
	Set_canBusoffFlag(0);
}
void DiagMange(void)
{
	hal_wdt_if_feed(0);
	UdsApi_Uds_Process();
}

void hal_test(void)
{
	hal_wdt_if_feed(0);
	CanNm_MainFunction();
	can_timeout_test();
	UdsApi_Uds_Process();
}


