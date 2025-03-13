/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv27_securityaccess.c
 * 摘要: UDS Application Layer service 0x27 SecurityAccess.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

//#include "E2Para.h"
#include "uds_confdefs.h"
#include "uds_confpriv.h"
#include "uds_inf.h"
#include "uds_priv.h"
#include "uds_pub.h"
#include "uds_target.h"
#include "uds_api.h"
#include "string.h"
#include "E2Para.h"
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
#include "BtaPos.h"
#endif

/*******蔚来项目请求种子固定值**********/
#define DIAG_REQ_BTA_SRF 	(0x25A2)
#define DIAG_REQ_BTA_L	 	(0x55C8)
#define DIAG_REQ_BTA_R	 	(0x80C2)
#define DIAG_REQ_BTA_SRR	(0x870A)
#define DIAG_REQ_BTA_RB		(0x5D29)
#define DIAG_REQ_BTA_RB2	(0x5D2A)
#define DIAG_REQ_BTA_FB		(0x5D2B)
/********************************/

#ifdef UDS_ENABLE_SECURITYACCESS /* feature inclusion compilation option */
#define EP40_USE 1
static uint8_t rub_27SecuAcc_SeedLvl_1; /* store seed level 1 for previous request seed service*/
static uint8_t rub_27SecuAcc_SeedLvl_2; /* store seed level 2 for previous request seed service*/
static uint8_t rub_27SecuAcc_SeedLvl_3; /* store seed level 3 for previous request seed service*/
static uint8_t rub_27SecuAcc_Prev_SavedSeedLvl;

static uint8_t rub_27SecuAcc_SecurityLvl_1;
static uint8_t rub_27SecuAcc_SecurityLvl_2;
static uint8_t rub_27SecuAcc_SecurityLvl_3;
static uint8_t rub_27SecuAcc_SecurityLvl_4;
static uint8_t rub_27SecuAcc_Prev_SavedSecurityLvl;

static uint8_t rub_27SecuAcc_FailAccessAttemptCntr = 0;
static uint8_t rub_27SecuAcc_unlockFlag = FALSE;
static uint32_t rul_27SecuAcc_FailAccessAttemptDlyTmr = 0xFFFFFFFFu;

static T_UDS_27SECUACC_E2_DAT_TYPE rub_E2_RamImg_27SecuAcc_State;

static T_UDS_27SECUACC_E2_DAT_TYPE rub_Using_E2_RamImg_27SecuAcc_State;

static uint8_t rub_27SecuAcc_ConsecuReqSeedFlg;
static uint8_t rub_27SecuAcc_AllowAttemptSendFlg = FALSE;
static SVRW1_DIDS FFA_FLAG;
static uint8_t raub_27SecuAcc_FrozenSeed[UDS_C_SECU_ACC_KEY_MAX_NUM];
static uint8_t raub_27SecuAcc_LocalComputeKey[UDS_C_SECU_ACC_KEY_MAX_NUM];

static uint8_t rub_ReadOutSerialNumber[0x04u];
static uint8_t rub_ByteNumOfSN;
static uint8_t rub_Sal1ConstArray[0x04u];
static uint8_t rub_Sal2ConstArray[0x04u];
static uint8_t rub_Sal3ConstArray[0x04u];

static uint32_t rul_SA_RandomGenerator_1_IncreaseCntr = 0x00000000u;
static uint32_t rul_SA_RandomGenerator_2_IncreaseCntr = 0x17638C5Eu;
static uint32_t rul_SA_RandomGenerator_3_DecreaseCntr = 0xFFFFFFFFu;
static uint32_t rul_SA_RandomGenerator_4_DecreaseCntr = 0x42952A3Du;

static uint32_t v[0x02u];
static uint32_t k[0x04u];

/* the random integer generator constants definitions */

/* private functions declerations. */

static uint8_t UDS_FN_TYPE UdsApp_27SecuAcc_secLvlSeedKeyHandle(Uds_APduData_t *pAPduInd,
                                                                Uds_APduData_t *pAPduResp,
                                                                uint8_t *i_pRub_secuLvl,
                                                                uint8_t *i_pRub_seedLvl,
                                                                uint8_t i_lub_accessMode,
                                                                uint8_t i_lub_accessLevel,
                                                                uint8_t *i_praub_27SecuAcc_FrozenSeed,
                                                                uint8_t *i_praub_27SecuAcc_LocalComputeKey,
                                                                T_UDS_27SECUACC_COMPUTEKEY_LVL i_pComputeKeyLvlProc);
static void UDS_FN_TYPE UdsApp_27SecuAcc_EncFrmSeed1Key_SAL1(const uint8_t *lpub_seed, uint8_t *lpub_key);
static void UDS_FN_TYPE UdsApp_27SecuAcc_EncFrmSeed2Key_SAL2(const uint8_t *lpub_seed, uint8_t *lpub_key);
static void UDS_FN_TYPE UdsApp_27SecuAcc_EncFrmSeed3Key_SAL3(const uint8_t *lpub_seed, uint8_t *lpub_key);
// static void UDS_FN_TYPE UdsApp_27SecuAcc_IncreFailRetryCntr(void);
static void UDS_FN_TYPE UdsApp_27SecuAcc_CreateSAL1ConstArray(void);
static void UDS_FN_TYPE UdsApp_27SecuAcc_CreateSALnConstArray(uint8_t *lptr_SALConstArray,
                                                              uint8_t *lptr_SALPrevConstArray);
static uint8_t UDS_FN_TYPE UdsApp_27SecuAcc_RandomGenerateSeed(uint8_t i_BytePosition);
static void UDS_FN_TYPE UdsApp_27SecuAcc_GenericEncryptAlgorithm(uint32_t i_num_rounds,
                                                                 uint32_t *iPtr_v,
                                                                 uint32_t *iPtr_k);
static uint32_t canculate_security_access_bcm(uint32_t send, uint32_t APPP_MASK1);
//function of calculate the Key
static unsigned char ASAP1A_CCP_crc8(unsigned char *data, int length);
static uint8_t ASAP1A_CCP_ComputeKeyFromSeed(char *seed1, char * key, unsigned short *sizeKey);
static uint8_t  ASAP1A_CCP_ComputeKeyFromSeed_Internally(char *seed1,char * key,unsigned short *sizeKey);
#if 1
static uint8_t Security_Algorithm_Internally_Proc(Uds_APduData_t *pAPduInd,
                                                      Uds_APduData_t *pAPduResp);
#endif
/* Sun-Function parameter for SecurityAccess(ISO15765-3 limited)
 * 	securityAccessType
 * 	0x01	requestSeed of security level 1
 *	0x02	sendKey of security level 1
 *	0x03	requestSeed of security level 3
 * 	0x04	sendKey of security level 3
 *	...
 *	0x5F	requestSeed of security level 95
 *	0x60	sendKey of security level 95
 */

/* NRC for SecurityAccess
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x24	UDS_NRC_REQUESTSEQUENCEERROR
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 * 	0x35	UDS_NRC_INVALIDKEY
 * 	0x36	UDS_NRC_EXCEEDNUMBEROFATTEMPTS
 * 	0x37	UDS_NRC_REQUIREDTIMEDELAYNOTEXPIRED
 */

/* securityAccessDataRecord for SecurityAccess
 *	user Optional(ISO14229 9.4.2.3)
 */

/*
 * 函数介绍: to process the 0x27 Security Access Request and give response
 * 参数:
 * pAPduInd  : input parameters.
   pAPduResp : output parameters.
 * 返回值:error code
 * 备注: This is the main function that manages the tester requests
 *              concerning the security access services.
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_SecurityAccess(Uds_APduData_t *pAPduInd,
                                                    Uds_APduData_t *pAPduResp)
{
    uint8_t lub_accessMode;
    uint8_t lub_accessLevel;
    uint8_t lub_response;
    uint8_t lub_tmp_Req_IsFuncAddr;

    lub_response = UDS_POSITIVE_RESPONSE;
    lub_accessMode = ((pAPduInd->pduData[0x01u]) & 0x7F);
    lub_accessLevel = ((uint8_t)(lub_accessMode + 0x01u) >> 0x01u);
    /*Check whether the pointer is empty*/
    if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NRC_GENERALREJECT;
    }
    else
    {
        /* code */
    }
#if 1
    /*内部会话模式下进入内部安全算法*/
    if (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_INTERNAL_SESSION)
    {
        lub_response = Security_Algorithm_Internally_Proc(pAPduInd, pAPduResp);
        return lub_response;
    }
    else
    {
        /* code */
    }
#endif
    lub_tmp_Req_IsFuncAddr = UDS_APP_ITF_IMP_GET_TP_CHNL();

    if ((pAPduInd->msgLength) < 0x02u)
    {
        /* (min length of request must not be less than 2)(请求的最小长度不得小于 2) */
        lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        return lub_response;
    }
    else if (lub_tmp_Req_IsFuncAddr == TARGETADDR_FUNCTIONAL) /* Functional addressing */
    {
        lub_response = UDS_NRC_SERVICENOTSUPPORTED;
    }
    else if ((UDS_DEFAULT_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION()) &&
             ((UDS_C_SECU_ACC_SUB_KEY_SAL2 == lub_accessMode) || (UDS_C_SECU_ACC_REQ_SEED_SAL3 == lub_accessMode) ||
              (UDS_C_SECU_ACC_SUB_KEY_SAL3 == lub_accessMode) || (UDS_C_SECU_ACC_REQ_SEED_SAL2 == lub_accessMode) ||
              (UDS_C_SECU_ACC_REQ_SEED_SAL1 == lub_accessMode) || (UDS_C_SECU_ACC_SUB_KEY_SAL1 == lub_accessMode)))
    {
        lub_response = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
        return lub_response;
    }
    else if ((UDS_EXTENDED_DIAGNOSTIC_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION()) &&
             ((UDS_C_SECU_ACC_REQ_SEED_SAL2 == (lub_accessMode & 0xf)) ||
              (UDS_C_SECU_ACC_SUB_KEY_SAL2 == (lub_accessMode & 0xf))))
    {
        lub_response = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
        return lub_response;
    }
    else if ((UDS_EXTENDED_DIAGNOSTIC_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION()) &&
             ((UDS_C_SECU_ACC_REQ_SEED_SAL3 == (lub_accessMode)) ||
              (UDS_C_SECU_ACC_SUB_KEY_SAL3 == (lub_accessMode))))
    {

        lub_response = UDS_NRC_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;
        return lub_response;
    }
    else if ((0x01u == (lub_accessMode & 0x01u)) &&
             lub_accessMode != UDS_C_SECU_ACC_REQ_SEED_SAL1 &&
             lub_accessMode != UDS_C_SECU_ACC_REQ_SEED_SAL2 &&
             lub_accessMode != UDS_C_SECU_ACC_REQ_SEED_SAL3)
    {
        /* request seeds must be odd sub-function ,
           sub-function must be in supported range(请求种子必须是奇数子函数，子函数必须在支持的范围内)
         */
        lub_response = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
        return lub_response;
    }
    else if ((0x00u == (lub_accessMode & 0x01u)) &&
             lub_accessMode != UDS_C_SECU_ACC_SUB_KEY_SAL1 &&
             lub_accessMode != UDS_C_SECU_ACC_SUB_KEY_SAL2 &&
             lub_accessMode != UDS_C_SECU_ACC_SUB_KEY_SAL3)
    {
        /* submit key must be even sub-function ,
           sub-function must be in supported range(提交键必须是偶数子函数，子函数必须在支持范围内)
         */
        lub_response = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
        return lub_response;
    }
    else if ((0x01u == (lub_accessMode & 0x01u)) &&
             pAPduInd->msgLength != 0x02u)
    {
        /* request seeds must be odd sub-function , length must be 2 for ALL SAL(请求种子必须是奇数子函数，所有 SAL 的长度必须为 2) */
        lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        return lub_response;
    }
    else if ((0x00u == (lub_accessMode & 0x01u)) &&
             pAPduInd->msgLength != 0x06u)
    {
        /* submit key must be even sub-function , length must be 6 for all SAL(提交密钥必须是偶数子函数，所有 SAL 的长度必须为 6) */
        lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    else
    {
    }

    if (lub_response != UDS_POSITIVE_RESPONSE)
    {
        return lub_response;
    }
    else
    {
    }

    switch (lub_accessLevel)
    {
    default:
        break;
    case UDS_C_SECU_ACC_SECU_LEVEL1:
        lub_response = UdsApp_27SecuAcc_secLvlSeedKeyHandle(pAPduInd,
                                                            pAPduResp,
                                                            &rub_27SecuAcc_SecurityLvl_1,
                                                            &rub_27SecuAcc_SeedLvl_1,
                                                            lub_accessMode,
                                                            lub_accessLevel,
                                                            raub_27SecuAcc_FrozenSeed,
                                                            raub_27SecuAcc_LocalComputeKey,
                                                            UdsApp_27SecuAcc_EncFrmSeed1Key_SAL1);
        break;

    case UDS_C_SECU_ACC_SECU_LEVEL2:
        lub_response = UdsApp_27SecuAcc_secLvlSeedKeyHandle(pAPduInd,
                                                            pAPduResp,
                                                            &rub_27SecuAcc_SecurityLvl_2,
                                                            &rub_27SecuAcc_SeedLvl_2,
                                                            lub_accessMode,
                                                            lub_accessLevel,
                                                            raub_27SecuAcc_FrozenSeed,
                                                            raub_27SecuAcc_LocalComputeKey,
                                                            UdsApp_27SecuAcc_EncFrmSeed2Key_SAL2);
        break;

    case UDS_C_SECU_ACC_SECU_LEVEL3:
        lub_response = UdsApp_27SecuAcc_secLvlSeedKeyHandle(pAPduInd,
                                                            pAPduResp,
                                                            &rub_27SecuAcc_SecurityLvl_3,
                                                            &rub_27SecuAcc_SeedLvl_3,
                                                            lub_accessMode,
                                                            lub_accessLevel,
                                                            raub_27SecuAcc_FrozenSeed,
                                                            raub_27SecuAcc_LocalComputeKey,
                                                            UdsApp_27SecuAcc_EncFrmSeed3Key_SAL3);
        break;

    } /* end of switch (lub_accessLevel) */

    if ((rub_27SecuAcc_FailAccessAttemptCntr >= UDS_C_SECU_ACC_MAX_ALLOW_FAIL_ATMPT_NUM) &&
        /*(rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag == (uint8_t)FALSE) &&*/
        (rul_27SecuAcc_FailAccessAttemptDlyTmr == 0xFFFFFFFFu ||
         rul_27SecuAcc_FailAccessAttemptDlyTmr == 0x00000000u))
    {

        rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag = (uint8_t)TRUE;
        FFA_FLAG.sig.FAA_Fag_27Service_App = rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag;
		Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT1, FFA_FLAG.u8_data, SVRW1_DID_BYTES_SIZE);
		/*  40 * 250 = 10000ms */
        rul_27SecuAcc_FailAccessAttemptDlyTmr = UDS_C_SECUACC_RETRY_DELAY_REL_TIME;


    }
    else
    {
    }

    if (UDS_POSITIVE_RESPONSE != lub_response &&
        UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT != lub_response)
    {
        rub_27SecuAcc_SeedLvl_1 = UDS_C_SECU_ACC_NO_SEED_VALUE;
        rub_27SecuAcc_SeedLvl_2 = UDS_C_SECU_ACC_NO_SEED_VALUE;
        rub_27SecuAcc_SeedLvl_3 = UDS_C_SECU_ACC_NO_SEED_VALUE;
    }
    else
    {
    }
    return lub_response;

} /* end of Uds_SecurityAccess() */
#if 1
/******************************************************************************
 * Name: Security_Algorithm_Internally_Proc
 * Description: The security algorithm is used internally by the vendor
 * Parameter:   lub_accessMode:security algorithm mode
 * Return: 		Nothing
 * Critical:    No.
 * Explanation: No.
 ******************************************************************************/
#define APP_SUPPLY_MASK 0x5A5A5A5A
static uint8_t Security_Algorithm_Internally_Proc(Uds_APduData_t* pAPduInd, Uds_APduData_t* pAPduResp)
{
    uint8_t lub_accessMode;
    uint8_t  lub_response = UDS_POSITIVE_RESPONSE;

    static uint8_t key[4] = {0};
    uint8_t send[4] = {0};
    uint8_t Rcv_Key[4] = {0};
    uint8_t lpub_key[4] = {0};
    uint16_t key_size;
    lub_accessMode = ((pAPduInd->pduData[0x01u]) & 0x7F);
    /*For internal use, request seed*/
    if ((lub_accessMode == UDS_C_SECU_ACC_REQ_SEED_SALS) && (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_INTERNAL_SESSION))
    {
       /*Response filling*/
#if 1
       raub_27SecuAcc_FrozenSeed[0x00u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x00u);
       raub_27SecuAcc_FrozenSeed[0x01u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x01u);
       raub_27SecuAcc_FrozenSeed[0x02u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x02u);
       raub_27SecuAcc_FrozenSeed[0x03u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x03u);
#else
       raub_27SecuAcc_FrozenSeed[0x00u] = (0x11);
       raub_27SecuAcc_FrozenSeed[0x01u] = (0x22);
       raub_27SecuAcc_FrozenSeed[0x02u] = (0x33);
       raub_27SecuAcc_FrozenSeed[0x03u] = (0x44);
#endif
       pAPduResp->msgLength = 0x06u;
       pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u] + 0x40u;
       pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];
       pAPduResp->pduData[0x02u] = raub_27SecuAcc_FrozenSeed[0x00u];
       pAPduResp->pduData[0x03u] = raub_27SecuAcc_FrozenSeed[0x01u];
       pAPduResp->pduData[0x04u] = raub_27SecuAcc_FrozenSeed[0x02u];
       pAPduResp->pduData[0x05u] = raub_27SecuAcc_FrozenSeed[0x03u];
       return lub_response;
    }
    /*For internal use, get the key*/
    else if ((lub_accessMode == UDS_C_SECU_ACC_SUB_KEY_SALS) && (UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION() == UDS_INTERNAL_SESSION))
    {
       memcpy(Rcv_Key, &(pAPduInd->pduData[2]), 0x04u);
       //send = raub_27SecuAcc_FrozenSeed[3] + (raub_27SecuAcc_FrozenSeed[2] << 8) + (raub_27SecuAcc_FrozenSeed[1] << 16) + (raub_27SecuAcc_FrozenSeed[0] << 24);
       ASAP1A_CCP_ComputeKeyFromSeed_Internally(raub_27SecuAcc_FrozenSeed, key,&key_size);


      lpub_key[0x00u] = key[0];
      lpub_key[0x01u] = key[1];
      lpub_key[0x02u] = key[2];
      lpub_key[0x03u] = key[3];
       if ((lpub_key[0x00u] == Rcv_Key[0x00u]) && (lpub_key[0x01u] == Rcv_Key[0x01u]) &&
        (lpub_key[0x02u] == Rcv_Key[0x02u]) && (lpub_key[0x03u] == Rcv_Key[0x03u]))
       {
    	   /*供应商解锁标志*/
    	   udsapi_Supplier_unlock_flag = TRUE;
            /*The internal security algorithm is successfully unlocked. Procedure*/
           rub_27SecuAcc_SecurityLvl_4 = lub_accessMode/2;
           pAPduResp->msgLength = 0x02u;
           pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u] + 0x40u;
           pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];

       }
       else
       {
           rub_27SecuAcc_SecurityLvl_4 = UDS_C_SECU_ACC_SECU_LEVEL4_LOCKED;
           lub_response = UDS_NRC_INVALIDKEY;
       }
       return lub_response;
    }
    else
    {
       return lub_response;
    }
}
#endif

/*
 * 函数介绍: common process for handling seed and key
 * 参数:
 * 返回值: {*}
 * 备注:
 */
static uint8_t UDS_FN_TYPE UdsApp_27SecuAcc_secLvlSeedKeyHandle(Uds_APduData_t *pAPduInd,
                                                                Uds_APduData_t *pAPduResp,
                                                                uint8_t *i_pRub_secuLvl,
                                                                uint8_t *i_pRub_seedLvl,
                                                                uint8_t i_lub_accessMode,
                                                                uint8_t i_lub_accessLevel,
                                                                uint8_t *i_praub_27SecuAcc_FrozenSeed,
                                                                uint8_t *i_praub_27SecuAcc_LocalComputeKey,
                                                                T_UDS_27SECUACC_COMPUTEKEY_LVL i_pComputeKeyLvlProc)

{
    static uint8_t laub_keyR[UDS_C_SECU_ACC_KEY_MAX_NUM];
    uint8_t lub_response;

    lub_response = UDS_POSITIVE_RESPONSE;
    if (pAPduInd == NULL || pAPduResp == NULL || i_pRub_secuLvl == NULL || i_pRub_seedLvl == NULL ||
        i_praub_27SecuAcc_FrozenSeed == NULL || i_praub_27SecuAcc_LocalComputeKey == NULL || i_pComputeKeyLvlProc == NULL)
    {
        return UDS_NRC_GENERALREJECT;
    }
    else
    {
        /* code */
    }

    /* test if is request seed or submit key
       if sub-function is odd number , it is request seeds.
       otherwise, it is an even number, it is submit key
     */
    if (UDS_27SECUACC_SUBFUNC_IS_ODD_NUMBER(i_lub_accessMode))
    {
        /* odd number = request seed */
        if (rul_27SecuAcc_FailAccessAttemptDlyTmr &&
            rul_27SecuAcc_FailAccessAttemptDlyTmr != 0xFFFFFFFFu)
        {
            /* if the delay timer of the 0x27 security Retry restriction is running */
            lub_response = UDS_NRC_REQUIREDTIMEDELAYNOTEXPIRED;
            /* reset the potential consecutive req seed flag to false */
            UDS_DISABLE_ALL_INTERRUPTS();
            rub_27SecuAcc_ConsecuReqSeedFlg = (uint8_t)FALSE;
            UDS_ENABLE_ALL_INTERRUPTS();
            return lub_response;
        }
        else
        {
        }

        /* store the length of response */
        (pAPduResp->msgLength) = UDS_C_SECU_ACC_SUBMIT_KEY_LEN;

        /* first byte is the same like in request : sub function */
        pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u];
        pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];

        /* prepare response */
        if (i_lub_accessLevel == *i_pRub_secuLvl) // rub_27SecuAcc_SecurityLvl_1)
        {
            /* the ECU was already unlocked */
            pAPduResp->pduData[0x02u] = UDS_C_SECU_ACC_ALRDY_ULCKD_SED_BYTE;
            pAPduResp->pduData[0x03u] = UDS_C_SECU_ACC_ALRDY_ULCKD_SED_BYTE;
            pAPduResp->pduData[0x04u] = UDS_C_SECU_ACC_ALRDY_ULCKD_SED_BYTE;
            pAPduResp->pduData[0x05u] = UDS_C_SECU_ACC_ALRDY_ULCKD_SED_BYTE;
        }
        else /* if (i_lub_accessLevel != *i_pRub_secuLvl) */
        {
            if (rub_27SecuAcc_Prev_SavedSeedLvl == UDS_C_SECU_ACC_NO_SEED_VALUE)
            {
                /* freeze the current seed */
#if EP40_USE
                raub_27SecuAcc_FrozenSeed[0x00u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x00u);
                raub_27SecuAcc_FrozenSeed[0x01u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x01u);
                raub_27SecuAcc_FrozenSeed[0x02u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x02u);
                raub_27SecuAcc_FrozenSeed[0x03u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x03u);
#else
                raub_27SecuAcc_FrozenSeed[0x00u] = 0x11;
                raub_27SecuAcc_FrozenSeed[0x01u] = 0x22;
                raub_27SecuAcc_FrozenSeed[0x02u] = 0x33;
                raub_27SecuAcc_FrozenSeed[0x03u] = 0x44;
#endif
            }
            else /* if (rub_27SecuAcc_Prev_SavedSeedLvl != UDS_C_SECU_ACC_NO_SEED_VALUE) */
            {
                if (rub_27SecuAcc_ConsecuReqSeedFlg == (uint8_t)TRUE)
                {
                    UDS_DISABLE_ALL_INTERRUPTS();
                    if(rub_27SecuAcc_FailAccessAttemptCntr < UDS_C_SECU_ACC_MAX_ALLOW_FAIL_ATMPT_NUM)
                    {
                    	rub_27SecuAcc_FailAccessAttemptCntr++;
                    }
                    else
                    {
                    	rub_27SecuAcc_FailAccessAttemptCntr= UDS_C_SECU_ACC_MAX_ALLOW_FAIL_ATMPT_NUM;
                    	UDS_DISABLE_ALL_INTERRUPTS();
						rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag = (uint8_t)TRUE;
						FFA_FLAG.sig.FAA_Fag_27Service_App = rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag;
						Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT1, FFA_FLAG.u8_data, SVRW1_DID_BYTES_SIZE);
						rul_27SecuAcc_FailAccessAttemptDlyTmr = UDS_C_SECUACC_RETRY_DELAY_REL_TIME; /*  40 * 250 = 10000ms */
						UDS_ENABLE_ALL_INTERRUPTS();
                    }
                   UDS_ENABLE_ALL_INTERRUPTS();
                }
                else
                {
                }

                if (rub_27SecuAcc_Prev_SavedSeedLvl == i_lub_accessLevel &&
                    rub_27SecuAcc_ConsecuReqSeedFlg == (uint8_t)TRUE)
                {
                }
                else /* if (rub_27SecuAcc_Prev_SavedSeedLvl != i_lub_accessLevel) */
                {
                    /* freeze the current seed */

#if EP40_USE
                    raub_27SecuAcc_FrozenSeed[0x00u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x00u);
                    raub_27SecuAcc_FrozenSeed[0x01u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x01u);
                    raub_27SecuAcc_FrozenSeed[0x02u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x02u);
                    raub_27SecuAcc_FrozenSeed[0x03u] = UdsApp_27SecuAcc_RandomGenerateSeed(0x03u);
#else
                    raub_27SecuAcc_FrozenSeed[0x00u] = 0x55;
                    raub_27SecuAcc_FrozenSeed[0x01u] = 0x66;
                    raub_27SecuAcc_FrozenSeed[0x02u] = 0x77;
                    raub_27SecuAcc_FrozenSeed[0x03u] = 0x88;
#endif
                    if(rub_27SecuAcc_FailAccessAttemptCntr == UDS_C_SECU_ACC_MAX_ALLOW_FAIL_ATMPT_NUM &&
                    		rul_27SecuAcc_FailAccessAttemptDlyTmr == 0x0)
                    {
                    	UDS_ENABLE_ALL_INTERRUPTS();
                    	rul_27SecuAcc_FailAccessAttemptDlyTmr = UDS_C_SECUACC_RETRY_DELAY_REL_TIME; /*  40 * 250 = 10000ms */
                    	UDS_ENABLE_ALL_INTERRUPTS();
                    }
                    else
                    {}
                } /* end of else if (rub_27SecuAcc_Prev_SavedSeedLvl != UDS_C_SECU_ACC_NO_SEED_VALUE) */

            } /* end of else if (rub_27SecuAcc_Prev_SavedSeedLvl != UDS_C_SECU_ACC_NO_SEED_VALUE) */
            /* store the seed level*/
            *i_pRub_seedLvl = i_lub_accessLevel;
            rub_27SecuAcc_Prev_SavedSeedLvl = i_lub_accessLevel;
            pAPduResp->pduData[0x02u] = i_praub_27SecuAcc_FrozenSeed[0x00u];
            pAPduResp->pduData[0x03u] = i_praub_27SecuAcc_FrozenSeed[0x01u];
            pAPduResp->pduData[0x04u] = i_praub_27SecuAcc_FrozenSeed[0x02u];
            pAPduResp->pduData[0x05u] = i_praub_27SecuAcc_FrozenSeed[0x03u];

            /* set the potential consecutive req seed flag to true */
            UDS_DISABLE_ALL_INTERRUPTS();
            rub_27SecuAcc_ConsecuReqSeedFlg = (uint8_t)TRUE;
            UDS_ENABLE_ALL_INTERRUPTS();

        } /* end of else if (i_lub_accessLevel != *i_pRub_secuLvl) */

    } /* end of if (UDS_27SECUACC_SUBFUNC_IS_ODD_NUMBER(i_lub_accessMode) ) */
    else /* if (!UDS_27SECUACC_SUBFUNC_IS_ODD_NUMBER(i_lub_accessMode) ) */
    {

        /* reset the potential consecutive req seed flag to false */
        UDS_DISABLE_ALL_INTERRUPTS();
        rub_27SecuAcc_ConsecuReqSeedFlg = (uint8_t)FALSE;
        UDS_ENABLE_ALL_INTERRUPTS();

        /* if the access level of this submit key is matching the previous
           request seed access level .
           */
        if (*i_pRub_seedLvl == i_lub_accessLevel &&
            rub_27SecuAcc_Prev_SavedSeedLvl == i_lub_accessLevel &&
            *i_pRub_secuLvl != i_lub_accessLevel &&
            rub_27SecuAcc_Prev_SavedSecurityLvl != i_lub_accessLevel)
        {
            /* a request for a seed with same level was done in the past */
            laub_keyR[0x00u] = pAPduInd->pduData[0x02u];
            laub_keyR[0x01u] = pAPduInd->pduData[0x03u];
            laub_keyR[0x02u] = pAPduInd->pduData[0x04u];
            laub_keyR[0x03u] = pAPduInd->pduData[0x05u];

            /* compute key by calling the encryption function pointer */
            (i_pComputeKeyLvlProc)(i_praub_27SecuAcc_FrozenSeed, i_praub_27SecuAcc_LocalComputeKey);
            static uint8_t temp_key[4];
            memcpy(temp_key,i_praub_27SecuAcc_LocalComputeKey,4);
            /* compare results is matching with every bytes */
            if ((laub_keyR[0x00u] == i_praub_27SecuAcc_LocalComputeKey[0x00u]) &&
                (laub_keyR[0x01u] == i_praub_27SecuAcc_LocalComputeKey[0x01u]) &&
                (laub_keyR[0x02u] == i_praub_27SecuAcc_LocalComputeKey[0x02u]) &&
                (laub_keyR[0x03u] == i_praub_27SecuAcc_LocalComputeKey[0x03u]))
            {
            	if(rub_27SecuAcc_FailAccessAttemptCntr > 0)
            	{
            		rub_27SecuAcc_FailAccessAttemptCntr--;
            		/*失败次数小于3时清掉FAA标志，开启延时时间设置无效*/
            	    rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag = (uint8_t)FALSE;
            	    FFA_FLAG.sig.FAA_Fag_27Service_App = rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag;
            	    Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT1, FFA_FLAG.u8_data, SVRW1_DID_BYTES_SIZE);
            	    rul_27SecuAcc_FailAccessAttemptDlyTmr = 0xffffffff;
            	}
            	else
            	{}
                /* store the length of response */
                (pAPduResp->msgLength) = 0x02u;
                rub_27SecuAcc_unlockFlag = TRUE;
                /* first byte is the same like in request : sub function */
                pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];

                /* set the security level */
                *i_pRub_secuLvl = i_lub_accessLevel;
                if (rub_27SecuAcc_Prev_SavedSecurityLvl != i_lub_accessLevel)
                {
                    switch (rub_27SecuAcc_Prev_SavedSecurityLvl)
                    {
                    default:
                        break;
                    case UDS_C_SECU_ACC_SECU_LEVEL1:
                        rub_27SecuAcc_SecurityLvl_1 = UDS_C_SECU_ACC_SECU_LEVEL1_LOCKED;
                        break;
                    case UDS_C_SECU_ACC_SECU_LEVEL2:
                        rub_27SecuAcc_SecurityLvl_2 = UDS_C_SECU_ACC_SECU_LEVEL2_LOCKED;
                        break;
                    case UDS_C_SECU_ACC_SECU_LEVEL3:
                        rub_27SecuAcc_SecurityLvl_3 = UDS_C_SECU_ACC_SECU_LEVEL3_LOCKED;
                        break;
                    }
                }
                else
                {
                }
                rub_27SecuAcc_Prev_SavedSecurityLvl = i_lub_accessLevel;

                /* reset seed */
                rub_27SecuAcc_SeedLvl_1 = UDS_C_SECU_ACC_NO_SEED_VALUE;
                rub_27SecuAcc_SeedLvl_2 = UDS_C_SECU_ACC_NO_SEED_VALUE;
                rub_27SecuAcc_SeedLvl_3 = UDS_C_SECU_ACC_NO_SEED_VALUE;
            }
            else /* if ((laub_keyR[0u] != i_praub_27SecuAcc_LocalComputeKey[0u]) || ... */
            {
                UDS_DISABLE_ALL_INTERRUPTS();
                if(rub_27SecuAcc_FailAccessAttemptCntr < UDS_C_SECU_ACC_MAX_ALLOW_FAIL_ATMPT_NUM)
                {
                rub_27SecuAcc_FailAccessAttemptCntr++;
                }
                else
                {
                	rub_27SecuAcc_FailAccessAttemptCntr = UDS_C_SECU_ACC_MAX_ALLOW_FAIL_ATMPT_NUM;
                }
                UDS_ENABLE_ALL_INTERRUPTS();

                *i_pRub_seedLvl = UDS_C_SECU_ACC_NO_SEED_VALUE;

                /* prepare negative response */
                if (rub_27SecuAcc_FailAccessAttemptCntr >= UDS_C_SECU_ACC_MAX_ALLOW_FAIL_ATMPT_NUM)
                {
					lub_response = UDS_NRC_EXCEEDNUMBEROFATTEMPTS;
					rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag = (uint8_t)TRUE;
					FFA_FLAG.sig.FAA_Fag_27Service_App = rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag;
					Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT1, FFA_FLAG.u8_data,SVRW1_DID_BYTES_SIZE);
					rul_27SecuAcc_FailAccessAttemptDlyTmr = UDS_C_SECUACC_RETRY_DELAY_REL_TIME; /*  40 * 250 = 10000ms */
                }
                else
                {
                    lub_response = UDS_NRC_INVALIDKEY;
                }
            } /* end of else // if ((laub_keyR[0u] != raub_27SecuAcc_LocalComputeKey[0u]) || ... */
        } /* end of if (*i_pRub_seedLvl == i_lub_accessLevel ) */
        else /* if (*i_pRub_seedLvl != i_lub_accessLevel ) */
        {
            /* invalid security level ( doesn't concurr with seed request )
             the failure retry counter will only be increased a complete
             request seed + send key sequence , if sequence is not correct
             only return 0x24 requestSequenceError , no counter increasing will be done.
             prepare negative response
           */
            lub_response = UDS_NRC_REQUESTSEQUENCEERROR;
        } /* end of else if (*i_pRub_seedLvl != i_lub_accessLevel ) */
    } /* end of else	//if (!SECU_IS_ODD_NUMBER(i_lub_accessMode) ) */

    return lub_response;

} /* end of FAR_FCT uint8_t disecu_secLvlSeedKeyHandle() */

/*
 * 函数介绍: Initialize the security access values
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_27SecuAcc_Init(void)
{
    UdsApp_27SecuAcc_SwitchDiagSessionInit();

    UDS_DISABLE_ALL_INTERRUPTS();
    /* init counter */
    rub_27SecuAcc_FailAccessAttemptCntr = 0x00u;
    rub_27SecuAcc_unlockFlag = FALSE;
    /* init the potential consecutive req seed flag to false		  */
    rub_27SecuAcc_ConsecuReqSeedFlg = (uint8_t)FALSE;
    UDS_ENABLE_ALL_INTERRUPTS();

    Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_BOOT1, FFA_FLAG.u8_data, SVRW1_DID_BYTES_SIZE);
    /* this should check the FAA_Flag */
    rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag = FFA_FLAG.sig.FAA_Fag_27Service_App;

    if (rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag == (uint8_t)TRUE)
    {
        UDS_DISABLE_ALL_INTERRUPTS();
        rul_27SecuAcc_FailAccessAttemptDlyTmr = UDS_C_SECUACC_RETRY_DELAY_REL_TIME; /*  40 * 250 = 10000ms */
        rub_27SecuAcc_FailAccessAttemptCntr = UDS_C_SECU_ACC_MAX_ALLOW_FAIL_ATMPT_NUM;
        UDS_ENABLE_ALL_INTERRUPTS();
    }
    else
    {
        UDS_DISABLE_ALL_INTERRUPTS();
        /* stop timer */
        rul_27SecuAcc_FailAccessAttemptDlyTmr = 0xFFFFFFFFu;
        rub_27SecuAcc_FailAccessAttemptCntr = 0;
        UDS_ENABLE_ALL_INTERRUPTS();
    }

    UdsApp_27SecuAcc_CreateSAL1ConstArray(); /* create sa const */

}

/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_27SecuAcc_SwitchDiagSessionInit(void)
{
    /* no seed request before */
    rub_27SecuAcc_SeedLvl_1 = UDS_C_SECU_ACC_NO_SEED_VALUE;
    rub_27SecuAcc_SeedLvl_2 = UDS_C_SECU_ACC_NO_SEED_VALUE;
    rub_27SecuAcc_SeedLvl_3 = UDS_C_SECU_ACC_NO_SEED_VALUE;
    rub_27SecuAcc_Prev_SavedSeedLvl = UDS_C_SECU_ACC_NO_SEED_VALUE;

    /* after init the ECU is locked */
    rub_27SecuAcc_SecurityLvl_1 = UDS_C_SECU_ACC_SECU_LEVEL1_LOCKED;
    rub_27SecuAcc_SecurityLvl_2 = UDS_C_SECU_ACC_SECU_LEVEL2_LOCKED;
    rub_27SecuAcc_SecurityLvl_3 = UDS_C_SECU_ACC_SECU_LEVEL3_LOCKED;
    rub_27SecuAcc_SecurityLvl_4 = UDS_C_SECU_ACC_SECU_LEVEL4_LOCKED;
    rub_27SecuAcc_Prev_SavedSecurityLvl = UDS_C_SECU_ACC_SECU_LEVEL1_LOCKED;

    raub_27SecuAcc_FrozenSeed[0x00u] = 0x00u;
    raub_27SecuAcc_FrozenSeed[0x01u] = 0x00u;
    raub_27SecuAcc_FrozenSeed[0x02u] = 0x00u;
    raub_27SecuAcc_FrozenSeed[0x03u] = 0x00u;

    /*供应商解锁标志*/
    udsapi_Supplier_unlock_flag = FALSE;
}

/*
 * 函数介绍: timer manager for the security
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_27SecuAcc_CyclicTskEntryPoint(void)
{

    if (rul_SA_RandomGenerator_1_IncreaseCntr < 0xFFFFFFFFu)
    {
        rul_SA_RandomGenerator_1_IncreaseCntr++;
    }
    else
    {
        rul_SA_RandomGenerator_1_IncreaseCntr = 0x00000000u;
    }

    if (rul_SA_RandomGenerator_2_IncreaseCntr < 0xFFFFFFFFu)
    {
        rul_SA_RandomGenerator_2_IncreaseCntr += 0x02u;
    }
    else
    {
        rul_SA_RandomGenerator_2_IncreaseCntr = 0x17638C5Eu;
    }

    if (rul_SA_RandomGenerator_3_DecreaseCntr > 0x00000000u)
    {
        rul_SA_RandomGenerator_3_DecreaseCntr -= 0x03u;
    }
    else
    {
        rul_SA_RandomGenerator_3_DecreaseCntr = 0xFFFFFFFFu;
    }

    if (rul_SA_RandomGenerator_4_DecreaseCntr > 0x00000000u)
    {
        rul_SA_RandomGenerator_4_DecreaseCntr -= 0x04u;
    }
    else
    {
        rul_SA_RandomGenerator_4_DecreaseCntr = 0x42952A3Du;
    }

    if (rul_27SecuAcc_FailAccessAttemptDlyTmr &&
        rul_27SecuAcc_FailAccessAttemptDlyTmr != 0xFFFFFFFFu)
    {
        UDS_DISABLE_ALL_INTERRUPTS();
        rul_27SecuAcc_FailAccessAttemptDlyTmr--;
        /* reset the potential consecutive req seed flag to false */
        rub_27SecuAcc_ConsecuReqSeedFlg = (uint8_t)FALSE;
        UDS_ENABLE_ALL_INTERRUPTS();
    }
    else
    {
    }
#if 0
    if (0x00000000u == rul_27SecuAcc_FailAccessAttemptDlyTmr)
    {
        if (rub_27SecuAcc_FailAccessAttemptCntr > 0x00u)
        {
            UDS_DISABLE_ALL_INTERRUPTS();
            rub_27SecuAcc_FailAccessAttemptCntr = 0;
            UDS_ENABLE_ALL_INTERRUPTS();
        }
        else
        {
        }

        /* init the potential consecutive req seed flag to false
           after security delay 10s timer is expired.
         */
        UDS_DISABLE_ALL_INTERRUPTS();
        rub_27SecuAcc_ConsecuReqSeedFlg = (uint8_t)FALSE;
        rul_27SecuAcc_FailAccessAttemptDlyTmr = 0xFFFFFFFFu;
        UDS_ENABLE_ALL_INTERRUPTS();
    }
    else
    {
    }
#endif
#if 0
    if (rub_27SecuAcc_FailAccessAttemptCntr < UDS_C_SECU_ACC_MAX_ALLOW_FAIL_ATMPT_NUM &&
        (((uint32_t)0x00000000u == rul_27SecuAcc_FailAccessAttemptDlyTmr) ||
         ((uint32_t)0xFFFFFFFFu == rul_27SecuAcc_FailAccessAttemptDlyTmr)))
    {
        UDS_DISABLE_ALL_INTERRUPTS();
        rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag = (uint8_t)FALSE;
        //FAA_Fag_27Service[1] = rub_Using_E2_RamImg_27SecuAcc_State.m_bitmap.m_FAA_Flag;
        //Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_BOOT1, FAA_Fag_27Service, sizeof(FAA_Fag_27Service));
        UDS_ENABLE_ALL_INTERRUPTS();
    }
    else
    {
    }
#endif
    if ((rub_E2_RamImg_27SecuAcc_State.uw != rub_Using_E2_RamImg_27SecuAcc_State.uw))

    {
        rub_Using_E2_RamImg_27SecuAcc_State.uw = rub_E2_RamImg_27SecuAcc_State.uw;
        /*  this condition can not be applicable temporarily
        if ((!UDS_APP_ITF_IMP_GET_OVER_VOLTAGE_FAIL() ) &&
            (!UDS_APP_ITF_IMP_GET_UNDER_VOLTAGE_FAIL()))
         */
        {

            UDS_APP_ITF_IMP_SET_E2_WRITE_FLAG(UDS_APP_ITF_IMP_GET_E2_IDX_27SECUACC_ST);
        }
        /* else  */
        {
        }
    }
    else
    {
    }
    if (UDS_DEFAULT_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION())
    {
        rub_27SecuAcc_unlockFlag = FALSE;
    }
}
/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_27SecuAcc_Pending(void)
{
    /* dummy function , reserved for future */
}
/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_27SecuAcc_Pend_Overtime(void)
{
    /* dummy function , reserved for future. */
}

/*
 * 函数介绍: this function is just a interface example, should use the value read from the DID of F18C
   EcuSerialNumberDataIdentifier
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_27SecuAcc_ReadSerialNumber(uint8_t *lptr_ReadOutSerialNumber, uint8_t *lptr_ByteNumOfSN)
{
    if (NULL ==lptr_ReadOutSerialNumber || NULL == lptr_ByteNumOfSN)
    {
        return;
    }
    else
    {
#if 0
    uint8_t ecu_sn_len = 8;
    lptr_ReadOutSerialNumber[0] = ECU_SeriesNumber[ecu_sn_len - 4];
    lptr_ReadOutSerialNumber[1] = ECU_SeriesNumber[ecu_sn_len - 3];
    lptr_ReadOutSerialNumber[2] = ECU_SeriesNumber[ecu_sn_len - 2];
    lptr_ReadOutSerialNumber[3] = ECU_SeriesNumber[ecu_sn_len - 1];
#else
        lptr_ReadOutSerialNumber[0] = 0x14;
        lptr_ReadOutSerialNumber[1] = 0x15;
        lptr_ReadOutSerialNumber[2] = 0x16;
        lptr_ReadOutSerialNumber[3] = 0x17;
#endif
        *lptr_ByteNumOfSN = 0x04u;
    }
}

/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
static void UDS_FN_TYPE UdsApp_27SecuAcc_CreateSAL1ConstArray(void)
{
    uint8_t i;
    uint8_t lub_tmpByteNumOfSN;
    /* to read in the serial number value */
    UdsApp_27SecuAcc_ReadSerialNumber(&(rub_ReadOutSerialNumber[0x00u]), &rub_ByteNumOfSN);
    lub_tmpByteNumOfSN = rub_ByteNumOfSN;

    /* create the const array for SAL 1 */
    for (i = 0x04; i > 0x00u; i--)
    {
        if (lub_tmpByteNumOfSN > 0x00u)
        {

            rub_Sal1ConstArray[i - 0x01u] = rub_ReadOutSerialNumber[i - 0x01u];
            lub_tmpByteNumOfSN--;
        }
        else
        {
            rub_Sal1ConstArray[i - 0x01u] = 0x00;
        }
    }
    UdsApp_27SecuAcc_CreateSALnConstArray(&(rub_Sal2ConstArray[0x00u]),
                                          &(rub_Sal1ConstArray[0x00u]));

    UdsApp_27SecuAcc_CreateSALnConstArray(&(rub_Sal3ConstArray[0x00u]),
                                          &(rub_Sal2ConstArray[0x00u]));
}

/*
 * 函数介绍:创建SAL常量数组
 * 参数:
 * @param lptr_SALConstArray 指向新生成的SAL常量数组的指针
 * @param lptr_SALPrevConstArray 指向之前版本的SAL常量数组的指针
 * 返回值: {*}
 * 备注:
 */

static void UDS_FN_TYPE UdsApp_27SecuAcc_CreateSALnConstArray(uint8_t *lptr_SALConstArray,
                                                              uint8_t *lptr_SALPrevConstArray)
{
    /* 遍历数组的索引变量 */
    uint8_t i;
    /* 用于临时存储常量数组值的变量 */
    uint8_t lub_tmpConstArrayValue;
    /* 用于临时存储右移操作结果（不四舍五入）的变量 */
    uint8_t lub_tmpConstRShiftNoRoundValue;
    /* 用于临时存储右移操作结果（四舍五入）的变量 */
    uint8_t lub_tmpConstRShiftRoundValue;

    /* 检查输入指针是否为空，如果为空则直接返回 */
    if((NULL == lptr_SALConstArray) || (NULL == lptr_SALPrevConstArray))
    {
        return;
    }
    else
    {
        /* 遍历前一个常量数组的每个元素，进行位操作并存储到新的常量数组中 */
        for (i = 0x00; i < 0x04u; i++)
        {
            /* 从前一个常量数组中获取当前元素的值 */
            lub_tmpConstArrayValue = lptr_SALPrevConstArray[i];
            /* 通过位操作生成四舍五入后的右移值 */
            lub_tmpConstRShiftRoundValue = (lub_tmpConstArrayValue & 0x07u) << 0x05u;
            /* 通过位操作生成不四舍五入的右移值 */
            lub_tmpConstRShiftNoRoundValue = (lub_tmpConstArrayValue & 0xF8u) >> 0x03u;
            /* 将四舍五入后的右移值和不四舍五入的右移值组合，存储到新的常量数组中 */
            lptr_SALConstArray[i] = (uint8_t)(lub_tmpConstRShiftRoundValue | lub_tmpConstRShiftNoRoundValue);
        }
    }
}
/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
static uint8_t UDS_FN_TYPE UdsApp_27SecuAcc_RandomGenerateSeed(uint8_t i_BytePosition)
{
    uint8_t i;
    uint8_t lub_tmpRandom = 0x00u;
    uint32_t lul_SA_RandomGenerator_Counter = 0x00000000u;

    switch (i_BytePosition)
    {
    case 0x00u:
        lul_SA_RandomGenerator_Counter = rul_SA_RandomGenerator_1_IncreaseCntr;
        break;
    case 0x01u:
        lul_SA_RandomGenerator_Counter = rul_SA_RandomGenerator_2_IncreaseCntr;
        break;
    case 0x02u:
        lul_SA_RandomGenerator_Counter = rul_SA_RandomGenerator_3_DecreaseCntr;
        break;
    case 0x03u:
        lul_SA_RandomGenerator_Counter = rul_SA_RandomGenerator_4_DecreaseCntr;
        break;
    }

    for (i = 0x00; i < 0x04u; i++)
    {
        lub_tmpRandom = (uint8_t)((lul_SA_RandomGenerator_Counter >> (i * 0x08u)) & 0xFFu);
        if (lub_tmpRandom != 0x00u && lub_tmpRandom != 0xFFu)
        {
            break;
        }
        else
        {
        }
    }

    if (lub_tmpRandom == 0x00u ||
        lub_tmpRandom == 0xFFu)
    {
        switch (i_BytePosition)
        {
        default:
            lub_tmpRandom = 0x42u;
            break;
        case 0x00u:
            lub_tmpRandom = 0x53u;
            break;
        case 0x01u:
            lub_tmpRandom = 0x6Au;
            break;
        case 0x02u:
            lub_tmpRandom = 0x91u;
            break;
        case 0x03u:
            lub_tmpRandom = 0x8Du;
            break;
        }
    }
    else
    {
    }

    return lub_tmpRandom;
}
/*
 * 函数介绍: 对给定的数据进行加密算法操作
 * 参数:
 * @param i_num_rounds 加密轮数，决定了加密的强度。
 * @param iPtr_v 数据输入指针，指向需要加密的數據。
 * @param iPtr_k 密钥输入指针，指向加密使用的密钥。
 * 返回值: {*}
 * 备注:
 */
static void UDS_FN_TYPE UdsApp_27SecuAcc_GenericEncryptAlgorithm(uint32_t i_num_rounds,
                                                                 uint32_t *iPtr_v,
                                                                 uint32_t *iPtr_k)
{
    /* 初始化循环变量 */
    uint32_t i;
    /* 从输入指针读取初始数据 */
    uint32_t v0 = iPtr_v[0x00u];
    uint32_t v1 = iPtr_v[0x01u];
    /* 初始化累加器sum和增量delta */
    uint32_t sum = 0x00000000u;
    uint32_t delta = 0x9E3779B9u; // just a example

    /* 检查密钥指针是否为空，若为空则直接返回 */
    if ((NULL == iPtr_k) || (NULL == iPtr_k))
    {
       return;
    }
    else
    {
        /* 对数据进行加密操作，循环i_num_rounds轮 */
        for (i = 0x00000000u; i < i_num_rounds; i++)
        {
            /* 执行加密算法的第一部分 */
            v0 += (((v1 << 0x04u) ^ (v1 >> 0x05u)) + v1) ^ (sum + k[sum & 0x03u]);
            /* 更新累加器sum */
            sum += delta;
            /* 执行加密算法的第二部分 */
            v1 += (((v0 << 0x04u) ^ (v0 >> 0x05u)) + v0) ^ (sum + k[((sum >> 11u) & 0x03u)]);
        }
        /* 将加密后的数据写回原始指针位置 */
        iPtr_v[0x00u] = v0;
        iPtr_v[0x01u] = v1;
    }
}
/*
 * 函数介绍: NIO安全认证CRC8算法
 * 参数:
 * @param data校验数据
 * @param length校验数据的长度
 * 返回值: {*}
 * 备注:function of calculate the Key
 */
static unsigned char ASAP1A_CCP_crc8(unsigned char *data, int length)
{
	unsigned char t_crc;
	int f, b;
	t_crc = 0xFF;
	for (f = 0; f < length; f++)
	{
		t_crc ^= data[f];
		for (b = 0; b < 8; b++)
		{
			if ((t_crc & 0x80) != 0)
			{
				t_crc <<= 1;
				t_crc ^= 0x1D;
			}
			else
			{
				t_crc <<= 1;
			}
		}
	}
	return ~t_crc;
}
/*
 * 函数介绍: NIO安全认证算法
 * 参数:
 * @param seed 请求种子
 * @param key 计算得到的密钥指针。
 * @param sizeKey 密钥大小的指针。
 * 返回值: {*}
 * 备注:
 */
static uint8_t  ASAP1A_CCP_ComputeKeyFromSeed(char *seed1,char * key,unsigned short *sizeKey)
{
	int seedlength = 6;
	unsigned char seed[6] = {0};
	unsigned char buf_byte[6];
	unsigned char crc_byte[7];

	memcpy(seed,seed1,4);

#ifdef UdsApi_BTA_R_FUN_EN
	seed[4] = (uint8_t)(DIAG_REQ_BTA_R>>8);		//high byte of secret code(example)
	seed[5] = (uint8_t)DIAG_REQ_BTA_R;			//low byte of secret code(example)
#endif
#ifdef UdsApi_BTA_L_FUN_EN
	seed[4] = (uint8_t)(DIAG_REQ_BTA_L>>8);		//high byte of secret code(example)
	seed[5] = (uint8_t)DIAG_REQ_BTA_L;			//low byte of secret code(example)
#endif
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
if(Get_BTAPos() == External_Rear)
{
	seed[4] = (uint8_t)(DIAG_REQ_BTA_RB>>8);	//high byte of secret code(example)
	seed[5] = (uint8_t)DIAG_REQ_BTA_RB;		//low byte of secret code(example)
}
else if(Get_BTAPos() == External_Front)
{
	seed[4] = (uint8_t)(DIAG_REQ_BTA_FB>>8);	//high byte of secret code(example)
	seed[5] = (uint8_t)DIAG_REQ_BTA_FB;			//low byte of secret code(example)
}
else
{}
#endif
#ifdef UdsApi_BTA_SRF_FUN_EN
	seed[4] = (uint8_t)(DIAG_REQ_BTA_SRF>>8);	//high byte of secret code(example)
	seed[5] = (uint8_t)DIAG_REQ_BTA_SRF;		//low byte of secret code(example)
#endif
#ifdef UdsApi_BTA_SRR_FUN_EN
	seed[4] = (uint8_t)(DIAG_REQ_BTA_SRR>>8);	//high byte of secret code(example)
	seed[5] = (uint8_t)DIAG_REQ_BTA_SRR;		//low byte of secret code(example)
#endif
#ifdef UdsApi_BTA_RB2_FUN_EN
	seed[4] = (uint8_t)(DIAG_REQ_BTA_RB2>>8);	//high byte of secret code(example)
	seed[5] = (uint8_t)DIAG_REQ_BTA_RB2;			//low byte of secret code(example)
#endif
	buf_byte[0] = seed[0];
	buf_byte[1] = seed[1];
	buf_byte[2] = seed[2];
	buf_byte[3] = seed[3];
	buf_byte[4] = seed[4];
	buf_byte[5] = seed[5];

	crc_byte[0] = ASAP1A_CCP_crc8(buf_byte,seedlength);
	buf_byte[0] = crc_byte[0];
	crc_byte[1] = ASAP1A_CCP_crc8(buf_byte,seedlength);
	buf_byte[0] = seed[0];
	buf_byte[1] = crc_byte[1];
	crc_byte[2] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	buf_byte[1] = seed[1];
	buf_byte[2] = crc_byte[2];
	crc_byte[3] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	buf_byte[2] = seed[2];
	buf_byte[3] = crc_byte[3];
	crc_byte[4] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	buf_byte[3] = seed[3];
	buf_byte[4] = crc_byte[4];
	crc_byte[5] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	buf_byte[4] = seed[4];
	buf_byte[5] = crc_byte[5];
	crc_byte[6] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	if (crc_byte[3] == 0 && crc_byte[4] == 0 && crc_byte[5] == 0 && crc_byte[6] == 0)
	{
		key[0] = crc_byte[1];
		key[1] = crc_byte[2];
		key[2] = crc_byte[3];
		key[3] = crc_byte[4];
	}else
	{
		key[0] = crc_byte[3];
		key[1] = crc_byte[4];
		key[2] = crc_byte[5];
		key[3] = crc_byte[6];
	}
	*sizeKey = 4;
	// If the return value is false the flash tool stops
	return 1;
}
/*
 * 函数介绍: NIO安全认证算法
 * 参数:
 * @param seed 请求种子
 * @param key 计算得到的密钥指针。
 * @param sizeKey 密钥大小的指针。
 * 返回值: {*}
 * 备注:
 */
static uint8_t  ASAP1A_CCP_ComputeKeyFromSeed_Internally(char *seed1,char * key,unsigned short *sizeKey)
{
	int seedlength = 6;
	unsigned char seed[6] = {0};
	unsigned char buf_byte[6];
	unsigned char crc_byte[7];
	memcpy(seed,seed1,4);
	seed[4] = 0xA5;	//high byte of secret code(example)
	seed[5] = 0xA5;		//low byte of secret code(example)
	buf_byte[0] = seed[0];
	buf_byte[1] = seed[1];
	buf_byte[2] = seed[2];
	buf_byte[3] = seed[3];
	buf_byte[4] = seed[4];
	buf_byte[5] = seed[5];
	crc_byte[0] = ASAP1A_CCP_crc8(buf_byte,seedlength);
	buf_byte[0] = crc_byte[0];
	crc_byte[1] = ASAP1A_CCP_crc8(buf_byte,seedlength);
	buf_byte[0] = seed[0];
	buf_byte[1] = crc_byte[1];
	crc_byte[2] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	buf_byte[1] = seed[1];
	buf_byte[2] = crc_byte[2];
	crc_byte[3] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	buf_byte[2] = seed[2];
	buf_byte[3] = crc_byte[3];
	crc_byte[4] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	buf_byte[3] = seed[3];
	buf_byte[4] = crc_byte[4];
	crc_byte[5] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	buf_byte[4] = seed[4];
	buf_byte[5] = crc_byte[5];
	crc_byte[6] = ASAP1A_CCP_crc8(buf_byte, seedlength);
	if (crc_byte[3] == 0 && crc_byte[4] == 0 && crc_byte[5] == 0 && crc_byte[6] == 0)
	{
		key[0] = crc_byte[1];
		key[1] = crc_byte[2];
		key[2] = crc_byte[3];
		key[3] = crc_byte[4];
	}else
	{
		key[0] = crc_byte[3];
		key[1] = crc_byte[4];
		key[2] = crc_byte[5];
		key[3] = crc_byte[6];
	}
	*sizeKey = 4;
	// If the return value is false the flash tool stops
	return 1;
}
/*
 * 函数介绍: 哪吒安全算法
 * 参数:
 * 返回值: {*}
 * 备注:
 */
static uint32_t canculate_security_access_bcm(uint32_t seed, uint32_t APP_MASK1)
{
    uint32_t tmpseed = seed;
    uint32_t key_2;
    uint32_t key;
    uint32_t key_1 = tmpseed ^ APP_MASK1;
    uint32_t seed_2 = tmpseed;

    seed_2 = (seed_2 & 0x55555555) << 1 ^ (seed_2 & 0xAAAAAAAA) >> 1;
    seed_2 = (seed_2 ^ 0x33333333) << 2 ^ (seed_2 ^ 0xCCCCCCCC) >> 2;
    seed_2 = (seed_2 & 0x0F0F0F0F) << 4 ^ (seed_2 & 0xF0F0F0F0) >> 4;
    seed_2 = (seed_2 ^ 0x00FF00FF) << 8 ^ (seed_2 ^ 0xFF00FF00) >> 8;
    seed_2 = (seed_2 & 0x0000FFFF) << 16 ^ (seed_2 & 0xFFFF0000) >> 16;
    key_2 = seed_2;
    key = key_1 + key_2;
    return key;
}
/*
 * 函数介绍: 使用给定的种子计算安全访问密钥
 * 参数:
 * @param lpub_seed 公钥种子，一个指向4字节数据的指针。
 * @param lpub_key 公钥，一个指向至少4字节空间的可写指针。
 * 返回值: {*}
 * 备注:
 */
static void UDS_FN_TYPE UdsApp_27SecuAcc_EncFrmSeed1Key_SAL1(const uint8_t *lpub_seed, uint8_t *lpub_key)
{
#if 0
    /* 定义一个应用特定的掩码，用于密钥计算 */
    const uint32_t APP_MASK = 0x90F3E9BC;
#endif
    /* 静态变量key用于存储计算出的密钥 */
    static uint8_t key[4] = {0};
    uint16_t key_size;
    /* send变量用于临时存储种子值的32位表示 */
    uint8_t send[6] = {0};

    /* 检查输入指针是否为NULL，如果是，则直接返回 */
    if (lpub_seed == NULL || lpub_key == NULL)
    {
        return;
    }
    else
    {
        /* 将种子值的4个字节组合成一个32位无符号整数 */
        memcpy(send,lpub_seed,4);
        /* 使用预定义的掩码和种子值计算密钥 */
        ASAP1A_CCP_ComputeKeyFromSeed(send, key,&key_size);
#if 0
        /* 将计算出的密钥的4个字节分别存储在公钥数组的前4个位置 */
        lpub_key[0x00u] = (uint8_t)((key >> 24) & 0xFF);
        lpub_key[0x01u] = (uint8_t)((key >> 16) & 0xFF);
        lpub_key[0x02u] = (uint8_t)((key >> 8) & 0xFF);
        lpub_key[0x03u] = (uint8_t)((key >> 0) & 0xFF);
#endif
        /* 将计算出的密钥的4个字节分别存储在公钥数组的前4个位置 */
	    lpub_key[0x00u] = key[0];
	    lpub_key[0x01u] = key[1];
	    lpub_key[0x02u] = key[2];
	    lpub_key[0x03u] = key[3];
    }
}

/*
 * 函数介绍:使用给定的公钥种子生成公钥
 * 参数:
 * @param lpub_seed 公钥种子，输入参数，不能为空
 * @param lpub_key 生成的公钥，输出参数，不能为空
 * 返回值: {*}
 * 备注:
 */

static void UDS_FN_TYPE UdsApp_27SecuAcc_EncFrmSeed2Key_SAL2(const uint8_t *lpub_seed, uint8_t *lpub_key)
{
    /* 检查输入参数是否为空 */
    if (lpub_seed == NULL || lpub_key == NULL)
    {
        return;
    }
    else
    {
        /* 根据预定义的 FrozenSeed 数组初始化变量 v 的第一个元素 */
        v[0x00u] = (uint32_t)(raub_27SecuAcc_FrozenSeed[0x00u] << 24u) |
               (uint32_t)(raub_27SecuAcc_FrozenSeed[0x01u] << 16u) |
               (uint32_t)(raub_27SecuAcc_FrozenSeed[0x02u] << 8u) |
               (uint32_t)(raub_27SecuAcc_FrozenSeed[0x03u]);

        /* 根据预定义的 FrozenSeed 数组初始化变量 v 的第二个元素，元素值为 FrozenSeed 的按位取反 */
        v[0x01u] = (uint32_t)(((~(raub_27SecuAcc_FrozenSeed[0x00u])) << 24u) & 0xff000000) |
               (uint32_t)(((~(raub_27SecuAcc_FrozenSeed[0x01u])) << 16u) & 0x00ff0000) |
               (uint32_t)(((~(raub_27SecuAcc_FrozenSeed[0x02u])) << 8u) & 0x0000ff00) |
               (uint32_t)(((~(raub_27SecuAcc_FrozenSeed[0x03u])) & 0x000000ff));

        /* 根据预定义的 Sal2ConstArray 数组初始化加密关键字 k */
        k[0x00u] = (uint32_t)(rub_Sal2ConstArray[0x00u] & 0xFFu);
        k[0x01u] = (uint32_t)(rub_Sal2ConstArray[0x01u] & 0xFFu);
        k[0x02u] = (uint32_t)(rub_Sal2ConstArray[0x02u] & 0xFFu);
        k[0x03u] = (uint32_t)(rub_Sal2ConstArray[0x03u] & 0xFFu);

        /* 调用通用加密算法，使用关键字 k 对变量 v 进行加密 */
        UdsApp_27SecuAcc_GenericEncryptAlgorithm(0x02u, &(v[0x00u]), &(k[0x00u]));

        /* 将加密后的结果 v 的第一个元素转换为公钥 */
        lpub_key[0x00u] = (uint8_t)((v[0x00u] & 0xFF000000u) >> 24u);
        lpub_key[0x01u] = (uint8_t)((v[0x00u] & 0x00FF0000u) >> 16u);
        lpub_key[0x02u] = (uint8_t)((v[0x00u] & 0x0000FF00u) >> 8u);
        lpub_key[0x03u] = (uint8_t)((v[0x00u] & 0x000000FFu));
    }
}
/*
 * 函数介绍:使用给定的公钥种子生成公钥
 * 参数:
 * @param lpub_seed 公钥种子，作为生成公钥的输入。
 * @param lpub_key 用于存储生成的公钥的缓冲区。
 * 返回值: {*}
 * 备注:
 */
static void UDS_FN_TYPE UdsApp_27SecuAcc_EncFrmSeed3Key_SAL3(const uint8_t *lpub_seed, uint8_t *lpub_key)
{
    /* 检查输入参数是否为空 */
    if (lpub_seed == NULL || lpub_key == NULL)
    {
        return;
    }
    else
    {
        /* 根据输入的公钥种子生成加密所需的初始值v */
        v[0x00u] = (uint32_t)(raub_27SecuAcc_FrozenSeed[0x00u] << 24u) |
                   (uint32_t)(raub_27SecuAcc_FrozenSeed[0x01u] << 16u) |
                   (uint32_t)(raub_27SecuAcc_FrozenSeed[0x02u] << 8u) |
                   (uint32_t)(raub_27SecuAcc_FrozenSeed[0x03u]);

        v[0x01u] = (uint32_t)(((~(raub_27SecuAcc_FrozenSeed[0x00u])) << 24u) & 0xff000000) |
                   (uint32_t)(((~(raub_27SecuAcc_FrozenSeed[0x01u])) << 16u) & 0x00ff0000) |
                   (uint32_t)(((~(raub_27SecuAcc_FrozenSeed[0x02u])) << 8u) & 0x0000ff00) |
                   (uint32_t)(((~(raub_27SecuAcc_FrozenSeed[0x03u])) & 0x000000ff));

        /* 使用常量数组rub_Sal3ConstArray的值初始化加密所需的密钥k */
        k[0x00u] = (uint32_t)(rub_Sal3ConstArray[0x00u] & 0xFFu);
        k[0x01u] = (uint32_t)(rub_Sal3ConstArray[0x01u] & 0xFFu);
        k[0x02u] = (uint32_t)(rub_Sal3ConstArray[0x02u] & 0xFFu);
        k[0x03u] = (uint32_t)(rub_Sal3ConstArray[0x03u] & 0xFFu);

        /* 调用通用加密算法，使用密钥k对初始值v进行加密 */
        UdsApp_27SecuAcc_GenericEncryptAlgorithm(0x02u, &(v[0x00u]), &(k[0x00u]));

        /* 将加密后的值转换为公钥，并存储到提供的公钥缓冲区中 */
        lpub_key[0x00u] = (uint8_t)((v[0x00u] & 0xFF000000u) >> 24u);
        lpub_key[0x01u] = (uint8_t)((v[0x00u] & 0x00FF0000u) >> 16u);
        lpub_key[0x02u] = (uint8_t)((v[0x00u] & 0x0000FF00u) >> 8u);
        lpub_key[0x03u] = (uint8_t)((v[0x00u] & 0x000000FFu));
    }
}

/*
 * 函数介绍: 获取安全状态等级
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_FN_TYPE UdsApp_27SecuAcc_GetCurrSecurityLevelStatus(uint8_t i_lub_SAL2Read)
{
    uint8_t lub_CurrSecurityLevel = UDS_C_SECU_ACC_SECU_LEVEL1_LOCKED;
    switch (i_lub_SAL2Read)
    {
    default:
        break;
    case UDS_C_SECU_ACC_SECU_LEVEL1:
        lub_CurrSecurityLevel = rub_27SecuAcc_SecurityLvl_1;
        break;
    case UDS_C_SECU_ACC_SECU_LEVEL2:
        lub_CurrSecurityLevel = rub_27SecuAcc_SecurityLvl_2;
        break;
    case UDS_C_SECU_ACC_SECU_LEVEL3:
        lub_CurrSecurityLevel = rub_27SecuAcc_SecurityLvl_3;
        break;
    case UDS_C_SECU_ACC_SECU_LEVEL4:
        lub_CurrSecurityLevel = rub_27SecuAcc_SecurityLvl_4;
        break;
    }
    return lub_CurrSecurityLevel;
}

#endif /* UDS_ENABLE_SECURITYACCESS */
/******************************************************************************
 *                               end of file                                   *
 ******************************************************************************/
