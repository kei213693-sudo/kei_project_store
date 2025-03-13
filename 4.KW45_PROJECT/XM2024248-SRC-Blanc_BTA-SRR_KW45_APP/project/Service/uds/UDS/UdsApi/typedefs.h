/*
 * @Descripttion:   调用UDS外部头文件
 * @version:        V1.0
 * @Author:         chenjianbo
 * @Date:           2022-06-16 16:25:42
 * @LastEditors: chenjianbo
 * @LastEditTime: 2022-08-15 13:50:33
 */


#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

/*Import an extern header file */
//#include "../USER/bytpedef.h"



/*Internal header file reference area*/

#include "uds_pub.h"
/*Fault management related header files*/



#include "udscan_confdefs.h"

#include "dfpm_dfpInfo.h"




/************If the redefined data type was not defined before
the integration, the corresponding macro definition is opened*/
#define UINT8_T_EN      0
#define INT8_t_EN       0
#define UINT16_T_EN     0
#define INT16_T_EN      0
#define UINT32_T_EN     0
#define _64_T_EN        0
#define T_F_EN          0
#define UINT_EN         1
#define SINT_EN         0
#define VINT_EN         0
#define BOOL_EN         0




#ifndef FALSE
#define FALSE                          ((B_TRUE)0U)
#endif
#ifndef TRUE
#define TRUE                          ((B_TRUE)1U)
#endif
/*Primitive data types*/
#if UINT8_T_EN
typedef unsigned char                 uint8_t;
#endif

#if INT8_t_EN
typedef char                          int8_t;
#endif

#if UINT16_T_EN
typedef unsigned short                uint16_t;
#endif


#if INT16_T_EN
typedef short                         int16_t;
#endif

#if UINT32_T_EN
typedef unsigned long                 uint32_t;
#endif


#if _64_T_EN
typedef long                          int32_t;
typedef unsigned long long            uint64_t;
typedef long long                     int64_t;
#endif

#if T_F_EN
typedef float                         t_fl32;
typedef double                        t_fl64;
typedef int                           t_int;
typedef unsigned int                  t_uint;
#endif

#if UINT_EN
typedef unsigned char                 uint8;
typedef unsigned short                uint16;
typedef unsigned int                  uint32;
typedef unsigned long                 uint64;
#endif

#if SINT_EN
typedef signed char                   sint8;
typedef signed short                  sint16;
typedef signed int                    sint32;
#endif

#if VINT_EN
typedef volatile char                 vint8_t;
typedef volatile int                  vint32_t;
#endif

#if BOOL_EN
typedef unsigned char                 boolean;
typedef unsigned char                 bool;
#endif


typedef enum
{
    UDSAPI_RLT_OK =0,
    UDSAPI_RLT_ERR

}UdsApi_reslt;
#endif
