/*
 * bsp_rsa.c
 *
 *  Created on: 2024年8月1日
 *      Author: lj
 *	  Abstract: RSA算法实现，用于数据的加解密，例如AES的密钥传递或数字签名，参考：https://blog.csdn.net/chengjunchengjun/article/details/125981437?spm=1001.2014.3001.5506
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "bsp_rsa.h"
#include "app_log.h"

/*******************************全局变量定义*******************************/
#if defined(BSP_RSA_512)
/*原始密钥pem格式
-----BEGIN RSA PUBLIC KEY-----
MEgCQQDTuAWKRH57s/BQ6T1RLTsqa0RBoUKien656TaBjklYpyppo+fsWqXncLG5
2ErgPTXgQELRv7uR4xr4MJhwwFOrAgMBAAE=
-----END RSA PUBLIC KEY-----

-----BEGIN RSA PRIVATE KEY-----
MIIBOgIBAAJBANO4BYpEfnuz8FDpPVEtOyprREGhQqJ6frnpNoGOSVinKmmj5+xa
pedwsbnYSuA9NeBAQtG/u5HjGvgwmHDAU6sCAwEAAQJAX+eRIuQKA5EsAArXiskZ
bwquHail3aHte9unpCJHGU+00Ngm2CjrLbjJOorhIuhfqIj5bdF5+sjjpac8k2mQ
sQIhAPVzi4yJgoJfV4Ucq253EC6C0QIbSdp7L4b6gjrn0D4zAiEA3NFZ/drmY+P0
PbhClfo//I1ZTFSCWay44uXqkkCPrKkCIGjLnWAycbtSE57sn7D5Orpw9eXNWtni
UGdz21hlEv8LAiA5oSmrZwXZ7BRCR2SZX8ZzUzCLC0M5xCo8jWS65r6YgQIhAKAx
qLfdZKwbdOUO9Ge401ugzLJB9dV5vuIlZ/Aef5dL
-----END RSA PRIVATE KEY-----
*/
//指数
int8_t *g_RsaPublicKey_exponent = "010001";
int8_t *g_RsaPrivateKey_exponent = "5fe79122e40a03912c000ad78ac9196f0aae1da8a5dda1ed7bdba7a42247194fb4d0d826d828eb2db8c93a8ae122e85fa888f96dd179fac8e3a5a73c936990b1";

//模数
int8_t *g_RsaModulus ="d3b8058a447e7bb3f050e93d512d3b2a6b4441a142a27a7eb9e936818e4958a72a69a3e7ec5aa5e770b1b9d84ae03d35e04042d1bfbb91e31af8309870c053ab";
#elif defined(BSP_RSA_1024)
/*原始密钥pem格式
-----BEGIN RSA PUBLIC KEY-----
MIGJAoGBAMPDZFZB9l46NQozq592E9USKw3IVpOXLsuYl1gURsrrZSu06A0IvHD5
yueVCR/EzH9QpGDrx54J9LihqbtpeFHSpWkSFsVlxkEeATsQn8mNeLkQ4bTaVn3M
MoXJTHgVsVOjpAvzzB08OwOji23XvVwTQ9OpNntcQdXnz4fmgCATAgMBAAE=
-----END RSA PUBLIC KEY-----

-----BEGIN RSA PRIVATE KEY-----
MIICXgIBAAKBgQDDw2RWQfZeOjUKM6ufdhPVEisNyFaTly7LmJdYFEbK62UrtOgN
CLxw+crnlQkfxMx/UKRg68eeCfS4oam7aXhR0qVpEhbFZcZBHgE7EJ/JjXi5EOG0
2lZ9zDKFyUx4FbFTo6QL88wdPDsDo4tt171cE0PTqTZ7XEHV58+H5oAgEwIDAQAB
AoGBAMLMZcWnsb6l8S2LqRXua2G0yH7Ygb4vzHLwDnmpA6DciDnrtJVzwgAxsISt
Ov0P2eY+mrOPK33JrmkjBUDmfMBBZ+Qj7O4b7httvuO5XNBihKhutBe2u2gvqMWc
NVIVyPwIeXRwynhxdTMPl4aqpKdTrN/W1+FXZ1fBSuI6zKhRAkEA4vXi+WIcXDBj
DkOYV9CvLuiydoHy6WE14aJKX+WDVcdQo6bVie6+N+ABdNktAyjIv9iKwSop08RH
VOLRehB/NwJBANzPovEr3PABeg4ZQHz/VUlWSQaPT1X7OnqYcHcnUmqfqaopxYum
9VeB9Kbi17ApcpvwTLMaAI9bgpc3eO9pfAUCQQDLmFPh7pcORAG3+/8fV36vD+N3
1562qJOTMuiRGew0+LVn+PM55scQjIwB7Agq+10KbBAcs9wNsddjWI0deMsFAkAh
zqCR8DdJf9BiLDeVa7pxGgbgPDzXWzme34wIkjqqHHvrk62tLgjONxVwH7cKP65O
dcIogcioKkiaQmH/tdTJAkEAiJ2HrlR4ET6WjN8uacEqOuLqGcxBU2iN3JGsqXBm
icoM3xVsJPwz9TIakE3HQ58PGYEKylpxP2WXlMq6qKl8Xg==
-----END RSA PRIVATE KEY-----
*/
//指数
int8_t *g_RsaPublicKey_exponent = "010001";
int8_t *g_RsaPrivateKey_exponent = "c2cc65c5a7b1bea5f12d8ba915ee6b61b4c87ed881be2fcc72f00e79a903a0dc8839ebb49573c20031b084ad3afd0fd9e63e9ab38f2b7dc9ae69230540e67cc04167e423ecee1bee1b6dbee3b95cd06284a86eb417b6bb682fa8c59c355215c8fc08797470ca787175330f9786aaa4a753acdfd6d7e1576757c14ae23acca851";

//模数
int8_t *g_RsaModulus ="c3c3645641f65e3a350a33ab9f7613d5122b0dc85693972ecb9897581446caeb652bb4e80d08bc70f9cae795091fc4cc7f50a460ebc79e09f4b8a1a9bb697851d2a5691216c565c6411e013b109fc98d78b910e1b4da567dcc3285c94c7815b153a3a40bf3cc1d3c3b03a38b6dd7bd5c1343d3a9367b5c41d5e7cf87e6802013";
#elif defined(BSP_RSA_2048)
/*原始密钥pem格式
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwmejRhw/SB2xB3rgJYhg
OWBDX/DponDVVPzTWhn3J4INv6jUa9HDkeHhys4OOZTNajr8kRy4TIemotnIYONJ
noW7VyIQEAkEyxcMett5mqRPBLuyc8Fygn4ho/rd9JId4+PgKLmr6NVcuZCpVXPe
gyqNx0nR/UojISbq/Bu+NlcStmicZUuAeVbkGGUOlvtMzFehkBPwE31EdpYUq+/z
LuJ8OaxC+zm5PFo2AZJfI5Gz5lgb1g5ud0TG1JUrm9Dl5/JSNSL3SXBEC77mdfd0
BA5VFl8lV7IfTfSTUE9IKoMevqZxoaGpyN+ZcBby5NgsoqJJ6vmcJRFjI92UrFHV
1wIDAQAB
-----END PUBLIC KEY-----

-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDCZ6NGHD9IHbEH
euAliGA5YENf8OmicNVU/NNaGfcngg2/qNRr0cOR4eHKzg45lM1qOvyRHLhMh6ai
2chg40mehbtXIhAQCQTLFwx623mapE8Eu7JzwXKCfiGj+t30kh3j4+Aouavo1Vy5
kKlVc96DKo3HSdH9SiMhJur8G742VxK2aJxlS4B5VuQYZQ6W+0zMV6GQE/ATfUR2
lhSr7/Mu4nw5rEL7Obk8WjYBkl8jkbPmWBvWDm53RMbUlSub0OXn8lI1IvdJcEQL
vuZ193QEDlUWXyVXsh9N9JNQT0gqgx6+pnGhoanI35lwFvLk2Cyioknq+ZwlEWMj
3ZSsUdXXAgMBAAECggEAEnEUaw0475VpesUsSEM0pZy9J3fKIg/EHQjS3+RAru3G
ch0I8aV3gPpFmiCL9uhnyCEKXpWz4gaoRyCTwqUtEa2sBOsFTRAd9Uodc/YoBgR6
Pn+zwQlj3H8sn8qnjZDi5wBx/ksGxNKgtjXD6ohQXm8F/hbBpd6HkJiJiBr1o1/a
4Kzh9L1OpYVS5QF0EN3idNkt911vfjA+bqBXXjAf9IHvirQ2GeCNzmHygxiRhAWA
ki2fm0pGbKFUAfo+sgtDPT2jrA4rhSHG4mMr8IWBpr082aKNns7ujxT9ndBCl0Pp
7efPNCT4+zqfyVAB6x2rYhtXTqR7wsASMIfSLzyoIQKBgQDi2sb9z4OEl0mTV0Ey
1Lia4F06QhD7Kl1mhBWYPA1RO1ieAHHINFvwPJQLjHR2yMRMDmTOc3S9Hy3H3QMG
XZlrIl5sgeFzsdqGyd2kscGFUGkH9RY2V/XB0mxQFIGI2PBztqd6e6BJQz4oSyMz
cBZq/NH5u95QRlyI0I1u5b5lMQKBgQDbYZdhqHcDYmc61aHRNEhnxbK4ph1bdTiM
QbIF0wUssXQKwEzvYzswRlI27rXeSHpJA8vc+XAoD/+Ut30I8Td3yUnnsHZctU2D
b/8OTqwFqyrwXM+SvyWZejuvX6IVJuRwPMFG88L88lryEs0ntlQKC/x6wxtcs67X
6ZGsLSfJhwKBgQDAo09/kIv6OA4+lEXFSGZK/mOsaRXKcztFJry/vZ8BcAfchDwa
6nt4EbkV5XuwsuQeQcrQlbJ4NtXFdqRu72SsWU8djV1Jxanv89PHWzseXh4Sp8jo
9OC4aluX1RH6h14IpP6rP/fovrU1ujh2IaSnzXDxRNuQB2/krlSr62Q2wQKBgGNL
jbgvBwcqH+06So6lKmyFx/nZfgoqSVj6VzhZpcrv2sUO+wOTF3QnMAkbDIg6p9aq
eDhhUklfzF+kmVxVybRXEDNk5H3bteTa6Uexhhzet4WpjG4wRDVuZNtg3rzSKK1A
Yn7Z0BSrIUzWA7OIzArsF+/8pULVNTsWxc93dL27AoGAT3eaPv07rhzyztxFYTsr
dHcgNb+vwroN3Ic7IJnxoa2BjyqczaAG3vMrOcxFg2MLJ7fPmI1W4pPOJyxmUhO0
mZhk+F7h+dg7XB7h0lqR1usp3Ak3qyu1f4XZj4imzQWWyETpgE4uA9tBniPQIRK6
miXhZ2YoXfdsSZOatWojGLI=
-----END PRIVATE KEY-----
*/

//指数
int8_t *g_RsaPublicKey_exponent = "10001";
int8_t *g_RsaPrivateKey_exponent = "1271146b0d38ef95697ac52c484334a59cbd2777ca220fc41d08d2dfe440aeedc6721d08f1a57780fa459a208bf6e867c8210a5e95b3e206a8472093c2a52d11adac04eb054d101df54a1d73f62806047a3e7fb3c10963dc7f2c9fcaa78d90e2e70071fe4b06c4d2a0b635c3ea88505e6f05fe16c1a5de87909889881af5a35fdae0ace1f4bd4ea58552e5017410dde274d92df75d6f7e303e6ea0575e301ff481ef8ab43619e08dce61f2831891840580922d9f9b4a466ca15401fa3eb20b433d3da3ac0e2b8521c6e2632bf08581a6bd3cd9a28d9eceee8f14fd9dd0429743e9ede7cf3424f8fb3a9fc95001eb1dab621b574ea47bc2c0123087d22f3ca821";

// nt8_t g_RsaPublicKey_exponent[] = {0x01,0x00,0x01};
// int8_t g_RsaPrivateKey_exponent[] = 
// {
//     0x12,0x71,0x14,0x6b,0x0d,0x38,0xef,0x95,0x69,0x7a,0xc5,0x2c,0x48,0x43,0x34,0xa5,
//     0x9c,0xbd,0x27,0x77,0xca,0x22,0x0f,0xc4,0x1d,0x08,0xd2,0xdf,0xe4,0x40,0xae,0xed,
//     0xc6,0x72,0x1d,0x08,0xf1,0xa5,0x77,0x80,0xfa,0x45,0x9a,0x20,0x8b,0xf6,0xe8,0x67,
//     0xc8,0x21,0x0a,0x5e,0x95,0xb3,0xe2,0x06,0xa8,0x47,0x20,0x93,0xc2,0xa5,0x2d,0x11,
//     0xad,0xac,0x04,0xeb,0x05,0x4d,0x10,0x1d,0xf5,0x4a,0x1d,0x73,0xf6,0x28,0x06,0x04,
//     0x7a,0x3e,0x7f,0xb3,0xc1,0x09,0x63,0xdc,0x7f,0x2c,0x9f,0xca,0xa7,0x8d,0x90,0xe2,
//     0xe7,0x00,0x71,0xfe,0x4b,0x06,0xc4,0xd2,0xa0,0xb6,0x35,0xc3,0xea,0x88,0x50,0x5e,
//     0x6f,0x05,0xfe,0x16,0xc1,0xa5,0xde,0x87,0x90,0x98,0x89,0x88,0x1a,0xf5,0xa3,0x5f,
//     0xda,0xe0,0xac,0xe1,0xf4,0xbd,0x4e,0xa5,0x85,0x52,0xe5,0x01,0x74,0x10,0xdd,0xe2,
//     0x74,0xd9,0x2d,0xf7,0x5d,0x6f,0x7e,0x30,0x3e,0x6e,0xa0,0x57,0x5e,0x30,0x1f,0xf4,
//     0x81,0xef,0x8a,0xb4,0x36,0x19,0xe0,0x8d,0xce,0x61,0xf2,0x83,0x18,0x91,0x84,0x05,
//     0x80,0x92,0x2d,0x9f,0x9b,0x4a,0x46,0x6c,0xa1,0x54,0x01,0xfa,0x3e,0xb2,0x0b,0x43,
//     0x3d,0x3d,0xa3,0xac,0x0e,0x2b,0x85,0x21,0xc6,0xe2,0x63,0x2b,0xf0,0x85,0x81,0xa6,
//     0xbd,0x3c,0xd9,0xa2,0x8d,0x9e,0xce,0xee,0x8f,0x14,0xfd,0x9d,0xd0,0x42,0x97,0x43,
//     0xe9,0xed,0xe7,0xcf,0x34,0x24,0xf8,0xfb,0x3a,0x9f,0xc9,0x50,0x01,0xeb,0x1d,0xab,
//     0x62,0x1b,0x57,0x4e,0xa4,0x7b,0xc2,0xc0,0x12,0x30,0x87,0xd2,0x2f,0x3c,0xa8,0x21
// };

//模数
int8_t *g_RsaModulus ="c267a3461c3f481db1077ae02588603960435ff0e9a270d554fcd35a19f727820dbfa8d46bd1c391e1e1cace0e3994cd6a3afc911cb84c87a6a2d9c860e3499e85bb572210100904cb170c7adb799aa44f04bbb273c172827e21a3faddf4921de3e3e028b9abe8d55cb990a95573de832a8dc749d1fd4a232126eafc1bbe365712b6689c654b807956e418650e96fb4ccc57a19013f0137d44769614abeff32ee27c39ac42fb39b93c5a3601925f2391b3e6581bd60e6e7744c6d4952b9bd0e5e7f2523522f74970440bbee675f774040e55165f2557b21f4df493504f482a831ebea671a1a1a9c8df997016f2e4d82ca2a249eaf99c25116323dd94ac51d5d7";

// int8_t g_RsaModulus[BSP_RSA_ENCODE_LEN] =
// {//直接使用16进制数据可略微提高速度，软件也支持传入字符串，具体看代码中 GetHex 和 Get
//     0xC2, 0x67, 0xA3, 0x46, 0x1C, 0x3F, 0x48, 0x1D, 0xB1, 0x07, 0x7A, 0xE0, 0x25, 0x88, 0x60, 0x39,
//     0x60, 0x43, 0x5F, 0xF0, 0xE9, 0xA2, 0x70, 0xD5, 0x54, 0xFC, 0xD3, 0x5A, 0x19, 0xF7, 0x27, 0x82,
//     0x0D, 0xBF, 0xA8, 0xD4, 0x6B, 0xD1, 0xC3, 0x91, 0xE1, 0xE1, 0xCA, 0xCE, 0x0E, 0x39, 0x94, 0xCD,
//     0x6A, 0x3A, 0xFC, 0x91, 0x1C, 0xB8, 0x4C, 0x87, 0xA6, 0xA2, 0xD9, 0xC8, 0x60, 0xE3, 0x49, 0x9E,
//     0x85, 0xBB, 0x57, 0x22, 0x10, 0x10, 0x09, 0x04, 0xCB, 0x17, 0x0C, 0x7A, 0xDB, 0x79, 0x9A, 0xA4,
//     0x4F, 0x04, 0xBB, 0xB2, 0x73, 0xC1, 0x72, 0x82, 0x7E, 0x21, 0xA3, 0xFA, 0xDD, 0xF4, 0x92, 0x1D,
//     0xE3, 0xE3, 0xE0, 0x28, 0xB9, 0xAB, 0xE8, 0xD5, 0x5C, 0xB9, 0x90, 0xA9, 0x55, 0x73, 0xDE, 0x83,
//     0x2A, 0x8D, 0xC7, 0x49, 0xD1, 0xFD, 0x4A, 0x23, 0x21, 0x26, 0xEA, 0xFC, 0x1B, 0xBE, 0x36, 0x57,
//     0x12, 0xB6, 0x68, 0x9C, 0x65, 0x4B, 0x80, 0x79, 0x56, 0xE4, 0x18, 0x65, 0x0E, 0x96, 0xFB, 0x4C,
//     0xCC, 0x57, 0xA1, 0x90, 0x13, 0xF0, 0x13, 0x7D, 0x44, 0x76, 0x96, 0x14, 0xAB, 0xEF, 0xF3, 0x2E,
//     0xE2, 0x7C, 0x39, 0xAC, 0x42, 0xFB, 0x39, 0xB9, 0x3C, 0x5A, 0x36, 0x01, 0x92, 0x5F, 0x23, 0x91,
//     0xB3, 0xE6, 0x58, 0x1B, 0xD6, 0x0E, 0x6E, 0x77, 0x44, 0xC6, 0xD4, 0x95, 0x2B, 0x9B, 0xD0, 0xE5,
//     0xE7, 0xF2, 0x52, 0x35, 0x22, 0xF7, 0x49, 0x70, 0x44, 0x0B, 0xBE, 0xE6, 0x75, 0xF7, 0x74, 0x04,
//     0x0E, 0x55, 0x16, 0x5F, 0x25, 0x57, 0xB2, 0x1F, 0x4D, 0xF4, 0x93, 0x50, 0x4F, 0x48, 0x2A, 0x83,
//     0x1E, 0xBE, 0xA6, 0x71, 0xA1, 0xA1, 0xA9, 0xC8, 0xDF, 0x99, 0x70, 0x16, 0xF2, 0xE4, 0xD8, 0x2C,
//     0xA2, 0xA2, 0x49, 0xEA, 0xF9, 0x9C, 0x25, 0x11, 0x63, 0x23, 0xDD, 0x94, 0xAC, 0x51, 0xD5, 0xD7
// };
#endif

/*******************************内部变量定义*******************************/
static const uint8_t base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*******************************内部函数的声明*******************************/
static bsp_rsa_cbigint_t *Mov_Big_Long(bsp_rsa_cbigint_t *X, uint32_t A);
static bsp_rsa_cbigint_t *Add_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A);
static bsp_rsa_cbigint_t *Add_Big_Long(bsp_rsa_cbigint_t *X, uint32_t A);
static bsp_rsa_cbigint_t *Sub_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A);
#if (BSP_RSA_NONE)
static bsp_rsa_cbigint_t *Sub_Big_Long(bsp_rsa_cbigint_t *X, uint32_t A);
#endif
static bsp_rsa_cbigint_t *Mul_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A);
static bsp_rsa_cbigint_t *Mul_Big_Long(bsp_rsa_cbigint_t *X, uint32_t A);
#if (BSP_RSA_NONE)
static bsp_rsa_cbigint_t *Div_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A);
static bsp_rsa_cbigint_t *Div_Big_Long(bsp_rsa_cbigint_t *X, uint32_t A);
#endif
static bsp_rsa_cbigint_t *Mod_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A);
#if (BSP_RSA_NONE)
static uint32_t Mod_Big_Long(bsp_rsa_cbigint_t *N, uint32_t A);
#endif
static int32_t Cmp(bsp_rsa_cbigint_t *N, bsp_rsa_cbigint_t *A);

static bsp_rsa_cbigint_t *RsaTrans(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A, bsp_rsa_cbigint_t *B);

static bsp_rsa_cbigint_t *Get(bsp_rsa_cbigint_t *N, int8_t *str, uint32_t system);
static bsp_rsa_cbigint_t *GetHex(bsp_rsa_cbigint_t *N, uint8_t *s, uint16_t len, uint32_t system);
#if (BSP_RSA_NONE)
static int8_t *Put(bsp_rsa_cbigint_t *N, uint32_t system);
#endif
static void PutHex(bsp_rsa_cbigint_t *N, uint8_t *out, uint16_t *len);

#if (BSP_RSA_NONE)
static uint32_t rand_seed_get(void);
#endif
static void entropy_poll(uint8_t *output, uint32_t len);
static int32_t add_PKCS1Padding(uint8_t *input, uint32_t len, uint8_t *output);
static int32_t remove_PKCS1Padding(uint8_t *input, uint16_t *len, uint8_t *output);

uint8_t * base64_encode(const uint8_t *src, size_t len, size_t *out_len);
uint8_t * base64_decode(const uint8_t *src, size_t len, size_t *out_len);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：公钥加密，PKCS1填充模式
*函数参数 ：s_pBuffer，明文数据存储地址
            s_length，明文数据长度，支持长度小于等于（BSP_RSA_ENCODE_LEN - 11）的明文数据
            e_pBuffer，密文数据存储地址
            publicKey，指向公钥指数e指针，字符串形式使用Get（），数组形式使用GetHex（）
            Modulus，指向公私钥模数n指针，字符串形式使用Get（），数组形式使用GetHex（）
*函数返回值 ：密文数据长度，如果为UINT16_MAX代表长度超过，无法加密
*备注 ：
**************************************************************/
uint16_t bsp_rsa_public_encrypt(uint8_t *s_pBuffer, uint16_t s_length, uint8_t *e_pBuffer, int8_t *publicKey, int8_t *Modulus)
{
    uint16_t e_len = BSP_RSA_ENCODE_LEN;
    uint8_t buf[BSP_RSA_ENCODE_LEN];
    bsp_rsa_cbigint_t N, E, mw;

    if(-1 == add_PKCS1Padding(s_pBuffer, s_length, buf))
    {//填充明文数据，用buf存储
        e_len = UINT16_MAX;
    }
    else
    {
        GetHex(&mw, buf, BSP_RSA_ENCODE_LEN, 16);//处理明文数据

        Get(&N, Modulus, 16);//从string获取计算元素
        // GetHex(&N, (uint8_t *)Modulus, BSP_RSA_ENCODE_LEN, 16);//从hex获取计算元素
        Get(&E, publicKey, 16);
        // GetHex(&E, (uint8_t *)publicKey, 3, 16);

        RsaTrans(&mw, &E, &N);//这里最耗时
        PutHex(&mw, e_pBuffer, &e_len);
    }
    return e_len;
}

/**************************************************************
*函数功能描述 ：私钥解密，PKCS1填充模式
*函数参数 ：e_pBuffer，密文数据存储地址
            e_length，密文数据长度
            s_pBuffer，明文数据存储地址
            privateKey，指向私钥指数d指针，字符串形式使用Get（），数组形式使用GetHex（）
            Modulus，指向公私钥模数n指针，字符串形式使用Get（），数组形式使用GetHex（）
*函数返回值 ：明文数据长度
*备注 ：
**************************************************************/
uint16_t bsp_rsa_private_decrypt(uint8_t *e_pBuffer, uint16_t e_length, uint8_t *s_pBuffer, int8_t *privateKey, int8_t *Modulus)
{
    uint16_t s_len = 0;
    uint8_t buf[BSP_RSA_ENCODE_LEN];
    bsp_rsa_cbigint_t N, D, mw;

    Get(&N, Modulus, 16);//从string获取计算元素
    // GetHex(&N, (uint8_t *)Modulus, BSP_RSA_ENCODE_LEN, 16);//从hex获取计算元素
    Get(&D, privateKey, 16);
    // GetHex(&D, privateKey, BSP_RSA_ENCODE_LEN, 16);

    GetHex(&mw, e_pBuffer, e_length, 16);//处理密文数据

    RsaTrans(&mw, &D, &N);//这里最耗时
    PutHex(&mw, buf, &e_length);
    s_len = remove_PKCS1Padding(buf, &e_length, s_pBuffer);//去除填充数据
    return s_len;
}

/**************************************************************
*函数功能描述 ：私钥签名，PKCS1填充模式
*函数参数 ：s_pBuffer，明文数据存储地址
            s_length，明文数据长度，支持长度小于等于（BSP_RSA_ENCODE_LEN - 11）的明文数据
            e_pBuffer，密文数据存储地址
            privateKey，指向私钥指数d指针，字符串形式使用Get（），数组形式使用GetHex（）
            Modulus，指向公私钥模数n指针，字符串形式使用Get（），数组形式使用GetHex（）
*函数返回值 ：密文数据长度，如果为UINT16_MAX代表长度超过，无法加密
*备注 ：
**************************************************************/
uint16_t bsp_rsa_private_encrypt(uint8_t *s_pBuffer, uint16_t s_length, uint8_t *e_pBuffer, int8_t *privateKey, int8_t *Modulus)
{
    uint16_t e_len = BSP_RSA_ENCODE_LEN;
    uint8_t buf[BSP_RSA_ENCODE_LEN];
    bsp_rsa_cbigint_t N, E, mw;

    if(-1 == add_PKCS1Padding(s_pBuffer, s_length, buf))
    {//填充明文数据，用buf存储
        e_len = UINT16_MAX;
    }
    else
    {
        GetHex(&mw, buf, BSP_RSA_ENCODE_LEN, 16);//处理明文数据

        Get(&N, Modulus, 16);//从string获取计算元素
        // GetHex(&N, (uint8_t *)Modulus, BSP_RSA_ENCODE_LEN, 16);//从hex获取计算元素
        Get(&E, privateKey, 16);
        // GetHex(&E, privateKey, BSP_RSA_ENCODE_LEN, 16);

        RsaTrans(&mw, &E, &N);
        PutHex(&mw, e_pBuffer, &e_len);
    }
    return e_len;
}

/**************************************************************
*函数功能描述 ：公钥解签，PKCS1填充模式
*函数参数 ：e_pBuffer，密文数据存储地址
            e_length，密文数据长度
            s_pBuffer，明文数据存储地址
            publicKey，指向公钥指数e指针，字符串形式使用Get（），数组形式使用GetHex（）
            Modulus，指向公私钥模数n指针，字符串形式使用Get（），数组形式使用GetHex（）
*函数返回值 ：明文数据长度
*备注 ：
**************************************************************/
uint16_t bsp_rsa_public_decrypt(uint8_t *e_pBuffer, uint16_t e_length, uint8_t *s_pBuffer, int8_t *publicKey, int8_t *Modulus)
{
    uint16_t s_len = 0;
	uint8_t buf[BSP_RSA_ENCODE_LEN];
    bsp_rsa_cbigint_t N, D, mw;

    Get(&N, Modulus, 16);//从string获取计算元素
    // GetHex(&N, (uint8_t *)Modulus, BSP_RSA_ENCODE_LEN, 16);//从hex获取计算元素
    Get(&D, publicKey, 16);
    // GetHex(&D, publicKey, BSP_RSA_ENCODE_LEN, 16);

    GetHex(&mw, e_pBuffer, e_length, 16);//处理密文数据

    RsaTrans(&mw, &D, &N);
    PutHex(&mw, buf, &e_length);
    s_len = remove_PKCS1Padding(buf, &e_length, s_pBuffer);//去除填充数据
    return s_len;
}

#if (BSP_RSA_NONE)
//随机数种子源
static uint32_t rand_seed_get(void)
{
    time_t timestamp;
    time(&timestamp);
    return timestamp;
}
#endif

static void entropy_poll(uint8_t *output, uint32_t len)
{
	static uint32_t u32send = 0;
    if(len > 0)
    {
		//获取随机数种子方法，第一个利用time_t，但是可能会卡死或无法实现，所以可以用一个static变量自增实现
        //u32send = rand_seed_get();//第一种方法
		u32send += BSP_RSA_ENCODE_LEN;//第二种方法
        srand(u32send);
        for(uint32_t i = 0; i < len; i++)
        {
            output[i] = rand() % 0xff + 1;
        }
    }
}

static int32_t add_PKCS1Padding(uint8_t *input, uint32_t len, uint8_t *output)
{
    if(len > BSP_RSA_ENCODE_LEN - 11)
    {
        return -1;
    }
    else
    {
        /*要加密的msg*/
        memcpy(&output[BSP_RSA_ENCODE_LEN - len], input, len);//源数据放在output的末尾
        output[0] = 0;
        output[1] = 2;
        /*至少8字节的随机数*/
        entropy_poll(&output[2], BSP_RSA_ENCODE_LEN - 3 - len);//前半部分填充随机数
        output[BSP_RSA_ENCODE_LEN - len - 1] = 0;
        return 0;
    }
}

static int32_t remove_PKCS1Padding(uint8_t *input, uint16_t *len, uint8_t *output)
{
    if(input[0] == 0 && (input[1] == 1 || input[1] == 2))
    {
        uint32_t i;
        for(i = 2; i < *len; i++)
        {
            if(input[i] == 0)
            {
                *len -= (i + 1);
                memcpy(output, &input[i + 1], *len);//取出最后的有效数据
                return *len;
            }
        }
    }
    return -1;
}

/****************************************************************************************
大数比较,比较运算，可重载为运算符“==”、“!=”、“>=”、“<=”等
调用方式：Cmp(N,A)
返回值：若N<A返回-1；若N=A返回0；若N>A返回1
****************************************************************************************/
static int32_t Cmp(bsp_rsa_cbigint_t *N, bsp_rsa_cbigint_t *A)
{
    int32_t i;
    if(N->m_nLength > A->m_nLength)
    {
        return 1;
    }
    if(N->m_nLength < A->m_nLength)
    {
        return -1;
    }
    for(i = N->m_nLength - 1; i >= 0; i--)
    {
        if(N->m_ulValue[i] > A->m_ulValue[i])
        {
            return 1;
        }
        if(N->m_ulValue[i] < A->m_ulValue[i])
        {
            return -1;
        }
    }
    return 0;
}

/****************************************************************************************
大数赋值,赋值运算，可赋值为大数或普通整数，可重载为运算符“=”
调用方式：__Mov_Big_Big(A)
返回值：N,被赋值为A
****************************************************************************************/
static bsp_rsa_cbigint_t *Mov_Big_Long(bsp_rsa_cbigint_t *N, uint32_t A)
{
    if(A > BSP_RSA_CARRYLAST)
    {
        N->m_nLength = 2;
        N->m_ulValue[1] = (uint16_t)(A >> 16);
        N->m_ulValue[0] = (uint16_t)A;
    }
    else
    {
        N->m_nLength = 1;
        N->m_ulValue[0] = (uint16_t)A;
    }
    memset((uint8_t*)&N->m_ulValue[N->m_nLength], 0, sizeof(uint16_t) * (BSP_RSA_BI_MAXLEN - N->m_nLength));
    return N;
}

/****************************************************************************************
大数相加,加，求大数与大数或大数与普通整数的和，可重载为运算符“+”
调用形式：Add_Big_Big(X,A)
返回值：X=X+A
****************************************************************************************/
static bsp_rsa_cbigint_t *Add_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A)
{
    uint32_t i;
    uint16_t carry = 0;
    uint32_t sum = 0;
    if(X->m_nLength < A->m_nLength)
    {
        X->m_nLength = A->m_nLength;
    }
    for(i = 0; i < X->m_nLength; i++)
    {
        sum = A->m_ulValue[i];
        sum = sum + X->m_ulValue[i] + carry;
        X->m_ulValue[i] = (uint16_t)sum;
        carry = (uint16_t)(sum >> 16);
    }
    X->m_ulValue[X->m_nLength] = carry;
    X->m_nLength += carry;
    return X;
}

static bsp_rsa_cbigint_t *Add_Big_Long(bsp_rsa_cbigint_t *X, uint32_t A)
{
    uint32_t sum;
    sum = X->m_ulValue[0];
    sum += A;
    X->m_ulValue[0] = (uint16_t)sum;
    if(sum > BSP_RSA_CARRYLAST)
    {
        uint32_t i = 1;
        while(X->m_ulValue[i] == BSP_RSA_CARRYLAST)
        {
            X->m_ulValue[i] = 0;
            i++;
        }
        X->m_ulValue[i]++;
        if(X->m_nLength == i)
        {
            X->m_nLength++;
        }
    }
    return X;
}

/****************************************************************************************
大数相减，减，求大数与大数或大数与普通整数的差，可重载为运算符“-”
调用形式：Sub_Big_Big(X,A)
返回值：X=X-A
****************************************************************************************/
static bsp_rsa_cbigint_t *Sub_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A)
{
    if(Cmp(X, A) <= 0)
    {
        memset(X, 0, sizeof(bsp_rsa_cbigint_t));
        return X;
    }
    else
    {
        uint16_t carry = 0;
        uint32_t num;
        uint32_t i;
        for(i = 0; i < X->m_nLength; i++)
        {
            if((X->m_ulValue[i] > A->m_ulValue[i]) || ((X->m_ulValue[i] == A->m_ulValue[i]) && (carry == 0)))
            {
                X->m_ulValue[i] = X->m_ulValue[i] - carry - A->m_ulValue[i];
                carry = 0;
            }
            else
            {
                num = BSP_RSA_CARRYOVER + X->m_ulValue[i];
                X->m_ulValue[i] = (uint32_t)(num - carry - A->m_ulValue[i]);
                carry = 1;
            }
        }
        while(X->m_ulValue[X->m_nLength - 1] == 0)
        {
            X->m_nLength--;
        }
        return X;
    }
}

#if (BSP_RSA_NONE)
static bsp_rsa_cbigint_t *Sub_Big_Long(bsp_rsa_cbigint_t *X, uint32_t A)
{
    if(X->m_ulValue[0] >= A)
    {
        X->m_ulValue[0] -= A;
        return X;
    }
    if(X->m_nLength == 1)
    {
        memset(X, 0, sizeof(bsp_rsa_cbigint_t));
        return X;
    }
    else
    {
        uint32_t num = BSP_RSA_CARRYOVER + X->m_ulValue[0];
        int32_t i = 1;
        X->m_ulValue[0] = (uint16_t)(num - A);
        while(X->m_ulValue[i] == 0)
        {
            X->m_ulValue[i] = BSP_RSA_CARRYLAST;
            i++;
        }
        X->m_ulValue[i]--;
        if(X->m_ulValue[i] == 0)
        {
            X->m_nLength--;
        }
        return X;
    }
}
#endif
/****************************************************************************************
大数相乘
调用形式：Mul_Big_Big(N,A)
返回值：X=N*A
    A	a 0
    N	c d
        0					d*0
        1			c*0
                    d*a
        2	c*a

****************************************************************************************/
static bsp_rsa_cbigint_t *Mul_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A)
{
    if(A->m_nLength == 1)
    {
        return Mul_Big_Long(X, A->m_ulValue[0]);
    }
    else
    {
        uint32_t sum, mul = 0, carry = 0;
        int32_t i, j;
        bsp_rsa_cbigint_t N = {0};
        memcpy(&N, X, sizeof(bsp_rsa_cbigint_t));
        memset(X, 0, sizeof(bsp_rsa_cbigint_t));
        X->m_nLength = N.m_nLength + A->m_nLength - 1;
        for(i = 0; i < X->m_nLength; i++)
        {
            sum = carry;
            carry = 0;
            for(j = 0; j < A->m_nLength; j++)
            {
                if(((i - j) >= 0) && ((i - j) < N.m_nLength))
                {
                    mul = N.m_ulValue[i - j];
                    mul *= A->m_ulValue[j];
                    carry += mul >> 16;
                    mul = mul & BSP_RSA_CARRYLAST;
                    sum += mul;
                }
            }
            carry += sum >> 16;
            X->m_ulValue[i] = (uint16_t)sum;
        }
        if(carry)
        {
            X->m_nLength++;
            X->m_ulValue[X->m_nLength - 1] = (uint16_t)carry;
        }
        return X;
    }
}

//乘，求大数与大数或大数与普通整数的积，可重载为运算符“*”
static bsp_rsa_cbigint_t *Mul_Big_Long(bsp_rsa_cbigint_t *X, uint32_t A)
{
    uint32_t mul;
    uint32_t carry = 0;
    uint32_t i;
    for(i = 0; i < X->m_nLength; i++)
    {
        mul = X->m_ulValue[i];
        mul = mul * A + carry;
        X->m_ulValue[i] = (uint16_t)mul;
        carry = (uint16_t)(mul >> 16);
    }
    if(carry)
    {
        X->m_nLength++;
        X->m_ulValue[X->m_nLength - 1] = carry;
    }
    return X;
}

#if (BSP_RSA_NONE)
/****************************************************************************************
大数相除，除，求大数与大数或大数与普通整数的商，可重载为运算符
调用形式：Div_Big_Big(N,A)
返回值：X=N/A
****************************************************************************************/
static bsp_rsa_cbigint_t *Div_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A)
{
    bsp_rsa_cbigint_t Y = {0}, Z = {0}, T;
    if(A->m_nLength == 1)
    {
        return Div_Big_Long(X, A->m_ulValue[0]);
    }
    else
    {
        uint32_t i, len;
        uint32_t num, div;
        memcpy(&Y, X, sizeof(bsp_rsa_cbigint_t));
        while(Cmp(&Y, A) >= 0)
        {
            div = Y.m_ulValue[Y.m_nLength - 1];
            num = A->m_ulValue[A->m_nLength - 1];
            len = Y.m_nLength - A->m_nLength;
            if((div == num) && (len == 0))
            {
                Add_Big_Long(X, 1);
                break;
            }
            if((div <= num) && len)
            {
                len--;
                div = (div << 16) + Y.m_ulValue[Y.m_nLength - 2];
            }
            div = div / (num + 1);
            Mov_Big_Long(&Z, div);
            if(len)
            {
                Z.m_nLength += len;
                for(i = Z.m_nLength - 1; i >= len; i--)
                {
                    Z.m_ulValue[i] = Z.m_ulValue[i - len];
                }
                for(i = 0; i < len; i++)
                {
                    Z.m_ulValue[i] = 0;
                }
            }
            Add_Big_Big(X, &Z);
            memcpy(&T, A, sizeof(bsp_rsa_cbigint_t));
            Mul_Big_Big(&T, &Z);
            Sub_Big_Big(&Y, &T);
        }
        return X;
    }
}

static bsp_rsa_cbigint_t *Div_Big_Long(bsp_rsa_cbigint_t *X, uint32_t A)
{
    if(X->m_nLength == 1)
    {
        X->m_ulValue[0] = X->m_ulValue[0] / A;
        return X;
    }
    else
    {
        uint32_t div, mul;
        uint32_t carry = 0;
        int32_t i;
        for(i = X->m_nLength - 1; i >= 0; i--)
        {
            div = carry;
            div = (div << 16) + X->m_ulValue[i];
            X->m_ulValue[i] = (uint16_t)(div / A);
            mul = (div / A) * A;
            carry = (uint16_t)(div - mul);
        }
        if(X->m_ulValue[X->m_nLength - 1] == 0)
        {
            X->m_nLength--;
        }
        return X;
    }
}
#endif

/****************************************************************************************
大数求模,，模，求大数与大数或大数与普通整数的模，可重载为运算符“%”
调用形式：Mod_Big_Big(N,A)
返回值：X=N%A
****************************************************************************************/
static bsp_rsa_cbigint_t *Mod_Big_Big(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A)
{
    bsp_rsa_cbigint_t Y = {0}, Z;
    uint32_t div, num;
    uint32_t i, len;
    while(Cmp(X, A) >= 0)
    {
        div = X->m_ulValue[X->m_nLength - 1];
        num = A->m_ulValue[A->m_nLength - 1];
        len = X->m_nLength - A->m_nLength;
        if((div == num) && (len == 0))
        {
            Sub_Big_Big(X, A);
            break;
        }
        if((div <= num) && len)
        {
            len--;
            div = (div << 16) + X->m_ulValue[X->m_nLength - 2];
        }
        div = div / (num + 1);
        Mov_Big_Long(&Y, div);
        memcpy(&Z, A, sizeof(bsp_rsa_cbigint_t));
        Mul_Big_Big(&Z, &Y);
        memcpy(&Y, &Z, sizeof(bsp_rsa_cbigint_t));
        if(len)
        {
            Y.m_nLength += len;
            for(i = Y.m_nLength - 1; i >= len; i--)
            {
                Y.m_ulValue[i] = Y.m_ulValue[i - len];
            }
            for(i = 0; i < len; i++)
            {
                Y.m_ulValue[i] = 0;
            }
        }
        Sub_Big_Big(X, &Y);
    }
    return X;
}

#if (BSP_RSA_NONE)
static uint32_t Mod_Big_Long(bsp_rsa_cbigint_t *N, uint32_t A)
{
    if(N->m_nLength == 1)
    {
        return(N->m_ulValue[0] % A);
    }
    else
    {
        uint32_t div;
        uint32_t carry = 0;
        int32_t i;
        for(i = N->m_nLength - 1; i >= 0; i--)
        {
            div = N->m_ulValue[i];
            div += carry * BSP_RSA_CARRYOVER;
            carry = (uint16_t)(div % A);
        }
        return carry;
    }
}
#endif

/****************************************************************************************
从字符串按10进制或16进制格式输入到大数
调用格式：Get(N,str,sys)
返回值：N被赋值为相应大数
sys暂时只能为10或16
****************************************************************************************/
static bsp_rsa_cbigint_t *Get(bsp_rsa_cbigint_t *N, int8_t *s, uint32_t system)
{
    uint32_t i;
    uint32_t len = strlen((const char *)s), k;
    memset(N, 0, sizeof(bsp_rsa_cbigint_t));
    N->m_nLength = 1;
    for(i = 0; i < len; i++)
    {//每一个字符转换为一个hex，例如17变为0x31 0x37
        Mul_Big_Long(N, system);
        if((s[i] >= '0') && (s[i] <= '9'))
        {
            k = s[i] - 48;
        }
        else if((s[i] >= 'A') && (s[i] <= 'F'))
        {
            k = s[i] - 55;
        }
        else if((s[i] >= 'a') && (s[i] <= 'f'))
        {
            k = s[i] - 87;
        }
        else
        {
            k = 0;
        }
        Add_Big_Long(N, k);
    }
    return N;
}

static bsp_rsa_cbigint_t *GetHex(bsp_rsa_cbigint_t *N, uint8_t *s, uint16_t len, uint32_t system)
{
    int32_t i, j;
    uint8_t *p = (uint8_t*)N->m_ulValue;
    memset(N, 0, sizeof(bsp_rsa_cbigint_t));
    N->m_nLength = 1;
    for(i = len - 1, j = 0; i >= 0; i--, j++)
    {
        p[j] = s[i];
    }
    i = len % 2;
    if(i > 0)
    {
        N->m_nLength = len / 2 + 1;
    }
    else
    {
        N->m_nLength = len / 2;
    }
    return N;
}

#if (BSP_RSA_NONE)
/****************************************************************************************
将大数按10进制或16进制格式输出为字符串
调用格式：Put(N,str,sys)
返回值：无，参数str被赋值为N的sys进制字符串
sys暂时只能为10或16
****************************************************************************************/
static int8_t *Put(bsp_rsa_cbigint_t *N, uint32_t system)
{
    int8_t t[17] = "0123456789ABCDEF";
    int32_t i, a;
    static int8_t s[2048];

    if((N->m_nLength == 1) && (N->m_ulValue[0] == 0))
    {
        return NULL;
    }
    else
    {
        bsp_rsa_cbigint_t X = {0};
        memcpy(&X, N, sizeof(bsp_rsa_cbigint_t));
        memset(s, 0, 2048);
        for(i = 2046; X.m_ulValue[X.m_nLength - 1] > 0 && i > 0; i--)
        {
            a = Mod_Big_Long(&X, system);
            s[i] = t[a];
            Div_Big_Long(&X, system);
        }
        if(i % 2 == 0)
        {
            return &s[i + 1];
        }
        else
        {
            s[i] = '0';
            return &s[i];
        }
    }
}
#endif

static void PutHex(bsp_rsa_cbigint_t *N, uint8_t *out, uint16_t *len)
{
    int32_t i, j, size;
    if((N->m_nLength == 1) && (N->m_ulValue[0] == 0))
    {
        return;
    }
    size = N->m_nLength * sizeof(N->m_ulValue[0]);
    for(i = size - 1, j = 0; i >= 0; i--, j++)
    {
        out[j] = ((uint8_t*)N->m_ulValue)[i];
    }
    *len = size;
}

/****************************************************************************************
求乘方的模,反复平方算法进行幂模运算
调用方式：RsaTrans(N,A,B)
返回值：X=N^A MOD B
****************************************************************************************/
static bsp_rsa_cbigint_t *RsaTrans(bsp_rsa_cbigint_t *X, bsp_rsa_cbigint_t *A, bsp_rsa_cbigint_t *B)
{
    bsp_rsa_cbigint_t N = {0}, Y = {0}, Z;
    int32_t i, j, k;
    uint32_t n;
    uint32_t num;
    k = A->m_nLength * 16 - 16;
    num = A->m_ulValue[A->m_nLength - 1];
    while(num)
    {
        num = num >> 1;
        k++;
    }
    memcpy(&N, X, sizeof(bsp_rsa_cbigint_t));
    for(i = k - 2; i >= 0; i--)
    {
        memcpy(&Y, X, sizeof(bsp_rsa_cbigint_t));
        Mul_Big_Long(&Y, X->m_ulValue[X->m_nLength - 1]);
        Mod_Big_Big(&Y, B);
        for(n = 1; n < X->m_nLength; n++)
        {
            for(j = Y.m_nLength; j > 0; j--)
            {
                Y.m_ulValue[j] = Y.m_ulValue[j - 1];
            }
            Y.m_ulValue[0] = 0;
            Y.m_nLength++;
            memcpy(&Z, X, sizeof(bsp_rsa_cbigint_t));
            Mul_Big_Long(&Z, X->m_ulValue[X->m_nLength - n - 1]);
            Add_Big_Big(&Y, &Z);
            Mod_Big_Big(&Y, B);
        }
        memcpy(X, &Y, sizeof(bsp_rsa_cbigint_t));
        if((A->m_ulValue[i >> 4] >> (i & 15)) & 1)
        {
            memcpy(&Y, &N, sizeof(bsp_rsa_cbigint_t));
            Mul_Big_Long(&Y, X->m_ulValue[X->m_nLength - 1]);
            Mod_Big_Big(&Y, B);
            for(n = 1; n < X->m_nLength; n++)
            {
                for(j = Y.m_nLength; j > 0; j--)
                {
                    Y.m_ulValue[j] = Y.m_ulValue[j - 1];
                }
                Y.m_ulValue[0] = 0;
                Y.m_nLength++;
                memcpy(&Z, &N, sizeof(bsp_rsa_cbigint_t));
                Mul_Big_Long(&Z, X->m_ulValue[X->m_nLength - n - 1]);
                Add_Big_Big(&Y, &Z);
                Mod_Big_Big(&Y, B);
            }
            memcpy(X, &Y, sizeof(bsp_rsa_cbigint_t));
        }
    }
    return X;
}

//hex转换base64，需要释放内存，使用不RSA2048
uint8_t * base64_encode(const uint8_t *src, size_t len, size_t *out_len)
{
    uint8_t *out, *pos;
    const uint8_t *end, *in;
    size_t olen;
    int32_t line_len;

    olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
    olen += olen / 72; /* line feeds */
    olen++; /* nul termination */
    if(olen < len)
    {
        return NULL;    /* integer overflow */
    }
    out = malloc(olen);
    if(out == NULL)
    {
        return NULL;
    }

    end = src + len;
    in = src;
    pos = out;
    line_len = 0;
    while(end - in >= 3)
    {
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
        line_len += 4;
        if(line_len >= 72)
        {
            *pos++ = '\n';
            line_len = 0;
        }
    }

    if(end - in)
    {
        *pos++ = base64_table[in[0] >> 2];
        if(end - in == 1)
        {
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else
        {
            *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
        line_len += 4;
    }

    if(line_len)
    {
        *pos++ = '\n';
    }

    *pos = '\0';
    if(out_len)
    {
        *out_len = pos - out;
    }
    return out;
}

//base64转hex，需要释放内存
uint8_t * base64_decode(const uint8_t *src, size_t len, size_t *out_len)
{
    uint8_t dtable[256], *out, *pos, block[4], tmp;
    size_t i, count, olen;
    int32_t pad = 0;

    memset(dtable, 0x80, 256);
    for(i = 0; i < sizeof(base64_table) - 1; i++)
    {
        dtable[base64_table[i]] = (uint8_t) i;
    }
    dtable['='] = 0;

    count = 0;
    for(i = 0; i < len; i++)
    {
        if(dtable[src[i]] != 0x80)
        {
            count++;
        }
    }

    if(count == 0 || count % 4)
    {
        return NULL;
    }

    olen = count / 4 * 3;
    pos = out = malloc(olen);
    if(out == NULL)
    {
        return NULL;
    }

    count = 0;
    for(i = 0; i < len; i++)
    {
        tmp = dtable[src[i]];
        if(tmp == 0x80)
        {
            continue;
        }

        if(src[i] == '=')
        {
            pad++;
        }
        block[count] = tmp;
        count++;
        if(count == 4)
        {
            *pos++ = (block[0] << 2) | (block[1] >> 4);
            *pos++ = (block[1] << 4) | (block[2] >> 2);
            *pos++ = (block[2] << 6) | block[3];
            count = 0;
            if(pad)
            {
                if(pad == 1)
                {
                    pos--;
                }
                else if(pad == 2)
                {
                    pos -= 2;
                }
                else
                {
                    /* Invalid padding */
                    free(out);
                    return NULL;
                }
                break;
            }
        }
    }

    *out_len = pos - out;
    return out;
}

#if (1 == BSP_RSA_TEST)
static uint8_t encode_str[BSP_RSA_ENCODE_LEN] = {0};
static uint8_t decode_str[BSP_RSA_ENCODE_LEN] = {0};

static uint8_t pub_source_string[] = "embedded-system > Public key to RSA encode,hehe";
static uint8_t pri_source_string[] = "embedded-system > Private key to RSA encode,haha";

void bsp_rsa_test(void)
{
    uint16_t outlen = 0;

    hw_printf("RSA demo start\r\n");
    //------------------------------------------------------------------------------------------------------
	memset(encode_str, 0, sizeof(encode_str));
    memset(decode_str, 0, sizeof(decode_str));
    //公钥加密
    outlen = bsp_rsa_public_encrypt(pub_source_string, strlen((const char *)pub_source_string), encode_str, g_RsaPublicKey_exponent, g_RsaModulus);

    //hex和base64在线转换:https://www.btool.cn/base64-hex
    hw_printf("Public encode hex");
    hw_printf_hex(encode_str,outlen);

    // 私钥解密，240802测试解密256bytes耗时将近28min,240806测试解密64bytes耗时将近51s
    outlen = bsp_rsa_private_decrypt(encode_str, outlen, decode_str, g_RsaPrivateKey_exponent, g_RsaModulus);
    hw_printf("Private decode %d\r\n%s\r\n", outlen, decode_str);

    //------------------------------------------------------------------------------------------------------
    memset(encode_str, 0, sizeof(encode_str));
    memset(decode_str, 0, sizeof(decode_str));

    //私钥签名
    outlen = bsp_rsa_private_encrypt(pri_source_string, strlen((const char *)pri_source_string), encode_str, g_RsaPrivateKey_exponent, g_RsaModulus);

    //hex和base64在线转换:https://www.btool.cn/base64-hex
    hw_printf("Private encode hex");
    hw_printf_hex(encode_str,outlen);
    
    //公钥验签
    outlen = bsp_rsa_public_decrypt(encode_str, outlen, decode_str, g_RsaPublicKey_exponent, g_RsaModulus);
    hw_printf("Public decode %d\r\n%s\r\n", outlen, decode_str);
    //------------------------------------------------------------------------------------------------------

    hw_printf("RSA demo end\r\n");
}
#endif



