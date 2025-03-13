/*
 * bleTimer_cfg.h
 *
 *  Created on: 2024年7月12日
 *      Author: F704012276_
 */

#ifndef DRV_BLE_BLETIMER_BLETIMER_CFG_H_
#define DRV_BLE_BLETIMER_BLETIMER_CFG_H_

#include "signals_api.h"

typedef enum
{
	T_Start_Pair = 0,
	T_Start_Pair_Complete,
	T_Pending_for_Adv,
	T_SP_Auth_Tmot,
	T_SKF_Auth_Tmot,
	T_Pending_for_Adv_Scan,
	T_Auth_Valid,
	T_BLEImmo,
	T_PhoneKey_SupervisionTO,
	T_KeyFob_SupervisionTO,
	T_AuthFail_RestartAdv,
	T_NioAdv_App,
	T_NioAdv_Ibeacon,
/*	T_BleCanTpFC_Timeout,
	T_BleCanTpSTMin_Timeout,*/
	T_BleCanTp_Timeout,
	bleAppTimerCnt,
} bleAppTimerEnum_t;

typedef struct
{
	union{
		uint16_t	data;
		struct{
			uint8_t	startPair							:1;
			uint8_t	pairComplete						:1;
			uint8_t	pendingAdv							:1;
			uint8_t	spAuthTmot							:1;

			uint8_t skfAuthTmot							:1;
			uint8_t pendingAdvScan						:1;
			uint8_t authVaild							:1;
			uint8_t bleImmo								:1;

			uint8_t phoneKeySupervisionTO				:1;
			uint8_t keyFobSupervisionTO					:1;
			uint8_t	restartAdv							:1;
			uint8_t startAppAdv							:1;

			uint8_t startIBeaconAdv						:1;
			uint8_t BleCanTpFC							:1;/*
			uint8_t BleCanTpSTMin						:1;*/
			uint8_t reserved							:2;
		}bits;
	}timerStartflag;

	union{
		uint32_t data[16];
		struct{
			uint32_t startPairTimeout;
			uint32_t pairCompleteTimeout;
			uint32_t pendingAdvTimeout;
			uint32_t spAuthTmotTimeout;

			uint32_t skfAuthTmotTimeout;
			uint32_t pendingAdvScanTimeout;
			uint32_t authVaildTimeout;
			uint32_t bleImmoTimeout;

			uint32_t phoneKeySupervisionTOTimeout;
			uint32_t keyFobSupervisionTOTimeout;
			uint32_t AuthFailRestartAdvTimeout;
			uint32_t startAppAdvTimeout;

			uint32_t startIBeaconAdvTimeout;
			uint32_t BleCanTpFC_Timeout;/*
			uint32_t BleCanTpSTMin_Timeout;*/
		}halfwords;
	}timerOutCnt;

	union{
		uint16_t data;
		struct{
			uint8_t	startPair							:1;
			uint8_t	pairComplete						:1;
			uint8_t	pendingAdv							:1;
			uint8_t	spAuthTmot							:1;

			uint8_t skfAuthTmot							:1;
			uint8_t pendingAdvScan						:1;
			uint8_t authVaild							:1;
			uint8_t bleImmo								:1;

			uint8_t phoneKeySupervisionTO				:1;
			uint8_t keyFobSupervisionTO					:1;
			uint8_t	restartAdv							:1;
			uint8_t startAppAdv							:1;

			uint8_t startIBeaconAdv						:1;
			uint8_t BleCanTpFC_Timeout					:1;/*
			uint8_t BleCanTpSTMin_Timeout				:1;*/
			uint8_t reserved							:2;
		}bits;
	}timeroutflag;
} bleAppTimer_t;

typedef struct{
	uint8_t device_id;
	uint8_t timeout_flag;
} bleAppTimerEvent_t;

#endif /* DRV_BLE_BLETIMER_BLETIMER_CFG_H_ */
