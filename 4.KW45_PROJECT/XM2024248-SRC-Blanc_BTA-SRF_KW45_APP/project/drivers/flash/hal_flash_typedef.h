/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_flash_typedef.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述flash模块专用自定义类型，需要根据实际情况判定是否是要需改。
 *      文件调用：
 *                  文件应在模块内部产生直接调用，外部使用需通过hal_flash_if.h 或 hal_flash.h
 *              间接调用。
 *                  文件应包含hal_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */

#ifndef HAL_flash_TYPEDEF_H
#define HAL_flash_TYPEDEF_H

/*  Include     Begine      */
#include "hal_typedef.h"
/*  Include     End         */

// typedef enum{
//     FAIL = 0,
//     SUCCESS,
// }HAL_FLASH_RESULT_et;
/**
 * 与flash里的内容比较结果
 *
 * HAL_FLASH_COMPARE_SAME               相同
 * HAL_FLASH_COMPARE_DIFF               不同
*/
typedef enum
{
    HAL_FLASH_COMPARE_SAME = 0,
    HAL_FLASH_COMPARE_DIFF,
		HAL_FLASH_COMPARE_ERROR
} hal_flash_compare_et;

/**
 * 检查flash是否第一次写
 *
 * HAL_FLASH_CHECK_NOT_OPT              检查的flash没有写过
 * HAL_FLASH_CHECK_OPT                  检查的flash已经写过
*/
typedef enum
{
    HAL_FLASH_CHECK_NOT_OPT = 0,
    HAL_FLASH_CHECK_OPT,
		HAL_FLASH_CHECK_ERROR
}hal_flash_check_et;

#endif
