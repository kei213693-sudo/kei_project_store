/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * @FilePath: D6Update.c
 * @abstract:
 * @Current_Version: V1.0.0
 * @Date: 2024-05-10 10:00:00
 * @History_Version: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#include <stdint.h>
#include <string.h>
#include "D6Update.h"
#include "swup.h"

#include "phscaTypes.h"
#include "phscaUci.h"
#include "phscaNcj29d6.h"
#include "logger.h"

/* =============================================================================
 * Internal Includes
 * ========================================================================== */
#define PHSCAUWB_EXTERN_GUARD
#include "phscaUwb.h"
#undef PHSCAUWB_EXTERN_GUARD

typedef struct
{
    uint32_t baseAddr; /** 净荷数据地址 */
    uint8_t crtIndex;  /** 当前文件索引 */
    uint8_t fileLen;   /** 文件数量 */
    uint8_t errorCnt;  /** 错误计数 */
    uint8_t req;       /** 传输请求 */
    uint8_t finish;    /** D6返回数据长度 */
} UpManage;            /** 更新控制器 */

UpManage manage; /** 控制管理器 */
UpManage *pManage = &manage;

/** CMD LIST */
#define CMD_LEN(VAR) (sizeof(VAR) - 4u)
static uint8_t cmdActivateSwup[] = {0x2E, 0x12, 0x00, 0x01, 0x00};                     /** Activate SWUP from the application */
static uint8_t cmdTrManifest[] = {0x2F, 0x07, 0x10, 0x00};                             /** TransferManifest: */
static uint8_t cmdStartUpdate[] = {0x2F, 0x0A, 0x00, 0x00};                            /** Transmitting CMD_SWUP_START_UPDATE: */
static uint8_t cmdSelectKeyStore[] = {0x2F, 0x1c, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00}; /** Transmitting CMD_SWUP_SELECT_KEY_STORE: */
static uint8_t cmdGetInfo[] = {0x27, 0x00, 0x00, 0x00};                            /** Get device private  info */
static uint8_t cmdGetVersion[] = {0x2F, 0x2, 0x00, 0x0};
/** END */

/** 文件头信息 */
uint8_t *filePath = NULL;  /** 当前文件地址 */
uint8_t *recPath = NULL;   /** 总文件地址 */
uint8_t *uci_frame = NULL; /** 帧信息 */
D6UpSta upSta = D6UP_IDLE; /** 更新状态 */

swup_package swupInfo; /** 头信息 */
swup_package *pack = &swupInfo;
BinInfo binInfo; /** 头信息 */
BinInfo *pbinInfo = &binInfo;
D6PriviteInfo info; /** 29d6私有信息 */

#define CMD_TX (0U)
#define CMD_RX (1U)
uint8_t cmdSta = CMD_TX; /** 指令状态 */
#define CMD_READY() ((cmdSta) == (CMD_TX))
#define CMD_CLEAR() (cmdSta = CMD_TX)
#define CMD_TOG() ((cmdSta) = (cmdSta) ? (CMD_TX) : (CMD_RX))

uint8_t verify_index = 0u; /** 校验索引 */



uint8_t sigTrBuffer[300];
uint8_t tpTrBuffer[300];

#define payload_length (4096 + 3) // UCI payload is 2 bytes for segment_id + 1 byte for component_id + 4096 bytes of binary data
#define payload_length_low ((uint8_t)(payload_length % 256))
#define payload_length_hi ((uint8_t)(payload_length / 256))

typedef enum
{
    TRANS_IDLE = 0,
    TRANS_ING = 1,
    TRANS_FINISH = 2
} TranceSta;

typedef struct
{
    TranceSta sta;  /** 传输状态 */
    uint8_t req;    /** 请求状态 */
    uint8_t finish; /** 完成标志 */

    uint8_t mode;       /** 传输类型 */
    uint8_t *uci_frame; /** 传输标志 */
    uint8_t *data;      /** 传输数据 */
    uint32_t len;       /** 数据长度 */
    uint32_t retLen;    /** D6返回数据长度 */

    uint32_t max_uci_payload_bytes;    /** 分段长度控制 */
    uint32_t uci_frame_payload_length; /** 本次传输长度 */

    uint32_t byte_index_start;    // byte index for accessing the payload bytes skips the 4 UCI header bytes
    uint32_t byte_index_stop;     // defines the range of payload bytes
    uint32_t bytes_in_last_frame; /** 最后一次传输长度 */
    uint8_t sigTrBuffer[300];     /** 传输缓冲区 */
} Trancefer;                      /** 分段控制器 */

Trancefer trfer;

typedef struct
{
    TranceSta sta;           /** 传输状态 */
    uint32_t retLen;         /** D6返回数据长度 */
    uint16_t segments;       /** 分段数量 */
    uint8_t compLen;         /** 组件数量 */
    uint8_t finish;          /** 完成标志 */
    uint8_t req;             /** 请求状态 */
    uint16_t segment_id;     /** 分段索引 */
    uint8_t component_index; /** 组件索引 */
    uint8_t cmd[8];          /** 组件指令 */
    uint8_t *pdata;          /** 分段数据地址 */
} Component_t;               /** 组件控制器 */

Component_t compInfo;

/**
 * 函 数 介 绍: 获取执行结果
 *
 * 参       数: uci_payload
 * 返   回  值: uint8_t 1 成功
 * 返   回  值: uint8_t x 失败
 */
static uint8_t get_swup_response(uint8_t *uci_payload)
{
    uint16_t ret = uci_payload[0] + 256 * uci_payload[1];
    return (ret == 0u);
}

/**
 * 函 数 介 绍: 更新状态
 *
 * 参       数: sta 下一个状态
 */
static void upDateSta(D6UpSta sta)
{
    D6UpSta temp = D6UP_ERROR;
    if (phscaUci_GetResponse(tpTrBuffer))
    {
        uint8_t ret = get_swup_response(&tpTrBuffer[4]);
        if (ret)
        {
            temp = sta;
        }
    }
    upSta = temp;
}

/**
 * 函 数 介 绍: 开始升级流程
 *
 * 参       数: 文件地址
 * 参       数: mode 1 正常启动
 * 参       数: mode 0 错误后自动恢复
 * 返   回  值: uint8_t 1 成功
 * 返   回  值: uint8_t 0 失败
 */
uint8_t D6SWUPStart(uint8_t *path, uint8_t mode)
{
    uint8_t ret = 0u;
    upSta = D6UP_IDLE;
    recPath = path;
    if (recPath && (!pManage->req))
    {
        memset(pManage, 0, sizeof(manage));
        memset(pbinInfo, 0, sizeof(binInfo));
        if (!initUpdate(pbinInfo, recPath))
        {
            CMD_CLEAR();
            pManage->req = 1U;
            pManage->crtIndex = 0U;
            pManage->finish = D6SWUP_BUSY;
            pManage->fileLen = pbinInfo->fileLen;
            if (mode)
            {
                pManage->errorCnt = 0u;
            }
            pManage->baseAddr = (uint32_t)path + SWUP_BIN_SIZE;
            ret = 1u;
        }
    }
    return ret;
}

/**
 * 函 数 介 绍: 激活SWUP
 *
 */
void cmd_swup_activate(void)
{
    if (CMD_READY())
    {
        (void)isPass(0); /** 重置定时器 */
        phscaUci_SendCommand(cmdActivateSwup, CMD_LEN(cmdActivateSwup));
        CMD_TOG();
    }
    else
    {
        if (bePass(1000)) /** 500ms 后选择key */
        {
            CMD_TOG();
            (void)phscaUci_GetResponse(tpTrBuffer);
            uint8_t uciSta = phscaUci_GetSta();
            if (SWUP_STATE_ACTIVE == uciSta)
            {
                upSta = D6UP_TRSELKEY;
                logPrintf("ACT D6UP_TRSELKEY !\r\n");
            }
            else
            {
                upSta = D6UP_TRFEST;
                logPrintf("ACT D6UP_TRFEST !\r\n");
            }
        }
    }
}

/**
 * 函 数 介 绍: 激活SWUP
 *
 */
void cmd_swup_SelectKeyStore(void)
{
    if (CMD_READY())
    {
        phscaUci_SendCommand(cmdSelectKeyStore, CMD_LEN(cmdSelectKeyStore));
        CMD_TOG();
    }
    else
    {
        if (bePass(500)) /** 500ms 后传输manifest */
        {
            CMD_TOG();
            upDateSta(D6UP_TRFEST);
            logPrintf("D6UP_TRFEST !\r\n");
        }
    }
}

/**
 * 函 数 介 绍: 分段传输请求
 *
 * 参       数: mode      0 组件为manifest
 * 参       数: mode      1 组件为component
 * 参       数: uci_frame 指令
 * 参       数: data      数据
 * 参       数: len       数据长度
 */
static void cmd_transmit_segmented(uint8_t mode, uint8_t *uci_frame, uint8_t *data, uint32_t len)
{
    trfer.mode = mode;
    trfer.uci_frame = uci_frame;
    trfer.data = data;
    trfer.len = len;
    trfer.req = 1U;
}

/**
 * 函 数 介 绍: 传输 manifest
 *
 */
static void cmd_swup_transfer_manifest(void)
{
    if (CMD_READY())
    {
        cmd_transmit_segmented(0, cmdTrManifest, filePath, SWUP_PAGE_SIZE);
        CMD_TOG();
    }
    else
    {

        if (trfer.finish)
        {
            if (bePass(500))
            {
                CMD_TOG();
                upSta = D6UP_STUPDATE;
                logPrintf("cmd_swup_transfer_manifest !\r\n");

            }
        }
    }
}

/**
 * 函 数 介 绍: 开始更新
 *
 */
static void cmd_swup_start_update(void)
{
    if (CMD_READY())
    {
        phscaUci_SendCommand(cmdStartUpdate, CMD_LEN(cmdStartUpdate));
        (void)isPass(0);
        CMD_TOG();
    }
    else
    {
        if (bePass(500))
        {
            CMD_TOG();
            upDateSta(D6UP_TRPONENT);
            logPrintf("cmd_swup_start_update !\r\n");
        }
    }
}

/**
 * 函 数 介 绍: 组件传输例程
 *
 */
static void cmd_transfer_component(void)
{
    if (CMD_READY())
    {
        compInfo.req = 1U;
        CMD_TOG();
    }
    else
    {
        if (compInfo.finish)
        {
            if (bePass(500))
            {
                CMD_TOG();
                upSta = D6UP_VRCOMPONENT;
                verify_index = 0u;
                logPrintf("cmd_transfer_component !\r\n");
            }
        }
    }
}

/**
 * 函 数 介 绍: 组件传输服务
 *
 */
static void componentEntry(void)
{
    switch (compInfo.sta)
    {
    case TRANS_IDLE:
        if (compInfo.req)
        {
            compInfo.finish = 0U;
            compInfo.component_index = 0u;
            compInfo.retLen = 0u;
            compInfo.compLen = pack->compLen;
            compInfo.segment_id = 0u;
            compInfo.sta = TRANS_ING;
        }
        break;
    case TRANS_ING:
        if (!bePass(40)) /** 1.5*16+16 */
        {
            break;
        }
        if (trfer.req)
        {
            break;
        }

        if (compInfo.component_index < compInfo.compLen)
        {
            compInfo.segments = pack->trInfo[compInfo.component_index].sigNum; /** every components consists of segments of size 4096 bytes each (one flash page) */
            if (compInfo.segment_id < compInfo.segments)
            {
                uint8_t segment_id_low = compInfo.segment_id % 0xFF;
                uint8_t segment_id_hi = (uint8_t)(compInfo.segment_id / 256);
                uint32_t byte_index_start = compInfo.segment_id * 4096;
                // construct long UCI frame with one segment
                compInfo.pdata = ((uint8_t *)pack->trInfo[compInfo.component_index].startAddr) + byte_index_start;
                compInfo.cmd[0] = 0x2F;
                compInfo.cmd[1] = 0x10;
                compInfo.cmd[2] = payload_length_hi;
                compInfo.cmd[3] = payload_length_low;
                compInfo.cmd[4] = compInfo.component_index;
                compInfo.cmd[5] = segment_id_low;
                compInfo.cmd[6] = segment_id_hi;
                cmd_transmit_segmented(1, compInfo.cmd, compInfo.pdata, payload_length);
                compInfo.segment_id++;
            }
            else
            {
                compInfo.segment_id = 0u;
                compInfo.component_index++;
            }
        }
        else
        {
            compInfo.sta = TRANS_FINISH;
        }

        break;
    case TRANS_FINISH:
        if (bePass(550)) /** verify component */
        {
            compInfo.req = 0U;
            compInfo.finish = 1U;
            compInfo.sta = TRANS_IDLE;
        }
        break;

    default:
        break;
    }
}

/**
 * 函 数 介 绍: 校验组件
 *
 */
static void cmd_verify_component(void)
{
    // iterate through all 8 component slots, independent from the amount present in this package
    if (verify_index < pack->compLen)
    {
        if (CMD_READY())
        {
            uint8_t cmd_verify_component[5] = {0x2F, 0x11, 0x00, 0x01, 0};
            cmd_verify_component[4] = verify_index;
            phscaUci_SendCommand(cmd_verify_component, CMD_LEN(cmd_verify_component));
            (void)bePass(0);
            CMD_TOG();
        }
        else
        {
            if (bePass(20))
            {
                uint32_t retLen = phscaUci_GetResponse(tpTrBuffer);
                if (retLen)
                {
                    uint8_t ret = get_swup_response(&tpTrBuffer[4]);
                    if (ret == 0u)
                    {
                    	logPrintf("D6UP_ERROR cmd_verify_component !\r\n");
                        upSta = D6UP_ERROR; /** "VerifyComponent(): error at verify_index */
                    }
                }
                verify_index++;
                CMD_TOG();
            }
        }
    }
    else
    {
        if (bePass(200))
        {
            upSta = D6UP_VRALL;
        }
    }
}

/**
 * 函 数 介 绍: 校验本次传输
 *
 */
static void cmd_verify_all(void)
{
    if (CMD_READY())
    {
        uint8_t cmd_verify_all[4] = {0x2F, 0x12, 0x00, 0x00};
        phscaUci_SendCommand(cmd_verify_all, CMD_LEN(cmd_verify_all));
        (void)isPass(0);
        CMD_TOG();
    }
    else
    {
        if (bePass(500))
        {
            CMD_TOG();
            upDateSta(D6UP_FINISH);
        }
    }
}

/**
 * 函 数 介 绍: 更新完成处理
 *
 */
static void cmd_finish_update(void)
{
    if (CMD_READY())
    {
        uint8_t cmd_finish[4] = {0x2F, 0x1a, 0x00, 0x00};
        phscaUci_SendCommand(cmd_finish, CMD_LEN(cmd_finish));
        (void)isPass(0);
        CMD_TOG();
    }
    else
    {
        if (bePass(500))
        {
            CMD_TOG();
            upDateSta(D6UP_DEACTIVE);
        }
    }
}

/**
 * 函 数 介 绍: 完成并重启
 *
 */
static void cmd_deactive_swup(void)
{
    if (CMD_READY())
    {
        if (bePass(500))
        {
            uint8_t cmd_deactive[4] = {0x2F, 0x1b, 0x00, 0x00};
            phscaUci_SendCommand(cmd_deactive, CMD_LEN(cmd_deactive));
            (void)isPass(0);
            CMD_TOG();
        }
    }
    else
    {
        if (bePass(200))
        {
            CMD_TOG();
            upSta = D6UP_GETINF;
        }
    }
}
/**
 * 函 数 介 绍: 给出29D6私有信息
 *
 * 参       数:
 * 返   回  值: 数据指针
 */
D6PriviteInfo* D6SWUPGetInfo(void)
{
    return &info;
}


/**
 * 函 数 介 绍: 获取29D6私有信息
 *
 */
static void cmd_get_info(void)
{
    if (CMD_READY())
    {
        memset(&info, 0 ,sizeof(info));
        phscaUci_SendCommand(cmdGetInfo, CMD_LEN(cmdGetInfo));
        (void)bePass(0);
        CMD_TOG();
    }
    else
    {
        if (bePass(250))
        {
            uint32_t retLen = phscaUci_GetResponse(tpTrBuffer);
            if (retLen)
            {
                memcpy(info.sig0, &tpTrBuffer[7], 0x0b);
                memcpy(info.sig1, &tpTrBuffer[20], 0x02);
                memcpy(info.sig2, &tpTrBuffer[24], 0x06);
                upSta = D6UP_IDLE;
                logPrintfImmedi("d6: %x !\r\n",info.sig0);
            }
            else
            {
            	logPrintfImmedi("cmd_get_info !\r\n");
               // upSta = D6UP_ERROR;
            }
            upSta = D6UP_IDLE;
            CMD_TOG();
        }
    }
}

/**
 * 函 数 介 绍: 分段传输服务
 *
 */
static void segmentEntry(void)
{
    switch (trfer.sta)
    {
    case TRANS_IDLE:
        if (trfer.req) /** 分段请求 */
        {
            trfer.finish = 0u;
            trfer.max_uci_payload_bytes = 255u;
            trfer.uci_frame_payload_length = trfer.uci_frame[3] + 256 * trfer.uci_frame[2];
            trfer.byte_index_start = 0; // byte index for accessing the payload bytes skips the 4 UCI header bytes
            trfer.byte_index_stop = 0;  // defines the range of payload bytes

            // UCI frame with PBF bit set, followed by payload bytes
            trfer.sigTrBuffer[0] = trfer.uci_frame[0] | 0x10;
            trfer.sigTrBuffer[1] = trfer.uci_frame[1];
            trfer.sigTrBuffer[2] = 0x00u;
            trfer.sigTrBuffer[3] = trfer.max_uci_payload_bytes;

            if (trfer.mode) /** 特殊处理component第一帧 */
            {
                trfer.byte_index_stop = trfer.byte_index_start + trfer.max_uci_payload_bytes - 3u;
                memcpy(trfer.sigTrBuffer + 4, trfer.uci_frame + 4, 3);
                memcpy(trfer.sigTrBuffer + 7, trfer.data + trfer.byte_index_start, trfer.byte_index_stop);
                trfer.byte_index_start = trfer.byte_index_stop; // next segmented frame continues after last used byte index
                phscaUci_SendCommand(trfer.sigTrBuffer, trfer.max_uci_payload_bytes);
            }
            (void)isPass(0); /** 发送完后再更新时间 */
            trfer.sta = TRANS_ING;
        }
        break;
    case TRANS_ING:
        if (!isPass(2))
        {
            break;
        }

        if ((trfer.uci_frame_payload_length - trfer.byte_index_start) > trfer.max_uci_payload_bytes)
        {
            trfer.byte_index_stop = trfer.byte_index_start + trfer.max_uci_payload_bytes;
            memcpy(trfer.sigTrBuffer + 4, trfer.data + trfer.byte_index_start, trfer.max_uci_payload_bytes);

            trfer.byte_index_start = trfer.byte_index_stop; // next segmented frame continues after last used byte index
            phscaUci_SendCommand(trfer.sigTrBuffer, trfer.max_uci_payload_bytes);
        }
        else
        {
            trfer.bytes_in_last_frame = trfer.len - trfer.byte_index_start;
            if (trfer.mode)
            {
                if (trfer.bytes_in_last_frame >= 3u)
                {
                    trfer.bytes_in_last_frame -= 3u;
                }
            }

            trfer.sigTrBuffer[0] = trfer.uci_frame[0];
            trfer.sigTrBuffer[1] = trfer.uci_frame[1];
            trfer.sigTrBuffer[2] = 0x00;
            trfer.sigTrBuffer[3] = trfer.bytes_in_last_frame;
            memcpy(trfer.sigTrBuffer + 4, trfer.data + trfer.byte_index_start, trfer.bytes_in_last_frame);
            phscaUci_SendCommand(trfer.sigTrBuffer, trfer.bytes_in_last_frame);
            trfer.sta = TRANS_FINISH;
        }
        (void)isPass(0);
        break;
    case TRANS_FINISH:
        if (isPass(5))
        {
            trfer.retLen = phscaUci_GetResponse(tpTrBuffer);
            if (trfer.retLen)
            {
                uint8_t ret = get_swup_response(&tpTrBuffer[4]);
                if (ret == 0u)
                {
                	logPrintf("D6UP_ERROR TRANS_FINISH !\r\n");
                    upSta = D6UP_ERROR;
                }
            }
            trfer.sta = TRANS_IDLE;
            trfer.req = 0u;
            trfer.finish = 1u; /** 无论响应如何，本次传输已经完成 */
            (void)isPass(0);
        }
        break;
    default:
        break;
    }
}

/**
 * 函 数 介 绍: 空闲处理
 *
 */
static void cmd_swup_idle(void)
{
    if (pManage->req)
    {
        if (pManage->crtIndex < pManage->fileLen)
        {
            filePath = (uint8_t *)(pManage->baseAddr + pbinInfo->attr[pManage->crtIndex].addr);
            upSta = D6UP_ACTSWUP;
            memset(&swupInfo, 0, sizeof(swupInfo));
            initSwup(&swupInfo, filePath);
            pManage->crtIndex++;
            logPrintf("D6 INDEX:%d !\r\n",pManage->crtIndex);
        }
        else
        {
            pManage->finish = D6SWUP_FINISH;
            pManage->req = 0U;
            phscaNcj29d6_HardReset(10, 5);
            logPrintf("D6 INDEX finsh !\r\n");
        }
    }
}

/**
 * 函 数 介 绍: 错误处理
 *
 */
static void cmd_swup_error(void)
{
    if (pManage->errorCnt < 3u)
    {
    	pManage->errorCnt++;

    	if(pManage->crtIndex)
    	{
    		pManage->crtIndex--;
    	}
        (void)D6SWUPStart(recPath, 0);
        phscaNcj29d6_HardReset(10, 5);
    }
    else
    {
        pManage->req = 0U;
        pManage->finish = D6SWUP_FAIL;
    }
    logPrintf("errorCnt:%d !\r\n",pManage->errorCnt);
    upSta = D6UP_IDLE;

}

/**
 * 函 数 介 绍: 进程控制
 *
 */
static void cmdEntry(void)
{
    switch (upSta)
    {
    case D6UP_IDLE:
        cmd_swup_idle();
        break;
    case D6UP_ACTSWUP:
        cmd_swup_activate();
        break;
    case D6UP_TRSELKEY:
        cmd_swup_SelectKeyStore();
        break;
    case D6UP_TRFEST:
        cmd_swup_transfer_manifest();
        break;
    case D6UP_STUPDATE:
        cmd_swup_start_update();
        break;
    case D6UP_TRPONENT:
        cmd_transfer_component();
        break;
    case D6UP_VRCOMPONENT:
        cmd_verify_component();
        break;
    case D6UP_VRALL:
        cmd_verify_all();
        break;
    case D6UP_FINISH:
        cmd_finish_update();
        break;
    case D6UP_DEACTIVE:
        cmd_deactive_swup();
        break;
    case D6UP_GETINF:
        cmd_get_info();
        break;
    case D6UP_ERROR:
        cmd_swup_error();
        break;
    default:
        break;
    }
}

/**
 * 函 数 介 绍: 获取结果
 *
 * 返   回  值: uint8_t 0 失败
 * 返   回  值: uint8_t 1 处理中
 * 返   回  值: uint8_t 2 完成
 */
uint8_t D6SWUPGetSta(void)
{
    return pManage->finish;
}

/**
 * 函 数 介 绍: 更新主循环
 *
 */
void D6SWUPEntry(void)
{
    /** CMD */
    cmdEntry();
    componentEntry();
    segmentEntry();
}

/** EOF */
