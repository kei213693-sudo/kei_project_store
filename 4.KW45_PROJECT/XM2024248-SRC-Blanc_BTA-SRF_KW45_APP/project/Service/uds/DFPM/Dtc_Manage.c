/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: Dtc_Manage.c
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "Dtc_Manage.h"
#include "SnapshotTable.h"

#include "string.h"
#include "uds_priv.h"
#include "dfc_type_init.h"
#include "app.h"


#define DTC_INIT_SUCCESS_DELAY_1S			(1000u)
static Dtc_Status *pDfcArry = NULL;
Dtc_FaultManage pDtc;

uint8_t historyDtc[DTC_NUM];     /*历史故障数组*/
uint8_t historyDtc_cnt[DTC_NUM]; /*历史故障已发生驾驶循环计数*/

uint8_t extern_dtc_data[DTC_NUM * DTC_EXT_DATA_LEN] = {0};
DFPM_ExtDataInfo l_dtcExtDataifno;
static dtc_flash_write_un dtc_flash_info_un;
/*DTC老化计数需要写标志*/
static uint8_t DTC_Historycyclecnt_Writeflag = DTC_HIST_CYCLE_CNT_INIT;
/*DTC初始化标志，TRUE为初始化完成*/
static uint8_t DTC_Init_Flag = false;
/*初始化操作成功后延时1s再进行记录DTC操作*/
static uint16_t DTC_opt_delay_ms = DTC_INIT_SUCCESS_DELAY_1S;
static void DTC_HistorCycleCnt_Proc(uint8_t dfp);

/*
 * 函数介绍: 故障管理初始化接口
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void DTC_Init(void)
{
    uint8_t i; // 用于循环遍历故障历史记录的索引变量。

    /* 将dfcArray指针赋值给pDfcArry，用于后续访问故障状态。 */
    pDfcArry = (Dtc_Status *)(dfcArray);

    /* 调用dfc_type_init函数进行故障类型初始化。 */
    dfc_type_init();

    /* 初始化所有故障历史记录的循环标志为TRUE。 */
    for (i = 0; i < DTC_NUM; i++)
    {
        pDtc.HistoryArr[i].cycle_flag = TRUE;
        pDtc.ConfirmArry[i].dtc_ext_data_flag = 0;
    }
    DTC_opt_delay_ms = DTC_INIT_SUCCESS_DELAY_1S;
    DTC_Init_Flag = true;
}
/*
 * 函数介绍: 1ms任务
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void DTC_1ms_Proc(void)
{
	 if(DTC_opt_delay_ms != 0)
	 {
		 DTC_opt_delay_ms--;
	 }
	 else
	 {}
 }
/*
 * 函数介绍: 设置DTC扩展数据接口
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void DFPM_setDtcExtData(DFPM_ExtDataInfo * dtc_extdata_info)
{
    if(dtc_extdata_info != NULL)
    {
        memcpy(&l_dtcExtDataifno,dtc_extdata_info,sizeof(l_dtcExtDataifno));
    }
	else
    {
        /* code */
    }

}
/*
 * 函数介绍: 设置DTC扩展数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
static void Dtc_set_extended_Data(uint16_t dfp)
{
    uint8_t i;
    uint8_t offset;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.Occurrence_counter = dtc_flash_info_un.sig.Occurrence_cnt[dfp - 1];
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Timestamp_year = l_dtcExtDataifno.Timestamp_year;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Timestamp_Month = l_dtcExtDataifno.Timestamp_Month;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Timestamp_Day = l_dtcExtDataifno.Timestamp_Day;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Timestamp_Hour = l_dtcExtDataifno.Timestamp_Hour;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Timestamp_Minute = l_dtcExtDataifno.Timestamp_Minute;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Timestamp_Second = l_dtcExtDataifno.Timestamp_Second;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Odometer_value_h = l_dtcExtDataifno.Odometer_value_h;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Odometer_value_m = l_dtcExtDataifno.Odometer_value_m;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Odometer_value_l = l_dtcExtDataifno.Odometer_value_l;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.Battery_voltage = l_dtcExtDataifno.Battery_voltage;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.vehspd_H = l_dtcExtDataifno.vehspd_H;
    pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.sig.extdata_can.vehspd_L = l_dtcExtDataifno.vehspd_L;

    /*读取flash之前DTC扩展数据保存到extern_dtc_data中*/
    Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
    /*计算该dfp应该存放的位置*/
    offset = (dfp - 1) * DTC_EXT_DATA_LEN;
    /*循环赋值*/
    for (i = 0; i < DTC_EXT_DATA_LEN; i++)
    {
    	dtc_flash_info_un.sig.dtc_ext_data[offset + i] = pDtc.ConfirmArry[dfp - 1].dtc_ext_data_u.ext_data[i];
    }
    /*将获取到的扩展数据写入到flash*/
    Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
}
/*
 * 函数介绍: DTC更新接口
 * 参数:
 * 返回值: {*}
 * 备注: 循环调用
 */
//#define TIMEOUT_DEBUG
void Dtc_update(void)
{
    uint16_t dfp;
    Dtc_Status status;
    uint8_t OFFSET;
    uint8_t result;
    uint16_t vehspd;
    vehspd = (l_dtcExtDataifno.vehspd_H << 8)+(l_dtcExtDataifno.vehspd_L);
    UdsApi_setVehspdInfo(vehspd);
    UdsApi_setBattInfo(l_dtcExtDataifno.Battery_voltage);
    UdsApi_setVehSpdst(l_dtcExtDataifno.vehspd_st);

#ifdef TIMEOUT_DEBUG
    l_dtcExtDataifno.can_sig_11E_timeout_flg = TRUE;
#endif
    UdsApi_set11EtimeoutFlg(l_dtcExtDataifno.can_sig_11E_timeout_flg);
    /*85 Service Shutdown Update the DTC function*/
    if (DTC_STORE_OFF == UdsApp_85CtrlDtcSet_Get_CurrDtcSettingFlg())
    {
        return;
    }
    /*初始化完成后延时1s再记录DTC*/
    if((DTC_opt_delay_ms != 0) && (TRUE == DTC_Init_Flag ))
    {
    	return;
    }
    else
    {
    	DTC_Init_Flag = FALSE;
    }
    /*read before writing*/
    result = Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
    for (dfp = 1; dfp < DFPM_DFP_NUM; dfp++)
    {
        status = pDfcArry[dfp];

        /*DTC被确认。只要故障被确认，此值会被置1，即Bit0==0 && Bit3==1代表历史故障*/
        if ((1 == status.B.b1_test_failed) && (1 == status.B.b1_confirm_dtc))
        {
            pDtc.ConfirmArry[dfp - 1].dfp = dfp;
            pDtc.ConfirmArry[dfp - 1].dtc_sta = DTC_FaultSta_confirmed;
            if (0 == pDtc.ConfirmArry[dfp - 1].dtc_ext_data_flag)
            {

                pDtc.ConfirmArry[dfp - 1].dtc_ext_data_flag = 1;
                if(dtc_flash_info_un.sig.Occurrence_cnt[dfp - 1] != 0xFF)
                {
                	dtc_flash_info_un.sig.Occurrence_cnt[dfp - 1]++;
                }
                else
                {
                	dtc_flash_info_un.sig.Occurrence_cnt[dfp - 1] = 1;
                }
                Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
                Dtc_set_extended_Data(dfp);
            }
            else
            {
            }
#if 0
            DFPM_setSnapshot(pDtc.ConfirmArry[dfp - 1].snapshotbuffer, pSnapshot, 5);

            /*Adrr offset*/
            OFFSET = ((dfp - 1) * 14);
            /*write snapshot to eeproom*/
            if (pDtc.ConfirmArry[dfp - 1].SnapShotFlag == 0)
            {
                //EnterCritical();
                Uds_Api_fdl_Ereas_Write(uds_api_fdl_flash_addr_a[1] + OFFSET, pDtc.ConfirmArry[dfp - 1].snapshotbuffer, 14);
                //ExitCritical();
                pDtc.ConfirmArry[dfp - 1].SnapShotFlag = 1;
            }
            else
            {}
#endif
        }
        else if ((0 == status.B.b1_test_failed) && (1 == status.B.b1_confirm_dtc))
        {

            if (R_FDL_OK == result)
            {
                pDtc.HistoryArr[dfp - 1].dfp = dfp;
                pDtc.HistoryArr[dfp - 1].dtc_sta = DTC_FaultSta_previously;

                /*进入历史故障后，当前故障状态要清掉*/
                pDtc.ConfirmArry[dfp - 1].dfp = DFP_INVALUE;
                pDtc.ConfirmArry[dfp - 1].dtc_sta = DTC_FaultSta_ok;
#if 0
                pDtc.ConfirmArry[dfp - 1].SnapShotFlag = 0;
#endif
                pDtc.ConfirmArry[dfp - 1].dtc_ext_data_flag = 0;
                /*判断读出的历史故障值是否等于当前需要更新的故障dfp,如果相等则不更新flash中存储的值*/
                if(dtc_flash_info_un.sig.historyDtc[dfp - 1] != dfp)
                {
                	dtc_flash_info_un.sig.historyDtc[dfp - 1] = dfp;
					/*write history fault to dataflash*/
					Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
                }
                else
                {}

            }
            else
            {
            }
        }
        else
        {
            /*该dfp无故障*/
            pDtc.ConfirmArry[dfp - 1].dfp = DFP_INVALUE;
            pDtc.ConfirmArry[dfp - 1].dtc_sta = DTC_FaultSta_ok;
            pDtc.HistoryArr[dfp - 1].dfp = DFP_INVALUE;
            pDtc.HistoryArr[dfp - 1].dtc_sta = DTC_FaultSta_ok;
        }

        /*If no fault occurs again in 40 cycles after the occurrence
        of the historical fault, the historical fault will be cleared*/
        DTC_HistorCycleCnt_Proc(dfp);
    }
    /*所有DTC遍历完成后检查是否需要更新老化计数器计数*/
    if(DTC_HIST_CYCLE_CNT_WRITE == DTC_Historycyclecnt_Writeflag)
    {
    	/*更新DTC老化计数器*/
    	Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
    	DTC_Historycyclecnt_Writeflag = DTC_HIST_CYCLE_CNT_INIT;
    }
    else
    {}
}
/*
 * 函数名: DFPM_getIgnstate
 * 函数介绍: 获取点火开关状态
 * 返回值: {*}
 * 备注:
 */
uint8_t DFPM_getIgnstate()
{
    return 0;
}
/*
 * 函数名: DTC_HistorCycleCnt_Proc
 * 函数介绍: If no fault occurs again in 40 cycles after the occurrence of the historical fault, the historical fault will be cleared
 * 返回值: {*}
 * 备注:
 */
static void DTC_HistorCycleCnt_Proc(uint8_t dfp)
{
    uint8_t IgnSta;
    uint8_t IgnStaValid;
    IgnSta = UdsApi_getIgnstate();
    IgnStaValid = UdsApi_getIgnstateValid();
    /*判断该DFP是否进历史故障*/
    if (DFP_INVALUE != dtc_flash_info_un.sig.historyDtc[dfp - 1])
    {
        /*判断历史故障发生后是否跑了40个循环*/
        if (dtc_flash_info_un.sig.historyDtc_cnt[dfp-1] < DTC_HISTORY_CYCLE_CNT)
        {
            /*判断发生历史故障后是否发生当前故障*/
            if ((DFP_INVALUE == (pDtc.ConfirmArry[dfp - 1].dfp)))
            {
                /*一个驾驶循环计数器增加1*/
                if ((TRUE == IgnStaValid))
                {
                    if ((UDSAPI_IGN_ON == IgnSta) && (TRUE == pDtc.HistoryArr[dfp - 1].cycle_flag))
                    {
                    	dtc_flash_info_un.sig.historyDtc_cnt[dfp - 1] += 1;
                        pDtc.HistoryArr[dfp - 1].cycle_flag = FALSE;
                        /*write history fault to dataflash*/
                        DTC_Historycyclecnt_Writeflag = DTC_HIST_CYCLE_CNT_WRITE;
#if 0
                        Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
#endif
                    }
                    else{}
                    /*如果ign每超时且信号为IGN_OFF，则清掉计数标志*/
                    if (UDSAPI_IGN_OFF == IgnSta)
                    {
                        pDtc.HistoryArr[dfp - 1].cycle_flag = TRUE;
                    }
                    else{}
                }
                else
                {
                    // pDtc.HistoryArr[dfp - 1].cycle_flag = TRUE;
                }
            } /*如果发生当前故障，则清掉循环计数器*/
            else
            {
            	/*如果计数不等于0，则清零*/
            	if(dtc_flash_info_un.sig.historyDtc_cnt[dfp - 1] != 0)
            	{
            		dtc_flash_info_un.sig.historyDtc_cnt[dfp - 1] = 0;
					pDtc.HistoryArr[dfp - 1].cycle_flag = TRUE;
					DTC_Historycyclecnt_Writeflag = DTC_HIST_CYCLE_CNT_WRITE;
#if 0
					Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
#endif
            	}
            	else
            	{}
            }
        } /*如果历史故障发生后经过40个驾驶循环未发生当前故障则清掉历史故障，并把历史故障其他状态清掉*/
        else
        {
            pDfcArry[dfp].B.b1_test_failed = 0;
            pDfcArry[dfp].B.b1_confirm_dtc = 0;
            pDtc.HistoryArr[dfp - 1].dfp = DFP_INVALUE;
            pDtc.HistoryArr[dfp - 1].dtc_sta = DTC_FaultSta_ok;
            pDtc.HistoryArr[dfp - 1].cycle_flag = TRUE;
            if(dtc_flash_info_un.sig.historyDtc_cnt[dfp - 1] != 0)
            {
            	dtc_flash_info_un.sig.historyDtc_cnt[dfp - 1] = 0;
            	dtc_flash_info_un.sig.historyDtc[dfp - 1] = DFP_INVALUE;
            	DTC_Historycyclecnt_Writeflag = DTC_HIST_CYCLE_CNT_WRITE;
#if 0
				/*write history fault to dataflash*/
				Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
#endif
            }
            else
            {}
        }
    }
    else
    {}
}
/*
 * 函数介绍:
  * 该函数用于清除车辆的诊断故障码和历史故障码，确保车辆故障信息的刷新，
 * 以便于维修人员准确判断车辆的当前状态。
 *
 * 清除过程包括：
 * 1. 遍历故障确认数组和历史故障数组，将故障码和状态信息重置为无效值。
 * 2. 清除故障历史计数器。
 * 3. 使用UDS协议的擦除写入功能，将故障信息存储区域的数据清零。
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void DTC_clear(void)
{
    uint8_t i;
    /* 初始化一个长度为256的数组，用于后续的擦除写入操作。 */
    uint8_t clr_snapbuf[256] = {0};

    /* 遍历故障确认数组，清除故障码和状态信息。 */
    for (i = 0; i < DTC_NUM; i++)
    {
        /* 如果当前故障码有效，则将其设置为无效，并更新状态为正常。 */
        if (pDtc.ConfirmArry[i].dfp != DFP_INVALUE)
        {
            pDtc.ConfirmArry[i].dfp = DFP_INVALUE;
            pDtc.ConfirmArry[i].dtc_sta = DTC_FaultSta_ok;
        }
        else
        {}
        /* 对于历史故障数组，同样进行清除操作。 */
        if (pDtc.HistoryArr[i].dfp != DFP_INVALUE)
        {
            pDtc.HistoryArr[i].dfp = DFP_INVALUE;
            pDtc.HistoryArr[i].dtc_sta = DTC_FaultSta_ok;
            pDtc.HistoryArr[i].cycle_flag = TRUE;
        }
        else
        {}
        /* 清除历史故障计数器和故障码。 */
        dtc_flash_info_un.sig.historyDtc[i] = 0;
        dtc_flash_info_un.sig.historyDtc_cnt[i] = 0;
        pDfcArry[i + 1].B.b1_confirm_dtc = 0;
        pDfcArry[i + 1].B.b1_test_failed = 0;
    }

    /* 使用UDS协议的擦除写入功能，清空诊断故障码存储区域。 */
    Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
#ifdef UdsApi_BTA_SRF_FUN_EN
    extern void ClrKW45_RFCSpiCommDtcState(void);
    ClrKW45_RFCSpiCommDtcState();
#endif
#if (1 == UdsApi_BTA_SRF_FUN_EN) || (1 == UdsApi_BTA_SRR_FUN_EN)
    ClrKW45_UWBFaultDtcState();
#endif
}

/*
 * 函数介绍: 此函数用于处理特定条件下的DTC激活逻辑。它首先通过DFC_id获取DFC类型，
 * 然后根据DFC数组中相应条目的状态来决定是否将DTC设置为激活状态。
 * 参数:
 * @param DFC_id 指向DSM_DFCTYPE_IMPL类型的指针，用于识别故障代码。
 * 返回值: {*}
 * 备注:
 */
void Dtc_Active(const struct DSM_DFCTYPE_IMPL *DFC_id)
{
    if (!(UdsApp_85CtrlDtcSet_Get_CurrDtcSettingFlg() == DTC_STORE_OFF))
    {
        uint16_t dfp;
        if (DFC_id != NULL)
        {
            /* 获取DFC类型 */
            dfp = DSM_DFCTYPE_IMPL_getDFCType(DFC_id);

            /* 检查是否满足条件以激活DTC：测试未失败且确认状态为未确认或已确认 */
            if ((0 == pDfcArry[dfp].B.b1_test_failed) && ((0 == pDfcArry[dfp].B.b1_confirm_dtc) || (1 == pDfcArry[dfp].B.b1_confirm_dtc)))
            {
                /* 设置DTC为测试失败并确认状态 */
                pDfcArry[dfp].B.b1_test_failed = 1;
                pDfcArry[dfp].B.b1_confirm_dtc = 1;
            }
            else
            {
                /* 如果不满足条件，则不进行任何操作 */
            }
        }
        else
        {
            /* code */
        }
    }
    else
    {
    }
}

/*
 * 函数介绍: Dtc_Recove函数用于处理DFC类型的数据恢复。
 * 此函数首先通过DFC_id获取特定DFC类型的标识符。然后检查该DFC类型是否满足特定条件：
 * 即b1_test_failed和b1_confirm_dtc均为1。如果满足条件，说明该DFC类型需要进行数据恢复，
 * 则将b1_test_failed字段重置为0，表示数据恢复已完成或不再需要。
 * 参数:
 * param DFC_id 指向DFC类型定义的指针，用于获取DFC类型的具体信息。
 * 返回值: {*}
 * 备注:
 */
void Dtc_Recove(const struct DSM_DFCTYPE_IMPL *DFC_id)
{

    if (!(UdsApp_85CtrlDtcSet_Get_CurrDtcSettingFlg() == DTC_STORE_OFF))
    {
        uint16_t dfp;
        if (DFC_id != NULL)
        {
            /* 获取DFC_id对应的DFC类型 */
            dfp = DSM_DFCTYPE_IMPL_getDFCType(DFC_id);

            /* 检查是否满足数据恢复条件：b1_test_failed和b1_confirm_dtc均为1 */
            if ((1 == pDfcArry[dfp].B.b1_test_failed) && (1 == pDfcArry[dfp].B.b1_confirm_dtc))
            {
                /* 满足条件时，重置b1_test_failed为0，表示数据恢复已完成或不再需要 */
                pDfcArry[dfp].B.b1_test_failed = 0;
            }
            else
            {
                /* 不满足条件时，不做任何处理 */
            }
        }
        else
        {
            /* code */
        }
    }
    else
    {
    }
}

/*
 * 函数介绍:
    本函数通过遍历故障确认数组或读取Flash中的故障历史记录，来计算故障数量。
 * 根据传入的状态掩码的不同，函数将执行不同的逻辑：
 * - 当状态掩码为0x01时，计算当前已确认的故障数量。
 * - 当状态掩码为0x08时，从Flash中读取故障历史记录，并计算历史故障数量。
 * 其他状态掩码值将不执行任何操作，并返回0。
 * 参数:
 * @param statusMsk 状态掩码，用于确定计算哪种类型的故障数量。
 * 返回值: 故障数量，根据状态掩码的不同，返回当前确认的故障数量或历史故障数量。
 * 备注:
 */

uint8_t Dtc_getDtcNumtoStatusMsk(uint8 statusMsk)
{
    uint8_t i;
    uint8_t dtcnum = 0; // 故障数量计数器
    uint8_t res; // 用于存储从Flash读取操作返回结果的变量

    switch (statusMsk)
    {
        // 当前故障数量
        case 0x01:
            // 遍历确认故障数组，计算已确认故障的数量
            for (i = 0; i < DTC_NUM; i++)
            {
                if (pDtc.ConfirmArry[i].dtc_sta == DTC_FaultSta_confirmed)
                {
                    dtcnum++;
                }
            }
            break;

        /*0x08：历史故障数量*/
        case 0x08:
            // 从Flash中读取故障历史记录
            res = Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);

            // 如果读取操作成功
            if (res == R_FDL_OK)
            {
                // 遍历故障历史记录数组，计算非无效值的故障数量
                for (i = 0; i < DTC_NUM; i++)
                {
                    if (dtc_flash_info_un.sig.historyDtc[i] != DFP_INVALUE)
                    {
                        dtcnum++;
                    }
                }
            }
            break;

        default:
            break;
    }
    return dtcnum;
}

/*
 * 函数介绍: 根据dfp查询DTC
 * 参数: index：dfp对应的index
 * 返回值: {*}
 * 备注:
 */
DTC_Confirm Dtc_getDtcConfirmDfp(uint8_t index)
{
    return pDtc.ConfirmArry[index];
}

/*
 * 函数介绍: 获取DTC（Diagnostic Trouble Code）历史数据的函数
 * 参数:
 * 返回值: 返回一个指向uint8_t类型的数组，该数组包含DTC历史数据，
 *         如果读取失败，则返回一个初始化为空的相同大小的数组。
 * 备注:该函数通过调用Uds_Api_fdl_FlashRead函数来读取DTC历史数据，
 * 并根据读取操作的结果来决定是否返回读取的数据或初始化一个空的数据数组
 */
uint8_t *Dtc_getDtcHistoryDfp(void)
{
    uint8_t res;

    // 尝试从flash中读取DTC历史数据
    res = Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);

    // 判断读取操作是否成功
    if (res == R_FDL_OK)
    {
        // 读取成功，返回历史DTC数据数组的地址
        return (uint8_t *)(dtc_flash_info_un.sig.historyDtc);
    }
    else
    {
//        // 读取失败，将历史DTC数据数组初始化为空
//        memcpy((uint8_t *)histDtcArr, (const uint8_t *)0, DFPM_DFP_NUM);
        // 返回初始化后的历史DTC数据数组的地址
        return (uint8_t *)(dtc_flash_info_un.sig.historyDtc);
    }
}
/*
 * 函数介绍: 根据dfp获取dtc
 * 参数:
 * 返回值: {*}
 * 备注:
 */
DFPM_Error DFPM_getDtcByDfp(uint32_t *dtc, uint16_t dfp)
{
    DFPM_Error err = DFPM_E_ok;
    const DFPM_FaultInfo *faultInfo;

    faultInfo = DFPM_getFaultInfo(dfp);
    if ((faultInfo != NULL) &&  (dtc != NULL))
    {
        *dtc = faultInfo->dtc;
    }
    else
    {
        err = DFPM_E_parameter;
    }

    return err;
}

/*
 * 函数介绍: 获取冻结帧信息
 * 参数:
 * 返回值: {*}
 * 备注:
 */
DFPM_Error DFPM_getSnapshot(uint8_t *group, uint16_t dfp)
{
    uint8_t OFFSET = 0;
    DFPM_Error err = DFPM_E_ok;
    uint8_t res = R_FDL_OK;
    OFFSET = (dfp - 1) * 14;
    if (group != NULL)
    {
#if 0
        res = Uds_Api_fdl_FlashRead(uds_api_fdl_flash_addr_a[1] + OFFSET, group, 14);
#endif
        if (res == R_FDL_OK)
        {
            err = DFPM_E_ok;
        }
        else
        {
            err = DFPM_E_parameter;
        }
    }
    else
    {
        err = DFPM_E_parameter;
    }

    return err;
}
/*
 * 函数介绍: 设置冻结帧信息
 * 参数:
 * 返回值: {*}
 * 备注:
 */
DFPM_Error DFPM_setSnapshot(uint8_t *snapshotbuffer, uint16_t *Uds_g_Snapshot_group0, uint8_t num)
{

    DFPM_Error err = DFPM_E_ok;
    uint8_t i;
    uint32_t odometer = 0;
    uint16_t veh = 0;
    uint8_t ptime[7] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    if ((NULL == Uds_g_Snapshot_group0) || (NULL == snapshotbuffer))
    {
        err = DFPM_E_parameter;
    }
    else
    {
        for (i = 0; i < num; i++)
        {
            switch (Uds_g_Snapshot_group0[i])
            {
#if 0
            /*Power Supply Voltage*/
            case 0x0112u:
                snapshotbuffer[0] = 0x0Bu;
                break;
            case 0xE101u:
                //odometer = CANFDRX_getOdometer();
                snapshotbuffer[1] = 0x0u;
                snapshotbuffer[2] = 0x27u;
                snapshotbuffer[3] = 0x10u;

                break;
            case 0xB100u:
                //veh = CANFDRX_getVehSpd();
                snapshotbuffer[4] = 0x0u;
                snapshotbuffer[5] = 0x50u;
                break;
            case 0xD001u:
                snapshotbuffer[6] = 0x02;
                break;
            case 0xF020u:
                //CANFDRX_getTime(ptime);
                memcpy(&snapshotbuffer[7], ptime, 7);
                break;
            default:
                break;
#else
                /*Power Supply Voltage*/
            case 0x0112u:
                // snapshotbuffer[0] = (uint8_t)pm_get_batt_volt();
                break;
            case 0xE101u:
                // odometer = CANFDRX_getOdometer();
                snapshotbuffer[1] = (uint8_t)(odometer >> 16);
                snapshotbuffer[2] = (uint8_t)(odometer >> 8);
                snapshotbuffer[3] = (uint8_t)(odometer);

                break;
            case 0xB100u:
                // veh = CANFDRX_getVehSpd();
                snapshotbuffer[4] = (uint8_t)(veh >> 8);
                snapshotbuffer[5] = (uint8_t)(veh);
                break;
            case 0xD001u:
                // snapshotbuffer[6] = getIgnitionStatus();
                break;
            case 0xF020u:
                // CANFDRX_getTime(ptime);
                memcpy(&snapshotbuffer[7], ptime, 7);
                break;
            default:
                break;
#endif
            }
        }
    }
    return err;
}
/*
 * 函数介绍: 根据DTC获取dfp
 * 参数:
 * 返回值: {*}
 * 备注:
 */
DFPM_Error DFPM_getDfpByDtc(uint32_t dtc, uint16_t *dfp)
{
    DFPM_Error err = DFPM_E_ok;
    uint16_t i;

    *dfp = DFP_INVALID;

    if (dfp != NULL)
    {
        for (i = 0; i < DFPM_DFP_NUM; i++)
        {
            if (FaultList[i].dtc == dtc)
            {
                *dfp = i;
                break;
            }
            else
            {
            }
        }
    }
    else
    {
        err = DFPM_E_parameter;
    }

    if (*dfp == DFP_INVALID)
    {
        err = DFPM_E_parameter;
    }
    else
    {
    }

    return err;
}
/*
 * 函数介绍: 检查DTC是否有效
 * 参数:
 * 返回值: {*}
 * 备注:
 */
DFPM_Error DFPM_CheckDtcIsValid(uint32_t dtc)
{
    DFPM_Error err = DFPM_E_parameter;
    uint16_t i;

    if (dtc != 0x000000)
    {
        for (i = 0; i < DFPM_DFP_NUM; i++)
        {
            if (FaultList[i].dtc == dtc)
            {
#ifdef UdsApi_BTA_SRF_FUN_EN
                err = DFPM_E_ok;
#endif
#if((1 == UdsApi_BTA_R_FUN_EN) ||(1 == UdsApi_BTA_RB_FUN_EN) || (1 == UdsApi_EXTERNAL_BTA_FUN_EN))
                if((5 == i) || (6 == i) || (7 == i))
                {
                	err = DFPM_E_parameter;
                }
                else
                {
                	err = DFPM_E_ok;
                }
#endif
#ifdef UdsApi_BTA_SRR_FUN_EN
                if((5 == i) || (6 == i))
			   {
				err = DFPM_E_parameter;
			   }
			   else
			   {
				err = DFPM_E_ok;
			   }
#endif
#ifdef UdsApi_BTA_L_FUN_EN
                if(7 == i)
			   {
				err = DFPM_E_parameter;
			   }
			   else
			   {
				err = DFPM_E_ok;
			   }
#endif
            }
            else
            {
            }
        }
    }
    else
    {
        err = DFPM_E_parameter;
    }
    return err;
}
/*
 * 函数介绍: 根据dfp获取故障状态
 * 参数:
 * 返回值: {*}
 * 备注:
 */
DFPM_Error DFPM_getStaByDfp(uint32_t dfp, uint16_t *sta)
{
    DFPM_Error err = DFPM_E_ok;
    uint8_t res;
    if (sta != NULL)
    {
        res = Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DTC, dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);
        if (res == R_FDL_OK)
        {
            if (pDtc.ConfirmArry[dfp - 1].dfp != DFP_INVALUE)
            {
                *sta = DTC_FaultSta_confirmed;
            }
            else if (dtc_flash_info_un.sig.historyDtc[dfp - 1] != DFP_INVALUE)
            {
                *sta = DTC_FaultSta_previously;
            }
            else
            {
                *sta = DTC_FaultSta_ok;
            }
        }
    }
    else
    {
        err = DFPM_E_parameter;
    }
    return err;
}

