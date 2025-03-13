/*
***************************************************************************************
* FileName:  canbus_trans.c
* Description:
* Date:
***************************************************************************************
*/
#include <string.h>
#include "fsl_os_abstraction.h"
#include "list_fifo.h"
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"
#include "canbus_drv.h"
#include "canbus_trans.h"
#include "queue.h"
#include "autosar_nm.h"
#include "app.h"
#include "ComSM.h"
#define CANBUS_USE_QUEUE

//#define canbusLog	logPrintf
#define canbusLog(...)

#define CANBUS_TRANS_TX_BUFF_LEN      (64U)

#ifdef CANBUS_USE_QUEUE
static QueueHandle_t canbus_xQueue;
#else
static CanbusDataBuffType canbusTxBuff[CANBUS_TRANS_TX_BUFF_LEN];
static LIST_FIFO_STRUCT_TYPEDEF  canbusTxFifo[CANBUS_TRANS_TX_BUFF_LEN];
#endif
CANBUS_FIFO_TYPEDEF canbusTransFifo;


#if CANBUS_SEND_USE_TASK
SemaphoreHandle_t xSemaphore_canSend = NULL;
static TickType_t xTransTicksWait = portMAX_DELAY;
#endif

/*
*****************************************************************
*name: canbus_fifo_init
*descrip:
*parameter:
*return:
*****************************************************************
*/
static void canbus_fifo_init(void)
{
#ifdef CANBUS_USE_QUEUE
	if(NULL == canbus_xQueue)
	{
		canbus_xQueue = xQueueCreate(CANBUS_TRANS_TX_BUFF_LEN, sizeof(CanbusDataBuffType));
		if(NULL == canbus_xQueue)
		{
			//canbusLog("CAN Bus create queue fail.\r\n");
			while(1);
		}
	}
#else
    // 初始化链表
    ListFifo_Init(&(canbusTransFifo.fifoHead), canbusTxFifo, CANBUS_TRANS_TX_BUFF_LEN);
    for(uint8_t i = 0; i < CANBUS_TRANS_TX_BUFF_LEN; i ++)	canbusTxFifo[i].para = (void*)&(canbusTxBuff[i]);
    canbusTransFifo.pushFlag = 0;
    canbusTransFifo.popFlag = 0;
#endif
}

/*
*****************************************************************
*name: canbus_fifo_push_data
*descrip:
*parameter:
*return:
*****************************************************************
*/
static uint8_t canbus_fifo_push_data(CANBUS_FIFO_TYPEDEF *pFifo, uint32_t id, uint8_t len, uint8_t *data)
{
#ifdef CANBUS_USE_QUEUE
	CanbusDataBuffType candatabuf;
	candatabuf.id = id;
	candatabuf.len = len;
	memcpy(candatabuf.data, data, len);
	BaseType_t xReturn = xQueueSend(canbus_xQueue, &candatabuf, 0);
	if(xReturn == pdPASS)
	{
		return CANBUS_SUCCESS;
	}
	else
	{
		//canbusLog("canbus queue send fail. xReturn = %d\r\n", xReturn);
		return CANBUS_FAIL;
	}
#else
	LIST_FIFO_STRUCT_TYPEDEF *list_empty;
	CanbusDataBuffType *p_AppData;
	uint8_t *p_data, ret = CANBUS_SUCCESS;

	if(pFifo->popFlag)
	{
		canbusLog("error: canbus fifo is pop_locked! canbus_fifo_push_data()\r\n");
		return CANBUS_FAIL;
	}
	pFifo->pushFlag = 1;
	list_empty = ListFifo_GetEmpty(pFifo->fifoHead);
	if(0 != list_empty)                                                  // 如果有可用节点
	{
		list_empty->fullfg =  1;                                            // 标记接收到数据
		p_AppData = (CanbusDataBuffType *)(list_empty->para);                  // 数据缓存区地址

		p_AppData->len = len;                                               // 记录数据&长度
		p_data = (uint8_t *)p_AppData->data;
		while(len --)
		{
			*p_data ++ = *data ++;
		}
		p_AppData->id = id;
		ret = CANBUS_SUCCESS;
	}
	else
	{
		canbusLog("error: canbus fifo is full! canbus_fifo_push_data()\r\n");
		ret = CANBUS_FAIL;// 如果没有可用节点，，不接收数据
	}
	pFifo->pushFlag = 0;
	return ret;
#endif
}

/*
*****************************************************************
*name: canbus_fifo_push_data_to_front
*descrip:
*parameter:
*return:
*****************************************************************
*/
static uint8_t canbus_fifo_push_data_to_front(CANBUS_FIFO_TYPEDEF *pFifo, uint32_t id, uint8_t len, uint8_t *data)
{
#ifdef CANBUS_USE_QUEUE
	CanbusDataBuffType candatabuf;
	candatabuf.id = id;
	candatabuf.len = len;
	memcpy(candatabuf.data, data, len);
	BaseType_t xReturn = xQueueSendToFront(canbus_xQueue, &candatabuf, 0);
	if(xReturn == pdPASS)
	{
		return CANBUS_SUCCESS;
	}
	else
	{
		//canbusLog("canbus queue send fail. xReturn = %d\r\n", xReturn);
		return CANBUS_FAIL;
	}
#else
	return CANBUS_SUCCESS;
#endif
}

/*
*****************************************************************
*name: canbus_fifo_pop_data
*descrip:
*parameter:
*return:
*****************************************************************
*/
uint8_t canbus_fifo_pop_data(CANBUS_FIFO_TYPEDEF *pFifo, uint32_t *id, uint8_t *rlen, uint8_t *rdata)
{
#ifdef CANBUS_USE_QUEUE
	CanbusDataBuffType candatabuf;
	BaseType_t xReturn = xQueueReceiveFromISR(canbus_xQueue, &candatabuf, NULL);
	if(xReturn == pdPASS)
	{
		*id = candatabuf.id;
		*rlen = candatabuf.len;
		memcpy(rdata, candatabuf.data, candatabuf.len);
		return CANBUS_SUCCESS;
	}
	else
	{
		return CANBUS_FAIL;
	}
#else
	CanbusDataBuffType *p_AppData;
    uint8_t *p_data, len, ret = CANBUS_SUCCESS;

    if(pFifo->pushFlag)
	{
    	canbusLog("warning: canbus fifo is push_locked! canbus_fifo_pop_data()\r\n");
		return CANBUS_FAIL;
	}
    pFifo->popFlag = 1;
    p_AppData = (CanbusDataBuffType *)ListFifo_GetPara(pFifo->fifoHead);          		// 获取数据地址
    if(0 != p_AppData)
    {
		*rlen = p_AppData->len;                                               			// 获取数据&长度
		p_data = (uint8_t *)p_AppData->data;
		len = *rlen;
		while(len --)
		{
			*rdata ++ = *p_data ++;
		}
		*id = p_AppData->id;
		ListFifo_PutHeadToEnd(&(pFifo->fifoHead));                            // 释放节点
		ret = CANBUS_SUCCESS;
    }
    else
    {
    	ret = CANBUS_FAIL;                                               						   // 如果无可用数据，退出
    }
    pFifo->popFlag = 0;
    return ret;
#endif
}

/*
*****************************************************************
*name: canbus_fifo_clear
*descrip:
*parameter:
*return:
*****************************************************************
*/
static uint8_t canbus_fifo_clear(CANBUS_FIFO_TYPEDEF *pFifo)
{
#ifdef CANBUS_USE_QUEUE
	if(!xQueueIsQueueEmptyFromISR(canbus_xQueue))
	{
		xQueueReset(canbus_xQueue);
	}
	return CANBUS_SUCCESS;
#else
    return CANBUS_SUCCESS;
#endif
}

/*
*****************************************************************
*name: canbusTransInit
*descrip:
*parameter:
*return:
*****************************************************************
*/
void canbusTransInit(void)
{
	canbus_fifo_init();
	can_msg_init();
	canbusDrvInit();
}
#if !CANBUS_SEND_USE_TASK
/*
*****************************************************************
*name: canbusTransSendMsgFrame
*descrip:
*parameter:
*return:
*****************************************************************
*/
uint8_t canbusTransSendMsgFrame(uint32_t dwMsgID, uint8_t len, uint8_t *sndbuf)
{
        uint8_t ret = CANBUS_SUCCESS;
        Nm_StateType nmStatePtr;
        Nm_ModeType nmModePtr;

        CanNm_GetState(0, &nmStatePtr, &nmModePtr);

        if(nmModePtr != NM_NETWORK)
        {
                return CANBUS_SUCCESS;
        }

	NVIC_DisableIRQ(CAN0_IRQn);
	if(canbusDrvIsTransmitting())
	{
		ret = canbus_fifo_push_data(&canbusTransFifo, dwMsgID, len, sndbuf);
		if(CANBUS_FAIL == ret)
		{
			ret = canbusDrvTransmit(dwMsgID, len, sndbuf);
		}
	}
	else
	{
		ret = canbusDrvTransmit(dwMsgID, len, sndbuf);
	}
	NVIC_EnableIRQ(CAN0_IRQn);
	return ret;
}
#else
/*
*****************************************************************
*name: canbusTransSendMsgFrame
*descrip:
*parameter:
*return:
*****************************************************************
*/
uint8_t canbusTransSendMsgFrame(uint32_t dwMsgID, uint8_t len, uint8_t *sndbuf)
{
	uint8_t ret = CANBUS_SUCCESS;

	  Nm_StateType nmStatePtr;
	Nm_ModeType nmModePtr;

	CanNm_GetState(0, &nmStatePtr, &nmModePtr);

	if((!CanSM_GetAppTxMsgIsEnable()&&(dwMsgID != (0x573U)))||(UdsApi_GetD6UpradeRefreshFlg() == 1))
	{
			return CANBUS_SUCCESS;
	}

		ret = canbus_fifo_push_data(&canbusTransFifo, dwMsgID, len, sndbuf);
		if(ret != CANBUS_SUCCESS)
		{
			//dbg_Printf("canbus fifo push fail!\r\n id = 0x%x\r\n", dwMsgID);
		}
#if CANBUS_SEND_USE_TASK
	OSA_DisableScheduler();
	if(xTransTicksWait == portMAX_DELAY)
	{
	NVIC_DisableIRQ(CAN0_IRQn);
	extern SemaphoreHandle_t xSemaphore_canSend;
	xSemaphoreGive(xSemaphore_canSend);
	NVIC_EnableIRQ(CAN0_IRQn);
	}
	OSA_EnableScheduler();
#endif

	return ret;
}


uint8_t canbusTransSendBusoffMsgFrame(uint32_t dwMsgID, uint8_t len, uint8_t *sndbuf)
{
        uint8_t ret = CANBUS_SUCCESS;

        if(EcuM_GetCurrentState() != ECUM_State_Normal)
        {
                return CANBUS_SUCCESS;
        }

        ret = canbus_fifo_push_data(&canbusTransFifo, dwMsgID, len, sndbuf);

        if(ret != CANBUS_SUCCESS)
        {
                //logPrintf("canbus fifo push fail!\r\n id = 0x%x\r\n", dwMsgID);
        }

        NVIC_DisableIRQ(CAN0_IRQn);
        extern SemaphoreHandle_t xSemaphore_canSend;
        xSemaphoreGive(xSemaphore_canSend);
        NVIC_EnableIRQ(CAN0_IRQn);

	return ret;
}
#endif
#if CANBUS_SEND_USE_TASK
/*
*****************************************************************
*name: canbusTransSendMsg
*descrip:
*parameter:
*return:
*****************************************************************
*/
uint8_t canbusTransSendMsg(void)
{
	static uint8_t ret = CANBUS_FAIL;
	static uint32_t dwMsgID;
	static uint8_t msgLen;
	static uint8_t sndMsgBuf[64];
	static uint8_t send_wait_confirm_flag;

	if(NULL == xSemaphore_canSend)
	{
		xSemaphore_canSend = xSemaphoreCreateBinary();
		if(xSemaphore_canSend == NULL)
		{
			//logPrintf("CAN Send binary semaphore create failed.\r\n");
			while(1);
		}
	}

	xSemaphoreTake(xSemaphore_canSend, xTransTicksWait);
	{
		if(xTransTicksWait != portMAX_DELAY)
		{
			if(send_wait_confirm_flag)
			{
				if(!canbusDrvSendIsConfirm(dwMsgID)) /* 发送超时 */
				{
					ret = canbus_fifo_push_data(&canbusTransFifo, dwMsgID, msgLen, sndMsgBuf);
					if(ret != CANBUS_SUCCESS)
					{
						//canbusLog("canbus fifo push fail, tx no confirm, msg reinsert queue, id = 0x%x\r\n", last_dwMsgID);
					}
				}
				send_wait_confirm_flag = 0;
			}
		}

		if(CAN_BUSOFF == Get_canBusoffFlag())
		{
			if(!xQueueIsQueueEmptyFromISR(canbus_xQueue))
			{
				canbus_fifo_clear(&canbusTransFifo);
				//logPrintf("discover busoff, in can send task ==> clear tx fifo!\r\n");
			}
		}

		if(CANBUS_SUCCESS == canbus_fifo_pop_data(&canbusTransFifo, &dwMsgID, &msgLen, sndMsgBuf))
		{
			static uint16_t canSndErrorPrintCnt = 0;

			ret = canbusDrvTransmit(dwMsgID, msgLen, sndMsgBuf);
			if(ret != CANBUS_SUCCESS)
			{
				canSndErrorPrintCnt ++;
				if(canSndErrorPrintCnt >= 500)
				{
					canSndErrorPrintCnt = 0;
					//logPrintf("can send fail, id = 0x%X\r\n", dwMsgID);
				}

				ret = canbus_fifo_push_data_to_front(&canbusTransFifo, dwMsgID, msgLen, sndMsgBuf);
				if(ret != CANBUS_SUCCESS)
				{
					//canbusLog("canbus fifo push fail, msg reinsert queue front, id = 0x%x\r\n", dwMsgID);
				}
			}
			else
			{
				canSndErrorPrintCnt = 0;
				send_wait_confirm_flag = 1;
			}
			xTransTicksWait = 2;
		}
		else
		{
			if(xQueueIsQueueEmptyFromISR(canbus_xQueue))
			{
				xTransTicksWait = portMAX_DELAY;
			}
			else
			{
				//logPrintf("error 1299.\r\n");
			}
		}
	}
}
#endif
/*
*****************************************************************
*name: CANIf_NM_Transmit
*descrip:
*parameter:
*return:
*****************************************************************
*/
uint8_t CANIf_NM_Transmit(uint8_t channelnum, uint32_t id, uint8_t dlc, uint8_t *data)
{
	(void)channelnum;
	if(UdsApi_get_nm_tx_sta())
	{
		return canbusTransSendMsgFrame(id, dlc, data);
	}
	else
	{
		return CANBUS_SUCCESS;
	}
}


















