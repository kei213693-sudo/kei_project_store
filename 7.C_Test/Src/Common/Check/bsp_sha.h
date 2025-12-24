/*
 * bsp_sha.h
 *
 *  Created on: 2024年7月29日
 *      Author: lj
 *	  Abstract: 实现SHA算法
 */
#ifndef _BSP_SHA_H_
#define _BSP_SHA_H_

/*******************************头文件*******************************/

/*******************************宏定义*******************************/
//循环左移 bits 位
#define BSP_SHA1CircularShift(bits,word) ((((word) << (bits)) & 0xFFFFFFFF) | ((word) >> (32 - (bits))))

#define BSP_SHA1_OUT_LEN           (20)//bytes，最终输出结果长度

/*******************************结构体、枚举******************************/
//SHA 函数的结构体
typedef struct bsp_sha1_ctx_t
{
	uint32_t Message_Digest[BSP_SHA1_OUT_LEN/sizeof(uint32_t)];//缓存器/信息摘要，存储初始值、中间值与结果值
	uint32_t Length_Low;//消息长度低 32 位
	uint32_t Length_High;//消息长度高 32 位
	uint8_t  Message_Block[64];// 512bits 的块，总共 64 个字节
	int32_t  Message_Block_Index; //512bits 块索引号

	int32_t  Computed;//摘要计算标识
	int32_t  Corrupted;//信息摘要损坏标识
}bsp_sha1_ctx_t;

/*******************************全局变量的外部声明extern*******************************/

/*******************************全局函数的声明*******************************/
void bsp_sha1_init(bsp_sha1_ctx_t* context);
void bsp_sha1_update(bsp_sha1_ctx_t* context, const uint8_t *buffer,uint32_t length);
int32_t bsp_sha1_final(uint8_t result[BSP_SHA1_OUT_LEN],bsp_sha1_ctx_t* context);


#endif /* _BSP_SHA_H_ */
