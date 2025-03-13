/*
 * uciMessageRanging.c
 *
 *  Created on: 2024年7月23日
 *      Author: F704010638_
 */

#include <uciMsg.h>
#include "phscaUci.h"
#include <UWB_CAN.h>
#include "logger.h"

uint8_t rangeStartingFlg = 0;
uint8_t rangeResultUpdataFlag = 0;
ucimsg_RangeFormat_t rangeResult = {0};
uint32_t rangeResultNotifTimeout_T = 0;
void uciMsgRanging_RangeStartCMD(void)
{
	logPrintf("\r\n UWB Startup !\r\n");
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_RANGE;
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

void uciMsgRanging_RangeStopCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_RANGE;
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

void uciMsgRanging_RangeSessionParaCfgCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_RANGE;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_RANGE_SESSION_PARA_CONFIG;
	memcpy(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payload, UWB_Info_Ntf_t.msg.data, UWB_Info_Ntf_t.DLC);
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = UWB_Info_Ntf_t.DLC;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}

void uciMsgRanging_RangeURSKcfgCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_RANGE;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_RANGE_URSK_CONFIG;
	memcpy(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payload, UWB_Info_Ntf_Sup_t.msg.data, UWB_Info_Ntf_Sup_t.DLC);
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = UWB_Info_Ntf_Sup_t.DLC;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}

void uciMsgRanging_RangeBleMacAddcfgCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_RANGE;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_RANGE_BLEMACADD_CONFIG;
	memcpy(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payload, &BLE_SnifferParam_Dev3_t.msg.data[1], 6);
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 6;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
	//dbg_Printf("\r\n UWB ADDmarkaddress !\r\n");
}

void uciMsgRanging_RangeRestartCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_RANGE;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_RANGE_RESTART_CONFIG;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}

static void uciMsgRanging_GetRangingCountCMD(void)
{

}

static void uciMsgRanging_RangResumeCMD(void)
{

}

static void uciMsgRanging_RangRecoverCMD(void)
{

}

/*********************************************************
 * Public functions
 *********************************************************/
void uciMsgRanging_IndicationHandle(void)
{
	if(UWB_Info_Ntf_t.On_line_flag == 1)
	{
		uciMsgRanging_RangeSessionParaCfgCMD();
		UWB_Info_Ntf_t.On_line_flag = 0;
	}

	if(UWB_Info_Ntf_Sup_t.On_line_flag == 1)
	{
		uciMsgRanging_RangeURSKcfgCMD();
		UWB_Info_Ntf_Sup_t.On_line_flag = 0;
	}

	if(rangeStartingFlg == 1 && rangeResultUpdataFlag == 1)
	{
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev1_0 = (uint8_t)rangeResult.distacne[0];
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev1_1 = (uint8_t)(rangeResult.distacne[0] >> 8);
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev2_0 = (uint8_t)rangeResult.distacne[1];
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev2_1 = (uint8_t)(rangeResult.distacne[1] >> 8);
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev3_0 = (uint8_t)rangeResult.distacne[2];
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev3_1 = (uint8_t)(rangeResult.distacne[2] >> 8);
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev4_0 = (uint8_t)rangeResult.distacne[3];
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev4_1 = (uint8_t)(rangeResult.distacne[3] >> 8);
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev5_0 = (uint8_t)rangeResult.distacne[3];
		BTA1_RSSI_UWB_t.msg.sig.BTA1_UWB_Dist_Dev5_1 = (uint8_t)(rangeResult.distacne[3] >> 8);
		SetKW45_UWBFaultState(1);
		rangeResultNotifTimeout_T = uciMsgTimerCreate(1000);
	}

	if((rangeStartingFlg == 1) && (uciMsgTimerIsExpired(rangeResultNotifTimeout_T) == 1))
	{
		SetKW45_UWBFaultState(0);
		rangeResultNotifTimeout_T = uciMsgTimerCreate(1000);
	}
}
uint8_t uciMsgRanging_IndicationRSP(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	/* If device is in error state or not supported state, UCI commands are
	 * rejected */
	switch (opcodeId)
	{
		case PHSCA_UCIMSG_OID_RANGE_START:
		{
			rangeStartingFlg = 1;
			rangeResultNotifTimeout_T = uciMsgTimerCreate(1000);
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_STOP:
		{
			rangeStartingFlg = 0;
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_SESSION_PARA_CONFIG:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_URSK_CONFIG:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_BLEMACADD_CONFIG:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_RESTART_CONFIG:
		{
			//rangeStartingFlg = 1;
			break;
		}

		case PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_GET_RANGING_COUNT:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_RESUME:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_RECOVER:
		{
			break;
		}

		default:
		{
			uciMsgState = 0;
			break;
		}
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

uint8_t uciMsgRanging_rspTimeoutHandle(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	/* If device is in error state or not supported state, UCI commands are
	 * rejected */
	switch (opcodeId)
	{
		case PHSCA_UCIMSG_OID_RANGE_START:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_STOP:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_SESSION_PARA_CONFIG:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_URSK_CONFIG:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_BLEMACADD_CONFIG:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_RANGE_RESTART_CONFIG:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_GET_RANGING_COUNT:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_RESUME:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_SESSIONCONTROL_RANGE_RECOVER:
		{
			break;
		}

		default:
		{
			uciMsgState = 0;
			break;
		}
	}
}
