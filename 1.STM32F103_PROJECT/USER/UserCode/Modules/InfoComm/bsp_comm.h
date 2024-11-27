/*
 * bsp_comm.h
 *
 *  Created on: 2023年8月4日
 *      Author: F704011834_
 */

#ifndef _BSP_COMM_H_
#define _BSP_COMM_H_
/*******************************宏定义*******************************/
#define BSP_COMM_QUEUE_SIZE (4)//一个队列最多的数据条数
#define BSP_COMM_PACKET_SIZE (256)//一条数据最多的字节数

#define BSP_COMM_TIMEOUT_TIME (200)//单次超时时间，单位ms，过短容易重复发送；过长容易导致tx buff满从而丢失数据
#define BSP_COMM_TIMEOUT_COUNT (2)//重发次数

/*******************************结构体、枚举******************************/
typedef void (*appCommCB_t)(void *);//bsp传递信息给app的接口

typedef struct
{//单包数据参数
    uint8_t buf[BSP_COMM_PACKET_SIZE];//存储空间
    uint8_t len;
    uint8_t channel;//数据来源
    //响应标志,因为app_comm和protocol是分开的
    //，所以这个数值可以根据protocol实际的数据格式来改变
    uint16_t waitflag;
    uint8_t waitcnt;
    uint8_t res[3];//预留
}bsp_comm_packet_t;

typedef struct
{
    uint8_t txnum;//待发送数据条数
    uint8_t rxnum;//已接收数据条数
    uint8_t res[2];//预留

    //数据队列
    bsp_comm_packet_t txQueue[BSP_COMM_QUEUE_SIZE];
    bsp_comm_packet_t rxQueue[BSP_COMM_QUEUE_SIZE];

    //指向处理函数的指针,channel可以同步收发通道，就是什么发过来的就通过什么还回去
    void (*handle_tx)(uint8_t *buf,uint32_t len,uint8_t channel);
    void (*handle_rx)(uint8_t *buf,uint32_t len,uint8_t channel);
    uint16_t (*handle_last_rsp)(uint8_t channel);
}bsp_comm_ctl_t;

typedef struct
{//提供给应用层的接口
    void (*bsp_comm_init)(appCommCB_t pCallback
        ,void (*handle_tx)(uint8_t *buf,uint32_t len,uint8_t channel)
        ,void (*handle_rx)(uint8_t *buf,uint32_t len,uint8_t channel)
        ,uint16_t (*handle_last_rsp)(uint8_t channel));
    uint8_t (*bsp_comm_enter_txQueue)(uint8_t *buf,uint32_t len,uint8_t channel,uint16_t waitflag);
    uint8_t (*bsp_comm_enter_rxQueue)(uint8_t *buf,uint32_t len,uint8_t channel);
    void (*bsp_comm_do_tx)(uint8_t queueIndex);
    void (*bsp_comm_do_rx)(uint8_t queueIndex);
}BSP_COMM_API_T;

/*******************************全局变量的外部声明extern*******************************/
extern bsp_comm_ctl_t *g_pStBspCommCtl;
extern const BSP_COMM_API_T g_BspCommApi;

/*******************************全局函数的声明*******************************/
void bsp_comm_init(appCommCB_t pCallback
    ,void (*handle_tx)(uint8_t *buf,uint32_t len,uint8_t channel)
    ,void (*handle_rx)(uint8_t *buf,uint32_t len,uint8_t channel)
    ,uint16_t (*handle_last_rsp)(uint8_t channel));
uint8_t bsp_comm_enter_txQueue(uint8_t *buf,uint32_t len,uint8_t channel,uint16_t waitflag);
uint8_t bsp_comm_enter_rxQueue(uint8_t *buf,uint32_t len,uint8_t channel);
void bsp_comm_do_tx(uint8_t queueIndex);
void bsp_comm_do_rx(uint8_t queueIndex);

#endif /* APPLICATION_BSP_COMM_H_ */
