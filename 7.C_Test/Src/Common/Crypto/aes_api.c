/*
 * aes_api.c
 *
 *  Created on: 2024年7月23日
 *      Author: lj
 *	  Abstract: AES算法实现，用于数据的加解密，支持128/192/256密钥长度，ECB/CBC运算模式，PKCS7/Zeros/None填充
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "aes_api.h"

/*******************************全局变量定义*******************************/

/*******************************内部变量定义*******************************/
// AES算法默认公钥
#if defined(AES_128) // 2b7e151628aed2a6abf7158809cf4f3c
static uint8_t g_u8AesKey[AES_KEY_LEN] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
#elif defined(AES_192) // 8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b
static uint8_t g_u8AesKey[AES_KEY_LEN] = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};
#elif defined(AES_256) // 603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4
static uint8_t g_u8AesKey[AES_KEY_LEN] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
#endif
// AES算法默认Iv值，000102030405060708090a0b0c0d0e0f
static uint8_t g_u8AesIv[AES_BLOCKLEN] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

static const uint8_t g_u8AesRcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

static const uint8_t g_u8AesSbox[256] =
    { // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

static const uint8_t g_u8AesRsbox[256] =
    { // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

/*******************************内部函数的声明*******************************/
static void KeyExpansion(uint8_t *RoundKey, const uint8_t *Key);
static void Cipher(aes_state_t *state, const uint8_t *RoundKey);
static void InvCipher(aes_state_t *state, const uint8_t *RoundKey);
static void AddRoundKey(uint8_t round, aes_state_t *state, const uint8_t *RoundKey);
static void SubBytes(aes_state_t *state);
static void InvSubBytes(aes_state_t *state);
static void ShiftRows(aes_state_t *state);
static void InvShiftRows(aes_state_t *state);
static void MixColumns(aes_state_t *state);
static void InvMixColumns(aes_state_t *state);
static void XorWithIv(uint8_t *buf, const uint8_t *Iv);
static uint8_t xtime(uint8_t x);
static uint8_t Multiply(uint8_t x, uint8_t y);

/*******************************实际函数的实现*******************************/

/**************************************************************
*函数功能描述 ：aes算法初始化，CBC模式需要Iv
*函数参数 ：context，aes算法控制结构体
            aesKey，密钥数值
*函数返回值 ：
*备注 ：
**************************************************************/
void aes_init(aes_ctx_t *context, uint8_t aesKey[AES_KEY_LEN])
{
    if (NULL != context)
    {
        if (NULL == aesKey)
        {
            KeyExpansion(context->RoundKey, g_u8AesKey); // 根据默认密钥数值来扩展
        }
        else
        {
            KeyExpansion(context->RoundKey, aesKey); // 根据自定义密钥数值来扩展
        }

        memcpy(context->Iv, g_u8AesIv, AES_BLOCKLEN);
        memcpy(context->IvStore, g_u8AesIv, AES_BLOCKLEN);
    }
}

/**************************************************************
*函数功能描述 ：aes算法Iv值修改
*函数参数 ：context，aes算法控制结构体
            Iv，要更新的Iv值
*函数返回值 ：
*备注 ：
**************************************************************/
void aes_Iv_set(aes_ctx_t *context, uint8_t Iv[AES_BLOCKLEN])
{
    if (NULL != context)
    {
        memcpy(context->Iv, Iv, AES_BLOCKLEN);
        memcpy(context->IvStore, Iv, AES_BLOCKLEN); // 修改默认值
    }
}

/**************************************************************
*函数功能描述 ：aes算法加密操作，CBC模式，NoPadding填充模式
*函数参数 ：context，aes算法控制结构体
            pBuffer，存储明文数据地址，直接加密生成密文也存储在这里
            length，明文/密文数据长度，明文长度==密文长度
*函数返回值 ：
*备注 ：明文长度length必须是块（16）的倍数
**************************************************************/
void aes_cbc_encrypt_none(aes_ctx_t *context, uint8_t *pBuffer, uint32_t length)
{
    if ((NULL == context) || (NULL == pBuffer) || (0 == length) || (0 != (length % AES_BLOCKLEN)))
    { // 判断入参有效性
        return;
    }

    for (uint32_t i = 0; i < length; i += AES_BLOCKLEN)
    {
        XorWithIv(pBuffer, context->Iv);                   // 将当前明文数据组与Iv异或
        Cipher((aes_state_t *)pBuffer, context->RoundKey); // 加密此组数据，直接更改e_pBuffer数值从明文到密文

        memcpy(context->Iv, pBuffer, AES_BLOCKLEN); // 将当前密文数据组存储到Iv中
        pBuffer += AES_BLOCKLEN;                    // 指向下一组数据
    }
    memcpy(context->Iv, context->IvStore, AES_BLOCKLEN); // 加密完成后，重置Iv值
}

/**************************************************************
*函数功能描述 ：aes算法解密操作，CBC模式，NoPadding填充模式
*函数参数 ：context，aes算法控制结构体
            pBuffer，存储密文数据地址，直接解密生成明文也存储在这里
            length，明文/密文数据长度，明文长度==密文长度
*函数返回值 ：
*备注 ：密文长度length必须是块（16）的倍数
**************************************************************/
void aes_cbc_decrypt_none(aes_ctx_t *context, uint8_t *pBuffer, uint32_t length)
{
    if ((NULL == context) || (NULL == pBuffer) || (0 == length) || (0 != (length % AES_BLOCKLEN)))
    { // 判断入参有效性
        return;
    }

    uint8_t storeNextIv[AES_BLOCKLEN] = {0};
    uint8_t u8tempFlag = 0;

    for (uint32_t i = 0; i < length; i += AES_BLOCKLEN)
    {
        memcpy(storeNextIv, pBuffer, AES_BLOCKLEN);           // 存储此组密文数据
        InvCipher((aes_state_t *)pBuffer, context->RoundKey); // 把此组密文数据转换成明文
        XorWithIv(pBuffer, context->Iv);                      // 将明文数据组与Iv异或

        memcpy(context->Iv, storeNextIv, AES_BLOCKLEN); // 把密文数据赋给Iv
        pBuffer += AES_BLOCKLEN;                        // 指向下一组数据
    }
    memcpy(context->Iv, context->IvStore, AES_BLOCKLEN); // 解密完成后，重置Iv值
}

/***************************************************************
*函数功能描述 ：aes算法加密操作，CBC模式
*函数参数 ：context，aes算法控制结构体
            s_pBuffer，存储明文数据地址
            s_length，明文数据长度
            e_pBuffer，存储密文数据地址
            e_pLength，存储密文长度地址
            padding_mode，填充模式
*函数返回值 ：
*备注 ：实际计算没有更改s_pBuffer区域，直接在e_pBuffer区域进行数据变更；
        非none填充模式时，明文长度s_length可以不是16的倍数
***************************************************************/
void aes_cbc_encrypt(aes_ctx_t *context, const uint8_t *s_pBuffer, uint32_t s_length,
                     uint8_t *e_pBuffer, uint32_t *e_pLength, AES_PADDING_MODE_E padding_mode)
{
    if ((NULL == context) || (NULL == s_pBuffer) || (0 == s_length) || (NULL == e_pBuffer) || (NULL == e_pLength) || (padding_mode >= AES_PADDING_END))
    { // 判断入参有效性
        return;
    }

    uint8_t u8padding_val = 0; // 待填充的数据值

    if (padding_mode == AES_PADDING_PKCS7)
    {
        u8padding_val = AES_BLOCKLEN - (s_length % AES_BLOCKLEN);
        *e_pLength = s_length + u8padding_val; // 实际要处理的数据长度，16的倍数

        // 根据明文填充数据
        memcpy(e_pBuffer, s_pBuffer, s_length);
        for (uint8_t i = 0; i < u8padding_val; i++)
        {
            e_pBuffer[s_length + i] = u8padding_val; // PKCS7填充数据
        }
    }
    else if (padding_mode == AES_PADDING_ZERO)
    {
        u8padding_val = AES_BLOCKLEN - (s_length % AES_BLOCKLEN);
        if (u8padding_val == AES_BLOCKLEN)
            u8padding_val = 0;                 // 如果刚好是16的倍数，不需要额外填充
        *e_pLength = s_length + u8padding_val; // 实际要处理的数据长度，16的倍数

        // 根据明文填充数据
        memcpy(e_pBuffer, s_pBuffer, s_length);
        if (0 != u8padding_val)
        {
            for (uint8_t i = 0; i < u8padding_val; i++)
            {
                e_pBuffer[s_length + i] = 0x00; // 零填充数据
            }
        }
    }
    else // No padding
    {
        if (0 != (s_length % AES_BLOCKLEN))
        {
            return; // 长度必须是块（16）的倍数
        }
        *e_pLength = s_length;
        memcpy(e_pBuffer, s_pBuffer, s_length);
    }

    // 开始进行加密计算
    memcpy(context->IvStore, context->Iv, AES_BLOCKLEN);
    for (uint32_t i = 0; i < *e_pLength; i += AES_BLOCKLEN)
    {
        XorWithIv(e_pBuffer, context->Iv);                   // 将当前明文数据组与Iv异或
        Cipher((aes_state_t *)e_pBuffer, context->RoundKey); // 加密此组数据，直接更改e_pBuffer数值从明文到密文

        memcpy(context->Iv, e_pBuffer, AES_BLOCKLEN); // 将当前密文数据组存储到Iv中
        e_pBuffer += AES_BLOCKLEN;                    // 指向下一组数据
    }
    memcpy(context->Iv, context->IvStore, AES_BLOCKLEN); // 加密完成后，重置Iv值
}

/***************************************************************
*函数功能描述 ：aes算法解密操作，CBC模式
*函数参数 ：context，aes算法控制结构体
            e_pBuffer，存储密文数据地址
            e_length，密文数据长度
            s_pBuffer，存储明文数据地址
            s_pLength，存储明文长度地址
            padding_mode，填充模式
*函数返回值 ：
*备注 ：密文长度e_length必须是块（16）的倍数
***************************************************************/
void aes_cbc_decrypt(aes_ctx_t *context, const uint8_t *e_pBuffer, uint32_t e_length,
                     uint8_t *s_pBuffer, uint32_t *s_pLength, AES_PADDING_MODE_E padding_mode)
{
    if ((NULL == context) || (NULL == e_pBuffer) || (0 == e_length) || (NULL == s_pBuffer) || (NULL == s_pLength) || (0 != (e_length % AES_BLOCKLEN)))
    {
        return;
    }

    // 解密计算
    memcpy(context->IvStore, context->Iv, AES_BLOCKLEN);
    uint8_t storeNextIv[AES_BLOCKLEN] = {0};
    uint8_t u8tempFlag = 0;

    memcpy(s_pBuffer, e_pBuffer, e_length); // 将密文数据拷贝到明文数据buff中，然后直接操作明文数据buff计算
    for (uint32_t i = 0; i < e_length; i += AES_BLOCKLEN)
    {
        memcpy(storeNextIv, s_pBuffer, AES_BLOCKLEN);           // 存储当前密文块作为下一轮的 IV
        InvCipher((aes_state_t *)s_pBuffer, context->RoundKey); // 解密当前块
        XorWithIv(s_pBuffer, context->Iv);                      // CBC 模式：将解密后的数据与 IV 或前一个密文块异或

        memcpy(context->Iv, storeNextIv, AES_BLOCKLEN); // 更新 IV 为当前密文块
        s_pBuffer += AES_BLOCKLEN;                      // 指向下一组数据
    }
    memcpy(context->Iv, context->IvStore, AES_BLOCKLEN); // 解密完成后，重置Iv值

    // 处理填充
    s_pBuffer -= e_length; // 指针回退到原始位置
    if (padding_mode == AES_PADDING_PKCS7)
    {
        uint8_t u8padding_val = s_pBuffer[e_length - 1]; // 获取最后一个字节的值
        if ((u8padding_val <= AES_BLOCKLEN) && (u8padding_val != 0))
        {
            // 验证填充是否正确
            for (uint8_t j = 0; j < u8padding_val; j++)
            {
                if (s_pBuffer[e_length - 1 - j] != u8padding_val)
                {
                    u8tempFlag = 1; // 异常
                    break;
                }
            }

            if (u8tempFlag)
            {
                *s_pLength = e_length; // 填充无效，返回全部数据
            }
            else
            {
                *s_pLength = e_length - u8padding_val; // 原始数据长度
            }
        }
        else
        {
            *s_pLength = e_length; // 没有有效填充
        }
    }
    else if (padding_mode == AES_PADDING_ZERO)
    {
        // 对于零填充，需要找到最后一个非零字节的位置
        uint32_t actual_len = e_length;
        for (int32_t i = e_length - 1; i >= 0; i--)
        {
            if (s_pBuffer[i] != 0x00)
            {
                actual_len = i + 1;
                break;
            }
        }
        *s_pLength = actual_len;
    }
    else // No padding or unsupported mode
    {
        *s_pLength = e_length;
    }
}

/**************************************************************
*函数功能描述 ：aes算法加密操作，ECB模式，NoPadding填充模式
*函数参数 ：context，aes算法控制结构体
            pBuffer，存储明文数据地址，直接加密生成密文也存储在这里
            length，明文/密文数据长度，明文长度==密文长度
*函数返回值 ：
*备注 ：明文长度length必须是块（16）的倍数
**************************************************************/
void aes_ecb_encrypt_none(aes_ctx_t *context, uint8_t *pBuffer, uint32_t length)
{
    if ((NULL == context) || (NULL == pBuffer) || (0 == length) || (0 != (length % AES_BLOCKLEN)))
    { // 判断入参有效性
        return;
    }

    for (uint32_t i = 0; i < length; i += AES_BLOCKLEN)
    {
        // ECB 模式：每个块独立加密，不需要 IV 或与前一块异或
        Cipher((aes_state_t *)pBuffer, context->RoundKey); // 加密此组数据，直接更改pBuffer数值从明文到密文

        pBuffer += AES_BLOCKLEN; // 指向下一组数据
    }
}

/**************************************************************
*函数功能描述 ：aes算法解密操作，ECB模式，NoPadding填充模式
*函数参数 ：context，aes算法控制结构体
            pBuffer，存储密文数据地址，直接解密生成明文也存储在这里
            length，明文/密文数据长度，明文长度==密文长度
*函数返回值 ：
*备注 ：密文长度length必须是块（16）的倍数
**************************************************************/
void aes_ecb_decrypt_none(aes_ctx_t *context, uint8_t *pBuffer, uint32_t length)
{
    if ((NULL == context) || (NULL == pBuffer) || (0 == length) || (0 != (length % AES_BLOCKLEN)))
    { // 判断入参有效性
        return;
    }

    for (uint32_t i = 0; i < length; i += AES_BLOCKLEN)
    {
        // ECB 模式：每个块独立解密，不需要 IV 或与前一块异或
        InvCipher((aes_state_t *)pBuffer, context->RoundKey); // 把此组密文数据转换成明文

        pBuffer += AES_BLOCKLEN; // 指向下一组数据
    }
}

/**************************************************************
*函数功能描述 ：aes算法加密操作，ECB模式
*函数参数 ：context，aes算法控制结构体
            s_pBuffer，存储明文数据地址
            s_length，明文数据长度
            e_pBuffer，存储密文数据地址
            e_pLength，存储密文长度地址
            padding_mode，填充模式
*函数返回值 ：
*备注 ：实际计算没有更改s_pBuffer区域，直接在e_pBuffer区域进行数据变更；
        非none填充模式时，明文长度s_length可以不是16的倍数
**************************************************************/
void aes_ecb_encrypt(aes_ctx_t *context, const uint8_t *s_pBuffer, uint32_t s_length,
                     uint8_t *e_pBuffer, uint32_t *e_pLength, AES_PADDING_MODE_E padding_mode)
{
    if ((NULL == context) || (NULL == s_pBuffer) || (0 == s_length) || (NULL == e_pBuffer) || (NULL == e_pLength) || (padding_mode >= AES_PADDING_END))
    { // 判断入参有效性
        return;
    }

    uint8_t u8padding_val = 0; // 待填充的数据值

    if (padding_mode == AES_PADDING_PKCS7)
    {
        u8padding_val = AES_BLOCKLEN - (s_length % AES_BLOCKLEN);
        *e_pLength = s_length + u8padding_val; // 实际要处理的数据长度，16的倍数

        // 根据明文填充数据
        memcpy(e_pBuffer, s_pBuffer, s_length);
        for (uint8_t i = 0; i < u8padding_val; i++)
        {
            e_pBuffer[s_length + i] = u8padding_val; // PKCS7填充数据
        }
    }
    else if (padding_mode == AES_PADDING_ZERO)
    {
        u8padding_val = AES_BLOCKLEN - (s_length % AES_BLOCKLEN);
        if (u8padding_val == AES_BLOCKLEN)
            u8padding_val = 0;                 // 如果刚好是16的倍数，不需要额外填充
        *e_pLength = s_length + u8padding_val; // 实际要处理的数据长度，16的倍数

        // 根据明文填充数据
        memcpy(e_pBuffer, s_pBuffer, s_length);
        if (0 != u8padding_val)
        {
            for (uint8_t i = 0; i < u8padding_val; i++)
            {
                e_pBuffer[s_length + i] = 0x00; // 零填充数据
            }
        }
    }
    else // No padding
    {
        if (0 != (s_length % AES_BLOCKLEN))
        {
            return; // 长度必须是块（16）的倍数
        }
        *e_pLength = s_length;
        memcpy(e_pBuffer, s_pBuffer, s_length);
    }

    // ECB 加密：每个块独立加密，不需要 IV
    for (uint32_t i = 0; i < *e_pLength; i += AES_BLOCKLEN)
    {
        Cipher((aes_state_t *)e_pBuffer, context->RoundKey); // 加密此组数据，直接更改e_pBuffer数值从明文到密文
        e_pBuffer += AES_BLOCKLEN;                           // 指向下一组数据
    }
}

/**************************************************************
*函数功能描述 ：aes算法解密操作，ECB模式
*函数参数 ：context，aes算法控制结构体
            e_pBuffer，存储密文数据地址
            e_length，密文数据长度
            s_pBuffer，存储明文数据地址
            s_pLength，存储明文长度地址
            padding_mode，填充模式
*函数返回值 ：
*备注 ：密文长度e_length必须是块（16）的倍数
**************************************************************/
void aes_ecb_decrypt(aes_ctx_t *context, const uint8_t *e_pBuffer, uint32_t e_length,
                     uint8_t *s_pBuffer, uint32_t *s_pLength, AES_PADDING_MODE_E padding_mode)
{
    if ((NULL == context) || (NULL == e_pBuffer) || (0 == e_length) || (NULL == s_pBuffer) || (NULL == s_pLength) || (0 != (e_length % AES_BLOCKLEN)))
    {
        return;
    }

    // ECB 解密：每个块独立解密，不需要 IV
    memcpy(s_pBuffer, e_pBuffer, e_length); // 将密文数据拷贝到明文数据buff中，然后直接操作明文数据buff计算
    for (uint32_t i = 0; i < e_length; i += AES_BLOCKLEN)
    {
        InvCipher((aes_state_t *)s_pBuffer, context->RoundKey); // 解密当前块
        s_pBuffer += AES_BLOCKLEN;                              // 指向下一组数据
    }

    // 处理填充
    s_pBuffer -= e_length; // 指针回退到原始位置
    if (padding_mode == AES_PADDING_PKCS7)
    {
        uint8_t u8padding_val = s_pBuffer[e_length - 1]; // 获取最后一个字节的值
        if ((u8padding_val <= AES_BLOCKLEN) && (u8padding_val != 0))
        {
            // 验证填充是否正确
            uint8_t u8tempFlag = 0;
            for (uint8_t j = 0; j < u8padding_val; j++)
            {
                if (s_pBuffer[e_length - 1 - j] != u8padding_val)
                {
                    u8tempFlag = 1; // 异常
                    break;
                }
            }

            if (u8tempFlag)
            {
                *s_pLength = e_length; // 填充无效，返回全部数据
            }
            else
            {
                *s_pLength = e_length - u8padding_val; // 原始数据长度
            }
        }
        else
        {
            *s_pLength = e_length; // 没有有效填充
        }
    }
    else if (padding_mode == AES_PADDING_ZERO)
    {
        // 对于零填充，需要找到最后一个非零字节的位置
        uint32_t actual_len = e_length;
        for (int32_t i = e_length - 1; i >= 0; i--)
        {
            if (s_pBuffer[i] != 0x00)
            {
                actual_len = i + 1;
                break;
            }
        }
        *s_pLength = actual_len;
    }
    else // No padding or unsupported mode
    {
        *s_pLength = e_length;
    }
}

// aes算法密钥扩展
static void KeyExpansion(uint8_t *RoundKey, const uint8_t *Key)
{
    unsigned i, j, k;
    uint8_t tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0; i < AES_Nk; ++i)
    {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = AES_Nk; i < AES_Nb * (AES_Nr + 1); ++i)
    {
        k = (i - 1) * 4;
        tempa[0] = RoundKey[k + 0];
        tempa[1] = RoundKey[k + 1];
        tempa[2] = RoundKey[k + 2];
        tempa[3] = RoundKey[k + 3];

        if (i % AES_Nk == 0)
        {
            // This function shifts the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]
            const uint8_t u8tmp = tempa[0];
            tempa[0] = tempa[1];
            tempa[1] = tempa[2];
            tempa[2] = tempa[3];
            tempa[3] = u8tmp;

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.
            tempa[0] = g_u8AesSbox[tempa[0]];
            tempa[1] = g_u8AesSbox[tempa[1]];
            tempa[2] = g_u8AesSbox[tempa[2]];
            tempa[3] = g_u8AesSbox[tempa[3]];

            tempa[0] = tempa[0] ^ g_u8AesRcon[i / AES_Nk];
        }

#if defined(AES_256)
        if (i % AES_Nk == 4)
        {
            tempa[0] = g_u8AesSbox[tempa[0]];
            tempa[1] = g_u8AesSbox[tempa[1]];
            tempa[2] = g_u8AesSbox[tempa[2]];
            tempa[3] = g_u8AesSbox[tempa[3]];
        }
#endif

        j = i * 4;
        k = (i - AES_Nk) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

// aes算法对密文进行加密
static void Cipher(aes_state_t *state, const uint8_t *RoundKey)
{
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    AddRoundKey(0, state, RoundKey);

    // There will be AES_Nr rounds.
    // The first AES_Nr-1 rounds are identical.
    // These AES_Nr rounds are executed in the loop below.
    // Last one without MixColumns()
    for (round = 1;; ++round)
    {
        SubBytes(state);
        ShiftRows(state);
        if (round == AES_Nr)
        {
            break;
        }
        MixColumns(state);
        AddRoundKey(round, state, RoundKey);
    }
    // Add round key to last round
    AddRoundKey(AES_Nr, state, RoundKey);
}

// aes算法对明文进行解密
static void InvCipher(aes_state_t *state, const uint8_t *RoundKey)
{
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    AddRoundKey(AES_Nr, state, RoundKey);

    // There will be AES_Nr rounds.
    // The first AES_Nr-1 rounds are identical.
    // These AES_Nr rounds are executed in the loop below.
    // Last one without InvMixColumn()
    for (round = (AES_Nr - 1);; --round)
    {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(round, state, RoundKey);
        if (round == 0)
        {
            break;
        }
        InvMixColumns(state);
    }
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
static void AddRoundKey(uint8_t round, aes_state_t *state, const uint8_t *RoundKey)
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            (*state)[i][j] ^= RoundKey[(round * AES_Nb * 4) + (i * AES_Nb) + j];
        }
    }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void SubBytes(aes_state_t *state)
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            (*state)[j][i] = g_u8AesSbox[(*state)[j][i]];
        }
    }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void InvSubBytes(aes_state_t *state)
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            (*state)[j][i] = g_u8AesRsbox[(*state)[j][i]];
        }
    }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
static void ShiftRows(aes_state_t *state)
{
    uint8_t temp;

    // Rotate first row 1 columns to left
    temp = (*state)[0][1];
    (*state)[0][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[3][1];
    (*state)[3][1] = temp;

    // Rotate second row 2 columns to left
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to left
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[3][3];
    (*state)[3][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[1][3];
    (*state)[1][3] = temp;
}

static void InvShiftRows(aes_state_t *state)
{
    uint8_t temp;

    // Rotate first row 1 columns to right
    temp = (*state)[3][1];
    (*state)[3][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[0][1];
    (*state)[0][1] = temp;

    // Rotate second row 2 columns to right
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to right
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[1][3];
    (*state)[1][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[3][3];
    (*state)[3][3] = temp;
}

// MixColumns function mixes the columns of the state matrix
static void MixColumns(aes_state_t *state)
{
    uint8_t i;
    uint8_t Tmp, Tm, t;
    for (i = 0; i < 4; ++i)
    {
        t = (*state)[i][0];
        Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3];
        Tm = (*state)[i][0] ^ (*state)[i][1];
        Tm = xtime(Tm);
        (*state)[i][0] ^= Tm ^ Tmp;
        Tm = (*state)[i][1] ^ (*state)[i][2];
        Tm = xtime(Tm);
        (*state)[i][1] ^= Tm ^ Tmp;
        Tm = (*state)[i][2] ^ (*state)[i][3];
        Tm = xtime(Tm);
        (*state)[i][2] ^= Tm ^ Tmp;
        Tm = (*state)[i][3] ^ t;
        Tm = xtime(Tm);
        (*state)[i][3] ^= Tm ^ Tmp;
    }
}

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
static void InvMixColumns(aes_state_t *state)
{
    int i;
    uint8_t a, b, c, d;
    for (i = 0; i < 4; ++i)
    {
        a = (*state)[i][0];
        b = (*state)[i][1];
        c = (*state)[i][2];
        d = (*state)[i][3];

        (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}

// 异或操作
static void XorWithIv(uint8_t *buf, const uint8_t *Iv)
{
    for (uint8_t i = 0; i < AES_BLOCKLEN; ++i) // The block in AES is always 128bit no matter the key size
    {
        buf[i] ^= Iv[i];
    }
}

static uint8_t xtime(uint8_t x)
{
    return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
}

static uint8_t Multiply(uint8_t x, uint8_t y)
{
    return (((y & 1) * x) ^
            ((y >> 1 & 1) * xtime(x)) ^
            ((y >> 2 & 1) * xtime(xtime(x))) ^
            ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^
            ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x)))))); /* this last call to xtime() can be omitted */
}

#if 0 // 调用例子
    aes_ctx_t fw_aes_ctx;
    aes_init(&fw_aes_ctx, NULL);

    // 16,30313233343536373839616263646566
    uint8_t s_data_nono[128] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66};
    uint32_t s_length_nono = 16;
    uint8_t e_data_nono[128] = {0};
    uint32_t e_length_nono = sizeof(e_data_nono);
    // 20,3031323334353637383961626364656634353637
    uint8_t s_data[128] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x34, 0x35, 0x36, 0x37};
    uint32_t s_length = 20;
    uint8_t e_data[128] = {0};
    uint32_t e_length = sizeof(e_data);

    // cbc，明文数据是块（16）的倍数
    aes_cbc_encrypt_none(&fw_aes_ctx, s_data_nono, s_length_nono);
    printf("aes cbc none en");
    printf_hex(s_data_nono, s_length_nono); // 64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B

    aes_cbc_decrypt_none(&fw_aes_ctx, s_data_nono, s_length_nono);
    printf("aes cbc none de");
    printf_hex(s_data_nono, s_length_nono);

    aes_cbc_encrypt(&fw_aes_ctx, s_data_nono, s_length_nono, e_data_nono, &e_length_nono, AES_PADDING_NONE);
    printf("aes cbc none en");
    printf_hex(e_data_nono, e_length_nono); // 64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B

    aes_cbc_decrypt(&fw_aes_ctx, e_data_nono, e_length_nono, s_data_nono, &s_length_nono, AES_PADDING_NONE);
    printf("aes cbc none de");
    printf_hex(s_data_nono, s_length_nono);

    aes_cbc_encrypt(&fw_aes_ctx, s_data_nono, s_length_nono, e_data_nono, &e_length_nono, AES_PADDING_ZERO);
    printf("aes cbc zero en");
    printf_hex(e_data_nono, e_length_nono); // 64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B

    aes_cbc_decrypt(&fw_aes_ctx, e_data_nono, e_length_nono, s_data_nono, &s_length_nono, AES_PADDING_ZERO);
    printf("aes cbc zero de");
    printf_hex(s_data_nono, s_length_nono);

    aes_cbc_encrypt(&fw_aes_ctx, s_data_nono, s_length_nono, e_data_nono, &e_length_nono, AES_PADDING_PKCS7);
    printf("aes cbc pkcs7 en");
    printf_hex(e_data_nono, e_length_nono); // 64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B DE 0A 12 68 43 6E 15 94 34 FC 21 DE 36 96 D9 28

    aes_cbc_decrypt(&fw_aes_ctx, e_data_nono, e_length_nono, s_data_nono, &s_length_nono, AES_PADDING_PKCS7);
    printf("aes cbc pkcs7 de");
    printf_hex(s_data_nono, s_length_nono);

    // ecb，明文数据是块（16）的倍数
    aes_ecb_encrypt_none(&fw_aes_ctx, s_data_nono, s_length_nono);
    printf("aes ecb none en");
    printf_hex(s_data_nono, s_length_nono); // 5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70

    aes_ecb_decrypt_none(&fw_aes_ctx, s_data_nono, s_length_nono);
    printf("aes ecb none de");
    printf_hex(s_data_nono, s_length_nono);

    aes_ecb_encrypt(&fw_aes_ctx, s_data_nono, s_length_nono, e_data_nono, &e_length_nono, AES_PADDING_NONE);
    printf("aes ecb none en");
    printf_hex(e_data_nono, e_length_nono); // 5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70

    aes_ecb_decrypt(&fw_aes_ctx, e_data_nono, e_length_nono, s_data_nono, &s_length_nono, AES_PADDING_NONE);
    printf("aes ecb none de");
    printf_hex(s_data_nono, s_length_nono);

    aes_ecb_encrypt(&fw_aes_ctx, s_data_nono, s_length_nono, e_data_nono, &e_length_nono, AES_PADDING_ZERO);
    printf("aes ecb zero en");
    printf_hex(e_data_nono, e_length_nono); // 5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70

    aes_ecb_decrypt(&fw_aes_ctx, e_data_nono, e_length_nono, s_data_nono, &s_length_nono, AES_PADDING_ZERO);
    printf("aes ecb zero de");
    printf_hex(s_data_nono, s_length_nono);

    aes_ecb_encrypt(&fw_aes_ctx, s_data_nono, s_length_nono, e_data_nono, &e_length_nono, AES_PADDING_PKCS7);
    printf("aes ecb pkcs7 en");
    printf_hex(e_data_nono, e_length_nono); // 5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70 A2 54 BE 88 E0 37 DD D9 D7 9F B6 41 1C 3F 9D F8

    aes_ecb_decrypt(&fw_aes_ctx, e_data_nono, e_length_nono, s_data_nono, &s_length_nono, AES_PADDING_PKCS7);
    printf("aes ecb pkcs7 de");
    printf_hex(s_data_nono, s_length_nono);

    // cbc，明文数据不是块（16）的倍数
    aes_cbc_encrypt(&fw_aes_ctx, s_data, s_length, e_data, &e_length, AES_PADDING_ZERO);
    printf("aes cbc zero en");
    printf_hex(e_data, e_length); // 64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B 3F 9E FD D1 68 85 DC 20 BE 9E C6 BC 1D AA D5 B1

    aes_cbc_decrypt(&fw_aes_ctx, e_data, e_length, s_data, &s_length, AES_PADDING_ZERO);
    printf("aes cbc zero de");
    printf_hex(s_data, s_length);

    aes_cbc_encrypt(&fw_aes_ctx, s_data, s_length, e_data, &e_length, AES_PADDING_PKCS7);
    printf("aes cbc pkcs7 en");
    printf_hex(e_data, e_length); // 64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B 13 8B AD AB 76 23 50 DD 0A 98 E4 46 A1 CF 5D 41

    aes_cbc_decrypt(&fw_aes_ctx, e_data, e_length, s_data, &s_length, AES_PADDING_PKCS7);
    printf("aes cbc pkcs7 de");
    printf_hex(s_data, s_length);

    aes_cbc_encrypt(&fw_aes_ctx, s_data, s_length, e_data, &e_length, AES_PADDING_NONE);
    printf("aes cbc none en");
    printf_hex(e_data, e_length); // 这里明文不是块的大小，所以实际上不会执行加密

    // aes_cbc_decrypt(&fw_aes_ctx, e_data, e_length, s_data, &s_length, AES_PADDING_NONE);
    // printf("aes cbc none de");
    // printf_hex(s_data, s_length); // 这里会改变s_data的原始值

    // ecb，明文数据不是块（16）的倍数
    aes_ecb_encrypt(&fw_aes_ctx, s_data, s_length, e_data, &e_length, AES_PADDING_ZERO);
    printf("aes ecb zero en");
    printf_hex(e_data, e_length); // 5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70 CE 2C FD 86 15 04 58 36 62 5C B0 60 97 E9 64 03

    aes_ecb_decrypt(&fw_aes_ctx, e_data, e_length, s_data, &s_length, AES_PADDING_ZERO);
    printf("aes ecb zero de");
    printf_hex(s_data, s_length);

    aes_ecb_encrypt(&fw_aes_ctx, s_data, s_length, e_data, &e_length, AES_PADDING_PKCS7);
    printf("aes ecb pkcs7 en");
    printf_hex(e_data, e_length); // 5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70 D8 11 25 FC 26 59 F7 7D 4F 9D 5A 72 F3 97 60 BB

    aes_ecb_decrypt(&fw_aes_ctx, e_data, e_length, s_data, &s_length, AES_PADDING_PKCS7);
    printf("aes ecb pkcs7 de");
    printf_hex(s_data, s_length);

    aes_ecb_encrypt(&fw_aes_ctx, s_data, s_length, e_data, &e_length, AES_PADDING_NONE);
    printf("aes ecb none en");
    printf_hex(e_data, e_length); // 这里明文不是块的大小，所以实际上不会执行加密

    aes_ecb_decrypt(&fw_aes_ctx, e_data, e_length, s_data, &s_length, AES_PADDING_NONE);
    printf("aes ecb none de");
    printf_hex(s_data, s_length); // 这里会改变s_data的原始值

/*
计算结果参考：
AES在线网站：https://www.toolhelper.cn/SymmetricEncryption/AES

AES256
aes cbc none en[16]:AF 79 82 F6 78 F4 98 BE 05 3F 0E C0 6B C5 47 CA 
aes cbc none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes cbc none en[16]:AF 79 82 F6 78 F4 98 BE 05 3F 0E C0 6B C5 47 CA
aes cbc none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes cbc zero en[16]:AF 79 82 F6 78 F4 98 BE 05 3F 0E C0 6B C5 47 CA
aes cbc zero de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes cbc pkcs7 en[32]:AF 79 82 F6 78 F4 98 BE 05 3F 0E C0 6B C5 47 CA F8 1D AB 90 22 F1 00 99 91 CD F5 A0 2A 86 4A 5D
aes cbc pkcs7 de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes ecb none en[16]:C6 D9 B4 57 EB CF 3F 0B 5A EF 67 D2 93 08 58 04
aes ecb none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes ecb none en[16]:C6 D9 B4 57 EB CF 3F 0B 5A EF 67 D2 93 08 58 04 
aes ecb none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes ecb zero en[16]:C6 D9 B4 57 EB CF 3F 0B 5A EF 67 D2 93 08 58 04
aes ecb zero de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes ecb pkcs7 en[32]:C6 D9 B4 57 EB CF 3F 0B 5A EF 67 D2 93 08 58 04 4C 45 DF B3 B3 B4 84 EC 35 B0 51 2D C8 C1 C4 D6
aes ecb pkcs7 de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes cbc zero en[32]:AF 79 82 F6 78 F4 98 BE 05 3F 0E C0 6B C5 47 CA 78 9B 7D A6 73 41 D2 7A E3 C0 75 AE 6C 8B D1 55
aes cbc zero de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes cbc pkcs7 en[32]:AF 79 82 F6 78 F4 98 BE 05 3F 0E C0 6B C5 47 CA BB 94 1D F1 89 9E 38 1F 58 8E F1 90 22 79 97 29
aes cbc pkcs7 de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes cbc none en[32]:AF 79 82 F6 78 F4 98 BE 05 3F 0E C0 6B C5 47 CA BB 94 1D F1 89 9E 38 1F 58 8E F1 90 22 79 97 29
aes ecb zero en[32]:C6 D9 B4 57 EB CF 3F 0B 5A EF 67 D2 93 08 58 04 77 8E B7 BD C1 28 8F 52 9F 76 9A 1E B4 66 84 EA
aes ecb zero de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes ecb pkcs7 en[32]:C6 D9 B4 57 EB CF 3F 0B 5A EF 67 D2 93 08 58 04 26 C6 D5 AD 49 9F 42 C3 45 4E FA A8 FD B2 82 1B
aes ecb pkcs7 de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes ecb none en[32]:C6 D9 B4 57 EB CF 3F 0B 5A EF 67 D2 93 08 58 04 26 C6 D5 AD 49 9F 42 C3 45 4E FA A8 FD B2 82 1B
aes ecb none de[32]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C


AES192
aes cbc none en[16]:90 4C F1 35 2E 42 2A 95 7C B7 D6 CD 5B EE C7 F0 
aes cbc none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes cbc none en[16]:90 4C F1 35 2E 42 2A 95 7C B7 D6 CD 5B EE C7 F0 
aes cbc none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes cbc zero en[16]:90 4C F1 35 2E 42 2A 95 7C B7 D6 CD 5B EE C7 F0
aes cbc zero de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes cbc pkcs7 en[32]:90 4C F1 35 2E 42 2A 95 7C B7 D6 CD 5B EE C7 F0 73 D8 CD 8F 6B AB 0D DC 91 86 D5 CC 80 F3 BD 39
aes cbc pkcs7 de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes ecb none en[16]:DD A1 B5 59 96 E8 42 CC 0C B4 8B DC F3 AE D6 09
aes ecb none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes ecb none en[16]:DD A1 B5 59 96 E8 42 CC 0C B4 8B DC F3 AE D6 09
aes ecb none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes ecb zero en[16]:DD A1 B5 59 96 E8 42 CC 0C B4 8B DC F3 AE D6 09 
aes ecb zero de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes ecb pkcs7 en[32]:DD A1 B5 59 96 E8 42 CC 0C B4 8B DC F3 AE D6 09 DA A0 AF 07 4B D8 08 3C 8A 32 D4 FC 56 3C 55 CC 
aes ecb pkcs7 de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes cbc zero en[32]:90 4C F1 35 2E 42 2A 95 7C B7 D6 CD 5B EE C7 F0 6D 3C 44 6E D1 2A FB 41 20 47 9A 52 C4 CE 36 13 
aes cbc zero de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes cbc pkcs7 en[32]:90 4C F1 35 2E 42 2A 95 7C B7 D6 CD 5B EE C7 F0 C9 F5 91 23 4A A3 7D B6 9E CF 0C 0C 76 43 65 23 
aes cbc pkcs7 de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes cbc none en[32]:90 4C F1 35 2E 42 2A 95 7C B7 D6 CD 5B EE C7 F0 C9 F5 91 23 4A A3 7D B6 9E CF 0C 0C 76 43 65 23 
aes ecb zero en[32]:DD A1 B5 59 96 E8 42 CC 0C B4 8B DC F3 AE D6 09 81 C0 42 96 73 B0 81 00 F9 18 39 21 CD D3 BA CD
aes ecb zero de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37 
aes ecb pkcs7 en[32]:DD A1 B5 59 96 E8 42 CC 0C B4 8B DC F3 AE D6 09 D3 13 98 70 99 36 20 5A 71 47 B9 AC AB 8C 5E AD
aes ecb pkcs7 de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37 
aes ecb none en[32]:DD A1 B5 59 96 E8 42 CC 0C B4 8B DC F3 AE D6 09 D3 13 98 70 99 36 20 5A 71 47 B9 AC AB 8C 5E AD
aes ecb none de[32]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 


AES128
aes cbc none en[16]:64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B 
aes cbc none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes cbc none en[16]:64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B
aes cbc none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes cbc zero en[16]:64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B
aes cbc zero de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes cbc pkcs7 en[32]:64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B DE 0A 12 68 43 6E 15 94 34 FC 21 DE 36 96 D9 28 
aes cbc pkcs7 de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes ecb none en[16]:5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70
aes ecb none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes ecb none en[16]:5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70
aes ecb none de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes ecb zero en[16]:5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70
aes ecb zero de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 
aes ecb pkcs7 en[32]:5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70 A2 54 BE 88 E0 37 DD D9 D7 9F B6 41 1C 3F 9D F8
aes ecb pkcs7 de[16]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66
aes cbc zero en[32]:64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B 3F 9E FD D1 68 85 DC 20 BE 9E C6 BC 1D AA D5 B1 
aes cbc zero de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes cbc pkcs7 en[32]:64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B 13 8B AD AB 76 23 50 DD 0A 98 E4 46 A1 CF 5D 41 
aes cbc pkcs7 de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes cbc none en[32]:64 76 85 48 00 7A EF 9F 3D 25 8E 5C 34 CD C2 1B 13 8B AD AB 76 23 50 DD 0A 98 E4 46 A1 CF 5D 41
aes ecb zero en[32]:5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70 CE 2C FD 86 15 04 58 36 62 5C B0 60 97 E9 64 03 
aes ecb zero de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes ecb pkcs7 en[32]:5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70 D8 11 25 FC 26 59 F7 7D 4F 9D 5A 72 F3 97 60 BB 
aes ecb pkcs7 de[20]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37
aes ecb none en[32]:5D 9C AF 02 52 9E E0 02 DC FF 2B 13 FF 1A 8F 70 D8 11 25 FC 26 59 F7 7D 4F 9D 5A 72 F3 97 60 BB
aes ecb none de[32]:30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 34 35 36 37 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C
*/
#endif
