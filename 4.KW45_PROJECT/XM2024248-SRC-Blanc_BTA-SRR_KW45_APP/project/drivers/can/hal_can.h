/***
 * @Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * @All rights reserved.
 * @FilePath: hal_can.h
 * @abstract:
 * @Current_Version:
 * @Date:
 * @History_Version: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#ifndef HAL_CAN_H
#define HAL_CAN_H

#include "hal_can_cfg.h"
#include "hal_typedef.h"
#include "hal_can_typedef.h"
/*
 * 函 数 介 绍: CAN初始化接口
 * 参       数 [in]channel      CAN通道号
 *             [in]can_cfg      CAN配置信息结构体
 *
 * 返   回  值: = 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注: 1、通道号需要对应映射到CAN硬件通道号上，初始化配置在hal_can_cfg.c中g_cancfg_arr_s去配置。
 *              2、CAN波特率根据采样点和CAN时钟来计算，fCAN由时钟配置那边决定，这里只需配置好tq就行。
 *              举例：NTS2=16; NTS1=63; NSJW=16; NBRP=0，Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/4/5 = 2Mbps
 */
hal_result_t hal_can_init(uint8_t channel);
#ifdef HAL_CAN_FDFRAME
/*
 * 函 数 介 绍: CANFD设置TDC
 * 参       数  [in]channel    CAN通道号
 *              [in]TDC_offset TDC偏移量，具体寄存器需要根据芯片手册配置}
 *              [in]TDC_enable TDC使能标签，这里定义0:Disable,1:enable;具体寄存器配置需要根据芯片手册配置
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:
 */
hal_result_t hal_can_fd_set_tdc(uint8_t channel, uint8_t tdc_offset, hal_enabled_et tdc_enabled);
#endif
/*
 * 函 数 介 绍: 配置仲裁域tq
 * 参       数
 *              [in]channel         CAN通道号
 *
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:                 1、波特率根据传入的tq配置来计算
 *                              举例：NTS2=16; NTS1=63; NSJW=16; NBRP=0，Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/4/5 = 2Mbps
 *                              2、仲裁域tq可通过hal_can_cfg.h中g_canatq_arr_sa结构体数据进行配置，实现过程中直接使用该结构体内容即可
 *
 */

hal_result_t hal_can_set_arbitratetq(uint8_t channel);
/*
 * 函 数 介 绍: 配置数据域tq
 * 参       数
 *              [in]channel           CAN通道号
 *              [in]can_dtq           tq配置结构体指针,该指针实例化可以在hal_can_cfg.c中配置}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:                 波特率根据传入的tq配置来计算
 *                              举例：NTS2=16; NTS1=63; NSJW=16; NBRP=0，Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/4/5 = 2Mbps
 */
hal_result_t hal_can_set_datatq(uint8_t channel, hal_can_tq_st *can_dtq);
/*
 * 函 数 介 绍: CAN过滤配置
 * 参       数
 *              [in]channel            CAN通道号
 *              [in]can_filter_arr     过滤数组指针，详情请参考hal_can_filter_st
 *              [in]filterarr_len   过滤数组长度,由宏定义HAL_CAN_FILTER[n]_LEN决定，在hal_can_cfg.h中定义
 *                                   n与CAN通道号保持一致，举例CAN0,n=0}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:每个CAN通道号对应一个hal_can_filter_st数组和数组长度宏定义，传参的时候注意对应起来
 */
hal_result_t hal_can_filter_config(uint8_t channel, hal_can_filter_st *can_filter_arr, uint16_t filterarr_len);
/*
 * 函 数 介 绍: CAN 发送接口
 * 参       数
 *             [in]channel              CAN通道号
 *             [in]can_trans_pdu        CAN发送结构体，怎样填充详见hal_can_pdu_st}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 *
 * 备       注:
 */
hal_result_t hal_can_write(uint8_t channel, hal_can_pdu_st *can_trans_pdu);
/*
 * 函 数 介 绍: CAN接收接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]can_recv_pdu        CAN接收结构体，怎样填充详见hal_can_pdu_st}}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:
 */
hal_result_t hal_can_read(uint8_t channel, hal_can_pdu_st *can_recv_pdu);
/*
 * 函 数 介 绍: CAN接收回调接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]recv_func           回调函数，传参参考hal_can_recv_cbk_t[hal_can_typedef.h]}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: 该接口需要在CAN接收地方调用
 */
hal_result_t hal_can_recv_callback(uint8_t channel, hal_can_recv_cbk_t recv_func);
/*
 * 函 数 介 绍: busoff回调函数配置
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]busoff_func         busoff回调函数
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: busoff触发后调用该函数
 */
hal_result_t hal_can_busoff_callback(uint8_t channel, hal_can_busoff_cbk_t busoff_func);
/*
 * 函 数 介 绍: 配置传输模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]trcvmode            传输模式,详见hal_can_trcvmode_et
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_set_trcv_mode(uint8_t channel, hal_can_trcvmode_et trcvmode);
/*
 * 函 数 介 绍: 获取CAN传输模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]trcvmode            CAN传输模式指针}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_get_trcvmode(uint8_t channel, hal_can_trcvmode_et *trcvmode);
/*
 * 函 数 介 绍: 配置CAN控制模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]ctrl_mode           控制模式，详见hal_can_ctrmode_et
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: 实际修改对应寄存器的值还请对照芯片数据手册来配置
 */
hal_result_t hal_can_set_ctrlmode(uint8_t channel, hal_can_ctrmode_et ctrlmode);
/*
 * 函 数 介 绍: 获取CAN控制模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]ctrmode             CAN控制模式指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_get_ctrlmode(uint8_t channel, hal_can_ctrmode_et *ctrmode);
/*
 * 函 数 介 绍: 获取CAN发送错误计数器计数
 * 参       数
 *              [in]channel          CAN通道号
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_get_tec(uint8_t channel, uint8_t *tec);
/*
 * 函 数 介 绍: 获取CAN接收错误计数器计数
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]p_u8_rec            获取CAN接收错误计数器的指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_get_rec(uint8_t channel, uint8_t *rec);

/*
 * 函 数 介 绍: 配置CAN发送完成回调函数
 * 参       数
 *              [in]channel          CAN通道号
 *              [in]tx_cbk           回调函数指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_tx_callback(uint8_t channel, hal_can_tx_cbk_t tx_cbk);
/*
 * 函 数 介 绍: CAN去初始化函数
 * 参       数
 *
 * 返   回  值:
 * 备       注:
 */
void hal_can_deinit(void);


#endif
