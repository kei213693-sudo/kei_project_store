/*

Copyright 2016-2020 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Drv */
#include "fsl_component_led.h"
#include "fsl_component_button.h"

/* Fwk */
#include "fsl_component_mem_manager.h"
#include "fsl_component_timer_manager.h"
#include "RNG_Interface.h"
#include "fsl_component_messaging.h"
#include "SecLib.h"
#include "fsl_component_panic.h"
#include "fsl_os_abstraction.h"
#include "fsl_adapter_reset.h"

/* KSDK */
#include "board.h"
#include "app.h"
#if defined(KW45B41Z83_SERIES) || defined(KW45B41Z82_SERIES) || defined(K32W1480_SERIES)
#include "fwk_platform.h"
#include "platform_genfsk.h"
#endif

#include "genfsk_interface.h"
#include "gen_fsk_tests.h"

#include "menus.h"
#include "connectivity_test.h"
#include "hal_typedef.h"
#include "hal_can_if.h"
#include "hal_pin_if.h"
#include "hal_clock_if.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define gAppNumberOfTests_d (5)
#define App_NotifySelf() OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtSelfEvent_c)

#ifndef mAppIdleHook_c
#define mAppIdleHook_c 0
#endif

#define Serial_Print(a,b,c) SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, (uint8_t *)b, strlen(b))
/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/
typedef bool_t ( * pCtTestFunction)(ct_event_t evt, void* pAssocData);
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
/*Application main*/
static void App_Thread (uint32_t param);
/*Application event handler*/
static void App_HandleEvents(osa_event_flags_t flags);
/*Function that reads latest byte from Serial Manager*/
static void App_UpdateUartData(uint8_t* pData);
/*Application Init*/
static app_status_t App_InitApp();
/*Print shortcut menu values*/
static void App_PrintTestParameters(bool_t bEraseLine);
/*Generic FSK RX callback*/
static void App_GenFskReceiveCallback(uint8_t *pBuffer,
                                        uint16_t bufferLength,
                                        uint64_t timestamp,
                                        int8_t rssi,
                                        uint8_t crcValid);
/*Generic FSK Notification callback*/
static void App_GenFskEventNotificationCallback(genfskEvent_t event,
                                                genfskEventStatus_t eventStatus);
/*Serial Manager UART RX callback*/
static void App_SerialCallback(void* param,
                               serial_manager_callback_message_t *message,
                                serial_manager_status_t status );
/*Application Thread notification function (sends event to application task)*/
static void App_NotifyAppThread(void);
/*Timer callback*/
static void App_TimerCallback(void* param);

/*Handles all keyboard events*/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint8_t platformInitialized = 0;
/*event used by the application thread*/
static OSA_EVENT_HANDLE_DEFINE(mAppThreadEvt);
/*variable to store key pressed by user*/
static uint8_t mAppUartData = 0;
/*variable to store button pressed by user*/
static uint8_t mAppPbData = PB_INVALID;
/*application state*/
static app_states_t mAppState = gAppStateInit_c;//app运行状态
/*set TRUE when user presses [ENTER] on logo screen*/
static bool_t mAppStartApp = FALSE;
/*pointer to test currently running*/
static pCtTestFunction pfCtCurrentTest = NULL;
/*pointer to data associated to each event*/
static void* pEvtAssociatedData = NULL;
/*array of existing tests*/
static pCtTestFunction ppfCtAvailableTests[gAppNumberOfTests_d];
/*structure to store information regarding latest received packet*/
static ct_rx_indication_t mAppRxLatestPacket;//存储rx数据
/*latest generic fsk event status*/
static genfskEventStatus_t mAppGenfskStatus;//genfsk事件状态

/*extern GENFSK instance id*/
extern uint8_t mAppGenfskId;
/*extern MCU reset api*/

uint32_t mRxBufferBytesAvailable = 0U;

static OSA_TASK_HANDLE_DEFINE(s_startTaskHandle);

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
/*osa start_task*/
void start_task(void *argument);
/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize
*         the system
*
* \param[in]  param
*
********************************************************************************** */
void main_task(uint32_t param)
{
    if (!platformInitialized)
    {
        uint8_t pseudoRNGSeed[20] = {0};
        platformInitialized = 1;

        /* Framework init */
        (void)MEM_Init();
        SecLib_Init();

        RNG_Init();
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[0])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[4])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[8])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[12])));
        RNG_GetRandomNo((uint32_t*)(&(pseudoRNGSeed[16])));
        RNG_SetPseudoRandomNoSeed(pseudoRNGSeed);

        #if (APP_SNIFFER_EN)
        hal_clock_if_init();
        hal_pin_if_init();
        hal_can_if_init(HAL_CAN0_CHANNEL);
		hal_pin_if_write(3, HAL_PIN_LEVEL_HIGH);
		hal_can_if_recv_callback(HAL_CAN0_CHANNEL,app_sniffer_can_rx);
        #endif
        
        GENFSK_Init();//初始化genfsk基本参数
        GENFSK_AllocInstance(&mAppGenfskId, NULL, NULL, NULL);//分配一个实例并配置相应参数

        OSA_EventCreate((osa_event_handle_t)mAppThreadEvt, TRUE);//创建主进程

        mAppSerId = gSerMgrIf;
        SerialManager_OpenReadHandle((serial_handle_t)mAppSerId, (serial_read_handle_t)g_connReadHandle);
        SerialManager_OpenWriteHandle((serial_handle_t)mAppSerId, (serial_write_handle_t)g_connWriteHandle);
        (void)SerialManager_InstallRxCallback((serial_read_handle_t)g_connReadHandle, (serial_manager_callback_t)App_SerialCallback, NULL);

        (void)TM_Open((timer_handle_t)mAppTmrId);//打开计时器

        /*Prints the Welcome screens in the terminal*/
        PrintMenu(cu8Logo, (serial_write_handle_t)g_connWriteHandle);

        //上电自动进入运行模式，不用按下确认按键'\r'
        mAppStartApp = TRUE;
        App_NotifySelf();
    }
    /* Call application task */
    App_Thread(param);

}


void start_task(void *argument)
{
    main_task((uint32_t)argument);
}

static OSA_TASK_DEFINE(start_task, gMainThreadPriority_c, 1, gMainThreadStackSize_c, 0);

int main(void)
{
    /*init OSA: should be called before any other OSA API*/
    OSA_Init();
    BOARD_InitHardware();
    PLATFORM_InitGenfsk();
    APP_InitServices();
//    BOARD_InitDebugConsole();//跟SerialManager不能共用一个uart

    (void)OSA_TaskCreate((osa_task_handle_t)s_startTaskHandle, OSA_TASK(start_task), NULL);
    
    OSA_Start();/*start scheduler*/

    /*won't run here*/
    assert(0);
    return 0;
}


/*! *********************************************************************************
* \brief  This function represents the Application task.
*         This task reuses the stack alocated for the MainThread.
*         This function is called to process all events for the task. Events
*         include timers, messages and any other user defined events.
* \param[in]  argument
*
********************************************************************************** */
void App_Thread (uint32_t param)
{
    osa_event_flags_t mAppThreadEvtFlags = 0;

    while(1)
    {
        (void)OSA_EventWait((osa_event_handle_t)mAppThreadEvt, gCtEvtEventsAll_c, FALSE, osaWaitForever_c ,&mAppThreadEvtFlags);

        if(mAppThreadEvtFlags)
        {
            if(mAppStartApp)
            {
                App_HandleEvents(mAppThreadEvtFlags);/*handle app events*/
            }
            else
            {
                if(mAppThreadEvtFlags & gCtEvtUart_c) /*if uart event*/
                {
                    App_UpdateUartData(&mAppUartData); /*read new byte*/

                    if(mAppUartData == '\r')
                    {
                        mAppStartApp = TRUE;
                        /*notify task again to start running*/
                        App_NotifySelf();//传递gCtEvtSelfEvent_c
                    }
                    else
                    {
                        /*if other key is pressed show screen again*/
                        PrintMenu(cu8Logo, (serial_write_handle_t)g_connWriteHandle);
                    }
                }

                if(mAppThreadEvtFlags & gCtEvtKBD_c) /*if KBD event*/
                {
                    mAppStartApp = TRUE;
                    /*notify task again to start running*/
                    App_NotifySelf();
                }
            }
        }
        if(gUseRtos_c == 0) /*if bare-metal break while*/
        {
            break;
        }
    }
}

/*! *********************************************************************************
* \brief  The application event handler
*         This function is called each time there is an OS event for the AppThread
* \param[in]  flags The OS event flags specific to the Connectivity Test App.
*
********************************************************************************** */
void App_HandleEvents(osa_event_flags_t flags)
{
    app_status_t status;

    if(flags & gCtEvtUart_c)
    {//串口事件
        App_UpdateUartData(&mAppUartData);//获取数据
        if(CT_IsShortcutMenuEnabled())
        {
            /*if it is a shortcut key clear the flag so it is not handled twice*/
            if(CT_UpdateShortcutKeyParam(mAppUartData))
            {//校验uart指令有效性
                /* Only print the RF parameter list once. In running mode, the RF parameter list will be updated by the
                category of test (Continuous tests, PER test or Range test). */
                if (mAppState != gAppStateRunning_c)
                {
                    App_PrintTestParameters(TRUE);//打印参数
                }
            }
        }
    }

    switch(mAppState)
    {
        case gAppStateInit_c://初始化成功了，运行时第1次进入这里
            /*initialize app and map tests*/
            status = App_InitApp();//注册测试函数
            if( status != gAppStatusSuccess )
            {
                Serial_Print(mAppSerId, cu8AppError[status], gAllowToBlock_d);
                break;
            }
            /*enter app default state*/
            mAppState = gAppStateIdle_c;//修改状态为空闲
            /*notify app task to move to new state*/
            App_NotifySelf();
        break;

        case gAppStateIdle_c://执行第1步
            /*print main menu*/
            PrintMenu(cu8MainMenu, (serial_write_handle_t)g_connWriteHandle);
            /*print test params*/
            App_PrintTestParameters(FALSE);
            mAppState = gAppStateSelectTest_c;
        #if (APP_SNIFFER_EN)
            flags = gCtEvtUart_c;//直接进入到第二步
            mAppUartData = '1';
        #else
            break;
        #endif

        case gAppStateSelectTest_c://执行第2步
            if(flags & gCtEvtUart_c) /*if uart event*/
            {
                if(mAppUartData >= '1' && mAppUartData <= ('0' + gAppNumberOfTests_d)) /*select which test to run*/
                {
                    pfCtCurrentTest = ppfCtAvailableTests[mAppUartData - '1'];//把待执行的测试函数地址提取出来
                    mAppState = gAppStateRunning_c;
                    App_NotifySelf();
                }
                else if(mAppUartData == '!')
                {
                    HAL_ResetMCU();
                }
            }
        break;

        /*event handling for test currently running*/
        case gAppStateRunning_c: //执行第3步
            if(flags & gCtEvtRxDone_c)
            {//rx接收到有效数据处理，CT_ContinuousTests（）处理
                pEvtAssociatedData = &mAppRxLatestPacket;
                if(pfCtCurrentTest(gCtEvtRxDone_c, pEvtAssociatedData))
                {
                    mAppState = gAppStateIdle_c;
                    App_NotifySelf();
                }
            }

            #if (APP_SNIFFER_EN)
            if(flags & gCtEvtCan_Lin_c)
            {//处理can数据
                if(pfCtCurrentTest(gCtEvtCan_Lin_c, pEvtAssociatedData))
                {
                    mAppState = gAppStateIdle_c;
                    App_NotifySelf();
                }
            }
            #endif

            /*if(flags & gCtEvtTxDone_c)
            {
                pEvtAssociatedData = &mAppGenfskStatus;
                if(pfCtCurrentTest(gCtEvtTxDone_c, pEvtAssociatedData))
                {
                    mAppState = gAppStateIdle_c;
                    App_NotifySelf();
                }
            }*/
            if(flags & gCtEvtRxFailed_c)
            {//rx接收失败处理
                if(pfCtCurrentTest(gCtEvtRxFailed_c, pEvtAssociatedData))
                {
                    mAppState = gAppStateIdle_c;
                    App_NotifySelf();
                }
            }
            if(flags & gCtEvtSeqTimeout_c)
            {//rx接收超时处理
                if(pfCtCurrentTest(gCtEvtSeqTimeout_c, NULL))
                {
                    mAppState = gAppStateIdle_c;
                    App_NotifySelf();
                }
            }
            if(flags & gCtEvtUart_c)
            {
                pEvtAssociatedData = &mAppUartData;
                if(pfCtCurrentTest(gCtEvtUart_c, pEvtAssociatedData))
                {
                    mAppState = gAppStateIdle_c;
                    App_NotifySelf();
                }
            }
            if(flags & gCtEvtTimerExpired_c)
            {
                pEvtAssociatedData = NULL;
                if(pfCtCurrentTest(gCtEvtTimerExpired_c, pEvtAssociatedData))
                {
                    mAppState = gAppStateIdle_c;
                    App_NotifySelf();
                }
            }
            if(flags & gCtEvtKBD_c)
            {
                pEvtAssociatedData = &mAppPbData;
                if(pfCtCurrentTest(gCtEvtKBD_c, pEvtAssociatedData))
                {
                    mAppState = gAppStateIdle_c;
                    App_NotifySelf();
                }
            }
            if(flags & gCtEvtSelfEvent_c)
            {
                if(pfCtCurrentTest(gCtEvtSelfEvent_c, NULL))
                {
                    mAppState = gAppStateIdle_c;
                    App_NotifySelf();
                }
            }
        break;

        default:
        break;
    }
}

/*! *********************************************************************************
* \brief  This function is called each time SerialManager notifies the application
*         task that a byte was received.
*         The function checks if there are additional bytes in the SerialMgr
*         queue and simulates a new SM event if there is more data.
* \param[in]  pData Pointer to where to store byte read.
*
********************************************************************************** */
static void App_UpdateUartData(uint8_t* pData)
{
    uint16_t u16SerBytesCount = 0;

    if(kStatus_SerialManager_Success == SerialManager_TryRead((serial_read_handle_t)g_connReadHandle, pData, 1U, (uint32_t *)&u16SerBytesCount))
    {
        mRxBufferBytesAvailable -= u16SerBytesCount;
        if(mRxBufferBytesAvailable > 0U)
        {
            (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtUart_c);
        }
    }
}

/*! *********************************************************************************
* \brief  Application initialization. It installs the main menu callbacks and
*         calls the Connectivity Test for Generic FSK init.
*
********************************************************************************** */
static app_status_t App_InitApp()
{
    //配置各opt对应的测试函数
    ppfCtAvailableTests[0] = CT_ContinuousTests;
    ppfCtAvailableTests[1] = CT_PacketErrorRate;
    ppfCtAvailableTests[2] = CT_RangeTest;
    ppfCtAvailableTests[3] = CT_TrimAdjust;
    ppfCtAvailableTests[4] = CT_PllNumOffsetAdjust;

    /*register callbacks for the generic fsk LL */
    GENFSK_RegisterCallbacks(mAppGenfskId,
                            App_GenFskReceiveCallback,//接收回调
                            App_GenFskEventNotificationCallback);//genfsk事件通知回调

    /*init and provide means to notify the app thread from connectivity tests*/
    return CT_GenFskInit(App_NotifyAppThread, App_TimerCallback);
}

/*! *********************************************************************************
* \brief  This function represents the Generic FSK receive callback.
*         This function is called each time the Generic FSK Link Layer receives a
*         valid packet
* \param[in]  pBuffer Pointer to receive buffer as byte array
* \param[in]  timestamp Generic FSK timestamp for received packet
* \param[in]  rssi The RSSI measured during the reception of the packet
*
********************************************************************************** */
static void App_GenFskReceiveCallback(uint8_t *pBuffer,
                                        uint16_t bufferLength,
                                        uint64_t timestamp,
                                        int8_t rssi,
                                        uint8_t crcValid)
{
    mAppRxLatestPacket.pBuffer      = pBuffer;
    mAppRxLatestPacket.bufferLength = bufferLength;
    mAppRxLatestPacket.timestamp    = timestamp;
    mAppRxLatestPacket.rssi         = rssi;
    mAppRxLatestPacket.crcValid     = crcValid;

    /*send event to app thread*/
    (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtRxDone_c);
}

/*! *********************************************************************************
* \brief  This function represents the Generic FSK event notification callback.
*         This function is called each time the Generic FSK Link Layer has
*         a notification for the upper layer
* \param[in]  event The event that generated the notification
* \param[in]  eventStatus status of the event
*
********************************************************************************** */
static void App_GenFskEventNotificationCallback(genfskEvent_t event,
                                                genfskEventStatus_t eventStatus)
{
    if(event & gGenfskTxEvent)
    {
        mAppGenfskStatus = eventStatus;
        /*send event done*/
        (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtTxDone_c);
    }
    if(event & gGenfskRxEvent)
    {
        if(eventStatus == gGenfskTimeout)
        {
            (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtSeqTimeout_c);
        }
        else
        {
            (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtRxFailed_c);
        }
    }
}

static void App_SerialCallback(void* param,
                               serial_manager_callback_message_t *message,
                                serial_manager_status_t status )
{
    mRxBufferBytesAvailable = message->length;
    (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtUart_c);
}

static void App_NotifyAppThread(void)
{
    App_NotifySelf();
}
static void App_TimerCallback(void* param)
{
    (void)OSA_EventSet((osa_event_handle_t)mAppThreadEvt, gCtEvtTimerExpired_c);
}

#if (APP_SNIFFER_EN)
void APP_CanCallback(void* param)
{
    OSA_EventSet(mAppThreadEvt, gCtEvtCan_Lin_c);
}
#endif
/*! *********************************************************************************
* \brief  Prints the test parameters: mode, channel, TX power and payload length.
*         Some or all of the above parameters are used throughout the tests.
********************************************************************************** */
static void App_PrintTestParameters(bool_t bEraseLine)
{
    PrintTestParams(gaConfigParams, bEraseLine, mAppSerId);
}

