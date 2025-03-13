/*
 * RFData_Operation_If.h
 *
 *  Created on: 2024年8月1日
 *      Author: F704013166_
 */

#ifndef RF_2910_RFDATA_OPERATION_IF_H_
#define RF_2910_RFDATA_OPERATION_IF_H_

#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "canbus_app.h"

#define MAX_RF_QUEUESIZE 16
typedef struct
{
	BtaOutputRfData_msg message[MAX_RF_QUEUESIZE];
	uint8_t front;
	uint8_t rear;
	uint8_t size;
}RFDataQueue;

extern RFDataQueue RF_DataQueue;;
extern bool	isMsg293Send;
extern uint16_t  msg293_sendTime;

extern void RFData_if_QueueInit(RFDataQueue *queue);
extern bool RFData_if_QueueIsEmpty(RFDataQueue *queue);
extern bool RFData_if_QueuePush(RFDataQueue *queue, BtaOutputRfData_msg msg293);
extern bool RFData_if_QueuePop(RFDataQueue *queue, BtaOutputRfData_msg *msg293);
extern uint8_t RFData_if_GetQueueLenth(RFDataQueue *queue);

#endif /* RF_2910_RFDATA_OPERATION_IF_H_ */
