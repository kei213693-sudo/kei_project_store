/*
 * bleTP.c
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#include "bleTP_cfg.h"
#include "bleTP_api.h"
#include "bleTP.h"
#include "canbus_trans.h"
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"

#define BLE_AIR_SF_FF_CONTROL_BYTES		2u
#define BLE_AIR_CF_CONTROL_BYTES		1u
#define BLE_AIR_TP_LENGTH				197u
#define BLE_AIR_PCI_LENGTH				4u
#define UNUSED_BYTE_FILLOUT				0xAAu

#define kTpNoChannel          0xFF
#define kTpTxChannel          0x00
#define kTpRxChannel          0x80

static uint8_t  		    bleTpTransmitChannel;
#define BleTpIntTxTransmitChannel()      	(bleTpTransmitChannel)
#define BleTpIntRxTransmitChannel()      	(bleTpTransmitChannel)
#define BleTpIntTxTransmitChannel_IsFree()  (BleTpIntTxTransmitChannel()  == kTpNoChannel)
#define BleTpIntTxTransmitChannel_SetFree() (BleTpIntTxTransmitChannel()  =  kTpNoChannel)
#define BleTpIntRxTransmitChannel_IsFree()  (BleTpIntRxTransmitChannel()  == kTpNoChannel)
#define BleTpIntRxTransmitChannel_SetFree() (BleTpIntRxTransmitChannel()  =  kTpNoChannel)
#define BleTpIntTxTransmitChannel_SetLock() (BleTpIntTxTransmitChannel()  =  kTpTxChannel)
#define BleTpIntTxTransmitChannel_IsTx()    (BleTpIntTxTransmitChannel()  == kTpTxChannel)
#define BleTpIntRxTransmitChannel_SetLock() (BleTpIntRxTransmitChannel()  =  kTpRxChannel)
#define BleTpIntRxTransmitChannel_IsRx()    (BleTpIntRxTransmitChannel()  == kTpRxChannel)


static bleTpTxState_t bleTpTxState[gAppMaxConnections_c];
static bleTpRxState_t bleTpRxState[gAppMaxConnections_c];

static uint8_t canTpSendFlag = 0;
static uint8_t bleTpSendFlag = 0;

static uint8_t cntCF = 0;

static uint8_t TpTxMsgBuf[BLE_AIR_TP_LENGTH] = {0};

blePayloads_t bleDataBuff;


bleTxMsgInfo_t bleTxMsgInfo[gAppMaxConnections_c];
bleRxMsgInfo_t bleRxMsgInfo[gAppMaxConnections_c];

static void BleTp_TxStateTask(void);
static void BleTpTxInternalPrepareOfCF(uint8_t deviceId);
static uint8_t BleTp_PreTransmit(uint8_t deviceId, uint8_t* len);
static void BleTpTxPostProcessing(uint8_t deviceId);

static void BleTpRxStateTask(void);
static void BleTpRxPostProcessing(uint8_t deviceId);
void BleTpRxIndication(void);


static uint8_t Get_TpTimeoutFlag(void)
{
	return Get_TpTimer_TimeoutFlag(0, T_BleCanTp_Timeout);
}

static void BleTp_TxStateTask(void)
{
	TpReturnType preTransmitResult;
	uint8_t deviceId;
	uint8_t len;
	uint16_t servicehanle;


	if (bleTpTxState[deviceId].sendReq != 0)
	{
		if (BleTpIntTxTransmitChannel_IsFree())
		{
			BleTpIntTxTransmitChannel_SetLock();
			bleTpTxState[deviceId].sendReq = 0;
			preTransmitResult = BleTp_PreTransmit(deviceId, &len);
			if (preTransmitResult == TpSuccess)
			{
				BleIf_Transmit(deviceId, len, servicehanle, bleTxMsgInfo[deviceId].buffPtr);
			}
			else
			{
				BleTpIntTxTransmitChannel_SetFree();
			}
		}
	}
}

static uint8_t BleTp_PreTransmit(uint8_t deviceId, uint8_t* len)
{
	switch(bleTpTxState[deviceId].engine)
	{
		case enum_bleTx_SF:
			*len = bleTxMsgInfo[deviceId].bufLen;
			bleTxMsgInfo[deviceId].CurrentTpCanDLC = BLE_AIR_PCI_LENGTH + bleTxMsgInfo[deviceId].bufLen;
			break;

		case enum_bleTx_FF:
			*len = BLE_AIR_TP_LENGTH;
			bleTxMsgInfo[deviceId].bufIndex = BLE_AIR_TP_LENGTH;
			bleTxMsgInfo[deviceId].CurrentTpCanDLC = BLE_AIR_TP_LENGTH;
			break;

		case enum_bleTx_CF:
		case enum_bleTx_LastCF:
			BleTpTxInternalPrepareOfCF(deviceId);
			bleTxMsgInfo[deviceId].CurrentTpCanDLC = BLE_AIR_TP_LENGTH;
			if (bleTpTxState[deviceId].engine == enum_bleTx_LastCF)
			{
				*len = bleTxMsgInfo[deviceId].bufLen-bleTxMsgInfo[deviceId].bufIndex;
			}
			else
			{
				*len = BLE_AIR_TP_LENGTH;
				bleTxMsgInfo[deviceId].CurrentTpCanDLC = BLE_AIR_TP_LENGTH;
			}
			bleTxMsgInfo[deviceId].bufIndex = bleTxMsgInfo[deviceId].bufIndex + BLE_AIR_TP_LENGTH;
			break;

		case enum_bleTx_Idle:
			break;

		default:
	    	return TpFailed;
	}
}

static void BleTpTxInternalPrepareOfCF(uint8_t deviceId)
{
	if((bleTxMsgInfo[deviceId].bufIndex + BLE_AIR_TP_LENGTH) >= bleTxMsgInfo[deviceId].bufLen)
	{
		bleTpTxState[deviceId].engine = enum_bleTx_LastCF;
	}
	else
	{
		bleTpTxState[deviceId].engine = enum_bleTx_CF;
	}
}

static void BleTpTxPostProcessing(uint8_t deviceId)
{
	switch(bleTpTxState[deviceId].engine)
	{
		case enum_bleTx_SF:
		case enum_bleTx_LastCF:
			bleTpTxState[deviceId].engine = enum_bleTx_Idle;
			break;

		case enum_bleTx_FF:
			bleTpTxState[deviceId].engine = enum_bleTx_CF;
			break;

		case enum_bleTx_CF:
			bleTpTxState[deviceId].engine = enum_bleTx_CF;
	  		break;

		default:
			return;
	}
	BleTpIntTxTransmitChannel_SetFree();
}

static void BleTpRxStateTask(void)
{
	/*TODO */
}

static void BleTpRxPostProcessing(uint8_t deviceId)
{
	BleTpIntRxTransmitChannel_SetFree();
}






void BleTp_TxMainFunction(uint8_t deviceId)
{
	switch(bleTpTxState[deviceId].engine)
	{
	case enum_bleTx_CF:			/* It's time to send consecutive frame */
		bleTpTxState[deviceId].sendReq = 1;
		BleTp_TxStateTask();
		break;
	}
}

void BleTp_RxMainFunction(uint8_t deviceId)
{
	if (Get_TpTimeoutFlag())	/* TP timer timeout*/
	{
		switch (bleTpRxState[deviceId].engine)
		{
			case enum_canRx_CF:
				BleTp_Init();			/* No consecutive frame received */
				break;

			default:
				break;
		}
	}
	else
	{
		BleTpRxStateTask();
	}
}

/*! *********************************************************************************
* \fn           uint8_t BleTp_Transmit(uint32_t id, bleMsgInfo_t *bleMsgInfo)
* \brief        Ble send can message. Put data into canDataBuff. Initialize BleTP state.
* \call			由 pass through service/app 进行调用, 与UDS类型类似，由BleTp_Transmit进行调用
*
* \param  [in]  id      can message id
* \param  [in]  bleMsgInfo      ble message
*
* \retval       gHciSuccess_c         Initialization was successful.
********************************************************************************** */
uint8_t BleTp_Transmit(bleMsgInfo_t *bleMsgInfo)
{
	uint16_t len;
	uint8_t deviceId;

	deviceId = bleMsgInfo->deviceId;

	if(bleTpTxState[deviceId].engine != enum_bleTx_Idle)
	{
		return 0;
	}
	else
	{
		if(bleTxMsgInfo[deviceId].bufSts == CAN_PASSTHROUGH_BUF_IDLE)
		{
			bleTxMsgInfo[deviceId].bufSts = CAN_PASSTHROUGH_BUF_BUSY;
			bleTxMsgInfo[deviceId].bufLen = bleMsgInfo->packetLength;
			memcpy(bleDataBuff.bytes.data, bleMsgInfo->pPacket, bleMsgInfo->packetLength);

			if(len <= (BLE_AIR_TP_LENGTH - BLE_AIR_SF_FF_CONTROL_BYTES))
			{
				bleTpTxState[deviceId].engine = enum_bleTx_SF;
			}
			else
			{
				bleTpTxState[deviceId].engine = enum_bleTx_FF;
			}
			bleTxMsgInfo[deviceId].buffPtr = bleDataBuff.bytes.data;
			bleTxMsgInfo[deviceId].bufIndex = 0;
			bleTpTxState[deviceId].sendReq = 1;

			/*TODO set Event*/
		    if(mpfBleAppMessageEventHandler != NULL)
		    {
		         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(appMsgBleCanData_t));
		         if(pEventData != NULL)
		         {
		             pEventData->appEvent = mAppEvt_BleMsgTransmit_c;
		             pEventData->eventData.pData = &pEventData[1];
		             appMsgBleCanData_t* appMsgBleCanData = pEventData->eventData.pData;
		             appMsgBleCanData->deviceId = bleMsgInfo->deviceId;
		             appMsgBleCanData = NULL;
		             if (gBleSuccess_c != BleApp_MessageCallback(mpfBleAppMessageEventHandler, pEventData))
		             {
		            	 (void)MEM_BufferFree(pEventData);
		             }
		         }
		    }
		}
		else
		{
			/*for MISRA C*/
		}
	}
	return 0;
}

void BleTp_RxFromInterrupt(uint16_t dlc, uint8_t deviceId, const uint8_t *data)
{
	static uint8_t blockIndex = 0;
	static uint16_t length = 0;
	static uint8_t frameType = ble_SingleFrame;

	if(bleTpRxState[deviceId].engine == enum_canRx_Idle)
	{
		length = (data[3] & 0xFF) << 8 + data[2];
		if(length > BLE_AIR_TP_LENGTH)
		{
			frameType = ble_FirstFrame;
		}
		else
		{
			frameType = ble_SingleFrame;
		}
	}

	switch(frameType)	/* Receive frame Type */
	{
	case ble_SingleFrame:
	case ble_FirstFrame:
		if (bleTpRxState[deviceId].engine != enum_canRx_Idle)
		{
			bleRxMsgInfo[deviceId].bufSts == CAN_PASSTHROUGH_BUF_IDLE;
		}
		/*tpRxState.engine = kRxState_CanFrameReceived;*/
		switch(data[0] & 0xF0)
		{
		case ble_FirstFrame:
			if (dlc == BLE_AIR_TP_LENGTH)
			{
				bleRxMsgInfo[deviceId].DataLength = (data[3] & 0xFF) << 8 + data[2];
				if(bleRxMsgInfo[deviceId].DataLength > BLE_AIR_TP_LENGTH - BLE_AIR_PCI_LENGTH)
				{
					bleRxMsgInfo[deviceId].bufSts = CAN_PASSTHROUGH_BUF_BUSY;
					memcpy(bleRxMsgInfo[deviceId].DataBufferPtr, data, BLE_AIR_TP_LENGTH);
					bleRxMsgInfo[deviceId].DataIndex += BLE_AIR_SF_FF_CONTROL_BYTES;
					bleRxMsgInfo[deviceId].sequencenumber = 1;
					Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_AIRTP_RX_TIMEOUT);
					Start_BleTimer(0, T_BleCanTp_Timeout);
					bleTpRxState[deviceId].engine = enum_bleRx_CF;
				}
				else
				{
					BleTp_Init();		/* Receive wrong */
					bleTpRxState[deviceId].engine = enum_bleRx_Idle;
				}
			}
			else
			{
				BleTp_Init();		/* Receive wrong */
				bleTpRxState[deviceId].engine = enum_bleRx_Idle;
			}
			break;
		case ble_SingleFrame:		/* Receive Single Frame, Do Nothing */
			bleRxMsgInfo[deviceId].DataLength = (data[3] & 0xFF) << 8 + data[2];
			if(bleRxMsgInfo[deviceId].DataLength <= (BLE_AIR_TP_LENGTH - BLE_AIR_PCI_LENGTH))
			{
				bleRxMsgInfo[deviceId].bufSts = CAN_PASSTHROUGH_BUF_BUSY;
				memcpy(bleRxMsgInfo[deviceId].DataBufferPtr, data, bleRxMsgInfo[deviceId].DataLength);
				bleRxMsgInfo[deviceId].DataIndex += dlc;
				bleTpRxState[deviceId].engine = enum_bleRx_Idle;
				/*TODO Do something!*/
				BleTpRxIndication();
			}
			else
			{
				BleTp_Init();		/* Receive wrong */
				bleTpRxState[deviceId].engine = enum_bleRx_Idle;
			}
			break;
		}

	case ble_FlowControl:
		break;

	case ble_ConsecutiveFrame:
		uint16_t storelen;
		if(bleTpRxState[deviceId].engine == enum_canRx_CF)
		{
			/* Receive Complete */
			if((bleRxMsgInfo[deviceId].DataIndex + BLE_AIR_TP_LENGTH) >= bleRxMsgInfo[deviceId].DataLength)
			{
				if(dlc == bleRxMsgInfo[deviceId].DataLength - bleRxMsgInfo[deviceId].DataIndex)
				{
					memcpy(&(bleRxMsgInfo[deviceId].DataBufferPtr[bleRxMsgInfo[deviceId].DataIndex]), \
							data, bleRxMsgInfo[deviceId].DataLength-bleRxMsgInfo[deviceId].DataIndex);
					bleTpRxState[deviceId].engine = enum_bleRx_Idle;
					Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_AIRTP_RX_TIMEOUT);
					Start_BleTimer(0, T_BleCanTp_Timeout);
					BleTpRxIndication();
				}
				else
				{
					BleTp_Init();
				}
			}
			else
			{
				if(dlc == BLE_AIR_TP_LENGTH)
				{
					memcpy(&(bleRxMsgInfo[deviceId].DataBufferPtr[bleRxMsgInfo[deviceId].DataIndex]), \
							data, BLE_AIR_TP_LENGTH);
					bleRxMsgInfo[deviceId].DataIndex = bleRxMsgInfo[deviceId].DataIndex + BLE_AIR_TP_LENGTH;
					/* Consecutive frame timeout*/
					Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_AIRTP_RX_TIMEOUT);
					Start_BleTimer(0, T_BleCanTp_Timeout);
					bleTpRxState[deviceId].engine = enum_bleRx_CF;
				}
			}
		}
		break;

	default:
		break;
	}

    if(mpfBleAppMessageEventHandler != NULL)
    {
         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(appMsgBleCanData_t));
         if(pEventData != NULL)
         {
             pEventData->appEvent = mAppEvt_BleMsgRev_c;
             pEventData->eventData.pData = &pEventData[1];
             appMsgBleCanData_t* appMsgBleCanData = pEventData->eventData.pData;
             /* TODO Need to add device id logic */
             appMsgBleCanData->deviceId = 0;
             appMsgBleCanData = NULL;
             if (gBleSuccess_c != BleApp_MessageCallback(mpfBleAppMessageEventHandler, pEventData))
             {
            	 (void)MEM_BufferFree(pEventData);
             }
         }
    }
}

void BleTpDrvConfirmation(uint8_t deviceId)
{
	if (BleTpIntTxTransmitChannel_IsTx())
	{
		BleTpTxPostProcessing(deviceId);
	}
	else
	{
		BleTpRxPostProcessing(deviceId);
	}
}

void BleTpRxIndication(void)
{
	/*TODO Receive Complete!*/
}

void BleTp_Init(void)
{
	memset(&bleTxMsgInfo, 0, sizeof(bleTxMsgInfo_t));
	memset(&bleRxMsgInfo, 0, sizeof(bleRxMsgInfo_t));
	BleTpIntTxTransmitChannel_SetFree();
}


void Tp_Init(void)
{
	BleTp_Init();
}

void Tp_Transmit(uint32_t id, bleMsgInfo_t *bleMsgInfo)
{
	if(id == INVALID_CAN_ID)
	{
		BleTp_Transmit(bleMsgInfo);
	}
	else
	{

	}
}

void Tp_Receive(bleMsgInfo_t *bleMsgInfo)
{
	deviceId_t deviceId;

	if(bleMsgInfo->source == messageSource_air_c)
	{
		BleTp_RxFromInterrupt(bleMsgInfo->packetLength, bleMsgInfo->deviceId, bleMsgInfo->pPacket);
	}
	else if(bleMsgInfo->source == messageSource_can_c)
	{

	}
	else
	{
		/*TODO */
	}
}
