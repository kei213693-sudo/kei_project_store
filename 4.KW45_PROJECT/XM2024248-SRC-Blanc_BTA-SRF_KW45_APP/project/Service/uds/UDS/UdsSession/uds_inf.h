/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_inf.h
 * 摘要: This file declares all the APIs (i.e. interfaces) which the UDS Application Layer
* requires from other components of the system.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDS_INF_H
#define _UDS_INF_H


#include "uds_target.h"
#include "uds_callbacks.h"
#include "udstp_pub.h"
#include "uds_api.h"

/*lint -e750*/

/* MISRA 2004 Rules 1.2, 1.4 and 5.1-Identifier clash */
/*lint -esym(621,UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_TX_TRAFFIC)*/
/*lint -esym(621,UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_RX_TRAFFIC)*/
/*lint -esym(621,UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_TX_TRAFFIC)*/
/*lint -esym(621,UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_RX_TRAFFIC)*/
/*lint -esym(621,UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_RX_TRAFFIC)*/
/*lint -esym(621,UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_TX_TRAFFIC)*/
/*lint -esym(621,UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_TX_TRAFFIC)*/
/*lint -esym(621,UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_RX_TRAFFIC)*/
/*lint -esym(621,UDS_APP_ITF_IMP_GET_E2_IDX_27SECUACC_RAND_GEN_SEED)*/
/*lint -esym(621,UDS_APP_ITF_IMP_GET_E2_IDX_27SECUACC_ST)*/

/******************************************************************************
 *
 * 1st class interfaces: imported interfaces , which is provided by other modules.
 *
 *****************************************************************************/

/**
 * UDS_APP_ITF_IMP_CB_FUNC_NUSDAT_IND :
 * legend:
 * UDS_APP: Unified Diagnostic Service Application
 * ITF    : Interface
 * IMP    : Imported (interface type)
 * CB     : Callback
 * FUNC   : Function (interface type)
 * NUSDAT : Network Layer Unacknowledged Segemented Data
 * IND    : Indication Primitive
 */
/*
 * Callbacks provided by transport layer.
 */
#define UDS_APP_ITF_IMP_SET_NUSDAT_REQ(mType, nSa, nTa, nTaType, nAe, msgLen, pMsgData)		\
		UdsTp_NUSData_Request(mType, nSa, nTa, nTaType, nAe, msgLen, pMsgData)

#define UDS_APP_ITF_IMP_CHANGE_PARAM_REQ(mType, nSa, nTa, nTaType, nAe, param, paramValue)	\
		UdsTp_NChangeParam_Request(mType, nSa, nTa, nTaType, nAe, param, paramValue)

/*
 * DiagnosticSessionControl required callbacks.
 */
#define UDS_APP_ITF_INT_CMD_STRT_NEW_SESN_ACTION           /* TODO: 0x11 EcuReset related new session action */
#define UDS_APP_ITF_INT_DTC_STRT_NEW_SESN_ACTION           UdsApp_85CtrlDtcSet_Init()/* TODO: 0x14 0x19 and dtc mngr related new session action */
#ifdef UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER
   #define UDS_APP_ITF_INT_DID_STRT_NEW_SESN_ACTION         UdsApp_2fIoCtrlByDid_ReturnAllCtrl2ECU()/* TODO: 0x22 0x2F ReadDatabyDID or IOCtrlByDID related new session action */
#else
   #define UDS_APP_ITF_INT_DID_STRT_NEW_SESN_ACTION         /* TODO: 0x22 0x2F ReadDatabyDID or IOCtrlByDID related new session action */
#endif
#define UDS_APP_ITF_INT_SECU_STRT_NEW_SESN_ACTION           UdsApp_27SecuAcc_SwitchDiagSessionInit() /* TODO: 0x27 Security Access related new session action */
#define UDS_APP_ITF_INT_COMCTRL_STRT_NEW_SESN_ACTION        UdsApp_28ComCtrl_Init() /* TODO: 0x28 CommunicationControl related new session action */
#define UDS_APP_ITF_INT_E2BLK_STRT_NEW_SESN_ACTION          /* TODO: 0x2E WriteDataByIdentifier related new session action */
#define UDS_APP_ITF_INT_RTN_STRT_NEW_SESN_ACTION            UdsApp_31RtnCtrl_Init() /* TODO: 0x31 Routine Control related new session action */
#ifdef UDS_ENABLE_LINKCONTROL
   #define UDS_APP_ITF_INT_LNKCTRL_STRT_NEW_SESN_ACTION     UdsApp_87LnkCtrl_Init() /* TODO: 0x87 LinkControl related new session action */
#else
   #define UDS_APP_ITF_INT_LNKCTRL_STRT_NEW_SESN_ACTION
#endif

#define UDS_APP_ITF_INT_CMD_HANDLE_S3_SVR_TMR_TO_EVT_ACTION        /* TODO: 0x11 EcuReset related s3_severTmr TO action */
#define UDS_APP_ITF_INT_DTC_HANDLE_S3_SVR_TMR_TO_EVT_ACTION        /* TODO: 0x14 0x19 and dtc mngr related s3_severTmr TO action  */
#ifdef UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER
   #define UDS_APP_ITF_INT_DID_HANDLE_S3_SVR_TMR_TO_EVT_ACTION      UdsApp_2fIoCtrlByDid_ReturnAllCtrl2ECU()/* TODO: 0x22 0x2F ReadDatabyDID or IOCtrlByDID related s3_severTmr TO action */
#else
   #define UDS_APP_ITF_INT_DID_HANDLE_S3_SVR_TMR_TO_EVT_ACTION
#endif
#define UDS_APP_ITF_INT_SECU_HANDLE_S3_SVR_TMR_TO_EVT_ACTION        UdsApp_27SecuAcc_SwitchDiagSessionInit() /* TODO: 0x27 security access related s3_severTmr TO action */
#define UDS_APP_ITF_INT_COMCTRL_HANDLE_S3_SVR_TMR_TO_EVT_ACTION     UdsApp_28ComCtrl_Init() /* TODO: 0x28 CommunicationControl related s3_severTmr TO action */
#define UDS_APP_ITF_INT_E2BLK_HANDLE_S3_SVR_TMR_TO_EVT_ACTION       /* TODO: 0x2E WriteDataByIdentifier related s3_severTmr TO action */
#define UDS_APP_ITF_INT_RTN_HANDLE_S3_SVR_TMR_TO_EVT_ACTION         UdsApp_31RtnCtrl_S3TmrTOHandler() /* TODO: 0x31 routine control related s3_severTmr TO action */

#ifdef UDS_ENABLE_LINKCONTROL
   #define UDS_APP_ITF_INT_LNKCTRL_HANDLE_S3_SVR_TMR_TO_EVT_ACTION  UdsApp_87LnkCtrl_Init() /* TODO: 0x87 LinkControl related s3_severTmr TO action */
#else
   #define UDS_APP_ITF_INT_LNKCTRL_HANDLE_S3_SVR_TMR_TO_EVT_ACTION
#endif

/* the below intefaces are used to check the pending states periodically
 */
#define UDS_APP_ITF_INT_DTC_PEND_PERIODIC_CHK           UdsApp_14ClrDiagInfor_Pending   /* TODO:  if the DTC managing itf need these operations  */
#define UDS_APP_ITF_INT_RTN_PEND_PERIODIC_CHK           UdsApp_31RtnCtrl_pending   /* UdsApp_31RtnCtrl_pending */  /* TODO:  if the Routine managing itf need these operations  */
#define UDS_APP_ITF_INT_SECU_PEND_PERIODIC_CHK          NULL   /* TODO:  if the Security Access managing itf need these operations  */
#define UDS_APP_ITF_INT_WTDID_PEND_PERIODIC_CHK         UdsApp_2eWtDatByDid_Pending   /* TODO:  if the WriteDataByDid managing itf need these operations  */
#define UDS_APP_ITF_INT_RDDID_PEND_PERIODIC_CHK         NULL   /* TODO:  if the ReadDataByDid managing itf need these operations  */
#define UDS_APP_ITF_INT_IOCTL_PEND_PERIODIC_CHK         NULL   /* TODO:  if the Iocontrol managing itf need these operations  */
#define UDS_APP_ITF_INT_CMD_PEND_PERIODIC_CHK           NULL   /* TODO:  if the diagnostic commands managing itf need these operations  */
#ifdef UDS_ENABLE_TRANSFERDATA
#define UDS_APP_ITF_INT_TSFRDAT_PEND_PERIODIC_CHK       UdsApp_36TransferDat_Pending   /* UdsApp_36TransferDat_Pending */   /* TODO:  if the diagnostic transfer data managing itf need these operations  */
#else
#define UDS_APP_ITF_INT_TSFRDAT_PEND_PERIODIC_CHK       NULL
#endif

#define UDS_APP_ITF_INT_DTC_PEND_OVERTIME_HANDLER()      /* TODO ; need the interface if pending overtime should be handled for DTC manager */
#define UDS_APP_ITF_INT_RTN_PEND_OVERTIME_HANDLER()      UdsApp_31RtnCtrl_pend_overtime()
#define UDS_APP_ITF_INT_SECU_PEND_OVERTIME_HANDLER()     /* TODO : need the interface if pending overtime should be handed 0x27 SecurityAccess service */
#define UDS_APP_ITF_INT_WTDID_PEND_OVERTIME_HANDLER()    /* TODO : need the interface if pending overtime should be handed 0x2e WriteDataByIdentifier service */
#define UDS_APP_ITF_INT_RDDID_PEND_OVERTIME_HANDLER()    /* TODO : need the interface if pending overtime should be handed 0x22 ReadDataByIdentifier service */
#define UDS_APP_ITF_INT_IOCTL_PEND_OVERTIME_HANDLER()    /* TODO : need the interface if pending overtime should be handed 0x2F InputOutputControl service */
#define UDS_APP_ITF_INT_CMD_PEND_OVERTIME_HANDLER()      /* TODO : need the interface if pending overtime should be handed 0x31 RoutineControl service */
#define UDS_APP_ITF_INT_TSFRDAT_PEND_OVERTIME_HANDLER()  /* TODO : need the interface if pending overtime should be handed 0x36 TransferData service */


#define UDS_APP_ITF_IMP_GET_ECU_OVR_VOLT_FLG()         ((uint8_t)FALSE) /* TODO : to get the overvoltage flag */
#define UDS_APP_ITF_IMP_GET_ECU_UDR_VOLT_FLG()         ((uint8_t)FALSE) /* TODO : to get the undervoltage flag */


#define UDS_APP_ITF_IMP_SET_PRVNT_ECU_2_SLEEP()    /* TODO: need the target to provide the power mode management interface */
#define UDS_APP_ITF_IMP_SET_PRVNT_CAN_2_SLEEP()    /* TODO: need the target to provide the CAN Network management interface */

/*
 * LinkControl required callbacks: need the itf from CAN driver.
 */
#define UDS_APP_ITF_IMP_SET_CHK_FIX_BDRT_IS_ALLOWD(FixedBaudRateId)    UdsApp_87LnkCtrl_ChkFixedBaudrateIsAllowed(FixedBaudRateId)
#define UDS_APP_ITF_IMP_SET_TRANSIT_FIXED_BDRT(FixedBaudRateId)        UdsApp_87LnkCtrl_Transit2FixedBaudrate(FixedBaudRateId)
#define UDS_APP_ITF_IMP_SET_CHK_SPEC_BDRT_IS_ALLOWD(SpecBaudRateHighByte,         		\
                                                    SpecBaudRateMiddleByte,       		\
                                                    SpecBaudRateLowByte)          		\
                UdsApp_87LnkCtrl_ChkSpecifiedBaudrateIsAllowed(SpecBaudRateHighByte,  	\
															   SpecBaudRateMiddleByte,	\
															   SpecBaudRateLowByte)
#define UDS_APP_ITF_IMP_SET_TRANSIT_SPEC_BDRT(SpecBaudRateHighByte,               		\
                                                    SpecBaudRateMiddleByte,       		\
                                                    SpecBaudRateLowByte)          		\
                UdsApp_87LnkCtrl_TransitSpecifiedBaudrate(SpecBaudRateHighByte,     	\
                                                          SpecBaudRateMiddleByte, 		\
                                                          SpecBaudRateLowByte)

/*
 * CommunicationControl required callbacks.
 */
#define UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_RX_TRAFFIC()    Uds_itf_enable_normal_rx()    /* TODO: need the itf from IL   */
#define UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NORM_TX_TRAFFIC()    Uds_itf_enable_normal_tx()    /* TODO: need the itf from IL   */
#define UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_RX_TRAFFIC()    Uds_itf_disable_normal_rx()   /* TODO: need the itf from IL   */
#define UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NORM_TX_TRAFFIC()    Uds_itf_disable_normal_tx()   /* TODO: need the itf from IL   */

#define UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_RX_TRAFFIC()      Uds_itf_enable_nm_rx()		/* TODO: need the itf from NM */
#define UDS_APP_ITF_IMP_SET_COM_CTRL_ENA_NM_TX_TRAFFIC()      Uds_itf_enable_nm_tx()		/* TODO: need the itf from NM */
#define UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_RX_TRAFFIC()      Uds_itf_disable_nm_rx()		/* TODO: need the itf from NM */
#define UDS_APP_ITF_IMP_SET_COM_CTRL_DIS_NM_TX_TRAFFIC()      Uds_itf_disable_nm_tx()		/* TODO: need the itf from NM */

/*
 * SecurityAccess required callbacks.
 */
#define UDS_APP_ITF_EXP_GET_SAL1_IS_UNLOCKED()    UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL1)
#define UDS_APP_ITF_EXP_GET_SAL2_IS_UNLOCKED()    UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL2)
#define UDS_APP_ITF_EXP_GET_SAL3_IS_UNLOCKED()    UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(UDS_C_SECU_ACC_SECU_LEVEL3)
#define UDS_APP_ITF_IMP_SYNC_E2_WRITE_ITF(E2_Block_Index, pDataSrc, DataSize)  /*  need the synchronous interface to save the data into EEPROM */
#define UDS_APP_ITF_IMP_SET_E2_WRITE_FLAG(E2_Block_Index)   NULL  /*  need the asynchronous interface to save the data into EEPROM */
#define UDS_APP_ITF_IMP_C_E2_27SECUACC_ST_BLK_IDX           0x00u  /* need the interface from EEPROM */
#define UDS_APP_ITF_IMP_GET_E2_IDX_27SECUACC_ST             0x00u  /* need the interface from EEPROM */
#define UDS_APP_ITF_IMP_C_E2_27SECUACC_RAND_GEN_SEED_IDX    0x00u  /* need the interface from EEPROM */
#define UDS_APP_ITF_IMP_GET_E2_IDX_27SECUACC_RAND_GEN_SEED  0x00u  /* defined in EmuEep_Extitf.h */
#define UDS_APP_ITF_IMP_GET_E2_CURR_IS_BUSY(E2_Block_Index) ((uint8_t)FALSE ) /* needed the interface to check the writing operation is finished */

#define UDS_APP_ITF_IMP_SET_E2_UPDATE_DAT(E2_Block_Index, BufPtr, DataSize)    /* ee_Copy_RamImg_into_ee_Buffer(BufPtr, DataSize) */


/*
 * EcuReset required callbacks.
 */
#define UDS_APP_ITF_IMP_SET_ECURST_HARD_RESET()       	    MCU_GENERIC_TRIG_MCU_SW_RESET /* TODO: if the different hard reset interface can be provided */
#define UDS_APP_ITF_IMP_SET_ECURST_KEYONOFF_RESET()   	    MCU_GENERIC_TRIG_MCU_SW_RESET /* TODO: if the different KeyOnOff Reset Interface can be provided  */
#define UDS_APP_ITF_IMP_SET_ECURST_SOFT_RESET()				MCU_GENERIC_TRIG_MCU_SW_RESET /*  */
#define UDS_APP_ITF_IMP_SET_ENA_RPD_PWRDN_RESET()     		/* TODO */
#define UDS_APP_ITF_IMP_SET_DIS_RPD_PWRDN_RESET()     		/* TODO */

/*
 * Callouts provided to CAN drivers if required.
 */
#define UDS_APP_ITF_EXP_REQ_CAN_FRAME_IND					/* reserved. */
#define UDS_APP_ITF_EXP_REQ_CAN_FRAME_CLR					/* reserved. */

/*
 *  0x22 ReadDataByIdentifier 0x2E WriteDataByIdentifier 0x2F IOControlByIdentifier  callbacks.
 */
#define UDS_ITF_IMP_RD_FNCPTR_F180         NULL   /* need the interface to read data  */
#define UDS_ITF_IMP_RD_FNCPTR_F181         NULL   /* need the interface to read data  */
#define UDS_ITF_IMP_RD_FNCPTR_F182         NULL   /* need the interface to read data  */
#define UDS_ITF_IMP_RD_FNCPTR_F183         NULL   /* need the interface to read data  */
#define UDS_ITF_IMP_RD_FNCPTR_F184         NULL   /* need the interface to read data  */

#define UDS_ITF_IMP_IOCTRL_3042_MSK_1           0x0001u
#define UDS_ITF_IMP_IOCTRL_3042_MSK_2           0x0002u
#define UDS_ITF_IMP_IOCTRL_3042_MSK_3           0x0004u
#define UDS_ITF_IMP_IOCTRL_3042_MSK_4           0x0008u
#define UDS_ITF_IMP_IOCTRL_3042_MSK_5           0x0010u
#define UDS_ITF_IMP_IOCTRL_3042_MSK_6           0x0020u
#define UDS_ITF_IMP_IOCTRL_3042_MSK_7           0x0040u
#define UDS_ITF_IMP_IOCTRL_3042_MSK_8           0x0080u

#define UDS_ITF_IMP_IOCTRL_3042_FP_1           NULL
#define UDS_ITF_IMP_IOCTRL_3042_FP_2           NULL
#define UDS_ITF_IMP_IOCTRL_3042_FP_3           NULL
#define UDS_ITF_IMP_IOCTRL_3042_FP_4           NULL
#define UDS_ITF_IMP_IOCTRL_3042_FP_5           NULL
#define UDS_ITF_IMP_IOCTRL_3042_FP_6           NULL
#define UDS_ITF_IMP_IOCTRL_3042_FP_7           NULL
#define UDS_ITF_IMP_IOCTRL_3042_FP_8           NULL



/*
 * 0x14 ClearDiagnosticInformation interfaces definitions.
 */
//#define UDS_ITF_IMP_CLR_ALL_DTCS(DtcCodeToClear)               Uds_DtcMgr_ClearDtc (DtcCodeToClear)  /*TODO , the inteface to clear all of DTCs*/
//#define UDS_ITF_IMP_CLR_SPEC_GRP_DTCS(DtcCodeToClear)          Uds_DtcMgr_ClearDtc (DtcCodeToClear)  /*TODO, the interface to clear a specific group of DTCs */
//#define UDS_ITF_IMP_CLR_SPEC_ONE_DTC(DtcCodeToClear)           Uds_DtcMgr_ClearDtc (DtcCodeToClear)  /*TODO, the interface to clear a specific on dtc  */

#define UDS_ITF_IMP_CLR_ALL_DTCS(DtcCodeToClear)               UDS_itf_clearAllDtcs(DtcCodeToClear)  		/*, the inteface to clear all of DTCs*/
#define UDS_ITF_IMP_CLR_SPEC_GRP_DTCS(DtcCodeToClear)          UDS_itf_clearGroupDtcs(DtcCodeToClear)  		/*, the interface to clear a specific group of DTCs */
#define UDS_ITF_IMP_CLR_SPEC_ONE_DTC(DtcCodeToClear)           UDS_itf_clearSpecificDtc(DtcCodeToClear)  	/*, the interface to clear a specific on dtc  */

#define UDS_ITF_IMP_GET_CLR_DTC_ACT_FINISHED()   (uint8_t)TRUE /*TODO , the interface to get when the clear action is finished. */


/* legend : FP: Function Pointer PRERD: pre-read ; CHK : Check
   the suffix is : 32 bit unsigned integer to present address,
                   format is always hexdecimal
 */

#define UDS_ITF_IMP_PRERD_CHK_FP_40000000  NULL   /* need the interface function to check valid pre-reading MemByAdd  */
#define UDS_ITF_IMP_RD_FP_40000000         NULL   /* need the interface function to actually read MemByAdd  */
#define UDS_ITF_IMP_PREWT_CHK_FP_40000000  NULL   /* need the interface function to check valid pre-writing MemByAdd  */
#define UDS_ITF_IMP_WT_FP_40000000         NULL   /* need the interface function to actually write MemByAdd */


#define UDS_ITF_IMP_PRE_DNLD_CHK_FP_00000000     UdsApp_34ReqDnld_PreDnldCheckProc   /* need the interface function to check valid pre-request downloading */
#define UDS_ITF_IMP_DNLD_WT_DAT_FP_00000000      UdsApp_36TransferDat_WriteDataItf_Stub   /* need the interface function to perform the request download data writing operation  */
#define UDS_ITF_IMP_PROG_IS_FINISH_FP_00000000   UdsApp_36TransferDat_WriteDataIsFinished_Stub   /* need the interface function to perform the request download data writing operation  */

#define UDS_ITF_IMP_PRE_DNLD_CHK_FP_40000000     UdsApp_34ReqDnld_PreDnldCheckProc   /* need the interface function to check valid pre-request downloading */
#define UDS_ITF_IMP_DNLD_WT_DAT_FP_40000000      UdsApp_36TransferDat_WriteDataItf_Stub    /* need the interface function to perform the request download data writing operation  */
#define UDS_ITF_IMP_PROG_IS_FINISH_FP_40000000   UdsApp_36TransferDat_WriteDataIsFinished_Stub    /* need the interface function to perform the request download data writing operation  */

#define UDS_ITF_IMP_DNLD_WT_DAT_FP_ANY_ADDR         UdsApp_36TransferDat_WriteDataItf_Stub /* need the interface function to perform the request download data writing operation  */
#define UDS_ITF_IMP_PROG_IS_FINISH_FP_ANY_ADDR      UdsApp_36TransferDat_WriteDataIsFinished_Stub/* need the interface function to perform the request download data writing operation  */

#define UDS_ITF_IMP_PRE_UPLD_CHK_FP_00000000     UdsApp_35ReqUpld_PreUpldCheckProc    /* need the interface function to check valid pre-request uploading */
#define UDS_ITF_IMP_UPLD_RD_DAT_FP_00000000      NULL    /* need the interface function to perform the request upload data reading operation  */

#define UDS_ITF_IMP_PRE_UPLD_CHK_FP_40000000     UdsApp_35ReqUpld_PreUpldCheckProc    /* need the interface function to check valid pre-request uploading */
#define UDS_ITF_IMP_UPLD_RD_DAT_FP_40000000      NULL    /* need the interface function to perform the request upload data reading operation  */

/******************************************************************************
 *
 * General functions and types
 *
 *****************************************************************************/

#endif /* _UDS_INF_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
