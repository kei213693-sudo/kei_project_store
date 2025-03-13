#ifndef _ASD_DFP_H_
#define _ASD_DFP_H_

#include "DSM_DtcBitCtl.h"

#define _DFP_IMPL_TYPE_DEF_

/* min=0, max=65535, DEZ, limit=no */
extern   uint16_t dfcArray[256];

/* forward declaration of substruct variable 'DFC_BattMax_IRAM' */
/* containing 'IRAM' memory class tree */

extern volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_CANBusoff_IRAM;

/* forward declaration of component variable 'DFC_BattMax' */
/* containing 'IFLASH' memory class tree */

extern const struct DSM_DFCTYPE_IMPL DFC_CANBusoff;

/* forward declaration of substruct variable 'DFC_BattMin' */
/* containing 'IRAM' memory class tree */

extern volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_BattMin_IRAM;

/* forward declaration of component variable 'DFC_BattMin' */
/* containing 'IFLASH' memory class tree */

extern const struct DSM_DFCTYPE_IMPL DFC_BattMin;

/* forward declaration of substruct variable 'DFC_BLESig' */
/* containing 'IRAM' memory class tree */

extern volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_BattMax_IRAM;

/* forward declaration of component variable 'DFC_BLESig' */
/* containing 'IFLASH' memory class tree */

extern const struct DSM_DFCTYPE_IMPL DFC_BattMax;

/* forward declaration of substruct variable 'DFC_CANBusoff' */
/* containing 'IRAM' memory class tree */

extern volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_BLE_INIT_FAIL_IRAM;

/* forward declaration of component variable 'DFC_CANBusoff' */
/* containing 'IFLASH' memory class tree */

extern const struct DSM_DFCTYPE_IMPL DFC_BLE_INIT_FAIL;

/* forward declaration of substruct variable 'DFC_BDCSTimeOut' */
/* containing 'IRAM' memory class tree */




/* forward declaration of substruct variable 'DFC_VCUSTimeOut' */
/* containing 'IRAM' memory class tree */

extern volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_RF_CHIP_COM_ERROR_IRAM;

/* forward declaration of component variable 'DFC_VCUSTimeOut' */
/* containing 'IFLASH' memory class tree */

extern const struct DSM_DFCTYPE_IMPL DFC_RF_CHIP_COM_ERROR;
/* forward declaration of substruct variable 'DFC_BMSTimeOut' */
/* containing 'IRAM' memory class tree */

extern volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_29D6_COM_LOST_IRAM;

/* forward declaration of component variable 'DFC_VCUSTimeOut' */
/* containing 'IFLASH' memory class tree */
extern const struct DSM_DFCTYPE_IMPL DFC_29D6_COM_LOST;

extern volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_UWB_BASE_FAIL_IRAM;

/* forward declaration of component variable 'DFC_VCUSTimeOut' */
/* containing 'IFLASH' memory class tree */
extern const struct DSM_DFCTYPE_IMPL DFC_UWB_BASE_FAIL;
extern volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT DFC_11E_TimOut_IRAM;
extern const struct DSM_DFCTYPE_IMPL DFC_11E_TimOut;

extern const struct DFES_DFCCLRREQUEST_IMPL DFES_DFCClrRequest;

extern volatile struct DSM_GETCHECKSTATUS_IMPL_IRAM_SUBSTRUCT DSM_GetCheckStatus_IRAM;

extern const struct DSM_GETCHECKSTATUS_IMPL DSM_GetCheckStatus;

extern void DFP_IMPL__ini(void);
#endif /* DFP_IMPL__ini */
