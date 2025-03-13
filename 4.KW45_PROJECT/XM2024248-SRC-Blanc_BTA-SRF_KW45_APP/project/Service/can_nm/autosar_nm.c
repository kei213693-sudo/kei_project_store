/*
*Copyright(c)2022,宁波福尔达智能科技股份有限公司
*Allrightsreserved.
*文件名称：autosar_nm.h
*摘要：定义autosar网络管理头文件
*当前版本：V0.0.1，易源东，2022.04.14，首版
*历史版本：Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
*/
#include "autosar_nm.h"
#include "ComSM.h"
#include "hal_test.h"
#include "autosar_nm_cfg.h"
#include "uds_priv.h"
#include "ComBusoff.h"
#define NA_DATA_ADDR_OFFSET 2
static CanNm_InfoType cannminfo[NMCHANNELNUM];
typedef uint8_t NMCOMCTRL_type;
static NMCOMCTRL_type nmcomctrlflag[NMCHANNELNUM];//控制对应通道网络管理报文通讯的使能/失能

#define NM_TEST_FLAG (0)
#if NM_TEST_FLAG
static uint8_t NM_ST_TEST[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif
/*void CanNm_Init(const CanNm_ConfigType *cannmConfigPtr)
{
    memset(cannmConfigPtr,0,sizeof(cannmConfigPtr));
}*/

/*
 *函数介绍：激活nmChannelHandle被动唤醒，远程唤醒
 *参数：nmChannelHandle 通道号
 *返回值：返回结果
 *备注：暂定 检查当前状态异常（非NM_BUS_SLEEP NM_PREPAREBUSSLEEP）时返回NM_E_NotOk 标识调用异常 ；否者切换状态为NM_REPEAT_MSG_PDUTX
 */
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType res = NM_E_NOT_Ok; // 初始化返回状态为非成功
    uint8_t bits = 0;
    uint8_t i = 0;
    // 检查当前通道是否处于睡眠模式或准备进入睡眠模式
    if (cannminfo[nmChannelHandle].mode == NM_BUS_SLEEP || cannminfo[nmChannelHandle].mode == NM_PREPAREBUSSLEEP)
    {
        cannminfo[nmChannelHandle].indstate.data &= 0x80;          // 重置指示状态的数据部分
        cannminfo[nmChannelHandle].indstate.bit.passivewakeup = 1; // 设置被动唤醒标志
        cannminfo[nmChannelHandle].repeatmsgcount = 0;
        // 遍历清除所有通道定时器所有通道
        for (i = 0; i < NMCHANNELNUM; i++)
        {
            for (bits = 0; bits < 8; bits++)
            {
                cannminfo[i].timer.timercount.data[bits] = 0;
            }
        }
#if NM_TEST_FLAG
        /*测试发送被动唤醒状态*/
        NM_ST_TEST[0] = 0X01;
        NM_ST_TEST[1] = 0X03;
        CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
        res = NM_E_Ok; // 设置返回状态为成功
    }

    return res; // 返回执行状态
}

/*
 *函数介绍：nmChannelHandle通道网络请求主动唤醒
 *参数：nmChannelHandle 通道号
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType res = NM_E_Ok; // 初始化返回状态为成功
    uint8_t bits = 0;
    uint8_t i = 0;
#if !CANNM_PASSIVE_MODE_ENABLED
    // 检查网络模式，如果不是网络模式，则设置激活唤醒标志
    if (cannminfo[nmChannelHandle].mode != NM_NETWORK)
    {
        cannminfo[nmChannelHandle].indstate.data &= 0x80;         // 重置指示状态的数据部分
        cannminfo[nmChannelHandle].indstate.bit.activewakeup = 1; // 设置激活唤醒标志
        // 遍历清除所有通道定时器所有通道
        for (i = 0; i < NMCHANNELNUM; i++)
        {
            for (bits = 0; bits < 8; bits++)
            {
                cannminfo[i].timer.timercount.data[bits] = 0;
            }
        }
#if NM_TEST_FLAG
        NM_ST_TEST[0] = 0X02;
        NM_ST_TEST[1] = 0X02;
        CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
    }
    else
        res = NM_E_NOT_Ok; // 如果是网络模式，则设置返回状态为失败

    if (nmTure == cannminfo[nmChannelHandle].PNInfo.CanNmPnEnabledDef && NM_NETWORK == cannminfo[nmChannelHandle].mode)
    {
        cannminfo[nmChannelHandle].indstate.bit.activewakeup = 1; // 设置激活唤醒标志
        memset(&cannminfo[nmChannelHandle].timer, 0, sizeof(cannminfo[nmChannelHandle].timer));
        cannminfo[nmChannelHandle].state = NM_REPEATMESSAGE;

        cannminfo[nmChannelHandle].timer.timerflag.bits.repeatmsg = 1;
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.repeatmsg = 0;
        cannminfo[nmChannelHandle].timer.timercount.bytes.T_REPEAT_MSG = 0;

        cannminfo[nmChannelHandle].indstate.bit.repeatmsgcountout = 0;
        cannminfo[nmChannelHandle].msgcount = 0;
        cannminfo[nmChannelHandle].timer.timerflag.bits.immediatecycle = 0;
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.immediatecycle = 1;
#if NM_TEST_FLAG
        NM_ST_TEST[0] = 0X02;
        NM_ST_TEST[1] = 0X02;
        CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
        res = NM_E_Ok;
    }
#endif

    return res; // 返回执行状态
}

/*
 *函数介绍：nmChannelHandle释放通道
 *参数：nmChannelHandle 通道号
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType res = NM_E_Ok;
    uint8_t bits = 0;
    uint8_t i = 0;
#if !CANNM_PASSIVE_MODE_ENABLED

    // 清除指定网络通道的指示状态，禁用唤醒功能
    cannminfo[nmChannelHandle].indstate.data &= 0x80; // 重置指示状态的数据部分
    cannminfo[nmChannelHandle].indstate.bit.activewakeup = 0;
    cannminfo[nmChannelHandle].indstate.bit.passivewakeup = 0;
    memset(&cannminfo[nmChannelHandle].timer, 0, sizeof(cannminfo[nmChannelHandle].timer));
    cannminfo[nmChannelHandle].state = NM_READYSLEEP;
    cannminfo[nmChannelHandle].timer.timeoutflag.bits.nmtimeout = 0;
    cannminfo[nmChannelHandle].timer.timerflag.bits.nmtimeout = 1;
    cannminfo[nmChannelHandle].timer.timercount.bytes.T_NM_TIMEOUT = 0;
    // 调用网络准备睡眠状态的函数
    cannm_cfg_fun[nmChannelHandle].Nm_NetWorkReadySleepState();
    // 下一行代码被注释，用于在未来可能的支持下改变网络模式到BUS_SLEEP
    // 遍历清除所有通道定时器所有通道
    for (i = 0; i < NMCHANNELNUM; i++)
    {
        for (bits = 0; bits < 8; bits++)
        {
            cannminfo[i].timer.timercount.data[bits] = 0;
        }
    }
#if NM_TEST_FLAG
    NM_ST_TEST[0] = 0X03;
    NM_ST_TEST[1] = 0X05;
        CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
#endif
    return res;
}

#if 0
/*
*函数介绍：nmChannelHandle通讯禁止
*参数：nmChannelHandle 通道号
*返回值：返回结果
*备注：28服务禁用网络管理报文
*/
Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType res =  NM_E_Ok;

#if CANNM_COM_CONTROL_ENABLED
    if(CANNM_PASSIVE_MODE_ENABLED   ||
    cannminfo[nmChannelHandle].mode == NM_NETWORK)
    {
        res =  NM_E_NOT_Ok;
    }
    else{
        cannminfo[nmChannelHandle].PduComState = Disable;
        res =  NM_E_Ok;
    }
#endif
    return res;
}

/*
*函数介绍：nmChannelHandle通讯使能
*参数：nmChannelHandle 通道号
*返回值：返回结果
*备注：28服务启用网络管理报文
*/
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle)
{
 Std_ReturnType res =  NM_E_Ok;

#if CANNM_COM_CONTROL_ENABLED
    if(CANNM_PASSIVE_MODE_ENABLED   ||
    (cannminfo[nmChannelHandle].mode == NM_NETWORK &&
    cannminfo[nmChannelHandle].PduComState == Enable;))
    {
        res =  NM_E_NOT_Ok
    }
    else{
        cannminfo[nmChannelHandle].PduComState = Enable;
    }

#endif
    return res;
}
#endif

/*
 *函数介绍：设置nmChannelHandle通道first_frame数据
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：
 */
void CanNm_Set_first_frame(NetworkHandleType nmChannelHandle,NM_first_frame first_frame)
{
    cannminfo[nmChannelHandle].firstframe = first_frame;
}

/*
 *函数介绍：获取nmChannelHandle通道first_frame数据
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：
 */
NM_first_frame CanNm_Get_first_frame(NetworkHandleType nmChannelHandle)
{
    return cannminfo[nmChannelHandle].firstframe;
}

/*
 *函数介绍：设置nmChannelHandle通道txpdu userdata数据
 *参数：nmChannelHandle 通道号
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle, const unsigned char *nmUserDataPtr, const unsigned char length)
{
    Std_ReturnType res = NM_E_Ok; // 初始化返回状态为成功

    // 获取用户数据在PDU中的偏移量和长度
    uint8_t CanNmPduUserDataByteOffset = cannm_cfg_info->userdataoffset;
    uint8_t CanNmPduUserDataByteLength = cannm_cfg_info->userdatalen;

#if CANNM_USER_DATA_ENABLED && !CANNM_PASSIVE_MODE_ENABLED
    // 只有在用户数据功能启用且非被动模式下才处理用户数据

    // 检查用户数据长度是否符合要求
    if (length >= CanNmPduUserDataByteLength)
        memcpy(&(cannminfo[nmChannelHandle].TxPdu.data[CanNmPduUserDataByteOffset]), nmUserDataPtr, CanNmPduUserDataByteLength); // 复制用户数据到指定的网络通道
    else
        res = NM_E_NOT_Ok; // 用户数据长度不满足要求，设置返回状态为失败
#endif

    return res; // 返回执行状态
}

/*
 *函数介绍：获取nmChannelHandle通道txpdu userdata数据
 *参数：nmChannelHandle 通道号
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle, unsigned char *nmUserDataPtr)
{
    Std_ReturnType res = NM_E_Ok; // 初始化返回状态为成功

    // 获取用户数据在PDU中的偏移量
    uint8_t CanNmPduUserDataByteOffset = cannm_cfg_info->userdataoffset;
    // 获取用户数据的长度
    uint8_t CanNmPduUserDataByteLength;

    CanNmPduUserDataByteLength = cannm_cfg_info->userdatalen;

#if CANNM_USER_DATA_ENABLED && !CANNM_PASSIVE_MODE_ENABLED
    // 当用户数据功能启用且非被动模式时，执行用户数据的获取
    if (sizeof(nmUserDataPtr) >= CanNmPduUserDataByteLength)
        memcpy(nmUserDataPtr, &cannminfo[nmChannelHandle].RxPdu.data[CanNmPduUserDataByteOffset], CanNmPduUserDataByteLength); // 复制用户数据到传入的缓冲区
    else
        res = NM_E_NOT_Ok; // 如果提供的缓冲区大小不足以存放用户数据，则设置返回状态为失败
#endif

    return res;
}


/*
 *函数介绍：报文数据发送
 *参数：nmChannelHandle 数据发送
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_Transmit(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType res; // 函数执行状态变量

    unsigned short CBVByte = 0; // 控制字节变量，用于构建将要发送的PDU

#if 1 // 主控制块，根据宏定义决定是否启用下列功能

    // 根据配置，构建CBVByte中的重复消息请求位
#if CanNmPduCBVRepeatMsgBit
    CBVByte |= cannminfo[nmChannelHandle].indstate.bit.repeatmsgrequset << cannm_cfg_pdu[nmChannelHandle].RepeatMessageRequstbit;
#endif

    // 根据配置，构建CBVByte中的就绪/睡眠位
#if CanNmPduCBVReadySleepBit
    CBVByte |= cannminfo[nmChannelHandle].indstate.bit.SleepReady << CANNM_PDU_CBV_ReadySleepbit;
#endif

    // 根据配置，构建CBVByte中的唤醒位
#if CanNmPduCBVAwakeUpBit
#ifdef CJB_CHANGE
    CBVByte |= cannminfo[nmChannelHandle].indstate.bit.activewakeup << cannm_cfg_pdu[nmChannelHandle].ActiveWakeupBit;
#else
    CBVByte = 0x00; // 如果没有定义CJB_CHANGE，则清空CBVByte
#endif
#endif

    // 根据配置，构建CBVByte中的PN信息位
#if CanNmPduCBVPNIBit
    if(nmTure == cannminfo[nmChannelHandle].indstate.bit.PnAvailability)
    {
        CBVByte |= 1 << CANNM_PDU_CBV_PNinfobit;
    }
#endif

#endif // 结束主控制块

    // 根据配置，设置消息ID
#if CanNmNodeIdEnabled
    cannminfo[nmChannelHandle].TxPdu.data[cannm_cfg_pdu[nmChannelHandle].IDByte] = cannminfo[nmChannelHandle].TxPdu.id & 0xff;
#endif

    // 设置控制字节到PDU中
    cannminfo[nmChannelHandle].TxPdu.data[cannm_cfg_pdu[nmChannelHandle].CBVByte] = CBVByte;

    // 调用CAN接口进行消息发送
    if (NM_ENABLE == nmcomctrlflag[nmChannelHandle])
    {
        res = cannm_cfg_fun[nmChannelHandle].CANIf_Transmit(cannminfo[nmChannelHandle].relchannelnum, cannminfo[nmChannelHandle].TxPdu.id, cannminfo[nmChannelHandle].TxPdu.len, cannminfo[nmChannelHandle].TxPdu.data);
        if (NM_FF_NO == CanNm_Get_first_frame(nmChannelHandle) && 0u == res)
        {
            CanNm_Set_first_frame(nmChannelHandle,NM_FF_YES);
        }
    }

    // 根据配置，设置发送确认标志
#if !CANNM_IMMEDIATE_TXCONF_ENABLED
    cannminfo[nmChannelHandle].indstate.bit.TxConfire = 1;
#endif

    return res; // 返回函数执行状态
}

/*
 *函数介绍：从最近收到的NM PDU中获取节点标识符。
 *参数：nmChannelHandle Can通道 nmNodeIdPtr ID信息
 *返回值：返回结果
 *备注：
 */
unsigned int CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle)
{
    //Std_ReturnType res = NM_E_NOT_Ok; // 初始化返回状态为成功
    unsigned int nmNodeIdPtr;
#if CANNM_NODE_ID_ENABLE

    // 如果节点标识符功能使能，则根据网络通道句柄获取节点标识符，并且只保留低8位
    nmNodeIdPtr = cannminfo[nmChannelHandle].RxPdu.id & 0x000000ff;
#endif
    return nmNodeIdPtr; // 返回执行状态
}

/*
 *函数介绍：获取本节点的Nodeid。
 *参数：nmChannelHandle Can通道 nmNodeIdPtr ID信息
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle, unsigned char *nmNodeIdPtr)
{
    Std_ReturnType res = NM_E_NOT_Ok; // 初始化返回状态为成功

#if CANNM_NODE_ID_ENABLE

    *nmNodeIdPtr = cannminfo[nmChannelHandle].nodeid; // 将指定网络通道的节点ID赋值给输出参数
    res = NM_E_Ok;                                    // 初始化返回状态为成功
#endif
    return res; // 返回操作结果状态码
}

/*
 *函数介绍：请求重复报文发送。
 *参数：nmChannelHandle Can通道 nmNodeIdPtr ID信息
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType res = NM_E_Ok; // 初始化返回值为操作成功

#if CanNmNodeDetectionEnabled
    // 如果节点检测功能启用，则设置重复消息请求指示状态为1
    cannminfo[nmChannelHandle].indstate.bit.repeatmsgrequset = 1;
#endif

    return res; // 返回操作结果状态码
}

/*
 *函数介绍：获取pdu信息。M_R
 *参数：nmChannelHandle Can通道 nmNodeIdPtr ID信息
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, unsigned char *nmPduDataPtr)
{
    Std_ReturnType res = NM_E_Ok; // 初始化返回状态为成功

    // 检查是否启用了节点检测、用户数据或节点ID功能，如果启用，则从接收PDU中复制数据
    if (CanNmNodeDetectionEnabled || CanNmUserDataEnabled || CanNmNodeIdEnabled)
    {
        memcpy(nmPduDataPtr, cannminfo[nmChannelHandle].RxPdu.data, cannm_cfg_info[nmChannelHandle].msgdlc);
    }
    return res; // 返回执行状态
}

/*
 *函数介绍：获取状态模式信息。
 *参数：nmChannelHandle Can通道 nmStatePtr 状态信息 nmModePtr模式信息
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle, Nm_StateType *nmStatePtr, Nm_ModeType *nmModePtr)
{
    Std_ReturnType res = NM_E_Ok; // 初始化返回值为成功状态

    // 获取并存储网络状态和模式到传入的指针地址
    *nmStatePtr = cannminfo[nmChannelHandle].state;
    *nmModePtr = cannminfo[nmChannelHandle].mode;

    return res; // 返回执行状态
}
/*
void CanNm_GetVersionInfo(Std_VersionInfoType* versioninfo)
{

}*/
/*
 *函数介绍：触发单次PDU Tx。
 *参数：nmChannelHandle Can通道
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType res = NM_E_Ok; // 初始化返回值为操作成功

#if CANNM_BUS_SYNCHRONIZATION_ENABLED && !CANNM_PASSIVE_MODE_ENABLED
    // 检查是否在正确的网络模式下，并尝试设置传输标志为激活状态
    if (cannminfo[nmChannelHandle].mode == NM_NETWORK)
    {
        cannminfo[nmChannelHandle].indstate.bit.TxFlag = 1; // 激活传输标志
    }
    else
    {
        res = NM_E_NOT_Ok; // 如果不在正确的网络模式下，设置返回值为操作不成功
    }

#endif
    return res; // 返回操作结果
}

/*
 *函数介绍：获取睡眠状态
 *参数：nmChannelHandle Can通道 nmRemoteSleepIndPtr远程睡眠状态
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle, BOOL *nmRemoteSleepIndPtr)
{
    Std_ReturnType res = NM_E_NOT_Ok; // 默认返回状态为错误

#if CANNM_REMOTE_SLEEP_IND_ENABLED
    // 如果支持远程睡眠指示功能，将远程睡眠指示的状态赋值给输出参数
    *nmRemoteSleepIndPtr = cannminfo[nmChannelHandle].indstate.bit.RemoteSleepInd;
#endif

    return res; // 返回执行结果
}

/*
 *函数介绍：设置睡眠准备标志位
 *参数：nmChannelHandle Can通道 nmSleepReadyBit睡眠准备标志位值
 *返回值：返回结果
 *备注：
 */
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle, BOOL nmSleepReadyBit)
{
    Std_ReturnType res = NM_E_Ok; // 初始化返回状态为成功

#if CANNM_COORDINATOR_SYNC_SUPPORT
    // 根据传入的睡眠准备位状态，设置网络管理器的睡眠准备位
    cannminfo[nmChannelHandle].indstate.bit.SleepReady = nmSleepReadyBit;
#endif

    return res; // 返回操作结果状态码
}

/*
 *函数介绍：发送确认标识
 *参数：TxPduId 发送PDU标识符
 *返回值：
 *备注：
 */
void CanNm_TxConfirmation(PduIdType TxPduId)
{
    unsigned char i;
#if !CANNM_IMMEDIATE_TXCONF_ENABLED
    // Loop through all network channels to find the node that matches the TxPduId.
    for (i = 0; i < NMCHANNELNUM; i++)
    {
        // If a match is found, set the transmission confirmation flag for that node.
        if (cannminfo[i].nodeid == TxPduId)
        {
            cannminfo[i].indstate.bit.TxConfire = 1;
            break;
        }
    }

#endif
}

/*
 *函数介绍：执行NMPDU（网络管理协议数据单元）过滤算法。
 *参数：Channel 通道索引，指示NMPDU过滤适用的CAN通道。
 *参数：unsigned char 如果PN根据过滤条件被启用，则返回NM_E_Ok；否则返回NM_E_NOT_Ok。
 *返回值：unsigned char 如果PN根据过滤条件被启用，则返回NM_E_Ok；否则返回NM_E_NOT_Ok。
 *备注：
 */
unsigned char CanNm_NMPDUFilterAlgorithm(unsigned char Channel)
{
    unsigned char PNIndex = 0;
    unsigned char PNEnable = 0;
    unsigned char status = NM_E_Ok;
    unsigned char PNInfo[8] = {0};
    unsigned char PNOffset = cannm_cfg_info[Channel].userdataoffset;
    unsigned char PNLen = cannm_cfg_info[Channel].userdatalen;

    // 检查NM是否已初始化
    if (NM_ENABLE == nmcomctrlflag[Channel])
    {
        // 确认PN可用
        if (nmTure == cannminfo[Channel].indstate.bit.PnAvailability)
        {
            // 复制PN信息到PNInfo数组中
            memcpy(PNInfo, &cannminfo[Channel].RxPdu.data[PNOffset], PNLen);

            // 遍历PN信息并应用过滤掩码
            for (PNIndex = 0; PNIndex < PNLen; PNIndex++)
            {
                PNEnable |= ((PNInfo[PNIndex]) & cannminfo[Channel].FilterMask[PNIndex].CanNmPnFilterMaskByteValue);

                // 根据编译选项决定是否执行额外处理
#if(CanNmPnEiraCalcEnabled == STD_ON )//autosar4.3 [SWS_CanNm_00426]
            }
#else
            }
#endif

            // 如果所有PN位都未启用且配置要求非全部NM消息保持唤醒状态，则标记为不正常
            if(PNEnable == 0 || cannminfo[Channel].ChannelCfgInfo.CanNmAllNmMessagesKeepAwake == nmFalse)
            {
                status = NM_E_NOT_Ok;
            }
        }
        else
        {
            // PN不可用时的处理（此处代码逻辑省略，实际应用中可能需要填充）
        }
    }
    else
    {
        // NM未初始化时，直接返回错误状态
        status = NM_E_NOT_Ok;
    }

    // 返回处理状态
    return status;
}

/*
 *函数介绍：处理指定通道的NMPDU接收。
 *参数：Channel 通道索引，指示NMPDU过滤适用的CAN通道。
 *返回值：unsigned char 返回处理状态。NM_E_Ok表示成功，NM_E_NOT_Ok表示失败或条件不满足。
 *备注：
 */
static unsigned char CanNm_RxHandlingNMPDUS(unsigned char Channel)
{
    unsigned char status = NM_E_Ok; // 初始化处理状态为成功

    unsigned char CBVPNI; // 用于存储网络唤醒信息
    // 从接收的PDU中提取网络唤醒信息（CBVPNI）
    CBVPNI = (cannminfo[Channel].RxPdu.data[cannm_cfg_pdu->CBVByte] & cannm_cfg_info[Channel].CANNM_PDU_CBV_PARTUAKNETWORKINFOMAT) >> 6;

    // 检查是否需要保持网络唤醒及CBVPNI的值来决定处理状态
    if (0 == CBVPNI || nmFalse == cannminfo[Channel].ChannelCfgInfo.CanNmAllNmMessagesKeepAwake)
    {
        status = NM_E_NOT_Ok; // 如果不需要保持唤醒且CBVPNI为0，则设置状态为不成功
    } else if (1 == CBVPNI)
    {
        // 如果CBVPNI为1，则调用NMPDU过滤算法进行进一步处理
        status = CanNm_NMPDUFilterAlgorithm(Channel);
    }
    return status; // 返回处理状态
}

/*
 *函数介绍：接收数据
 *参数：TxPduId 发送PDU标识符
 *返回值：
 *备注：
 */
void CanNm_RxIndication(unsigned char Channel, unsigned int id, unsigned char len, const unsigned char *PduInfoPtr)
{
    unsigned char i;
    unsigned char PnFilterSt = nmTure; // 初始化PN过滤状态为启用
    unsigned char status = NM_E_Ok;    // 初始化处理状态为成功
    for (i = 0; i < NMCHANNELNUM; i++) // 遍历所有通道
    {
        // 匹配接收通道
        if (cannminfo[i].relchannelnum == Channel)
        {
            cannminfo[i].RxPdu.id = id;
            cannminfo[i].RxPdu.len = len;
            memcpy(&cannminfo[i].RxPdu.data, PduInfoPtr, len);
            //break; // 找到匹配通道后，更新PDU信息，并退出循环（注释掉了原始代码中的break）
        }

        // 检查是否启用了PN过滤，并处理PN过滤
        if (nmTure == cannminfo[i].PNInfo.CanNmPnEnabledDef)
        {
            status = CanNm_RxHandlingNMPDUS(i);
            if (NM_E_Ok != status)
            {
                PnFilterSt = nmFalse; // PN过滤失败，设置状态为过滤失败
            }
        }

        // 如果PN过滤成功，则标记报文为有效，并调用配置函数
        if (nmTure == PnFilterSt)
        {
            cannminfo[i].indstate.bit.RxFlag = 1; /* PNC正确则接受报文 */
            cannm_cfg_fun->CanPnc_effective();    // 调用配置函数
        }
    }
}

/*
 *函数介绍：使能PN过滤
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：
 */
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle)
{
#if CanNmPnEiraCalcEnabled
    // 设置网络通道的部件号可用状态为可用
    cannminfo[nmChannelHandle].indstate.bit.PnAvailability = nmTure;
    // 默认启用CAN网络管理功能
    cannminfo[nmChannelHandle].PNInfo.CanNmPnEnabledDef = nmTure;
#endif
}

/*
 *函数介绍：bussleepmode主处理函数
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：
 */
static void CanNm_FunBusSleepMode(NetworkHandleType nmChannelHandle)
{
    // 如果接收标志为1，则不执行任何操作
    if (cannminfo[nmChannelHandle].indstate.bit.RxFlag == 1)
    {

        cannm_cfg_fun[nmChannelHandle].Nm_NetWorkStartIndication();
#if CanNmDevErrorDetect // 暂不使用

#endif
    }

    // 检查是否处于被动唤醒或主动唤醒状态
    if (cannminfo[nmChannelHandle].indstate.bit.passivewakeup ||
        cannminfo[nmChannelHandle].indstate.bit.activewakeup)
    {
        // 设置网络模式，更新模式状态和定时器标志
        cannm_cfg_fun[nmChannelHandle].Nm_NetWorkMode();
        cannminfo[nmChannelHandle].mode = NM_NETWORK;

        cannminfo[nmChannelHandle].timer.timerflag.bits.repeatmsg = 1;
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.repeatmsg = 0;
        cannminfo[nmChannelHandle].timer.timercount.bytes.T_REPEAT_MSG = 0;

        // 根据唤醒类型，设置相应的定时器标志
        if (cannminfo[nmChannelHandle].indstate.bit.activewakeup)
        {
            cannminfo[nmChannelHandle].timer.timerflag.bits.immediatecycle = 0;
            cannminfo[nmChannelHandle].timer.timeoutflag.bits.immediatecycle = 1;
#if NM_TEST_FLAG
            NM_ST_TEST[0] = 0X02;
            NM_ST_TEST[1] = 0X02;
            CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
        }
        if (cannminfo[nmChannelHandle].indstate.bit.passivewakeup)
        {
            cannminfo[nmChannelHandle].timer.timerflag.bits.msgcycle = 0;
            cannminfo[nmChannelHandle].timer.timeoutflag.bits.msgcycle = 1;
#if NM_TEST_FLAG
            NM_ST_TEST[0] = 0X01;
            NM_ST_TEST[1] = 0X03;
            CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
        }
        // 重置传输标志
        // cannminfo[nmChannelHandle].indstate.bit.TxFlag = 1;
    }
}

/*
 *函数介绍：重复发送报文状态
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：
 */
static void CanNm_FunRepeatMsgState(NetworkHandleType nmChannelHandle)
{
    // 检查主动唤醒状态，并且重复消息计数未超出限制
    if (cannminfo[nmChannelHandle].indstate.bit.activewakeup)
    {

        if (!cannminfo[nmChannelHandle].indstate.bit.repeatmsgcountout)
        {
            // 立即循环发送的条件判断与处理
            if (cannminfo[nmChannelHandle].timer.timeoutflag.bits.immediatecycle)
            {
                cannminfo[nmChannelHandle].msgcount++;
                cannminfo[nmChannelHandle].timer.timeoutflag.bits.immediatecycle = 0;
                // 判断是否达到重复消息发送的最大次数
                if (cannminfo[nmChannelHandle].msgcount > cannm_cfg_info[nmChannelHandle].N_RepeatMsg)
                {
                    cannminfo[nmChannelHandle].msgcount = 0;
                    cannminfo[nmChannelHandle].indstate.bit.repeatmsgcountout = 1;
                    cannminfo[nmChannelHandle].timer.timerflag.bits.msgcycle = 0;
                    cannminfo[nmChannelHandle].timer.timeoutflag.bits.msgcycle = 1;
                    cannminfo[nmChannelHandle].timer.timercount.bytes.T_NM_MessageCycle = 0;
#if NM_TEST_FLAG
                    NM_ST_TEST[0] = 0X02;
                    NM_ST_TEST[1] = 0X03;
                    CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
                }
                else
                {
                    cannminfo[nmChannelHandle].timer.timerflag.bits.immediatecycle = 1;
                    cannminfo[nmChannelHandle].indstate.bit.TxFlag = 1;
                }
            }
        }
        else
        {
            // 消息周期发送的条件判断与处理
            if (cannminfo[nmChannelHandle].timer.timeoutflag.bits.msgcycle)
            {

                cannminfo[nmChannelHandle].indstate.bit.TxFlag = 1;
                cannminfo[nmChannelHandle].timer.timeoutflag.bits.immediatecycle = 0;
                cannminfo[nmChannelHandle].timer.timeoutflag.bits.msgcycle = 0;
                cannminfo[nmChannelHandle].timer.timerflag.bits.msgcycle = 1;
            }
        }
    }

    // 检查被动唤醒状态，并处理消息发送状态
    if (cannminfo[nmChannelHandle].indstate.bit.passivewakeup)
    {
        NM_NetWorkRepeatMsgState(); // 调用处理网络重复消息状态的函数
        if (cannminfo[nmChannelHandle].timer.timeoutflag.bits.msgcycle)
        {
            // 设置发送标志并重置相关计时器标志
            if (cannminfo[nmChannelHandle].repeatmsgcount < cannm_cfg_info[nmChannelHandle].N_RepeatMessage)
            {
                cannminfo[nmChannelHandle].indstate.bit.TxFlag = 1;
                cannminfo[nmChannelHandle].repeatmsgcount++;
            }
            cannminfo[nmChannelHandle].timer.timeoutflag.bits.immediatecycle = 0;
            cannminfo[nmChannelHandle].timer.timeoutflag.bits.msgcycle = 0;
            cannminfo[nmChannelHandle].timer.timerflag.bits.msgcycle = 1;
        }
    }

    // 处理重复消息超时状态，更新网络状态
    if (cannminfo[nmChannelHandle].timer.timeoutflag.bits.repeatmsg)
    {
        // 清除重复消息超时标志，并根据激活状态更新网络状态
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.repeatmsg = 0;
        if (cannminfo[nmChannelHandle].indstate.bit.activewakeup)
        {
            cannminfo[nmChannelHandle].state = NM_NORMALOPERATION;
#if NM_TEST_FLAG
            NM_ST_TEST[0] = 0X02;
            NM_ST_TEST[1] = 0X04;
            CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
        }
        else
        {
            cannminfo[nmChannelHandle].timer.timeoutflag.bits.nmtimeout = 0;
            cannminfo[nmChannelHandle].timer.timerflag.bits.nmtimeout = 1;
            cannminfo[nmChannelHandle].timer.timercount.bytes.T_NM_TIMEOUT = 0;
            cannminfo[nmChannelHandle].state = NM_READYSLEEP;
            // 调用网络准备睡眠状态的函数
            cannm_cfg_fun[nmChannelHandle].Nm_NetWorkReadySleepState();
#if NM_TEST_FLAG
            NM_ST_TEST[0] = 0X01;
            NM_ST_TEST[1] = 0X05;
            CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
        }
#if CANNM_NODE_DETECTION_ENABLED

#endif
    }
}

/*
 *函数介绍：正常操作状态
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：
 */
static void CanNm_FunNormalOperationState(NetworkHandleType nmChannelHandle)
{

    // 检查消息周期超时并设置发送标志
    if (cannminfo[nmChannelHandle].timer.timeoutflag.bits.msgcycle)
    {
        cannminfo[nmChannelHandle].indstate.bit.TxFlag = 1;
        cannminfo[nmChannelHandle].timer.timerflag.bits.msgcycle = 1;
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.msgcycle = 0;
    }

    // 处理重复消息请求或接收
    if (cannminfo[nmChannelHandle].indstate.bit.repeatmsgrequset || cannminfo[nmChannelHandle].indstate.bit.repeatmsgrevc)
    {
        memset(&cannminfo[nmChannelHandle].timer, 0, sizeof(cannminfo[nmChannelHandle].timer));
        cannminfo[nmChannelHandle].indstate.bit.repeatmsgcountout = 0;
        cannminfo[nmChannelHandle].state = NM_REPEATMESSAGE;

        cannminfo[nmChannelHandle].timer.timerflag.bits.repeatmsg = 1;
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.repeatmsg = 0;
        cannminfo[nmChannelHandle].timer.timercount.bytes.T_REPEAT_MSG = 0;
    }

    // 如果没有激活唤醒，准备进入休眠状态
    if (!cannminfo[nmChannelHandle].indstate.bit.activewakeup)
    {
        memset(&cannminfo[nmChannelHandle].timer, 0, sizeof(cannminfo[nmChannelHandle].timer));
        cannminfo[nmChannelHandle].state = NM_READYSLEEP;
        // 调用网络准备睡眠状态的函数
        cannm_cfg_fun[nmChannelHandle].Nm_NetWorkReadySleepState();

        cannminfo[nmChannelHandle].timer.timeoutflag.bits.nmtimeout = 0;
        cannminfo[nmChannelHandle].timer.timercount.bytes.T_NM_TIMEOUT = 0;
        cannminfo[nmChannelHandle].timer.timerflag.bits.nmtimeout = 1;
    }
}

/*
 *函数介绍：准备睡眠作状态
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：
 */
static void CanNm_FunReadySleepState(NetworkHandleType nmChannelHandle)
{
    // 检查接收标志是否设置，如果设置，则重置相关标志和计时器
    if (cannminfo[nmChannelHandle].indstate.bit.RxFlag)
    {
        cannminfo[nmChannelHandle].indstate.bit.RxFlag = 0;
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.nmtimeout = 0;
        cannminfo[nmChannelHandle].timer.timerflag.bits.nmtimeout = 1;
        cannminfo[nmChannelHandle].timer.timercount.bytes.T_NM_TIMEOUT = 0;
    }

    // 检查网络超时标志，如果设置，则准备进入总线预睡眠模式
    if (cannminfo[nmChannelHandle].timer.timeoutflag.bits.nmtimeout)
    {
        cannm_cfg_fun[nmChannelHandle].Nm_PrepareBusSleepMode();
        cannminfo[nmChannelHandle].mode = NM_PREPAREBUSSLEEP;
        cannminfo[nmChannelHandle].indstate.bit.passivewakeup = 0;

        cannminfo[nmChannelHandle].timer.timeoutflag.bits.waitbussleep =0;
        if (can_FreezeNMCmd == Get_FreezeCanNmCMD())
        {
            cannminfo[nmChannelHandle].timer.timerflag.bits.waitbussleep = 0;/*蔚来项目冻结NM*/
        }else{
            cannminfo[nmChannelHandle].timer.timerflag.bits.waitbussleep = 1;
        }

        cannminfo[nmChannelHandle].timer.timercount.bytes.T_WAIT_BUS_SLEEP = 0;
        CanNm_Set_first_frame(nmChannelHandle,NM_FF_NO);
#if NM_TEST_FLAG
        NM_ST_TEST[1] = 0X06;
        CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
    }

    // 检查是否请求重复消息或已接收到重复消息，如果是，则重置相关状态和计时器，准备处理重复消息
    if (cannminfo[nmChannelHandle].indstate.bit.repeatmsgrequset || cannminfo[nmChannelHandle].indstate.bit.repeatmsgrevc)
    {
        memset(&cannminfo[nmChannelHandle].timer, 0, sizeof(cannminfo[nmChannelHandle].timer));
        cannminfo[nmChannelHandle].state = NM_REPEATMESSAGE;
        cannminfo[nmChannelHandle].timer.timerflag.bits.repeatmsg = 1;
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.repeatmsg = 0;
        cannminfo[nmChannelHandle].timer.timercount.bytes.T_REPEAT_MSG = 0;
        cannminfo[nmChannelHandle].indstate.bit.repeatmsgcountout = 0;
        cannminfo[nmChannelHandle].msgcount = 0;
    }
}

/*
 *函数介绍：预睡眠作状态
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：
 */
static void CanNm_FunPrepareBusSleepMode(NetworkHandleType nmChannelHandle)
{
    // 清除待发送PDU的数据
    // memset(&cannminfo[nmChannelHandle].TxPdu.data, 0, sizeof(cannminfo[nmChannelHandle].TxPdu.data));
    if (can_FreezeNMCmd != Get_FreezeCanNmCMD() &&\
        0 == cannminfo[nmChannelHandle].timer.timerflag.bits.waitbussleep)
    {
        cannminfo[nmChannelHandle].timer.timerflag.bits.waitbussleep = 1;/*蔚来项目取消冻结NM*/
    }

#if 0
    // 已注释的代码块，原用于检查是否等待进入总线睡眠模式
#else
    // 检查是否满足进入总线睡眠模式的条件
#ifdef cjb
    if (cannminfo[nmChannelHandle].timer.timerflag.bits.waitbussleep)
#endif
        if (cannminfo[nmChannelHandle].timer.timeoutflag.bits.waitbussleep)
        {

            cannminfo[nmChannelHandle].mode = NM_BUS_SLEEP;
            // 不再等待进入总线睡眠模式
#ifndef CJB_CHANGE
            cannm_cfg_fun[nmChannelHandle].Nm_BusSleepMode();
            cannminfo[nmChannelHandle].state = NM_REPEATMESSAGE;
#endif
            cannminfo[nmChannelHandle].timer.timeoutflag.bits.waitbussleep = 0;
            cannminfo[nmChannelHandle].timer.timerflag.bits.waitbussleep = 0;
#if NM_TEST_FLAG
            NM_ST_TEST[1] = 0X01;
            CANIf_Transmit(0,0X7EF,8,NM_ST_TEST);
#endif
        }
#endif

    // 检查是否需要激活网络或有接收标志，以决定是否保持网络活跃
    if (cannminfo[nmChannelHandle].indstate.bit.activewakeup || cannminfo[nmChannelHandle].indstate.bit.RxFlag
#if 1
        || cannminfo[nmChannelHandle].indstate.bit.passivewakeup)
#endif
    {
        // 激活网络模式并设置重复消息状态
        NM_NetWorkRepeatMsgState();
        cannminfo[nmChannelHandle].mode = NM_NETWORK;
        cannminfo[nmChannelHandle].state = NM_REPEATMESSAGE;
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.waitbussleep = 0;
        cannminfo[nmChannelHandle].timer.timerflag.bits.waitbussleep = 0;

        cannminfo[nmChannelHandle].timer.timerflag.bits.repeatmsg = 1;
        cannminfo[nmChannelHandle].timer.timeoutflag.bits.repeatmsg = 0;
        cannminfo[nmChannelHandle].timer.timercount.bytes.T_REPEAT_MSG = 0;

        // 标记即将重复发送消息
        if (cannminfo[nmChannelHandle].indstate.bit.passivewakeup)
        {
            cannminfo[nmChannelHandle].timer.timerflag.bits.msgcycle = 0;
            cannminfo[nmChannelHandle].timer.timeoutflag.bits.msgcycle = 1;
        }
        if (cannminfo[nmChannelHandle].indstate.bit.activewakeup)
        {
            cannminfo[nmChannelHandle].timer.timerflag.bits.immediatecycle = 0;
            cannminfo[nmChannelHandle].timer.timeoutflag.bits.immediatecycle = 1;
        }
    }
}

/*
 *函数介绍：NetWorkMode主处理函数
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：
 */
static void CanNm_FunNetWorkMode(NetworkHandleType nmChannelHandle)
{
    // 根据网络通道的状态选择相应的处理函数
    switch (cannminfo[nmChannelHandle].state)
    {
    case NM_REPEATMESSAGE:
        // 处理重复消息状态
        CanNm_FunRepeatMsgState(nmChannelHandle);
        break;
    case NM_NORMALOPERATION:
        // 处理正常操作状态
        CanNm_FunNormalOperationState(nmChannelHandle);
        break;
    case NM_READYSLEEP:
        // 处理准备休眠状态
        CanNm_FunReadySleepState(nmChannelHandle);
        break;
    default:
        // 对于未知状态，不执行任何操作
        break;
    }
}

/*
 *函数介绍：timer主函数
 *参数：nmChannelHandle 通道号
 *返回值：
 *备注：1ms调用一次
 */
void CanNm_Timer()
{
    unsigned char i;    // 用于循环遍历通道的索引
    unsigned char bits; // 用于循环遍历每个定时器位的索引

    // 遍历所有通道
    for (i = 0; i < NMCHANNELNUM; i++)
    {
        // 遍历每个通道的8个定时器位
        for (bits = 0; bits < 8; bits++)
        {
            // 如果当前位的定时器标志为1，则递增计数器
            if (cannminfo[i].timer.timerflag.data & (1 << bits))
            {
                cannminfo[i].timer.timercount.data[bits]++;
            }
            // 如果计数器达到预设时间，重置计数器，设置超时标志，并清除定时器标志
            if (cannminfo[i].timer.timercount.data[bits] >= cannm_cfg_timer[i].data[bits])
            {
                cannminfo[i].timer.timercount.data[bits] = 0;
                cannminfo[i].timer.timeoutflag.data |= 1 << bits;
                cannminfo[i].timer.timerflag.data &= ~(1 << bits);
                // 此处代码用于调试输出，显示通道号和超时标志
                // printf("Channel = %d,timeroutflag =0x %.2x\r\n",i,cannminfo[i].timer.timeoutflag.data);
            }
        }
    }
}

//蔚来项目单独新增处理
static void BLANC_Diag_wakeup(NetworkHandleType nmChannelHandle)
{
	uint8_t DIAG_DFT[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    static unsigned char Diag_wakeup_flag = 0;
    if ((UDS_DEFAULT_SESSION != UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION()) &&\
        (NM_NETWORK == cannminfo[nmChannelHandle].mode) &&\
        (0 ==  Diag_wakeup_flag))
    {
        CanNm_NetworkRequest(nmChannelHandle);
        Diag_wakeup_flag = 1;
        CanNm_SetUserData(0,DIAG_DFT,6);
    }else if ((UDS_DEFAULT_SESSION == UDS_APP_ITF_IMP_GET_CURR_ACTIVE_SESSION()) &&\
                (1 ==  Diag_wakeup_flag) )
    {
        if ((NM_NETWORK == cannminfo[nmChannelHandle].mode) &&\
            ((cannminfo[nmChannelHandle].state == NM_REPEATMESSAGE) ||\
            (cannminfo[nmChannelHandle].state == NM_NORMALOPERATION)))
        {
            CanNm_NetworkRelease(nmChannelHandle);
            memset(DIAG_DFT,0,6);
            CanNm_SetUserData(0,DIAG_DFT,6);
        }
        Diag_wakeup_flag = 0;
    }
}

/*
 *函数介绍：主函数
 *参数： 通道号
 *返回值：
 *备注：
 */
void CanNm_MainFunction(void)
{
    unsigned char i;

    // 遍历所有CAN通道
    for (i = 0; i < NMCHANNELNUM; i++)
    {
        // 根据当前通道的模式执行相应的操作
        switch (cannminfo[i].mode)
        {
        case NM_BUS_SLEEP:
            // 进入总线休眠模式
            CanNm_FunBusSleepMode(i);
            break;
        case NM_NETWORK:
            // 进入网络工作模式
            CanNm_FunNetWorkMode(i);
            break;
        case NM_PREPAREBUSSLEEP:
            // 准备进入总线休眠模式
            CanNm_FunPrepareBusSleepMode(i);
            break;
        default:
            // 默认情况，不执行任何操作
            break;
        }

        // 检查是否需要进行数据传输
        if (cannminfo[i].indstate.bit.TxFlag)
        {
            // 执行数据传输，并重置传输标志
            CanNm_Transmit(i);

            cannminfo[i].indstate.bit.TxFlag = 0;
        }
        BLANC_Diag_wakeup(i);
    }
}

/*
 *函数介绍：初始化函数
 *参数： 通道号
 *返回值：
 *备注：
 */
void CanNm_Init()
{
    unsigned char i;

    // 清零CAN网络管理信息结构体
    memset(&cannminfo, 0, sizeof(cannminfo));
#ifdef UdsApi_EXTERNAL_BTA_FUN_EN
    /*蔚来项目配置ID*/
    cannm_get_node_id();
#endif
    // 遍历所有通道，配置每个通道的相关信息
    for (i = 0; i < NMCHANNELNUM; i++)
    {
        // 设置通道号
        cannminfo[i].relchannelnum = cannm_cfg_info[i].relchannelnum;
        // 计算发送PDU的消息ID
        cannminfo[i].TxPdu.id = cannm_cfg_info[i].baseid + cannm_cfg_info[i].nodeid;
        // 设置发送PDU的消息长度
        cannminfo[i].TxPdu.len = cannm_cfg_info[i].msgdlc;
        // 启用通信控制标志
        nmcomctrlflag[i] = NM_ENABLE;
        // 设置保持唤醒状态
        cannminfo[i].ChannelCfgInfo.CanNmAllNmMessagesKeepAwake = nmTure;
        // 设置过滤掩码指针
        cannminfo[i].FilterMask = &cannm_FilterMask[i][0];
    }

    /*测试上电主动唤醒*/
    //CanNm_NetworkRequest(0);
}

/*
 *函数介绍：禁止网络管理报文通讯
 *参数： 通道号
 *返回值：
 *备注：
 */
#if (CanNmComControlEnabled == STD_ON)
Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType res = NM_E_Ok; // 初始化返回值，表示默认成功。

    // 检查网络通道是否不在正确的模式下。
    //if (cannminfo[nmChannelHandle].mode != NM_NETWORK)
    //{
    // res = NM_E_NOT_Ok; // 设置返回值，表示因模式不正确导致失败。
    //}
    if (CanNmPassiveModeEnabled) // 检查被动模式是否启用。
    {
        res = NM_E_NOT_Ok; // 设置返回值，表示因被动模式启用导致失败。
    }
    else // 如果以上两个条件都满足，继续关闭通信。
    {
        // 通过将相应标志设置为NM_DISABLE来禁用通信。
        nmcomctrlflag[nmChannelHandle] = NM_DISABLE;
        //cannminfo[nmChannelHandle].timer.timerflag.bits.msgcycle = NM_DISABLE;
        //cannminfo[nmChannelHandle].timer.timerflag.bits.nmtimeout = NM_DISABLE;
        //cannminfo[nmChannelHandle].timer.timerflag.bits.startx_app = NM_DISABLE;
        //cannminfo[nmChannelHandle].timer.timerflag.bits.startnmtx = NM_DISABLE;
    }

    return res; // 返回操作的结果。
}
#else
/*
 *函数介绍：关闭指定网络通道的通信功能。
 *参数： 通道号
 *返回值：
 *备注：
 */
Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle)
{
    return NM_E_NOSPPURT;// Always return the "not supported" error code.
}
#endif

/*
 *函数介绍：使能网络管理报文通讯
 *参数： 通道号
 *返回值：
 *备注：
 */
#if (CanNmComControlEnabled == STD_ON)
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle)
{
    Std_ReturnType res = NM_E_Ok;
    // 检查通道模式是否为NM_NETWORK
    //if (cannminfo[nmChannelHandle].mode != NM_NETWORK)
    //{
        //res = NM_E_NOT_Ok;
    //}
    // 如果处于被动模式，通信无法启用
    if (CanNmPassiveModeEnabled)
    {
        res = NM_E_NOT_Ok;
    }
    // 如果通信控制标志已启用，通信无法启用
    else if (nmcomctrlflag[nmChannelHandle])
    {
        res = NM_E_NOT_Ok;
    }
    else
    {
        // 启用通信并设置相关定时器标志
        nmcomctrlflag[nmChannelHandle] = NM_ENABLE;
        //cannminfo[nmChannelHandle].timer.timerflag.bits.msgcycle = NM_ENABLE;
        //cannminfo[nmChannelHandle].timer.timerflag.bits.nmtimeout = NM_ENABLE;
        //cannminfo[nmChannelHandle].timer.timerflag.bits.startx_app = NM_ENABLE;
        //cannminfo[nmChannelHandle].timer.timerflag.bits.startnmtx = NM_ENABLE;
    }
    // 返回操作结果
    return res;
}
#else
/*
 *函数介绍：启用指定CAN网络通道的通信功能。
 *参数： 通道号
 *返回值：
 *备注：
 */
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle)
{
    return NM_E_NOSPPURT;
}
#endif
