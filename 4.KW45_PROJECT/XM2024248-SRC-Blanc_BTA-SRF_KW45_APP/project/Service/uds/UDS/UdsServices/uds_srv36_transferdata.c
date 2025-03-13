/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv36_transferdata.c
 * 摘要: UDS Application Layer service 0x36 TransferData.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "uds_confdefs.h"
#include "uds_confpriv.h"
#include "uds_inf.h"
#include "uds_priv.h"
#include "uds_pub.h"
#include "uds_target.h"

#ifdef UDS_ENABLE_TRANSFERDATA
static UDS_TransferDatState_t rub_TransferDatState = UDS_C_TANSFER_DAT_ST_INIT;

/* the server expected next Block Sequence counter
 */
static uint8_t rub_36TransferDat_PendingFlg;
// static uint8_t rub_36TransferDat_CurrUsingTblIndex ;

static uint8_t rub_DnldDatSvcJust1stTrsfrDatFlg = (uint8_t)FALSE;

static uint8_t rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt = 0x00u;
static uint8_t rub_DnldDatSvrCurrRcvdBlkSeqCntrFrmClnt = 0x00u;
static uint8_t rub_DnldDatSvrPrevRcvdOkBlkSeqCntrFrmClnt = 0x00u;

static uint32_t rul_TrnsfDatCurrMemAdd2Wrt = 0x00000000u;
static uint32_t rul_TrnsfDatTotalMemSize2Wrt = 0x00000000u;
static uint32_t rul_TrnsfDatCurrAlrdyRcvdMemSize = 0x00000000u;

static uint8_t rub_UpldDatSvcJust1stTrsfrDatFlg = (uint8_t)FALSE;

static uint8_t rub_UpldDatSvrExpctdNxtBlkSeqCntrFrmClnt = 0x00u;
static uint8_t rub_UpldDatSvrCurrTsfrdBlkSeqCntrFrmClnt = 0x00u;
static uint8_t rub_UpldDatSvrPrevTsfrdOkBlkSeqCntrFrmClnt = 0x00u;

static uint32_t rul_TrnsfDatCurrMemAdd2Rd = 0x00000000u;
static uint32_t rul_TrnsfDatTotalMemSize2Rd = 0x00000000u;

static uint32_t rul_TrnsfDatCurrAlrdyTsfrdMemSize = 0x00000000u;
static uint32_t rul_TrnsfDatPrevTsfrdMemSize = 0x00000000u;

static uint16_t ruw_36TransferDatCurrReqMsgLen = 0x0000u;

uint8_t rub_ReqDnUpldDataBuf[UDS_C_REQ_DN_OR_UP_LD_BUF_SIZE];

static Std_UdsReturnType UDS_FN_TYPE UdsApp_36TransferDataReqDnldProc(Uds_APduData_t *pAPduInd,
                                                                      Uds_APduData_t *pAPduResp);

static Std_UdsReturnType UDS_FN_TYPE UdsApp_36TransferDataReqUpldProc(Uds_APduData_t *pAPduInd,
                                                                      Uds_APduData_t *pAPduResp);

/* no Sun-Function parameter for this service */

/*
 * 函数介绍: 传输数据
 * 参数:
 * pAPduInd  : input parameters.
   pAPduResp : output parameters
 * 返回值: {*}
 * 备注: NRC for ReadDataByIdentifier
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 * 	0x31	UDS_NRC_REQUESTOUTOFRANGE
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_TransferData(Uds_APduData_t *pAPduInd,
                                                  Uds_APduData_t *pAPduResp)
{
    uint8_t lub_error = UDS_POSITIVE_RESPONSE;

    if ((NULL == pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NRC_GENERALREJECT;
    }
    else
    {
        /* code */
    }
    /* uint8_t lub_PrevReqDnldTblIdx = UdsApp_34ReqDnld_GetCurrDnldTblIdx();  */
    uint32_t lul_34ReqDnldMemSize = UdsApp_34ReqDnld_GetCurrDnldMemSize();
#ifdef UDS_ENABLE_REQUESTUPLOAD
    uint32_t lul_35ReqUpldMemSize = UdsApp_35ReqUpld_GetCurrUpldMemSize();
#else
    uint32_t lul_35ReqUpldMemSize = 0;
#endif
    rul_TrnsfDatTotalMemSize2Wrt = UdsApp_34ReqDnld_GetCurrDnldMemSize();
#ifdef UDS_ENABLE_REQUESTUPLOAD
    rul_TrnsfDatTotalMemSize2Rd = UdsApp_35ReqUpld_GetCurrUpldMemSize();
#endif
    if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
    {
        lub_error = UDS_NRC_SERVICENOTSUPPORTED;
    }
    else if (UDS_APP_ITF_IMP_GET_ECU_OVR_VOLT_FLG()) /* to get the overvoltage flag */
    {
        lub_error = UDS_NRC_VOLTAGETOOHIGH;
    }
    else if (UDS_APP_ITF_IMP_GET_ECU_UDR_VOLT_FLG()) /* to get the undervoltage flag */
    {
        lub_error = UDS_NRC_VOLTAGETOOLOW;
    }
    else if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
    {
        /* if this request is received from the functional addressing channel
           reject it.
         */
        lub_error = UDS_NRC_SERVICENOTSUPPORTED;
    }
    else if ((UDS_DEFAULT_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION()) ||
             (UDS_EXTENDED_DIAGNOSTIC_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION()))
    {
        /* if the current active session is still default session ,
           reject it.
         */
        lub_error = UDS_NRC_SERVICENOTSUPPORTEDINACTIVESESSION;
    }
    else if (rub_TransferDatState != UDS_C_TANSFER_DAT_ST_DNLD &&
             rub_TransferDatState != UDS_C_TANSFER_DAT_ST_UPLD)
    {
        lub_error = UDS_NRC_REQUESTSEQUENCEERROR;
    }
    else if (((UDS_C_TANSFER_DAT_ST_DNLD == rub_TransferDatState) &&
              (rul_TrnsfDatCurrAlrdyRcvdMemSize == lul_34ReqDnldMemSize)) ||
             ((UDS_C_TANSFER_DAT_ST_UPLD == rub_TransferDatState) &&
              (rul_TrnsfDatCurrAlrdyTsfrdMemSize == lul_35ReqUpldMemSize)))
    {
        lub_error = UDS_NRC_REQUESTSEQUENCEERROR;
    }
    else if ((UDS_C_TANSFER_DAT_ST_DNLD == rub_TransferDatState) &&
             (pAPduInd->msgLength < 0x03u ||
              pAPduInd->msgLength > (UDS_C_34REQDNLD_USING_NUM_BLK_LEN + 0x02u) ||
              ((rul_TrnsfDatCurrAlrdyRcvdMemSize + pAPduInd->msgLength - 0x02u) >
               lul_34ReqDnldMemSize)))
    {
        lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    else if (rub_TransferDatState == UDS_C_TANSFER_DAT_ST_UPLD &&
             pAPduInd->msgLength != 0x02u)
    {
        lub_error = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }

    else if ((UDS_C_TANSFER_DAT_ST_DNLD == rub_TransferDatState) &&
             ((uint8_t)FALSE == rub_DnldDatSvcJust1stTrsfrDatFlg) &&
             (pAPduInd->pduData[0x01u] == rub_DnldDatSvrPrevRcvdOkBlkSeqCntrFrmClnt))
    {
        /* if a repeated block sequence counter is received from client side ,
           then to return the positive response directly
         */
        /* lub_error =  UDS_POSITIVE_RESPONSE; */
        pAPduResp->msgLength = 0x02u;
    }
    else if ((UDS_C_TANSFER_DAT_ST_UPLD ==rub_TransferDatState)  &&
             ((uint8_t)FALSE == rub_UpldDatSvcJust1stTrsfrDatFlg)  &&
             (pAPduInd->pduData[0x01u] == rub_UpldDatSvrPrevTsfrdOkBlkSeqCntrFrmClnt))
    {
        /* if a repeated block sequence counter is received from client side ,
           then to return the positive response directly
         */
        /* lub_error =  UDS_POSITIVE_RESPONSE; */
        pAPduResp->msgLength = 0x02u;
    }
    else
    {
        if (UDS_C_TANSFER_DAT_ST_DNLD == rub_TransferDatState )
        {
            if ((uint8_t)TRUE == rub_DnldDatSvcJust1stTrsfrDatFlg)
            {
                if (pAPduInd->pduData[0x01u] != rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt)
                {
                    lub_error = UDS_NRC_WRONGBLOCKSEQUENCECOUNTER;
                }
                else
                {
                }
            }
            else
            {
                if (pAPduInd->pduData[0x01u] != rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt &&
                    pAPduInd->pduData[0x01u] != rub_DnldDatSvrPrevRcvdOkBlkSeqCntrFrmClnt)
                {
                    lub_error = UDS_NRC_WRONGBLOCKSEQUENCECOUNTER;
                }
                else
                {
                }
            }

            if (UDS_POSITIVE_RESPONSE == lub_error)
            {
                rub_DnldDatSvrCurrRcvdBlkSeqCntrFrmClnt = pAPduInd->pduData[0x01u];
                lub_error = UdsApp_36TransferDataReqDnldProc(pAPduInd,
                                                             pAPduResp);
            }
            else
            {
            }
        }
        else if (UDS_C_TANSFER_DAT_ST_UPLD == rub_TransferDatState)
        {
            if ((uint8_t)TRUE == rub_UpldDatSvcJust1stTrsfrDatFlg)
            {
                if (pAPduInd->pduData[0x01u] != rub_UpldDatSvrExpctdNxtBlkSeqCntrFrmClnt)
                {
                    lub_error = UDS_NRC_WRONGBLOCKSEQUENCECOUNTER;
                }
                else
                {
                }
            }
            else
            {
                if (pAPduInd->pduData[0x01u] != rub_UpldDatSvrExpctdNxtBlkSeqCntrFrmClnt &&
                    pAPduInd->pduData[0x01u] != rub_UpldDatSvrPrevTsfrdOkBlkSeqCntrFrmClnt)
                {
                    lub_error = UDS_NRC_WRONGBLOCKSEQUENCECOUNTER;
                }
                else
                {
                }
            }

            if (UDS_POSITIVE_RESPONSE == lub_error)
            {
                rub_UpldDatSvrCurrTsfrdBlkSeqCntrFrmClnt = pAPduInd->pduData[0x01u];
                lub_error = UdsApp_36TransferDataReqUpldProc(pAPduInd,
                                                             pAPduResp);
            }
            else
            {
            }
        }
        else
        {
        }
    }

    return lub_error;

} /* end of UdsApp_TransferData() */
/*
 * 函数介绍:传输数据接口
 * 参数:
 * pAPduInd  : input parameters.
   pAPduResp : output parameters
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE UdsApp_36TransferDataReqDnldProc(Uds_APduData_t *pAPduInd,
                                                                      Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response = UDS_POSITIVE_RESPONSE;
    uint8_t lub_DnldChkWrtResult = (uint8_t)FALSE;
    /* uint8_t lub_PrevReqDnldTblIdx = UdsApp_34ReqDnld_GetCurrDnldTblIdx();  */
    uint32_t lub_DataLen2Write = (pAPduInd->msgLength - 0x02u);
    uint16_t lub_DataPadLen = 0x0000u;
    uint8_t i = 0x00u;

    if ((NULL == pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NRC_GENERALREJECT;
    }
    else
    {
        /* code */
    }
    if (rul_TrnsfDatCurrMemAdd2Wrt >= 0x40000000u &&
        rul_TrnsfDatCurrMemAdd2Wrt <= 0x400177ffu)
    {
        /* this means the downloading target address is in the RAM range */
        Uds_MemCopy((uint8_t *)rul_TrnsfDatCurrMemAdd2Wrt,    /* pDest */
                    &(pAPduInd->pduData[0x02u]),              /* pSrc */
                    (uint32_t)(pAPduInd->msgLength - 0x02u)); /* numBytes */

        ruw_36TransferDatCurrReqMsgLen = pAPduInd->msgLength;

        rub_DnldDatSvcJust1stTrsfrDatFlg = (uint8_t)FALSE;
        rul_TrnsfDatCurrMemAdd2Wrt += (pAPduInd->msgLength - 0x02u);
        rul_TrnsfDatCurrAlrdyRcvdMemSize += (pAPduInd->msgLength - 0x02u);
        rub_DnldDatSvrPrevRcvdOkBlkSeqCntrFrmClnt = rub_DnldDatSvrCurrRcvdBlkSeqCntrFrmClnt;
        if (rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt < 0xFFu)
        {
            rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt++;
        }
        else
        {
            rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt = 0x00u;
        }
        pAPduResp->msgLength = 0x02u;
    } /* end of if ( add in the range of ram ) */
    else if (rul_TrnsfDatCurrMemAdd2Wrt >= 0x00000000u &&
             rul_TrnsfDatCurrMemAdd2Wrt <= 0x0017FFFFu)
    {
        /* this means the downloading target address is in the FLASH range */
        /* step1 : to copy all of data from the request buffer into
                           the receiving data buffer of transfer data .
         */
        Uds_MemCopy(UDS_ITF_IMP_DPTR_REQ_DN_ANY_ADDR, /* Uds_g_ReqDnldMngTbl[lub_PrevReqDnldTblIdx].pDataBuf, */ /* pDest */
                    &(pAPduInd->pduData[0x02u]),                                                                 /* pSrc */
                    (uint32_t)(pAPduInd->msgLength - 0x02u));                                                    /* numBytes */
        /* to make sure the data should be 8 bytes aligned
         */
        if (UDS_C_REQ_DN_OR_UP_LD_BUF_SIZE == lub_DataLen2Write)
        {
            /* this means this is a complete block of 64 bytes , has aligned , no need padding */
        }
        else /* if (  lub_DataLen2Write < UDS_C_REQ_DN_OR_UP_LD_BUF_SIZE)   */
        {
            if ((uint8_t)(lub_DataLen2Write & 0x07u))
            {
                lub_DataPadLen = (uint16_t)(0x08u - (uint8_t)(lub_DataLen2Write & 0x07u));
                for (i = 0x00u; i < lub_DataPadLen; i++)
                {
                    UDS_ITF_IMP_DPTR_REQ_DN_ANY_ADDR[lub_DataLen2Write + i] = 0xFFu;
                }
                lub_DataLen2Write += lub_DataPadLen;
            }
            else /* if (!(lub_DataLen2Write & 0x07u))  */
            {
                /* this means this block is less than 64 bytes, and also 8 bytes aligned , no need padding */
            }
        } /* end of else  // if (  lub_DataLen2Write < UDS_C_REQ_DN_OR_UP_LD_BUF_SIZE)   */
        /* step2 : to call the flash or eeprom driver interface to write data

         */
        /*
         lub_DnldChkWrtResult = (*(Uds_g_ReqDnldMngTbl[lub_PrevReqDnldTblIdx].pFuncPtrDnldWrtData))((uint32_t *)rul_TrnsfDatCurrMemAdd2Wrt,
                                                                                                    lub_DataLen2Write,
                                                                                                    (uint32_t *)(Uds_g_ReqDnldMngTbl[lub_PrevReqDnldTblIdx].pDataBuf));
         */
        lub_DnldChkWrtResult = UDS_ITF_IMP_DNLD_WT_DAT_FP_ANY_ADDR((uint32_t *)rul_TrnsfDatCurrMemAdd2Wrt,
                                                                   lub_DataLen2Write,
                                                                   (uint32_t *)UDS_ITF_IMP_DPTR_REQ_DN_ANY_ADDR); /* (uint32_t *)(Uds_g_ReqDnldMngTbl[lub_PrevReqDnldTblIdx].pDataBuf)); */

        if ((uint8_t)TRUE == lub_DnldChkWrtResult)
        {
            ruw_36TransferDatCurrReqMsgLen = pAPduInd->msgLength;
            rub_DnldDatSvcJust1stTrsfrDatFlg = (uint8_t)FALSE;
            rul_TrnsfDatCurrMemAdd2Wrt += (pAPduInd->msgLength - 0x02u);
            rul_TrnsfDatCurrAlrdyRcvdMemSize += (pAPduInd->msgLength - 0x02u);
            rub_DnldDatSvrPrevRcvdOkBlkSeqCntrFrmClnt = rub_DnldDatSvrCurrRcvdBlkSeqCntrFrmClnt;
            if (rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt < 0xFFu)
            {
                rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt++;
            }
            else
            {
                rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt = 0x00u;
            }
            pAPduResp->msgLength = 0x02u;
        }
        else
        {
            lub_response = UDS_NRC_GENERALPROGRAMMINGFAILURE;
        }
    } /* end of else if ( add in the range of flash ) */
    else
    {
        /* all of the other address is not allowed to download */
        lub_response = UDS_NRC_GENERALPROGRAMMINGFAILURE;
    }
    return lub_response;
}
/*
 * 函数介绍: 传输数据子接口
 * 参数:
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE UdsApp_36TransferDataReqUpldProc(Uds_APduData_t *pAPduInd,
                                                                      Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response = UDS_POSITIVE_RESPONSE;
    uint8_t lub_UpldChkRdResult = (uint8_t)FALSE;
    // uint32_t  lub_DataLen2Rd          ; // =  (pAPduInd->msgLength - 0x02u);
    uint16_t lub_DataPadLen = 0x0000u;
    uint8_t i = 0x00u;

    if ((NULL == pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NRC_GENERALREJECT;
    }
    else
    {
        /* code */
    }

    if (rul_TrnsfDatTotalMemSize2Rd > rul_TrnsfDatCurrAlrdyTsfrdMemSize)
    {
        if ((rul_TrnsfDatTotalMemSize2Rd - rul_TrnsfDatCurrAlrdyTsfrdMemSize) >= (UDS_C_REQ_DN_OR_UP_LD_BUF_SIZE - 0x02u))
        {
            pAPduResp->msgLength = (UDS_C_REQ_DN_OR_UP_LD_BUF_SIZE);
        }
        else
        {
            pAPduResp->msgLength = (rul_TrnsfDatTotalMemSize2Rd - rul_TrnsfDatCurrAlrdyTsfrdMemSize);
        }

        Uds_MemCopy(&(pAPduInd->pduData[0x02u]),                                                /* pDest */
                    (uint8_t *)(rul_TrnsfDatCurrMemAdd2Rd + rul_TrnsfDatCurrAlrdyTsfrdMemSize), /* pSrc */
                    (uint32_t)(pAPduResp->msgLength - 0x02u));                                  /* numBytes */

        if ((pAPduInd->pduData[0x01u]) == rub_UpldDatSvrExpctdNxtBlkSeqCntrFrmClnt)
        {
            rul_TrnsfDatCurrAlrdyTsfrdMemSize += rul_TrnsfDatPrevTsfrdMemSize; // (pAPduResp->msgLength - 0x02u)
            if (rub_UpldDatSvrExpctdNxtBlkSeqCntrFrmClnt < 0xFFu)
            {
                rub_UpldDatSvrExpctdNxtBlkSeqCntrFrmClnt++;
            }
            else
            {
                rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt = 0x00u;
            }
        }
        else
        {
        }
    }
    else
    {
        lub_response = UDS_NRC_WRONGBLOCKSEQUENCECOUNTER;
    }

    rul_TrnsfDatPrevTsfrdMemSize = (pAPduResp->msgLength - 0x02u);

    return lub_response;
}
/*
 * 函数介绍: 传输数据等待处理接口
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UDS_FN_TYPE UdsApp_36TransferDat_Pending(void)
{
    uint8_t lub_response = UDS_NRC_REQUESTCORRECTLYRECEIVED_RESPPENDING;

    /* uint8_t lub_PrevReqDnldTblIdx = UdsApp_34ReqDnld_GetCurrDnldTblIdx(); */

    if (rub_36TransferDat_PendingFlg > 0x00u)
    {
        /* check if Flash driver has already finished programming the flash block
         */
        /* if (Uds_g_ReqDnldMngTbl[lub_PrevReqDnldTblIdx].pFuncPtrIsProgFinished()) */
        if (UDS_ITF_IMP_PROG_IS_FINISH_FP_ANY_ADDR())
        {

            lub_response = UDS_POSITIVE_RESPONSE;
            rub_36TransferDat_PendingFlg = (uint8_t)FALSE;
        }
        else
        {
        }
    }
    else
    {
    }

    return lub_response;
} /* end of UdsApp_36TransferDat_Pending() */
/*
 * 函数介绍: 初始化设置默认值
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_36TransferDat_InitBlkSeqCntrBefrTsfrDat(void)
{
    /* uint8_t lub_PrevReqDnldTblIdx = UdsApp_34ReqDnld_GetCurrDnldTblIdx();  */
    rub_36TransferDat_PendingFlg = (uint8_t)FALSE;
    rub_DnldDatSvcJust1stTrsfrDatFlg = (uint8_t)TRUE;

    rub_DnldDatSvrExpctdNxtBlkSeqCntrFrmClnt = 0x01u;
    rub_DnldDatSvrCurrRcvdBlkSeqCntrFrmClnt = 0x00u;
    rub_DnldDatSvrPrevRcvdOkBlkSeqCntrFrmClnt = 0x00u;

    rul_TrnsfDatCurrMemAdd2Wrt = UdsApp_34ReqDnld_GetCurrDnldStartAddr(); /* Uds_g_ReqDnldMngTbl[lub_PrevReqDnldTblIdx].ReqDnldStartMemAdd;  */
    rul_TrnsfDatTotalMemSize2Wrt = 0x00000000u;
    rul_TrnsfDatCurrAlrdyRcvdMemSize = 0x00000000u;

    rub_UpldDatSvcJust1stTrsfrDatFlg = (uint8_t)FALSE;

    rub_UpldDatSvrExpctdNxtBlkSeqCntrFrmClnt = 0x01u;
    rub_UpldDatSvrCurrTsfrdBlkSeqCntrFrmClnt = 0x00u;
    rub_UpldDatSvrPrevTsfrdOkBlkSeqCntrFrmClnt = 0x00u;
#ifdef UDS_ENABLE_REQUESTUPLOAD
    rul_TrnsfDatCurrMemAdd2Rd = UdsApp_35ReqUpld_GetCurrUpldStartAddr();
#endif
    rul_TrnsfDatTotalMemSize2Rd = 0x00000000u;
    rul_TrnsfDatCurrAlrdyTsfrdMemSize = 0x00000000u;

    ruw_36TransferDatCurrReqMsgLen = 0x0000u;

    /* if (lub_PrevReqDnldTblIdx != 0xFFu)  */
    {
        Uds_MemClear(UDS_ITF_IMP_DPTR_REQ_DN_ANY_ADDR, /* Uds_g_ReqDnldMngTbl[lub_PrevReqDnldTblIdx].pDataBuf, */ /* pDest */
                     UDS_C_34REQDNLD_USING_NUM_BLK_LEN);                                                          /* numBytes */
    }
    /* else  */
    {
    }
}
/*
 * 函数介绍: 获取传输状态
 * 参数:
 * 返回值: {*}
 * 备注:
 */
UDS_TransferDatState_t UDS_FN_TYPE UdsApp_36TransferDat_GetCurrTsfrDatSt(void)
{
    return rub_TransferDatState;
}
/*
 * 函数介绍: 设置传输状态
 * 参数:
 * i_TransferDatSt2Set：传输状态
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE UdsApp_36TransferDat_SetCurrTsfrDatSt(UDS_TransferDatState_t i_TransferDatSt2Set)
{
    rub_TransferDatState = i_TransferDatSt2Set;
}
/*
 * 函数介绍: 写数据接口
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UdsApp_36TransferDat_WriteDataItf_Stub(uint32_t *ptr_DataDestAddr,
                                               uint32_t i_DataLen2Write,
                                               uint32_t *ptr_DataSrcAddr)
{
    /* this is just a stub function , which should be implemented to write data
     */
    return TRUE;
}
/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UdsApp_36TransferDat_WriteDataIsFinished_Stub(void)
{
    /* this is just a stub function , which should be implemented to check the writing operation is finished
     */
    return TRUE;
}

#endif /* UDS_ENABLE_TRANSFERDATA */
/******************************************************************************
 *                               end of file                                   *
 ******************************************************************************/
