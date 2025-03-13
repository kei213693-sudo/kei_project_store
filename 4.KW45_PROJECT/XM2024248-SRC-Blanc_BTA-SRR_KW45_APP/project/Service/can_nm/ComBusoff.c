/*
 * ComBusoff.c
 *
 *  Created on: 2024年6月28日
 *      Author: F704012276_
 */

#include "ComBusoff.h"
#include "hal_rte.h"
#include "hal_test.h"
#include "canbus_app.h"

#define BUS_OFF_L1_TICKS		50		/* 1ms*100 = 100ms */
#define BUS_OFF_L2_TICKS		1000		/* 1ms*1000 = 1000ms */
#define BUS_OFF_L1_L2_TIMES		5	/* fast recovery to slow recovery time */

/* Private variables*/
static canBusoffFlagBits_t canBusoffFlags;
static uint8_t can_isBufoff = CAN_NO_BUSOFF;//busoff错误标志
static uint8_t busoffState = No_Busoff;//恢复标志
static uint16_t fastRecoveryticks = 0;//快恢复计数
static uint16_t slowRecoveryticks = 0;//慢恢复计数
static uint32_t busoff_sleepcount = 0;//强制休眠计数

/* Private functions*/
static void No_busoffHandle(void);
static void Busoff_fastRecoveryHandle(void);
static void Busoff_slowRecoveryHandle(void);

//设置busoff错误标志
void Set_canBusoffFlag(uint8_t flag)
{
	canBusoffFlags.canBusoffFlag = flag;
}

uint8_t Get_canBusoffFlag(void)
{
	return canBusoffFlags.canBusoffFlag;
}

void Busoff_SetSleepFlag(uint8_t statusint)
{
	canBusoffFlags.canBusoffSleepFlag = statusint;
}

uint8_t Busoff_GetSleepFlag(void)
{
	return canBusoffFlags.canBusoffSleepFlag;
}

/* Initialize can driver flag*/
void Set_BusoffInitCanFlag(uint8_t data)
{
	canBusoffFlags.canReInitDone = data;
}

/* Initialize can driver*/
uint8_t Get_BusoffInitCanFlag(void)
{
	return canBusoffFlags.canReInitDone;
}

void Busoff_timer(void)
{
	if(Busoff_fastRecovery == busoffState)
	{
		fastRecoveryticks++;
	}
	else if(Busoff_slowRecovery == busoffState)
	{
		slowRecoveryticks++;
		if(Busoff_GetSleepFlag() == 0)
		{
			busoff_sleepcount++;
		}
	}
	else
	{
		fastRecoveryticks = 0;
		slowRecoveryticks = 0;
	}
}


/*
 * busoff function:
 * when busoff err detected, Get_canBusoffFlag() = CAN_BUSOFF.
 * Then, fast recovery started and Tx message will be transmitted.
 * When fast recovery is tried 5 times,
 * and can controller is still in busoff,
 * can controller will enter slow recovery.
 * */
void can_busoff_proc(void)
{
	static uint8_t busoff_L1_L2Times = 0;
	static uint8_t	cnt[8] = {0};

	switch(busoffState)
	{
		case No_Busoff:
			if(Get_canBusoffFlag() == CAN_BUSOFF)
			{//出现问题，进入错误状态，执行快恢复模式
				busoff_L1_L2Times = 0;
				busoffState = Busoff_fastRecovery;
			}
			else
			{
				busoff_sleepcount = 0;
			}
			break;

		case Busoff_fastRecovery:
			if(fastRecoveryticks >= BUS_OFF_L1_TICKS)
			{//每50ms判定一次
				if(Get_canBusoffFlag() == CAN_BUSOFF)
				{//还是错误状态
					if(busoff_L1_L2Times < BUS_OFF_L1_L2_TIMES)
					{
						busoff_L1_L2Times++;
						fastRecoveryticks = 0;

						hal_can_if_deinit();//can去初始化
						canbusDrvInit();//can comm重新初始化
						Set_canBusoffFlag(CAN_NO_BUSOFF);
						BUSOFF_MsgSend();//发送busoff恢复信息
						dbg_Printf("fast recovery.\r\n");
					}
					else
					{//重试5次还没成功，执行慢恢复模式
						busoff_L1_L2Times = 0;
						busoffState = Busoff_slowRecovery;
					}
				}
				else
				{
					busoffState = No_Busoff;
				}
			}
			break;

		case Busoff_slowRecovery:
			if(slowRecoveryticks >= BUS_OFF_L2_TICKS)
			{//每1000ms判定一次
				if(Get_canBusoffFlag() == CAN_BUSOFF)
				{
					slowRecoveryticks = 0;

					hal_can_if_deinit();//can去初始化
					canbusDrvInit();//can comm重新初始化
					Set_canBusoffFlag(CAN_NO_BUSOFF);
					BUSOFF_MsgSend();//发送busoff恢复信息
					dbg_Printf("slow recovery.\r\n");

					if(busoff_sleepcount >= 300000)
					{//重试30次慢恢复还不成功，就把强制休眠标志置位
						busoff_sleepcount = 0;
						Busoff_SetSleepFlag(1);
					}
				}
				else
				{
					busoffState = No_Busoff;
				}
			}
			break;
		default:
			break;
	}
}
