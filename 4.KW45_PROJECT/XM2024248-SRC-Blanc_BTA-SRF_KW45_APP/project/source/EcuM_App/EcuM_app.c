/*
 * EcuM_app.c
 *
 *  Created on: 2024年7月14日
 *      Author: F704011769_
 */
#include "app.h"
#include "autosar_nm.h"
#include "bleDrv_api.h"
#include "canbus_trans.h"
#include "bleAppService_api.h"

/************************************************************************************
 *************************************************************************************
 * Private functions prototypes
 *************************************************************************************
 ************************************************************************************/
extern osa_semaphore_handle_t semaphoreHandle;
static uint8_t Status =0;
static uint8_t diag_task_sleep_flag = 0;
static uint8_t rf_task_sleep_flag = 1;
static uint8_t testsleepflag = 0;
static uint8_t bleSleepFlag = 0;
extern hal_tim_cbk_t timer_cbk_array;
uint8_t UWB_Sleep_Flag = 0;
static uint8_t uwbSleepFlg = 0;
static uint8_t uwbSleep_timeoutFlg = 0;
static uint8_t ECU_sleep_Flg = 0;
extern uint8_t log_os_start_complete;
/************************************************************************************
 *************************************************************************************
 * Private memory declarations
 *************************************************************************************
 ************************************************************************************/
static OSA_TASK_HANDLE_DEFINE(s_logTaskHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_EcuMHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_ComHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_DiagHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_BleStackHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_BleServiceHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_UWBHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_RFHandle);

static OSA_TASK_DEFINE(task_EcuM, gMainThreadPriority_c, 1, 2048, 0);
static OSA_TASK_DEFINE(task_Com, 4 , 1, 2048, 0);
static OSA_TASK_DEFINE(task_Diag, gMainThreadPriority_c+2, 1, 2048, 0);
static OSA_TASK_DEFINE(task_BleStack, gMainThreadPriority_c+3, 1, 1024, 0);
static OSA_TASK_DEFINE(task_BleService, gMainThreadPriority_c+4, 1, 512, 0);
static OSA_TASK_DEFINE(task_UWB, gMainThreadPriority_c+4, 1, 2048, 0);
static OSA_TASK_DEFINE(task_RF, gMainThreadPriority_c+3, 1, 2048, 0);
static OSA_TASK_DEFINE(log_task, gMainThreadPriority_c+5, 1, 2*1024, 0);
uint8_t EcuM_GetUwbSleepFlg()
{
        return uwbSleepFlg;
}

void EcuM_SetUwbSleepFlg(uint8_t stateflg)
{
        uwbSleepFlg = stateflg;
}

uint8_t EcuM_GetUwb_timeout_SleepFlg()
{
        return uwbSleep_timeoutFlg;
}

void EcuM_SetUwb_timeout_SleepFlg(uint8_t stateflg)
{
	uwbSleep_timeoutFlg = stateflg;
}


uint8_t EcuM_GetCurrentState()
{
	return Status;
}

void EcuM_SetModuleState(uint8_t statusint)
{
	Status = statusint;
}

uint8_t diag_GetSleepFlag()
{
	return diag_task_sleep_flag;
}

void diag_SetSleepFlag(uint8_t statusint)
{
	diag_task_sleep_flag = statusint;
}

uint8_t rf_GetSleepFlag()
{
	return rf_task_sleep_flag;
}

void rf_SetSleepFlag(uint8_t statusint)
{
	rf_task_sleep_flag = statusint;
}

void ble_SetSleepFlag(uint8_t statusint)
{
	rf_task_sleep_flag = statusint;
}

uint8_t ble_GetSleepFlag(void)
{
	return rf_task_sleep_flag;
}

uint8_t test_GetSleepFlag()
{
	return testsleepflag;
}

void test_SetSleepFlag(uint8_t statusint)
{
	testsleepflag = statusint;
}


void OsStart(void)
{
   /* Initialize debug console. */
   TaskCreate();

	/* Start scheduler*/
	OSA_Start();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : TaskCreate
 * Description   : This function is used to creat task.
 *
 *END**************************************************************************/
void TaskCreate(void)
{
	(void)OSA_TaskCreate((osa_task_handle_t)s_task_EcuMHandle, OSA_TASK(task_EcuM), NULL);
	(void)OSA_TaskCreate((osa_task_handle_t)s_task_ComHandle, OSA_TASK(task_Com), NULL);
	(void)OSA_TaskCreate((osa_task_handle_t)s_task_DiagHandle, OSA_TASK(task_Diag), NULL);
	(void)OSA_TaskCreate((osa_task_handle_t)s_task_BleStackHandle, OSA_TASK(task_BleStack), NULL);
	(void)OSA_TaskCreate((osa_task_handle_t)s_task_BleServiceHandle, OSA_TASK(task_BleService), NULL);
	(void)OSA_TaskCreate((osa_task_handle_t)s_task_UWBHandle, OSA_TASK(task_UWB), NULL);
	(void)OSA_TaskCreate((osa_task_handle_t) s_task_RFHandle, OSA_TASK(task_RF), NULL);
	(void)OSA_TaskCreate((osa_task_handle_t)s_logTaskHandle, OSA_TASK(log_task), NULL);
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

TimerHandle_t SwTimer1msHandle = NULL;
TimerHandle_t SwTimer10msHandle = NULL;
void swTimerCreate(void)
{
        /* The software timer period. */
        #define SW_TIMER_PERIOD_1MS          (1 / portTICK_PERIOD_MS)
        #define SW_TIMER_PERIOD_10MS         (10 / portTICK_PERIOD_MS)

    /* Create the software timer. */
        SwTimer1msHandle = xTimerCreate("SwTimer1ms",          /* Text name. */
                                                                 SW_TIMER_PERIOD_1MS, /* Timer period. */
                                 pdTRUE,             /* Enable auto reload. */
                                 0,                  /* ID is not used. */
                                 SwTimer1msCallback);   /* The callback function. */

    /* Create the software timer. */
        SwTimer10msHandle = xTimerCreate("SwTimer10ms",          /* Text name. */
                                                                 SW_TIMER_PERIOD_10MS, /* Timer period. */
                                 pdTRUE,             /* Enable auto reload. */
                                 0,                  /* ID is not used. */
                                 SwTimer10msCallback);   /* The callback function. */
    /* Start timer. */
    xTimerStart(SwTimer1msHandle, 0);
    xTimerStart(SwTimer10msHandle, 0);
}


void swTimerDestroy(void)
{
        xTimerDelete(SwTimer1msHandle, 0);
        xTimerDelete(SwTimer10msHandle, 0);
}


void task_EcuM(void *argument)
{
	uint32_t  Sleeptestcount = 0;
	static uint16_t uwbSleepRspWaitCnt = 0;
    while(1)
    {
    	Nm_StateType nmStatePtr;
    	Nm_ModeType nmModePtr;
    	CanNm_GetState(0, &nmStatePtr, &nmModePtr);
    	hal_wdt_if_feed(0);
    	Sleeptestcount++;
    	if(Sleeptestcount %20 == 0)
    	{
    		//Sleeptestcount = 0;
    		test_SetSleepFlag(1);
    		canlogPrintf("uwb() Sleeptestcount = %d %u\r\n",Sleeptestcount);
    		canlogPrintf("rf() Sleeptestcount = %d %r\r\n",Sleeptestcount);
    	}
    	canlogPrintf("ble() Sleeptestcount = %d %e\r\n",Sleeptestcount);
		switch(EcuM_GetCurrentState())
		{
		    case ECUM_State_Init:
		    	EcuM_SetModuleState(ECUM_State_PreSleep);
		    	break;
			case ECUM_State_Normal:
				if(((test_GetSleepFlag() == 1)&&(UdsApi_GetD6UpradeRefreshFlg() == 0)&&(diag_GetSleepFlag() == 1)&&\
						(rf_GetSleepFlag() ==1)&&(nmModePtr == NM_BUS_SLEEP)&&(Get_canBusoffFlag() ==0))||(Busoff_GetSleepFlag() ==1))
				{

					diag_SetSleepFlag(0);
					test_SetSleepFlag(0);
					Busoff_SetSleepFlag(0);
					//ble_SetSleepFlag(0);
					NM_SleepPre_Judge();
					EcuM_SetModuleState(ECUM_State_PreSleep);
				}
				break;

			case ECUM_State_Wakeup:
				//初始化相关唤醒源，清除相关标志位
				EcuM_SetModuleState(ECUM_State_Normal);
				break;

			case ECUM_State_PreSleep:
				//需判断休眠前是否存在唤醒源，若无相关唤醒源，则进入休眠状态，若此时存在唤醒源则进入normal状态
				if(PreSleep_Judge() == 1)
				{
					if(EcuM_GetUwbSleepFlg() != 1)
					{
						uciMsgCustom_LowPowerEnableCMD();
					}
					EcuM_SetModuleState(ECUM_State_Sleep);
					uwbSleepRspWaitCnt = 0;
				}
				break;

			case ECUM_State_Sleep:
				logPrintfImmedi("Presleep !\r\n");
				if((EcuM_GetUwbSleepFlg() == 1)|| (uwbSleepRspWaitCnt >= 30))
				{
					hal_pin_if_irq_enabled(UWB_INT0_INDEX,HAL_ENABLED_DISABLE);
					OSA_DisableScheduler();
					Set_canBusoffFlag(CAN_NO_BUSOFF);
					EcuM_SetUwb_timeout_SleepFlg(0);
					EcuM_SetUwbSleepFlg(1);
					canbusTransBufferClear();
					//MCU_lowPW();//休眠处理
					rangeStartingFlg = 0;
					cpdStartingFlg = 0;
					uciMsg_Init();
					logPrintf("Wakeup !\r\n");
					logPrintf("\r\n NM State: %d", nmModePtr);
					OSA_EnableScheduler();
					hal_pin_if_irq_enabled(UWB_INT0_INDEX,HAL_ENABLED_ENABLE);
					EcuM_SetModuleState(ECUM_State_Normal);
					uwbSleepRspWaitCnt = 0;
					RF_PrivtVINReadFlag = 0;
				}
				else
				{
					uwbSleepRspWaitCnt++;
				}
				break;

			default:

				break;
		}
    	OSA_TimeDelay(10/portTICK_RATE_MS);
    }
}


void task_Com(void *argument)
{
	 while(TRUE)
	{
		canbusTransSendMsg();
	}
}

void task_Diag(void *argument)
{
	while(1)
	{
		DiagMange();
		AppDtc_MainFunction();

		if(1)
		{
			diag_SetSleepFlag(1);
		}
		canbusRxService();
		RF_TpmsID_Check();
		OSA_TimeDelay(10/portTICK_RATE_MS);
	}
}

static uint8_t BleRecoveFlag = 0;
void task_BleStack(void *argument)
{
    /* Start BLE Platform related ressources such as clocks, Link layer and HCI transport to Link Layer */
	for(uint8_t i = 0; i < 3; i++)
	{
		(void)APP_InitBle();
		if((Get_uint8_bleInitFailureFlag() == BLE_INIT_SUCCESS_FLAG) &&\
				(Get_uint8_NBUInitFailureFlag() == BLE_NBU_INIT_SUCCESS_FLAG))
		{
			Dtc_Recove(&DFC_BLE_INIT_FAIL);
			break;
		}
		else if((Get_uint8_bleInitFailureFlag() == BLE_INIT_FAILURE_FLAG) ||\
				(Get_uint8_NBUInitFailureFlag() == BLE_NBU_INIT_FAILURE_FLAG))
		{
			Dtc_Active(&DFC_BLE_INIT_FAIL);
		}
	}

	if((Get_uint8_bleInitFailureFlag() == BLE_INIT_SUCCESS_FLAG) &&\
		(Get_uint8_NBUInitFailureFlag() == BLE_NBU_INIT_SUCCESS_FLAG))
	{
		/* Start Host stack */
		BluetoothLEHost_AppInit();

		while(TRUE)
		{
			BluetoothLEHost_HandleMessages();
		}
	}
	else
	{
		while(TRUE)
		{
			OSA_TimeDelay(100/portTICK_RATE_MS);
			if(BleRecoveFlag == 0)
			{
				BleRecoveFlag = 1;
				Dtc_Recove(&DFC_BLE_INIT_FAIL);
			}
		}
	}

}

static bleMsgInfo_t bleMsgInfo;
static uint8_t data[200] = {0};
static uint8_t bleData[200] = {0};
extern canTxMsgInfo_t canTxMsgInfo;
void task_BleService(void *argument)
{
	Init_BleService();

	while(1)
	{
		BleService_HandleMessage();
	}
//	BleCom_Init();
#if USE_OWNER_BLETP
	BleTp_Init();
#else
	app_transmit_init();
#endif
}

bool_t D6VersionFlag = 0;
void task_UWB(void *argument)
{
	(void)argument;
	static TickType_t xLastWakeTime;
	D6VersionFlag = 1;
	uwbTaskApp_Init();
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		hal_wdt_if_feed(0);
		uwbTaskApp_PeriodicTask(&xLastWakeTime);

	}
}

bool_t rfInitSuccFlag = 0;
void task_RF(void *argument)
{
	rfInitSuccFlag = NCK2910_Init();
	if(rfInitSuccFlag)
	{
	   if(KOSA_StatusSuccess != OSA_SemaphoreCreateBinary(&semaphoreHandle))
	   {
		   rfInitSuccFlag = 0;
		   /* Record Error Log */
		   logPrintf("RF binary semaphore create failed.\r\n");
	   }
	   else
	   {
		   hal_pin_if_irq_enabled(NCK2910_INT_N_PIN, HAL_ENABLED_ENABLE);
	   }
	}

	while(1)
	{	//to do  读can上发来的数据
		hal_wdt_if_feed(0);
		if(rfInitSuccFlag)
		{
			OSA_SemaphoreWait(semaphoreHandle, osaWaitForever_c);
			NCK2910_RfRcvDataDeal();
		}
		else
		{
			OSA_TimeDelay(10);
		}
	}
}

void log_task(void *argument)
{
	log_os_start_complete = 1;
	logUsartInit();
	while(1)
	{
		logUsartQueProce();
	}
}


