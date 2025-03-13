/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * @FilePath: D6Update.h
 * @abstract:
 * @Current_Version: V1.0.0
 * @Date: 2024-05-10 10:00:00
 * @History_Version: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#ifndef D6_UPDATE_H
#define D6_UPDATE_H

#include<stdint.h>

typedef enum
{
    D6UP_IDLE = 0,
    D6UP_ACTSWUP,
    D6UP_TRSELKEY,
    D6UP_TRFEST,
    D6UP_STUPDATE,
    D6UP_TRPONENT,
    D6UP_VRCOMPONENT,
    D6UP_VRALL,
    D6UP_FINISH,
    D6UP_DEACTIVE,
    D6UP_GETINF,
    D6UP_ERROR
}D6UpSta;
typedef struct
{
    uint8_t sig0[11];
    uint8_t sig1[2];
    uint8_t sig2[6];
}D6PriviteInfo;
#define D6SWUP_FAIL (0U)   /** 失败  */
#define D6SWUP_BUSY (1U)   /** 处理中  */
#define D6SWUP_FINISH (2U) /** 完成  */

/**
 * 函 数 介 绍: 开始升级流程
 *
 * 参       数: 文件地址
 * 参       数: mode 1 正常启动
 * 参       数: mode 0 错误后自动恢复
 * 返   回  值: uint8_t 1 成功
 * 返   回  值: uint8_t 0 失败
 */
uint8_t D6SWUPStart(uint8_t *path, uint8_t mode);

/**
 * 函 数 介 绍: 更新主循环
 *
 */
void D6SWUPEntry(void);

/**
 * 函 数 介 绍: 获取结果
 *
 * 返   回  值: uint8_t 0 失败
 * 返   回  值: uint8_t 1 处理中
 * 返   回  值: uint8_t 2 完成
 */
uint8_t D6SWUPGetSta(void);

/**
 * 函 数 介 绍: 内部定时器
 *
 * 参       数:
 * 返   回  值:
 * 返   回  值:
 */
void D6SWUPTICK(void);

D6PriviteInfo* D6SWUPGetInfo(void);
#endif
/** EOF */

