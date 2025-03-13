/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_can_if.h
 * 摘要：
 *          文件主要描述clock模块
 * 当前版本：
 * 历史版本：
 */
#ifndef HAL_CAN_IF_H
#define HAL_CAN_IF_H

/*  Include Begine  */
#include "hal_typedef.h"
#include "hal_can_typedef.h"
/*  Include End  */

/*  Typedef Begine  */
/*
 * 函 数 介 绍: CAN初始化接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]can_cfg             CAN配置信息结构体
 *
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注: 1、通道号需要对应映射到CAN硬件通道号上，初始化配置can_cfg_st在hal_can_cfg.c中去配置。
 *              2、CAN波特率根据采样点和CAN时钟来计算，fCAN由时钟配置那边决定，这里只需配置好tq就行。
 *              举例：NTS2=16; NTS1=63; NSJW=16; NBRP=0，Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/4/5 = 2Mbps
 */
hal_result_t hal_can_if_init(uint8_t channel);
/*
 * 函 数 介 绍: CAN 发送接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]can_trans_pdu       CAN发送结构体，怎样填充详见hal_can_pdu_st}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 *
 * 备       注:
 */
hal_result_t hal_can_if_write(uint8_t channel, hal_can_pdu_st *can_trans_pdu);
/*
 * 函 数 介 绍: CAN接收接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]can_recv_pdu        CAN接收结构体，怎样填充详见hal_can_pdu_st}}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:
 */
hal_result_t hal_can_if_read(uint8_t channel, hal_can_pdu_st *can_recv_pdu);
/*
 * 函 数 介 绍: CAN接收回调接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]recv_func           回调函数，传参参考hal_can_recv_cbk_t[hal_can_typedef.h]
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: 该接口需要在CAN接收地方调用
 */
hal_result_t hal_can_if_recv_callback(uint8_t channel, hal_can_recv_cbk_t recv_func);
/*
 * 函 数 介 绍: busoff回调函数配置
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]busoff_func         busoff回调函数
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: busoff触发后调用该函数
 */
hal_result_t hal_can_if_busoff_callback(uint8_t channel, hal_can_busoff_cbk_t busoff_func);
/*
 * 函 数 介 绍: 配置传输模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]trcvmode            传输模式,详见hal_can_trcvmode_et}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_set_trcv_mode(uint8_t channel, hal_can_trcvmode_et trcvmode);
/*
 * 函 数 介 绍: 获取CAN传输模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]trcvmode            CAN传输模式指针}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_get_trcvmode(uint8_t channel, hal_can_trcvmode_et *trcvmode);
/*
 * 函 数 介 绍: 配置CAN控制模式
 * 参       数
 *              [in]channel                 CAN通道号
 *              [in]ctrl_mode               控制模式，详见hal_can_ctrmode_et
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_set_ctrlmode(uint8_t channel, hal_can_ctrmode_et ctrlmode);
/*
 * 函 数 介 绍: 获取CAN控制模式
 * 参       数
 *              [in]channel                 CAN通道号
 *              [in]ctrmode                 CAN控制模式指针}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_get_ctrlmode(uint8_t channel, hal_can_ctrmode_et *ctrmode);
/*
 * 函 数 介 绍: 获取CAN发送错误计数器计数
 * 参       数
 *              [in]channel          CAN通道号
 *              [in]tec              获取CAN接收错误计算器的指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_get_tec(uint8_t channel, uint8_t *tec);
/*
 * 函 数 介 绍: 获取CAN接收错误计数器计数
 * 参       数
 *              [in]channel          CAN通道号
 *              [in]rec              获取CAN接收错误计数器的指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_get_rec(uint8_t channel, uint8_t *rec);
/*
 * 函 数 介 绍: 配置CAN发送完成回调函数
 * 参       数
 *              [in]channel          CAN通道号
 *              [in]tx_cbk           回调函数指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_tx_callback(uint8_t channel, hal_can_tx_cbk_t tx_cbk);
/*
 * 函 数 介 绍: CAN去初始化函数
 * 参       数
 *
 * 返   回  值:
 * 备       注:
 */
void hal_can_if_deinit(void);
/*  Typedef End  */

#endif
