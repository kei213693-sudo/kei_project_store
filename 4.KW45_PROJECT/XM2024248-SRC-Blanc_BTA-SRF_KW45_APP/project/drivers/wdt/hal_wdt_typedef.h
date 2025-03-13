/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_xx_typedef.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述xx模块专用自定义类型，需要根据实际情况判定是否是要需改。
 *      文件调用：
 *                  文件应在模块内部产生直接调用，外部使用需通过hal_xx_if.h 或 hal_xx.h
 *              间接调用。
 *                  文件应包含hal_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

#ifndef HAL_WDT_TYPEDEF_H
#define HAL_WDT_TYPEDEF_H

/*  Include     Begine      */
#include "hal_typedef.h"
/*  Include     End         */

/**
 * 看门狗复位时间
 * HAL_WDT_SPEED_SLOW           60s
 * HAL_WDT_SPEED_MEDI           10s
 * HAL_WDT_SPEED_FAST           1s
*/
typedef enum
{
    HAL_WDT_SPEED_FAST = 0,
    HAL_WDT_SPEED_MEDI ,
    HAL_WDT_SPEED_SLOW ,
} hal_wdt_speed_et;

/*  Base Type  Begine      */

/*  Base Type  End         */

/*  Macro       Begine      */

/*  Macro       End         */

/*  Enum        Begine      */

/*  Enum        End         */

/*  Struct/union Begine      */

/*  Struct/union End         */

#endif
