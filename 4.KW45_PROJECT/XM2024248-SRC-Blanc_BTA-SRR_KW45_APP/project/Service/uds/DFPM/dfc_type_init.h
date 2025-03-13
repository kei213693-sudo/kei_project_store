/* This file was automatically generated */

#ifndef DFC_TYPE_INIT_H
#define DFC_TYPE_INIT_H

typedef enum
{
    DFP_DFC_UNUSE= 0u,
    DFP_DFC_CANBUSOFF,
    DFP_DFC_BATTMIN,
    DFP_DFC_BATTMAX,
    DFP_DFC_KW45_WDOG_RESET,
    DFP_DFC_KW45_LVD_RESET,
    DFP_DFC_KW45_HVD_RESET,
    DFP_DFC_RF_CHIP_COM_ERROR,
    DFP_DFC_29D6_COM_LOST,
    DFP_DFC_UWB_BASE_FAIL,
    DFP_DFC_11E_TIMOUT,
}DFC_Type;
extern void dfc_type_init(void);
#endif
