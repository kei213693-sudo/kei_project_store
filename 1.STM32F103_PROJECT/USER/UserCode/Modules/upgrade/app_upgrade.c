/*
 * app_upgrade.c
 *
 *  Created on: 2024年10月28日
 *      Author: lijing
 *	  Abstract: 程序刷写模块，实现多途径程序接收、校验、存储、刷写的流程，app层处理"传输方式、通信协议解析、传输过程加解密"，其他交给bsp层
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "app_upgrade.h"
#include "bsp_upgrade.h"
#include "bsp_ymodem.h"
#include "queue.h"
#include "timers.h"
#include "portable.h"
#include "bsp_log.h"
#include "app_task.h"
#include "app_ble.h"
#include "delay.h"

/*******************************全局变量定义*******************************/
TaskHandle_t g_UpgradeTaskHandle  = NULL;//任务句柄

/*******************************内部变量定义*******************************/
static QueueHandle_t g_hUpgradeQueue = NULL;//队列句柄
static TimerHandle_t g_hUpgradeTimer = NULL;//定时器句柄

/*******************************内部函数的声明*******************************/
static void app_upgrade_task(void* pvParameters);
static void app_upgrade_timeout_set(uint8_t u8flag);
static void app_upgrade_vTimerCallback(TimerHandle_t xTimer);

static void app_upgrade_start(uint8_t *pData,uint32_t u32Len);
static void app_upgrade_data(uint8_t *pData,uint32_t u32Len);
static void app_upgrade_stop(uint8_t *pData,uint32_t u32Len);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：创建程序刷写任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：
**************************************************************/
int32_t app_upgrade_init(void)
{
    int32_t i32ret = pdPASS;
	
    //创建队列
	g_hUpgradeQueue = xQueueCreate(APP_UPGRADE_QUEUE_LEN,sizeof(APP_UPGRADE_QUEUE_REC_T));
    if(NULL == g_hUpgradeQueue)
    {
        bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_WARNING,"Create g_hUpgradeQueue fail.");
    }
    //创建定时器
    g_hUpgradeTimer = xTimerCreate("UpgradeTimer",pdMS_TO_TICKS(APP_UPGRADE_TIMEOUT)
                        ,pdFALSE//一次性
                        ,(void *)0//ID为0，该回调函数只分配给这一个句柄
                        ,app_upgrade_vTimerCallback);
    if(NULL == g_hUpgradeTimer)
    {
        bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_WARNING,"Create g_hUpgradeTimer fail.");
    }
    //申请内存
    g_pStBspUpgradeCtl = pvPortMalloc(sizeof(bsp_upgrade_ctl_t));
    if(NULL == g_pStBspUpgradeCtl)
    {
        bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_WARNING,"g_pStBspUpgradeCtl malloc fail!");
    }
    //创建任务
    i32ret = (int32_t)xTaskCreate(app_upgrade_task
                                ,"app_upgrade_task"
                                ,APP_UPGRADE_STACK_DEPTH
                                ,NULL
                                ,APP_UPGRADE_PRIORITY
                                ,&g_UpgradeTaskHandle);
    if(pdPASS != i32ret)
    {
        bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_WARNING,"Create app_upgrade_task fail.");
    }

    return i32ret;
}

/**************************************************************
*函数功能描述 ：删除log任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：
**************************************************************/
int32_t app_upgrade_deinit(void)
{
    int32_t i32ret = pdPASS;
/*
    //测试用，查看该任务所需空间
    APP_TASK_MODULE_E eModule = APP_TASK_MODULE_UPGRADE;
    uint32_t u32Len = sizeof(eModule);
    app_task_opt_put(APP_TASK_OPT_GET_INFO,(uint8_t *)&eModule,u32Len);
    app_task_opt_put(APP_TASK_OPT_GET_INFO_ALL,NULL,0);
*/
    vPortFree(g_pStBspYmodemCtl);//释放内存空间
    vPortFree(g_pStBspUpgradeCtl);//释放内存空间
    xTimerDelete(g_hUpgradeTimer,0);//删除定时器，不阻塞
    vQueueDelete(g_hUpgradeQueue);//删除队列
    vTaskDelete(g_UpgradeTaskHandle);//删除任务

    return i32ret;
}

/**************************************************************
*函数功能描述 ：程序刷写任务函数
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_upgrade_task(void* pvParameters)
{
    APP_UPGRADE_QUEUE_REC_T *pRec = NULL;

    if(NULL == g_hUpgradeQueue)
    {
        // app_task_delete(APP_TASK_MODULE_UPGRADE);//删掉
        APP_TASK_MODULE_E eModule = APP_TASK_MODULE_UPGRADE;
        app_task_opt_put(APP_TASK_OPT_DELETE,&eModule,sizeof(eModule));
    }

    while(1)
    {
        if((pdPASS == xQueueReceive(g_hUpgradeQueue,(void *)&pRec,portMAX_DELAY))//阻塞等待队列有信息
            && (NULL != pRec))//获取的信息不为NULL
        {
            if(APP_UPGRADE_OPT_YMODEM == pRec->eOpt)
            {
                g_pStBspYmodemCtl = pvPortMalloc(sizeof(bsp_ymodem_ctl_t));
                if(NULL == g_pStBspYmodemCtl)
                {
                    bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_WARNING,"g_pStBspYmodemCtl malloc fail!");
                }
                else
                {
                    bsp_ymodem_recv((uint8_t)*pRec->pData);
                }
                
                // app_task_delete(APP_TASK_MODULE_UPGRADE);//删掉
                APP_TASK_MODULE_E eModule = APP_TASK_MODULE_UPGRADE;
                app_task_opt_put(APP_TASK_OPT_DELETE,&eModule,sizeof(eModule));
            }
            else if(APP_UPGRADE_OPT_START == pRec->eOpt)
            {
                app_upgrade_start(pRec->pData,pRec->u32Len);
                app_upgrade_timeout_set(1);
            }
            else if(APP_UPGRADE_OPT_DATA == pRec->eOpt)
            {
                app_upgrade_data(pRec->pData,pRec->u32Len);
                app_upgrade_timeout_set(1);
            }
            else if(APP_UPGRADE_OPT_STOP == pRec->eOpt)
            {
                app_upgrade_stop(pRec->pData,pRec->u32Len);
                app_upgrade_timeout_set(0);//一般也不会运行到这，直接就重启了
            }
            else
            {
                bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_WARNING,"eOpt is err:%d",pRec->eOpt);
            }
            
            //处理完成，释放信息节点
            if(NULL != pRec->pData)
            {
                vPortFree(pRec->pData);
            }
            vPortFree(pRec);
        }
    }
}

/**************************************************************
*函数功能描述 ：信息入队列，供外部模块调用
*函数参数 ：eOpt，app层自定义操作码，不同的操作码对应不同的数据结构
            pData，待处理数据地址
            u32Len，待处理数据长度
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注 ：因为需要pvPortMalloc（），所以不能用在中断里调用
**************************************************************/
int32_t app_upgrade_opt_put(APP_UPGRADE_OPT_E eOpt,uint8_t *pData,uint32_t u32Len)
{
    int32_t i32ret = pdFAIL;
    APP_UPGRADE_QUEUE_REC_T *pRec = NULL;

    if((eOpt < APP_UPGRADE_OPT_END) && (NULL != g_hUpgradeQueue))
    {
        pRec = pvPortMalloc(sizeof(APP_UPGRADE_QUEUE_REC_T));//失败就在本函数释放，成功就在task中释放
        if(NULL != pRec)
        {
            pRec->eOpt = eOpt;
            if((NULL != pData) && (0 != u32Len))
            {
                pRec->u32Len = u32Len;
                pRec->pData = (uint8_t *)pvPortMalloc(u32Len);//失败就在本函数释放，成功就在task中释放
                if(NULL != pRec->pData)
                {
                    memcpy(pRec->pData,pData,u32Len);
                }
                else
                {
                    vPortFree(pRec);
                    return i32ret;
                }
            }
            else
            {
                pRec->u32Len = 0;
                pRec->pData = NULL;
            }

            //数据组包完成，放入队列，放满了就阻塞在这里
            (int32_t)xQueueSend(g_hUpgradeQueue,(void *)&pRec,portMAX_DELAY);
            i32ret = pdPASS;
        }
    }
    
    return i32ret;
}

/**************************************************************
*函数功能描述 ：配置超时定时器的启动、停止、重启
*函数参数 ：u8flag，0，停止定时任务
            1，启动/重启定时任务
*函数返回值 ：
*备注 ：自定义协议超时用
**************************************************************/
static void app_upgrade_timeout_set(uint8_t u8flag)
{
    if(NULL != g_hUpgradeTimer)
    {
        if(0 == u8flag)
        {
            xTimerStop(g_hUpgradeTimer,0);//删除
        }
        else
        {
            if(pdFALSE != xTimerIsTimerActive(g_hUpgradeTimer))
            {//已激活，重置计时
                xTimerReset(g_hUpgradeTimer,0);
            }
            else
            {//未激活
                xTimerStart(g_hUpgradeTimer,0);
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：连续APP_UPGRADE_TIMEOUT（60s）没有接收到新的数据包就退出接收流程，并删除升级任务
*函数参数 ：
*函数返回值 ：
*备注 ：自定义协议超时用
**************************************************************/
static void app_upgrade_vTimerCallback(TimerHandle_t xTimer)
{
    if(NULL != xTimer)
    {
        bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_WARNING,"upgrade timeout");
        g_pStBspUpgradeCtl->u8ReceiveFlag = 0;
        // app_task_delete(APP_TASK_MODULE_UPGRADE);//删掉
        APP_TASK_MODULE_E eModule = APP_TASK_MODULE_UPGRADE;
        app_task_opt_put(APP_TASK_OPT_DELETE,&eModule,sizeof(eModule));
    }
}


/**************************************************************
*函数功能描述 ：自定协议，起始包，判定产品类型、版本、长度等是否符合规范，判定是从头传递还是续传
*函数参数 ：
*函数返回值 ：标志类型
*备注 ：
**************************************************************/
static void app_upgrade_start(uint8_t *pData,uint32_t u32Len)
{
    if((NULL == pData) || (0 == u32Len) || (NULL == g_pStBspUpgradeCtl))
    {
        return;
    }
    bsp_log_print_hex(BSP_LOG_MODULE_UPGRADE,"upgrade start",pData,u32Len);

    typedef struct
    {
        uint8_t u8Type;//代码类型，用于区分app、boot、外部组件模块等升级信息，也可以用高位字节存储产品类型
        uint8_t res1[3];
        uint32_t u32Version;//已存储程序的软件版本，例如220701或V1.0.0
        uint8_t u8Name[32];//代码文件名称
        uint32_t u32TotalLen;//总长度，bytes，最大接收4095Mb的数据
    }Param_t;
    
    typedef struct
    {
        uint32_t u32Index;//已存储程序索引
        uint8_t u8Flag;//0x00：从头开始升级
                        // 0x01：续传升级
                        // 0x02：不允许升级，类型异常
                        // 0x03：不允许升级，版本异常
                        // 0x04：不允许升级，文件大小异常
                        // 0xFF：其他错误
        uint8_t res1[3];
    }Ret_t;

    Param_t *pStData = (Param_t *)pData;
    Ret_t stRet = {0};

    if(pStData->u8Type != 0x00)
    {
        //根据u8Type决定处理哪个区域，默认APP
        bsp_upgrade_init(BSP_UPGRADE_AREA_APP);//初始化app区域的控制结构体
        if(pStData->u32Version > 0x00000000)
        {
            if(pStData->u32TotalLen <= g_pStBspUpgradeCtl->u32StoreSize)
            {
                if((0x03 == g_pStBspUpgradeCtl->stInfo.u8UpFlag)
                    && (0 == strncmp((const char *)&g_pStBspUpgradeCtl->stInfo,(const char *)pStData,sizeof(Param_t)))//校验包头参数
                    && (0 == (g_pStBspUpgradeCtl->stBuf.u16BufLen % APP_UPGRADE_SINGLE_LEN)))//校验u32StoreLen参数
                {
                    stRet.u8Flag = 0x01;
                    stRet.u32Index = (g_pStBspUpgradeCtl->stInfo.u32StoreLen / APP_UPGRADE_SINGLE_LEN) + 1;
                    bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_ERROR,"continuation[%d]:%d bytes",stRet.u32Index,g_pStBspUpgradeCtl->stInfo.u32StoreLen);
                }//否则就从头开始升级
            }
            else
            {
                stRet.u8Flag = 0x04;
            }
        }
        else
        {
            stRet.u8Flag = 0x03;
        }
    }
    else
    {
        stRet.u8Flag = 0x02;
    }
    
    //确定是否允许升级
    if(stRet.u8Flag == 0x00)
    {
        g_pStBspUpgradeCtl->u32ReceiveIndex = 1;
        g_pStBspUpgradeCtl->u32ReceiveLen = 0;
        g_pStBspUpgradeCtl->u8ReceiveFlag = 1;//允许接收
        g_pStBspUpgradeCtl->u8ProtocolType = 1;

        memcpy(&g_pStBspUpgradeCtl->stInfo,pData,sizeof(Param_t));//暂时不更新到升级信息存储区域
        bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_INFO,"upgrade start anew");
    }
    else if(stRet.u8Flag == 0x01)
    {
        g_pStBspUpgradeCtl->u32ReceiveIndex = stRet.u32Index;
        g_pStBspUpgradeCtl->u32ReceiveLen = g_pStBspUpgradeCtl->stInfo.u32StoreLen;
        g_pStBspUpgradeCtl->u8ReceiveFlag = 1;//允许接收
        g_pStBspUpgradeCtl->u8ProtocolType = 1;

        g_pStBspUpgradeCtl->u32StoreAddr += g_pStBspUpgradeCtl->stInfo.u32StoreLen;//存储位置校准
        bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_INFO,"upgrade start continue");
    }

    //调用外部接口，返回stRet数值给对端设备
    app_ble_opt_put(APP_BLE_TX_EVENTS,APP_BLE_OPT_UPGRADE_START,(uint8_t *)&stRet,sizeof(stRet));
}

/**************************************************************
*函数功能描述 ：自定协议，数据包
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_upgrade_data(uint8_t *pData,uint32_t u32Len)
{
    if((NULL == pData) || (0 == u32Len) || (NULL == g_pStBspUpgradeCtl)
        || (1 != g_pStBspUpgradeCtl->u8ReceiveFlag))
    {
        return;
    }
    bsp_log_print_hex(BSP_LOG_MODULE_UPGRADE,"upgrade data",pData,u32Len);

    typedef struct
    {
        uint32_t u32Index;//包序号，表示是第几包数据
        uint8_t *u8pData;//单包传送的实际数据，需要是4的倍数,APP_UPGRADE_SINGLE_LEN
    }Param_t;

    typedef struct
    {//返回响应
        uint32_t u32Index;//包序号，表示是第几包数据
        uint8_t u8Flag;//1失败，0成功
        uint8_t res[3];
    }Ret_t;

    Param_t *pStData = (Param_t *)pData;
    Ret_t stRet = {0};
    uint8_t u8Force = 0;

    if(g_pStBspUpgradeCtl->u32ReceiveIndex == pStData->u32Index)
    {//判断包序号正确
        g_pStBspUpgradeCtl->u32ReceiveIndex++;//自增

        g_pStBspUpgradeCtl->u32ReceiveLen += (u32Len - sizeof(pStData->u32Index));
        if(g_pStBspUpgradeCtl->u32ReceiveLen >= g_pStBspUpgradeCtl->stInfo.u32TotalLen)
        {//判断接收长度
            u8Force = 1;//接收完成
            g_pStBspUpgradeCtl->u8ReceiveFlag = 2;//接收完成
            bsp_log_print(BSP_LOG_MODULE_UPGRADE,BSP_LOG_TYPE_INFO,"upgrade data Receive over:%d,%d,%d",g_pStBspUpgradeCtl->u32ReceiveLen,g_pStBspUpgradeCtl->stInfo.u32TotalLen,stRet.u32Index);
        }

        bsp_upgrade_store(pStData->u8pData,(u32Len - sizeof(pStData->u32Index)),u8Force);
    }
    else
    {
        stRet.u8Flag = 1;
    }

    //调用外部接口，返回stRet数值给对端设备
    stRet.u32Index = g_pStBspUpgradeCtl->u32ReceiveIndex - 1;
    app_ble_opt_put(APP_BLE_TX_EVENTS,APP_BLE_OPT_UPGRADE_DATA,(uint8_t *)&stRet,sizeof(stRet));
}


/**************************************************************
*函数功能描述 ：自定协议，终止包
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_upgrade_stop(uint8_t *pData,uint32_t u32Len)
{
    if((NULL == pData) || (0 == u32Len) || (NULL == g_pStBspUpgradeCtl)
        || (2 != g_pStBspUpgradeCtl->u8ReceiveFlag))
    {
        return;
    }
    bsp_log_print_hex(BSP_LOG_MODULE_UPGRADE,"upgrade stop",pData,u32Len);

    uint8_t u8Ret = 1;

    #if (0 == BSP_UPGRADE_AUTH_EN)
    if(0 == strncmp((const char *)pData,(const char *)g_pStBspUpgradeCtl->stInfo.u8Md5,BSP_MD5_OUT_LEN))
    {//直接校验md5数值，通过
        u8Ret = 0;
    }
    #else
    u8Ret == bsp_upgrade_auth(pData,BSP_RSA_ENCODE_LEN);
    #endif

    //调用外部接口，返回stRet数值给对端设备
    app_ble_opt_put(APP_BLE_TX_EVENTS,APP_BLE_OPT_UPGRADE_STOP,(uint8_t *)&u8Ret,1);
    delay_ms(1000);//延时1s等待ble返回响应

    bsp_upgrade_finish(u8Ret);
}

