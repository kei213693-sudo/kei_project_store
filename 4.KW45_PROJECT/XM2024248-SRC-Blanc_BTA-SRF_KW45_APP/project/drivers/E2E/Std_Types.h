/*
 * Std_Types.h
 *
 *  Created on: 2024年7月1日
 *      Author: F704011556_
 */

#ifndef E2E_STD_TYPES_H
#define E2E_STD_TYPES_H
#include "Compiler.h"

typedef unsigned char                 uint8;
typedef unsigned short                uint16;
typedef unsigned long                 uint32;
typedef unsigned long long            uint64;

typedef unsigned char                 uint8_t;
typedef unsigned short                uint16_t;
typedef unsigned long                  uint32_t;
typedef unsigned long long            uint64_t;

typedef unsigned char Std_ReturnType;
typedef uint8_t boolean;

#define TRUE	1
#define FALSE	0

#define E_NOT_OK 1
#define E_OK	 0

typedef struct
{
	uint8_t	vendorID;
	uint8_t moduleID;
	uint8_t sw_major_version;
	uint8_t sw_minor_version;
	uint8_t sw_patch_version;
}Std_VersionInfoType;

#endif /* E2E_STD_TYPES_H_ */
