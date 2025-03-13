/*
***************************************************************************************
* FileName:  canbus_trans.h
* Description:
* Date:
***************************************************************************************
*/
#ifndef CANBUS_TRANS_H__
#define CANBUS_TRANS_H__

#include "list_fifo.h"

#define CANBUS_SEND_USE_TASK 1

#define CANBUS_SUCCESS	0
#define CANBUS_FAIL		1

typedef struct
{
	uint32_t id;                          /* ID */
	uint8_t len;                          /* 数据长度 */
	uint8_t data[64];    				  /* 数据 */
}CanbusDataBuffType;

typedef struct
{
	LIST_FIFO_STRUCT_TYPEDEF *fifoHead;
	uint8_t pushFlag;
	uint8_t popFlag;
}CANBUS_FIFO_TYPEDEF;

extern CANBUS_FIFO_TYPEDEF canbusTransFifo;

void canbusTransInit(void);

uint8_t canbus_fifo_pop_data(CANBUS_FIFO_TYPEDEF *pFifo, uint32_t *id, uint8_t *rlen, uint8_t *rdata);

uint8_t canbusTransSendMsgFrame(uint32_t dwMsgID, uint8_t len, uint8_t *sndbuf);
uint8_t canbusTransSendBusoffMsgFrame(uint32_t dwMsgID, uint8_t len, uint8_t *sndbuf);
uint8_t canbusTransSendMsg(void);

uint8_t CANIf_NM_Transmit(uint8_t channelnum, uint32_t id, uint8_t dlc, uint8_t *data);

uint8_t canbus_send_tpframe(uint8_t addr_type, const uint8_t *data_pu8, uint8_t len_u8);

#endif







