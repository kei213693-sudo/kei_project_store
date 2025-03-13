/*
 * hal_e2e_test.c
 *
 *  Created on: 2024年7月1日
 *      Author: F704011556_
 */

#include "hal_e2e_test.h"
#include "hal_e2e_profile1.h"
#include "hal_e2e_profile5.h"

void hal_e2e_test(void)
{
#if 0
	uint32_t i = 0;
	Std_ReturnType std_re = 0;
	uint8_t buffer[128] = {0};
	uint8_t check_array[10] = {0};
	uint32_t bufferlength = 0;
	uint8_t inputbuffer[64] = {0};
	//uint32_t inputbufferlength = 64-3;
	uint32_t inputbufferlength = 8-1;
	E2EXf_OptionsType e2exf_options = {0};
	E2EXf_ConfigType e2exf_config = {0};
	E2E_P01ConfigType e2e_p01_config = {0};
	E2E_P01ProtectStateType e2e_p01_protect0 = {0};
	E2E_P01ProtectStateType e2e_p01_protect1 = {0};
	E2E_P01CheckStateType e2e_p01_check = {0};
	E2E_SMConfigType e2e_sm_config = {0};
	E2E_SMCheckStateType e2e_sm_check = {0};
	E2E_P05ConfigType e2e_p05_config = {0};
	E2E_P05ProtectStateType e2e_p05_protect = {0};
	E2E_P05CheckStateType e2e_p05_check={0};
//初始化
	for(i = 0; i < 64; ++i)
	{
		inputbuffer[i] = i+1;
	}
	E2EXf_InitMemory();
	E2EXf_Init(&e2exf_config);

	E2E_P01ProtectInit(&e2e_p01_protect0);
	E2E_P01ProtectInit(&e2e_p01_protect1);
	E2E_P01CheckInit(&e2e_p01_check);
	E2E_SMCheckInit(&e2e_sm_check,&e2e_sm_config);
	E2E_P05ProtectInit(&e2e_p05_protect);
//测试
//	E2E_P01Protect(&e2e_config,&e2e_p01_protect,inputbuffer);
	e2exf_options.disableEndToEndCheck = FALSE;								//解析时使用
	e2exf_options.executeDespiteDataUnavailability = FALSE;					//E2EXf_Check_Parameters函数调用
	e2exf_options.headerLength = 12; 										//header的长度
	//e2exf_options.headerLength = 24; 										//header的长度
	e2exf_options.inPlace = FALSE;
	e2exf_options.profileBehavior = TRUE;
	e2exf_options.upperHeaderBitsToShift = 0;								//数据需要插入的位置

	e2e_p01_config.CounterOffset = 8;
	e2e_p01_config.CRCOffset = 0;
	e2e_p01_config.DataID = 0x01;
	e2e_p01_config.DataIDNibbleOffset = 0;
	e2e_p01_config.DataIDMode = E2E_P01_DATAID_LOW;
	e2e_p01_config.DataLength = 64;											//总长度
	e2e_p01_config.MaxDeltaCounterInit = 1;
	e2e_p01_config.MaxNoNewOrRepeatedData = 1;
	e2e_p01_config.SyncCounterInit = 1;

	e2e_p05_config.DataID = 0x01;
	e2e_p05_config.DataLength = 512;
	e2e_p05_config.MaxDeltaCounter = 1;
	e2e_p05_config.Offset = 0;

	e2e_p01_check.MaxDeltaCounter = 1;
	e2e_p01_check.WaitForFirstData = TRUE;
	e2e_p01_check.NoNewOrRepeatedDataCounter = 1;

	e2e_sm_config.WindowSize = 10;
	e2e_sm_config.MinOkStateInit = 3;
	e2e_sm_config.MaxErrorStateInit = 3;
	e2e_sm_config.MinOkStateValid = 3;
	e2e_sm_config.MaxErrorStateValid = 3;
	e2e_sm_config.MinOkStateInvalid = 3;
	e2e_sm_config.MaxErrorStateInvalid = 3;

	e2e_sm_check.ProfileStatusWindow = check_array;
	while(1)
	{
		std_re = E2EXf_P01(buffer,&bufferlength,inputbuffer,inputbufferlength,&e2exf_options,&e2e_p01_config,&e2e_p01_protect0);
		std_re = E2EXf_P01_Inv(inputbuffer, &inputbufferlength, buffer, bufferlength, &e2exf_options, &e2e_p01_config, &e2e_p01_check, &e2e_sm_config, &e2e_sm_check);

		std_re = E2EXf_P01(buffer,&bufferlength,inputbuffer,inputbufferlength,&e2exf_options,&e2e_p01_config,&e2e_p01_protect0);
		std_re = E2EXf_P01_Inv(inputbuffer, &inputbufferlength, buffer, bufferlength, &e2exf_options, &e2e_p01_config, &e2e_p01_check, &e2e_sm_config, &e2e_sm_check);

		std_re = E2EXf_P01(buffer,&bufferlength,inputbuffer,inputbufferlength,&e2exf_options,&e2e_p01_config,&e2e_p01_protect0);
		std_re = E2EXf_P01_Inv(inputbuffer, &inputbufferlength, buffer, bufferlength, &e2exf_options, &e2e_p01_config, &e2e_p01_check, &e2e_sm_config, &e2e_sm_check);

		std_re = E2EXf_P01(buffer,&bufferlength,inputbuffer,inputbufferlength,&e2exf_options,&e2e_p01_config,&e2e_p01_protect0);
		std_re = E2EXf_P01(buffer,&bufferlength,inputbuffer,inputbufferlength,&e2exf_options,&e2e_p01_config,&e2e_p01_protect0);
		std_re = E2EXf_P01(buffer,&bufferlength,inputbuffer,inputbufferlength,&e2exf_options,&e2e_p01_config,&e2e_p01_protect0);
		std_re = E2EXf_P01(buffer,&bufferlength,inputbuffer,inputbufferlength,&e2exf_options,&e2e_p01_config,&e2e_p01_protect0);
		std_re = E2EXf_P01(buffer,&bufferlength,inputbuffer,inputbufferlength,&e2exf_options,&e2e_p01_config,&e2e_p01_protect0);

		std_re = E2EXf_P05(buffer, &bufferlength, inputbuffer, inputbufferlength, &e2exf_options, &e2e_p05_config, &e2e_p05_protect);
		std_re = E2EXf_P05_Inv(inputbuffer, &inputbufferlength, buffer, bufferlength, &e2exf_options, &e2e_p05_config, &e2e_p05_check, &e2e_sm_config, &e2e_sm_check);
	}
#endif

	hal_e2e_p01_test();
	hal_e2e_p05_test();
}
#define HAL_E2E_P01_LEN  8
#define HAL_E2E_P05_LEN	 64
void hal_e2e_p01_test(void)
{

	uint8_t re = 0;
	uint8_t i = 0;
	uint8_t test_data[HAL_E2E_P01_LEN] = {0};
	uint8_t test_data2[HAL_E2E_P01_LEN] = {0};
	hal_e2e_profile1_init();
	for(i = 0; i < HAL_E2E_P01_LEN; ++i)
	{
		test_data[i] = i;
		test_data2[i] = i;
	}
	while(1)
	{
		re = hal_e2e_profile1(0, test_data, HAL_E2E_P01_LEN);
		re = hal_e2e_profile1(1, test_data2, HAL_E2E_P01_LEN);

		re = hal_e2e_profile1_inv(0, test_data, HAL_E2E_P01_LEN);
		re = hal_e2e_profile1_inv(1, test_data2, HAL_E2E_P01_LEN);
	}
}
void hal_e2e_p05_test(void)
{
	uint8_t re = 0;
	uint8_t i = 0;
	uint8_t test_data[HAL_E2E_P05_LEN] = {0};
	uint8_t test_data2[HAL_E2E_P05_LEN] = {0};
	hal_e2e_profile5_init();
	for(i = 0; i < HAL_E2E_P05_LEN; ++i)
	{
		test_data[i] = i;
		test_data2[i] = i;
	}
	while(1)
	{
		re = hal_e2e_profile5(0, test_data, HAL_E2E_P05_LEN);
		re = hal_e2e_profile5(1, test_data2, HAL_E2E_P05_LEN);
		re = hal_e2e_profile5_inv(0, test_data, HAL_E2E_P05_LEN);
		re = hal_e2e_profile5_inv(1, test_data2, HAL_E2E_P05_LEN);
	}
}
