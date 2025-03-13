/*
 * Copyright (c) 2021，上海福宇龙汽车科技有限公司
 * All rights reserved.
 * 文件名称: uds_callbacks.c
 * 摘要: This file implements all the APIs (i.e. interfaces) which the UDS Application Layer
* requires from other components of the system.
 * 当前版本: V1.0.0 Chenjianbo
 * Date: 2024-05-30
 * 历史版本: Vx.x.x，编写者/修改者，修改时间，修改内容（重大发布时，总结重点变更内容）
 */



#include "uds_callbacks.h"
#include "uds_confdefs.h"
#include "uds_pub.h"


/*lint -e715*/

/******************************************************************************
 *
 * LinkControl required callback functions.
 *
 *****************************************************************************/
#ifdef UDS_ENABLE_LINKCONTROL

uint8_t UDS_FN_TYPE UdsApp_87LnkCtrl_ChkFixedBaudrateIsAllowed(uint8_t i_FixedBaudRateId2Chk)
{
   i_FixedBaudRateId2Chk = i_FixedBaudRateId2Chk; /* just to remove the compilation warning */
   /* just a stub , need the itf from CAN driver */
   return ((uint8_t)TRUE);
}

uint8_t UDS_FN_TYPE UdsApp_87LnkCtrl_Transit2FixedBaudrate(uint8_t i_FixedBaudRateId2Transit)
{
   i_FixedBaudRateId2Transit = i_FixedBaudRateId2Transit; /* just to remove the compilation warning */
   /* just a stub , need the itf from CAN driver */
   return ((uint8_t)TRUE);
}

uint8_t UDS_FN_TYPE UdsApp_87LnkCtrl_ChkSpecifiedBaudrateIsAllowed(
		uint8_t i_SpecBaudRateHByte,
		uint8_t i_SpecBaudRateMByte,
		uint8_t i_SpecBaudRateLByte
)
{
    /* just a stub , need the itf from CAN driver */
   uint32_t  lul_InSpecifiedBaudrate = 0x00000000u;
   lul_InSpecifiedBaudrate  = (((uint32_t)(i_SpecBaudRateHByte)) << 16u);		/*lint !e838*/
   lul_InSpecifiedBaudrate |= (((uint32_t)(i_SpecBaudRateMByte)) <<  8u);
   lul_InSpecifiedBaudrate |= (uint32_t)(i_SpecBaudRateLByte       );

   if (lul_InSpecifiedBaudrate < 125000ul ||
       lul_InSpecifiedBaudrate > 1000000ul )
   {
       /* just a stub , need the itf from CAN driver */
       return ((uint8_t)FALSE);             /* MISRA 2004 Rule 14.7-Return statement before end of function*//*lint !e904*/
   }
   else
   {
       /* just a stub , need the itf from CAN driver */
       return ((uint8_t)TRUE);            /* MISRA 2004 Rule 14.7-Return statement before end of function*//*lint !e904*/
   }
} /* end of UdsApp_87LnkCtrl_ChkSpecifiedBaudrateIsAllowed() */

uint8_t UDS_FN_TYPE UdsApp_87LnkCtrl_TransitSpecifiedBaudrate(
		uint8_t i_SpecBaudRateHByte,
		uint8_t i_SpecBaudRateMByte,
		uint8_t i_SpecBaudRateLByte
)
{
   /* just to remove the compilation warning */
   i_SpecBaudRateHByte = i_SpecBaudRateHByte;
   i_SpecBaudRateMByte = i_SpecBaudRateMByte;
   i_SpecBaudRateLByte = i_SpecBaudRateLByte;
    /* just a stub , need the itf from CAN driver */
    return ((uint8_t)TRUE);
}

#endif /* end of #ifdef UDS_ENABLE_LINKCONTROL */

/******************************************************************************
 *
 * SecurityAccess required callback functions.
 *
 *****************************************************************************/
#ifdef UDS_ENABLE_SECURITYACCESS

uint8_t UDS_FN_TYPE UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked(uint8_t i_lub_Sal2Checked)
{
   uint8_t lub_IsSalHasUnlocked = (uint8_t)FALSE;


   uint8_t lub_TmpSecurityLevel_1 = UDS_APP_ITF_EXP_GET_CURR_SECU_LEVEL(UDS_C_SECU_ACC_SECU_LEVEL1);
   uint8_t lub_TmpSecurityLevel_2 = UDS_APP_ITF_EXP_GET_CURR_SECU_LEVEL(UDS_C_SECU_ACC_SECU_LEVEL2);
   uint8_t lub_TmpSecurityLevel_3 = UDS_APP_ITF_EXP_GET_CURR_SECU_LEVEL(UDS_C_SECU_ACC_SECU_LEVEL3);
   uint8_t lub_TmpSecurityLevel_4 = UDS_APP_ITF_EXP_GET_CURR_SECU_LEVEL(UDS_C_SECU_ACC_SECU_LEVEL4);


   switch(i_lub_Sal2Checked)
   {
       default :
            break;

       case UDS_C_SECU_ACC_SECU_LEVEL1:
            if ((lub_TmpSecurityLevel_1 != UDS_C_SECU_ACC_SECU_LEVEL1_LOCKED) ||
                (lub_TmpSecurityLevel_2 != UDS_C_SECU_ACC_SECU_LEVEL2_LOCKED) ||
                (lub_TmpSecurityLevel_3 != UDS_C_SECU_ACC_SECU_LEVEL3_LOCKED))
            {
                lub_IsSalHasUnlocked = (uint8_t)TRUE;
            }
            else
            {

            }
            break;

       case UDS_C_SECU_ACC_SECU_LEVEL2:
            if ((lub_TmpSecurityLevel_2 != UDS_C_SECU_ACC_SECU_LEVEL2_LOCKED) ||
                (lub_TmpSecurityLevel_3 != UDS_C_SECU_ACC_SECU_LEVEL3_LOCKED))
            {
                lub_IsSalHasUnlocked = (uint8_t)TRUE;
            }
            else
            {

            }
            break;

       case UDS_C_SECU_ACC_SECU_LEVEL3:
            if ((lub_TmpSecurityLevel_3 != UDS_C_SECU_ACC_SECU_LEVEL3_LOCKED))
            {
                lub_IsSalHasUnlocked = (uint8_t)TRUE;
            }
            else
            {

            }
            break;
       case UDS_C_SECU_ACC_SECU_LEVEL4:
            if ((lub_TmpSecurityLevel_4 != UDS_C_SECU_ACC_SECU_LEVEL4_LOCKED))
            {
                lub_IsSalHasUnlocked = (uint8_t)TRUE;
            }
            else
            {

            }
            break;
   } /* end of switch(i_lub_Sal2Checked) */

   return lub_IsSalHasUnlocked;
} /* end of UdsApp_27SecuAcc_IsSpecifiedSALHasUnlocked()  */

#endif /* end of #ifdef UDS_ENABLE_SECURITYACCESS */

/******************************************************************************
*                               end of file                                   *
******************************************************************************/
