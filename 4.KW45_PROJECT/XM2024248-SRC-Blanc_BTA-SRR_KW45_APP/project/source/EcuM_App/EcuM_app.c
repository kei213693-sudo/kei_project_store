/*
 * EcuM_app.c
 *
 *  Created on: 2024年7月14日
 *      Author: F704011769_
 */
#include "app.h"
#include "autosar_nm.h"
#include "canbus_trans.h"
#include "uciMsg.h"
#include "ble_DV_cfg.h"
#include "UWB_CAN.h"
#include "hal_rte.h"
#include "hal_uart.h"
#include "app_api.h"

SemaphoreHandle_t xSemaphore_debugLog = NULL;
nio_ble_message_protocol_t *g_pStMessage = NULL;
uint8_t g_u8MessageData[512] = {0};//g_pStMessage的数据缓存空间实体
uint8_t g_u8SeriaNetData[512] = {0};//透传的数据缓存空间实体
uint8_t g_nio_ble_auth_flag = 0;//认证标志位，确定返回成功或失败

/************************************************************************************
 *************************************************************************************
 * Private functions prototypes
 *************************************************************************************
 ************************************************************************************/


static uint8_t itv293_counter;
static uint16_t start_time;
struct timeval tv;
extern bool isINT_NTrigger;

static uint8_t Status =0;
static uint8_t diag_task_sleep_flag = 0;
static uint8_t rf_task_sleep_flag = 0;
static uint8_t testsleepflag = 0;

static uint8_t uwbSleepFlg = 0;
/************************************************************************************
 *************************************************************************************
 * Private memory declarations
 *************************************************************************************
 ************************************************************************************/
static OSA_TASK_HANDLE_DEFINE(s_task_EcuMHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_ComHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_DiagHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_BleStackHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_BleServiceHandle);
static OSA_TASK_HANDLE_DEFINE(s_task_UWBHandle);

static OSA_TASK_HANDLE_DEFINE(s_task_DebugLogHandle);

static OSA_TASK_DEFINE(task_EcuM, gMainThreadPriority_c, 1, 1024, 0);
static OSA_TASK_DEFINE(task_Com, 4 , 1, 2048, 0);
static OSA_TASK_DEFINE(task_Diag, gMainThreadPriority_c+2, 1, 2048, 0);
static OSA_TASK_DEFINE(task_BleStack, gMainThreadPriority_c+3, 1, 2048, 0);
static OSA_TASK_DEFINE(task_BleService, gMainThreadPriority_c+4, 1, 1024, 0);
static OSA_TASK_DEFINE(task_UWB, gMainThreadPriority_c+4, 1, 2048, 0);

static OSA_TASK_DEFINE(task_DebugLog, gMainThreadPriority_c+4, 1, 2048, 0);

uint8_t EcuM_GetUwbSleepFlg()
{
	return uwbSleepFlg;
}

void EcuM_SetUwbSleepFlg(uint8_t stateflg)
{
	uwbSleepFlg = stateflg;
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
	(void)OSA_TaskCreate((osa_task_handle_t)s_task_DebugLogHandle, OSA_TASK(task_DebugLog), NULL);
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
#define SERIANETNUM		(3)//单包数据要透传的次数
void SwTimer500msCallback(TimerHandle_t xTimer)
{
	static uint32_t cycliCnt = 0;
	uint8_t deviceId = 0;//一个角色默认为0
	uint16_t valuehandle = (uint16_t)value_SK_RX;
	bleResult_t stRet = 0;
	uint16_t u16Length = ((uint16_t)g_u8SeriaNetData[2] << 8) | (uint16_t)g_u8SeriaNetData[3];

	cycliCnt++;
	if(1 == (cycliCnt % SERIANETNUM))
	{//每10s修改一次数据，也就是同一条数据发送SERIANETNUM次
		nio_ble_message_protocol_t *pStMessage = (nio_ble_message_protocol_t *)g_u8SeriaNetData;
		memset(g_u8SeriaNetData,0x00,sizeof(g_u8SeriaNetData));
		pStMessage->u8Type = ((cycliCnt / SERIANETNUM) % 26) + 5;//范围5~31
		pStMessage->u8ID = ((cycliCnt / SERIANETNUM) % 4) + 1;//范围1~5

		//范围2~503
		if((u16Length >= 2) && (u16Length <= 52))
		{
			u16Length += 3;
		}
		else if((u16Length > 52) && (u16Length <= 500))
		{
			u16Length += 50;
		}
		else
		{
			u16Length = 2;
		}

		g_u8SeriaNetData[2] = (uint8_t)(u16Length >> 8);
		g_u8SeriaNetData[3] = (uint8_t)(u16Length & 0x00ff);
		memset(&g_u8SeriaNetData[4],0x55,(u16Length - 1));
		g_u8SeriaNetData[u16Length - 1 + 4] = 0xAA;//最后一位
	}

	//notify发送给SRF
	stRet = GattServer_SendInstantValueNotification(deviceId,valuehandle,(u16Length + 4),(const uint8_t *)g_u8SeriaNetData);
	dbg_Printf("notify ret:0x%x\r\n",stRet);
	//打印出来对照处理
	dbg_Printf("Index(%d),SeriaNet Data[%d]:",(cycliCnt / SERIANETNUM),(u16Length + 4));
	dbg_Printf_hex(g_u8SeriaNetData,(u16Length + 4));
}

TimerHandle_t SwTimer1msHandle = NULL;
TimerHandle_t SwTimer10msHandle = NULL;
TimerHandle_t SwTimer500msHandle = NULL;

void swTimerCreate(void)
{
        /* The software timer period. */
        #define SW_TIMER_PERIOD_1MS         (1 / portTICK_PERIOD_MS)
        #define SW_TIMER_PERIOD_10MS         (10 / portTICK_PERIOD_MS)
        #define SW_TIMER_PERIOD_500MS         (500 / portTICK_PERIOD_MS)

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

		SwTimer500msHandle = xTimerCreate("SwTimer500ms",          /* Text name. */
									SW_TIMER_PERIOD_500MS, /* Timer period. */
                                 	pdTRUE,             /* Enable auto reload. */
                                 	0,                  /* ID is not used. */
                                 	SwTimer500msCallback);   /* The callback function. */
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
    	hal_wdt_if_feed(0);
#if 0
    	Nm_StateType nmStatePtr;
		Nm_ModeType nmModePtr;
		CanNm_GetState(0, &nmStatePtr, &nmModePtr);

		switch(EcuM_GetCurrentState())
		{
		    case ECUM_State_Init:
		    	EcuM_SetModuleState(ECUM_State_PreSleep);
		    	break;

			case ECUM_State_Normal:
		    	Sleeptestcount++;
		    	if(Sleeptestcount > 70)
		    	{
		    		Sleeptestcount = 0;
		    		test_SetSleepFlag(1);
		    	}
				if(((test_GetSleepFlag() == 1)&&(UdsApi_GetD6UpradeRefreshFlg() == 0)&&(diag_GetSleepFlag() == 1)&&(nmModePtr == NM_BUS_SLEEP)&&(Busoff_GetSleepFlag() ==0))||(Busoff_GetSleepFlag() ==1))
				{
					diag_SetSleepFlag(0);
					test_SetSleepFlag(0);
					Busoff_SetSleepFlag(0);
					Sleeptestcount = 0;
					APP_Gap_StopAdv();
					APP_GapStopConnection();
					/* 停止广播 */
//					Gap_StopExtAdvertising(0xff);
					EcuM_SetModuleState(ECUM_State_PreSleep);
					dbg_Printf("Presleep !\r\n");
				}
				break;

			case ECUM_State_Wakeup:
				//初始化相关唤醒源，清除相关标志位
				EcuM_SetModuleState(ECUM_State_Normal);
				dbg_Printf("\r\nNM Mode is: %d\r\n", nmModePtr);
				BleAppIf_StartConAdv();
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
					dbg_Printf("Presleep End !\r\n");
				}
				break;

			case ECUM_State_Sleep:
				dbg_Printf("Presleep !\r\n");
				if(EcuM_GetUwbSleepFlg() == 1 || uwbSleepRspWaitCnt >= 30)
				{
					dbg_Printf("\r\nNM Mode is: %d\r\n", nmModePtr);
					EcuM_SetUwbSleepFlg(1);
					hal_pin_if_irq_enabled(UWB_INT0_INDEX,HAL_ENABLED_DISABLE);
					vTaskSuspendAll();
					//MCU_lowPW();
					xTaskResumeAll();
					hal_pin_if_irq_enabled(UWB_INT0_INDEX,HAL_ENABLED_ENABLE);
					uciMsg_Init();
					canMsg1ccRx.newMsgRecv_flg = 0;
					last_scanCmd = 0;
					BleTimer_Init();
					Sleeptestcount = 0;
					EcuM_SetModuleState(ECUM_State_Wakeup);
//					canMsg1ccRx.newMsgRecv_flg = 1;
//					canMsg1ccRx.msg.sig.scanCMD = 1;
					uwbSleepRspWaitCnt = 0;
					dbg_Printf("Wakeup !\r\n");
				}
				else
				{
					uwbSleepRspWaitCnt++;
				}
				break;

			default:

				break;
		}
#endif
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
		OSA_TimeDelay(10/portTICK_RATE_MS);
	}
}

void task_BleStack(void *argument)
{
    /* Start Application services (timers, serial manager, low power, led, button, etc..) */
    APP_InitServices();

    /* Start BLE Platform related ressources such as clocks, Link layer and HCI transport to Link Layer */
    (void)APP_InitBle();

    /* Start Host stack */
    BluetoothLEHost_AppInit();
	while(1)
	{
		BluetoothLEHost_HandleMessages();
	}
}

void task_BleService(void *argument)
{
	Init_BleService();
	while(1)
	{
		BleService_HandleMessage();
		OSA_TimeDelay(10/portTICK_RATE_MS);
	}
}

void task_UWB(void *argument)
{
	(void)argument;
	static TickType_t xLastWakeTime;

	uwbTaskApp_Init();
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		uwbTaskApp_PeriodicTask(&xLastWakeTime);
	}
}

void task_DebugLog(void *argument)
{
	//创建信号量
	xSemaphore_debugLog = xSemaphoreCreateBinary();
	if(xSemaphore_debugLog == NULL)
	{
		dbg_Printf("xSemaphore_debugLog create failed.\r\n");
	}

	uint8_t u8Mode = 0;
	uint8_t deviceId = 0;//一个角色默认为0
	uint16_t valuehandle = (uint16_t)value_SK_RX;
	uint16_t u16Length = 0;
	bleResult_t stRet = 0;

	while(1)
	{
		xSemaphoreTake(xSemaphore_debugLog, portMAX_DELAY);

		g_u8UartRxBuff[g_u16UartRxLen] = 0x00;//末尾加上空字符，可以当作字符串打印
		dbg_Printf("uart rx data:%s\r\n",g_u8UartRxBuff);
		dbg_Printf("uart rx hex[%d]:",g_u16UartRxLen);
		dbg_Printf_hex(g_u8UartRxBuff,g_u16UartRxLen);

		if(0 == strncmp("LED",(const char *)g_u8UartRxBuff,3))
        {
            u8Mode = g_u8UartRxBuff[3] - 0x30;
            dbg_Printf("led mode:%d\r\n",u8Mode);
        }
        else if(0 == strncmp("Auth YES",(const char *)g_u8UartRxBuff,8))
        {//ble发送(MType:0x01 Mid:0x02)给SRF表示成功
			g_nio_ble_auth_flag = 0;
			dbg_Printf("'Auth YES' set succ\r\n");
        }
		else if(0 == strncmp("Auth NO",(const char *)g_u8UartRxBuff,7))
        {//ble发送(MType:0x01 Mid:0x04)给SRF表示失败
			g_nio_ble_auth_flag = 1;
			dbg_Printf("'Auth NO' set succ\r\n");
		}
		else if(0 == strncmp("SeriaNet Start",(const char *)g_u8UartRxBuff,14))
        {//启动周期发送透传数据功能
			xTimerStart(SwTimer500msHandle, 0);
		}
		else if(0 == strncmp("SeriaNet Stop",(const char *)g_u8UartRxBuff,13))
        {//停止周期发送透传数据功能
			xTimerStop(SwTimer500msHandle, 0);
		}
		else if(0 == strncmp("Send Waring",(const char *)g_u8UartRxBuff,11))
        {//发送告警信息
			typedef struct
			{
				uint32_t rolling_Counter;
				uint16_t Keyfob_Btatt_Vtg;
				uint8_t KeyFob_Batt_Pct;
				uint8_t Keyfob_IsBattLow;
				uint8_t Keyfob_Temp;
				uint8_t Keyfob_Montion_Sts;
			}param_t;
			param_t *pParam = NULL;

			memset(g_u8MessageData,0x00,sizeof(g_u8MessageData));
			g_pStMessage = (nio_ble_message_protocol_t *)g_u8MessageData;
			g_pStMessage->u8Type = 0x1A;
			g_pStMessage->u8ID = 0x01;
			u16Length = 0x000A;
			g_u8MessageData[2] = (uint8_t)(u16Length >> 8);
			g_u8MessageData[3] = (uint8_t)(u16Length & 0x00ff);
			pParam = (param_t *)&g_u8MessageData[4];
			pParam->rolling_Counter = 0x00123456;
			pParam->Keyfob_Btatt_Vtg = 0x1A1A;
			pParam->KeyFob_Batt_Pct = 0x2A;
			pParam->Keyfob_IsBattLow = 0x01;
			pParam->Keyfob_Temp = 0x3A;
			pParam->Keyfob_Montion_Sts = 0x01;
			//notify发送给SRF
			stRet = GattServer_SendInstantValueNotification(deviceId,valuehandle,(u16Length + 4),(const uint8_t *)g_u8MessageData);
			dbg_Printf("notify ret:0x%x\r\n",stRet);
		}
		else if(0 == strncmp("Send Long",(const char *)g_u8UartRxBuff,9))
        {//发送超过mtu数据测试
			if(g_u8UartRxBuff[9] < 0x30)
			{
				u8Mode = 0;
			}
			else
			{
				u8Mode = g_u8UartRxBuff[9] - 0x30;
			}

			memset(g_u8MessageData,0x00,sizeof(g_u8MessageData));
			u16Length = gAttMaxMtu_c - u8Mode;
			for(uint16_t i = 0;i < u16Length;i++)
			{
				g_u8MessageData[i] = i;
			}
			//notify发送给SRF
			#if 1
			stRet = GattServer_SendInstantValueNotification(deviceId,valuehandle,u16Length,(const uint8_t *)g_u8MessageData);
			#else
			stRet = GattDb_WriteAttribute(valuehandle, u16Length, g_u8MessageData);
			dbg_Printf("value set:0x%x\r\n",stRet);//0x605,gGattInvalidValueLength_c,超度超出gAttMaxWriteDataSize_d(gAttMaxMtu_c)限制
			stRet = GattServer_SendNotification(deviceId, valuehandle);
			#endif
			dbg_Printf("notify ret:0x%x\r\n",stRet);
			dbg_Printf("Send Long Data[%d]:",u16Length);
			dbg_Printf_hex(g_u8MessageData,u16Length);
		}
		else if(0 == strncmp("Get pair info",(const char *)g_u8UartRxBuff,13))
        {//获取配对信息
			uint8_t u8OutBondedDevicesCount	 = 0;
			Gap_GetBondedDevicesCount(&u8OutBondedDevicesCount);
			dbg_Printf("Bonded num:%d\r\n",u8OutBondedDevicesCount);
			//Gap_GetBondedDeviceName();
			//Gap_GetBondedDevicesIdentityInformation();
		}
		else if(0 == strncmp("Clear pair info",(const char *)g_u8UartRxBuff,15))
        {//清空所有配对信息
			Gap_RemoveAllBonds();
			//Gap_RemoveBond();
		}
		else if(0 == strncmp("NM Wakeup",(const char *)g_u8UartRxBuff,9))
		{//NM主动唤醒操作
			u8Mode = g_u8UartRxBuff[9] - 0x30;
			if(0 == u8Mode)
			{//请求通道
				CanNm_NetworkRequest(0);
			}
			else if(1 == u8Mode)
			{//释放唤醒
				CanNm_NetworkRelease(0);
			}
		}
		else
        {
        }

		//清空参数
		memset(g_u8UartRxBuff,0,sizeof(g_u8UartRxBuff));
		g_u16UartRxLen = 0;
		g_u8UartRxFlag = 0;
	}
}


