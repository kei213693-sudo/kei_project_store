/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_target.c
 * 摘要: Definitions specific to the xxx target
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#include "uds_target.h"

/*
 * 函数介绍: This function copies bytes from one location to another.
 * 参数:
 * \param [in] pDest        The destination location.
 * \param [in] pSrc         The source location.
 * \param [in] numBytes     The number of bytes to be copied.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_MemCopy(
    uint8_t*          pDest,
    const uint8_t*    pSrc,
    uint32_t          numBytes
)
{
    /* Check if the destination address or source address is NULL, and return immediately if so */
    if (pDest == NULL || pSrc == NULL)
    {
        return;
    }
    else
    {
        /* Code block for copying memory */
        /* Iterate through each byte, copying from the source address to the destination address */
        while (numBytes--)
        {
            *pDest++ = *pSrc++;
        }
    }
    return;
}

/*
 * 函数介绍: This function clear all of the bytes indicated by the pDest.
 * 参数:
 * \param [in] pDest        The destination buffer location.
 * \param [in] numBytes     The number of bytes to be copied.
 * 返回值: {*}
 * 备注:
 */
void UDS_FN_TYPE Uds_MemClear(
    uint8_t*          pDest,
    uint32_t          numBytes
)
{
    /* 如果指针不为空，则开始清零内存 */
    if (pDest != NULL)
    {
        /* 循环遍历每个字节，直到清零完所有字节 */
        while (numBytes > 0x00000000u)
        {
            /* 将当前字节设置为0 */
            *pDest = 0x00u;
            /* 移动到下一个字节 */
            pDest++;
            /* 减少剩余待清零字节的数量 */
            numBytes--;
        }
    }
    /* 如果指针为空，则不执行任何操作 */
    else
    {
        /* 代码区域，但此情况下不执行任何操作 */
        /* code */
    }

    /* 函数返回，无返回值 */
    return;
}

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
