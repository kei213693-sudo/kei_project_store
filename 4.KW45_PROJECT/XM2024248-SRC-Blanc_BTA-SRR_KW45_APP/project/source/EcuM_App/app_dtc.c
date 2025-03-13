/*
 * app_dtc.c
 *
 *  Created on: 2024年7月1日
 *      Author: F704012276_
 */


#include "Dtc_Manage.h"
#include "canbus_app.h"
#include "DFP.h"
#include "app_dtc.h"
#include "canbus_app.h"
#include "KW45B41Z83.h"
#include "hal_adc_typedef.h"
#include "fsl_debug_console.h"
#include "dbg_printf.h"
#include "ComBusoff.h"
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"

#define VOLTAGE_DOWN_6			0
#define VOLTAGE_BET_6_6p5		1
#define VOLTAGE_BET_6p5_16		2
#define VOLTAGE_BET_16_16p5		3
#define VOLTAGE_UP_16p5			4


#define PMS_VOLTAGE_6V			1059
#define PMS_VOLTAGE_6p5V		1163
#define PMS_VOLTAGE_9V			1670
#define PMS_VOLTAGE_9p5V		1772
#define PMS_VOLTAGE_15p5V		2992
#define PMS_VOLTAGE_16V			3093
#define PMS_VOLTAGE_18V			3498
#define PMS_VOLTAGE_18p5V		3600

#define AD_FIFO_DEPTH        5
#define Get_AD_Value()			adc_result[14]

extern uint32_t adc_result[HAL_ADC0_NUM];
extern uint16_t BAT_V_Count;
extern uint16_t DTC_V_Count;
uint32_t Vbat_V = 0;
uint8_t PMVoltageState;
static uint16_t adValueFifo[AD_FIFO_DEPTH] = {0, 0, 0, 0, 0};
static uint8_t	PMState = PMS_NORMAL;
static uint8_t  VoltageDtcSts = DTC_NORMAL_VOLTAGE;
static DtcState_t dtcSts;

static uint32_t GetResetReason(void);
static void PMStateHandle(void);
static void ResetStateHandle(void);
static void BusoffDTCHandle(void);
static void CommunicationLosehandle(void);
static void VoltageDtcStsHandle(uint16_t voltage);

uint8_t Get_PMState(void)
{
	return PMState;
}

static uint32_t GetResetReason(void)
{
	return CMC0->SRS;
}

void AppDtc_MainFunction(void)
{
	ResetStateHandle();
	PMStateHandle();
	BusoffDTCHandle();
	CommunicationLosehandle();
}

static void BusoffDTCHandle(void)
{
        static uint16_t powerUpTimeoutCnt = 0;
        static uint8_t busoffTimeoutCnt = 0;

        powerUpTimeoutCnt++;
        if( (Vbat_V >= 90) && (Vbat_V <= 160) && (powerUpTimeoutCnt >= 500))
        {
                powerUpTimeoutCnt = 500;
                if(Get_canBusoffFlag() == CAN_BUSOFF)
                {
                        busoffTimeoutCnt++;
                        if(busoffTimeoutCnt >= 25)
                        {
                                busoffTimeoutCnt = 0;
                                Dtc_Active(&DFC_CANBusoff);
                                dtcSts.b.b_busoffUpdate = 1;
                        }
                }
                else
                {
                        if(dtcSts.b.b_busoffUpdate == 1)
                        {
                                dtcSts.b.b_busoffUpdate = 0;
                                Dtc_Recove(&DFC_CANBusoff);
                        }
                }
        }

}

static void CommunicationLosehandle(void)
{
        static uint16_t timeoutCnt = 0;
        static uint8_t CommLoseConditionFlag = 0;
        if( (Vbat_V >= 90) && (Vbat_V <= 160) && (Get_canBusoffFlag() == CAN_NO_BUSOFF))
        {
                if(((ZONE_13C_t.msg.sig.DrvState == DrvState_DRVSTMD_3) ||\
                                (ZONE_13C_t.msg.sig.DrvState == DrvState_DRVSTAD_4))\
                        || ((ZONE_13C_t.msg.sig.OperatorState == OperatorState_OPERATORSTAVAILABLE_8) ||\
                                (ZONE_13C_t.msg.sig.OperatorState == OperatorState_OPERATORSTINSIDE_9)))
                {
                        if(CommLoseConditionFlag == 0)
                        {
                                timeoutCnt++;
                                if(timeoutCnt > 299)
                                {
                                        CommLoseConditionFlag = 1;
                                }
                        }
                }
                else
                {
                        timeoutCnt = 0;
                        CommLoseConditionFlag = 0;
                }
        }
        else
        {
                timeoutCnt = 0;
                CommLoseConditionFlag = 0;
        }

        if(CommLoseConditionFlag == 1)
        {
                if(BCU_11E_t.On_line_flag == 0)
                {
                        dtcSts.b.b_loseControlUpdate = 1;
                        Dtc_Active(&DFC_11E_TimOut);
                }
                else if(BCU_11E_t.On_line_flag == 1)
                {
                        if(dtcSts.b.b_loseControlUpdate == 1)
                        {
                                Dtc_Recove(&DFC_11E_TimOut);
                        }
                }
        }else
        {
                Dtc_Recove(&DFC_11E_TimOut);
        }
}

static uint16_t Get_AD_Average(uint16_t data)
{
	uint16_t adSum = 0;

	for(uint8_t index = 0; index < AD_FIFO_DEPTH-1; index++)
	{
			adValueFifo[index] = adValueFifo[index+1];
			adSum = adValueFifo[index] + adSum;
	}
	adValueFifo[AD_FIFO_DEPTH-1] = data;
	adSum += data;

	return (uint16_t)(adSum/(AD_FIFO_DEPTH));
}

static void PMStateHandle(void)
{
	static uint8_t oldPMVoltageState = VOLTAGE_BET_6p5_16;
	static uint16_t currentVoltage = 0;

	currentVoltage = Get_AD_Average(Get_AD_Value());
	Vbat_V = (uint8_t)(((currentVoltage/4095.0) * 20.223 + 0.8)*10);//转化电压值存在偏差，若后续需要使用实际电压值判定，会加入电压补偿机制。
	//dbg_Printf("currentVoltage is: %d\r\n", currentVoltage);
	dfpm_exData.Timestamp_year = ZONE_3B2_t.msg.sig.Yr_phy;
	dfpm_exData.Timestamp_Month = ZONE_3B2_t.msg.sig.Mth;
	dfpm_exData.Timestamp_Day = ZONE_3B2_t.msg.sig.Day;
	dfpm_exData.Timestamp_Hour = ZONE_3B2_t.msg.sig.Hr;
	dfpm_exData.Timestamp_Minute = ZONE_3B2_t.msg.sig.Min;
	dfpm_exData.Timestamp_Second = ZONE_3B2_t.msg.sig.Sec;
	dfpm_exData.Odometer_value_h = ZONE_3AA_t.msg.sig.VehOdometer_0;
	dfpm_exData.Odometer_value_m = ZONE_3AA_t.msg.sig.VehOdometer_1;
	dfpm_exData.Odometer_value_l = ZONE_3AA_t.msg.sig.VehOdometer_2;
	dfpm_exData.Battery_voltage = Vbat_V;
	dfpm_exData.vehspd_st = BCU_11E_t.msg.sig.VehSpdSts_Rdnt1;
	dfpm_exData.vehspd_H = BCU_11E_t.msg.sig.VehSpd_Rdnt1_phy_0;
	dfpm_exData.vehspd_L = BCU_11E_t.msg.sig.VehSpd_Rdnt1_phy_1;
	dfpm_exData.can_sig_11E_timeout_flg = !BCU_11E_t.On_line_flag;
	DFPM_setDtcExtData(&dfpm_exData);

	if( currentVoltage < PMS_VOLTAGE_6V)
	{
		if(BAT_V_Count > 1000)
		{
			BAT_V_Count = 0;
			PMVoltageState = VOLTAGE_DOWN_6;
			PMState = PMS_UNDER;
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
		}

	}
	else if(currentVoltage < PMS_VOLTAGE_6p5V)
	{
		if(BAT_V_Count > 1000)
		{
			BAT_V_Count = 0;
			PMVoltageState = VOLTAGE_BET_6_6p5;
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
			if(PMState == PMS_ABOVE)
			{
				PMState = PMS_NORMAL;
			}
		}

	}
	else if(currentVoltage < PMS_VOLTAGE_18V)
	{
		if(BAT_V_Count > 1000)
		{
			BAT_V_Count = 0;
			PMVoltageState = VOLTAGE_BET_6p5_16;
			PMState = PMS_NORMAL;
		//	dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
		}
	}
	else if(currentVoltage < PMS_VOLTAGE_18p5V)
	{
		if(BAT_V_Count > 1000)
		{
			BAT_V_Count = 0;
			PMVoltageState = VOLTAGE_BET_16_16p5;
		//	dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
			if(PMState == PMS_UNDER)
			{
				PMState = PMS_NORMAL;
			}
		}
	}
	else	/*AD > 16.5V*/
	{
		if(BAT_V_Count > 1000)
		{
			BAT_V_Count = 0;
			PMVoltageState = VOLTAGE_UP_16p5;
			PMState = PMS_ABOVE;
		//	dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
		}
	}

	switch(PMState)
	{
	case PMS_NORMAL:
		VoltageDtcStsHandle(currentVoltage);
		break;

	case PMS_UNDER:
		break;

	case PMS_ABOVE:
		break;

	default:
		break;
	}
}

static void ResetStateHandle(void)
{
	static uint32_t ResetReason = 0;
	static uint8_t firstFlag = 1;

	ResetReason = GetResetReason();

	if(firstFlag == 1)
	{
		firstFlag = 0;
		if(ResetReason & CMC_SRS_WDOG0_MASK)
		{
			dtcSts.b.b_watchRstUpdate = 1;
			Dtc_Active(&DFC_KW45_WDOG_RESET);
		}

		if(ResetReason & CMC_SRS_LVD_MASK)
		{
			/*Dtc_Active(&DFC_KW45_LVD_RESET);*/
		}

		if(ResetReason & CMC_SRS_HVD_MASK)
		{
			dtcSts.b.b_HVDRstUpdate = 1;
			Dtc_Active(&DFC_KW45_HVD_RESET);
		}
	}
	else
	{
		if(dtcSts.b.b_watchRstUpdate == 1)
		{
			dtcSts.b.b_watchRstUpdate = 0;
			Dtc_Recove(&DFC_KW45_WDOG_RESET);
		}
		/*Dtc_Recove(&DFC_KW45_LVD_RESET);*/

		if(dtcSts.b.b_HVDRstUpdate == 1)
		{
			dtcSts.b.b_HVDRstUpdate = 0;
			Dtc_Recove(&DFC_KW45_HVD_RESET);
		}
	}
}


static void VoltageDtcStsHandle(uint16_t voltage)
{
	if( voltage < PMS_VOLTAGE_9V)
	{
		if(DTC_V_Count > 1000)
		{
			DTC_V_Count = 0;
			VoltageDtcSts = DTC_UNDER_VOLTAGE;
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
		}
	}
	else if(voltage < PMS_VOLTAGE_9p5V)
	{
		if(DTC_V_Count > 1000)
		{
			DTC_V_Count = 0;
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
			if(VoltageDtcSts == DTC_OVER_VOLTAGE)
			{
				VoltageDtcSts = DTC_NORMAL_VOLTAGE;
			}
		}
	}
	else if(voltage < PMS_VOLTAGE_15p5V)
	{
		if(DTC_V_Count > 1000)
		{
			DTC_V_Count = 0;
			VoltageDtcSts = DTC_NORMAL_VOLTAGE;
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
		}
	}
	else if(voltage < PMS_VOLTAGE_16V)
	{
		if(DTC_V_Count > 1000)
		{
			DTC_V_Count = 0;
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
			if(VoltageDtcSts == DTC_UNDER_VOLTAGE)
			{
				VoltageDtcSts = DTC_NORMAL_VOLTAGE;
			}
		}
	}
	else	/*AD > 16.5V*/
	{
		if(DTC_V_Count > 1000)
		{
			DTC_V_Count = 0;
			VoltageDtcSts = DTC_OVER_VOLTAGE;
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
		}
	}

	switch(VoltageDtcSts)
	{
	case DTC_NORMAL_VOLTAGE:
		if(dtcSts.b.b_underVoltageUpdate == 1)
		{
			dtcSts.b.b_underVoltageUpdate = 0;
			Dtc_Recove(&DFC_BattMin);
		}
		if(dtcSts.b.b_OverVoltageUpdate == 1)
		{
			dtcSts.b.b_OverVoltageUpdate = 0;
			Dtc_Recove(&DFC_BattMax);
		}
		break;

	case DTC_UNDER_VOLTAGE:
		dtcSts.b.b_underVoltageUpdate = 1;
		Dtc_Active(&DFC_BattMin);
		break;

	case DTC_OVER_VOLTAGE:
		dtcSts.b.b_OverVoltageUpdate = 1;
		Dtc_Active(&DFC_BattMax);
		break;

	default:
		break;
	}
}
