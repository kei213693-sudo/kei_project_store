/*
 * ComBusoff.h
 *
 *  Created on: 2024年6月28日
 *      Author: F704012276_
 */

#ifndef CAN_NM_COMBUSOFF_H_
#define CAN_NM_COMBUSOFF_H_

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "Dtc_Manage.h"
#include "DFP.h"

#define CAN_NO_BUSOFF		0
#define CAN_BUSOFF			1

typedef enum
{
	No_Busoff = 0,
	Busoff_fastRecovery,//快恢复
	Busoff_slowRecovery,//慢恢复
}busoffState_t;

typedef struct
{
	uint8_t canBusoffFlag : 1;//busoff错误标志
	uint8_t canReInitDone : 1;
	uint8_t canBusoffSleepFlag : 1;

	uint8_t reserved:5;
} canBusoffFlagBits_t;

void Set_canBusoffFlag(uint8_t flag);
uint8_t Get_canBusoffFlag(void);

void Busoff_SetSleepFlag(uint8_t statusint);
uint8_t Busoff_GetSleepFlag(void);

void Set_BusoffInitCanFlag(uint8_t data);
uint8_t Get_BusoffInitCanFlag(void);

void can_busoff_proc(void);
void Busoff_timer(void);

#endif /* CAN_NM_COMBUSOFF_H_ */

