/*
 * uciMessageRanging.c
 *
 *  Created on: 2024年7月23日
 *      Author: F704010638_
 */

#include <uciMsg.h>
#include "phscaUci.h"
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"
#include <UWB_CAN.h>
#include "dbg_printf.h"
/*********************************************************
 * static functions
 *********************************************************/
void uciMsgRanging_RangeStartCMD(void)
{
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
	memcpy(uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payload, canMsg1ccRx.msg.sig.scanPara, sizeof(canMsg1ccRx.msg.sig.scanPara));
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = sizeof(canMsg1ccRx.msg.sig.scanPara);
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
uint8_t uciMsgRanging_IndicationRSP(uint8_t const opcodeId)
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
			shell_write("\r\n Mac address Config success\r\n");
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
