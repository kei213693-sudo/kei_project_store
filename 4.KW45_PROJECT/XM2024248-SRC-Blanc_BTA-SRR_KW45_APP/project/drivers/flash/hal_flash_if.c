/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: hal_flash_if.c
 * 摘要:
 * 当前版本:
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#include "hal_flash_if.h"
#include "hal_flash.h"
/**
 * 函 数 介 绍 ：
 *                 dataflash模块进行初始化
 *
 *  * 参       数 ：
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化
 */
hal_result_t hal_pfls_if_init(void)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pfls_init();
    return result;
}

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
hal_result_t hal_pfls_if_erase(uint8_t index, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pfls_erase(index,datalen);
    return result;
}

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
hal_result_t hal_pfls_if_read(uint8_t index, uint8_t *buffer, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pfls_read(index,buffer,datalen);
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 dataflash模块擦写函数
 *
 *  * 参       数 ：
 *                  [in] index              通过该索引号去数组（g_u32_flash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer             写入的数据
 *                  [in] eraselen           擦除的长度
 *                  [in] datalen            写入的长度
 * 返   回  值  hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_pfls_if_erase_write(uint8_t index, const uint8_t *buffer, uint32_t eraselen, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pfls_erase_write(index,buffer,eraselen,datalen);
    return result;
}

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
hal_result_t hal_pfls_if_write(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pfls_write(index,buffer,datalen);
    return result;
}

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
hal_flash_compare_et hal_pfls_if_compare(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pfls_compare(index,buffer,datalen);
    return result;
}

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
hal_flash_check_et hal_pfls_if_blank_check(uint8_t index, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_pfls_blank_check(index,datalen);
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 codeflash模块初始化函数
 *
 *  * 参       数 ：

 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
*/
hal_result_t hal_cfls_if_init(void)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_cfls_init();
    return result;
}

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
hal_result_t hal_cfls_if_erase(uint8_t index, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_cfls_erase(index,datalen);
    return result;
}

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
hal_result_t hal_cfls_if_read(uint8_t index, uint8_t *buffer, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_cfls_read(index,buffer,datalen);
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 codeflash模块擦写函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        写入的数据
 *                  [in] eraselen     擦除的长度
 *                  [in] datalen      写入的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_if_erase_write(uint8_t index, const uint8_t *buffer, uint32_t eraselen, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_cfls_erase_write(index,buffer,eraselen,datalen);
    return result;
}

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
hal_result_t hal_cfls_if_write(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_cfls_write(index,buffer,datalen);
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 cflash模块对比函数
 *
 *  * 参       数 ：
 *                  [in] index         通过该索引号去数组（g_u32_codeflash_addr_a，定义在hal_flash_cfg.h中）中查找对应的地址信息
 *                  [in] buffer        比较的数据
 *                  [in] datalen       比较的长度
 *                       result        比较结果指针，详见hal_flash_compare_et[hal_flash_typedef.h]
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_flash_compare_et hal_cfls_if_compare(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_result_t l_i8_result = HAL_FLASH_COMPARE_ERROR;
    l_i8_result = hal_cfls_compare(index,buffer,datalen);
    return l_i8_result;
}

/**
 * 函 数 介 绍 ：
 *                 codeflash在写之前检查整个flash是否已被清除
 *
 *  * 参       数 ：
 *                  [in] index  要比较的位置（页数）
 *                  [in] buff   比较的数据
 *                  [in] len    比较的长度
 * 返   回  值 hal_result_t   hal_result_t   =0 :空白检查成功，=-10 :空白检查失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_cfls_if_blank_check(uint8_t index, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_cfls_blank_check(index,datalen);
    return result;
}

/**
 * 函 数 介 绍 ：
 *                 nvm模块初始化函数
 *
 *  * 参       数 ：
 * 返   回  值 hal_result_t   hal_result_t   =0 :操作成功，<0 :操作失败 详情参考 hal_typedef.h:Error code宏定义组
 * 备       注 ：
 *              调用前必须先完成相关初始化，擦除位置不能超出flash大小
 */
hal_result_t hal_nvm_if_init(void)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_nvm_init();
    return result;
}

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
hal_result_t hal_nvm_if_read(uint8_t index, uint8_t *buffer, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_nvm_read(index,buffer,datalen);
    return result;
}

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
hal_result_t hal_nvm_if_write(uint8_t index, const uint8_t *buffer, uint32_t datalen)
{
    hal_result_t result = HAL_RESULT_SUCCESS;
    result = hal_nvm_write(index,buffer,datalen);
    return result;
}
