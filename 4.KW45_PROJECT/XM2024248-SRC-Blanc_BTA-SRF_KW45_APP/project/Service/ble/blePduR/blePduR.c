/*
 * blePduR.c
 *
 *  Created on: 2024年6月13日
 *      Author: F704012276_
 */

#include "blePduR_cfg.h"
typedef void (*fctname) (uint8_t *data);

#define Msg2CAN		0x55
#define Msg2BLE		0x66
#define BLE2CAN_MAC_LEN				6u
#define CANFD_DATA_LEN				64U

#define BTA1_Zone_F_Req_CANID				0x4E1
#define Zone_F_BTA1_Req_CANID				0x4E2
#define BTA1_Zone_F_FlwCtrl_CANID			0x4E3
#define Zone_F_BTA1_FlwCtrl_CANID			0x4E4

static uint8_t canTpBuff[64] = {0};
static uint8_t bleTpBuff[200] = {0};

void BlePduR_CanIfRxIndication(bleMsgInfo_t *bleMsgInfo);

void BlePduR_CanIfRxIndication(bleMsgInfo_t *bleMsgInfo)
{
	/*Do Nothing*/
}

void BlePduR_Transmit(bleMsgInfo_t *bleMsgInfo)
{
	if(bleMsgInfo->destination == Msg2CAN)		/* pass through data to can */
	{
//		CANTp_Transmit(BTA1_Zone_F_Req_CANID, bleMsgInfo->packetLength + BLE2CAN_MAC_LEN, bleMsgInfo->pPacket);
	}
	else if(bleMsgInfo->destination == Msg2BLE)	/* pass through data to BLE */
	{
		if(bleMsgInfo->packetLength < 20)
		{
//			BleIf_Transmit(bleMsgInfo);
		}
		else
		{
			/* 只是将数据放入到Buffer */
//			BleTp_Transmit();
		}
	}
	else
	{
		/* for MISRA C*/
	}
}

