#include <stdint.h>
#include <string.h>
#include "swup.h"
#include "phscaUci.h"

/**
 * 函 数 介 绍: Construct a new init Update object
 *
 * 参       数: info      解析器
 * 参       数: filePath 文件路径
 * 返   回  值: uint8_t 1 失败
 * 返   回  值: uint8_t 0 成功
 */
uint8_t initUpdate(BinInfo *info, uint8_t *filePath)
{
    uint8_t ret = 0u;
    if (info && filePath)
    {
        memcpy(info, filePath, SWUP_BIN_SIZE);
        if ((info->fileLen>5u) || (!info->fileLen))
        {
            ret = 1u;
        }
        uint16_t crc16 = crc16_xmodem(filePath, 46);
        if (info->crc16 != crc16)
        {
            ret = 1u;
        }
    }
    else
    {
        ret = 1u;
    }
    return ret;
}

/**
 * 函 数 介 绍: 解析分段传输的包
 *
 * 参       数: pack      swup
 * 参       数: filePath 文件路径
 */
static void dealSwup(swup_package *pack, uint8_t *filePath)
{
    uint8_t i = 0;
    for (; i < SWUP_COMPONENT_COUNT_MAX; i++)
    {
        if (pack->components[i].size == 0x00) /** 如果分页长度为0，后续无component */
        {
            pack->compLen = i; /** 更新component */
            break;
        }
        else
        {
            pack->trInfo[i].sigNum = pack->components[i].size; /** 记录组件大小 （分页） */
            pack->trInfo[i].size = (uint32_t)pack->trInfo[i].sigNum*SWUP_PAGE_SIZE; /** 记录组件大小 （字节） */
            /** 组件起始地址 */
            pack->trInfo[i].startAddr = i?(pack->trInfo[i - 1].startAddr + pack->trInfo[i - 1].size):((uint32_t)filePath + SWUP_PAGE_SIZE);
        }
    }
}

/**
 * 函 数 介 绍: 初始化SWUP
 *
 * 参       数: pack      swup
 * 参       数: filePath  文件地址
 */
void initSwup(swup_package *pack, uint8_t *filePath)
{
    if (pack && filePath)
    {
        memcpy(pack, filePath+SWUP_PACK_OFFSET, sizeof(swup_package));
        dealSwup(pack, filePath);
    }
}
/** EOF */
