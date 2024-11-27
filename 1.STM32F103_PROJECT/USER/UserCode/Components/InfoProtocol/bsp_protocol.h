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
#define PROTOCOL_BUFF_SIZE (256)

#define PROTOCOL_START_1 (0x68)//起始位
#define PROTOCOL_START_2 (0xFE)//起始位
#define PROTOCOL_END_1   (0x66)//结束位
#define PROTOCOL_END_2   (0xFC)//结束位

#define PROTOCOL_CMD(pid,sid) ((sid<<8)|(pid))

//主命令
#define PROTOCOL_PID_BLE 	(0x21)
#define PROTOCOL_PID_TEST	(0xE1)

/*******************************结构体、枚举******************************/
typedef struct
{//8bytes
    uint8_t start[2];//起始位
    uint8_t len;//包长度，有效数据总长度
    uint8_t single;//长包发送标志,0表示后续没有包要发送
    uint16_t cmd;//主命令+子命令
    uint8_t err;//错误码
    uint8_t res;//预留
}protocol_head_t;//包头

typedef struct
{//4bytes
    uint16_t crc;//校验码,计算起始位到crc之间的数据，不包含起始位和crc
    uint8_t end[2];//结束位
}protocol_tail_t;//包尾

typedef struct
{//4+1+4 = 9bytes
    protocol_head_t *head;
    uint8_t *buf;//不同的命令码数据格式不同
    protocol_tail_t *tail;
}protocol_t;//整包

//控制体
typedef struct
{//PROTOCOL_BUFF_SIZE*2 + 8 + 9*2 +2 bytes
    uint32_t txlen;//接收数据总长度
    uint32_t rxlen;
    uint8_t txbuf[PROTOCOL_BUFF_SIZE];//存储tx buf的缓存
    uint8_t rxbuf[PROTOCOL_BUFF_SIZE];//存储rx buf的缓存
    protocol_t tx;//控制指针
    protocol_t rx;
    uint16_t lasrsp;//最后一包的响应指令
}protocol_ctl_t;

typedef enum
{
    cmdProtocolStart                     = PROTOCOL_CMD(0x00,0x00),
    
    /*-------------------------ble类指令类--------------------------------*/
	cmdBleUpgradeStartReq               = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x00),//升级起始包
	cmdBleUpgradeStartRsp               = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x01),//
	
	cmdBleUpgradDataReq                 = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x02),//升级数据包
	cmdBleUpgradDataRsp                 = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x03),//
	
	cmdBleUpgradStopReq                 = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x04),//升级结束包
	cmdBleUpgradStopRsp                 = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x05),//

    cmdBleButtonReq                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x06),//按键指令
	cmdBleButtonRsp                     = PROTOCOL_CMD(PROTOCOL_PID_BLE,0x07),//
	
	cmdBLEEndRsp                         = PROTOCOL_CMD(PROTOCOL_PID_BLE,0xff),//无意义指令

    /*-------------------------test类指令类--------------------------------*/
	cmdTestButtonReq                     = PROTOCOL_CMD(PROTOCOL_PID_TEST,0x00),//
	cmdTestButtonRsp                     = PROTOCOL_CMD(PROTOCOL_PID_TEST,0x01),//
    
    cmdTestEndRsp                        = PROTOCOL_CMD(PROTOCOL_PID_TEST,0xff),//无意义指令

    cmdProtocolEnd                       = PROTOCOL_CMD(0xff,0xff)
}PROTOCOL_CMD_E;

typedef struct
{//提供给应用层接口的结构体
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
