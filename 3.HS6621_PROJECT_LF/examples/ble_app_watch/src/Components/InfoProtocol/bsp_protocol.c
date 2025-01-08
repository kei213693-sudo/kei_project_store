/*
 * bsp_protocol.c
 *
 *  Created on: 2023年6月19日
 *      Author: F704011834_
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "bsp_protocol.h"
#include "crc32.h"
#include "co_debug.h"


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
    uint32_t crcvalue = 0;

    for(i = 0;i < len;i++)
    {
        if((PROTOCOL_START_1 == buf[i]) && (PROTOCOL_START_2 == buf[i+1]))
        {//找到起始位
            pProtocolCtl->rx.head = (protocol_head_t *)&buf[i];
            pProtocolCtl->rx.tail = (protocol_tail_t *)&buf[i + pProtocolCtl->rx.head->len - sizeof(pProtocolCtl->tx.tail->crc) - sizeof(pProtocolCtl->tx.tail->end)];

            if((i + pProtocolCtl->rx.head->len) <= len)
            {//长度未超出
                if(PROTOCOL_END == pProtocolCtl->rx.tail->end)
                {//结束位相同
                    //计算crc校验值
                    crcvalue = calcCRC32((unsigned char *)&buf[i+sizeof(pProtocolCtl->rx.head->start)]
                            ,(char)(pProtocolCtl->rx.head->len - sizeof(pProtocolCtl->rx.head->start) - sizeof(pProtocolCtl->rx.tail->crc) - sizeof(pProtocolCtl->rx.tail->end)));
                    if(crcvalue == pProtocolCtl->rx.tail->crc)
                    {//crc校验通过
                        cmd = pProtocolCtl->rx.head->cmd;
                        pProtocolCtl->lasrsp = cmd;
                        if(pProtocolCtl->rx.head->len > (sizeof(protocol_head_t) + sizeof(pProtocolCtl->tx.tail->crc) + sizeof(pProtocolCtl->tx.tail->end)))
                        {//数据位不为空就单独取出地址
                            pProtocolCtl->rx.buf = &buf[i+sizeof(protocol_head_t)];
                        }
                        break;
                    }
                    else
                    {
                        log_debug("rx data crc err!");
                    }
                }
                else
                {
                    log_debug("rx data end err!");
                }
            }
            else
            {
                log_debug("rx data len err!");
            }
        }
    }
    return cmd;
}

//这样是不分配data存储空间，直接在txBuf上写数据，适用于应用层函数写在当前c文件bsp_protocol场景
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
        memset(pProtocolCtl->txBuf,0,sizeof(pProtocolCtl->txBuf));//组包前先清空txBuffer

        pProtocolCtl->tx.head = (protocol_head_t *)&pProtocolCtl->txBuf[0];
        pProtocolCtl->tx.head->start[0] = PROTOCOL_START_1;
        pProtocolCtl->tx.head->start[1] = PROTOCOL_START_2;
        pProtocolCtl->tx.head->cmd = cmd;
        //pProtocolCtl->tx.head->len = len + sizeof(protocol_head_t) + sizeof(protocol_tail_t);
        pProtocolCtl->tx.head->len = len + sizeof(protocol_head_t) + sizeof(pProtocolCtl->tx.tail->crc) + sizeof(pProtocolCtl->tx.tail->end);
        pProtocolCtl->tx.head->err = 0;//这里默认配置为0.根据实际的清空再设置

        if(len)
        {//数据包不为空
            pProtocolCtl->tx.buf = &pProtocolCtl->txBuf[sizeof(protocol_head_t)];
            if(NULL != param)
            {//理解：假定传入参数是&Pdata = param，也就是说*param = Pdata，所以实际上我们要更改是Pdata。而不是*Pdata，所以更改指针变量Pdata需要用到指针的指针
                *param = pProtocolCtl->tx.buf;//将txBuf的存储空间取出来，不需要再重新定义临时变量来存储
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
        pProtocolCtl->tx.tail = (protocol_tail_t *)&pProtocolCtl->txBuf[pProtocolCtl->tx.head->len - sizeof(pProtocolCtl->tx.tail->crc) - sizeof(pProtocolCtl->tx.tail->end)];

        //计算crc校验值
        pProtocolCtl->tx.tail->end = PROTOCOL_END;
        pProtocolCtl->tx.tail->crc = calcCRC32((unsigned char *)&pProtocolCtl->txBuf[sizeof(pProtocolCtl->tx.head->start)]
            ,(char)(pProtocolCtl->tx.head->len - sizeof(pProtocolCtl->tx.head->start) - sizeof(pProtocolCtl->tx.tail->crc) - sizeof(pProtocolCtl->tx.tail->end)));
        pProtocolCtl->txLen = pProtocolCtl->tx.head->len;
    }
}


