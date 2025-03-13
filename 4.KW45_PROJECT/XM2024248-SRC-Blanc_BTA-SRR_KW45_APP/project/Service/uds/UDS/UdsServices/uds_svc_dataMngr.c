/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_svc_dataMngr.c
 * 摘要: UDS Diagnostic Data Manager for Reading Writing and IOControl operation.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef UDS_SVC_DATA_MNGR_C
#define UDS_SVC_DATA_MNGR_C

#include "uds_target.h"
#include "uds_confpriv.h"
#include "uds_confdefs.h"
#include "uds_priv.h"
#include "uds_inf.h"
#include "uds_svc_dataMngr.h"
#include "uds_pub.h"

extern uint8_t rub_ReqDnUpldDataBuf[UDS_C_REQ_DN_OR_UP_LD_BUF_SIZE];

#include "DidList.h"
#include "Uds_IODIDCtl.h"
#include "Uds_RtDIDCtl.h"


#if 0
static uint8_t rub_DidDataOperBuf[UDS_C_DID_DATA_OPERATE_BUF_SIZE] = {0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u,
                                                                    0x09u, 0x0Au, 0x0Bu, 0x0Cu, 0x0Du, 0x0Eu, 0x0Fu, 0x10u,
                                                                    0x11u, 0x12u, 0x13u, 0x14u, 0x15u, 0x16u, 0x17u, 0x18u,
                                                                    0x19u, 0x1Au, 0x1Bu, 0x1Cu, 0x1Du, 0x1Eu, 0x1Fu, 0x20u
                                                                   };


#ifdef UDS_ENABLE_READDATABYIDENTIFIER

/* 0x22 ReadDataByIdentifier service data Managing table
        Note : this table is located in the rom, can not be changed dynamically
               if a new did which is need to read, then must update this table
               before build the whole of project again;
               The periodicalDid must also be configured into this table first,
               then to configure the below table  Uds_g_RdDatByPeriodDidMngTbl
               to decide using which kind of period rate to read.
               The DynamicDefined Did is not put into this table , but
               the Dynamically defined Did must reference to the one or more of
               DID in this read-only table, i.e. dynamicaDefined Did can only reference
               to the pre-configured common DID.

 */
UDS_CONFIG_TYPE UDS_DidDatMngRecd_t Uds_g_DidDataTable[] =
{
/*      dataIdentifier                DataSize            AccessAttribute      pDataBuf                  pFuncPtrToReadData */
/* 0u*/{UDS_C_DID_BOOT_SW_ID_F180,    UDS_C_DID_LEN_F180, UDS_C_DID_ATTR_F180, UDS_ITF_IMP_DAT_PTR_F180, UDS_ITF_IMP_RD_FNCPTR_F180 },  /* ECU Boot Software Identification */
/* 1u*/{UDS_C_DID_APP_SW_ID_F181,     UDS_C_DID_LEN_F181, UDS_C_DID_ATTR_F181, UDS_ITF_IMP_DAT_PTR_F181, UDS_ITF_IMP_RD_FNCPTR_F181 },  /* ECU Application Software Identification */
/* 2u*/{UDS_C_DID_ASW_FNGR_PRNT_F184, UDS_C_DID_LEN_F184, UDS_C_DID_ATTR_F184, UDS_ITF_IMP_DAT_PTR_F184, UDS_ITF_IMP_RD_FNCPTR_F184 },  /* ECU Application Software FingerPrint Identification */
/* 3u*/{UDS_C_DID_ACT_DIAG_SESN_F186, UDS_C_DID_LEN_F186, UDS_C_DID_ATTR_F186, UDS_ITF_IMP_DAT_PTR_F186, UDS_ITF_IMP_RD_FNCPTR_F186 },  /* ECU current active diagnostic session Identification */
/* 4u*/{UDS_C_DID_ASW_VER_F189,       UDS_C_DID_LEN_F189, UDS_C_DID_ATTR_F189, UDS_ITF_IMP_DAT_PTR_F189, UDS_ITF_IMP_RD_FNCPTR_F189 },  /* ECU Application Software Version number Identification */
/* 5u*/{UDS_C_DID_SYS_SUP_ID_F18A,    UDS_C_DID_LEN_F18A, UDS_C_DID_ATTR_F18A, UDS_ITF_IMP_DAT_PTR_F18A, UDS_ITF_IMP_RD_FNCPTR_F18A },  /* ECU System Supplier Identifier Identification */
/* 6u*/{UDS_C_DID_MANU_DATE_F18B,     UDS_C_DID_LEN_F18B, UDS_C_DID_ATTR_F18B, UDS_ITF_IMP_DAT_PTR_F18B, UDS_ITF_IMP_RD_FNCPTR_F18B },  /* ECU Manufacturing Date Data Identification */
/* 7u*/{UDS_C_DID_ECU_SN_F18C,        UDS_C_DID_LEN_F18C, UDS_C_DID_ATTR_F18C, UDS_ITF_IMP_DAT_PTR_F18C, UDS_ITF_IMP_RD_FNCPTR_F18C },  /* ECU Serial Number Data Identification */
/* 8u*/{UDS_C_DID_VIN_F190,           UDS_C_DID_LEN_F190, UDS_C_DID_ATTR_F190, UDS_ITF_IMP_DAT_PTR_F190, UDS_ITF_IMP_RD_FNCPTR_F190 },  /* ECU VIN Number Software Identification */
/* 9u*/{UDS_C_DID_SYS_SUP_HWNUM_F192, UDS_C_DID_LEN_F192, UDS_C_DID_ATTR_F192, UDS_ITF_IMP_DAT_PTR_F192, UDS_ITF_IMP_RD_FNCPTR_F192 },  /* ECU System Supplier Hardware Number Identification */
/*10u*/{UDS_C_DID_SYS_SUP_HWVER_F193, UDS_C_DID_LEN_F193, UDS_C_DID_ATTR_F193, UDS_ITF_IMP_DAT_PTR_F193, UDS_ITF_IMP_RD_FNCPTR_F193 },  /* ECU System Supplier Hardware Version Identification */
/*11u*/{UDS_C_DID_BOOT_SW_VER_F195,   UDS_C_DID_LEN_F195, UDS_C_DID_ATTR_F195, UDS_ITF_IMP_DAT_PTR_F195, UDS_ITF_IMP_RD_FNCPTR_F195 },  /* ECU System Supplier Boot software Version Identification */
/*12u*/{UDS_C_DID_4S_CD_TSTR_SN_F198, UDS_C_DID_LEN_F198, UDS_C_DID_ATTR_F198, UDS_ITF_IMP_DAT_PTR_F198, UDS_ITF_IMP_RD_FNCPTR_F198 },  /* ECU repair shop code or tester serial number Identification */
/*13u*/{UDS_C_DID_PROGRAM_DATE_F199,  UDS_C_DID_LEN_F199, UDS_C_DID_ATTR_F199, UDS_ITF_IMP_DAT_PTR_F199, UDS_ITF_IMP_RD_FNCPTR_F199 },  /* ECU programming date Identification */
/*14u*/{UDS_C_DID_PEND_PROG_FLG_F1A0, UDS_C_DID_LEN_F1A0, UDS_C_DID_ATTR_F1A0, UDS_ITF_IMP_DAT_PTR_F1A0, UDS_ITF_IMP_RD_FNCPTR_F1A0 },  /* ECU pending reprogramming flag Identification */
/*15u*/{UDS_C_DID_27SECU_ACC_ST_F1A1, UDS_C_DID_LEN_F1A1, UDS_C_DID_ATTR_F1A1, UDS_ITF_IMP_DAT_PTR_F1A1, UDS_ITF_IMP_RD_FNCPTR_F1A1 },  /* ECU 0x27 Security Access State Identification */
/*16u*/{UDS_C_DID_27SEC_RDM_SED_F1A2, UDS_C_DID_LEN_F1A2, UDS_C_DID_ATTR_F1A2, UDS_ITF_IMP_DAT_PTR_F1A2, UDS_ITF_IMP_RD_FNCPTR_F1A2 },  /* ECU 0x27 Security Access Random seed Identification */
/*17u*/{UDS_C_DID_E2BLK_IVLD_BMP_F1A3,UDS_C_DID_LEN_F1A3, UDS_C_DID_ATTR_F1A3, UDS_ITF_IMP_DAT_PTR_F1A3, UDS_ITF_IMP_RD_FNCPTR_F1A3 },  /* ECU Eeprom Block Invalidity flag bitmap Identification */
/*18u*/{UDS_C_DID_ASW_PRGOK_CNTR_F1A4,UDS_C_DID_LEN_F1A4, UDS_C_DID_ATTR_F1A4, UDS_ITF_IMP_DAT_PTR_F1A4, UDS_ITF_IMP_RD_FNCPTR_F1A4 },  /* ECU Application Software Programming success counter Identification */
/*19u*/{UDS_C_DID_ASW_ATMPT_CNTR_F1A5,UDS_C_DID_LEN_F1A5, UDS_C_DID_ATTR_F1A5, UDS_ITF_IMP_DAT_PTR_F1A5, UDS_ITF_IMP_RD_FNCPTR_F1A5 },  /* ECU Application Software Programming atemptation counter Identification */
/*20u*/{UDS_C_DID_CRC_START_ADD_F1A6, UDS_C_DID_LEN_F1A6, UDS_C_DID_ATTR_F1A6, UDS_ITF_IMP_DAT_PTR_F1A6, UDS_ITF_IMP_RD_FNCPTR_F1A6 },  /* ECU FBL CRC calculation starting address Identification */
/*21u*/{UDS_C_DID_CRC_CALC_SIZE_F1A7, UDS_C_DID_LEN_F1A7, UDS_C_DID_ATTR_F1A7, UDS_ITF_IMP_DAT_PTR_F1A7, UDS_ITF_IMP_RD_FNCPTR_F1A7 },  /* ECU FBL CRC calculation memory size Identification */
/*22u*/{UDS_C_DID_ASWBSWHW_DCID_F1A8, UDS_C_DID_LEN_F1A8, UDS_C_DID_ATTR_F1A8, UDS_ITF_IMP_DAT_PTR_F1A8, UDS_ITF_IMP_RD_FNCPTR_F1A8 },  /* ECU FBL ASW BSW HW Data Compatability InterLock ID Identification */
/*23u*/{UDS_C_DID_MAX_ALOW_PNUM_F1A9, UDS_C_DID_LEN_F1A9, UDS_C_DID_ATTR_F1A9, UDS_ITF_IMP_DAT_PTR_F1A9, UDS_ITF_IMP_RD_FNCPTR_F1A9 },  /* ECU FBL Maximum allowable programming number Identification */
/*24u*/{UDS_C_DID_SECU_PIN_CODE_F1AA, UDS_C_DID_LEN_F1AA, UDS_C_DID_ATTR_F1AA, UDS_ITF_IMP_DAT_PTR_F1AA, UDS_ITF_IMP_RD_FNCPTR_F1AA },  /* ECU FBL Security Pin code Identification */
/*25u*/{UDS_C_DID_SYS_VOLT_CODE_F201, UDS_C_DID_LEN_F201, UDS_C_DID_ATTR_F201, UDS_ITF_IMP_DAT_PTR_F201, UDS_ITF_IMP_RD_FNCPTR_F201 },  /* ECU System voltage Identification */
/*26u*/{UDS_C_DID_ENG_SPEED_CODE_F202, UDS_C_DID_LEN_F202, UDS_C_DID_ATTR_F202, UDS_ITF_IMP_DAT_PTR_F202, UDS_ITF_IMP_RD_FNCPTR_F202 },  /* System engine speed Identification */

/* ADD 2019-10-21 */
/*27u*/{UDS_C_DID_PROT_ID_F810, 	UDS_C_DID_LEN_F810, UDS_C_DID_ATTR_F810, UDS_ITF_IMP_DAT_PTR_F810, UDS_ITF_IMP_RD_FNCPTR_F810 },  /* Protocol Identification */
/*28u*/{UDS_C_DID_VIN_F802, 		UDS_C_DID_LEN_F802, UDS_C_DID_ATTR_F802, UDS_ITF_IMP_DAT_PTR_F802, UDS_ITF_IMP_RD_FNCPTR_F802 },  /* Vehicle Identification Number */
/*29u*/{UDS_C_DID_ECUNAME_F80A, 	UDS_C_DID_LEN_F80A, UDS_C_DID_ATTR_F80A, UDS_ITF_IMP_DAT_PTR_F80A, UDS_ITF_IMP_RD_FNCPTR_F80A },  /* ECU NAME */
/*30u*/{UDS_C_DID_OBDINFO_F491, 	UDS_C_DID_LEN_F491, UDS_C_DID_ATTR_F491, UDS_ITF_IMP_DAT_PTR_F491, UDS_ITF_IMP_RD_FNCPTR_F491 },  /* WWH-OBD ECU OBD System Information */
/*31u*/{UDS_C_DID_SUPT_F400, 		UDS_C_DID_LEN_F400, UDS_C_DID_ATTR_F400, UDS_ITF_IMP_DAT_PTR_F400, UDS_ITF_IMP_RD_FNCPTR_F400 },  /* Identifiers supported */
/*32u*/{UDS_C_DID_SUPT_F420, 		UDS_C_DID_LEN_F420, UDS_C_DID_ATTR_F420, UDS_ITF_IMP_DAT_PTR_F420, UDS_ITF_IMP_RD_FNCPTR_F420 },  /* Identifiers supported */
/*33u*/{UDS_C_DID_SUPT_F440, 		UDS_C_DID_LEN_F440, UDS_C_DID_ATTR_F440, UDS_ITF_IMP_DAT_PTR_F440, UDS_ITF_IMP_RD_FNCPTR_F440 },  /* Identifiers supported */
/*34u*/{UDS_C_DID_SUPT_F460, 		UDS_C_DID_LEN_F460, UDS_C_DID_ATTR_F460, UDS_ITF_IMP_DAT_PTR_F460, UDS_ITF_IMP_RD_FNCPTR_F460 },  /* Identifiers supported */
/*35u*/{UDS_C_DID_SUPT_F480, 		UDS_C_DID_LEN_F480, UDS_C_DID_ATTR_F480, UDS_ITF_IMP_DAT_PTR_F480, UDS_ITF_IMP_RD_FNCPTR_F480 },  /* Identifiers supported */

/*36u*/{UDS_C_DID_SUPT_F600, 		UDS_C_DID_LEN_F600, UDS_C_DID_ATTR_F600, UDS_ITF_IMP_DAT_PTR_F600, UDS_ITF_IMP_RD_FNCPTR_F600 },  /* Identifiers supported */
/*37u*/{UDS_C_DID_SUPT_F620, 		UDS_C_DID_LEN_F620, UDS_C_DID_ATTR_F620, UDS_ITF_IMP_DAT_PTR_F620, UDS_ITF_IMP_RD_FNCPTR_F620 },  /* Identifiers supported */
/*38u*/{UDS_C_DID_SUPT_F640, 		UDS_C_DID_LEN_F640, UDS_C_DID_ATTR_F640, UDS_ITF_IMP_DAT_PTR_F640, UDS_ITF_IMP_RD_FNCPTR_F640 },  /* Identifiers supported */
/*39u*/{UDS_C_DID_SUPT_F660, 		UDS_C_DID_LEN_F660, UDS_C_DID_ATTR_F660, UDS_ITF_IMP_DAT_PTR_F660, UDS_ITF_IMP_RD_FNCPTR_F660 },  /* Identifiers supported */
/*40u*/{UDS_C_DID_SUPT_F680, 		UDS_C_DID_LEN_F680, UDS_C_DID_ATTR_F680, UDS_ITF_IMP_DAT_PTR_F680, UDS_ITF_IMP_RD_FNCPTR_F680 },  /* Identifiers supported */
/*41u*/{UDS_C_DID_SUPT_F6A0, 		UDS_C_DID_LEN_F6A0, UDS_C_DID_ATTR_F6A0, UDS_ITF_IMP_DAT_PTR_F6A0, UDS_ITF_IMP_RD_FNCPTR_F6A0 },  /* Identifiers supported */

/*42u*/{UDS_C_DID_SUPT_F800, 		UDS_C_DID_LEN_F800, UDS_C_DID_ATTR_F800, UDS_ITF_IMP_DAT_PTR_F800, UDS_ITF_IMP_RD_FNCPTR_F800 },  /* Identifiers supported */

/*This Line indicate the end of the data table , do not delete */
/*FFu*/{UDS_C_DID_END_OF_TBL_FFFF,    0x00u,              0x0000u,             NULL,                     NULL                       }

}; /* end of table : UDS_CONFIG_TYPE UDS_DidDatMngRecd_t Uds_g_DidDataTable[]  */

#endif  /* #ifdef UDS_ENABLE_READDATABYIDENTIFIER  */

#endif

#ifdef UDS_ENABLE_WRITEMEMORYBYADDRESS

/* 0x23 ReadMemoryByAddress and
   0x3D WriteMemoryByAddress service data Managing Table
   Note : this table is located in the rom, can not be changed dynamically
               if a new did which is need to read, then must update this table
               before build the whole of project again;

 */
UDS_CONFIG_TYPE UDS_RdWtMemByAddDatMngRecd_t Uds_g_RdWtMemoryByAddrMngTbl[]  =
{
/* Note : bi-rows for each record , please don't use other editor to modify , to avoid indent changing */
/*      RdWtByAddStartAdd                  DataSize                    AccessAttribute                    pDataBuf                        */
/* 0u*/{UDS_C_MADD_40000000,               UDS_C_MADD_LEN_40000000,    UDS_C_MADD_ATTR_40000000,          UDS_ITF_IMP_DPTR_MADD_40000000,
/*      PFuncPtrPreRdChk                   pFuncPtrToReadData          pFuncPtrPreWtChk                   pFuncPtrToWriteData             */
/* 0u*/ UDS_ITF_IMP_PRERD_CHK_FP_40000000,  UDS_ITF_IMP_RD_FP_40000000, UDS_ITF_IMP_PREWT_CHK_FP_40000000, UDS_ITF_IMP_WT_FP_40000000     },



/*This Line indicate the end of the data table , do not delete */
/*FFu*/{0xFFFFFFFFu,                       0x0000u,                    0x0000u,                           NULL,
/*FFu*/ NULL,                              NULL,                       NULL,                              NULL                           },
}; /* end of table UDS_CONFIG_TYPE UDS_RdWtMemByAddDatMngRecd_t Uds_g_RdWtMemoryByAddrMngTbl[]] */
#endif /* end of #ifdef UDS_ENABLE_WRITEMEMORYBYADDRESS */






#ifdef UDS_ENABLE_READDATABYPERIODICIDENTIFIER

/* 0x2A ReadDataByPeriodicalId service data Managing Table
   Note : this table is located in ram, can be updated dynamically,
          the below table is just initialization.
          The period rate of periodicalDid can be configued into below table ,
          so it is very flexable
 */

UDS_RdDatByPeriodDidMngRecd_t Uds_g_RdDatByPeriodDidMngTbl[UDS_C_PERIODIC_DID_MAX_NUM] =
{
/*      PeriodDid  PeriodDidTbl  IsDynaDef  PeriodRateType                                                     Starting    Running     */
/*                      RcdIndex                                                                                                   Offset       PeriodTimer  */
/* 0u*/{0x01u,     0x01u,        FALSE,     UDS_C_PERIODIC_DID_TXMOD_SENDATSLOWRATE, 0x01u,      0x00000000u   },
/* 1u*/{0x02u,     0x02u,        FALSE,     UDS_C_PERIODIC_DID_TXMOD_SENDATSLOWRATE, 0x02u,      0x00000000u   },
/* 2u*/{0xFFu,     0xFFu,        FALSE,     UDS_C_PERIODIC_DID_TXMOD_STOPSENDING, 0x03u,      0x00000000u   },
/* 3u*/{0xFFu,     0xFFu,        FALSE,     UDS_C_PERIODIC_DID_TXMOD_STOPSENDING, 0x04u,      0x00000000u   },
/* 4u*/{0xFFu,     0xFFu,        FALSE,     UDS_C_PERIODIC_DID_TXMOD_STOPSENDING, 0x05u,      0x00000000u   },
/* 5u*/{0xFFu,     0xFFu,        FALSE,     UDS_C_PERIODIC_DID_TXMOD_STOPSENDING, 0x06u,      0x00000000u   },
/* 6u*/{0xFFu,     0xFFu,        FALSE,     UDS_C_PERIODIC_DID_TXMOD_STOPSENDING, 0x07u,      0x00000000u   },
/* 7u*/{0xFFu,     0xFFu,        FALSE,     UDS_C_PERIODIC_DID_TXMOD_STOPSENDING, 0x08u,      0x00000000u   }

}; /* end of table UDS_RdDatByPeriodDidMngRecd_t Uds_g_RdDatByPeriodDidMngTbl[UDS_C_PERIODIC_DID_MAX_NUM] */

#endif /* end of #ifdef UDS_ENABLE_READDATABYPERIODICIDENTIFIER */


#ifdef UDS_ENABLE_DYNAMICALLYDEFINEDATAIDENTIFIER

/* 0x2C DynamicallyDefineDataIdentifier service data Managing Table
   Note : this table is located in ram, can be updated dynamically,
          the below table is just initialization
          The dynamically defined Data Identifier can reference to
          one or more of the pre-configured common DID in table
          Uds_g_DidDataTable.
          The number of the dynamically defined DID , and the number of
          the referenced common did can be combined flexiblely,
          only depends on the RAM resource limitation.
          for example :
          F300 is 1st DynaDefDid, can refer to 4 common did;
          F301 is 2nd dynDefDid, can refer to 8 common did;
          F302 is 3rd dynDefDid, can refer to 2 common did;
          F303 is 4th dynDefDid, can refer to 2 common did;
          totally , all of these 4 dynaDefDid using up all these
          16 record resource in the below table.
          or in the next use case , tester operate can
          define a single DynaDefDid:
          F305 , refer to 16 common did, then no more
          dynaDefDid is supported anymore.


 */
UDS_DynaDefDidMngRecd_t Uds_g_DynaDefDidMngTbl[UDS_C_DYNA_DEF_DID_MAX_NUM] =
{
/*      DynaDefDid ReferedIs Refered   PriorDyna NextDyna Access    Refered  ReferedDid   ReferedDid   Refered   Refered      Refered   */
/*                 DynaDef   DidTblIdx TblIdx    TblIdx   Attribute Did      DatPosition  DatSize      MemTblIdx MemAddr      MemSize   */
/* 0u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/* 1u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/* 2u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/* 3u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/* 4u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/* 5u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/* 6u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/* 7u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/* 8u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/* 9u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/*10u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/*11u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/*12u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/*13u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/*14u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   },
/*15u*/{0xFFFFu,   FALSE,     0xFFu,   0xFFu,     0xFFu,  0x0000u,  0x0000u, 0x00000000u, 0x00000000u, 0x00u,    0x00000000u, 0x00000000u   }

}; /* end of table : UDS_DynaDefDidMngRecd_t Uds_g_DynaDefDidMngTbl[UDS_C_DYNA_DEF_DID_MAX_NUM] */
#endif /* end of #ifdef UDS_ENABLE_DYNAMICALLYDEFINEDATAIDENTIFIER */


#ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER

/* 0x2E WriteDataByIdentifier service data Managing table
        Note : this table is located in the rom, can not be changed dynamically
               if a new did which is need to write , then must update this table
               before build the whole of project again

               a Writable Did must be also readable , so
               the writable Did must be alos configured in reading table :
               Uds_g_DidDataTable

 */
#if 0
UDS_CONFIG_TYPE UDS_DidEepromDatMngRecd_t Uds_g_DidEepromDataTable[] =
{
/*      dataIdentifier                DataSize            AccessAttribute      EepromBlkIdx,               pDataBuf                  pFuncPtrCheckData             pFuncPtrToWriteData */
/* 0u*/{UDS_C_DID_BOOT_SW_ID_F180,    UDS_C_DID_LEN_F180, UDS_C_DID_ATTR_F180, UDS_ITF_IMP_E2_BLKIDX_F180, UDS_ITF_IMP_DAT_PTR_F180, UDS_ITF_IMP_CHK_FNCPTR_F180,  UDS_ITF_IMP_WT_FNCPTR_F180 },  /* ECU Boot Software Identification */
/* 1u*/{UDS_C_DID_APP_SW_ID_F181,     UDS_C_DID_LEN_F181, UDS_C_DID_ATTR_F181, UDS_ITF_IMP_E2_BLKIDX_F181, UDS_ITF_IMP_DAT_PTR_F181, UDS_ITF_IMP_CHK_FNCPTR_F181,  UDS_ITF_IMP_WT_FNCPTR_F181 },  /* ECU Application Software Identification */
/* 2u*/{UDS_C_DID_ASW_FNGR_PRNT_F184, UDS_C_DID_LEN_F184, UDS_C_DID_ATTR_F184, UDS_ITF_IMP_E2_BLKIDX_F184, UDS_ITF_IMP_DAT_PTR_F184, UDS_ITF_IMP_CHK_FNCPTR_F184,  UDS_ITF_IMP_WT_FNCPTR_F184 },  /* ECU Application Software FingerPrint Identification */
/* 3u*/{UDS_C_DID_ASW_VER_F189,       UDS_C_DID_LEN_F189, UDS_C_DID_ATTR_F189, UDS_ITF_IMP_E2_BLKIDX_F189, UDS_ITF_IMP_DAT_PTR_F189, UDS_ITF_IMP_CHK_FNCPTR_F189,  UDS_ITF_IMP_WT_FNCPTR_F189 },  /* ECU Application Software Version number Identification */
/* 4u*/{UDS_C_DID_SYS_SUP_ID_F18A,    UDS_C_DID_LEN_F18A, UDS_C_DID_ATTR_F18A, UDS_ITF_IMP_E2_BLKIDX_F18A, UDS_ITF_IMP_DAT_PTR_F18A, UDS_ITF_IMP_CHK_FNCPTR_F18A,  UDS_ITF_IMP_WT_FNCPTR_F18A },  /* ECU System Supplier Identifier Identification */
/* 5u*/{UDS_C_DID_MANU_DATE_F18B,     UDS_C_DID_LEN_F18B, UDS_C_DID_ATTR_F18B, UDS_ITF_IMP_E2_BLKIDX_F18B, UDS_ITF_IMP_DAT_PTR_F18B, UDS_ITF_IMP_CHK_FNCPTR_F18B,  UDS_ITF_IMP_WT_FNCPTR_F18B },  /* ECU Manufacturing Date Data Identification */
/* 6u*/{UDS_C_DID_ECU_SN_F18C,        UDS_C_DID_LEN_F18C, UDS_C_DID_ATTR_F18C, UDS_ITF_IMP_E2_BLKIDX_F18C, UDS_ITF_IMP_DAT_PTR_F18C, UDS_ITF_IMP_CHK_FNCPTR_F18C,  UDS_ITF_IMP_WT_FNCPTR_F18C },  /* ECU Serial Number Data Identification */
/* 7u*/{UDS_C_DID_VIN_F190,           UDS_C_DID_LEN_F190, UDS_C_DID_ATTR_F190, UDS_ITF_IMP_E2_BLKIDX_F190, UDS_ITF_IMP_DAT_PTR_F190, UDS_ITF_IMP_CHK_FNCPTR_F190,  UDS_itf_writeDid_F190 },  /* ECU VIN Number Software Identification */
/* 8u*/{UDS_C_DID_SYS_SUP_HWNUM_F192, UDS_C_DID_LEN_F192, UDS_C_DID_ATTR_F192, UDS_ITF_IMP_E2_BLKIDX_F192, UDS_ITF_IMP_DAT_PTR_F192, UDS_ITF_IMP_CHK_FNCPTR_F192,  UDS_ITF_IMP_WT_FNCPTR_F192 },  /* ECU System Supplier Hardware Number Identification */
/* 9u*/{UDS_C_DID_SYS_SUP_HWVER_F193, UDS_C_DID_LEN_F193, UDS_C_DID_ATTR_F193, UDS_ITF_IMP_E2_BLKIDX_F193, UDS_ITF_IMP_DAT_PTR_F193, UDS_ITF_IMP_CHK_FNCPTR_F193,  UDS_ITF_IMP_WT_FNCPTR_F193 },  /* ECU System Supplier Hardware Version Identification */
/*10u*/{UDS_C_DID_BOOT_SW_VER_F195,   UDS_C_DID_LEN_F195, UDS_C_DID_ATTR_F195, UDS_ITF_IMP_E2_BLKIDX_F195, UDS_ITF_IMP_DAT_PTR_F195, UDS_ITF_IMP_CHK_FNCPTR_F195,  UDS_ITF_IMP_WT_FNCPTR_F195 },  /* ECU System Supplier Boot software Version Identification */
/*11u*/{UDS_C_DID_4S_CD_TSTR_SN_F198, UDS_C_DID_LEN_F198, UDS_C_DID_ATTR_F198, UDS_ITF_IMP_E2_BLKIDX_F198, UDS_ITF_IMP_DAT_PTR_F198, UDS_ITF_IMP_CHK_FNCPTR_F198,  UDS_ITF_IMP_WT_FNCPTR_F198 },  /* ECU repair shop code or tester serial number Identification */
/*12u*/{UDS_C_DID_PROGRAM_DATE_F199,  UDS_C_DID_LEN_F199, UDS_C_DID_ATTR_F199, UDS_ITF_IMP_E2_BLKIDX_F199, UDS_ITF_IMP_DAT_PTR_F199, UDS_ITF_IMP_CHK_FNCPTR_F199,  UDS_ITF_IMP_WT_FNCPTR_F199 },  /* ECU programming date Identification */
/*13u*/{UDS_C_DID_PEND_PROG_FLG_F1A0, UDS_C_DID_LEN_F1A0, UDS_C_DID_ATTR_F1A0, UDS_ITF_IMP_E2_BLKIDX_F1A0, UDS_ITF_IMP_DAT_PTR_F1A0, UDS_ITF_IMP_CHK_FNCPTR_F1A0,  UDS_ITF_IMP_WT_FNCPTR_F1A0 },  /* ECU pending reprogramming flag Identification */
/*14u*/{UDS_C_DID_27SECU_ACC_ST_F1A1, UDS_C_DID_LEN_F1A1, UDS_C_DID_ATTR_F1A1, UDS_ITF_IMP_E2_BLKIDX_F1A1, UDS_ITF_IMP_DAT_PTR_F1A1, UDS_ITF_IMP_CHK_FNCPTR_F1A1,  UDS_ITF_IMP_WT_FNCPTR_F1A1 },  /* ECU 0x27 Security Access State Identification */
/*15u*/{UDS_C_DID_27SEC_RDM_SED_F1A2, UDS_C_DID_LEN_F1A2, UDS_C_DID_ATTR_F1A2, UDS_ITF_IMP_E2_BLKIDX_F1A2, UDS_ITF_IMP_DAT_PTR_F1A2, UDS_ITF_IMP_CHK_FNCPTR_F1A2,  UDS_ITF_IMP_WT_FNCPTR_F1A2 },  /* ECU 0x27 Security Access Random seed Identification */
/*16u*/{UDS_C_DID_E2BLK_IVLD_BMP_F1A3,UDS_C_DID_LEN_F1A3, UDS_C_DID_ATTR_F1A3, UDS_ITF_IMP_E2_BLKIDX_F1A3, UDS_ITF_IMP_DAT_PTR_F1A3, UDS_ITF_IMP_CHK_FNCPTR_F1A3,  UDS_ITF_IMP_WT_FNCPTR_F1A3 },  /* ECU Eeprom Block Invalidity flag bitmap Identification */
/*17u*/{UDS_C_DID_ASW_PRGOK_CNTR_F1A4,UDS_C_DID_LEN_F1A4, UDS_C_DID_ATTR_F1A4, UDS_ITF_IMP_E2_BLKIDX_F1A4, UDS_ITF_IMP_DAT_PTR_F1A4, UDS_ITF_IMP_CHK_FNCPTR_F1A4,  UDS_ITF_IMP_WT_FNCPTR_F1A4 },  /* ECU Application Software Programming success counter Identification */
/*18u*/{UDS_C_DID_ASW_ATMPT_CNTR_F1A5,UDS_C_DID_LEN_F1A5, UDS_C_DID_ATTR_F1A5, UDS_ITF_IMP_E2_BLKIDX_F1A5, UDS_ITF_IMP_DAT_PTR_F1A5, UDS_ITF_IMP_CHK_FNCPTR_F1A5,  UDS_ITF_IMP_WT_FNCPTR_F1A5 },  /* ECU Application Software Programming atemptation counter Identification */
/*19u*/{UDS_C_DID_CRC_START_ADD_F1A6, UDS_C_DID_LEN_F1A6, UDS_C_DID_ATTR_F1A6, UDS_ITF_IMP_E2_BLKIDX_F1A6, UDS_ITF_IMP_DAT_PTR_F1A6, UDS_ITF_IMP_CHK_FNCPTR_F1A6,  UDS_ITF_IMP_WT_FNCPTR_F1A6 },  /* ECU FBL CRC calculation starting address Identification */
/*20u*/{UDS_C_DID_CRC_CALC_SIZE_F1A7, UDS_C_DID_LEN_F1A7, UDS_C_DID_ATTR_F1A7, UDS_ITF_IMP_E2_BLKIDX_F1A7, UDS_ITF_IMP_DAT_PTR_F1A7, UDS_ITF_IMP_CHK_FNCPTR_F1A7,  UDS_ITF_IMP_WT_FNCPTR_F1A7 },  /* ECU FBL CRC calculation memory size Identification */
/*21u*/{UDS_C_DID_ASWBSWHW_DCID_F1A8, UDS_C_DID_LEN_F1A8, UDS_C_DID_ATTR_F1A8, UDS_ITF_IMP_E2_BLKIDX_F1A8, UDS_ITF_IMP_DAT_PTR_F1A8, UDS_ITF_IMP_CHK_FNCPTR_F1A8,  UDS_ITF_IMP_WT_FNCPTR_F1A8 },  /* ECU FBL ASW BSW HW Data Compatability InterLock ID Identification */
/*22u*/{UDS_C_DID_MAX_ALOW_PNUM_F1A9, UDS_C_DID_LEN_F1A9, UDS_C_DID_ATTR_F1A9, UDS_ITF_IMP_E2_BLKIDX_F1A9, UDS_ITF_IMP_DAT_PTR_F1A9, UDS_ITF_IMP_CHK_FNCPTR_F1A9,  UDS_ITF_IMP_WT_FNCPTR_F1A9 },  /* ECU FBL Maximum allowable programming number Identification */
/*23u*/{UDS_C_DID_SECU_PIN_CODE_F1AA, UDS_C_DID_LEN_F1AA, UDS_C_DID_ATTR_F1AA, UDS_ITF_IMP_E2_BLKIDX_F1AA, UDS_ITF_IMP_DAT_PTR_F1AA, UDS_ITF_IMP_CHK_FNCPTR_F1AA,  UDS_ITF_IMP_WT_FNCPTR_F1AA },  /* ECU FBL Security Pin code Identification */


/*This Line indicate the end of the data table , do not delete */
/*FFu*/{UDS_C_DID_END_OF_TBL_FFFF,    0x00u,              0x0000u,             0x00u,                      NULL,                     NULL,                         NULL                       }

}; /* end of table : UDS_CONFIG_TYPE UDS_DidEepromDatMngRecd_t Uds_g_DidEepromDataTable[]  */
#endif /* end of #ifdef UDS_ENABLE_WRITEDATABYIDENTIFIER */
#endif
#if 0
#ifdef UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER
/* 0x2F InputOutputControlByIdentifier
   Note : this table is located in the rom, can not be changed dynamically
          if a new did which is need to control InputOutput , then must update this table
          before build the whole of project again;

 */

UDS_CONFIG_TYPE UDS_IoCtrlIdxTblRcd_t Uds_g_IoCtrlByDidIdxTable[] =
{
/*      dataIdentifier             DataSize            AccessAttribute      StartIndex,  NumOfEntry    */
/* 0u*/{UDS_C_DID_ECU_OUT_ST_3042, UDS_C_DID_LEN_3042, UDS_C_DID_ATTR_3042, 0x00u,       0x08u     },

/*This Line indicate the end of the data table , do not delete */
/*FFu*/{UDS_C_DID_END_OF_TBL_FFFF, 0x00u,              0x0000u,             0x00u,       0x00u     }

}; /* end of table : UDS_CONFIG_TYPE UDS_IoCtrlIdxTblRcd_t Uds_g_IoCtrlByDidIdxTable[]  */


UDS_CONFIG_TYPE UDS_IoCtrlMngtDatTblRcd_t Uds_g_IoCtrlByDidMngtTable[UDS_2FIOCTRL_MNGT_RCD_MAX_NUM] =
{
/*      pDataBuf                    IoCtrlBitmapMask               pFuncPtrToIoCtrl               */
/* 0u*/{UDS_ITF_IMP_DAT_PTR_3042,   UDS_ITF_IMP_IOCTRL_3042_MSK_1, UDS_ITF_IMP_IOCTRL_3042_FP_1 },
/* 1u*/{UDS_ITF_IMP_DAT_PTR_3042,   UDS_ITF_IMP_IOCTRL_3042_MSK_2, UDS_ITF_IMP_IOCTRL_3042_FP_2 },
/* 2u*/{UDS_ITF_IMP_DAT_PTR_3042,   UDS_ITF_IMP_IOCTRL_3042_MSK_3, UDS_ITF_IMP_IOCTRL_3042_FP_3 },
/* 3u*/{UDS_ITF_IMP_DAT_PTR_3042,   UDS_ITF_IMP_IOCTRL_3042_MSK_4, UDS_ITF_IMP_IOCTRL_3042_FP_4 },
/* 4u*/{UDS_ITF_IMP_DAT_PTR_3042,   UDS_ITF_IMP_IOCTRL_3042_MSK_5, UDS_ITF_IMP_IOCTRL_3042_FP_5 },
/* 5u*/{UDS_ITF_IMP_DAT_PTR_3042,   UDS_ITF_IMP_IOCTRL_3042_MSK_6, UDS_ITF_IMP_IOCTRL_3042_FP_6 },
/* 6u*/{UDS_ITF_IMP_DAT_PTR_3042,   UDS_ITF_IMP_IOCTRL_3042_MSK_7, UDS_ITF_IMP_IOCTRL_3042_FP_7 },
/* 7u*/{UDS_ITF_IMP_DAT_PTR_3042,   UDS_ITF_IMP_IOCTRL_3042_MSK_8, UDS_ITF_IMP_IOCTRL_3042_FP_8 },

}; /* end of table : UDS_CONFIG_TYPE UDS_IoCtrlMngtDatTblRcd_t Uds_g_IoCtrlByDidMngtTable[]  */

#endif // end of #ifdef UDS_ENABLE_INPUTOUTPUTCONTROLBYIDENTIFIER
#endif
#if 0
#ifdef UDS_ENABLE_ROUTINECONTROL
/* 0x31 RoutineControl service data Managing table
        Note : this table is located in the rom, can not be changed dynamically
               if a new did which is need to perform routine control  , then must update this table
               before build the whole of project again
               for this table, due to the number of attributes is greater than 10 , so split as two rows.
 */



UDS_CONFIG_TYPE UDS_RtnCtrlMngtDatTblRcd_t Uds_g_RtnCtrlMngtDataTable[] =
{
/*      routineIdentifier              AccessAttribute               ValidArgumentNum,               RtnRunLimitTime                  ExpectedPosResult              */
/* 0u*/{UDS_C_RID_ERS_MEM_RTN_FD00,    UDS_C_RID_ATTR_FD00,          UDS_C_RID_VLD_ARGU_NUM_FD00,    UDS_C_RID_RUN_TIM_LIMIT_FD00,    UDS_C_RID_EXP_POS_RSLT_FD00,  /* Routine : Erease Memory record , 1st line */
/*      pFuncPtrStart                  pFuncPtrStop                  pFuncPtrGetResult               pFuncPtrRtnCheck                 pFuncPtrIsRtnFinished          */
/* 0u*/ UDS_C_RID_FP_START_RTN_FD00,   UDS_C_RID_FP_STOP_RTN_FD00,   UDS_C_RID_FP_RESULT_RTN_FD00,   UDS_C_RID_FP_CHECK_RTN_FD00,     UDS_C_RID_FP_FINISH_RTN_FD00},/* Routine : Erease Memory record , 2nd line  */
/*      routineIdentifier              AccessAttribute               ValidArgumentNum,               RtnRunLimitTime                  ExpectedPosResult              */
/* 1u*/{UDS_C_RID_CHK_PROG_DPDC_FD01,  UDS_C_RID_ATTR_FD01,          UDS_C_RID_VLD_ARGU_NUM_FD01,    UDS_C_RID_RUN_TIM_LIMIT_FD01,    UDS_C_RID_EXP_POS_RSLT_FD01,  /* Routine: CheckProgramming Dependency record , 1st line */
/*      pFuncPtrStart                  pFuncPtrStop                  pFuncPtrGetResult               pFuncPtrRtnCheck                 pFuncPtrIsRtnFinished          */
/* 1u*/ UDS_C_RID_FP_START_RTN_FD01,   UDS_C_RID_FP_STOP_RTN_FD01,   UDS_C_RID_FP_RESULT_RTN_FD01,   UDS_C_RID_FP_CHECK_RTN_FD01,     UDS_C_RID_FP_FINISH_RTN_FD01},/* Routine: CheckProgramming Dependency record , 2nd line  */
/*      routineIdentifier              AccessAttribute               ValidArgumentNum,               RtnRunLimitTime                  ExpectedPosResult              */
/* 2u*/{UDS_C_RID_CHK_FBL_P_CND_FD02,  UDS_C_RID_ATTR_FD02,          UDS_C_RID_VLD_ARGU_NUM_FD02,    UDS_C_RID_RUN_TIM_LIMIT_FD02,    UDS_C_RID_EXP_POS_RSLT_FD02,  /* Routine: CheckFBL Programming pre-condition record , 1st line */
/*      pFuncPtrStart                  pFuncPtrStop                  pFuncPtrGetResult               pFuncPtrRtnCheck                 pFuncPtrIsRtnFinished          */
/* 2u*/ UDS_C_RID_FP_START_RTN_FD02,   UDS_C_RID_FP_STOP_RTN_FD02,   UDS_C_RID_FP_RESULT_RTN_FD02,   UDS_C_RID_FP_CHECK_RTN_FD02,     UDS_C_RID_FP_FINISH_RTN_FD02},/* Routine: CheckFBL Programming pre-condition record , 2nd line  */
/*      routineIdentifier              AccessAttribute               ValidArgumentNum,               RtnRunLimitTime                  ExpectedPosResult              */
/* 3u*/{UDS_C_RID_CHK_FLSHD_ILI_FD03,  UDS_C_RID_ATTR_FD03,          UDS_C_RID_VLD_ARGU_NUM_FD03,    UDS_C_RID_RUN_TIM_LIMIT_FD03,    UDS_C_RID_EXP_POS_RSLT_FD03,  /* Routine: Check flash driver interlock integrity record , 1st line */
/*      pFuncPtrStart                  pFuncPtrStop                  pFuncPtrGetResult               pFuncPtrRtnCheck                 pFuncPtrIsRtnFinished          */
/* 3u*/ UDS_C_RID_FP_START_RTN_FD03,   UDS_C_RID_FP_STOP_RTN_FD03,   UDS_C_RID_FP_RESULT_RTN_FD03,   UDS_C_RID_FP_CHECK_RTN_FD03,     UDS_C_RID_FP_FINISH_RTN_FD03},/* Routine: Check flash driver interlock integrity record , 2nd line  */
/*      routineIdentifier              AccessAttribute               ValidArgumentNum,               RtnRunLimitTime                  ExpectedPosResult              */
/* 4u*/{UDS_C_RID_CHK_ASWD_VLD_FD04,   UDS_C_RID_ATTR_FD04,          UDS_C_RID_VLD_ARGU_NUM_FD04,    UDS_C_RID_RUN_TIM_LIMIT_FD04,    UDS_C_RID_EXP_POS_RSLT_FD04,  /* Routine: Check application software or data validity record , 1st line */
/*      pFuncPtrStart                  pFuncPtrStop                  pFuncPtrGetResult               pFuncPtrRtnCheck                 pFuncPtrIsRtnFinished          */
/* 4u*/ UDS_C_RID_FP_START_RTN_FD04,   UDS_C_RID_FP_STOP_RTN_FD04,   UDS_C_RID_FP_RESULT_RTN_FD04,   UDS_C_RID_FP_CHECK_RTN_FD04,     UDS_C_RID_FP_FINISH_RTN_FD04},/* Routine: Check application software or data validity record , 2nd line  */
/*      routineIdentifier              AccessAttribute               ValidArgumentNum,               RtnRunLimitTime                  ExpectedPosResult              */
/* 5u*/{UDS_C_RID_STY_IN_BT_FD05,      UDS_C_RID_ATTR_FD05,          UDS_C_RID_VLD_ARGU_NUM_FD05,    UDS_C_RID_RUN_TIM_LIMIT_FD05,    UDS_C_RID_EXP_POS_RSLT_FD05,  /* Routine: Request Stay In Boot Mode record , 1st line */
/*      pFuncPtrStart                  pFuncPtrStop                  pFuncPtrGetResult               pFuncPtrRtnCheck                 pFuncPtrIsRtnFinished          */
/* 5u*/ UDS_C_RID_FP_START_RTN_FD05,   UDS_C_RID_FP_STOP_RTN_FD05,   UDS_C_RID_FP_RESULT_RTN_FD05,   UDS_C_RID_FP_CHECK_RTN_FD05,     UDS_C_RID_FP_FINISH_RTN_FD05},/* Routine: Request Stay In Boot Mode record , 2nd line  */




/* These last two Lines indicate the end of the data table , do not delete                                                                                           */
/*      routineIdentifier              AccessAttribute               ValidArgumentNum,               RtnRunLimitTime                  ExpectedPosResult              */
/*FFu*/{0xFFFFu,                       0x0000u,                      0x00u,                          0x0000u,                         0x00u,                        /* Routine : Last Ending indication record , 1st line */
/*      pFuncPtrStart                  pFuncPtrStop                  pFuncPtrGetResult               pFuncPtrRtnCheck                 pFuncPtrIsRtnFinished          */
/*FFu*/ NULL,                          NULL,                         NULL,                           NULL,                            NULL                        },/* Routine : Last Ending indication record , 2nd line */
}; /* end of table : UDS_CONFIG_TYPE UDS_RtnCtrlMngtDatTblRcd_t Uds_g_RtnCtrlMngtDataTable[]  */

#endif /* end of #ifdef UDS_ENABLE_ROUTINECONTROL */
#endif



#ifdef UDS_ENABLE_REQUESTDOWNLOAD

/* 0x34 RequestDownload
   Note : this table is located in the rom, can not be changed dynamically
          if a new memory address which is need to download by 0x34 service ,
          then must update this table before build the whole of project again;

 */
UDS_CONFIG_TYPE UDS_ReqDnldMngRecd_t Uds_g_ReqDnldMngTbl[]  =
{
/*      ReqDnldStartMemAdd         MemorySize                 AccessAttribute             pDataBuf                          pFuncPtrPreDnldChk                    pFuncPtrDnldWrtData       pFuncPtrIsProgFinished  */
/* 0u*/{UDS_C_REQ_DN_ADD_00000000, UDS_C_REQ_DN_LEN_00000000, UDS_C_REQ_DN_ATTR_00000000, UDS_ITF_IMP_DPTR_REQ_DN_00000000, UDS_ITF_IMP_PRE_DNLD_CHK_FP_00000000, UDS_ITF_IMP_DNLD_WT_DAT_FP_00000000, UDS_ITF_IMP_PROG_IS_FINISH_FP_00000000 },
/* 1u*/{UDS_C_REQ_DN_ADD_40000000, UDS_C_REQ_DN_LEN_40000000, UDS_C_REQ_DN_ATTR_40000000, UDS_ITF_IMP_DPTR_REQ_DN_40000000, UDS_ITF_IMP_PRE_DNLD_CHK_FP_40000000, UDS_ITF_IMP_DNLD_WT_DAT_FP_40000000, UDS_ITF_IMP_PROG_IS_FINISH_FP_40000000 },

/*This Line indicate the end of the data table , do not delete */
/*FFu*/{0xFFFFFFFFu,               0x00000000u,               0x0000u,                    NULL,                             NULL,                                 NULL ,                 NULL                      }

}; /* end of table UDS_CONFIG_TYPE UDS_ReqDnldMngRecd_t Uds_g_ReqDnldMngTbl[]  */

#endif /* end of #ifdef UDS_ENABLE_REQUESTDOWNLOAD */

#ifdef UDS_ENABLE_REQUESTUPLOAD

/* 0x35 RequestUpload
   Note : this table is located in the rom, can not be changed dynamically
          if a new memory address which is need to upload by 0x35 service ,
          then must update this table before build the whole of project again;

 */
UDS_CONFIG_TYPE UDS_ReqUpldMngRecd_t Uds_g_ReqUpldMngTbl[]  =
{
/*      ReqUpldStartMemAdd         MemorySize                 AccessAttribute             pDataBuf                          pFuncPtrPreUpldChk                    pFuncPtrUpldRdData       */
/* 0u*/{UDS_C_REQ_UP_ADD_00000000, UDS_C_REQ_UP_LEN_00000000, UDS_C_REQ_UP_ATTR_00000000, UDS_ITF_IMP_DPTR_REQ_UP_00000000, UDS_ITF_IMP_PRE_UPLD_CHK_FP_00000000, UDS_ITF_IMP_UPLD_RD_DAT_FP_00000000 },
/* 1u*/{UDS_C_REQ_UP_ADD_40000000, UDS_C_REQ_UP_LEN_40000000, UDS_C_REQ_UP_ATTR_40000000, UDS_ITF_IMP_DPTR_REQ_UP_40000000, UDS_ITF_IMP_PRE_UPLD_CHK_FP_40000000, UDS_ITF_IMP_UPLD_RD_DAT_FP_40000000 },

/*This Line indicate the end of the data table , do not delete */
/*FFu*/{0xFFFFFFFFu,               0x00000000u,               0x0000u,                    NULL,                             NULL,                                 NULL                                                           }

}; /* end of table UDS_CONFIG_TYPE UDS_ReqUpldMngRecd_t Uds_g_ReqUpldMngTbl[]  */

#endif /* end of #ifdef UDS_ENABLE_REQUESTUPLOAD */


/*********************************************************************************
* Name: Uds_Generic_DatMgr_SearchTblByKey                                        *
* Description:  Find an index into a table (the searched id is uword             *
*               (16bits)value, or a ubyte(8 bits) value)                         *
* Inputs:  searched ID                                                           *
*			     adress of first ID in table                                           *
*			     size of a table entry                                                 *
* Outputs: index in table for ID found or  UDS_C_IDX_NOT_IN_TBL in case of error *
**********************************************************************************
*/
uint8_t  Uds_Generic_DatMgr_SearchTblByKey (uint16_t                         i_luwKey2Search,    /* searched ID */
                                          UDS_DidDatMngSearchKeyType_t   i_lubSearchKeyType, /* uword(16bits) or ubyte(8bits) */
                                          uint8_t const *                  iPTbl2LookUp, 		   /* address of first ID in table */
                                          uint8_t                          lub_entrySize )	   /* size of a table entry */
{
	 uint8_t  lub_ndx = UDS_C_IDX_NOT_IN_TBL;
	 uint8_t  lub_response = 0x00u;
	 uint16_t luw_DIDInTbl = 0x0000u;
	 uint16_t luw_Key2Search = 0x0000u;
	 uint16_t luw_EndOfTblValue = UDS_C_DID_END_OF_TBL_FFFF;

	 luw_Key2Search = i_luwKey2Search;
   if (i_lubSearchKeyType == KeyTypeIsUINT8)
   {
       luw_Key2Search    &= 0x00FFu;
       luw_EndOfTblValue &= 0x00FFu;
   }
   else
   {
   }

   while (lub_response < lub_ndx )
	 {

       if (i_lubSearchKeyType == KeyTypeIsUINT8)
       {
	         luw_DIDInTbl = (uint16_t)((*iPTbl2LookUp) & 0x00FFu);
       }
       else
       {
		       luw_DIDInTbl = (*((uint16_t *)iPTbl2LookUp));
       }

   		 if (luw_DIDInTbl == luw_EndOfTblValue )
		   {
			     lub_response = UDS_C_IDX_NOT_IN_TBL ;
			     lub_ndx-- ;
		   }
		   else if ( luw_DIDInTbl == luw_Key2Search)
		   {
			     lub_ndx = 0x00u	;
		   }
		   else
		   {
			     lub_response++;
			     iPTbl2LookUp += lub_entrySize ;
		   }
	 }  /* end of while (lub_response < lub_ndx ) */

	 return lub_response ;
} /* end of Uds_Generic_DatMgr_SearchTblByKey */








#endif /* _UDS_SVC_DATA_MNGR_C */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
