/*
 * bsp_crc.h
 *
 *  Created on: 2023年6月19日
 *      Author: lj
 *	  Abstract: 用于crc计算，包括crc8、crc16、crc32，有计算和查表两种方式，前者比较耗时，后者占据存储空间
 */
#ifndef _BSP_CRC_H_
#define _BSP_CRC_H_

/*******************************头文件*******************************/

/*******************************宏定义*******************************/
//决定是否使能crc8、crc16、crc32代码
//#define BSP_CRC_8
#define BSP_CRC_16
//#define BSP_CRC_32
// #define BSP_CRC_64//必须要在支持64位的系统上运行，嵌入式一般用不到

/*******************************结构体、枚举******************************/

/*******************************全局变量的外部声明extern*******************************/

/*******************************全局函数的声明*******************************/
#ifdef BSP_CRC_8
unsigned char bsp_crc8_calculate(unsigned char *pData, unsigned int len);
unsigned char bsp_crc8_table(unsigned char *pData, unsigned int len);
unsigned char bsp_crc8_calculate_MAXIM(unsigned char *pData, unsigned int len);
unsigned char bsp_crc8_table_MAXIM(unsigned char *pData, unsigned int len);
#endif

#ifdef BSP_CRC_16
unsigned short int bsp_crc16_calculate(unsigned char *pData, unsigned int len);
unsigned short int bsp_crc16_table(unsigned char *pData, unsigned int len);
#endif

#ifdef BSP_CRC_32
unsigned int bsp_crc32_calculate(unsigned char *pData, unsigned int len);
unsigned int bsp_crc32_table(unsigned char *pData, unsigned int len);
#endif

#endif /* _BSP_CRC_H_ */
