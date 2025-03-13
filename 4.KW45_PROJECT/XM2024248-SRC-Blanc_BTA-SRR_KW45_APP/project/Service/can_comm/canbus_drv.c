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
#include "fsl_os_abstraction.h"
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"
#include "canbus_drv.h"
#include "hal_test.h"
#include "ComBusoff.h"
#include "UWB_CAN.h"

uint8_t canbus_transmitting;
static uint8_t canbus_tx_confirm_flag = false;
static uint32_t canbus_tx_confirm_id = 0;
static uint32_t last_dwMsgID;
static uint8_t last_len;
static uint8_t last_sndbuf[64];

uint8_t canbusDrvTransmit(uint32_t id, uint8_t dlc, uint8_t *data)
{
	hal_result_t l_i8_result;
	uint8_t ret;

	hal_can_pdu_st pdu={0};
	pdu.m_u32_canid = id;
	pdu.m_id_type_e = HAL_CAN_ID_TYPE_StAND;
	pdu.m_u8_dlc = dlc;
	pdu.m_u8_pdata = data;

	l_i8_result = hal_can_if_write(CANFD_INDEX, &pdu);
	if(HAL_RESULT_SUCCESS == l_i8_result)
	{
		ret = CANBUS_SUCCESS;
	}
	else
	{
		ret = CANBUS_FAIL;
	}

	if(CANBUS_SUCCESS == ret)
	{
		canbus_transmitting = 1;
		last_dwMsgID = id;
		last_len = dlc;
		memcpy(last_sndbuf, data, dlc);
		canbus_tx_confirm_id = id;
		canbus_tx_confirm_flag = false;
	}
	else
	{
		canbus_transmitting = 0;
	}

	return ret;
}

void CANIf_Indication(const hal_can_pdu_st * pdata)
{
	if(pdata != NULL)
	{
		switch(pdata->m_u32_canid)
		{
			case UWB_CAN_UCI_CMD_RECV_CAN_ID:
				uwbCan_UciMsgRxBuff.rx_flag = 1;
				uwbCan_UciMsgRxBuff.rx_len = pdata->m_u8_dlc;
				memcpy(uwbCan_UciMsgRxBuff.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case DV_TEST_UPPER_296_CAN_ID:
				canMsg296RxBuff.rx_flag = 1;
				canMsg296RxBuff.rx_len = pdata->m_u8_dlc;
				memcpy(canMsg296RxBuff.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case DV_TEST_SRF_1CC_CAN_ID:
				canMsg1ccRxBuff.rx_flag = 1;
				canMsg1ccRxBuff.rx_len = pdata->m_u8_dlc;
				memcpy(canMsg1ccRxBuff.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case DV_TEST_SRF_1CA_CAN_ID:
				canMsg1caRxBuff.rx_flag = 1;
				canMsg1caRxBuff.rx_len = pdata->m_u8_dlc;
				memcpy(canMsg1caRxBuff.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
				break;
			case BCU_11E_CANID:
				BCU_11E_bf.rx_flag = 1;
				BCU_11E_bf.rx_len = pdata->m_u8_dlc;
				memcpy(BCU_11E_bf.rx_buff, pdata->m_u8_pdata, pdata->m_u8_dlc);
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
			default:
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
	if(0 == id)
	{
		return canbus_tx_confirm_flag;
	}
	else
	{
		if(id == canbus_tx_confirm_id)
		{
			return canbus_tx_confirm_flag;
		}
		else
		{
			return false;
		}
	}
}
static void canbusDrvSendCbk(uint32_t id)
{
	Set_canBusoffFlag(CAN_NO_BUSOFF);
	UdsApi_Can_TxConfirm_cbk(id);
	if(canbus_tx_confirm_id == id) canbus_tx_confirm_flag = true;
	extern SemaphoreHandle_t xSemaphore_canSend;
	BaseType_t xHigherPriorityTaskWoken;
	xSemaphoreGiveFromISR(xSemaphore_canSend, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
#endif

static void canbusDrvErrorCbk(void)
{
	canbus_transmitting = 0;
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







