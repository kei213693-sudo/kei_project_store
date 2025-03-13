/*
 * Copyright (C) 2024,宁波福尔达智能科技股份有限公司（重庆）
 * All rights reserved.
 * 文件名称：dbg_printf.h
 * 摘要：函数声明
 * 当前版本：V1.0.0,TaoWenzhuo,2024/5/11,初版创建
 * 历史版本：Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */

#ifndef DBG_PRINTF_H_
#define DBG_PRINTF_H_ 1

#define shell_write		dbg_Printf
void dbg_Printf(char *format, ...);
void dbg_Printf_hex(char *pData, int len);

#endif /* DBG_PRINTF_H_ */
