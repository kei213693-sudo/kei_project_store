/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_api.h
 * 摘要: UDS接口文件
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2022-09-08
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#ifndef _UDS_API_H
#define _UDS_API_H

#include "typedefs.h"
#include "hal_can_if.h"
#include "hal_sys_if.h"
#include "stdint.h"
#include "Dtc_Manage.h"

#define NIO_PRJ_DID_CHECK
#define D6_UPGRADE_SUCC ((uint16_t) 0x5555u) //d6升级成功
#define D6_UPGRADE_FAIL ((uint16_t) 0xAAAAu) //d6升级失败
/* boot跳转app标志 */
#define JUMP_TRUE ((uint8_t)0xFFu) /* 可以跳转 */
#define JUMP_FALSE ((uint8_t)0xA5u)
extern uint8_t udsapi_Supplier_unlock_flag;
//extern uint32_t uds_api_fdl_flash_addr_a[15];
typedef enum
{
	UDSAPI_FLASH_INDEX_BOOT0 = 124,
	UDSAPI_FLASH_INDEX_BOOT1 ,
	UDSAPI_FLASH_INDEX_DTC,
	UDSAPI_FLASH_INDEX_DID,
	UDSAPI_FLASH_INDEX_MAX
}UdsApi_flash_index_e;

typedef enum R_FDL_STATUS_T
{
    R_FDL_OK = 0,                /**< Operation terminated successfully */
    R_FDL_BUSY = 1,              /**< Operation is still ongoing */
    R_FDL_SUSPENDED = 2,         /**< Flash operation is suspended */
    R_FDL_ERR_CONFIGURATION = 3, /**< The FDL configuration (descriptor) was wrong */
    R_FDL_ERR_PARAMETER = 4,     /**< A parameter of the FDL function call was wrong */
    R_FDL_ERR_PROTECTION = 5,    /**< Operation blocked due to wrong parameters */
    R_FDL_ERR_REJECTED = 6,      /**< Flow error, e.g. another operation is still busy */
    R_FDL_ERR_WRITE = 7,         /**< Flash write error */
    R_FDL_ERR_ERASE = 8,         /**< Flash erase error */
    R_FDL_ERR_BLANKCHECK = 9,    /**< Flash blank check error */
    R_FDL_ERR_COMMAND = 10,      /**< Unknown command */
    R_FDL_ERR_ECC_SED = 11,      /**< Single bit error detected by ECC */
    R_FDL_ERR_ECC_DED = 12,      /**< Double bit error detected by ECC */
    R_FDL_ERR_INTERNAL = 13,     /**< Library internal error */
    R_FDL_CANCELLED = 14         /**< Flash operation is cancelled */
} r_fdl_status_t;


/* 类型定义 */
/* 在app也应该定义这个联合体，在同一个DataFlash地址下修改相对应的值 */
typedef union
{
    uint8_t buff[16];
    struct
    {
        uint8_t jump_flag; /**<< 跳转标志 */ /* JUMP_TRUE->可以跳转,从app跳转到boot时需要将值写为0xA9 */
        uint8_t refresh_flag; /**<< 刷写标志 */ /* REFRESH_FALSE-》刷写失败，不能跳转 */
        uint16_t resp_flag; /**<< boot响应10 02 标志 */ /* 根据上位机的请求，将0x1002或者0x1082写入 */
        uint16_t did_flag; /**<< DID写默认值标志 */ /* DID_TRUE->app将DID默认值写入Flash,写入成功后应将该值写为FOTA_FALSE */
        uint16_t d6_refesh_flag; /**== 0xD6可以刷写,删除时写入0xFF */
        uint32_t jump_boot2_flag;    /* 跳转二级boot */
        uint32_t boot2_refresh_flag; /* 二级boot刷写标志 */
    } infor;
} JUMP_REFRESH_INFOR;
typedef enum
{
	UDSAPI_IGN_OFF = 0,
	UDSAPI_IGN_ON
}ign_state_e;
void uds_dtc_test(void);

/******************************************************************************************************************
 * ****************************************************************************************************************/
/*
 * 函数介绍: UDS初始化函数接口
 * 参数:
 * 返回值: {*}
 * 备注: 系统初始化时调用
 */
void UdsApi_Uds_Initialize(void);

/*
 * 函数介绍: UDS任务函数
 * 参数:
 * 返回值: {*}
 * 备注: 需要循环调用
 */
void UdsApi_Uds_Process(void);

/*
 * 函数介绍: UDS数据发送接口
 * 参数: msgId：CAN/CANFD ID
 *      numBytes:数据长度
 *      pBytes：发送数据
 * 返回值: {*}
 * 备注:
 */
void UdsApi_TXSendData(uint32_t msgId, uint8_t numBytes, uint8_t *pBytes);

/*
 * 函数介绍: UDS接收函数
 * 参数: msgId：CAN/CANFD ID
 *      msgLen:数据长度
 *      pMsgData：接收数据
 * 返回值: {*}
 * 备注:
 */
uint8_t UdsApi_ReciveData(uint32_t msgId, uint8_t msgLen, uint8_t *pMsgData);
/*
 * 函数介绍: UDS 10ms任务函数
 * 参数:
 * 返回值: {*}
 * 备注: 需要循环调用
 */
void UdsApi_10ms_Proc(void);
void UdsApi_1ms_Proc(void);

void UdsApi_D6_upgrage_Deinit(void);
void UdsApi_D6_upgrage_Check(void);
/*
 * 函数介绍: UDS timer
 * 参数:
 * 返回值: {*}
 * 备注: 1ms任务中调用
 */
void UdsApi_TimeoutTimeer(void);
/*
 * 函数介绍: 设置诊断请求标志
 * 参数: reqFlag：诊断请求标志
 * 返回值: {*}
 * 备注: 用于网络管理睡眠唤醒
 */
void UdsApi_setDiagFrameReqFlag(uint8_t reqFlag);
/*
 * 函数介绍: 获取诊断请求标志
 * 参数:
 * 返回值: 诊断请求状态
 * 备注:
 */
uint8_t UdsApi_getDiagFrameReqFlag(void);
/*
 * 函数介绍: 清楚boot跳转标志
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UdsApi_clearBootFlag(uint8_t sub);
/*
 * 函数介绍:设置10会话模式
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UdsApi_SetRub_0x10Program_SubFunc(uint8_t subFunc);
/*
 * 函数介绍: 编程会话处理函数
 * 参数:
 * 返回值: {*}
 * 备注: 10ms周期调用
 */
void UDS_FN_TYPE UdsApp_10Dsc_Programing_Session_Handler(void);
/*
 * 函数介绍: 获取uds地址
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint32_t UdsApi_GetAddrMode(void);
/*
 * 函数介绍:flash擦除写入封装函数
 * 参数: index：    需要操作的flash地址序列号
 *       databuf：  写入的数据
 *       len    ：  写入的长度
 * 返回值: R_FDL_OK：操作成功
 * 备注:
 */
r_fdl_status_t Uds_Api_fdl_Ereas_Write(uint8_t index, const uint8_t *databuf, uint32_t len);
/*
 * 函数介绍: flash读操作函数接口
 * 参数:  index：    需要操作的flash地址序列号
 *       databuf：  写入的数据
 *       len    ：  写入的长度
 * 返回值: R_FDL_OK：操作成功
 * 备注:
 */
r_fdl_status_t Uds_Api_fdl_FlashRead(uint8_t index, uint8_t *databuf, uint32_t len);
/*
 * 函数介绍: 接收中断回调函数
 * 参数: can_recv_st：can接收结构体
 * 返回值: {*}
 * 备注:
 */
void UdsApi_ReceiveCallBack(hal_can_pdu_st * can_recv_st);
/**
 * 函 数 介 绍 ：由编程会话请求，写入标志后系统复位
 * 参       数 ：无
 * 返   回  值 ：无
 * 备       注 ：无
 */
void UdsApi_APP_JumpToBoot(void);
/*
 * 函数介绍: 复位接口
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void UdsApi_Hardware_Reset(void);

#if 1
/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UdsApi_getIgnstateValid(void);
/*
 * 函数介绍:
 * 参数:
 * 返回值: {*}
 * 备注:
 */
uint8_t UdsApi_getIgnstate(void);
#endif
/*
 * 函数介绍: uds数据发送完成回调函数
 * 参数: id：发送完成的CAN id
 * 返回值: {*}
 * 备注:
 */
void UdsApi_Can_TxConfirm_cbk(uint32_t id);
/*
 * 函数介绍: 获取APP报文接收状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_get_normal_rx_sta(void);
/*
 * 函数介绍: 获取APP报文发送状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_get_normal_tx_sta(void);
/*
 * 函数介绍: 获取NM报文接收状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_get_nm_rx_sta(void);
/*
 * 函数介绍: 获取NM报文发送状态
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_get_nm_tx_sta(void);
/*
 * 函数介绍: 设置车速信息
 * 参数:
 * 返回值:
 * 备注:km/h
 */
void UdsApi_setVehspdInfo(uint16_t vehspd);
/*
 * 函数介绍: 设置电压信息
 * 参数:
 * 返回值:
 * 备注:单位V
 */
void UdsApi_setBattInfo(uint16_t BattVol);
/*
 * 函数介绍: 获取车速信息
 * 参数:
 * 返回值: 车速信息
 * 备注:单位km/h
 */
uint16_t UdsApi_getVehspdInfo(void);
/*
 * 函数介绍: 获取电压信息
 * 参数:
 * 返回值: 电压信息
 * 备注:单位v
 */
uint8_t UdsApi_getBattInfo(void);
/*
 * 函数介绍: 设置车速有效信号
 * 参数:
 * 返回值:
 * 备注:
 */
void UdsApi_setVehSpdst(uint8_t VehSpdst);
/*
 * 函数介绍: 获取车速有效信号
 * 参数:
 * 返回值: 车速信息
 * 备注:单位km/h
 */
uint8_t UdsApi_getVehSpdst(void);
/*
 * 函数介绍: 设置0x11E报文超时标志
 * 参数:
 * 返回值:
 * 备注:
 */
void UdsApi_set11EtimeoutFlg(uint8_t timeoutflg);
/*
 * 函数介绍: 获取0x11E报文超时标志
 * 参数:
 * 返回值: 车速信息
 * 备注:单位km/h
 */
uint8_t UdsApi_get11EtimeoutFlg(void);
/*
 * 函数介绍: 检查boot跳转条件是否满足
 * 参数:
 * 返回值: 返回状态值，TRUE:允许；FALSE:禁止
 * 备注:
 */
uint8_t UdsApi_Check_Bootjump_Condition(void);
/*
 * 函数介绍: 获取D6刷写标志
 * 参数:
 * 返回值:  1：开始刷写D6底层发送接口允许发送数据
 * 		  0：D6刷写完成或失败，底层发送接口需要检测网络管理状态来决定是否能发送报文
 *
 *
 * 备注:
 */
uint8_t UdsApi_GetD6UpradeRefreshFlg(void);
#endif
