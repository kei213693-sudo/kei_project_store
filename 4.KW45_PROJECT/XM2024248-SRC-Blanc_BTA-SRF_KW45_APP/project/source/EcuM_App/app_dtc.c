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
#include "KW45B41Z83.h"
#include "hal_adc_typedef.h"
#include "fsl_debug_console.h"
#include "logger.h"
#include "ComBusoff.h"
#include "onvo_nt3_lite_bta_srf_v_1_10_0.h"
#include "autosar_nm.h"
#include "canbus_app.h"
#include "E2Para.h"

#define VOLTAGE_DOWN_6			0
#define VOLTAGE_BET_6_6p5		1
#define VOLTAGE_BET_6p5_16		2
#define VOLTAGE_BET_16_16p5		3
#define VOLTAGE_UP_16p5			4

#define PMS_VOLTAGE_6V          1059
#define PMS_VOLTAGE_6p5V        1158
#define PMS_VOLTAGE_9V			1677
#define PMS_VOLTAGE_9p5V		1780
#define PMS_VOLTAGE_15p5V		3004
#define PMS_VOLTAGE_16V			3107
#define PMS_VOLTAGE_18V         3510
#define PMS_VOLTAGE_18p5V       3613

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
uint8_t RF_initvalue_flag = 0;
static uint32_t GetResetReason(void);
static void PMStateHandle(void);
static void ResetStateHandle(void);
static void BusoffDTCHandle(void);
static void CommunicationLosehandle(void);
static void VoltageDtcStsHandle(uint16_t voltage);

static uint8_t RFSpiCommDTC_record = 0, RFSpiCommDTC_recovery = 0;
static uint8_t spi_error_count = 0, error_f = 0;
void ClrKW45_RFCSpiCommDtcState(void)
{
        spi_error_count = 0;
        error_f = 0;
        RFSpiCommDTC_record = 0;
        RFSpiCommDTC_recovery = 0;
}

void SetKW45_RFSpiCommResult(uint8_t is_succe)
{

    if(is_succe)
    {
    	spi_error_count = 0;
    	if(error_f)
    	{
    		error_f = 0;
    		RFSpiCommDTC_recovery = 1;
    	}
    }
    else
	{
    	if(!error_f)
    	{
			spi_error_count ++;
			if(spi_error_count >= 3)
			{
				error_f = 1;
				RFSpiCommDTC_record = 1;
			}
    	}
	}
}

static void RFSpiCommDTC_recordFunc(void)
{
	if(RFSpiCommDTC_record)
	{
		RFSpiCommDTC_record = 0;
		Dtc_Active(&DFC_29D6_COM_LOST);
	}
	else if(RFSpiCommDTC_recovery)
	{
		RFSpiCommDTC_recovery = 0;
		Dtc_Recove(&DFC_29D6_COM_LOST);
	}
}

static uint8_t RFInitDTC_record = 0, RFInitDTC_recovery = 0;
void RFInitDTC_recordSet(void)
{
	RFInitDTC_record = 1;
}
void RFInitDTC_recoverySet(void)
{
	RFInitDTC_recovery = 1;
}
static void RFInitDTC_recordFunc(void)
{
	if(RFInitDTC_record)
	{
		RFInitDTC_record = 0;
		Dtc_Active(&DFC_RF_CHIP_COM_ERROR);
		RF_initvalue_flag = 1;
		//Dtc_Recove(&DFC_RF_CHIP_COM_ERROR);
	}
	else if(RFInitDTC_recovery)
	{
		RFInitDTC_recovery = 0;
		Dtc_Recove(&DFC_RF_CHIP_COM_ERROR);
	}
}

static uint8_t uwbFaultDTC_record = 0, uwbFaultDTC_recovery = 0;
static uint8_t uwbFault_count = 0, uwbFault_flg = 0;
void ClrKW45_UWBFaultDtcState(void)
{
		uwbFault_count = 0;
        uwbFault_flg = 0;
        uwbFaultDTC_record = 0;
        uwbFaultDTC_recovery = 0;
}

void SetKW45_UWBFaultState(uint8_t is_succe)
{

    if(is_succe)
    {
    	uwbFault_count = 0;
    	if(uwbFault_flg)
    	{
    		uwbFault_flg = 0;
    		uwbFaultDTC_recovery = 1;
    	}
    }
    else
	{
    	if(!uwbFault_flg)
    	{
    		uwbFault_count ++;
			if(uwbFault_count >= 3)
			{
				uwbFault_flg = 1;
				uwbFaultDTC_record = 1;
			}
    	}
	}
}

static void UWBFaultDTC_recordFunc(void)
{
	if(uwbFaultDTC_record)
	{
		uwbFaultDTC_record = 0;
		Dtc_Active(&DFC_UWB_BASE_FAIL);
	}
	else if(uwbFaultDTC_recovery)
	{
		uwbFaultDTC_recovery = 0;
		Dtc_Recove(&DFC_UWB_BASE_FAIL);
	}
}

/*
0x01=WAKEUP Wake-up Reset/唤醒复位
0x02=POR Power-on Reset/Power-on复位
0x03=LVD Low Voltage Detect Reset/低电压检测复位
0x04=HVD High Voltage Detect Reset/高电压检测复位
0x05=WARM Warm Reset/热复位
0x06=FATAL Fatal Reset/Fatal复位
0x08=Reset Pin Reset/复位管脚复位
0x09=DAP Debug Access Port Reset/Debug管脚复位
0x0A=RSTACK Reset Timeout/系统复位生成逻辑中出现超时或其他错误条件导致复位
0x0B=LPACK Low Power Acknowledge Timeout Reset/低功耗模式进入逻辑中的超时导致的复位
0x0C=SCG System Clock Generation Reset/SCG 中的时钟丢失或锁定丢失事件导致复位
0x0D=WDOG0 Watchdog 0 Reset/看门狗0超时复位
0x0E=SW Software Reset/ ARM内核SYSRESETREQ软件复位
0x0F=Lockup Reset/ARM内核指示LOCKUP事件导致复位
0x10=WDOG1 Watchdog 1 Reset/看门狗1超时复位
0x11=SECVIO Security Violation Reset/安全违规逻辑导致复位
 */
void PrintfResetReason(void)
{
	uint8_t KW45resetSource[10] = {0}, KW45resetSourceSave[10] = {0};
	uint32_t regsrs = CMC0->SRS;
	uint8_t resetSourec = 0x00;

	/* 读取flash数据10个字节 */
	uds_write_flash_data_un did_flash_data_un;
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,did_flash_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(KW45resetSource, did_flash_data_un.sig._4a0a_data, DID_4A0A_LENGTH);

	for(uint8_t i = 1; i < 10; i++)
	{
		KW45resetSourceSave[i-1] = KW45resetSource[i];
	}

	if(regsrs & (0x1<<0))
	{
		resetSourec = 0x01;
		logPrintfImmedi("ResetReason: %08X, Wake-up Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<1))
	{
		resetSourec = 0x02;
		logPrintfImmedi("ResetReason: %08X, Power-on Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<2))
	{
		resetSourec = 0x03;
		logPrintfImmedi("ResetReason: %08X, Low Voltage Detect Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<3))
	{
		resetSourec = 0x04;
		logPrintfImmedi("ResetReason: %08X, High Voltage Detect Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<4))
	{
		resetSourec = 0x05;
		logPrintfImmedi("ResetReason: %08X, Warm Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<5))
	{
		resetSourec = 0x06;
		logPrintfImmedi("ResetReason: %08X, Fatal Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<8))
	{
		resetSourec = 0x08;
		logPrintfImmedi("ResetReason: %08X, Pin Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<9))
	{
		resetSourec = 0x09;
		logPrintfImmedi("ResetReason: %08X, Debug Access Port Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<10))
	{
		resetSourec = 0x0A;
		logPrintfImmedi("ResetReason: %08X, Reset Timeout!\r\n", regsrs);
	}
	if(regsrs & (0x1<<11))
	{
		resetSourec = 0x0B;
		logPrintfImmedi("ResetReason: %08X, Low Power Acknowledge Timeout Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<12))
	{
		resetSourec = 0x0C;
		logPrintfImmedi("ResetReason: %08X, System Clock Generation Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<13))
	{
		resetSourec = 0x0D;
		logPrintfImmedi("ResetReason: %08X, Watchdog 0 Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<14))
	{
		resetSourec = 0x0E;
		logPrintfImmedi("ResetReason: %08X, Software Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<15))
	{
		resetSourec = 0x0F;
		logPrintfImmedi("ResetReason: %08X, Lockup Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<25))
	{
		resetSourec = 0x10;
		logPrintfImmedi("ResetReason: %08X, Watchdog 1 Reset!\r\n", regsrs);
	}
	if(regsrs & (0x1<<30))
	{
		resetSourec = 0x11;
		logPrintfImmedi("ResetReason: %08X, Security Violation Reset!\r\n", regsrs);
	}
	if(!resetSourec)
	{
		logPrintfImmedi("ResetReason: %08X, Unkonwn!\r\n", regsrs);
	}

	KW45resetSourceSave[9] = resetSourec;

#if ENABLE_WDT
	hal_wdt_if_feed(0);
#endif
	/* 存储flash数据10个字节 */
	UdsApi_set_KW45resetSource(KW45resetSourceSave);
}
uint32_t GetVbatV(void)
{
	return Vbat_V;
}

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

	RFSpiCommDTC_recordFunc();
	RFInitDTC_recordFunc();

	UWBFaultDTC_recordFunc();
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
	//logPrintf("currentVoltage is: %d\r\n", Vbat_V);
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
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
		}
	}
	else if(currentVoltage < PMS_VOLTAGE_18p5V)
	{
		if(BAT_V_Count > 1000)
		{
			BAT_V_Count = 0;
			PMVoltageState = VOLTAGE_BET_16_16p5;
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
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
			//dbg_Printf("Vbat_V is: %d\r\n", Vbat_V);
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
			//Dtc_Active(&DFC_KW45_WDOG_RESET);
		}

		if(ResetReason & CMC_SRS_LVD_MASK)
		{
			/*Dtc_Active(&DFC_KW45_LVD_RESET);*/
		}

		if(ResetReason & CMC_SRS_HVD_MASK)
		{
			dtcSts.b.b_HVDRstUpdate = 1;
			//Dtc_Active(&DFC_KW45_HVD_RESET);
		}
	}
	else
	{
		if(dtcSts.b.b_watchRstUpdate == 1)
		{
			dtcSts.b.b_watchRstUpdate = 0;
			//Dtc_Recove(&DFC_KW45_WDOG_RESET);
		}
		/*Dtc_Recove(&DFC_KW45_LVD_RESET);*/

		if(dtcSts.b.b_HVDRstUpdate == 1)
		{
			dtcSts.b.b_HVDRstUpdate = 0;
			//Dtc_Recove(&DFC_KW45_HVD_RESET);
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
