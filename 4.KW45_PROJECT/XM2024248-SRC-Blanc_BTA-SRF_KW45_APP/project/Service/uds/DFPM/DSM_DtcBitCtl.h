#ifndef _DSM_DTCBITCTL_H_
#define _DSM_DTCBITCTL_H_

//#include "typedefs.h"
#include "stdint.h"
extern  uint8_t sfgArray[256];
extern  uint16_t dfcArray[256];

/*Bit Contrl*/
#define GETBIT_U8_getBit(position, basis) ((bool)(((uint8)((basis) >> (position))) & ((uint8)(1u))))
#define GETBIT_U16_getBit(position, basis) ((bool)(((uint16)((basis) >> (position))) & ((uint16)(1u))))
#define GETBIT_U32_getBit(position, basis) ((bool)(((uint32)((basis) >> (position))) & ((uint32)(1u))))
#define GETBIT_S8_getBit(position, basis) ((bool)(((sint8)((basis) >> (position))) & ((sint8)(1u))))
#define GETBIT_S16_getBit(position, basis) ((bool)(((sint16)((basis) >> (position))) & ((sint16)(1u))))
#define GETBIT_S32_getBit(position, basis) ((bool)(((sint32)((basis) >> (position))) & ((sint32)(1u))))
#define CLRBITU16_U16_clrBit(bitPosition, bitLeiste) (bitLeiste & (~(1 << bitPosition)))

#define GETBITU8_U8_getBit(bitPosition, bitLeiste) ((bitLeiste >> bitPosition) & 1)
#define GETBITU16_U16_getBit(bitPosition, bitLeiste) ((bitLeiste >> bitPosition) & 1)
#define GETBITU32_U32_getBit(bitPosition, bitLeiste) ((bitLeiste >> bitPosition) & 1)
#define SETBITU16_U16_setBit(bitPosition, bitLeiste) (bitLeiste | (1 << bitPosition))

#define _DSM_GETDSCPERMISSION_IMPL_TYPE_DEF_
#define _DSM_FIDTYPE_IMPL_TYPE_DEF_
#define _DSM_SFCTYPE_IMPL_TYPE_DEF_

struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT
{
	uint16_t DFCType; /* min=0, max=65535, dez, limit=no */
};
/* ----------------------------------------------------------------------------
 * END: DEFINITION OF MEMORY CLASS STRUCTURE FOR CLASS 'DSM_DFCTYPE_IMPL'
 ******************************************************************************/

/******************************************************************************
 * BEGIN: DEFINITION OF MAIN STRUCTURE FOR CLASS 'DSM_DFCTYPE_IMPL'
 * ----------------------------------------------------------------------------
 * memory class:.................................'IFLASH'
 * ---------------------------------------------------------------------------*/

struct DSM_SFCTYPE_IMPL_IRAM_SUBSTRUCT
{
	uint16_t SFCType; /* min=0, max=65535, dez, limit=no */
};
struct DSM_SFCTYPE_IMPL
{
   volatile struct DSM_SFCTYPE_IMPL_IRAM_SUBSTRUCT *DSM_SFCTYPE_IMPL_IRAM;
};

struct DSM_DFCTYPE_IMPL
{
   volatile struct DSM_DFCTYPE_IMPL_IRAM_SUBSTRUCT *DSM_DFCTYPE_IMPL_IRAM;
};

struct DSM_FIDTYPE_IMPL_IRAM_SUBSTRUCT
{
   uint8_t FIdType; /* min=0, max=255, dez, limit=no */
};

struct DSM_FIDTYPE_IMPL
{
   volatile struct DSM_FIDTYPE_IMPL_IRAM_SUBSTRUCT *DSM_FIDTYPE_IMPL_IRAM;
};

struct DSM_GETCHECKSTATUS_IMPL_IRAM_SUBSTRUCT
{
   uint8_t DFCType; /* min=0, max=255, dez, limit=yes */
   struct DSM_SFCTYPE_IMPL_IRAM_SUBSTRUCT locSFCType;
};

struct DSM_GETCHECKSTATUS_IMPL
{
   volatile struct DSM_GETCHECKSTATUS_IMPL_IRAM_SUBSTRUCT *DSM_GETCHECKSTATUS_IMPL_IRAM;
   struct DSM_SFCTYPE_IMPL locSFCType;
};

struct DSM_REPCHECK_IMPL_IRAM_SUBSTRUCT
{
   uint8_t DFCType; /* min=0, max=255, dez, limit=yes */
};

extern  struct DSM_REPCHECK_IMPL_IRAM_SUBSTRUCT DSM_RepCheck_IRAM;
/* Following DEFINE signalizes the completion of definition                   */
/* of data structs for component: ...........................DSM_DFCTYPE_IMPL */
#define _DSM_DFCTYPE_IMPL_TYPE_DEF_

/* forward declaration of types used for method arguments */
#ifndef _DSM_FIDTYPE_IMPL_TYPE_DEF_
struct DSM_FIDTYPE_IMPL;
#endif

/* forward declaration of types used for method arguments */
#ifndef _DSM_SFCTYPE_IMPL_TYPE_DEF_
struct DSM_SFCTYPE_IMPL;
#endif

extern uint16_t DSM_SFCTYPE_IMPL_getSFCType(const struct DSM_SFCTYPE_IMPL *self);

extern uint16_t DSM_SFCTYPE_IMPL_setSFCType(
    const struct DSM_SFCTYPE_IMPL *self,
    /* IN    */ const uint16_t parm);

extern void DSM_REPCHECK_IMPL_DSM_RepCheck(
    /* INOUT */ const struct DSM_DFCTYPE_IMPL *DFC_id,
    /* IN    */ const uint32_t stResult,
    /* IN    */ const uint32_t stTst,
    /* IN    */ const uint32_t stDeb,
    /* IN    */ const uint32_t stAttrib);
extern unsigned char DSM_GETSFCZYF_IMPL_DSM_GetSFCZyf(/* INOUT */ const struct DSM_SFCTYPE_IMPL *SFC_id);

extern unsigned char DSM_GETSFCERD_IMPL_DSM_GetSFCErD(/* INOUT */ const struct DSM_SFCTYPE_IMPL *SFC_id);

extern uint16_t DSM_DFCTYPE_IMPL_DSM_DFCType(const struct DSM_DFCTYPE_IMPL *self);

extern uint16_t DSM_DFCTYPE_IMPL_getDFCType(const struct DSM_DFCTYPE_IMPL *self);

extern uint16_t DSM_DFCTYPE_IMPL_setDFCType(
    const struct DSM_DFCTYPE_IMPL *self,
    /* IN    */ const uint16_t parm);

extern uint8_t DSM_FIDTYPE_IMPL_DSM_FIdType(const struct DSM_FIDTYPE_IMPL *self);

extern uint8_t DSM_FIDTYPE_IMPL_getFIdType(const struct DSM_FIDTYPE_IMPL *self);

extern const struct DSM_SFCTYPE_IMPL *DSM_GETCHECKSTATUS_IMPL_DSM_GetCheckStatus(
    const struct DSM_GETCHECKSTATUS_IMPL *self,
    /* INOUT */ const struct DSM_DFCTYPE_IMPL *DFC_id);
extern uint16_t DSM_SFCTYPE_IMPL_getSFCType(const struct DSM_SFCTYPE_IMPL *self);

extern uint16_t DSM_SFCTYPE_IMPL_setSFCType(
    const struct DSM_SFCTYPE_IMPL *self,
    /* IN    */ const uint16_t parm);
#endif
