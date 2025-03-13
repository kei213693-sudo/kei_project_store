/*
 * BtaPos.c
 *
 *  Created on: 2024年7月3日
 *      Author: F704012276_
 */

#define PIN_HIGH	1
#define PIN_LOW		0

#include "hal_pin_if.h"
#include "hal_rte.h"
#include "BtaPos.h"

BtaPos_t Get_BTAPos(void)
{
	static uint8_t canId1 = 0;
	static uint8_t canId2 = 0;

	//canId1 = hal_pin_if_read(CANID1_INDEX);
	//canId2 = hal_pin_if_read(CANID2_INDEX);
	/*后保*/
	if((canId1 == PIN_LOW) && (canId2 == PIN_LOW))
	{
		return External_Rear;
	}
	/*前保*/
	if((canId1 == PIN_LOW) && (canId2 == PIN_HIGH))
	{
		return External_Front;
	}

}
