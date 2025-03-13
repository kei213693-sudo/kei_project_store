/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_can_cfg.c
 * 摘要：
 *          文件主要描述clock模块
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

/*  Include Begine  */
#include "hal_can_cfg.h"
#include "hal_can_typedef.h"
/*  Include End*/

/*
**CAN0过滤配置
*/

hal_can_filter_st g_canfilter0_arr_sa[HAL_CAN_FILTER0_LEN] = {
    // {0x103, 0x103, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CAN},//sniffer用
    {0x104, 0x104, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CAN},
	{0x110, 0x110, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CAN},
    {0x111, 0x111, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CAN},
	{0x112, 0x112, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CAN},
    {0x73A, 0x73A, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CAN},
	{0x560, 0x560, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CAN},
    {0x561, 0x561, HAL_CAN_ID_TYPE_StAND, HAL_CAN_FRAME_TYPE_CAN}//ble节点用
};

/**
 *
 * 数据场tq配置
 */
hal_can_tq_st g_candtq_arr_sa[HAL_CAN_CHANNEL_NUM] = {
    {
        1,  // sync_seg
        7, // pro_seg
        3, // tq1
        3, // tq2
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
        16, // pro_seg
        6, // tq1
        6, // tq2
        6, // sjw
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

void hal_can_get_cfg(hal_can_cfg_st **adccfg, uint32_t *len)
{
      *len = HAL_CAN_CHANNEL_NUM;
      *adccfg = g_cancfg_arr_sa;
}
