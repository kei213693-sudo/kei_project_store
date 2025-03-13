/*
 * uciMessageProprietary.c
 *
 *  Created on: 2024年7月23日
 *      Author: F704010638_
 */


#include "phscaUci.h"
#include "uciMsg.h"
/*********************************************************
 * static functions
 *********************************************************/
static void uciMsgPropri_RadarConfigDownloadCMD(void)
{

}

static void uciMsgPropri_SwupCMD(void)
{

}

static void uciMsgPropri_DeviceGetRebootReasonCMD(void)
{

}

static void uciMsgPropri_TestStartCMD(void)
{

}

static void uciMsgPropri_TeatStopCMD(void)
{

}

static void uciMsgPropri_QueryUwbTimeStampCMD(void)
{

}

static void uciMsgPropri_DeviceSuspendCMD(void)
{

}

static void uciMsgPropri_SetAppCfgPageValuesCMD(void)
{

}

static void uciMsgPropri_GetAllUwbSessionsCMD(void)
{

}

static void uciMsgPropri_GetAppCfgPageValuesCMD(void)
{

}

static void uciMsgPropri_StoreProtectionKeyCMD(void)
{

}

static void uciMsgPropri_SetEpochIdCMD(void)
{

}

static void uciMsgPropri_SetRadarConfigCMD(void)
{

}

static void uciMsgPropri_GetRadarConfigCMD(void)
{

}

static void uciMsgPropri_TuspStoreTtpKeyCMD(void)
{

}

static void uciMsgPropri_RunRadarCalibrationCMD(void)
{

}

static void uciMsgPropri_TuspGetAnchorRequestsCMD(void)
{

}

static void uciMsgPropri_TuspStoreLongtermPairingKeyCMD(void)
{

}

static void uciMsgPropri_TuspSetEpochIdCMD(void)
{

}

static void uciMsgPropri_TuspTransmitEncryptedDataCMD(void)
{

}

/*********************************************************
 * Public functions
 *********************************************************/

uint8_t uciMsgPropri_Indication(uint8_t const opcodeId)
{
	uint8_t uciMsgState = 1;
	/* If the device is in error state or in a not supported state, UCI commands are rejected. */
	uciMsg_WorkStatus_t uciState = uciMsg_GetUciMsgWorkState();

	if (PHSCA_UCIMSG_OID_PROPRIETARY_ACTIVATE_SWUP == opcodeId)
	{
		/* Activating swup is always supported. */
		uciMsgPropri_SwupCMD();
	}
	else if (PHSCA_UCIMSG_OID_PROPRIETARY_GET_DEVICE_REBOOT_REASON == opcodeId)
	{
		uciMsgPropri_DeviceGetRebootReasonCMD();
	}
	else if (UCIMSG_WORK_STATE_IDLE != uciState)
	{
		uciMsg_UciMsgErrorStateNotif(PHSCA_UCIMSG_GENERIC_STATUS_REJECTED);	//向CAN总线发送请求被拒绝错误状态通知;
		uciMsgState = 0;
	}
	else
	{
		switch (opcodeId)
		{
			case PHSCA_UCIMSG_OID_PROPRIETARY_START_TEST:
			{
				uciMsgPropri_TestStartCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_STOP_TEST:
			{
				uciMsgPropri_TestStartCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_RADIO_CONFIG_DOWNLOAD:
			{
				uciMsgPropri_RadarConfigDownloadCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_QUERY_UWB_TIMESTAMP:
			{
				uciMsgPropri_QueryUwbTimeStampCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_DEV_SUSPEND:
			{
				uciMsgPropri_DeviceSuspendCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_SET_APP_CFG_PAGE_VALUES:
			{
				uciMsgPropri_SetAppCfgPageValuesCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_GET_ALL_SESSIONS:
			{
				uciMsgPropri_GetAllUwbSessionsCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_GET_APP_CFG_PAGE_VALUES:
			{
				uciMsgPropri_GetAppCfgPageValuesCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_STORE_KEYPROTECTION_KEY:
			{
				uciMsgPropri_StoreProtectionKeyCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_SET_EPOCH_ID:
			{
				uciMsgPropri_SetEpochIdCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_SET_RADAR_CONFIG:
			{
				uciMsgPropri_SetRadarConfigCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_GET_RADAR_CONFIG:
			{
				uciMsgPropri_GetRadarConfigCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_RUN_RADAR_CALIBRATION:
			{
				uciMsgPropri_RunRadarCalibrationCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_STORE_TTP_KEY:
			{
				uciMsgPropri_TuspStoreTtpKeyCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_GET_ANCHOR_REQUESTS:
			{
				uciMsgPropri_TuspGetAnchorRequestsCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_STORE_LONGTERM_PAIRING_KEYS:
			{
				uciMsgPropri_TuspStoreLongtermPairingKeyCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_SET_EPOCH_ID:
			{
				uciMsgPropri_TuspSetEpochIdCMD();
				break;
			}

			case PHSCA_UCIMSG_OID_PROPRIETARY_TUSP_TRANSMIT_ENCRYPTED_DATA:
			{
				uciMsgPropri_TuspTransmitEncryptedDataCMD();
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
