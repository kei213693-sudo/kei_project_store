/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: UdsItf_Srv.h
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDSITF_SRV_H_
#define _UDSITF_SRV_H_

#include "typedefs.h"

/*
 * 函数介绍: UDS service interface, clear all dtcs, called by sid=0X14.
 * 参数: [in] dtcCode2Clear: dtccode to clear, it must be 0xFFFFFF.
 * 返回值: {*}
 * 备注:
 */
extern void UDS_itf_clearAllDtcs(uint32_t dtcCode2Clear);

/*
 * 函数介绍: UDS service interface, clear group dtcs, called by sid=14.
 * 参数: [in] dtcCode2Clear: Low 8bit is group number, 0xffff00<dtcCode2Clear<0xffffff.
 * 返回值: {*}
 * 备注:
 */
extern void UDS_itf_clearGroupDtcs(uint32_t dtcCode2Clear);

/*
 * 函数介绍: UDS service interface, clear specific dtc, called by sid=14.
 * 参数: [in] dtcCode2Clear: dtc code.
 * 返回值: {*}
 * 备注:
 */
extern void UDS_itf_clearSpecificDtc(uint32_t dtcCode2Clear);

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
extern uint16_t UDS_itf_reportWWHOBDDTCByMaskRecord(uint8_t statusMask, uint8_t severityMask, uint8_t *pData, uint16_t maxDtcNum);

/*
 * 函数介绍: UDS service interface, report DTC by status mask, service SID=0X19 SUBID=0X02.
 * 参数:
 * \param [in] statusMask: status mask 0x01:当前故障，0x08:历史故障
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * \param [in] maxDtcNum: the max DTC number can be storage in data buffer.
 * 返回值: the respond data length.
 * 备注:
 */
extern uint16_t UDS_itf_reportDTCByStatusMask(uint8_t statusMask, uint8_t *pData, uint16_t maxDtcNum);

/*
 * 函数介绍: UDS service interface, report all suprot DTC , service SID=0X19 SUBID=0X0A.
 * 参数:
 * reportDTCByStatusMask
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * \param [in] maxDtcNum: the max DTC number can be storage in data buffer.
 * 返回值: the respond data length.
 * 备注:
 */
extern uint16_t UDS_itf_reportAllSuportDTC(uint8_t *pData, uint16_t maxDtcNum);

/*
 * 函数介绍: UDS service interface, report Permanent DTC, service SID=0X19 SUBID=0X55.
 * 参数:
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * \param [in] maxDtcNum: the max DTC number can be storage in data buffer.
 * 返回值: the respond data length.
 * 备注:
 */
extern uint16_t UDS_itf_reportWWHOBDDTCWithPermanentStatus(uint8_t *pData, uint16_t maxDtcNum);

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
extern uint16_t UDS_itf_reportDTCSnapshotRecordByDTCNumber(uint32_t dtc, uint8_t recordNumber, uint8_t *pData);

/*
 * 函数介绍: UDS service interface, report number of DTC by status mask, service SID=0X19 SUBID=0X01.
 * 参数:
 * reportNumberOfDTCByStatusMask
 * \param [in] statusMask: status mask.
 * 返回值: the number of DTC.
 * 备注:
 */
extern uint16_t Uds_itf_reportNumOfDTCByStatusMask(uint8_t statusMask);

/*
 * 函数介绍: UDS service interface, report ext data record by DTC number, service SID=0X19 SUBID=0X06.
 * 参数:
 * \param [in] dtc: DTC number.
 * \param [in] recordNumber: record number, the current support 0x90 only.
 * \param [in] pData: data buffer pointer, it is storage the response data.
 * 返回值: the respond data length.
 * 备注:
 */
extern uint16_t Uds_itf_reportDTCExtDataRecordByDTCNumber(uint32_t dtc, uint8_t recordNumber, uint8_t *pData);
/*
 * 函数介绍: Report fault snapshot identifier (reportDTCSnapshotIdentification) requests for information
 * 参数:
 * 返回值: {*}
 * 备注: 19 03
 */
extern void UDS_itf_reportDTCSnapshotIdentificationK(uint8_t recordNmu,uint8_t *pData);
#if 0
/*
 * 函数名: Uds_itf_set_lowPowerInstuct
 * 函数介绍: 设置低功耗指令状态
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_set_lowPowerInstuct(uint8_t value);

/*
 * 函数名: Uds_Itf_0xFEFC_Handle
 * 函数介绍:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_Itf_0xFEFC_Handle(void);
/*
 * 函数名: Uds_Itf_0xF195_Handle
 * 函数介绍:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_Itf_0xF195_Handle(void);

/*
 * 函数名: Uds_Itf_GetBleVerInfo
 * 函数介绍:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_Itf_GetBleVerInfo(VerInfoType *BleVerInfo);

/*
 * 函数名: Uds_Itf_set_Handle_F195
 * 函数介绍:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_Itf_set_Handle_F195(uint8_t value);
/*
 * 函数名: Uds_Itf_Get_Handle_F195
 * 函数介绍:
 * 返回值: {*}
 * 备注:
 */
extern uint8_t Uds_Itf_Get_Handle_F195(void);
#endif
/*
 * 函数介绍: 使能接收报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_enable_normal_rx(void);
/*
 * 函数介绍: 使能发送报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_enable_normal_tx(void);
/*
 * 函数介绍: 禁止接收报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_disable_normal_rx(void);
/*
 * 函数介绍: 禁止报文发送
 * 参数:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_disable_normal_tx(void);

/*
 * 函数介绍: 使能网络管理接收报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_enable_nm_rx(void);
/*
 * 函数介绍: 使能网络管理发送报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_enable_nm_tx(void);
/*
 * 函数介绍: 禁止网络管理接收报文
 * 参数:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_disable_nm_rx(void);
/*
 * 函数介绍: 禁止网络管理报文发送
 * 参数:
 * 返回值: {*}
 * 备注:
 */
extern void Uds_itf_disable_nm_tx(void);
/*
 * 函数介绍: 获取APP报文接收状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t Uds_itf_normal_rx_state(void);
/*
 * 函数介绍: 获取APP报文发送状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t Uds_itf_normal_tx_state(void);
/*
 * 函数介绍: 获取NM报文接收状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t Uds_itf_nm_rx_state(void);
/*
 * 函数介绍: 获取NM报文发送状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t Uds_itf_nm_tx_state(void);

#endif /* _UDSITF_SRV_H_ */
