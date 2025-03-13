#ifndef __APP_SNIFFER__
#define __APP_SNIFFER__

/*******************************头文件*******************************/
#include "hal_can_typedef.h"

/*******************************宏定义*******************************/
#define APP_SNIFFER_EN  (1)//参考程序修改点，1就是打开

#define APP_SNIFFER_CHAMP_TIME  (20)//多角色监控默认事件窗口20ms

#define gLinkMonitorNumMax  4//最大监控数量

#define BTLE_NONE        0
#define BTLE_CONNECT     1
#define BTLE_DISCONN     2

#define GENFSKBLECONINFOCMD     (0x5A)//识别有效连接信息的句柄

/*******************************结构体、枚举等******************************/
typedef struct link_monitor_status_tag
{
    uint8_t status;  //1:connect,  2:disconnect
    uint8_t id;//peerDeviceId
}link_monitor_status_t;

typedef struct link_monitor_para_tag
{//连接参数
    //连接状态
    uint32_t connStatus;

    //信道相关
    uint8_t tableUsedchannels[37];//可用信道列表
    uint8_t numUsedchannel;//可用信道总数
    uint8_t channelLastUnmaped;//上一次的信道
    
    uint8_t chM[5];//aChMapBm
    uint8_t hopIncrement;//ucHop
    uint8_t ucChannelSelection;//跳频算法，0表示算法#1，1表示算法#2
    uint8_t ucUnMapChIdx;//信道索引，适用于跳频算法#1
    uint16_t uiConnEvent;//当前连接计数器

    //基本参数相关
    uint8_t ucRole;//自己的设备角色，0 for central/master and 1 for peripheral/slave
    uint8_t aucRemoteMasRxPHY;//通信使用的PHY数值，0是1Mbps
    uint16_t CI;//uiConnInterval，连接间隔，取值范围为0x0006 ~0x0C80
    uint16_t uiSuperTO;//连接timeout
    uint16_t uiConnLatency;//从机潜伏次数
    uint32_t syncAddress;//ulTxAccCode，是接入地址，不是mac addr
    uint32_t crcSeed;//aCrcInitVal，crc初始化数值

    //时钟相关
    uint8_t  ucCentralSCA;//休眠时间精度
    uint32_t ulAnchorClk;//连接事件的锚点计时槽
    uint16_t uiAnchorDelay;//连接事件的锚点定时的槽位偏移
    uint32_t ulRxInstant;//最后一次成功接入地址接收瞬间(单元625us)
}link_monitor_para_t;

typedef struct getConnParams_tag
{//44bytes
    uint16_t        connectionHandle;   /*!< Connection identifier */
    uint32_t        ulTxAccCode;        /*!< Access address */
    uint8_t         aCrcInitVal[3U];    /*!< CRC init */
    uint16_t        uiConnInterval;     /*!< Connection interval (unit 1.25ms) */
    uint16_t        uiSuperTO;          /*!< Supervision timeout (unit 10ms) */
    uint16_t        uiConnLatency;      /*!< Latency (unit connection interval) */

    uint8_t         aChMapBm[5U];       /*!< Channel map (FF FF FF FF 1F if all channels are used) */
    uint8_t         ucChannelSelection; /*!< Hop algorithm (0/1 for hop algorithm #1 or #2) */
    uint8_t         ucHop;              /*!< Hop increment */
    uint8_t         ucUnMapChIdx;       /*!< Unmapped channel index (used only for hop algorithm #1) */

    uint8_t         ucCentralSCA;       /*!< Sleep clock accuracy (0 to 7) */
    uint8_t         ucRole;             /*!< Role (0 for central and 1 for peripheral) */
    uint8_t         aucRemoteMasRxPHY;  /*!< TX/RX PHY */
    uint8_t         seqNum;             /*!< Sequence number; b1 and b0: the latest received SN and NESN; b5 and b4: the latest transmitted SN and NESN; other fields are reserved. */
    uint16_t        uiConnEvent;        /*!< Current connection event counter */
    uint32_t        ulAnchorClk;        /*!< Slot of the anchor point timing of the connection event */
    uint16_t        uiAnchorDelay;      /*!< Slot offset of the anchor point timing of the connection event */
    uint32_t        ulRxInstant;        /*!< Last successful access address reception instant (unit 625us) */
} getConnParams_t;

/*******************************全局变量的外部声明extern*******************************/
extern link_monitor_status_t mLMTrackConnStatus;
extern link_monitor_para_t gLMPara;
extern link_monitor_para_t gLMParaArry[gLinkMonitorNumMax];

extern uint8_t  packetFromRole;//决定本次接收到数据的类型，1是slave数据，0是master数据
extern uint8_t  masterPacket[256];
extern uint16_t masterbyteCount;
extern int8_t   masterRSSI;
extern uint8_t  slavePacket[256];
extern uint16_t slavebyteCount;
extern int8_t   slaveRSSI;

/*******************************全局函数的声明*******************************/
extern void GfskApp_InitCAN_LIN(void);
extern void GfskApp_StartLM(void);

extern void app_sniffer_init(link_monitor_para_t* LMparaArry, link_monitor_para_t LMpara);
extern uint8_t app_sniffer_NextChannel(link_monitor_para_t* LMpara, uint8_t id);
extern uint8_t app_sniffer_NextChannel(link_monitor_para_t* LMpara, uint8_t id);
extern void app_sniffer_SetGENFSK(uint8_t id);
extern void app_sniffer_can_rx(const hal_can_pdu_st * pdata);
extern void app_sniffer_can_tx(uint8_t* pData,uint8_t length);


#endif
