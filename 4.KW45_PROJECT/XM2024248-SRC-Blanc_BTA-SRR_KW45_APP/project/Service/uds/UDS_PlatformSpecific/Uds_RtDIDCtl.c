/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: Uds_RtDIDCtl.c
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
/* This file was automatically generated */
#include "uds_confpriv.h"
#include "Uds_RtDIDCtl.h"
#include "uds_priv.h"


#ifdef UDS_ENABLE_ROUTINECONTROL
    /* 0x31 RoutineControl service data Managing table
        Note : this table is located in the rom, can not be changed dynamically
               if a new did which is need to perform routine control  , then must update this table
               before build the whole of project again
               for this table, due to the number of attributes is greater than 10 , so split as two rows.
    */

UDS_CONFIG_TYPE UDS_RtnCtrlMngtDatTblRcd_t Uds_g_RtnCtrlMngtDataTable[] = {
/* routineIdentifier  AccessAttribute  ValidArgumentNum  RtnRunLimitTime   ExpectedPosResult  pFuncPtrStart                pFuncPtrStop                   pFuncPtrGetResult              pFuncPtrRtnCheck                pFuncPtrIsRtnFinished */
/*0u */{0xFF01        ,0x1             ,14              ,10000             ,1                ,UDS_C_RID_FP_START_RTN_FF01,  UDS_C_RID_FP_STOP_RTN_FF01,  UDS_C_RID_FP_RESULT_RTN_FF01,  UDS_C_RID_FP_CHECK_RTN_FF01,  UDS_C_RID_FP_FINISH_RTN_FF01},
/*1u */{0xFF00        ,0x1             ,12              ,10000             ,1                ,UDS_C_RID_FP_START_RTN_FF00,  UDS_C_RID_FP_STOP_RTN_FF00,  UDS_C_RID_FP_RESULT_RTN_FF00,  UDS_C_RID_FP_CHECK_RTN_FF00,  UDS_C_RID_FP_FINISH_RTN_FF00},
/*2u */{0x0203        ,0x101             ,4             ,10000             ,1                ,UDS_C_RID_FP_START_RTN_0203,  UDS_C_RID_FP_STOP_RTN_0203,  UDS_C_RID_FP_RESULT_RTN_0203,  UDS_C_RID_FP_CHECK_RTN_0203,  UDS_C_RID_FP_FINISH_RTN_0203},
/*3u */{0x0202        ,0x001             ,388            ,10000             ,1                ,UDS_C_RID_FP_START_RTN_0202,  UDS_C_RID_FP_STOP_RTN_0202,  UDS_C_RID_FP_RESULT_RTN_0202,  UDS_C_RID_FP_CHECK_RTN_0202,  UDS_C_RID_FP_FINISH_RTN_0202},
/* These last one Lines indicate the end of the data table , do not delete*/
/*FFu*/{0xFFFFu       , 0x0000u        , 0x00u         ,0x0000u         ,              0x00u,NULL                          ,NULL                         ,             NULL,      NULL,       NULL  }
}; /* end of table : UDS_CONFIG_TYPE UDS_RtnCtrlMngtDatTblRcd_t Uds_g_RtnCtrlMngtDataTable[]  */
#endif /* end of #ifdef UDS_ENABLE_ROUTINECONTROL */

/*
 * 函数介绍: 刷写检验
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_START_RTN_FF01(uint8_t *p_RtnCtrlOpt)
{
 (void)p_RtnCtrlOpt;
 return 0;
}
/*
 * 函数介绍: 刷写校验
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_C_RID_FP_STOP_RTN_FF01(void)
{
 return;
}
/*
 * 函数介绍: 刷写校验
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_RESULT_RTN_FF01(void)
{
 return 0;
}
/*
 * 函数介绍: 刷写校验
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_CHECK_RTN_FF01(uint8_t *p_RtnCtrlOpt)
{
 (void)p_RtnCtrlOpt;
 return 0;
}
/*
 * 函数介绍: 刷写校验
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_FINISH_RTN_FF01(void)
{
 return 0;
}

/*
 * 函数介绍: 内存擦写
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_START_RTN_FF00(uint8_t *p_RtnCtrlOpt)
{
 (void)p_RtnCtrlOpt;
 return 0;
}
/*
 * 函数介绍: 内存擦写
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_C_RID_FP_STOP_RTN_FF00(void)
{
 return ;
}
/*
 * 函数介绍: 内存擦写
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_RESULT_RTN_FF00(void)
{
 return 0;
}
/*
 * 函数介绍: 内存擦写
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_CHECK_RTN_FF00(uint8_t *p_RtnCtrlOpt)
{
 (void)p_RtnCtrlOpt;
 return 0;
}
/*
 * 函数介绍: 内存擦写
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_FINISH_RTN_FF00(void)
{
 return 0;
}

/*
 * 函数介绍: 编程条件检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_START_RTN_0203(uint8_t *p_RtnCtrlOpt)
{
	uint16 veh_temp;
	 uint8_t batt_temp;
	 uint8_t vehspd_st;
	 uint8_t timeout_flg;
	 uint8_t ign_temp;
	 uint8_t RspStatus = TRUE;
	 //factor：0.03125
	 veh_temp = UdsApi_getVehspdInfo();
	 batt_temp = UdsApi_getBattInfo();
	 vehspd_st = UdsApi_getVehSpdst();
	 timeout_flg = UdsApi_get11EtimeoutFlg();
#if 1
	 //ign_temp = getIgnitionStatus();
	 //vehspd< 3km/h ,ign等于ON或者OFF,电压在9V-16V
	 if(TRUE == timeout_flg)
	 {
		 if((batt_temp >=90) && (batt_temp <= 160))
		 {
			 p_RtnCtrlOpt[0] = 0x00u;
		 }
		 else
		 {
			 p_RtnCtrlOpt[0] = 0x01u;
			 RspStatus = FALSE;
		 }
	 }
	 else
	 {
		 if ((veh_temp <= 53)&&(0x1 == vehspd_st)&&((batt_temp >=90) && (batt_temp <= 160)))
		 {
			 p_RtnCtrlOpt[0] = 0x00u;
		 }
		 else
		 {
			 p_RtnCtrlOpt[0] = 0x01u;
			 RspStatus = FALSE;
		 }
	 }
#endif
	 return RspStatus;
}
/*
 * 函数介绍: 编程条件检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_C_RID_FP_STOP_RTN_0203(void)
{
 return ;
}
/*
 * 函数介绍: 编程条件检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_RESULT_RTN_0203(void)
{
	 uint8_t ret;
	 ret = UDS_C_RID_FP_START_RTN_0203(NULL);
	 return ret;
}
/*
 * 函数介绍: 编程条件检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_CHECK_RTN_0203(uint8_t *p_RtnCtrlOpt)
{
 (void)p_RtnCtrlOpt;
 return 0;
}
/*
 * 函数介绍: 编程条件检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_FINISH_RTN_0203(void)
{
 return TRUE;
}

/*
 * 函数介绍: 内存检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_START_RTN_0202(uint8_t *p_RtnCtrlOpt)
{
 (void)p_RtnCtrlOpt;
 return 0;
}
/*
 * 函数介绍: 内存检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_C_RID_FP_STOP_RTN_0202(void)
{
 return;
}
/*
 * 函数介绍: 内存检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_RESULT_RTN_0202(void)
{
 return 0;
}
/*
 * 函数介绍: 内存检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_CHECK_RTN_0202(uint8_t *p_RtnCtrlOpt)
{
 (void)p_RtnCtrlOpt;
 return 0;
}
/*
 * 函数介绍: 内存检查
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_C_RID_FP_FINISH_RTN_0202(void)
{
 return 0;
}

