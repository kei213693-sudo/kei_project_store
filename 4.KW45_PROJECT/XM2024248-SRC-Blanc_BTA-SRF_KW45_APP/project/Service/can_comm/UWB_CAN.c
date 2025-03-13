/*
 * UWB_CAN.c
 *
 *  Created on: 2024年7月23日
 *      Author: F704010638_
 */
#include <string.h>
#include "fsl_os_abstraction.h"
#include "UWB_CAN.h"
#include "uciMsg.h"

uint8_t uwbCan_UciMsgTxBuff[UWB_CAN_UCI_RSP_DATA_DLC] = {0};
Can_Rx_MsgBuff	uwbCan_UciMsgRxBuff = {0};

static uint8_t ContinuFrameRecving = 0;
static uint16_t ContinuFrameRecvedDataLen = 0;
static uint8_t ContinuFrameSending = 0;
static uint16_t ContinuFrameToBeSendCnt = 0;

void uwbCan_UciRspDataPreSendCallback(uint8_t index)
{
	uciMsg_PackHead_t canSendMsgHead = {0};
	uint16_t dataLength = 0;
	memset(uwbCan_UciMsgTxBuff, 0, UWB_CAN_UCI_RSP_DATA_DLC);
	if(ContinuFrameSending == 0)
	{
		if(uciMsg_Rsp.rspCanSending == 1)
		{
			dataLength = (uint16_t)(uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payloadLength_H << 8 | uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payloadLength);
			if(dataLength + 4 > 64)
			{
				ContinuFrameSending = 1;
				ContinuFrameToBeSendCnt = dataLength - 60;
				uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.pbf = 1;
				memcpy(uwbCan_UciMsgTxBuff, uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciData, UWB_CAN_UCI_RSP_DATA_DLC);
			}
			else
			{
				uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.pbf = 0;
				memcpy(uwbCan_UciMsgTxBuff, uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciData, dataLength + 4);
				uciMsg_Rsp.rspCanSending = 0;
				uciMsg_Rsp.sendHead++;
				if(uciMsg_Rsp.sendHead >= UCIMSG_RSP_BUFF_LEN)
				{
					uciMsg_Rsp.sendHead = 0;
					uciMsg_Rsp.sendHeadRoundCnt++;
				}
			}
		}
		else if(uciMsg_Ntf.notifCanSending == 1)
		{
			dataLength = (uint16_t)(uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.payloadLength_H << 8 | uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.payloadLength);
			if(dataLength + 4 > 64)
			{
				ContinuFrameSending = 1;
				ContinuFrameToBeSendCnt = dataLength - 60;
				uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.pbf = 1;
				memcpy(uwbCan_UciMsgTxBuff, uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciData, UWB_CAN_UCI_RSP_DATA_DLC);
			}
			else
			{
				uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.pbf = 0;
				memcpy(uwbCan_UciMsgTxBuff, uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciData, dataLength + 4);
				uciMsg_Ntf.notifCanSending = 0;
				uciMsg_Ntf.sendHead++;
				if(uciMsg_Ntf.sendHead >= UCIMSG_NTF_BUFF_LEN)
				{
					uciMsg_Ntf.sendHead = 0;
					uciMsg_Ntf.sendHeadRoundCnt++;
				}
			}
		}
	}
	else
	{
		if(uciMsg_Rsp.rspCanSending == 1)
		{
			dataLength = (uint16_t)(uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payloadLength_H << 8 | uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payloadLength);
			memcpy(canSendMsgHead.Byte, uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciData, 4);
			uint16_t rspDataSendOffset = dataLength - ContinuFrameToBeSendCnt;
			if(ContinuFrameToBeSendCnt + 4 > 64)
			{
				canSendMsgHead.Bits.pbf = 1;
				canSendMsgHead.Bits.payloadLength = (uint8_t)ContinuFrameToBeSendCnt;
				canSendMsgHead.Bits.payloadLength_H = (uint8_t)(ContinuFrameToBeSendCnt >> 8);
				memcpy(uwbCan_UciMsgTxBuff, canSendMsgHead.Byte, 4);
				memcpy(uwbCan_UciMsgTxBuff + 4, uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciData + rspDataSendOffset + 4, 60);
				ContinuFrameToBeSendCnt -= 60;
			}
			else
			{
				canSendMsgHead.Bits.pbf = 0;
				canSendMsgHead.Bits.payloadLength = (uint8_t)ContinuFrameToBeSendCnt;
				canSendMsgHead.Bits.payloadLength_H = (uint8_t)(ContinuFrameToBeSendCnt >> 8);
				memcpy(uwbCan_UciMsgTxBuff, canSendMsgHead.Byte, 4);
				memcpy(uwbCan_UciMsgTxBuff + 4, uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciData + rspDataSendOffset + 4, ContinuFrameToBeSendCnt);
				ContinuFrameToBeSendCnt = 0;
				ContinuFrameSending = 0;
				uciMsg_Rsp.rspCanSending = 0;
				uciMsg_Rsp.sendHead++;
				if(uciMsg_Rsp.sendHead >= UCIMSG_RSP_BUFF_LEN)
				{
					uciMsg_Rsp.sendHead = 0;
					uciMsg_Rsp.sendHeadRoundCnt++;
				}
			}
		}
		else if(uciMsg_Ntf.notifCanSending == 1)
		{
			dataLength = (uint16_t)(uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.payloadLength_H << 8 | uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciInfo.payloadLength);
			memcpy(canSendMsgHead.Byte, uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciData, 4);
			uint16_t notifDataSendOffset = dataLength - ContinuFrameToBeSendCnt;
			if(ContinuFrameToBeSendCnt + 4 > 64)
			{
				canSendMsgHead.Bits.pbf = 1;
				canSendMsgHead.Bits.payloadLength = (uint8_t)ContinuFrameToBeSendCnt;
				canSendMsgHead.Bits.payloadLength_H = (uint8_t)(ContinuFrameToBeSendCnt >> 8);
				memcpy(uwbCan_UciMsgTxBuff, canSendMsgHead.Byte, 4);
				memcpy(uwbCan_UciMsgTxBuff + 4, uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciData + notifDataSendOffset + 4, 60);
				ContinuFrameToBeSendCnt -= 60;
			}
			else
			{
				canSendMsgHead.Bits.pbf = 0;
				canSendMsgHead.Bits.payloadLength = (uint8_t)ContinuFrameToBeSendCnt;
				canSendMsgHead.Bits.payloadLength_H = (uint8_t)(ContinuFrameToBeSendCnt >> 8);
				memcpy(uwbCan_UciMsgTxBuff, canSendMsgHead.Byte, 4);
				memcpy(uwbCan_UciMsgTxBuff + 4, uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendHead].uciData + notifDataSendOffset + 4, ContinuFrameToBeSendCnt);
				ContinuFrameToBeSendCnt = 0;
				ContinuFrameSending = 0;
				uciMsg_Ntf.notifCanSending = 0;
				uciMsg_Ntf.sendHead++;
				if(uciMsg_Ntf.sendHead >= UCIMSG_NTF_BUFF_LEN)
				{
					uciMsg_Ntf.sendHead = 0;
					uciMsg_Ntf.sendHeadRoundCnt++;
				}
			}
		}
	}
}

void uwbCan_UciCmdDataRecvedCallback(uint8_t index)
{
	uciMsg_PackHead_t canRecvMsgHead = {0};

	memcpy(canRecvMsgHead.Byte, uwbCan_UciMsgRxBuff.rx_buff, 4);
	if(UWB_CAN_UCI_CMD_DATA_DLC == uwbCan_UciMsgRxBuff.rx_len)
	{
		if(ContinuFrameRecving == 0)
		{
			memset(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciData, 0, sizeof(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciData));
			OSA_InterruptDisable();
			memcpy(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciData, uwbCan_UciMsgRxBuff.rx_buff, uwbCan_UciMsgRxBuff.rx_len);
			OSA_InterruptEnable();

			if(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf == 1)
			{
				ContinuFrameRecvedDataLen += 60;
				ContinuFrameRecving = 1;
			}
			else
			{
				uciMsg_Cmd.sendEnd++;
				if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
				{
					uciMsg_Cmd.sendEnd = 0;
					uciMsg_Cmd.sendEndRoundCnt++;
				}
			}
		}
		else
		{
			if(canRecvMsgHead.Bits.gid == uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid && canRecvMsgHead.Bits.oid == uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid)
			{
				OSA_InterruptDisable();
				memcpy((uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciData+ContinuFrameRecvedDataLen + 4), uwbCan_UciMsgRxBuff.rx_buff + 4 , uwbCan_UciMsgRxBuff.rx_len -4);
				OSA_InterruptEnable();
				if(canRecvMsgHead.Bits.pbf == 1)
				{
					ContinuFrameRecvedDataLen += 60;
				}
				else
				{
					ContinuFrameRecving = 0;
					ContinuFrameRecvedDataLen = 0;
					uciMsg_Cmd.sendEnd++;
					if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
					{
						uciMsg_Cmd.sendEnd = 0;
						uciMsg_Cmd.sendEndRoundCnt++;
					}
				}
			}
		}
	}
}

void uwbCan_UciCmdDataRecvedTimeutCallback(uint8_t index)
{

}
