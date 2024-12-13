/*
 * app_ble.c
 *
 *  Created on: 2023年6月19日
 *      Author: F704011834_
 *	  Abstract: ble模块，实现ble应用逻辑
 */
/*******************************头文件*******************************/
#include <stdint.h>
#include <string.h>

#include "app_ble.h"
#include "bsp_ble.h"
#include "portable.h"
#include "task.h"
#include "queue.h"
#include "bsp_log.h"
#include "app_comm.h"
#include "app_upgrade.h"
#include "bsp_upgrade.h"
#include "app_task.h"

/*******************************内部函数的声明*******************************/
static void app_ble_task(void* pvParameters);
static void app_ble_processStackRec(APP_BLE_QUEUE_REC_T stRec);
static void app_ble_processTxRec(APP_BLE_QUEUE_REC_T stRec);
static void app_ble_processRxRec(APP_BLE_QUEUE_REC_T stRec);

static void app_ble_cmdBleUpgradeStartReq(void *pData);
static void app_ble_cmdBleUpgradeStartRsp(void *pData);
static void app_ble_cmdBleUpgradDataReq(void *pData);
static void app_ble_cmdBleUpgradDataRsp(void *pData);
static void app_ble_cmdBleUpgradStopReq(void *pData);
static void app_ble_cmdBleUpgradStopRsp(void *pData);
static void app_ble_cmdBleButtonReq(void *pData);

/*******************************全局变量定义*******************************/
TaskHandle_t g_BleTaskHandle  = NULL;//任务句柄

/*******************************内部变量定义*******************************/
static QueueHandle_t g_hBleQueue = NULL;
static QueueHandle_t g_hBleTxQueue = NULL;
static QueueHandle_t g_hBleRxQueue = NULL;

static const BSP_PROTOCOL_API_T* const g_pBspProtocolApi = &g_BspProtocolApi;//不同的开发环境可以切换成不同的api函数组
static protocol_ctl_t g_stBleProCtl = {0};//ble专用protocol控制结构体

static const APP_BLE_STACK_FUC_TAB_T g_stBleStackFucTab[] =
{//自定义指令处理函数列表
    {APP_BLE_OPT_START_ADV
    ,bsp_ble_start_adv},

    {APP_BLE_OPT_STOP_ADV
    ,bsp_ble_stop_adv},

    {APP_BLE_OPT_GAP_DISCON
    ,bsp_ble_disconnect},

    {APP_BLE_OPT_GATT_SERVER_TX//slave发送数据
    ,bsp_ble_send_data_notify},

    {APP_BLE_OPT_SET_POWER
    ,bsp_ble_set_power}
};

static const APP_BLE_CMD_FUC_TAB_T g_stBleCmdFucTab[] =
{//自定义指令处理函数列表
    {APP_BLE_OPT_UPGRADE_START
    ,cmdBleUpgradeStartRsp
    ,app_ble_cmdBleUpgradeStartRsp
    ,cmdBleUpgradeStartReq
    ,app_ble_cmdBleUpgradeStartReq},

    {APP_BLE_OPT_UPGRADE_DATA
    ,cmdBleUpgradDataRsp
    ,app_ble_cmdBleUpgradDataRsp
    ,cmdBleUpgradDataReq
    ,app_ble_cmdBleUpgradDataReq},

    {APP_BLE_OPT_UPGRADE_STOP
    ,cmdBleUpgradStopRsp
    ,app_ble_cmdBleUpgradStopRsp
    ,cmdBleUpgradStopReq
    ,app_ble_cmdBleUpgradStopReq},

    {APP_BLE_OPT_BUTTON
    ,cmdBleButtonReq
    ,app_ble_cmdBleButtonReq
    ,cmdBleButtonRsp
    ,NULL}
};

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：创建ble任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_ble_init(void)
{
    int32_t i32ret = pdPASS;

    bsp_ble_init(0);//ble底层协议栈 or 模块初始化

    //创建队列
    g_hBleQueue   = xQueueCreate(APP_BLE_QUEUE_LEN,sizeof(APP_BLE_QUEUE_REC_T));
    g_hBleTxQueue = xQueueCreate(APP_BLE_QUEUE_LEN,sizeof(APP_BLE_QUEUE_REC_T));
    g_hBleRxQueue = xQueueCreate(APP_BLE_QUEUE_LEN,sizeof(APP_BLE_QUEUE_REC_T));
    if((NULL == g_hBleQueue) | (NULL == g_hBleTxQueue) | (NULL == g_hBleRxQueue))
    {
        bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_ERROR,"Create g_hBleQueue fail");
    }
    
    //创建任务
    i32ret = xTaskCreate(app_ble_task
                        ,"app_ble_task"
                        ,APP_BLE_STACK_DEPTH
                        ,NULL
                        ,APP_BLE_PRIORITY
                        ,&g_BleTaskHandle);
    if(i32ret != pdPASS)
    {
        bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_ERROR,"Create app_ble_task fail");
    }

    return i32ret;
}

/**************************************************************
*函数功能描述 ：删除ble任务
*函数参数 ：
*函数返回值 ：pdPASS表示成功，pdFAIL表示失败
*备注：
**************************************************************/
int32_t app_ble_deinit(void)
{
    int32_t i32ret = pdPASS;

    vQueueDelete(g_hBleQueue);//删除队列
    vQueueDelete(g_hBleTxQueue);//删除队列        
    vQueueDelete(g_hBleRxQueue);//删除队列
    vTaskDelete(g_BleTaskHandle);//删除任务

    return i32ret;
}

/**************************************************************
*函数功能描述 ：ble任务函数
*函数参数 ：
*函数返回值 ：
*备注：该任务有多个队列需要处理，所以这里的不能直接阻塞某个队列
    ，使用事件组来同步任务，然后分别处理不同的队列信息
**************************************************************/
static void app_ble_task(void* pvParameters)
{
    EventBits_t events = 0;
    APP_BLE_QUEUE_REC_T stRec;

    while(1)
    {
        memset(&stRec,0,sizeof(APP_BLE_QUEUE_REC_T));

        events = bsp_task_events_wait(APP_BLE_ALL_EVENTS,portMAX_DELAY);//阻塞在这里
        if(events)
        {
            if((events & APP_BLE_STACK_EVENTS)
                && (NULL != g_hBleQueue))
			{//一直读取队列数据，然后处理，直到队列为空
                while(uxQueueMessagesWaiting(g_hBleQueue))//返回当前队列有效元素数目
                {
                    if(pdPASS == xQueueReceive(g_hBleQueue,(void *)&stRec,0))
                    {//不阻塞,接收数据成功后自动从队列中删除项目
                        app_ble_processStackRec(stRec);
                    }
                }
            }

            if((events & APP_BLE_TX_EVENTS)
                && (NULL != g_hBleTxQueue))
			{//一直读取队列数据，然后处理，直到队列为空
                while(uxQueueMessagesWaiting(g_hBleTxQueue))//返回当前队列有效元素数目
                {
                    if(pdPASS == xQueueReceive(g_hBleTxQueue,(void *)&stRec,0))
                    {//不阻塞,接收数据成功后自动从队列中删除项目
                        app_ble_processTxRec(stRec);
                    }
                }
            }

            if((events & APP_BLE_RX_EVENTS)
                && (NULL != g_hBleRxQueue))
			{//一直读取队列数据，然后处理，直到队列为空
                while(uxQueueMessagesWaiting(g_hBleRxQueue))//返回当前队列有效元素数目
                {
                    if(pdPASS == xQueueReceive(g_hBleRxQueue,(void *)&stRec,0))
                    {//不阻塞,接收数据成功后自动从队列中删除项目
                        app_ble_processRxRec(stRec);
                    }
                }
            }

            //处理完成，释放信息节点
            if(NULL != stRec.pData)
            {
                vPortFree(stRec.pData);
            }
        }
    }
}

/**************************************************************
*函数功能描述 ：将待处理的操作和对应的数据传递给任务（供外部调用，内部可以直接调用app_ble_enQueueRec）
*函数参数 ：enOpt，app层自定义操作码，不同的操作码对应不同的数据结构
            pData，待处理数据
            u32Len，待处理数据长度
*函数返回值 ：
*备注：中断无法调用
**************************************************************/
int32_t app_ble_opt_put(EventBits_t events,APP_BLE_OPT_E enOpt,uint8_t *pData,uint32_t u32Len)
{
    int32_t i32ret = pdFAIL;
    APP_BLE_QUEUE_REC_T stRec;
    memset(&stRec,0,sizeof(APP_BLE_QUEUE_REC_T));

    if((events & APP_BLE_ALL_EVENTS) && (enOpt < APP_BLE_OPT_END))
    {
        stRec.enOpt = enOpt;
        if(NULL != pData)
        {//有数据
            stRec.u32Len = u32Len;
            stRec.pData = (uint8_t *)pvPortMalloc(u32Len);//失败就在本函数释放，成功就在task中释放
            if(NULL != stRec.pData)
            {
                memcpy(stRec.pData,pData,u32Len);
            }
            else
            {
                return i32ret;
            }
        }
        else
        {//没有数据
            stRec.u32Len = 0;
            stRec.pData = NULL;
        }

        //数据组包完成，放入队列，放满了就阻塞在这里
        if((events & APP_BLE_STACK_EVENTS) && (NULL != g_hBleQueue))
        {
            xQueueSend(g_hBleQueue,(void *)&stRec,portMAX_DELAY);
        }
        else if((events & APP_BLE_TX_EVENTS) && (NULL != g_hBleTxQueue))
        {
            xQueueSend(g_hBleTxQueue,(void *)&stRec,portMAX_DELAY);
        }
        else if((events & APP_BLE_RX_EVENTS) && (NULL != g_hBleRxQueue))
        {
            xQueueSend(g_hBleRxQueue,(void *)&stRec,portMAX_DELAY);
        }
        else
        {}

        bsp_task_events_set(events);//传递事件
        i32ret = pdPASS;
    }

    return i32ret;
}

/**************************************************************
*函数功能描述 ：ble stack自定义事件处理线程
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_processStackRec(APP_BLE_QUEUE_REC_T stRec)
{
    uint8_t u8Count = 0;
    bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_INFO,"App ble stack opt:%d",stRec.enOpt);

    for(u8Count = 0;u8Count < (sizeof(g_stBleStackFucTab)/sizeof(g_stBleStackFucTab[0]));u8Count++)
    {
        if(stRec.enOpt == g_stBleStackFucTab[u8Count].enOpt)
        {//判断事件类型
            if(NULL != g_stBleStackFucTab[u8Count].appBleFuc)
            {
                (*g_stBleStackFucTab[u8Count].appBleFuc)(&stRec);//需要长度信息，所以传pRec，而不是stRec.pData
            }
            break;
        }
    }
}

/**************************************************************
*函数功能描述 ：ble tx自定义事件处理线程
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_processTxRec(APP_BLE_QUEUE_REC_T stRec)
{
    uint8_t u8Count = 0;
    bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_INFO,"App ble tx opt:%d",stRec.enOpt);

    for(u8Count = 0;u8Count < (sizeof(g_stBleCmdFucTab)/sizeof(g_stBleCmdFucTab[0]));u8Count++)
    {
        if(stRec.enOpt == g_stBleCmdFucTab[u8Count].enOpt)
        {//判断事件类型
            if(NULL != g_stBleCmdFucTab[u8Count].appBleFucTx)
            {
                (*g_stBleCmdFucTab[u8Count].appBleFucTx)(stRec.pData);
            }
            break;
        }
    }
}

/**************************************************************
*函数功能描述 ：ble rx自定义事件处理线程
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_processRxRec(APP_BLE_QUEUE_REC_T stRec)
{
    uint8_t u8Count = 0;
    bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_INFO,"App ble rx opt:%d",stRec.enOpt);

    for(u8Count = 0;u8Count < (sizeof(g_stBleCmdFucTab)/sizeof(g_stBleCmdFucTab[0]));u8Count++)
    {
        if(stRec.enOpt == g_stBleCmdFucTab[u8Count].enOpt)
        {//判断事件类型
            if(NULL != g_stBleCmdFucTab[u8Count].appBleFucRx)
            {
                (*g_stBleCmdFucTab[u8Count].appBleFucRx)(stRec.pData);
            }
            break;
        }
    }
}

/**************************************************************
*函数功能描述 ：返回当前时间最后一包数据的响应指令码
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
uint16_t app_ble_protocol_last_rsp(void)
{
    return g_stBleProCtl.lasrsp;
}

/**************************************************************
*函数功能描述 ：封装ble发送数据接口，提供给给外界使用
*函数参数 ：buf，指向数据指针,comm进程传递过来的数据
            len，数据长度
*函数返回值 ：
*备注：进入这里前已经组包组好了，就不要去操作g_stBleProCtl.txbuf参数
**************************************************************/
void app_ble_protocol_tx(uint8_t *pBuf,uint32_t len)
{
    if((NULL != pBuf) && (0 != len))
    {
        app_ble_opt_put(APP_BLE_STACK_EVENTS,APP_BLE_OPT_GATT_SERVER_TX,(uint8_t *)pBuf,len);//从机发送
    }
}

/**************************************************************
*函数功能描述 ：解析协议函数
*函数参数 ：buf，指向数据指针,comm进程传递过来的数据
            len，数据长度
*函数返回值 ：
*备注：APP_BLE_OPT_GATT_SERVER_RX事件中调用
**************************************************************/
void app_ble_protocol_rx(uint8_t *pBuf,uint32_t len)
{
    uint16_t u16Cmd = 0;
    uint8_t u8Count = 0;
    if((len <= sizeof(g_stBleProCtl.rxbuf)) && (NULL != pBuf))
    {
        memset(g_stBleProCtl.rxbuf,0,sizeof(g_stBleProCtl.rxbuf));//先清空rxbuffer
        memcpy(g_stBleProCtl.rxbuf, pBuf, len);

        u16Cmd = protocol_parse(&g_stBleProCtl,g_stBleProCtl.rxbuf,len);
        bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_INFO,"rx cmd %02x:%02x",(uint8_t)u16Cmd,(uint8_t)(u16Cmd>>8));

        //将cmd指令转换为APP_BLE_EVENT_E事件传递给进程
        for(u8Count = 0;u8Count < (sizeof(g_stBleCmdFucTab)/sizeof(g_stBleCmdFucTab[0]));u8Count++)
        {
            if(u16Cmd == g_stBleCmdFucTab[u8Count].enCmdRx)
            {//判断指令码，获取opt码，然后/将buf中的净荷数据提取出来传入队列中
                app_ble_opt_put(APP_BLE_RX_EVENTS,g_stBleCmdFucTab[u8Count].enOpt,(uint8_t *)g_stBleProCtl.rx.buf
                            ,(g_stBleProCtl.rx.head->len - sizeof(protocol_head_t) - sizeof(protocol_tail_t)));
            }
        }
    }
}

/*************************下列是具体cmd对应的函数实现接口****************************************************************************/

/**************************************************************
*函数功能描述 ：升级起始包，将程序信息传递给app_upgrade_task处理
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleUpgradeStartReq(void *pData)
{
    if(NULL != pData)
    {
        typedef struct
        {
            uint8_t u8Type;//代码类型，用于区分app、boot、外部组件模块等升级信息，也可以用高位字节存储产品类型
            uint8_t res1[3];
            uint32_t u32Version;//已存储程序的软件版本，例如220701或V1.0.0
            uint8_t u8Name[32];//代码文件名称
            uint32_t u32TotalLen;//总长度，bytes，最大接收4095Mb的数据
        }Param_t;
        //app_task_create(APP_TASK_MODULE_UPGRADE);//先创建任务
        APP_TASK_MODULE_E eModule = APP_TASK_MODULE_UPGRADE;
        app_task_opt_put(APP_TASK_OPT_CREATE,&eModule,sizeof(eModule));
        app_upgrade_opt_put(APP_UPGRADE_OPT_START,pData,sizeof(Param_t));//固定44bytes
    }
}

/**************************************************************
*函数功能描述 ：升级起始包响应，app_upgrade_task处理完成后返回响应给对端设备
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleUpgradeStartRsp(void *pData)
{
    typedef struct
    {//指令cmdBleUpgradeStartRsp对应的数据格式
        uint32_t u32Index;
        uint8_t u8Flag;
        uint8_t res[3];
    }Param_t;
    Param_t *pStData;

    //配置包头，并在g_stBleProCtl.txbuf中为Pdata分配位置
    g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
      ,cmdBleUpgradeStartRsp,sizeof(Param_t),(void *)&pStData);
    if((NULL != pStData) && (NULL != pData))
    {//配置实际的参数
		memcpy((void *)pStData,(void *)pData,sizeof(Param_t));
    }
    //配置包尾
    g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);

    //将数据传入comm进程
    app_comm_txEnQueue(g_stBleProCtl.txbuf,g_stBleProCtl.txlen,APP_COMM_CHANNEL_BLE,0);
}

/**************************************************************
*函数功能描述 ：升级数据包，将程序代码传递给app_upgrade_task处理
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleUpgradDataReq(void *pData)
{
    if(NULL != pData)
    {
        app_upgrade_opt_put(APP_UPGRADE_OPT_DATA,pData,(4 + APP_BLE_UPGRADE_LEN));
    }
}

/**************************************************************
*函数功能描述 ：升级数据包响应，app_upgrade_task处理完成后返回响应给对端设备
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleUpgradDataRsp(void *pData)
{
    typedef struct
    {//指令cmdBleUpgradDataRsp对应的数据格式
        uint32_t u32Index;
        uint8_t u8Flag;
        uint8_t res[3];
    }Param_t;
    Param_t *pStData;

    //配置包头，并在g_stBleProCtl.txbuf中为Pdata分配位置
    g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
      ,cmdBleUpgradDataRsp,sizeof(Param_t),(void *)&pStData);
    if((NULL != pStData) && (NULL != pData))
    {//配置实际的参数
		memcpy((void *)pStData,(void *)pData,sizeof(Param_t));
    }
    //配置包尾
    g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);

    //将数据传入comm进程
    app_comm_txEnQueue(g_stBleProCtl.txbuf,g_stBleProCtl.txlen,APP_COMM_CHANNEL_BLE,0);
}

/**************************************************************
*函数功能描述 ：升级终止包，将程序代码传递给app_upgrade_task处理
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleUpgradStopReq(void *pData)
{
    if(NULL != pData)
    {
        //指令cmdBleUpgradStopReq对应的数据格式
        #if (0 == BSP_UPGRADE_AUTH_EN)
        typedef struct
        {
            uint8_t u8Md5[16];//md5明文
        }Param_t;
        #else
        typedef struct
        {
            uint8_t u8SignVerify[64];//md5明文加密后的签名密文值
        }Param_t;
        #endif
        app_upgrade_opt_put(APP_UPGRADE_OPT_STOP,pData,sizeof(Param_t));
    }
}

/**************************************************************
*函数功能描述 ：升级终止包响应，app_upgrade_task处理完成后返回响应给对端设备
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleUpgradStopRsp(void *pData)
{
    //配置包头，并在g_stBleProCtl.txbuf中为Pdata分配位置
    g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
        ,cmdBleUpgradStopRsp,0,NULL);
    if(NULL != pData)
    {//配置实际的参数
        memcpy((void *)&g_stBleProCtl.tx.head->err,(void *)pData,1);
    }
    //配置包尾
    g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);

    //将数据传入comm进程
    app_comm_txEnQueue(g_stBleProCtl.txbuf,g_stBleProCtl.txlen,APP_COMM_CHANNEL_BLE,0);
}

/**************************************************************
*函数功能描述 ：发送按键数值给对端设备
*函数参数 ：
*函数返回值 ：
*备注：
**************************************************************/
static void app_ble_cmdBleButtonReq(void *pData)
{
    typedef struct
    {//指令cmdBleUpgradeReq对应的数据格式
        uint8_t u8Button;
        uint8_t res[3];
    }Param_t;
    Param_t *pStData;

    //配置包头，并在g_stBleProCtl.txbuf中为Pdata分配位置
    g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
      ,cmdBleButtonReq,sizeof(Param_t),(void *)&pStData);
    if((NULL != pStData) && (NULL != pData))
    {//配置实际的参数
		memcpy((void *)pStData,(void *)pData,sizeof(Param_t));
    }
    //配置包尾
    g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);

    //将数据传入comm进程
    app_comm_txEnQueue(g_stBleProCtl.txbuf,g_stBleProCtl.txlen,APP_COMM_CHANNEL_BLE,cmdBleButtonRsp);
}

#if 0
//ble 对端调用接口函数（主动发送升级数据），先写在这里，后期再移植过去

#define APP_UPGRADE_CODE_TYPE       (0x43)//程序包类型
#define APP_UPGRADE_CODE_VERSION    (0x241126)//程序包版本
#define APP_UPGRADE_CODE_NAME       "STM32F103_PROGRAM_V1.0.1"//程序包名称
#define APP_UPGRADE_CODE_LEN        (65269)//程序包长度 bytes

/**************************************************************
*函数功能描述 ：自定协议，发送端发送起始包
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_ble_cmdBleUpgradeStartReq(void *pData)
{
    //获取这些信息的途径就是：提前写死，不论是通过脚本还是代码直接实现
    //，最终存储在某一个特定区域，或者程序包的包头，这里用宏定义配置实现
    typedef struct
    {
        uint8_t u8Type;//代码类型，用于区分app、boot、外部组件模块等升级信息，也可以用高位字节存储产品类型
        uint8_t res1[3];
        uint32_t u32Version;//已存储程序的软件版本，例如220701或V1.0.0
        uint8_t u8Name[32];//代码文件名称
        uint32_t u32TotalLen;//总长度，bytes，最大接收4095Mb的数据
    }Param_t;
    Param_t *pStData;
    
    g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
      ,cmdBleUpgradeStartReq,sizeof(Param_t),(void *)&pStData);
    if(NULL != pStData)
    {//配置实际的参数
		pStData->u8Type = APP_UPGRADE_CODE_TYPE;//测试产品，app程序
        pStData->u32Version = APP_UPGRADE_CODE_VERSION;
        memcpy(pStData->u8Name,APP_UPGRADE_CODE_NAME,(sizeof(APP_UPGRADE_CODE_NAME) - 1));
        pStData->u32TotalLen = APP_UPGRADE_CODE_LEN;
    }
    //配置包尾
    g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);

    //将数据传入comm进程
    app_comm_txEnQueue(g_stBleProCtl.txbuf,g_stBleProCtl.txlen,APP_COMM_CHANNEL_BLE,cmdBleUpgradeStartReq);
}

/**************************************************************
*函数功能描述 ：自定协议，发送端处理起始包响应
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_ble_cmdBleUpgradeStartRsp(void *pData)
{
    typedef struct
    {//指令cmdBleUpgradeStartRsp对应的数据格式
        uint32_t u32Index;
        uint8_t u8Flag;
        uint8_t res[3];
    }Param_t;
    Param_t *pStData = (Param_t *)pData;

    if(0x00 == pStData->u8Flag)
    {//从头开始升级

    }
    else if(0x01 == pStData->u8Flag)
    {//续传升级
        // pStData->u32Index;//传递该参数到对应任务
    }
    else
    {   // 0x03：不允许升级，版本异常
        // 0x04：不允许升级，文件大小异常
        // 0xFF：其他错误
        bsp_log_print(BSP_LOG_MODULE_BLE,BSP_LOG_TYPE_WARNING,"upgrade reject:%d",pStData->u8Flag);
    }
}

/**************************************************************
*函数功能描述 ：自定协议，发送端发送数据包
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_ble_cmdBleUpgradDataReq(void *pData)
{
    typedef struct
    {
        uint32_t u32Index;//包序号，表示是第几包数据
        uint8_t u8Data[APP_UPGRADE_SINGLE_LEN];//单包传送的实际数据
    }Param_t;
    Param_t *pStData;
    
    g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
      ,cmdBleUpgradDataReq,sizeof(Param_t),(void *)&pStData);
    if(NULL != pStData)
    {//配置实际的参数
		// g_txDataIndex++;
        // pStData->u32Index = g_txDataIndex;
        // bsp_upgrade_store_read(BSP_UPGRADE_APP_STORE_ADDR_S + pStData->u32Index * APP_UPGRADE_SINGLE_LEN,pStData->u8Data,APP_UPGRADE_SINGLE_LEN);
    }
    //配置包尾
    g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);

    //将数据传入comm进程
    app_comm_txEnQueue(g_stBleProCtl.txbuf,g_stBleProCtl.txlen,APP_COMM_CHANNEL_BLE,cmdBleUpgradeStartReq);
}

/**************************************************************
*函数功能描述 ：自定协议，发送端处理数据包响应
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_ble_cmdBleUpgradDataRsp(void *pData)
{
    typedef struct
    {//返回响应
        uint32_t u32Index;//包序号，表示是第几包数据
        uint8_t u8Flag;//1失败，0成功
        uint8_t res[3];
    }Param_t;
    Param_t *pStData = (Param_t *)pData;

    if(0x00 == pStData->u8Flag)
    {//升级成功，继续发包
        // pStData->u32Index;//该参数作为下一包发送依据的判定
    }
    else
    {//升级失败，停止发包
    }
}

/**************************************************************
*函数功能描述 ：自定协议，发送端发送结束包
*函数参数 ：
*函数返回值 ：
*备注 ：
**************************************************************/
static void app_ble_cmdBleUpgradStopReq(void *pData)
{
    bsp_md5_ctx_t stMd5Ctx = {0};
    uint8_t u8CodeData[64];
    uint8_t u8Md5[BSP_MD5_OUT_LEN];
    uint32_t u32ReadAddr = BSP_UPGRADE_APP_STORE_ADDR_S;
    uint32_t read_len = 0;

    //计算程序数据的md5值
    bsp_md5_init(&stMd5Ctx);
    for(uint32_t i = 0;i < (APP_UPGRADE_CODE_LEN - 1);i += 64)
    {
        read_len = ((i + 64) < (g_pStBspUpgradeCtl->stBuf.u16BufLen - 1)) ? 64 : (g_pStBspUpgradeCtl->stBuf.u16BufLen - 1) - i;
        bsp_upgrade_store_read(u32ReadAddr,u8CodeData,64);
        bsp_md5_update(&stMd5Ctx, u8CodeData, read_len);
        u32ReadAddr += 64;
    }
    bsp_md5_final(u8Md5,&stMd5Ctx);

    #if (0 == BSP_UPGRADE_AUTH_EN)//md5校验
    typedef struct
    {
        uint8_t u8Md5[BSP_MD5_OUT_LEN];
    }Param_t;
    Param_t *pStData = (Param_t *)pData;

    g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
      ,cmdBleUpgradStopReq,sizeof(Param_t),(void *)&pStData);
    if(NULL != pStData)
    {//配置实际的参数
        memcpy(pStData->u8Md5,u8Md5,sizeof(Param_t));
    }
    #else//数字签名校验
    typedef struct
    {
        uint8_t u8Sign[BSP_RSA_ENCODE_LEN];//64bytes的md5值rsa加密后的值
    }Param_t;
    Param_t *pStData = (Param_t *)pData;

    g_pBspProtocolApi->protocol_alloc_head(&g_stBleProCtl
      ,cmdBleUpgradStopReq,sizeof(Param_t),(void *)&pStData);
    if(NULL != pStData)
    {//配置实际的参数
        bsp_rsa_private_encrypt(u8Md5,sizeof(u8Md5),pStData->u8Sign,g_RsaPrivateKey_exponent, g_RsaModulus);
    }
    #endif

    //配置包尾
    g_pBspProtocolApi->protocol_alloc_tail(&g_stBleProCtl);

    //将数据传入comm进程
    app_comm_txEnQueue(g_stBleProCtl.txbuf,g_stBleProCtl.txlen,APP_COMM_CHANNEL_BLE,cmdBleUpgradStopRsp);
}
#endif

