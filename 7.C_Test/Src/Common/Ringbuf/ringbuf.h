/**
 * @copyright Copyright (c) 2025 Seres Technology Co., Ltd. All rights reserved.
 * 
 * @file ringbuf.h
 * @brief 环形缓冲区头文件
 * @author 
 * @version 1.0.0
 * @date 
 * 
 */

#ifndef RING_BUF__
#define RING_BUF__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ringbuf_ctxt {
    /* ring buffer entry address */
    uint8_t  *entry;
    /* ring data head position */
    uint16_t read_idx;
    /* ring data end position */
    uint16_t write_idx;
    /* buffer byte size */
    uint16_t size;
} ringbuf_ctxt_t;

/**
 * @brief 获取环形缓冲区总容量
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @return uint32_t 环形缓冲区总容量
 * 
 * @note 返回环形缓冲区的最大存储元素数量
 * @warning 
 */
uint32_t ringbuf_element_num(ringbuf_ctxt_t *ring);

/**
 * @brief 获取环形缓冲区当前存储元素数量
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @return uint32_t 当前存储的元素数量
 * 
 * @note 计算读写指针之间的元素数量
 * @warning 
 */
uint32_t ringbuf_cur_elements(ringbuf_ctxt_t *ring);

/**
 * @brief 从环形缓冲区获取数据
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @param[in] buf 数据接收缓冲区指针
 * @param[in] elements 请求获取的元素数量
 * @return uint32_t 实际获取的元素数量
 * 
 * @note 获取数据后会更新读指针位置
 * @warning 
 */
uint32_t ringbuf_gets(ringbuf_ctxt_t *ring, uint8_t *buf, uint32_t elements);

/**
 * @brief 虚拟获取环形缓冲区数据(不更新读指针)
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @param[in] buf 数据接收缓冲区指针
 * @param[in] elements 请求获取的元素数量
 * @return uint32_t 实际获取的元素数量
 * 
 * @note 仅读取数据但不更新读指针位置
 * @warning 
 */
uint32_t ringbuf_dummy_gets(ringbuf_ctxt_t *ring, uint8_t *buf, uint32_t elements);

/**
 * @brief 丢弃环形缓冲区中的数据
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @param[in] elements 请求丢弃的元素数量
 * @return uint32_t 实际丢弃的元素数量
 * 
 * @note 更新读指针位置以丢弃指定数量的数据
 * @warning 
 */
uint32_t ringbuf_drop(ringbuf_ctxt_t *ring, uint32_t elements);

/**
 * @brief 向环形缓冲区放入数据
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @param[in] buf 数据源缓冲区指针
 * @param[in] elements 请求放入的元素数量
 * @return uint32_t 实际放入的元素数量
 * 
 * @note 向缓冲区写入数据并更新写指针位置
 * @warning 
 */
uint32_t ringbuf_puts(ringbuf_ctxt_t *ring, uint8_t *buf, uint32_t elements);

/**
 * @brief 初始化环形缓冲区
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @param[in] buf 数据存储缓冲区指针
 * @param[in] size 缓冲区大小
 * @return void
 * 
 * @note 初始化读写指针和缓冲区参数
 * @warning 
 */
void ringbuf_init(ringbuf_ctxt_t *ring, uint8_t *buf, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif