#include "hal_e2e_profile1.h"

#define E2E_SM_WINDOWSIZE	6

typedef struct
{
	E2EXf_OptionsType *e2exf_option;
	E2E_P01ConfigType *e2e_p01_config;
	E2E_P01ProtectStateType * e2e_p01_protect;
}e2e_p01_protect_st;

typedef struct
{
	E2EXf_OptionsType *e2exf_option;
	E2E_P01ConfigType *e2e_p01_config;
	E2E_P01CheckStateType *e2e_p01_check;
	E2E_SMConfigType *e2e_sm_config;
	E2E_SMCheckStateType *e2e_sm_check;
}e2e_p01_check_st;


//配置相关文件
/*****************protect********************/
//E2EXF
static E2EXf_OptionsType e2exf_protect_option[1] =
{
	{
		.disableEndToEndCheck = FALSE,
		.executeDespiteDataUnavailability = FALSE,
		.headerLength = 12,
		.inPlace = TRUE,
		.profileBehavior = TRUE,
		.upperHeaderBitsToShift = 0
	}
};
//P01
static E2E_P01ConfigType e2e_p01_protect_config[P01_PROTECTID_NUM] =
{
	{
		.CounterOffset = 8,
		.CRCOffset = 0,
		.DataID = 0x01,
		.DataIDNibbleOffset = 0,
		.DataIDMode = E2E_P01_DATAID_LOW,
		.DataLength = 64,
		.MaxDeltaCounterInit = 1,
		.MaxNoNewOrRepeatedData = 1,
		.SyncCounterInit = 1
	},
	{
		.CounterOffset = 8,
		.CRCOffset = 0,
		.DataID = 0x02,
		.DataIDNibbleOffset = 0,
		.DataIDMode = E2E_P01_DATAID_LOW,
		.DataLength = 64,
		.MaxDeltaCounterInit = 1,
		.MaxNoNewOrRepeatedData = 1,
		.SyncCounterInit = 1
	}
};
static E2E_P01ProtectStateType e2e_p01_protect_protect[P01_PROTECTID_NUM] =
{
	{
		.Counter = 0
	},
	{
		.Counter = 0
	}
};

/*****************check********************/
//E2EXF
static E2EXf_OptionsType e2exf_check_option[1] =
{
	{
		.disableEndToEndCheck = FALSE,
		.executeDespiteDataUnavailability = FALSE,
		.headerLength = 12,
		.inPlace = FALSE,
		.profileBehavior = TRUE,
		.upperHeaderBitsToShift = 0
	}
};
//P01
static E2E_P01ConfigType e2e_p01_check_config[P01_CHECKID_NUM] =
{
	{
		.CounterOffset = 8,
		.CRCOffset = 0,
		.DataID = 0x01,
		.DataIDNibbleOffset = 0,
		.DataIDMode = E2E_P01_DATAID_LOW,
		.DataLength = 64,
		.MaxDeltaCounterInit = 1,
		.MaxNoNewOrRepeatedData = 1,
		.SyncCounterInit = 1
	},
	{
		.CounterOffset = 8,
		.CRCOffset = 0,
		.DataID = 0x02,
		.DataIDNibbleOffset = 0,
		.DataIDMode = E2E_P01_DATAID_LOW,
		.DataLength = 64,
		.MaxDeltaCounterInit = 1,
		.MaxNoNewOrRepeatedData = 1,
		.SyncCounterInit = 1
	}
};
static E2E_P01CheckStateType e2e_p01_check_check[P01_CHECKID_NUM] =
{
	{
		.LastValidCounter = 0,
		.MaxDeltaCounter = 0,
		.WaitForFirstData = TRUE,
		.NewDataAvailable = FALSE,
		.LostData = 0,
		.Status = E2E_P01STATUS_NONEWDATA,
		.NoNewOrRepeatedDataCounter = 0,
		.SyncCounter = 0
	},
	{
		.LastValidCounter = 0,
		.MaxDeltaCounter = 0,
		.WaitForFirstData = TRUE,
		.NewDataAvailable = FALSE,
		.LostData = 0,
		.Status = E2E_P01STATUS_NONEWDATA,
		.NoNewOrRepeatedDataCounter = 0,
		.SyncCounter = 0
	}
};
//SM
static E2E_SMConfigType e2e_sm_check_config[1] =
{
	{
		.WindowSize = E2E_SM_WINDOWSIZE,
		.MinOkStateInit = 3,
		.MaxErrorStateInit = 3,
		.MinOkStateValid = 3,
		.MaxErrorStateValid = 3,
		.MinOkStateInvalid = 3,
		.MaxErrorStateInvalid = 3
	}
};
static E2E_SMCheckStateType e2e_sm_check_check[P01_CHECKID_NUM] =
{
	{
		.ProfileStatusWindow = 0,
		.WindowTopIndex = 0,
		.OkCount = 0,
		.ErrorCount = 0,
		.SMState = E2E_SM_NODATA
	},
	{
		.ProfileStatusWindow = 0,
		.WindowTopIndex = 0,
		.OkCount = 0,
		.ErrorCount = 0,
		.SMState = E2E_SM_NODATA
	}
};

static uint8_t e2e_sm_window[P01_CHECKID_NUM][E2E_SM_WINDOWSIZE] = {0};

static e2e_p01_protect_st e2e_p01_protect_cfg[P01_PROTECTID_NUM];
static e2e_p01_check_st e2e_p01_check_cfg[P01_CHECKID_NUM];

void hal_e2e_profile1_init(void)
{

	uint8_t x = 0;
	uint8_t y = 0;
	E2EXf_ConfigType e2exf_config = {0};
	E2EXf_InitMemory();
	E2EXf_Init(&e2exf_config);
	//protect参数初始化
	for(x = 0; x < P01_PROTECTID_NUM; ++x)
	{
		e2e_p01_protect_protect[x].Counter = 0;
		e2e_p01_protect_cfg[x].e2exf_option = &e2exf_protect_option[0];
		e2e_p01_protect_cfg[x].e2e_p01_protect = &e2e_p01_protect_protect[x];
		e2e_p01_protect_cfg[x].e2e_p01_config = &e2e_p01_protect_config[x];
	}
	//check
	for(x = 0; x < P01_CHECKID_NUM; ++x)
	{
		for(y = 0; y < E2E_SM_WINDOWSIZE; ++y)
		{
			e2e_sm_window[x][y] = 0;
		}
		e2e_sm_check_check[x].ProfileStatusWindow = e2e_sm_window[x];
		e2e_sm_check_check[x].ErrorCount = 0;
		e2e_sm_check_check[x].OkCount = 0;
		e2e_sm_check_check[x].SMState = E2E_SM_NODATA;
		e2e_sm_check_check[x].WindowTopIndex = 0;

		e2e_p01_check_check[x].LastValidCounter = 0;
		e2e_p01_check_check[x].MaxDeltaCounter = 0;
		e2e_p01_check_check[x].WaitForFirstData = TRUE;
		e2e_p01_check_check[x].NewDataAvailable = FALSE;
		e2e_p01_check_check[x].LostData = 0;
		e2e_p01_check_check[x].Status = E2E_P01STATUS_NONEWDATA;
		e2e_p01_check_check[x].NoNewOrRepeatedDataCounter = 0;
		e2e_p01_check_check[x].SyncCounter = 0;

		e2e_p01_check_cfg[x].e2exf_option = &e2exf_check_option[0];
		e2e_p01_check_cfg[x].e2e_p01_check = &e2e_p01_check_check[x];
		e2e_p01_check_cfg[x].e2e_sm_check = &e2e_sm_check_check[x];
		e2e_p01_check_cfg[x].e2e_sm_config = &e2e_sm_check_config[0];
		e2e_p01_check_cfg[x].e2e_p01_config = &e2e_p01_check_config[x];
	}
}

uint8_t hal_e2e_profile1(p01_protectid_en id_index,uint8_t *pdata,uint32_t len)
{
	uint8_t re = 0;
	uint8_t buffer[30] = {0};
	uint32_t bufferlenth = 0;
	uint32_t data_len = 0;
	if((id_index < P01_PROTECTID_NUM)&&(len <= 30))
	{
//		data_len = len - e2e_p01_protect_cfg[id_index].e2exf_option->headerLength/8;
		data_len = len;
		re = E2EXf_P01(pdata, &bufferlenth, pdata, data_len, e2e_p01_protect_cfg[id_index].e2exf_option, e2e_p01_protect_cfg[id_index].e2e_p01_config, e2e_p01_protect_cfg[id_index].e2e_p01_protect);
	}
	else
	{
		re = 0xff;
	}

	return re;
}

uint8_t hal_e2e_profile1_inv(p01_checkid_en id_index,uint8_t *pdata,uint32_t len)
{
	uint8_t re = 0;
	uint8_t buffer[30] = {0};
	uint32_t bufferlenth = 0;
	if((id_index < P01_CHECKID_NUM)&&(len <= 30))
	{
		re = E2EXf_P01_Inv(buffer, &bufferlenth, pdata, len, e2e_p01_check_cfg[id_index].e2exf_option, e2e_p01_check_cfg[id_index].e2e_p01_config, e2e_p01_check_cfg[id_index].e2e_p01_check, e2e_p01_check_cfg[id_index].e2e_sm_config, e2e_p01_check_cfg[id_index].e2e_sm_check);
	}
	else
	{
		re = 0xff;
	}
	return re;
}


