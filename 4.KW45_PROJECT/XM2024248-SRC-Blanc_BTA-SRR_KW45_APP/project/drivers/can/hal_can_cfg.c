/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_can_cfg.c
 * 摘要：
 *          文件主要描述clock模块
 * 当前版本：
 * 历史版本：
 */

/*  Include Begine  */
#include "hal_can_cfg.h"
#include "hal_can_typedef.h"
#include "alps_nt3_bta_srr_1229_bl051_v_1_0.h"
/*  Include End*/

/*
**CAN0过滤配置
*/

hal_can_filter_st g_canfilter0_arr_sa[HAL_CAN_FILTER0_LEN] = {
//    {0x103, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//    {0x104, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//	{0x110, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
    {0x111, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x601, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//
    {0x671, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x672, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x673, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x674, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x675, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},

	{0x67c, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x670, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x500, 0x580, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
    {0x13C, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//	{0x296, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x11E, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//
	{0x3AA, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x3B2, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//
	{0x30B, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x30C, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//
	{0x679, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
	{0x67A, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//	{0x781, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//	{0x296, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//	{0x1cc, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},
//	{0x1ca, 0xFFFF, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CANFD},

};

/**
 *
 * 数据场tq配置
 */
hal_can_tq_st g_candtq_arr_sa[HAL_CAN_CHANNEL_NUM] = {
    {
        1,  // sync_seg
        8, // pro_seg
        3, // tq1
        2, // tq2
        3, // sjw
        0  // brp
    }
};
/**
 *
 * 仲裁场tq配置，数组下标0对应CAN CHL0通道，1对应CAN CHL1通道，以此类推
 *
 */
hal_can_tq_st g_canatq_arr_sa[HAL_CAN_CHANNEL_NUM] = {
    {
        1,  // sync_seg
        36, // pro_seg
        12, // tq1
        12, // tq2
        12, // sjw
        0  // brp
    }
};
/*
**初始化结构体实例化
*/
hal_can_cfg_st g_cancfg_arr_sa[HAL_CAN_CHANNEL_NUM] = {
    {
		HAL_CAN0_CHANNEL,
		g_canfilter0_arr_sa,  //过滤配
        HAL_CAN_FILTER0_LEN, //
        0,                   //
        HAL_ENABLED_DISABLE,
        &g_canatq_arr_sa[HAL_CAN0_CHANNEL],
        &g_candtq_arr_sa[HAL_CAN0_CHANNEL],
        CANTRCV_TRCVMODE_UNINIT,
        ControllerMode_UNINIT
		}
};
/**
 * 函 数 介 绍 ：
 *                 获得CAN配置表
 *
 *  * 参       数 ：
 *                  adccfg      获得配置表指针
 *                  len         或者配置表长度
 * 返   回  值 ：
 * 备       注 ：
 *
 */
void hal_can_get_cfg(hal_can_cfg_st **cancfg, uint32_t *len)
{
      *len = HAL_CAN_CHANNEL_NUM;
      *cancfg = g_cancfg_arr_sa;
}
