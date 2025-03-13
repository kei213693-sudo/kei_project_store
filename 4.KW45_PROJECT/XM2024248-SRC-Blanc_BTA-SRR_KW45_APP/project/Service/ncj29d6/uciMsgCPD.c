/*
 * uciMsgCPD.c
 *
 *  Created on: 2024年8月15日
 *      Author: F704010638_
 */

#include <uciMsg.h>
#include "phscaUci.h"

void uciMsgCPD_CPDStartCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_CPD;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_CPD_START;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}

void uciMsgCPD_CPDStopCMD(void)
{
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.gid = PHSCA_UCIMSG_GROUP_ID_CPD;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.msgType = PHSCAUCI_MESSAGETYPE_COMMAND;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.oid = PHSCA_UCIMSG_OID_CPD_STOP;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.pbf = 0;
	uciMsg_Cmd.uciMsg[uciMsg_Cmd.sendEnd].uciInfo.payloadLength = 0;
	uciMsg_Cmd.sendEnd++;
	if(uciMsg_Cmd.sendEnd >= UCIMSG_CMD_BUFF_LEN)
	{
		uciMsg_Cmd.sendEnd = 0;
		uciMsg_Cmd.sendEndRoundCnt++;
	}
}

/*********************************************************
 * Public functions
 *********************************************************/
uint8_t uciMsgCPD_IndicationRSP(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	/* If device is in error state or not supported state, UCI commands are
	 * rejected */
	switch (opcodeId)
	{
		case PHSCA_UCIMSG_OID_CPD_START:
		{
			break;
		}

		case PHSCA_UCIMSG_OID_CPD_STOP:
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
