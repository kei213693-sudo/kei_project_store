/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_target.h
 * 摘要: Internal definitions and declarations
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */


#ifndef _UDS_TARGET_H
#define _UDS_TARGET_H



/* [MISRA 2004 Rule 19.15] Repeated include file */
/*lint -e537 */
#define EP40_DEF  //EP40项目定义
//#define UDS_ENV_RTAOSEK5
#include "stdint.h"
#include "hal_typedef.h"
#include "hal_sys.h"
#if defined( UDS_ENV_RTAOSEK5 )
    /* We are using RTA-OSEK. */
#ifndef OS_OSEKCOMN_H
    /* We have not yet included an RTA-OSEK header, so do so now. */
    #include "osek.h"
#endif
#else

#endif
/*lint -e750*//*lint -e751*//*lint -e752*/

/* [MISRA 2004 Advisory Rule 19.7] Function-like macro defined */
/*lint -esym(961,19.7) */

/******************************************************************************
*
* Preprocessor definitions
*
******************************************************************************/

/* This is the optimum number of bytes for writes to an address aligned on a natural boundary.
 * Often this is 2 for a 16-bit target and 4 for a 32-bit target. */
#define UDS_MEM_BLOCK_SIZE   0x04u

/* This macro prefixes every definition of a type which contains configuration (i.e. ROM data) for the UDS software. */
#define UDS_CONFIG_TYPE const

/* This macro prefixes every definition of a type which contains state (i.e. RAM data) for the UDS software. */
#define UDS_STATE_TYPE

/* This macro prefixes every definition of a function for the UDS software. */
#define UDS_FN_TYPE

#define MCU_GENERIC_TRIG_MCU_SW_RESET 	UdsApi_Hardware_Reset()	/* the interface to reset MCU */
//#define MCU_GENERIC_TRIG_MCU_SW_RESET
/* These macros enable or disable all interrupts and must be able to be nested.
 *
 * If we are using RTA-OSEK (either standalone or via ASCET) we can just delegate to the appropriate RTA-OSEK API.
 * Otherwise the user must supply his own implementation.
 */
#if defined(SFBL_BOOT_SW_BUILD) || defined( UDS_ENV_RTAOSEK5 ) || defined( UDS_ENV_RTAOS ) || defined( UDS_ENV_ASCET )
    #define UDS_DISABLE_ALL_INTERRUPTS()    SuspendAllInterrupts()
    #define UDS_ENABLE_ALL_INTERRUPTS()     ResumeAllInterrupts()
#else
    #define UDS_DISABLE_ALL_INTERRUPTS()     hal_sys_enabled_irq(HAL_ENABLED_DISABLE)
    #define UDS_ENABLE_ALL_INTERRUPTS()      hal_sys_enabled_irq(HAL_ENABLED_ENABLE)
#endif

/******************************************************************************
*
* Type definitions
*
******************************************************************************/

typedef uint16_t			Uds_PduId_t;		/* the type used for all layers during define/reference to a PDU instance. */
typedef uint16_t 		Uds_PduLength_t;
typedef uint8_t 			Uds_PduData_t;
typedef uint8_t			UdsTp_Addr_t;		/* the type used for network layer SA, TA, AE for NSDU and NPDU. */
typedef uint8_t			Uds_Addr_t;			/* the type used for UDS application layer SA, TA, RA for A_SDU and A_PDU. */

typedef uint8_t      		Uds_Seed_t;         /* The seed is an array of this type. "uint8_t"-seed is caculated at runtime,"const uint8_t"-seed is stored as predefined */
typedef uint8_t             Uds_Key_t;          /* The key buffer is an array of this type. */

typedef UDS_CONFIG_TYPE uint8_t*          Uds_CfgPtr8;        /* Instances of this type refer to a location of type UDS_CONFIG_TYPE. */
typedef UDS_CONFIG_TYPE uint16_t*         Uds_CfgPtr16;       /* Instances of this type refer to a location of type UDS_CONFIG_TYPE. */
typedef UDS_CONFIG_TYPE uint32_t*		  Uds_CfgPtr32;       /* Instances of this type refer to a location of type UDS_CONFIG_TYPE. */
typedef UDS_STATE_TYPE  uint8_t*          Uds_StatePtr8;      /* Instances of this type refer to a location of type UDS_STATE_TYPE. */
typedef UDS_STATE_TYPE  uint16_t*         Uds_StatePtr16;     /* Instances of this type refer to a location of type UDS_STATE_TYPE. */
typedef UDS_STATE_TYPE  uint32_t*         Uds_StatePtr32;     /* Instances of this type refer to a location of type UDS_STATE_TYPE. */



#ifndef TRUE
   #define TRUE			(0x01u)
#endif

#ifndef FALSE
   #define FALSE		(0x00u)
#endif

#if 0
#define STD_HIGH		0x01
#define STD_LOW			0x00

#define STD_ACTIVE		0x01
#define STD_IDLE		0x00

#define STD_ON			0x01
#define STD_OFF			0x00

#endif

#ifndef NULL
    #define NULL        ((void *)0x00000000ul)
#endif

/******************************************************************************
*
* Function definitions
*
******************************************************************************/

void UDS_FN_TYPE Uds_MemCopy( uint8_t* pDest, const uint8_t* pSrc, uint32_t numBytes );	/*lint !e963*/
void UDS_FN_TYPE Uds_MemClear( uint8_t* pDest, uint32_t numBytes);

#endif /* _UDS_TARGET_H */
/******************************************************************************
*                               end of file                                   *
******************************************************************************/
