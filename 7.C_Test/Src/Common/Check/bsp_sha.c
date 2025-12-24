/*
 * bsp_sha.c
 *
 *  Created on: 2024年7月29日
 *      Author: lj
 *	  Abstract: 实现SHA算法
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "bsp_sha.h"

/*******************************全局变量定义*******************************/

/*******************************内部变量定义*******************************/

/*******************************内部函数的声明*******************************/
static void bsp_sha1_process_message_block(bsp_sha1_ctx_t* context);
static void bsp_sha1_pad_message(bsp_sha1_ctx_t* context);

/*******************************实际函数的实现*******************************/
/**************************************************************
*函数功能描述 ：赋初值函数
*函数参数 ：context，sha1算法控制结构体
*函数返回值 ：
*备注：
**************************************************************/
void bsp_sha1_init(bsp_sha1_ctx_t* context)
{
	context->Length_Low = 0;
	context->Length_High = 0;
	context->Message_Block_Index = 0;
	context->Computed = 0;
	context->Corrupted = 0;

	context->Message_Digest[0] = 0x67452301;//初始变量H为160bit数据块。
	context->Message_Digest[1] = 0xEFCDAB89;
	context->Message_Digest[2] = 0x98BADCFE;
	context->Message_Digest[3] = 0x10325476;
	context->Message_Digest[4] = 0xC3D2E1F0;
}

/**************************************************************
*函数功能描述 ：源数据读取和处理，对除了最后一组512bit的数据进行计算
*函数参数 ：context，sha1算法控制结构体
			buffer，待处理的信息分组的字节数组
			length，数组中信息的长度
*函数返回值 ：
*备注：
**************************************************************/
void bsp_sha1_update(bsp_sha1_ctx_t* context, const uint8_t *buffer,uint32_t length)
{
	if (!length)//如果输入为空就返回
	{
		return;
	}
	if (context->Computed || context->Corrupted)//防止对已经完成的哈希再次进行输入，或者避免在损坏的上下文中继续处理
	{
		context->Corrupted = 1;
		return;
	}
	while (length-- && !context->Corrupted)
	{
		// 每 8bits 的存放
		context->Message_Block[context->Message_Block_Index++] =(*buffer & 0xFF);//只保留最低8位

		context->Length_Low += 8;//位长度
		context->Length_Low &= 0xFFFFFFFF;//低四字节
		if (context->Length_Low == 0)
		{
			context->Length_High++;
			context->Length_High &= 0xFFFFFFFF;
			if (context->Length_High == 0)//高4字节位长度溢出
			{
				context->Corrupted = 1;//已损坏
			}
		}
		if (context->Message_Block_Index == 64)//数组被填满时调用
		{
			bsp_sha1_process_message_block(context);
		}
		buffer++;//指向下一个字符
	}
}

/**************************************************************
*函数功能描述 ：对信息的最后部分进行处理并输出最终计算结果
*函数参数 ：context，sha1算法控制结构体
			result，存储最终要输出的结果
*函数返回值 ：0，失败；1，成功
*备注：
**************************************************************/
int bsp_sha1_final(uint8_t result[BSP_SHA1_OUT_LEN],bsp_sha1_ctx_t* context)
{
	if (context->Corrupted)
	{
		return 0;
	}

	if (!context->Computed)
	{
		bsp_sha1_pad_message(context);
		context->Computed = 1;

		for(uint8_t i = 0; i < BSP_SHA1_OUT_LEN; ++i)
		{
			result[i] = context->Message_Digest[i>>2]>>8*(3-(i&0x03));
		}

		//第2中给result赋值的方式
		// uint32_t i, j;
		// for (i = 0, j = 0; j < BSP_SHA1_OUT_LEN; i++, j += 4)
		// {
		// 	result[j]     = (uint8_t)(context->Message_Digest[i] & 0xff);
		// 	result[j + 1] = (uint8_t)((context->Message_Digest[i] >> 8) & 0xff);
		// 	result[j + 2] = (uint8_t)((context->Message_Digest[i] >> 16) & 0xff);
		// 	result[j + 3] = (uint8_t)((context->Message_Digest[i] >> 24) & 0xff);
		// }

		memset((void*)context,0,sizeof(*context));//清空数据
	}
	return 1;
}

/**************************************************************
*函数功能描述 ：每512bits数据块处理，将其处理为80个子明文分组，并进行4轮80步运算生成160bits数据
*函数参数 ：context，sha1算法控制结构体
*函数返回值 ：
*备注：
**************************************************************/
static void bsp_sha1_process_message_block(bsp_sha1_ctx_t* context)
{
	int32_t t;
	int32_t temp;
	uint32_t W[80];//子明文分组数组
	uint32_t A, B, C, D, E;//初始值缓存变量
	const uint32_t K[] =
	{
		0x5A827999,
		0x6ED9EBA1,
		0x8F1BBCDC,
		0xCA62C1D6
	};//常量值kt，SHA-1计算中用到的常数定义

	for (t = 0; t < 16; t++)//W[t]有四个字节
	{//初始化子明文分组W
		W[t] = ((unsigned)context->Message_Block[t * 4]) << 24;//空出后三字节
		W[t] |= ((unsigned)context->Message_Block[t * 4 + 1]) << 16;
		W[t] |= ((unsigned)context->Message_Block[t * 4 + 2]) << 8;
		W[t] |= ((unsigned)context->Message_Block[t * 4 + 3]);
	}
	for (t = 16; t < 80; t++)
	{//将16个32bit字扩充为80个32bit字
		W[t] = BSP_SHA1CircularShift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);//循环左移一位
	}

	A = context->Message_Digest[0];
	B = context->Message_Digest[1];
	C = context->Message_Digest[2];
	D = context->Message_Digest[3];
	E = context->Message_Digest[4];

	for (t = 0; t < 20; t++)//逻辑函数f1=(b&c)|((~b)&d)
	{//第1轮20步计算
		temp = BSP_SHA1CircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = BSP_SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 20; t < 40; t++)//逻辑函数f2=b^c^d
	{//第2轮20步计算
		temp = BSP_SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];//压缩函数，T=ROTL5(A)+ft(B,C,D)+E+W[t]+Kt(mod 2^32)
		temp &= 0xFFFFFFFF;//相当于mod 2^32
		E = D;
		D = C;
		C = BSP_SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 40; t < 60; t++)//逻辑函数f3=(b&c)|(b&d)|(c&d)
	{//第3轮20步计算
		temp = BSP_SHA1CircularShift(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];//压缩函数
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = BSP_SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	for (t = 60; t < 80; t++)//逻辑函数f4=b^c^d
	{//第4轮20步计算
		temp = BSP_SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = BSP_SHA1CircularShift(30, B);
		B = A;
		A = temp;
	}

	//最后一步H_0(i)=A+H_0(i-1)，H_1(i)=B+H_1(i-1)...
	context->Message_Digest[0] = (context->Message_Digest[0] + A) & 0xFFFFFFFF;
	context->Message_Digest[1] = (context->Message_Digest[1] + B) & 0xFFFFFFFF;
	context->Message_Digest[2] = (context->Message_Digest[2] + C) & 0xFFFFFFFF;
	context->Message_Digest[3] = (context->Message_Digest[3] + D) & 0xFFFFFFFF;
	context->Message_Digest[4] = (context->Message_Digest[4] + E) & 0xFFFFFFFF;

	//每处理一块之后，SHAContext 块索引 0
	context->Message_Block_Index = 0;
}

/**************************************************************
*函数功能描述 ：填充函数，分<56字节和>=56字节两种填充方法
*函数参数 ：context，sha1算法控制结构体
*函数返回值 ：
*备注：
**************************************************************/
static void bsp_sha1_pad_message(bsp_sha1_ctx_t* context) 
{
	if (context->Message_Block_Index > 55)//至少448位
	{//大于等于56bytes时，填充块到512位，并处理它，然后继续填充到第二个块中到448位，最后填写长度信息。
		context->Message_Block[context->Message_Block_Index++] = 0x80;//可能会越界,但不影响扩展时这个块填满，下一个块第一个字节就是0x80
		while (context->Message_Block_Index < 64)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
		bsp_sha1_process_message_block(context);//并且索引值归0
		while (context->Message_Block_Index < 56)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
	}
	else
	{//小于56bytes时，直接添加补位和长度信息
		context->Message_Block[context->Message_Block_Index++] = 0x80;
		while (context->Message_Block_Index < 56)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
	}
	context->Message_Block[56] = (context->Length_High >> 24) & 0xFF;
	context->Message_Block[57] = (context->Length_High >> 16) & 0xFF;
	context->Message_Block[58] = (context->Length_High >> 8) & 0xFF;
	context->Message_Block[59] = (context->Length_High) & 0xFF;
	context->Message_Block[60] = (context->Length_Low >> 24) & 0xFF;
	context->Message_Block[61] = (context->Length_Low >> 16) & 0xFF;
	context->Message_Block[62] = (context->Length_Low >> 8) & 0xFF;
	context->Message_Block[63] = (context->Length_Low) & 0xFF;
	bsp_sha1_process_message_block(context);
}

#if 0//调用例子，跟md5一样
uint32_t read_len = 0;
uint8_t bsp_md5_calc[BSP_MD5_OUT_LEN] = {0};
uint8_t bsp_sha1_calc[BSP_SHA1_OUT_LEN] = {0};
bsp_md5_ctx_t fw_bsp_md5_ctx_t;
bsp_sha1_ctx_t fw_bsp_sha1_ctx_t;
bsp_md5_init(&fw_bsp_md5_ctx_t);
bsp_sha1_init(&fw_bsp_sha1_ctx_t);

uint8_t data[] = "cnkddnvnbia632xulcjdlkeovjaccnkddnvnbia632xulcjdlkeovjaccnkddnvnbia632xulcjdlkeovjaccnkddnvnbia632xulcjdlkeovjaccnkddnvnbia632xulcjdlkeovjaccnkddnvnbia632xulcjdlkeovjac";
app_log_printf("data len:%d\r\n",(sizeof(data) - 1));//待计算字符长度
for(uint32_t i = 0; i < (sizeof(data) - 1); i+= 64)
{
	read_len = ((i + 64) < (sizeof(data) - 1)) ? 64 : (sizeof(data) - 1) - i;
	bsp_md5_update(&fw_bsp_md5_ctx_t, (data+i), read_len);
}
bsp_md5_final(bsp_md5_calc, &fw_bsp_md5_ctx_t);
app_log_printf("bsp_md5_calc");
app_log_printf_hex(bsp_md5_calc,sizeof(bsp_md5_calc));

for(uint32_t i = 0; i < (sizeof(data) - 1); i+= 64)
{
	read_len = ((i + 64) < (sizeof(data) - 1)) ? 64 : (sizeof(data) - 1) - i;
	bsp_sha1_update(&fw_bsp_sha1_ctx_t, (data+i), read_len);
}
bsp_sha1_final(bsp_sha1_calc, &fw_bsp_sha1_ctx_t);
app_log_printf("bsp_sha1_calc");
app_log_printf_hex(bsp_sha1_calc,sizeof(bsp_sha1_calc));
#endif

