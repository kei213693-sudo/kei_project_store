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
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"
#include "canbus_drv.h"
#include "canbus_trans.h"
#include "queue.h"
#include "autosar_nm.h"
#include "logger.h"
#include "app.h"
#include "ComSM.h"
#define CANBUS_USE_QUEUE

//#define canbusLog	logPrintf
#define canbusLog(...)

#define CANBUS_TRANS_TX_BUFF_LEN      (96U)

#ifdef CANBUS_USE_QUEUE
static QueueHandle_t canbus_xQueue = NULL;
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
		canbusLog("canbus queue send fail. xReturn = %d\r\n", xReturn);
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
		canbusLog("canbus queue send fail. xReturn = %d\r\n", xReturn);
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


/*
*****************************************************************
*name: canbusTransBufferClear
*descrip:
*parameter:
*return:
*****************************************************************
*/
void canbusTransBufferClear(void)
{
	if(NULL != canbus_xQueue)
	{
		canbus_fifo_clear(&canbusTransFifo);
	}
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
#if DEBUG_CANBUS_MSG
uint8_t canFifoFullErrorAppearFlag = 0;
#endif
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

	if((!CanSM_GetAppTxMsgIsEnable()&&(dwMsgID != (0x570U)))||(UdsApi_GetD6UpradeRefreshFlg() == 1))
	{
			return CANBUS_SUCCESS;
	}

	ret = canbus_fifo_push_data(&canbusTransFifo, dwMsgID, len, sndbuf);
	if(ret != CANBUS_SUCCESS)
	{
#if DEBUG_CANBUS_MSG
		uint8_t tdata[8]={0};
		canFifoFullErrorAppearFlag = canbusDrvTransmit(0x333, 8, tdata);
#endif
		canbusLog("canbus fifo push fail, id = 0x%x\r\n", dwMsgID);
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
#if 0
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
#else
        canbusDrvTransmit(dwMsgID, len, sndbuf);
#endif
        return ret;
}
#endif
#if CANBUS_SEND_USE_TASK

bool canbusTransNeedReSend(uint32_t id)
{
	return !(id == BLE_Common_Sts_CANID || id == BLE_SnifferParam_Dev1_CANID);
}

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
	#define CAN_TRANS_TICK_WAIT_TICK	10U
	static uint8_t ret = CANBUS_FAIL;
	static uint32_t dwMsgID, toBeConfirmMsgId;
	static uint8_t msgLen, toBeConfirmMsgLen;
	static uint8_t sndMsgBuf[64], toBeConfirmSndMsgBuf[64];
	static uint8_t send_wait_confirm_flag;
#if DEBUG_CANBUS_MSG
	static uint32_t SetTick, ChkTick;
#endif

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
	OSA_DisableScheduler();//关闭是为了避免被别的任务打断，那为什么不直接设置为最高优先级？
	{
		if(portMAX_DELAY == xTransTicksWait) /* 第一次启动发送不需要发送确认 */
		{
			send_wait_confirm_flag = 0;
		}

		if(send_wait_confirm_flag)
		{
			if(!canbusDrvSendIsConfirm(toBeConfirmMsgId)) /* 发送超时 */
			{
				extern volatile uint32_t CANInt_Cnt;
#if DEBUG_CANBUS_MSG
				ChkTick = SysTimerGetCurrentTime();
				canbusLog("SetTick=%d, ChkTick=%d\r\n", SetTick, ChkTick);
				extern void canbusDrvPrintIdInfo(void);
				canbusDrvPrintIdInfo();
#endif
				canbusLog("send msg is timeout, id = 0x%X, CANInt_Cnt = %d\r\n", toBeConfirmMsgId, CANInt_Cnt);
				if(canbusTransNeedReSend(toBeConfirmMsgId))
				{
					canbusLog("ReSend id = 0x%X msg\r\n", toBeConfirmMsgId);
					ret = canbus_fifo_push_data(&canbusTransFifo, toBeConfirmMsgId, toBeConfirmMsgLen, toBeConfirmSndMsgBuf);
					if(ret != CANBUS_SUCCESS)
					{
						//logPrintf("canbus fifo push fail, tx no confirm, id = 0x%X, msg reinsert queue tail~\r\n", toBeConfirmMsgId);
					}
				}
#if DEBUG_CANBUS_MSG
				uint8_t tdata[8]={0};
				ret = canbusDrvTransmit(0x222, 8, tdata);
#endif
			}
		}

		send_wait_confirm_flag = 0; /* 清除等待发送确认标志 */
		canbusDrvSendClrConfirmId();

		if(CAN_BUSOFF == Get_canBusoffFlag())
		{
			if(!xQueueIsQueueEmptyFromISR(canbus_xQueue))
			{
				canbus_fifo_clear(&canbusTransFifo);
				//logPrintf("discover busoff, in can send task ==> clear tx fifo!\r\n");
			}
		}

#if DEBUG_CANBUS_MSG
		if(canFifoFullErrorAppearFlag)
		{
			uint8_t tdata[8]={0};
			ret = canbusDrvTransmit(0x333, 8, tdata);
			canFifoFullErrorAppearFlag = 0;
			xTransTicksWait = CAN_TRANS_TICK_WAIT_TICK;
		}
		else ret = CANBUS_FAIL;
		if(CANBUS_SUCCESS != ret)
		{
#endif
		if(CANBUS_SUCCESS == canbus_fifo_pop_data(&canbusTransFifo, &dwMsgID, &msgLen, sndMsgBuf))
		{//接收数据
			static uint16_t canSndErrorPrintCnt = 0, canSndErrorCnt = 0;
			static uint32_t canSndErrorCounter = 0;

			ret = canbusDrvTransmit(dwMsgID, msgLen, sndMsgBuf);
			if(ret != CANBUS_SUCCESS)
			{
				canSndErrorCounter ++;
				canSndErrorPrintCnt %= 500;
				if(0 == canSndErrorPrintCnt)
				{
				//	canSndErrorPrintCnt = 0;
					//logPrintf("can send fail, id = 0x%X\r\n", dwMsgID);
				}
				canSndErrorPrintCnt++;
				canSndErrorCnt ++;

				ret = canbus_fifo_push_data_to_front(&canbusTransFifo, dwMsgID, msgLen, sndMsgBuf);
				if(ret != CANBUS_SUCCESS)
				{
					//logPrintf("canbus fifo push fail, msg reinsert queue front, id = 0x%X\r\n", dwMsgID);
				}
				else
				{
					if(1 == canSndErrorPrintCnt)
					{
					//	canbusLog("canbus fifo push succe, msg reinsert queue front, id = 0x%x\r\n", dwMsgID);
					}
				}

				if(canSndErrorCnt >= 60)
				{
					if(xQueueIsQueueFullFromISR(canbus_xQueue))
					{
						canbus_fifo_clear(&canbusTransFifo);
						canSndErrorCnt = 0;
						//logPrintf("discover Send Error, in can send task ==> clear tx fifo!\r\n");
					}
				}
			}
			else
			{
#if DEBUG_CANBUS_MSG
				SetTick = SysTimerGetCurrentTime();
#endif
				canbusDrvSendWaitConfirmId(dwMsgID);
				toBeConfirmMsgId = dwMsgID;
				toBeConfirmMsgLen = msgLen;
				memcpy(toBeConfirmSndMsgBuf, sndMsgBuf, msgLen);

				canSndErrorPrintCnt = 0;
				canSndErrorCounter = 0;
				canSndErrorCnt = 0;
				send_wait_confirm_flag = 1;
				xSemaphoreTake(xSemaphore_canSend, 0);
			}
			xTransTicksWait = CAN_TRANS_TICK_WAIT_TICK;
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
#if DEBUG_CANBUS_MSG
		}
#endif
	}
	OSA_EnableScheduler();
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
		//logPrintf("CAN NM Sending !\r\n");
		return canbusTransSendMsgFrame(id, dlc, data);
	}
	else
	{
		return CANBUS_SUCCESS;
	}
}

/*
*****************************************************************
*name: canbus_send_tpframe
*descrip:
*parameter:
*return:
*****************************************************************
*/
uint8_t canbus_send_tpframe(uint8_t addr_type, const uint8_t *data_pu8, uint8_t len_u8)
{
	uint32_t dwMsgID;
	if(APP_TP_TYPE_BLE_UP_LINK == addr_type)
	{
		dwMsgID = BLE_Uplink_Data_CANID;
	}
	else if(APP_TP_TYPE_BLE_DOWN_LINK_FC == addr_type)
	{
		dwMsgID = BLE_Downlink_FlowCtrl_CANID;
	}
	else if(APP_TP_TYPE_CCC_UP_LINK == addr_type)
	{
		dwMsgID = CccDkBleRsp_CANID;
	}
	else if(APP_TP_TYPE_CCC_DOWN_LINK_FC == addr_type)
	{
		dwMsgID = CccDkBleCmdFlowCtrl_CANID;
	}
	return canbusTransSendMsgFrame(dwMsgID, len_u8, (uint8_t *)data_pu8);
}

















