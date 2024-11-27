/*
 * bsp_upgrade.h
 *
 *  Created on: 2024年10月23日
 *      Author: lijing
 *	  Abstract: 程序刷写模块，实现多途径程序接收、校验、存储、刷写的流程，app层处理"传输方式、通信协议解析、传输过程加解密"，其他交给bsp层
 */
/*******************************预编译指令*******************************/
#ifndef _BSP_UPGRADE_H_
#define _BSP_UPGRADE_H_

/*******************************头文件*******************************/
#include "main.h"
#include "stmflash.h"
#include "hw_w25qxx.h"
#include "bsp_aes.h"
#include "bsp_md5.h"

/*******************************宏定义*******************************/
//内部flash参数
#define hw_inflash_write(addr,pData,len)    STMFLASH_Write(addr,(uint16_t *)pData,(uint16_t)len/2)
#define hw_inflash_read(addr,pData,len)     STMFLASH_Read(addr,(uint16_t *)pData,(uint16_t)len/2)
#define hw_inflash_erase(addr)              STMFLASH_Erase(addr)
#define BSP_INFLASH_SECTOR_SIZE             (STM_SECTOR_SIZE)//内部flash扇区大小，2k 

//外部flash参数
#define hw_exflash_write(addr,pData,len)    W25QXX_Write((uint8_t *)pData,addr,(uint16_t)len)
#define hw_exflash_read(addr,pData,len)     W25QXX_Read((uint8_t *)pData,addr,(uint16_t)len)
#define hw_exflash_erase(addr)              W25QXX_Erase_Sector(addr)
#define BSP_EXFLASH_SECTOR_SIZE             (4096)//外部flash扇区大小，4k

//通过以下宏定义决定升级方案
//1.程序存储参数，包括“存储区域类型、单位大小、起始地址、结束地址”
#define BSP_UPGRADE_STORE_TYPE                      (0)//存储区域类型，0外部flash，1内部flash，2u盘，3sd卡
#if (0 == BSP_UPGRADE_STORE_TYPE)
#define BSP_UPGRADE_BUF_LEN                         (BSP_EXFLASH_SECTOR_SIZE)//外部，W25Q256外部flash是4k
#define bsp_upgrade_store_write(addr,pData,len)     hw_exflash_write(addr,pData,len)
#define bsp_upgrade_store_read(addr,pData,len)      hw_exflash_read(addr,pData,len)
//各模块地址信息，如果有其他模块，继续定义存储地址范围
#define BSP_UPGRADE_APP_STORE_ADDR_S                (0x00000)//外部flash存储APP程序的起始地址
#define BSP_UPGRADE_APP_STORE_ADDR_E                (BSP_UPGRADE_APP_STORE_ADDR_S + 256*1024)//外部flash存储APP程序的终止地址，给256kb
#elif (1 == BSP_UPGRADE_STORE_TYPE)
#define BSP_UPGRADE_BUF_LEN                         (BSP_INFLASH_SECTOR_SIZE)//内部，stm32f103内部flash是2k
#define bsp_upgrade_store_write(addr,pData,len)     hw_inflash_write(addr,pData,len)
#define bsp_upgrade_store_read(addr,pData,len)      hw_inflash_read(addr,pData,len)
//各模块地址信息，如果有其他模块，继续定义存储地址范围
#define BSP_UPGRADE_APP_STORE_ADDR_S                (STM32_FLASH_BASE + STM32_FLASH_SIZE*1024/2)//内部flash存储APP程序的起始地址0x08004000
#define BSP_UPGRADE_APP_STORE_ADDR_E                (BSP_UPGRADE_APP_STORE_ADDR_S + 126*1024)//内部flash存储APP程序的终止地址，给126kb
#endif

//2.app的升级信息存储参数，包括“存储区域类型、起始地址、结束地址”
#define BSP_UPGRADE_INFO_TYPE                       (1)//存储区域类型，0外部flash，1内部flash，3eeprom
#if (0 == BSP_UPGRADE_INFO_TYPE)
#define bsp_upgrade_info_write(addr,pData,len)      hw_exflash_write(addr,pData,len)
#define bsp_upgrade_info_read(addr,pData,len)       hw_exflash_read(addr,pData,len)
//各模块地址信息，如果有其他模块，继续定义存储地址范围
#define BSP_UPGRADE_APP_INFO_ADDR_S                 (BSP_EXFLASH_STORE_ADDR_S + 256*1024)
#define BSP_UPGRADE_APP_INFO_ADDR_E                 (BSP_UPGRADE_APP_INFO_ADDR_S + BSP_EXFLASH_SECTOR_SIZE)//外部flash存储APP信息的终止地址，给4k
#elif (1 == BSP_UPGRADE_INFO_TYPE)
#define bsp_upgrade_info_write(addr,pData,len)      hw_inflash_write(addr,pData,len)
#define bsp_upgrade_info_read(addr,pData,len)       hw_inflash_read(addr,pData,len)
//各模块地址信息，如果有其他模块，继续定义存储地址范围
#define BSP_UPGRADE_APP_INFO_ADDR_S                 (STM32_FLASH_BASE + STM32_FLASH_SIZE*1024 - STM_SECTOR_SIZE)//内部flash存储APP信息的起始地址
#define BSP_UPGRADE_APP_INFO_ADDR_E                 (BSP_UPGRADE_APP_INFO_ADDR_S + BSP_INFLASH_SECTOR_SIZE)//内部flash存储APP信息的结束地址，给2k
#endif

//3.安全性参数，包括“存储加解密使能标志、数据加解密api（明文密文长度必须相同）”
//使能之后，接收和写入所需时间会边长，例如55kb的代码文件刷写时间由2.5s变长为10s
#define BSP_UPGRADE_SEC_EN            (1)//使能后写入/读取存储区域数据都要加解密一遍
#if (1 == BSP_UPGRADE_SEC_EN)
#define bsp_upgrade_en(context,pBuffer,length)      bsp_aes_encrypt_none(context,pBuffer,length)
#define bsp_upgrade_de(context,pBuffer,length)      bsp_aes_decrypt_none(context,pBuffer,length)
#endif

//4.权限认证参数，包括“权限认证使能标志、签名api、验签api”
#define BSP_UPGRADE_AUTH_EN           (0)//使能后需要验证升级权限，复用md5值进行数字签名认证
#if (1 == BSP_UPGRADE_AUTH_EN)
#define bsp_upgrade_sign(s_pBuffer,s_length,e_pBuffer,privateKey,Modulus)          bsp_rsa_private_encrypt(s_pBuffer,s_length,e_pBuffer,privateKey,Modulus)
#define bsp_upgrade_sign_verify(e_pBuffer,e_length,s_pBuffer,publicKey,Modulus)    bsp_rsa_public_decrypt(e_pBuffer,e_length,s_pBuffer,publicKey,Modulus)
#endif

/*******************************结构体、枚举******************************/
typedef enum
{//可升级区域枚举，每一个区域都需要专属的“信息存储区域、程序存储区域、控制结构体”
    BSP_UPGRADE_AREA_APP            = 0,//APP区域
    BSP_UPGRADE_AREA_END
}BSP_UPGRADE_AREA_E;

typedef struct bsp_upgrade_info_t
{//升级信息存储结构体，存储在内部flash或eeprom，52bytes
    //程序信息
    uint8_t u8Type;//代码类型，用于区分app、boot、外部组件模块等升级信息，也可以用高位字节存储产品类型
                    //；多模块升级方案，通过这个区分程序和升级信息存储区域
    uint8_t res1[3];
    uint32_t u32Version;//已存储程序的软件版本，例如220701或V1.0.0
    uint8_t u8Name[32];//代码文件名称
    uint32_t u32TotalLen;//总长度，bytes，最大接收4095Mb的数据

    //续传参数，仅支持自定义协议
    uint8_t u8Md5[BSP_MD5_OUT_LEN];//整包的一致性验证参数，和终止包的数值比较,也可以用crc代替
    uint32_t u32StoreLen;//已存储明文数据的长度,可能比u32TotalLen大，因为是BSP_AES_BLOCKLEN（16）的倍数

    //标志位信息
    uint8_t u8UpFlag;//升级标志
                    //；0x00，不操作直接跳转app
                    //；0x01，程序接收完成，需要刷写app代码，完成后再跳转
                    //；0x02，程序接收完校验失败，擦除升级包存储区域，将现有程序重新拷贝过去，重置该标志信息，完成后再跳转
                    //；0x03，程序接收未完成，意外中断(超时、掉电、卡死)，保存已有数据，等待续传
    uint8_t res2;

    uint16_t u16Crc;//该结构体数据的校验值，确定该结构体数据的有效性
}bsp_upgrade_info_t;

typedef struct bsp_upgrade_buf_t
{//数据接收结构体，不论什么升级方式，都用这个来临时存储数据
//，每存满一包或最后一包就往flash写一次
//，freertos去应用层动态申请，裸机就定义一个全局变量
    uint8_t u8Buf[BSP_UPGRADE_BUF_LEN];
    uint16_t u16BufLen;//已接收数据长度
    uint8_t res[2];
}bsp_upgrade_buf_t;

typedef struct bsp_upgrade_ctl_t
{//程序升级控制结构体
    //任意方案都用到
    bsp_upgrade_info_t stInfo;//存储升级信息
    bsp_upgrade_buf_t  stBuf;//存储消息缓存
    uint32_t u32StoreAddr;//存储区域的地址，依次递增
    uint32_t u32StoreSize;//存储区域大小
    uint32_t u32InfoAddr;//升级信息的地址
    uint32_t u32InfoSize;//升级信息区域大小
    uint8_t u8ProtocolType;//通信协议类型，0-Ymodem，1-自定义通信协议
    uint8_t res1[3];
    uint32_t u32ReceiveIndex;//已接收索引值
    uint32_t u32ReceiveLen;//已接收长度

    //自定义通信协议用到
    bsp_md5_ctx_t stMd5Ctx;
    #if (BSP_UPGRADE_SEC_EN)
    bsp_aes_ctx_t stAesCtx;
    #endif
    uint8_t u8ReceiveFlag;//接收标志，0不允许接收，1允许接受，2接收完成
    uint8_t res2[3];
}bsp_upgrade_ctl_t;

/*******************************全局变量的外部声明extern*******************************/
extern bsp_upgrade_ctl_t *g_pStBspUpgradeCtl;
extern uint8_t *g_pDataCode;

/*******************************全局函数的声明*******************************/
extern void bsp_upgrade_init(BSP_UPGRADE_AREA_E eArea);
extern void bsp_upgrade_store(uint8_t *pData,uint16_t u16Len,uint8_t u8Force);
extern uint8_t bsp_upgrade_auth(uint8_t *pData,uint16_t u16Len);
extern void bsp_upgrade_finish(uint8_t u8Flag);

extern void bsp_upgrade_copy_area_code(uint32_t u32CodeAddr,uint32_t u32CodeSize);
extern void bsp_upgrade_copy_area_store(uint32_t u32CodeAddr,uint32_t u32CodeSize);

#endif /* _BSP_UPGRADE_H_ */
