/*
 * uciMessageCore.c
 *
 *  Created on: 2024年7月23日
 *      Author: F704010638_
 */
#include <string.h>
#include <uciMsg.h>
#include "phscaUci.h"

/*********************************************************
 * static functions
 *********************************************************/

static void uciMsgCore_DeviceResetCMD(void)
{


}

static void uciMsgCore_GetDeviceInfoCMD(void)
{


}


static void uciMsgCore_GetCapsInfoCMD(void)
{

}

static void uciMsgCore_SetConfigCMD(void)
{

}

static void uciMsgCore_GetConfigCMD(void)
{

}



/*********************************************************
 * Public function
 *********************************************************/

uint8_t UciMsgCore_Indication(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	/* The CORE_RESET command is always allowed. */
	if (PHSCA_UCIMSG_OID_CORE_RESET == opcodeId)
	{
		uciMsgCore_DeviceResetCMD();
	}
	/* If device is in error state or not supported state, other UCI commands are rejected. */
	else
	{
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
				case PHSCA_UCIMSG_OID_CORE_GET_DEVICE_INFO:
				{
					uciMsgCore_GetDeviceInfoCMD();
				}
				break;
				case PHSCA_UCIMSG_OID_CORE_GET_CAPS_INFO:
				{
					uciMsgCore_GetCapsInfoCMD();
				}
				break;
				case PHSCA_UCIMSG_OID_CORE_SET_CONFIG:
				{
					uciMsgCore_SetConfigCMD();
				}
				break;
				case PHSCA_UCIMSG_OID_CORE_GET_CONFIG:
				{
					uciMsgCore_GetConfigCMD();
				}
				break;
				default:
				{
					uciMsg_UciMsgErrorStateNotif(PHSCA_UCIMSG_GENERIC_STATUS_UNKNOWN_OID);	//向CAN总线发送未知的oid错误状态通知;
					uciMsgState = 0;
				}
				break;
			}
		}
	}
	return uciMsgState;
}
