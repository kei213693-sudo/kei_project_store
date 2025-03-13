/*
*Copyright(c)2022,宁波福尔达智能科技股份有限公司
*Allrightsreserved.
*文件名称：autosar_nm.h
*摘要：定义autosar网络管理头文件
*当前版本：V0.1.1，易源东，2022.04.14，首版
*          变更项：
*               1.宏开关增加说明
*               2.增加NM Autosar contrl bit vector 配置
*               3.增加网络管理报文偏移设置项
*               4.增加回调函数类型说明
*历史版本：
*          V0.0.1，易源东，2022.04.14，首版
*          V0.1.0，易源东，2022.08.01，合众项目使用BUG修复
*
*/

#ifndef AUTOSAR_NM_CFG_H
#define AUTOSAR_NM_CFG_H
#include <stdint.h>
//执行网络管理的通道数量  目前仅验证1各通道情况 若有多通道需求请联系模块开发人员
#define  NM_CHANNEL_NUM      1

#ifndef STD_ON
    #define STD_ON      1
    #define STD_OFF     0
#endif

typedef void (*CANNM_CALLVOID)();//模块通用回调函数类型

/*
*函数介绍：发送报文回调函数
*参数：
*       [in]    ChannelNum  真实的CAN通道 值为autosar_nm_cfg.c中 cannm_cfg_info.relchannelnum
*       [in]    Id          CAN报文id 值为autosar_nm_cfg.c中 cannm_cfg_info.baseid+cannm_cfg_info.nodeid
*       [in]    datadlc     报文长度
*       [in]    pdu         报文数据
*返回值：返回结果
*备注：
*/
typedef unsigned char (* nm_callcanif)(unsigned char ChannelNum,unsigned int Id,unsigned char datadlc,unsigned char * pdu);

/*
*宏开关定义
*/
#ifdef uint8
typedef unsigned char uint8;
#endif

//网络管理功能开关
#define        CanNmPduCBVPNIBit                STD_ON               //contrl bit vecotr PNI 位功能使能控制
#define        CanNmPduCBVAwakeUpBit            STD_ON               //contrl bit vecotr WakeUp 位功能使能控制
#define        CanNmPduCBVReadySleepBit         STD_OFF              //contrl bit vecotr ReadySleep 位功能使能控制
#define        CanNmPduCBVRepeatMsgBit          STD_ON               //contrl bit vecotrRepeatMsgBit 位功能使能控制
#define        CanNmPassiveModeEnabled          STD_OFF              //被动模式开关
#define        CanNmGlobalPnSupport             STD_ON               //全局PN过滤开关
#define        CanNmBusLoadReductionEnabled     STD_ON               //总线降负载功能开关
#define        NmBusSynchronizationEnabled      STD_ON               //总线同步开关
#define        CanNmComControlEnabled           STD_ON               //通讯控制功能
#define        CanNmComUserDataSupport          STD_ON
#define        CanNmImmediateTxconfEnabled      STD_ON
#define        CanNmImmediateRestartEnabled     STD_ON
#define        CanNmNodeDetectionEnabled        STD_ON
#define        CanNmRemoteSleepIndEnabled       STD_ON
#define        CanNmDevErrorDetect              STD_ON
#define        CanNmNodeIdEnabled               STD_ON
#define        CanNmPduRxIndicationEnabled      STD_ON
#define        CanNmPnEiraCalcEnabled           STD_ON
#define        CanNmRepeatMsgIndEnabled         STD_ON
#define        CanNmStateChangeIndEnabled       STD_ON
#define        CanNmUserDataEnabled             STD_ON
#define        CanNmVersionInfoApi              STD_ON
#define        CanNmPnEiraRxNSduRef             STD_ON

#define        CANNM_NODE_ID_ENABLE             STD_ON

//时间信息列表
typedef union{
    unsigned short data[8];
    struct{
        unsigned short T_REPEAT_MSG;//重复报文时间
        unsigned short T_NM_TIMEOUT;//网络管理超时
        unsigned short T_WAIT_BUS_SLEEP;//等待休眠
        unsigned short T_STARTX_APPFRAME;//app报文发送时间
        unsigned short T_NM_IMMEDIATECYCLETIME;//pdu快发时间
        unsigned short T_NM_MessageCycle;//pdu正常时间
        unsigned short T_START_NM_TX;//nm发送事件
        unsigned short T_WakeUp;//唤醒时间
    }bytes;
}CANNM_CFG_TIMERType;

//基本参数
typedef struct{
    unsigned char relchannelnum;//nm通道
    unsigned short baseid;//基地址
    unsigned char nodeid;//节点地址
    unsigned char msgdlc;//报文长度
    unsigned char userdataoffset;//数据偏移
    unsigned char userdatalen;//数据长度
    unsigned char N_RepeatMsg;//快发帧数
    unsigned char N_RepeatMessage;//重复消息帧数
    unsigned char CANNM_PDU_CBV_REPEATMESSAGEREUEST;//CBV第一位掩码
    unsigned char CANNM_PDU_CBV_NMCOORDINATORSLEEPREADY;//CBV第二位掩码
    unsigned char CANNM_PDU_CBV_ACTIVEWAKEUP;//CBV第三位掩码
    unsigned char CANNM_PDU_CBV_PARTUAKNETWORKINFOMAT;//CBV第四位PNI掩码
}CANNM_CFG_INFOType;

//操作函数列表
typedef struct{
    nm_callcanif CANIf_Transmit;//发送报文api
    CANNM_CALLVOID Nm_BusSleepMode;//通知休眠模式api
    CANNM_CALLVOID Nm_NetWorkMode;//通知网络模式api
    CANNM_CALLVOID Nm_NetWorkStartIndication;//通知网络模式开始api
    CANNM_CALLVOID Nm_NetWorkReadySleepState;//通知进入准备睡眠模式api
    CANNM_CALLVOID Nm_PrepareBusSleepMode;//通知预睡眠模式api
    CANNM_CALLVOID CanSM_TxTimeoutException;//通知发送超时api
    CANNM_CALLVOID CanPnc_effective;//通知接受到的PNC有效api
}CANNM_CFG_FUNCALLType;

//CBVByte位参数
typedef struct{
    unsigned char CBVByte;//CBVByte字段索引，bytes1
    unsigned char RepeatMessageRequstbit;//bytes1的bit0：重复消息请求状态
    unsigned char ActiveWakeupBit;//bytes1的bit3：主动激活标志，或者说网络管理统筹睡眠标志
    unsigned char IDSW;//节点开关使能（未用到）
    unsigned char IDByte;//节点ID字段索引，bytes0
}CANNM_CFG_PDUType;

typedef struct{
	unsigned char	CanNmPnFilterMaskByteValue;
}CanNm_PnFilterMaskByteType;

#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
void cannm_get_node_id(void);
#endif

extern const CANNM_CFG_TIMERType cannm_cfg_timer[NM_CHANNEL_NUM];
extern  CANNM_CFG_INFOType  cannm_cfg_info[NM_CHANNEL_NUM];
extern const CANNM_CFG_FUNCALLType cannm_cfg_fun[NM_CHANNEL_NUM];
extern const CANNM_CFG_PDUType cannm_cfg_pdu[NM_CHANNEL_NUM];
extern CanNm_PnFilterMaskByteType cannm_FilterMask[NM_CHANNEL_NUM][6];

#endif
