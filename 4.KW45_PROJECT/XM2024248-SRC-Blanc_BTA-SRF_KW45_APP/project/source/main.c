/*! *********************************************************************************
 * \addtogroup Main
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2021-2022 NXP
* All rights reserved.
*
* \file
*
* This is the source file for the main entry point for a FreeRTOS application.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/
#include "app.h"

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
#define BOOT_DEBUG
#define MEMORY_APP_ADDR (0x28000)
int main(void)
{
#ifdef BOOT_DEBUG
	SCB->VTOR = MEMORY_APP_ADDR;
#endif
    /* Init OSA: should be called before any other OSA API */
	OSA_Init();
	SystemosIint();
	PrintfResetReason();
	EcuM_SetModuleState(ECUM_State_Normal);
	swTimerCreate();
	OsStart();

    /*won't run here*/
    assert(0);
    return 0;
}

/*! *********************************************************************************
 * @}
 * 				memcpy(maPeerInformation[i].maPeerDeviceAddress_IRK,\
						gAppOutKeys.aIrk, gcSmpIrkSize_c);
 * prvCopyDataFromQueue
 * Identity Resolving Key:
 * Ble_Initialize
 * gDeviceAddedToFilterAcceptList_c
 * Gap_ReadRssi()
 * maPeerInformation
 * NVM_region
 * PROD_DATA
 * Gap_GetBondedDevicesIdentityInformation
 * Gap_SaveKeys
 * Gap_LoadKeys
 * Gap_LoadEncryptionInformation
 * Gap_Authorize
 * Gap_SaveCustomPeerInformation
 * App_NvmRead
 * Hci_LeAddDeviceToResolvingList
 * HostPrivacy
 * {'A','A','A','A'}
 * GAP_LoadKeys
 ********************************************************************************** */
