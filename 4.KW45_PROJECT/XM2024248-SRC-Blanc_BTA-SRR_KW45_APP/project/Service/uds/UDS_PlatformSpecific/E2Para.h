/***
 * @Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * @All rights reserved.
 * @FilePath: E2Para.h
 * @摘要:
 * @当前版本: V1.0.0 Chenjianbo
 * @Date: 2023-01-10 09:38:45
 * @历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */
#ifndef E2PARA_H
#define E2PARA_H

#include "stdint.h"


#define DID_F15B_LENGTH (12)
#define DID_F15A_LENGTH (12)
#define DID_F010_LENGTH (4)
#define DID_F190_LENGTH (17)
#define DID_F110_LENGTH (11)
#define DID_F111_LENGTH (2)
#define DID_F118_LENGTH (11)
#define DID_F119_LENGTH (2)
#define DID_F18A_LENGTH (8)
#define DID_F18B_LENGTH (3)
#define DID_F19E_LENGTH (3)
#define DID_F150_LENGTH (6)
#define DID_F18C_LENGTH (19)
#define DID_F193_LENGTH (3)
#define DID_F195_LENGTH (6)

#define DID_4A00_LENGTH (16)
#define DID_4A01_LENGTH (16)
#define DID_4A02_LENGTH (6)
#define DID_4A03_LENGTH (1)
#define DID_4A43_LENGTH (1)
#define DID_4A33_LENGTH (1)
#define DID_4A23_LENGTH (1)
#define DID_4A13_LENGTH (1)
#define DID_4A04_LENGTH (2)
#define DID_4A05_LENGTH (2)
#define DID_4A06_LENGTH (2)
#define DID_4AFF_LENGTH	(1)

#define DID_4A08_LENGTH (120)
#define DID_4A09_LENGTH (110)
#define DID_4A50_LENGTH (13)
#define DID_4A51_LENGTH (8)
#define DID_4A52_LENGTH (10)
#define DID_4A53_LENGTH (10)
#define DID_4A54_LENGTH (6)
#define DID_4A55_LENGTH (6)
#define DID_4A1F_LENGTH (3)
#define DID_F130_LENGTH (11)
#define DID_F131_LENGTH (2)
#define DID_F151_LENGTH (6)


#define DID_WRITE_DATA_LEN			(128u)
typedef union
{
	uint8_t write_buffer[DID_WRITE_DATA_LEN];
	struct
	{
		uint8_t f110_data[DID_F110_LENGTH];					/*len = 11,在供应商会话模式和安全模式下进行写操作*/
		uint8_t f111_data[DID_F111_LENGTH];					/*len = 2，在供应商会话模式和安全模式下进行写操作*/
		uint8_t f18B_data[DID_F18B_LENGTH];					/*len = 3，在供应商会话模式和安全模式下进行写操作*/
		uint8_t f18C_data[DID_F18C_LENGTH];				    /*len = 19，在供应商会话模式和安全模式下进行写操作*/
		uint8_t f190_data[DID_F190_LENGTH];					/*len = 17只在EOL模式下写一次*/
		uint8_t f193_data[DID_F193_LENGTH];					/*len = 5，在供应商会话模式和安全模式下进行写操作*/

		uint8_t _4a1F_data[DID_4A1F_LENGTH];				/*len = 3，扩展会话模式下写*/
		uint8_t f190_write_flag;							/*eol模式下写一次，写完后标志位写0x55*/
		uint8_t F130_data[DID_F130_LENGTH];					/*29D6写，len = 11*/
		uint8_t F131_data[DID_F131_LENGTH];					/*29D6写,len = 2*/
		uint8_t F151_data[DID_F151_LENGTH];					/*29D6写,len = 6*/
		uint8_t _4a00_data[DID_4A00_LENGTH];				/*len = 16*/
		uint8_t _4a01_data[DID_4A01_LENGTH];				/*len = 16*/
		uint8_t _4a04_data[DID_4A04_LENGTH];				/*len = 2*/
		uint8_t _4a05_data[DID_4A05_LENGTH];				/*len = 2*/
		uint8_t _4a06_data[DID_4A06_LENGTH];				/*len = 2*/
		uint8_t _4aFF_data[DID_4AFF_LENGTH];
		uint8_t res[11];
	}sig;
}uds_write_flash_data_un;

#define DID_HW_VER_LENGTH (11u) /* Vehicle Manufacture Hardware Part and Version Number. ($F1 $10) */
#define DID_HW_BASELINE_LENGTH (2u) /* Vehicle Manufacture Hardware Baseline. ($F1 $11) */
#define DID_FBL_REPROG_ATT_CNT_LENGTH (4u) /* Reprogramming attempt counter. ($F0 $10) */
#define DID_FINGER_LENGTH (12u) /* Fingerprint Data.  ($F1 $5B)  ($F1 $5A) */
#define DID_DIG_MODULE_LENGTH (1u) /* Current diagnostic mode    ($F1 $86) */
#define DID_BOOT_SW_VER_LENGTH (6u) /* Boot Software Version.     ($F1 $50) */
#define DID_PART_NUMBER_LENGTH (8) /* part number    ($4A $01) */
#define FAA_FLAG_LENGTH (2)
/* 16字节对齐，32-24=8*/
#define ALIGNMENT_RESERVED_BYTE (8)
/* 2+12+4+6 = 24字节  */
#define SVRW1_DID_BYTES_SIZE (FAA_FLAG_LENGTH \
+ DID_FINGER_LENGTH \
+ DID_FBL_REPROG_ATT_CNT_LENGTH \
+ DID_BOOT_SW_VER_LENGTH \
+ ALIGNMENT_RESERVED_BYTE)
/* 类型定义 */
typedef union tagSVRW1_DIDS {
    uint8_t u8_data[SVRW1_DID_BYTES_SIZE];
    struct {
    	uint8_t FAA_Fag_27Service_Boot;
    	uint8_t FAA_Fag_27Service_App;
        uint8_t DID_F15A_B[DID_FINGER_LENGTH];
        uint8_t DID_F010[DID_FBL_REPROG_ATT_CNT_LENGTH];
        uint8_t DID_F150[DID_BOOT_SW_VER_LENGTH];
    }sig;
} SVRW1_DIDS;


void did_F110_set_value(uint8_t*i_pDataBufPtr);
void did_F111_set_value(uint8_t*i_pDataBufPtr);
void did_F118_set_value(uint8_t*i_pDataBufPtr);
void did_F119_set_value(uint8_t*i_pDataBufPtr);
void did_F010_set_value(uint8_t*i_pDataBufPtr);
void did_F15B_set_value(uint8_t*i_pDataBufPtr);
void did_F18A_set_value(uint8_t*i_pDataBufPtr);
void did_F18B_set_value(uint8_t*i_pDataBufPtr);
void did_F19E_set_value(uint8_t*i_pDataBufPtr);
void did_F150_set_value(uint8_t*i_pDataBufPtr);
void did_F18C_set_value(uint8_t*i_pDataBufPtr);
void did_F193_set_value(uint8_t*i_pDataBufPtr);
void did_F195_set_value(uint8_t*i_pDataBufPtr);
void did_F190_set_value(uint8_t*i_pDataBufPtr);

void did_F130_set_value(uint8_t*i_pDataBufPtr);
void did_F131_set_value(uint8_t*i_pDataBufPtr);
void did_F151_set_value(uint8_t*i_pDataBufPtr);

void did_4A00_set_value(uint8_t*i_pDataBufPtr);
void did_4A01_set_value(uint8_t*i_pDataBufPtr);
void did_4A02_set_value(uint8_t*i_pDataBufPtr);

void did_4A03_set_value(uint8_t*i_pDataBufPtr);
void did_4A04_set_value(uint8_t*i_pDataBufPtr);
void did_4A05_set_value(uint8_t*i_pDataBufPtr);
void did_4A06_set_value(uint8_t*i_pDataBufPtr);

void did_4A08_set_value(uint8_t*i_pDataBufPtr);
void did_4A09_set_value(uint8_t*i_pDataBufPtr);
void did_4A43_set_value(uint8_t*i_pDataBufPtr);
void did_4A33_set_value(uint8_t*i_pDataBufPtr);
void did_4A23_set_value(uint8_t*i_pDataBufPtr);
void did_4A13_set_value(uint8_t*i_pDataBufPtr);
void did_4A50_set_value(uint8_t*i_pDataBufPtr);
void did_4A51_set_value(uint8_t*i_pDataBufPtr);
void did_4A52_set_value(uint8_t*i_pDataBufPtr);
void did_4A53_set_value(uint8_t*i_pDataBufPtr);
void did_4A54_set_value(uint8_t*i_pDataBufPtr);
void did_4A55_set_value(uint8_t*i_pDataBufPtr);
void did_4A1F_set_value(uint8_t*i_pDataBufPtr);
void did_4AFF_set_value(uint8_t*i_pDataBufPtr);

void E2Para_deinit(void);
#endif
