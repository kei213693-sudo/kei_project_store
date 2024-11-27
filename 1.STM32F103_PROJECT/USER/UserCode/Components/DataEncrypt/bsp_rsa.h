/*
 * bsp_rsa.h
 *
 *  Created on: 2024年8月1日
 *      Author: lj
 *	  Abstract: RSA算法实现，用于数据的加解密，例如AES的密钥传递或数字签名
 */
#ifndef _BSP_RSA_H_
#define _BSP_RSA_H_

/*******************************宏定义*******************************/
//用于选择RSA密钥长度
#define BSP_RSA_512
// #define BSP_RSA_1024
// #define BSP_RSA_2048
// #define BSP_RSA_3072
// #define BSP_RSA_4096

#if defined(BSP_RSA_512)
    #define BSP_RSA_ENCODE_LEN  (512/8)   //RSA2048即64字节
#elif defined(BSP_RSA_1024)
    #define BSP_RSA_ENCODE_LEN  (1024/8)   //RSA2048即128字节
#elif defined(BSP_RSA_2048)
    #define BSP_RSA_ENCODE_LEN  (2048/8)   //RSA2048即256字节
#endif

#define BSP_RSA_BI_MAXLEN   130
#define BSP_RSA_CARRYOVER   0x10000
#define BSP_RSA_CARRYLAST   0xFFFF

#define BSP_RSA_NONE 0//未用到api编译开关

#define BSP_RSA_TEST 0//测试标志

/*******************************结构体、枚举******************************/
typedef struct bsp_rsa_cbigint_t
{//rsa计算结构体
    uint32_t m_nLength;    //大数在0x1 00 00 00 00进制下的长度
    uint16_t m_ulValue[BSP_RSA_BI_MAXLEN];    //用数组记录大数在0x100000000进制下每一位的值
}bsp_rsa_cbigint_t;

/*******************************全局变量的外部声明extern*******************************/
extern int8_t *g_RsaPublicKey_exponent;
extern int8_t *g_RsaPrivateKey_exponent;
extern int8_t *g_RsaModulus;

/*******************************全局函数的声明*******************************/
extern uint16_t bsp_rsa_public_encrypt(uint8_t *s_pBuffer, uint16_t s_length, uint8_t *e_pBuffer, int8_t *publicKey, int8_t *Modulus);
extern uint16_t bsp_rsa_private_decrypt(uint8_t *e_pBuffer, uint16_t e_length, uint8_t *s_pBuffer, int8_t *privateKey, int8_t *Modulus);

extern uint16_t bsp_rsa_private_encrypt(uint8_t *s_pBuffer, uint16_t s_length, uint8_t *e_pBuffer, int8_t *privateKey, int8_t *Modulus);
extern uint16_t bsp_rsa_public_decrypt(uint8_t *e_pBuffer, uint16_t e_length, uint8_t *s_pBuffer, int8_t *publicKey, int8_t *Modulus);

#endif /* _BSP_RSA_H_ */
