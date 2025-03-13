/*
***************************************************************************************
* FileName:  canbus_drv.c
* Description:
* Date:
***************************************************************************************
*/
#include <string.h>
#include "hal_rte.h"
#include "hal_can_if.h"
#include "canbus_app.h"
#include "canbus_trans.h"
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"
#include "canbus_drv.h"
#include "hal_test.h"
#include "ComBusoff.h"
#include "ble_signals_api.h"
#include "bleIF_api.h"
#include "bleTP_api.h"
//#include "uds api.h"
#include "UWB_CAN.h"
//uint8_t canbus_transmitting;
static uint8_t canbus_transmitting = 0;
static uint32_t canbus_tx_succe_id = 0, canbus_tx_confirm_id = 0, canbus_tx_wait_confirm_id = 0;
static uint32_t last_dwMsgID;
static uint8_t last_len;
static uint8_t last_sndbuf[64];
volatile uint32_t CANInt_Cnt;

uint8_t canbusDrvTransmit(uint32_t id, uint8_t dlc, uint8_t *data)
{
	static uint16_t canSndErrorPrintCnt = 0;
	static uint32_t canSndErrorCounter = 0;

	hal_result_t l_i8_result;
	uint8_t ret;

	hal_can_pdu_st pdu={0};
	pdu.m_u32_canid = id;
	pdu.m_id_type_e = HAL_CAN_ID_TYPE_StAND;
	pdu.m_u8_dlc = dlc;
	pdu.m_u8_pdata = data;

	if(0x570 == id)
	{
			pdu.m_frame_type_e = HAL_CAN_FRAME_TYPE_CAN;
	}
	else
	{
			pdu.m_frame_type_e = HAL_CAN_FRAME_TYPE_CANFD;
	}

	l_i8_result = hal_can_if_write(0, &pdu, 0);
	if(HAL_RESULT_SUCCESS == l_i8_result)
	{
		canSndErrorPrintCnt = 0;
		canSndErrorCounter = 0;
		ret = CANBUS_SUCCESS;
	}
	else
	{
		canSndErrorCounter ++;
		canSndErrorPrintCnt %= 200;
		if(0 == canSndErrorPrintCnt)
		{
			//logPrintf("hal_can_if_write() return result:%d, canSndErrorCounter = %d\r\n", l_i8_result, canSndErrorCounter);
		}
		canSndErrorPrintCnt++;
		ret = CANBUS_FAIL;
	}

	if(CANBUS_SUCCESS == ret)
	{
		canbus_transmitting = 1;

		last_dwMsgID = id;
		last_len = dlc;
		memcpy(last_sndbuf, data, dlc);
	}
	else
	{
		canbus_transmitting = 0;
	}

	return ret;
}

void CANIf_Indication(const hal_can_pdu_st * pdata)
{
	static bleMsgRevInfo_t bleMsgRxInfo;
	if(pdata != NULL)
	{
		switch(pdata->m_u32_canid)
		{
			case UWB_CAN_UCI_CMD_RECV_CAN_ID:
				uwbCan_UciMsgRxBuff.rx_flag = 1;
				uwbCan_UciMsgRxBuff.rx_len = pdata->m_u8_dlc;
				memcpy(uwbCan_UciMsgRxBuff.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case ZONE_L_TPMS_01_CANID:
					ZONE_L_TPMS_01_481_bf.rx_flag = 1;
					ZONE_L_TPMS_01_481_bf.rx_len = pdata->m_u8_dlc;
					memcpy(ZONE_L_TPMS_01_481_bf.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case BCU_11E_CANID:
					BCU_11E_11E_bf.rx_flag = 1;
					BCU_11E_11E_bf.rx_len = pdata->m_u8_dlc;
					memcpy(BCU_11E_11E_bf.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case ZONE_3AA_CANID:
					ZONE_3AA_bf.rx_flag = 1;
					ZONE_3AA_bf.rx_len = pdata->m_u8_dlc;
					memcpy(ZONE_3AA_bf.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case ZONE_3B2_CANID:
					ZONE_3B2_bf.rx_flag = 1;
					ZONE_3B2_bf.rx_len = pdata->m_u8_dlc;
					memcpy(ZONE_3B2_bf.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case ZONE_13C_CANID:
				ZONE_13C_bf.rx_flag = 1;
				ZONE_13C_bf.rx_len = pdata->m_u8_dlc;
				memcpy(ZONE_13C_bf.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case BLE_Downlink_Data_CANID:
//				BLE_Downlink_Data_bf.rx_flag = 1;
//				BLE_Downlink_Data_bf.rx_len = pdata->m_u8_dlc;
				memcpy(BLE_Downlink_Data_bf.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				bleMsgRxInfo.source = messageSource_can_c;
				bleMsgRxInfo.data = BLE_Downlink_Data_bf.rx_buff;
				bleMsgRxInfo.len = pdata->m_u8_dlc;
				bleMsgRxInfo.canId = BLE_Downlink_Data_CANID;
				BleIf_Receive(&bleMsgRxInfo);
				break;
			case BLE_Uplink_FlowCtrl_CANID:

				break;
			case VC_Data_5E0_CANID:
				VC_Data_5E0_bf.rx_flag = 1;
				VC_Data_5E0_bf.rx_len = pdata->m_u8_dlc;
				memcpy(VC_Data_5E0_bf.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
			break;
			case ZONE_296_CANID:
				ZONE_296_bf.rx_flag = 1;
				ZONE_296_bf.rx_len = pdata->m_u8_dlc;
				memcpy(ZONE_296_bf.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
			break;
		}
	}
}

#if !CANBUS_SEND_USE_TASK
static void canbusDrvSendCbk(uint32_t id)
{
	uint8_t ret;
	uint32_t dwMsgID;
	uint8_t len;
	uint8_t sndbuf[64];
	Set_canBusoffFlag(CAN_NO_BUSOFF);
	UdsApi_Can_TxConfirm_cbk(id);

	if(canbus_transmitting)
	{
		if(CANBUS_SUCCESS == canbus_fifo_pop_data(&canbusTransFifo, &dwMsgID, &len, sndbuf))
		{
			ret = canbusDrvTransmit(dwMsgID, len, sndbuf);
		}
		else
		{
			canbus_transmitting = 0;
		}
	}
	(void)ret;
}
#else
uint8_t canbusDrvSendIsConfirm(uint32_t id)
{
	uint8_t ret;
	NVIC_DisableIRQ(CAN0_IRQn);
	ret = (canbus_tx_confirm_id == id);
	NVIC_EnableIRQ(CAN0_IRQn);
	return ret;
}
void canbusDrvPrintIdInfo(void)
{
	logPrintf("canbus_tx_succe_id = 0x%X, wait_confirm_id = 0x%X, confirm_id = 0x%X\r\n", canbus_tx_succe_id, canbus_tx_wait_confirm_id, canbus_tx_confirm_id);
}

void canbusDrvSendClrConfirmId(void)
{
	NVIC_DisableIRQ(CAN0_IRQn);
	canbus_tx_wait_confirm_id = 0;
	canbus_tx_confirm_id = 0;
	//canbus_tx_succe_id = 0;
	NVIC_EnableIRQ(CAN0_IRQn);
}
void canbusDrvSendWaitConfirmId(uint32_t id)
{
	NVIC_DisableIRQ(CAN0_IRQn);
	canbus_tx_wait_confirm_id = id;
	canbus_tx_confirm_id = 0;
	canbus_tx_succe_id = 0;
	NVIC_EnableIRQ(CAN0_IRQn);
}
static void canbusDrvSendCbk(uint32_t id)
{
	//Set_canBusoffFlag(0);
	Set_canBusoffFlag(CAN_NO_BUSOFF);
	CANInt_Cnt ++;
	canbus_tx_succe_id = id;
	UdsApi_Can_TxConfirm_cbk(id);
	if(id == canbus_tx_wait_confirm_id)
	{
		canbus_tx_confirm_id = id;
		extern SemaphoreHandle_t xSemaphore_canSend;
		BaseType_t xHigherPriorityTaskWoken;
		xSemaphoreGiveFromISR(xSemaphore_canSend, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
#endif

static void canbusDrvErrorCbk(void)
{
	canbus_transmitting = 0;
	logPrintf("CanBus Error Cbk!\r\n");
}

void hal_can_busoff_cbk(void)
{
	Set_canBusoffFlag(1);
	canbusDrvErrorCbk();
}

void canbusDrvInit(void)
{
	hal_can_if_init(CANFD_INDEX);
	hal_can_if_recv_callback(CANFD_INDEX, hal_can_cbk);
	hal_can_if_tx_callback(CANFD_INDEX, canbusDrvSendCbk);
	hal_can_if_busoff_callback(CANFD_INDEX, hal_can_busoff_cbk);
	NVIC_SetPriority(CAN0_IRQn, 4);
}

uint8_t canbusDrvIsTransmitting(void)
{
	return canbus_transmitting;
}







