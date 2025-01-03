#ifndef  _HAL_FLASH_H_
#define  _HAL_FLASH_H_
/*******************************头文件*******************************/
#include <stdint.h>

/*******************************宏定义*******************************/
#define  HAL_FLASH_CS 			        (0)//0是内部flash；1/2是外部flash，需要初始化对应io口

//HS6621CG-C的内部flash大小为1024kb
//用户自定数据区域设定为1000~1016kb,需要保证为一个扇区大小的倍数，这里一个扇区大小是SF_SECTOR_SIZE = 4kb
#define  HAL_FLASH_USER_DATA_START 		(1000*1024)
#define  HAL_FLASH_USER_DATA_END 		(1017*1024)//1024kb flash，从1017kb开始就是系统信息

#define  HAL_FLASH_SN_LEN 				(6)//SN码长度，bytes

/*******************************结构体、枚举等******************************/
typedef struct
{
    uint8_t sn[HAL_FLASH_SN_LEN];//序列号
    uint8_t pairNum;//已配对手机个数
    uint8_t  res1[3];//预留
    uint8_t administratorMAC[6];//管理员用户mac addr，用于权限认定
    uint8_t  res2[2];//预留

    uint16_t AdvInterval;//广播间隔
    uint8_t  res3[2];//预留
}hal_flash_sys_info_t;

/*******************************全局变量的外部声明extern*******************************/
/*******************************全局函数的声明*******************************/
void hal_flash_init(void);
void hal_flash_deinit(void);
void hal_flash_erase(uint32_t addr,uint32_t size);
void hal_flash_read(uint32_t addr,uint8_t *pData,uint32_t len);
void hal_flash_write(uint32_t addr,uint8_t *pData,uint32_t len);

#endif
