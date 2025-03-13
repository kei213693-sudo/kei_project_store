/*

Copyright 2016-2020 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#include "fsl_component_mem_manager.h"
#include "fsl_component_timer_manager.h"
#include "fsl_component_serial_manager.h"
#include "fsl_format.h"
#include "RNG_Interface.h"
#include "gen_fsk_tests_states.h"
#include "gen_fsk_tests.h"
#include "nxp_xcvr_msk_config.h"
#include "nxp_xcvr_coding_config.h"
#include "genfsk_utils.h"
#include "menus.h"
#include "clock_config.h"
#include "fsl_port.h"
#include "fwk_platform.h"
// #include "fsl_debug_console.h"

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
/*serial interface id*/
serial_handle_t mAppSerId;
SERIAL_MANAGER_READ_HANDLE_DEFINE(g_connReadHandle);
SERIAL_MANAGER_WRITE_HANDLE_DEFINE(g_connWriteHandle);
/*timers manager app timer id*/
TIMER_MANAGER_HANDLE_DEFINE(mAppTmrId);
/*GENFSK instance id*/
uint8_t mAppGenfskId;
/*configuration params*/
ct_config_params_t gaConfigParams[gConfParamIdxMax];

char* strDescr[gMaxRate_c+1]        = {"GENFSK 250Kbps",
                                        "GENFSK 500Kbps",
                                        "  GENFSK 1Mbps",
                                        "  GENFSK 2Mbps",
                                        "BLE LR 125Kbps",
                                        "BLE LR 500Kbps",
                                        "     BLE 1Mbps",
                                        "     BLE 2Mbps"};

char* strWhitenDescr[gMaxWhitenMode_c+1]        = {"Fixed"," Chan"};

uint8_t crtRate[gRateMaxNbr]                  = {gRateGenfsk1Mbps_c, gRateGenfsk500Kbps_c, gRateGenfsk250Kbps_c, gRateGenfsk2Mbps_c};
uint8_t targetRate[gMaxRate_c+1]    = {gGenfskDR250Kbps, gGenfskDR500Kbps, gGenfskDR1Mbps, gGenfskDR2Mbps,
                                        /* For BLE LR rates, no need to select alternate rate => use gGenfskDR1Mbps rate*/
                                        gGenfskDR1Mbps, gGenfskDR1Mbps, gGenfskDR1Mbps, gGenfskDR2Mbps};


/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define gPerOpcode1 (0xCA)
#define gPerOpcode2 (0xFE)

#define gRangeOpcode1 (0xAB)
#define gRangeOpcode2 (0xCD)

/* To be checked */
#define CCM32K_CSHUNT_VALUE     5U

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
/* PER RX handler */
static bool_t CT_PacketErrorRateRx(ct_event_t evType, void* pAssociatedValue, bool_t bReInit);
/* PER TX handler */
static bool_t CT_PacketErrorRateTx(ct_event_t evType, void* pAssociatedValue, bool_t bReInit);
/* Range RX handler */
static bool_t CT_RangeRx(ct_event_t evType, void* pAssociatedValue, bool_t bReInit);
/* Range RX handler */
static bool_t CT_RangeTx(ct_event_t evType, void* pAssociatedValue, bool_t bReInit);
/* Handles the activation/deactivation of low/high power transmit levels */
static void CT_HandleHighLowPower(void);

/* Wrapper to convert types */
static void CT_WrapperRadioModeAndDataRate(xcvr_currConfig_t * pCurrConfig,
                                        genfskRadioMode_t* pRadioModeIn, genfskDataRate_t*  pDataRate);

/* Handler that applies latest values for channel and tx power and prints shortcuts values */
static bool_t CT_ApplyPrintConfigParams(void);
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
/* buffers for interaction with Generic FSK */
uint8_t* gRxBuffer;
uint8_t* gTxBuffer;

/* Generic FSK packets to get formatted data*/
static GENFSK_packet_t gRxPacket;
static GENFSK_packet_t gTxPacket;

/* control if shortcuts are enabled or disabled*/
static bool_t   bEnableShortcuts = FALSE;

/*hook to notify app thread*/
static pHookAppNotification pNotifyAppThread = NULL;
/*hook to notify app thread*/
static pTmrHookNotification pTmrCallback = NULL;

/*! @brief GENFSK default mode configuration. */
static GENFSK_mode_config_t mModeConfig =
{
    .mode = gGenfskGllMode,
    .stayInRx = 0,
    .turnaroundTime = 150
};

/*packet configuration*/
static GENFSK_packet_config_t pktConfig =
{
    .preambleSizeBytes = 0, /*1 byte of preamble*/
    .packetType = gGenfskFormattedPacket,
    .lengthSizeBits = gGenFskDefaultLengthFieldSize_c,
    .lengthBitOrder = gGenfskLengthBitLsbFirst,
    /*sync address bytes = size + 1*/
    .syncAddrSizeBytes = gGenFskDefaultSyncAddrSize_c,
    .lengthAdjBytes = gGenFskDefaultCrcSize_c, /*length field not including CRC so adjust by crc len*/
    .h0SizeBits = gGenFskDefaultH0FieldSize_c,
    .h1SizeBits = gGenFskDefaultH1FieldSize_c,
    .h0Match = gGenFskDefaultH0Value_c, /*match field containing zeros*/
    .h0Mask = gGenFskDefaultH0Mask_c,
    .h1Match = gGenFskDefaultH1Value_c,
    .h1Mask = gGenFskDefaultH1Mask_c,
#if defined (RADIO_IS_GEN_3P5) || defined(RADIO_IS_GEN_4P0)
    .preambleBytePattern = 0
#endif
};

/*CRC configuration*/
GENFSK_crc_config_t crcConfig =
{
    .crcEnable = gGenfskCrcEnable,
    .crcSize = gGenFskDefaultCrcSize_c,
    .crcStartByte = gGenFskDefaultSyncAddrSize_c+1,
    .crcRefIn = gGenfskCrcInputNoRef,
    .crcRefOut = gGenfskCrcOutputNoRef,
    .crcByteOrder = gGenfskCrcLSByteFirst,
    .crcSeed = gGenFskDefaultCrcSeed_c,
    .crcPoly = gGenFskDefaultCrccPoly_c,
    .crcXorOut = 0
};

/*whitener configuration*/
GENFSK_whitener_config_t whitenConfig =
{
    .whitenEnable = gGenfskWhitenEnable,
 /* TEST_DTM_LIKE */
    .whitenStart = gWhitenStartWhiteningAtH0,
    .whitenEnd = gWhitenEndAtEndOfCrc,
    .whitenB4Crc = gCrcB4Whiten,
    .whitenPolyType = gGaloisPolyType,
    .whitenRefIn = gGenfskWhitenInputNoRef,
    .whitenPayloadReinit = gGenfskWhitenNoPayloadReinit,
    .whitenSize = 7,
    .whitenInit = 0x53,
    .whitenPoly = 0x11, /*x^7 + x^4 + 1! x^7 is never set*/
    .whitenSizeThr = 0,
    .manchesterEn = gGenfskManchesterDisable,
    .manchesterStart = gGenfskManchesterStartAtPayload,
    .manchesterInv = gGenfskManchesterNoInv,
};

/*radio configuration*/
GENFSK_radio_config_t radioConfig =
{
    .radioMode = gGenfskGfskBt0p5h0p5,
    .dataRate = gGenfskDR1Mbps
};

/*bit processing configuration*/

/*network / sync address configuration*/
GENFSK_nwk_addr_match_t ntwkAddr =
{
    .nwkAddrSizeBytes = gGenFskDefaultSyncAddrSize_c,
    .nwkAddrThrBits = 0,
    .nwkAddr = gGenFskDefaultSyncAddress_c,
};

/* Xtal trim set to invalid value*/
static uint8_t xtalTrim = 0xFFU; /**/

/**********************************************************************************/
app_status_t CT_GenFskInit(pHookAppNotification pFunc, pTmrHookNotification pTmrFunc)
{
    bEnableShortcuts = TRUE;

    /*configure hook*/
    pNotifyAppThread = pFunc;

    /*configure timer callback*/
    pTmrCallback = pTmrFunc;

    /* populate shortcut array */
    gaConfigParams[gConfParamMode].paramType = gParamTypeString_c;
    FLib_MemCpy(gaConfigParams[gConfParamMode].paramName, "Mode", 5);
    FLib_MemCpy(gaConfigParams[gConfParamMode].paramValue.stringValue, "RX", 3);
    if(gDefaultMode_c == gModeTx_c)
    {
        gaConfigParams[gConfParamMode].paramValue.stringValue[0] = 'T';
    }

    gaConfigParams[gConfParamRateMode].paramType = gParamTypeMixed_c;
    FLib_MemCpy(gaConfigParams[gConfParamRateMode].paramName, "Rate", 5);
    gaConfigParams[gConfParamRateMode].id = gDefaultRate_c;
    gaConfigParams[gConfParamRateMode].ptr = strDescr;

    gaConfigParams[gConfParamWhitenMode].paramType = gParamTypeMixed_c;
    FLib_MemCpy(gaConfigParams[gConfParamWhitenMode].paramName, "Whiten", 7);
    gaConfigParams[gConfParamWhitenMode].id = gDefaultWhitenMode_c;
    gaConfigParams[gConfParamWhitenMode].ptr = strWhitenDescr;

    gaConfigParams[gConfParamChannel].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[gConfParamChannel].paramName, "Channel", 8);
    gaConfigParams[gConfParamChannel].paramValue.decValue = gGenFskDefaultChannel_c;

    gaConfigParams[gConfParamPower].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[gConfParamPower].paramName, "Power", 6);
    gaConfigParams[gConfParamPower].paramValue.decValue = gGenFskDefaultTxPowerLevel_c;

    gaConfigParams[gConfParamPayload].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[gConfParamPayload].paramName, "Payload", 8);
    gaConfigParams[gConfParamPayload].paramValue.decValue = gGenFskDefaultPayloadLen_c;

    gaConfigParams[gConfParamXtalTrim].paramType = gParamTypeNumber_c;
    FLib_MemCpy(gaConfigParams[gConfParamXtalTrim].paramName, "XtalTrim", 9);

    /* Get trim value */
    xtalTrim = GENFSK_GetSavedXtalTrim();
    gaConfigParams[gConfParamXtalTrim].paramValue.decValue = xtalTrim;

    gaConfigParams[gConfParamTypeMax].paramType = gParamTypeMaxType_c;
    /* allocate once to use for the entire application */
    gRxBuffer  = MEM_BufferAlloc(gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize);
    if(gRxBuffer==0)
        return gAppStatusMemoryAllocFailure;

    gTxBuffer  = MEM_BufferAlloc(gGenFskDefaultMaxBufferSize_c);
    while(gTxBuffer==0)
    {
        MEM_BufferFree(gRxBuffer);
        return gAppStatusMemoryAllocFailure;
    }

    gRxPacket.payload = (uint8_t*)MEM_BufferAlloc(gGenFskMaxPayloadLen_c  + crcConfig.crcSize);
    while(gRxPacket.payload==0)
    {
        MEM_BufferFree(gRxBuffer);
        MEM_BufferFree(gTxBuffer);
        return gAppStatusMemoryAllocFailure;
    }

    gTxPacket.payload = (uint8_t*)MEM_BufferAlloc(gGenFskMaxPayloadLen_c);
    while(gTxPacket.payload==0)
    {
        MEM_BufferFree(gRxBuffer);
        MEM_BufferFree(gTxBuffer);
        MEM_BufferFree(gRxPacket.payload);
        return gAppStatusMemoryAllocFailure;
    }

    /*prepare the part of the tx packet that is common for all tests*/
    gTxPacket.addr = gGenFskDefaultSyncAddress_c;
    gTxPacket.header.h0Field = gGenFskDefaultH0Value_c;
    gTxPacket.header.h1Field = gGenFskDefaultH1Value_c;

    /*set bitrate*/
    GENFSK_RadioConfig(mAppGenfskId, &radioConfig);
    /*set packet config*/
    GENFSK_SetPacketConfig(mAppGenfskId, &pktConfig);
    /*set whitener config*/
    GENFSK_SetWhitenerConfig(mAppGenfskId, &whitenConfig);
    /*set crc config*/
    GENFSK_SetCrcConfig(mAppGenfskId, &crcConfig);

    /*set network address at location 0 and enable it*/
    GENFSK_SetNetworkAddress(mAppGenfskId, 0, &ntwkAddr);
    GENFSK_EnableNetworkAddress(mAppGenfskId, 0);

    /*set tx power level*/
    GENFSK_SetTxPowerLevel(mAppGenfskId, gGenFskDefaultTxPowerLevel_c);
    /*set channel: Freq = 2360MHz + ChannNumber*1MHz*/
    GENFSK_SetChannelNumber(mAppGenfskId, gGenFskDefaultChannel_c);

    /* Whitening specific configuration (withen init depending on channel) only applies to BLE */
    {
        GENFSK_mode_config_t modeConfig;

        (void)GENFSK_GetModeConfig(mAppGenfskId, &modeConfig);

        if ((modeConfig.mode == gGenfskBleLrMode) || (modeConfig.mode == gGenfskBleUncodedMode))
        {
            GENFSK_SetBleWhitenInit(mAppGenfskId, gGenFskDefaultChannel_c);
        }
    }

    return gAppStatusSuccess;
}


/****************************************************************************
*****************************Packet error rate*******************************
****************************************************************************/
bool_t CT_PacketErrorRate(ct_event_t evType, void* pAssociatedValue)
{
    bool_t bReturnFromSM = FALSE;
    static uint8_t currentTest = 0;

    if(gaConfigParams[gConfParamMode].paramValue.stringValue[0] == 'R')
    {
        bReturnFromSM = CT_PacketErrorRateRx(evType, pAssociatedValue, (currentTest != 'R'));
        currentTest = bReturnFromSM? 0 : 'R';
    }
    else
    {
        bReturnFromSM = CT_PacketErrorRateTx(evType, pAssociatedValue, currentTest != 'T');
        currentTest = bReturnFromSM ? 0 : 'T';
    }
    return bReturnFromSM;
}

/*! *********************************************************************************
* \brief  Handles the Packet error rate RX test
********************************************************************************** */
static bool_t CT_PacketErrorRateRx(ct_event_t evType,void* pAssociatedValue,bool_t bReInit)
{
    static ct_per_rx_states_t perRxState = gPerRxStateInit_c;
    static int32_t  i32RssiSum;
    static uint16_t u16ReceivedPackets;
    static uint16_t u16PacketsIndex;
    static uint16_t u16TotalPackets;

    int8_t   i8AverageRssi = 0;
    ct_rx_indication_t* pIndicationInfo = NULL;

    uint8_t u8UartData = 0xFF;

    uint8_t* pRxBuffer = NULL;
    bool_t bRestartRx = FALSE;
    bool_t bReturnFromSM = FALSE;

    (void)CT_ApplyPrintConfigParams();

    if(bReInit) /* Reset the state machine */
    {
        perRxState = gPerRxStateInit_c;
    }
    if(gCtEvtUart_c == evType) /* if UART event extract the character */
    {
        u8UartData = *((uint8_t*)pAssociatedValue);
    }
    switch(perRxState)
    {
    case gPerRxStateInit_c:
        CT_EnableShortcuts(TRUE);
        /*print per rx menu and shortcut keys*/
        PrintMenu(cu8ShortCutsBar, (serial_write_handle_t)g_connWriteHandle);
        PrintMenu(cu8PerRxTestMenu, (serial_write_handle_t)g_connWriteHandle);

        PrintTestParams(gaConfigParams, FALSE, mAppSerId);

        u16TotalPackets = 0;
        u16ReceivedPackets = 0;
        u16PacketsIndex = 0;
        i32RssiSum = 0;
        perRxState = gPerRxStateWaitStartTest_c;
        break;
    case gPerRxStateWaitStartTest_c:
        if(gCtEvtUart_c == evType)
        {
            if(' ' == u8UartData)
            {
                Serial_Print(mAppSerId, cu8PerRxMessages[0], gAllowToBlock_d);

                if(gGenfskSuccess_c !=
                    GENFSK_StartRx(mAppGenfskId,
                                    gRxBuffer,
                                    gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize,
                                    0,
                                    0)
                    )
                {
                    GENFSK_AbortAll();
                    Serial_Print(mAppSerId, cu8PerRxMessages[1], gAllowToBlock_d);
                    perRxState = gPerRxStateIdle_c;
                }
                else
                {
                    CT_EnableShortcuts(FALSE);
                    perRxState = gPerRxStateStartTest_c;
                }
            }
            else if('p' == u8UartData)
            {
                perRxState = gPerRxStateInit_c;
                bReturnFromSM = TRUE;
            }
        }
        break;
        
    case gPerRxStateStartTest_c:
        if(gCtEvtRxDone_c == evType || gCtEvtRxFailed_c == evType || gCtEvtSeqTimeout_c == evType) /*check if RX related events are fired */
        {
            if (gCtEvtRxDone_c == evType) /*if rx succesfull, get packet information */
            {
                pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;
                pRxBuffer = pIndicationInfo->pBuffer; /*same as gRxBuffer*/

                /*map rx buffer to generic fsk packet*/
                GENFSK_ByteArrayToPacket(mAppGenfskId, pRxBuffer, &gRxPacket);
                if(gRxPacket.payload[4] == gPerOpcode1 &&
                   gRxPacket.payload[5] == gPerOpcode2) /* check if packet payload is PER type */
                {
                    u16TotalPackets = ((uint16_t)gRxPacket.payload[0] <<8) + gRxPacket.payload[1];
                    u16PacketsIndex = ((uint16_t)gRxPacket.payload[2] <<8) + gRxPacket.payload[3];
                    u16ReceivedPackets++;
                    i32RssiSum += (int8_t)(pIndicationInfo->rssi);

                    /* print statistics */
                    PrintPerRxInfo(u16PacketsIndex, u16ReceivedPackets, pIndicationInfo, mAppSerId);

                    if(u16PacketsIndex == u16TotalPackets) /* received last packet */
                    {
                        Serial_Print(mAppSerId, cu8PerRxMessages[3], gAllowToBlock_d);
                        perRxState = gPerRxStateIdle_c;
                        pNotifyAppThread();
                    }
                    else
                    {
                        bRestartRx = TRUE;
                    }
                }
                else
                {
                    bRestartRx = TRUE;
                }
            }
            else
            {
                bRestartRx = TRUE;
            }
            if(bRestartRx) /*restart RX immediately with no timeout*/
            {
                if(gGenfskSuccess_c !=
                    GENFSK_StartRx(mAppGenfskId,
                                    gRxBuffer,
                                    gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize,
                                    0,
                                    0)
                    )
                {
                    GENFSK_AbortAll();
                    Serial_Print(mAppSerId, cu8PerRxMessages[1], gAllowToBlock_d);
                    perRxState = gPerRxStateIdle_c;
                }
            }
        }
        if(gCtEvtUart_c == evType)
        {
            if(' ' == u8UartData)
            {
                GENFSK_AbortAll();
                Serial_Print(mAppSerId, cu8PerRxMessages[2], gAllowToBlock_d);
                pNotifyAppThread(); /* send gCtEvtSelfEvent_c event */
                perRxState = gPerRxStateIdle_c;
            }
        }
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8PerRxMessages[2], gAllowToBlock_d);
            pNotifyAppThread(); /* send gCtEvtSelfEvent_c event */
            perRxState = gPerRxStateIdle_c;
        }
        break;
    case gPerRxStateIdle_c:
        if((gCtEvtUart_c == evType) && ('\r' == u8UartData)) /* reinit state machine (for error cases)*/
        {
            perRxState = gPerRxStateInit_c;
            pNotifyAppThread();
        }
        if(gCtEvtSelfEvent_c == evType) /* print statistics for PER test */
        {

            i8AverageRssi = u16ReceivedPackets > 0 ?
                i32RssiSum/u16ReceivedPackets: 0;

            PrintPerRxFinal(u16ReceivedPackets,
                            u16TotalPackets,
                            i8AverageRssi,
                            mAppSerId);
            Serial_Print(mAppSerId,cu8PerRxMessages[4],gAllowToBlock_d);
        }
        break;
    default:
        break;
    }
    return bReturnFromSM;
}

/*! *********************************************************************************
* \brief  Handles the Packet error rate TX test
********************************************************************************** */
static bool_t CT_PacketErrorRateTx(ct_event_t evType,void* pAssociatedValue,bool_t bReInit)
{
    const uint16_t u16TotalPacketsOptions[] = {1,25,100,500,1000,2000,5000,10000,65535};

    static ct_per_tx_states_t perTxState = gPerTxStateInit_c;
    static uint32_t miliSecDelay;

    static uint16_t u16TotalPackets;
    static uint16_t u16SentPackets;

    uint16_t buffLen = 0;
    bool_t bReturnFromSM = FALSE;

    uint8_t u8Index;
    uint8_t u8UartData = 0xFF;

    (void)CT_ApplyPrintConfigParams();

    if(bReInit)
    {
        perTxState = gPerTxStateInit_c;
    }

    if(gCtEvtUart_c == evType)
    {
        u8UartData = *((uint8_t*)pAssociatedValue);
    }
    switch(perTxState)
    {
    case gPerTxStateInit_c:
        PrintMenu(cu8ShortCutsBar, (serial_write_handle_t)g_connWriteHandle);
        PrintMenu(cu8PerTxTestMenu, (serial_write_handle_t)g_connWriteHandle);
        PrintTestParams(gaConfigParams, FALSE, mAppSerId);

        CT_EnableShortcuts(TRUE);

        perTxState = gPerTxStateSelectPacketNum_c;
        miliSecDelay = 0;
        break;
    case gPerTxStateSelectPacketNum_c:
        if(gCtEvtUart_c == evType) /*select number of packets to send*/
        {
            if((u8UartData >= '0') && (u8UartData <= '8'))
            {
                u16TotalPackets = u16TotalPacketsOptions[u8UartData - '0'];
                CT_EnableShortcuts(FALSE);
                Serial_Print(mAppSerId, cu8PerTxMessages[0], gAllowToBlock_d);
                perTxState = gPerTxStateInputPacketDelay_c;
            }
            else if('p' == u8UartData) /*go back to previous menu*/
            {
                perTxState = gPerTxStateInit_c;
                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
            }
        }
        break;
    case gPerTxStateInputPacketDelay_c:
        if(gCtEvtUart_c == evType) /*select delay*/
        {
            if(u8UartData == '\r')
            {
                /*approximate limitation due to printing data on the Serial Interface*/
                if(miliSecDelay < 7)
                {
                    Serial_Print(mAppSerId, cu8PerTxMessages[1], gAllowToBlock_d);
                    perTxState = gPerTxStateInit_c;
                    pNotifyAppThread(); /*notify application thread*/
                }
                else
                {
                    perTxState = gPerTxStateStartTest_c;
                    pNotifyAppThread();
                }
            }
            else if((u8UartData >= '0') && (u8UartData <='9'))
            {
                miliSecDelay = miliSecDelay*10 + (u8UartData - '0');
                Serial_PrintDec(mAppSerId, (uint32_t)(u8UartData - '0'));
            }
            else if('p' == u8UartData)
            {
                perTxState = gPerTxStateInit_c;
                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
            }
        }
        break;
    case gPerTxStateStartTest_c:
        miliSecDelay *= 1000; /*convert into microseconds*/
        u16SentPackets = 0;
        gTxPacket.header.lengthField =
            (uint16_t)gaConfigParams[gConfParamPayload].paramValue.decValue;

        gTxPacket.payload[0] = (u16TotalPackets >> 8);
        gTxPacket.payload[1] = (uint8_t)u16TotalPackets;
        gTxPacket.payload[2] = ((u16SentPackets+1) >> 8);
        gTxPacket.payload[3] = (uint8_t)(u16SentPackets+1);
        gTxPacket.payload[4] = gPerOpcode1;
        gTxPacket.payload[5] = gPerOpcode2;

        if(gGenFskMinPayloadLen_c < gaConfigParams[gConfParamPayload].paramValue.decValue)
        {
            for(u8Index = gGenFskMinPayloadLen_c ;
                u8Index < gaConfigParams[gConfParamPayload].paramValue.decValue;
                u8Index++)
            {
                gTxPacket.payload[u8Index] = (u8Index%10)+'0';  /*add some padding*/
            }
        }
        /*pack everything into a buffer*/
        GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
        /*calculate buffer length*/
        buffLen = gTxPacket.header.lengthField+
                    (gGenFskDefaultHeaderSizeBytes_c)+
                        (gGenFskDefaultSyncAddrSize_c + 1);

        if(gGenfskSuccess_c != /*start tx at current time + input delay*/
            GENFSK_StartTx(mAppGenfskId, gTxBuffer, buffLen, 0))
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8PerTxMessages[2], gAllowToBlock_d);
            perTxState = gPerTxStateIdle_c;
        }

        u16SentPackets++;
        Serial_Print(mAppSerId, cu8PerTxMessages[3], gAllowToBlock_d);
        Serial_PrintDec(mAppSerId, (uint32_t)u16TotalPackets);

        perTxState = gPerTxStateRunningTest_c;
        break;
    case gPerTxStateRunningTest_c:
        if(gCtEvtTxDone_c == evType)
        {
            PrintPerTxPackets(u16SentPackets,(gGenfskSuccess == *((genfskEventStatus_t*)pAssociatedValue)),mAppSerId);

            if(u16SentPackets == u16TotalPackets)
            {
                Serial_Print(mAppSerId, cu8PerTxMessages[4], gAllowToBlock_d);
                perTxState = gPerTxStateIdle_c;
            }
            else
            {

                gTxPacket.payload[2] = ((u16SentPackets+1) >> 8);
                gTxPacket.payload[3] = (uint8_t)(u16SentPackets+1);
                /*pack everything into a buffer*/
                GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
                /*calculate buffer length*/
                buffLen = gTxPacket.header.lengthField+
                    (gGenFskDefaultHeaderSizeBytes_c)+
                        (gGenFskDefaultSyncAddrSize_c + 1);

                if(gGenfskSuccess_c !=
                   GENFSK_StartTx(mAppGenfskId, gTxBuffer, buffLen, GENFSK_GetTimestamp() + miliSecDelay))
                {
                    GENFSK_AbortAll();
                    Serial_Print(mAppSerId, cu8PerTxMessages[5], gAllowToBlock_d);
                    perTxState = gPerTxStateIdle_c;
                }

                u16SentPackets++;
            }
        }

        if(gCtEvtUart_c == evType && u8UartData == ' ')
        {
            GENFSK_AbortAll();

            Serial_Print(mAppSerId, cu8PerTxMessages[6],gAllowToBlock_d);
            perTxState = gPerTxStateIdle_c;
        }
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();

            Serial_Print(mAppSerId, cu8PerTxMessages[6],gAllowToBlock_d);
            perTxState = gPerTxStateIdle_c;
        }
        break;
    case gPerTxStateIdle_c:
        if((gCtEvtUart_c == evType) && ('\r' == u8UartData))
        {
            perTxState = gPerTxStateInit_c;
            pNotifyAppThread();
        }
        break;
    default:
        break;
    }

    return bReturnFromSM;
}

/****************************************************************************
*********************************Range test**********************************
****************************************************************************/
bool_t CT_RangeTest(ct_event_t evType, void* pAssociatedValue)
{

    static uint8_t currentTest = 0;
    bool_t bReturnFromSM = FALSE;

    if(gaConfigParams[gConfParamMode].paramValue.stringValue[0] == 'R')
    {
        bReturnFromSM = CT_RangeRx(evType, pAssociatedValue, (currentTest != 'R'));
        currentTest = bReturnFromSM? 0 : 'R';
    }
    else
    {
        bReturnFromSM = CT_RangeTx(evType, pAssociatedValue, currentTest != 'T');
        currentTest = bReturnFromSM? 0 : 'T';
    }
    return bReturnFromSM;
}

/*! *********************************************************************************
* \brief  Handles the Range RX test
********************************************************************************** */
static bool_t CT_RangeRx(ct_event_t evType,void* pAssociatedValue,bool_t bReInit)
{
    static ct_range_rx_states_t rangeRxState = gRangeRxStateInit_c;
    static int32_t i32RssiSum = 0;
    static uint16_t u16ReceivedPackets = 0;

    ct_rx_indication_t* pIndicationInfo = NULL;
    uint8_t u8UartData = 0xFF;
    uint16_t buffLen = 0;
    int8_t i8AverageRssi = 0;
    int8_t i8TempValue = 0;
    bool_t bReturnFromSM = FALSE;
    bool_t bRestartRx = FALSE;

    if(bReInit)
    {
        rangeRxState = gRangeRxStateInit_c;
    }

    if(gCtEvtUart_c == evType)
    {
        u8UartData = *((uint8_t*)pAssociatedValue);
    }

    (void)CT_ApplyPrintConfigParams();

    switch(rangeRxState)
    {
    case gRangeRxStateInit_c:
        i32RssiSum = 0;
        u16ReceivedPackets = 0;

        PrintMenu(cu8ShortCutsBar, (serial_write_handle_t)g_connWriteHandle);
        PrintMenu(cu8RangeRxTestMenu, (serial_write_handle_t)g_connWriteHandle);

        PrintTestParams(gaConfigParams, FALSE,  mAppSerId);

        CT_EnableShortcuts(TRUE);

        gTxPacket.payload[1] = gRangeOpcode1;
        gTxPacket.payload[2] = gRangeOpcode2;
        gTxPacket.payload[3] = gRangeOpcode1 ^ gRangeOpcode2;
        gTxPacket.payload[4] = 255 - gTxPacket.payload[3];
        gTxPacket.payload[5] = 0x00;
        rangeRxState = gRangeRxStateStartTest_c;
        break;
    case gRangeRxStateStartTest_c:
        if(gCtEvtUart_c == evType)
        {
            if(' ' == u8UartData)
            {
                CT_EnableShortcuts(FALSE);

                if(gGenfskSuccess_c ==
                    GENFSK_StartRx(mAppGenfskId,
                                    gRxBuffer,
                                    gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize,
                                    0,
                                    0))
                {
                    Serial_Print(mAppSerId, cu8RangeRxMessages[0], gAllowToBlock_d);
                    rangeRxState = gRangeRxStateRunningTest_c;
                }
                else
                {
                    Serial_Print(mAppSerId, cu8RangeRxMessages[1], gAllowToBlock_d);
                    rangeRxState = gRangeRxStateIdle_c;
                }
            }
            else if('p' == u8UartData)
            {
                rangeRxState = gRangeRxStateInit_c;
                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
            }
        }
        break;
    case gRangeRxStateRunningTest_c:
        if(gCtEvtUart_c == evType &&(' ' == u8UartData)) /*stop test in case [SPACE] is pressed*/
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8RangeRxMessages[2], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ?
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;

            PrintAvgRssi(i8AverageRssi, mAppSerId); /*Print average RSSI*/

            Serial_Print(mAppSerId, cu8RangeRxMessages[3], gAllowToBlock_d);
            rangeRxState = gRangeRxStateIdle_c;
        }
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8RangeRxMessages[2], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ?
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;

            PrintAvgRssi(i8AverageRssi, mAppSerId); /*Print average RSSI*/

            Serial_Print(mAppSerId, cu8RangeRxMessages[3], gAllowToBlock_d);
            rangeRxState = gRangeRxStateIdle_c;
        }
        if(gCtEvtRxDone_c == evType ||
            gCtEvtSeqTimeout_c == evType ||
            gCtEvtRxFailed_c == evType)
        {
            if(gCtEvtRxDone_c == evType) /*if RX is succesfull*/
            {
                pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;
                GENFSK_ByteArrayToPacket(mAppGenfskId, pIndicationInfo->pBuffer, &gRxPacket);
                /*check if Range Test payload type*/
                if(gRxPacket.payload[1] == gRangeOpcode1 &&
                    gRxPacket.payload[2] == gRangeOpcode2 &&
                    gRxPacket.header.lengthField == gGenFskMinPayloadLen_c)
                {
                    gTxPacket.header.lengthField = gGenFskMinPayloadLen_c;
                    /*extract RSSI for the received packet and send it back*/
                    gTxPacket.payload[0] = pIndicationInfo->rssi;

                    /*prepare TX packet and calculate buffer length for the resulting buffer*/
                    GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
                    buffLen = gTxPacket.header.lengthField+
                        (gGenFskDefaultHeaderSizeBytes_c)+
                            (gGenFskDefaultSyncAddrSize_c + 1);
                    if(gGenfskSuccess_c !=
                        GENFSK_StartTx(mAppGenfskId,
                                        gTxBuffer,
                                        buffLen,
                                        GENFSK_GetTimestamp() + 1000))
                    {
                        Serial_Print(mAppSerId, cu8RangeRxMessages[1], gAllowToBlock_d);
                        rangeRxState = gRangeRxStateIdle_c;
                    }
                    else
                    {
                        rangeRxState = gRangeRxStatePrintTestResults_c;
                    }
                }
                else
                {
                    bRestartRx = TRUE;
                }
            }
            else
            {
                bRestartRx = TRUE;
            }
            if(bRestartRx)
            {
                if(gGenfskSuccess_c !=
                    GENFSK_StartRx(mAppGenfskId,
                                    gRxBuffer,
                                    gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize,
                                    0,
                                    0))
                {
                    Serial_Print(mAppSerId, cu8RangeRxMessages[1], gAllowToBlock_d);
                    rangeRxState = gRangeRxStateIdle_c;
                }
            }
        }
        break;
    case gRangeRxStatePrintTestResults_c:
        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8RangeRxMessages[2], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ?
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;

            PrintAvgRssi(i8AverageRssi, mAppSerId); /*Print average RSSI*/

            Serial_Print(mAppSerId, cu8RangeRxMessages[3], gAllowToBlock_d);
            rangeRxState = gRangeRxStateIdle_c;
        }
        if(gCtEvtTxDone_c == evType)
        {
            i8TempValue = (int8_t)(gTxPacket.payload[0]);
            i32RssiSum += i8TempValue;
            u16ReceivedPackets++;
            /*after sending the RSSI to TX device succesfully, print it*/
            PrintCrtRssi(i8TempValue, mAppSerId);


            if(gGenfskSuccess_c !=
               GENFSK_StartRx(mAppGenfskId,
                              gRxBuffer,
                              gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize,
                              0,
                              0))
            {
                Serial_Print(mAppSerId, cu8RangeRxMessages[1], gAllowToBlock_d);
                rangeRxState = gRangeRxStateIdle_c;
            }
            else
            {
                rangeRxState = gRangeRxStateRunningTest_c;
            }
        }
        break;
    case gRangeRxStateIdle_c:
        if((gCtEvtUart_c == evType) &&
           ('\r' == u8UartData))
        {
            rangeRxState = gRangeRxStateInit_c;
            pNotifyAppThread();
        }
        break;
    default:
        break;
    }

    return bReturnFromSM;
}

/*! *********************************************************************************
* \brief  Handles the Range TX test
********************************************************************************** */
static bool_t CT_RangeTx(ct_event_t evType,
                         void* pAssociatedValue,
                         bool_t bReInit)
{
    static ct_range_tx_states_t rangeTxState = gRangeTxStateInit_c;
    static int32_t i32RssiSum = 0;
    static uint16_t u16ReceivedPackets = 0;
    static uint16_t u16PacketsDropped = 0;

    ct_rx_indication_t* pIndicationInfo;
    int8_t  i8AverageRssi;
    int8_t  i8TempValue;
    uint8_t u8UartData = 0xFF;
    uint16_t buffLen = 0;
    bool_t bRestartRx = FALSE;
    bool_t bReturnFromSM = FALSE;

    if(bReInit)
    {
        rangeTxState = gRangeTxStateInit_c;
    }

    if(gCtEvtUart_c == evType)
    {
        u8UartData = *((uint8_t*)pAssociatedValue);
    }

    (void)CT_ApplyPrintConfigParams();

    switch(rangeTxState)
    {
    case gRangeTxStateInit_c:
        i32RssiSum = 0;
        u16ReceivedPackets = 0;
        u16PacketsDropped = 0;

        PrintMenu(cu8ShortCutsBar, (serial_write_handle_t)g_connWriteHandle);
        PrintMenu(cu8RangeTxTestMenu, (serial_write_handle_t)g_connWriteHandle);

        PrintTestParams(gaConfigParams, FALSE, mAppSerId);

        gTxPacket.payload[1] = gRangeOpcode1;
        gTxPacket.payload[2] = gRangeOpcode2;
        gTxPacket.payload[3] = gRangeOpcode1 ^ gRangeOpcode2;
        gTxPacket.payload[4] = 255 - gTxPacket.payload[3];
        gTxPacket.payload[5] = 0x00;

        CT_EnableShortcuts(TRUE);
        rangeTxState = gRangeTxStateStartTest_c;
        break;
    case gRangeTxStateStartTest_c:
        if(gCtEvtUart_c == evType)
        {
            if(' ' == u8UartData)
            {
                CT_EnableShortcuts(FALSE);
                /*set first byte of payload*/
                gTxPacket.payload[0] = 0x00;
                /*set payload length*/
                gTxPacket.header.lengthField = gGenFskMinPayloadLen_c;
                /*convert to a ready-to-send buffer*/
                GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
                /*calculate gTxBuffer length*/
                buffLen = gTxPacket.header.lengthField +
                         (gGenFskDefaultHeaderSizeBytes_c) +
                         (gGenFskDefaultSyncAddrSize_c + 1);

                if(gGenfskSuccess_c == GENFSK_StartTx(mAppGenfskId,
                                                      gTxBuffer,
                                                      buffLen,
                                                      0))
                {
                    Serial_Print(mAppSerId, cu8RangeTxMessages[0], gAllowToBlock_d);
                    rangeTxState = gRangeTxStateRunningTest_c;
                }
                else
                {
                    Serial_Print(mAppSerId, cu8RangeTxMessages[1], gAllowToBlock_d);
                    rangeTxState = gRangeTxStateIdle_c;
                }
            }
            else if('p' == u8UartData)
            {
                rangeTxState = gRangeTxStateInit_c;
                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
            }
        }
        break;
    case gRangeTxStateRunningTest_c:
        if((gCtEvtUart_c == evType) && (' ' == u8UartData))
        {
            GENFSK_AbortAll();

            Serial_Print(mAppSerId, cu8RangeTxMessages[3], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ?
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            /*print average RSSI for the Range test*/
            PrintAvgRssi(i8AverageRssi, mAppSerId);
            /*print number of dropped packets*/
            PrintRangeTxDroppedPackets(u16PacketsDropped, mAppSerId);

            Serial_Print(mAppSerId, cu8RangeTxMessages[4], gAllowToBlock_d);

            rangeTxState = gRangeTxStateIdle_c;
        }

        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();

            Serial_Print(mAppSerId, cu8RangeTxMessages[3], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ?
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            /*print average RSSI for the Range test*/
            PrintAvgRssi(i8AverageRssi, mAppSerId);
            /*print number of dropped packets*/
            PrintRangeTxDroppedPackets(u16PacketsDropped, mAppSerId);

            Serial_Print(mAppSerId, cu8RangeTxMessages[4], gAllowToBlock_d);

            rangeTxState = gRangeTxStateIdle_c;
        }

        if(gCtEvtTxDone_c == evType)
        {
            if(gGenfskSuccess_c !=
                GENFSK_StartRx(mAppGenfskId,
                                gRxBuffer,
                                gGenFskDefaultMaxBufferSize_c,
                                0,
                                10000)) /*wait 10 ms for the response*/
            {
                Serial_Print(mAppSerId, cu8RangeTxMessages[1], gAllowToBlock_d);
                rangeTxState = gRangeTxStateIdle_c;
            }
            else
            {
                rangeTxState = gRangeTxStatePrintTestResults_c;
            }
        }
        break;
    case gRangeTxStatePrintTestResults_c:
        if((gCtEvtUart_c == evType) && (' ' == u8UartData))
        {
            GENFSK_AbortAll();
            
            Serial_Print(mAppSerId, cu8RangeTxMessages[3], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ? 
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            /*print average RSSI for the Range test*/
            PrintAvgRssi(i8AverageRssi, mAppSerId);
            /*print number of dropped packets*/
            PrintRangeTxDroppedPackets(u16PacketsDropped, mAppSerId);
            
            Serial_Print(mAppSerId, cu8RangeTxMessages[4], gAllowToBlock_d);
            
            rangeTxState = gRangeTxStateIdle_c;
        }

        if(gCtEvtKBD_c == evType)
        {
            GENFSK_AbortAll();

            Serial_Print(mAppSerId, cu8RangeTxMessages[3], gAllowToBlock_d);

            i8AverageRssi = u16ReceivedPackets > 0 ?
                (int8_t)(i32RssiSum/u16ReceivedPackets) : 0;
            /*print average RSSI for the Range test*/
            PrintAvgRssi(i8AverageRssi, mAppSerId);
            /*print number of dropped packets*/
            PrintRangeTxDroppedPackets(u16PacketsDropped, mAppSerId);

            Serial_Print(mAppSerId, cu8RangeTxMessages[4], gAllowToBlock_d);

            rangeTxState = gRangeTxStateIdle_c;
        }
        if(gCtEvtRxDone_c == evType ||
            gCtEvtRxFailed_c == evType ||
                gCtEvtSeqTimeout_c == evType)
        {
            if(gCtEvtRxDone_c == evType)
            {
                pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;
                GENFSK_ByteArrayToPacket(mAppGenfskId, pIndicationInfo->pBuffer, &gRxPacket);

                if(gRxPacket.payload[1] == gRangeOpcode1 &&
                    gRxPacket.payload[2] == gRangeOpcode2 &&
                        gRxPacket.header.lengthField == gGenFskMinPayloadLen_c)
                {
                    i8TempValue = (int8_t)(gRxPacket.payload[0]);
                    i32RssiSum += i8TempValue;
                    u16ReceivedPackets++;

                    PrintCrtRssi(i8TempValue, mAppSerId);
                }
                else
                {
                    bRestartRx = TRUE;
                }
            }
            else if(gCtEvtSeqTimeout_c == evType)
            {
                u16PacketsDropped++;
                Serial_Print(mAppSerId, cu8RangeTxMessages[2], gAllowToBlock_d);
            }
            else
            {
                bRestartRx = TRUE;
            }
            if(bRestartRx)
            {
                if(gGenfskSuccess_c !=
                   GENFSK_StartRx(mAppGenfskId,
                                  gRxBuffer,
                                  gGenFskDefaultMaxBufferSize_c,
                                  0,
                                  10000))
                {
                    Serial_Print(mAppSerId, cu8RangeTxMessages[1], gAllowToBlock_d);
                    rangeTxState = gRangeTxStateIdle_c;
                }
            }
            else
            {

                gTxPacket.payload[0] = 0x00;
                gTxPacket.header.lengthField = gGenFskMinPayloadLen_c;
                GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);

                buffLen = gTxPacket.header.lengthField +
                    (gGenFskDefaultHeaderSizeBytes_c) +
                        (gGenFskDefaultSyncAddrSize_c + 1);

                if(gGenfskSuccess_c != GENFSK_StartTx(mAppGenfskId,
                                                      gTxBuffer,
                                                      buffLen,
                                                      GENFSK_GetTimestamp() + 100000))
                {
                    Serial_Print(mAppSerId, cu8RangeTxMessages[1], gAllowToBlock_d);
                    rangeTxState = gRangeTxStateIdle_c;
                }
                else
                {
                    rangeTxState = gRangeTxStateRunningTest_c;
                }
            }
        }
        break;
    case gRangeTxStateIdle_c:
        if((gCtEvtUart_c == evType) &&
           ('\r' == u8UartData))
        {
            rangeTxState = gRangeTxStateInit_c;
            pNotifyAppThread();
        }
        break;
    default:
        break;
    }
    return bReturnFromSM;
}


/****************************************************************************
*****************************Continuous Tests********************************
****************************************************************************/
/*! ************************************************************************
* \brief  Restores RBME configuration for continuous tests
*************************************************************************** */
static void CT_RestoreRbmeConfiguration(bool_t *rbme_rec_required)
{
    if(*rbme_rec_required)
    {
        /*Restore whitener and crc config (RBME parameter) due to XCVR_ChangeMode() API called inside XCVR_DftTxPatternReg() or
        the XCVR_DftTxLfsrReg() APIs*/
        /*set whitener config*/
        GENFSK_SetWhitenerConfig(mAppGenfskId, &whitenConfig);
        /*set crc config*/
        GENFSK_SetCrcConfig(mAppGenfskId, &crcConfig);
        *rbme_rec_required = FALSE;
    }
}

/*! ************************************************************************
* \brief  Handles continuous tests
*************************************************************************** */
bool_t CT_ContinuousTests(ct_event_t evType, void* pAssociatedValue)
{
    static ct_cont_tests_states_t contTestState = gContStateInit_c;
    static ct_cont_tests_states_t prevState = gContStateInvalid_c;
    const xcvr_coding_config_t *rbme_config = &xcvr_ble_uncoded_config;
    ct_rx_indication_t* pIndicationInfo = NULL;
    uint32_t u32RandomValue = 0;
    uint16_t buffLen = 0;
    uint16_t u16Idx = 0;
    uint8_t u8ByteCount = 0;
    uint8_t u8UartData = 0xFF;

    bool_t bRestartRx = FALSE;
    bool_t bReturnFromSM = FALSE;
    xcvr_config_t xcvr_config;
    const xcvr_config_t *p_xcvr_config = &xcvr_config;
    xcvr_currConfig_t currConfig;
    xcvr_currConfig_t * p_currConfig = &currConfig;
    uint8_t chanNum;
    genfskRadioMode_t radioModeIn;
    genfskDataRate_t  dataRate;

    static bool_t is_rbme_recovery_required = FALSE;

    if(gCtEvtUart_c == evType)
    {
        u8UartData = *((uint8_t*)pAssociatedValue);//获取空中包数据
    }

    /*if continuous tx test go through idle so that channel and tx power can
        be changed during test*/
    if((gContStateRunModTxOne_c <= prevState &&
        gContStateRunUnmodTx_c >= prevState) ||
        gContStateRunIdle_c == prevState)
    {
        XCVR_DftTxOff();
        /* Remember continuous mode has been used (XCVR_ChangeMode() API called) */
        is_rbme_recovery_required = TRUE;

        if('1' <= u8UartData && '8' >= u8UartData)
        {
            /* Do not print out RF params yet but postpone print
            when sub-menu state is reached (gContStateRunModTxOne_c, ...) */
        }
        else
        {
            if(CT_ApplyPrintConfigParams())
            {
                contTestState = prevState;
            }
        }
    }
    else
    {
        if('1' <= u8UartData && '8' >= u8UartData)
        {
            /* Do not print out RF params yet but postpone print
            when sub-menu state is reached (gContStateRunModTxOne_c, ...) */
        }
        else
        {
            (void)CT_ApplyPrintConfigParams();
        }
    }

    chanNum  = GENFSK_GetChannelNumber(mAppGenfskId);

    /* If packet mode is used and continuous mode was used previously, restore whitener and crc config
        (RBME parameter) due to XCVR_ChangeMode() API called inside XCVR_DftTxPatternReg() or the XCVR_DftTxLfsrReg() APIs*/
    if ((contTestState == gContStateRunPRBS_c) || (contTestState == gContStateRunRx_c) ||
        (contTestState == gContStateRunEd_c))
    {
        CT_RestoreRbmeConfiguration(&is_rbme_recovery_required);
    }


    switch(contTestState)
    {
    case gContStateInit_c:
        PrintMenu(cu8ShortCutsBar, (serial_write_handle_t)g_connWriteHandle);
        PrintMenu(cu8ContinuousTestMenu, (serial_write_handle_t)g_connWriteHandle);
        PrintTestParams(gaConfigParams, FALSE, mAppSerId);
        CT_EnableShortcuts(TRUE);
        contTestState = gContStateSelectTest_c;
        #if (APP_SNIFFER_EN)
        u8UartData = '7';//直接进入监听模式
        evType = gCtEvtUart_c; 
        #else
        break;
        #endif

    case gContStateSelectTest_c:
        if(gCtEvtUart_c == evType)
        {
            if('1' <= u8UartData && '8' >= u8UartData)
            {
                contTestState = (ct_cont_tests_states_t)((u8UartData - '1') + gContStateRunIdle_c);
                prevState = contTestState;
                pNotifyAppThread();
            }
            else if('p' == u8UartData)
            {
                /* When leaving continuous mode, restore whitener and crc config (RBME parameter)
                to be used for next tests (continous packet tests, PER tests or Range tests. */
                CT_RestoreRbmeConfiguration(&is_rbme_recovery_required);

                CT_EnableShortcuts(TRUE);
                bReturnFromSM = TRUE;
                prevState     = gContStateInvalid_c;
                contTestState = gContStateInit_c;
            }
        }
        break;

    case gContStateRunIdle_c:
        Serial_Print(mAppSerId, cu8ContMessages[7], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;

    case gContStateRunPRBS_c:
        if(gCtEvtTxDone_c == evType ||   /* if packet is sent */
           gCtEvtSelfEvent_c == evType) /* if first run */
        {
            if(gCtEvtSelfEvent_c == evType)
            {
                CT_EnableShortcuts(FALSE);
                Serial_Print(mAppSerId, cu8ContMessages[0], gAllowToBlock_d);
            }
            /*set payload length with user set value*/
            gTxPacket.header.lengthField = gaConfigParams[gConfParamPayload].paramValue.decValue;
            /*calculate buffer length*/
            buffLen = gTxPacket.header.lengthField+
                (gGenFskDefaultHeaderSizeBytes_c)+
                    (gGenFskDefaultSyncAddrSize_c + 1);
            /*fill the payload*/
            for(u16Idx = 0; u16Idx < gTxPacket.header.lengthField; u16Idx++)
            {
                if(u8ByteCount == 0)
                {
                    RNG_GetRandomNo(&u32RandomValue); /*get 4 bytes from RNG*/
                    u8ByteCount = 4;
                }
                --u8ByteCount;
                gTxPacket.payload[u16Idx] = (uint8_t)(u32RandomValue >> (u8ByteCount << 3));
            }
            /*convert the packet to generic fsk formatted byte array*/
            GENFSK_PacketToByteArray(mAppGenfskId, &gTxPacket, gTxBuffer);
            if(gGenfskSuccess_c != GENFSK_StartTx(mAppGenfskId, gTxBuffer, buffLen, 0))
            {
                Serial_Print(mAppSerId, cu8ContMessages[1], gAllowToBlock_d);
                contTestState = gContStateIdle_c;
            }
        }
        if(gCtEvtUart_c == evType && ' ' == u8UartData)
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8ContMessages[2], gAllowToBlock_d);
            contTestState = gContStateIdle_c;
        }
        break;

    case gContStateRunRx_c:
        if(gCtEvtUart_c == evType && (' ' == u8UartData))
        {
            GENFSK_AbortAll();
            Serial_Print(mAppSerId, cu8ContMessages[5], gAllowToBlock_d);
            contTestState = gContStateIdle_c;
        }

    #if (APP_SNIFFER_EN)
        app_sniffer_handle(evType,pAssociatedValue,&contTestState);
    #else
        if(gCtEvtRxDone_c == evType)
        {
            pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;
            Serial_Print(mAppSerId, cu8ContMessages[6], gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, pIndicationInfo->bufferLength);
            Serial_Print(mAppSerId, " bytes: ", gAllowToBlock_d);

            if( pIndicationInfo->bufferLength <= 50 )
            {//打印有长度限制
                for(uint16_t i=0; i<pIndicationInfo->bufferLength; i++)
                {
                    (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii(pIndicationInfo->pBuffer[i]), 2U);
                }
            }
            else
            {
                const uint16_t length = 25U;
                for(uint16_t i=0; i<length; i++)
                {
                    (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii(pIndicationInfo->pBuffer[i]), 2U);
                }
                Serial_Print(mAppSerId, "...", gAllowToBlock_d);
                for(uint16_t i=0; i<length; i++)
                {
                    (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii(pIndicationInfo->pBuffer[pIndicationInfo->bufferLength-length+i]), 2U);
                }
            }

            // PrintPerRxInfo(u16PacketsIndex, u16ReceivedPackets, pIndicationInfo, mAppSerId);

            int32_t i32TempRssiValue = 0;//使用SerialManager打印有符号数据转换操作
            i32TempRssiValue = (int32_t)(pIndicationInfo->rssi);
            if(i32TempRssiValue < 0)
            {
                i32TempRssiValue *= -1;
            }
            Serial_Print(mAppSerId, "\r\n------------------------rssi:-", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, i32TempRssiValue);
            // Serial_Print(mAppSerId, ",crc: 0x", gAllowToBlock_d);
            // Serial_PrintHex(mAppSerId, (uint8_t*)&pIndicationInfo->crcValid, 1U, gPrtHexaBigEndian_c);
        //    Serial_Print(mAppSerId, ",timestamp: ", gAllowToBlock_d);
        //    Serial_PrintDec(mAppSerId, pIndicationInfo->timestamp);

            // PRINTF("\r\nrssi:%d,crc:0x%x,timestamp:%d\r\n",pIndicationInfo->rssi,pIndicationInfo->crcValid,pIndicationInfo->timestamp);//id error handle

            bRestartRx = TRUE;
        }
        else if(gCtEvtRxFailed_c == evType || gCtEvtSeqTimeout_c == evType)
        {
            GENFSK_AbortAll();
            bRestartRx = TRUE;
        }

        if(gCtEvtSelfEvent_c == evType)
        {
            CT_EnableShortcuts(FALSE);
            Serial_Print(mAppSerId, cu8ContMessages[3], gAllowToBlock_d);
            bRestartRx = TRUE;
        }

        if(bRestartRx)
        {
            if(gGenfskSuccess_c !=
                GENFSK_StartRx(mAppGenfskId,
                                gRxBuffer,
                                gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize,
                                0,
                                0))
            {
                Serial_Print(mAppSerId, cu8ContMessages[4], gAllowToBlock_d);
                contTestState = gContStateIdle_c;
            }
        }
    #endif
    break;

    case gContStateRunEd_c:
        if(gCtEvtSelfEvent_c == evType)
        {
            CT_EnableShortcuts(FALSE);
            (void)TM_InstallCallback((timer_handle_t)mAppTmrId, pTmrCallback, NULL);
            (void)TM_Start((timer_handle_t)mAppTmrId, kTimerModeIntervalTimer, 50);
        }
        if(gCtEvtUart_c == evType &&(' ' == u8UartData))
        {
            (void)TM_Stop((timer_handle_t)mAppTmrId);

            Serial_Print(mAppSerId, cu8ContMessages[12], gAllowToBlock_d);
            contTestState = gContStateIdle_c;
        }
        if(gCtEvtTimerExpired_c == evType)
        {
            PrintCrtRssi((int8_t)XCVR_GetInstantRssi(), mAppSerId);

        }
        break;

    case gContStateRunModTxOne_c:
        XCVR_GetCurrentConfig(p_currConfig);
        CT_WrapperRadioModeAndDataRate(p_currConfig, &radioModeIn, &dataRate);
        GENFSK_GetXcvrConfig(radioModeIn, dataRate, &p_xcvr_config);
        GENFSK_BackupXcvrSetting();
        XCVR_DftTxPatternReg(chanNum, &p_xcvr_config, &rbme_config,0xFFFFFFFFU);
        GENFSK_RestoreXcvrSetting();

        Serial_Print(mAppSerId, cu8ContMessages[8], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;
    case gContStateRunModTxZero_c:
        XCVR_GetCurrentConfig(p_currConfig);
        CT_WrapperRadioModeAndDataRate(p_currConfig, &radioModeIn, &dataRate);
        GENFSK_GetXcvrConfig(radioModeIn, dataRate, &p_xcvr_config);
        GENFSK_BackupXcvrSetting();
        XCVR_DftTxPatternReg(chanNum, &p_xcvr_config, &rbme_config, 0x00000000);
        GENFSK_RestoreXcvrSetting();

        Serial_Print(mAppSerId, cu8ContMessages[9], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;
    case gContStateRunModTxPN_c:
        XCVR_GetCurrentConfig(p_currConfig);
        CT_WrapperRadioModeAndDataRate(p_currConfig, &radioModeIn, &dataRate);
        GENFSK_GetXcvrConfig(radioModeIn, dataRate, &p_xcvr_config);
        GENFSK_BackupXcvrSetting();
        XCVR_DftTxLfsrReg(chanNum, &p_xcvr_config, &rbme_config, 3U /* arbitrary choice */);
        GENFSK_RestoreXcvrSetting();

        Serial_Print(mAppSerId, cu8ContMessages[10], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;

    case gContStateRunUnmodTx_c:
    {
        uint32_t rf_freq = (2360U + chanNum)*1000000U; /* In Hertz */
        GENFSK_BackupXcvrSetting();
        XCVR_DftTxCW(rf_freq);
        GENFSK_RestoreXcvrSetting();

        Serial_Print(mAppSerId, cu8ContMessages[11], gAllowToBlock_d);
        contTestState = gContStateSelectTest_c;
        break;
    }


    case gContStateIdle_c:
        if(gCtEvtUart_c == evType &&('\r' == u8UartData))
        {
            contTestState = gContStateInit_c;
            pNotifyAppThread();
        }
    default:
        break;
    }

    return bReturnFromSM;
}

/*! *********************************************************************************
* \brief  Updates the corresponding parameter depending on key pressed
********************************************************************************** */
bool_t CT_UpdateShortcutKeyParam(uint8_t u8PressedKey)
{
    ct_config_params_t* pConfig;
    bool_t bReturnFlag = TRUE;

    switch(u8PressedKey)
    {
    case 't'://执行tx操作
        pConfig = &gaConfigParams[gConfParamMode];
        pConfig->paramValue.stringValue[0] = 'T';
        break;
    case 'r'://启动rx
        pConfig = &gaConfigParams[gConfParamMode];
        pConfig->paramValue.stringValue[0] = 'R';
        break;
    case 'q'://更新信道——上升
        pConfig = &gaConfigParams[gConfParamChannel];
        if(pConfig->paramValue.decValue == gGenFskMaxChannel_c)
        {
            pConfig->paramValue.decValue = gGenFskMinChannel_c;
        }
        else
        {
            pConfig->paramValue.decValue++;
        }
        break;
    case 'w'://更新信道——下降
        pConfig = &gaConfigParams[gConfParamChannel];
        if(pConfig->paramValue.decValue == gGenFskMinChannel_c)
        {
            pConfig->paramValue.decValue = gGenFskMaxChannel_c;
        }
        else
        {
            pConfig->paramValue.decValue--;
        }
        break;
    case 'a'://上电
        pConfig = &gaConfigParams[gConfParamPower];
        pConfig->paramValue.decValue++;
        if(gGenFskMaxTxPowerLevel_c < pConfig->paramValue.decValue)
        {
            pConfig->paramValue.decValue = gGenFskMinTxPowerLevel_c;
        }
        CT_HandleHighLowPower();
        break;
    case 's'://断电
        pConfig = &gaConfigParams[gConfParamPower];
        if( pConfig->paramValue.decValue == gGenFskMinTxPowerLevel_c)
        {
            pConfig->paramValue.decValue = gGenFskMaxTxPowerLevel_c;
        }
        else
        {
            pConfig->paramValue.decValue--;
        }
        CT_HandleHighLowPower();
        break;
    case 'n'://净荷数据长度——增加
        pConfig = &gaConfigParams[gConfParamPayload];
        pConfig->paramValue.decValue++;
        if(gGenFskMaxPayloadLen_c < pConfig->paramValue.decValue)
        {
            pConfig->paramValue.decValue = gGenFskMinPayloadLen_c;
        }
        break;
    case 'm'://净荷数据长度——减少
        pConfig = &gaConfigParams[gConfParamPayload];
        pConfig->paramValue.decValue--;
        if(gGenFskMinPayloadLen_c > pConfig->paramValue.decValue)
        {
            pConfig->paramValue.decValue = gGenFskMaxPayloadLen_c;
        }
        break;
    case 'd'://XTAL Trim——增加
        pConfig = &gaConfigParams[gConfParamXtalTrim];
        if(pConfig->paramValue.decValue == gMaxTrimValue_c)
        {
            pConfig->paramValue.decValue = gMinTrimValue_c;
        }
        else
        {
            pConfig->paramValue.decValue++;
        }
        break;
    case 'f'://XTAL Trim——减少
        pConfig = &gaConfigParams[gConfParamXtalTrim];
        if( pConfig->paramValue.decValue == gMinTrimValue_c)
        {
            pConfig->paramValue.decValue = gMaxTrimValue_c;
        }
        else
        {
            pConfig->paramValue.decValue--;
        }
        break;
    case 'z'://速率——增加
        pConfig = &gaConfigParams[gConfParamRateMode];
        if(pConfig->id == gMaxRate_c)
        {
            pConfig->id = gMinRate_c;
        }
        else
        {
            pConfig->id++;
        }
        break;
    case 'x'://速率——减少
        pConfig = &gaConfigParams[gConfParamRateMode];
        if( pConfig->id == gMinRate_c)
        {
            pConfig->id = gMaxRate_c;
        }
        else
        {
            pConfig->id--;
        }
        break;
    case 'c'://ble模式修改数据白化参数
        pConfig = &gaConfigParams[gConfParamWhitenMode];
        pConfig->id++;
        pConfig->id = pConfig->id%gMaxWhitenMode_c;
        break;
    case '\r':
        /* Print configuration parameters. */
        break;

    default:
        bReturnFlag = FALSE;
        break;
    };

    return bReturnFlag;
}

/*! *********************************************************************************
* \brief  Checks if shortcuts can be used
********************************************************************************** */
bool_t CT_IsShortcutMenuEnabled(void)
{
    return bEnableShortcuts;
}

/*! *********************************************************************************
* \brief  Handles the activation/deactivation of low/high power trasnmit levels
********************************************************************************** */
static void CT_HandleHighLowPower(void)
{
    genfskStatus_t status;

    if (GENFSK_IsHighPowerConfigured() == 0x1U)
    {
        if (gaConfigParams[gConfParamPower].paramValue.decValue > gGenFskLastLowPowerIdx_c)
        {
            gaConfigParams[gConfParamPower].paramValue.decValue -= gGenFskLastLowPowerIdx_c + 1U;
        }
        else
        {
            gaConfigParams[gConfParamPower].paramValue.decValue += gGenFskLastLowPowerIdx_c + 1U;
        }
    }
    /* Enable high power TX */
    if (gaConfigParams[gConfParamPower].paramValue.decValue > gGenFskLastLowPowerIdx_c)
    {
        gaConfigParams[gConfParamPower].paramValue.decValue -= gGenFskLastLowPowerIdx_c + 1U;
        status = GENFSK_ConfigurePower(1U);
        if (status == gGenfskSuccess_c)
        {
            FLib_MemCpy(gaConfigParams[gConfParamPower].paramName, "Hi Power", 9);
        }
    }
    else /* Enable low power TX */
    {
        status = GENFSK_ConfigurePower(0U);
        if (status == gGenfskSuccess_c)
        {
            FLib_MemCpy(gaConfigParams[gConfParamPower].paramName,"Power", 6);
        }
    }
}

/*! *********************************************************************************
* \brief  Enables / Disables the shortcut keys
********************************************************************************** */
void CT_EnableShortcuts(bool_t bEnable)
{
    bEnableShortcuts = bEnable;
}

/*! *********************************************************************************
* \brief  Calls Generic FSK api to set mode and rate and prints
*         the shortcuts values
********************************************************************************** */
static bool_t CT_ApplyPrintRateParams(void)
{
    bool_t bParamsUpdated = FALSE;

#if defined(RADIO_IS_GEN_3P5)  || defined(RADIO_IS_GEN_4P0) || defined(RADIO_IS_GEN_4P5)
    static const xcvr_coding_config_t *old_rbmeConfig = &xcvr_ble_uncoded_config;
    const xcvr_coding_config_t *rbmeConfig = &xcvr_ble_uncoded_config;
    uint8_t preamble_pattern    = 0U;
#endif
    GENFSK_mode_config_t modeConfig;
    GENFSK_packet_config_t packetConfig;
    uint8_t crtMode             = 0U;
    uint16_t preamble_size      = 0U;
    uint8_t lengthSizeBits      = gGenFskDefaultLengthFieldSize_c;
    uint8_t h1SizeBits          = gGenFskDefaultH1FieldSize_c;
    uint16_t h1Mask             = gGenFskDefaultH1Mask_c;

    (void)GENFSK_GetModeConfig(mAppGenfskId, &modeConfig);
    (void)GENFSK_GetPacketConfig(mAppGenfskId, &packetConfig);

    if (old_rbmeConfig == &xcvr_ble_coded_s2_config)
    {
        crtMode = gRateBle500Kbps_c;
    }
    else if (old_rbmeConfig == &xcvr_ble_coded_s8_config)
    {
        crtMode = gRateBle125Kbps_c;
    }
    else
    {
        crtMode = crtRate[radioConfig.dataRate];
        if ((modeConfig.mode == gGenfskBleLrMode) || (modeConfig.mode == gGenfskBleUncodedMode))
        {
            crtMode += gRateBle125Kbps_c;
        }
    }

    if(gaConfigParams[gConfParamRateMode].id != crtMode)
    {
        /* Determine mode */
        genfskMode_t        mode = gGenfskGllMode;

        if (gaConfigParams[gConfParamRateMode].id <= gMaxRateGenfsk_c)
        {
            mode = gGenfskGllMode;
        }
        else if (gaConfigParams[gConfParamRateMode].id <= gRateBle500Kbps_c)
        {
            mode = gGenfskBleLrMode;
            preamble_size = 9U;
            preamble_pattern = 0x3C;
            lengthSizeBits = 8U;
            h1SizeBits     = 0U;
            h1Mask         = 0U;
            if (gaConfigParams[gConfParamRateMode].id == gRateBle500Kbps_c)
            {
                rbmeConfig = &xcvr_ble_coded_s2_config;
            }
            else
            {
                rbmeConfig = &xcvr_ble_coded_s8_config;
            }
        }
        else
        {
            mode = gGenfskBleUncodedMode;

            if (gaConfigParams[gConfParamRateMode].id == gRateBle1Mbps_c)
            {
                preamble_size = 0U;
            }
            else
            {
                preamble_size = 1U;
            }
            preamble_pattern = 0U;
            lengthSizeBits = 8U;
            h1SizeBits     = 0U;
            h1Mask         = 0U;
        }

        /* Determine rate */
        genfskDataRate_t    rate = (genfskDataRate_t)targetRate[gaConfigParams[gConfParamRateMode].id];

        /* Mode change detected */
        if (mode != modeConfig.mode)
        {
            genfskMode_t old_mode = mModeConfig.mode;
            mModeConfig.mode = mode;
            if(gGenfskSuccess_c != GENFSK_SetModeConfig(mAppGenfskId, &mModeConfig))
            {
                gaConfigParams[gConfParamRateMode].id = crtMode;
                mModeConfig.mode = old_mode;
            }
            else
            {
                bParamsUpdated = TRUE;
            }
            {
                /* Whitening specific configuration (withen init depending on channel) only applies to BLE */
                if ((mModeConfig.mode == gGenfskBleLrMode) || (mModeConfig.mode == gGenfskBleUncodedMode))
                {
                    GENFSK_SetBleWhitenInit(mAppGenfskId, GENFSK_GetChannelNumber(mAppGenfskId));
                }
                else
                {
                    /* Restore GENFSK whitening configuration */
                    GENFSK_SetWhitenerConfig(mAppGenfskId, &whitenConfig);
                }
            }
        }

        /* Rate change detected or rbme configuration changed*/
        if ((rate != radioConfig.dataRate) || (old_rbmeConfig != rbmeConfig))
        {
            genfskDataRate_t old_rate = radioConfig.dataRate;
            radioConfig.dataRate = rate;
            if(gGenfskSuccess_c != GENFSK_RadioConfigWithRbme(mAppGenfskId, &radioConfig, &rbmeConfig))
            {
                gaConfigParams[gConfParamRateMode].id = crtMode;
                radioConfig.dataRate = old_rate;
            }
            else
            {
                GENFSK_whitener_config_t whitenerUpdatedConfig;

                /* Reads the current whitening configuration (as the whitening init may have been updated previously due to a channel update) */
                GENFSK_GetWhitenerConfig(mAppGenfskId, &whitenerUpdatedConfig);
                /*Restore whitener and crc config (RBME parameter) due to XCVR_ChangeMode()*/
                /*set whitener config*/
                GENFSK_SetWhitenerConfig(mAppGenfskId, &whitenerUpdatedConfig);
                /*set crc config*/
                GENFSK_SetCrcConfig(mAppGenfskId, &crcConfig);
                /* Workaround to set SPOV_EN=1 for S2 */
                if (rbmeConfig == &xcvr_ble_coded_s2_config)
                {
                    RBME->SPREAD_CFG |= RBME_SPREAD_CFG_SPOV_EN(1);
                }
                bParamsUpdated = TRUE;
            }
        }
        old_rbmeConfig = rbmeConfig;

        if ((packetConfig.preambleSizeBytes != preamble_size) || (packetConfig.preambleBytePattern != preamble_pattern) ||
            (packetConfig.lengthSizeBits != lengthSizeBits) || (packetConfig.h1SizeBits != h1SizeBits) ||
            (packetConfig.h1Mask != h1Mask))
        {
            GENFSK_packet_config_t old_packetConfig = packetConfig;
            packetConfig.preambleSizeBytes      = preamble_size;
            packetConfig.preambleBytePattern    = preamble_pattern;
            packetConfig.lengthSizeBits         = lengthSizeBits;
            packetConfig.h1SizeBits             = h1SizeBits;
            packetConfig.h1Mask                 = h1Mask;

            if(gGenfskSuccess_c != GENFSK_SetPacketConfig(mAppGenfskId, &packetConfig))
            {
                gaConfigParams[gConfParamRateMode].id = crtMode;
                packetConfig = old_packetConfig;
            }
            else
            {
                bParamsUpdated = TRUE;
            }
        }
    }

    return bParamsUpdated;
}
/*! *********************************************************************************
* \brief  Calls Generic FSK api to set channel and tx power level and prints
*         the shortcuts values
********************************************************************************** */
static bool_t CT_ApplyPrintConfigParams(void)
{
    bool_t bParamsUpdated = FALSE;
    static genfskMode_t old_mode = gGenfskGllMode;
    static uint8_t old_whiten_mode = gDefaultWhitenMode_c;
    bool_t update_whitening = false;
    if(CT_IsShortcutMenuEnabled())
    {
        uint8_t crtChannel = GENFSK_GetChannelNumber(mAppGenfskId);
        uint8_t crtPwr     = GENFSK_GetTxPowerLevel(mAppGenfskId);

        bParamsUpdated = CT_ApplyPrintRateParams();

        if(gaConfigParams[gConfParamChannel].paramValue.decValue != crtChannel)
        {
            if(gGenfskSuccess_c !=
                GENFSK_SetChannelNumber(mAppGenfskId, (uint8_t)gaConfigParams[gConfParamChannel].paramValue.decValue))
            {
                gaConfigParams[gConfParamChannel].paramValue.decValue = crtChannel;
            }
            else
            {
                bParamsUpdated = TRUE;
                update_whitening = true;
            }
        }

        GENFSK_mode_config_t modeConfig;

        (void)GENFSK_GetModeConfig(mAppGenfskId, &modeConfig);
        /* Upon mode change (BLE/GENFSK) or whiten mode change */
        if ((old_mode != modeConfig.mode) || (old_whiten_mode != gaConfigParams[gConfParamWhitenMode].id))
        {
            update_whitening = true;
        }
        /* Whitening specific configuration (withen init depending on channel) only applies to BLE */
        if (update_whitening)
        {
            if ((modeConfig.mode == gGenfskBleLrMode) || (modeConfig.mode == gGenfskBleUncodedMode))
            {
                uint8_t channelNum = gaConfigParams[gConfParamChannel].paramValue.decValue;
                if (gaConfigParams[gConfParamWhitenMode].id == gWhitenFixed_c)
                {
                    /* If fixed whitening is selected always use the whitening configuration for BLE channel 37
                    (corresponds to 42 in genfsk). */
                    channelNum = 42U;
                }
                GENFSK_SetBleWhitenInit(mAppGenfskId, channelNum);
            }
            update_whitening = false;
        }
        old_mode = modeConfig.mode;
        old_whiten_mode = gaConfigParams[gConfParamWhitenMode].id;

        if(gaConfigParams[gConfParamPower].paramValue.decValue != crtPwr)
        {
            genfskStatus_t status;

            if (GENFSK_IsHighPowerConfigured() == 0x1U)
            {
                if (gaConfigParams[gConfParamPower].paramValue.decValue > gGenFskLastLowPowerIdx_c)
                {
                    gaConfigParams[gConfParamPower].paramValue.decValue -= gGenFskLastLowPowerIdx_c + 1U;
                }
                else
                {
                    gaConfigParams[gConfParamPower].paramValue.decValue += gGenFskLastLowPowerIdx_c + 1U;
                }
            }
            /* Enable high power TX */
            if (gaConfigParams[gConfParamPower].paramValue.decValue > gGenFskLastLowPowerIdx_c)
            {
                gaConfigParams[gConfParamPower].paramValue.decValue -= gGenFskLastLowPowerIdx_c + 1U;
                status = GENFSK_ConfigurePower(1U);
                if (status == gGenfskSuccess_c)
                {
                    FLib_MemCpy(gaConfigParams[gConfParamPower].paramName, "Hi Power", 9);
                }
            }
            else /* Enable low power TX */
            {
                status = GENFSK_ConfigurePower(0U);
                if (status == gGenfskSuccess_c)
                {
                   FLib_MemCpy(gaConfigParams[gConfParamPower].paramName,"Power", 6);
                }
            }
            if(gGenfskSuccess_c !=
               GENFSK_SetTxPowerLevel(mAppGenfskId, gaConfigParams[gConfParamPower].paramValue.decValue))
            {
                gaConfigParams[gConfParamPower].paramValue.decValue = crtPwr;
            }
            else
            {
                bParamsUpdated = TRUE;
            }
        }

        if(gaConfigParams[gConfParamXtalTrim].paramValue.decValue != xtalTrim)
        {
            if( GENFSK_SetXtalTrim(gaConfigParams[gConfParamXtalTrim].paramValue.decValue) != gGenfskSuccess_c )
            {
                gaConfigParams[gConfParamXtalTrim].paramValue.decValue = xtalTrim;
            }
            else
            {
                xtalTrim = gaConfigParams[gConfParamXtalTrim].paramValue.decValue;
                bParamsUpdated = TRUE;
            }
        }

        PrintTestParams(gaConfigParams, TRUE, mAppSerId);
    }
    return bParamsUpdated;
}
/*! *********************************************************************************
* \brief  Converts radio_mode_t to genfskRadioMode_t & data_rate_t to genfskDataRate_t
*         This is used to provide the right types to GENFSK_GetXcvrConfig() after a
*         call to XCVR_GetCurrentConfig().
********************************************************************************** */
static void CT_WrapperRadioModeAndDataRate(xcvr_currConfig_t * pCurrConfig, genfskRadioMode_t* pRadioModeIn, genfskDataRate_t*  pDataRate)
{
    *pRadioModeIn = gGenfskGfskBt0p5h0p5;
    *pDataRate = gGenfskDR1Mbps;


    if ((pCurrConfig->radio_mode >= GFSK_BT_0p5_h_0p5) && (pCurrConfig->radio_mode <= MSK))
    {
        *pRadioModeIn = (genfskRadioMode_t)((uint8_t)pCurrConfig->radio_mode - GFSK_BT_0p5_h_0p5);
    }
    else
    {
        /* Conversion cannot be done */
        assert(0U);
    }
    if (pCurrConfig->data_rate <= DR_2MBPS)
    {
        *pDataRate = (genfskDataRate_t)(pCurrConfig->data_rate);
    }
    else
    {
        /* Conversion cannot be done */
        assert(0U);
    }
}

/********************************************************************************
*****************************Adjust RTC XTAL Menu********************************
********************************************************************************/
bool_t CT_TrimAdjust(ct_event_t evType, void* pAssociatedValue)
{
    bool_t bReturnFromSM = FALSE;

#ifndef RTC_TRIM_DISABLED
    static ct_trim_test_states_t trimStateTest = gTrimStateInit_c;
    uint8_t u8UartData = 0xFF;
    static uint8_t aTrimValue;
    uint16_t CxtalValue;
    uint16_t CextalValue;
    uint16_t CxtalRegValue;
    uint16_t CextalRegValue;

    switch (trimStateTest)
    {
        case gTrimStateInit_c:
        {
            /* Print Menu, Stop Flash All LEDs and configure PTB3 for frequency measurement */
            PrintMenu(cu8RTCTrimMenu, (serial_write_handle_t)g_connWriteHandle);
            //LED_StopFlashingAllLeds();
            //LED_UnInit();

            /* init 32k osc */
            PLATFORM_InitOsc32K();
            /* output 32k clock on ptc7 */
            CLOCK_EnableClock(kCLOCK_PortC);
            PORT_SetPinMux(PORTC, 7u, kPORT_MuxAlt7);
            SCG0->CLKOUTCNFG = SCG_CLKOUTCNFG_CLKOUTSEL(0x4);

            /* compute Cload value */
            CxtalRegValue  = (CCM32K->OSC32K_CTRL & CCM32K_OSC32K_CTRL_XTAL_CAP_SEL_MASK) >> CCM32K_OSC32K_CTRL_XTAL_CAP_SEL_SHIFT;
            CextalRegValue = (CCM32K->OSC32K_CTRL & CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL_MASK) >> CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL_SHIFT;
            CxtalValue     = CxtalRegValue * 2U; // in pF
            CextalValue    = CextalRegValue * 2U;
            aTrimValue     = (CxtalValue * CextalValue)/(CxtalValue + CextalValue)  + CCM32K_CSHUNT_VALUE;

            /* Print Trim Value */
            Serial_Print(mAppSerId, "\rCxtal: ", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, CxtalValue);
            Serial_Print(mAppSerId, " pF ", gAllowToBlock_d);
            Serial_Print(mAppSerId, "Cextal: ", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, CextalValue);
            Serial_Print(mAppSerId, " pF ", gAllowToBlock_d);
            Serial_Print(mAppSerId, "Cload: ", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, aTrimValue);
            Serial_Print(mAppSerId, " pF ", gAllowToBlock_d);
            /* Move to the next state */
            trimStateTest = gTrimStateRun_c;
        }
        break;

        case gTrimStateRun_c:
        {
            if(gCtEvtUart_c == evType)
            {
                u8UartData = *((uint8_t*)pAssociatedValue);
                CxtalRegValue  = (CCM32K->OSC32K_CTRL & CCM32K_OSC32K_CTRL_XTAL_CAP_SEL_MASK) >> CCM32K_OSC32K_CTRL_XTAL_CAP_SEL_SHIFT;
                CextalRegValue = (CCM32K->OSC32K_CTRL & CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL_MASK) >> CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL_SHIFT;

                /* Increment XTAL Trim Value */
                if('1' == u8UartData)
                {
                    if(CxtalRegValue < 15U)
                    {
                        CxtalRegValue++;
                        CCM32K->OSC32K_CTRL &= ~(CCM32K_OSC32K_CTRL_OSC_EN_MASK);
                        CCM32K->OSC32K_CTRL &= ~CCM32K_OSC32K_CTRL_XTAL_CAP_SEL_MASK;
                        CCM32K->OSC32K_CTRL |= CCM32K_OSC32K_CTRL_XTAL_CAP_SEL(CxtalRegValue);
                        CCM32K->OSC32K_CTRL |= CCM32K_OSC32K_CTRL_OSC_EN(1U);
                    }

                }
                /* Decrement XTAL Trim Value */
                else if('2' == u8UartData)
                {
                    if(CxtalRegValue > 0U)
                    {
                        CxtalRegValue--;
                        CCM32K->OSC32K_CTRL &= ~(CCM32K_OSC32K_CTRL_OSC_EN_MASK);
                        CCM32K->OSC32K_CTRL &= ~CCM32K_OSC32K_CTRL_XTAL_CAP_SEL_MASK;
                        CCM32K->OSC32K_CTRL |= CCM32K_OSC32K_CTRL_XTAL_CAP_SEL(CxtalRegValue);
                        CCM32K->OSC32K_CTRL |= CCM32K_OSC32K_CTRL_OSC_EN(1U);
                    }
                }
                /* Increment EXTAL Trim Value */
                else if('3' == u8UartData)
                {
                    if(CextalRegValue < 15U)
                    {
                        CextalRegValue++;
                        CCM32K->OSC32K_CTRL &= ~(CCM32K_OSC32K_CTRL_OSC_EN_MASK);
                        CCM32K->OSC32K_CTRL &= ~CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL_MASK;
                        CCM32K->OSC32K_CTRL |= CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL(CextalRegValue);
                        CCM32K->OSC32K_CTRL |= CCM32K_OSC32K_CTRL_OSC_EN(1U);
                    }
                }
                /* Decrement EXTAL Trim Value */
                else if('4' == u8UartData)
                {
                    if(CextalRegValue > 0U)
                    {
                        CextalRegValue--;
                        CCM32K->OSC32K_CTRL &= ~(CCM32K_OSC32K_CTRL_OSC_EN_MASK);
                        CCM32K->OSC32K_CTRL &= ~CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL_MASK;
                        CCM32K->OSC32K_CTRL |= CCM32K_OSC32K_CTRL_EXTAL_CAP_SEL(CextalRegValue);
                        CCM32K->OSC32K_CTRL |= CCM32K_OSC32K_CTRL_OSC_EN(1U);
                    }
                }
                /* Exit from Trim RTC menu */
                else if('p' == u8UartData)
                {
                    /* Start Flash all LEDs, Stop PTC7 for frequency measurement */
                    PORT_SetPinMux(PORTC, 7u, kPORT_PinDisabledOrAnalog);
                    //LED_Init();
                    //LED_StartSerialFlash(LED1);
                    trimStateTest = gTrimStateInit_c;
                    bReturnFromSM = TRUE;
                    break;
                }
                else
                {
                    break;
                }

                CxtalValue     = CxtalRegValue * 2U; // in pF
                CextalValue    = CextalRegValue * 2U;
                aTrimValue  = (CxtalValue * CextalValue)/(CxtalValue + CextalValue)  + CCM32K_CSHUNT_VALUE;

                /* Print Trim Value */
                Serial_Print(mAppSerId, "\rCxtal: ", gAllowToBlock_d);
                Serial_PrintDec(mAppSerId, CxtalValue);
                Serial_Print(mAppSerId, " pF ", gAllowToBlock_d);
                Serial_Print(mAppSerId, "Cextal: ", gAllowToBlock_d);
                Serial_PrintDec(mAppSerId, CextalValue);
                Serial_Print(mAppSerId, " pF ", gAllowToBlock_d);
                Serial_Print(mAppSerId, "Cload: ", gAllowToBlock_d);
                Serial_PrintDec(mAppSerId, aTrimValue);
                Serial_Print(mAppSerId, " pF ", gAllowToBlock_d);
            }
        }
        break;

        default:
        break;
    }
#endif

    return bReturnFromSM;
}

/* get input value. possible inputs, e.g. 0, 1234, +1234, -1234, 01234, 0x1234 */
static bool_t GetPllNumOffset(int32_t *pVal, uint8_t u8UartData)
{
    static bool_t   val_valid = false;
    static uint32_t val;
    static uint32_t length;
    static uint32_t base;
    static int32_t  sign = 1;
    bool_t status = true;
    
    #define BIT_FIELDS_SIZE             28
    #define MAX_INPUT_DECIMAL           ((1<<(BIT_FIELDS_SIZE-1))-1)    // 0x7FFFFFF
    #define MIN_INPUT_DECIMAL           (-((1<<(BIT_FIELDS_SIZE-1))))   // 0x8000000
    #define MAX_INPUT_HEXA              ((1<<(BIT_FIELDS_SIZE))-1)      // 0xFFFFFFF
    
    do
    {
        if( pVal == NULL && u8UartData == 0 )
        {
            // reset context
            val_valid = false;
            val = 0;
            length = 0;
            base = 0;
            sign = 1;
            break;
        }
        
        if( u8UartData =='\r' )
        {
            // get result
            if (!val_valid)
            {
                status = false;
                break;
            }

            if(base==10)
            {
                int32_t v = val*sign;
                if( v > MAX_INPUT_DECIMAL || v < MIN_INPUT_DECIMAL )
                {
                    status = false;
                    break;
                }
                *pVal = v;
            }
            else if(base==16)
            {
                // extend the sign
                if( val > MAX_INPUT_HEXA )
                {
                    status = false;
                    break;
                }
                if( (val & (1<<(BIT_FIELDS_SIZE-1))) != 0 )
                {
                    // sign extention, cast to avoid "integer conversion resulted in a change of sign"
                    val |=  (uint32_t)(0 - (int32_t)(1<<(BIT_FIELDS_SIZE-1)));
                }
                *pVal = (int32_t)val;
            }
            break;
        }

        if (length == 0)
        {
            // need to determine if the input is in hexa or decimal
            if (u8UartData>='1' && u8UartData<='9')
            {
                base = 10;
                sign = 1;
                val_valid = true;
                val = u8UartData - '0';
            }
            else if (u8UartData=='+')
            {
                base = 10;
                sign = 1;
            }
            else if (u8UartData=='-')
            {
                base = 10;
                sign = -1;
            }
            else if (u8UartData=='0')
            {
                // can be hexa or decimal, set to 10 for now
              val_valid = true;
              base = 10;
              val = 0;
            }
            else
            {
                status = false;
                break;
            }
            length++;
            break;
        }
        
        if (length==1 && base==10 && u8UartData=='x'||u8UartData=='X')
        {
          base = 16;
          length++;
          val = 0;
          val_valid = true;
          break;
        }

        if (base==10 && u8UartData>='0' && u8UartData<='9')
        {
            val = val*10 + u8UartData - '0';
        }
        else if (base==16 && (u8UartData>='0' && u8UartData<='9'))
        {
            val = val*16 + u8UartData - '0';
        }
        else if (base==16 && (u8UartData>='a' && u8UartData<='f'))
        {
            val = val*16 + u8UartData - 'a' + 10;
        }
        else if (base==16 && (u8UartData>='A' && u8UartData<='F'))
        {
            val = val*16 + u8UartData - 'A' + 10;
        }
        else
        {
          status = false;
          break;
        }
        length++;
        val_valid = true;

    } while (false);
    
    return status;
}

static void PrintPllNumOffset(void)
{
    Serial_Print(mAppSerId, "\rCurrent value 0x", gAllowToBlock_d);
    uint32_t ulPllNumOffset = (XCVR_PLL_DIG->PLL_OFFSET_CTRL & XCVR_PLL_DIG_PLL_OFFSET_CTRL_PLL_NUMERATOR_OFFSET_MASK)
                                                            >> XCVR_PLL_DIG_PLL_OFFSET_CTRL_PLL_NUMERATOR_OFFSET_SHIFT;
    Serial_PrintHex(mAppSerId, (uint8_t*)&ulPllNumOffset, 4U, gPrtHexaBigEndian_c);
    Serial_Print(mAppSerId, " (", gAllowToBlock_d);
    if( (ulPllNumOffset&(1<<27)) !=0)
    {
        Serial_Print(mAppSerId, "-", gAllowToBlock_d);
        ulPllNumOffset = 0x10000000 - ulPllNumOffset;
    }
    Serial_PrintDec(mAppSerId, ulPllNumOffset);
    Serial_Print(mAppSerId, "). New value: ", gAllowToBlock_d);
}

bool_t CT_PllNumOffsetAdjust(ct_event_t evType, void* pAssociatedValue)
{
    bool_t bReturnFromSM = FALSE;

    static bool_t bPllStateTestEnabled = false;
    uint8_t u8UartData = 0xFF;

    if( bPllStateTestEnabled == false )
    {
        Serial_Print(mAppSerId, "\r\rAdjust PLL numerator offset.", gAllowToBlock_d);
        Serial_Print(mAppSerId, "\rInput can be hexa (0x0000000 to 0xFFFFFFF) or decimal (-134217728 to +134217727).", gAllowToBlock_d);
        Serial_Print(mAppSerId, "\rPress [p] for previous menu.\r", gAllowToBlock_d);
        bPllStateTestEnabled = true;

        PrintPllNumOffset();

        // reset input parer
        (void)GetPllNumOffset(NULL, 0);
    }
    else
    {
        if(gCtEvtUart_c == evType)
        {
            u8UartData = *((uint8_t*)pAssociatedValue);
            if ( u8UartData == '\r' )
            {
                int32_t val;
                bool_t status = GetPllNumOffset(&val, u8UartData);
                if( !status )
                {
                    Serial_Print(mAppSerId, " ==> invalid input !", gAllowToBlock_d);
                }
                else
                {
                    XCVR_PLL_DIG->PLL_OFFSET_CTRL &= ~XCVR_PLL_DIG_PLL_OFFSET_CTRL_PLL_NUMERATOR_OFFSET_MASK;
                    XCVR_PLL_DIG->PLL_OFFSET_CTRL |= XCVR_PLL_DIG_PLL_OFFSET_CTRL_PLL_NUMERATOR_OFFSET(val);
                }
                
                PrintPllNumOffset();
                
                // reset input parer
                (void)GetPllNumOffset(NULL, 0);
            }
            /* Exit */
            else if('p' == u8UartData)
            {
                bPllStateTestEnabled = false;
                bReturnFromSM = TRUE;
            }
            else
            {
                bool_t status = GetPllNumOffset(NULL, u8UartData);
                if( status )
                {
                   // print only if the input is valid
                   char s[2] = {u8UartData, 0};
                   Serial_Print(mAppSerId, s, gAllowToBlock_d);
                }
            }
        }
    }
    
    return bReturnFromSM;
}



