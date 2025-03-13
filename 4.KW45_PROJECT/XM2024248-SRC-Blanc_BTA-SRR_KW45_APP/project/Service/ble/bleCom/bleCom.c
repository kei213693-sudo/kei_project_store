/*
 * bleCom.c
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */


#include "bleCom_cfg.h"
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"

uint8_t bleMsgTxRequest[enum_bleMsgTx_total_num] = {0};
uint8_t bleMsgRxRequest[enum_bleMsgRx_total_num] = {0};

static void BleCom_RxTx(void);
static void BleCom_Rx(void);
static void BleCom_Tx(void);

void BleCom_Init(void)
{
	for (uint8_t i = 0; i < enum_bleMsgTx_total_num; i++)
	{
		/*FLib_MemSet(bleMsgTxPtr[i], 0x22, bleMsgTxLen[i]);*/
	}

	for (uint8_t i = 0; i < enum_bleMsgRx_total_num; i++)
	{
		/*FLib_MemSet(bleMsgRxPtr[i], 0x22, bleMsgRxLen[i]);*/
	}
}
void BleCom_MainFunction(void)
{
	BleCom_Rx();
	BleCom_Tx();
}

static void BleCom_Rx(void)
{
	/* Prase data or pass through data */
//	if(BLE_Downlink_Data_t.On_line_flag == 1)
//	{
//		for(uint8_t handle = 0; handle < enum_bleMsgRx_total_num; handle++)
//		{
//			if(bleMsgRxRequest[handle] == 1)
//			{
//				/* Prase Data */
//			}
//		}
//	}

}

static void BleCom_Tx(void)
{
	/*是否有发送事件*/
//	for(uint8_t handle = 0; handle < enum_bleMsgTx_total_num; handle++)
//	{
//		if(bleMsgTxRequest[handle] == 1)
//		{
///*			bleMsgTxInfo.pPacket = bleMsgTxPtr[handle];
//			bleMsgTxInfo.packetLength = bleMsgTxLen[handle];
//			BlePduR_Transmit(&bleMsgTxInfo);*/
//		}
//	}
}
