#ifndef __BSP_MD5_H
#define __BSP_MD5_H

#define BSP_MD5_OUT_LEN           (16)//bytes，最终输出结果长度

/**
@brief	MD5 context. 
 */
typedef struct bsp_md5_ctx_t
{
    uint32_t state[4];    /**< state (ABCD) 缓存寄存器数值，存储初始值、中间值、结果值*/
    uint32_t count[2];    /**< number of bits, modulo 2^64 (lsb first) 所有输入数据总长度*/
    uint8_t  buffer[64];  /**< input buffer*/
}bsp_md5_ctx_t;

extern void bsp_md5_init(bsp_md5_ctx_t *context);
extern void bsp_md5_update(bsp_md5_ctx_t *context, uint8_t *buffer, uint32_t length);
extern void bsp_md5_final(uint8_t result[BSP_MD5_OUT_LEN], bsp_md5_ctx_t *context);

#endif	// __BSP_MD5_H


