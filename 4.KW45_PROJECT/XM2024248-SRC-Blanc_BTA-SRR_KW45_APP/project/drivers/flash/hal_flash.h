/**
 * Copyright (C) 2023,宁波福尔达智能科技股份有限公司
 * All rights reserved
 * 文件名称：hal_flash.h
 * 摘要：
 *      文件概述：
 *                  文件主要描述flash模块抽象层接口，不建议修改函数接口。
 *      文件调用：
 *                  文件应被应hal层内部调用。
 *                  文件应包含hal_flash_typedef.h。
 * 当前版本：V0.0.1,易源东，2023-09-01，创建
 * 历史版本：
 */
#ifndef HAL_flash_H
#define HAL_flash_H

/*  Include     Begine      */
#include "hal_flash_typedef.h"
/*  Include     End         */

/**
 * 函 数 介 绍 ：
 *                 dataflash模块进行初始化
 *
 *  * 参       数 ：
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_pfls_init(void);

/**
 * 函 数 介 绍 ：
 *                 dataflash模块擦除函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] datalen      擦除的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_pfls_erase(uint8_t index, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 dataflash模块读取函数
 *
 *  * 参       数 ：
 *                  [in] index     通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer    读取数据存放BUFF
 *                  [in] datalen       读取的长度
 * 返   回  值      hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，读取位置不能超出flash大小
 */
hal_result_t hal_pfls_read(uint8_t index, uint8_t *buffer, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 dataflash模块擦写函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        写入的数据
 *                  [in] l_u32_eraselen     擦除的长度
 *                  [in] datalen      写入的长度
 * 返   回  值  hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_pfls_erase_write(uint8_t index, const uint8_t *buffer, uint32_t l_u32_eraselen, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 dataflash模块写函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        写入的数据
 *                  [in] datalen      写入的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_pfls_write(uint8_t index, const uint8_t *buffer, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 dataflash模块对比函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        比较的数据
 *                  [in] datalen      比较的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_flash_compare_et hal_pfls_compare(uint8_t index, const uint8_t *buffer, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 dataflash在写之前检查整个flash是否已被清除
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *
 *                  [in] datalen      检查长度
 *
 * 返   回  值 hal_result_t   hal_result_t   =0 :空白检查成功，=-10 :空白检查失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_flash_check_et hal_pfls_blank_check(uint8_t index, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 codeflash模块初始化函数
 *
 *  * 参       数 ：

 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
*/
hal_result_t hal_cfls_init(void);

/**
 * 函 数 介 绍 ：
 *                 cflash模块擦除函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *
 *                  [in] datalen      擦除的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_erase(uint8_t index, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 codeflash模块读取函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        读取的数据
 *                  [in] datalen      读取的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，读取位置不能超出flash大小
 */
hal_result_t hal_cfls_read(uint8_t index, uint8_t *buffer, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 codeflash模块擦写函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        写入的数据
 *                  [in] l_u32_eraselen     擦除的长度
 *                  [in] datalen      写入的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_erase_write(uint8_t index, const uint8_t *buffer, uint32_t l_u32_eraselen, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 codeflash模块写函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        写入的数据
 *                  [in] datalen      写入的长度
 * 返   回  值      hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_write(uint8_t index, const uint8_t *buffer, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 cflash模块对比函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        比较的数据
 *                  [in] datalen      比较的长度
 *                       cresult_e          比较结果指针，详见hal_flash_compare_et[hal_flash_typedef.h]
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_flash_compare_et hal_cfls_compare(uint8_t index, const uint8_t *buffer, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 codeflash在写之前检查整个flash是否已被清除
 *
 *  * 参       数 ：
 *                  [in] index  要比较的位置（页数）
 *                  [in] datalen    比较的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :空白检查成功，=-10 :空白检查失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_blank_check(uint8_t index, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 nvm模块初始化函数
 *
 *  * 参       数 ：
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_nvm_init(void);

/**
 * 函 数 介 绍 ：
 *                 nvm模块读函数
 *
 *  * 参       数 ：index               通过该索引号去数组（g_u32_nvm_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                 buffer               读取数据缓存指针
 *                 datalen            读取数据长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_nvm_read(uint8_t index, uint8_t *buffer, uint32_t datalen);

/**
 * 函 数 介 绍 ：
 *                 nvm模块写函数
 *
 *  * 参       数 ：index               通过该索引号去数组（g_u32_nvm_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                 buffer              写数据缓存指针
 *                 datalen            写数据长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_nvm_write(uint8_t index, const uint8_t *buffer, uint32_t datalen);

#endif
