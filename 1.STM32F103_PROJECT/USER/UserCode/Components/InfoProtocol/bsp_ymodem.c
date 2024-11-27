/*
 * bsp_ymodem.c
 *
 *  Created on: 2024年11月1日
 *      Author: lijing
 *	  Abstract: 实现ymodem协议，这个api
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bsp_ymodem.h"
#include "bsp_crc.h"
#include "usart.h"
#include "bsp_upgrade.h"

/*******************************全局变量定义*******************************/
bsp_ymodem_ctl_t *g_pStBspYmodemCtl = NULL;

/*******************************内部变量定义*******************************/

/*******************************内部函数的声明*******************************/

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：注册数据收发api，指向实际实现数据收发的底层api
*函数参数 ：put_data，一般指向uart_tx
            get_data，一般指向uart_rx
*函数返回值 ：
*备注 ：
**************************************************************/
void bsp_ymodem_init(int32_t (*tx_data)(int8_t* data,uint32_t len,uint32_t mstime),
                    int32_t (*rx_data)(int8_t* data,uint32_t len,uint32_t mstime))
{
    if(NULL != g_pStBspYmodemCtl)
    {
        memset(g_pStBspYmodemCtl,0,sizeof(bsp_ymodem_ctl_t));
        g_pStBspYmodemCtl->tx_data = tx_data;
        g_pStBspYmodemCtl->rx_data = rx_data;
    }
}

/**************************************************************
*函数功能描述 ：发送单个指令
*函数参数 ：cmd，可选ACK、NACK、charC、EOT、CAN
*函数返回值 ：
*备注 ：
**************************************************************/
void bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_E eCmd)
{
    if(NULL != g_pStBspYmodemCtl)
    {
        int8_t cmd = (int8_t)eCmd;
        g_pStBspYmodemCtl->tx_data(&cmd,1,BSP_YMODEM_PACKET_TIMEOUT);
    }
}

/**************************************************************
*函数功能描述 ：接收单个指令
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
BSP_YMODEM_CMD_E bsp_ymodem_cmd_rx(void)
{
    BSP_YMODEM_CMD_E eCmd = BSP_YMODEM_CMD_END;
    int8_t cmd = 0;
    if(NULL != g_pStBspYmodemCtl)
    {
        g_pStBspYmodemCtl->rx_data(&cmd,1,BSP_YMODEM_PACKET_TIMEOUT);
        eCmd = (BSP_YMODEM_CMD_E)cmd;
    }
    return eCmd;
}

/**************************************************************
*函数功能描述 ：启动一次接收，进行相应判定
*函数参数 ：pData，存储有效数据地址，不需要获取就传入NULL
            packet_num，本次要接收的包号
*函数返回值 ：UINT32_MAX，出现错误
            0，结束传输
            128/1024，数据帧
*备注 ：
**************************************************************/
uint32_t bsp_ymodem_recv_packet(int8_t *pData,uint8_t packet_num)
{
    uint32_t u32ret = UINT32_MAX;
    uint32_t packet_len = 0;
    if(NULL == g_pStBspYmodemCtl)
    {
        return packet_len;
    }

    memset(g_pStBspYmodemCtl->packet_buf,0,sizeof(g_pStBspYmodemCtl->packet_buf));
    //1.接收第一个字节，然后判定是“指令”还是“帧”
    g_pStBspYmodemCtl->packet_buf[0] = (int8_t)bsp_ymodem_cmd_rx();
    if((g_pStBspYmodemCtl->packet_buf[0] != BSP_YMODEM_CMD_SOH) && (g_pStBspYmodemCtl->packet_buf[0] != BSP_YMODEM_CMD_STX))
    {//指令
        if(g_pStBspYmodemCtl->packet_buf[0] == BSP_YMODEM_CMD_EOT)
        {//不是结束帧就是帧头错误
            u32ret = 0;
        }
    }
    else
    {//帧
        //2.就接收后续数据
        packet_len = (g_pStBspYmodemCtl->packet_buf[0] == BSP_YMODEM_CMD_SOH)?BSP_YMODEM_PACKET_SIZE:BSP_YMODEM_PACKET_1K_SIZE;
        if(0 == g_pStBspYmodemCtl->rx_data(&g_pStBspYmodemCtl->packet_buf[1],(packet_len+4),BSP_YMODEM_PACKET_TIMEOUT))//HAL_OK
        {
            //3.判定包号有效性
            if((g_pStBspYmodemCtl->packet_buf[1] ==  (int8_t)packet_num && g_pStBspYmodemCtl->packet_buf[2] == (int8_t)(~packet_num)))
            {
                //4.判定crc有效性
                uint16_t crc = bsp_crc16_table((uint8_t *)&g_pStBspYmodemCtl->packet_buf[3],packet_len);
                if(g_pStBspYmodemCtl->packet_buf[packet_len + 3] == (int8_t)((crc >> 8) & 0xFF) &&
                    g_pStBspYmodemCtl->packet_buf[packet_len + 4] == (int8_t)(crc & 0xFF))
                {
                    u32ret = packet_len;
                    if(pData != NULL)
                    {//校验成功，获取有效数据值
                        memcpy(pData,&g_pStBspYmodemCtl->packet_buf[3],packet_len);
                    }
                }
            }
        }
    }

    return u32ret;
}

/**************************************************************
*函数功能描述 ：接收第一包数据，并存储文件名和数据总长度
*函数参数 ：pFilename，存储文件名地址
            pFilesize，存储数据总长度地址
*函数返回值 ：-1，读取失败
            0，读取成功
*备注 ：
**************************************************************/
int32_t bsp_ymodem_recv_filename(int8_t *pFilename,uint32_t *pFilesize)
{
    int32_t i32ret = -1;
    uint32_t i = 0;
    uint32_t packet_len = bsp_ymodem_recv_packet(NULL,0);

    if(UINT32_MAX != packet_len)
    {
		i32ret = 0;
        if(pFilename != NULL)
        {
            while((g_pStBspYmodemCtl->packet_buf[i + 3] != 0) && (i < packet_len))
            {//存储文件名
                pFilename[i] = g_pStBspYmodemCtl->packet_buf[i + 3];
                i++;
            }
        }

        if(pFilesize != NULL)
        {
            i++;
            while((g_pStBspYmodemCtl->packet_buf[i + 3] >= 0x30) 
                && (g_pStBspYmodemCtl->packet_buf[i + 3] <= 0x39) && (i < packet_len))
            {//存储总长度，高位在前，低位在后，例如54781 bytes就是0x05 0x04 0x07 0x08 0x01
                (*pFilesize) = (*pFilesize) * 10 + (g_pStBspYmodemCtl->packet_buf[i + 3] - '0');
                i++;
            }
        }
    }
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：Ymodem协议接收流程代码，适用于g_pStBspUpgradeCtl控制结构体
*函数参数 ：u8Type，接收代码类型（产品+模块）
*函数返回值 ：
*备注 ：要保证死循环或优先级高一些，要么超时退出，要么接收完成退出；
        注意这里如果用uart需要使用阻塞式收发方式，调试打印一般是中断式，所以进入前和结束后都需要uart相应的初始化操作;
        超时失败使用内部变量计数，避免两个任务交替调用uart的tx/rx启动和终止操作导致异常情况。
**************************************************************/
void bsp_ymodem_recv(uint8_t u8Type)
{
    if((NULL == g_pStBspYmodemCtl) || (NULL == g_pStBspUpgradeCtl))
    {
        return;
    }

    uint8_t u8Force = 0;
    uint8_t u8Retry = 0;//连续接收失败计数
    uint32_t packet_len = 0;
    BSP_YMODEM_PROCESS_E eProcess = BSP_YMODEM_PROCESS_FILEHEADER;

    //根据u8Type决定处理哪个区域，默认APP
    bsp_upgrade_init(BSP_UPGRADE_AREA_APP);//初始化app区域的控制结构体
    memset(&g_pStBspUpgradeCtl->stInfo,0,sizeof(bsp_upgrade_info_t));//该通信协议，需要先清空升级信息
    g_pStBspUpgradeCtl->stInfo.u8Type = u8Type;//保存代码类型信息
    g_pStBspUpgradeCtl->u8ProtocolType = 0;
    g_pStBspUpgradeCtl->u32ReceiveIndex = 0;

    printf("upgrade ymodem rx start\r\n");
    HAL_UART_Abort_IT(&UART1_Handler);//取消中断式传输
    bsp_ymodem_init(uart_tx_data,uart_rx_data);//注册uart收发api

    while((eProcess < BSP_YMODEM_PROCESS_END)//接收失败，大多是len错误
        && (u8Retry <= BSP_YMODEM_PACKET_TIMEOUT_TIMES))//超时失败
    {
        switch (eProcess)
        {
            case BSP_YMODEM_PROCESS_FILEHEADER:
            {
                bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_C);//打印ccccc
                if(0 <= bsp_ymodem_recv_filename((int8_t *)g_pStBspUpgradeCtl->stInfo.u8Name,&g_pStBspUpgradeCtl->stInfo.u32TotalLen))
                {//起始帧接收成功
                    if(g_pStBspUpgradeCtl->stInfo.u32TotalLen > g_pStBspUpgradeCtl->u32StoreSize)
                    {
                        for(uint8_t i = 0; i < 5; i++)
                        {//取消传输
                            bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_CAN);
                        }
                        eProcess = BSP_YMODEM_PROCESS_END;//退出while循环
                    }
					else
					{
						bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_ACK);
						bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_C);
                        u8Retry = 0;
                        g_pStBspUpgradeCtl->u32ReceiveIndex++;//包索引递增，0是起始包，数据包从1开始
						eProcess = BSP_YMODEM_PROCESS_TRANS;
					}
                }
                else
                {
                    u8Retry++;
                }
                break;
            }

            case BSP_YMODEM_PROCESS_TRANS:
            {
                packet_len = bsp_ymodem_recv_packet(NULL,g_pStBspUpgradeCtl->u32ReceiveIndex);
                if(packet_len == 0)
                {//第1个EOT接收成功
                    u8Force = 1;//这里怎么处理？最后一包之后才会收到EOT进入这个流程，该如何调用bsp_upgrade_store？
                    bsp_upgrade_store(NULL,0,u8Force);
                    bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_NAK);
                    u8Retry = 0;
                    eProcess = BSP_YMODEM_PROCESS_OVER;
                }
                else if(packet_len == UINT32_MAX)
                {//数据接收失败
                    bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_NAK);
                    u8Retry++;
                }
                else
                {
                    
                    g_pStBspUpgradeCtl->u32ReceiveLen += packet_len;//已接收数据长度
                    if(g_pStBspUpgradeCtl->u32ReceiveLen  > (g_pStBspUpgradeCtl->stInfo.u32TotalLen + BSP_YMODEM_PACKET_1K_SIZE))
                    {//如果接收到的数据已经超过了总长度，强制取消，并配置升级失败
                        for(uint8_t i = 0; i < 5; i++)
                        {//取消传输
                            bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_CAN);
                        }
                        bsp_upgrade_finish(1);//接收失败，擦除已写入的数据，直接重启，不会重置uart，也不会删除任务
                    }
                    else
                    {//接收一包数据成功
                        bsp_upgrade_store((uint8_t *)&g_pStBspYmodemCtl->packet_buf[3],packet_len,u8Force);

                        u8Retry = 0;
                        g_pStBspUpgradeCtl->u32ReceiveIndex++;//包索引递增，0是起始包，数据包从1开始
                        bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_ACK);
                    }
                }
                break;
            }

            case BSP_YMODEM_PROCESS_OVER:
            {
                packet_len = bsp_ymodem_recv_packet(NULL,0);
                if(packet_len == 0)
                {//第2个EOT接收成功
                    bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_ACK);
                    bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_C);
                    u8Retry = 0;
                    eProcess = BSP_YMODEM_PROCESS_STOP;
                }
                else
                {
                    u8Retry++;
                }
                break;
            }

            case BSP_YMODEM_PROCESS_STOP:
            {
                packet_len = bsp_ymodem_recv_packet(NULL,0);
                if(packet_len != UINT32_MAX)
                {//结束帧接收成功
                    bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_ACK);
                    bsp_upgrade_finish(0);//接收完成，直接重启，不会重置uart，也不会删除任务
					eProcess = BSP_YMODEM_PROCESS_END;
                }
                else
                {
                    u8Retry++;
                }
                break;
            }

            default:
                break;
        }
    }

    HAL_UART_Abort(&UART1_Handler);//取消阻塞式传输
    HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//启动uart中断接收

    if(BSP_YMODEM_PROCESS_END != eProcess)
    {//不是正常结束的
        if(u8Retry > BSP_YMODEM_PACKET_TIMEOUT_TIMES)
        {//超时
            printf("upgrade ymodem rx timeout,eProcess:%d\r\n",eProcess);
        }
        else
        {//长度错误取消
            printf("upgrade ymodem rx rx len err,TotalLen:%d,ReceiveIndex:%d,ReceiveLen:%d,u32StoreLen:%d",g_pStBspUpgradeCtl->stInfo.u32TotalLen,g_pStBspUpgradeCtl->u32ReceiveIndex,g_pStBspUpgradeCtl->u32ReceiveLen,g_pStBspUpgradeCtl->stInfo.u32StoreLen);
        }
    }
    else
    {
        printf("upgrade ymodem rx end\r\n");
    }
}

/**************************************************************
*函数功能描述 ：发送一包数据
*函数参数 ：pData，带发送数据地址
            len，待发送数据长度
            packet_num，本次要发送的包号
*函数返回值 ：-1，发送失败
            0，发送成功
*备注 ：起始帧、数据帧（填充在传入这个函数前就要做完，不然填充都是0x00）、结束帧三种
**************************************************************/
int32_t bsp_ymodem_send_packet(int8_t *pData,uint32_t u32Len,uint8_t packet_num)
{
    int32_t i32ret = -1;
    uint32_t packet_len = 0;
    if((NULL == g_pStBspYmodemCtl) || (pData == NULL) || (u32Len > BSP_YMODEM_PACKET_1K_SIZE))
    {
        return i32ret;
    }

    memset(g_pStBspYmodemCtl->packet_buf,0,sizeof(g_pStBspYmodemCtl->packet_buf));
    //包头
    packet_len = (u32Len <= BSP_YMODEM_PACKET_SIZE) ? BSP_YMODEM_PACKET_SIZE : BSP_YMODEM_PACKET_1K_SIZE;
    g_pStBspYmodemCtl->packet_buf[0] = (packet_len == BSP_YMODEM_PACKET_SIZE) ? BSP_YMODEM_CMD_SOH : BSP_YMODEM_CMD_STX;
    g_pStBspYmodemCtl->packet_buf[1] = packet_num;
    g_pStBspYmodemCtl->packet_buf[2] = ~packet_num;
    //数据
    memcpy(&g_pStBspYmodemCtl->packet_buf[3],pData,u32Len);
    //包尾
    uint16_t crc = bsp_crc16_table((uint8_t *)&g_pStBspYmodemCtl->packet_buf[3],packet_len);
    g_pStBspYmodemCtl->packet_buf[packet_len +3] = (crc >> 8) & 0xFF; // Append the higher byte of the CRC.
    g_pStBspYmodemCtl->packet_buf[packet_len +4] = crc & 0xFF; // Append the lower byte of the CRC.

    i32ret = (uint32_t)g_pStBspYmodemCtl->tx_data(g_pStBspYmodemCtl->packet_buf,packet_len + BSP_YMODEM_PACKET_OVERHEAD,BSP_YMODEM_PACKET_TIMEOUT);
    return i32ret;
}

/**************************************************************
*函数功能描述 ：发送起始帧和结束帧
*函数参数 ：pFilename，存储文件名地址，必须传入字符串，末尾自带0x00
            filesize，存储数据总长度
*函数返回值 ：-1，发送失败
            0，发送成功
*备注 ：pFilename为NULL，filesize为0就是发送结束帧
**************************************************************/
int32_t bsp_ymodem_send_filename(const int8_t *pFilename,uint32_t filesize)
{
    int32_t i32ret = -1;
	
    int8_t ym_packet[BSP_YMODEM_PACKET_SIZE] = {0};//用于拼接文件名和文件大小
    if((NULL != pFilename) && (0 != filesize))
    {
        //处理文件名
        uint8_t u8Index = (uint8_t)sprintf((char *)ym_packet,"%s",pFilename);
        u8Index++;//0x00长度

        //处理长度
        uint8_t u8BitLen = (uint8_t)log10(filesize) + 1;//获取十进制的位数，例如54781就是5
        for(uint8_t i = u8BitLen; i > 0; i--)
        {//54781转换就是0x05 0x04 0x07 0x08 0x01
            ym_packet[u8Index] = (int8_t)(filesize / (uint32_t)pow(10,(i - 1)));
            filesize %= (uint32_t)pow(10,(i - 1));
            u8Index++;
        }
    }
    
    i32ret = bsp_ymodem_send_packet(ym_packet,BSP_YMODEM_PACKET_SIZE,0);

    return i32ret;
}

/**************************************************************
*函数功能描述 ：Ymodem协议发送流程代码
*函数参数 ：u32Addr，指向数据区域的地址
            pFilename，存储文件名地址，必须传入字符串，末尾自带0x00
            filesize，存储数据总长度
*函数返回值 ：
*备注 ：
**************************************************************/
void bsp_ymodem_send(uint32_t u32Addr,const int8_t *pFilename,uint32_t filesize)
{
    if((NULL == g_pStBspYmodemCtl) || (NULL == pFilename) || (0 == filesize))
    {
        return;
    }

    uint8_t u8Retry = 0;//连续接收失败计数
    int32_t i32ret = -1;
    uint8_t packet_num = 0;//包索引
    uint32_t packet_len = 0;//单包长度
    uint32_t u32SendLen = 0;//已发送长度
    int8_t ym_packet[BSP_YMODEM_PACKET_1K_SIZE] = {0};//用于存储已读取的数据
    BSP_YMODEM_CMD_E eCmd = BSP_YMODEM_CMD_END;
    BSP_YMODEM_PROCESS_E eProcess = BSP_YMODEM_PROCESS_FILEHEADER;

    printf("upgrade ymodem tx start\r\n");
    HAL_UART_Abort_IT(&UART1_Handler);//取消中断式传输
    bsp_ymodem_init(uart_tx_data,uart_rx_data);//注册uart收发api

    while((eProcess < BSP_YMODEM_PROCESS_END)//接收端主动取消失败
        && (u8Retry <= BSP_YMODEM_PACKET_TIMEOUT_TIMES))//超时失败
    {
        switch (eProcess)
        {
            case BSP_YMODEM_PROCESS_FILEHEADER:
            {
                eCmd = bsp_ymodem_cmd_rx();
                if(BSP_YMODEM_CMD_C == eCmd)
                {
                    i32ret = bsp_ymodem_send_filename(pFilename,filesize);//发送起始帧
                    if(0 == i32ret)
                    {
                        eCmd = bsp_ymodem_cmd_rx();
                        if(BSP_YMODEM_CMD_ACK == eCmd)
                        {//分两次接收ack和c是否可能会错过，要修改为rx_data（）收2bytes吗？实测后才能知道
                            eCmd = bsp_ymodem_cmd_rx();
                            if(BSP_YMODEM_CMD_C == eCmd)
                            {
                                packet_num++;
                                eProcess = BSP_YMODEM_PROCESS_TRANS;
                            }
                        }
                    }
                }

                if(BSP_YMODEM_PROCESS_FILEHEADER == eProcess)
                {
                    u8Retry++;
                }
                break;
            }

            case BSP_YMODEM_PROCESS_TRANS:
            {//只发1024bytes的包
                packet_len = ((filesize - u32SendLen) > BSP_YMODEM_PACKET_1K_SIZE) ? BSP_YMODEM_PACKET_1K_SIZE : (filesize - u32SendLen);
                bsp_upgrade_store_read(u32Addr,ym_packet,packet_len);
                i32ret = bsp_ymodem_send_packet(ym_packet,packet_len,packet_num);//不填充0x1A
                if(0 == i32ret)
                {
                    eCmd = bsp_ymodem_cmd_rx();
                    if(BSP_YMODEM_CMD_ACK == eCmd)
                    {
                        packet_num++;
                        u32Addr += packet_len;
                        u32SendLen += packet_len;
                        if(packet_len < BSP_YMODEM_PACKET_1K_SIZE)
                        {//最后一包发完了
                            eProcess = BSP_YMODEM_PROCESS_OVER;
                        }
                    }
                    else if(BSP_YMODEM_CMD_NAK == eCmd)
                    {
                        u8Retry++;
                    }
                    else if(BSP_YMODEM_CMD_CAN == eCmd)
                    {//对端用户取消，退出while循环
                        break;
                    }
                    else
                    {
                        u8Retry++;
                    }
                }
                else
                {
                    u8Retry++;
                }
                break;
            }

            case BSP_YMODEM_PROCESS_OVER:
            {
                bsp_ymodem_cmd_tx(BSP_YMODEM_CMD_EOT);
                eCmd = bsp_ymodem_cmd_rx();
                if(BSP_YMODEM_CMD_ACK == eCmd)
                {//分两次接收ack和c是否可能会错过，要修改为rx_data（）收2bytes吗？实测后才能知道
                    eCmd = bsp_ymodem_cmd_rx();
                    if(BSP_YMODEM_CMD_C == eCmd)
                    {
                        eProcess = BSP_YMODEM_PROCESS_STOP;
                    }
                }

                if(BSP_YMODEM_PROCESS_OVER == eProcess)
                {
                    u8Retry++;
                }
                break;
            }

            case BSP_YMODEM_PROCESS_STOP:
            {
                i32ret = bsp_ymodem_send_filename(NULL,0);//发送终止帧
                if(0 == i32ret)
                {
                    eCmd = bsp_ymodem_cmd_rx();
                    if(BSP_YMODEM_CMD_ACK == eCmd)
                    {
                        eProcess = BSP_YMODEM_PROCESS_END;
                    }
                }

                if(BSP_YMODEM_PROCESS_STOP == eProcess)
                {
                    u8Retry++;
                }
                break;
            }

            default:
                break;
        }
    }

    HAL_UART_Abort(&UART1_Handler);//取消阻塞式传输
    HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//启动uart中断接收

    printf("packet_num:%d,u32SendLen:%d bytes\r\n",packet_num,u32SendLen);

    if(BSP_YMODEM_PROCESS_END != eProcess)
    {//不是正常结束的
        if(u8Retry > BSP_YMODEM_PACKET_TIMEOUT_TIMES)
        {//超时
            printf("upgrade ymodem tx timeout,eProcess:%d\r\n",eProcess);
        }
        else
        {//对端取消
            printf("upgrade ymodem tx cancel\r\n");
        }
    }
    else
    {
        printf("upgrade ymodem tx end\r\n");
    }
}


