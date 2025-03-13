/* This file was automatically generated */

#ifndef UDS_IODIDCTL_H_
#define UDS_IODIDCTL_H_
#include "typedefs.h"



#define UDS_2FIOCTRL_MNGT_RCD_MAX_NUM  5


extern UDS_CONFIG_TYPE UDS_IoCtrlIdxTblRcd_t Uds_g_IoCtrlByDidIdxTable[];
extern UDS_CONFIG_TYPE UDS_IoCtrlMngtDatTblRcd_t Uds_g_IoCtrlByDidMngtTable[UDS_2FIOCTRL_MNGT_RCD_MAX_NUM];
extern void UDS_ITF_IMP_IOCTRL_1001_FF(uint8_t i_IoCtrlOption, uint8_t *iP_IoCtrlParam);
extern void UDS_ITF_IMP_IOCTRL_1002_FF(uint8_t i_IoCtrlOption, uint8_t *iP_IoCtrlParam);
extern void UDS_ITF_IMP_IOCTRL_1005_FF(uint8_t i_IoCtrlOption, uint8_t *iP_IoCtrlParam);
extern void UDS_ITF_IMP_IOCTRL_1010_FF(uint8_t i_IoCtrlOption, uint8_t *iP_IoCtrlParam);
extern void UDS_ITF_IMP_IOCTRL_1020_FF(uint8_t i_IoCtrlOption, uint8_t *iP_IoCtrlParam);

#endif
