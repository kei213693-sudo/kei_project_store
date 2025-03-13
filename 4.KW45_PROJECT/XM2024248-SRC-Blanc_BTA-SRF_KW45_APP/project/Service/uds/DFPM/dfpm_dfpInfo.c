/* This file was automatically generated */

#include "Dtc_Manage.h"

//#pragma section CONST ".dataset" R
const DFPM_FaultInfo    FaultList[] =
{
/* confirm   healing   reconfirm        class          dtc     snapshot spn    fmi   enable           */
    { 0u      , 0u      , 0u      , DFPM_Class_C   , 0X000000u   , 0u , 0u   , 0u , FALSE}, /* unuse item */
    { 1u      , 1u      , 1u      , DFPM_Class_C   , 0XC00288u   , 0u , 158u , 0u , TRUE   }, /* Dfp =   1, BUSOFF故障 */
    { 1u      , 1u      , 1u      , DFPM_Class_C   , 0X800116u   , 0u , 158u , 1u , TRUE   }, /* Dfp =   2, 欠压故障 */
    { 1u      , 1u      , 1u      , DFPM_Class_C   , 0X800117u   , 0u , 158u , 0u , TRUE   }, /* Dfp =   3, 过压故障 */
    { 1u      , 1u      , 1u      , DFPM_Class_C   , 0XF11E87u   , 0u , 158u , 1u , TRUE   }, /* Dfp =   4, 报文BCU_11E通讯丢失 */
    { 1u      , 1u      , 1u      , DFPM_Class_C   , 0X930396u   , 0u , 158u , 0u , TRUE   }, /* Dfp =   5, KW45与RF芯片SPI通讯故障 */
    { 1u      , 1u      , 1u      , DFPM_Class_C   , 0X930496u   , 0u , 158u , 0u , TRUE   }, /* Dfp =   6, RF初始化失败 */
    { 1u      , 1u      , 1u      , DFPM_Class_C   , 0X930596u   , 0u , 158u , 0u , TRUE   }, /* Dfp =   7, UWB基带故障 */
    { 1u      , 1u      , 1u      , DFPM_Class_C   , 0X930696u   , 0u , 158u , 0u , TRUE   }, /* Dfp =   8, BLE内部初始化错误 */
};
#pragma section CONST
