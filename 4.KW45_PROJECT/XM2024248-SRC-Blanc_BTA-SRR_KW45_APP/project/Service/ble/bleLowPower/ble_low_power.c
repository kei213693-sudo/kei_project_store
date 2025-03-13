/*
 * ble_low_power.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "app_preinclude.h"

#if defined(gAppUseNvm_d) && (gAppUseNvm_d > 0)
#include "NVM_Interface.h"
#if defined(gFsciIncluded_c) && (gFsciIncluded_c == 1)
#include "NV_FsciCommands.h"
#endif /* gFsciIncluded_c */
#endif /* gAppUseNvm_d */

/*! *********************************************************************************
*\private
*\fn           void BluetoothLEHost_ProcessIdleTask(void)
*\brief        Handles Connectivity background task, usually executed from Idle task.
*
*\param  [in]  none.
*
*\retval       void.
********************************************************************************** */
void BluetoothLEHost_ProcessIdleTask(void)
{
#if defined(gAppUseNvm_d) && (gAppUseNvm_d > 0)
    if(NvIdle() == 0)
#endif /* gAppUseNvm_d */
    {
#if defined (gAppOtaASyncFlashTransactions_c) && (gAppOtaASyncFlashTransactions_c > 0)
        (void)OTA_TransactionResume();
#endif
    }
}

/*! *********************************************************************************
*\private
*\fn           void BluetoothLEHost_IsConnectivityTaskToProcess(void)
*\brief        Returns if there is Connectivity background task to process.
*
*\param  [in]  none.
*
*\retval       TRUE     If there is a connectivity task to process
*\retval       FALSE    If there is no connectivity task to process
********************************************************************************** */
bool_t BluetoothLEHost_IsConnectivityTaskToProcess(void)
{
    bool_t isConnectivityTaskToProcess = FALSE;
    do
    {
#if defined(gAppUseNvm_d) && (gAppUseNvm_d > 0)
        if(NvIsPendingOperation())
        {
            isConnectivityTaskToProcess = TRUE;
            break;
        }
#endif /* gAppUseNvm_d */

#if defined (gAppOtaASyncFlashTransactions_c) && (gAppOtaASyncFlashTransactions_c > 0)
        if(OTA_IsTransactionPending())
        {
            isConnectivityTaskToProcess = TRUE;
            break;
        }
#endif
    }while(false);

    return isConnectivityTaskToProcess;
}
