#ifndef SWUP_H
#define SWUP_H


#include <stdint.h>

#define SWUP_COMPONENT_COUNT_MAX  (16U) /** 组件最大数量 */
#define SWUP_PAGE_SIZE  (0x1000U)       /** 分页大小(4K) */
#define SWUP_PACK_OFFSET (0x220)        /** swup header偏移 */
#define SWUP_BIN_SIZE (0X30U)           /** Bin header 长度 */
#define SWUP_COMPREV (84U)              /** 预留 */

typedef struct
{
    struct
    {
        uint16_t offset;         /** 页偏移 */
        uint16_t size;            /** 页数量 */
        uint8_t rev[SWUP_COMPREV];
    }__attribute__((packed)) components[SWUP_COMPONENT_COUNT_MAX]; /** 原始加载信息 */

    uint8_t compLen;
    struct
    {
        uint32_t startAddr; /** 绝对起始地址(flash地址) */
        uint32_t size;      /** 组件大小(字节) */
        uint16_t sigNum;     /** 分页大小（4K） */
    }__attribute__((packed)) trInfo[SWUP_COMPONENT_COUNT_MAX]; /** 解析后的传输信息(内部使用) */

} __attribute__((packed)) swup_package ;


/** 48 字节，位于bin文件头 */
typedef struct
{
    struct
    {
        uint32_t addr;  /** 起始地址 相对于header */
        uint32_t size;  /** 文件大小 */
    } __attribute((packed)) attr[5];           /** SR10最多5个pack，后续SR11为3个 */
    uint32_t binSize;   /** pack总大小 */
    uint8_t fileIndex;  /** 当前刷写的文件索引，(内部使用)，非生成器 */
    uint8_t fileLen;    /** pack文件个数 */
    uint16_t crc16;     /** pack文件CRC */
} __attribute__((packed)) BinInfo;

/**
 * 函 数 介 绍: Construct a new init Update object
 *
 * 参       数: info      解析器
 * 参       数: filePath 文件路径
 */
uint8_t initUpdate(BinInfo *info, uint8_t *filePath);

/**
 * 函 数 介 绍: 初始化SWUP
 *
 * 参       数: pack      swup
 * 参       数: filePath  文件地址
 */
void initSwup(swup_package *pack, uint8_t *filePath);


#endif
