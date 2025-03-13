/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_can_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_can_if.h"
#include "hal_can.h"
#include "hal_can_typedef.h"
#include "app_dtc.h"
/*
 * 函 数 介 绍: CAN初始化接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]can_cfg             CAN配置信息结构体
 *
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注: 该接口为应用层调用，为hal_can_init底层函数的再封装，里面已经对CAN模块一些基本信息
 *              进行了初始化操作，应用层在使用CAN模块功能时调用该模块即可
 */
hal_result_t hal_can_if_init(uint8_t channel)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_init(channel);

    return l_i8_result;
}

/*
 * 函 数 介 绍: CAN 发送接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]can_trans_pdu       CAN发送结构体，怎样填充详见hal_can_pdu_st}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 *
 * 备       注:
 */
hal_result_t hal_can_if_write(uint8_t channel, hal_can_pdu_st *can_trans_pdu)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;
    if(Get_PMState() == PMS_NORMAL)
    {
    	l_i8_result = hal_can_write(channel, can_trans_pdu);
    }else
    {
    	l_i8_result =  HAL_RESULT_FAIL;
    }


    return l_i8_result;
}
/*
 * 函 数 介 绍: CAN接收接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]can_recv_pdu        CAN接收结构体，怎样填充详见hal_can_pdu_st}}
 * 返   回  值: {= 0：操作成功，<0:操作失败,详见hal_typedef.h错误码组定义
 * 备       注:
 */
hal_result_t hal_can_if_read(uint8_t channel, hal_can_pdu_st *can_recv_pdu)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_read(channel, can_recv_pdu);

    return l_i8_result;
}
/*
 * 函 数 介 绍: CAN接收回调接口
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]recv_func           回调函数，传参参考hal_can_recv_cbk_t[hal_can_typedef.h]
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: 该接口需要在CAN接收地方调用
 */
hal_result_t hal_can_if_recv_callback(uint8_t channel, hal_can_recv_cbk_t recv_func)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_recv_callback(channel, recv_func);

    return l_i8_result;
}
/*
 * 函 数 介 绍: busoff回调函数配置
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]busoff_func         busoff回调函数
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注: busoff触发后调用该函数
 */
hal_result_t hal_can_if_busoff_callback(uint8_t channel, hal_can_busoff_cbk_t busoff_func)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_busoff_callback(channel, busoff_func);

    return l_i8_result;
}
/*
 * 函 数 介 绍: 配置传输模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]trcvmode            传输模式,详见hal_can_trcvmode_et}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_set_trcv_mode(uint8_t channel, hal_can_trcvmode_et trcvmode)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_set_trcv_mode(channel, trcvmode);

    return l_i8_result;
}
/*
 * 函 数 介 绍: 获取CAN传输模式
 * 参       数
 *              [in]channel             CAN通道号
 *              [in]trcvmode            CAN传输模式指针}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_get_trcvmode(uint8_t channel, hal_can_trcvmode_et *trcvmode)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_get_trcvmode(channel, trcvmode);

    return l_i8_result;
}
/*
 * 函 数 介 绍: 配置CAN控制模式
 * 参       数
 *              [in]channel                 CAN通道号
 *              [in]ctrl_mode               控制模式，详见hal_can_ctrmode_et
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_set_ctrlmode(uint8_t channel, hal_can_ctrmode_et ctrlmode)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_set_ctrlmode(channel, ctrlmode);

    return l_i8_result;
}
/*
 * 函 数 介 绍: 获取CAN控制模式
 * 参       数
 *              [in]channel                 CAN通道号
 *              [in]ctrmode                 CAN控制模式指针}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_get_ctrlmode(uint8_t channel, hal_can_ctrmode_et *ctrmode)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_get_ctrlmode(channel, ctrmode);

    return l_i8_result;
}
/*
 * 函 数 介 绍: 获取CAN发送错误计数器计数
 * 参       数
 *              [in]channel          CAN通道号
 *              [in]tec              获取CAN接收错误计算器的指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_get_tec(uint8_t channel, uint8_t *tec)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_get_tec(channel, tec);

    return l_i8_result;
}
/*
 * 函 数 介 绍: 获取CAN接收错误计数器计数
 * 参       数
 *              [in]channel          CAN通道号
 *              [in]rec              获取CAN接收错误计数器的指针}}
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_get_rec(uint8_t channel, uint8_t *rec)
{
    hal_result_t l_i8_result = HAL_RESULT_SUCCESS;

    l_i8_result = hal_can_get_rec(channel, rec);

    return l_i8_result;
}
/*
 * 函 数 介 绍: 配置CAN发送完成回调函数
 * 参       数
 *              [in]channel          CAN通道号
 *              [in]tx_cbk           回调函数指针
 * 返   回  值: {hal_result_t:=0操作成功，其他返回值类型详见hal_typedef.h错误码组定义}
 * 备       注:
 */
hal_result_t hal_can_if_tx_callback(uint8_t channel, hal_can_tx_cbk_t tx_cbk)
{
	hal_result_t result = HAL_RESULT_SUCCESS;

	result = hal_can_tx_callback(channel, tx_cbk);

	return result;
}
/*
 * 函 数 介 绍: CAN去初始化函数
 * 参       数
 *
 * 返   回  值:
 * 备       注:
 */
void hal_can_if_deinit(void)
{
	hal_can_deinit();
}
