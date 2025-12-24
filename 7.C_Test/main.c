#include <stdio.h>
#include <stdint.h>
#include "aes_api.h"

#define SCANF_TEST 0
#define AES_TEST 0

void printf_hex(uint8_t *pData, uint32_t length)
{
    uint32_t i;
    printf("[%d]:", length);
    for (i = 0; i < length; i++)
    {
        printf("%02X ", pData[i]);
    }
    printf("\n");
}

int main()
{
    printf("Enter Main\n");
#if defined(SCANF_TEST) && SCANF_TEST
    int a, b, c;
    printf("Enter three numbers: ");
    scanf("%d %d %d", &a, &b, &c);

    if (a >= b && a >= c)
    {
        printf("Largest number is %d\n", a);
    }
    else if (b >= a && b >= c)
    {
        printf("Largest number is %d\n", b);
    }
    else
    {
        printf("Largest number is %d\n", c);
    }
#endif

#if defined(AES_TEST) && AES_TEST
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
#endif

    return 0;
}