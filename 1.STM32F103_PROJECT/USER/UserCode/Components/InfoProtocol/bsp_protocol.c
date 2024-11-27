/*
 * bsp_protocol.c
 *
 *  Created on: 2023年6月19日
 *      Author: F704011834_
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "bsp_protocol.h"
#include "bsp_crc.h"

/*******************************全局变量定义*******************************/
const BSP_PROTOCOL_API_T g_BspProtocolApi = 
{
    protocol_init,
    protocol_last_rsp,
    protocol_alloc_head,
    protocol_alloc_tail,
    protocol_parse
};
/*******************************内部变量定义*******************************/
//static protocol_ctl_t ProtocolCtl = {0};

/*******************************内部函数的声明*******************************/

/*******************************实际函数的实现*******************************/

/**************************************************************
*函数功能描述 ：协议栈初始化函数
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void protocol_init(protocol_ctl_t *pProtocolCtl)
{
    if(NULL != pProtocolCtl)
    {
        memset(pProtocolCtl,0,sizeof(protocol_ctl_t));
    }
}

/**************************************************************
*函数功能描述 ：获取最后的响应指令
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
uint16_t protocol_last_rsp(protocol_ctl_t *pProtocolCtl)
{
    uint16_t u16rsp = cmdProtocolStart;
    if(NULL != pProtocolCtl)
    {
        u16rsp = pProtocolCtl->lasrsp;
    }
    return u16rsp;
}

/**************************************************************
*函数功能描述 ：协议解析，获取cmd和净荷数据地址pProtocolCtl->rx.buf
*函数参数 ：buf，指向数据指针
        len，数据长度
*函数返回值 ：
*备注：
**************************************************************/
uint16_t protocol_parse(protocol_ctl_t *pProtocolCtl,uint8_t *buf,uint32_t len)
{
    uint8_t i = 0;
    uint16_t cmd = cmdProtocolStart;
    uint16_t u16crc = 0;

    for(i = 0;i < len;i++)
    {
        if((PROTOCOL_START_1 == buf[i]) && (PROTOCOL_START_2 == buf[i+1]))
        {//找到起始位
            pProtocolCtl->rx.head = (protocol_head_t *)&buf[i];
            pProtocolCtl->rx.tail = (protocol_tail_t *)&buf[i + pProtocolCtl->rx.head->len - sizeof(protocol_tail_t)];

            if((i + pProtocolCtl->rx.head->len) <= len)
            {//长度未超出
                if((PROTOCOL_END_1 == pProtocolCtl->rx.tail->end[0]) && (PROTOCOL_END_2 == pProtocolCtl->rx.tail->end[1]))
                {//结束位相同
                    //计算crc校验值
                    u16crc =  bsp_crc16_table((unsigned char *)&buf[i+sizeof(pProtocolCtl->rx.head->start)]
                            ,(pProtocolCtl->rx.head->len - sizeof(pProtocolCtl->rx.head->start) - sizeof(protocol_tail_t)));
                    if(u16crc == pProtocolCtl->rx.tail->crc)
                    {//crc校验通过
                        cmd = pProtocolCtl->rx.head->cmd;
                        pProtocolCtl->lasrsp = cmd;
                        if(pProtocolCtl->rx.head->len > (sizeof(protocol_head_t) + sizeof(protocol_tail_t)))
                        {//数据位不为空就单独取出地址
                            pProtocolCtl->rx.buf = &buf[i+sizeof(protocol_head_t)];
                        }
                        break;
                    }
                    else
                    {
                        printf("rx data crc err!");
                    }
                }
                else
                {
                    printf("rx data end err!");
                }
            }
            else
            {
                printf("rx data len err!");
            }
        }
    }
    return cmd;
}

#if 1
//这样是不分配data存储空间，直接在txbuf上写数据，适用于应用层函数写在当前c文件bsp_protocol场景
/**************************************************************
*函数功能描述 ：协议组包，处理包头
*函数参数 ：cmd，命令码
            len，数据长度
            param，指向数据存储区域的指针
*函数返回值 ：
*备注：
**************************************************************/
void protocol_alloc_head(protocol_ctl_t *pProtocolCtl,uint16_t cmd,uint32_t len,void **param)
{
    if(NULL != pProtocolCtl)
    {
        memset(pProtocolCtl->txbuf,0,sizeof(pProtocolCtl->txbuf));//组包前先清空txbuffer

        pProtocolCtl->tx.head = (protocol_head_t *)&pProtocolCtl->txbuf[0];
        pProtocolCtl->tx.head->start[0] = PROTOCOL_START_1;
        pProtocolCtl->tx.head->start[1] = PROTOCOL_START_2;
        pProtocolCtl->tx.head->cmd = cmd;
        pProtocolCtl->tx.head->len = len + sizeof(protocol_head_t) + sizeof(protocol_tail_t);
        pProtocolCtl->tx.head->err = 0;//这里默认配置为0.根据实际的清空再设置

        if(len)
        {//数据包不为空
            pProtocolCtl->tx.buf = &pProtocolCtl->txbuf[sizeof(protocol_head_t)];
            if(NULL != param)
            {//理解：假定传入参数是&Pdata = param，也就是说*param = Pdata，所以实际上我们要更改是Pdata。而不是*Pdata，所以更改指针变量Pdata需要用到指针的指针
                *param = pProtocolCtl->tx.buf;//将txbuf的存储空间取出来，不需要再重新定义临时变量来存储
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：协议组包，处理包尾
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
void protocol_alloc_tail(protocol_ctl_t *pProtocolCtl)
{
    if(NULL != pProtocolCtl)
    {
        pProtocolCtl->tx.tail = (protocol_tail_t *)&pProtocolCtl->txbuf[pProtocolCtl->tx.head->len - sizeof(protocol_tail_t)];

        //计算crc校验值
        pProtocolCtl->tx.tail->crc = bsp_crc16_table((unsigned char *)&pProtocolCtl->txbuf[sizeof(pProtocolCtl->tx.head->start)]
            ,(pProtocolCtl->tx.head->len - sizeof(pProtocolCtl->tx.head->start) - sizeof(protocol_tail_t)));
		pProtocolCtl->tx.tail->end[0] = PROTOCOL_END_1;
        pProtocolCtl->tx.tail->end[1] = PROTOCOL_END_2;
        pProtocolCtl->txlen = pProtocolCtl->tx.head->len;
    }
}
#else
//这样是先用一个空间存储好data数据，然后将存储空间指针传过来，copy到txbuf，适用于应用层函数写在另外的c文件场景
/**************************************************************
*函数功能描述 ：协议组包
*函数参数 ：cmd，命令码
        len，数据长度
        param，指向数据存储区域的指针
*函数返回值 ：
*备注：
**************************************************************/
protocol_t *protocol_alloc(uint16_t cmd,uint32_t len,uint8_t *pData)
{
    memset(pProtocolCtl->txbuf,0,sizeof(pProtocolCtl->txbuf));//组包前先清空txbuffer

    pProtocolCtl->tx.head = (protocol_head_t *)&pProtocolCtl->txbuf[0];
    pProtocolCtl->tx.head->start[0] = PROTOCOL_START_1;
    pProtocolCtl->tx.head->start[1] = PROTOCOL_START_2;
    pProtocolCtl->tx.head->cmd = cmd;
    pProtocolCtl->tx.head->len = len + sizeof(protocol_head_t) + sizeof(protocol_tail_t);
    pProtocolCtl->tx.head->err = 0;//这里默认配置为0.根据实际的清空再设置

    if(len)
    {//数据包不为空
        pProtocolCtl->tx.buf = &pProtocolCtl->txbuf[sizeof(protocol_head_t)];
        if(NULL != param)
        {
            *param = pProtocolCtl->tx.buf;//将txbuf的存储空间取出来，不需要再重新定义临时变量来存储
        }
    }

    pProtocolCtl->tx.tail = (protocol_tail_t *)&pProtocolCtl->txbuf[sizeof(protocol_head_t) + len];
    pProtocolCtl->txlen = pProtocolCtl->tx.head->len;

    return &pProtocolCtl->tx;
}

/**************************************************************
*函数功能描述 ：提供给app层的接口
*函数参数 ：stHead，包头参数，这里用到了cmd、len、single、err
            pData，指向app已经配置好参数的缓存地址
            channel，传输通道
            waitcmd，是否有重传机制，0不需要重传
*函数返回值 ：
*备注：
**************************************************************/
void bsp_protocol_tx(protocol_head_t stHead,uint8_t *pData,uint8_t channel,uint16_t waitcmd)
{
    protocol_t *Ptx = protocol_alloc(stHead.cmd,stHead.len,pData);

    if(NULL != Ptx)
    {
        Ptx->head->single = stHead.single;
        Ptx->head->err = stHead.err;

        //最后计算crc，并将数据传入tx任务
        protocol_tx(channel,waitcmd);
    }
}
#endif
