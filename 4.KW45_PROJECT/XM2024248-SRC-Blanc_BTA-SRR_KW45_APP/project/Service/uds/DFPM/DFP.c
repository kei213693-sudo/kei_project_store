
/*
 * @Descripttion:   dtc信息定义
 * @version:        V1.0
 * @Author:         chenjianbo
 * @Date:           2022-06-10 10:43:32
 * @LastEditors: chenjianbo
 * @LastEditTime: 2022-07-29 16:03:46
 */
/* This file was automatically generated */
#include "DFP.h"

uint16_t dfcArray[256]; /*故障路径数组*/
uint8_t sfgArray[256];

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_CANBusoff_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_CANBusoff = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_CANBusoff_IRAM};

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_BattMin_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_BattMin = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_BattMin_IRAM};

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_BattMax_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_BattMax = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_BattMax_IRAM};

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_KW45_WDOG_RESET_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_KW45_WDOG_RESET = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_KW45_WDOG_RESET_IRAM};

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_KW45_LVD_RESET_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_KW45_LVD_RESET = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_KW45_LVD_RESET_IRAM};

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_KW45_HVD_RESET_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_KW45_HVD_RESET = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_KW45_HVD_RESET_IRAM};

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_RF_CHIP_COM_ERROR_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_RF_CHIP_COM_ERROR = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_RF_CHIP_COM_ERROR_IRAM};

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_29D6_COM_LOST_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_29D6_COM_LOST = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_29D6_COM_LOST_IRAM};

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_UWB_BASE_FAIL_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_UWB_BASE_FAIL = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_UWB_BASE_FAIL_IRAM};
volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_UWB_BASE_FAIL_IRAM;

volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_11E_TimOut_IRAM;
const struct DSM_DFCTYPE_IMPL DFC_11E_TimOut = {
    /* type descriptor pointer 'DSM_DFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DFC_11E_TimOut_IRAM};

volatile struct DSM_GETCHECKSTATUS_IMPL_IRAM_SUBSTRUCT DSM_GetCheckStatus_IRAM;
const struct DSM_GETCHECKSTATUS_IMPL DSM_GetCheckStatus = {
    /* type descriptor pointer 'DSM_GETCHECKSTATUS_IMPL_IRAM' for memory class substruct for 'IRAM' */
    &DSM_GetCheckStatus_IRAM,
    /* substruct: DSM_GetCheckStatus.locSFCType (modeled as:'locSFCType.DSM_GetCheckStatus') */
    {
        /* type descriptor pointer 'DSM_SFCTYPE_IMPL_IRAM' for memory class substruct for 'IRAM' */
        &DSM_GetCheckStatus_IRAM.locSFCType}};

void DFP_IMPL__ini(void)
{
    /* _ini: sequence call #96  */
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_CANBusoff, 0U);
    /* _ini: sequence call #97  */
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_BattMin, 1U);
    /* _ini: sequence call #98  */
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_BattMax, 2U);
    /* _ini: sequence call #100  */
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_KW45_WDOG_RESET, 3U);
    /* _ini: sequence call #101  */
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_KW45_LVD_RESET, 4U);
    /* _ini: sequence call #102  */
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_KW45_HVD_RESET, 5U);
    /* _ini: sequence call #97  */
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_RF_CHIP_COM_ERROR, 6U);
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_29D6_COM_LOST, 7U);
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_UWB_BASE_FAIL, 8U);
    DSM_DFCTYPE_IMPL_setDFCType(&DFC_11E_TimOut, 9U);
}
