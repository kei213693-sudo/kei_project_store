/**
  ******************************************************************************
  * @file    Templates/Inc/main.h 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    29-April-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define APP_NVIC_VECTAB_FLASH_OFFSET    (0x6000)//app程序中断向量表偏移地址，也可以理解为boot程序区域大小
#define APP_START_ADDR          		(FLASH_BASE | APP_NVIC_VECTAB_FLASH_OFFSET)//0x08006000//APP区域起始地址
#define APP_AREA_SIZE          		    (115 * STM_SECTOR_SIZE)//APP区域大小，115个扇区，230kb = 235520bytes
#define BOOT_START_ADDR          		(FLASH_BASE | 0x0000)//0x08004000//BOOT区域起始地址
#define BOOT_AREA_SIZE          		(12 * STM_SECTOR_SIZE)//BOOT区域大小，12个扇区，24kb = 24576bytes

//程序版本信息
#define APP_VERSION_MAIN				(0x01U) /*!< [31:24] main version */
#define APP_VERSION_SUB1        		(0x00U) /*!< [23:16] sub1 version */
#define APP_VERSION_SUB2        		(0x00U) /*!< [15:8]  sub2 version */
//程序编译时间
#define APP_VERSION_DATE      			(__DATE__)
#define APP_VERSION_TIME      			(__TIME__)

#define APP_WDT_ENABLE            		(0)//是否使能看门狗
#define APP_DEBUG_ENABLE          		(1)//是否使能串口调试用的指令处理
#define APP_SYSVIEW_ENABLE        		(1)//是否使能SystemView可视化调试
#define APP_BOOT_ENABLE            		(1)//是否使能boot刷写功能，keil中的设置也要对应修改

#define BIT(x)     						        (1u << (x))
/*
//如果不带freertos，用这个格式代替，就用这个宏定义作为返回参数
#ifndef pdPASS
#define pdPASS                          (1)//成功
#endif
#ifndef pdFAIL
#define pdFAIL                          (0)//失败
#endif
*/
/* Exported functions ------------------------------------------------------- */



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
