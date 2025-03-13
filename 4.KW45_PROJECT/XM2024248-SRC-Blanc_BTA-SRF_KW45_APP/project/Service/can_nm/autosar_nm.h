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

//状态枚举
typedef enum{
    NM_BUS_SLEEP = 0,
    NM_NETWORK,
    NM_PREPAREBUSSLEEP
}Nm_ModeType;

typedef enum{
    Disable     = 0,
    Enable
}Nm_EnableType;

//NM_NETWORK子状态枚举
typedef enum{
    NM_REPEATMESSAGE = 0,
    NM_NORMALOPERATION,
    NM_READYSLEEP
}Nm_StateType;

typedef enum {
	CANNM_PDU_BYTE_0 = 0x00,
	CANNM_PDU_BYTE_1 = 0x01,
	CANNM_PDU_OFF = 0xFF
} CanNm_PduBytePositionType;

typedef enum{
    CANNM_PDU_CBV_REPEATMESSAGEREUEST       =   0x01,
    CANNM_PDU_CBV_NMCOORDINATORSLEEPREADY   =   0x08,
    CANNM_PDU_CBV_ACTIVEWAKEUP              =   0x10,
    CANNM_PDU_CBV_PARTUAKNETWORKINFOMAT     =   0x40
}CanNm_PduByteCBVBITType;

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
    }timercount;
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
    }timerflag;
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
    }timeoutflag;
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
    unsigned int id;
    unsigned char len;
    unsigned char data[64];
}PduInfoType;

typedef union{
    unsigned short data;
    struct{
        unsigned short passivewakeup         :1;
        unsigned short activewakeup          :1;
        unsigned short RemoteSleepInd        :1;
        unsigned short repeatmsgrequset      :1;

        unsigned short TxFlag                :1;//待发送
        unsigned short RxFlag                :1;//接收处
        unsigned short TxConfire             :1;
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

typedef struct {
    Nm_TimerType timer;
    Nm_StateType state;
    Nm_ModeType  mode;//nm状态
    unsigned short msgcount;
    unsigned short repeatmsgcount;
    NM_first_frame firstframe;
    PduIdType    nodeid;
    NmIndType    indstate;
    PduInfoType  TxPdu;
    PduInfoType  RxPdu;
    PduIdType    TxPduId;
//   Nm_EnableType PduComState;
    CanNm_ChannelConfigType ChannelCfgInfo;
    CanNm_PnInfoType    PNInfo;
    CanNm_PnFilterMaskByteType* FilterMask;
//    CanNm_NetWorkStateType  netstate;
    NetworkHandleType relchannelnum;
}CanNm_InfoType;

void CanNm_Init();//初始化
void CanNm_Timer();//定时器定时处理标志信息
void CanNm_MainFunction(void);//nm状态机主体
//接收到nm报文进行处理
void CanNm_RxIndication( unsigned char Channel,unsigned int id,unsigned char len,const unsigned char* PduInfoPtr);

Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const unsigned char* nmUserDataPtr,unsigned char length );
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, unsigned char *nmUserDataPtr);
NM_first_frame CanNm_Get_first_frame(NetworkHandleType nmChannelHandle);
Std_ReturnType  CanNm_DisableCommunication(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType *nmStatePtr, Nm_ModeType *nmModePtr);
unsigned int CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle);
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle);


#endif
