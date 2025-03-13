/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *             File:  E2EXf_LCfg.h
 *           Config:  F033.dpa
 *      ECU-Project:  F033
 *
 *        Generator:  MICROSAR E2EXf Generator Version 1.11.0
 *                    RTE Core Version 1.24.0
 *          License:  CBD2100491
 *
 *      Description:  MICROSAR E2EXf header file for link-time variant
 *********************************************************************************************************************/

/* double include prevention */
#ifndef E2EXF_LCFG_H
# define E2EXF_LCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "SchM_E2EXf_Type.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Version identification */
# define E2EXF_CFG_MAJOR_VERSION   (0x01U)
# define E2EXF_CFG_MINOR_VERSION   (0x0bU)

# define E2EXF_VERSION_INFO_API    (STD_OFF)

# define E2EXF_DEV_ERROR_DETECT    (STD_OFF)
# define E2EXF_DEV_ERROR_REPORT    (STD_OFF)

# define E2EXF_P01_ENABLED         (STD_ON)
# define E2EXF_P02_ENABLED         (STD_OFF)
# define E2EXF_P04_ENABLED         (STD_OFF)
# define E2EXF_P05_ENABLED         (STD_ON)
# define E2EXF_P06_ENABLED         (STD_OFF)
# define E2EXF_P07_ENABLED         (STD_OFF)
# define E2EXF_P11_ENABLED         (STD_OFF)

# define E2EXF_OUTOFPLACE_ENABLED  (STD_OFF)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

#ifndef E2EXF_DUMMY_STATEMENT
# define E2EXF_DUMMY_STATEMENT(v) (v)=(v)
#endif
#ifndef E2EXF_DUMMY_STATEMENT_CONST
# define E2EXF_DUMMY_STATEMENT_CONST(v) (void)(v)
#endif


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct {
    boolean inPlace;
    boolean executeDespiteDataUnavailability;
    boolean profileBehavior;
    boolean disableEndToEndCheck;
    uint32 headerLength;
    uint32 upperHeaderBitsToShift;
} E2EXf_OptionsType;

typedef struct
{
    /* postbuild not yet supported */
    uint8 E2EXf_dummy;
} E2EXf_ConfigType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define E2EXF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2EXf_Init_Calls
 *********************************************************************************************************************/
/*! \brief       State structures initialization function
 *  \details     This function initializes all state structures managed by E2EXf.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, E2EXF_CODE) E2EXf_Init_Calls(void);

# define E2EXF_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* E2EXF_LCFG_H */
