/*
 * bsp_protocol.h
 *
 *  Created on: 2023年6月19日
 *      Author: F704011834_
 */
/*******************************预编译指令*******************************/
#ifndef _BSP_PROTOCOL_H_
#define _BSP_PROTOCOL_H_

/*******************************宏定义*******************************/
#define PROTOCOL_BUFF_SIZE 128

#define PROTOCOL_START_1 (0x68)//起始位
#define PROTOCOL_START_2 (0xFE)//起始位

#define PROTOCOL_END (0xFC)//结束位

#define PROTOCOL_CMD(pid,sid) ((sid<<8)|(pid))

#define PROTOCOL_PID_UWB (0x01)
#define PROTOCOL_PID_BLE (0x21)
#define PROTOCOL_PID_LF  (0x41)

/*******************************结构体、枚举******************************/
typedef struct
{//8bytes
    uint8_t start[2];//起始位
    uint8_t len;//包长度，有效数据总长度
    uint8_t single;//长包发送标志,0表示后续没有包要发送
    uint16_t cmd;//主命令+子命令
    uint8_t err;//错误码
    uint8_t res;//预留
}protocol_head_t;

typedef struct
{//5bytes
    uint32_t crc;//校验码,计算起始位到crc之间的数据，不包含起始位和crc
    uint8_t end;//结束位
}protocol_tail_t;

typedef struct
{//4+1+4 = 9bytes
    protocol_head_t *head;
    uint8_t *buf;//不同的命令码数据格式不同
    protocol_tail_t *tail;
}protocol_t;

//控制体
typedef struct
{//284bytes
    uint32_t txLen;//接收数据总长度
    uint32_t rxLen;
    uint8_t txBuf[PROTOCOL_BUFF_SIZE];//存储tx buf的缓存
    uint8_t rxBuf[PROTOCOL_BUFF_SIZE];//存储rx buf的缓存
    protocol_t tx;//控制指针
    protocol_t rx;
    uint16_t lasrsp;//最后一包的响应指令
}protocol_ctl_t;

typedef enum
{
    cmdProtocolStart                     = PROTOCOL_CMD(0x00,0x00),
    /*-------------------------uwb类指令类--------------------------------*/
    cmdUwbUpgradeReq                     = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x00),//升级确认
    cmdUwbUpgradeRsp                     = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x01),//升级确认响应

    cmdUwbUpdateReq                      = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x02),//升级数据
    cmdUwbUpdateRsp                      = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x03),//升级数据响应

    cmdUwbConfigIDReq                    = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x04),//配置设备ID
    cmdUwbConfigIDRsp                    = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x05),//配置设备ID响应

    cmdUwbConfigKeyReq                   = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x06),//配置设备密钥
    cmdUwbConfigKeyRsp                   = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x07),//配置设备密钥响应
    
    cmdUwbEventStartRangeReq             = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x08),//启动测距
    cmdUwbEventStartRangeRsp             = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x09),//启动测距响应
    
    cmdUwbEventStopRangeReq              = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x0a),//停止测距
    cmdUwbEventStopRangeRsp              = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x0b),//停止测距响应

    cmdUwbEventStartLiveReq              = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x0c),//启动活体检测
    cmdUwbEventStartLiveRsp              = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x0d),//启动活体检测响应

    cmdUwbEventStopLiveReq               = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x0e),//停止活体检测
    cmdUwbEventStopLiveRsp               = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x0f),//停止活体检测响应

    cmdUwbQueryRangeReq                  = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x10),//查询测距结果
    cmdUwbQueryRangeRsp                  = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x11),//查询测距结果响应

    cmdUwbQueryLiveReq                   = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x12),//查询活体检测结果
    cmdUwbQueryLiveRsp                   = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x13),//查询活体检测结果响应

    cmdUwbFrameErrReq                    = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x14),//帧异常（uwb芯片——》mcu
    cmdUwbFrameErrRsp                    = PROTOCOL_CMD(PROTOCOL_PID_UWB,0x15),//帧异常响应（mcu——》uwb芯片

    cmdUwbEndRsp                         = PROTOCOL_CMD(PROTOCOL_PID_UWB,0xff),//无意义指令
    /*-------------------------ble类指令类--------------------------------*/
    cmdBleUpgradeReq                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x00),//升级确认
    cmdBleUpgradeRsp                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x01),//升级确认响应

    cmdBleUpdateReq                      = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x02),//升级数据
    cmdBleUpdateRsp                      = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x03),//升级数据响应

    cmdBleButtonReq                      = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x04),//按键操作
    cmdBleButtonRsp                      = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x05),//按键操作响应

    cmdBleMotionStateReq                 = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x06),//运动状态上报
    cmdBleMotionStateRsp                 = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x07),//运动状态上报响应

    cmdBleGetStateReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x08),//查询运动状态
    cmdBleGetStateRsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x09),//查询运动状态响应

    cmdBleLowVoltageReq                  = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x0a),//低电告警
    cmdBleLowVoltageRsp                  = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x0b),//低电告警响应

    cmdBleGetVoltageReq                  = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x0c),//电量查询
    cmdBleGetVoltageRsp                  = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x0d),//电量查询响应

    cmdBleEnterLearnReq                  = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x0e),//进入学习模式
    cmdBleEnterLearnRsp                  = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x0f),//进入学习模式响应

    cmdBleEnterOTAReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x10),//进入OTA升级模式
    cmdBleEnterOTARsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x11),//进入OTA升级模式响应

    cmdBleStartUWBReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x12),//启动uwb测距
    cmdBleStartUWBRsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x13),//启动uwb测距响应

    cmdBleUWBKeySetReq                   = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x14),//uwb密钥设置
    cmdBleUWBKeySetRsp                   = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x15),//uwb密钥设置响应

	cmdBle3GDataReq                      = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x16),//加速度数值传递给车身 
	cmdBle3GDataRsp                      = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x17),// 

	cmdBleAdvStartReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x18),//钥匙ble广播控制 
	cmdBleAdvStartRsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x19),//
	
	cmdBleIrqSeedReq                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x1a),//身份种子请求 
	cmdBleIrqSeedRsp                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x1b),//
	
	cmdBleIrqCheckReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x1c),//身份校验
	cmdBleIrqCheckRsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x1d),//
	
	cmdBleBaseInfoReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x1e),//上传电量和位置信息 
	cmdBleBaseInfoRsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x1f),//
	
	cmdBleQueryBaseInfoReq               = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x20),//修改场强阈值 
	cmdBleQueryBaseInfoRsp               = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x21),//
	
	cmdBleChangeSKReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x22),//交换密钥信息
	cmdBleChangeSKRsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x23),//
	
	cmdBleLearnResReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x24),//上报学习结果 
	cmdBleLearnResRsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x25),//
	
	cmdBleDelKeyReq                      = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x26),//删除钥匙 
	cmdBleDelKeyRsp                      = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x27),//
	
	cmdBleKeyTypeReq                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x28),//发送钥匙类型 
	cmdBleKeyTypeRsp                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x29),//
	
	cmdBleSnSetReq                       = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x2a),//设置序列号
	cmdBleSnSetRsp                       = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x2b),//
	
	cmdBleBondAddReq                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x2c),//启动配对
	cmdBleBondAddRsp                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x2d),//
	
	cmdBleBondDelReq                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x2e),//删除配对 
	cmdBleBondDelRsp                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x2f),//
	
	cmdBleParamSetReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x30),//设置ble参数
	cmdBleParamSetRsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x31),//
	
	cmdBleRssiSendReq                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x32),//发送rssi值 
	cmdBleRssiSendRsp                    = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x33),//
	
	cmdBLEEndRsp                         = PROTOCOL_CMD(PROTOCOL_PID_BLE,0xff),//无意义指令

/*-------------------------LF类指令类--------------------------------*/
	cmdLFBaseInfoReq                     = PROTOCOL_CMD(PROTOCOL_PID_LF,0x00),//上报电量和位置信息 
	cmdLFBaseInfoRsp                     = PROTOCOL_CMD(PROTOCOL_PID_LF,0x01),//
	
	cmdLFQueryBaseInfoReq                = PROTOCOL_CMD(PROTOCOL_PID_LF,0x02),//修改场强阈值  
	cmdLFQueryBaseInfoRsp                = PROTOCOL_CMD(PROTOCOL_PID_LF,0x03),//
	
	cmdLFChangeSKReq                     = PROTOCOL_CMD(PROTOCOL_PID_LF,0x04),//把密钥发给LF
	cmdLFChangeSKRsp                     = PROTOCOL_CMD(PROTOCOL_PID_LF,0x05),//
	
	cmdLFLearnResReq                     = PROTOCOL_CMD(PROTOCOL_PID_LF,0x06),//上报学习结果 
	cmdLFLearnResRsp                     = PROTOCOL_CMD(PROTOCOL_PID_LF,0x07),//
	
	cmdLFDelKeyReq                       = PROTOCOL_CMD(PROTOCOL_PID_LF,0x08),//删除钥匙
	cmdLFDelKeyRsp                       = PROTOCOL_CMD(PROTOCOL_PID_LF,0x09),//

    cmdLFButtonReq                       = PROTOCOL_CMD(PROTOCOL_PID_LF,0x0a),//删除钥匙
	cmdLFButtonRsp                       = PROTOCOL_CMD(PROTOCOL_PID_LF,0x0b),//
	
    cmdLFBleLPReq                        = PROTOCOL_CMD(PROTOCOL_PID_LF,0x0c),//ble可休眠
	cmdLFBleLPRsp                        = PROTOCOL_CMD(PROTOCOL_PID_LF,0x0d),//

	cmdLFEndRsp                          = PROTOCOL_CMD(PROTOCOL_PID_LF,0xff),//无意义指令

    cmdProtocolEnd                       = PROTOCOL_CMD(0xff,0xff)
}PROTOCOL_CMD_E;

typedef struct
{//提供给应用层的接口
void (*protocol_init)(protocol_ctl_t *pProtocolCtl);
uint16_t (*protocol_last_rsp)(protocol_ctl_t *pProtocolCtl);
void (*protocol_alloc_head)(protocol_ctl_t *pProtocolCtl,uint16_t cmd,uint32_t len,void **param);
void (*protocol_alloc_tail)(protocol_ctl_t *pProtocolCtl);
uint16_t (*protocol_parse)(protocol_ctl_t *pProtocolCtl,uint8_t *buf,uint32_t len);
}BSP_PROTOCOL_API_T;
/*******************************全局变量的外部声明extern*******************************/
extern const BSP_PROTOCOL_API_T g_BspProtocolApi;

/*******************************全局函数的声明*******************************/
void protocol_init(protocol_ctl_t *pProtocolCtl);
uint16_t protocol_last_rsp(protocol_ctl_t *pProtocolCtl);

void protocol_alloc_head(protocol_ctl_t *pProtocolCtl,uint16_t cmd,uint32_t len,void **param);
void protocol_alloc_tail(protocol_ctl_t *pProtocolCtl);

uint16_t protocol_parse(protocol_ctl_t *pProtocolCtl,uint8_t *buf,uint32_t len);

#endif /* _APP_PROTOCOL_H_ */
