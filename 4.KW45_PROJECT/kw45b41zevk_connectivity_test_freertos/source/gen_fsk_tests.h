/*

Copyright 2016-2020 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#ifndef __GEN_FSK_TESTS_H__
#define __GEN_FSK_TESTS_H__

#include "fsl_component_serial_manager.h"
#include "fsl_component_timer_manager.h"
#include "app_sniffer.h"
#include "genfsk_interface.h"


#define Serial_Print(a,b,c)  SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, (uint8_t *)b, strlen(b))
#define Serial_PrintDec(a,b) SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Dec2Str(b), strlen((char const*)FORMAT_Dec2Str(b)))
#define Serial_PrintHex(a,b,c,d)              \
                for(int32_t i=c-1; i>=0; i--)   \
                {                             \
                    (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii((b)[i]), 2U);\
                }

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef enum app_status_tag
{
    gAppStatusSuccess,
    gAppStatusMemoryAllocFailure
} app_status_t;

typedef enum ct_event_tag
{
    gCtEvtRxDone_c       = 0x00000001U,//genfsk rx事件
    gCtEvtTxDone_c       = 0x00000002U,
    gCtEvtSeqTimeout_c   = 0x00000004U,
    gCtEvtRxFailed_c     = 0x00000008U,

    gCtEvtTimerExpired_c = 0x00000010U,
    gCtEvtUart_c         = 0x00000020U,
    gCtEvtKBD_c          = 0x00000040U,
    gCtEvtSelfEvent_c    = 0x00000080U,

    gCtEvtWakeUp_c       = 0x00000100U,

    #if (APP_SNIFFER_EN)
    gCtEvtCan_Lin_c      = 0x00000200U,
    gCtEvtMaxEvent_c     = 0x00000800U,
    gCtEvtEventsAll_c    = 0x00000FFFU
    #else
    gCtEvtMaxEvent_c     = 0x00000200U,
    gCtEvtEventsAll_c    = 0x000003FFU
    #endif
}ct_event_t;

typedef enum ct_param_type_tag{
    gParamTypeNumber_c = 0,
    gParamTypeString_c,
    gParamTypeBool_c,
    gParamTypeMixed_c,
    gParamTypeMaxType_c
}ct_param_type_t;

typedef struct ct_config_params_tag
{
    ct_param_type_t paramType;//数据类型
    uint8_t paramName[20];//字符串描述
    uint8_t id;
    char**  ptr;
    union
    {
        uint32_t decValue;
        uint8_t stringValue[4];
        bool_t  boolValue;
    }
    paramValue;
}ct_config_params_t;

typedef struct ct_rx_indication_tag
{//rx接收到的数据格式
    uint64_t timestamp;//这个是监控节点自身的时间戳，如果没有配置每次运行时从0开始计数
    uint8_t *pBuffer;//存储接入地址后面的数据，后3bytes是crc
    uint16_t bufferLength;
    int8_t rssi;
    uint8_t crcValid;//总是固定为0x01
}ct_rx_indication_t;

typedef void (* pHookAppNotification) ( void );
typedef void (* pTmrHookNotification) (void*);

typedef enum ct_conf_param_idx_tag
{//运行过程可以动态配置的参数
    gConfParamMode = 0,//rx
    gConfParamRateMode,//BLE 1Mbps
    gConfParamWhitenMode,//选择固定或所有通道过滤
    gConfParamChannel,//0~127，频率依次为2360Mhz + x*1Mhz
                    //；ble的40个信道频率范围是2402Mhz + x*2Mhz，也就是对于GenFSK的42~122信道
                    //；其中ble37对应42+43、ble38对应66+67、ble39对应122+123
    gConfParamPower,//0~32
    gConfParamPayload,//0~63
    gConfParamXtalTrim,
    gConfParamTypeMax,
    gConfParamIdxMax
}ct_conf_param_idx;

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define gModeRx_c (1)
#define gModeTx_c (2)
#define gDefaultMode_c gModeRx_c

#define gRateGenfsk250Kbps_c    (0)
#define gRateGenfsk500Kbps_c    (1)
#define gRateGenfsk1Mbps_c      (2)
#define gRateGenfsk2Mbps_c      (3)
#define gRateBle125Kbps_c       (4)
#define gRateBle500Kbps_c       (5)
#define gRateBle1Mbps_c         (6)
#define gRateBle2Mbps_c         (7)

#define gMaxRate_c              gRateBle2Mbps_c
#define gMaxRateGenfsk_c        gRateGenfsk2Mbps_c
#define gRateMaxNbr             (4)

#define gWhitenFixed_c          (0)
#define gWhitenChanging_c       (1)
#define gMaxWhitenMode_c        (2)
#define gDefaultWhitenMode_c    (gWhitenFixed_c)

#define gMinRate_c     gRateGenfsk250Kbps_c
#define gDefaultRate_c gRateGenfsk1Mbps_c

/*tx power*/
#define gGenFskLastLowPowerIdx_c   (31)
#define gGenFskMaxTxPowerLevel_c   gGenFskLastLowPowerIdx_c
#define gGenFskMinTxPowerLevel_c     (0x00)                             /* Low power table is used from index 0 to index 31. */

#define gGenFskDefaultTxPowerLevel_c (0x08)

/*channel*/
#define gGenFskMaxChannel_c     (0x7F)
#define gGenFskMinChannel_c     (0x00)
#define gGenFskDefaultChannel_c (0x2A)
/*network address*/
#define gGenFskDefaultSyncAddress_c  (0x8E89BED6)//广播接入地址
#define gGenFskDefaultSyncAddrSize_c (0x03) /*bytes = size + 1*/

/*the following field sizes must be multiple of 8 bit*/
#define gGenFskDefaultH0FieldSize_c     (8)
/* payload length field size can be from 1 to 16 bits, but the maximum payload length is limited to 2047-gGenFskDefaultCrcSize_c bytes.
   PoolsDetails_c may need to be modofied accordingly */
#define gGenFskDefaultLengthFieldSize_c (8)
#define gGenFskDefaultH1FieldSize_c     ((16U-gGenFskDefaultLengthFieldSize_c) & 0x07U)//0
#define gGenFskDefaultHeaderSizeBytes_c ((gGenFskDefaultH0FieldSize_c + \
                                            gGenFskDefaultLengthFieldSize_c + \
                                            gGenFskDefaultH1FieldSize_c) >> 3)

#define gGenFskDefaultCrcSize_c         3
#define gGenFskDefaultCrcSeed_c         0x00555555
#define gGenFskDefaultCrccPoly_c        0x0000065B

/*payload length is limited to 2047 bytes including CRC*/
#define gGenFskMaxPayloadLen_c ((1 << gGenFskDefaultLengthFieldSize_c) - 1)

/*test opcode + 2byte packet index + 2byte number of packets for PER test*/
#define gGenFskMinPayloadLen_c (6)
#define gGenFskDefaultPayloadLen_c (gGenFskMinPayloadLen_c)

#define gMaxTrimValue_c                 0x7F
#define gMinTrimValue_c                 0x00

#define gGenFskDefaultMaxBufferSize_c (gGenFskDefaultSyncAddrSize_c + 1 + \
                                        gGenFskDefaultHeaderSizeBytes_c  + \
                                            gGenFskMaxPayloadLen_c)

/*H0 and H1 config*/
#if (APP_SNIFFER_EN)
#define gGenFskDefaultH0Value_c        (0x0000)
#define gGenFskDefaultH0Mask_c         0
#define gGenFskDefaultH1Value_c        (0x0000)
#define gGenFskDefaultH1Mask_c         0
#else
#define gGenFskDefaultH0Value_c        (0x0000)
#define gGenFskDefaultH0Mask_c         ((1 << gGenFskDefaultH0FieldSize_c) - 1)
#define gGenFskDefaultH1Value_c        (0x0000)
#define gGenFskDefaultH1Mask_c         ((1 << gGenFskDefaultH1FieldSize_c) - 1)
#endif
/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern ct_config_params_t gaConfigParams[];
extern serial_handle_t mAppSerId;
extern SERIAL_MANAGER_READ_HANDLE_DEFINE(g_connReadHandle);
extern SERIAL_MANAGER_WRITE_HANDLE_DEFINE(g_connWriteHandle);
extern TIMER_MANAGER_HANDLE_DEFINE(mAppTmrId);


extern GENFSK_nwk_addr_match_t ntwkAddr;
extern GENFSK_crc_config_t crcConfig;
extern GENFSK_whitener_config_t whitenConfig;
extern GENFSK_radio_config_t radioConfig;
extern uint8_t mAppGenfskId;
extern uint8_t* gRxBuffer;
extern uint8_t* gTxBuffer;
/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
extern app_status_t CT_GenFskInit(pHookAppNotification pFunc, pTmrHookNotification pTmrFunc);

extern bool_t CT_PacketErrorRate(ct_event_t evType, void* pAssociatedValue);
extern bool_t CT_RangeTest(ct_event_t evType, void* pAssociatedValue);
extern bool_t CT_ContinuousTests(ct_event_t evType, void* pAssociatedValue);
extern bool_t CT_TrimAdjust(ct_event_t evType, void* pAssociatedValue);
extern bool_t CT_PllNumOffsetAdjust(ct_event_t evType, void* pAssociatedValue);

extern bool_t CT_UpdateShortcutKeyParam(uint8_t u8PressedKey);
extern bool_t CT_IsShortcutMenuEnabled(void);
extern void CT_EnableShortcuts(bool_t bEnable);/* Shortcuts enable/disable depending on test state */

#endif
