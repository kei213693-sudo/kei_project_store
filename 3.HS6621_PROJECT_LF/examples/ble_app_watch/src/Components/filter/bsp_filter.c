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
#include "co_debug.h"

/*******************************全局变量定义*******************************/


/*******************************内部变量定义*******************************/


/*******************************内部函数的声明*******************************/

/*******************************实际函数的实现*******************************/

/**************************************************************
*函数功能描述 ：低通滤波器，移动平均法
*函数参数 ：pDataIn，指向待处理的数据，单位是“int8_t——》char”
            pDataOut，用于存储已经处理完成的数据
            u32Len，待处理数据总长度，1bytes
            u32WindowSize，移动窗口尺寸，必须小于u32Len
*函数返回值 ：处理完成数据的平均值
*备注：最简单的低功率波方法，消耗的资源最少；
        常用于去除高频噪声（变化过大）、平滑信号的场合
**************************************************************/
int8_t bsp_filter_low_pass_moving_average(int8_t *pDataIn, int8_t *pDataOut, uint32_t u32Len, uint32_t u32WindowSize)
{
    int8_t i8Average = 0;
    int32_t i32Sum = 0;
    int32_t i32Count = 0;
    int32_t i32Len = (int32_t)u32Len;//类型转换

    if((NULL != pDataIn) && (NULL != pDataOut) && (0 != u32Len) && (0 != u32WindowSize) && (u32Len > u32WindowSize))
    {
        //计算过滤后的结果，是长度和in[]相同的数值out[]
        for(int32_t i = 0;i < i32Len;i++)
        {
            i32Count = 0;
            i32Sum = 0;
            for(int32_t j = (i - u32WindowSize + 1);j <= i;j++)
            {
                if((j >= 0) && (j < i32Len))
                {//假设u32WindowSize为3
                //out[0] = (in[0])/1
                //out[1] = (in[1] + in[0])/2
                //out[2] = (in[2] + in[1] + in[0])/3
                //out[3] = (in[3] + in[2] + in[1])/3
                //out[i] = (in[i] + in[i-1] + in[i-2])/3
                    i32Count++;//累加值计数
                    i32Sum += (int32_t)(*(pDataIn + j));
                }
            }
            (*(pDataOut + i)) = (int8_t)(i32Sum/i32Count);//out[i] = (in[i] + in[i+1] + …… + in[i])/count
            //log_debug("sumIn[%d]:%d   out[%d]:%d\n",i32Count,i32Sum,i,(*(pDataOut + i)));
        }

        //计算滤波后out[]的平均值
        i32Sum = 0;
        for(int32_t i = 0;i < i32Len;i++)
        {
            i32Sum += (int32_t)(*(pDataOut + i));
        }
        i8Average = (int8_t)(i32Sum/(int32_t)i32Len);
        //log_debug("sumOut:%d,%d\n",i32Sum,i8Average);
    }
    return i8Average;
}
