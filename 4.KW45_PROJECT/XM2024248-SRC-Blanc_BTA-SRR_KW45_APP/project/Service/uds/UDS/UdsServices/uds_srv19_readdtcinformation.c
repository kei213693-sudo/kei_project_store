/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_srv19_readdtcinformation.c
 * 摘要: UDS Application Layer service 0x19 ReadDTCInformation.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include "uds_confdefs.h"
#include "uds_confpriv.h"
#include "uds_inf.h"
#include "uds_priv.h"
#include "uds_target.h"
#include "udscan_confdefs.h"
#include "UdsItf_Srv.h"
#include "Dtc_Manage.h"
// #include "UdsDtcMgr.h"

#ifdef UDS_ENABLE_READDTCINFORMATION
#define DTCNUM_MAX ((ISO15765_APDU_MSGDATA_LENGTH - 6) / 5)

/* Sun-Function parameter for ReadDTCInformation(ISO15765-3 limited)
 */

/* NRC for EcuReset
 * 	0x12	UDS_NRC_SUBFUNCTIONNOTSUPPORTED
 * 	0x13	UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 * 	0x22	UDS_NRC_CONDITIONSNOTCORRECT
 *	0x33	UDS_NRC_SECUTITYACCESSDENIED
 */

static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportNumOfDTCByStatusMask(Uds_APduData_t *pAPduInd,
                                                                                Uds_APduData_t *pAPduResp);
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportDTCByStatusMask(Uds_APduData_t *pAPduInd,
                                                                           Uds_APduData_t *pAPduResp);
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportExtDatRcdByDTCNum(Uds_APduData_t *pAPduInd,
                                                                             Uds_APduData_t *pAPduResp);
#if 0
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportDTCStoredDataByRecordNumber(Uds_APduData_t* pAPduInd,
                                                                             Uds_APduData_t* pAPduResp );
#endif
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportDTCSnapshotIdentificationK(Uds_APduData_t *pAPduInd,
                                                                                      Uds_APduData_t *pAPduResp);

static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportSupportedDTC(Uds_APduData_t *pAPduInd,
                                                                        Uds_APduData_t *pAPduResp);

static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportSnapshotRcdByDtcNum(Uds_APduData_t *pAPduInd,
                                                                               Uds_APduData_t *pAPduResp);
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportWWHOBDDTCByMaskRecord(Uds_APduData_t *pAPduInd,
                                                                                 Uds_APduData_t *pAPduResp);
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportOBDDTCWithPerstatus(Uds_APduData_t *pAPduInd, Uds_APduData_t *pAPduResp);
/*
 * 函数介绍: 读取DTC信息
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 */
Std_UdsReturnType UDS_FN_TYPE UdsApp_ReadDTCInformation(Uds_APduData_t *pAPduInd,
                                                        Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response = UDS_POSITIVE_RESPONSE;
    uint8_t rec_msg01 = pAPduInd->pduData[0x01u];
    /*Check whether the pointer is empty*/
    if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NO_RESPONSE;
    }
    else
    {
        /* code */
    }
    /* 0x19 not support funtion addring*/
    if (TARGETADDR_FUNCTIONAL == UDS_APP_ITF_IMP_GET_TP_CHNL())
    {
        lub_response = UDS_NRC_SERVICENOTSUPPORTED;
        return lub_response;
    }
    /*不支持禁止肯定响应位*/
    if (((pAPduInd->pduData[0x01u]) >> 7) == 0x1u)
    {
        lub_response = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
        return lub_response;
    }

    /* Minimum length check*/
    if (0x02u > (pAPduInd->msgLength))
    {
        /* the requst message length is invalid*/
        lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        return lub_response;
    }
    else
    {}
#ifdef EP40_PRJ
    if (((READDTC_REPORT_NUMDTC_BYSTATUSMSK == ((pAPduInd->pduData[0x01u]))) &&
         ((pAPduInd->msgLength) != 0x03u)) ||
        ((READDTC_REPORT_DTC_BYSTATUSMSK == ((pAPduInd->pduData[0x01u]))) &&
         ((pAPduInd->msgLength) != 0x03u)) ||
        ((READDTC_REPORT_DTCSNAPSHOT_BYDTCNUM == ((pAPduInd->pduData[0x01u]))) &&
         ((pAPduInd->msgLength) != 0x06u)) ||
        ((READDTC_REPORT_SUPPORTED_DTC == ((pAPduInd->pduData[0x01u]))) &&
         ((pAPduInd->msgLength) != 0x02u))
#if 0
		 ||
        ((READDTC_REPORT_DTC_SnapshotIdentificationK == ((pAPduInd->pduData[0x01u]))) &&
         ((pAPduInd->msgLength) != 0x02u))
#endif
		 )

#else
    if (((READDTC_REPORT_NUMDTC_BYSTATUSMSK == ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7)) &&
         ((pAPduInd->msgLength) != 0x03u)) ||
        ((READDTC_REPORT_DTC_BYSTATUSMSK == ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7)) &&
         ((pAPduInd->msgLength) != 0x03u)) ||
        ((READDTC_REPORT_DTCSNAPSHOT_BYDTCNUM == ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7)) &&
         ((pAPduInd->msgLength) != 0x06u)) ||
        ((READDTC_REPORT_DTC_EXTDAT_RCD_BYDTCNUM == ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7)) &&
         ((pAPduInd->msgLength) != 0x06u)) ||
        ((READDTC_REPORT_SUPPORTED_DTC == ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7)) &&
         ((pAPduInd->msgLength) != 0x02u)) ||
        ((READDTC_REPORT_DTC_SnapshotIdentificationK == ((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7)) &&
         ((pAPduInd->msgLength) != 0x02u)))
#endif
    {
        /* the requst message is invalid*/
        lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        return lub_response;
    }
    else
    {
        switch (((pAPduInd->pduData[0x01u]) & UDS_C_SUB_FUNC_MASK_NO_SPRB7))
        {
        default:
            lub_response = UDS_NRC_SUBFUNCTIONNOTSUPPORTED;
            break;

            /* reportNumberOfDTCByStatusMask */
        case READDTC_REPORT_NUMDTC_BYSTATUSMSK:
            lub_response = Uds_19RdDtcInfo_reportNumOfDTCByStatusMask(pAPduInd,
                                                                      pAPduResp);
            break;
            /* reportDTCByStatusMask */
        case READDTC_REPORT_DTC_BYSTATUSMSK:
            lub_response = Uds_19RdDtcInfo_reportDTCByStatusMask(pAPduInd,
                                                                 pAPduResp);
            break;
#if 0
                /* reportDTCSnapshotIdentification */
            case READDTC_REPORT_DTC_SnapshotIdentificationK:
                lub_response = Uds_19RdDtcInfo_reportDTCSnapshotIdentificationK(pAPduInd,
                                                                                pAPduResp);
                //lub_response = UDS_POSITIVE_RESPONSE;
                break;

                /* reportDTCSnapshotRecordByDTCNumber */
            case READDTC_REPORT_DTCSNAPSHOT_BYDTCNUM:
                lub_response = Uds_19RdDtcInfo_reportSnapshotRcdByDtcNum(pAPduInd,
                                                                         pAPduResp);
                break;

               /* reportDTCStoredDataByRecordNumber */
            case READDTC_REPORT_DTCSTOREDATA_BYRECORDNUM:
            	lub_response = Uds_19RdDtcInfo_reportDTCStoredDataByRcdNum(pAPduInd,
                        														 pAPduResp);
            	break;
#endif
#if 1
            /* reportDTCExtendedDataRecordByDTCNumber */
        case READDTC_REPORT_DTC_EXTDAT_RCD_BYDTCNUM:
            lub_response = Uds_19RdDtcInfo_reportExtDatRcdByDTCNum(pAPduInd,
                                                                   pAPduResp);

            break;
#endif

            /* reportSupportedDTC */
        case READDTC_REPORT_SUPPORTED_DTC:
            lub_response = Uds_19RdDtcInfo_reportSupportedDTC(pAPduInd,
                                                              pAPduResp);
            break;
#if 0
               /* reportWWHOBDDTCByMaskRecord */
            case READDTC_REPORT_WWHOBDDTC_BYMASKRECORD:
            	lub_response = Uds_19RdDtcInfo_reportWWHOBDDTCByMaskRecord(pAPduInd,
            	                                                           pAPduResp);
            	break;
            	/* reportWWHOBDDTCByMaskRecord */
            case READDTC_REPORT_WWHOBDDTC_WITHPERMANENTSTATUS:
            	lub_response = Uds_19RdDtcInfo_reportOBDDTCWithPerstatus(pAPduInd,
            	                                                           pAPduResp);
            	break;
#endif
        }
    }

    return lub_response;
}

/*
 * 函数介绍: 根据掩码读取dtc数量
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportNumOfDTCByStatusMask(Uds_APduData_t *pAPduInd,
                                                                                Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response;
    uint8_t statusMask;
    uint16_t luw_nbDTC = 0x0000u;

    lub_response = UDS_POSITIVE_RESPONSE;
    /*Check whether the pointer is empty*/
    if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NO_RESPONSE;
    }
    else
    {
        /* code */
    }
    /*Check request length*/
    if (pAPduInd->msgLength != 0x3u)
    {
        lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        return lub_response;
    }
    /*Gets the status mask*/
    statusMask = ((pAPduInd->pduData[0x02u]) & 0x0F);
    luw_nbDTC = Uds_itf_reportNumOfDTCByStatusMask(statusMask);
    pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u];
    pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];
    pAPduResp->pduData[0x02u] = UDS_C_SUPPORTED_DTC_ST_MASK;
    pAPduResp->pduData[0x03u] = UDS_C_DTC_CODE_FORMAT_ISO15031_6_FMT;
    pAPduResp->pduData[0x04u] = (uint8_t)((luw_nbDTC >> 0x08u) & 0xFFu);
    pAPduResp->pduData[0x05u] = (uint8_t)((luw_nbDTC) & 0xFFu);
    pAPduResp->msgLength = 0x06u;
    return lub_response;
}
/*
 * 函数介绍: 根据掩码读取DTC
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportDTCByStatusMask(Uds_APduData_t *pAPduInd,
                                                                           Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response;
    uint8_t statusMask;
    uint16_t dtcLen;

    lub_response = UDS_POSITIVE_RESPONSE;
    /*Check whether the pointer is empty*/
   if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NO_RESPONSE;
    }
    else
    {
        /* code */
    }
    /*Check request length*/
    if (pAPduInd->msgLength != 0x3u)
    {
        lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        return lub_response;
    }
    else
    {}
    statusMask = pAPduInd->pduData[0x02u];
    pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u];
    pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];
    pAPduResp->pduData[0x02u] = UDS_C_SUPPORTED_DTC_ST_MASK;
    (pAPduResp->msgLength) = 0x03u;
    dtcLen = UDS_itf_reportDTCByStatusMask(statusMask, &pAPduResp->pduData[(pAPduResp->msgLength)], DTCNUM_MAX);
    pAPduResp->msgLength = pAPduResp->msgLength + dtcLen;
    return lub_response;
}

#if 0
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportDTCStoredDataByRcdNum(Uds_APduData_t* pAPduInd,
                                                                             Uds_APduData_t* pAPduResp )
{
    uint8_t    	lub_response;
    uint8_t    	lub_indexFound= 0xFFu;
    uint8_t    	lub_index ;
    uint32_t  	lul_DTCcode ;
    uint8_t		recordNumber;
    uint16_t	len;

    lub_response = UDS_POSITIVE_RESPONSE ;
    recordNumber = pAPduInd->pduData[0x01u];
    pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u];
    pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];
    pAPduResp->pduData[0x02u] = pAPduInd->pduData[0x02u];
    (pAPduResp->msgLength) = 0x03u;

    len = Uds_itf_getStoredDatByRecordNum(recordNumber, &pAPduResp->pduData[(pAPduResp->msgLength)]);
    if(len>0)
    {
    	pAPduResp->msgLength = pAPduResp->msgLength + len;
    }
    else
    {
    	lub_response = UDS_NRC_REQUESTOUTOFRANGE ;
    }
    return lub_response;

} /* end of Uds_19RdDtcInfo_reportDTCStoredDataByRecordNumber() */
#endif
/*
 * 函数介绍: 读取DTC扩展数据
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportExtDatRcdByDTCNum(Uds_APduData_t *pAPduInd,
                                                                             Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response;
    // uint8_t    	lub_indexFound= 0xFFu;
    // uint8_t    	lub_index ;
    uint32_t lul_DTCcode;
    uint8_t recordNumber;
    uint16_t len;

    lub_response = UDS_POSITIVE_RESPONSE;
    lul_DTCcode = (uint32_t)(((pAPduInd->pduData[0x02u]) << 16u) & (0x00FF0000u));
    lul_DTCcode |= (uint32_t)(((pAPduInd->pduData[0x03u]) << 8u) & (0x0000FF00u));
    lul_DTCcode |= (uint32_t)(((pAPduInd->pduData[0x04u])) & (0x000000FFu));
    recordNumber = pAPduInd->pduData[0x05u];
    /*Check whether the pointer is empty*/
    if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NO_RESPONSE;
    }
    else
    {
        /* code */
    }
    /*检查最小长度*/
    if(pAPduInd->msgLength != 6 )
    {
    	lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    	return lub_response;
    }
    else
    {}
    /*check dtc is valid */
    if (DFPM_CheckDtcIsValid(lul_DTCcode) != DFPM_E_ok)
    {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
    }
    else
    {
    }
    pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u];
    pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];
    (pAPduResp->msgLength) = 0x02u;
    len = Uds_itf_reportDTCExtDataRecordByDTCNumber(lul_DTCcode, recordNumber, &pAPduResp->pduData[(pAPduResp->msgLength)]);
    if (len > 0)
    {
        pAPduResp->msgLength = pAPduResp->msgLength + len;
    }
    else
    {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    }
    return lub_response;

} /* end of Uds_19RdDtcInfo_reportExtDatRcdByDTCNum() */
/*
 * 函数介绍: 读取所有支持的DTC
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportSupportedDTC(Uds_APduData_t *pAPduInd,
                                                                        Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response;
    uint8_t dtcLen;

    lub_response = UDS_POSITIVE_RESPONSE;
    /*Check whether the pointer is empty*/
    if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NO_RESPONSE;
    }
    else
    {
        /* code */
    }
    if (pAPduInd->msgLength != 0x02u)
    {
        lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        return lub_response;
    }

    pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u];
    pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];
    pAPduResp->pduData[0x02u] = UDS_C_SUPPORTED_DTC_ST_MASK;
    (pAPduResp->msgLength) = 0x03u;

    dtcLen = UDS_itf_reportAllSuportDTC(&pAPduResp->pduData[(pAPduResp->msgLength)], DTCNUM_MAX);
    pAPduResp->msgLength = pAPduResp->msgLength + dtcLen;
    return lub_response;
}
/*
 * 函数介绍: 读取冻结帧信息
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportDTCSnapshotIdentificationK(Uds_APduData_t *pAPduInd,
                                                                                      Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response;
    uint8_t recordNumber;
    uint16_t len = 0;
    lub_response = UDS_POSITIVE_RESPONSE;
    /*Check whether the pointer is empty*/
    if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NO_RESPONSE;
    }
    else
    {
        /* code */
    }
    // Fault code Number of a snapshot record
    recordNumber = 0x01;
    pAPduResp->msgLength = 0x2;
    //len = UDS_itf_reportDTCSnapshotIdentificationK(recordNumber, (uint8_t *)&(pAPduResp->pduData[pAPduResp->msgLength]));
    if (len > 0)
    {
        pAPduResp->msgLength += len;
    }
    else
    {
        /* code */
    }
    return lub_response;
}
/*
 * 函数介绍: 读取冻结帧根据DTCcode
 * 参数:
 * pAPduInd：上位机传输下来的数据指针
 * pAPduResp：应答数据指针
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportSnapshotRcdByDtcNum(Uds_APduData_t *pAPduInd,
                                                                               Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response;
    uint32_t lul_DTCcode;
    uint8_t recordNumber;
    uint16_t len;

    lub_response = UDS_POSITIVE_RESPONSE;
    lul_DTCcode = (uint32_t)(((pAPduInd->pduData[0x02u]) << 16u) & (0x00FF0000u));
    lul_DTCcode |= (uint32_t)(((pAPduInd->pduData[0x03u]) << 8u) & (0x0000FF00u));
    lul_DTCcode |= (uint32_t)(((pAPduInd->pduData[0x04u])) & (0x000000FFu));
    /*Check whether the pointer is empty*/
    if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NRC_GENERALREJECT;
    }
    else
    {
        /* code */
    }
    // Fault code Number of a snapshot record
    recordNumber = pAPduInd->pduData[0x05u]; // BAM snapshot record 0x01
    if (pAPduInd->msgLength != 0x06u)
    {
        lub_response = UDS_NRC_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        return lub_response;
    }
    else if (DFPM_CheckDtcIsValid(lul_DTCcode) != 0)
    {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
        return lub_response;
    }
    else
    {
    }

    pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u];
    pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];
    (pAPduResp->msgLength) = 0x02u;
    len = UDS_itf_reportDTCSnapshotRecordByDTCNumber(lul_DTCcode, recordNumber, &pAPduResp->pduData[(pAPduResp->msgLength)]);
    if (len >= 0)
    {
        pAPduResp->msgLength = pAPduResp->msgLength + len;
    }
    else
    {
        lub_response = UDS_NRC_REQUESTOUTOFRANGE;
    }
    return lub_response;

} /* end of Uds_19RdDtcInfo_reportSnapshotRcdByDtcNum() */

/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportWWHOBDDTCByMaskRecord(Uds_APduData_t *pAPduInd,
                                                                                 Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response;
    uint8_t functionalGroupId;
    uint8_t statusMask;
    uint8_t severityMask;
    uint16_t dtcLen;

    lub_response = UDS_POSITIVE_RESPONSE;
    functionalGroupId = pAPduInd->pduData[0x02u];
    statusMask = pAPduInd->pduData[0x03u];
    severityMask = pAPduInd->pduData[0x04u];

    /*Check whether the pointer is empty*/
   if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NO_RESPONSE;
    }
    else
    {
        /* code */
    }

    if (functionalGroupId == UDS_FUNGROUPID_EMISSIONS)
    {
        pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u];
        pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];
        pAPduResp->pduData[0x02u] = pAPduInd->pduData[0x02u];
        pAPduResp->pduData[0x03u] = UDS_C_SUPPORTED_DTC_ST_MASK;
        pAPduResp->pduData[0x04u] = UDS_C_SUPPORTED_DTC_SV_MASK;
        pAPduResp->pduData[0x05u] = USD_C_DTC_CODE_FORMAT_SAEJ2012_DA_FMT;
        (pAPduResp->msgLength) = 0x06u;
        dtcLen = UDS_itf_reportWWHOBDDTCByMaskRecord(statusMask, severityMask, &pAPduResp->pduData[(pAPduResp->msgLength)], DTCNUM_MAX);
        pAPduResp->msgLength = pAPduResp->msgLength + dtcLen;
    }
    else
    {
    }
    return lub_response;
}
/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
static Std_UdsReturnType UDS_FN_TYPE Uds_19RdDtcInfo_reportOBDDTCWithPerstatus(Uds_APduData_t *pAPduInd,
                                                                               Uds_APduData_t *pAPduResp)
{
    uint8_t lub_response;
    uint8_t functionalGroupId;
    uint16_t dtcLen;

    lub_response = UDS_POSITIVE_RESPONSE;
    functionalGroupId = pAPduInd->pduData[0x02u];

    /*Check whether the pointer is empty*/
    if ((NULL ==pAPduInd) || (NULL == pAPduResp))
    {
        return UDS_NO_RESPONSE;
    }
    else
    {
        /* code */
    }
    if (functionalGroupId == UDS_FUNGROUPID_EMISSIONS)
    {
        pAPduResp->pduData[0x00u] = pAPduInd->pduData[0x00u];
        pAPduResp->pduData[0x01u] = pAPduInd->pduData[0x01u];
        pAPduResp->pduData[0x02u] = pAPduInd->pduData[0x02u];
        pAPduResp->pduData[0x03u] = UDS_C_SUPPORTED_DTC_ST_MASK;
        pAPduResp->pduData[0x04u] = USD_C_DTC_CODE_FORMAT_SAEJ2012_DA_FMT;
        (pAPduResp->msgLength) = 0x05u;
        dtcLen = UDS_itf_reportWWHOBDDTCWithPermanentStatus(&pAPduResp->pduData[(pAPduResp->msgLength)], DTCNUM_MAX);
        pAPduResp->msgLength = pAPduResp->msgLength + dtcLen;
    }
    else
    {
    }
    return lub_response;
}

#endif /* UDS_ENABLE_READDTCINFORMATION */
/******************************************************************************
 *                               end of file                                   *
 ******************************************************************************/
