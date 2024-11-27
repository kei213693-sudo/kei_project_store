/*
 * bsp_log.c
 *
 *  Created on: 2024年7月22日
 *      Author: lj
 *	  Abstract: 日志模块，包含tx和rx两个部分，tx就是“模块化+标准化打印”，rx就是“调试指令处理逻辑”
 */
#include <stdint.h>
#include <string.h>
#include <stdarg.h> 
#include <stdio.h> 

#include "bsp_log.h"
#include "main.h"
#include "timer.h"
#include "app_task.h"
#include "iwdg.h"
#include "rtc.h"
#include "bsp_time.h"
#include "stmflash.h"
#include "hw_w25qxx.h"
#include "bsp_aes.h"
#include "app_upgrade.h"
#include "bsp_ymodem.h"
#include "bsp_upgrade.h"
#include "bsp_crc.h"
#include "bsp_md5.h"
#include "atk_ble03.h"
#include "app_ble.h"

/*******************************全局变量定义*******************************/
const BSP_LOG_API_T g_BspLogApi = 
{
    bsp_log_switch_set,
    bsp_log_print,
    bsp_log_print_hex,
    bsp_log_cmd_handle
};

/*******************************内部变量定义*******************************/
static char g_logBuf[BSP_LOG_MAX_LEN];

static bsp_log_ctl_t g_modules[] = 
{//初始化默认打印等级全开，根据BSP_LOG_MODULE_E命名
    {"NONE",BSP_LOG_SWITCH_ON},
    {"LOG",BSP_LOG_SWITCH_ON},
    {"LP",BSP_LOG_SWITCH_ON},
    {"BLE",BSP_LOG_SWITCH_ON},
    {"BUTTON",BSP_LOG_SWITCH_ON},
    {"LED",BSP_LOG_SWITCH_ON},
    {"COMM",BSP_LOG_SWITCH_ON},
    {"TASK",BSP_LOG_SWITCH_ON},
    {"UPGRADE",BSP_LOG_SWITCH_ON},
};

/*******************************内部函数的声明*******************************/

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：设置某模块打印开关
*函数参数 ：module，模块类型
            en，开或关
*函数返回值 ：
*备注 ：
**************************************************************/
void bsp_log_switch_set(BSP_LOG_MODULE_E module,BSP_LOG_SWITCH_E en)
{
    if(en < BSP_LOG_SWITCH_END)
    {
        if(BSP_LOG_MODULE_ALL == module)
        {//所有模块一起操作，全开或全关
            for(uint8_t i = 0; i < BSP_LOG_MODULE_ALL; i++)
            {
                g_modules[i].en = en;
            }
        }
        else if(module < BSP_LOG_MODULE_ALL)
        {//控制单个模块的开关
            g_modules[module].en = en;
        }
    }
}

/**************************************************************
*函数功能描述 ：标准化打印接口
*函数参数 ：module，模块类型
            type，信息类型
            format，标准化参数
*函数返回值 ：
*备注 ：时间+模块+打印类型+数据信息；
        注意打印长度不能超过BSP_LOG_MAX_LEN，不然会出现内存覆盖错误
**************************************************************/
void bsp_log_print(BSP_LOG_MODULE_E module,BSP_LOG_TYPE_E type,const char *format, ...)
{
    if(type < BSP_LOG_TYPE_END)
    {
        if(BSP_LOG_SWITCH_ON == g_modules[module].en)
        {//该模块打印开启
            va_list args;
            va_start(args,format);//获取format各变量地址

            memset(g_logBuf,0,sizeof(g_logBuf));
            #ifdef _BSP_TIME_H_//如果将time组件加入到了工程中
            sprintf(&g_logBuf[strlen(g_logBuf)],"[%s]",bsp_time_get_string());//把时间信息如年月日时分秒插入打印前缀，如果该项目没有rtc就屏蔽
            #endif
            sprintf(&g_logBuf[strlen(g_logBuf)],"[%s]",g_modules[module].name);//把模块名称插入打印前缀

            if(BSP_LOG_TYPE_INFO == type)//把打印等级信息类别插入打印前缀
            {
                sprintf(&g_logBuf[strlen(g_logBuf)],"[INFO]"); 
            }
            else if(BSP_LOG_TYPE_WARNING == type)
            {
                sprintf(&g_logBuf[strlen(g_logBuf)],"[WARNING]");
            }
            else if(BSP_LOG_TYPE_ERROR == type)
            {
                sprintf(&g_logBuf[strlen(g_logBuf)],"[ERROR]");
            }

            #if 1	//C99
            vsnprintf(&g_logBuf[strlen(g_logBuf)],(sizeof(g_logBuf) - strlen(g_logBuf)),format,args);//将格式化数据插入到字符串数组中，返回转化字符串的长度
            #else	//C89
            int length = 0;
            vsprintf(&g_logBuf[strlen(g_logBuf)], format, args);
            length = strlen(g_logBuf);
            length = length > (sizeof(g_logBuf) - strlen(g_logBuf)) ? (sizeof(g_logBuf) - strlen(g_logBuf)): length;
            // hw_printf("len:%d\r\n",length);
            #endif
            sprintf(&g_logBuf[strlen(g_logBuf)],"\0");//将字符串数组转化为字符串
            hw_printf("%s\r\n",g_logBuf);

            va_end(args);//释放资源
        }
    }
}

/**************************************************************
*函数功能描述 ：hex格式打印接口
*函数参数 ：module，模块类型
            note，文本信息
            pData，指向数据的指针
            len，要打印的数据长度
*函数返回值 ：
*备注 ：时间+模块+文本信息+数据hex格式信息；
        注意打印长度不能超过BSP_LOG_MAX_LEN，不然会出现内存覆盖错误
**************************************************************/
void bsp_log_print_hex(BSP_LOG_MODULE_E module,const char *note,const uint8_t *pData,uint32_t u32len)
{
    if((BSP_LOG_SWITCH_ON == g_modules[module].en)
		&& (NULL != pData) && (0 != u32len))
    {
        memset(g_logBuf,0,sizeof(g_logBuf));
        #ifdef _BSP_TIME_H_//如果将time组件加入到了工程中
        sprintf(&g_logBuf[strlen(g_logBuf)],"[%s]",bsp_time_get_string());//把时间信息如年月日时分秒插入打印前缀，如果该项目没有rtc就屏蔽
        #endif
        sprintf(&g_logBuf[strlen(g_logBuf)],"[%s]",g_modules[module].name);//把模块名称插入打印前缀
        sprintf(&g_logBuf[strlen(g_logBuf)],"[%s][%u]:",note,u32len);//把文本和长度插入打印前缀
        sprintf(&g_logBuf[strlen(g_logBuf)],"\0");//将字符串数组转化为字符串
        hw_printf("%s",g_logBuf);

        for(uint32_t i = 0; i < u32len; i++)
        {
            hw_printf("%02x ",pData[i]);//逐个打印
        }
        hw_printf("\r\n");

		#if 0
        //旧的方案，1byex转换为3bytes字符串表示，太浪费空间，还容易超内存出现内存覆盖错误
        if((sizeof(g_logBuf) - strlen(g_logBuf))/3 < u32len)
        {//不能超过g_logBuf的长度
            u32len = sizeof(g_logBuf)/3 - strlen(g_logBuf);
        }

        for(uint32_t i = 0; i < u32len; i++)
        {
            sprintf(&g_logBuf[strlen(g_logBuf)],"%02x ",pData[i]);//把hex数据转化为三个字符串再打印
        }
        sprintf(&g_logBuf[strlen(g_logBuf)],"\0");//将字符串数组转化为字符串
        hw_printf("%s\r\n",g_logBuf);
		#endif
	}
}

/**************************************************************
*函数功能描述 ：根据上位机传递的指令数据来执行特定的处理
*函数参数 ：pData，指向数据的指针
            len，数据长度
*函数返回值 ：
*备注 ：rtos环境建议使用一个任务来处理
**************************************************************/
void bsp_log_cmd_handle(const uint8_t *pData,uint32_t u32len)
{
    if((NULL != pData) && (0 != u32len))
    {
        bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"uart rx:%s",pData);
        bsp_log_print_hex(BSP_LOG_MODULE_LOG,"UART RX HEX",pData,u32len);

        if(0 == strncmp("LED",(const char *)pData,3))
        {
            uint8_t u8Opt = pData[3] - 0x30;
            bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"led u8Opt:%d\r\n",u8Opt);
        }
        else if(0 == strncmp("IWDG",(const char *)pData,4))
        {//喂狗
            IWDG_Feed();
        }
		else if(0 == strncmp("Reset",(const char *)pData,5))
        {//软件重启
            __set_FAULTMASK(1);
			NVIC_SystemReset();
        }
        else if(0 == strncmp("Upgrade",(const char *)pData,7))
        {//启动串口升级
            uint8_t u8Type = pData[7] - 0x30;//获取升级类型信息
            app_task_create(APP_TASK_MODULE_UPGRADE);//先创建任务
            app_upgrade_opt_put(APP_UPGRADE_OPT_YMODEM,&u8Type,1);
        }
        else if(0 == strncmp("UpInfoGet",(const char *)pData,9))
        {//获取升级信息
            bsp_upgrade_info_t stInfo = {0};
            bsp_upgrade_info_read(BSP_UPGRADE_APP_INFO_ADDR_S,(uint32_t)&stInfo,sizeof(bsp_upgrade_info_t));
            uint16_t u16Crc = bsp_crc16_table((unsigned char *)&stInfo,(sizeof(bsp_upgrade_info_t) - sizeof(stInfo.u16Crc)));
            if((0xffff == stInfo.u16Crc) || (0x0000 == stInfo.u16Crc) || (u16Crc != stInfo.u16Crc))
            {//确保info信息的有效性
                bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_ERROR,"upgrade info crc err:0x%x-0x%x",stInfo.u16Crc,u16Crc);
            }
            else
            {
                bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_INFO,"u8Type:%d\r\nu32Version:0x%x\r\nu8Name:%s\r\nu32TotalLen:%d\r\nu32StoreLen:%d\r\nu8UpFlag:%d\r\n"
                    ,stInfo.u8Type,stInfo.u32Version,stInfo.u8Name,stInfo.u32TotalLen,stInfo.u32StoreLen,stInfo.u8UpFlag);
            }
        }
        else if(0 == strncmp("task",(const char *)pData,4))
        {
            uint8_t u8Opt = pData[4] - 0x30;
            APP_TASK_MODULE_E eModule = (APP_TASK_MODULE_E)(pData[5] - 0x30);
            uint32_t u32Len = sizeof(eModule);

            if(u8Opt <= APP_TASK_OPT_RESUME)//4
            {
                app_task_opt_put((APP_TASK_OPT_E)u8Opt,(uint8_t *)&eModule,u32Len);
            }
            else if(5 == u8Opt)
            {//设置优先级
                typedef struct
                {
                    APP_TASK_MODULE_E eModule;
                    UBaseType_t uxNewPriority;
                }Param_t;
                Param_t stData = {eModule,3};
                u32Len = sizeof(stData);
                app_task_opt_put(APP_TASK_OPT_SET_PRI,(uint8_t *)&stData,u32Len);
            }
            else if(6 == u8Opt)
            {//单个任务信息获取
                app_task_opt_put(APP_TASK_OPT_GET_INFO,(uint8_t *)&eModule,u32Len);
            }
            else if(7 == u8Opt)
            {//所有任务信息获取
                app_task_opt_put(APP_TASK_OPT_GET_INFO_ALL,NULL,0);
            }
        }
		#if APP_DEBUG_ENABLE//测试指令，仅调试用
        else if(0 == strncmp("bletest",(const char *)pData,7))
        {//ble模组测试
            uint8_t u8Opt = pData[7] - 0x30;

            if(0 == u8Opt)
            {
                bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_INFO,"connect state:%d",atk_ble03_get_conn_sta());
            }
            else if(1 == u8Opt)
            {//掉电
                ATK_BLE03_LDO(0);
            }
            else if(2 == u8Opt)
            {//上电
                ATK_BLE03_LDO(1);
            }
            else if(3 == u8Opt)
            {//接收ble主机的数据
                uint8_t *pRecvData = atk_ble03_uart_rx_get_frame();
                if (pRecvData != NULL)
                {
                    bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_INFO,"pRecvData:%s", pRecvData);
                    bsp_log_print_hex(BSP_LOG_MODULE_BLE,"pRecvData hex",(const uint8_t *)pRecvData,atk_ble03_uart_rx_get_frame_len());
                    atk_ble03_uart_rx_restart();//重置接收参数
                }
            }
            else if(4 == u8Opt)
            {//发送数据给ble主机
                uint8_t sendData[] = {0,1,2,3,4,5,6,7,8,9};
                bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_INFO,"sendData ret:%d", atk_ble03_uart_tx(sendData,sizeof(sendData),1000));
            }
            else if(5 == u8Opt)
            {//按键指令传递
                typedef struct
                {//指令cmdBleUpgradeReq对应的数据格式
                    uint8_t u8Button;
                    uint8_t res[3];
                }Param_t;
                Param_t stData = {4};
                app_ble_opt_put(APP_BLE_TX_EVENTS,APP_BLE_OPT_BUTTON,(uint8_t *)&stData,sizeof(Param_t));
            }
            else if(6 == u8Opt)
            {//up start指令传递
                typedef struct
                {
                    uint32_t u32Index;
                    uint8_t u8Flag;
                    uint8_t res1[3];
                }Param_t;
                Param_t stData = {4};
                app_ble_opt_put(APP_BLE_TX_EVENTS,APP_BLE_OPT_UPGRADE_START,(uint8_t *)&stData,sizeof(Param_t));
            }
            else if(7 == u8Opt)
            {//up data指令传递
                typedef struct
                {
                    uint32_t u32Index;
                    uint8_t u8Flag;
                    uint8_t res1[3];
                }Param_t;
                Param_t stData = {4};
                app_ble_opt_put(APP_BLE_TX_EVENTS,APP_BLE_OPT_UPGRADE_DATA,(uint8_t *)&stData,sizeof(Param_t));
            }
            else if(8 == u8Opt)
            {//up stop指令传递
                uint8_t u8Err = 1;
                app_ble_opt_put(APP_BLE_TX_EVENTS,APP_BLE_OPT_UPGRADE_STOP,(uint8_t *)&u8Err,1);
            }
        }
        // else if(0 == strncmp("Ymodem",(const char *)pData,6))
        // {//Ymodem测试
        //     int8_t name[] = "Tempproject_2241107.bin";
        //     uint32_t filesize = 54781;
        //     bsp_ymodem_send_filename(name,filesize);
        // }
        // else if(0 == strncmp("Md5Time",(const char *)pData,7))
        // {
        //     bsp_md5_ctx_t stMd5Ctx = {0};
        //     bsp_md5_init(&stMd5Ctx);
        //     uint8_t u8Md5[BSP_MD5_OUT_LEN];
        //     uint8_t data[BSP_UPGRADE_BUF_LEN];//获取升级类型信息
        //     for(uint32_t i = 0;i < BSP_UPGRADE_BUF_LEN;i++)
        //     {
        //         data[i] = i;
        //     }

        //     bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_WARNING,"md5 s\r\n");//测时间用
        //     for(uint32_t i = 0; i < BSP_UPGRADE_BUF_LEN; i+= 64)
        //     {
        //         bsp_md5_update(&stMd5Ctx,&data[i],64);
        //     }
        //     bsp_md5_final(u8Md5,&stMd5Ctx);
        //     bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_WARNING,"md5 e\r\n");//测时间用
        //     bsp_log_print_hex(BSP_LOG_MODULE_UPGRADE,"code md5",u8Md5,BSP_MD5_OUT_LEN);
        // }
        // else if(0 == strncmp("Uarttest",(const char *)pData,8))
        // {//uart阻塞超时接收测试
        //     HAL_UART_Abort_IT(&UART1_Handler);//取消中断式传输
        //     bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"uart state:0x%x",HAL_UART_GetState(&UART1_Handler));

        //     int8_t data[128] = {0};
        //     int ret = uart_tx_data(data,sizeof(data),BSP_YMODEM_PACKET_TIMEOUT);
        //     bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"uart rx start");
		// 	ret = uart_rx_data(data,sizeof(data),BSP_YMODEM_PACKET_TIMEOUT);
        //     bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"uart rx end:%d",ret);
        //     bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"uart state:0x%x",HAL_UART_GetState(&UART1_Handler));
        //     HAL_UART_Abort(&UART1_Handler);//取消阻塞式传输
        //     HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//启动uart中断接收
        //     bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"uart state:0x%x",HAL_UART_GetState(&UART1_Handler));
        // }
        // else if(0 == strncmp("aestest",(const char *)pData,7))
        // {//AES算法
        //     uint8_t u8Opt = pData[7] - 0x30;
        //     //30313233343536373839616263646566
        //     uint8_t data[] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,0x65,0x66};
        //     uint32_t length = sizeof(data);//16bytes
        //     //3031323334353637383961626364656634353637
        //     uint8_t s_data[] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,0x65,0x66,0x34,0x35,0x36,0x37};
        //     uint32_t s_length = sizeof(s_data);//20bytes
        //     uint8_t e_data[128] = {0};
        //     uint32_t e_length = sizeof(e_data);

        //     bsp_aes_ctx_t fw_bsp_aes_ctx;
        //     bsp_aes_init(&fw_bsp_aes_ctx,NULL);

        //     if(0 == u8Opt)
        //     {//测试NoPadding填充
        //         bsp_aes_encrypt_none(&fw_bsp_aes_ctx,data,length);
        //         bsp_log_print_hex(BSP_LOG_MODULE_LOG,"aes none en",data,length);

        //         bsp_aes_decrypt_none(&fw_bsp_aes_ctx,data,length);
        //         bsp_log_print_hex(BSP_LOG_MODULE_LOG,"aes none de",data,length);
        //     }
        //     else if(1 == u8Opt)
        //     {//测试PKCS7
        //         bsp_aes_encrypt(&fw_bsp_aes_ctx,s_data,s_length,e_data,&e_length);
        //         bsp_log_print_hex(BSP_LOG_MODULE_LOG,"aes PKCS7 en",e_data,e_length);

        //         bsp_aes_decrypt(&fw_bsp_aes_ctx,e_data,e_length,s_data,&s_length);
        //         bsp_log_print_hex(BSP_LOG_MODULE_LOG,"aes PKCS7 de",s_data,s_length);
        //     }
        // }
		// else if(0 == strncmp("Jump",(const char *)pData,4))
        // {//跳转到boot
        //     app_jump_to_boot();
        // }
        // else if(0 == strncmp("exflash",(const char *)pData,7))
        // {//外部flash测试
        //     uint8_t u8Data[16] = {0};
        //     uint8_t u8Opt = pData[7] - 0x30;
        //     if(0 == u8Opt)
        //     {//写入
        //         for(uint8_t i = 0;i< sizeof(u8Data);i++)
        //         {
        //             u8Data[i] = i;
        //         }
        //         W25QXX_Write(u8Data,0x00000000,sizeof(u8Data));
        //     }
        //     else if(1 == u8Opt)
        //     {//读取
        //         W25QXX_Read(u8Data,0x00000000,sizeof(u8Data));
        //         bsp_log_print_hex(BSP_LOG_MODULE_LOG,"exflash info hex",u8Data,sizeof(u8Data));
        //     }
        //     else if(2 == u8Opt)
        //     {//擦除指定扇区，4k
        //         W25QXX_Erase_Sector(0x00000000);
        //     }
        //     else if(3 == u8Opt)
        //     {//获取id
        //         bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"exflash ID:0x%x\r\n",W25QXX_ReadID());
        //     }
        //     else if(4 == u8Opt)
        //     {//唤醒
        //         W25QXX_WAKEUP();
        //     }
        //     else if(5 == u8Opt)
        //     {//掉电休眠
        //         W25QXX_PowerDown();
        //     }
        //     else if(6 == u8Opt)
        //     {//整片擦除
        //         W25QXX_Erase_Chip();//大概需要4s
        //         bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"exflash erase chip end");
        //     }
        // }
        // else if(0 == strncmp("inflash",(const char *)pData,7))
        // {//内部flash测试
        //     bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"proBrushInfoAddr:0x%x\r\n",APP_PRO_BRUSH_FINFO_ADDR);
        //     uint8_t u8Data[16] = {0};
        //     uint8_t u8Opt = pData[7] - 0x30;
        //     if(0 == u8Opt)
        //     {//写入
        //         for(uint8_t i = 0;i< sizeof(u8Data);i++)
        //         {
        //             u8Data[i] = i;
        //         }
        //         STMFLASH_Write(APP_PRO_BRUSH_FINFO_ADDR,(uint16_t *)u8Data,sizeof(u8Data)/2);
        //     }
        //     else if(1 == u8Opt)
        //     {//读取
        //         STMFLASH_Read(APP_PRO_BRUSH_FINFO_ADDR,(uint16_t *)u8Data,sizeof(u8Data)/2);
        //         bsp_log_print_hex(BSP_LOG_MODULE_LOG,"inflash info hex",u8Data,sizeof(u8Data));
        //     }
        //     else if(2 == u8Opt)
        //     {//擦除指定扇区，2k
        //         STMFLASH_Erase(APP_PRO_BRUSH_FINFO_ADDR);
        //     }
        // }
        // else if(0 == strncmp("timtest",(const char *)pData,7))
        // {
        //     uint8_t u8Opt = pData[7] - 0x30;
        //     if(0 == u8Opt)
        //     {//启动tim3
        //         HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
        //     }
        //     else if(1 == u8Opt)
        //     {//停止tim3
        //         HAL_TIM_Base_Stop_IT(&TIM3_Handler);//如果先stop，再start会在上次的基础上继续计数，不会重置
        //     }
        //     else if(2 == u8Opt)
        //     {//重置tim3计数值
        //         __HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//tim运行状态下，重新配置计数器
        //     }
        //     else if(3 == u8Opt)
        //     {//获取tim3状态
        //         bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"tim3 state:%d,tim2 state:%d\r\n"
        //         ,HAL_TIM_Base_GetState(&TIM3_Handler),HAL_TIM_Base_GetState(&TIM2_Handler));
        //     }
        // }
        // else if(0 == strncmp("rtc",(const char *)pData,3))
        // {//rtc测试
        //     uint8_t u8Opt = pData[3] - 0x30;
        //     if(0 == u8Opt)
        //     {
		// 		RTC_Get();
		// 		bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO,"rtc:%d.%d.%d-%d:%d:%d-(%d)\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec,calendar.week);
		// 	}
		// 	else if(1 == u8Opt)
        //     {
		// 		RTC_Set(2024,9,30,17,30,30);
		// 	}
        //     else if(2 == u8Opt)
        //     {
		// 		bsp_time_set_stamp(1728353181);
		// 	}
        //     else if(3 == u8Opt)
        //     {
		// 		bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO
        //             ,"stamep:%d--Week:%d--yDays:%d\r\nString:%s\r\n"
        //             ,bsp_time_get_stamp(),bsp_time_get_week(),bsp_time_get_ydays(),bsp_time_get_string());
		// 	}
        //     else if(4 == u8Opt)
        //     {
		// 		bsp_time_calendar_t stCalendar = {2024,10,5,20,30,35};
        //         bsp_time_set_caledar(stCalendar);
		// 	}
        //     else if(5 == u8Opt)
        //     {
        //         bsp_time_calendar_t stCalendar = {0};
        //         bsp_time_get_caledar(&stCalendar);

		// 		bsp_log_print(BSP_LOG_MODULE_LOG,BSP_LOG_TYPE_INFO
        //             ,"%d.%d.%d-%d:%d:%d\r\nyTypes:%d\r\n"
        //             ,stCalendar.year,stCalendar.month,stCalendar.day
		// 			,stCalendar.hour,stCalendar.min,stCalendar.sec
		// 			,bsp_time_get_year_type(stCalendar.year));
		// 	}
        // }
		#endif//测试指令，仅调试用
        else
        {//可以无限扩充
        }
    }
}

