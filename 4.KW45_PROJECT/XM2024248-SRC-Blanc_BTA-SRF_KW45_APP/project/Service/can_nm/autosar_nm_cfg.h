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
#define  NMCHANNELNUM      1

#ifndef STD_ON
    #define STD_ON      1
    #define STD_OFF     0
#endif

//模块通用回调函数类型
typedef void (*CANNM_CALLVOID)();


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




#define N_ImmediateNM_TIMES                         10
//定时器枚举
typedef union{
    unsigned short data[8];
    struct{
        unsigned short T_REPEAT_MSG;
        unsigned short T_NM_TIMEOUT;
        unsigned short T_WAIT_BUS_SLEEP;
        unsigned short T_STARTX_APPFRAME;
        unsigned short T_NM_IMMEDIATECYCLETIME;
        unsigned short T_NM_MessageCycle;
        unsigned short T_START_NM_TX;
        unsigned short T_WakeUp;

    }bytes;
}CANNM_CFG_TIMERType;

//基本参数结构体
typedef struct{
    unsigned char relchannelnum;
    unsigned short baseid;
    unsigned char nodeid;
    unsigned char msgdlc;
    unsigned char userdataoffset;
    unsigned char userdatalen;
    unsigned char N_RepeatMsg;
    unsigned char N_RepeatMessage;
    unsigned char CANNM_PDU_CBV_REPEATMESSAGEREUEST;
    unsigned char CANNM_PDU_CBV_NMCOORDINATORSLEEPREADY;
    unsigned char CANNM_PDU_CBV_ACTIVEWAKEUP;
    unsigned char CANNM_PDU_CBV_PARTUAKNETWORKINFOMAT;/*PNI MSK*/
}CANNM_CFG_INFOType;

//功能函数列表
typedef struct{
    nm_callcanif CANIf_Transmit;
    CANNM_CALLVOID Nm_BusSleepMode;
    CANNM_CALLVOID Nm_NetWorkMode;
    CANNM_CALLVOID Nm_NetWorkStartIndication;
    CANNM_CALLVOID Nm_NetWorkReadySleepState;
    CANNM_CALLVOID Nm_PrepareBusSleepMode;
    CANNM_CALLVOID CanSM_TxTimeoutException;
    CANNM_CALLVOID CanPnc_effective;
}CANNM_CFG_FUNCALLType;

//报文格式结构体
typedef struct{
    unsigned char CBVByte;
    unsigned char RepeatMessageRequstbit;
    unsigned char ActiveWakeupBit;
    unsigned char IDSW;
    unsigned char IDByte;
}CANNM_CFG_PDUType;

typedef struct{
	unsigned char	CanNmPnFilterMaskByteValue;
}CanNm_PnFilterMaskByteType;
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
void cannm_get_node_id(void);
#endif
extern const CANNM_CFG_TIMERType cannm_cfg_timer[NMCHANNELNUM];
extern  CANNM_CFG_INFOType  cannm_cfg_info[NMCHANNELNUM];
extern const CANNM_CFG_FUNCALLType cannm_cfg_fun[NMCHANNELNUM];
extern const CANNM_CFG_PDUType cannm_cfg_pdu[NMCHANNELNUM];
extern CanNm_PnFilterMaskByteType cannm_FilterMask[NMCHANNELNUM][6];
#endif
