/*
 * bsp_aes.h
 *
 *  Created on: 2024年7月23日
 *      Author: lj
 *	  Abstract: AES算法实现，使用CBC模式PKCS7/NoPadding填充，用于数据的加解密
 */
#ifndef _BSP_AES_H_
#define _BSP_AES_H_

/*******************************宏定义*******************************/
#define BSP_AES_128 0//用于选择AES密钥长度
//#define BSP_AES_192 0
//#define BSP_AES_256 0

#define BSP_AES_Nb 4//AES组成状态的列数
#define BSP_AES_BLOCKLEN 16//AES最小计算单元大小，bytes，与密钥长度无关

#if defined(BSP_AES_128)
    #define BSP_AES_KEY_LEN 16   // 密钥长度 bytes
    #define BSP_AES_KEY_EXP_LEN 176//密钥扩展长度 bytes
    #define BSP_AES_Nk (BSP_AES_KEY_LEN/4)//密钥中u32数量，也就是4
    #define BSP_AES_Nr (6 + BSP_AES_Nk)//计算轮数,10
#elif defined(BSP_AES_192)
    #define BSP_AES_KEY_LEN 24
    #define BSP_AES_KEY_EXP_LEN 208
    #define BSP_AES_Nk (BSP_AES_KEY_LEN/4)//6
    #define BSP_AES_Nr (6 + BSP_AES_Nk)//12
#elif defined(BSP_AES_256)
    #define BSP_AES_KEY_LEN 32
    #define BSP_AES_KEY_EXP_LEN 240
    #define BSP_AES_Nk (BSP_AES_KEY_LEN/4)//8
    #define BSP_AES_Nr (6 + BSP_AES_Nk)//14
#endif

/*******************************结构体、枚举******************************/
typedef struct bsp_aes_ctx_t
{//CBC模式
    uint8_t RoundKey[BSP_AES_KEY_EXP_LEN];//密钥扩展数值，不论有几个aes，都使用默认的数值
    uint8_t Iv[BSP_AES_BLOCKLEN];//偏移量
    uint8_t IvStore[BSP_AES_BLOCKLEN];//偏移量备份，当存在多个aes，且Iv值不同的场景
}bsp_aes_ctx_t;

typedef uint8_t bsp_aes_state_t[4][4];//保存加解密中间量数组

/*******************************全局变量的外部声明extern*******************************/

/*******************************全局函数的声明*******************************/
void bsp_aes_init(bsp_aes_ctx_t* context, uint8_t aesKey[BSP_AES_KEY_LEN]);
void bsp_aes_Iv_set(bsp_aes_ctx_t* context,uint8_t Iv[BSP_AES_BLOCKLEN]);
void bsp_aes_encrypt_none(bsp_aes_ctx_t* context,uint8_t *pBuffer,uint32_t length);
void bsp_aes_decrypt_none(bsp_aes_ctx_t* context,uint8_t *pBuffer,uint32_t length);
void bsp_aes_encrypt(bsp_aes_ctx_t* context,const uint8_t *s_pBuffer,uint32_t s_length,uint8_t *e_pBuffer,uint32_t *e_pLength);
void bsp_aes_decrypt(bsp_aes_ctx_t* context,const uint8_t *e_pBuffer,uint32_t e_length,uint8_t *s_pBuffer,uint32_t *s_pLength);


#endif /* _BSP_AES_H_ */
