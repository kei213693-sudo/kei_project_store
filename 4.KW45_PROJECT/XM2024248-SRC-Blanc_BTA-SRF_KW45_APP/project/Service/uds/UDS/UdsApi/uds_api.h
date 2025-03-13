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
	 uint8_t buff[32];
    struct
    {
        uint8_t jump_flag; /**<< 跳转标志 */ /* JUMP_TRUE->可以跳转,从app跳转到boot时需要将值写为0xA9 */
        uint8_t refresh_flag; /**<< 刷写标志 */ /* REFRESH_FALSE-》刷写失败，不能跳转 */
        uint16_t resp_flag; /**<< boot响应10 02 标志 */ /* 根据上位机的请求，将0x1002或者0x1082写入 */
        uint16_t did_flag; /**<< DID写默认值标志 */ /* DID_TRUE->app将DID默认值写入Flash,写入成功后应将该值写为FOTA_FALSE */
	        uint16_t d6_flag;                  /* d6刷写标志 */
        uint32_t jump_boot2_flag;    /* 跳转二级boot */
        uint32_t boot2_refresh_flag; /* 二级boot刷写标志 */
	        uint32_t erasureAddr;           /* 二级boot擦除地址 */
	        uint32_t erasureLen;             /* 二级boot擦除字节长度 */
    } infor;
} JUMP_REFRESH_INFOR;
typedef enum
{
	UDSAPI_IGN_OFF = 0,
	UDSAPI_IGN_ON
}ign_state_e;
/**
 * KW45 reset source
 * */
typedef struct
{
	uint8_t d6_reset_data[10];
	uint8_t data_len;
}d6_restsource_st;
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
/*
 * 函数介绍: 设置kw45复位源
 * 参数:resetData：复位源数据
 * 返回值:
 * 备注:
 * 	0x01=WAKEUP Wake-up Reset/唤醒复位
	0x02=POR Power-on Reset/Power-on复位
	0x03=LVD Low Voltage Detect Reset/低电压检测复位
	0x04=HVD High Voltage Detect Reset/高电压检测复位
	0x05=WARM Warm Reset/热复位
	0x06=FATAL Fatal Reset/Fatal复位
	0x08=Reset Pin Reset/复位管脚复位
	0x09=DAP Debug Access Port Reset/Debug管脚复位
	0x0A=RSTACK Reset Timeout/系统复位生成逻辑中出现超时或其他错误条件导致复位
	0x0B=LPACK Low Power Acknowledge Timeout Reset/低功耗模式进入逻辑中的超时导致的复位
	0x0C=SCG System Clock Generation Reset/SCG 中的时钟丢失或锁定丢失事件导致复位
	0x0D=WDOG0 Watchdog 0 Reset/看门狗0超时复位
	0x0E=SW Software Reset/ ARM内核SYSRESETREQ软件复位
	0x0F=Lockup Reset/ARM内核指示LOCKUP事件导致复位
	0x10=WDOG1 Watchdog 1 Reset/看门狗1超时复位
	0x11=SECVIO Security Violation Reset/安全违规逻辑导致复位
 */
void UdsApi_set_KW45resetSource(uint8_t* resetData);
/*
 * 函数介绍: 设置D6复位源
 * 参数:resetData：复位源数据
 * 返回值:
 * 备注:
 * 	0x01=brownout on the CPU retention power domain/CPU 保留电源域上的欠压故障
0x02=toggling the reset pin during deep power down/在深度掉电期间触发复位引脚
0x03=last reset was due to EDC/上一次重置由EDC引起
0x04=last reset was due to failure detected by SRAM ECC/上次重置是由于 SRAM ECC 检测到故障
0x05=last reset was due to failure detected by Frequency monitoring/上次重置是由于频率监控检测到故障
0x06=Reset caused by power failure of IO power domain/IO 电源域断电导致复位
0x07=Reset caused by power failure of IO power domain related to NV operation/与 NV 操作有关的 IO 电源域电源故障导致复位
0x08=Reset caused by power failure on the bias bandgap/由于偏置带隙上的电源故障导致的复位
0x09=Reset caused by power failure on the CPU retention power domain/CPU 保留电源域电源故障导致复位
0x0A=Reset caused by power failure on the non volatile (page flash) power domain(NV_POK)/非易失性电源域断电导致的复位
0x0B=last reset was due to glitch detector/上次重置是由于探测器故障
0x0C=last reset was due to Analog glitch sensor/上次重置是由于模拟瞬态检测器触发
0x0D=Reset caused by toggling the reset pin during hard power down/在硬掉电期间触发复位引脚导致复位
0x0E=Reset caused by toggling the wake up pin during hard power down/在硬掉电期间触发唤醒引脚导致复位
0x0F=last reset was due to noise generation and fault injection detection/上次重置是由于噪声产生和故障注入检测
0x10=last reset was due to INGFID/上一次重置是由于INGFID
0x11=last reset was due to failure detected by local supply drop detector/上次重置是由于本地供电中断检测器检测到故障
0x12=Indicates a Power on reset (POR)/开机复位 (POR)
0x13=Reset caused by toggling the reset pin/通过触发复位引脚引起复位
0x14=last reset was due to failure detected by secure fetch/上次重置是由于安全获取检测到故障
0x15=Reset caused by software master RESET/由软件主控 RESET 引起的复位
0x16=Indicate the last reset was due to System reset request/表示上次重置是由于系统重置请求
0x17=last reset was due to failure detected by violation detector/上次重置是由于违规检测器检测到故障
0x18=Reset caused by the watchdog timer/看门狗定时器引起的复位
 */
void UdsApi_set_D6resetSource(uint8_t* resetData);
/*
 * 函数介绍: 设置蓝牙广播状态
 * 参数:state = 0x01:打开，=0x00:关闭
 * 返回值:
 * 备注:
 */
void UdsApi_set_BleTurnOnState(uint8_t state);
/*
 * 函数介绍: 设置蓝牙广播状态
 * 参数:
 * 返回值: 状态信息，state = 0x01:打开，=0x00:关闭
 * 备注:
 */
uint8_t UdsApi_get_BleTurnOnState(void);
#endif
