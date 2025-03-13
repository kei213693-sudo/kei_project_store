/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_can_cfg.h
 * 摘要：
 *          文件主要描述clock模块
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
*/
#ifndef HAL_CAN_CFG_H
#define HAL_CAN_CFG_H

/*  Include Begine  */
#include "hal_typedef.h"
#include "hal_can_typedef.h"

/*  Include End  */
/*CAN通道映射关系，后面的值开发者可根据芯片进行更改*/
// #define HAL_CAN_CHANNEL0     (0U)
// #define HAL_CAN_CHANNEL1     (1U)
// #define HAL_CAN_CHANNEL_NUM  (2U)

//过滤数组长度定义
#define HAL_CAN_FILTER0_LEN  (8U)			//最大32

#define HAL_CAN_FDFRAME     1

//#define USE_TIMINGCONFIG

#define CAN0_BAUDRATE		(1000000)
#define CAN0FD_BAUDRATE		(2000000)

/* Typedef Begine*/
/**
 * 初始化配置
 * m_filter_s                   CAN ID过滤配置，详见hal_can_filter_st
 * m_u16_filtarrlen             过滤数组长度，赋值为HAL_CAN_FILTER_LEN，此宏定义为过滤数组长度
 *                              HAL_CAN_FILTER_LEN定义在han_can_cfg.h中  
 * m_u8_fdtdcofset              采样延时偏移量
 * m_fdtdcenable_e              采样延时使能标签，如不需要配TDC，则配置为HAL_ENABLED_DISABLE
 * m_atqstr_s                   仲裁域tq配置结构体指针
 * m_dtqstr_s                   数据域tq配置结构体指针
 * m_trcvmode_e                 初始化完成传输模式定义为CANTRCV_TRCVMODE_NORMAL,如未初始化完成则定义
 *                              为CANTRCV_TRCVMODE_UNINIT，默认为CANTRCV_TRCVMODE_UNINIT
 * m_ctrlmode_e                 默认为ControllerMode_UNINIT，初始化完成则定义为ControllerMode_STARTED
 * 
 * 备注：                       波特率根据传入的tq配置来计算
 *                              举例：NTS2=16; NTS1=63; NSJW=16; NBRP=0，Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/4/5 = 2Mbps
 */
typedef struct
{
    hal_can_channel_et m_channel_e;
    hal_can_filter_st *m_filter_s;
    uint16_t m_u16_filtarrlen;
#ifdef HAL_CAN_FDFRAME
    uint8_t m_u8_fdtdcofset;
    hal_enabled_et m_fdtdcenable_e;
#endif
    hal_can_tq_st *m_atqstr_s;
    hal_can_tq_st *m_dtqstr_s;
    hal_can_trcvmode_et m_trcvmode_e;
    hal_can_ctrmode_et m_ctrlmode_e;
} hal_can_cfg_st;

/**
 * CAN ID过滤数组声明，长度可配置,数组配置信息详见hal_can_filter_st[hal_can_cfg.h]
 */
//const  hal_can_filter_st g_canfilter0_arr_sa[HAL_CAN_FILTER0_LEN];
/**
 * CAN ID过滤数组声明，长度可配置,数组配置信息详见hal_can_filter_st[hal_can_cfg.h]
 */
//const  hal_can_filter_st g_canfilter1_arr_sa[HAL_CAN_FILTER1_LEN];


/**
 * 
 * 初始化配置数组声明,配置详见hal_can_cfg_st[hal_can_cfg.h]
 * 
*/
//const  hal_can_cfg_st g_cancfg_arr_sa[HAL_CAN_CHANNEL_NUM];

/**
 * 
 * 仲裁场tq配置，数组下标0对应CAN CHL0通道，1对应CAN CHL1通道，以此类推
 * 
*/
//const hal_can_tq_st   g_canatq_arr_sa[HAL_CAN_CHANNEL_NUM];
/**
 * 
 * 数据场tq配置
*/
//const hal_can_tq_st   g_candtq_arr_sa[HAL_CAN_CHANNEL_NUM];
/* Typedef End*/

void hal_can_get_cfg(hal_can_cfg_st **adccfg, uint32_t *len);
#endif
