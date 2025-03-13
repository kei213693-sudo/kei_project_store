/*
*Copyright(c)2022,宁波福尔达智能科技股份有限公司
*Allrightsreserved.
*文件名称：autosar_nm.h
*摘要：定义autosar网络管理头文件
*当前版本：V0.0.1，易源东，2022.04.14，首版
*历史版本：Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
*/

#ifndef AUTOSAR_NM_H_
#define AUTOSAR_NM_H_
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "autosar_nm_cfg.h"

#define NM_E_Ok         (0u)
#define NM_E_NOT_Ok     (1u)
#define NM_E_NOSPPURT   (2u)

#define CANNM_USER_DATA_ENABLED (1u)
#define CANNM_PDU_CBV_PNinfobit (6u)

#ifndef NM_ENABLE
#define NM_ENABLE               (1u)
#endif

#ifndef NM_DISABLE
#define NM_DISABLE              (0u)
#endif

/*
*类型定义
*/
typedef unsigned char NetworkHandleType;
typedef unsigned char PduIdType;
//typedef unsigned char * PduInfoType;
typedef unsigned char Std_ReturnType;
typedef unsigned char * Std_VersionInfoType;

typedef enum{
    nmTure = 0,
    nmFalse
}BOOL;

typedef enum{
    Disable     = 0,
    Enable
}Nm_EnableType;

typedef enum
{//模式列表
    NM_BUS_SLEEP = 0,//默认休眠
    NM_NETWORK,
    NM_PREPAREBUSSLEEP
}Nm_ModeType;

typedef enum
{//NM模式状态列表
    NM_REPEATMESSAGE = 0,
    NM_NORMALOPERATION,
    NM_READYSLEEP
}Nm_StateType;

typedef enum
{
	CANNM_PDU_BYTE_0 = 0x00,
	CANNM_PDU_BYTE_1 = 0x01,
	CANNM_PDU_OFF = 0xFF
} CanNm_PduBytePositionType;

typedef enum
{
    CANNM_PDU_CBV_REPEATMESSAGEREUEST       =   0x01,
    CANNM_PDU_CBV_NMCOORDINATORSLEEPREADY   =   0x08,
    CANNM_PDU_CBV_ACTIVEWAKEUP              =   0x10,
    CANNM_PDU_CBV_PARTUAKNETWORKINFOMAT     =   0x40
}CanNm_PduByteCBVBITType;

//定时器控制结构体
typedef struct{
    union{
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
    }timercount;//时间计数值
    union{
        unsigned char data;
        struct{
            unsigned char repeatmsg         :1;
            unsigned char nmtimeout         :1;
            unsigned char waitbussleep      :1;
            unsigned char startx_app        :1;

            unsigned char immediatecycle    :1;
            unsigned char msgcycle          :1;
            unsigned char startnmtx         :1;
            unsigned char wakeup            :1;
        }bits;
    }timerflag;//时间计数标志
    union{
        unsigned char data;
        struct{
            unsigned char repeatmsg         :1;
            unsigned char nmtimeout         :1;
            unsigned char waitbussleep      :1;

            unsigned char startx_app        :1;
            unsigned char immediatecycle    :1;
            unsigned char msgcycle          :1;
            unsigned char startnmtx         :1;
            unsigned char wakeup            :1;
        }bits;
    }timeoutflag;//超时标志，累计计数达到对应状态的超时时间
}Nm_TimerType;

typedef struct {
BOOL T1;
}CanNm_ConfigType;

typedef struct{
BOOL T2;
}CanNm_GolbleConfigType;


typedef enum{
    NetWorkRelease  = 0,
    PassiveAwakeUp  ,
    AcitveAwakeUp   ,
}CanNm_NetWorkStateType;

typedef struct{
    BOOL CanNmActiveWakeupBitEnabled;
    BOOL CanNmAllNmMessagesKeepAwake;
    BOOL CanNmBusLoadReductionActive;

}CanNm_ChannelConfigType;

typedef struct{
    BOOL T;
    BOOL CanNmPnEnabledDef;
}CanNm_PnInfoType;


typedef struct{
    unsigned int id;//can id
    unsigned char len;
    unsigned char data[64];//支持can fd的报文
}PduInfoType;

//控制标志结构体
typedef union{
    unsigned short data;
    struct{
        unsigned short passivewakeup         :1;//被动唤醒标志
        unsigned short activewakeup          :1;//主动唤醒标志
        unsigned short RemoteSleepInd        :1;
        unsigned short repeatmsgrequset      :1;

        unsigned short TxFlag                :1;//待发送标志
        unsigned short RxFlag                :1;//接收处标志
        unsigned short TxConfire             :1;//发送确认标志
        unsigned short PnAvailability        :1;

        unsigned short repeatmsgcountout     :1;
        unsigned short repeatmsgrevc         :1;
        unsigned short res0                  :2;

        unsigned short SleepReady            :1;
        unsigned short res1                  :3;
    }bit;
}NmIndType;

typedef enum{
    NM_FF_NO,
    NM_FF_YES
}NM_first_frame;

typedef struct
{//nm控制列表结构体
    Nm_TimerType timer;//定时器参数
    Nm_ModeType  mode;//3种工作模式
    Nm_StateType state;//NM模式的3种状态
    unsigned short msgcount;//消息发送计数
    unsigned short repeatmsgcount;//重发消息计数
    NM_first_frame firstframe;//起到一个使能发送的标志位作用
    PduIdType    nodeid;//报文id，u8类型
    NmIndType    indstate;//标志信息
    PduInfoType  TxPdu;//tx buff
    PduInfoType  RxPdu;//rx buff
    PduIdType    TxPduId;//发送PDU标识符，u8类型，未用到
//   Nm_EnableType PduComState;
    CanNm_ChannelConfigType ChannelCfgInfo;//参数信息
    CanNm_PnInfoType    PNInfo;//PNC过滤使能标志
    CanNm_PnFilterMaskByteType* FilterMask;//PNC掩码实际数值
//    CanNm_NetWorkStateType  netstate;
    NetworkHandleType relchannelnum;//u8类型，通道号
}CanNm_InfoType;

void CanNm_Init(void);//外部调用
void CanNm_Timer(void);//外部调用
void CanNm_MainFunction(void);//外部调用
void CanNm_RxIndication(unsigned char Channel,unsigned int id,unsigned char len,const unsigned char* PduInfoPtr);//外部调用

Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle);//被动唤醒
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle);//主动唤醒
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle);//主动唤醒-释放

Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType *nmStatePtr, Nm_ModeType *nmModePtr);//外部调用
NM_first_frame CanNm_Get_first_frame(NetworkHandleType nmChannelHandle);//外部调用

#if 0//屏蔽未被调用api
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const unsigned char* nmUserDataPtr,unsigned char length );
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, unsigned char *nmUserDataPtr);

Std_ReturnType  CanNm_DisableCommunication(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle);

unsigned int CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, unsigned char *nmNodeIdPtr);
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, unsigned char *nmPduDataPtr);

Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, BOOL *nmRemoteSleepIndPtr);
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, BOOL nmSleepReadyBit);
void CanNm_TxConfirmation(PduIdType TxPduId);
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle);
#endif

#endif
