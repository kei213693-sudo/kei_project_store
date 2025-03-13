/*
 * RFData_Operation_If.c
 *
 *  Created on: 2024年8月1日
 *      Author: F704013166_
 */
#include "fsl_os_abstraction.h"
#include "RFData_Operation_If.h"
RFDataQueue RF_DataQueue;
bool	isMsg293Send = false;
uint16_t  msg293_sendTime = 0;

// 初始化创建链式队列
static void RFData_QueueInit(RFDataQueue *queue)
{
	queue->front = queue->rear = 0;
	queue->size = 0;
}

// 判断队列为空
static bool RFData_QueueIsEmpty(RFDataQueue *queue)
{
	if (queue->size == 0)
		return true;
	else
		return false;
}

// 数据入队
static bool RFData_QueuePush(RFDataQueue *queue, BtaOutputRfData_msg msg)
{
	if (queue->size == MAX_RF_QUEUESIZE)
		return false;

	queue->message[queue->rear] = msg;
	queue->rear = (queue->rear + 1) % MAX_RF_QUEUESIZE;
	queue->size++;
	return true;
}

// 数据出队
static bool RFData_QueuePop(RFDataQueue *queue, BtaOutputRfData_msg *message)
{
	if (queue->size == MAX_RF_QUEUESIZE)
		return false;

	*message = queue->message[queue->front];
	queue->front = (queue->front + 1) % MAX_RF_QUEUESIZE;
	queue->size--;
	return true;
}

// 获取队列长度
static uint8_t RFData_GetQueueLenth(RFDataQueue *queue)
{
	return queue->size;
}

// 初始化队列对外接口
void RFData_if_QueueInit(RFDataQueue *queue)
{
	RFData_QueueInit(queue);
}

// 队列是否为空对外接口
bool RFData_if_QueueIsEmpty(RFDataQueue *queue)
{
	return RFData_QueueIsEmpty(queue);
}

// 数据入队对外接口
bool RFData_if_QueuePush(RFDataQueue *queue, BtaOutputRfData_msg msg293)
{
	taskENTER_CRITICAL();
	bool ret = RFData_QueuePush(queue, msg293);
	taskEXIT_CRITICAL();
	return ret;
}

// 数据出队对外接口
bool RFData_if_QueuePop(RFDataQueue *queue, BtaOutputRfData_msg *msg293)
{
	taskENTER_CRITICAL();
	bool ret = RFData_QueuePop(queue, msg293);
	taskEXIT_CRITICAL();
	return ret;
}

// 获取队列长度数据接口
uint8_t RFData_if_GetQueueLenth(RFDataQueue *queue)
{
	return RFData_GetQueueLenth(queue);
}

// RF缓存数据一次性通过CAN发送接口
//void RFData_if_SendMsg(void)
//{
//	BtaOutputRfData_msg		msg0x293;
//
//	RFData_if_QueuePop(&RFData_queue, msg0x293);
//
//	canbusTxMsgFast(0x293, 1,0,1);
//}

