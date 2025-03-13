/*
 * uciMessageCustom.c
 *
 *  Created on: 2024年7月23日
 *      Author: F704010638_
 */

#include "phscaUci.h"
#include "uciMsg.h"
#include "app.h"
#include "E2Para.h"
#define VEHICLE_CALIBRA_PART_NUM_TAG	0x00
#define VEHICLE_CALIBRA_PART_NUM_LEN	0x0B
#define VEHICLE_CALIBRA_BASE_NUM_TAG	0x01
#define VEHICLE_CALIBRA_BASE_NUM_LEN	0x02
#define VEHICLE_CALIBRA_VERSION_NUM_TAG	0x02
#define VEHICLE_CALIBRA_VERSION_NUM_LEN	0x06

void uciMsgCustom_GetProductInfoCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_CUSTOM;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_GET_PRODUCT_INIF;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}

void uciMsgCustom_LowPowerEnableCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_CUSTOM;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_LOWPOWER_ENABLE;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}
extern bool_t D6VersionFlag;
extern uint8_t D6VersionCnt;
void uciMsgCustom_GetProductInfoRSP(void)
{
	uint8_t uciMsgState = 1;
	uint8_t data[11] = {0};
	uint8_t i = 0;
	/*判断响应数据有效性，提取响应数据*/
	uds_write_flash_data_un DidD6data;
	//if(D6VersionFlag == 1)//&&(D6VersionCnt >= 200))
	//{
		//D6VersionFlag = 0;
		did_F130_set_value(data);
		for(i;i < 11;i++)
		{
			if(uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payload[3+i] != data[i])
			{
				break;
			}
		}
		if(i != 11)
		{
			Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,DidD6data.write_buffer,DID_WRITE_DATA_LEN);
			memcpy(DidD6data.sig.F130_data,&uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payload[3],11);
			memcpy(DidD6data.sig.F131_data,&uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payload[0x10],2);
			memcpy(DidD6data.sig.F151_data,&uciMsg_Rsp.uciMsg[uciMsg_Rsp.sendHead].uciInfo.payload[0x14],6);
			Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID,DidD6data.write_buffer,DID_WRITE_DATA_LEN);
			logPrintfImmedi("D6 version Write success !\r\n");
		}
	//}
}

void uciMsgCustom_LowPowerEnablRSP(void)
{
	uint8_t uciMsgState = 1;
	EcuM_SetUwbSleepFlg(1);
	/*判断响应数据有效性，提取响应数据*/
}

void uciMsgCustom_IndicationHandle(void)
{
	;
}

uint8_t uciMsgCustom_IndicationRSP(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	switch (opcodeId)
	{
		case PHSCA_UCIMSG_OID_GET_PRODUCT_INIF:
		{
			uciMsgCustom_GetProductInfoRSP();
		}
		break;
		case PHSCA_UCIMSG_OID_LOWPOWER_ENABLE:
		{
			uciMsgCustom_LowPowerEnablRSP();
		}
		break;
		default:
		{
			uciMsgState = 0;
		}
		break;
	}
	uciMsg_Rsp.sendHead++;
	if(uciMsg_Rsp.sendHead >= UCIMSG_RSP_BUFF_LEN)
	{
		uciMsg_Rsp.sendHead = 0;
		uciMsg_Rsp.sendHeadRoundCnt++;
	}
	uciMsg_SetUciMsgWorkState(UCIMSG_WORK_STATE_IDLE);
	uciMsg_Cmd.sendHead++;
	if(uciMsg_Cmd.sendHead >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendHead = 0;
		uciMsg_Cmd.sendHeadRoundCnt++;
	}
	return uciMsgState;
}

uint8_t uciMsgCustom_rspTimeoutHandle(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	switch (opcodeId)
	{
		case PHSCA_UCIMSG_OID_GET_PRODUCT_INIF:
		{
			//uciMsgCustom_GetProductInfoRSP();
		}
		break;
		case PHSCA_UCIMSG_OID_LOWPOWER_ENABLE:
		{
			//uciMsgCustom_LowPowerEnablRSP();
		}
		break;
		default:
		{
			uciMsgState = 0;
		}
		break;
	}
}
