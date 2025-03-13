/*
 * bleTimer.c
 *
 *  Created on: 2024年7月12日
 *      Author: F704012276_
 */

#include "bleTimer_cfg.h"
#include "drv_api.h"

/*------------------------------Private define--------------------------------*/
#define SW_BLE_APP_TIMER_MS					10
#define SW_BLE_APP_TIMER_CNT				10

#define T_Start_Pair_timeout				10 * 1000		/*10s*/
#define T_Start_Pair_Complete_timeout		30 * 1000		/*30s*/
#define T_Pending_for_Adv_timeout			1  * 1500		/*2s*/
#define T_SP_Auth_Tmot_timeout				10 * 1000		/*10s*/
#define T_SKF_Auth_Tmot_timeout				5  * 1000		/*5s*/

#define T_Pending_for_Adv_Scan_timeout		200* 1000		/*200s*/
#define T_Auth_Valid_timeout				10 * 1000		/*10s*/
#define T_BLEImmo_timeout					2  * 60   * 1000		/*120s*/
#define T_PhoneKey_SupervisionTO_timeout	3  * 1000		/*3s*/
#define T_KeyFob_SupervisionTO_timeout		4  * 1000		/*4s*/

#define T_AuthFail_RestartAdv_timeout		5  * 1000		/*5s*/

#define T_NioAdv_App_timeout				30
#define T_NioAdv_IBeacon_timeout			100

#define T_BleCanTpFC_timeout					50
#define T_BleCanTpSTMin_timeout					10


/*--------------------------------Private Variable----------------------------------*/
static bleAppTimer_t bleAppTimerArray[gAppMaxConnections_c];
static uint32_t bleAppTimeroutCnt[bleAppTimerCnt] = {
		T_Start_Pair_timeout,\
		T_Start_Pair_Complete_timeout, \
		T_Pending_for_Adv_timeout,\
		T_SP_Auth_Tmot_timeout, \
		T_SKF_Auth_Tmot_timeout,\

		T_Pending_for_Adv_Scan_timeout,\
		T_Auth_Valid_timeout,\
		T_BLEImmo_timeout,\
		T_PhoneKey_SupervisionTO_timeout,\
		T_KeyFob_SupervisionTO_timeout, \

		T_AuthFail_RestartAdv_timeout,	\
		T_NioAdv_App_timeout,	\
		T_NioAdv_IBeacon_timeout, \
		T_BleCanTpFC_timeout,
};

/*----------------------------------------Private Function--------------------------------------------*/
static void BleTimerInformBleApp(void);

/*-----------------------------------------Function Define------------------------------------------------*/

/* BLE Session Timer Callback */
void BleAppTimerCallback(void)
{
	for(uint8_t id = 0; id < gAppMaxConnections_c; id++ )
	{
		for(uint8_t bits = 0; bits < bleAppTimerCnt; bits++)
		{
			if( bleAppTimerArray[id].timerStartflag.data & (1 << bits) )
			{
				bleAppTimerArray[id].timerOutCnt.data[bits]++;
			}

			if( bleAppTimerArray[id].timerOutCnt.data[bits] >= bleAppTimeroutCnt[bits])	/* time out */
			{
				bleAppTimerArray[id].timerOutCnt.data[bits] = 0;
				bleAppTimerArray[id].timeroutflag.data |= 1 << bits;
				bleAppTimerArray[id].timerStartflag.data &= ~(1 << bits);

				if(mpfBleAppEventHandler != NULL)
				{
					appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(bleAppTimerEvent_t));
					if(pEventData != NULL)
					{
						pEventData->appEvent = mAppEvt_TimeOut_c;
						pEventData->eventData.pData = &pEventData[1];
						bleAppTimerEvent_t* bleAppTimerEvent = pEventData->eventData.pData;
						bleAppTimerEvent->device_id = id;
						bleAppTimerEvent->timeout_flag = bits;	/* Time out event trigger */
						bleAppTimerEvent = NULL;
						if (gBleSuccess_c != BleApp_EventCallback(mpfBleAppEventHandler, pEventData))
						{
							(void)MEM_BufferFree(pEventData);
						}
					}
				}
			}
		}
	}
}

void BleTimer_Init(void)
{
	(void)memset(bleAppTimerArray, 0, sizeof(bleAppTimer_t)*gAppMaxConnections_c);
}

bleAppTimer_t* Get_BleTimerInfo(void)
{
	return bleAppTimerArray;
}

void Start_BleTimer(uint8_t device_id, uint8_t timerId)
{
	bleAppTimerArray[device_id].timerStartflag.data |= 1 << timerId;
}

void Set_BleTimerTimeoutValue(uint8_t device_id, uint8_t timerId, uint32_t timeoutVal)
{
	bleAppTimerArray[device_id].timerOutCnt.data[timerId] = timeoutVal;
}

uint8_t Get_TpTimer_TimeoutFlag(uint8_t device_id, uint8_t timerId)
{
	return bleAppTimerArray[device_id].timeroutflag.data & (1<<timerId);
}

void Clear_BleTimer(uint8_t device_id, uint8_t timerId)
{
	bleAppTimerArray[device_id].timerStartflag.data &= ~(1 << timerId);
	bleAppTimerArray[device_id].timeroutflag.data &= ~(1 << timerId);
	bleAppTimerArray[device_id].timerOutCnt.data[timerId] = 0;
}

void Reset_BleTimer(uint8_t device_id)
{
	(void)memset(&(bleAppTimerArray[device_id]), 0, sizeof(bleAppTimer_t));
}

static void BleTimerInformBleApp(void)
{
	/* when timer cnt reached, add message to mAppCbInputQueue */

}
