#ifndef RINGBUFFER_H__
#define RINGBUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "snj32w10x.h"

#define RB_NULL                         (0)
#define RB_ASSERT(EX)

//循环存储控制结构体
struct rt_ringbuffer
{//剩余的空间是（read_index - write_index）
    uint16_t read_mirror : 1;
    uint16_t read_index : 15;//未读取数据的索引

    uint16_t write_mirror : 1;//1表示已存储数据超过了end，从start重新开始了
    uint16_t write_index : 15;//已经存储数据索引

    int16_t buffer_size;//当前buff的总空间大小
    uint8_t *buffer_ptr;//指向buff的地址
    uint8_t res;
};

//循环存储状态
enum rt_ringbuffer_state
{
    RT_RINGBUFFER_EMPTY,//没数据
    RT_RINGBUFFER_FULL,//存满了
    RT_RINGBUFFER_HALFFULL,//有数据但没满
};

/**
 * RingBuffer for DeviceDriver
 *
 * Please note that the ring buffer implementation of RT-Thread
 * has no thread wait or resume feature.
 */
void rt_ringbuffer_init(struct rt_ringbuffer *rb, uint8_t *pool, int16_t size);
void rt_ringbuffer_reset(struct rt_ringbuffer *rb);
uint32_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb);

uint32_t rt_ringbuffer_put(struct rt_ringbuffer *rb, const uint8_t *ptr, uint16_t length);
uint32_t rt_ringbuffer_put_force(struct rt_ringbuffer *rb, const uint8_t *ptr, uint16_t length);
uint32_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const uint8_t ch);
uint32_t rt_ringbuffer_putchar_force(struct rt_ringbuffer *rb, const uint8_t ch);
uint32_t rt_ringbuffer_get(struct rt_ringbuffer *rb, uint8_t *ptr, uint16_t length);
uint32_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, uint8_t *ch);
uint32_t rt_ringbuffer_peak(struct rt_ringbuffer *rb, uint8_t **ptr);

#ifdef RT_USING_HEAP
struct rt_ringbuffer* rt_ringbuffer_create(uint16_t length);
void rt_ringbuffer_destroy(struct rt_ringbuffer *rb);
#endif

#ifdef __cplusplus
}
#endif

#endif
