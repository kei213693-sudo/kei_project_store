/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_pub.h
 * 摘要: Public definitions and declarations
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDS_PUB_H
#define _UDS_PUB_H

#include "uds_target.h"
#include "udstp_pub.h"

/* [MISRA 2004 Rules 1.2, 1.4 and 5.1] */
/*lint -esym(621, UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_FFIND,UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_RESPCON,UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_CHANGEPARAMCON)*/

/* [Encompasses MISRA 2004 Rule 8.3 - Symbol redeclared] */
/*lint -esym(18,Uds_GetRxBuf)*/

/* general types definition which will be used globally
 */
typedef enum{
  UDS_C_TANSFER_DAT_ST_INIT = 0x00u,
  UDS_C_TANSFER_DAT_ST_DNLD = 0x01u,
  UDS_C_TANSFER_DAT_ST_UPLD = 0x02u
}UDS_TransferDatState_t;

typedef enum{
  UDS_C_REQDNLD_UPLD_MEM_RANGE_INIT   = 0x00u,
  UDS_C_REQDNLD_UPLD_MEM_RANGE_RAM    = 0x01u,
  UDS_C_REQDNLD_UPLD_MEM_RANGE_FLASH  = 0x02u
}UDS_ReqDnLdUpLdMemRange_t;

/******************************************************************************
 *
 * General functions and types
 *
 *****************************************************************************/

extern void UDS_FN_TYPE Uds_Initialize( void );
extern void UDS_FN_TYPE Uds_Processor( void );
extern uint8_t UDS_FN_TYPE Uds_SesnMgr_IsRxBufAvailable( void);
// void* UDS_FN_TYPE Uds_GetRxBuf  ( void );
extern void UDS_FN_TYPE Uds_ServiceFFIndication ( uint8_t serverType, uint8_t sA, uint8_t tA, uint8_t tAType, uint8_t rA, uint16_t msgLen );
extern void UDS_FN_TYPE Uds_ServiceIndication	 ( uint8_t serverType, uint8_t sA, uint8_t tA, uint8_t tAType, uint8_t rA, uint8_t result, uint16_t msgLen, uint8_t* pPduData );
extern void UDS_FN_TYPE Uds_ServiceRespConfirm	 ( uint8_t serverType, uint8_t sA, uint8_t tA, uint8_t tAType, uint8_t rA, uint8_t result );
extern void UDS_FN_TYPE Uds_ChangeParamConfirm	 ( uint8_t serverType, uint8_t sA, uint8_t tA, uint8_t tAType, uint8_t rA, uint8_t param, uint8_t result );
extern void Uds_MainTaskEntryPoint_CyclicTask(void);

/******************************************************************************
 *
 * 2nd calss interfaces: exported interfaces, which is provided by UDS module for other modules.
 *
 *****************************************************************************/

/**
 * UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_IND :
 * legend:
 * UDS_APP: Unified Diagnostic Service Application
 * ITF    : Interface
 * EXP    : Exported (interface type)
 * CB     : Callback
 * FUNC   : Function (interface type)
 * NUSDAT : Network Layer Unacknowledged Segemented Data
 * IND    : Indication Primitive
 */
/*
 * Callouts provided to transport layer.
 */
#define UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_IND( mType, nSa, nTa, nTaType, nAe, nResult, msgLen, pMsgData )  \
		Uds_ServiceIndication( mType, nSa, nTa, nTaType, nAe, nResult, msgLen, pMsgData )

#define	UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_FFIND( mType, nSa, nTa, nTaType, nAe, msgLen )		\
		Uds_ServiceFFIndication( mType, nSa, nTa, nTaType, nAe, msgLen )

#define	UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_RESPCON( mType, nSa, nTa, nTaType, nAe, nResult )	\
		Uds_ServiceRespConfirm( mType, nSa, nTa, nTaType, nAe, nResult )

#define UDS_APP_ITF_EXP_CB_FUNC_NUSDAT_CHANGEPARAMCON( mType, nSa, nTa, nTaType, nAe, param, result )	\
		Uds_ChangeParamConfirm( mType, nSa, nTa, nTaType, nAe, param, result )

#define UDS_APP_ITF_EXP_CB_FUNC_ISRXBUFAVAILABLE()	Uds_SesnMgr_IsRxBufAvailable()

#ifdef UDS_ENABLE_ECURESET
   extern void UDS_FN_TYPE UdsApp_11EcuRst_CyclicTskEntryPoint( void );
#endif


#ifdef UDS_ENABLE_CLEARDIAGNOSTICINFORMATION
 extern uint8_t UDS_FN_TYPE UdsApp_14ClrDiagInfor_Pending(void);
 extern void UDS_FN_TYPE UdsApp_14ClrDiagInfor_Pend_Overtime(void) ;
 extern void UDS_FN_TYPE UdsApp_14ClrDiagInfor_Init(void);
#endif /* end of #ifdef UDS_ENABLE_CLEARDIAGNOSTICINFORMATION */

#ifdef UDS_ENABLE_READDATABYIDENTIFIER
   extern uint8_t UDS_FN_TYPE UdsApp_22RdDatByDid_ChkDidValidity(uint16_t i_luw_RefDid2Chk);
#endif

#ifdef UDS_ENABLE_SECURITYACCESS
   #define UDS_APP_ITF_EXP_GET_CURR_SECU_LEVEL(SAL_Level)  UdsApp_27SecuAcc_GetCurrSecurityLevelStatus(SAL_Level)

   /* UDS services exported functions interface delerations */
   extern void UDS_FN_TYPE UdsApp_27SecuAcc_Init(void);
   extern void UDS_FN_TYPE UdsApp_27SecuAcc_SwitchDiagSessionInit(void);
   extern void UDS_FN_TYPE UdsApp_27SecuAcc_CyclicTskEntryPoint(void);
   extern void UDS_FN_TYPE UdsApp_27SecuAcc_Pending(void);
   extern void UDS_FN_TYPE UdsApp_27SecuAcc_Pend_Overtime(void);
   extern uint8_t UDS_FN_TYPE UdsApp_27SecuAcc_GetCurrSecurityLevelStatus(uint8_t i_lub_SAL2Read);
   extern void UDS_FN_TYPE UdsApp_27SecuAcc_ReadSerialNumber(uint8_t *lptr_ReadOutSerialNumber, uint8_t *lptr_ByteNumOfSN);

#endif /* end of #ifdef UDS_ENABLE_SECURITYACCESS */


#ifdef UDS_ENABLE_COMMUNICATIONCONTROL
extern void UDS_FN_TYPE		Uds_28ComCtrl_Init(void);
#endif /* UDS_ENABLE_COMMUNICATIONCONTROL */


#ifdef UDS_ENABLE_READDATABYPERIODICIDENTIFIER
   extern void UDS_FN_TYPE UdsApp_2aRdDatByPeriodDid_Init(void);
   extern void UDS_FN_TYPE UdsApp_2aRdDatByPeriodDid_CyclicTskEntryPoint(void);

#endif /* end of #ifdef UDS_ENABLE_READDATABYPERIODICIDENTIFIER */

#ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER
   extern void UDS_FN_TYPE UdsApp_2eWtDatByDid_Init(void);
   extern uint8_t UDS_FN_TYPE UdsApp_2eWtDatByDid_Pending(void);
   extern void UDS_FN_TYPE UdsApp_2eWtDatByDid_Pend_Overtime(void);
#endif /* end of #ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER  */


#ifdef UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER
   extern void UDS_FN_TYPE UdsApp_2fIoCtrlByDid_ReturnAllCtrl2ECU (void);
#endif /* end of #ifdef UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER */

#ifdef UDS_ENABLE_ROUTINECONTROL
   extern void    UdsApp_31RtnCtrl_Init(void);

   extern void    UdsApp_31RtnCtrl_S3TmrTOHandler(void);
   extern uint8_t UdsApp_31RtnCtrl_pending(void);
   extern void    UdsApp_31RtnCtrl_pend_overtime(void);
   extern uint8_t Uds_31RtnCtrl_Get_IsRoutineAlreadyRunning(void);
#endif /* end of #ifdef UDS_ENABLE_ROUTINECONTROL */




#ifdef UDS_ENABLE_REQUESTDOWNLOAD
   extern uint8_t UdsApp_34ReqDnld_PreDnldCheckProc(uint32_t  i_InMemAdd2Write,
                                                  uint32_t  i_InMemSize2Dnld,
                                                  uint16_t *o_pMaxNumOfBlkLen);
   extern void UDS_FN_TYPE UdsApp_34ReqDnld_InitCurrReqDnldData(void);
   // extern uint8_t UDS_FN_TYPE UdsApp_34ReqDnld_GetCurrDnldTblIdx(void);
   extern uint32_t UDS_FN_TYPE UdsApp_34ReqDnld_GetCurrDnldStartAddr(void);
   extern uint32_t UDS_FN_TYPE UdsApp_34ReqDnld_GetCurrDnldMemSize(void);

#endif /* end of #ifdef UDS_ENABLE_REQUESTDOWNLOAD*/

#ifdef UDS_ENABLE_REQUESTUPLOAD
   extern uint8_t UdsApp_35ReqUpld_PreUpldCheckProc(uint32_t  i_InMemAdd2Rd,
                                                  uint32_t  i_InMemSize2Upld,
                                                  uint16_t *o_pMaxNumOfBlkLen);
   extern void UDS_FN_TYPE UdsApp_35ReqUpld_InitCurrReqUpldData(void);
   extern uint32_t UDS_FN_TYPE UdsApp_35ReqUpld_GetCurrUpldStartAddr(void);
   extern uint32_t UDS_FN_TYPE UdsApp_35ReqUpld_GetCurrUpldMemSize(void);
#endif /* end of #ifdef UDS_ENABLE_REQUESTUPLOAD*/

#ifdef UDS_ENABLE_TRANSFERDATA
   uint8_t UDS_FN_TYPE UdsApp_36TransferDat_GetCurrBlkSeqCntr(void);
   uint8_t UDS_FN_TYPE UdsApp_36TransferDat_Pending(void);

   void  UDS_FN_TYPE UdsApp_36TransferDat_InitBlkSeqCntrBefrTsfrDat(void);
   UDS_TransferDatState_t UDS_FN_TYPE UdsApp_36TransferDat_GetCurrTsfrDatSt(void);
   void UDS_FN_TYPE UdsApp_36TransferDat_SetCurrTsfrDatSt(UDS_TransferDatState_t i_TransferDatSt2Set);
   uint8_t UdsApp_36TransferDat_WriteDataItf_Stub(uint32_t * i_p32bitDataDestAddr,
                                                  uint32_t    i_8bitDataLen2Write,
                                                  uint32_t * i_p32bitDataSrcAddr);

   uint8_t UdsApp_36TransferDat_WriteDataIsFinished_Stub(void);

#endif /* end of #ifdef UDS_ENABLE_TRANSFERDATA */


#ifdef UDS_ENABLE_CONTROLDTCSETTING
   extern void UDS_FN_TYPE Uds_85CtrlDtcSet_Init(void);
#endif /* end of #ifdef UDS_ENABLE_CONTROLDTCSETTING */




#ifdef UDS_ENABLE_LINKCONTROL
   extern void UDS_FN_TYPE UdsApp_87LnkCtrl_Init(void);
#endif /* end of #ifdef UDS_ENABLE_LINKCONTROL */





#endif /* _UDS_PUB_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
