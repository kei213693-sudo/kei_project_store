/*
 * bleInit.c
 *
 *  Created on: 2024年6月11日
 *      Author: F704012276_
 */

#include "ble_general.h"
#include "fwk_platform_ble.h"
#include "fwk_platform_ics.h" /* to get NbuInfo */
#include "assert.h"
#include "stdbool.h"
#include "app_preinclude.h"
#include "board.h"
#include "fwk_platform.h"
#include "signals_api.h"
#include "HWParameter.h"
#include "app_api.h"
#include "bleInstallCallback_api.h"
#include "srv_api.h"
#include "RNG_Interface.h"
#include "controller_api.h"
#include "NV_Flash.h"
#include "hci_transport.h"
#include "drv_api.h"
#include "srv_api.h"
#include "UWB_CAN.h"
#include "bleInit_cfg.h"
#include "UdsItf_ReadDid.h"

#if (defined(gAppForceLvdResetOnResetPinDet_d) && (gAppForceLvdResetOnResetPinDet_d > 0)) || \
    (defined(gAppForceDeepPowerDownResetOnResetPinDet_d) && (gAppForceDeepPowerDownResetOnResetPinDet_d > 0))
#include "fwk_platform_reset.h"
#endif

#if ((defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)) ||               \
     (defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)) || \
     (defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)))
/* board_comp.h only required for above services   */
#include "board_comp.h"
#endif

#if defined(gAppUseSensors_d) && (gAppUseSensors_d > 0)
#include "sensors.h"
#endif

#if defined(gAppLowpowerEnabled_d) && (gAppLowpowerEnabled_d > 0)
#include "fsl_pm_board.h"
#include "fsl_pm_core.h"
#include "PWR_Interface.h"
#include "board_lp.h"
#endif


/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#ifndef cMCU_SleepDuringBleEvents
    #define cMCU_SleepDuringBleEvents                      0
#endif /* cMCU_SleepDuringBleEvents */

#if defined(gUseHciTransportDownward_d) && gUseHciTransportDownward_d
#if defined (gBleSetMacAddrFromVendorCommand_d) && (gBleSetMacAddrFromVendorCommand_d == 1)
#define mHciSetMacAddrCommandLength_c       (8U)
#define gHciSetMacAddrCommand_c             0x0022U
#define HciCommand(opCodeGroup, opCodeCommand)\
    (((uint16_t)(opCodeGroup) & (uint16_t)0x3FU)<<(uint16_t)SHIFT10)|(uint16_t)((opCodeCommand) & 0x3FFU)
#define BT_USER_BD 254
#endif /* gBleSetMacAddrFromVendorCommand_d */
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
#if !defined(gUseHciTransportUpward_d) || (!gUseHciTransportUpward_d)
bool_t gHostInitResetController = gHostInitResetController_c;
#endif /* gUseHciTransportUpward_d */


/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

/* Define constraints for LPUART0 wake up so we can set/release the constraints at runtime based on application call
 * Wake up on serial can be done only on LPUART0 and FRO6M which are located in WAKE domain
 * We need to use the FRO6M as clock source for LPUART0
 * Regarding constraints, we need to keep the FRO6M running and keep the WAKE domain operational (SLEEP mode)
 */
#define APP_LPUART0_WAKEUP_CONSTRAINTS 2, PM_RESC_FRO_6M_ON, PM_RESC_WAKE_PD_PERI_OPERATIONAL

/* -------------------------------------------------------------------------- */
/*                              Public Variables                              */
/* -------------------------------------------------------------------------- */
#if defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)
/* Define serial manager handle*/
SERIAL_MANAGER_HANDLE_DEFINE(gSerMgrIf);
#endif /*gAppUseSerialManager_c*/


#if defined(gAppLowpowerEnabled_d) && (gAppLowpowerEnabled_d > 0)
#if (defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0))
static const serial_manager_lowpower_critical_CBs_t gSerMgr_LowpowerCriticalCBs = {
    .serialEnterLowpowerCriticalFunc = &PWR_LowPowerEnterCritical,
    .serialExitLowpowerCriticalFunc  = &PWR_LowPowerExitCritical,
};
#endif
#if (defined(gAppUseSensors_d) && (gAppUseSensors_d > 0))
static const Sensors_LowpowerCriticalCBs_t app_LowpowerSensorsCriticalCBs = {
    .SensorsEnterLowpowerCriticalFunc = &PWR_LowPowerEnterCritical,
    .SensorsExitLowpowerCriticalFunc  = &PWR_LowPowerExitCritical,
};
#endif
#endif


static void AppSetBD_ADDR(void);
static void BleInitPeerInfo(void);

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */
#if defined(gAppLowpowerEnabled_d) && (gAppLowpowerEnabled_d > 0)
static void APP_ServiceInitLowpower(void)
{
    PWR_ReturnStatus_t status = PWR_Success;

    /* It is required to initialize PWR module so the application
     * can call PWR API during its init (wake up sources...) */
    PWR_Init();

    /* Initialize board_lp module, likely to register the enter/exit
     * low power callback to Power Manager */
    BOARD_LowPowerInit();

    /* Set Deep Sleep constraint by default (works for All application)
     *   Application will be allowed to release the Deep Sleep constraint
     *   and set a deepest lowpower mode constraint such as Power down if it needs
     *   more optimization */
    status = PWR_SetLowPowerModeConstraint(PWR_DeepSleep);
    assert(status == PWR_Success);

#if (defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0))

#if defined(gAppLpuart0WakeUpSourceEnable_d) && (gAppLpuart0WakeUpSourceEnable_d > 0)
    /* To be able to wake up from LPUART0, we need to keep the FRO6M running
     * also, we need to keep the WAKE domain is SLEEP.
     * We can't put the WAKE domain in DEEP SLEEP because the LPUART0 is not mapped
     * to the WUU as wake up source */
    (void)PM_SetConstraints(PM_LP_STATE_NO_CONSTRAINT, APP_LPUART0_WAKEUP_CONSTRAINTS);
#endif

    /* Register PWR functions into SerialManager module in order to disable device lowpower
        during SerialManager processing. Typically, allow only WFI instruction when
        uart data are processed by serail manager  */
    SerialManager_SetLowpowerCriticalCb(&gSerMgr_LowpowerCriticalCBs);
#endif

#if defined(gAppUseSensors_d) && (gAppUseSensors_d > 0)
    Sensors_SetLowpowerCriticalCb(&app_LowpowerSensorsCriticalCBs);
#endif

    (void)status;
}
#endif /* gAppLowpowerEnabled_d */

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */
void APP_InitServices(void)
{
    /* Initialize Timers for Application, you can comment this out if no timer is used on Application */
    (void)PLATFORM_InitTimerManager();

    /* init application serial manager*/
//    BOARD_InitSerialManager((serial_handle_t)gSerMgrIf);

#if defined(gAppLowpowerEnabled_d) && (gAppLowpowerEnabled_d > 0)
    APP_ServiceInitLowpower();
#endif

}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void APP_RecoverLinkLayerFimrware(int reason)
{
    /* Customer shall add any code that could help for Software/Hardware recovery here */
    (void)reason;
    while (true)
    {
    }
}

static bool App_CheckNbuCompatibility(NbuInfo_t *nbu_info_p)
{
    /* Do whatever compatibility check with NBU version and return false for requesting recovery */
    (void)nbu_info_p;
    return true;
}

static void BleInitPeerInfo(void)
{
    uint8_t mPeerId = 0;

    /* Initialize table with peer devices information  */
    for (mPeerId = 0; mPeerId < (uint8_t)gAppMaxConnections_c; mPeerId++)
    {
        maPeerInformation[mPeerId].deviceId = gInvalidDeviceId_c;
        maPeerInformation[mPeerId].isLinkEncrypted = FALSE;
        maPeerInformation[mPeerId].appState = mAppIdle_c;
        maPeerInformation[mPeerId].appStage = pAppIdle_c;
        FLib_MemSet(&maPeerInformation[mPeerId].oobData, 0x00, sizeof(gapLeScOobData_t));
        FLib_MemSet(&maPeerInformation[mPeerId].peerOobData, 0x00, sizeof(gapLeScOobData_t));
    }
    //TODO temp
    maPeerInformation[0].customInfo.clientCfg.valueHandle = value_SK_TX;
}


/*! *********************************************************************************
* \brief        Configures Bluetooth Address
*
********************************************************************************** */
uint8_t MAC_ADDRESS[6] = {0};
static void AppSetBD_ADDR(void)
{
	static uint8_t bleAddressIndex = 0;
    hardwareParameters_t *pHWParams = NULL;
    uint8_t appBdAddr[gcBleDeviceAddressSize_c] = {0}/*APP_BD_ADDR*/;
    uint8_t appFixAddr[gcBleDeviceAddressSize_c] = APP_BD_ADDR;

    UDS_itf_readDid_4AFF(&bleAddressIndex);

    if(bleAddressIndex == 0xff)
    {
    	memcpy(appBdAddr, appFixAddr, gcBleDeviceAddressSize_c);
    }
    else
    {
    	memset(&bleTagAddressGroup[1], (bleAddressIndex/10)*16 + bleAddressIndex%10, 4);
    	memcpy(appBdAddr, bleTagAddressGroup, gcBleDeviceAddressSize_c);
    }

    memcpy(MAC_ADDRESS, appBdAddr, 6);
    Set_BLE_AddressIndex(bleAddressIndex);

    {
//		canMsg1ccRx.newMsgRecv_flg = 1;
//		canMsg1ccRx.msg.sig.scanCMD = 1;
		memcpy(canMsg1ccRx.msg.sig.scanPara, appBdAddr, 6);
	}

    (void)NV_ReadHWParameters(&pHWParams);

    if ( !FLib_MemCmp(appBdAddr, pHWParams->bluetooth_address, gcBleDeviceAddressSize_c) )
    {
        OSA_DisableIRQGlobal();
        FLib_MemCpy(pHWParams->bluetooth_address, appBdAddr, gcBleDeviceAddressSize_c);
        OSA_EnableIRQGlobal();
        (void)NV_WriteHWParameters();
    }
}

#if defined(gUseHciTransportDownward_d) && gUseHciTransportDownward_d
void Ble_SetBDAddr(void)
{
    uint8_t bleDeviceAddress[gcBleDeviceAddressSize_c] = {0};

    PLATFORM_GetBDAddr(bleDeviceAddress);

#if !defined(gBleSetMacAddrFromVendorCommand_d) || (defined(gBleSetMacAddrFromVendorCommand_d) && (gBleSetMacAddrFromVendorCommand_d == 0))
    /* Set BD address by API call */
    (void)Controller_SetDeviceAddress(bleDeviceAddress);
#else
    /* Set BD address by HCI message */
    uint8_t aHciPacket[mHciSetMacAddrCommandLength_c + gHciCommandPacketHeaderLength_c];
    uint16_t opcode = HciCommand(gHciVendorSpecificDebugCommands_c, gHciSetMacAddrCommand_c);

        /* Set HCI opcode */
    FLib_MemCpy((void*)aHciPacket, (const void*)&opcode, 2U);
    /* Set HCI parameter length */
    aHciPacket[2] = (uint8_t)mHciSetMacAddrCommandLength_c;
    /* Set command parameter ID */
    aHciPacket[3] = (uint8_t)BT_USER_BD;
    /* Set command parameter length */
    aHciPacket[4] = (uint8_t)6U;

    FLib_MemCpy((void*)&aHciPacket[gHciCommandPacketHeaderLength_c + 2U], (const void*)bleDeviceAddress, gcBleDeviceAddressSize_c);

    /* Send HCI command */
    (void)Hcit_SendPacket(gHciCommandPacket_c, aHciPacket, gHciCommandPacketHeaderLength_c + mHciSetMacAddrCommandLength_c);
#endif /* gBleSetMacAddrFromVendorCommand_d */
}
#endif

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

int APP_InitBle(void)
{
    int              status = 0;
    static NbuInfo_t nbu_info;

    do
    {
        status = PLATFORM_InitTimerManager();
        if (status < 0)
        {
            break;
        }
        /* Start link layer firmware and set up HCI link */
        status = PLATFORM_InitBle();
        if (status < 0)
        {
            /* Platform Ble can not start, start recovery */
            APP_RecoverLinkLayerFimrware(1);
            break;
        }
        else
        {
            status = PLATFORM_GetNbuInfo(&nbu_info);
            if ((status < 0) || (App_CheckNbuCompatibility(&nbu_info) == false))
            {
                /* Link Layer version is not compatible, start recovery */
                APP_RecoverLinkLayerFimrware(2);
                break;
            }
        }
    } while (0);

    return status;
}

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BluetoothLEHost_AppInit(void)
{
    union Prompt_tag
    {
        const char * constPrompt;
        char * prompt;
    } shellPrompt;

	BleInitPeerInfo();

    /* Configures Bluetooth address on the anchor form APP_BD_ADDR  */
    AppSetBD_ADDR();

    /* Add/modify init code starting from here */
    BleApp_RegisterUserInterfaceEventHandler(APP_UserInterfaceEventHandler);
    BleApp_RegisterEventHandler(APP_BleEventHandler);
    BleApp_RegisterAppEventHandler(APP_AppEventHandler);
    BleApp_RegisterAppMessageHandler(APP_AppMessageHandler);

    /* Set generic callback */
    BluetoothLEHost_SetGenericCallback(BleApp_GenericCallback);

    /* Initialize Bluetooth Host Stack */
    BluetoothLEHost_Init(BluetoothLEHost_Initialized);
}

/*! *********************************************************************************
*\fn           void BluetoothLEHost_Init(
*                  appBluetoothLEInitCompleteCallback_t pCallback
*              )
*\brief        This is the host initialization function (initialize the Bluetooth LE
*              stack and also all framework and controller required components).
*
*\param  [in]  pCallback    Callback triggered when the initialization is
*                           complete.
*
*\retval       void.
********************************************************************************** */
void BluetoothLEHost_Init
(
    appBluetoothLEInitCompleteCallback_t pCallback
)
{
        (void)MEM_Init();

        /* Framework init */
#if defined(gRngSeedStorageAddr_d) || defined(gXcvrDacTrimValueSorageAddr_d)
        NV_Init();
#endif /* gRngSeedStorageAddr_d || gXcvrDacTrimValueSorageAddr_d */

        /* Cryptographic hardware initialization */
        SecLib_Init();

        /* RNG software initialization and PRNG initial seeding (from hardware) */
        (void)RNG_Init();
        RNG_SetPseudoRandomNoSeed(NULL);

        /*Has to be called after RNG_Init(), once seed is generated.*/
        (void)Controller_SetRandomSeed();

#if defined(BOARD_32KHZ_SRC_CLK_ACCURACY)
        /* Set sleep clock accuracy value in the controller,
         * set to 0 by LL if Controller_SetSleepClockAccuracy() not called */
        Controller_SetSleepClockAccuracy(BOARD_32KHZ_SRC_CLK_ACCURACY);
#endif

#if (defined(gAppSecureMode_d) && (gAppSecureMode_d > 0U))
        (void)PLATFORM_EnableBleSecureKeyManagement();
#endif
#if defined(gAppUseNvm_d) && (gAppUseNvm_d>0)
        /* Initialize NV module */
        (void)NvModuleInit();
#endif /* gAppUseNvm_d */

        /* Create application event */
        (void)OSA_EventCreate(mAppEvent, TRUE);
        (void)OSA_EventCreate(mBleAppEvent, TRUE);

        /* Prepare application input queue.*/
        MSG_QueueInit(&mHostAppInputQueue);
        MSG_QueueInit(&mAppCbInputQueue);
        MSG_QueueInit(&mAppEventQueue);
        MSG_QueueInit(&mAppMessageQueue);

        /* BLE common part */
        mpfInitDoneCallback = pCallback;
        mpfGenericHandler = App_GenericHandler;

        /* BLE Host Stack Init */
        if (Ble_Initialize(App_GenericCallback) != gBleSuccess_c)
        {
//            panic(0,0,0,0);
            return;
        }

#if (gAppUseNvm_d && defined(gFsciIncluded_c) && (gFsciIncluded_c))
#if gNvmEnableFSCIMonitoring_c
        NV_SetFSCIMonitoringState(TRUE);
#endif /* gNvmEnableFSCIMonitoring_c */
#if gNvmEnableFSCIRequests_c
        NV_RegisterToFSCI();
#endif /* gNvmEnableFSCIRequests_c */
#endif /* gAppUseNvm_d && gFsciIncluded_c */

    /* configure tx power to use in NBU specfic to BLE */
    (void)Controller_SetTxPowerLevelDbm(mAdvertisingDefaultTxPower_c, gAdvTxChannel_c);
    (void)Controller_SetTxPowerLevelDbm(mConnectionDefaultTxPower_c, gConnTxChannel_c);
    (void)Controller_ConfigureInvalidPduHandling(gLlInvalidPduHandlingType_c);
}

/*! *********************************************************************************
*\fn           bleResult_t Ble_Initialize(gapGenericCallback_t gapGenericCallback)
*\brief        Performs full initialization of the BLE stack.
*
*\param  [in]  genericCallback    Callback used by the Host Stack to propagate GAP
*                                 generic events to the application.
*
*\return       bleResult_t        Result of the operation.
********************************************************************************** */
bleResult_t Ble_Initialize
(
    gapGenericCallback_t gapGenericCallback
)
{
#if defined(gUseHciTransportDownward_d) && gUseHciTransportDownward_d
    /* HCI Transport Init */
    if (gHciSuccess_c != Hcit_Init(Ble_HciRecvFromIsr))
    {
        return gHciTransportError_c;
    }

    /*
     * Set BD Address in Controller. Must be done after HCI init
     * and before Host init.
     */
    Ble_SetBDAddr();

    /* Check for available memory storage */
    if (!Ble_CheckMemoryStorage())
    {
        return gBleOutOfMemory_c;
    }

    /* BLE Host Tasks Init */
    if (KOSA_StatusSuccess != Ble_HostTaskInit())
    {
        return gBleOsError_c;
    }

    /* BLE Host Stack Init */
    return Ble_HostInitialize(gapGenericCallback, Hcit_SendPacket);

#elif defined(gUseHciTransportUpward_d) && gUseHciTransportUpward_d

    if (KOSA_StatusSuccess != Controller_TaskInit())
    {
        return gBleOsError_c;
    }

    /* BLE Controller Init */
    if (KOSA_StatusSuccess != Controller_Init(Hcit_SendPacket))
    {
        return gBleOsError_c;
    }

    return Controller_SendPacketToController(Hci_SendPacketToController);

#else /* gUseHciTransportUpward_d */

    /* BLE Controller Init */
    if (KOSA_StatusSuccess != Controller_Init(Ble_HciRecv))
    {
        return gBleOsError_c;
    }

    /* Check for available memory storage */
    if (!Ble_CheckMemoryStorage())
    {
        return gBleOutOfMemory_c;
    }

    /* BLE Host Tasks Init */
    if (KOSA_StatusSuccess != Ble_HostTaskInit())
    {
        return gBleOsError_c;
    }

    /* BLE Host Stack Init */
    return Ble_HostInitialize(gapGenericCallback,
                (hciHostToControllerInterface_t) Controller_SendPacketToController);

#endif /* gUseHciTransportUpward_d */
}

/*! *********************************************************************************
*\fn           bleResult_t Ble_DeInitialize(void)
*\brief        Frees the resources not allocated by the BLE stack.
*
*\param  [in]  none
*
*\return       bleResult_t        Result of the operation.
********************************************************************************** */
bleResult_t Ble_DeInitialize(void)
{
#if defined(gUseHciTransportDownward_d) && gUseHciTransportDownward_d
    /* HCI Transport Init */
    bleResult_t result = Hcit_Deinit();
    if(result == gHciSuccess_c)
    {
         /* BLE Host Tasks DeInit */
        if (KOSA_StatusSuccess != Ble_HostTaskDeInit())
        {
            result = gBleOsError_c;
        }
    }
    return result;
#elif defined(gUseHciTransportUpward_d) && gUseHciTransportUpward_d
return gBleSuccess_c;
#else /* gUseHciTransportUpward_d */
return gBleSuccess_c;
#endif /* gUseHciTransportUpward_d */
}
