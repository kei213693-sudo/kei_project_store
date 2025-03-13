/*
 * uciMessageSession.c
 *
 *  Created on: 2024年7月23日
 *      Author: F704010638_
 */

#include "phscaUci.h"
#include "uciMsg.h"

/*********************************************************
 * static functions
 *********************************************************/
static void uciMsgSession_InitCMD(void)
{

}

static void uciMsgSession_DeInitCMD(void)
{

}

static void uciMsgSession_SetAppConfigCMD(void)
{

}

static void uciMsgSession_GetAppConfigCMD(void)
{

}

static void uciMsgSession_GetCountCMD(void)
{

}

static void uciMsgSession_GetStateCMD(void)
{

}

static void uciMsgSession_GetPossibleRanMultiplierValueCMD(void)
{

}

/*********************************************************
 * Public functions
 *********************************************************/
uint8_t uciMsgSession_Indication(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	/* If device is in error state or not supported state, UCI commands are rejected. */
	uciMsg_WorkStatus_t uciState = uciMsg_GetUciMsgWorkState();
	if (UCIMSG_WORK_STATE_IDLE != uciState)
	{
		uciMsg_UciMsgErrorStateNotif(PHSCA_UCIMSG_GENERIC_STATUS_REJECTED);	//向CAN总线发送请求被拒绝错误状态通知;
		uciMsgState = 0;
	}
	else
	{
		switch (opcodeId)
		{
			case PHSCA_UCIMSG_OID_SESSIONCFG_INIT:
			{
				uciMsgSession_InitCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_SESSIONCFG_DEINIT:
			{
				uciMsgSession_DeInitCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_SESSIONCFG_SET_APP_CONFIG:
			{
				uciMsgSession_SetAppConfigCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_SESSIONCFG_GET_APP_CONFIG:
			{
				uciMsgSession_GetAppConfigCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_SESSIONCFG_GET_STATE:
			{
				uciMsgSession_GetStateCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_SESSIONCFG_GET_COUNT:
			{
				uciMsgSession_GetCountCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_SESSIONCFG_GET_RAN_MULTIPLIER_VAL:
			{
				uciMsgSession_GetPossibleRanMultiplierValueCMD();
				break;
			}

			default:
			{
				uciMsg_UciMsgErrorStateNotif(PHSCA_UCIMSG_GENERIC_STATUS_UNKNOWN_OID);	//向CAN总线发送未知的oid错误状态通知;
				uciMsgState = 0;
				break;
			}
		}
	}
	return uciMsgState;
}
