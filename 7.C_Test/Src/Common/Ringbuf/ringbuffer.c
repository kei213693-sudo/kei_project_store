#include <string.h>
#include "ringbuffer.h"

static uint32_t rt_ringbuffer_space_len(struct rt_ringbuffer *rb);
static __inline enum rt_ringbuffer_state rt_ringbuffer_status(struct rt_ringbuffer *rb);

//初始化循环存储区域
void rt_ringbuffer_init(struct rt_ringbuffer *rb,
                        uint8_t           *pool,
                        int16_t            size)
{
    RB_ASSERT(rb != RB_NULL);
    RB_ASSERT(size > 0);

    /* initialize read and write index */
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    /* set buffer pool and size */
    rb->buffer_ptr = pool;
	rb->buffer_size = size;
}

//数据存入循环存储区域，未读取数据没满就继续存，否则需要读取或者清空才能继续操作
uint32_t rt_ringbuffer_put(struct rt_ringbuffer *rb,
                            const uint8_t     *ptr,
                            uint16_t           length)
{
    uint16_t space_length;
    RB_ASSERT(rb != RB_NULL);
    space_length = rt_ringbuffer_space_len(rb);//获取剩余的空间大小，（总大小-未读取的大小）

    /* no space or length is zero*/
    if (space_length == 0 || length == 0)
        return 0;

    /* drop some data */
    if (space_length < length)
    {
        length = space_length;//如果待存入数据大于空余空间，超过的数据不存储
    }

    if (rb->buffer_size - rb->write_index > length)
    {//write_index到end空间大于length就直接赋值
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        rb->write_index += length;
    }
    else
    {//需要从start开始存储数据了
        //先拷贝到end
        memcpy(&rb->buffer_ptr[rb->write_index],
            &ptr[0],
            rb->buffer_size - rb->write_index);

        //再从start开始拷贝剩下的数据
        memcpy(&rb->buffer_ptr[0],
            &ptr[rb->buffer_size - rb->write_index],
            length - (rb->buffer_size - rb->write_index));

        /* we are going into the other side of the mirror */
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = length - (rb->buffer_size - rb->write_index);
    }
    
    return length;
}

//数据存入循环存储区域，不考虑未读取的数据，强制刷写覆盖
uint32_t rt_ringbuffer_put_force(struct rt_ringbuffer *rb,
                            const uint8_t     *ptr,
                            uint16_t           length)
{
    uint16_t space_length;
    RB_ASSERT(rb != RB_NULL);
    space_length = rt_ringbuffer_space_len(rb);

    if (length > rb->buffer_size)
    {//要写入数据大于最大长度，取后面的数据覆盖
        ptr = &ptr[length - rb->buffer_size];
        length = rb->buffer_size;
    }

    if (rb->buffer_size - rb->write_index > length)
    {//write_index到end空间大于length就直接赋值
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        rb->write_index += length;

        if (length > space_length)
        {//此时把未读取的数据覆盖了一部分，read_index重新映射
            rb->read_index = rb->write_index;
        }
    }
    else
    {
        memcpy(&rb->buffer_ptr[rb->write_index],
                &ptr[0],
                rb->buffer_size - rb->write_index);

        memcpy(&rb->buffer_ptr[0],
                &ptr[rb->buffer_size - rb->write_index],
                length - (rb->buffer_size - rb->write_index));

        /* we are going into the other side of the mirror */
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = length - (rb->buffer_size - rb->write_index);

        if (length > space_length)
        {//此时把未读取的数据覆盖了一部分，read_index重新映射
            rb->read_mirror = ~rb->read_mirror;
            rb->read_index = rb->write_index;
        }
    }
    
    return length;
}

//获取未读取的数据
uint32_t rt_ringbuffer_get(struct rt_ringbuffer *rb,
                            uint8_t           *ptr,
                            uint16_t           length)
{
    uint32_t size;
    RB_ASSERT(rb != RB_NULL);
    size = rt_ringbuffer_data_len(rb);//未读取数据长度

    /* no data or length is zero*/
    if (size == 0 || length == 0)
        return 0;

    /* less data */
    if (size < length)
    {//想获取的长度超过未读取长度
        length = size;
    }

    if (rb->buffer_size - rb->read_index > length)
    {//没有跨区域
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->read_index += length;
    }
    else
    {
        memcpy(&ptr[0],
            &rb->buffer_ptr[rb->read_index],
            rb->buffer_size - rb->read_index);

        memcpy(&ptr[rb->buffer_size - rb->read_index],
            &rb->buffer_ptr[0],
            length - (rb->buffer_size - rb->read_index));

        /* we are going into the other side of the mirror */
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = length - (rb->buffer_size - rb->read_index);
    }

    return length;
}

/**
 *  peak data from ring buffer
 */
uint32_t rt_ringbuffer_peak(struct rt_ringbuffer *rb, uint8_t **ptr)
{
    RB_ASSERT(rb != RB_NULL);
    *ptr = RB_NULL;
    uint32_t size = rt_ringbuffer_data_len(rb);

    /* no data */
    if (size == 0)
        return 0;

    *ptr = &rb->buffer_ptr[rb->read_index];

    if(rb->buffer_size - rb->read_index > size)
    {//未读取数据没有跨区，就全部读取
        rb->read_index += size;
    }
    else
    {//跨区了，就把read_index——》end的读取出来，start——》write_index的不读
        size = rb->buffer_size - rb->read_index;

        /* we are going into the other side of the mirror */
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    }

    return size;
}

/**
 * put a character into ring buffer
 */
uint32_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const uint8_t ch)
{
    RB_ASSERT(rb != RB_NULL);
    /* whether has enough space */
    if (!rt_ringbuffer_space_len(rb))
        return 0;

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    }
    else
    {
        rb->write_index++;
    }

    return 1;
}

/**
 * put a character into ring buffer
 *
 * When the buffer is full, it will discard one old data.
 */
uint32_t rt_ringbuffer_putchar_force(struct rt_ringbuffer *rb, const uint8_t ch)
{
    enum rt_ringbuffer_state old_state;
    RB_ASSERT(rb != RB_NULL);
    old_state = rt_ringbuffer_status(rb);

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
        if (old_state == RT_RINGBUFFER_FULL)
        {
            rb->read_mirror = ~rb->read_mirror;
            rb->read_index = rb->write_index;
        }
    }
    else
    {
        rb->write_index++;
        if (old_state == RT_RINGBUFFER_FULL)
            rb->read_index = rb->write_index;
    }

    return 1;
}

/**
 * get a character from a ringbuffer
 */
uint32_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, uint8_t *ch)
{
    RB_ASSERT(rb != RB_NULL);

    /* ringbuffer is empty */
    if (!rt_ringbuffer_data_len(rb))
        return 0;

    /* put character */
    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index == rb->buffer_size-1)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    }
    else
    {
        rb->read_index++;
    }

    return 1;
}

//获取当前存储状态
static __inline enum rt_ringbuffer_state rt_ringbuffer_status(struct rt_ringbuffer *rb)
{
    if (rb->read_index == rb->write_index)
    {
        if (rb->read_mirror == rb->write_mirror)
            return RT_RINGBUFFER_EMPTY;//索引相同，状态相同，就是空
        else
            return RT_RINGBUFFER_FULL;//索引相同，状态不同，就是满
    }
    return RT_RINGBUFFER_HALFFULL;
}

//获取剩余空间长度，总长度-未读取数据长度 
static uint32_t rt_ringbuffer_space_len(struct rt_ringbuffer *rb)
{
    return ((rb)->buffer_size - rt_ringbuffer_data_len(rb));
}

//获取未读取储数据长度
uint32_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb)
{
    switch (rt_ringbuffer_status(rb))
    {
    case RT_RINGBUFFER_EMPTY:
        return 0;
    case RT_RINGBUFFER_FULL:
        return rb->buffer_size;
    case RT_RINGBUFFER_HALFFULL:
    default:
        if (rb->write_index > rb->read_index)//未读取数据没跨区域，start——》read_index——》write_index——》end
            return rb->write_index - rb->read_index;
        else//未读取数据跨区域了，从read_index——》end，再从start——》write_index
            return rb->buffer_size - (rb->read_index - rb->write_index);
    };
}

/** 
 * empty the rb 
 */
void rt_ringbuffer_reset(struct rt_ringbuffer *rb)
{
    RB_ASSERT(rb != RB_NULL);

    rb->read_mirror = 0;
    rb->read_index = 0;
    rb->write_mirror = 0;
    rb->write_index = 0;
}

#ifdef RT_USING_HEAP

struct rt_ringbuffer* rt_ringbuffer_create(uint16_t size)
{
    struct rt_ringbuffer *rb;
    uint8_t *pool;

	RB_ASSERT(size > 0);

    size = RT_ALIGN_DOWN(size, RT_ALIGN_SIZE);

    rb = (struct rt_ringbuffer *)rt_malloc(sizeof(struct rt_ringbuffer));
    if (rb == RB_NULL)
        goto exit;

    pool = (uint8_t *)rt_malloc(size);
    if (pool == RB_NULL)
    {
        rt_free(rb);
        rb = RB_NULL;
        goto exit;
    }
    rt_ringbuffer_init(rb, pool, size);

exit:
    return rb;
}

void rt_ringbuffer_destroy(struct rt_ringbuffer *rb)
{
    RB_ASSERT(rb != RB_NULL);

    rt_free(rb->buffer_ptr);
    rt_free(rb);
}

#endif
