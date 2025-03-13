/*******************************头文件*******************************/
#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include "app_sniffer.h"
#include "connectivity_test.h"
#include "gen_fsk_tests.h"
#include "gen_fsk_tests_states.h"
#include "fsl_debug_console.h"
#include "fsl_component_serial_manager.h"
#include "fsl_format.h"
#include "menus.h"
#include "genfsk_ll.h"
#include "fsl_elemu.h"

/*******************************内部函数的声明*******************************/

/*******************************内部变量定义*******************************/
static uint32_t g_rxCount[gLinkMonitorNumMax] = {0};//当前抓取成功次数，更新参数后重新计数
static uint32_t g_rxTimeoutCount[gLinkMonitorNumMax] = {0};//当前启动监控次数，接收成功一次重新计数
static uint32_t g_rxfailCount[gLinkMonitorNumMax] = {0};//当前启动监控次数，接收成功一次重新计数
static uint8_t  g_mLMTrackNum = 0;//当前正在监控总数量

uint8_t  packetFromRole = 1;//一次接收事件区分两个角色，1是slave数据，0是master数据
uint8_t  masterPacket[256] = {0};
uint16_t masterbyteCount = 0;
int8_t   masterRSSI = 0;
uint8_t  slavePacket[256] = {0};
uint16_t slavebyteCount = 0;
int8_t   slaveRSSI = 0;

/* BLE Channels mapped to radio channels */
const uint8_t physicalChannelValue[40] = {
        44,   46,  48,  50,  52,  54,  56,  58,  60,  62, 
        64,   68,  70,  72,  74,  76,  78,  80,  82,  84, 
        86,   88,  90,  92,  94,  96,  98, 100, 102, 104, 
        106, 108, 110, 112, 114, 116, 118,  42,  66, 120};  

#define Reverse7Bit(i)  \
    ((((i)&0x01) << 6)| \
    (((i)&0x02) << 4)| \
    (((i)&0x04) << 2)| \
    (((i)&0x08)     )| \
    (((i)&0x10) >> 2)| \
    (((i)&0x20) >> 4)| \
    (((i)&0x40) >> 6))

/*******************************全局变量定义*******************************/
link_monitor_para_t gLMParaArry[gLinkMonitorNumMax] = {0};//存储所有设备的参数信息
link_monitor_para_t gLMPara = {0};//存储最新的设备参数信息
link_monitor_status_t mLMTrackConnStatus = {0, BTLE_NONE};//接收到can数据时赋值

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：可用信道列表计算
*函数参数 ：channel_map，用bit表示可用信道，bit0表示信道0，bit39表示信道39
            tableUsedchannels，存储可用信道列表，格式举例为tableUsedchannels[] = {9,10,21,22,23,33,34,35,36}
*函数返回值 ：可用信道总数
*备注：
**************************************************************/
static uint8_t app_sniffer_ChannelsNum(uint8_t channel_map[5], uint8_t tableUsedchannels[37])
{
    uint8_t numused = 0;
    for (uint8_t i = 0; i < 37; i++) 
    {
        if(channel_map[i >> 3] & (BIT(i % 8)))//取余数
//        if(channel_map[i >> 3] & (1 << (i & 0x07)))
        {
            tableUsedchannels[numused++] = i;
        }
    }

    return  numused;
}

/**************************************************************
*函数功能描述 ：跳频算法#2 MAN计算操作
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static uint16_t api_channel_mam(uint16_t a,uint16_t b)
{
    uint16_t output = 0;

    output = (((uint32_t)a*17) + (uint32_t)b) % (65536);//2^16 = 65536 = 0x00010000

    return output;
}

/**************************************************************
*函数功能描述 ：跳频算法#2 PERM计算操作
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static uint16_t api_channel_perm(uint16_t input)
{
    uint16_t output = 0;
    uint8_t input_L;
    uint8_t input_H;

    input_L = (uint8_t)input;
    input_H = (uint8_t)(input >> 8);
    
    input_L = ((input_L * 0x0802LU & 0x22110LU) | (input_L * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
    input_H = ((input_H * 0x0802LU & 0x22110LU) | (input_H * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;

    output = ((uint16_t)input_H << 8) | ((uint16_t)input_L);

    return output;
}

/**************************************************************
*函数功能描述 ：跳频算法#2 hop计算操作
*函数参数 ：Counter：事件计数器，每一个连接事件+1；
            syncAddress：接入地址Access Address
*函数返回值 ：
*备注：
**************************************************************/
static uint16_t api_channel_prn_e(uint16_t Counter,uint32_t syncAddress)
{
    uint16_t output = 0;
    uint16_t ChannelIdentifier;//Access Address的高16bit和低16bit进行XOR异或运算；
    uint16_t syncAddress_L;
    uint16_t syncAddress_H;

    syncAddress_L = (uint16_t)syncAddress;
    syncAddress_H = (uint16_t)(syncAddress >> 16);
    ChannelIdentifier = syncAddress_L ^ syncAddress_H;

    output = Counter ^ ChannelIdentifier;
    output = api_channel_perm(output);
    output = api_channel_mam(output,ChannelIdentifier);
    output = api_channel_perm(output);
    output = api_channel_mam(output,ChannelIdentifier);
    output = api_channel_perm(output);
    output = api_channel_mam(output,ChannelIdentifier);
    output = output ^ ChannelIdentifier;

    return output;
}

/**************************************************************
*函数功能描述 ：计算下一个信道，计算公式：unmappedChannel = (lastUnmappedChannel + hopIncrement) mod 37
                ble4.x使用算法#1，hop数值固定；
                ble5.x使用算法#2，hop数值会动态计算
*函数参数 ：
*函数返回值 ：下一次通信使用的信道
*备注：
**************************************************************/
uint8_t app_sniffer_NextChannel(link_monitor_para_t* LMpara, uint8_t id)
{
    uint8_t remappingIndex;
    uint8_t usedChannel;//信道索引
    uint8_t chUnmapped;
    uint16_t prn_e;

    LMpara[id].uiConnEvent++;//连接事件计数+1

    if(0 == LMpara[id].ucChannelSelection)
    {//算法#1
        chUnmapped =  (LMpara[id].channelLastUnmaped + LMpara[id].hopIncrement) % 37;
        if(LMpara[id].chM[chUnmapped >> 3] & (BIT(chUnmapped % 8)))
        // if (LMpara[id].chM[chUnmapped >> 3] & (1<<(chUnmapped & 0x7)))
        {//chUnmapped是可用信道，直接使用
            usedChannel = chUnmapped;
        }
        else
        {//chUnmapped不是可用信道，重映射，在tableUsedchannels[]中找可用信道
            remappingIndex = chUnmapped % LMpara[id].numUsedchannel;
            usedChannel = LMpara[id].tableUsedchannels[remappingIndex];
        }

        LMpara[id].channelLastUnmaped = chUnmapped;
    }
    else if(1 == LMpara[id].ucChannelSelection)
    {//算法#2
        prn_e = api_channel_prn_e(LMpara[id].uiConnEvent,LMpara[id].syncAddress);
        chUnmapped =  prn_e % 37;
        if(LMpara[id].chM[chUnmapped >> 3] & (BIT(chUnmapped % 8)))
        // if (LMpara[id].chM[chUnmapped >> 3] & (1<<(chUnmapped & 0x7)))
        {//chUnmapped是可用信道，直接使用
            usedChannel = chUnmapped;
        }
        else 
        {//chUnmapped不是可用信道，重映射，在tableUsedchannels[]中找可用信道
            remappingIndex = floor(((uint32_t)prn_e * LMpara[id].numUsedchannel) / 65536);
            usedChannel = LMpara[id].tableUsedchannels[remappingIndex]; 
        }
    }
    else
    {
        Serial_Print(mAppSerId, "ucChannelSelection err!\r\n", gAllowToBlock_d);
    }
    Serial_Print(mAppSerId, "next chn->", gAllowToBlock_d);
    Serial_PrintDec(mAppSerId, usedChannel);
    Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);

    return usedChannel;
}

/**************************************************************
*函数功能描述 ：根据id配置连接参数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static app_sniffer_param_set(link_monitor_para_t* LMparaArry,link_monitor_para_t LMpara,uint8_t id)
{
//    FLib_MemCpy(&LMparaArry[id], &LMpara, sizeof(link_monitor_para_t));
    LMparaArry[id].hopIncrement = LMpara.hopIncrement;
    LMparaArry[id].ucChannelSelection = LMpara.ucChannelSelection;
    LMparaArry[id].ucUnMapChIdx = LMpara.ucUnMapChIdx;
    LMparaArry[id].uiConnEvent = LMpara.uiConnEvent;
    LMparaArry[id].ucRole = LMpara.ucRole;
    LMparaArry[id].aucRemoteMasRxPHY = LMpara.aucRemoteMasRxPHY;
    LMparaArry[id].CI = (LMpara.CI * 1.25);//转换为ms
    LMparaArry[id].uiSuperTO = LMpara.uiSuperTO;
    LMparaArry[id].uiConnLatency = LMpara.uiConnLatency;
    LMparaArry[id].syncAddress = LMpara.syncAddress;
    LMparaArry[id].crcSeed = LMpara.crcSeed;
    // LMparaArry[id].ucCentralSCA = LMpara.ucCentralSCA;
    // LMparaArry[id].ulAnchorClk = LMpara.ulAnchorClk;
    // LMparaArry[id].uiAnchorDelay = LMpara.uiAnchorDelay;
    // LMparaArry[id].ulRxInstant = LMpara.ulRxInstant;

    FLib_MemCpy(LMparaArry[id].chM, LMpara.chM, 5);
    LMparaArry[id].numUsedchannel = app_sniffer_ChannelsNum(LMparaArry[id].chM, LMparaArry[id].tableUsedchannels);

    if(0 == LMparaArry[id].ucChannelSelection)
    {//#1算法
        LMparaArry[id].channelLastUnmaped = LMparaArry[id].ucUnMapChIdx;
    }
}

/**************************************************************
*函数功能描述 ：根据从can接收到的信息给信息存储结构体LMparaArry赋值
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void app_sniffer_init(link_monitor_para_t* LMparaArry, link_monitor_para_t LMpara)
{
    uint8_t id = mLMTrackConnStatus.id;//设备索引
    
    if(BTLE_CONNECT == mLMTrackConnStatus.status)
    {
        if(BTLE_CONNECT == LMparaArry[id].connStatus)
        {//已经建立连接，更新连接参数
            Serial_Print(mAppSerId, "con param up\r\n", gAllowToBlock_d);
            app_sniffer_param_set(LMparaArry,LMpara,id);
        }
        else
        {//新的设备建立了连接
            if(g_mLMTrackNum < gLinkMonitorNumMax)
            {//监控数量未达上限 
                g_mLMTrackNum++;//监控设备数+1

                Serial_Print(mAppSerId, "con ok\r\n", gAllowToBlock_d);
                //首次连接更新参数
                app_sniffer_param_set(LMparaArry,LMpara,id);
                LMparaArry[id].connStatus = BTLE_CONNECT;
            }
            else
            {
                Serial_Print(mAppSerId, "sniffer full!\r\n", gAllowToBlock_d);
            }
        }
    }
    else if(BTLE_DISCONN == mLMTrackConnStatus.status)  //disconnection
    {
        if(0 != g_mLMTrackNum)
        {
            g_mLMTrackNum--;
        }
        g_rxCount[id] = 0;
        g_rxTimeoutCount[id] = 0;
        g_rxfailCount[id] = 0;

        FLib_MemSet(&LMparaArry[id],0,sizeof(link_monitor_para_t));
        LMparaArry[id].connStatus = BTLE_DISCONN;
        Serial_Print(mAppSerId, "discon ok\r\n", gAllowToBlock_d);
    }
    else
    {
        Serial_Print(mAppSerId, "status error\r\n", gAllowToBlock_d);
    }
}

/**************************************************************
*函数功能描述 ：设置下一个监控数据包的参数，包括“数据白化、信道、crc、接入地址”
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void app_sniffer_SetGENFSK(uint8_t id)
{
    uint8_t usedChannel = app_sniffer_NextChannel(gLMParaArry, id);

    //PHY设置
    if(0 == gLMParaArry[id].aucRemoteMasRxPHY)
    {//1Mbps
        radioConfig.dataRate = gGenfskDR1Mbps;
    }
    else
    {//2Mbps
        radioConfig.dataRate = gGenfskDR2Mbps;
    }
    GENFSK_RadioConfig(mAppGenfskId, &radioConfig);

    /*set whitener config*/
    whitenConfig.whitenInit = Reverse7Bit(0x40 + usedChannel);
    GENFSK_SetWhitenerConfig(mAppGenfskId, &whitenConfig);
    
    /*set channel: Freq = 2360MHz + ChannNumber*1MHz*/
    GENFSK_SetChannelNumber(mAppGenfskId, physicalChannelValue[usedChannel]);
    
    /*set crc config*/
    crcConfig.crcSeed = gLMParaArry[id].crcSeed; 
    GENFSK_SetCrcConfig(mAppGenfskId, &crcConfig);
            
    /*set network address at location 0 and enable it*/
    ntwkAddr.nwkAddr = gLMParaArry[id].syncAddress;
    GENFSK_SetNetworkAddress(mAppGenfskId, 0, &ntwkAddr);
    GENFSK_EnableNetworkAddress(mAppGenfskId, 0);
}

/**************************************************************
*函数功能描述 ：多角色条件下，监控超时时间确定，并计算好当前要检测的信道
*函数参数 ：
*函数返回值 ：超时时间
*备注：
**************************************************************/
static uint32_t app_sniffer_timeout_set(uint8_t id)
{
    uint32_t timeout = 0;
    uint16_t CI_temp = 0;
    uint8_t CI_flag = 0;
    uint8_t u8chapnum = 0;

    //判定各个角色连接间隔是否相同
    for(uint8_t i = 0; i < gLinkMonitorNumMax;i++)
    {
        if(BTLE_CONNECT == gLMParaArry[i].connStatus)
        {
            if(0 == CI_temp)
            {
                CI_temp = gLMParaArry[i].CI;
                CI_flag = 1;
            }
            else
            {
                if(CI_temp == gLMParaArry[i].CI)
                {
                    CI_flag = 1;
                }
                else
                {
                    CI_flag = 0;
                    break;//有一组不相等就进入方案1
                }
            }
        }
    }

    if(0 == CI_flag)
    {
        //计算错过信道数信道
        u8chapnum = (APP_SNIFFER_CHAMP_TIME * (g_mLMTrackNum - 1))/gLMParaArry[id].CI;
        if((0 == u8chapnum) || (1 == g_mLMTrackNum))
        {//只有一个角色，或该角色连接参数小于20ms只更新一次信道
            u8chapnum = 1;
        }
        else
        {
            u8chapnum += 1;//消除监控其他设备时，耽误的时间
        }

        timeout = APP_SNIFFER_CHAMP_TIME * 1000;
    }
    else
    {//所有角色的CI相同
        u8chapnum = g_mLMTrackNum;
        timeout = (uint32_t)CI_temp * 1000;
    }

    for(uint8_t i = 0;i < u8chapnum;i++)
    {
        app_sniffer_SetGENFSK(id);//更新监听参数
    }

    return timeout;
}

/**************************************************************
*函数功能描述 ：主任务执行，实际信道跟踪的处理逻辑就在这里实现
*函数参数 ：evType，事件类型
            pAssociatedValue，存储数据结构体指针
            pcontTestState，决定是否继续监控
*函数返回值 ：
*备注：
**************************************************************/
void app_sniffer_handle(ct_event_t evType,void* pAssociatedValue,ct_cont_tests_states_t* pcontTestState)
{
    bool_t bRestartRx = FALSE;//决定是否要启动下一次监控
    static uint8_t mLMTrackId = 0;//当前要操作设备索引

    // Serial_Print(mAppSerId, "evType:", gAllowToBlock_d);
    // Serial_PrintDec(mAppSerId, evType);
    // Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);

    if(gCtEvtSelfEvent_c == evType)
    {//没建立连接时等待连接
        CT_EnableShortcuts(FALSE);
        Serial_Print(mAppSerId, "\r\nWaiting for onnection to peripheral\r\n", gAllowToBlock_d);
    }

    if(gCtEvtCan_Lin_c == evType)
    {//接收到can数据
        if(BTLE_DISCONN == mLMTrackConnStatus.status)  //disconnection event
        {
            // GENFSK_AbortAll();//多角色不能调用，避免耽误监听其他角色
            bRestartRx = FALSE;
        }
        else if(BTLE_CONNECT == mLMTrackConnStatus.status)
        {
            if(gGenfskBusyRx_c != GENFSK_GetState(mAppGenfskId))
            {//rx不工作时，再打开rx
                bRestartRx = TRUE;
                mLMTrackId = mLMTrackConnStatus.id;
            } 
        }
        else
        {
            Serial_Print(mAppSerId, "status error:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId,mLMTrackConnStatus.status);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
        }
        mLMTrackConnStatus.status = BTLE_NONE;
    }

    if(gCtEvtRxDone_c == evType)//两包数据都接收完成
    {//接收到了genfsk数据
        g_rxCount[mLMTrackId]++;//接收成功

        Serial_Print(mAppSerId, "\r\n==>RX suc,ID:", gAllowToBlock_d);
        Serial_PrintDec(mAppSerId, mLMTrackId);
        Serial_Print(mAppSerId, ",rxSucCount:", gAllowToBlock_d);
        Serial_PrintDec(mAppSerId, g_rxCount[mLMTrackId]);
        Serial_Print(mAppSerId, ",Counter:", gAllowToBlock_d);
		Serial_PrintDec(mAppSerId,gLMParaArry[mLMTrackId].uiConnEvent);
        Serial_Print(mAppSerId, ",TimeoutCount:", gAllowToBlock_d);
        Serial_PrintDec(mAppSerId,g_rxTimeoutCount[mLMTrackId]);
        Serial_Print(mAppSerId, ",failCount:", gAllowToBlock_d);
        Serial_PrintDec(mAppSerId,g_rxfailCount[mLMTrackId]);
        // Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);

        if(1 == gLMParaArry[mLMTrackId].ucRole)
        {//需要捕捉master的数据
            Serial_Print(mAppSerId, "\r\nm L: ", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, masterbyteCount);
            // Serial_Print(mAppSerId, ",Data:", gAllowToBlock_d);
            // for(uint16_t i=0; i<masterbyteCount; i++)
            // {
            //     (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii(masterPacket[i]), 2U);
            //     Serial_Print(mAppSerId, " ", gAllowToBlock_d);
            // }
            PrintCrtRssi(masterRSSI, mAppSerId);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);

            //调用can tx api将数据传递给车端ble节点
            uint8_t rssiData[3];
            rssiData[0] = 1;//event type
            rssiData[1] = mLMTrackId;//id
            rssiData[2] = masterRSSI;//data
            app_sniffer_can_tx(rssiData,sizeof(rssiData));
        }
        else if(0 == gLMParaArry[mLMTrackId].ucRole)
        {//需要捕捉slave的数据
            Serial_Print(mAppSerId, "\r\ns L: ", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, slavebyteCount);
            // Serial_Print(mAppSerId, ",Data:", gAllowToBlock_d);
            // for(uint16_t i=0; i<slavebyteCount; i++)
            // {
            //     (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii(slavePacket[i]), 2U);
            //     Serial_Print(mAppSerId, " ", gAllowToBlock_d);
            // }
            PrintCrtRssi(slaveRSSI, mAppSerId);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);

            //调用can tx api将数据传递给车端ble节点
            uint8_t rssiData[3];
            rssiData[0] = 1;//event type
            rssiData[1] = mLMTrackId;//id
            rssiData[2] = slaveRSSI;//data
            app_sniffer_can_tx(rssiData,sizeof(rssiData));
        }
        else
        {
            Serial_Print(mAppSerId, "ucRole err\r\n ", gAllowToBlock_d);
        }

        //清空各计数标志
        g_rxTimeoutCount[mLMTrackId] = 0;
        g_rxfailCount[mLMTrackId] = 0;
        masterbyteCount = 0;
        slavebyteCount = 0;

        #if 0
        ct_rx_indication_t* pIndicationInfo = (ct_rx_indication_t*)pAssociatedValue;//一般而言传递的都是slave的数据
        PrintCrtRssi(pIndicationInfo->rssi, mAppSerId);

        // if(0 == packetFromRole)
        // {//接收到master数据
        //     Serial_Print(mAppSerId, "\r\nMaster len:", gAllowToBlock_d);
        // }
        // else
        {
            Serial_Print(mAppSerId, "\r\nReceive len: ", gAllowToBlock_d);
        }
        Serial_PrintDec(mAppSerId, pIndicationInfo->bufferLength-4);
        Serial_Print(mAppSerId, ",Data:", gAllowToBlock_d);
        for(uint16_t i=0; i<(pIndicationInfo->bufferLength - 4); i++)
        {
            (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii(pIndicationInfo->pBuffer[4+i]), 2U);
            Serial_Print(mAppSerId, " ", gAllowToBlock_d);
        }
        Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
        #endif

        //计算下一个要检测的设备索引
        for(uint8_t i = 0; i < gLinkMonitorNumMax;i++)
        {
        	mLMTrackId++;
			mLMTrackId %= gLinkMonitorNumMax;
			if(BTLE_CONNECT == gLMParaArry[mLMTrackId].connStatus)
			{
                bRestartRx = TRUE;
                break;
            }
        }
    }
    else if(gCtEvtRxFailed_c == evType || gCtEvtSeqTimeout_c == evType)
    {
        GENFSK_AbortAll();

        Serial_Print(mAppSerId, "\r\n==>RX fail,ID:", gAllowToBlock_d);
        Serial_PrintDec(mAppSerId, mLMTrackId);

        if((0 == packetFromRole) && (0 != masterbyteCount))
        {//一次监听只收到了一包数据，打印提示，但是不传递给主控模块(因为无法判定是m还是s发送的)
            packetFromRole = 1;
            Serial_Print(mAppSerId, "\r\nr L: ", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, masterbyteCount);
            // Serial_Print(mAppSerId, ",Data:", gAllowToBlock_d);
            // for(uint16_t i=0; i<masterbyteCount; i++)
            // {
            //     (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii(masterPacket[i]), 2U);
            //     Serial_Print(mAppSerId, " ", gAllowToBlock_d);
            // }
            PrintCrtRssi(masterRSSI, mAppSerId);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            masterbyteCount = 0;
            slavebyteCount = 0;
        }
        
        if(gCtEvtRxFailed_c == evType)
        {//接收失败
            g_rxfailCount[mLMTrackId]++;
            Serial_Print(mAppSerId, ",rx failed\r\n", gAllowToBlock_d);
        }
        else
        {//接收超时
            g_rxTimeoutCount[mLMTrackId]++;
            Serial_Print(mAppSerId, ",->rx timeout:", gAllowToBlock_d);
            uint16_t CI_temp = 0;
            uint8_t CI_flag = 0;
            uint8_t u8chapnum = 0;
            
            for(uint8_t i = 0; i < gLinkMonitorNumMax;i++)
            {//判定各个角色连接间隔是否相同
                if(BTLE_CONNECT == gLMParaArry[i].connStatus)
                {
                    if(0 == CI_temp)
                    {
                        CI_temp = gLMParaArry[i].CI;
                        CI_flag = 1;
                    }
                    else
                    {
                        if(CI_temp == gLMParaArry[i].CI)
                        {
                            CI_flag = 1;
                        }
                        else
                        {
                            CI_flag = 0;
                            break;//有一组不相等就进入方案1
                        }
                    }
                }
            }
            if(0 == CI_flag)
            {
                Serial_PrintDec(mAppSerId,APP_SNIFFER_CHAMP_TIME);
            }
            else
            {
                Serial_PrintDec(mAppSerId,gLMParaArry[mLMTrackId].CI);
            }
            Serial_Print(mAppSerId, "ms\r\n", gAllowToBlock_d);

            if(0 == (g_rxTimeoutCount[mLMTrackId] % gLMParaArry[mLMTrackId].numUsedchannel))
            {//连续n次没有收到数据，主动向ble节点请求最新的连接参数
                uint8_t sendData[2];
                sendData[0] = 2;//event type
                sendData[1] = mLMTrackId;//id
                app_sniffer_can_tx(sendData,sizeof(sendData));
            }
        }

        //计算下一个要检测的设备索引
        for(uint8_t i = 0; i < gLinkMonitorNumMax;i++)
        {
            mLMTrackId++;
            mLMTrackId %= gLinkMonitorNumMax;
            if(BTLE_CONNECT == gLMParaArry[mLMTrackId].connStatus)
            {
                bRestartRx = TRUE;
                break;
            }
        }
    }

    if(bRestartRx)
    {
        GENFSK_timestamp_t timeout = app_sniffer_timeout_set(mLMTrackId);
        // Serial_Print(mAppSerId, "start rx,timeout:", gAllowToBlock_d);
        // Serial_PrintDec(mAppSerId, (uint32_t)timeout/1000);
        // Serial_Print(mAppSerId, "ms\r\n", gAllowToBlock_d);

        //启动一次接收
        if(gGenfskSuccess_c != 
            GENFSK_StartRx(mAppGenfskId,
            gRxBuffer,
            gGenFskDefaultMaxBufferSize_c + crcConfig.crcSize,//264
            0,
            timeout))
        {
            Serial_Print(mAppSerId, "Rx failed", gAllowToBlock_d);
            // Serial_Print(mAppSerId, cu8ContMessages[4], gAllowToBlock_d);
            // Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            // *pcontTestState = gContStateIdle_c;
        }
    }
}

/**************************************************************
*函数功能描述 ：处理can传递的数据，并传递给主任务处理
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void app_sniffer_can_rx(const hal_can_pdu_st * pdata)
{
    getConnParams_t getConnParams = {0};
    Serial_Print(mAppSerId, "\r\nCan RX data:", gAllowToBlock_d);

    for(uint16_t i=0; i<64; i++)
    {
        (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii(pdata->m_u8_pdata[i]), 2U);
        Serial_Print(mAppSerId, " ", gAllowToBlock_d);
    }
    Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);

    if(GENFSKBLECONINFOCMD == pdata->m_u8_pdata[0])
    {
        FLib_MemSet(&mLMTrackConnStatus,0,sizeof(link_monitor_status_t));//先清空
        mLMTrackConnStatus.id = pdata->m_u8_pdata[1];//索引
        mLMTrackConnStatus.status = pdata->m_u8_pdata[2];//连接状态

        if(BTLE_CONNECT == mLMTrackConnStatus.status)
        {
            FLib_MemSet(&gLMPara,0,sizeof(link_monitor_para_t));//先清空
            FLib_MemCpy(&getConnParams,&pdata->m_u8_pdata[3],sizeof(getConnParams_t));
            FLib_MemCpy(gLMPara.chM,getConnParams.aChMapBm,5);
            gLMPara.hopIncrement = getConnParams.ucHop;
            gLMPara.ucChannelSelection = getConnParams.ucChannelSelection;
            gLMPara.ucUnMapChIdx = getConnParams.ucUnMapChIdx;
            gLMPara.uiConnEvent = getConnParams.uiConnEvent;

            gLMPara.ucRole = getConnParams.ucRole;
            gLMPara.aucRemoteMasRxPHY = getConnParams.aucRemoteMasRxPHY;
            gLMPara.CI = getConnParams.uiConnInterval;
            gLMPara.uiSuperTO = getConnParams.uiSuperTO;
            gLMPara.uiConnLatency = getConnParams.uiConnLatency;
            gLMPara.syncAddress = getConnParams.ulTxAccCode;
            gLMPara.crcSeed  = getConnParams.aCrcInitVal[2]<<16;
            gLMPara.crcSeed |= getConnParams.aCrcInitVal[1]<<8;
            gLMPara.crcSeed |= getConnParams.aCrcInitVal[0];

            // gLMPara.ucCentralSCA = getConnParams.ucCentralSCA;
            // gLMPara.ulAnchorClk = getConnParams.ulAnchorClk;
            // gLMPara.uiAnchorDelay = getConnParams.uiAnchorDelay;
            // gLMPara.ulRxInstant = getConnParams.ulRxInstant;

            #if 0
            Serial_Print(mAppSerId, "peerDeviceId:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, mLMTrackConnStatus.id);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "connOrDisconn:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, mLMTrackConnStatus.status);        
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);

            Serial_Print(mAppSerId, "chM:", gAllowToBlock_d);
            for(uint16_t i=0; i<5; i++)
            {
                (void)SerialManager_WriteBlocking((serial_write_handle_t)g_connWriteHandle, FORMAT_Hex2Ascii(gLMPara.chM[i]), 2U);
            }
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "HopIncrement:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.hopIncrement);    
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "ucChannelSelection:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.ucChannelSelection);    
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "ucUnMapChIdx:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.ucUnMapChIdx);    
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "uiConnEvent:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.uiConnEvent);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);

            Serial_Print(mAppSerId, "ucRole:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.ucRole);    
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "aucRemoteMasRxPHY:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.aucRemoteMasRxPHY);    
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "Connection Interval:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, (gLMPara.CI*1.25));
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "uiSuperTO:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.uiSuperTO);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "uiConnLatency:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.uiConnLatency);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "Access ADDR:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.syncAddress);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            Serial_Print(mAppSerId, "CRC seed:", gAllowToBlock_d);
            Serial_PrintDec(mAppSerId, gLMPara.crcSeed);
            Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);

            // Serial_Print(mAppSerId, "ucCentralSCA:", gAllowToBlock_d);
            // Serial_PrintDec(mAppSerId, gLMPara.ucCentralSCA);    
            // Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            // Serial_Print(mAppSerId, "ulAnchorClk:", gAllowToBlock_d);
            // Serial_PrintDec(mAppSerId, gLMPara.ulAnchorClk);
            // Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            // Serial_Print(mAppSerId, "uiAnchorDelay:", gAllowToBlock_d);
            // Serial_PrintDec(mAppSerId, gLMPara.uiAnchorDelay);
            // Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            // Serial_Print(mAppSerId, "ulRxInstant:", gAllowToBlock_d);
            // Serial_PrintDec(mAppSerId, gLMPara.ulRxInstant);
            // Serial_Print(mAppSerId, "\r\n", gAllowToBlock_d);
            #endif
        }

        app_sniffer_init(gLMParaArry, gLMPara);//给信息数组赋值
        APP_CanCallback(NULL);//传递事件给主任务让其处理
    }
}

uint8_t gCanTxData[64] = {0};

/**************************************************************
*函数功能描述 ：通过can传递数据给ble节点
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void app_sniffer_can_tx(uint8_t* pData,uint8_t length)
{
    hal_can_pdu_st pdu={0};

    if(length < (sizeof(gCanTxData) - 1))
    {
        FLib_MemSet(gCanTxData,0,sizeof(gCanTxData));//先清空
        gCanTxData[0] = GENFSKBLECONINFOCMD;
        FLib_MemCpy(&gCanTxData[1],pData,length);

        pdu.m_u32_canid = 0x104;
        pdu.m_id_type_e = HAL_CAN_ID_TYPE_StAND;
        pdu.m_u8_dlc = 64;
        pdu.m_u8_pdata = gCanTxData;

        hal_can_if_write(HAL_CAN0_CHANNEL,&pdu);
    }
}
