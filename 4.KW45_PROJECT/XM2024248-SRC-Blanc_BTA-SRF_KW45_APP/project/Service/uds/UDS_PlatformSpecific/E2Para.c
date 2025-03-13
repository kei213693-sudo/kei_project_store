/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: E2Para.c
 * 摘要:
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */



#include "E2Para.h"
#include "string.h"
#include "uds_api.h"
uds_write_flash_data_un uds_read_data_un;
static SVRW1_DIDS boot_did_un;
/*读取指纹*/
uint8_t DID_F15B[DID_F15B_LENGTH]= {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
/*写入指纹*/
uint8_t DID_F15A[DID_F15A_LENGTH];
/*刷写次数*/
uint8_t DID_F010[DID_F010_LENGTH];
/*车辆识别号*/
uint8_t DID_F190[DID_F190_LENGTH];
/*硬件号*/
uint8_t DID_F110[DID_F110_LENGTH] = {'P','0','3','8','1','2','0','4',' ','A','C'};
/*硬件基线号*/
const uint8_t DID_F111[DID_F111_LENGTH] = {0x20,0x20};
/*软件号*/
const uint8_t DID_F118[DID_F118_LENGTH] = {'P','0','3','8','9','6','9','0',' ','A','E'};
/*软件基线号*/
const uint8_t DID_F119[DID_F119_LENGTH] = {0x20,0x20};
/*供应商代码*/
const uint8_t DID_F18A[DID_F18A_LENGTH] = {'V','6','8','2','4','8'};
/*生产日期*/
const uint8_t DID_F18B[DID_F18B_LENGTH] = {0x24,0x5,0x16};
/*诊断数据库版本*/
const uint8_t DID_F19E[DID_F19E_LENGTH] = {0x03,0x10,0x01};
/*bootloader 版本号*/
const uint8_t DID_F150[DID_F150_LENGTH] = {'V','0','.','1'};
/*序列号*/
const uint8_t DID_F18C[DID_F18C_LENGTH] = {'V','0','1'};
/*供应商硬件号*/
const uint8_t DID_F193[DID_F193_LENGTH] = {'V','0','1'};
/*供应商软件号*/
const uint8_t DID_F195[DID_F195_LENGTH] = {'V','1','.','0','.','0'};

const uint8_t DID_4A02[DID_4A02_LENGTH] = {0};
const uint8_t DID_4A08[DID_4A08_LENGTH] = {0};
const uint8_t DID_4A09[DID_4A09_LENGTH] = {0};

/*仅有SRF支持*/
const uint8_t DID_4A0B[DID_4A0B_LENGTH] = {0};

uint8_t canlog_Eflag = 0;

/*供应商内部ECU零件号*/
#ifdef UdsApi_BTA_R_FUN_EN
/*供应商APP软件测试版本*/
const uint8_t DID_4A50[DID_4A50_LENGTH] = {'V','1','.','0','.','0','_','B','0','1'};
uint8_t DID_4A51[DID_4A51_LENGTH] = {'P', '0', '3', '8', '0','5','0','3'};
/*福尔达总成料号*/
const uint8_t DID_4A52[DID_4A52_LENGTH] = {'F','1','0','0','4','5','1','2','4','2'};
#endif

#ifdef UdsApi_BTA_L_FUN_EN
/*供应商APP软件测试版本*/
const uint8_t DID_4A50[DID_4A50_LENGTH] = {'V','1','.','0','.','0','_','B','0','1'};
uint8_t DID_4A51[DID_4A51_LENGTH] = {'P', '0', '3', '8', '0','5','0','4'};
/*福尔达总成料号*/
const uint8_t DID_4A52[DID_4A52_LENGTH] = {'F','1','0','0','4','5','1','2','4','3'};
#endif

#ifdef UdsApi_BTA_RB2_FUN_EN
/*供应商APP软件测试版本*/
const uint8_t DID_4A50[DID_4A50_LENGTH] = {'V','1','.','0','.','0','_','B','0','1'};
uint8_t DID_4A51[DID_4A51_LENGTH] = {'P', '0', '3', '8', '2','8','6','8'};
/*福尔达总成料号*/
const uint8_t DID_4A52[DID_4A52_LENGTH] = {'F','1','0','0','4','5','1','2','4','7'};
#endif

#ifdef UdsApi_BTA_SRR_FUN_EN
/*供应商APP软件测试版本*/
const uint8_t DID_4A50[DID_4A50_LENGTH] = {'V','1','.','0','.','0','_','B','0','1'};
uint8_t DID_4A51[DID_4A51_LENGTH] = {'P', '0', '3', '8', '1','2','0','3'};
/*福尔达总成料号*/
const uint8_t DID_4A52[DID_4A52_LENGTH] = {'F','1','0','0','4','5','1','2','4','5'};
/*供应商APP2软件测试版本*/
const uint8_t DID_4A53[DID_4A53_LENGTH] = {'V','1','.','0','.','0','B','_','0','1'};
/*供应商APP2软件发布版本*/
const uint8_t DID_4A54[DID_4A54_LENGTH] = {'V','1','.','0','.','0'};
/*供应商BOOTLOADER2软件版本*/
const uint8_t DID_4A55[DID_4A55_LENGTH] = {'V','1','.','0','.','0'};
#endif

#ifdef UdsApi_BTA_SRF_FUN_EN
/*供应商APP软件测试版本*/
const uint8_t DID_4A50[DID_4A50_LENGTH] = {'V','2','.','0','.','0','_','B','0','5'};
uint8_t DID_4A51[DID_4A51_LENGTH] = {'P', '0', '3', '8', '1','2','0','4'};
/*福尔达总成料号*/
const uint8_t DID_4A52[DID_4A52_LENGTH] = {'F','1','0','0','4','5','1','2','4','6'};
/*供应商APP2软件测试版本*/

const uint8_t DID_4A53[DID_4A53_LENGTH] = {'V','1','.','0','.','0','_','B','0','1'};
/*供应商APP2软件发布版本*/
const uint8_t DID_4A54[DID_4A54_LENGTH] = {'V','1','.','0','.','0'};
/*供应商BOOTLOADER2软件版本*/
const uint8_t DID_4A55[DID_4A55_LENGTH] = {'V','1','.','0','.','0'};
#endif
#if ((UdsApi_BTA_SRF_FUN_EN == 1) || (UdsApi_BTA_SRR_FUN_EN == 1))
/*软件标定号*/
const uint8_t DID_F130[DID_F130_LENGTH] = {'P','0','3','9','5','5','0','0',' ','A','E'};
/*软件标定基线号*/
const uint8_t DID_F131[DID_F131_LENGTH] = {0x20,0x20};
/*29D6版本号*/
const uint8_t DID_F151[DID_F151_LENGTH] = {'V','0','.','0','.','1'};
#endif
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
/*供应商APP软件测试版本*/
const uint8_t DID_4A50[DID_4A50_LENGTH] = {'V','1','.','0','.','0','_','B','0','1'};
uint8_t DID_4A51[8] = {'P', '0', '3', '8', '0','5','0','6'};
/*福尔达总成料号*/
const uint8_t DID_4A52[DID_4A52_LENGTH] = {'F','1','0','0','4','5','1','2','4','4'};
#endif

#if ((UdsApi_BTA_SRF_FUN_EN == 1) || (UdsApi_BTA_SRR_FUN_EN == 1))
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F130_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig.F130_data,DID_F130_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F131_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig.F131_data,DID_F131_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F151_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig.F151_data,DID_F151_LENGTH);
}
#endif
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F110_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig.f110_data,DID_F110_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F111_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig.f111_data,DID_F111_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F118_set_value(uint8_t*i_pDataBufPtr)
{
     memcpy(i_pDataBufPtr,DID_F118,DID_F118_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F119_set_value(uint8_t*i_pDataBufPtr)
{
     memcpy(i_pDataBufPtr,DID_F119,DID_F119_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F010_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_BOOT1,boot_did_un.u8_data,SVRW1_DID_BYTES_SIZE);
	memcpy(i_pDataBufPtr,boot_did_un.sig.DID_F010,DID_FBL_REPROG_ATT_CNT_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F15B_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_BOOT1,boot_did_un.u8_data,SVRW1_DID_BYTES_SIZE);
	memcpy(i_pDataBufPtr,boot_did_un.sig.DID_F15A_B,DID_FINGER_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F18A_set_value(uint8_t*i_pDataBufPtr)
{
     memcpy(i_pDataBufPtr,DID_F18A,DID_F18A_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F18B_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig.f18B_data,DID_F18B_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F19E_set_value(uint8_t*i_pDataBufPtr)
{
     memcpy(i_pDataBufPtr,DID_F19E,DID_F19E_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F150_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_BOOT1,boot_did_un.u8_data,SVRW1_DID_BYTES_SIZE);
	memcpy(i_pDataBufPtr,boot_did_un.sig.DID_F150,DID_BOOT_SW_VER_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F18C_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig.f18C_data,DID_F18C_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F193_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig.f193_data,DID_F193_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F195_set_value(uint8_t*i_pDataBufPtr)
{
     memcpy(i_pDataBufPtr,DID_F195,DID_F195_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_F190_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
    memcpy(i_pDataBufPtr,uds_read_data_un.sig.f190_data,DID_F190_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A00_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig._4a00_data,DID_4A00_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A01_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig._4a01_data,DID_4A01_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A02_set_value(uint8_t*i_pDataBufPtr)
{
     memcpy(i_pDataBufPtr,DID_4A02,DID_4A02_LENGTH);
}


/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A03_set_value(uint8_t*i_pDataBufPtr)
{
	uint8_t batt_temp;
	batt_temp = UdsApi_getBattInfo();
	*i_pDataBufPtr = batt_temp;
}

/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A04_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig._4a04_data,DID_4A04_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A05_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig._4a05_data,DID_4A05_LENGTH);
}


/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A06_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig._4a06_data,DID_4A06_LENGTH);
}

/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A08_set_value(uint8_t*i_pDataBufPtr)
{
	 memcpy(i_pDataBufPtr,DID_4A08,DID_4A08_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A09_set_value(uint8_t*i_pDataBufPtr)
{
	 memcpy(i_pDataBufPtr,DID_4A09,DID_4A09_LENGTH);
}

/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A43_set_value(uint8_t*i_pDataBufPtr)
{
	uint8_t batt_temp;
	batt_temp = UdsApi_getBattInfo();
	*i_pDataBufPtr = batt_temp;
}


/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A33_set_value(uint8_t*i_pDataBufPtr)
{
	uint8_t batt_temp;
	batt_temp = UdsApi_getBattInfo();
	*i_pDataBufPtr = batt_temp;
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A23_set_value(uint8_t*i_pDataBufPtr)
{
	uint8_t batt_temp;
	batt_temp = UdsApi_getBattInfo();
	*i_pDataBufPtr = batt_temp;
}

/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A13_set_value(uint8_t*i_pDataBufPtr)
{
	uint8_t batt_temp;
	batt_temp = UdsApi_getBattInfo();
	*i_pDataBufPtr = batt_temp;
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A50_set_value(uint8_t*i_pDataBufPtr)
{
	memcpy(i_pDataBufPtr,DID_4A50,DID_4A50_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A51_set_value(uint8_t*i_pDataBufPtr)
{
	memcpy(i_pDataBufPtr,DID_4A51,DID_4A51_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A52_set_value(uint8_t*i_pDataBufPtr)
{
	memcpy(i_pDataBufPtr,DID_4A52,DID_4A52_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A53_set_value(uint8_t*i_pDataBufPtr)
{
#if ((1 == UdsApi_BTA_SRR_FUN_EN) || (1 == UdsApi_BTA_SRF_FUN_EN))
	memcpy(i_pDataBufPtr,DID_4A53,DID_4A53_LENGTH);
#endif
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A54_set_value(uint8_t*i_pDataBufPtr)
{
#if ((1 == UdsApi_BTA_SRR_FUN_EN) || (1 == UdsApi_BTA_SRF_FUN_EN))
	memcpy(i_pDataBufPtr,DID_4A54,DID_4A54_LENGTH);
#endif
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A55_set_value(uint8_t*i_pDataBufPtr)
{
#if ((1 == UdsApi_BTA_SRR_FUN_EN) || (1 == UdsApi_BTA_SRF_FUN_EN))
	memcpy(i_pDataBufPtr,DID_4A55,DID_4A55_LENGTH);
#endif
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A0B_set_value(uint8_t*i_pDataBufPtr)
{
	memcpy(i_pDataBufPtr,DID_4A0B,DID_4A0B_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A1F_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig._4a1F_data,DID_4A1F_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A0A_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig._4a0a_data,DID_4A0A_LENGTH);
}/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void did_4A57_set_value(uint8_t*i_pDataBufPtr)
{
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	memcpy(i_pDataBufPtr,uds_read_data_un.sig._4a57_data,DID_4A57_LENGTH);
}
/*
 * 函数介绍: 给DID拷贝数据
 * 参数:
 * 返回值: {*}
 * 备注:
 */
void E2Para_deinit(void)
{
	uint16_t i;
	uint8_t num = 0;
	Uds_Api_fdl_FlashRead(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	for(i = 0 ; i < DID_WRITE_DATA_LEN ; i++)
	{
		if(0xFF == uds_read_data_un.write_buffer[i] )
		{
			num++;
		}
		else
		{}
	}
	if(num == DID_WRITE_DATA_LEN)
	{
		memcpy(uds_read_data_un.sig.f110_data,DID_F110,DID_F110_LENGTH);
		memcpy(uds_read_data_un.sig.f111_data,DID_F111,DID_F111_LENGTH);
		Uds_Api_fdl_Ereas_Write(UDSAPI_FLASH_INDEX_DID,uds_read_data_un.write_buffer,DID_WRITE_DATA_LEN);
	}
#if 0
	for(i = 0; i < DID_4A09_LENGTH; i++)
	{
		DID_4A09[i] = i;
	}
#endif
}
