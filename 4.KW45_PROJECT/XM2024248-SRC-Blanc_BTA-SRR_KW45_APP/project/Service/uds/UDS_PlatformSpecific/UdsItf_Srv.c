/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: UdsItf_Srv.c
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include <string.h>
#include "E2Para.h"
#include "DidList.h"
#include "Dtc_Manage.h"
#include "SnapshotTable.h"
#include "uds_api.h"
#define DFPM_EN 1



static uint8_t normal_com_rx_enable;   	/* normal communication message reception enable */
static uint8_t normal_com_tx_enable;   	/* normal communication message transmission enable */
static uint8_t nm_com_rx_enable;		/* NM communication message reception enable */
static uint8_t nm_com_tx_enable;		/* NM communication message transmission enable */

extern void DSM_fcmclr(void); /* dsm module clear */

/*
 * 函数介绍: check uds status by mask code
 * 参数:
 * param [in] sta	usd status.
 * param [in] mask	mask code.
 * 返回值: 1:status valid, 0:status invalid.
 * 备注:
 */
static uint8_t DFPM_udsStatusMask(uint8_t sta, uint8_t mask)
{
#if DFPM_EN
	uint8_t valid = 0;
	if (sta != 0)
	{
		if (0x04 ==mask ) /* pending status */
		{
			if (sta & mask)
			{
				valid = 1;
			}
			else
			{
			}
		}
		else
		{
			if (sta == mask)
			{
				valid = 1;
			}
			else
			{
			}
		}
	}
	else
	{
	}
	return valid;
#endif
	return 0;
}

/*
 * 函数介绍: UDS service interface, clear all dtcs, called by sid=0X14.
 * 参数: [in] dtcCode2Clear: dtccode to clear, it must be 0xFFFFFF.
 * 返回值: {*}
 * 备注:
 */
void UDS_itf_clearAllDtcs(uint32_t dtcCode2Clear)
{
	/*clear all dtcs*/
	DTC_clear();
}

/*
 * 函数介绍: UDS service interface, clear group dtcs, called by sid=14.
 * 参数: [in] dtcCode2Clear: Low 8bit is group number, 0xffff00<dtcCode2Clear<0xffffff.
 * 返回值: {*}
 * 备注:
 */
void UDS_itf_clearGroupDtcs(uint32_t dtcCode2Clear)
{
}

/*
 * 函数介绍: UDS service interface, clear specific dtc, called by sid=14.
 * 参数: [in] dtcCode2Clear: dtc code.
 * 返回值: {*}
 * 备注:
 */
void UDS_itf_clearSpecificDtc(uint32_t dtcCode2Clear)
{
}
/*
 * 函数介绍: UDS service interface, report DTC by mask, service SID=0X19 SUBID=0X06.
 * 参数:
 * reportWWHOBDDTCByMaskRecord
 * \param [in] statusMask: status mask.
 * \param [in] severityMask: severity mask.
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * \param [in] maxDtcNum: the max DTC number can be storage in data buffer.
 * 返回值: the respond data length.
 * 备注:
 */
uint16_t UDS_itf_reportWWHOBDDTCByMaskRecord(uint8_t statusMask, uint8_t severityMask, uint8_t *pData, uint16_t maxDtcNum)
{
	return 0;
}

/*
 * 函数介绍: UDS service interface, report DTC by status mask, service SID=0X19 SUBID=0X02.
 * 参数:
 * \param [in] statusMask: status mask 0x01:当前故障，0x08:历史故障
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * \param [in] maxDtcNum: the max DTC number can be storage in data buffer.
 * 返回值: the respond data length.
 * 备注:
 */
uint16_t UDS_itf_reportDTCByStatusMask(uint8_t statusMask, uint8_t *pData, uint16_t maxDtcNum)
{

	uint8_t i = 0;
	uint8_t result = DFPM_E_initialized;
	uint32_t dtc = 0;
	uint16_t dtcLen = 0;
	uint8_t dtcNum = 0;
	DTC_Confirm DtcConfirm;

	dtc_flash_write_un l_dtc_flash_info_un;
	uint8_t confirm_hist_dtc_arr[DTC_NUM] = {0};
	uint8_t dtc_sta[DTC_NUM] = {0};

	if (NULL == pData)
	{
		return 0 ;
	}
	else
	{
		/* code */
	}

	switch (statusMask)
	{
		/*状态掩码为当前故障*/
	case 0x01:
		for (i = 0; i < DTC_NUM; i++)
		{
			DtcConfirm = Dtc_getDtcConfirmDfp(i);
			if (DtcConfirm.dfp != DFP_INVALUE)
			{
				if (DFPM_E_ok == DFPM_getDtcByDfp(&dtc, DtcConfirm.dfp))
				{
					pData[dtcLen + 0] = (uint8_t)(dtc >> 16);
					pData[dtcLen + 1] = (uint8_t)(dtc >> 8);
					pData[dtcLen + 2] = (uint8_t)dtc;
					pData[dtcLen + 3] = DtcConfirm.dtc_sta;
					dtcLen = dtcLen + 4u;
					dtcNum++;
					if (dtcNum >= maxDtcNum)
					{
						break;
					}
					else
					{
					}
				}
				else
				{
					/* code */
				}
			}
			else
			{
				/* code */
			}
		}
		break;
		/*状态掩码为历史故障*/
	case DTC_FaultSta_previously:

		result = Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DTC, l_dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);

		if (result == R_FDL_OK)
		{
			for (i = 0; i < DTC_NUM; i++)
			{
				/*if history arr is value*/
				if (l_dtc_flash_info_un.sig.historyDtc[i] != DFP_INVALUE)
				{
					if (DFPM_E_ok == DFPM_getDtcByDfp(&dtc, l_dtc_flash_info_un.sig.historyDtc[i]))
					{
						pData[dtcLen + 0] = (uint8_t)(dtc >> 16);
						pData[dtcLen + 1] = (uint8_t)(dtc >> 8);
						pData[dtcLen + 2] = (uint8_t)dtc;
						pData[dtcLen + 3] = DTC_FaultSta_previously; /*fault status*/
						dtcLen = dtcLen + 4u;
						dtcNum++;
						if (dtcNum >= maxDtcNum)
						{
							break;
						}
						else
						{
						}
					}
					else
					{
					}
				}
				else
				{
				}
			}
		}
		else
		{
		}
		break;
	case 0x09:
		/*读取历史故障*/
		result = Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DTC, l_dtc_flash_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);

		if (result != R_FDL_OK)
		{
			break;
		}
		else
		{
			/* code */
		}

		for (i = 0; i < DTC_NUM; i++)
		{
			DtcConfirm = Dtc_getDtcConfirmDfp(i);
			if ((DtcConfirm.dfp != DFP_INVALUE))
			{
				confirm_hist_dtc_arr[i] = DtcConfirm.dfp;

				dtc_sta[i] = DTC_FaultSta_confirmed;
			}
			else
			{
				if (l_dtc_flash_info_un.sig.historyDtc[i] != DFP_INVALUE)
				{
					confirm_hist_dtc_arr[i] = l_dtc_flash_info_un.sig.historyDtc[i];
					dtc_sta[i] = DTC_FaultSta_previously;
				}
				else
				{}
			}
			if (confirm_hist_dtc_arr[i] != DFP_INVALUE)
			{
				if (DFPM_E_ok == DFPM_getDtcByDfp(&dtc, confirm_hist_dtc_arr[i]))
				{
					pData[dtcLen + 0] = (uint8_t)(dtc >> 16);
					pData[dtcLen + 1] = (uint8_t)(dtc >> 8);
					pData[dtcLen + 2] = (uint8_t)dtc;
					pData[dtcLen + 3] = dtc_sta[i]; /*fault status*/
					dtcLen = dtcLen + 4u;
					dtcNum++;
					if (dtcNum >= maxDtcNum)
					{
						break;
					}
					else
					{
					}
				}
				else
				{
				}
			}
			else
			{
				/* code */
			}
		}
		break;
	default:
		break;
	}

	return dtcLen;
}
/*
 * 函数介绍: Report fault snapshot identifier (reportDTCSnapshotIdentification) requests for information
 * 参数:
 * 返回值: {*}
 * 备注: 19 03
 */
uint16_t UDS_itf_reportDTCSnapshotIdentificationK(uint8_t recordNmu,uint8_t *pData)
{
	uint8_t result = 0;
	uint8_t i;
	uint32_t dtc = 0;
	uint16_t dtcLen = 0;
	uint8_t dtcNum = 0;
	uint8_t HistDTC_DfpArr[DTC_NUM] = {0};
	uint8_t confirm_hist_dtc_arr[DTC_NUM] = {0};
	DTC_Confirm DtcConfirm;

	if (NULL == pData)
	{
		return 0;
	}
	else
	{
		/* code */
	}

#if 0
	result = fdl_FlashRead(DTC_BASE_ADDR, HistDTC_DfpArr, DTC_NUM);

	if (result != R_FDL_OK)

	{
		return dtcLen;
	}
#endif
	for (i = 0; i < DTC_NUM; i++)
	{
		DtcConfirm = Dtc_getDtcConfirmDfp(i);
		if ((DtcConfirm.dfp != DFP_INVALUE))
		{
			confirm_hist_dtc_arr[i] = DtcConfirm.dfp;
		}
		else
		{
			if (HistDTC_DfpArr[i] != DFP_INVALUE)
			{
				confirm_hist_dtc_arr[i] = HistDTC_DfpArr[i];
			}
		}

		if (confirm_hist_dtc_arr[i] != DFP_INVALUE)
		{
			if (DFPM_getDtcByDfp(&dtc, confirm_hist_dtc_arr[i]) == DFPM_E_ok)
			{
				pData[dtcLen + 0] = (uint8_t)(dtc >> 16);
				pData[dtcLen + 1] = (uint8_t)(dtc >> 8);
				pData[dtcLen + 2] = (uint8_t)dtc;
				/*Snapshot number 0x01*/
				pData[dtcLen + 3] = recordNmu;

				dtcLen = dtcLen + 4u;
				dtcNum++;
				if (dtcNum > DTC_NUM)
				{
					return (dtcLen-4);
				}
				else
				{
				}
			}
			else
			{
			}
		}
		else
		{

		}
	}
	return dtcLen;
}
/*
 * 函数介绍: UDS service interface, report all suprot DTC , service SID=0X19 SUBID=0X0A.
 * 参数:
 * reportDTCByStatusMask
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * \param [in] maxDtcNum: the max DTC number can be storage in data buffer.
 * 返回值: the respond data length.
 * 备注:
 */
uint16_t UDS_itf_reportAllSuportDTC(uint8_t *pData, uint16_t maxDtcNum)
{
	uint16_t dtcLen = 0;
	uint32_t dtc;
	uint16_t i;
	uint8_t dtc_sta = 0;
	Dtc_FaultManage pActInfio;
	dtc_flash_write_un l_dtc_info_un;

	if (NULL == pData)
	{
		return 0;
	}
	else
	{
		/* code */
	}

	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DTC, l_dtc_info_un.dtc_mangage_buffer, DTC_FLASH_LEN);

	for (i = 0; i < DTC_NUM; i++)
	{
		pActInfio.ConfirmArry[i] = Dtc_getDtcConfirmDfp(i);
		if(pActInfio.ConfirmArry[i].dfp != 0)
		{
			dtc_sta = 0x01u;
		}
		else if (l_dtc_info_un.sig.historyDtc[i] != 0)
		{
			dtc_sta = 0x08u;
		}

		DFPM_getDtcByDfp(&dtc, i + 1);
		pData[dtcLen + 0] = (uint8_t)(dtc >> 16);
		pData[dtcLen + 1] = (uint8_t)(dtc >> 8);
		pData[dtcLen + 2] = (uint8_t)dtc;
		pData[dtcLen + 3] = dtc_sta;
		dtcLen = dtcLen + 4u;
	}
	return dtcLen;
}
/*
 * 函数介绍: UDS service interface, report Permanent DTC, service SID=0X19 SUBID=0X55.
 * 参数:
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * \param [in] maxDtcNum: the max DTC number can be storage in data buffer.
 * 返回值: the respond data length.
 * 备注:
 */
uint16_t UDS_itf_reportWWHOBDDTCWithPermanentStatus(uint8_t *pData, uint16_t maxDtcNum)
{
	return 0;
}

/*
 * 函数介绍: UDS service interface, report Snapshot record by DTC number, service SID=0X19 SUBID=0X04.
 * 参数:
 * reportDTCSnapshotRecordByDTCNumber
 * \param [in] dtc: DTC number.
 * \param [in] recordNumber: record number, 0x00 is reserved for legislated purposes (e.g., WWH-OBD).
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * 返回值: the respond data length.
 * 备注: The current support 0x00(WWH-OBD), 0XFF(all snapshot), others not support.
 */
uint16_t UDS_itf_reportDTCSnapshotRecordByDTCNumber(uint32_t dtc, uint8_t recordNumber, uint8_t *pData)
{

	uint8_t *pSnapshot = NULL;
	uint8_t dtcSta;
	uint16_t len = 0;
	uint16_t dfp;
	uint8_t group[14];
	const Snapshot_Info *snapshot;

	if (NULL == pData)
	{
		return 0;
	}
	else
	{
		/* code */
	}

	switch (recordNumber)
	{
	case 0x00u:
	case 0x01u:
#if 0
		if (DFPM_getDfpByDtc(dtc, &dfp) == DFPM_E_ok)
		{
			if ((DFPM_getStaByDfp(dfp, (uint8_t *)&dtcSta) == DFPM_E_ok) && (DFPM_getSnapshot((uint8_t *)&group, dfp) == DFPM_E_ok))
			{
				pData[len++] = (uint8_t)(dtc >> 16);
				pData[len++] = (uint8_t)(dtc >> 8);
				pData[len++] = (uint8_t)(dtc);
				pData[len++] = dtcSta;
				pData[len++] = recordNumber;
				pData[len++] = 0x05;
				pData[len++] = 0x01;
				pData[len++] = 0x12;
				pData[len++] = group[0];
				pData[len++] = 0xE1;
				pData[len++] = 0x01;
				memcpy(&pData[len], &group[1], 3);
				len += 3;
				pData[len++] = 0xB1;
				pData[len++] = 0x00;
				memcpy(&pData[len], &group[4], 2);
				len += 2;
				pData[len++] = 0xD0;
				pData[len++] = 0x01;
				pData[len++] = group[6];
				pData[len++] = 0xF0;
				pData[len++] = 0x20;
				memcpy(&pData[len], &group[7], 7);
				len += 7;
			}
			else
			{
			}
		}
		else
		{
		}
#endif
		break;
	default:
		break;
	}

	return len;
}

/*
 * 函数介绍: UDS service interface, report number of DTC by status mask, service SID=0X19 SUBID=0X01.
 * 参数:
 * reportNumberOfDTCByStatusMask
 * \param [in] statusMask: status mask.
 * 返回值: the number of DTC.
 * 备注:
 */
uint16_t Uds_itf_reportNumOfDTCByStatusMask(uint8_t statusMask)
{
	uint8_t dtcNum = 0;

	dtcNum = Dtc_getDtcNumtoStatusMsk(statusMask);
	return dtcNum;
}
/*
 * 函数介绍: UDS service interface, report ext data record by DTC number, service SID=0X19 SUBID=0X06.
 * 参数:
 * \param [in] dtc: DTC number.
 * \param [in] recordNumber: record number, the current support 0x90 only.
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * 返回值: the respond data length.
 * 备注:
 */
uint16_t Uds_itf_reportDTCExtDataRecordByDTCNumber(uint32_t dtc, uint8_t recordNumber, uint8_t *pData)
{
    /* DTC状态变量 */
    uint8_t dtc_sta;
    /* DFP编号变量 */
    uint16_t dfp;
    /* 已写入pData的字节数 */
    uint8_t len = 0;
    /* 扩展数据临时存储数组 */
    dtc_flash_write_un l_dtc_info_un;
    /* 数据偏移量 */
    uint8_t offset  = 0;
    /* 循环变量 */
    uint8_t i;

    /* 检查pData指针是否为空 */
    if (NULL == pData)
    {
        return 0 ;
    }
    else
    {
        /* code */
    }

    /* 根据DTC编号查找DFP */
    if(DFPM_E_ok == DFPM_getDfpByDtc(dtc,&dfp))
    {
        /* 获取DFP的状态 */
        DFPM_getStaByDfp(dfp, (uint16_t*)&dtc_sta);
        /* 将DTC的高位字节写入pData */
        pData[len++] = (uint8_t)(dtc>>16);
        /* 将DTC的次高位字节写入pData */
        pData[len++] = (uint8_t)(dtc>>8);
        /* 将DTC的低位字节写入pData */
        pData[len++] = (uint8_t)dtc;
        /* 将DTC的状态写入pData */
        pData[len++] = dtc_sta;
        /* 将记录号写入pData */
        pData[len++] = recordNumber;
        /* 计算扩展数据在数组中的偏移量 */
        offset = (dfp-1)*DTC_EXT_DATA_LEN;
        /* 从Flash中读取扩展数据 */
        Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DTC,l_dtc_info_un.dtc_mangage_buffer,DTC_FLASH_LEN);
        /* 将扩展数据写入pData */
        for(i=0;i<DTC_EXT_DATA_LEN;i++)
        {
            pData[len++] = l_dtc_info_un.sig.dtc_ext_data[offset+i];
        }
    }
    else
    {
        /* code */
    }

    /* 返回实际写入的字节数 */
    return len;
}

/*
 * 函数介绍: 使能接收报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void Uds_itf_enable_normal_rx(void)
{
	normal_com_rx_enable = TRUE;
}
/*
 * 函数介绍: 使能发送报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void Uds_itf_enable_normal_tx(void)
{
	normal_com_tx_enable = TRUE;
}
/*
 * 函数介绍: 禁止接收报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void Uds_itf_disable_normal_rx(void)
{
	normal_com_rx_enable = FALSE;
}
/*
 * 函数介绍: 禁止报文发送
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void Uds_itf_disable_normal_tx(void)
{
	normal_com_tx_enable = FALSE;
}
/*
 * 函数介绍: 使能网络管理接收报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void Uds_itf_enable_nm_rx(void)
{
	nm_com_rx_enable = TRUE;
}
/*
 * 函数介绍: 使能网络管理发送报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_enable_nm_tx(void)
{
	nm_com_tx_enable = TRUE;
}
/*
 * 函数介绍: 禁止网络管理接收报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void Uds_itf_disable_nm_rx(void)
{
	nm_com_rx_enable = FALSE;
}
/*
 * 函数介绍: 禁止网络管理报文发送
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void Uds_itf_disable_nm_tx(void)
{
	nm_com_tx_enable = FALSE;
}
/*
 * 函数介绍: 获取APP报文接收状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t Uds_itf_normal_rx_state(void)
{
	return normal_com_rx_enable;
}
/*
 * 函数介绍: 获取APP报文发送状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t Uds_itf_normal_tx_state(void)
{
	return normal_com_tx_enable;
}
/*
 * 函数介绍: 获取NM报文接收状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t Uds_itf_nm_rx_state(void)
{
	return nm_com_rx_enable;
}
/*
 * 函数介绍: 获取NM报文发送状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t Uds_itf_nm_tx_state(void)
{
	return nm_com_tx_enable;
}
