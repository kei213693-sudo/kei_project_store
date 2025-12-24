/*
 * aes_api.h
 *
 *  Created on: 2024年7月23日
 *      Author: lj
 *	  Abstract: AES算法实现，用于数据的加解密，支持128/192/256密钥长度，ECB/CBC运算模式，PKCS7/Zeros/None填充
 */
#ifndef _AES_H_
#define _AES_H_

/*******************************宏定义*******************************/
#define AES_128
// #define AES_192
// #define AES_256

#define AES_Nb 4        // AES组成状态的列数
#define AES_BLOCKLEN 16 // AES最小计算单元大小，bytes，与密钥长度无关

#if defined(AES_128)
#define AES_KEY_LEN 16           // 密钥长度 bytes
#define AES_KEY_EXP_LEN 176      // 密钥扩展长度 bytes
#define AES_Nk (AES_KEY_LEN / 4) // 密钥中u32数量，也就是4
#define AES_Nr (6 + AES_Nk)      // 计算轮数,10
#elif defined(AES_192)
#define AES_KEY_LEN 24
#define AES_KEY_EXP_LEN 208
#define AES_Nk (AES_KEY_LEN / 4) // 6
#define AES_Nr (6 + AES_Nk)      // 12
#elif defined(AES_256)
#define AES_KEY_LEN 32
#define AES_KEY_EXP_LEN 240
#define AES_Nk (AES_KEY_LEN / 4) // 8
#define AES_Nr (6 + AES_Nk)      // 14
#endif

/*******************************结构体、枚举******************************/

typedef enum
{
    AES_PADDING_NONE,  // 不填充，但是明文一定要是块的整数倍
    AES_PADDING_ZERO,  // 当明文长度是块长度的整数倍时，不需要在填充
    AES_PADDING_PKCS7, // 当明文长度是块长度的整数倍时，仍需要填充一个完整的块
    AES_PADDING_END
} AES_PADDING_MODE_E;

typedef struct bsp_aes_ctx_t
{                                      // CBC模式
    uint8_t RoundKey[AES_KEY_EXP_LEN]; // 密钥扩展数值，不论有几个aes，都使用默认的数值
    uint8_t Iv[AES_BLOCKLEN];          // 偏移量
    uint8_t IvStore[AES_BLOCKLEN];     // 偏移量备份，当存在多个aes，且Iv值不同的场景
} aes_ctx_t;

typedef uint8_t aes_state_t[4][4]; // 保存加解密中间量数组

/*******************************全局变量的外部声明extern*******************************/

/*******************************全局函数的声明*******************************/
void aes_init(aes_ctx_t *context, uint8_t aesKey[AES_KEY_LEN]);
void aes_Iv_set(aes_ctx_t *context, uint8_t Iv[AES_BLOCKLEN]);

void aes_cbc_encrypt_none(aes_ctx_t *context, uint8_t *pBuffer, uint32_t length);
void aes_cbc_decrypt_none(aes_ctx_t *context, uint8_t *pBuffer, uint32_t length);
void aes_cbc_encrypt(aes_ctx_t *context, const uint8_t *s_pBuffer, uint32_t s_length,
                     uint8_t *e_pBuffer, uint32_t *e_pLength, AES_PADDING_MODE_E padding_mode);
void aes_cbc_decrypt(aes_ctx_t *context, const uint8_t *e_pBuffer, uint32_t e_length,
                     uint8_t *s_pBuffer, uint32_t *s_pLength, AES_PADDING_MODE_E padding_mode);

void aes_ecb_encrypt_none(aes_ctx_t *context, uint8_t *pBuffer, uint32_t length);
void aes_ecb_decrypt_none(aes_ctx_t *context, uint8_t *pBuffer, uint32_t length);
void aes_ecb_encrypt(aes_ctx_t *context, const uint8_t *s_pBuffer, uint32_t s_length,
                     uint8_t *e_pBuffer, uint32_t *e_pLength, AES_PADDING_MODE_E padding_mode);
void aes_ecb_decrypt(aes_ctx_t *context, const uint8_t *e_pBuffer, uint32_t e_length,
                     uint8_t *s_pBuffer, uint32_t *s_pLength, AES_PADDING_MODE_E padding_mode);
#endif /* _AES_H_ */
