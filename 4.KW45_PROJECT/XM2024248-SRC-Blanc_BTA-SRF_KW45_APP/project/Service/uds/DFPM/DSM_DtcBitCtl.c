#include "DSM_DtcBitCtl.h"

#define l_DFCType (self->DSM_DFCTYPE_IMPL_IRAM->DFCType)

/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_DFCTYPE_IMPL_DSM_DFCType'
 * ----------------------------------------------------------------------------
 * model name:...................................'DSM_DFCType'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/

uint16_t DSM_DFCTYPE_IMPL_DSM_DFCType(const struct DSM_DFCTYPE_IMPL *self)
{
   /* DSM_DFCType: line #1  */
   /* return with expr from DSM_DFCType: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
   return 1U;
}

/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_DFCTYPE_IMPL_getDFCType'
 * ----------------------------------------------------------------------------
 * model name:...................................'getDFCType'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
uint16_t DSM_DFCTYPE_IMPL_getDFCType(const struct DSM_DFCTYPE_IMPL *self)
{
   /* return with expr from getDFCType: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
   uint8_t dfp;
   dfp = l_DFCType;
   return dfp;
}

/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_DFCTYPE_IMPL_setDFCType'
 * ----------------------------------------------------------------------------
 * model name:...................................'setDFCType'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
uint16_t DSM_DFCTYPE_IMPL_setDFCType(
    const struct DSM_DFCTYPE_IMPL *self,
    /* IN    */ const uint16_t parm)
{
   /* return with expr from setDFCType: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
   return l_DFCType = parm;
}

#define l_FIdType (self->DSM_FIDTYPE_IMPL_IRAM->FIdType)

/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_FIDTYPE_IMPL_DSM_FIdType'
 * ----------------------------------------------------------------------------
 * model name:...................................'DSM_FIdType'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
uint8_t DSM_FIDTYPE_IMPL_DSM_FIdType(const struct DSM_FIDTYPE_IMPL *self)
{
   /* DSM_FIdType: line #1  */
   /* return with expr from DSM_FIdType: min=0, max=255, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
   return 1U;
}

/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_FIDTYPE_IMPL_getFIdType'
 * ----------------------------------------------------------------------------
 * model name:...................................'getFIdType'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
uint8_t DSM_FIDTYPE_IMPL_getFIdType(const struct DSM_FIDTYPE_IMPL *self)
{
   /* return with expr from getFIdType: min=0, max=255, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
   return l_FIdType;
}

#define l_dfcArray dfcArray
//#define l_DFCType (self->DSM_GETCHECKSTATUS_IMPL_IRAM->DFCType)
#define l_locSFCType_REF_ (&(self->locSFCType))
#if 0
/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_GETCHECKSTATUS_IMPL_DSM_GetCheckStatus'
 * ----------------------------------------------------------------------------
 * model name:...................................'DSM_GetCheckStatus'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
const struct DSM_SFCTYPE_IMPL *DSM_GETCHECKSTATUS_IMPL_DSM_GetCheckStatus(
    const struct DSM_GETCHECKSTATUS_IMPL *self,
    /* INOUT */ const struct DSM_DFCTYPE_IMPL *DFC_id)
{
   /* temp. variables */
   uint16_t _t1uint16;

   /* DSM_GetCheckStatus: line #22  */
   _t1uint16 = DSM_DFCTYPE_IMPL_getDFCType(DFC_id);
   /* assignment to DFCType: min=0, max=255, hex=phys, limit=(maxBitLength: false, assign: true), zero incl.=true  */
   l_DFCType = (uint8_t)(((_t1uint16 <= 255U) ? _t1uint16 : 255U));
   /* DSM_GetCheckStatus: line #23  */
   DSM_SFCTYPE_IMPL_setSFCType(l_locSFCType_REF_, l_dfcArray[l_DFCType]);
   /* DSM_GetCheckStatus: line #24  */
   return l_locSFCType_REF_;
}
#endif
/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_GETSFCERD_IMPL_DSM_GetSFCErD'
 * ----------------------------------------------------------------------------
 * model name:...................................'DSM_GetSFCErD'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
unsigned char DSM_GETSFCERD_IMPL_DSM_GetSFCErD(/* INOUT */ const struct DSM_SFCTYPE_IMPL *SFC_id)
{
   /* DSM_GetSFCErD: line #49  */
   return GETBITU16_U16_getBit(10U, DSM_SFCTYPE_IMPL_getSFCType(SFC_id));
}

/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_GETSFCZYF_IMPL_DSM_GetSFCZyf'
 * ----------------------------------------------------------------------------
 * model name:...................................'DSM_GetSFCZyf'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
unsigned char DSM_GETSFCZYF_IMPL_DSM_GetSFCZyf(/* INOUT */ const struct DSM_SFCTYPE_IMPL *SFC_id)
{
   /* DSM_GetSFCZyf: line #49  */
   return GETBITU16_U16_getBit(9U, DSM_SFCTYPE_IMPL_getSFCType(SFC_id));
}

struct DSM_REPCHECK_IMPL_IRAM_SUBSTRUCT DSM_RepCheck_IRAM;
#define l_DFCType1 DSM_RepCheck_IRAM.DFCType

/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_REPCHECK_IMPL_DSM_RepCheck'
 * ----------------------------------------------------------------------------
 * model name:...................................'DSM_RepCheck'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
void DSM_REPCHECK_IMPL_DSM_RepCheck(
    /* INOUT */ const struct DSM_DFCTYPE_IMPL *DFC_id,
    /* IN    */ const uint32_t stResult,
    /* IN    */ const uint32_t stTst,
    /* IN    */ const uint32_t stDeb,
    /* IN    */ const uint32_t stAttrib)
{
   /* user defined local variables */
   // uint32 _dummy_stAttrib;
   uint16_t bitpos;
   /* temp. variables */
   uint16_t _t1uint16;
   uint16_t temp_u16 ;
   /* DSM_RepCheck: line #37  */
   /* assignment to _dummy_stAttrib: min=0, max=4294967295, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
   //_dummy_stAttrib = stAttrib;
   /* DSM_RepCheck: line #38  */
   _t1uint16 = DSM_DFCTYPE_IMPL_getDFCType(DFC_id);
   /* assignment to DFCType: min=0, max=255, hex=phys, limit=(maxBitLength: false, assign: true), zero incl.=true  */
//   l_DFCType1 = (uint8_t)(((_t1uint16 <= 255U) ? _t1uint16 : 255U));
l_DFCType1 =1;
   temp_u16 = l_dfcArray[l_DFCType1];
 //temp_u16 = 1;
   /* DSM_RepCheck: line #42  */
   for (bitpos = 0U; bitpos <= 3U; bitpos = bitpos + 1U)
   {
      /* DSM_RepCheck: line #44  */
      if (GETBITU32_U32_getBit((uint8_t)(((bitpos <= 255U /* always true */) ? bitpos : 255U)), stResult))
      {
         /* DSM_RepCheck: line #46  */
	bitpos = (uint8_t)(((bitpos <= 255U /* always true */) ? bitpos : 255U));

         /* assignment to dfcArray[]: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
         temp_u16 = SETBITU16_U16_setBit(bitpos, temp_u16);
      }
      else
      {
         /* DSM_RepCheck: line #50  */
	//temp_u16 = l_dfcArray[l_DFCType1];
         /* assignment to dfcArray[]: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
        temp_u16 = CLRBITU16_U16_clrBit((uint8_t)(((bitpos <= 255U /* always true */) ? bitpos : 255U)), temp_u16);
      } /* end if */
   }    /* end for */
   /* DSM_RepCheck: line #57  */
   for (bitpos = 0U; bitpos <= 3U; bitpos = bitpos + 1U)
   {
      /* DSM_RepCheck: line #59  */
      if (GETBITU32_U32_getBit((uint8_t)(((bitpos <= 255U /* always true */) ? bitpos : 255U)), stDeb))
      {
         /* DSM_RepCheck: line #61  */
         _t1uint16 = bitpos + 4U;
	//temp_u16 = l_dfcArray[l_DFCType1];
         /* assignment to dfcArray[]: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
         temp_u16 = SETBITU16_U16_setBit((uint8_t)(((_t1uint16 <= 255U /* always true */) ? _t1uint16 : 255U)), temp_u16);
      }
      else
      {
         /* DSM_RepCheck: line #65  */
         _t1uint16 = bitpos + 4U;
	//temp_u16 = l_dfcArray[l_DFCType1];
         /* assignment to dfcArray[]: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
         temp_u16 = CLRBITU16_U16_clrBit((uint8_t)(((_t1uint16 <= 255U /* always true */) ? _t1uint16 : 255U)), temp_u16);
      } /* end if */
   }    /* end for */
   /* DSM_RepCheck: line #72  */
   if (stTst != 0U)
   {
      /* DSM_RepCheck: line #74  */
	//temp_u16 = l_dfcArray[l_DFCType1];
      /* assignment to dfcArray[]: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
      temp_u16 = SETBITU16_U16_setBit(9U, temp_u16);
   }
	else
	{}/* end if */
	l_dfcArray[l_DFCType1] = temp_u16;
}

#define l_SFCType (self->DSM_SFCTYPE_IMPL_IRAM->SFCType)

/******************************************************************************
 * BEGIN: DEFINITION OF METHOD 'DSM_SFCTYPE_IMPL_getSFCType'
 * ----------------------------------------------------------------------------
 * model name:...................................'getSFCType'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
uint16_t DSM_SFCTYPE_IMPL_getSFCType(const struct DSM_SFCTYPE_IMPL *self)
{
   /* return with expr from getSFCType: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
   return l_SFCType;
}

/******************************************************************************
  BEGIN: DEFINITION OF METHOD 'DSM_SFCTYPE_IMPL_setSFCType'
 * ----------------------------------------------------------------------------
 * model name:...................................'setSFCType'
 * memory class:.................................'ICODE'
 * ---------------------------------------------------------------------------*/
uint16_t DSM_SFCTYPE_IMPL_setSFCType(
    const struct DSM_SFCTYPE_IMPL *self,
    /* IN    */ const uint16_t parm)
{
   /* return with expr from setSFCType: min=0, max=65535, hex=phys, limit=(maxBitLength: false, assign: false), zero incl.=true  */
   return l_SFCType = parm;
}
