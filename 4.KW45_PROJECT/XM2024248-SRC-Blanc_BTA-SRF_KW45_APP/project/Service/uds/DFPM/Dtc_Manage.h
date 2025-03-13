/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: Dtc_Manage.h
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#ifndef _DTC_MANAGE_H
#define _DTC_MANAGE_H
#include "DSM_DtcBitCtl.h"
#include "dfpm_dfpInfo.h"
#include "typedefs.h"
#include "uds_api.h"
/*DTC扩展数据长度*/
#define DTC_EXT_DATA_LEN	(13u)
#define DTC_NUM				(DFPM_DFP_NUM-1u)
/*DTC相关数据写入FLASH长度*/
#define DTC_FLASH_LEN		((DTC_NUM*3u)+(DTC_EXT_DATA_LEN*DTC_NUM))



/**
 * DTC管理写入flash的信息
 *
 *
 * */
typedef union
{
	uint8_t dtc_mangage_buffer[DTC_FLASH_LEN];
	struct
	{
		uint8_t historyDtc[DTC_NUM];     				/*历史故障数组*/
		uint8_t historyDtc_cnt[DTC_NUM]; 				/*历史故障已发生驾驶循环计数*/
		uint8_t Occurrence_cnt[DTC_NUM]; 				/*发生次数*/
		uint8_t dtc_ext_data[DTC_EXT_DATA_LEN*DTC_NUM];	/*DTC扩展数据数组*/
	}sig;
}dtc_flash_write_un;

typedef struct
{
	uint8_t confirmCycle;	/*!< potential to confirmed cycle */
	uint8_t healingCycle;	/*!< confirmed to previously cycle */
	uint8_t reconfirmCycle; /*!< confirmed to confirmed cycle */
	uint8_t class;			/*!< fault class,  */
	uint32_t dtc;			/*!< DTC code */
	uint8_t snapshotGroup;	/*!< snapshot group number */
	uint16_t spn;			/*!< Suspect Parameter Number */
	uint8_t fmi;			/*!< Failure Mode Identifier */
	uint8_t enable;			/*!< dtc enable or disable */
} DFPM_FaultInfo;			/* 12byte */
typedef struct
{
  uint8_t Timestamp_year;
  uint8_t Timestamp_Month;
  uint8_t Timestamp_Day;
  uint8_t Timestamp_Hour;
  uint8_t Timestamp_Minute;
  uint8_t Timestamp_Second;
  uint8_t Odometer_value_h;
  uint8_t Odometer_value_m;
  uint8_t Odometer_value_l;
  uint8_t Battery_voltage;
  uint8_t vehspd_H;
  uint8_t vehspd_L;
  uint8_t vehspd_st;
  uint8_t can_sig_11E_timeout_flg;
}DFPM_ExtDataInfo;

extern const DFPM_FaultInfo FaultList[];
#define DFPM_getFaultInfo(dfp) &FaultList[dfp]; /*!< get fault information */

#define DFP_INVALUE 0x00u
#define DFP_INVALID 0XFFu		   	/*!< invalid DFP flag */
#define INVALID_FCM_LINE (0xFFFFu) 	/*!< invalid FCM flag */

#define DTC_HISTORY_CYCLE_CNT (40)	/*40 driving cycles without failure, clear the history of failure*/
#define DTC_STORE_OFF  (0x02)       /*DTC update function close*/
/*DTC状态定义*/
typedef union
{
	struct
	{
		uint8_t b1_test_failed : 1;					  /*1：当前故障激活，0：故障恢复*/
		uint8_t b1_test_failed_this_cycle : 1;		  /*本运行周期测试失效*/
		uint8_t b1_pend_dtc : 1;					  /*等待DTC*/
		uint8_t b1_confirm_dtc : 1;					  /*确认DTC,1:历史故障，0：历史故障恢复*/
		uint8_t b1_test_not_cmplt_since_last_clr : 1; /*上次清零后测试未完成*/
		uint8_t b1_test_failed_since_last_clr : 1;	  /*上次清零后测试失效*/
		uint8_t b1_test_not_cmplt_this_cycle : 1;	  /*本运行周期测试未完成*/
		uint8_t b1_warn_indica_req : 1;				  /*警告指示位请求*/
		uint8_t res1 : 8;							  /*预留*/

	} B;

} Dtc_Status;

typedef enum
{
	DFPM_E_ok = 0u,
	DFPM_E_initialized, /*!< duplicate initializations */
	DFPM_E_parameter,	/*!< parameter invalid */
	DFPM_E_verify,		/*!< verify  failed */
	DFPM_E_notReady,	/*!< not ready  */
	DFPM_E_queueFail,	/*!< not ready  */
	DFPM_E_outOfRange
} DFPM_Error;

typedef enum
{
	DTC_FaultSta_ok = 0,	/*!< no  fault  */
	DTC_FaultSta_previously = 0x08 ,/*!< previously fault  */
	DTC_FaultSta_confirmed = 0x09 /*!< confirmed and actived fault  */

} DTC_FaultSta;

typedef enum
{
	DFPM_Class_A = 0x02u,  /*!< class A  relevant OBD threshold limits(OTLs) are assumed to be exceeded*/
	DFPM_Class_B1 = 0x04u, /*!< class B1 have the potential to lead to emissions being above the OTLs*/
	DFPM_Class_B2 = 0x08u, /*!< class B2 assumed to influence emissions but not to a level that exceeds the OTL */
	DFPM_Class_C = 0x10u   /*!< class C  assumed to influence emissions but to a level that would not exceed the regulated emission limits*/
} DFPM_Class;
typedef enum
{
	DTC_HIST_CYCLE_CNT_INIT = 0,
	DTC_HIST_CYCLE_CNT_WRITE
}DTC_historycyclecnt_write_em;
typedef struct
{
	uint8_t dtc_sta;
	uint8_t dfp;
	uint8_t cycle_flag;
} DTC_History;

typedef union
{
	uint8_t ext_data[DTC_EXT_DATA_LEN];
	struct
	{
	 uint8_t Occurrence_counter;
	 DFPM_ExtDataInfo extdata_can;
	}sig;
}dtc_extern_data_un;

typedef struct
{
	uint8_t dtc_sta;
	uint8_t dfp;
	uint8_t snapshotbuffer[14];
	uint8_t SnapShotFlag;
	dtc_extern_data_un dtc_ext_data_u;
	uint8_t dtc_ext_data_flag;
} DTC_Confirm;
/*DTC fault manage*/
typedef struct
{
	DTC_History HistoryArr[DTC_NUM];  /*历史故障数组*/
	DTC_Confirm ConfirmArry[DTC_NUM]; /*当前故障数组*/

} Dtc_FaultManage;


extern void DTC_Init(void);


extern void DTC_clear(void);


extern void Dtc_update(void);


extern void Dtc_Active(const struct DSM_DFCTYPE_IMPL *DFC_id);


extern void Dtc_Recove(const struct DSM_DFCTYPE_IMPL *DFC_id);


extern uint8_t Dtc_getDtcNumtoStatusMsk(uint8_t statusMsk);


extern DTC_Confirm Dtc_getDtcConfirmDfp(uint8_t index);


extern uint8_t * Dtc_getDtcHistoryDfp(void);


extern DFPM_Error DFPM_getDtcByDfp(uint32_t *dtc, uint16_t dfp);


extern DFPM_Error DFPM_getSnapshot(uint8_t *group, uint16_t dfp);

extern DFPM_Error DFPM_setSnapshot(uint8_t *snapshotbuffer, uint16_t *Uds_g_Snapshot_group0, uint8_t num);

DFPM_Error DFPM_getDfpByDtc(uint32_t dtc, uint16_t *dfp);

DFPM_Error DFPM_getStaByDfp(uint32_t dfp, uint16_t *sta);

DFPM_Error DFPM_CheckDtcIsValid(uint32_t dtc);

void DFPM_setDtcExtData(DFPM_ExtDataInfo * dtc_extdata_info);

void DTC_1ms_Proc(void);
#endif
