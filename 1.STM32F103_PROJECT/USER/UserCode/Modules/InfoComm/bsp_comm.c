/*
 * bsp_comm.c
 *
 *  Created on: 2023年8月4日
 *      Author: F704011834_
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "bsp_comm.h"

/*******************************全局变量定义*******************************/
bsp_comm_ctl_t *g_pStBspCommCtl = NULL;//控制结构体变量，app层去申请内存

const BSP_COMM_API_T g_BspCommApi =//提供给外部的api集合，不使用这个变量也可以直接去写函数名称
{
    bsp_comm_init,
    bsp_comm_enter_txQueue,
    bsp_comm_enter_rxQueue,
    bsp_comm_do_tx,
    bsp_comm_do_rx
};

/*******************************内部变量定义*******************************/
static appCommCB_t pCommTxAppRetryCB = NULL;

/*******************************内部函数的声明*******************************/
static void bsp_comm_clear_check_tx(uint8_t queueIndex);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：通信控制初始化初始化函数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void bsp_comm_init(appCommCB_t pCallback
    ,void (*handle_tx)(uint8_t *buf,uint32_t len,uint8_t channel)
    ,void (*handle_rx)(uint8_t *buf,uint32_t len,uint8_t channel)
    ,uint16_t (*handle_last_rsp)(uint8_t channel))
{
    if(NULL == g_pStBspCommCtl)
    {
        return;
    }

    memset(g_pStBspCommCtl,0,sizeof(bsp_comm_ctl_t));

    if(NULL != handle_tx)
    {
        g_pStBspCommCtl->handle_tx = handle_tx;
    }

    if(NULL != handle_rx)
    {
        g_pStBspCommCtl->handle_rx = handle_rx;
    }

    if(NULL != handle_last_rsp)
    {
        g_pStBspCommCtl->handle_last_rsp = handle_last_rsp;
    }

    if(NULL != pCallback)
    {
        pCommTxAppRetryCB = pCallback;
    }
}

/**************************************************************
*函数功能描述 ：发送数据入队列
*函数参数 ：buf，指向数据指针
            len，数据长度
            channel，传输通道
            waitflag，是否需要响应，需要的话就有失败重试机制
*函数返回值 ：
*备注：
**************************************************************/
uint8_t bsp_comm_enter_txQueue(uint8_t *buf,uint32_t len,uint8_t channel,uint16_t waitflag)
{
    uint8_t i = 0;
    uint8_t queueIndex = BSP_COMM_QUEUE_SIZE;
    if(NULL == g_pStBspCommCtl)
    {
        return queueIndex;
    }

    if((NULL != buf) && (len <= BSP_COMM_PACKET_SIZE))
    {//有效性
        if(g_pStBspCommCtl->txnum < BSP_COMM_QUEUE_SIZE)
        {//队列是否已满
            for(i = 0;i < BSP_COMM_QUEUE_SIZE;i++)
            {
                if(0 == g_pStBspCommCtl->txQueue[i].len)
                {
                    //信息入队列
                    memset(&g_pStBspCommCtl->txQueue[i],0,sizeof(g_pStBspCommCtl->txQueue[i]));
                    memcpy(g_pStBspCommCtl->txQueue[i].buf,buf,len);
                    g_pStBspCommCtl->txQueue[i].len = len;
                    g_pStBspCommCtl->txQueue[i].channel = channel;
                    g_pStBspCommCtl->txQueue[i].waitflag = waitflag;
                    g_pStBspCommCtl->txnum++;
                    //bsp_comm_do_tx(i);//裸机环境直接调用发送处理接口

                    queueIndex = i;//传出索引到app层
                    break;
                }
            }
        }
        
    }

    return queueIndex;
}

/**************************************************************
*函数功能描述 ：接收数据入队列
*函数参数 ：buf，指向数据指针
            len，数据长度
            channel，传输通道
*函数返回值 ：
*备注：
**************************************************************/
uint8_t bsp_comm_enter_rxQueue(uint8_t *buf,uint32_t len,uint8_t channel)
{
    uint8_t i = 0;
    uint8_t queueIndex = BSP_COMM_QUEUE_SIZE;
    if(NULL == g_pStBspCommCtl)
    {
        return queueIndex;
    }

    if((NULL != buf) && (len <= BSP_COMM_PACKET_SIZE))if((NULL != buf) && (len <= BSP_COMM_PACKET_SIZE))
    {
        if(g_pStBspCommCtl->rxnum < BSP_COMM_QUEUE_SIZE)
        {
            for(i = 0;i < BSP_COMM_QUEUE_SIZE;i++)
            {
                if(0 == g_pStBspCommCtl->rxQueue[i].len)
                {
                    memset(&g_pStBspCommCtl->rxQueue[i],0,sizeof(g_pStBspCommCtl->rxQueue[i]));
                    memcpy(g_pStBspCommCtl->rxQueue[i].buf,buf,len);
                    g_pStBspCommCtl->rxQueue[i].len = len;
                    g_pStBspCommCtl->rxQueue[i].channel = channel;
                    g_pStBspCommCtl->rxnum++;
                    //bsp_comm_do_rx(i);//裸机环境直接调用接收处理接口

                    queueIndex = i;
                    break;
                }
            }
        }
    }
    return queueIndex;
}

/**************************************************************
*函数功能描述 ：将队列中的数据发送出去
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void bsp_comm_do_tx(uint8_t queueIndex)
{
    if(NULL == g_pStBspCommCtl)
    {
        return;
    }

    if((NULL != g_pStBspCommCtl->handle_tx) && (0 != g_pStBspCommCtl->txQueue[queueIndex].len))
    {
        g_pStBspCommCtl->handle_tx(g_pStBspCommCtl->txQueue[queueIndex].buf,g_pStBspCommCtl->txQueue[queueIndex].len,g_pStBspCommCtl->txQueue[queueIndex].channel);

        if(0 != g_pStBspCommCtl->txQueue[queueIndex].waitflag)
        {
            if(g_pStBspCommCtl->txQueue[queueIndex].waitcnt < BSP_COMM_TIMEOUT_COUNT)
            {
                g_pStBspCommCtl->txQueue[queueIndex].waitcnt++;
                //调用定时函数检测是否有返回，根据实际开发环境实现，一般有如下三种
                //裸机（阻塞等待）
                //裸机有调度（直接重复调用task_put）
                //rtos（先入队列再定时，通过两个事件实现）
                if(NULL != pCommTxAppRetryCB)
                {//将索引index传到任务队列，让comm_task处理发送操作
                    (*pCommTxAppRetryCB)((void*)&queueIndex);
                }
            }
            else
            {//超时
//                Display_printf(dispHandle, 0, 0, "comm wait timeout[%d]:0x%04x",queueIndex,g_pStBspCommCtl->txQueue[queueIndex].waitflag);
                g_pStBspCommCtl->txQueue[queueIndex].len = 0;
                g_pStBspCommCtl->txnum--;
                g_pStBspCommCtl->txQueue[queueIndex].waitcnt = 0;
            }
        }
        else
        {
            g_pStBspCommCtl->txQueue[queueIndex].len = 0;//???????
            g_pStBspCommCtl->txnum--;
        }
    }
}

/**************************************************************
*函数功能描述 ：处理队列中的数据
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void bsp_comm_do_rx(uint8_t queueIndex)
{
    if(NULL == g_pStBspCommCtl)
    {
        return;
    }

    if((NULL != g_pStBspCommCtl->handle_rx) && (0 != g_pStBspCommCtl->rxQueue[queueIndex].len))
    {
        g_pStBspCommCtl->handle_rx(g_pStBspCommCtl->rxQueue[queueIndex].buf,g_pStBspCommCtl->rxQueue[queueIndex].len,g_pStBspCommCtl->rxQueue[queueIndex].channel);

        g_pStBspCommCtl->rxQueue[queueIndex].len = 0;
        g_pStBspCommCtl->rxnum--;

        bsp_comm_clear_check_tx(queueIndex);
    }
}

/**************************************************************
*函数功能描述 ：成功接收到响应，清空待发送数据
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void bsp_comm_clear_check_tx(uint8_t queueIndex)
{
    if(NULL == g_pStBspCommCtl)
    {
        return;
    }

    uint16_t waitflag = g_pStBspCommCtl->handle_last_rsp(g_pStBspCommCtl->rxQueue[queueIndex].channel);

    for(uint8_t i = 0;i < BSP_COMM_QUEUE_SIZE;i++)
    {
        if((0 != g_pStBspCommCtl->txQueue[i].len) && (waitflag == g_pStBspCommCtl->txQueue[i].waitflag)
                && (g_pStBspCommCtl->rxQueue[queueIndex].channel == g_pStBspCommCtl->txQueue[i].channel))
        {
            g_pStBspCommCtl->txQueue[i].len = 0;
            g_pStBspCommCtl->txnum--;
        }
    }
}
