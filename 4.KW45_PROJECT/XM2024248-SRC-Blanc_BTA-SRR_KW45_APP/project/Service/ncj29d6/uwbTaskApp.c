/*
 * uwbTaskApp.c
 *
 *  Created on: 2024年7月19日
 *      Author: F704010638_
 */
#include <string.h>
#include "phscaTypes.h"
#include "phscaUci.h"
#include "phscaNcj29d6.h"
#include <uwbTaskApp.h>
#include "uciMsg.h"
#include "app.h"

#define CAN_DEBUG_EN 1
static void uwbTaskApp_UciRspNtfCallback(const uint8_t u8_MessageType, const uint8_t u8_Gid, const uint8_t u8_Oid, const uint32_t u32_PayloadLength, const uint8_t * const u8arr_Payload);
static void SetStartCmd(uint8_t gid);
static void SetStopCmd(uint8_t gid);
static void SetQueryCmd(uint8_t gid);

static ucimsg_RangeFormat_t rangeData;
static ucimsg_CpdFormat_t   cpdData;
static Range_Cpd_DataType	rangeAndCpdResult;
static uint16_t distanceRecvBuff[10];
static uint16_t meanFilterBuff[10];
static uint16_t medianFilterBuff[10];
static uint8_t disRecvCnt = 0;

static uint8_t cpdResultRecvBuff[5];
static uint8_t cpdResultRecvCnt = 0;

uint8_t random_start = 0;
uint8_t random_stop = 0;

static void bubble_sort(uint16_t * dataBuffPtr, uint8_t buffLen);
static uint16_t medianFilterCalcu(uint16_t *dataBuffPtr, uint8_t buffLen);
static uint16_t meanFilterCalcu(uint16_t *dataBuffPtr, uint8_t buffLen);
static uint8_t cpdResultFilterCalcu(uint8_t *dataBuffPtr, uint8_t buffLen);
void uwbTaskApp_Init(void)
{
	/* Initialize Ranging Application */
	if(UdsApi_GetD6UpradeRefreshFlg() == 0)
	{
		phscaUci_Init(uwbTaskApp_UciRspNtfCallback);
	}
}
uint8_t flag = 1;
void uwbTaskApp_PeriodicTask(TickType_t * const xLastWakeTime)
{
#if CAN_DEBUG_EN
	if(UdsApi_GetD6UpradeRefreshFlg() == 0)
	{
		uciMsg_cmdHandler();
		uciMsg_rspNotifHandler();
	}
	vTaskDelay(pdMS_TO_TICKS(40u));

#else
	uint8_t Mode = MODE_RANGE_CPD;


//	if(flag == 1)    //需要控制只发送一次的时候 打开这里
//	{
//		TestMode_Range();
//		flag = 0;
//	}
//	vTaskDelay(55);


	switch(Mode)
	{
		case MODE_RANGE:
			TestMode_Range();
			break;
		case MODE_CPD:
			TestMode_Cpd();
			break;
		case MODE_RANGE_CPD:
			TestMode_Range_Cpd();
			break;
		case MODE_CPD_RANGE:
			TestMode_Cpd_Range();
			break;
		default:
			break;
	}
#endif
}

static void uwbTaskApp_UciRspNtfCallback(const uint8_t u8_MessageType, const uint8_t u8_Gid, const uint8_t u8_Oid, const uint32_t u32_PayloadLength, const uint8_t * const u8arr_Payload)
{
	phscaUci_en_MessageType_t en_MessageType = (phscaUci_en_MessageType_t)u8_MessageType;
	Range_Cpd_DataType *Range_Cpd_Result;
	uint16_t distance;
	uint8_t cpd_result;
	uint8_t cpd_state;
	int32_t delay;
	float motion;
//	uint16_t recvCrc = (((uint16_t)u8arr_Payload[u32_PayloadLength+2] << 8) | u8arr_Payload[u32_PayloadLength+3]);
//	uint16_t calcuCrc = crc16_xmodem(u8arr_Payload,u32_PayloadLength+2);
//	if(recvCrc == calcuCrc)
//	{
	switch(en_MessageType)
	{
		case PHSCAUCI_MESSAGETYPE_RESPONSE:
			memset(uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendEnd].uciData, 0, sizeof(uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendEnd].uciData));
			memcpy(uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendEnd].uciData, u8arr_Payload,u32_PayloadLength+2);
			uciMsg_Rsp.sendEnd++;
			if(uciMsg_Rsp.sendEnd >= UCIMSG_RSP_BUFF_LEN)
			{
				uciMsg_Rsp.sendEnd = 0;
				uciMsg_Rsp.sendEndRoundCnt++;
			}
		//	EcuM_SetUwbSleepFlg(0);
			#if !CAN_DEBUG_EN
			dbg_Printf("uciMsg_Rsp.uciMsg.uciInfo.gid:%d\n\r",uciMsg_Rsp.uciMsg.uciInfo.gid);
			dbg_Printf("uciMsg_Rsp.uciMsg.uciInfo.oid:%d\n\r",uciMsg_Rsp.uciMsg.uciInfo.oid);
			dbg_Printf("uciMsg_Rsp.uciMsg.uciInfo.payload[0]:%d\n\r",uciMsg_Rsp.uciMsg.uciInfo.payload[0]);
			#endif
			break;
		case PHSCAUCI_MESSAGETYPE_NOTIFICATION:
			memset(uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciData, 0, sizeof(uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciData));
			memcpy(uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciData, u8arr_Payload,u32_PayloadLength+2);
			uciMsg_Ntf.sendEnd++;
			if(uciMsg_Ntf.sendEnd >= UCIMSG_NTF_BUFF_LEN)
			{
				uciMsg_Ntf.sendEnd = 0;
				uciMsg_Ntf.sendEndRoundCnt++;
			}
			if(uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciInfo.gid == 0x04 && uciMsg_Ntf.uciMsg[uciMsg_Ntf.sendEnd].uciInfo.oid == 0x02)
			{
				Range_Cpd_Result = (Range_Cpd_DataType *) (&(u8arr_Payload[4]));
				if(disRecvCnt >= 10)
				{
					disRecvCnt = 0;
				}
				distanceRecvBuff[disRecvCnt++] = Range_Cpd_Result->distance1[0];

				if(cpdResultRecvCnt >= 5)
				{
					cpdResultRecvCnt = 0;
				}
				cpdResultRecvBuff[cpdResultRecvCnt++] = Range_Cpd_Result->result;

			//	rangeAndCpdResult.result = Range_Cpd_Result->result;
				rangeAndCpdResult.motion = Range_Cpd_Result->motion;
				rangeAndCpdResult.frq = Range_Cpd_Result->frq;
				rangeAndCpdResult.cpd_state = Range_Cpd_Result->cpd_state;
				rangeAndCpdResult.range_state = Range_Cpd_Result->range_state;
				rangeAndCpdResult.amp_bfeature = Range_Cpd_Result->amp_bfeature;
				rangeAndCpdResult.phs_bfeature = Range_Cpd_Result->phs_bfeature;
			}

			#if !CAN_DEBUG_EN
			Range_Cpd_Result = (Range_Cpd_DataType *) (&(u8arr_Payload[4]));
			distance = (uint16_t)Range_Cpd_Result -> distance1;
			cpd_result = (uint8_t)Range_Cpd_Result -> result;
			cpd_state = (uint8_t)Range_Cpd_Result -> cpd_state;
			//motion = (float)Range_Cpd_Result -> motion;
			delay = (int32_t)Range_Cpd_Result->delay;
			memcpy(uciMsg_Rsp.uciMsg.uciData, u8arr_Payload,u32_PayloadLength+4);
			dbg_Printf("NOTIFICATION uciMsg_Rsp.uciMsg.uciInfo.gid:%d\n\r",uciMsg_Rsp.uciMsg.uciInfo.gid);
			dbg_Printf("NOTIFICATION uciMsg_Rsp.uciMsg.uciInfo.oid:%d\n\r",uciMsg_Rsp.uciMsg.uciInfo.oid);

			dbg_Printf("NOTIFICATION distance:%d\n\r",distance);
			//dbg_Printf("NOTIFICATION delay:%d\n\r",delay);
			if(cpd_state == 1)
			{
				dbg_Printf("NOTIFICATION cpd_result:%d\n\r",cpd_result);
			}
			#endif
			break;
		default:
			break;
	}
}

void uwbTaskApp_29d6WeakupCmd(void)
{
	phscaNcj29d6_SetCs(1);
	vTaskDelay(200);
	phscaNcj29d6_SetCs(0);
}

Range_Cpd_DataType* uwbTaskApp_GetRangeAndCpdResult(void)
{
	static uint8_t meanFilterCnt = 0;
	uint16_t disFilterValue = 0;
	uint8_t cpdFilterValue = 0;
	for(uint8_t i = 0; i < 10; i++)
	{
		medianFilterBuff[i] = distanceRecvBuff[i];
	}

	if(meanFilterCnt >= 10)
	{
		meanFilterCnt = 0;
	}
	meanFilterBuff[meanFilterCnt++] = medianFilterCalcu(medianFilterBuff,10);
	disFilterValue = meanFilterCalcu(meanFilterBuff,10);
	rangeAndCpdResult.distance1[0] = disFilterValue;

	cpdFilterValue = cpdResultFilterCalcu(cpdResultRecvBuff, cpdResultRecvCnt);
	rangeAndCpdResult.result = cpdFilterValue;
	return &rangeAndCpdResult;
}

static void bubble_sort(uint16_t * dataBuffPtr, uint8_t buffLen)
{
	uint8_t i,j;
	for(i = 0; i < buffLen - 1; i++)
	{
		for(j = 0; j < buffLen - i - 1; j++)
		{
			uint16_t tempData;
			if(dataBuffPtr[j] >  dataBuffPtr[j+1])
			{
				tempData = dataBuffPtr[j];
				dataBuffPtr[j] = dataBuffPtr[j+1];
				dataBuffPtr[j+1] = tempData;
			}
		}
	}
}

static uint16_t medianFilterCalcu(uint16_t *dataBuffPtr, uint8_t buffLen)
{
	uint16_t medianValue = 0;
	bubble_sort(dataBuffPtr, buffLen);
	medianValue = dataBuffPtr[buffLen / 2 - 1];
	return medianValue;
}

static uint16_t meanFilterCalcu(uint16_t *dataBuffPtr, uint8_t buffLen)
{
	uint16_t meanValue = 0;
	uint32_t SumValue = 0;
	for(uint8_t i = 0; i < buffLen; i++)
	{
		SumValue += dataBuffPtr[i];
	}
	meanValue = (uint16_t)(SumValue / buffLen);
	return meanValue;
}

static uint8_t cpdResultFilterCalcu(uint8_t *dataBuffPtr, uint8_t buffLen)
{
	uint8_t cpdStateNoneCnt = 0;
	uint8_t cpdStateMotionCnt = 0;
	uint8_t cpdStateBreatheCnt = 0;
	for(uint8_t i = 0; i < buffLen; i++)
	{
		if(dataBuffPtr[i] == 1)
		{
			cpdStateBreatheCnt++;
		}
		else if(dataBuffPtr[i] == 2)
		{
			cpdStateMotionCnt++;
		}
		else
		{
			cpdStateNoneCnt++;
		}
	}

	if(cpdStateBreatheCnt >= cpdStateMotionCnt && cpdStateBreatheCnt >= cpdStateNoneCnt)
	{
		return 1;
	}
	else if(cpdStateMotionCnt >= cpdStateBreatheCnt && cpdStateMotionCnt >= cpdStateNoneCnt)
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

void TestMode_Range()
{
	/*   打开测距    */
	SetStartCmd(PHSCA_UCIMSG_GROUP_ID_RANGE);
	dbg_Printf("Send Cmd:EN RANGE\r\n");
	phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
	random_stop = xTaskGetTickCount()%100 + 10;   //10~110s之后关闭range
	dbg_Printf("random_stop %d\r\n",random_stop);
	vTaskDelay(random_stop * 1000);

	/*   关闭测距    */
	dbg_Printf("random_stop\r\n");
	SetStopCmd(PHSCA_UCIMSG_GROUP_ID_RANGE);
	dbg_Printf("Send Cmd:DN RANGE\r\n");
	phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
	random_start = xTaskGetTickCount()%10 + 5;   //5~12s之后重启
	dbg_Printf("random_start %d\r\n",random_start);
	vTaskDelay(random_start * 1000);
}

void TestMode_Cpd()
{
	SetStartCmd(PHSCA_UCIMSG_GROUP_ID_CPD);
	dbg_Printf("Send Cmd:EN RADAR\r\n");
	phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
	random_stop = xTaskGetTickCount()%30 + 10;   //10~40s之后关闭range
	vTaskDelay(random_stop * 1000);

	SetStopCmd(PHSCA_UCIMSG_GROUP_ID_CPD);
	dbg_Printf("Send Cmd:DN RADAR\r\n");
	phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
	random_start = xTaskGetTickCount()%10 + 5;   //2~12s之后重启
	vTaskDelay(random_start * 1000);
}

void TestMode_Range_Cpd()
{
	/*        首先打开测距          */
	SetStartCmd(PHSCA_UCIMSG_GROUP_ID_RANGE);
	phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
	random_start = xTaskGetTickCount()%10 + 5;   //5~12s之后打开cpd
	vTaskDelay(random_stop * 1000);

	while(1)                 //  在这里while(1)，可能存在风险
	{
		/*        打开呼吸          */
		dbg_Printf("************Send Cmd:EN RADAR************\r\n");
		SetStartCmd(PHSCA_UCIMSG_GROUP_ID_CPD);
		phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
		random_stop = xTaskGetTickCount()%20 + 10;   //10~30s之后关闭cpd
		vTaskDelay(random_stop * 1000);

		/*        关闭呼吸          */
		dbg_Printf("************Send Cmd:DN RADAR************\r\n");
		SetStopCmd(PHSCA_UCIMSG_GROUP_ID_CPD);
		phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
		random_start = xTaskGetTickCount()%10 + 5;   //5~12s之后重启
		vTaskDelay(random_start * 1000);
	}
}

void TestMode_Cpd_Range()
{
	/*        首先打开呼吸          */
	SetStartCmd(PHSCA_UCIMSG_GROUP_ID_CPD);
	phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
	random_start = xTaskGetTickCount()%10 + 5;   //5~15s之后打开range
	vTaskDelay(random_stop * 1000);

	while(1)                 //  在这里while(1)，可能存在风险
	{
		/*        打开测距          */
		SetStartCmd(PHSCA_UCIMSG_GROUP_ID_RANGE);
		phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
		random_stop = xTaskGetTickCount()%20 + 10;   //10~100s之后关闭range
		vTaskDelay(random_stop * 1000);

		/*        关闭测距          */
		SetStopCmd(PHSCA_UCIMSG_GROUP_ID_RANGE);
		phscaUci_SendCommand(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciData,uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendHead].uciInfo.payloadLength);
		random_start = xTaskGetTickCount()%10 + 2;   //2~12s之后重启
		vTaskDelay(random_start * 1000);
	}
}


static void SetStartCmd(uint8_t gid){
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = gid;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_RANGE_START;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}

static void SetStopCmd(uint8_t gid){
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = gid;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_RANGE_STOP;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}
static void SetQueryCmd(uint8_t gid){
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = gid;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_RANGE_QUERY;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}
