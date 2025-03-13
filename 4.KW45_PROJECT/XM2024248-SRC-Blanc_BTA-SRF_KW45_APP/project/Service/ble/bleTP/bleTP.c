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
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"

#define BLE_MESSAGE_TYPE_OFFSET			6u
#define BLE_MESSAGE_ID_OFFSET			7u
#define BLE_MESSAGE
#define BLE_CAN_MAC_HEADER_LEN			6u
#define BLE_CAN_SF_FF_CONTROL_BYTES		2u
#define BLE_CAN_CF_CONTROL_BYTES		1u
#define BLE_CAN_TP_LENGTH				64u
#define BLE_CAN_PCI_LENGTH				10u
#define UNUSED_BYTE_FILLOUT				0xAAu

#define BLE_AIR_SF_FF_CONTROL_BYTES		2u
#define BLE_AIR_CF_CONTROL_BYTES		1u
#define BLE_AIR_TP_LENGTH				197u
#define BLE_AIR_PCI_LENGTH				4u
#define UNUSED_BYTE_FILLOUT				0xAAu

#define kTpNoChannel          0xFF
#define kTpTxChannel          0x00
#define kTpRxChannel          0x80

static uint8_t  		    canTpTransmitChannel;
#define CanTpIntTxTransmitChannel()      	(canTpTransmitChannel)
#define CanTpIntRxTransmitChannel()      	(canTpTransmitChannel)
#define CanTpIntTxTransmitChannel_IsFree()  (CanTpIntTxTransmitChannel()  == kTpNoChannel)
#define CanTpIntTxTransmitChannel_SetFree() (CanTpIntTxTransmitChannel()  =  kTpNoChannel)
#define CanTpIntRxTransmitChannel_IsFree()  (CanTpIntRxTransmitChannel()  == kTpNoChannel)
#define CanTpIntRxTransmitChannel_SetFree() (CanTpIntRxTransmitChannel()  =  kTpNoChannel)
#define CanTpIntTxTransmitChannel_SetLock() (CanTpIntTxTransmitChannel()  =  kTpTxChannel)
#define CanTpIntTxTransmitChannel_IsTx()    (CanTpIntTxTransmitChannel()  == kTpTxChannel)
#define CanTpIntRxTransmitChannel_SetLock() (CanTpIntRxTransmitChannel()  =  kTpRxChannel)
#define CanTpIntRxTransmitChannel_IsRx()    (CanTpIntRxTransmitChannel()  == kTpRxChannel)

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

static canTpTxState_t canTpTxState;
static canTpRxState_t canTpRxState;

static uint8_t canTpSendFlag = 0;
static uint8_t bleTpSendFlag = 0;

static uint8_t cntCF = 0;

static uint8_t TpTxMsgBuf[BLE_CAN_TP_LENGTH] = {0};

canTxMsgInfo_t canTxMsgInfo;
canRxMsgInfo_t canRxMsgInfo;

bleTxMsgInfo_t bleTxMsgInfo[gAppMaxConnections_c];
bleRxMsgInfo_t bleRxMsgInfo[gAppMaxConnections_c];

static void CanTp_TxStateTask(void);
static void CanTpTxInternalPrepareOfCF(void);
static uint8_t CanTp_PreTransmit(uint8_t *packLen);
static void CanTpTxPostProcessing(void);

static void CanTpRxStateTask(void);
static void CanTpRxPostProcessing(void);
void CanTpRxIndication(canRxMsgInfo_t* canRxMsgInfo);

static void BleTp_TxStateTask(uint8_t deviceId);
static void BleTpTxInternalPrepareOfCF(uint8_t deviceId);
static uint8_t BleTp_PreTransmit(uint8_t deviceId, uint8_t* len);
static void BleTpTxPostProcessing(uint8_t deviceId);

static void BleTpRxStateTask(void);
static void BleTpRxPostProcessing(uint8_t deviceId);
void BleTpRxIndication(void);


static uint8_t Get_TpTimeoutFlag(void);

static void CanTp_TxStateTask(void)
{
	static uint8_t packLen;
	TpReturnType preTransmitResult;

	if (canTpTxState.sendReq != 0)
	{
		/* When anyone TP Frame transmit successfully, TransmitChannel will be released*/
		if (CanTpIntTxTransmitChannel_IsFree())
		{
			CanTpIntTxTransmitChannel_SetLock();
			canTpTxState.sendReq = 0;
			preTransmitResult = CanTp_PreTransmit(&packLen);
			if (preTransmitResult == TpSuccess)
			{
				if (canbus_send_tpframe(APP_TP_TYPE_BLE_UP_LINK, TpTxMsgBuf, packLen))
				{
					canTpRxState.retransmit = 1;
				}
			}
			else	/* stop transmit and free the transmit channel when data is wrong */
			{
				CanTpIntTxTransmitChannel_SetFree();
				/*canTpTxState.sendReq = 1;*/
			}
		}
	}
}

static void CanTpTxInternalPrepareOfCF(void)
{
	canTxMsgInfo.sequencenumber = canTxMsgInfo.sequencenumber + 1;
	if((canTxMsgInfo.bufIndex + BLE_CAN_TP_LENGTH - BLE_CAN_CF_CONTROL_BYTES) >= canTxMsgInfo.bufLen)
	{
		canTpTxState.engine = enum_canTx_LastCF;
	}
	else
	{
		canTpTxState.engine = enum_canTx_CF;
	}
}

static uint8_t CanTp_PreTransmit(uint8_t *packLen)
{
	uint8_t * pDestination;
	uint8_t * pSource;
	uint16_t Length;
	uint16_t i;
	uint8_t offset = 0;

	pSource = (uint8_t *)&canTxMsgInfo.buffPtr[canTxMsgInfo.bufIndex];
	switch(canTpTxState.engine)
	{
		case enum_canTx_SF:
			TpTxMsgBuf[0] = kL4_SingleFrame;
			TpTxMsgBuf[1] = (uint8_t)( canTxMsgInfo.bufLen & 0x00FF);
			pDestination = &TpTxMsgBuf[BLE_CAN_SF_FF_CONTROL_BYTES];
			memcpy(pDestination, pSource, 8);
			offset = BLE_CAN_SF_FF_CONTROL_BYTES;
			Length = canTxMsgInfo.bufLen;
			canTxMsgInfo.CurrentTpCanDLC = BLE_CAN_SF_FF_CONTROL_BYTES + canTxMsgInfo.bufLen;
			break;

		case enum_canTx_FF:
			TpTxMsgBuf[0] = kL4_FirstFrame;
			TpTxMsgBuf[0] |= (uint8_t)((canTxMsgInfo.bufLen & 0x0F00)>>8);
			TpTxMsgBuf[1] = (uint8_t)( canTxMsgInfo.bufLen & 0x00FF);
			pDestination = &TpTxMsgBuf[BLE_CAN_SF_FF_CONTROL_BYTES];
			memcpy(pDestination, pSource, 8);
			offset = BLE_CAN_SF_FF_CONTROL_BYTES;
			Length = BLE_CAN_TP_LENGTH - BLE_CAN_SF_FF_CONTROL_BYTES;
			canTxMsgInfo.bufIndex = BLE_CAN_TP_LENGTH;
			canTxMsgInfo.CurrentTpCanDLC = BLE_CAN_TP_LENGTH;
			canTxMsgInfo.sequencenumber = 0;
			canTxMsgInfo.BSCounter = 0;
			break;

		case enum_canTx_CF:
		case enum_canTx_LastCF:
			CanTpTxInternalPrepareOfCF();
			TpTxMsgBuf[0] = kL4_ConsecutiveFrame;
			TpTxMsgBuf[0] |= canTxMsgInfo.sequencenumber;
			pDestination = &TpTxMsgBuf[BLE_CAN_CF_CONTROL_BYTES];
			memcpy(pDestination, pSource, 8);
			offset = BLE_CAN_CF_CONTROL_BYTES;

			if (canTpTxState.engine == enum_canTx_LastCF)
			{
				Length = canTxMsgInfo.bufLen - canTxMsgInfo.bufIndex;
			}
			else
			{
				Length = BLE_CAN_TP_LENGTH - BLE_CAN_CF_CONTROL_BYTES;
				canTxMsgInfo.CurrentTpCanDLC = BLE_CAN_TP_LENGTH;
			}
			canTxMsgInfo.CurrentTpCanDLC = Length;
			canTxMsgInfo.bufIndex = canTxMsgInfo.bufIndex + Length;
			break;

		case enum_canTx_Idle:
			break;

		default:
	    	return TpFailed;
	}

	if( Length <= 8 )		{ *packLen = 8; }
	else if( Length <= 12 ) { *packLen = 12; }
	else if( Length <= 16 ) { *packLen = 16; }
	else if( Length <= 20 ) { *packLen = 20; }
	else if( Length <= 24 ) { *packLen = 24; }
	else if( Length <= 32 ) { *packLen = 32; }
	else if( Length <= 48 ) { *packLen = 48; }
	else if( Length <= 64 ) { *packLen = 64; }

	for(i=0;i<*packLen-offset;i++)
	{
		if(Length == 0)
		{
			*(pDestination + i) = UNUSED_BYTE_FILLOUT;
		}
		else
		{
			Length--;
			*(pDestination + i) = *(pSource + i);
		}
	}

	return TpSuccess;

}

/*! *********************************************************************************
* \fn           static void CanTpTxPostProcessing(void)
* \brief        can TP 每一帧发送完成确认;
* \call			由发送中断函数调用;
*
* \param
*
* \retval
********************************************************************************** */
static void CanTpTxPostProcessing(void)
{
	switch(canTpTxState.engine)
	{
		case enum_canTx_SF:
		case enum_canTx_LastCF:
			/* TP package send successfully */
			canTpTxState.engine = enum_canTx_Idle;
			canTxMsgInfo.bufSts = CAN_PASSTHROUGH_BUF_IDLE;
			break;

		case enum_canTx_FF:
			Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_CANTP_TX_FC_TIMEOUT);
			Start_BleTimer(0, T_BleCanTp_Timeout);
			canTpTxState.engine = enum_canTxWait_FC;
			break;

		case enum_canTx_CF:
			if(canTpTxState.BSCounter != 0)
			{
				canTpTxState.BSCounter--;
				if(canTpTxState.BSCounter == 0)
				{
					Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_CANTP_TX_FC_TIMEOUT);
					Start_BleTimer(0, T_BleCanTp_Timeout);
					canTpTxState.engine = enum_canTxWait_FC;
					break;
				}
			}
			Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, canTxMsgInfo.STMin);
			Start_BleTimer(0, T_BleCanTp_Timeout);
			canTpTxState.engine = enum_canTx_CF;
	  		break;

		default:
			return;
	}
	CanTpIntTxTransmitChannel_SetFree();
}

/*! *********************************************************************************
* \fn           static void CanTpRxStateTask(void)
* \brief        在接收过程中，用于发送流控帧;
* \call			由超时事件或mAppEvt_CanMsgRev_c事件调用;
*
* \param
*
* \retval
********************************************************************************** */
static void CanTpRxStateTask(void)
{
	if (canTpRxState.sendReq != 0)
	{
		if (CanTpIntRxTransmitChannel_IsFree())
		{
			Clear_BleTimer(0, T_BleCanTp_Timeout);
			CanTpIntRxTransmitChannel_SetLock();	/* Send flow control frame! */
			canTpRxState.sendReq = 0;
			TpTxMsgBuf[0] = 0x30;
			TpTxMsgBuf[1] = 0x00;
			TpTxMsgBuf[2] = 0x0A;
			memset(&TpTxMsgBuf[3], 0xAA, BLE_CAN_TP_LENGTH - BLE_CAN_CF_CONTROL_BYTES);
			canbusTransSendMsgFrame(BLE_Downlink_FlowCtrl_CANID, BLE_CAN_TP_LENGTH, TpTxMsgBuf);
		}
	}
}

/*! *********************************************************************************
* \fn           static void CanTpRxPostProcessing(void)
* \brief        在发送TP过程中，会接收到flow control帧，接收到流控帧时
* \call			由发送中断函数调用;
*
* \param
*
* \retval
********************************************************************************** */
static void CanTpRxPostProcessing(void)
{
	switch(canTpRxState.engine)
	{
		/* flow frame send successfully in the process of receive and start send again!*/
		case enum_canRxSend_FC:
			Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_CANTP_RX_CF_TIMEOUT);
			Start_BleTimer(0, T_BleCanTp_Timeout);
			canTpRxState.engine = enum_canRx_CF;
			break;

		case kRxState_WaitForFCOverConfIsr:
			break;

		default:
      		return;
	}
	CanTpIntRxTransmitChannel_SetFree();
}

static uint8_t Get_TpTimeoutFlag(void)
{
	return Get_TpTimer_TimeoutFlag(0, T_BleCanTp_Timeout);
}

static void BleTp_TxStateTask(uint8_t deviceId)
{
	TpReturnType preTransmitResult;
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
				BleIf_Transmit(deviceId, len, servicehanle, &(bleTxMsgInfo[deviceId].buffPtr[bleTxMsgInfo[deviceId].bufIndex]));
			}
			else
			{
				BleTpIntTxTransmitChannel_SetFree();
			}
		}
	}
}

/*! *********************************************************************************
* \fn           static uint8_t BleTp_PreTransmit(uint8_t deviceId, uint8_t* len)
* \brief        Ble send can message. Put data into canDataBuff. Initialize BleTP state.
* \call			由 pass through service/app 进行调用, 与UDS类型类似，由BleTp_Transmit进行调用
*
* \param  [in]  id      can message id
* \param  [in]  bleMsgInfo      ble message
*
* \retval       gHciSuccess_c         Initialization was successful.
********************************************************************************** */
static uint8_t BleTp_PreTransmit(uint8_t deviceId, uint8_t* len)
{
	switch(bleTpTxState[deviceId].engine)
	{
		case enum_bleTx_SF:
			*len = bleTxMsgInfo[deviceId].bufLen;
			bleTxMsgInfo[deviceId].CurrentTpCanDLC = bleTxMsgInfo[deviceId].bufLen;
			break;

		case enum_bleTx_FF:
			*len = BLE_AIR_TP_LENGTH;
			bleTxMsgInfo[deviceId].bufIndex = BLE_AIR_TP_LENGTH;
			bleTxMsgInfo[deviceId].CurrentTpCanDLC = BLE_AIR_TP_LENGTH;
			break;

		case enum_bleTx_CF:
		case enum_bleTx_LastCF:
			BleTpTxInternalPrepareOfCF(deviceId);
			if (bleTpTxState[deviceId].engine == enum_bleTx_LastCF)
			{
				*len = bleTxMsgInfo[deviceId].bufLen - bleTxMsgInfo[deviceId].bufIndex;
			}
			else
			{
				*len = BLE_AIR_TP_LENGTH;
				bleTxMsgInfo[deviceId].CurrentTpCanDLC = BLE_AIR_TP_LENGTH;
			}
			bleTxMsgInfo[deviceId].CurrentTpCanDLC = *len;
			bleTxMsgInfo[deviceId].bufIndex = bleTxMsgInfo[deviceId].bufIndex + *len;
			break;

		case enum_bleTx_Idle:
			break;

		default:
	    	return TpFailed;
	}
	return TpSuccess;
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
			bleTpTxState[deviceId].engine = enum_canTx_CF;
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
	switch(bleTpRxState[deviceId].engine)
	{
		case enum_canRxSend_FC:
			break;

		case kRxState_WaitForFCOverConfIsr:
			break;

		default:
      		return;
	}
	BleTpIntRxTransmitChannel_SetFree();
}

/*
 * CanTP send function
 * 1. Send single/first frame( when CanTp_Transmit() function called, cantp transmit started)
 * 2. Receive flow control frame( when no flow control frame received, cantp ReInit)
 * 3. Send consecutive frame( send consecutive frame when interval of consecutive frame is up)
 * */
void CanTp_TxMainFunction(void)
{
	if(Get_TpTimeoutFlag())		/* CanTp transmit timeout*/
	{
		switch(canTpTxState.engine)
		{
		case enum_canTxWait_FC:		/* NO flow control frame received */
			CanTp_Init();
			break;
		case enum_canTx_CF:			/* It's time to send consecutive frame */
			canTpTxState.sendReq = 1;
			CanTp_TxStateTask();
			break;
		}
	}
	else
	{
		CanTp_TxStateTask();
	}
}

void CanTp_RxMainFunction(void)
{
	if (Get_TpTimeoutFlag())	/* TP timer timeout*/
	{
		switch (canTpRxState.engine)
		{
			case enum_canRx_CF:
				BleTp_Init();			/* No consecutive frame received */
				break;

			case enum_canRxSend_FC:		/* No flow control frame send */
			case kRxState_WaitForFCOverConfIsr:
				BleTp_Init();
				break;

			case enum_canRxSend_FCWait:
				break;

			default:
				break;
		}
	}
	else
	{
		CanTpRxStateTask();
	}
}

/*! *********************************************************************************
* \fn           uint8_t CanTp_Transmit(uint32_t id, bleMsgInfo_t *bleMsgInfo)
* \brief        Ble send can message. Put data into canDataBuff. Initialize BleTP state.
* \call			由 pass through service/app 进行调用, 与UDS类型类似，由BleTp_Transmit进行调用
*
* \param  [in]  id      can message id
* \param  [in]  bleMsgInfo      ble message
*
* \retval       gHciSuccess_c         Initialization was successful.
********************************************************************************** */
TpReturnType CanTp_Transmit(uint32_t id, bleMsgInfo_t *bleMsgInfo)
{
	uint16_t len;
	uint8_t deviceId;
	deviceId = bleMsgInfo->deviceId;
	len = bleMsgInfo->packetLength;

	if(canTpTxState.engine != enum_canTx_Idle)
	{
		return TpBusy;
	}
	else
	{
		if(canTxMsgInfo.bufSts == CAN_PASSTHROUGH_BUF_IDLE)
		{
			if(maPeerInformation[bleMsgInfo->deviceId].appState == mAppRunning_c)
			{
				if(maPeerInformation[bleMsgInfo->deviceId].gapRole == gGapCentral_c)
				{
					memcpy(bleMsgInfo->MAC_IRK, maPeerInformation[bleMsgInfo->deviceId].maPeerDeviceAddress_IRK, 6);
				}
				else if(maPeerInformation[bleMsgInfo->deviceId].gapRole == gGapPeripheral_c)
				{
					memcpy(bleMsgInfo->MAC_IRK, maPeerInformation[bleMsgInfo->deviceId].maPeerDeviceAddress_IRK, 6);
				}
				else
				{
					/* 当前设备角色不正确时，直接返回发送成功 */
					return TpSuccess;
				}
			}
			else	/* 当前设备不在running 状态时，直接返回发送成功*/
			{
				return TpSuccess;
			}

			canTxMsgInfo.canId = id;
			canTxMsgInfo.bufSts = CAN_PASSTHROUGH_BUF_BUSY;
			canTxMsgInfo.bufLen = bleMsgInfo->packetLength + BLE_CAN_MAC_HEADER_LEN;
			canTxMsgInfo.buffPtr = (canPayloads_t*)bleMsgInfo->MAC_IRK;
			canTxMsgInfo.bufIndex = 0;
			canTpTxState.sendReq = 1;

			if(canTxMsgInfo.bufLen <= (BLE_CAN_TP_LENGTH - BLE_CAN_SF_FF_CONTROL_BYTES))
			{
				canTpTxState.engine = enum_canTx_SF;
			}
			else
			{
				canTpTxState.engine = enum_canTx_FF;
			}

			/*TODO set Event*/
		    if(mpfBleAppMessageEventHandler != NULL)
		    {
		         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(appMsgBleCanData_t));
		         if(pEventData != NULL)
		         {
		             pEventData->appEvent = mAppEvt_CanMsgTransmit_c;
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
			return TpFailed;
		}
	}
	return TpSuccess;
}

void CanTp_RxFromInterrupt(uint8_t dlc, const uint8_t *data)
{
	static uint8_t blockIndex = 0;

	switch(data[0] & 0xF0)	/* Receive frame Type */
	{
	case kL4_SingleFrame:
	case kL4_FirstFrame:
		if (canTpRxState.engine != enum_canRx_Idle)
		{
			canRxMsgInfo.bufSts == CAN_PASSTHROUGH_BUF_IDLE;
		}
		/*tpRxState.engine = kRxState_CanFrameReceived;*/
		switch(data[0] & 0xF0)
		{
		case kL4_FirstFrame:
			if (dlc == BLE_CAN_TP_LENGTH)
			{
				canRxMsgInfo.DataIndex = ((data[0] & 0x0F) << 8) + data[1];
				bleMsgCanBuff.packetLength = canRxMsgInfo.DataLength - BLE_CAN_MAC_HEADER_LEN;
				if(canRxMsgInfo.DataIndex > BLE_CAN_TP_LENGTH - BLE_CAN_SF_FF_CONTROL_BYTES)
				{
					canRxMsgInfo.DataBufferPtr = (uint8_t *)bleMsgCanBuff.MAC_IRK;
					canRxMsgInfo.bufSts = CAN_PASSTHROUGH_BUF_BUSY;
					memcpy(canRxMsgInfo.DataBufferPtr, &(data[BLE_CAN_SF_FF_CONTROL_BYTES]), dlc-BLE_CAN_SF_FF_CONTROL_BYTES);
					canRxMsgInfo.DataIndex += (BLE_CAN_TP_LENGTH-BLE_CAN_SF_FF_CONTROL_BYTES);
					canRxMsgInfo.sequencenumber = 1;
					Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_CANTP_RX_SEND_FC_DELAY);
					Start_BleTimer(0, T_BleCanTp_Timeout);
					canTpRxState.engine = enum_canRxSend_FC;	/* flow control send request */
					canTpRxState.sendReq = 1;
				}
				else
				{
					CanTp_Init();		/* Receive wrong */
					canTpRxState.engine = kRxState_ApplInformed;
				}
			}
			else
			{
				CanTp_Init();		/* Receive wrong */
				canTpRxState.engine = kRxState_ApplInformed;
			}
			break;

		case kL4_SingleFrame:		/* Receive Single Frame, Do Nothing */
			canRxMsgInfo.DataLength = ((data[0] & 0x0F) << 8) + data[1];
			bleMsgCanBuff.packetLength = canRxMsgInfo.DataLength - BLE_CAN_MAC_HEADER_LEN;
			if(canRxMsgInfo.DataLength <= (BLE_CAN_TP_LENGTH - BLE_CAN_SF_FF_CONTROL_BYTES))
			{
				canRxMsgInfo.DataBufferPtr = (uint8_t *)bleMsgCanBuff.MAC_IRK;
				canRxMsgInfo.bufSts = CAN_PASSTHROUGH_BUF_BUSY;
				memcpy(canRxMsgInfo.DataBufferPtr, &(data[BLE_CAN_SF_FF_CONTROL_BYTES]), dlc-BLE_CAN_SF_FF_CONTROL_BYTES);
				canRxMsgInfo.DataIndex += (dlc - BLE_CAN_SF_FF_CONTROL_BYTES);
				canTpRxState.engine = kRxState_ApplInformed;
				/*TODO Do something!*/
				CanTpRxIndication(&canRxMsgInfo);
				return;
			}
			else
			{
				CanTp_Init();		/* Receive wrong */
				canTpRxState.engine = kRxState_ApplInformed;
			}
			break;
		}

	case kL4_FlowControl:		/* Send flow control frame, used for transmit long message */
		if(canTpTxState.engine == enum_canTxWait_FC)
		{
			if(dlc >= 3)
			{
				switch (data[0])
				{
				case kL4_FlowControl:	/* receive flow control frame*/
					canTxMsgInfo.BSCounter = data[1];
					canTxMsgInfo.STMin = data[2];
					Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_CANTP_TX_SEND_CF_DELAY);
					Start_BleTimer(0, T_BleCanTp_Timeout);
					canTpTxState.engine = enum_canTx_CF;
					break;
				case kL4_FlowStatus_Wait:	/* receive flow control and wait */
					Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_CANTP_TX_SEND_CF_DELAY);
					Start_BleTimer(0, T_BleCanTp_Timeout);
					break;
				case kL4_FlowStatus_Overrun:
					CanTp_Init();
					break;
				default:
					CanTp_Init();
					break;
				}

			}
		}
		break;

	case kL4_ConsecutiveFrame:
		uint16_t storelen;
		if(canTpRxState.engine == enum_canRx_CF)
		{
			/* Receive Complete */
			if((canRxMsgInfo.DataIndex + BLE_CAN_TP_LENGTH - 1) >= canRxMsgInfo.DataLength)
			{
				if(dlc >= canRxMsgInfo.DataLength - canRxMsgInfo.DataIndex + 1)
				{
					if((data[0] & 0x0F) != canRxMsgInfo.sequencenumber)
					{
						CanTp_Init();
					}
					else
					{
						memcpy(&(canRxMsgInfo.DataBufferPtr[canRxMsgInfo.DataIndex]), \
								&(data[BLE_CAN_CF_CONTROL_BYTES]), canRxMsgInfo.DataLength-canRxMsgInfo.DataIndex - BLE_CAN_CF_CONTROL_BYTES);
						canTpRxState.engine = kRxState_ApplInformed;
						CanTpRxIndication(&canRxMsgInfo);
						return;
					}
				}
				else
				{
					CanTp_Init();
				}
			}
			else
			{
				if(dlc == BLE_CAN_TP_LENGTH)
				{
					if((data[0] & 0x0F) != canRxMsgInfo.sequencenumber)
					{
						CanTp_Init();
					}
					else
					{
						memcpy(&(canRxMsgInfo.DataBufferPtr[canRxMsgInfo.DataIndex]), \
								&(data[BLE_CAN_CF_CONTROL_BYTES]), BLE_CAN_TP_LENGTH - BLE_CAN_CF_CONTROL_BYTES);
						canRxMsgInfo.DataIndex = canRxMsgInfo.DataIndex + BLE_CAN_TP_LENGTH - BLE_CAN_CF_CONTROL_BYTES;
						canRxMsgInfo.sequencenumber = canRxMsgInfo.sequencenumber + 1;
						if(canTpRxState.BSCounter != 0)
						{
							canTpRxState.BSCounter--;
							if(canTpRxState.BSCounter == 0)
							{
								canTpRxState.engine = enum_canRxSend_FC;
								/* Flow Control Send Delay*/
								Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_CANTP_RX_SEND_FC_DELAY1);
								Start_BleTimer(0, T_BleCanTp_Timeout);
								canTpRxState.sendReq = 1;		/* Block size receive complete, request flow control transmit. */
								return ;
							}
							return ;
						}
						/* Consecutive frame timeout*/
						Set_BleTimerTimeoutValue(0, T_BleCanTp_Timeout, BLE_CANTP_RX_CF_TIMEOUT);
						Start_BleTimer(0, T_BleCanTp_Timeout);
						canTpRxState.engine = enum_canRx_CF;
					}
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
             pEventData->appEvent = mAppEvt_CanMsgRev_c;
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

void CanTpDrvConfirmation(uint32_t id)
{
	if(id == 0x4E4)
	{
		if (CanTpIntTxTransmitChannel_IsTx())
		{
			CanTpTxPostProcessing();
		}
		else
		{
			CanTpRxPostProcessing();
		}
	}
}

void CanTpRxIndication(canRxMsgInfo_t* canRxMsgInfo)
{	/*TODO Receive Complete!*/
    if(mpfBleAppMessageEventHandler != NULL)
    {
         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(appMsgBleCanData_t));
         if(pEventData != NULL)
         {
             pEventData->appEvent = mAppEvt_CanMsgRevCompleteHandle_c;
             pEventData->eventData.pData = &pEventData[1];
             appMsgBleCanData_t* appMsgBleCanData = pEventData->eventData.pData;
             /* TODO Need to add device id logic */
             appMsgBleCanData->deviceId = Get_DeviceIdbyMacAddress(canRxMsgInfo->DataBufferPtr);
             appMsgBleCanData = NULL;
             if (gBleSuccess_c != BleApp_MessageCallback(mpfBleAppMessageEventHandler, pEventData))
             {
            	 (void)MEM_BufferFree(pEventData);
             }
         }
    }
}

void BleTp_TxMainFunction(uint8_t deviceId)
{
	if(Get_TpTimeoutFlag())		/* BleTp transmit timeout*/
	{
		switch(bleTpTxState[deviceId].engine)
		{
		case enum_bleTx_CF:			/* It's time to send consecutive frame */
			bleTpTxState[deviceId].sendReq = 1;
			BleTp_TxStateTask(deviceId);
			break;
		}
	}
	else
	{
		BleTp_TxStateTask(deviceId);
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
TpReturnType BleTp_Transmit(bleMsgInfo_t *bleMsgInfo)
{
	uint8_t deviceId;

	deviceId = bleMsgInfo->deviceId;

	if(bleTpTxState[deviceId].engine != enum_bleTx_Idle)
	{
		return TpBusy;
	}
	else
	{
		if(bleTxMsgInfo[deviceId].bufSts == CAN_PASSTHROUGH_BUF_IDLE)
		{
			bleTxMsgInfo[deviceId].bufSts = CAN_PASSTHROUGH_BUF_BUSY;
			bleTxMsgInfo[deviceId].bufLen = bleMsgInfo->packetLength;
			bleTxMsgInfo[deviceId].buffPtr = bleMsgInfo->pPacket;
			bleTxMsgInfo[deviceId].bufIndex = 0;
			bleTpTxState[deviceId].sendReq = 1;

			if(bleTxMsgInfo[deviceId].bufLen <= (BLE_AIR_TP_LENGTH - BLE_AIR_SF_FF_CONTROL_BYTES))
			{
				bleTpTxState[deviceId].engine = enum_bleTx_SF;
			}
			else
			{
				bleTpTxState[deviceId].engine = enum_bleTx_FF;
			}

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
			return TpFailed;
		}
	}
	return TpSuccess;
}

void BleTp_RxFromInterrupt(uint16_t dlc, uint8_t deviceId, const uint8_t *data)
{
	static uint8_t blockIndex = 0;
	static uint16_t length = 0;
	static uint8_t frameType = ble_SingleFrame;

	if(bleTpRxState[deviceId].engine == enum_canRx_Idle)
	{
		length = (data[2] & 0xFF) << 8;
		length = (((uint16_t)data[2] & 0x00FF) << 8) + data[3];
		if(length > BLE_AIR_TP_LENGTH)
		{
			frameType = ble_FirstFrame;
		}
		else
		{
			frameType = ble_SingleFrame;
		}

		if (bleTpRxState[deviceId].engine != enum_canRx_Idle)
		{
			bleRxMsgInfo[deviceId].bufSts == CAN_PASSTHROUGH_BUF_IDLE;
		}
	}

	switch(frameType)	/* Receive frame Type */
	{
	case ble_FirstFrame:
		if (dlc == BLE_AIR_TP_LENGTH)
		{
			bleRxMsgInfo[deviceId].DataLength = length;
			bleMsgAirBuff.packetLength = length + BLE_AIR_PCI_LENGTH;
			if(bleRxMsgInfo[deviceId].DataLength > BLE_AIR_TP_LENGTH - BLE_AIR_PCI_LENGTH)
			{
				bleRxMsgInfo[deviceId].DataBufferPtr = (uint8_t *)bleMsgAirBuff.pPacket;
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
		bleRxMsgInfo[deviceId].DataLength = length;
		bleMsgAirBuff.packetLength = length + BLE_AIR_PCI_LENGTH;
		if(bleRxMsgInfo[deviceId].DataLength <= (BLE_AIR_TP_LENGTH - BLE_AIR_PCI_LENGTH))
		{
			bleRxMsgInfo[deviceId].DataBufferPtr = (uint8_t *)bleMsgAirBuff.pPacket;
			bleRxMsgInfo[deviceId].bufSts = CAN_PASSTHROUGH_BUF_BUSY;
			memcpy(bleRxMsgInfo[deviceId].DataBufferPtr, data, bleRxMsgInfo[deviceId].DataLength + BLE_AIR_PCI_LENGTH);
			bleRxMsgInfo[deviceId].DataIndex += dlc;
			bleTpRxState[deviceId].engine = enum_bleRx_Idle;
			/*TODO Do something!*/
			BleTpRxIndication();
			return ;
		}
		else
		{
			BleTp_Init();		/* Receive wrong */
			bleTpRxState[deviceId].engine = enum_bleRx_Idle;
		}
		break;

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
					return ;
				}
				else
				{
					BleTp_Init();
				}
			}
			else
			{
				if(dlc == BLE_CAN_TP_LENGTH)
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
    if(mpfBleAppMessageEventHandler != NULL)
    {
         appEventData_t *pEventData = MEM_BufferAlloc(sizeof(appEventData_t) + sizeof(appMsgBleCanData_t));
         if(pEventData != NULL)
         {
             pEventData->appEvent = mAppEvt_BleMsgRevCompleteHandle_c;
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

void BleTp_Init(void)
{
	memset(&bleTxMsgInfo, 0, sizeof(bleTxMsgInfo_t));
	memset(&bleRxMsgInfo, 0, sizeof(bleRxMsgInfo_t));
	BleTpIntTxTransmitChannel_SetFree();
}

void CanTp_Init(void)
{
	memset(&canTxMsgInfo, 0, sizeof(canTxMsgInfo_t));
	memset(&canRxMsgInfo, 0, sizeof(canRxMsgInfo_t));
	CanTpIntTxTransmitChannel_SetFree();
}

void Tp_Init(void)
{
	BleTp_Init();
	CanTp_Init();
}

/*! *********************************************************************************
* \fn           uint8_t Tp_Transmit(uint32_t id, bleMsgInfo_t *bleMsgInfo)
* \brief        BLE/CAN send can message. Put data into canDataBuff. Initialize BleTP state.
* \call			由 pass through service/app 进行调用, 与UDS类型类似，由BleTp_Transmit进行调用
*
* \param  [in]  id      can message id, 当ID为INVALID_CAN_ID时，由Ble进行数据发送，否则，由canTp进行发送；
* \param  [in]  bleMsgInfo      ble message
*
* \retval       gHciSuccess_c         Initialization was successful.
********************************************************************************** */
TpReturnType Tp_Transmit(uint32_t id, bleMsgInfo_t *bleMsgInfo)
{
	if(id == INVALID_CAN_ID)
	{
		return BleTp_Transmit(bleMsgInfo);
	}
	else
	{
		return CanTp_Transmit(id, bleMsgInfo);
	}
}

void Tp_Receive(bleMsgRevInfo_t *bleMsgInfo)
{
	if(bleMsgInfo->source == messageSource_air_c)
	{
		BleTp_RxFromInterrupt(bleMsgInfo->len, bleMsgInfo->deviceId, bleMsgInfo->data);
	}
	else if(bleMsgInfo->source == messageSource_can_c)
	{
		CanTp_RxFromInterrupt(bleMsgInfo->len, bleMsgInfo->data);
	}
	else
	{
		/*TODO */
	}
}
