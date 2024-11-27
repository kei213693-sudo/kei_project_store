/*
 * bsp_ymodem.h
 *
 *  Created on: 2024年11月1日
 *      Author: lijing
 *	  Abstract: 实现ymodem协议
 */
/*******************************预编译指令*******************************/
#ifndef _BSP_YMODEM_H__
#define _BSP_YMODEM_H__

#ifdef __cplusplus
extern "C" {
#endif

/*******************************头文件*******************************/

/*******************************宏定义*******************************/
#define BSP_YMODEM_PACKET_SIZE (128)
#define BSP_YMODEM_PACKET_1K_SIZE (1024)
#define BSP_YMODEM_PACKET_OVERHEAD (5)//包头+包尾长度
#define BSP_YMODEM_PACKET_TIMEOUT (1000)//超时时间,ms
#define BSP_YMODEM_PACKET_TIMEOUT_TIMES (60)//超时次数，超时时间 *= BSP_YMODEM_PACKET_TIMEOUT

/*******************************结构体、枚举******************************/
typedef enum
{//ymodem指令
    BSP_YMODEM_CMD_SOH         = 0x01,//帧数据长度128bytes，发送端
    BSP_YMODEM_CMD_STX         = 0x02,//帧数据长度1024bytes，发送端
    BSP_YMODEM_CMD_EOT         = 0x04,//文件传输结束命令，发送端
    BSP_YMODEM_CMD_ACK         = 0x06,//接收成功，返回ack响应，接收端
    BSP_YMODEM_CMD_NAK         = 0x15,//重传当前数据包请求命令，接收端
    BSP_YMODEM_CMD_CAN         = 0x18,//取消传输命令，连续发送5个该命令，接收端
    BSP_YMODEM_CMD_C           = 0x43,//字符C，接收端
    BSP_YMODEM_CMD_END
}BSP_YMODEM_CMD_E;

typedef enum
{//ymodem流程
    BSP_YMODEM_PROCESS_FILEHEADER = 2,//起始帧
    BSP_YMODEM_PROCESS_TRANS,//数据帧
    BSP_YMODEM_PROCESS_OVER,//数据传输完成
    BSP_YMODEM_PROCESS_STOP,//结束帧
    BSP_YMODEM_PROCESS_END
}BSP_YMODEM_PROCESS_E;

typedef struct
{
    /* int32_t -1发送失败，>0 发送字节数，data 数据buff , len 发送数据长度 , 超时时间 ms */
    int32_t (*tx_data)(int8_t* data,uint32_t len,uint32_t mstime);
    /* int32_t -1发送失败，>0 接收字节数，data 数据buff , len 接收数据长度 , 超时时间 ms */
    int32_t (*rx_data)(int8_t* data,uint32_t len,uint32_t mstime);
    /*存储单包数据，在一个芯片上，一般要么用于tx，要么用于rx，不会同时存在*/
    int8_t packet_buf[BSP_YMODEM_PACKET_1K_SIZE + BSP_YMODEM_PACKET_OVERHEAD];
}bsp_ymodem_ctl_t;

/*******************************全局变量的外部声明extern*******************************/
extern bsp_ymodem_ctl_t *g_pStBspYmodemCtl;

/*******************************全局函数的声明*******************************/
extern void bsp_ymodem_init(int32_t (*tx_data)(int8_t* data,uint32_t len,uint32_t mstime),
                            int32_t (*rx_data)(int8_t* data,uint32_t len,uint32_t mstime));

extern void bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_E eCmd);
extern BSP_YMODEM_CMD_E bsp_ymodem_cmd_rx(void);

extern uint32_t bsp_ymodem_recv_packet(int8_t *pData,uint8_t packet_num);
extern int32_t bsp_ymodem_recv_filename(int8_t *pFilename,uint32_t *pFilesize);
extern void bsp_ymodem_recv(uint8_t u8Type);

extern int32_t bsp_ymodem_send_packet(int8_t *pData,uint32_t u32Len,uint8_t packet_num);
extern int32_t bsp_ymodem_send_filename(const int8_t *pFilename,uint32_t filesize);
extern void bsp_ymodem_send(uint32_t u32Addr,const int8_t *pFilename,uint32_t filesize);

#ifdef __cplusplus
}
#endif
#endif  /* _BSP_YMODEM_H__ */

