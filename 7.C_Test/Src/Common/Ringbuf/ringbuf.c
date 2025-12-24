/**
 * @copyright Copyright (c) 2025 Seres Technology Co., Ltd. All rights reserved.
 * 
 * @file ringbuf.c
 * @brief 
 * @author 
 * @version 1.0.0
 * @date 
 * 
 */

#include "ringbuf.h"

/**
 * @brief 内部获取数据函数
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @param[in] buf 数据缓冲区指针
 * @param[in] cnt 请求获取的数据元素数量
 * @param[in] is_dummy 是否为虚拟读取标志(0:实际读取,1:虚拟读取)
 * @return uint32_t 实际获取的数据元素数量
 * 
 * @note 根据is_dummy参数决定是否更新读指针位置
 * @warning 
 */
static uint32_t ringbuf_gets_internal(ringbuf_ctxt_t *ring, uint8_t *buf,
    uint32_t cnt, uint8_t is_dummy)
{
    uint32_t num;

    if ((NULL == ring) || (NULL == buf) || (0u == cnt))
    {
        return 0;
    }
    else
    {
        /* Do nothing - avoid MISRA violation */
    }

    num = ringbuf_cur_elements(ring);
    if (0u == num)
    {
        return 0;
    }
    else
    {
        /* Do nothing - avoid MISRA violation */
    }

    if (num > cnt)
    {
        num = cnt;
    }
    else
    {
        /* Do nothing - avoid MISRA violation */
    }

    if ((ring->read_idx + num) < ring->size)
    {
        (void)utils_mem_cpy(buf, ring->entry + ring->read_idx, num);
        if (0u == is_dummy)
        {
            ring->read_idx += (uint16_t)num;
        }
        else
        {
            /* Do nothing - avoid MISRA violation */
        }
    }
    else {
        uint32_t first_sz, around_sz;
        first_sz = (uint32_t)ring->size - (uint32_t)ring->read_idx;
        (void)utils_mem_cpy(buf, ring->entry + ring->read_idx, first_sz);
        around_sz = (uint32_t)(num - first_sz);
        (void)utils_mem_cpy((void *)&buf[first_sz], (void *)ring->entry, around_sz);
        if (0u == is_dummy)
        {
            ring->read_idx = (uint16_t)around_sz;
        }
        else
        {
            /* Do nothing - avoid MISRA violation */
        }
    }

    return num;
}

/**
 * @brief 获取环形缓冲区总容量
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @return uint32_t 环形缓冲区总容量
 * 
 * @note 返回环形缓冲区的最大存储元素数量
 * @warning 
 */
uint32_t ringbuf_element_num(ringbuf_ctxt_t *ring)
{
    return (uint32_t)((NULL != ring) ? (uint32_t)ring->size : 0u);
}

/**
 * @brief 获取环形缓冲区当前存储元素数量
 * 
 * @param[in] ring 环形缓冲区上下文指针
 * @return uint32_t 当前存储的元素数量
 * 
 * @note 计算读写指针之间的元素数量
 * @warning 
 */
uint32_t ringbuf_cur_elements(ringbuf_ctxt_t *ring)
{
    uint16_t num;

    if (NULL == ring)
    {
        return 0u;
    }
    else
    {
        /* not do something */
    }

    if (ring->read_idx > ring->write_idx) {
        num = ring->size + ring->write_idx - ring->read_idx;
    }
    else {
        num = ring->write_idx - ring->read_idx;
    }

    return (uint32_t)num;
}

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
uint32_t ringbuf_gets(ringbuf_ctxt_t *ring, uint8_t *buf, uint32_t elements)
{
    return ringbuf_gets_internal(ring, buf, elements, 0);
}

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
uint32_t ringbuf_dummy_gets(ringbuf_ctxt_t *ring, uint8_t *buf, uint32_t elements)
{
    return ringbuf_gets_internal(ring, buf, elements, 1);
}

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
uint32_t ringbuf_drop(ringbuf_ctxt_t *ring, uint32_t elements)
{
    uint32_t adj_num, idx;

    if ((NULL != ring) && (0u != elements))
    {
        adj_num = ringbuf_cur_elements(ring);
        if (adj_num > elements) {
            adj_num = elements;
        }

        idx = ring->read_idx + adj_num;
        if (idx < ring->size) {
            ring->read_idx = (uint16_t)idx;
        } else {
            ring->read_idx = (uint16_t)idx - ring->size;
        }

        return adj_num;
    }
    else
    {
        return 0;
    }
}

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
uint32_t ringbuf_puts(ringbuf_ctxt_t *ring, uint8_t *buf, uint32_t elements)
{
    uint32_t num;

    if ((NULL == ring || NULL == buf))
    {
        return 0;
    }
    else
    {
        /* Do nothing - avoid MISRA violation */
    }

    /* ring buffer remaining available space. reserve 1byte to avoid
     * overwriting
     */
    num = ringbuf_element_num(ring) - ringbuf_cur_elements(ring) - 1u;

    if (num > elements) {
        //TODO: ring buffer is not enough free space
        num = elements;
    }

    if ((ring->write_idx + num) < ring->size) {
        (void)utils_mem_cpy(ring->entry + ring->write_idx, buf, num);
        ring->write_idx += (uint16_t)num;
    }
    else {
        uint32_t first_sz, around_sz;
        first_sz = (uint32_t)ring->size - (uint32_t)ring->write_idx;
        (void)utils_mem_cpy(ring->entry + ring->write_idx, buf, first_sz);
        around_sz = num - first_sz;
        (void)utils_mem_cpy(ring->entry, &buf[first_sz], around_sz);
        ring->write_idx = (uint16_t)around_sz;
    }

    return num;
}

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
void ringbuf_init(ringbuf_ctxt_t *ring, uint8_t *buf, uint32_t size)
{
    if (NULL != ring) {
        ring->entry = buf;
        ring->write_idx = 0;
        ring->read_idx = 0;
        ring->size = (uint16_t)size;
    }
    else
    {
        /* not do something */
    }
}
