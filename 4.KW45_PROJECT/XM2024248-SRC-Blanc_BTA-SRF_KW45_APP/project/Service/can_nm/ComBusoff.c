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
#include "canbus_drv.h"
#include "logger.h"
#include "fsl_flexcan.h"

#define BUS_OFF_L1_TICKS		50		/* 1ms*100 = 100ms */
#define BUS_OFF_L2_TICKS		1000		/* 1ms*1000 = 1000ms */
#define BUS_OFF_L1_L2_TIMES		5	/* fast recovery to slow recovery time */

#define CAN_REINIT_DONE		1
#define CAN_NO_REINIT		0

/* Private variables*/
static canBusoffFlagBits_t canBusoffFlags;
static uint8_t can_isBufoff = CAN_NO_BUSOFF;
static uint8_t busoffState = No_Busoff;
static uint16_t fastRecoveryticks = 0;
static uint16_t slowRecoveryticks = 0;
static uint8_t busoff_sleepflag = 0;
static uint32_t busoff_sleepcount = 0;
static freezeCanNmCmd_t freezeCanNmCmd = can_DefaultCMD;

/* Private functions*/
static void No_busoffHandle(void);
static void Busoff_fastRecoveryHandle(void);
static void Busoff_slowRecoveryHandle(void);

uint8_t Get_canBusoffSts(void)
{
        return busoffState;
}

void Set_canBusoffSts(uint8_t sts)
{
	busoffState = busoffState;
}

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

uint8_t Busoff_GetSleepFlag()
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
                if(Busoff_GetSleepFlag() == 0)
                {
                        busoff_sleepcount++;
                }
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

void update_canBusoffSts(void)
{

}

/*
 * busoff function:
 * when busoff err detected, Get_canBusoffFlag() = CAN_BUSOFF.
 * Then, fast recovery started and Tx message will be transmitted.
 * When fast recovery is tried 5 times,
 * and CAN controller is still in busoff,
 * CAN controller will enter slow recovery.
 * */
#define CAN_REINIT_CONTROL_MASK		0x01
#define CAN_RESEND_MESSAGE_MASK		0X02
#define CAN_REINTI_ALL_MASK			0x03
void can_busoff_proc(void)
{
        static uint8_t busoff_L1_L2Times = 0;
        static uint8_t        cnt[8] = {0};
        static uint8_t canRecoveryFlag = 0;

        if(Get_canBusoffFlag() == CAN_NO_BUSOFF )
        {
                busoff_L1_L2Times = 0;
                busoffState = No_Busoff;
        }
        else if((Get_canBusoffFlag() == CAN_BUSOFF) && (busoffState == No_Busoff))
        {
                busoffState = Busoff_fastRecovery;
        }
        else if((Get_canBusoffFlag() == CAN_BUSOFF) && (busoffState == Busoff_fastRecovery) && (busoff_L1_L2Times >= BUS_OFF_L1_L2_TIMES))
        {
                busoffState = Busoff_slowRecovery;
        }

        switch(busoffState)
        {
        case No_Busoff:
                busoff_sleepcount = 0;
                break;

        case Busoff_fastRecovery:
                if(fastRecoveryticks >= BUS_OFF_L1_TICKS)
                {
                        busoff_L1_L2Times++;
                        fastRecoveryticks = 0;
                        canRecoveryFlag = 1;
                        shell_write("\r\nFast Recovery!");
                }
                break;

        case Busoff_slowRecovery:
                if(slowRecoveryticks >= BUS_OFF_L2_TICKS)
                {
                        slowRecoveryticks = 0;
                        canRecoveryFlag = 1;
                        dbg_Printf("\r\nSlow recovery.");
                }
                break;

        default:
                break;
        }

        if(canRecoveryFlag == 1)
        {
                hal_can_if_deinit();
                canbusDrvInit();
                if(CanSM_GetIsNetwork())        /* NM为Network */
                {
                        BUSOFF_MsgSend();
                }
                canRecoveryFlag = 0;
        }

        if(busoff_sleepcount >= 300000)
        {
//                Set_canBusoffFlag(CAN_NO_BUSOFF);
                busoffState = No_Busoff;
                /*freezeCanNmCmd = can_unFreezeNMCmd;*/
                busoff_sleepcount = 0;
                Busoff_SetSleepFlag(1);
        }
}

freezeCanNmCmd_t Get_FreezeCanNmCMD(void)
{
	return freezeCanNmCmd;
}
