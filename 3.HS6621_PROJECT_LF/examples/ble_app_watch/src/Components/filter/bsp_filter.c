/*
 * bsp_filter.c
 *
 *  Created on: 2025年1月9日
 *      Author: lj
 *	  Abstract: 数字滤波器模块，用于实现数据处理中用到的各类不同滤波器
 */
#include <stdint.h>
#include <string.h>

#include "bsp_filter.h"

/*******************************全局变量定义*******************************/


/*******************************内部变量定义*******************************/


/*******************************内部函数的声明*******************************/

/*******************************实际函数的实现*******************************/

/**************************************************************
*函数功能描述 ：低通滤波器，移动平均法
*函数参数 ：pData，指向待处理的数据，单位是“int8_t——》char”
            u32Len，待处理数据总长度，1bytes
            u32WindowSize，移动窗口尺寸，必须小于u32Len
*函数返回值 ：
*备注：最简单的低功率波方法，消耗的资源最少；
        常用于去除高频噪声（变化过大）、平滑信号的场合
**************************************************************/
int8_t bsp_filter_low_pass_moving_average(int8_t *pData, uint32_t u32Len, uint32_t u32WindowSize)
{
    int8_t i8Average = 0;
    int32_t i32Sum = 0;

    if((NULL != pData) && (0 != u32Len) && (0 != u32WindowSize) && (u32Len > u32WindowSize))
    {
        //先计算初始窗口的数据总和与平均值
        for(uint32_t i = 0;i < u32WindowSize;i++)
        {
            i32Sum += (int32_t)(*(pData + i));
        }
        i8Average = (int8_t)(i32Sum/u32WindowSize);

        //从u32WindowSize开始，不断更新窗口以及平均值
        for(uint32_t i = u32WindowSize;i < u32Len;i++)
        {
            i32Sum = i32Sum + (int32_t)(*(pData + i))
                        - (int32_t)(*(pData + i - u32WindowSize));
            i8Average = (int8_t)(i32Sum/u32WindowSize);
        }
    }
    return i8Average;
}
